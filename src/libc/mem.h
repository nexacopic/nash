#include <stddef.h>
#include <stdint.h>
#include <stdarg.h>
#include <limine.h>
#include <nighterm/nighterm.h>

void init_physical_memory();

void *memcpy(void *dest, const void *src, size_t n); 

void *memset(void *s, int c, size_t n);

void *memmove(void *dest, const void *src, size_t n);

int memcmp(const void *s1, const void *s2, size_t n);

void *malloc(size_t size);
void free(void *ptr);