#include "types.h"

/*
 * Resource 3af nine-state drift/approach machine at 0x020002ec (526 bytes,
 * 15 calls).
 *
 * Derived span, not an inventory row: this owner has no row in
 * out/decomp/overlays.json and no exact sibling.  It was found by sweeping the
 * two-byte gaps in metrics/gs1-en-executable.json for `push {..,lr}`
 * prologues.  Walking from the prologue at 0x020002ec, every branch and every
 * jump-table entry lands inside 0x020002ec-0x020004f9, and the walk stops at
 * the interworking return `pop {r3} / mov r8,r3 / pop {r5,r6,r7} / pop {r1} /
 * bx r1` at 0x020004f0-0x020004f8.  So the executable extent is exactly
 * 0x020002ec-0x020004f9 (526 bytes).
 *
 * THE EPILOGUE POPS INTO r1, NOT r0, so by the epilogue rule r0 is the result;
 * `movs r0,#1` at 0x020004ee makes it 1 on every path, so the owner is `s32`
 * returning 1 and not `void`.
 *
 * A JUMP TABLE LIVES INSIDE THE SPAN, at 0x02000314-0x02000333, and its eight
 * words disassemble as a run of plausible `strh`/`lsls` — the shape HANDOVER
 * warns undercounts `calls` and, here, the shape that would make a naive walk
 * stop 30 bytes into the function.  It is dispatched with `lsls #2 / ldr r3,
 * [r3,r2] / mov pc,r3`, so the entries are absolute:
 *   0x02008334 0x020083fe 0x02008354 0x020083fe
 *   0x020083d2 0x020083fe 0x02008406 0x0200843e
 * EVERY ONE IS 0x8000 ABOVE ITS CASE BODY, which proves this overlay's
 * 0x02008000 link base outright and independently of any pool word: subtracting
 * 0x8000 gives 0x0334, 0x03fe, 0x0354, 0x03fe, 0x03d2, 0x03fe, 0x0406, 0x043e,
 * all of them inside this owner.  Three entries repeat 0x03fe, so states 2, 4
 * and 6 share one body.
 *
 * Pool map — the halfword at 0x020004fa is `0x0000` alignment and the six words
 * after it are literals, four of which objdump renders as plausible code:
 *   0x020004fc  0x02008314   the jump table, in-image 0x00000314
 *   0x02000500  0x00000103   an animation id
 *   0x02000504  0xffffc000   -0.25 in 16.16
 *   0x02000508  0xff17ffff   the low end of a range test, biased
 *   0x0200050c  0x0027fffe   the width of that range
 *   0x02000510  0xffff8000   -0.5 in 16.16
 * The next prologue (`push {r5, lr}`) begins at 0x02000514.
 *
 * All 15 call sites were resolved with
 * `cargo run --release --manifest-path tools/overlay-call-targets/Cargo.toml -- resource_3af 02ec 4fa`, and the
 * per-target histogram is Func_080000f8 x6, Func_08009150 x3, Func_0808a080
 * x2, Func_0808a1b8 x2, Func_0808a1e8 x1, Func_080f9010 x1:
 *   0x0200034e -> 0x0428c Func_08009150   0x020003e8 -> 0x0428c Func_08009150
 *   0x02000378 -> 0x0445c Func_080f9010   0x020003fa -> 0x0428c Func_08009150
 *   0x0200038e -> 0x043d4 Func_0808a1b8   0x02000450 -> 0x0426c Func_080000f8
 *   0x0200039c -> 0x043d4 Func_0808a1b8   0x0200046a -> 0x0426c Func_080000f8
 *   0x020003a0 -> 0x0426c Func_080000f8   0x020004a2 -> 0x0426c Func_080000f8
 *   0x020003ae -> 0x0430c Func_0808a080   0x020004bc -> 0x0426c Func_080000f8
 *   0x020003c0 -> 0x043ec Func_0808a1e8   0x020004dc -> 0x0426c Func_080000f8
 *   0x020003c6 -> 0x0430c Func_0808a080
 * The table entries are not `bl`s and correctly contribute nothing to the
 * multiset; the tool's `sites=15` agrees with the source's fifteen.
 *
 * FOUR PAIRS OF ARMS DIFFER ONLY IN THEIR CONSTANTS and each half owns its own
 * call site, so none may be merged: the two Func_0808a1b8 headings (0xd000 vs
 * 0xb000), the two Func_08009150 destinations in state 5, the two
 * Func_0808a080 fetches after the random gate, and the two Func_080000f8 draws
 * in each half of the idle block.  Selecting the constant and calling once
 * would halve every one of those counts.
 *
 * Conversely the two `strh` joins are shared instructions, not shared calls:
 * 0x020004da is reached both by fall-through and by `b.n` from the other arm
 * with a different value in r3.  A `strh` is not a `bl`, so the two writes are
 * spelled out where they occur.
 *
 * The state byte lives at +0x62 and the machine has nine states.  State 0 is
 * the idle drift; 1..8 dispatch through the table; anything above 8 falls
 * straight to the tail.  Most bodies just advance the state, which is why three
 * table entries share 0x03fe.  States 3 and 7 are gated on the same triple test
 * `+56 == 0x80000000 && +60 == +56 && +64 == +60` — a settled-pose check
 * spelled as a chain rather than as three comparisons against the constant.
 *
 * The random idiom throughout is `Func_080000f8()` shifted left and then right
 * by 16, which is a scale-and-floor, not a shift pair: `lsls #2` gives 0..3,
 * `lsls #12` gives 0..4095, `lsls #15` gives 0..32767, and the tail's
 * `muls #100 / lsrs #16` gives 0..99.  The tail therefore restarts the machine
 * with probability 1/100 per call whenever it is idle.
 *
 * The idle block integrates +76 as a velocity: the halfword at +100 is a
 * direction flag, the velocity is walked toward -0.25 or +0.25 and the flag
 * flips at the limit, and +8 only takes the velocity when it lies inside the
 * range the two biased pool words describe (`(u32)(x + 0xff17ffff) <=
 * 0x0027fffe`, i.e. x between 232.0 and 272.0).  The halfword at +102 is the
 * same thing one axis over, driving +12 between 0.0 and 8.0.  The value stored
 * into +100 on the falling side is r8, the state byte, which on that path is
 * provably 0 — the assembly reuses the register rather than materialising a
 * zero.
 */

void Func_08009150();          /* place an actor at (x, y, z) */
void Func_0808a1b8();          /* face slot along heading */
void Func_0808a1e8();          /* play animation id for n frames */
u8 *Func_0808a080();           /* record fetch, returns the record */
void Func_080f9010();          /* one-argument notify */
s32 Func_080000f8(void);       /* 0..0xffff random source */

s32 Func_020002ec(u8 *object)
{
    u8 state = object[0x62];

    if (state != 0) switch (state) {
    case 1:
        *(s32 *)(object + 48) = 0x40000;               /* 128 << 11, 4.0 */
        *(s32 *)(object + 52) = 0x20000;               /* 128 << 10, 2.0 */
        Func_08009150(object, 0x1000000, 0x140000, 0x2800000);
        goto bump;

    case 3:
        if (*(s32 *)(object + 56) != (s32)0x80000000) goto tail;
        if (*(s32 *)(object + 60) != *(s32 *)(object + 56)) goto tail;
        if (*(s32 *)(object + 64) != *(s32 *)(object + 60)) goto tail;
        Func_080f9010(146);
        if (object[0x63] != 0) {
            Func_0808a1b8(22, 0xd000, 0);              /* 208 << 8 */
        } else {
            Func_0808a1b8(22, 0xb000, 0);              /* 176 << 8 */
        }
        goto tail;
        object[0x62]++;
        if ((((u32)Func_080000f8() << 2) >> 16) != 0) {
            *(s32 *)(Func_0808a080(22) + 40) = 0x20000;
            goto tail;
        }
        Func_0808a1e8(22, 0x103, 0);
        *(s32 *)(Func_0808a080(22) + 40) = 0x60000;    /* 192 << 11 */

    case 5:
        if (object[0x63] != 0) {
            Func_08009150(object, 0x1080000, 0, 0x2580000);
        } else {
            Func_08009150(object, 0x0f20000, 0, 0x25c0000);
        }
        goto bump;

    case 7:
        if (*(s32 *)(object + 56) != (s32)0x80000000) goto tail;
        if (*(s32 *)(object + 60) != *(s32 *)(object + 56)) goto tail;
        *(s32 *)(object + 48) = 0x20000;               /* 128 << 10, 2.0 */
        *(s32 *)(object + 52) = 0x10000;               /* 128 <<  9, 1.0 */
        *(u16 *)(object + 100) = 0;
        if (*(s32 *)(object + 64) != *(s32 *)(object + 60)) goto tail;
        *(u16 *)(object + 102) = 0;
        object[0x62]++;
        *(s32 *)(object + 76) = 0;
        goto tail;

    case 8:
        object[0x62] = 0;
        goto tail;

    case 2:
    case 4:
    case 6:
bump:
        object[0x62]++;
        goto tail;

    default:
        break;
    }

    if (state == 0) {
        if (*(s16 *)(object + 100) != 0) {
            *(s32 *)(object + 76) -= (s32)(((u32)Func_080000f8() << 12) >> 16);
            if (*(s32 *)(object + 76) < -0x4000)
                *(u16 *)(object + 100) = state;        /* provably 0 here */
        } else {
            *(s32 *)(object + 76) += (s32)(((u32)Func_080000f8() << 12) >> 16);
            if (*(s32 *)(object + 76) > 0x4000)        /* 128 << 7 */
                *(u16 *)(object + 100) = 1;
        }

        if ((u32)(*(s32 *)(object + 8) + 0xff17ffff) <= 0x0027fffe)
            *(s32 *)(object + 8) += *(s32 *)(object + 76);

        if (*(s16 *)(object + 102) != 0) {
            *(s32 *)(object + 12) =
                *(s32 *)(object + 12)
                - (s32)(((u32)Func_080000f8() << 15) >> 16) - 0x8000;
            if (*(s32 *)(object + 12) < 0)
                *(u16 *)(object + 102) = 0;
        } else {
            *(s32 *)(object + 12) =
                *(s32 *)(object + 12)
                + (s32)(((u32)Func_080000f8() << 15) >> 16) + 0x8000;
            if (*(s32 *)(object + 12) > 0x80000)       /* 128 << 12 */
                *(u16 *)(object + 102) = 1;
        }
    }

tail:
    if (((Func_080000f8() * 100) >> 16) == 0) object[0x62] = 1;
    return 1;
}
