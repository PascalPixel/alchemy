#include "types.h"

/*
 * resource_39c owner at 0x0200013c, 472 bytes: the overlay's general
 * "spawn and fully configure" routine -- the same spawn-then-owner-record
 * shape resource_39c_c_02000048.c/02005158.c/02005388.c already document,
 * generalised to eight caller-supplied parameters (three spawn arguments,
 * three plain copy-through fields, a bitmask of optional features, and a
 * pointer to a record of further optional per-feature values) instead of
 * pulling everything from a fixed "current slot" lookup.
 *
 * Complete owner: `push {r5, r6, r7, lr}` + the r8/r9/sl/fp spill at
 * 0x0200013c through the matching unwind and `pop {r0} / bx r0` at
 * 0x2000304-0x2000306; the three-word literal pool 0x02000308-0x02000313
 * follows, then the next owner's prologue at 0x02000314. Eight arguments
 * (r0-r3, plus four stack words at, in the caller's address order, SP+0,
 * SP+4, SP+8, SP+12 -- read here in the order SP+8, SP+12, SP+0, SP+4),
 * void.
 *
 * Not found by the structural inventory walk (unindexed): reached only by
 * `bl`, resolved with `bun tools/overlay_call_targets.ts`'s `+2` rule.
 *
 * `entry = table[idx]` where `table` is the fixed pointer array at
 * 0x0200de64 and `idx = flags & 0xf` -- the low nibble of the `flags`
 * bitmask parameter selects a variant record, while bits 16 and up are
 * independent boolean feature switches, each gating one optional
 * initialisation step below. Bit 20 (tested once, before the spawn, using
 * the caller's `extra` pointer) picks the spawn call's own first argument
 * and is never tested again afterward.
 *
 * `Func_08009098(0)`'s result is read once, through its own field 0x50,
 * for two bits (0x0c) of the owner record's field 9 -- the same
 * clear-two-bits-then-copy idiom resource_39c_c_02000048.c's adopted
 * Func_02005a2e pair and resource_39c_c_02005158.c already use on this
 * overlay's owner-record field 9, just sourced from a different place. The
 * bit-17 block below can overwrite those same two bits again afterward
 * with a value taken from `extra`'s own byte field 0 -- both updates are
 * kept exactly as compiled, not merged.
 *
 * Fields 0x30 and 0x34 on the spawned object are zeroed unconditionally
 * first and then, only under the bit-18 gate, recomputed through three
 * physical calls to `Func_03000380`. The first input pair depends on whether
 * bit 19 populated fields 0x18/0x1c; the second call is shared after that
 * branch, matching the converged machine control flow.
 *
 * Uncertainty: the six imported callees, `Func_08009098`'s result, `entry`,
 * or `extra`'s field roles are identified beyond this call shape; the
 * default spawn-argument constant 0xde is recorded as read, not decoded
 * further.
 *
 * Correction (caught while adapting this routine's byte-identical
 * resource_3c9 copy): the bit-19 copy and the bit-18 subtraction read
 * object fields 0x18/0x1c ([r6, #24]/[r6, #28]), not 0x24/0x28 as this
 * file's first draft said -- four sites fixed against the disassembly.
 */

extern u8 *Func_0808a080(s32 arg0, s32 arg1, s32 arg2, s32 arg3);
extern void Func_080090c8(u8 *object, s32 arg1);
extern void Func_08009080(u8 *object, s32 arg1);
extern u8 *Func_08009098();
extern void Func_0808a160(u8 *object, s32 arg1);
extern s32 Func_03000380(s32 arg0, s32 arg1);
extern void Func_080091c0(u8 *object, s32 arg1);

void Func_0200013c(s32 a1, s32 a2, s32 a3, s32 a4, s32 a5, s32 a6, u32 flags, u8 *extra)
{
    u8 **table = (u8 **)0x0200de64;
    u32 idx = flags & 0xf;
    u8 *object;
    u8 *ownerRecord;
    u8 *callResult;
    u8 *entry;
    s32 spawnArg0;
    u16 *field64;

    if ((flags & 0x100000) != 0 && extra != 0) {
        spawnArg0 = *(short *)(extra + 0x18);
    } else {
        spawnArg0 = 0xde;
    }

    object = Func_0808a080(spawnArg0, a1, a2, a3);
    if (object == 0) {
        return;
    }

    ownerRecord = *(u8 **)(object + 0x50);

    Func_080090c8(object, (idx + 1) & 0xf);

    entry = table[idx];
    Func_08009080(object, (s32)entry);

    ownerRecord[0x26] = 0;
    object[0x55] = 0;
    *(u32 *)(object + 0x6c) = 0x02008105;
    *(s32 *)(object + 0x48) = a5;
    *(s32 *)(object + 0x4c) = a6;
    *(s32 *)(object + 0x44) = a4;

    callResult = Func_08009098(0);
    ownerRecord[9] = (ownerRecord[9] & ~0x0c) | (*(u8 *)(*(u8 **)(callResult + 0x50) + 9) & 0x0c);

    *(s32 *)(object + 0x30) = 0;
    *(s32 *)(object + 0x34) = 0;
    *field64 = 0;
    field64 = (u16 *)(object + 0x64);

    if ((flags & 0xffff0000) == 0 || extra == 0) {
        return;
    }

    if ((flags & 0x10000) != 0) {
        Func_0808a160(object, *(s32 *)(extra + 4));
    }

    if ((flags & 0x20000) != 0) {
        object[0x23] &= ~1;
        ownerRecord[9] = (ownerRecord[9] & ~0x0c) | ((extra[0] & 3) << 2);
    }

    {
        s32 field18Copied = (flags & 0x80000) != 0;
        s32 entryField0xc = *(s32 *)(entry + 0xc);
        s32 field34Input;

        if (field18Copied) {
            *(s32 *)(object + 0x18) = *(s32 *)(extra + 8);
            *(s32 *)(object + 0x1c) = *(s32 *)(extra + 12);
        }

        if ((flags & 0x40000) != 0) {
            if (field18Copied) {
                *(s32 *)(object + 0x30) =
                    Func_03000380(*(s32 *)(extra + 0x10) - *(s32 *)(object + 0x18), entryField0xc);
                field34Input = *(s32 *)(extra + 0x14) - *(s32 *)(object + 0x1c);
            } else {
                *(s32 *)(object + 0x30) =
                    Func_03000380(*(s32 *)(extra + 0x10) - 0x10000, entryField0xc);
                field34Input = *(s32 *)(extra + 0x14) - 0x10000;
            }
            *(s32 *)(object + 0x34) = Func_03000380(field34Input, entryField0xc);
        }
    }

    if ((flags & 0x200000) != 0) {
        Func_08009080(object, 1);
        Func_08009098(object, *(s32 *)(extra + 0x1c));
    }

    if ((flags & 0x400000) != 0) {
        *(u16 *)(ownerRecord + 0x1e) = *(u16 *)(extra + 0x20);
    }

    if ((flags & 0x800000) != 0) {
        *field64 = *(u16 *)(extra + 0x22);
    }

    if ((flags & 0x1000000) != 0) {
        *(u32 *)(object + 0x6c) = *(u32 *)(extra + 0x24);
    }
}
