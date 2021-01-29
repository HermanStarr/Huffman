/** @file */
#include "Structs.h"
#pragma warning(disable:4996)

/**Simple function used to describe abrupt program endings
@param func function name
@param reason reason for program ending*/
void endPrg(char* func, const char* reason)
{
	/*Print to stderr function name and reason for program ending*/
	fprintf(stderr, "Function %s: %s\n", func, reason);

	exit(1);
}

/**Function returns rounded up byte value
@param val length of bit code
@return ceil of division by 8*/
int ceiling(int val)
{
	if (val % 8 != 0)
		/*Return ceiling of the division*/
		return((val / 8) + 1);
	else
		return(val / 8);
}
/**Function append additional character at the end of passed string
@param string passed string
@param character character to be appended
@return string with appended character*/
char* appendCharacter(char* string, char character)
{
	size_t size = (strlen(string) + 2) * sizeof(char);
	char* new_code = realloc(string, size);
	if (new_code)
	{
		/*Set penultimate cahracter in a string to be equal to passed character*/
		new_code[size - 2] = character;
		/*Set the last character as 0x00*/
		new_code[size - 1] = 0;

		return new_code;
	}
	else
	{
		endPrg(__func__, "failed to reallocate");
		return NULL;
	}
}
/**Function used to construct huffman tree from existing Characters list
@param head an adress to head of Characters list
@return Keys head*/
Keys* huffmanTreeConstruction(Characters** head)
{
	if (*head)
	{
		/*If next element is NULL
		Performed to restrain unspecified behavior later*/
		if (!(*head)->next)
		{
			Keys* new_keys = malloc(sizeof(Keys));
			if (new_keys)
			{
				new_keys->left_node = NULL;
				new_keys->right_node = NULL;
				new_keys->value = (*head)->value;
				new_keys->character = (*head)->character;

				return new_keys;
			}
			else
				endPrg(__func__, "failed to allocate memory");
		}
		/*next element to the head is not empty
		Pairs are taken so it is crucial to be, at least, two elements present*/
		while ((*head)->next != NULL)
		{
			Keys* new_keys = malloc(sizeof(Keys));
			if (new_keys)
			{
				new_keys->left_node = newKey(head);
				removeCharacter(head);
				new_keys->right_node = newKey(head);
				removeCharacter(head);
				new_keys->value = new_keys->left_node->value + new_keys->right_node->value;
				newSortedInsertCharacter(head, new_keys);
			}
			else
				endPrg(__func__, "failed to allocate memory");
		}

		return (*head)->tree;
	}
	else
	{
		endPrg(__func__, "passed head was NULL");
		return NULL;
	}
}
/**Function removes leftmost element (with lowest value)
@param head is a head of Characters list*/
void removeCharacter(Characters** head)
{
	if (*head)
	{
		Characters* tmp = (*head);
		(*head) = tmp->next;
		free(tmp);
	}
	else
	{
		endPrg(__func__, "passed head was NULL");
	}
}
/**Function performs merge sort on Characters list
@param head_reference is a reference to head and later to the head of subset*/
void mergeSort(Characters** head_reference)
{
	Characters* head = *head_reference;
	Characters* a = NULL;
	Characters* b = NULL;

	if (head == NULL || head->next == NULL)
	{
		/*Nothing to sort*/
		return;
	}
	/*Split list into subsets*/
	frontBackSplit(head, &a, &b);
	/*Merge sort left values*/
	mergeSort(&a);
	/*Merge sort right values*/
	mergeSort(&b);
	/*Set head of characters subset to be equal to pointer returned from sortedMerge*/
	*head_reference = sortedMerge(a, b);
}
/**Function merges sorted subsets of Characters list
@param a head of left subset
@param b head of right subset
@return pointer to either a or b*/
Characters* sortedMerge(Characters* a, Characters* b)
{
	Characters* result = NULL;
	if (a == NULL)
		return (b);
	else if (b == NULL)
		return (a);
	if (a->value <= b->value) 
	{
		result = a;
		/*Sort from next element*/
		result->next = sortedMerge(a->next, b);
	}
	else 
	{
		result = b;
		/*Sort from next element*/
		result->next = sortedMerge(a, b->next);
	}

	return result;
}
/**Split the Characters list
@param head head of Characters list
@param left reference to left subset
@param right reference to right subset*/
void frontBackSplit(Characters* head, Characters** left, Characters** right)
{
	/*Pointer fast is set to be equal to the next element to the head
	During loop advances two places*/
	Characters* fast = head->next;
	/*Pointer slow equal head
	During loop advances one place*/
	Characters* slow = head;

	while (fast != NULL) 
	{
		fast = fast->next;
		if (fast != NULL) 
		{
			slow = slow->next;
			fast = fast->next;
		}
	}
	*left = head;
	*right = slow->next;
	slow->next = NULL;
}
/**Function adds new character or updates existing one in Characters list
@param character character to be added to Characters list
@param head head of the list*/
void addCharacter(char character, Characters** head)
{
	if (*head)
	{
		Characters* tmp = NULL;
		tmp = searchCharacter(character, (*head));
		if (tmp != NULL)
		{
			tmp->value += 1u;
		}
		else
			newCharacter(character, head);
	}
	else
	{
		endPrg(__func__, "head was empty");
	}
}
/**Function adds new Characters element
@param character character to be stored in element
@param head head of Characters list*/
void newCharacter(unsigned char character, Characters** head)
{
	if (!(*head)->character)
	{
		(*head)->character = character;
		(*head)->value++;
		(*head)->code = NULL;
		(*head)->tree = NULL;

		return;
	}
	Characters* new = malloc(sizeof(Characters));
	if (new)
	{
		new->code = NULL;
		new->character = character;
		new->value = 1u;
		new->next = NULL;
		new->tree = NULL;
		/*Insert character to the list*/
		insertCharacter(new, head);
		new = NULL;
	}
	else
	{
		endPrg(__func__, "failed to allocate");
	}
}
/**Function creates new Characters element and inserts it in sorted manner
@param head head of Characters list
@param tree passed Keys tree root*/
void newSortedInsertCharacter(Characters**head, Keys*tree)
{
	if (!(*head))
	{
		Characters* new = malloc(sizeof(Characters));
		if (new)
		{
			new->value = tree->value;
			new->tree = tree;
			new->next = NULL;
			new->code = NULL;
			*head = new;
			new = NULL;

			return;
		}
		else
		{
			endPrg(__func__, "failed to allocate for the head");
		}
	}
	else 
	{
		Characters* new = malloc(sizeof(Characters));
		if (new)
		{
			new->code = NULL;
			new->value = tree->value;
			new->tree = tree;
			if (new->value <= (*head)->value)
			{
				new->next = *head;
				*head = new;
			}
			else
			{
				Characters* prev = *head;
				Characters* next = NULL;
				if ((*head)->next)
					next = (*head)->next;
				for (; prev; prev = prev->next, next = next->next)
				{
					if (next)
					{
						/*If value of new is both more or equal to prev value and less than next value*/
						if (new->value >= prev->value && new->value < next->value)
						{
							prev->next = new;
							new->next = next;

							break;
						}
						continue;
					}
					else
					{
						prev->next = new;
						new->next = NULL;

						break;
					}
				}
			}
			new = NULL;
		}
		else
		{
			endPrg(__func__, "failed to allocate");
		}
	}
}
/**Function inserts new Characters element in an unsorted maner
@param new element to be inseted
@param head head of Characters list*/
static inline void insertCharacter(Characters* new, Characters** head)
{
	/*Set previous head to be next element of new Characters element*/
	new->next = (*head);
	/*Set head to be new Characters member*/
	(*head) = new;
}
/**Function searches for given character in Characters list
@param character character to be found
@param head head of Characters list
@return found character or lack thereof*/
Characters* searchCharacter(char character, Characters* head)
{
	if (head)
	{
		for( ; head; head = head->next)
			if (head->character == character)
				return head;

		return NULL;
	}
	else
	{	
		endPrg(__func__, "head was NULL");

		return NULL;
	}
}
/**Function copies Characters list
@param original is a head to original list
@return head to copied list*/
Characters* copyCharacters(Characters* original)
{
	if (original)
	{
		Characters* new = NULL, **tail = &new;
		for (; original; original = original->next) 
		{
			*tail = malloc(sizeof **tail);
			if (*tail)
			{
				/**Copy*/
				(*tail)->character = original->character;
				(*tail)->value = original->value;
				(*tail)->next = NULL;
				(*tail)->tree = NULL;
				(*tail)->code = NULL;
				/*Set tail to be its next element*/
				tail = &(*tail)->next;
			}
			else
			{
				endPrg(__func__, "failed to allocate");
			}
		}
		return new;
	}
	else
	{
		endPrg(__func__, "head was NULL");
		return NULL;
	}
}
/**Function recursively deletes Character list elements
@param head is passed head and then any passed element of Characters list*/
void deleteCharacters(Characters* head)
{
	if (head)
	{
		/*delete recursively*/
		deleteCharacters(head->next);
		if (head->code != NULL)
		{
			free(head->code);
		}
		free(head);
	}
}
/**Function creates new Keys element or returns existing one
@param head head of element list
@return newly created or existing huffman tree root*/
Keys* newKey(Characters** head)
{
	if ((*head)->tree == NULL)
	{
		Keys* new = malloc(sizeof(Keys));
		if (new)
		{
			new->character = (*head)->character;
			new->value = (*head)->value;
			new->left_node = NULL;
			new->right_node = NULL;

			return new;
		}
		else
		{
			endPrg(__func__, "failed to allocate element");
			return NULL;
		}
	}
	else
		return (*head)->tree;

}
/**Function recursively deletes Keys tree
@param root pointer to root and later any Keys element*/
void deleteKeys(Keys* root)
{
	if (root)
	{
		deleteKeys(root->left_node);
		deleteKeys(root->right_node);
		free(root);
	}
}