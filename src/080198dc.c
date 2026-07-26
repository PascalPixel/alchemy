#include "types.h"

typedef int bool;

void Func_080198dc(void)
{
    s32 i;
    u32 *word_base;
    u16 *half_base;
    volatile u32 *word_ptr;
    volatile u16 *half_ptr;

    word_base = (u32 *) ((u8 *) *(u32 *)0x03001E8C + 0x12bc);
    half_base = (u16 *) ((u8 *) *(u32 *)0x03001E8C + 0x12dc);
    i = 0;
    do {
        word_ptr = word_base + i;
        half_ptr = half_base + i;
        *word_ptr = 0;
        *half_ptr = 0;
        i++;
    } while (i != 8);
}
