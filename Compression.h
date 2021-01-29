/** @file */
#pragma once
#include "Structs.h"

/**Function manages compression process
@param input is an input handler
@param output is an output handler*/
void initCompression(char* input, char* output);
/**Function reads characters from input file and creates a characters list
@param file_name is a path to the input file for compression
@param head is a head to the Characters list*/
void readCharactersForCompression(char* file_name, Characters** head);
/**Function assigns huffman codes based on a huffman tree
*not embeded in the assignHuffmanCodes function to avoid redundant if statements
@param copy is a head of a copy Characters list
@param keys is a head of Keys tree*/
void assignFirstHuffmanCodes(Characters* copy, Keys* keys);
/**Function assigns huffman codes based on a huffman tree
@param copy is a head of a copy Characters list
@param keys is a head of Keys tree
@param code is a passed huffman code corresponding to current node*/
void assignHuffmanCodes(Characters* copy, Keys* keys, char* code);
/**Functions passes information about characters and coding into the output file
@param compression compression file
@param list header to Characters list*/
void compressCodes(FILE* compression, Characters* list);
/**Function passes encoded characters into the output file
@param for_compression file from where all of the uncompressed characters are read
@param compression compression file
@param list header to Characters list*/
compressText(FILE* for_compression, FILE* compression, Characters* list);