#include "types.h"

/* Rotate seven adjacent palette entries once every eight frames. */
void Func_02002dcc(void)
{
    unsigned int index;
    volatile u16 *dst;
    volatile u16 *slot6;
    volatile u16 *src;

    if ((*(volatile u32 *)0x03001e40 & 7) != 0) {
        return;
    }

    dst = (volatile u16 *)0x050000c2;
    slot6 = (volatile u16 *)0x050000ce;
    *slot6 = *dst;

    src = (volatile u16 *)0x050000c4;
    for (index = 0; index <= 5; index++) {
        *dst++ = *src++;
    }
}
