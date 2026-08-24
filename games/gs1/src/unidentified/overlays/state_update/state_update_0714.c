/*
 * resource_3a8 owner at 0x0200158c, 40 bytes.
 *
 * THE MINIMAL REPRODUCER for the constant-rematerialisation class, kept here
 * because the whole 40-byte residual IS that one behaviour and nothing else.
 *
 * 512 reaches two calls. The reference materialises it at EACH site --
 * `movs r0, #128 ; lsls r0, #2` twice -- and so needs only `push {lr}`. We hoist
 * it into r5 once, pay `push {r5, lr}` for a callee-saved register, and copy it
 * out with `adds r0, r5, #0` at each site. That is 12 wrong instructions out of a
 * 20-halfword function: everything.
 *
 * The source has no purchase on it. Five spellings compile to BYTE-IDENTICAL
 * output, 14 differing halfwords every time: the literal twice, `0x200` twice,
 * the second callee taking u16, both callees unprototyped, and a hand-shared
 * `s32 kind = 512` local. That last one matters, because sharing a temporary by
 * hand is the anti-pattern the guide names -- here it changes nothing, so the
 * sharing is gcc's and not the source's.
 *
 * Population: 59 of the 121 size-exact `wrong` overlay rows, 24,088 bytes, carry
 * this signature -- more register copies on our side than the reference's, and
 * fewer constant materialisations. It is the largest single group in that pile.
 * resource_38b:0240 is the same mechanism reached from the other direction: a
 * `u8` parameter there was truncating 0x301 to 1, and widening it to the correct
 * type made the count WORSE precisely because gcc then had a repeated constant to
 * share.
 *
 * So `wrong` is the right verdict in the narrow sense -- the instructions really
 * do differ, it is not a rename or a reorder -- and the wrong one in the sense a
 * reader wants. Anyone ranking by `wrongInstructions` should expect this shape
 * near the top and skip it.
 */
#include "types.h"

extern s32 *Data_03001ee0;
extern s32 Func_02005168(s32);
extern void Func_0200518c(s32);

void Func_0200158c(void) {
    if (Func_02005168(512) != 0) {
        Data_03001ee0[6] = 0;
        Func_0200518c(512);
    }
}
