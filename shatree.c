#include "FileReader.h"
#include "SHA256Hash.h"
#include "TreeHashTools.h"

// Old and used hashnodes are piled here until after checksum calculation.
static TreeHashNode* GARBAGE_STACK = NULL;

void GarbageStack_push(TreeHashNode* thnode)
{
        thnode->next = GARBAGE_STACK;
        GARBAGE_STACK = thnode;
}

// Empties and frees all memory in the garbage stacjk
void GarbageStack_flush(void)
{
        while(GARBAGE_STACK != NULL)
        {
                free(GARBAGE_STACK);
                GARBAGE_STACK = GARBAGE_STACK->next;
        }
}

// produces a linked list of tree hash nodes of each 1MB chunk of a file.
TreeHashNode* hash_entire_file(const char* path)
{
        FILE* fileToHash;
        TreeHashNode* hashList = NULL; // hash list that will be returned.
        TreeHashNode* traverse = hashList; // traverses and extends the hash list.
        unsigned char hasherSlot[HASH_PART_SIZE]; //place where new hashes are written.
        OPEN_BINARY_FILE(fileToHash, path);
        FileChunk* chunkHashHolder = FileChunk_new();
        int continueHashing = 1;
        while(continueHashing)
        {
                continueHashing = FileChunk_fill(fileToHash, chunkHashHolder);
                if(hashList == NULL)
                {
                        hashList = TreeHashNode_new_empty();
                        traverse = hashList;
                }
                else traverse = TreeHashNode_new_empty();
                sha256_byte_to_hash(chunkHashHolder->data, chunkHashHolder->dataSize, traverse->hash);
                traverse = traverse->next;
        }

        fclose(fileToHash);
        free(chunkHashHolder);
        return hashList;
}

int main(int argc, char const *argv[]) {
        TreeHashNode* hashedFile = hash_entire_file("Makefile");
        TreeHashNode_printinfo(hashedFile);
        TreeHashNode_del_all(hashedFile);
        return 0;
}
