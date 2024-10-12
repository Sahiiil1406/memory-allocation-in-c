# Dynamic Memory allocation in C
This project implements custom versions of malloc, calloc, realloc, and free functions in C from scratch. These functions provide dynamic memory allocation capabilities without relying on the standard library implementations.

Dynamic memory allocation is a crucial feature in C programming, allowing for efficient use of memory by allocating and deallocating memory at runtime. This project aims to recreate the functionality of the standard memory allocation functions:

malloc: Allocates a single block of memory
calloc: Allocates and initializes memory to zero
realloc: Resizes a previously allocated memory block
free: Deallocates memory


## Implementation Details


Our implementation uses a custom heap management system.
<br/>
We maintain a linked list of free memory blocks to handle allocation and deallocation efficiently.
The allocator uses a "first-fit" strategy for finding suitable memory blocks.
Coalescing of adjacent free blocks is performed to reduce fragmentation.
