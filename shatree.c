#include "FileReader.h"
#include "SHA256Hash.h"
#include "TreeHashTools.h"

static TreeHashNode* GARBAGE_STACK = NULL;

int main(int argc, char const *argv[]) {
        FILE* testfile;
        OPEN_BINARY_FILE(testfile, "Makefile");
        FileChunk* testchunk = FileChunk_new();
        FileChunk_fill(testfile, testchunk);
        printf("Read of Chunk %lu\n", testchunk->dataSize);
        fclose(testfile);
        free(testchunk);
        return 0;
}
