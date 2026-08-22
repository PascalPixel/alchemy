#include "types.h"

/*
 * resource_39c owner at 0x02005158, 88 bytes: spawn a secondary object from
 * three fields of the caller's object, initialise three of its own byte
 * fields, then set two low bits in its owner record's flag byte.
 *
 * Complete owner: `push {r5, r6, r7, lr}` at 0x02005158 through
 * `pop {r5, r6, r7} / pop {r0} / bx r0` at 0x20051a4-0x20051a8, followed by
 * one pool word (0x0200de08); the next owner's prologue is at 0x020051b0.
 * One argument (r0), void.
 *
 * Not found by the structural inventory walk (unindexed): reached only by
 * `bl`, resolved with `cargo run --release --manifest-path tools/overlay-call-targets/Cargo.toml --`'s `+2` rule.
 *
 * The spawn-then-flag shape and the record[9] mask/set idiom match
 * resource_39c_c_02000048.c's adopted Func_02005a2e/Func_02005aac pair
 * closely enough to be read as siblings, but they are not identical: that
 * adopted owner ANDs -0xD into record[9] (clears bits 0x0C); this owner ORs
 * 0x0C in instead. Recorded exactly as compiled, not unified.
 *
 * Uncertainty: `object`'s field roles (offset 8/12/16 spawn coordinates,
 * 0x22/0x23 byte pair, 0x50 owner-record pointer, 0x55 state byte) and the
 * owner record's field 9 (flag byte) and 0x26 (byte, cleared) are inferred
 * only from this call shape and the resource_39c_c_02000048.c sibling; none
 * are independently confirmed. The 0x0200de08 pointer passed to
 * Func_08009098 is not dereferenced here, so it is recorded as a raw
 * literal rather than a typed global, per HANDOVER's address-taken-literal
 * rule.
 */

extern u8 *Func_0200ab38(s32 kind, s32 arg1, s32 arg2, s32 arg3);
extern void Func_0200ab3e(u8 *object, void *table);
extern void Func_0200ab50(u8 *record, s32 arg1);

void Func_02005158(u8 *object)
{
    u8 *newObject;
    u8 *record;

    newObject = Func_0200ab38(24, *(s32 *)(object + 8), *(s32 *)(object + 12), *(s32 *)(object + 16));
    if (newObject == 0) {
        return;
    }

    record = *(u8 **)(newObject + 0x50);
    Func_0200ab3e(newObject, (void *)0x0200de08);

    newObject[0x55] = 0;
    newObject[0x22] = 1;
    newObject[0x23] = 2;

    if (record != 0) {
        Func_0200ab50(record, 2);
        record[0x26] = 0;
        record[9] |= 0x0C;
    }
}
