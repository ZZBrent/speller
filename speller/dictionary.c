#include <stdbool.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <ctype.h>
#include "dictionary.h"

// Number of lists in hash table
#define tableLength 2000

//Definition for node, which are the building blocks of the linked lists in the hash table
typedef struct node
{
    char word[LENGTH + 1];
    struct node* next;
}
node;

// Hash table definition; an array of linked lists
node* hashTable[tableLength];

int hash(const char* word);

// Number of words in the dictionary
int wordCount = 0;

/**
 * Returns true if word is in dictionary else false.
 */
bool check(const char* word)
{
    int len = strlen(word);
    char lowercaseWord[LENGTH+1];

    // Turn capital letters to lowercase
    for (int i = 0; i < len; i++)
    {
        if(isupper(word[i]))
        {
            lowercaseWord[i] = tolower(word[i]) ;
        }
        else
        {
            //Since word is already lowercase, use it
            lowercaseWord[i] = word[i];
        }
    }
    //set the last value in the array (of length len) equal to the null char
    lowercaseWord[len] = '\0';
    //Hash the word
    int hashValue = hash(lowercaseWord);

    //Start at the first node and then work through the list of nodes until you reach the end (when currentNode == null)
    //If you don't find the word then it is not in the loaded dictionary
    node* currentNode = hashTable[hashValue];
    while (currentNode != NULL)
    {
        if (strcmp(lowercaseWord, currentNode->word) == 0)
        {
            return true;
        }
        currentNode = currentNode->next;
    }

    return false;
}


/**
 * Loads dictionary into memory.  Returns true if successful else false.
 */
bool load(const char* dictionary)
{
    // Initialize the hash table (to prepare it for values; just in case there are random values already stored in memory)
    for(int i = 0; i < tableLength; i++)
    {
        hashTable[i] = NULL;
    }

    // Open the dictionary
    FILE* d;
    d = fopen(dictionary, "r");

    // Check if dictionary opened
    if (d == NULL)
    {
        printf("Failed to load dictionary");
        return false;
    }

    char buffer[LENGTH+1];
    //Add each new word to buffer and loop until the end of the file
    while (fscanf(d, "%s", buffer) > 0)
    {
        // Allocate memory
        node* newNode = malloc(sizeof(node));
        //Since the node is new, there is nowhere for it to point yet
        newNode->next = NULL;
        //Put word in node
        strcpy(newNode->word, buffer);
        //Get hash value for the word
        int hashValue = hash(newNode->word);
        //Check if first node in linked list
        if (hashTable[hashValue] == NULL)
        {
            hashTable[hashValue] = newNode;
        }
        else
        {
            newNode->next = hashTable[hashValue];
            hashTable[hashValue] = newNode;
        }
        // Track word count for later output
        wordCount++;
    }
    // Close file
    fclose(d);
    return true;
}


/**
 * Returns number of words in dictionary if loaded else 0 if not yet loaded.
 */
unsigned int size(void)
{
    return wordCount;
}


/**
 * Unloads dictionary from memory.  Returns true if successful else false.
 */
bool unload(void)
{
    // Iterate over all linked lists in hash table.  Set
    // currentNode to point at each one's location in memory.
    for (int i = 0; i < tableLength;i++)
    {
        node* currentNode = hashTable[i];
        while (currentNode != NULL)
        {
            //Before freeing current node, set temp to it so we don't lose track of it
            node* temp = currentNode;
            currentNode = currentNode->next;
            free(temp);
        }
    }
    return true;
}

//Hash function
int hash(const char* word)
{
    int sum = 0;
    int len = strlen(word);

    for (int i = 0; i < len; i++)
    {
        sum += word[i];
    }

    int hashValue = sum % tableLength;
    return hashValue;
}