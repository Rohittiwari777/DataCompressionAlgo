# File Compression Using Huffman Coding and LZ77

### A file compressing (and decompressing) web application designed using the concepts of Huffman Coding and LZ77.

## Introduction
Huffman Coding is a lossless data compression algorithm where variable-length prefix codes are assigned to the characters used in the file. Prefix codes are those that won't form the prefix of any other code assigned to other characters in the file. In Huffman Coding, these prefix codes (which are unique to each distinct character) are assigned efficiently such that the most frequently occurring character gets the smallest prefix code.

By doing this, we can represent a character in possibly **less than 8 bits or 1 byte**, thereby reducing the size of the compressed file. To assign these codes, a Huffman tree is generated and written into the compressed file so that it can be decompressed later from the information available in this tree. For every **left sub-tree, we move into, we take 0, and similarly, 1 for the right subtree** until we reach a node that has a character (not an internal node). More on [Huffman Coding](https://www.geeksforgeeks.org/huffman-coding-greedy-algo-3/). This application can be used to compress any text-based file, i.e., files with extensions like `.txt`, `.c`, `.cpp`, `.html`, `.js`, `.docx`, and so on!

## LZ77 Compression
LZ77 is a dictionary-based compression algorithm that replaces repeating sequences with **triplets (offset, length, next character)**:
- **Offset:** Distance to the previous occurrence of the sequence.
- **Length:** Number of matched characters.
- **Next Character:** The next unmatched character.

LZ77 is particularly efficient for large files with repeated substrings, making it an excellent complement to Huffman Coding.

## Working
The following steps are followed to build and traverse the **HUFFMAN TREE**:
<ol>
  <li>Find the frequency of each unique character present in the input file and maintain a hashmap.</li>
  <li>Create a minheap based on the frequencies of characters found in the previous step. This minheap acts as a priority queue wherein the node containing the character with the least frequency is present at the root (is the root node). The number of independent nodes in this minheap will be equal to the number of unique characters present in the input file.</li>
  <li>Until the number of independent nodes of the minheap (its size) becomes 1, -> keep popping 2 nodes one after the other from the minheap and create a parent node for these nodes -> the first popped node becoming the left child and then  -> insert the parent node back into the minheap and run the heapify function.</li>
  <li>Now traverse the Huffman Tree in such a way that for every left, take 0 and similarly 1 for right until we reach a node having a character (or a leaf node which won't be having children). The string of 0s and 1s generated now becomes the prefix code of the character. </li>
  <li>Create a hashmap mapping each character with its prefix code, and this hashmap is written at the start of the compressed/encoded file.</li>
</ol>

The following steps are followed in **LZ77 compression**:
<ol>
  <li>Maintain a sliding window consisting of a search buffer (past characters) and a lookahead buffer (upcoming characters).</li>
  <li>Find the longest match of the current sequence in the search buffer.</li>
  <li>Replace the sequence with a **triplet (offset, length, next character)**.</li>
  <li>If no match is found, store the character as-is.</li>
  <li>Move the window forward and repeat until the entire file is encoded.</li>
</ol>

### **Compression Process:**
1. Apply **LZ77** to the input file to replace repeating sequences with references.
2. Apply **Huffman Coding** on the LZ77-processed data to further reduce file size.
3. Store the Huffman Tree in the compressed file to enable decompression.

### **Decompression Process:**
1. Read the Huffman Tree from the compressed file.
2. Decode the Huffman-encoded data back to LZ77 format.
3. Apply LZ77 decompression by replacing pointers with original substrings.
4. Restore the original file.

#### Note:
The size of the input file must be big enough (**> 500 bytes**) to notice the size difference. A very small input file, if encoded/compressed, would otherwise result in a bigger compressed file owing to the additional contents of the hashmap written at the beginning (which would surpass the reduction in size obtained by replacing characters with prefix codes). This should be avoided as there is no point in having a compression algorithm if the input file is so small.

## Features
- **Supports text-based files** (e.g., `.txt`, `.c`, `.cpp`, `.html`, `.js`, `.docx`).
- **Combines Huffman Coding and LZ77** for optimized lossless compression.
- **Ensures efficient storage and fast retrieval** of compressed files.
- **Web-based interface** for easy file selection and compression.

## References
- More on [Huffman Coding](https://www.geeksforgeeks.org/huffman-coding-greedy-algo-3/)
- More on [LZ77](https://en.wikipedia.org/wiki/LZ77_and_LZ78)

This project demonstrates the power of **hybrid compression techniques** to achieve efficient and practical file storage solutions.

