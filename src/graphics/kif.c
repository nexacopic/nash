#include "kif.h"

void draw_image(struct limine_file* image) {
    char* rawData = (char*)image->address;

    // Manually parse width and height
    uint64_t width = 0, height = 0;
    int i = 0;
    while (rawData[i] != ' ') {
        width = width * 10 + (rawData[i] - '0');
        i++;
    }
    i++; // Move past the space character
    while (rawData[i] != '\n') {
        height = height * 10 + (rawData[i] - '0');
        i++;
    }

    // Move past the newline character to get to pixel data
    i++;

    // Parse pixel data
    uint64_t x = term.curX, y = 0;
    uint8_t r, g, b;

    while (rawData[i] != '\0') {
        // Parse RGB values
        r = 0; g = 0; b = 0;
        while (rawData[i] != ' ') {
            r = r * 10 + (rawData[i] - '0');
            i++;
        }
        i++; // Move past the space character

        while (rawData[i] != ' ') {
            g = g * 10 + (rawData[i] - '0');
            i++;
        }
        i++; // Move past the space character

        while (rawData[i] != '\n') {
            b = b * 10 + (rawData[i] - '0');
            i++;
        }
        i++; // Move past the newline character

        draw_pixel(x, y, r, g, b);
        x++;
        if (x == width) {
            x = 0;
            y++;
        }
    }
}