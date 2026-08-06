#include "types.h"

/*
 * resource_3bb owner at 0x02001898, 292 bytes: look up two objects (one
 * by the shared "current slot" index, one by `arg0`), compare a
 * fixed-point field against `arg1 / 2` and, depending on whether they
 * match, compute either a delta half or a position half to carry
 * forward, then run a long, mostly linear setup sequence (about
 * eighteen calls) combining the two objects' fields with that half and
 * fixed ids.
 *
 * Complete owner: `push {r5, r6, r7, lr}` + the r8/r9/sl/fp spill at
 * 0x02001898 through `pop {r3, r5, r6, r7} / mov r8,r3 / mov r9,r5 /
 * mov sl,r6 / mov fp,r7 / pop {r5, r6, r7} / pop {r0} / bx r0` at
 * 0x020019a4-0x020019b2; the two-word literal pool 0x020019b4-0x020019bb
 * follows; the next owner's prologue is at 0x020019bc. Three arguments
 * (r0, r1, r2), void.
 *
 * Not found by the structural inventory walk (unindexed): reached only by
 * `bl`, resolved with `bun tools/overlay_call_targets.ts`'s `+2` rule.
 *
 * `idx = *(u32*)0x02000434` is the same shared "current slot" index cell
 * resource_3bb_c_02001df4.c and this overlay's other siblings use. The
 * `(x + ((unsigned)x >> 31)) >> 1` shape read twice here is the
 * standard compiler lowering of plain signed `x / 2`, not a rounding
 * idiom -- written directly as `/ 2` below.
 *
 * Uncertainty: none of the eighteen callees are identified beyond call
 * shape; `objectA`/`objectB`'s fields (8, 16, 0x30, 0x34) are inferred
 * only from this call shape.
 */










extern u8 * Func_0200580e(s32 arg0);
extern u8 * Func_02005816(s32 arg0);
extern void Func_020058a8(s32 arg0, s32 arg1);
extern void Func_02005846(s32 arg0);
extern void Func_020059d6(s32 arg0);
extern void Func_02005776(u8 *object, s32 arg1);
extern void Func_020057b2(u8 *object, s32 arg1, s32 arg2, s32 arg3);
extern void Func_02005870(s32 arg0);
extern void Func_020058e0(s32 arg0, s32 arg1);
extern void Func_020058b4(s32 arg0, s32 arg1, s32 arg2);
extern void Func_020057a4(u8 *object, s32 arg1);
extern void Func_020057e8(u8 *object, s32 arg1, s32 arg2, s32 arg3);
extern void Func_020057f6(u8 *object);
extern void Func_020057c6(u8 *object, s32 arg1);
extern void Func_02005804(u8 *object);
extern void Func_020057d4(u8 *object, s32 arg1);
extern void Func_02005a44(s32 arg0);
extern void Func_02005a4a(s32 arg0);
extern void Func_020058d0(s32 arg0);
void Func_02001898(s32 arg0, s32 arg1, s32 arg2)
{
    u32 idx = *(u32 *)0x02000434;
    u8 *objectA = Func_0200580e(idx);
    u8 *objectB = Func_02005816(arg0);
    s32 half;
    s32 stackHalf = 0;
    s32 shiftedArg1 = arg1 << 16;
    s32 shiftedArg2 = arg2 << 16;

    if ((*(s32 *)(objectB + 8) >> 20) != arg1 / 2) {
        half = (shiftedArg1 - *(s32 *)(objectB + 8)) / 2;
        stackHalf = (shiftedArg2 - *(s32 *)(objectB + 16)) / 2;
    } else {
        half = 0;
    }

    Func_020058a8((s32)idx, 8);
    Func_02005846(6);

    *(s32 *)(objectB + 0x30) = 0x8000;
    *(s32 *)(objectB + 0x34) = 0x3333;

    Func_020059d6(239);
    Func_02005776(objectB, 3);
    Func_020057b2(objectB, shiftedArg1, 0, shiftedArg2);
    Func_02005870(6);
    Func_020058e0((s32)idx, 2);
    Func_020058b4((s32)idx, 0x8000, half);
    Func_020057a4(objectA, 2);

    Func_020057e8(objectA, *(s32 *)(objectA + 8) + half, 0, *(s32 *)(objectA + 16) + stackHalf);
    Func_020057f6(objectA);
    Func_020057c6(objectA, 1);

    Func_02005804(objectB);
    Func_020057d4(objectB, 1);

    Func_02005a44(288);
    Func_02005a4a(213);
    Func_020058d0(15);
}
