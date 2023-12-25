//  https://en.wikipedia.org/wiki/Interrupt_vector_table

#include <nighterm/nighterm.h>

#include "error_handler.h"
#include "idt/idt.h"
#include "cpu/cpu.h"


void division() {
    printf("Division by zero!");
    hcf();
}

void register_errors() {
    set_idt_gate(0x0, (uint64_t)&division, 0x28, 0x8E);
}