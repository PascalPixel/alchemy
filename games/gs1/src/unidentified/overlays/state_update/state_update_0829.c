#include "types.h"

/* Rotate seven adjacent palette entries once every eight frames. */
void Func_02002dcc(void)
{
    unsigned int index;
    u16 *dst;
    u16 *src;
    u32 front;

    if ((*(volatile u32 *)0x03001e40 & 7) != 0) {
        return;
    }

    dst = (u16 *)0x050000c2;
    front = *dst;
    index = 0;
    *(u16 *)0x050000ce = front;

    src = (u16 *)0x050000c4;
    while (index <= 5) {
        *dst++ = *src++;
        index++;
    }
}
