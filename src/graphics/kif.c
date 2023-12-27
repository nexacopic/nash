#include "kif.h"
#include <stdint.h>

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

    // Calculate number of rows needed for the image based on height
    uint64_t imageRows = (height + term.fonthdr.height - 1) / term.fonthdr.height;

    // Parse pixel data
    uint64_t x = term.curX * term.fonthdr.width, y = term.curY * term.fonthdr.height;
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
            if (y == term.rows) { // Check if we've reached the end of terminal rows
                term.curY++;
                y = 0;
            }
        }
    }

    // Adjust curY based on the number of rows used by the image
    term.curY += imageRows;
}

