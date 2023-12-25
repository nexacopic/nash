#include <nighterm/nighterm.h>

#include "serial/serial.h"
#include "display/vga.h"
#include "cpu/cpu.h"

#define PSF2_MODE

static volatile struct limine_module_request mod_request = {
    .id = LIMINE_MODULE_REQUEST, .revision = 0};

void _start(void) {
    init_display();
    init_nighterm(mod_request.response->modules[0]);

    term.draw_cursor = 0; // Makes sure the cursor isnt being drawn

    nighterm_set_char_fg(225, 130, 250);
    printf("Welcome to nash!");
    
    hlt();
}