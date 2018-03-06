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
