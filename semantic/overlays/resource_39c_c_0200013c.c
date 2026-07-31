typedef unsigned char u8;
typedef unsigned short u16;
typedef int s32;
typedef unsigned int u32;

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
 * `Func_02005c32(0)`'s result is read once, through its own field 0x50,
 * for two bits (0x0c) of the owner record's field 9 -- the same
 * clear-two-bits-then-copy idiom resource_39c_c_02000048.c's adopted
 * Func_02005a2e pair and resource_39c_c_02005158.c already use on this
 * overlay's owner-record field 9, just sourced from a different place. The
 * bit-17 block below can overwrite those same two bits again afterward
 * with a value taken from `extra`'s own byte field 0 -- both updates are
 * kept exactly as compiled, not merged.
 *
 * Fields 0x30 and 0x34 on the spawned object are zeroed unconditionally
 * first and then, only under the bit-18 gate, recomputed through one of
 * two different callee pairs depending on whether bit 19 populated fields
 * 0x24/0x28 first (`Func_02005be8`/`Func_02005c0e` if so, else
 * `Func_02005c00`/`Func_02005c0e` with a flat `-0x10000` adjustment
 * instead of the field-0x24/0x28 subtraction) -- recorded exactly as
 * compiled, not unified.
 *
 * Uncertainty: none of the ten callees, `Func_02005c32`'s result, `entry`,
 * or `extra`'s field roles are identified beyond this call shape; the
 * default spawn-argument constant 0xde is recorded as read, not decoded
 * further.
 */

extern u8 *Func_02005c32(s32 arg0);
extern u8 *Func_02005b58(s32 arg0, s32 arg1, s32 arg2, s32 arg3);
extern void Func_02005b62(u8 *object, s32 arg1);
extern void Func_02005b7c(u8 *object, s32 entryValue);
extern void Func_02005d8e(u8 *object, s32 arg1);
extern s32 Func_02005be8(s32 arg0, s32 arg1);
extern s32 Func_02005c00(s32 arg0, s32 arg1);
extern s32 Func_02005c0e(s32 arg0, s32 arg1);
extern void Func_02005c7c(u8 *object, s32 arg1);
extern void Func_02005c8c(u8 *object, s32 arg1);

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

    object = Func_02005b58(spawnArg0, a1, a2, a3);
    if (object == 0) {
        return;
    }

    ownerRecord = *(u8 **)(object + 0x50);

    Func_02005b62(object, (idx + 1) & 0xf);

    entry = table[idx];
    Func_02005b7c(object, (s32)entry);

    object[0x55] = 0;
    ownerRecord[0x26] = 0;
    *(u32 *)(object + 0x6c) = 0x02008105;
    *(s32 *)(object + 0x44) = a4;
    *(s32 *)(object + 0x48) = a5;
    *(s32 *)(object + 0x4c) = a6;

    callResult = Func_02005c32(0);
    ownerRecord[9] = (ownerRecord[9] & ~0x0c) | (*(u8 *)(*(u8 **)(callResult + 0x50) + 9) & 0x0c);

    *(s32 *)(object + 0x30) = 0;
    *(s32 *)(object + 0x34) = 0;
    field64 = (u16 *)(object + 0x64);
    *field64 = 0;

    if ((flags & 0xffff0000) == 0 || extra == 0) {
        return;
    }

    if ((flags & 0x10000) != 0) {
        Func_02005d8e(object, *(s32 *)(extra + 4));
    }

    if ((flags & 0x20000) != 0) {
        object[0x23] &= ~1;
        ownerRecord[9] = (ownerRecord[9] & ~0x0c) | ((extra[0] & 3) << 2);
    }

    {
        s32 field24Copied = (flags & 0x80000) != 0;
        s32 entryField0xc = *(s32 *)(entry + 0xc);

        if (field24Copied) {
            *(s32 *)(object + 0x24) = *(s32 *)(extra + 8);
            *(s32 *)(object + 0x28) = *(s32 *)(extra + 12);
        }

        if ((flags & 0x40000) != 0) {
            if (field24Copied) {
                *(s32 *)(object + 0x30) =
                    Func_02005be8(*(s32 *)(extra + 0x10) - *(s32 *)(object + 0x24), entryField0xc);
                *(s32 *)(object + 0x34) =
                    Func_02005c0e(*(s32 *)(extra + 0x14) - *(s32 *)(object + 0x28), entryField0xc);
            } else {
                *(s32 *)(object + 0x30) =
                    Func_02005c00(*(s32 *)(extra + 0x10) - 0x10000, entryField0xc);
                *(s32 *)(object + 0x34) =
                    Func_02005c0e(*(s32 *)(extra + 0x14) - 0x10000, entryField0xc);
            }
        }
    }

    if ((flags & 0x200000) != 0) {
        Func_02005c7c(object, 1);
        Func_02005c8c(object, *(s32 *)(extra + 0x1c));
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
