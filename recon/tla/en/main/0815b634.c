/* Near miss of Func_0815b634 (listing 39 instructions, draft 38, 21 differ): the native takes the
 * address of the saved half again after the first copy (add r4, sp, #128) where the draft keeps
 * it in r5 across the call; the pointers and the pool word 0x810 agree. */
#include "types.h"
#include "iwram_call.h"

void Func_0815b634(void)
{
    u16 saved[64];
    u16 flipped[64];
    u16 *dst;
    u16 *src;
    s32 i;

    Iwram_CopyWords(saved, (void *)0x05000000, 128);
    i = 1;
    dst = flipped + 1;
    src = saved + 63;
next:
    i++;
    *dst = *src;
    src--;
    dst++;
    if (i != 64)
        goto next;
    flipped[0] = saved[0];
    Iwram_CopyWords((void *)0x05000000, flipped, 128);
    *(u16 *)0x04000052 = 0x810;
}
