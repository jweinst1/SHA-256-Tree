#ifndef FILE_READER_HEADER
#define FILE_READER_HEADER
// Header for reading 1mb chunk from files.
#include <stdio.h>
#include <stdlib.h>

#define FILE_MB (1024 * 1024)
#define FILE_MB_EQ(num) (num == (1024 * 1024))

#define FileChunk_SIZE (sizeof(FileChunk) + (1024 * 1024))

// Util to obtain entire file size
#define GET_FILE_SIZE(filePtr, sizeHolder) do { \
                fseek (filePtr, 0, SEEK_END); \
                sizeHolder = ftell (filePtr); \
                rewind (filePtr); \
} while(0)

// Opens and sets a file ptr in reading binary mode
// Checks
#define OPEN_BINARY_FILE(filePtr, filePath) if((filePtr = fopen(filePath, "rb")) == NULL) { \
                fprintf(stderr, "File Error: Cannot open file: %s, exiting\n", filePath); \
                exit(2); \
}


typedef struct
{
        size_t dataSize;
        unsigned char data[0];
} FileChunk;

// Creates a new file chunk
// warning, allocates 1mb chunk of memory on heap.
FileChunk* FileChunk_new(void)
{
        FileChunk* newchunk = malloc(FileChunk_SIZE);
        newchunk->dataSize = 0;
        return newchunk;
}

// reads a chunk of up to 1 mb of data from a file, prep phase before hashing.
// returns 1 if a whole MB was read, retruns 0 if less than one MB was read.
int FileChunk_fill(FILE* fstream, FileChunk* fchunk)
{
        fchunk->dataSize = fread(fchunk->data, sizeof(unsigned char), FILE_MB, fstream);
        return FILE_MB_EQ(fchunk->dataSize);
}

#endif
