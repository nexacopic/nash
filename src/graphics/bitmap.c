#include "bitmap.h"

void draw_image(struct limine_file* image) {
    char* rawData = (char*)image->address;

    // Iterate through each byte in rawData and print it
    for (size_t i = 0; i < sizeof(rawData); i++) {
        nighterm_write(rawData[i]);
    }

    // Add a newline for better formatting
    printf("\n");
}