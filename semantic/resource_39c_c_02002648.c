#include "types.h"

/*
 * resource_39c palette-rotation owner at 0x02002648, complete 60-byte span
 * through its four-word pool. Once every eight frames, rotate the eight
 * BGR555 entries at palette indices 40-47 one place toward the front.
 */

void Func_02002648(void)
{
    unsigned int index;
    volatile u16 *dst;
    volatile u16 *slot7;
    volatile u16 *src;

    if ((*(volatile u32 *)0x03001e40 & 7) != 0) {
        return;
    }

    dst = (volatile u16 *)0x05000050;
    slot7 = (volatile u16 *)0x0500005e;
    *slot7 = *dst;

    src = (volatile u16 *)0x05000052;
    for (index = 0; index <= 6; index++) {
        *dst++ = *src++;
    }
}
