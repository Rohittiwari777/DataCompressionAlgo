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
        cout << "Usage: ./lz77_decompress <input file> <output file>" << endl;
        return 1;
    }

    ifstream inputFile(argv[1], ios::binary);
    if (!inputFile) {
        cout << "Error opening compressed file" << endl;
        return 2;
    }

    vector<LZ77Token> tokens;
    LZ77Token token;
    while (inputFile.read(reinterpret_cast<char *>(&token), sizeof(token))) {
        tokens.push_back(token);
    }
    inputFile.close();

    string decompressed = decompressLZ77(tokens);

    ofstream outputFile(argv[2]);
    if (!outputFile) {
        cout << "Error creating decompressed file" << endl;
        return 3;
    }

    outputFile << decompressed;
    outputFile.close();

    cout << "Decompression successful!" << endl;
    return 0;
}
