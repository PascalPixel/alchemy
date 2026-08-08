#include "types.h"

/*
 * resource_3bb owner at 0x02003344, 276 bytes: fetch a flag/count via a
 * fixed id (also this owner's return value), look up an object by the
 * shared "current slot" index, compute a position delta (clamped to a
 * fixed range) and a direction-selected offset gated by that flag, seed
 * the object's fields from them, wait out any pending animation on it,
 * then run one of two id-specific setup pairs (selected by the same
 * flag) before a shared closing sequence.
 *
 * Complete owner: `push {r5, r6, r7, lr}` + the r8/sl spill at
 * 0x02003344 through `pop {r3, r5} / mov r8,r3 / mov sl,r5 / pop
 * {r5, r6, r7} / pop {r1} / bx r1` at 0x0200342e-0x02003438 -- the
 * popped word goes to r1, not r0, so r0 (the flag/count from the first
 * call) IS the result, matching this project's documented real-return-
 * value convention (resource_3cb_c_02000580.c). The eight-word literal
 * pool 0x0200343c-0x02003457 follows; the next owner's prologue is at
 * 0x02003458. Two arguments (r0, r1), `s32` return.
 *
 * Not found by the structural inventory walk (unindexed): reached only by
 * `bl`, resolved with `cargo run --release --manifest-path tools/overlay-call-targets/Cargo.toml --`'s `+2` rule.
 *
 * `current = *Data_03001f3c` is a single-dereference global, the same
 * shape resource_3bb_c_02002d84.c's `Data_03001e68` documents at a
 * different address. `idx` is the shared cell
 * resource_3bb_c_02001df4.c/02001898.c already use.
 *
 * The `while (*status != 0) Func_0200717e(1);` loop is a plain
 * top-tested wait, not the "jump to test first" shape this overlay's
 * other spin-waits use.
 *
 * Uncertainty: none of the fourteen callees are identified beyond call
 * shape; `Func_08015120` and `Func_0808a080` are each called repeatedly with
 * different arities/arguments at two sites and are declared old-style
 * rather than miscredited either shape. `current`'s fields (0xe2/0xe4
 * halfword, 0xe8/0xec word) and `object`'s fields (8, 0x30, 0x34, 0x64)
 * are inferred only from this call shape.
 */

extern u8 *Data_03001f3c;

extern s32 Func_080770c0(s32 id);
extern u8 *Func_0808a080(s32 arg0);
extern void Func_08009150(u8 *object, s32 arg1, s32 arg2, s32 arg3);
extern void Func_080770c8(s32 arg0);
extern void Func_08009098(u8 *object, void *table);
extern s32 Func_080000c0(s32 arg0);
extern void Func_02002380(s32 arg0, s32 arg1);
extern void Func_08015120(s32 arg0, s32 arg1);
extern void Func_08015040(s32 arg0, s32 arg1);
extern void Func_08009148(u8 *object);

s32 Func_02003344(s32 arg0, s32 arg1)
{
    u8 *current = Data_03001f3c;
    s32 flag = Func_080770c0(0x211);
    u32 idx = *(u32 *)0x02000434;
    u8 *object = (u8 *)Func_0808a080((s32)idx);
    s32 posDelta;
    s32 dirOffset;
    u16 *field64;
    s16 status;

    posDelta = *(s32 *)(current + 0xe8) + 0xc0000;
    if (*(s32 *)(current + 0xe8) < *(s32 *)(object + 8)) {
        posDelta = *(s32 *)(current + 0xe8) - 0xc0000;
    }

    if (flag != 0) {
        dirOffset = *(s32 *)(current + 0xec) + 0x100000;
        field64 = (u16 *)(current + 0xe4);
    } else {
        dirOffset = *(s32 *)(current + 0xec) - 0x100000;
        field64 = (u16 *)(current + 0xe2);
    }

    *(u16 *)(object + 0x64) = *field64;
    *(s32 *)(object + 0x30) = 0x10000;
    *(s32 *)(object + 0x34) = 0x4000;

    Func_08009150(object, posDelta, 0, dirOffset);
    Func_080770c8(0x211);
    Func_08009098(object, (void *)0x0200cb9c);

    while (status != 0) {
        Func_080000c0(1);
        status = *(s16 *)(object + 0x64);
    }
    status = *(s16 *)(object + 0x64);

    if (flag == 0) {
        Func_02002380(0, arg1);
        Func_08015120(arg0, 2);
    } else {
        Func_02002380(0, arg1);
        Func_08015120(arg1, 2);
    }

    Func_08015120((s32)idx, 1);
    Func_08015040(0x96a, 3);
    Func_08009148(object);

    return flag;
}
