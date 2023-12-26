#include "nighterm.h"
#include "display/vga.h"
#include "libc/mem.h"

struct Terminal term;
uint8_t fg_r = 255;
uint8_t fg_g = 255;
uint8_t fg_b = 255;

uint8_t bg_r = 0;
uint8_t bg_g = 0;
uint8_t bg_b = 0;

// TODO: Move memset into its own stdlib
// done!


int init_nighterm(struct limine_file *font)
{
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

void nighterm_set_char_fg(uint8_t r, uint8_t b, uint8_t g)
{
    fg_r = r;
    fg_g = g;
    fg_b = b;
}
void nighterm_set_char_bg(uint8_t r, uint8_t b, uint8_t g)
{
    bg_r = r;
    bg_g = g;
    bg_b = b;
}

void nighterm_render_char(int row, int col, char ch)
{
    int rounding = ((term.fonthdr.width % 8) != 0) ^ (term.fonthdr.width == 9);
    uint8_t *glyph = term.fontData + ch * term.fonthdr.charSize;

    for (size_t y = 0; y < term.fonthdr.height; y++)
    {
        for (size_t x = 0; x < term.fonthdr.width; x++)
        {
            if ((glyph[y * ((term.fonthdr.width / 8) + rounding) + x / 8] >> (7 - x % 8)) & 1)
            {
                draw_pixel(col * term.fonthdr.width + x, row * term.fonthdr.height + y, fg_r, fg_g, fg_b);
            }
            else
            {
                draw_pixel(col * term.fonthdr.width + x, row * term.fonthdr.height + y, bg_r, bg_g, bg_b);
            }
        }
    }
}

void nighterm_refresh()
{
    // Note: do not overuse this function since refreshing one character at a time is much mre efficient
    term.curX = 0;
    term.curY = 0;
    int row, col;
    for (row = 0; row < term.rows; row++)
    {
        for (col = 0; col < term.cols; col++)
        {
            char ch = term.buffer[row * term.cols + col];
            nighterm_render_char(row, col, ch);
        }
    }
}

void nighterm_clear()
{
    size_t buffer_size = (size_t)term.rows * term.cols;
    memset(term.buffer, ' ', buffer_size);
    nighterm_refresh();
}

void nighterm_write(char ch)
{
    size_t buffer_size = (size_t)term.rows * term.cols;
    nighterm_redraw();

    switch (ch)
    {
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
        term.buffer[bufferIndex] = ch;
        nighterm_render_char(term.curY, term.curX, ch);
        term.curX++;
        if (term.curX - 1 == term.cols)
        {
            term.curY++;
        }

        nighterm_do_curinv();
        break;
    }
}


void nighterm_move_cursor(int row, int col)
{
    nighterm_redraw();
    term.curX = col;
    term.curY = row;
    nighterm_do_curinv();
}
void nighterm_redraw()
{
    int bufferIndex = term.curY * term.cols + term.curX;
    nighterm_render_char(term.curY, term.curX, term.buffer[bufferIndex]);
}

void nighterm_do_curinv()
{

    if (term.draw_cursor)
    {
        uint8_t tmp_r = 0;
        uint8_t tmp_g = 0;
        uint8_t tmp_b = 0;

        tmp_r = bg_r;
        tmp_g = bg_g;
        tmp_b = bg_b;

        bg_r = fg_r;
        bg_g = fg_g;
        bg_b = fg_b;

        fg_r = tmp_r;
        fg_g = tmp_g;
        fg_b = tmp_b;

        nighterm_redraw();

        tmp_r = bg_r;
        tmp_g = bg_g;
        tmp_b = bg_b;

        bg_r = fg_r;
        bg_g = fg_g;
        bg_b = fg_b;

        fg_r = tmp_r;
        fg_g = tmp_g;
        fg_b = tmp_b;
    }
}