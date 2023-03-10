#include <stdio.h>
#include <iostream>
#include <string>
#include <list>
#include <fstream>
#include <sstream>
#include <vector>
#include <map>
#include <math.h>
using namespace std; 

struct Freq 
{
	char s;
	int n;
	double high = 0;
	double low = 0;
};

string FileToString(ifstream& file) 
{
	string text;
	if (file.is_open())
	{
		stringstream ss;
		ss << file.rdbuf();
		text = ss.str();
		file.close();
	}
	else
		cout << "Your file are failed!" << endl;

	return text;
}
Freq* TableFrequency(string text) 
{
	char symbols[255];  
	for (int i = 0; i < 255; i++)
		symbols[i] = i;

	Freq* temp = new Freq[255]; 
	int k = 0;

	for (int i = 0; i < 255; i++) 
	{
		for (int j = 0; j < text.size(); j++)
			if (symbols[i] == text[j])
				k++;
		temp[i].s = symbols[i];
		temp[i].n = k;
		k = 0;
	}


	for (int i = 0; i < 255; i++) 
		if (temp[i].n != 0)
			k++;

	int size = k;
	Freq* table = new Freq[256]; 
	k = 0;

	for (int i = 0; i < 255; i++) 
		if (temp[i].n != 0)
		{
			table[k].n = temp[i].n;
			table[k].s = temp[i].s;
			k++;
		}
	table[k].n = 0;


	int tmp;
	char tmps;
	int j;
	for (int i = 1; i < size; i++) 
	{
		tmp = table[i].n;
		tmps = table[i].s;
		for (j = i - 1; j >= 0 && table[j].n < tmp; j--)
		{
			table[j + 1].n = table[j].n;
			table[j + 1].s = table[j].s;
		}
		table[j + 1].n = tmp;
		table[j + 1].s = tmps;
	}

	double templow = 0;
	double temphigh = 0;

	for (int i = 0; i < size; i++)
	{
		templow = temphigh;
		temphigh += ((double)table[i].n / text.size());

		table[i].low = templow;
		table[i].high = temphigh;
	}
	delete[]temp;
	return table;
}

double CostInterval(Freq* table, string text)
{
	int i = 0;
	int k = 0;
	double low = 0;
	double high = 0;

	while (table[i].s != text[k])
		i++;

	
	double lowtemp = table[i].low;
	double hightemp = table[i].high;

	k++;

	for (; k < text.size(); k++)
	{
		i = 0;
		while (table[i].s != text[k])
			i++;

		low = lowtemp + (hightemp - lowtemp) * table[i].low;
		high = lowtemp + (hightemp - lowtemp) * table[i].high;

		lowtemp = low;
		hightemp = high;
	}
	return low;
}
void Info(Freq* table) 
{
	cout << "CHAR FREQ INTERVAL" << endl << endl;
	int i = 0;
	while (table[i].n != 0)
	{
		cout << table[i].s << " = " << table[i].n << " = " << "[" << table[i].low << " ; " << table[i].high << ")";
		cout << endl;
		i++;
	}
}
string TableFrequencyText(Freq* table)
{
	int i = 0;
	string text;
	int k = 0;
	int n = 0;
	while (table[i].n != 0)
	{
		text += table[i].s;
		n += table[i].n;
		text += '[';
		text += to_string(table[i].low);
		text += ';';
		text += to_string(table[i].high);
		text += ')';
		i++;
		k++;
	}
	text += "count=";
	text += to_string(n);
	text += "size=";
	text += to_string(k);
	return text;
}
void Output(ofstream& file, string text)
{
	file << text;
}
void Encode(ifstream& input, ofstream& out, ofstream& outtable)
{
	string text = FileToString(input);
	Freq* table = TableFrequency(text);
	Info(table);
	double k = CostInterval(table, text);
	string tabletxt = TableFrequencyText(table);
	string outputEncode = to_string(k);
	Output(outtable, tabletxt);
	Output(out, outputEncode);
}


int LenghtText(string text)
{
	int i = 0;
	int k;
	for (i = text.find("size=", i++); i != string::npos; i = text.find("size=", i + 1))
		k = i;
	k += 5;

	string tmp;
	for (; k < size(text); k++)
		tmp += text[k];
	int result = stoi(tmp);
	return result;
}
int SizeText(string text)
{
	int i = 0;
	int k;
	int j;

	for (i = text.find("size=", i++); i != string::npos; i = text.find("size=", i + 1))
		k = i;

	i = 0;

	for (i = text.find("count=", i++); i != string::npos; i = text.find("count=", i + 1))
		j = i;
	j += 6;

	string tmp;
	for (; j < k; j++)
		tmp += text[j];
	int result = stoi(tmp);
	return result;
}

Freq* TableTextToFreq(string text, int lenght)
{
	Freq* table = new Freq[lenght + 1];
	int i = 0;
	string tmp;

	for (int j = 0; j < lenght; j++)
	{
		table[j].s = text[i];
		table[j].n = -1;
		i += 2;

		while (text[i] != ';')
		{
			tmp += text[i];
			i++;
		}
		table[j].low = stod(tmp);
		tmp.clear();
		i++;
		while (text[i] != ')')
		{
			tmp += text[i];
			i++;
		}
		table[j].high = stod(tmp);
		tmp.clear();
		i++;
	}
	table[lenght].n = 0;
	return table;
}

string Finish(Freq* table, double ll, int n)
{
	double code = ll;
	int j = 0;
	string result;

	for (int i = 0; i < n; i++)
	{
		while ((code < table[j].low) || (code >= table[j].high))
			j++;

		result += table[j].s;
		code = (code - table[j].low) / (table[j].high - table[j].low);
		j = 0;
	}
	return result;
}
void Decode(ifstream& input, ifstream& inputtable, ofstream& out)
{
	string tabletx = FileToString(inputtable);
	string Encode = FileToString(input);
	Freq* table = TableTextToFreq(tabletx, LenghtText(tabletx));
	string result = Finish(table, stod(Encode), SizeText(tabletx));
	Output(out, result);
}

int main(int argc, char **argv) {
	string action = argv[1];
	if (action == "Encode") {
		printf("Enter filename for Encode: ");
		string fname;
		gets(fname);
		ifstream inFileE(fname);
		ofstream outFileE("EncodedFile.txt");
		ofstream outTableE("EncodedTable.txt");
		Encode(inFileE, outFileE, outTableE);
		outFileE.close();
		inFileE.close();
		outTableE.close();
	}
	else if (action == "Decode") {
		printf("Enter filename for Decode: ");
		string fname;
		gets(fname);
		ifstream inFileD("fname");
		ifstream outTableD("tableDecode.txt");
		ofstream outFileD("OutputDecode.txt");
		Decode(inFileD, outTableD, outFileD);
		inFileD.close();
		outTableD.close();
		outFileD.close();
	}
	return 0;
}