#ifndef BITMAP_H
#define BITMAP_H

#include <limine.h>
#include <nighterm/nighterm.h>
#include <utilities/printf.h>
#include <display/vga.h>
#include <libc/mem.h>
#include <serial/serial.h>

#include <stdint.h>

void draw_image(struct limine_file* image);

#endif /* BITMAP_H */
