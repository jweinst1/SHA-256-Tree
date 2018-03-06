# SHA-256-Tree
A C implementation of the SHA 256 tree hash algorithm

## Intro

This repo contains an implementation of the SHA-256 tree hash algorithm. This algorithm takes a file of binary data (or treats it as binary data), and calculates a tree-ordered checksum of each one megabyte chunk of data present for the file. This implementation does not use any existing 3rd party libraries, just the C standard library. If a file is less than 1 MB in size, no tree hash reduction logic is involved.

## Installation

To build the CLI tool, clone the repo then run in your terminal:

```
$ make cli
```

This builds an executable in a `bin` dir named `shatree`.

## Usage

To open the help guide, simply run the executable

```
$ ./bin/shatree
----Usage Guide----
This executable perfroms as SHA-256 tree hash on documents and prints out detailed performance info.
It also prints out round of reduction info.
shatree <filename>
-------------------
```

To hash a file, supply the file name
```
$ ./bin/shatree testfiles/test1.JPG
```

## Mechanics

This implementation uses a data structure optimized for hashing larger files and deferring expensive calls to `free()` until after the entire checksum is calculated.

This is accomplished by usage a garbage stack, and not freeing any hold hash nodes until the final checksum is completed.

### HashListing

The first process in this implementation is to turn a document into 1MB hashed chunks, which is essentially a linked list of hashes. Here is an example debug-representation of that.

```
____Document_Hash_List______
HashNode:(1) @ 0x7f96b9402660
Hash = 185520110146192269015611140144228249224192322714914120324819582118249211941499127
- - - - - - -
HashNode:(2) @ 0x7f96b9402690
Hash = 312255676321378911615719087129204205887213745121385134163822271991722812221732
- - - - - - -
____________________________
```

Since only two chunks exist in this hash list, the file is just over 1 MB in size.

After this, the hash nodes in the list are reduced, by concatenation and rehashing. This is done until only 1 node remains, which is the final hash.


## Example

Below is a full print out of the hash result of a file.

```
Hashing File at path: testfiles/test2.jpg
Chunk Hashing completed, elapsed time = 0.056209s
---Reducing Hashed File Chunks---
Hash Reduction Round 1
::::::::::::::::::::::::
____Document_Hash_List______
HashNode:(1) @ 0x7f8315c02660
Hash = 216127152166107254150130128181986886246133188142183438739732437915754250761552297156
- - - - - - -
HashNode:(2) @ 0x7f8315d00000
Hash = 2920412165171605013163177229967024934302508511725422221221761826398136223211127225
- - - - - - -
HashNode:(3) @ 0x7f8315e00000
Hash = 1292038313822021715171236282181071271682121472166024610220224378154395422318520521343230
- - - - - - -
HashNode:(4) @ 0x7f8315f00000
Hash = 3185189426117824857411021822712821212417920612915837981775911138982517125324521256
- - - - - - -
____________________________
Hash Reduction Round 2
::::::::::::::::::::::::
____Document_Hash_List______
HashNode:(1) @ 0x7f8315c02660
Hash = 23918810059971541311206230941892917820837248200366720993180852209519814321923471253
- - - - - - -
HashNode:(2) @ 0x7f8315e00000
Hash = 58651729825324016796232334223895171337716544629143706711722417610112429253179148
- - - - - - -
____________________________
Hash Reduction Round 3
::::::::::::::::::::::::
____Document_Hash_List______
HashNode:(1) @ 0x7f8315c02660
Hash = 11173143892071531461621732122251137125150193240123497616925521016990140207232423664175
- - - - - - -
____________________________
Hash Chunk Reducing Finished, elapsed time = 0.000062s
Final Checksum Hash = 11173143892071531461621732122251137125150193240123497616925521016990140207232423664175
~~~~~~~~~
```
