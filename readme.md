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


### Memory Pool Setup:
Create a large block of memory (usually with malloc or a static array) that acts as the pool from which your allocations will be made. This could be a predefined size based on the needs of your application.

### Metadata Tracking:
Maintain a data structure (e.g., a linked list) to track memory blocks within the pool. Each block should store metadata like the size of the allocated block and whether it is free or in use.

### Allocation (my_malloc):

When a user requests memory, search the metadata for a free block that’s large enough to satisfy the request.
If found, split the block if necessary (if the block is larger than the requested size), update the metadata, and return a pointer to the allocated memory.
If no block is available, return NULL.
###Deallocation (my_free):

When a user frees memory, mark the corresponding block as free in the metadata.
Optionally, merge adjacent free blocks to reduce fragmentation.
