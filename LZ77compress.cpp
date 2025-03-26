#include <iostream>
#include <vector>
#include <string>
#include <fstream>

using namespace std;

struct LZ77Token {
    int offset;
    int length;
    char nextChar;
};

vector<LZ77Token> compressLZ77(const string &input, int searchBufferSize, int lookaheadBufferSize) {
    vector<LZ77Token> tokens;
    int i = 0, size = input.size();
    
    while (i < size) {
        int bestOffset = 0, bestLength = 0;
        
        for (int j = max(0, i - searchBufferSize); j < i; j++) {
            int k = 0;
            while (k < lookaheadBufferSize && (i + k) < size && input[j + k] == input[i + k]) {
                k++;
            }
            if (k > bestLength) {
                bestLength = k;
                bestOffset = i - j;
            }
        }
        
        char nextChar = (i + bestLength < size) ? input[i + bestLength] : '\0';
        tokens.push_back({bestOffset, bestLength, nextChar});
        i += bestLength + 1;
    }
    return tokens;
}

string decompressLZ77(const vector<LZ77Token> &tokens) {
    string output = "";
    for (const auto &token : tokens) {
        int start = output.size() - token.offset;
        for (int i = 0; i < token.length; i++) {
            output += output[start + i];
        }
        if (token.nextChar != '\0') {
            output += token.nextChar;
        }
    }
    return output;
}

int main(int argc, char *argv[]) {
    if (argc != 3) {
        cout << "Usage: ./lz77 <input file> <output file>" << endl;
        return 1;
    }

    ifstream inputFile(argv[1]);
    if (!inputFile) {
        cout << "Error opening input file" << endl;
        return 2;
    }
    
    string input((istreambuf_iterator<char>(inputFile)), istreambuf_iterator<char>());
    inputFile.close();
    
    vector<LZ77Token> compressed = compressLZ77(input, 256, 16);
    
    ofstream outputFile(argv[2], ios::binary);
    if (!outputFile) {
        cout << "Error creating output file" << endl;
        return 3;
    }
    
    for (const auto &token : compressed) {
        outputFile.write(reinterpret_cast<const char *>(&token), sizeof(token));
    }
    outputFile.close();
    
    cout << "Compression successful!" << endl;
    return 0;
}
