#include "nighterm.h"
#include "display/vga.h"
#include "libc/mem.h"

struct Terminal term;
uint32_t default_fg_color = 0xFFFFFFFF; // Default foreground color (white)
uint32_t default_bg_color = 0x00000000; // Default background color (black)

int init_nighterm(struct limine_file *font) {
    char *psf2buf = font->address;
    psf2Hdr hdr = *(psf2Hdr *)font->address;
    psf2buf += hdr.headerSize;

    if (PSF_MODE == 2 || PSF_MODE == 1 ? (hdr.magic[0] != PSF_MAGIC0 || hdr.magic[1] != PSF_MAGIC1 || hdr.magic[2] != PSF_MAGIC2 || hdr.magic[3] != PSF_MAGIC3) : 0)
        return 0;

    size_t buffer_size = (size_t)(getScreenWidth() / hdr.width) * (getScreenHeight() / hdr.height);
    term.fonthdr = hdr;
    term.fontData = psf2buf;
    term.rows = (getScreenHeight() / hdr.height);
    term.cols = (getScreenWidth() / hdr.width);
    term.curX = 0;
    term.curY = 0;
    term.title = "Nighterm";
    term.draw_cursor = 0;
    nighterm_clear();
    nighterm_do_curinv();

    return 1;
}

void nighterm_set_char_fg(uint8_t r, uint8_t g, uint8_t b) {
    default_fg_color = (r << 16) | (g << 8) | b;
}

void nighterm_set_char_bg(uint8_t r, uint8_t g, uint8_t b) {
    default_bg_color = (r << 16) | (g << 8) | b;
}

void nighterm_render_char(int row, int col, char ch, uint8_t fg_r, uint8_t fg_g, uint8_t fg_b, uint8_t bg_r, uint8_t bg_g, uint8_t bg_b) {
    int rounding = ((term.fonthdr.width % 8) != 0) ^ (term.fonthdr.width == 9);
    uint8_t *glyph = term.fontData + ch * term.fonthdr.charSize;

    for (size_t y = 0; y < term.fonthdr.height; y++) {
        for (size_t x = 0; x < term.fonthdr.width; x++) {
            if ((glyph[y * ((term.fonthdr.width / 8) + rounding) + x / 8] >> (7 - x % 8)) & 1) {
                draw_pixel(col * term.fonthdr.width + x, row * term.fonthdr.height + y, fg_r, fg_g, fg_b);
            } else {
                draw_pixel(col * term.fonthdr.width + x, row * term.fonthdr.height + y, bg_r, bg_g, bg_b);
            }
        }
    }
}



void nighterm_refresh() {
    for (int row = 0; row < term.rows; row++) {
        for (int col = 0; col < term.cols; col++) {
            int bufferIndex = row * term.cols + col;
            char ch = term.text_buffer[bufferIndex];
            uint32_t color = term.text_colors[bufferIndex];
            uint8_t fg_r = (color >> 16) & 0xFF;
            uint8_t fg_g = (color >> 8) & 0xFF;
            uint8_t fg_b = color & 0xFF;
            uint8_t bg_r = default_bg_color >> 16 & 0xFF;
            uint8_t bg_g = default_bg_color >> 8 & 0xFF;
            uint8_t bg_b = default_bg_color & 0xFF;
            nighterm_render_char(row, col, ch, fg_r, fg_g, fg_b, bg_r, bg_g, bg_b);
        }
    }
}

void nighterm_clear() {
    size_t buffer_size = (size_t)term.rows * term.cols;
    memset(term.text_buffer, ' ', buffer_size);

    // Set colors properly
    uint32_t default_color = default_fg_color;
    for (size_t i = 0; i < buffer_size; ++i) {
        term.text_colors[i] = default_color;
    }
    nighterm_refresh();
}

void nighterm_write(char ch) {
    size_t buffer_size = (size_t)term.rows * term.cols;
    nighterm_redraw();

    switch (ch) {
        case '\n':
            term.curX = 0;
            term.curY++;
            break;
        case '\t':
            term.curX += INDENT_AMOUNT - (term.curX % INDENT_AMOUNT);
            break;
        case '\b':
            term.curX -= 1;
            break;
        case 0:
            nighterm_do_curinv();
            break; // ignore termination
        default:
            // Check if ch is not an ASCII character
            if (ch < 0 || ch > 127) {
                ch = '\0';
            }

            int bufferIndex = term.curY * term.cols + term.curX;
            term.text_buffer[bufferIndex] = ch;

            if (term.curY + 1 >= term.rows) {
                nighterm_scroll_down();
            }

            // Use current default fg and bg colors
            uint32_t cur_fg_color = default_fg_color;
            uint32_t cur_bg_color = default_bg_color;

            uint32_t fg = (cur_fg_color & 0xFFFFFF) | 0xFF000000; // Adding alpha channel
            uint32_t bg = (cur_bg_color & 0xFFFFFF) | 0xFF000000; // Adding alpha channel

            term.text_colors[bufferIndex] = (fg << 16) | bg;

            nighterm_render_char(term.curY, term.curX, ch,
                                 (cur_fg_color >> 16) & 0xFF, (cur_fg_color >> 8) & 0xFF, cur_fg_color & 0xFF,
                                 (cur_bg_color >> 16) & 0xFF, (cur_bg_color >> 8) & 0xFF, cur_bg_color & 0xFF);

            term.curX++;
            nighterm_do_curinv();
            break;
    }
}


void nighterm_move_cursor(int row, int col) {
    nighterm_redraw();
    term.curX = col;
    term.curY = row;
    nighterm_do_curinv();
}

void nighterm_do_curinv() {
    if (term.draw_cursor) {
        uint32_t tmp_color = default_bg_color;
        default_bg_color = default_fg_color;
        default_fg_color = tmp_color;
        nighterm_redraw();
    }
}

void nighterm_redraw() {
    int bufferIndex = term.curY * term.cols + term.curX;
    nighterm_render_char(term.curY, term.curX, term.text_buffer[bufferIndex], (default_fg_color >> 16) & 0xFF, (default_fg_color >> 8) & 0xFF, default_fg_color & 0xFF, (default_bg_color >> 16) & 0xFF, (default_bg_color >> 8) & 0xFF, default_bg_color & 0xFF);
}

void nighterm_scroll_down() {
    for (int i = 0; i < term.rows - 1; ++i) {
        memcpy(term.text_buffer + i * term.cols, term.text_buffer + (i + 1) * term.cols, term.cols);
        memcpy(term.text_colors + i * term.cols, term.text_colors + (i + 1) * term.cols, term.cols * sizeof(uint32_t));
    }
    int last_row_start = (term.rows - 1) * term.cols;
    memset(term.text_buffer + last_row_start, ' ', term.cols);
    for (int i = 0; i < term.cols; ++i) {
        term.text_colors[last_row_start + i] = default_fg_color;
    }
    term.curY = term.rows - 1;
    nighterm_refresh();
}