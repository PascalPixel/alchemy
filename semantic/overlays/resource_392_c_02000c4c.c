typedef unsigned char u8;
typedef unsigned short u16;
typedef unsigned int u32;
typedef signed int s32;

/*
 * resource_392 owner at 0x02000c4c, 102 bytes: advance one orbiting effect
 * particle by a frame.
 *
 * Complete owner: `push {r5, r6, r7, lr}` at 0x02000c4c through
 * `pop {r5, r6, r7} / pop {r1} / bx r1` at 0x02000cac-0x02000cb0.  The return
 * address is popped into r1, not r0 (HANDOVER section 0, epilogue rule), so r0
 * survives and IS the result -- and `movs r0, #0` at 0x02000caa is on the only
 * path, so this returns a constant 0 status.  r0 is copied to r6 before being
 * written, so it takes one argument: the particle record.  code_bytes ==
 * span_bytes for this row, so there is no pool inside it and nothing after the
 * epilogue belongs to it.
 *
 * Straight-line body: the only branches are the two one-instruction
 * conditional skips at 0x02000c5c and 0x02000c82.  Nothing to fold and no arm
 * to merge.
 *
 * Five call sites, matching the inventory's calls=5, resolved with
 * `bun tools/overlay_call_targets.ts resource_392 0c4c`:
 *   0x02000c54 -> veneer 0x0da4 -> Func_08000118   x1
 *   0x02000c62 -> veneer 0x0dac -> Func_08000120   x2 (with 0x02000c7c)
 *   0x02000c8a -> veneer 0x0d9c -> Func_080000f8   x2 (with 0x02000c90)
 * The two Func_080000f8 sites are six bytes apart in the listing and print
 * callees six bytes apart, which is what the `bl` decoding artefact looks like
 * when it happens to be self-consistent -- resolving them is what shows they
 * are one import drawn twice, not two different sources.
 *
 * The three imports are all established elsewhere in the semantic tree:
 * Func_080000f8 is the argument-less pseudo-random source, and Func_08000118 /
 * Func_08000120 are the sine/cosine pair over a binary angle (0x10000 to the
 * turn).  The +0x8000 added to the angle before the second Func_08000120 call
 * is exactly a half turn, so that call is the negated lobe of the first.
 *
 * What it does, in order, with `record` = the argument:
 *  1. `lift = Func_08000118(record->angle) * 2`, then forced NEGATIVE
 *     (`if (lift > 0) lift = -lift`).  This is a sign clamp, not an absolute
 *     value: a lift that is already <= 0 is left alone, so the particle only
 *     ever rises.
 *  2. `record->x = record->base_x + Func_08000120(record->angle) * 2`.
 *  3. `record->y = record->base_y + lift`.
 *  4. `record->sprite->field_1e = Func_08000120(record->angle + 0x8000) / 8`,
 *     where the division is the compiler's signed divide-by-8: `if (v < 0)
 *     v += 7; v >>= 3`.  Written as `v >> 3` alone it would be a floor rather
 *     than a truncation and would differ for every negative value.
 *  5. `record->angle += 1024 + jitter`, where `jitter` is the sum of two
 *     independent draws each reduced by `(draw << 9) >> 16` -- an UNSIGNED
 *     shift pair, so each draw contributes `(draw >> 7) & 0x1ff`, i.e. 0..511.
 *     The base step 1024 is `movs r2,#128 / lsls r2,#3`, the displacement/value
 *     shape again but here plainly a value.
 *  6. return 0.
 *
 * Uncertainties.  Which of Func_08000118/Func_08000120 is sine and which is
 * cosine is recorded both ways in the semantic tree; nothing here decides it,
 * and nothing here depends on it -- the structure is "one lobe drives the
 * horizontal offset, the other the vertical, and the half-turn lobe drives the
 * sprite field".  Field +0x1e of the sprite record is a halfword written and
 * never read here; the value handed to it is a trig lobe scaled down by 8, so
 * it reads as a rotation or depth field rather than a coordinate.  The record
 * offsets below are named by their use in this owner alone.
 */

/* Overlay imports (via the veneer table).  Old-style declarations: one import
 * name can be reached with different argument counts at different sites. */
s32 Func_080000f8();    /* pseudo-random source, no arguments */
s32 Func_08000118();    /* trig lobe of a binary angle */
s32 Func_08000120();    /* the other trig lobe of a binary angle */

struct Particle_02000c4c {
    u8 unknown_00[8];
    s32 x;                  /* +0x08 */
    s32 y;                  /* +0x0c */
    u8 unknown_10[0x20];
    s32 angle;              /* +0x30, 0x10000 to the turn */
    u8 unknown_34[4];
    s32 base_x;             /* +0x38 */
    s32 base_y;             /* +0x3c */
    u8 unknown_40[0x10];
    u16 *sprite;            /* +0x50 */
};

s32 Func_02000c4c(struct Particle_02000c4c *record)
{
    u16 *sprite = record->sprite;
    s32 lift;
    s32 tilt;
    s32 jitter;

    lift = Func_08000118(record->angle) * 2;
    if (lift > 0)
        lift = -lift;

    record->x = record->base_x + Func_08000120(record->angle) * 2;
    record->y = record->base_y + lift;

    /* Signed divide by 8, spelled `if (v < 0) v += 7; v >>= 3`. */
    tilt = Func_08000120(record->angle + 0x8000);
    if (tilt < 0)
        tilt += 7;
    sprite[15] = (u16)(tilt >> 3);          /* +0x1e */

    jitter = (s32)(((u32)Func_080000f8() << 9) >> 16);
    jitter += (s32)(((u32)Func_080000f8() << 9) >> 16);
    record->angle += jitter + 1024;

    return 0;
}
