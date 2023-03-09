#define _CRT_SECURE_NO_WARNINGS
#include <iostream>
#include <fstream>
#include <string>
#include <queue>
#include <unordered_map>
#include <map>

using namespace std;

struct Node 
{
    char letter;
    unsigned int frequency;
    Node* left;
    Node* right;
};

Node* AddNode(char key, unsigned int freq, Node* left, Node* right) {
    Node* node = new Node();
    node->letter = key;
    node->frequency = freq;
    node->left = left;
    node->right = right;
    return node;
};

struct check 
{
    bool operator()(Node* l, Node* r) 
    {
        return l->frequency > r->frequency;
    }
};

void HuffmanDecode(const char* input_text = "chipher.txt", const char* output_text = "plain_text.txt") {
    FILE* input = fopen(input_text, "rb");
    if (!input)
    {
        puts("ERROR: not found input text\n");
        exit(1);
    }
    unsigned char count = 0;
    unsigned char key;
    int cart = 0;
    if (input != NULL) {
        count = fgetc(input);
    }

    map<char, int> frequency;
    for (int i = 0; i < count; i++) {
        key = fgetc(input);
        frequency[key] = fgetc(input);
        cart += frequency[key];
    }

    cout << "Count of unique symbols: " << (int)count << '\n';
    cout << "Huffman Codes are :\n";
    for (auto pair : frequency) {
        cout << pair.first << " " << pair.second << '\n';
    }

    priority_queue<Node*, vector<Node*>, check> pq;

    for (auto pair : frequency) {
        pq.push(AddNode(pair.first, pair.second, nullptr, nullptr));
    }

    Node* temp;

    while (pq.size() != 1)
    {

        Node* left = pq.top(); pq.pop();
        Node* right = pq.top(); pq.pop();

        if (left->frequency == right->frequency && left->letter < right->letter) {
            temp = left;
            left = right;
            right = temp;
        }

        int sum = left->frequency + right->frequency;
        pq.push(AddNode('\0', sum, left, right));
    }

    Node* root = pq.top();

    FILE* output;
    
    fopen_s(&output, output_text, "wb +");
    if (!input)
    {
        puts("ERROR: Output text not open\n");
        exit(1);
    }

    char symbol; char letter;
    while (!feof(input))
    {
        symbol = fgetc(input);
        if (!feof(input))
        {
            for (int i = 7; i >= 0 && cart != 0; i--)
            {
                if ((symbol >> i) & 1)
                {
                    if (root->right)
                    {
                        root = root->right;
                    }
                    else
                    {
                        letter = root->letter;
                        fputc(letter, output);
                        cart--;
                        root = pq.top()->right;
                    }
                }
                else
                {
                    if (root->left)
                    {
                        root = root->left;
                    }
                    else
                    {
                        letter = root->letter;
                        fputc(letter, output);
                        cart--;
                        root = pq.top()->left;
                    }
                }
            }
        }
    }
    fclose(input);
    fclose(output);
}

void AreEqual(const char* text_name1 = "text.txt", const char* text_name2 = "plain_text.txt")
{
    FILE* text1; FILE* text2;
    fopen_s(&text1, text_name1, "rb");
    fopen_s(&text2, text_name2, "rb");

    if (!text1 || !text2)
    {
        puts("ERROR: No such file or directory\n");
        exit(1);
    }

    char a = fgetc(text1), b = fgetc(text2);
    bool flag = true;

    while (!feof(text1) && flag)
    {
        flag = false;
        if (b == a)
        {
            a = fgetc(text1);
            b = fgetc(text2);
            flag = true;
        }
    }

    if (flag && feof(text2)) cout << "Texts are equal\n";
    else cout << "Texts are not equal\n";

    fclose(text1);
    fclose(text2);
}

int main() 
{
    HuffmanDecode();
    AreEqual();
}