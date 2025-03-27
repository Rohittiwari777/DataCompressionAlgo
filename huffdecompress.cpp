#include <iostream>
#include <unordered_map>
#include <fstream>
#include <bitset>
#include <string>

using namespace std;

string dectobin(unsigned char decimal) {
    return bitset<8>(decimal).to_string();
}

int main() {
    string input_filename = "compressed.bin";  // Input compressed file
    ifstream input(input_filename, ios::binary);
    
    if (!input) {
        cerr << "Error: Cannot open " << input_filename << endl;
        return 1;
    }
    
    string output_filename = "decompressed_output.txt";
    ofstream output(output_filename, ios::binary);
    
    if (!output) {
        cerr << "Error: Cannot create " << output_filename << endl;
        return 2;
    }
    
    unordered_map<string, char> decode_map;
    
    // Read Huffman code mappings
    string line;
    char c;
    while (input.get(c)) {
        if (c == '\0') break; // End of code section
        
        char key = c;
        getline(input, line); // Read the code (until newline)
        decode_map[line] = key;
    }
    
    // Read padding information
    char padding_char;
    input.get(padding_char);
    int padding = padding_char - '0';
    
    // Read binary data
    string binary_str;
    char byte;
    while (input.get(byte)) {
        binary_str += dectobin(static_cast<unsigned char>(byte));
    }
    
    // Remove padding bits
    binary_str = binary_str.substr(padding);
    
    // Decode binary string
    string current_code;
    for (char bit : binary_str) {
        current_code += bit;
        if (decode_map.find(current_code) != decode_map.end()) {
            output.put(decode_map[current_code]);
            current_code.clear();
        }
    }
    
    cout << "Decompression successful! Output file: " << output_filename << endl;
    
    input.close();
    output.close();
    return 0;
}