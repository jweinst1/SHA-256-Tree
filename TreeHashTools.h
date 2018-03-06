#ifndef TREE_HASH_TOOLS_HEADER
#define TREE_HASH_TOOLS_HEADER
// Header that contains functions for facilitating the tree hash.

#include <stdlib.h>
#include <string.h>

#define HASH_PART_SIZE (sizeof(unsigned char) * 32)

/** TREE HASH NODE
 * This structure holds hashes that each correspond to 1MB chunk of a file.
 * As each 1MB chunk is read and hashed, a new node is allocated with a new has, and the linked chain
 * is extended. No calls to free() occur until after all the nodes are reduced to a single checksum.
 */
struct TreeHashNode
{
        unsigned char hash[HASH_PART_SIZE];
        struct TreeHashNode* next;
};
typedef struct TreeHashNode TreeHashNode;

/** HASH REDUCER
 * This structure serves as a template for reducing the consecutive layers of hashs in the SHA tree, as two
 * hashes must be concatenated before being reduced to a new hash.
 * @size indicates whether 1 or 2 hashes are currently placed in the reducer.
 */
typedef struct
{
        unsigned char bytes[HASH_PART_SIZE * 2];
        int size;
} HashReducer;

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
