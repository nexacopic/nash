#include "bitmap.h"

void draw_image(struct limine_file* image) {
    char* rawData = (char*)image->address;
    printf("%s\n", rawData);
}