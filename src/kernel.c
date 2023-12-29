#include "serial/serial.h"
#include "display/vga.h"
#include "cpu/cpu.h"
#include "idt/idt.h"


#include "libc/mem.h"
#include "drivers/fs/tmpfs.h"
#include "drivers/speaker_pc/speaker.h"
#include <utilities/systerm.h>
#include <utilities/printf.h>

#define PSF2_MODE

static volatile struct limine_module_request mod_request = {
    .id = LIMINE_MODULE_REQUEST, .revision = 0};

int saved_x;
int saved_y;

void _start(void)
{
    init_display();
    initsysterm(mod_request.response->modules[0]);
    printf("Initialized display\n");

    init_idt();
    printf("Initialized IDT\n");
    
    init_physical_memory();
    printf("Initialized Memory Manager\n");
    

    printf("\n");
    printf("* Loaded %u modules\n", mod_request.response->module_count);
    printf("\n");

    filesystem_setup(mod_request.response->modules[3]);


    hlt();
}
