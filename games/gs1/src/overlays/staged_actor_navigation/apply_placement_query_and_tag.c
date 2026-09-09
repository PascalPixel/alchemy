/* The size test is unsigned, so out20 and out16 are u32. The two rec words
 * that travel on the stack are read into their own locals, which puts both
 * loads before both stores. The +0x23 flag set takes its mask from a local
 * assigned first, which fixes the register the mask lands in. */
#include "types.h"

/*
 * Apply a placement query and tag the object. Func_02001cc4 fills out20 and
 * out16, a 24-byte record of which only rec[2] and rec[4] are read back, and
 * two further out-params whose written values are never read -- only the
 * pointers matter. On success the object is tagged at +0x23 and one of two
 * near-identical finishing calls runs; on failure the object is untouched.
 */

extern u8 *Func_020039ec();
extern s32 Func_02001cc4();
extern void Func_020039c8();
extern void Func_02003970();
extern void Func_020039d8();
extern void Func_020039ee();
s32 SceneActor_ApplyPlacementQueryAndTag(u8 *no)
{
    u8 *obj = Func_020039ec(no);
    u32 out20, out16;
    s32 out12, out8;
    s32 rec[6];
    s32 r2, r4;
    u8 mask;

    if (Func_02001cc4(no, &out20, &out16, rec, &out12, &out8) == 0) {
        return 0;
    }

    r2 = rec[2];
    r4 = rec[4];
    Func_020039c8(2, 2, out20, out16, r2, r4);

    Func_02003970(obj, 4);
    mask = 2;
    obj[0x23] = obj[0x23] | mask;

    if (out20 > out16) {
        Func_020039d8(70, 40, rec[2] + 32, rec[4] + 2, out20, out16);
    } else {
        Func_020039ee(68, 40, rec[2] + 32, rec[4] + 2, out20, out16);
    }

    return 1;
}
