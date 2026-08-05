#include "types.h"

/*
 * resource_39c palette-rotation owner at 0x02002648, complete 60-byte span
 * through its four-word pool. Once every eight frames, rotate the eight
 * BGR555 entries at palette indices 40-47 one place toward the front.
 */

void Func_02002648(void)
{
    unsigned int index;
    u16 *dst;
    u16 *src;
    u32 front;

    if ((*(volatile u32 *)0x03001e40 & 7) != 0) {
        return;
    }

    dst = (u16 *)0x05000050;
    front = *dst;
    index = 0;
    *(u16 *)0x0500005e = front;

    src = (u16 *)0x05000052;
    while (index <= 6) {
        *dst++ = *src++;
        index++;
    }
}
