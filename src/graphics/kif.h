#ifndef KIF_H_
#define KIF_H_

#include <limine.h>
#include <utilities/printf.h>
#include <stdint.h>
#include <nighterm/nighterm.h>
#include <display/vga.h>

void draw_image(struct limine_file* image);

#endif // KIF_H_