/** @file */
#pragma once
#define _CRTDBG_MAP_ALLOC
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <stdbool.h>
#include <crtdbg.h>

/**Struct Character is characters list*/
typedef struct Character
{
	/**Character to be encoded, primary key*/
	char character;
	/**Character count*/
	unsigned int value;
	/**Huffman code*/
	char* code;
	/**Pointer to next element*/
	struct Character* next;
	/**Pointer to possible Key tree*/
	struct Key* tree;

}Characters;

/**Struct Key is a huffman tree struct*/
typedef struct Key
{
	/**Character to be encoded or decoded*/
	char character;
	/**Character count*/
	unsigned int value;
	/**Pointer to left node*/
	struct Key* left_node;
	/**Pointer to right node*/
	struct Key* right_node;

}Keys;

/**Simple function used to describe abrupt program endings
@param func function name
@param reason reason for program ending*/
void endPrg(char* func, const char* reason);
/**Function returns rounded up byte value
@param val length of bit code
@return ceil of division by 8*/
int ceiling(int val);
/**Function append additional character at the end of passed string
@param string passed string
@param character character to be appended
@return string with appended character*/
char* appendCharacter(char* string, char character);
/**Function used to construct huffman tree from existing Characters list
@param head an adress to head of Characters list
@return Keys head*/
Keys* huffmanTreeConstruction(Characters** head);
/**Function removes leftmost element (with lowest value)
@param head is a head of Characters list*/
void removeCharacter(Characters** head);
/**Function performs merge sort on Characters list
@param head_reference is a reference to head and later to the head of subset*/
void mergeSort(Characters** head_reference);
/**Function merges sorted subsets of Characters list
@param a head of left subset
@param b head of right subset
@return pointer to either a or b*/
Characters* sortedMerge(Characters* a, Characters* b);
/**Split the Characters list
@param head head of Characters list
@param left reference to left subset
@param right reference to right subset*/
void frontBackSplit(Characters* head, Characters** left, Characters** right);
/**Function adds new character or updates existing one in Characters list
@param character character to be added to Characters list
@param head head of the list*/
void addCharacter(char character, Characters** head);
/**Function adds new Characters element
@param character character to be stored in element
@param head head of Characters list*/
void newCharacter(unsigned char character, Characters** head);
/**Function creates new Characters element and inserts it in sorted manner
@param head head of Characters list
@param tree passed Keys tree root*/
void newSortedInsertCharacter(Characters** head, Keys* tree);
/**Function inserts new Characters element in an unsorted maner
@param new element to be inseted
@param head head of Characters list*/
static inline void insertCharacter(Characters* new, Characters** head);
/**Function searches for given character in Characters list
@param character character to be found
@param head head of Characters list
@return found character or lack thereof*/
Characters* searchCharacter(char character, Characters* head);
/**Function copies Characters list
@param original is a head to original list
@return head to copied list*/
Characters* copyCharacters(Characters* original);
/**Function recursively deletes Character list elements
@param head is passed head and then any passed element of Characters list*/
void deleteCharacters(Characters* head);
/**Function creates new Keys element
@param head head of element list
@return newly created or existing huffman tree root*/
Keys* newKey(Characters** head);
/**Function recursively deletes Keys tree
@param root pointer to root and later any Keys element*/
void deleteKeys(Keys* root);