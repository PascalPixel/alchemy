#include "types.h"

/*
 * Two-pass scene particle pattern selected by `variant`.
 *
 * Each pass visits eight evenly spaced positions and emits on the odd slots.
 * The three variants differ only in their world-space bases and in the two
 * transition rectangles drawn before and after each pass. The four-word
 * descriptor is consumed by this overlay's established general spawner.
 */






extern void Func_02007a2e(s32);
extern void Func_020077e4(s32, s32, s32, s32, s32, s32);
extern void Func_020077f4(s32, s32, s32, s32, s32, s32);
extern void Func_0200780a(s32, s32, s32, s32, s32, s32);
extern void Func_0200781a(s32, s32, s32, s32, s32, s32);
extern void Func_0200782e(s32, s32, s32, s32, s32, s32);
extern void Func_0200783e(s32, s32, s32, s32, s32, s32);
extern s32 Func_02007814(void);
extern s32 Func_02007828(void);
extern void Func_02002000(s32, s32, s32, s32, s32, s32, u32, void *);
extern s32 Func_02007862(void);
extern s32 Func_02007876(void);
extern void Func_02002052(s32, s32, s32, s32, s32, s32, u32, void *);
extern s32 Func_020078b0(void);
extern s32 Func_020078c4(void);
extern void Func_0200209c(s32, s32, s32, s32, s32, s32, u32, void *);
extern void Func_02007a12(s32);
extern void Func_02007998(s32, s32, s32, s32, s32, s32);
extern void Func_020079ac(s32, s32, s32, s32, s32, s32);
extern void Func_020079c4(s32, s32, s32, s32, s32, s32);
extern void Func_020079d6(s32, s32, s32, s32, s32, s32);
extern void Func_020079ee(s32, s32, s32, s32, s32, s32);
extern void Func_02007a04(s32, s32, s32, s32, s32, s32);
void Func_02001db4(s32 variant)
{
    s32 descriptor[4];
    u32 pass;

    Func_02007a2e(0xd3);

    if (variant == 0) {
        Func_020077e4(111, 57, 113, 42, 1, 1);
        Func_020077f4(111, 59, 113, 43, 1, 1);
    } else if (variant == 1) {
        Func_0200780a(113, 58, 112, 46, 1, 1);
        Func_0200781a(115, 58, 113, 46, 1, 1);
    } else {
        Func_0200782e(115, 57, 116, 44, 1, 1);
        Func_0200783e(113, 57, 115, 44, 1, 1);
    }

    descriptor[1] = 7;
    descriptor[2] = 0x8000;
    descriptor[3] = 0x8000;

    for (pass = 0; pass <= 1; pass++) {
        s32 right = 0x02c00000 + (pass << 20);
        s32 left = 0x032c0000 - (pass << 20);
        u32 slot;

        for (slot = 0; slot <= 7; slot++) {
            if ((slot & 1) != 0) {
                if (variant == 0) {
                    u32 randomX = (u32)(Func_02007814() << 3) >> 16;
                    u32 randomY = (u32)(Func_02007828() << 3) >> 16;
                    Func_02002000(
                        (s32)(randomX * 0x3333) - 0xcccc,
                        variant, (s32)(randomY * 0x3333) - 0xcccc,
                        0x03180000, 0, right,
                        0x00090000, descriptor);
                } else if (variant == 1) {
                    u32 randomX = (u32)(Func_02007862() << 3) >> 16;
                    u32 randomY = (u32)(Func_02007876() << 3) >> 16;
                    Func_02002052(
                        0, (s32)(randomY * 0x3333) - 0xcccc,
                        right + 0x00600000, 0, 0x02ea0000,
                        (s32)(randomX * 0x3333) - 0xcccc,
                        0x00090000, descriptor);
                } else {
                    u32 randomX = (u32)(Func_020078b0() << 3) >> 16;
                    u32 randomY = (u32)(Func_020078c4() << 3) >> 16;
                    Func_0200209c(
                        left, 0, 0x02ca0000,
                        (s32)(randomX * 0x3333) - 0xcccc,
                        0, (s32)(randomY * 0x3333) - 0xcccc,
                        0x00090000, descriptor);
                }
                Func_02007a12(1);
            }
            left -= 0x10000;
            right += 0x10000;
        }

        if (variant == 0) {
            Func_02007998(111, 58, 113, 43 + pass, 1, 1);
            Func_020079ac(111, 59, 113, 44 + pass, 1, 1);
        } else if (variant == 1) {
            Func_020079c4(114, 58, 113 + pass, 46, 1, 1);
            Func_020079d6(115, 58, 114 + pass, 46, 1, 1);
        } else {
            Func_020079ee(114, 57, 115 - pass, 44, 1, 1);
            Func_02007a04(113, 57, 114 - pass, 44, 1, 1);
        }
    }
}
