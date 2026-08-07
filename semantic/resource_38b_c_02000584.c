#include "types.h"

/*
 * Resource 38b cutscene at 0x02000584 (780 bytes, 71 call sites over 24
 * distinct callees).
 *
 * Complete owner: `push {r5, r6, lr}` at 0x02000584 and the interworking
 * return `pop {r5, r6} / pop {r0} / bx r0` at 0x0200085c.  The popped r0 is
 * the return address, so the owner is `void`.  0x02000862 is alignment and
 * 0x02000864-0x0200088f is the literal pool.  Derived from a control-flow
 * walk of the prologue: the body is straight-line apart from the two entry
 * guards and one if/else, so every instruction up to 0x02000860 is reached
 * and nothing below it is.
 *
 * INVERSE POOL TRAP, again.  overlay_show.ts reports a "pool word referenced"
 * at 0x02000ba4, which is inside a different owner (0x02000ae0).  The real
 * pool word 0x00004ccc at 0x02000870 decodes as `ldr r4,[pc,#816]` and the
 * tool followed that fake load.  The same spurious reference appears in
 * 0x02000404's listing, from the same constant.  Not data belonging to this
 * owner, and not a call.
 *
 * LINK-BASE / PARITY WITNESS.  The pool word 0x020091c0 is EVEN, so by the
 * parity rule it is an in-image data address, not a Thumb callback: under the
 * 0x02008000 base it is file offset 0x11c0, exactly where the import veneer
 * table ends and the overlay's data begins.  It is passed as the third
 * argument of the three Func_0808a168 calls, which is the documented position
 * for such a word.  This is a fifth independent confirmation of the base.
 *
 * SKIP-BEAT COUNTER at 0x02000784: `movs r3,#236 / lsls r3,#1` off the
 * 0x03001ebc workspace pointer — the u16 at workspace + 472, the same field
 * 0x02000240 bumps.  Recognised by its constants, per HANDOVER section 0.
 *
 * COMPLETENESS, per target (a matching total can hide a permutation, so this
 * is stated per callee): Func_0808a1b8 x13, Func_0808a080 x6,
 * Func_0808a188 x6, Func_0808a010 x6, Func_0808a1e8 x5, Func_0808a110 x5,
 * Func_0808a100 x4, Func_0808a0d0 x3, Func_0808a138 x3, Func_0808a168 x3,
 * Func_080770c0 x2, Func_0808a170 x2, Func_0808a090 x2, Func_0808a018 x1,
 * Func_0808a208 x1, Func_0808a210 x1, Func_0808a218 x1, Func_0808a1f0 x1,
 * Func_0808a180 x1, Func_08015040 x1, Func_0808a398 x1, Func_0808a060 x1,
 * Func_080770c8 x1, Func_0808a020 x1 = 71 sites.  Each appears exactly that
 * many times below.  The inventory reports `calls=67`; 71 >= 67 is the
 * expected direction for that field.  No `bl` is reached from more than one
 * control path — the only branch is a symmetric if/else whose arms have five
 * sites each, all distinct — so none of the five inflating shared-call-site
 * shapes applies and no join is needed.  All 71 targets are import veneers,
 * resolved with tools/lib/overlay_call_targets.ts.
 *
 * REGISTER NOTES.  r5 holds the slot-0 record only until 0x0200073a, where
 * `movs r5,#254` reuses it as an AND mask (and r5 is then overwritten again
 * at 0x0200082c with the data pointer 0x020091c0).  Its last use as a record
 * is the +0x08 read at 0x020005cc.  r6 is likewise the constant 1 from
 * 0x0200075e.  Both are tracked per use rather than as single variables.
 */

/* The overlay's scene workspace pointer CELL. */
extern u8 *Data_03001ebc;

/* Slot accessor. */
extern u8 *Func_0808a080();

/* Progress-flag test and set. */
extern s32 Func_080770c0();
extern void Func_080770c8();

/* In-image data at file offset 0x11c0, immediately after the veneer table. */
extern u8 Data_020091c0[];

/* Imports; old-style declarations, arity open. */
extern void Func_0808a018();
extern void Func_0808a020();
extern void Func_0808a010();
extern void Func_0808a060();
extern void Func_0808a090();
extern void Func_0808a0d0();
extern void Func_0808a100();
extern void Func_0808a110();
extern void Func_0808a138();
extern void Func_0808a168();
extern void Func_0808a170();
extern void Func_0808a180();
extern void Func_0808a188();
extern void Func_0808a1b8();
extern void Func_0808a1e8();
extern void Func_0808a1f0();
extern void Func_0808a208();
extern void Func_0808a210();
extern void Func_0808a218();
extern void Func_0808a398();
extern void Func_08015040();

void Func_02000584(void)
{
    u8 *player;
    u8 *actor;
    u8 *flags;
    u8 *workspace;

    player = Func_0808a080(0);

    /* Both gates must be set for the scene to play. */
    if (Func_080770c0(0x845) == 0) {
        return;
    }
    if (Func_080770c0(0x848) == 0) {
        return;
    }

    Func_0808a018();
    Func_0808a208(0x00026666, 0x00004ccc);
    /* 173 << 16 = 0x00ad0000; the second argument is a negated 1. */
    Func_0808a210(0x01070000, -1, 0x00ad0000, 1);
    Func_0808a218();

    /* Which of the two speakers leads depends on whether actor 12 is further
     * along the x axis than the player.  Signed 16.16 compare. */
    actor = Func_0808a080(12);
    if (*(s32 *)(actor + 8) > *(s32 *)(player + 8)) {
        /* 160 << 7 = 0x5000, 128 << 1 = 256. */
        Func_0808a1b8(13, 0x5000, 20);
        Func_0808a1e8(13, 256, 20);
        Func_0808a170(0x1775);
        Func_0808a188(13, 0, 10);
        Func_0808a1e8(12, 256, 0);
    } else {
        /* 192 << 6 = 0x3000. */
        Func_0808a1b8(12, 0x3000, 20);
        Func_0808a1e8(12, 256, 20);
        Func_0808a170(0x1775);
        Func_0808a188(12, 0, 10);
        Func_0808a1e8(13, 256, 0);
    }

    Func_0808a1e8(14, 256, 0);
    Func_0808a1b8(14, 0x3000, 0);
    Func_0808a1b8(12, 0x5000, 0);
    Func_0808a1b8(13, 0x3000, 0);

    /* 134 << 1 = 268. */
    Func_0808a0d0(0, 268, 184);
    /* 192 << 8 = 0xc000. */
    Func_0808a1b8(0, 0xc000, 40);

    Func_0808a138(13, 2);
    Func_0808a188(13, 0, 10);
    Func_0808a1b8(13, 0, 0);
    Func_0808a1b8(14, 0x3000, 20);
    /* 128 << 8 = 0x8000. */
    Func_0808a1b8(12, 0x8000, 20);
    Func_0808a110(12, 3);
    /* 129 << 1 = 258. */
    Func_0808a1f0(14, 258);
    Func_0808a010(40);

    Func_0808a1b8(14, 0x3000, 10);
    Func_0808a1b8(12, 0x5000, 0);
    Func_0808a1b8(13, 0x3000, 10);
    Func_0808a138(14, 1);
    Func_0808a188(14, 0, 10);
    Func_0808a100(12, 3);
    Func_0808a110(13, 3);
    Func_0808a010(20);
    Func_0808a180(14, 0);
    Func_0808a090(14, 0x9999, 0x4ccc);

    /* Clear bit 0 of actor 14's +0x5a flag byte for the line, restore after.
     * 133 << 1 = 266. */
    flags = Func_0808a080(14) + 90;
    *flags = (u8)(*flags & 0xfe);
    Func_0808a0d0(14, 266, 172);
    Func_0808a010(1);
    flags = Func_0808a080(14) + 90;
    *flags = (u8)(*flags | 1);

    Func_0808a010(10);
    Func_0808a110(14, 3);
    Func_0808a188(14, 0, 10);
    Func_08015040(0x177a, 1);

    /* Skip-beat counter: the u16 at workspace + 472. */
    workspace = Data_03001ebc;
    *(u16 *)(workspace + 472) += 1;

    Func_0808a398(194, 3);
    Func_0808a060(194, 0);
    Func_0808a110(14, 3);
    Func_0808a100(0, 1);
    Func_0808a1b8(0, 0xc000, 0);
    /* 128 << 9 = 0x10000, 128 << 8 = 0x8000. */
    Func_0808a090(14, 0x10000, 0x8000);

    /* Second line from actor 14, same flag-byte bracket.  131 << 1 = 262. */
    flags = Func_0808a080(14) + 90;
    *flags = (u8)(*flags & 0xfe);
    Func_0808a0d0(14, 262, 156);
    Func_0808a010(1);
    flags = Func_0808a080(14) + 90;
    *flags = (u8)(*flags | 1);

    Func_0808a010(20);
    Func_0808a138(12, 2);
    Func_0808a188(12, 0, 10);
    Func_0808a100(12, 3);
    Func_0808a100(13, 3);
    Func_0808a110(14, 3);

    /* Three exits, all handed the same in-image data block at 0x11c0. */
    Func_0808a168(12, 0x10000, Data_020091c0);
    Func_0808a168(13, 0x10000, Data_020091c0);
    Func_0808a168(14, 0x10000, Data_020091c0);

    /* Mark the cutscene seen. */
    Func_080770c8(0x849);
    Func_0808a020();
}
