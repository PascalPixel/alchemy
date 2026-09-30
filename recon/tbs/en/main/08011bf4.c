/* 2026-09-30 (Mercury): 30 differing halfwords, 236 of 236 bytes, no
   FAKEMATCH (was 65). The position steps in 16.16: pos = start << 16 before
   the second copy loop, next = pos + 0x10000, next = 0 when it reaches len
   << 16, and the store takes next >> 16 after the join, as the reference
   does (movs r1, #0 then lsrs r3, r1, #16). Comparing (next >> 16) >= len
   instead lets gcse PRE reuse the compare's shift at the join (59). Left:
   the reference turns the test into lsrs r3, r1, #16; cmp r3, r4 (len)
   where this build shifts len left; it forms start << 16 first (lsls r1,
   r7, #16; adds r7, r1, #0) and takes the second loop's start from it,
   where this build zero-extends start and shifts it back; and the colour
   pointer and the buffer base swap r1/r4. */
#include "TYPES.H"
#include "DMA.H"

struct PaletteCycle {
    void *dest;
    s16 pos;
    u16 timer;
    u16 delay;
    s16 len;
    u16 colors[16];
};

extern u8 *gPaletteWork;

void Func_08011bf4(void)
{
    u8 *work = gPaletteWork;
    u8 i;
    u16 flags = *(u16 *)(work + 176);
    u16 buf[16];

    for (i = 0; i < (flags & 3); i++) {
        struct PaletteCycle *cycle = (struct PaletteCycle *)(work + i * 44);

        if (cycle->timer == 0) {
            u16 start = cycle->pos;
            u16 len = cycle->len;
            void *dest = cycle->dest;
            u16 *src = cycle->colors;
            u8 j;
            u32 pos;
            u32 next;

            for (j = len - start; j < len; j++)
                buf[j] = *src++;
            pos = start << 16;
            for (j = 0; j < len - start; j++)
                buf[j] = *src++;
            Dma_Set(buf, dest, 0x80000000 | len, (volatile u32 *)0x040000d4);
            next = pos + 0x10000;
            if (next >= len << 16)
                next = 0;
            cycle->pos = next >> 16;
            cycle->timer = cycle->delay;
        } else {
            cycle->timer--;
        }
    }
}
