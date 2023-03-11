#include <iostream> 
#include <fstream>
#include <vector>
#include <map>
#include <list>
using namespace std;

class Node
{
public:
	int key;
	char s;
	Node *left, *right;
	Node() { left = NULL; right = NULL; }
	Node(Node *l, Node *r)
	{
		left = l;
		right = r;
		key = l->key + r->key;
	}

};

struct Sort
{
	bool operator() (const Node* l, const Node* r)
	{
		return l->key < r->key;
	}
};

vector<bool> code;
map<char, vector<bool> > letterCode;

void Tree(Node *root)
{
	if (root->left != NULL)
	{
		code.push_back(0);
		Tree(root->left);
	}
	if (root->right != NULL)
	{
		code.push_back(1);
		Tree(root->right);
	}
	if (root->right == NULL && root->left == NULL)
	{
		letterCode[root->s] = code;
	}
	if (!code.empty())
		code.pop_back();
}



int main(int argc, char **argv) {
	string action = argv[1];
	if (action == "encode")
	{
		printf("Enter filename for encode: ");
		char fname[30];
		scanf("%s", fname);
		ifstream f(fname, ios::out | ios::binary);
		ofstream g("code.txt", ios::out | ios::binary);
		map<char, int> m;
		map<char, int> ::iterator ii;
		while (!f.eof()) 
		{
			char c = f.get();
			m[c]++;
		}
			list<Node*> L;
		for (ii = m.begin(); ii != m.end(); ii++) 
		{
			Node* p = new Node;
			p->s = ii->first;
			p->key = ii->second; 
			L.push_back(p);
		}
		while (L.size() != 1)
		{
			L.sort(Sort());
			Node *Left = L.front(); 
			L.pop_front();
			Node *Right = L.front();
			L.pop_front();
			Node *pr = new Node(Left, Right);
			L.push_back(pr);
		}
		Node *root = L.front();
		Tree(root);
		int count=0;
		for (ii = m.begin(); ii != m.end(); ii++) 
		{		
			if (ii->second != 0) count += 40;	  		
		}
		g.write((char*)(&count), sizeof(count));
		for (int i=0; i<256; i++) 
		{
			if (m[char(i)]>0) 
			{
				char c=char(i);
				g.write((char*)(&c), sizeof(c));
				g.write((char*)(&m[char(i)]), sizeof(m[char(i)]));
			}
		}
		f.clear();
		f.seekg(0);
		count=0;
		char tx = 0;
		while (!f.eof())
		{
			char c = f.get();
			vector<bool> x = letterCode[c];
			for (int j = 0; j<x.size(); j++)
			{
				tx = tx | x[j] << (7 - count);
				count++;
				if (count == 8)
				{ 
					count = 0; 
					g << tx;
					tx = 0;
				}
			}
		}
		f.close();
		g.close();
	}	
	else if (action == "decode")
	{
		printf("Enter filename for decode: ");
		char fname[30];
		scanf("%s", fname);
		ifstream f(fname, ios::out | ios::binary);
		ofstream g("output.txt", ios::out | ios::binary);
		int x1, x2;
		char s;
		map<char, int> m;															
		f.read((char*)&x1, sizeof(x1));																												
		while (x1>0) 
		{
			f.read((char*)&s, sizeof(s));											
			f.read((char*)&x2, sizeof(x2));											
			x1-=40;																
			m[s]=x2;																
		}
		list<Node*> L;
		map<char, int>::iterator ii;
		for (ii = m.begin(); ii != m.end(); ii++) 
		{
			Node* p = new Node;
			p->s = ii->first;
			p->key = ii->second;
			L.push_back(p);
		}
		while (L.size() != 1) 
		{
			L.sort(Sort());
			Node *Left = L.front();
			L.pop_front(); 
			Node *Right = L.front(); 
			L.pop_front();
			Node *pr = new Node(Left, Right);
			L.push_back(pr);
		}
		Node *root = L.front();
		Tree(root);
		char byte;
		int count = 0;
		Node *p=root;
		byte = f.get();
		while (!f.eof())
		{
			bool b = byte & (1 << (7 - count));
			if (b)
				p = p->right;
			else
				p = p->left;
			if (p->left == NULL && p->right == NULL)
			{
				g<<p->s;
				p=root; 
			}
			count++;
			if (count == 8)
			{ 
				count = 0;
				byte = f.get();
			}
		}
   		f.close();
   		g.close();
   	}
	return 0;
}