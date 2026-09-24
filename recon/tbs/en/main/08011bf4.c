/* Draft, not exact (2026-09-24): 55 differing halfwords, 236 of 236 bytes.
   Residual: register allocation (cycle pointer r6 vs r5, source pointer
   r1 vs r4) and the count mask order; the start<<16 copy is missing. */

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

extern u8 *Data_03001ec0;

void Func_08011bf4(void)
{
    u8 *work = Data_03001ec0;
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

            for (j = len - start; j < len; j++)
                buf[j] = *src++;
            for (j = 0; j < len - start; j++)
                do { buf[j] = *src++; } while (0);
            pos = start + 1;
            Dma_Set(buf, dest, 0x80000000 | len, (volatile u32 *)0x040000d4);
            if (pos >= len)
                pos = 0;
            cycle->pos = pos;
            cycle->timer = cycle->delay;
        } else {
            cycle->timer--;
        }
    }
}
