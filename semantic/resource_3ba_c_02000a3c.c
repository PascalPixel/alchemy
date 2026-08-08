#include "types.h"

/*
 * resource_3ba owner at 0x02000a3c, 880 bytes (0x02000a3c-0x02000dab):
 * code, one embedded five-entry jump table at 0x02000c5c-0x02000c6f (branched
 * over by the dispatch itself), and the eleven-word literal pool at
 * 0x02000d88 that closes the row.
 *
 * Prologue `push {r5, r6, r7, lr}` plus `mov r7, sl / mov r6, r9 /
 * mov r5, r8 / push {r5, r6, r7}` and `sub sp, #12`; unwind at
 * 0x02000d76..0x02000d84.  The interworking return is `pop {r1} ; bx r1`, so
 * r0 survives the epilogue and IS the result — and `movs r0, #0` immediately
 * precedes it on the single exit path, so this owner always returns 0.  Every
 * arm branches to that one exit.  No argument register is read before being
 * written, so it takes none.
 *
 * LINK BASE 0x02008000 — this owner supplies the cheapest witness of all, a
 * jump-table base.  The dispatch loads the pool word 0x02008c5c and indexes it
 * by `(scene - 1) * 4`; the table physically sits at file offset 0x0c5c, i.e.
 * 0x02008c5c - 0x8000.  Its five entries (0x02008c70, 0x02008d10, 0x02008d42,
 * 0x02008d58, 0x02008d66) are file offsets 0x0c70/0x0d10/0x0d42/0x0d58/0x0d66,
 * all inside this owner, and none carries the Thumb bit — correct, because
 * `mov pc, r3` does not interwork.  Two further pool words agree: 0x0200804d
 * is Func_0200004c + 1 (a task callback, matching the byte-exact
 * `assets/code/resource_3ba_c_02000158.c` verbatim, rate 0xc80 and all) and
 * 0x020099e1 is Func_020019e0 + 1, whose byte-exact source is also in
 * assets/code.  0x03001ebc is the IWRAM pointer CELL, dereferenced once to
 * reach the workspace, and 0x02000240 is the shared RAM `s16[]` table.
 *
 * CALL ACCOUNTING.  Sixty-two `bl` sites, all resolved with
 * `cargo run --release --manifest-path tools/overlay-call-targets/Cargo.toml -- resource_3ba --annotate`:
 *   Func_080091c0 x8, Func_0808a088 x7, Func_0808a080 x7, Func_080770c0 x6,
 *   Func_02000a10 x3, Func_0808a588 x3, Func_08009080 x3,
 *   Func_02002ea0 x2, Func_02002738 x2, Func_02001910 x2, Func_02001898 x2,
 *   Func_02000974 x2, Func_0808a248 x2, Func_080000d0 x2,
 *   and one each of Func_0200384c, Func_02003764, Func_02001d20,
 *   Func_02000db8, Func_080f9010, Func_080770e0, Func_080770c8,
 *   Func_080091e0, Func_080091c8, Func_080091b8 and Func_080091a8.
 * The C below reproduces that multiset exactly.  The inventory's `calls=27` is
 * the usual low lower bound and is not used as the proof.
 *
 * SHAPE.  The overlay's scene entry point.  It clears the workspace word at
 * +448, sets story flag 0x144, then stages a fixed cast: actor 9 is grounded
 * only if it is already at ground level and the terrain query agrees; actor 10
 * is placed at a column read from story value 0x310 (defaulting to 25) and the
 * per-frame task Func_0200004c is installed; actors 15, 17, 18, 13 and 14 are
 * posed, three of them gated on story flags 0x303, 0x302 and 0x301.  Finally
 * it dispatches on the shared scene selector `Data_02000240[225]`, values 1-5,
 * each arm being one scene variant; anything else falls straight to the exit.
 *
 * UNCERTAINTIES.
 *  - Three intra-overlay callees have byte-exact sources that take no
 *    arguments, yet these call sites set r0: Func_0200384c (0xe4),
 *    Func_02000a10 (1, 2, 3) and Func_02001898 (nothing — that one agrees).
 *    The register writes are preserved as arguments here, with old-style
 *    declarations, because the byte-exact reconstructions may simply not have
 *    needed the parameter; the discrepancy is recorded rather than resolved.
 *  - Record fields asserted: position words at +8, +12 and +16, the state
 *    bytes at +0x22, +0x23, +0x55 and +0x59, and the motion-rate pair at
 *    +0x30/+0x34 (0xcccc / 0x6666, the same pair as 0x02000270).
 *  - Func_080091a8's result is only tested against zero; the byte stored to
 *    +0x55 in that arm is that same (zero) register.
 *  - The `bls` guarding the dispatch is unsigned on `selector - 1`, so a
 *    selector of 0 or a negative one falls through to the exit.
 */

/* Import veneers, named by the main-image function each one reaches.
 * Old-style declarations: arities vary between call sites in this overlay. */
void Func_020046bc();
u8 *Func_0200471a();
s32 Func_02004656();
void Func_02004696();
s32 Func_0200470e();
u8 *Func_0200476c();
void Func_020046e0();
void Func_02004602();
u8 *Func_020047b0();
s32 Func_02004760();
void Func_0200472c();
void Func_02004736();
u8 *Func_020047fc();
void Func_02004760_b();
u8 *Func_02004826();
void Func_0200478a();
s32 Func_020047f0();
void Func_020047a8();
void Func_020047ba();
void Func_020047c4();
u8 *Func_02004892();
s32 Func_0200483a();
void Func_020047f2();
void Func_020047be();
void Func_020047c8();
u8 *Func_020048de();
void Func_02003ad4();
void Func_02003adc();
void Func_020043ee();
void Func_0200489e();
void Func_02004964();
void Func_0200496a();
void Func_02004970();
void Func_02004976();
void Func_0200497c();
s32 Func_0200491a();
void Func_02004ac4();
void Func_020025e2();
void Func_0200256e();
void Func_020016ec();
void Func_020016f2();
void Func_020016f8();
void Func_02003426();
void Func_02004ade();
void Func_02004ae6();
void Func_02004aee();
void Func_02004558();
void Func_02004828();
void Func_020049de();
void Func_020049e4();
s32 Func_02004982();
void Func_020025ca();
void Func_02002648();
void Func_02003476();
s32 Func_0200499e();
void Func_02001b08();
void Func_02002a74();
void Func_020016d0();
void Func_02004aea();
void Func_020016e0();
void Func_02004afa();
                                /* scene record for an actor selector */
                     
                                /* scene request keyed by a selector */
                     
                                /* select presentation mode (record, mode) */
                                /* tile height at (x, z) */
                                /* six-argument renderer, last two on stack */
                                /* six-argument renderer, last two on stack */
                                /* six-argument scene presentation request */
                                /* set presentation phase (record, phase) */
                                /* install a per-frame task (callback, rate) */
                                /* set a story flag */
                                /* test a story flag */
                                /* read a story value */
                                /* play a cue */

/* This overlay's own routines.  Those marked (bx) have byte-exact sources in
 * assets/code; see the argument-count note above. */
                                /* bx */
                                /* bx */
                     
                                /* bx */
                     
                                /* bx */
                                /* bx */

                                /* bx */

/* The per-frame tasks installed here; in-image code, Thumb-tagged in the
 * pool as 0x0200804d and 0x020099e1. */
void Func_0200004c(void);
void Func_020019e0(void);

/* The IWRAM pointer cell holding the overlay workspace. */
extern u8 *Data_03001ebc;

/* The shared RAM table; index 225 is the scene selector. */
extern s16 Data_02000240[];

s32 Func_02000a3c(void)
{
    u8 *workspace;
    u8 *record;
    s32 column;
    s32 scene;

    workspace = Data_03001ebc;
    *(s32 *)(workspace + 448) = 0;              /* 224 << 1 */

    Func_020046bc(0x144);                       /* 162 << 1 */

    record = Func_0200471a(9);
    if (*(s32 *)(record + 12) == 0
        && Func_02004656(0, *(s32 *)(record + 8),
                         *(s32 *)(record + 16)) == 0) {
        *(record + 0x23) = 2;
        *(record + 0x55) = 0;
        Func_02004696(14, 13, 1, 1,
                      *(s32 *)(record + 8) >> 20,
                      *(s32 *)(record + 16) >> 20);
    }

    column = Func_0200470e(0x310);              /* 196 << 2 */
    if (column == 0) {
        column = 25;
    }

    record = Func_0200476c(10);
    *(s32 *)(record + 8) = (column << 20) + 0x80000;
    *(record + 0x55) = 0;
    Func_020046e0(14, 13, 1, 1, column, 12);
    *(record + 0x23) = 2;

    Func_02004602((void (*)(void))((s32)&Func_0200004c | 1), 0xc80);

    record = Func_020047b0(15);
    *(record + 0x22) = 1;
    if (Func_02004760(0x303) != 0) {
        Func_020046bc(record, 4);
        Func_0200472c(record, 0);
        *(record + 0x59) = 0;
        *(record + 0x23) = 3;
        Func_02004736(47, 24, 1, 1, 47, 12);
    }

    record = Func_020047fc(17);
    *(record + 0x55) = 0;
    *(record + 0x23) = 2;
    Func_02004760_b(64, 24, 3, 1, 64, *(s32 *)(record + 16) >> 20);

    record = Func_02004826(18);
    *(record + 0x55) = 0;
    *(record + 0x23) = 2;
    Func_0200478a(63, 25, 1, 3, *(s32 *)(record + 8) >> 20, 9);

    if (Func_020047f0(0x302) != 0) {
        Func_020047a8(37, 7, 1, 4, 34, 7);
        Func_020047ba(36, 7, 1, 4, 37, 7);
        Func_020047c4(100, 29, 1, 3, 34, 38);
    }

    record = Func_02004892(13);
    if (Func_0200483a(0x301) != 0) {
        Func_020047f2(43, 12, 1, 1, 41, 12);
        *(record + 0x55) = 0;
        *(s32 *)(record + 0x34) = 0x6666;
        *(s32 *)(record + 0x30) = 0xcccc;
        *(s32 *)(record + 12) = 0x80000;
        Func_020047be(record, 3);
    } else {
        Func_020047c8(record, 2);
    }

    *(record + 0x23) = 2;
    record = Func_020048de(14);

    Func_02003ad4(24, 120);
    Func_02003adc(25, 127);

    scene = Data_02000240[225];

    switch (scene) {
    case 1:
        Func_020043ee(0, 8, 4, 0x5180000, 0xc00000, 24, 25);
        Func_0200489e(127, 0, 1, 2, 19, 2);
        Func_02004964(19);
        Func_0200496a(20);
        Func_02004970(21);
        Func_02004976(22);
        Func_0200497c(23);
        if (Func_0200491a(0x109) == 0) {
            Func_02004ac4(17);
            Func_020025e2(0);
            Func_0200256e();
            Func_020016ec(1);
            Func_020016f2(2);
            Func_020016f8(3);
            Func_02003426(1);
        }
        Func_02004ade(1, 0);
        Func_02004ae6(2, 0);
        Func_02004aee(3, 0);
        Func_02004558(0xe4);
        break;

    case 2:
        Func_02004828((void (*)(void))((s32)&Func_020019e0 | 1), 0xc80);
        Func_020049de(24);
        Func_020049e4(25);
        if (Func_02004982(0x109) == 0) {
            Func_020025ca();
            Func_02002648(1);
            Func_02003476(0);
        }
        break;

    case 3:
        if (Func_0200499e(0x109) == 0) {
            Func_02001b08(19);
            Func_02002a74();
        }
        break;

    case 4:
        Func_020016d0(1);
        Func_02004aea(4);
        break;

    case 5:
        Func_020016e0(-1);
        Func_02004afa(5);
        break;
    }

    return 0;
}
