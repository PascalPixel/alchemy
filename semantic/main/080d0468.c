/*
 * Correctness fix, veneer audit (2026-08-01).
 *
 * `Func_080072f0` is not a function.  0x080072e4 begins the GCC
 * `__call_via_rN` veneer bank -- fifteen four-byte `bx rN; nop` entries,
 * r0..lr, ending at 0x08007320 -- so 0x080072f0 is `__call_via_r3` and
 * `bl 0x80072f0` calls whatever r3 holds.
 *
 * At every site in this file the ROM loads r3 from the literal pool with
 * the constant 0x03001388, so the callee is the relocated IWRAM word copy
 * at that address.  Its signature is not guessed: the EXACT source
 * src/080d40ec.c declares it as
 * `void *(*)(void *destination, const void *source, s32 size)` and
 * src/080e0524.c casts the same address to the same shape.
 *
 * Note what the previous draft had already half-seen: it passed
 * 0x03001388 as a fourth ARGUMENT.  That value was never an argument --
 * it is the callee, and the register load that produced it is the call
 * target, not a parameter.
 */
#include "types.h"

typedef void *(*WordCopy)(void *destination, const void *source, s32 size);
extern u8 *Data_03001ef4;

s32 Func_08004458(void);
void Func_080030f8(s32);

void Func_080d0468(void)
{
    u8 *offsets;
    s32 step;
    s32 extent;
    s32 pass;

    {
        volatile u16 *palette = (volatile u16 *)0x05000040;
        s32 index = 0;
        do {
            s32 half = index / 2;
            *palette++ = (index << 10) | (half << 5) | half;
            index++;
        } while (index != 32);
    }

    offsets = Data_03001ef4;
    step = 16;
    extent = 0;
    ((WordCopy)0x03001388)((void *)0x02010000, (void *)0x06008000,
                  0x7800);

    {
        s32 index = 0;
        do {
            offsets[index] = Func_08004458() & 0x3f;
            index++;
        } while (index != 256);
    }

    pass = 0;
    do {
        s32 row;

        extent += step / 4;
        step++;

        row = 0;
        while (row != extent) {
            s32 column = 0;

            do {
                s32 y = row - offsets[column];

                if (y >= 0 && y <= 119) {
                    u8 *pixel;
                    s16 color;
                    s32 red;
                    s32 green;
                    s32 blue;
                    s32 maximum;

                    pixel = (u8 *)(0x02010000
                        + (column / 8) * 64
                        + (column & 7)
                        + (y / 8) * 2048
                        + (y & 7) * 8);
                    color = *(volatile s16 *)(0x05000000 + *pixel * 2);
                    red = color & 0x1f;
                    green = (color >> 5) & 0x1f;
                    blue = (color >> 10) & 0x1f;
                    maximum = red;
                    if (maximum < green)
                        maximum = green;
                    if (maximum < blue)
                        maximum = blue;
                    *pixel = 63 - maximum;
                }
                column++;
            } while (column != 256);
            row++;
        }

        ((WordCopy)0x03001388)((void *)0x06008000, (void *)0x02010000,
                      0x7800);
        Func_080030f8(1);
        if (extent > 248)
            break;
        pass++;
    } while (pass != 27);

    {
        volatile u16 *palette = (volatile u16 *)0x050000c0;
        s32 index = 0;
        do {
            s32 half = index / 2;
            *palette++ = (index << 10) | (half << 5) | half;
            index++;
        } while (index != 32);
    }

    {
        u8 *pixels = (u8 *)0x02010000;
        s32 index = 0;
        do {
            pixels[index] += 64;
            index++;
        } while (index != 0x7800);
    }

    ((WordCopy)0x03001388)((void *)0x02010000, (void *)0x06008000,
                  0x7800);
    Func_080030f8(1);
}
