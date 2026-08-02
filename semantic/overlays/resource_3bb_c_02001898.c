typedef unsigned char u8;
typedef unsigned short u16;
typedef unsigned int u32;
typedef int s32;

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

extern u8 *Func_0808a080(s32 arg0);
extern void Func_0808a100(s32 arg0, s32 arg1);
extern void Func_0808a010(s32 arg0);
extern void Func_080f9010(s32 arg0);
extern void Func_08009080(u8 *object, s32 arg1);
extern void Func_08009150(u8 *object, s32 arg1, s32 arg2, s32 arg3);
extern void Func_0808a090(s32 arg0, s32 arg1, s32 arg2);
extern void Func_08009158(u8 *object);

void Func_02001898(s32 arg0, s32 arg1, s32 arg2)
{
    u32 idx = *(u32 *)0x02000434;
    u8 *objectA = Func_0808a080(idx);
    u8 *objectB = Func_0808a080(arg0);
    s32 half;
    s32 stackHalf = 0;
    s32 shiftedArg1 = arg1 << 16;
    s32 shiftedArg2 = arg2 << 16;

    if ((*(s32 *)(objectB + 8) >> 20) != arg1 / 2) {
        half = (shiftedArg1 - *(s32 *)(objectB + 8)) / 2;
    } else {
        half = 0;
        stackHalf = (shiftedArg2 - *(s32 *)(objectB + 16)) / 2;
    }

    Func_0808a100((s32)idx, 8);
    Func_0808a010(6);

    *(s32 *)(objectB + 0x30) = 0x8000;
    *(s32 *)(objectB + 0x34) = 0x3333;

    Func_080f9010(239);
    Func_08009080(objectB, 3);
    Func_08009150(objectB, shiftedArg1, 0, shiftedArg2);
    Func_0808a010(6);
    Func_0808a100((s32)idx, 2);
    Func_0808a090((s32)idx, 0x8000, half);
    Func_08009080(objectA, 2);

    Func_08009150(objectA, *(s32 *)(objectA + 8) + half, 0, *(s32 *)(objectA + 16) + stackHalf);
    Func_08009158(objectA);
    Func_08009080(objectA, 1);

    Func_08009158(objectB);
    Func_08009080(objectB, 1);

    Func_080f9010(288);
    Func_080f9010(213);
    Func_0808a010(15);
}
