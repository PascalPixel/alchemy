#include "types.h"

/*
 * resource_3a4 owner at 0x020000bc, 40 bytes: the per-frame integrator
 * for one actor record. Advance two position words by their velocities,
 * advance two more fields by a third velocity, then damp that velocity,
 * and return 0.
 *
 * A LEAF, and the largest of the four found by the 2026-08-01 fix to
 * sweep B. Forty bytes of real work with no prologue at all: it needs no
 * callee-saved register because it calls nothing, so it opens on a load
 * and closes on `bx lr`. Sweep B resolved its address correctly and then
 * discarded it for having no `push`.
 *
 * IT IS PUBLISHED FROM A DESCRIPTOR RECORD, not from the overlay header.
 * The word at image offset 0x4020 is 0x020080bd -- this offset with the
 * Thumb bit under the base + 0x8000 spelling -- and it sits in a record
 * whose neighbouring words are 0x18, 0xc0010000, 0x10, 0x22, then this
 * pointer, then 0, 1, 0xc. So the callback field is one member of a
 * structure, which is the same install-as-data mechanism this overlay
 * uses six times over, and the seventh instance of it here.
 *
 * Complete owner: 0x020000bc through `bx lr` at 0x020000e2. All forty
 * bytes are instructions -- there is no literal pool, because every
 * constant it needs is a small immediate or a register. Bounds are exact
 * from the overlay source: `AlchemyC_02000098` runs `.space 0x24` to
 * 0x020000bc, and `AlchemyC_020000e4` begins at 0x020000e4.
 *
 * ONE ARGUMENT, r0, the record, used for every access and never
 * reloaded.
 *
 * The field pattern is the actor record this overlay's drafted rows
 * already use: +8 and +12 are the position pair that 0x02000ec0 adjusts
 * by 224 << 12 and -0x80000, +24 and +28 are the pair it stamps with
 * 0x13333, and +72 is the field 0x02000ec0 writes 0x1999 into. So the
 * three velocity fields read here, +36, +40 and +44, are the deltas for
 * those same members.
 *
 * THE DAMPING READS A STALE REGISTER ON PURPOSE, and this is the one
 * place the row can be transcribed wrongly. r2 is loaded with *(r0 + 40)
 * at 0x020000c4 and is still holding that value at 0x020000dc, where
 * `subs r2, r2, r3` subtracts *(r0 + 72) from it and stores the result
 * back to +40. Nothing writes +40 in between, so the value is current
 * and the expression is `*(record + 40) - *(record + 72)`. Reading it as
 * a fresh load would give the same answer here, but only by luck; it is
 * transcribed from the register the instruction actually names.
 *
 * The +24 and +28 updates BOTH use r1, loaded once from +44 at
 * 0x020000ca, so the two fields advance by one shared quantity rather
 * than by two reads of the same offset.
 */

s32 Func_020000bc(u8 *record)
{
    s32 velocity_28;
    s32 velocity_2c;

    *(s32 *)(record + 8) = *(s32 *)(record + 8) + *(s32 *)(record + 36);

    velocity_28 = *(s32 *)(record + 40);
    *(s32 *)(record + 12) = *(s32 *)(record + 12) + velocity_28;

    velocity_2c = *(s32 *)(record + 44);
    *(s32 *)(record + 24) = *(s32 *)(record + 24) + velocity_2c;
    *(s32 *)(record + 28) = *(s32 *)(record + 28) + velocity_2c;

    *(s32 *)(record + 40) = velocity_28 - *(s32 *)(record + 72);

    return 0;
}
