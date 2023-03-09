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

void BinCode(Node* root, string bincode, unordered_map<char, string>& huffman_code) {
    if (root == NULL) return;

    if (!root->left && !root->right) {
        huffman_code[root->letter] = bincode;
    }

    BinCode(root->left, bincode + "0", huffman_code);
    BinCode(root->right, bincode + "1", huffman_code);
}

void HuffmanEncode(const char* input_text = "text.txt", const char* output_text = "chipher_text.txt") {
    FILE* input;
    fopen_s(&input, input_text, "r");
    if (!input) 
    {
        puts("ERROR: Input text not open\n");
        exit(1);
    }

    unsigned char lett;
    map<char, int> freq;
    while (!feof(input)) {
        lett = fgetc(input);
        if (!feof(input)) {
            freq[lett]++;
        }
    }

    char count_letters = freq.size();

    fclose(input);

    FILE* output;
    fopen_s(&output, output_text, "wb +");
    fopen_s(&input, input_text, "rb");
    if (!input || !output)
    {
        puts("ERROR: No such file or directory\n"); 
        exit(1);
    }

    fputc(count_letters, output);
    for (auto pair : freq) {
        fputc(pair.first, output);
        fputc(pair.second, output);
    }

    priority_queue<Node*, vector<Node*>, check> queue;

    for (auto pair : freq) {
        queue.push(AddNode(pair.first, pair.second, nullptr, nullptr));
    }

    Node* temp;

    while (queue.size() != 1)
    {

        Node* left = queue.top(); queue.pop();
        Node* right = queue.top(); queue.pop();

        if (left->frequency == right->frequency && left->letter < right->letter) {
            temp = left;
            left = right;
            right = temp;
        }

        int sum = left->frequency + right->frequency;
        queue.push(AddNode('\0', sum, left, right));
    }

    Node* root = queue.top();

    unordered_map<char, string> huffman_code;
    BinCode(root, "", huffman_code);

    cout << "Huffman Codes are :\n" << '\n';
    for (auto pair : huffman_code) {
        cout << pair.first << " " << pair.second << '\n';
    }

    char fkey, symbol = 0;
    int bit_len = 0;

    while (!feof(input))
    {
        fkey = fgetc(input);
        if (!feof(input))
        {
            if (bit_len + huffman_code[fkey].length() <= 8)
            {
                for (int i = 0; i < huffman_code[fkey].length(); i++)
                {
                    symbol = (symbol << 1) | (huffman_code[fkey][i] - '0');
                }
                bit_len += huffman_code[fkey].length();
            }
            else
            {
                if (huffman_code[fkey].length() - bit_len + 8 <= 8)
                {
                    for (int j = 0; j < 8 - bit_len; j++)
                    {
                        symbol = (symbol << 1) | (huffman_code[fkey][j] - '0');
                    }
                    fputc(symbol, output);
                    symbol = 0;
                    for (int j = 8 - bit_len; j < huffman_code[fkey].length(); j++)
                    {
                        symbol = (symbol << 1) | (huffman_code[fkey][j] - '0');
                    }
                    bit_len = huffman_code[fkey].length() - 8 + bit_len;
                }
                else
                {
                    for (int j = 0; j < 8 - bit_len; j++)
                    {
                        symbol = (symbol << 1) | (huffman_code[fkey][j] - '0');
                    }
                    
                    fputc(symbol, output);
                    int i = 8 - bit_len;
                    bit_len = 0;
                    symbol = 0;
                    
                    for (i; i < huffman_code[fkey].length(); i++)
                    {
                        symbol = (symbol << 1) | (huffman_code[fkey][i] - '0');
                        bit_len++;
                        if (bit_len == 8)
                        {
                            fputc(symbol, output);
                            bit_len = 0;
                            symbol = 0;
                        }
                    }
                }
            }
        }
        else
        {
            symbol = symbol << (8 - bit_len);
            fputc(symbol, output);
        }
    }

    fclose(input);
    fclose(output);
}

void Compress(const char* input_text = "text.txt", const char* output_text = "chipher_text.txt")
{
    long long file_size = 0;
    long long compress_size = 0;

    struct stat s1 {};
    struct stat s2 {};
    
    if (!stat(input_text, &s1)) {
        file_size = s1.st_size;
    }
    else {
        perror("STAT ERROR ");
    }
    if (!stat(output_text, &s2)) {
        compress_size = s2.st_size;
    }
    else {
        perror("STAT ERROR ");
    }

    cout << "\nCompress value is:" << (compress_size + 0.0) / file_size << "\n";
}

int main() 
{
    HuffmanEncode(); 
    Compress();
}