#include "types.h"

/*
 * resource_39c three-row particle curtain at 0x020018f8
 * (192-byte whole-owner span, 6 static call sites).
 *
 * The high-register/frame body ends at 0x020019a8.  Alignment and the three
 * referenced pool words (-0x20000, -0x4000, and script 0x02009791) extend the
 * owner to the next prologue at 0x020019b8.  The outer loop draws three screen
 * rows; the four odd values from 1 through 7 emit jittered particles on each
 * row.  Random calls are deliberately separate from the spawn call so source
 * order remains the same as the Thumb call order.
 */

typedef struct ParticleOptions {
    s32 callerReserved;
    s32 count;
    s32 scaleStart;
    s32 scaleEnd;
} ParticleOptions;







extern void Func_02007316(s32 left, s32 top, s32 right, s32 bottom,
                          s32 style, s32 enabled);
extern u32 Func_020072ce(void);
extern void Func_02001aa6(s32 x, s32 y, s32 z, s32 kind,
                          s32 velocity, s32 unused, s32 duration,
                          ParticleOptions *options);
extern void Func_0200741c(s32 frames);
extern void Func_02007394(s32 left, s32 top, s32 right, s32 bottom,
                          s32 style, s32 enabled);
extern void Func_02007326(const void *script);
void Func_020018f8(void)
{
    ParticleOptions options;
    u32 row;
    s32 pattern;
    s32 rowBase = -0x20000;

    Func_02007316(78, 58, 110, 36, 1, 1);

    options.scaleStart = 0x8000;
    options.scaleEnd = 0x8000;
    options.count = 5;

    for (row = 0; row < 3; row++) {
        for (pattern = 1; pattern <= 7; pattern++) {
            if ((pattern & 1) != 0) {
                u32 sample = Func_020072ce();
                s32 z = 584 - (s32)((sample * 5u) >> 16);
                s32 x = rowBase - row * 0x80000 + 182 * 0x40000;

                Func_02001aa6(x, 0, z * 0x10000, -0x4000,
                              0, 0, 0x90000, &options);
                Func_0200741c(1);
            }
        }

        Func_02007394(111, 35, 109 - row, 36, 1, 1);
        rowBase -= 0x20000;
    }

    Func_02007326((const void *)0x02009791);
}
