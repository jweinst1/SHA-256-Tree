#ifndef TREE_HASH_TOOLS_HEADER
#define TREE_HASH_TOOLS_HEADER
// Header that contains functions for facilitating the tree hash.

#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#define HASH_PART_SIZE (sizeof(unsigned char) * 32)

/** TREE HASH NODE
 * This structure holds hashes that each correspond to 1MB chunk of a file.
 * As each 1MB chunk is read and hashed, a new node is allocated with a new has, and the linked chain
 * is extended. No calls to free() occur until after all the nodes are reduced to a single checksum.
 * Hashes can be reduced in place.
 */
struct TreeHashNode
{
        unsigned char hash[HASH_PART_SIZE];
        struct TreeHashNode* next;
};
typedef struct TreeHashNode TreeHashNode;

// Creates a new TreeHashNode
TreeHashNode* TreeHashNode_new(unsigned char* hBytes)
{
        TreeHashNode* newnode = malloc(sizeof(TreeHashNode));
        newnode->next = NULL;
        memcpy(newnode->hash, hBytes, HASH_PART_SIZE);
        return newnode;
}

// Appends a new hash node to the current node and returns the newly appended node.
TreeHashNode* TreeHashNode_append(TreeHashNode* hashNode, unsigned char* newHash)
{
        hashNode->next = TreeHashNode_new(newHash);
        return hashNode->next;
}

// Returns and removes the next hash node.
// This allows old, unwanted hash nodes to be stored in a garbage stack,
// to defer freeing the memory till after the entire check sum is calculated.
TreeHashNode* TreeHashNode_pop_next(TreeHashNode* hashNode)
{
        TreeHashNode* temp = hashNode->next;
        if(temp == NULL) return temp;
        else
        {
                hashNode->next = temp->next;
                return temp;
        }
}

// Prints out investigative and debug info about a hashList.
void TreeHashNode_printinfo(TreeHashNode* hashList)
{
        int counter = 1;
        puts("____Document_Hash_List______");
        while(hashList != NULL)
        {
                printf("HashNode:(%d) @ %p\n", counter++, hashList);
                printf("Hash = %*.u\n- - - - - - -\n", HASH_PART_SIZE, hashList->hash);
                hashList = hashList->next;
        }
        puts("____________________________");
}

// Frees all allocated hash nodes in a chain.
void TreeHashNode_del_all(TreeHashNode* hashList)
{
        while(hashList != NULL)
        {
                free(hashList);
                hashList = hashList->next;
        }
}

#endif
