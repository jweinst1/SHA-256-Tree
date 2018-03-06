#include <time.h>
#include "FileReader.h"
#include "SHA256Hash.h"
#include "TreeHashTools.h"

#define HASH_BUF_2ND(hashbuf) (hashbuf + 32)

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
        printf("Hasing File at path: %s\n", path);
        clock_t startClock, endClock;
        startClock = clock();
        FILE* fileToHash;
        TreeHashNode* hashList = TreeHashNode_new_empty(); // hash list that will be returned.
        TreeHashNode* traverse = hashList; // traverses and extends the hash list.
        unsigned char hasherSlot[HASH_PART_SIZE]; //place where new hashes are written.
        OPEN_BINARY_FILE(fileToHash, path);
        FileChunk* chunkHashHolder = FileChunk_new();
        int continueHashing = 1;
        while(continueHashing)
        {
                continueHashing = FileChunk_fill(fileToHash, chunkHashHolder);
                sha256_byte_to_hash(chunkHashHolder->data, chunkHashHolder->dataSize, traverse->hash);
                if(continueHashing) traverse->next = TreeHashNode_new_empty();
                traverse = traverse->next;
        }
        fclose(fileToHash);
        free(chunkHashHolder);
        endClock = clock();
        printf("Chunk Hashing completed, elapsed time = %fs\n", (endClock-startClock)/(double)CLOCKS_PER_SEC);
        return hashList;
}

// reduces two hashes into 1 hash by re-hashing.
void reduce_two_hashes(unsigned char* hash1, unsigned char* hash2, unsigned char* output)
{
        unsigned char twoHashBuf[HASH_PART_SIZE * 2]; // used for concatenating hashes.
        memcpy(twoHashBuf, hash1, HASH_PART_SIZE);
        memcpy(HASH_BUF_2ND(twoHashBuf), hash2, HASH_PART_SIZE);
        sha256_byte_to_hash(twoHashBuf, HASH_PART_SIZE * 2, output);
}

// Reduces a hashed file list to a single check sum.
TreeHashNode* reduce_hash_list(TreeHashNode* fileHashList)
{
        printf("---Reducing Hashed File Chunks---\n");
        TreeHashNode* reducePtr;
        TreeHashNode* oldPoppedNode;
        int roundCount = 1;
        clock_t startClock, endClock;
        startClock = clock();
        while(fileHashList->next != NULL)
        {
                printf("Hash Reduction Round %d\n::::::::::::::::::::::::\n", roundCount++);
                TreeHashNode_printinfo(fileHashList);
                reducePtr = fileHashList;
                while(reducePtr != NULL)
                {
                        reduce_two_hashes(reducePtr->hash, reducePtr->next->hash, reducePtr->hash);
                        oldPoppedNode = TreeHashNode_pop_next(reducePtr);
                        if(oldPoppedNode != NULL) GarbageStack_push(oldPoppedNode);
                        reducePtr = reducePtr->next;
                }

        }
        endClock = clock();
        printf("Hash Reduction Round %d\n::::::::::::::::::::::::\n", roundCount++);
        TreeHashNode_printinfo(fileHashList);
        printf("Hash Chunk Reducing Finished, elapsed time = %fs\n", (endClock-startClock)/(double)CLOCKS_PER_SEC);
        GarbageStack_flush();
        return fileHashList;
}

void helper_info(void)
{
        puts("----Usage Guide----");
        puts("This executable perfroms as SHA-256 tree hash on documents and prints out detailed performance info.");
        puts("It also prints out round of reduction info.");
        puts("shatree <filename>");
        puts("-------------------");
}

int main(int argc, char const *argv[]) {
        if(argc < 2)
        {
                helper_info();
                return 0;
        }
        else if (argc == 2)
        {
                TreeHashNode* hashedFile = hash_entire_file(argv[1]);
                reduce_hash_list(hashedFile);
                TreeHashNode_del_all(hashedFile);
        }
        else
        {
                fprintf(stderr, "Error, expected 1 argument, got %d\n", argc);
                exit(2);
        }
        return 0;
}
