
#include "speaker.h"
#include "serial/serial.h"

void speaker_play(uint32_t hz) {
    uint32_t d = 1193180 / hz;
    outb16(0x43, 0xB6);
    //outportb(0x42, (u8) (d & 0xFF));
    //outportb(0x42, (u8) ((d >> 8) & 0xFF));
    outb16(0x42, 140);
    outb16(0x42, 140);

    uint8_t t = inb8(0x61);
    if (t != (t | 0x3)) {
        outb16(0x61, t | 0x3);
    }
}