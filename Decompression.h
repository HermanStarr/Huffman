/** @file */
#pragma once
#include "Structs.h"

/**Function manages through decompression process
@param compression_file is a path to compressed file
@param decompression_file is a path to decompression file*/
void initDecompression(const char* compression_file, const char* decompression_file);
/**Function reads characters and corresponding codes
@param root rooot of Keys tree
@param compression compressed file handle*/
void readKeys(Keys* root, FILE* compression);
/**Function creates a part of tree using keys from file
@param root is a root of Keys tree
@param code is acode corresponding to certain key
@param character is a character represented by given key*/
void keysBuildTree(Keys* root, char* code, char character);
/**Function decompresses given encoded string in file
@param root rooot of Keys tree
@param compression compressed file handle
@param decompression decompression file handle*/
void decompress(Keys* root, FILE* compression, FILE* decompression);