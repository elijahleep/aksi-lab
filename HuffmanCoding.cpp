#include <stdio.h>
#include <iostream>
#include <string>
#include <list>
#include <fstream>
#include <sstream>
#include <vector>
#include <map>
using namespace std; 

struct Freq
{
	char s;
	int n;
	vector <bool> code;
};

class Node
{
public:
	int n;
	
	char s;
	Node* left;
	Node* right;

	Node()
	{
		left = nullptr;
		right = nullptr;
	}

	Node(Node* ll, Node* rr)
	{
		left = ll;
		right = rr;
		n = ll->n + rr->n;
	}

};

string FileToString(ifstream& file)
{
	string text;
		stringstream ss;
		ss << file.rdbuf();
		text = ss.str();
		file.close();
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
	
	delete[]temp;
	return table;
}

list <Node*> TableToList(Freq* table) 
{
	int i = 0;
	list <Node*> List;

	while (table[i].n != 0)
	{
		Node* Symbol = new Node;
		Symbol->n = table[i].n;
		Symbol->s = table[i].s;
		List.push_back(Symbol);
		i++;
	}

	return List;
}

struct Lcompare 
{
	bool operator()(const Node* l, const Node* r)
		const
	{
		return l->n < r->n;
	}
};
Node* CreateTree(list <Node*> list) 
{
	while (list.size() != 1)
	{
		list.sort(Lcompare());
		Node* L = list.front();
		list.pop_front();
		Node* R = list.front();
		list.pop_front();

		Node* LR = new Node(L, R);
		list.push_back(LR);
	}

	return list.front();
}

vector <bool> tmp;
void TableCode(Node* root, Freq* table)
{
	if (root->left != nullptr)
	{
		tmp.push_back(0);
		TableCode(root->left, table);
	}

	if (root->right != nullptr)
	{
		tmp.push_back(1);
		TableCode(root->right, table);
	}

	int i = 0;
	while (table[i].n != 0)
	{
		if ((root->s) == table[i].s)
		{
			table[i].code = tmp;
			break;
		}
		i++;
		
	}
}

void Info(Freq* table) 
{
	cout << "CHAR   FREQUENCY" << endl;
	int i = 0;
	while (table[i].n != 0)
	{
		cout << table[i].s << "   =   " << table[i].n << endl;
		i++;
	}
}

 string LetterCode(string text, Freq* table) 
{
	 string out;
	 int i; 

	 for (int j = 0; j < size(text); j++)
	 {
		 i = 0;
		 while (table[i].n != 0)
		 {
			 if (text[j] == table[i].s)
			 {
				 for (auto k = 0; k < table[i].code.size(); k++)
				 {
					 if (table[i].code[k] == true)
						 out += '1';
					 else
						 out += '0';
				}
				 break;
			 }
			 i++;
		 }
	 }

	 return out;
} 
 string Finish(string text, string input) 
 {
	 string out;
	 unsigned char v = 0;
	 unsigned char mask = 128;

	 for (int i = 0; i < size(text); i++)
	 {
		 if (text[i] == '1')
			 v |= mask;
		 
		 mask >>= 1;

		 if (mask == 0)
		 {
			 out += v;
			 v = 0;
			 mask = 128;
		 }
	 }

	 out += "size=";
	 
	 int n = size(input);
	 int k = 1;

	 while ((n / 10) != 0)
	 {
		 n /= 10;
		 k++;
	 }

	 char *SIZE = new char[k];
	 n = size(input);

	 for (int i = (k - 1); i >= 0; i--)
	 {
		 SIZE[i] = (n % 10) + 48;
		 n = n / 10;
	 }

	 for (int i = 0; i < k; i++)
		 out += SIZE[i];

	 return out;
 }

 void Output(ofstream& file, string text) 
 {
	 file << text;
 }
 
void OutputTable(ofstream& file, Freq* table) 
 {
	 int i = 0;
	 while (table[i].n != 0)
	 {
		 file << table[i].s;
		 for (int j = 0; j < table[i].code.size(); j++)
		 {
			 if (table[i].code[j] == true)
				 file << '1';
			 else
				 file << '0';
		 }
		 i++;
	 }



 }


 void Encode(ifstream& file, ofstream& outfile, ofstream& outfilecode) 
 {
	 string text = FileToString(file);
	 Freq* table = TableFrequency(text);
	 list <Node*> List = TableToList(table);
	 Node* root = CreateTree(List);
	 TableCode(root, table);
	 Info(table);
	 string textcode = LetterCode(text, table);
	 string finally = Finish(textcode, text);
	 Output(outfile, finally);
	 OutputTable(outfilecode, table);
 }
 
 
 Freq* InputTable(ifstream& file)
 {
	 string table = FileToString(file);
	 cout << "FREQ: " << table << endl;

	 int n_of_s = 0;

	 for (int i = 0; i < size(table); i++) 
		 if (table[i] != '1' && table[i] != '0')
			 n_of_s++;

	 Freq* TableCode = new Freq[n_of_s + 1];
	 int i = 0;

	 for (int j = 0; j < n_of_s; j++)
	 {
		 TableCode[j].s = table[i];
		 i++;
		 while (table[i] == '1' || table[i] == '0')
		 {
			 if (table[i] == '1') TableCode[j].code.push_back(1);
			 if (table[i] == '0') TableCode[j].code.push_back(0);
			 i++;
		 }
	 }
	 TableCode[n_of_s].code.clear();
	 return TableCode;
 }
 string EncryptedText(ifstream& file) 
 {
	 string text = FileToString(file);

	 int i = 0;
	 int k;
	 for (i = text.find("size=", i++); i != string::npos; i = text.find("size=", i + 1))
		 k = i;

	 string out;
	 unsigned char mask;

	 for (int j = 0; j < k; j++)
	 {
		 mask = 128;
		 while (mask != 0)
		 {
			 if ((text[j] & mask) == mask) out += '1';
			 else out += '0';
			 mask >>= 1;
		 }
	 }
	 return out;
 }
 Node* Tree(Freq* table)
 {
	 Node* root = new Node;
	 Node* tmp;
	 int i = 0;

	 while (table[i].code.size() != 0)
	 {
		 tmp = root;
		 for (int k = 0; k < table[i].code.size(); k++)
		 {
			 if (table[i].code[k] == true)
			 {
				 if (tmp->right == nullptr)
				 {
					 tmp->right = new Node; tmp = tmp->right;
				 }
				 else
					 tmp = tmp->right;
			 }
			 else
			 {
				 if (tmp->left == nullptr)
				 {
					 tmp->left = new Node; tmp = tmp->left;
				 }
				 else
					 tmp = tmp->left;
			 }	 
		}
		 tmp->s = table[i].s;
		 i++;
	 }
	 return root;
 }
 string Out(Node* root, string byte)
 {
	string out;
	Node* tmp;
	int i = 0;

	while (i < size(byte))
	{
		tmp = root;
		while (tmp->left != nullptr && tmp->right != nullptr)
		{
			if (byte[i] == '1')
				tmp = tmp->right;
			else tmp = tmp->left;
			i++;
		}
		out += tmp->s;
	}
	 return out;
 }

 void Decode(ifstream& file, ifstream& filecode, ofstream& outfile)
 {
	 string byte = EncryptedText(file);
	 Freq* table = InputTable(filecode);
	 Node* root = Tree(table);
	 string out = Out(root, byte);
	 Output(outfile, out);
 }


int main(int argc, char **argv) {
	string action = argv[1];
	if (action == "encode") {
		printf("Enter filename for encode: ");
		string fname;
		gets(fname);
		ifstream inFileE(fname);
		ofstream outFileE("encodedFile.txt");
		ofstream outTableE("encodedTable.txt");
		Encode(inFileE, outFileE, outTableE);
		outFileE.close();
		inFileE.close();
		outTableE.close();
	}
	else if (action == "decode") {
		printf("Enter filename for decode: ");
		string fname;
		gets(fname);
		ifstream inFileD("fname");
		ifstream outTableD("tabledecode.txt");
		ofstream outFileD("OutputDECODE.txt");
		Decode(inFileD, outTableD, outFileD);
		inFileD.close();
		outTableD.close();
		outFileD.close();
	}
	return 0;
}