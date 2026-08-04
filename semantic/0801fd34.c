#include "types.h"

s32 Func_08002322(s32 value);

void Func_0801fd34(void) {
    s32 *source;
    volatile u16 *destination;
    s32 index;
    s32 value;
    s32 component;
    s32 low;
    s32 middle;

    source = (s32 *)0x03001800;
    destination = (u16 *)0x050001D0;
    index = 0;
    do {
        value = Func_08002322((*source + index * 8) * 0x300);
        if (value < 0)
            value += 0x3FFF;
        component = value >> 14;
        low = component << 1;
        middle = component;
        low += 22;
        middle += 16;
        component += 20;
        *destination++ = (component << 10) | (middle << 5) | low;
        index++;
    } while (index <= 3);
}
