#include <nighterm/nighterm.h>

#include "serial/serial.h"
#include "display/vga.h"
#include "cpu/cpu.h"
#include "utilities/logger.h"
#include "idt/idt.h"

#include "graphics/kif.h"

#include "libc/mem.h"

#define PSF2_MODE

static volatile struct limine_module_request mod_request = {
    .id = LIMINE_MODULE_REQUEST, .revision = 0};

int saved_x;
int saved_y;

void _start(void)
{
    init_display();
    init_nighterm(mod_request.response->modules[0]);
    term.draw_cursor = 0; // Makes sure the cursor isnt being drawn
    log(OK, "Initialized display.");
    log(OK, "Initialized Nighterm.");

    init_idt();
    log(OK, "Initialized IDT.");

    init_physical_memory();
    log(OK, "Initialized Memory Manager");
    
    printf("\n");
    printf("* Loaded %u modules\n", mod_request.response->module_count);
    printf("\n");

    nighterm_set_char_fg(225, 130, 250);
    printf("Welcome to nash!\n\n");
    nighterm_set_char_fg(225, 255, 255);
    draw_image(mod_request.response->modules[1]);

    hlt();
}
