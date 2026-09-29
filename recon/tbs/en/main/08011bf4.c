/* Draft, not exact (2026-09-24): 55 differing halfwords, 236 of 236 bytes.
   Residual: register allocation (cycle pointer r6 vs r5, source pointer
   r1 vs r4) and the count mask order; the start<<16 copy is missing.
   2026-09-29: the work is gPaletteWork (0x03001ec0 had only its address
   name). alchemy permute took the score from 1105 to 625 (8 register-only,
   3 operand, 2 reordered, 1 inserted, 3 deleted) over 35,753 candidates:
   the wrap test assigns pos inside its condition after the DMA and
   compares against a u16 copy of len (the permuter's temporary; 740
   without it), and a second run of 29,088 found nothing lower. Remaining:
   the flags mask order, the start<<16 copy made through r1, and len
   extracted once into r4 for both the DMA count and the wrap compare,
   where this build compares the shifted halfwords directly. */

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
            u16 pos;
            u16 tmp;

            for (j = len - start; j < len; j++)
                buf[j] = *src++;
            for (j = 0; j < len - start; j++)
                do { buf[j] = *src++; } while (0);
            Dma_Set(buf, dest, 0x80000000 | len, (volatile u32 *)0x040000d4);
            tmp = len; /* FAKEMATCH: a halfword copy of len reallocates the wrap test. */
            if ((pos = start + 1) >= tmp)
                pos = 0;
            cycle->pos = pos;
            cycle->timer = cycle->delay;
        } else {
            cycle->timer--;
        }
    }
}
