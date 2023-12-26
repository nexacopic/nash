#include "tmpfs.h"
#include <limine.h>

typedef struct inode {
    uint32_t header_size;
    uint16_t begin_offset;
    uint16_t file_size;
    char *path;
    char *data;
} inode_t;


inode_t *fs;

void filesystem_setup(char *data)
{
    printf("Init tmpfs\n");
    system
}

char *get_file_charptr(char *path)
{
    return path;
}

void *get_file_voidptr(char *path)
{
    return (void *)get_file_charptr(path);
}