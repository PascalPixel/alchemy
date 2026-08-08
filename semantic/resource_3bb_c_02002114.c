#include "types.h"

/*
 * resource_3bb owner at 0x02002114, 620 bytes: the largest owner in
 * this overlay's item-28 population. A multi-stage validation/setup
 * state machine: look up an object by handle, short-circuit for
 * mode==3, copy a small per-mode byte table from a fixed source, run
 * two count/content checks (closing early with one of three distinct
 * ids on failure), run three validation passes over the byte table
 * (each calling a different per-byte or per-nonzero-byte callee), get a
 * second id via a parameterless validation call, close on its -1
 * sentinel, then -- on full success -- run a long, mostly linear setup
 * sequence (about twenty-five calls) using the object's two signed
 * fields and the validated ids.
 *
 * Complete owner: `push {r5, r6, r7, lr}` + the r8/r9/sl/fp spill at
 * 0x02002114 through `pop {r3, r5, r6, r7} / mov r8,r3 / mov r9,r5 /
 * mov sl,r6 / mov fp,r7 / pop {r5, r6, r7} / pop {r0} / bx r0` at
 * 0x02002358-0x02002366, followed by the six-word literal pool
 * 0x02002368-0x0200237f; the next owner's prologue is at 0x02002380
 * (this overlay's own resource_3bb_c_02002380.c). Three arguments (r0
 * mode, r1 handle, r2 an out-pointer stashed on the stack), void.
 *
 * Not found by the structural inventory walk (unindexed): reached only
 * by `bl`, resolved with `cargo run --release --manifest-path tools/overlay-call-targets/Cargo.toml --`'s `+2`
 * rule.
 *
 * `Close(id)` below collects the shared two-call close sequence
 * (`Func_020061fa(id); Func_02006212(handle, 0);`) reached from three
 * different points (mode==3 directly; the count/content checks with
 * ids 0x2083/0x2084; and the validation failure/-1-sentinel path with
 * id 0x207e) -- all of them fall through to the same final `return`,
 * which this helper's `return` from the caller models directly.
 *
 * The stack byte table is sized 8 here (the real frame is only 12
 * bytes, 4 of which are the stashed out-pointer, so `count` -- read
 * from `Func_02006054()` -- is implicitly trusted by the compiled
 * owner to stay at most 8; the copy loop below adds an explicit `i < 8`
 * clamp that the real instructions do not have, to avoid undefined
 * behaviour in C if that invariant is ever violated, without changing
 * behaviour for any `count` the real owner actually handles safely).
 *
 * Uncertainty: none of the twenty-nine callees are identified beyond
 * call shape. Several addresses are called more than once with
 * different arities at different sites (`Func_020060ec`,
 * `Func_02006212`, `Func_0200624e`, `Func_0200625c`, `Func_02006286`)
 * and are declared old-style rather than miscredited any one shape, per
 * this project's "arities vary per site" convention
 * (resource_3cb_c_02000580.c). The object's fields 10/18 (signed
 * halfwords) and the fixed source table at 0x02000438 are inferred
 * only from this call shape.
 */

extern u8 *Func_0808a080(s32 handle);
extern s32 Func_08077148(void);
extern s32 Func_080770c0(s32 arg0);
extern void Func_080000c0(s32 arg0);
extern void Func_0808a170(s32 id);
extern void Func_0808a178(s32 handle, s32 arg1);
extern s32 Func_0808a070(s32 arg0, s32 arg1);
extern void Func_08077168(s32 arg0);
extern void Func_08077150(s32 arg0);
extern s32 Func_080a1018(void);
extern void Func_0808a180(s32 handle, s32 arg1);
extern void Func_08015120(s32 arg0, s32 arg1);
extern void Func_0808a090(s32 arg0, s32 arg1, s32 arg2);
extern void Func_0808a0f0(s32 arg0, s32 arg1, s32 arg2);
extern void Func_0808a0d0(s32 arg0, s32 arg1, s32 arg2);
extern void Func_0808a150(s32 arg0, s32 arg1, s32 arg2);
extern void Func_0808a100(s32 arg0, s32 arg1);
extern void Func_0808a110(s32 arg0, s32 arg1);
extern void Func_0808a0c8(s32 arg0, s32 arg1, s32 arg2);
extern void Func_0808a588(s32 arg0, s32 arg1);
extern void Func_0808a1b8(s32 arg0, s32 arg1, s32 arg2);
extern void Func_080770c8(s32 arg0);
extern void Func_080770e8(s32 arg0, s32 arg1);

void Func_02002114(s32 mode, s32 handle, u8 *outPtr)
{
    u8 *object = Func_0808a080(handle);
    s32 fieldA = *(short *)(object + 10);
    s32 fieldB = *(short *)(object + 18);
    signed char bytes[8];
    s32 count;
    s32 i;
    s32 id2;
    s32 closeId;

    if (mode == 3) {
        closeId = 0x207e;
        goto close;
    }

    count = Func_08077148();

    if (count > 0) {
        for (i = 0; i < count; i++) {
            bytes[i] = *(signed char *)(0x02000438 + i);
        }
    }

    if (count <= 1) {
        closeId = 0x2083;
        goto close;
    }

    if (Func_080770c0((s32)outPtr + 0x200) != 0) {
        closeId = 0x2084;
        goto close;
    }

    if (mode == 2) {
        Func_080000c0(6);
    } else {
        Func_0808a170(0x207d);
        Func_0808a178(handle, 0);
        if (Func_0808a070(0, 0) != 0) {
            closeId = 0x207e;
            goto close;
        }
    }

    for (i = 0; i < count; i++) {
        Func_08077168(bytes[i]);
    }
    for (i = 0; i < count; i++) {
        if (bytes[i] != 0) {
            Func_08077150(bytes[i]);
        }
    }

    id2 = Func_080a1018();

    for (i = 0; i < count; i++) {
        Func_08077168(bytes[i]);
    }
    for (i = 0; i < count; i++) {
        Func_08077150(bytes[i]);
    }

    if (id2 == -1) {
        closeId = 0x207e;
        goto close;
    }

    goto setup;

close:
    Func_0808a170(closeId);
    Func_0808a180(handle, 0);
    return;

setup:

    {
        u8 *hit;
        u8 *object2;

        Func_08015120(id2, 1);
        Func_0808a170(0x207f);
        Func_0808a180(handle, 0);

        Func_0808a090(0, 0x10000, 0x8000);
        Func_0808a090(id2, 0x10000, 0x8000);
        Func_0808a090(handle, 0x10000, 0x8000);

        hit = Func_0808a080(0);
        if (hit != 0) {
            Func_0808a0f0(id2, *(s32 *)(hit + 8), *(s32 *)(hit + 16));
        }

        Func_0808a0d0(id2, fieldA, fieldB + 16);
        Func_0808a0d0(0, fieldA + 16, fieldB + 16);
        Func_0808a150(id2, 0, 30);
        Func_0808a100(id2, 3);
        Func_0808a110(0, 3);
        Func_0808a0d0(handle, fieldA, fieldB - 16);
        Func_0808a0c8(handle, fieldA + 16, fieldB - 16);
        Func_0808a588(0, id2);
        Func_0808a0d0(id2, fieldA, fieldB - 16);
        Func_0808a100(handle, 1);
        Func_0808a1b8(handle, 0x8000, 0);
        Func_0808a0d0(id2, fieldA, fieldB - 48);
        Func_0808a0d0(handle, fieldA, fieldB - 16);
        Func_0808a0d0(handle, fieldA, fieldB);
        Func_08077168(id2);
        Func_080770c8((s32)outPtr + 0x200);

        object2 = Func_0808a080(id2);
        Func_080770e8(id2 * 16 + 0x370, *(s32 *)(object2 + 8) >> 20);
        Func_080770e8(id2 * 16 + 0x378, *(s32 *)(object2 + 16) >> 20);
    }
}
