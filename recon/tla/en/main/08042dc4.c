#include "types.h"
#include "ram_buffer.h"
#include "mem.h"

s32 Func_080cc67c(s32);
s32 Func_080cc7c4(void);
void UiWork_Finalize(void *, s32);
extern const u8 Data_0805f5ec[];

void Func_08042dc4(s32 step)
{
    u8 *save = Ram_HeapSlots->serial_work;
    u32 row;
    const u8 *src;
    u16 *dst;
    s32 i;

    /* FAKEMATCH: the native issues the workspace load first. */
    __asm__("");
    if (*(s16 *)(save + 0x3142) <= 3)
        step <<= 1;
    row = (s16)(*(u16 *)(save + 0x3140) += step);
    if (row > 6)
        row = 6;
    dst = (u16 *)0x06002252;
    src = Data_0805f5ec + row * 12;
    for (i = 11; i >= 0; i--)
        *dst++ = *src++ + 0xf080;
}
