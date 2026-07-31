#include "types.h"

/*
 * Core-drive row 0x080069a4, 92 bytes (kind nonstandard_thumb_branch_module,
 * admitted per Vale's 2026-07-31 ruling; body read confirms two ordinary
 * `bx lr` leaves, no register trick -- the classification is a byte-shape
 * artifact, not a cannot-express case).
 *
 * Two functions in asm/executable_gaps/080069a4.s, sharing a small pool
 * (pool words resolved against the compiled out/asm/080069a4.bin rather
 * than by hand, since two `ldr rN,[pc,#24]` at different addresses land on
 * different words despite the identical immediate).
 *
 * Func_080069a4(void): the tick handler. If Data_02004c22 (a u16 countdown)
 * is nonzero, decrement it; if the decrement just reached zero, set the
 * flag byte Data_02004c24 to 1.
 *
 * Func_080069c8(timerIndex, callback): the installer. Rejects timerIndex
 * outside 0-3 (returns 1). Otherwise stores timerIndex to Data_02004c20,
 * computes the matching GBA hardware timer count/reload register address
 * (0x04000100 + timerIndex * 4 -- the four TMxCNT_L registers are spaced 4
 * bytes apart starting there) into Data_02004c28, writes Func_080069a4's
 * own address through the caller-supplied `callback` out-pointer, and
 * returns 0.
 *
 * Complete owner: both functions in full, entry to `bx lr`; the described
 * pool words, no further exclusions.
 */

extern u16 Data_02004c22;
extern u8 Data_02004c24;
extern u8 Data_02004c20;
extern u32 Data_02004c28;

void Func_080069a4(void)
{
    if (Data_02004c22 != 0) {
        Data_02004c22--;
        if (Data_02004c22 == 0) {
            Data_02004c24 = 1;
        }
    }
}

s32 Func_080069c8(u8 timerIndex, void (**callback)(void))
{
    if (timerIndex > 3) {
        return 1;
    }

    Data_02004c20 = timerIndex;
    Data_02004c28 = 0x04000100 + (u32)timerIndex * 4;
    *callback = Func_080069a4;

    return 0;
}
