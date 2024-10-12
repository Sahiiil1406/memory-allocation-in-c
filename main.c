#include <sys/types.h>
#include <unistd.h>
#include <stdio.h>


#define META_BLOCK_SIZE 20
#define align4(x) (((((x)-1) >> 2) << 2) + 4)

typedef struct meta_block *meta_ptr;

void *base = NULL;

struct meta_block
{
    int free;
    size_t size;
    meta_ptr next;
    meta_ptr prev;
    void *ptr;
    char data[1];
};

//helper functions for malloc
meta_ptr find_suitable_block(meta_ptr *last, size_t size)
{
    meta_ptr b = base;
    while (b && !(b->free && b->size >= size))
    {
        *last = b;
        b = b->next;
    }
    return last;
}

void split_space(meta_ptr block, size_t size)
{
    meta_ptr new_block;
    new_block = block->data + size;
    new_block->size = block->size - size - META_BLOCK_SIZE;
    new_block->next = block->next;
    new_block->free = 1;
    new_block->ptr = new_block->data;
    new_block->prev = block;
    block->next = new_block;
    block->size = size;
    if (new_block->next)
    {
        new_block->next->prev = new_block;
    }
}


meta_ptr extend_heap(meta_ptr last, size_t size)
{
    meta_ptr old_break, new_break;
    old_break = sbrk(0);
    new_break = sbrk(META_BLOCK_SIZE + size);
    if (new_break == (void *)-1)
    {
        return NULL;
    }
    old_break->size = size;
    old_break->free = 0;
    old_break->next = NULL;
    old_break->prev = NULL;
    old_break->ptr = old_break->data;
    if (last)
    {
        last->next = old_break;
    }
    return (old_break);
}
//malloc function   
void *malloc(size_t size)
{
    meta_ptr block, last;
    size_t s;
    s = allign4(size);
    if (base)
    {
        last = base;
        block = find_suitable_block(&last, s);
        if (block)
        {
            if (block->size - s >= (META_BLOCK_SIZE + 4))
            {
                split_space(block, s);
            }
            block->free = 0;
        }
        else
        {
            block = extend_heap(last, s);
            if (!block)
            {
                return NULL;
            }
        }
    }
    else
    {
        block = extend_heap(NULL, s);
        if (!block)
        {
            return NULL;
        }
        base = block;
    }
    return block->data;
}

//helper functions for free
meta_ptr merge_blocks(meta_ptr block)
{
    if (block->next && block->next->free)
    {
        block->size += META_BLOCK_SIZE + block->next->size;
        block->next = block->next->next;
    }
    if (block->next)
    {
        block->next->prev = block;
    }
    return block;
}

meta_ptr get_block_addr(void *p)
{
    char *tmp = p;
    tmp = tmp - META_BLOCK_SIZE;
    p = tmp;
    return (p);
}

int is_addr_valid(void *p)
{
    if (base)
    {
        if (p > base && p < sbrk(0))
        {
            return (p == get_block_addr(p)->ptr);
        }
    }
    return 0;
}
//free function
void free(void *ptr)
{
    if (is_addr_valid(ptr))
    {
        meta_ptr block = get_block_addr(ptr);
        block->free = 1;
        if (block->prev && block->prev->free)
        {
            block = merge_blocks(block->prev);
        }

        if (block->next)
        {
            block = merge_blocks(block);
        }
        else
        {
            if (block->prev)
            {
                block->prev->next = NULL;
            }
            else
            {
                base = NULL;
            }
            brk(block);
        }
    }
}

//helper functions for realloc
void copy_data(meta_ptr src, meta_ptr dest)
{
    int *src_data = src->data;
    int *dest_data = dest->data;
    size_t i;
    for (i = 0; i * 4 < src->size && i * 4 < dest->size; i++)
    {
        dest_data[i] = src_data[i];
    }
}
//realloc function
void *realloc(void *p, size_t size)
{
    size_t s;
    meta_ptr new_block, old_block;
    void *new_ptr;

    if (!p)
    {
        return malloc(size);
    }
    if (is_addr_valid(p))
    {
        s = align4(size);
        old_block = get_block_addr(p);
        if (old_block->size >= s)
        {
            if (old_block->size >= (META_BLOCK_SIZE + 4))
            {
                split_space(old_block, s);
            }
        }
        else
        {
            if (old_block->next && old_block->next->free && (old_block->size + old_block->next->size + META_BLOCK_SIZE) >= s)
            {
                merge_blocks(old_block);
                if (old_block - s >= (META_BLOCK_SIZE + 4))
                {
                    split_space(old_block, s);
                }
            }
            else
            {
                new_ptr = malloc(s);
                
                if (!new_ptr)
                    return NULL;
                
                new_block = get_block_addr(new_ptr);
                copy_data(old_block, new_block);
                
                free(p);
                return new_ptr;
            }
        }
        return p;
    }
    return NULL;
}

//calloc function
void *calloc(size_t number, size_t size)
{
    size_t *new;
    size_t s, i;

    new = malloc(number * size);

    if (new)
    {

        s = align4(number * size) << 2;

        for (i = 0; i < s; i++)
        {
            new[i] = 0;
        }
    }
    return new;
}