#include "idt.h"
#include "utilities/printf.h"
#include "utilities/panic.h"
#include "cpu/cpu.h"

#define IDT_ENTRIES 256

idt_entry_t idt[IDT_ENTRIES];
idt_pointer_t idt_p;
interrupt_handler irq_handlers[16];

extern uint64_t isr_tbl[];

static const char *exception_strings[32] = {
    "division by zero",
    "debug",
    "nonmaskable interrupt",
    "breakpoint",
    "overflow",
    "bound range exceeded",
    "invalid opcode",
    "device not available",
    "double fault",
    "coprocessor segment overrun",
    "invalid tss",
    "segment not present",
    "stack segment fault",
    "general protection fault",
    "page fault",
    "reserved",
    "x87 fpu error"
    "alignment check",
    "machine check",
    "simd exception",
    "virtualization exception",
    "control protection exception",
    "reserved",
    "reserved",
    "reserved",
    "reserved",
    "reserved",
    "reserved",
    "hypervisor injection exception",
    "vmm communication exception",
    "security exception",
    "reserved" };

extern void load_idt(uint64_t);

void set_idt_gate(int num, uint64_t base, uint16_t sel, uint8_t flags) {
    idt[num].offset_low = (base & 0xFFFF);
    idt[num].offset_middle = (base >> 16) & 0xFFFF;
    idt[num].offset_high = (base >> 32) & 0xFFFFFFFF;
    idt[num].selector = sel;
    idt[num].ist = 0;
    idt[num].flags = flags;
    idt[num].zero = 0;
}

void init_idt() {
    idt_p.limit = sizeof(idt_entry_t) * IDT_ENTRIES - 1;
    idt_p.base = (uint64_t)&idt;

    for (size_t i = 0; i < 16; i++) {
		irq_handlers[i] = NULL;
	}

    for (int i = 0; i < IDT_ENTRIES; ++i) {
        set_idt_gate(i, isr_tbl[i], 0x28, 0x8E);
    }

    // TODO: Remap and enable PIC

    load_idt((uint64_t)&idt_p);
}

void excp_handler(int_frame_t frame) {
	if (frame.vector < 0x20) {
        panic(exception_strings[frame.vector], frame);
        hcf();
	} else if (frame.vector >= 0x20 && frame.vector <= 0x2f) {
        // NOTE: This is intentionally left commented out because PIC is not initialized yet.
		int irq = frame.vector - 0x20;

		//if (irq_handlers[irq] != NULL) {
			//irq_handlers[irq]();
		//}

        //pic_send_eoi();
		//apic_send_eoi();

        printf("Tried to call IRQ%d handler, but PIC is not remapped?", irq);
	} else if (frame.vector == 0x80) {
        // handle syscalls here
    }
}

void irq_register(uint8_t irq, interrupt_handler handler)
{
	irq_handlers[irq] = handler;
	printf("registered handler for irq %i", irq);
}

void irq_deregister(uint8_t irq)
{
	irq_handlers[irq] = NULL;
	printf("deregistered handler for irq %i", irq);
}