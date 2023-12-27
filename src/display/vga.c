#include "vga.h"

#include "cpu/cpu.h"
#include "libc/mem.h"

static volatile struct limine_framebuffer_request framebuffer_request = {
    .id = LIMINE_FRAMEBUFFER_REQUEST,
    .revision = 0};

struct limine_framebuffer *framebuffer;
volatile uint32_t *fb_ptr;

uint16_t display_red = 0;
uint16_t display_green = 0;
uint16_t display_blue = 0;

void init_display() {
    if (framebuffer_request.response == NULL || framebuffer_request.response->framebuffer_count < 1)
    {
        hcf();
    }

    framebuffer = framebuffer_request.response->framebuffers[0];
    fb_ptr = framebuffer->address;
}

int getScreenHeight() {
    return framebuffer->height;
}

int getScreenWidth() {
    return framebuffer->width;
}
struct limine_framebuffer *getFb(){
    return framebuffer;
}
void display_write_data(uint32_t address, uint8_t red, uint8_t green, uint8_t blue) {
    fb_ptr[address] = rgb(red, green, blue);
}

unsigned int rgb(uint8_t red, uint8_t green, uint8_t blue) {
    return (0xFF << 24) | (red << 16) | (green << 8) | blue;
}

void draw_pixel(uint64_t x, uint64_t y, uint8_t r, uint8_t g, uint8_t b) {
    uint32_t* fb_ptr = (uint32_t*)((uintptr_t)framebuffer->address + x * 4 + y * framebuffer->pitch);
    *fb_ptr = rgb(r, g, b);
}

void set_background_color(uint16_t red, uint16_t green, uint16_t blue)
{
    for (int y = 0; y < getScreenHeight(); y++) {
        for (int x = 0; x < getScreenWidth(); x++) {
            draw_pixel(x,y, red, green, blue);
        }
    }
}

RGBColor decode_color(uint32_t color) {
    RGBColor rgb;
    rgb.r = (color >> 16) & 0xFF;  // Extract red component
    rgb.g = (color >> 8) & 0xFF;   // Extract green component
    rgb.b = color & 0xFF;          // Extract blue component
    return rgb;
}

RGBColor decode_fg_color(uint32_t fg_color) {
    return decode_color(fg_color);
}

RGBColor decode_bg_color(uint32_t bg_color) {
    return decode_color(bg_color);
}
