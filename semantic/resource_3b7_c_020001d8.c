/*
 * resource_3b7 owner at 0x020001d8, 740 bytes (0x020001d8-0x020004bb).
 *
 * The overlay's room entry hook.  It takes one of two completely separate
 * paths depending on whether the current map id is 0xbd: either set up the
 * two-actor blend scene and hand off to Func_020016a8, or run the arrival
 * script for this room - a win/loss coin report on script state 13, and a prize
 * hand-out walk on script state 12.
 *
 * Code runs 0x020001d8-0x0200046f.  The trailing 76 bytes,
 * 0x02000470-0x020004bb, are a literal pool of nineteen words - 0x02000240,
 * 0x000000bd, 0x03001ebc, 0x00003f42, 0x04000050, 0x0000080c, 0x04000052,
 * 0xffff0000, 0x03001d18, 0x00000109, 0x02001000, 0x00004e1f, 0x00001387,
 * 0x00000e13, 0x00000e14, 0x00000e2e, 0x00000e2f, 0x00000e30, 0x00000e31.
 * They lie past the `bx r1` return and are read only by `ldr rN, [pc, ...]`,
 * so they are data.  `overlay_show.ts` lists a twentieth "pool word referenced"
 * at 0x0200051c, which this owner never touches - it comes from 0x00004e1f
 * decoding as `ldr r6, [pc, #124]`, and 0x0200048e decoding as a NEON
 * `vcvt.u16.f16`.  The pool map is taken from the owner's own control flow.
 *
 * No pool word is in the 0x0200_8xxx..0200_bxxx in-image band, so the overlay's
 * 0x02008000 link base (established in
 * semantic/overlays/resource_3b7_c_020016a8.c) does not bear on this owner.
 * 0x02000240 is the RAM workspace the byte-exact siblings
 * `assets/code/resource_3b7_c_02000044.c` and `..._020001a8.c` name - and this
 * owner reads exactly their `Data_02000240[224]` element and compares it to
 * exactly their 0xbd, which those files spell `(s32)&Value_000000bd`.  The
 * comparison here is a plain s16 test against 189, which is what the assembly
 * does (`ldrsh` against a pooled 0xbd); the sibling's pointer spelling is noted
 * rather than copied, because nothing in this owner supports it.
 * 0x03001ebc is the established IWRAM workspace pointer, 0x03001d18 an IWRAM
 * byte flag, 0x04000050/0x04000052 are BLDCNT/BLDALPHA, and 0x02001000 is the
 * balance snapshot that this overlay's Func_020009f8 writes - reading it here
 * is what makes the win/loss delta meaningful.
 *
 * Call targets were resolved with `tools/lib/overlay_call_targets.ts` (an overlay
 * `bl` stores `target_offset - 2`).  Completeness, as a multiset:
 *   Func_0808a010 x6, Func_0808a170 x6, Func_0808a180 x5, Func_080770c0 x4,
 *   Func_0808a080 x4, Func_0808a158 x4, Func_08015120 x3, Func_0808a360 x3,
 *   Func_080f9010 x3, Func_0808a018 x2, Func_0808a020 x2, Func_0808a100 x2,
 *   Func_0808a370 x2, and one each of Func_0808a060, Func_0808a0d0,
 *   Func_0808a0f0, Func_0808a1b8, Func_0808a398, Func_080b0060, plus the three
 *   in-overlay calls Func_020016a8, Func_020008f8 and Func_02000d70.
 * That is 55 sites over 22 distinct targets, exactly the tool's
 * `sites=55 distinct_targets=22`.  The inventory row says calls=47, the
 * documented lower bound.
 *
 * All three in-overlay callees are converted in this reconstruction:
 * Func_020016a8 is the scene initialiser, Func_020008f8 the conversation node,
 * and Func_02000d70 the non-repeating variant picker whose result is used here
 * as a prize id.
 *
 * Two instances of the documented "one register is both value and next
 * displacement" trap appear, at 0x020001f4-0x020001fe and
 * 0x0200029e-0x020002a8: r2 is loaded with 448, used as the displacement into
 * the IWRAM workspace, and only then adjusted to the value stored (256 and 521
 * respectively).  Neither is 448 - 192 + 73.
 *
 * The epilogue is `pop {r3, r5} / mov r8, r3 / mov sl, r5 / pop {r5, r6, r7} /
 * pop {r1} / bx r1`; the popped register is r1, so r0 survives and is the
 * result.  r0 is set to 0 immediately before it, so the owner returns the
 * constant 0 as s32.  r0 is written before any read, so it takes no argument.
 *
 * Uncertainties:
 *  - r7, r8 and sl each carry two unrelated lifetimes on mutually exclusive
 *    paths (sl and r8 hold BLDCNT's value and address on the 0xbd path; r7 is
 *    BLDALPHA's address there and the prize-list cursor on the other; r8 is the
 *    -1 terminator there).  They are split into separate variables here.
 *  - Func_0808a360, Func_0808a370 and Func_080b0060 are called with no argument
 *    register set and are spelled with no arguments.
 *  - The 0x02000240 workspace is read three ways in this owner - as s16 at
 *    elements 224 and 225, as an s32 at byte offset 16, and as an s8 list at
 *    byte offset 300.  It is declared s16, matching the byte-exact siblings,
 *    and the other two views are spelled as casts rather than as a second
 *    declaration.
 *  - The thresholds 0x4e1f (19999) and 0x1387 (4999) select one of three
 *    Func_080f9010 fanfares; the boundaries are `>` tests, so 20000 and 5000
 *    are the first values of the higher tiers.
 *  - The imports' interfaces are unknown, hence the old-style declarations.
 */

#include "types.h"

extern s16 Data_02000240[];     /* RAM workspace; element 224 is the map id */
extern s32 Data_02001000;       /* balance snapshot, written by Func_020009f8 */

s32 Func_080770c0();
s32 Func_02000d70();
void Func_0808a010();
void Func_0808a018();
void Func_0808a020();
void Func_0808a060();
u8 *Func_0808a080();
void Func_0808a0d0();
void Func_0808a0f0();
void Func_0808a100();
void Func_0808a158();
void Func_0808a170();
void Func_0808a180();
void Func_0808a1b8();
void Func_0808a360();
void Func_0808a370();
void Func_0808a398();
void Func_08015120();
void Func_080b0060();
void Func_080f9010();
void Func_020016a8();
void Func_020008f8();

s32 Func_020001d8(void)
{
    u8 *actor;
    s8 *prize;
    s8 *cursor;
    s32 delta;
    s32 id;

    if (Data_02000240[224] == 0xbd) {
        /* 448 is the displacement; 256 is the value.  See the note above. */
        *(s32 *)(*(char **)0x03001ebc + 448) = 256;

        *(volatile u16 *)0x04000050 = 0x3f42;   /* BLDCNT */
        *(volatile u16 *)0x04000052 = 0x080c;   /* BLDALPHA: EVA 12, EVB 8 */

        Func_0808a100(24, 2);
        Func_0808a100(25, 2);

        actor = Func_0808a080(24);
        *(s32 *)(actor + 24) = (s32)0xffff0000;     /* -1.0 in 16.16 */
        actor = Func_0808a080(25);
        *(s32 *)(actor + 24) = (s32)0xffff0000;

        *(u8 *)(actor + 35) = 2;
        actor = Func_0808a080(24);
        *(u8 *)(actor + 35) = 2;
        actor = Func_0808a080(25);

        Func_0808a360();

        if (Data_02000240[225] == 1) {
            Func_020016a8();
            if (Func_080770c0(512) != 0) {
                *(volatile u16 *)0x04000050 = 0x3f42;
                *(volatile u16 *)0x04000052 = 0x1000;   /* EVA 0, EVB 16 */
            }
        }
        return 0;
    }

    if (Func_080770c0(2384) != 0) {         /* 149 << 4 */
        Func_0808a0f0(17, 0, 0);
    }

    *(volatile u8 *)0x03001d18 = 1;

    /* Again 448 as the displacement, 521 as the value. */
    *(s32 *)(*(char **)0x03001ebc + 448) = 521;

    if (Data_02000240[225] == 10) {
        Func_0808a158(8, 1);
        Func_0808a158(9, 2);
    }

    /* Reloaded as an unsigned halfword after the block above. */
    if ((u16)Data_02000240[225] == 13 && Func_080770c0(0x109) == 0) {
        Func_0808a018();
        Func_0808a158(8, 1);
        Func_0808a158(9, 2);
        Func_0808a360();
        Func_0808a370();
        Func_0808a010(10);
        Func_0808a0d0(0, 0x78, 0x70);
        Func_0808a010(20);

        delta = *(s32 *)((char *)Data_02000240 + 16) - Data_02001000;

        if (delta > 0) {
            if (delta > 0x4e1f) {
                Func_080f9010(93);
            } else if (delta > 0x1387) {
                Func_080f9010(92);
            } else {
                Func_080f9010(91);
            }
            Func_0808a010(20);
            Func_0808a170(0xe13);
            Func_08015120(delta, 5);
            Func_0808a180(9, 0);
            Func_080b0060();
        } else if (delta < 0) {
            Func_0808a170(0xe14);
            Func_08015120(-delta, 5);
            Func_0808a180(9, 0);
        }

        Func_0808a020();
    }

    if (Data_02000240[225] != 12 || Func_080770c0(0x109) != 0) {
        return 0;
    }

    prize = (s8 *)Data_02000240 + 300;      /* 150 << 1 */

    Func_0808a018();
    Func_0808a360();
    Func_0808a370();
    Func_0808a010(10);

    /* Both early exits BRANCH to the shared close at 0x0200045e
     * (`b.n` at 0x020003c4, `beq` at 0x020003cc); there is one call site. */
    if (*prize == -1) {
        Func_020008f8(1);
        goto finish;
    }

    if (*prize == -2) {
        goto finish;
    }

    Func_0808a170(0xe2e);
    Func_0808a180(8, 0);

    if (*prize != -1) {
        cursor = prize;
        do {
            /* First entry gets its own lead-in line.  These are two SEPARATE
             * call sites (0x020003ee and 0x020003f6), not one. */
            if (cursor == prize) {
                Func_0808a170(0xe2f);
            } else {
                Func_0808a170(0xe30);
            }

            id = Func_02000d70(*cursor);
            Func_08015120(id, 2);
            Func_0808a180(8, 0);
            Func_0808a398(id, 3);
            Func_0808a060(id, 0);
            Func_0808a010(10);
            Func_0808a1b8(0, 0xc000, 0);    /* 192 << 8 */

            cursor++;
            Func_0808a010(30);
        } while (*cursor != -1);
    }

    /* Mark the list consumed: -2 is the "already collected" state tested above. */
    *prize = (s8)254;

    Func_0808a170(0xe31);
    Func_0808a180(8, 0);

finish:
    return 0;
    Func_0808a020();
}
