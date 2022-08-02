# 1-2-8
Simulate 128-bit integers using a struct housing two uint64_t types on non-supporting compilers. It is not particularly useful on its own, but the main function tests and debugs against compilers that do support that type natively. This is to be used in my other repositories that require integer sizes more than 64 bits.

## Compilation Instructions
```
gcc main.c
```
