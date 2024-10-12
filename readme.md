# Dynamic Memory allocation in C
This project implements custom versions of malloc, calloc, realloc, and free functions in C from scratch. These functions provide dynamic memory allocation capabilities without relying on the standard library implementations.

Dynamic memory allocation is a crucial feature in C programming, allowing for efficient use of memory by allocating and deallocating memory at runtime. This project aims to recreate the functionality of the standard memory allocation functions:
<br/>
malloc: Allocates a single block of memory
<br/>
calloc: Allocates and initializes memory to zero
<br/>
realloc: Resizes a previously allocated memory block
<br/>
free: Deallocates memory
<br/>


## Implementation Details

<br/>
Our implementation uses a custom heap management system.
<br/>
We maintain a linked list of free memory blocks to handle allocation and deallocation efficiently.
<br/>
The allocator uses a "first-fit" strategy for finding suitable memory blocks.
<br/>
Coalescing of adjacent free blocks is performed to reduce fragmentation.
