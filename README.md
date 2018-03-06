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

## Mechanics

This implementation uses a data structure optimized for hashing larger files and deferring expensive calls to `free()` until after the entire checksum is calculated.

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
