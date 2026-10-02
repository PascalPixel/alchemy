/* 2026-10-01 (wave 1, slice 1): still 300. Plain C with no device comes
   close: u16 start and len, an s16 step variable (pos = start + 1; if
   ((u16)pos >= len) pos = 0; cycle->pos = (u16)pos;) gives the reference's
   step shape and start/len registers (365, 20 off); a while form of the
   second loop then fixes the colour pointer and buffer registers (240).
   Remaining there: the reference shifts start into r1 and copies it to r7
   before the second loop, and keeps the stepped value in r1. */
/* 2026-10-01 (matcher 2): 300 (23 register-only, 3 reordered), every
   instruction in the reference's form. Three things fixed the shape: the
   count is masked once into a local (movs r2, #3; ands r2, r3), the colour
   pointer is taken after the destination, and pos is (s16)start << 16, so
   gcse gives the zero-extension of start for the second loop the same
   shift and copies it (lsls r1, r7, #16; adds r7, r1, #0). The step's test
   assigns pos and reads next through a copy of the sum, which keeps gcse
   from reusing the test's shift at the join (the reference shifts twice:
   lsrs r3, r1, #16 before the compare and after movs r1, #0). Left: global
   allocation. The colour pointer takes r1 (the reference's r4) because the
   DMA's length, local in r4, makes the length shift prefer r4 and the
   pointer outranks the two buffer copies (r1 in the reference); then pos
   takes r6 before the length shift, and start r4. Computing pos after the
   second loop and the control word before it scores 185 on the scorer
   (24 register-only, 1 reordered), the same allocation problem; 520,000
   permuter candidates from 335, 185 and 300 found nothing lower.
   2026-10-02: fresh baseline 300. Binding only the color cursor to r4 gives
   220 (18 register-only, 2 reordered), retained below. Binding start to r7
   gives 1110, for either signed or unsigned halfword spelling; binding the
   stepped position to r7 gives 110 but overlaps the live cycle pointer,
   so is rejected rather than treated as an improvement. Also binding the
   cycle pointer to r5 gives 620. An explicit shifted length constrained
   to r6 gives 1795. Those additional devices were all discarded. */
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
    u16 count = *(u16 *)(work + 176) & 3;
    u16 buf[16];

    for (i = 0; i < count; i++) {
        struct PaletteCycle *cycle = (struct PaletteCycle *)(work + i * 44);

        if (cycle->timer == 0) {
            u16 start = cycle->pos;
            u16 len = cycle->len;
            void *dest = cycle->dest;
            /* FAKEMATCH: 520,000 ordinary permutations still exchange r1/r4 for the color cursor and buffer; bind the cursor to r4. */
            register u16 *src asm("r4") = cycle->colors;
            u8 j;
            u32 pos;
            u32 next;
            u32 value;
            for (j = len - start; j < len; j++)
                buf[j] = *src++;
            pos = (s16)start << 16;
            for (j = 0; j < len - start; j++)
                buf[j] = *src++;
            Dma_Set(buf, dest, 0x80000000 | len, (volatile u32 *)0x040000d4);
            /* FAKEMATCH: the copy of the sum and the test assigning pos keep
               gcse from sharing the test's shift with the store. */
            value = pos + 0x10000;
            next = value;
            if ((pos = next >> 16) >= len)
                next = 0;
            value = next >> 16;
            cycle->pos = value;
            cycle->timer = cycle->delay;
        } else {
            cycle->timer--;
        }
    }
}
