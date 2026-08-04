#include "types.h"

/*
 * Two-pass scene particle pattern selected by `variant`.
 *
 * Each pass visits eight evenly spaced positions and emits on the odd slots.
 * The three variants differ only in their world-space bases and in the two
 * transition rectangles drawn before and after each pass. The four-word
 * descriptor is consumed by this overlay's established general spawner.
 */
extern s32 Func_080000f8(void);
extern void Func_08009180(s32, s32, s32, s32, s32, s32);
extern void Func_0808a010(s32);
extern void Func_080f9010(s32);
extern void Func_0200013c(s32, s32, s32, s32, s32, s32, u32, void *);

void Func_02001db4(s32 variant)
{
    s32 descriptor[4];
    s32 pass;

    Func_080f9010(0xd3);

    if (variant == 0) {
        Func_08009180(111, 57, 113, 42, 1, 1);
        Func_08009180(111, 59, 113, 43, 1, 1);
    } else if (variant == 1) {
        Func_08009180(113, 58, 112, 46, 1, 1);
        Func_08009180(115, 58, 113, 46, 1, 1);
    } else {
        Func_08009180(115, 57, 116, 44, 1, 1);
        Func_08009180(113, 57, 115, 44, 1, 1);
    }

    descriptor[1] = 7;
    descriptor[2] = 0x8000;
    descriptor[3] = 0x8000;

    for (pass = 0; pass <= 1; pass++) {
        s32 left = 0x032c0000 - (pass << 20);
        s32 right = 0x02c00000 + (pass << 20);
        s32 slot;

        for (slot = 0; slot <= 7; slot++) {
            if ((slot & 1) != 0) {
                if (variant == 0) {
                    u32 randomX = (u32)(Func_080000f8() << 3) >> 16;
                    u32 randomY = (u32)(Func_080000f8() << 3) >> 16;
                    Func_0200013c(
                        0x03180000, 0, right,
                        (s32)(randomX * 0x3333) - 0xcccc,
                        variant, (s32)(randomY * 0x3333) - 0xcccc,
                        0x00090000, descriptor);
                } else if (variant == 1) {
                    u32 randomX = (u32)(Func_080000f8() << 3) >> 16;
                    u32 randomY = (u32)(Func_080000f8() << 3) >> 16;
                    Func_0200013c(
                        right + 0x00600000, 0, 0x02ea0000,
                        (s32)(randomX * 0x3333) - 0xcccc,
                        0, (s32)(randomY * 0x3333) - 0xcccc,
                        0x00090000, descriptor);
                } else {
                    u32 randomX = (u32)(Func_080000f8() << 3) >> 16;
                    u32 randomY = (u32)(Func_080000f8() << 3) >> 16;
                    Func_0200013c(
                        left, 0, 0x02ca0000,
                        (s32)(randomX * 0x3333) - 0xcccc,
                        0, (s32)(randomY * 0x3333) - 0xcccc,
                        0x00090000, descriptor);
                }
                Func_0808a010(1);
            }
            left -= 0x10000;
            right += 0x10000;
        }

        if (variant == 0) {
            Func_08009180(111, 58, 113, 43 + pass, 1, 1);
            Func_08009180(111, 59, 113, 44 + pass, 1, 1);
        } else if (variant == 1) {
            Func_08009180(114, 58, 113 + pass, 46, 1, 1);
            Func_08009180(115, 58, 114 + pass, 46, 1, 1);
        } else {
            Func_08009180(114, 57, 115 - pass, 44, 1, 1);
            Func_08009180(113, 57, 114 - pass, 44, 1, 1);
        }
    }
}
