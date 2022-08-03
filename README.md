# 1-2-8
Simulate 128-bit integers using a struct housing two uint64_t types on non-supporting compilers. It is not particularly useful on its own, but it is to be used in my other repositories that require integer sizes more than 64 bits. The main function simply tests and debugs this struct against compilers that support this natively.

## Compilation Instructions
Any modern C compiler is fine.
```
gcc main.c
```
## Licensing
The code is free to use by anyone with or without my permission.
