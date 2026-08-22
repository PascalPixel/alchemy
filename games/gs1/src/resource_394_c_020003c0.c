#include "types.h"

void Func_020003c0(void)
{
    u32 *destination;
    const u32 *source;
    u16 *preset;

    source = (const u32 *)0x03001ad4;
    destination = (u32 *)0x0200a0d0;
    *destination++ = *source++;
    *destination++ = *source++;
    *destination = *source;
    preset = (u16 *)0x0200a0d0;
    preset[1] += 0xb0;
    preset[3] += 0xb0;
    preset[5] += 0xb0;
}
