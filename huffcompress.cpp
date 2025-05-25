#include <iostream>
#include <cstdlib>
#include <string.h>
#include <math.h>
#include <unordered_map>
#include <fstream>
#include<bits/stdc++.h>


using namespace std;
/*hoffman compression*/
#define io                        \
    ios_base::sync_with_stdio(0); \
    cin.tie(0);                   \
    cout.tie(0);

unordered_map<char, string> hashmap;
//struct for huffman tree
struct huffnode
{
    char character;
    unsigned int freq;
    struct huffnode *left, *right;
};
typedef struct huffnode *NODE;

struct Minheap
{
    unsigned short size, capacity;
    NODE *array;
};
typedef struct Minheap *HEAP;

NODE newleafnode(char c, int f)
{
    NODE tmp = new huffnode();
    tmp->character = c;
    tmp->freq = f;
    tmp->left = NULL;
    tmp->right = NULL;
    return tmp;
}

void swap(NODE *a, int i, int j)
{
    NODE tmp = a[i];
    a[i] = a[j];
    a[j] = tmp;
}

void heapify(HEAP heap)
{
    unsigned short n = heap->size;
    NODE *a = heap->array;
    for (int i = n / 2; i > 0; i--)
    {
        int j = (2 * i + 1 <= n) ? ((a[2 * i]->freq < a[2 * i + 1]->freq) ? 2 * i : 2 * i + 1) : (2 * i);
        if (a[j]->freq < a[i]->freq)
        {
            swap(a, i, j);
            int k;
            while (j <= n / 2 && (a[j]->freq > a[2 * j]->freq || (2 * j + 1 <= n ? a[j]->freq > a[2 * j + 1]->freq : 0)))
            {
                k = (2 * j + 1 <= n) ? ((a[2 * j]->freq < a[2 * j + 1]->freq) ? 2 * j : 2 * j + 1) : (2 * j);
                swap(a, k, j);
                j = k;
            }
        }
    }
}

NODE mintop(HEAP heap)
{
    NODE top = heap->array[1];
    heap->array[1] = heap->array[heap->size--];
    heapify(heap);
    return top;
}

void insertnode(HEAP heap, NODE leftchild, NODE rightchild, int f)
{
    NODE tmp = new huffnode();
    tmp->character = '\0';
    tmp->freq = f;
    tmp->left = leftchild;
    tmp->right = rightchild;
    heap->array[++heap->size] = tmp;
    heapify(heap);
}

void huffman_tree(HEAP heap)
{
    while (heap->size > 1)
    {
        NODE left_child = mintop(heap);
        NODE right_child = mintop(heap);
        insertnode(heap, left_child, right_child, (left_child->freq + right_child->freq));
    }
}

void assign_codes(NODE root, string str)
{
    if (!root->left && !root->right)
    {
        hashmap[root->character] = str;
    }
    else
    {
        assign_codes(root->left, str + "0");
        assign_codes(root->right, str + "1");
    }
}

int main()
{
    io;

    string input_filename = "input.txt"; // File to be compressed
    string output_filename = "compressed.bin";

    ifstream input(input_filename, ios::binary);
    if (!input)
    {
        cout << "Error: Cannot open " << input_filename << endl;
        return 1;
    }

    ofstream output(output_filename, ios::binary);
    if (!output)
    {
        cout << "Error: Cannot create " << output_filename << endl;
        return 2;
    }

    // Read file content and calculate frequency
    int freq[256] = {0};
    char ch;
    int file_size = 0, num_of_unique_chars = 0;
    while (input.get(ch))
    {
        if (!freq[(unsigned char)ch])
            num_of_unique_chars++;
        freq[(unsigned char)ch]++;
        file_size++;
    }

    input.clear();
    input.seekg(0, ios::beg);

    // Create a min-heap
    HEAP heap = new Minheap();
    heap->capacity = num_of_unique_chars + 1;
    heap->size = 0;
    heap->array = new NODE[heap->capacity];

    for (int i = 0; i < 256; i++)
    {
        if (freq[i])
        {
            heap->array[++heap->size] = newleafnode(i, freq[i]);
        }
    }

    heapify(heap);
    huffman_tree(heap);
    assign_codes(heap->array[heap->size], "");

    // Encode file content into binary string
    string file_contents = "";
    while (input.get(ch))
    {
        file_contents += hashmap[ch];
    }
    input.close();

    // Add padding to make it multiple of 8
    int padding = (8 - file_contents.length() % 8) % 8;
    file_contents.insert(0, padding, '0');

    // Write Huffman codes to output file
    for (auto &pair : hashmap)
    {
        output.put(pair.first);
        output << pair.second << '\n';
    }
    output.put('\0'); // Separator

    // Write padding info
    output.put(padding + '0');

    // Convert binary string to bytes and write to file
    for (size_t i = 0; i < file_contents.size(); i += 8)
    {
        bitset<8> bits(file_contents.substr(i, 8));
        output.put(static_cast<char>(bits.to_ulong()));
    }

    cout << "Compression successful! Compressed file: " << output_filename << endl;
    output.close();

    delete[] heap->array;
    delete heap;

    return 0;
}
