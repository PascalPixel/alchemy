#include "TYPES.H"

extern s32 Trig_Sin(s32 angle);

/* Cycles the four object palette colours at 0x050001d0 through a sine glow
   driven by the frame phase at 0x03001800. */
void PaletteGlow_UpdateSine(void)
{
    s32 *phase;
    volatile u16 *palette;
    s32 packed;
    s32 value;
    s32 index;

    phase = (s32 *)0x03001800;
    palette = (volatile u16 *)0x050001D0;
    index = 0;
    do {
        value = Trig_Sin((*phase + index * 8) * 0x300);
        if (value < 0) {
            value += 0x3FFF;
        }
        packed = value >> 14;
        {
            u32 low = packed << 1;
            u32 middle = packed;

            low += 22;
            middle += 16;
            packed += 20;
            /* FAKEMATCH: the do-while fixes the shift's place in the schedule. */
            do { packed <<= 10; } while (0);
            middle <<= 5;
            packed |= middle;
            packed |= low;
        }
        /* FAKEMATCH: likewise for the store. */
        do { *palette++ = packed; } while (0);
    } while (++index <= 3);
}
