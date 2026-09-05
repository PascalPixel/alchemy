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
 * Object_SetCallback is not dereferenced here, so it is recorded as a raw
 * literal rather than a typed global, per HANDOVER's address-taken-literal
 * rule.
 */

extern u8 *Func_0200ab38(s32 kind, s32 arg1, s32 arg2, s32 arg3);
extern void Func_0200ab3e(u8 *src, void *table);
extern void Func_0200ab50(u8 *rec, s32 arg1);

void OverlayObject_SpawnKind24AtObject(u8 *src)
{
    u8 *obj;
    u8 *rec;

    obj = Func_0200ab38(24, *(s32 *)(src + 8), *(s32 *)(src + 12), *(s32 *)(src + 16));
    if (obj == 0) {
        return;
    }

    rec = *(u8 **)(obj + 0x50);
    Func_0200ab3e(obj, (void *)0x0200de08);

    obj[0x55] = 0;
    obj[0x22] = 1;
    obj[0x23] = 2;

    if (rec != 0) {
        Func_0200ab50(rec, 2);
        rec[0x26] = 0;
        rec[9] |= 0x0C;
    }
}
