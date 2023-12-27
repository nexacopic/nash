#include <stddef.h>
#include <stdint.h>
#include <stdarg.h>

void init_physical_memory();

void initialize_memory_block(long unsigned start_addr, long unsigned size, long unsigned start_addr_next); // only so you can cause some cool panic screens

void *memcpy(void *dest, const void *src, size_t n); 

void *memset(void *s, int c, size_t n);

void *memmove(void *dest, const void *src, size_t n);

int memcmp(const void *s1, const void *s2, size_t n);

void *malloc(size_t size);
void free(void *ptr);