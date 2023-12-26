#include "mem.h"
#include <limine.h>
#include <nighterm/nighterm.h>


volatile struct limine_memmap_request memmap_request = { // memory map
    .id = LIMINE_MEMMAP_REQUEST,
    .revision = 0};
volatile struct limine_memmap_response *memmap;


typedef struct block {
    long unsigned size;            // size of the block
    int free;               // free?
    struct block *next;     // next
} block_t;

// free
static block_t *free_list = NULL;

void init_physical_memory()
{
    memmap = memmap_request.response;
    for (size_t i = 0; i < memmap->entry_count; i++) {
        struct limine_memmap_entry *entry = memmap->entries[i];
        if (entry->type == LIMINE_MEMMAP_USABLE){
            printf("Initializing memory block at %lu with a size of %lu\n", entry->base, entry->length);
            if(i!=memmap->entry_count){
                initialize_memory_block(entry->base, entry->length, (memmap->entries[i+1])->base); // jkghjdfjkghdfkjghdkjfgh
            }
            else{
                initialize_memory_block(entry->base, entry->length, NULL);
            }
        }
        else{
            printf("Unusable memory block at %lu\n", entry->base);
        }
    }
        
}

void initialize_memory_block(long unsigned start_addr, long unsigned size, long unsigned start_addr_next) {
    free_list = (block_t *)start_addr;
    free_list->size = size - sizeof(block_t);
    free_list->free = 1;
    free_list->next = (block_t *)start_addr_next;
}

void *memcpy(void *dest, const void *src, size_t n) {
    uint8_t *pdest = (uint8_t *)dest;
    const uint8_t *psrc = (const uint8_t *)src;

    for (size_t i = 0; i < n; i++) {
        pdest[i] = psrc[i];
    }

    return dest;
}

void *memset(void *s, int c, size_t n) {
    uint8_t *p = (uint8_t *)s;

    for (size_t i = 0; i < n; i++) {
        p[i] = (uint8_t)c;
    }

    return s;
}

void *memmove(void *dest, const void *src, size_t n) {
    uint8_t *pdest = (uint8_t *)dest;
    const uint8_t *psrc = (const uint8_t *)src;

    if (src > dest) {
        for (size_t i = 0; i < n; i++) {
            pdest[i] = psrc[i];
        }
    } else if (src < dest) {
        for (size_t i = n; i > 0; i--) {
            pdest[i-1] = psrc[i-1];
        }
    }

    return dest;
}

int memcmp(const void *s1, const void *s2, size_t n) {
    const uint8_t *p1 = (const uint8_t *)s1;
    const uint8_t *p2 = (const uint8_t *)s2;

    for (size_t i = 0; i < n; i++) {
        if (p1[i] != p2[i]) {
            return p1[i] < p2[i] ? -1 : 1;
        }
    }

    return 0;
}

void *malloc(size_t size)
{
    // i am going insane
    block_t *current, *previous = NULL;
    void *result = NULL;

    if (size <= 0) {
        return NULL;
    }

    current = free_list;
    while (current != NULL) {
        if (current->size >= size && current->free) {
            current->free = 0;
            result = (void *)(current + 1);
            break;
        }
        previous = current;
        current = current->next;
    }
    return result;
}

void free(void *ptr) {
    if (ptr != NULL) {
        block_t *block = (block_t *)ptr - 1;
        block->free = 1;
    }
}