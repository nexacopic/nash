
#include "display/vga.h" // systerm is a very dumbed down version of nighterm ( not supporting as much )
#include "systerm.h"
#include "libc/mem.h"
#include "limine.h"
#include "serial/serial.h"



typedef struct {
	uint8_t magic[4];
	uint32_t version;
	uint32_t headerSize;
	uint32_t flags;
	uint32_t length;
	uint32_t charSize;
	uint32_t height, width;
} __attribute__((packed)) psf2Hdr;

psf2Hdr font;

void *fontdata;

char text_buffer[16384]; // horizontal lines and vertical lines
int cury = 0;
int curx = 0;

int rows = 0;
int cols = 0;


void initsysterm(struct limine_file *lmfont)
{
	char *psf2buf = lmfont->address;
    psf2Hdr hdr = *(psf2Hdr *)lmfont->address;
    psf2buf += hdr.headerSize;

    /*if (PSF_MODE == 2 || PSF_MODE == 1 ? (hdr.magic[0] != PSF_MAGIC0 || hdr.magic[1] != PSF_MAGIC1 || hdr.magic[2] != PSF_MAGIC2 || hdr.magic[3] != PSF_MAGIC3) : 0)
        return 0;
	*/

    font = hdr;
    fontdata = psf2buf;
    rows = (getScreenHeight() / hdr.height);
    cols = (getScreenWidth() / hdr.width);
    return 1;
}

void put_text(const char* txt)
{
	
	int i = 0;
    while (txt[i] != '\0') {
        renderchar(cury, i, txt[i]);
		curx=i;
        i++;
    }
	cury++;
	curx=0;
}

void put_char(char ch)
{
    renderchar(cury, curx, ch);
	curx++;
	if (ch=="\n"){
		cury++;
		curx=0;
	}
	
	
}

void renderchar(int row, int col, char ch)
{
	int rounding = ((font.width % 8) != 0) ^ (font.width == 9);
	uint8_t *glyph = fontdata + ch * font.charSize;

	for (size_t y = 0; y < font.height; y++) {
		for (size_t x = 0; x < font.width; x++) {
			if ((glyph[y * ((font.width / 8) + rounding) + x / 8] >> (7 - x % 8)) & 1) {
				draw_pixel(col * font.width + x, row * font.height + y, 255, 255, 255);
			} else {
				draw_pixel(col * font.width + x, row * font.height + y, 0, 0, 0);
			}
		}
	}
}

