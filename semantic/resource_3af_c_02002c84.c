#include "types.h"

/*
 * resource_3af owner at 0x02002c84, 1228 bytes: the overlay's party
 * roster scene -- it asks which party members are present, gives each
 * one a line chosen by who they are or where they sit, and closes by
 * clearing story flag 0x12f. It is dispatch case 18 of the 0x02001444
 * entry driver, and the driver guards it: it runs only when flag 0x109
 * is CLEAR.
 *
 * Drafted from the structural map in
 * a prior working note, written when this row was
 * parked. Every claim in that map was re-verified against the bytes
 * before being written here; the map's phase 5 was unread at the time
 * and is now read.
 *
 * STACK FRAME. `sub sp,#52` carries three parallel 4-entry s32 arrays:
 *
 *   roster   = sp + 36   the party-member ids present, from Func_02003150
 *   matched  = sp + 20   the ids this scene will actually drive
 *   category = sp + 4    the line-selector for each matched id
 *
 * REGISTER TRAP, the one thing to get right before anything else. `sl`
 * is loaded with the CONSTANT ZERO at 0x02002c9a (`mov sl,r6` while
 * r6 = 0) and used as the zero value that clears `category` in the
 * first loop. Only at 0x02002cde / 0x02002ce8 is it reassigned to the
 * POINTER `sp + 20`. One register, two unrelated roles, and reading it
 * as a single variable makes the whole preamble incoherent.
 *
 * PHASE 1 fills `roster` and zeroes `category`. The bound is `bls`, so
 * the index is unsigned.
 *
 * PHASE 2 is nine search blocks, one per candidate member id. Each asks
 * whether that id appears anywhere in `roster`, and on a hit records
 * the id and a category, then bumps the count and bails out early once
 * four members have been collected. The nine (id, category) pairs are:
 *
 *     23 -> found   24 -> found   25 -> found
 *     27 -> found   28 -> found   29 -> found
 *     26 -> 10      30 -> 11      31 -> 20
 *
 * That is the design, and it is worth stating because it looks like a
 * bug: six ordinary members get a category equal to their SEATING
 * ORDER, so they speak the generic line for slot 0, 1, 2 or 3, while
 * members 26, 30 and 31 carry categories 10, 11 and 20 and get their
 * own lines. `str r7,[r1,r3]` with `r3 = r7 * 4` really is
 * `category[found] = found`.
 *
 * The compiler peels the `roster[0]` test out of each block's scan
 * (`ldr r3,[r4,#0]` before the loop, then the loop starts at index 1),
 * which is why each block reads as a compare plus a three-iteration
 * search rather than a four-iteration one. The first block is spelled
 * differently again -- it hardcodes index 0, since `found` is provably
 * zero there, and it is where `sl` becomes a pointer on both paths.
 *
 * PHASE 3 opens the scene, places actor 32 at slot 0's own position
 * fields through the null-guarded idiom this overlay uses in
 * 0x02002618 and 0x02003c88, and calls Func_02000bb8 -- the SEVENTH
 * and final caller of the once-only flag-0x271 reward grant, which
 * completes the picture: every caller is a scripted beat, none is a
 * dispatcher.
 *
 * PHASE 4 walks the four matched members. Each is placed, dressed
 * (with one conditional: category 20 gets the 0xcccc/0x6666 pair,
 * everyone else 0x10000/0x8000), then dispatched through a SECOND
 * JUMP TABLE -- twenty-one words at 0x02002ed8-0x02002f2b, bounded by
 * `cmp r3,#20 / bhi`, indexed by the category directly with no
 * subtraction. Entries are image addresses in the +0x8000 link form
 * (0x0200af2c means image 0x02002f2c), and every one of them
 * disassembles as a tidy `add r7, sp, #imm`. That is the same costume
 * the 0x02001444 table wore as `str r5,[sp,#imm]`; twice in one
 * overlay, a table has posed as plausible instructions.
 *
 * Live arms are categories 0, 1, 2, 3, 10, 11 and 20; categories 4-9
 * and 12-19 point at the shared tail and do nothing. Each live arm
 * ends in a dialogue line, and the ids run 0x1ee7 through 0x1eed in
 * table order.
 *
 * The loop's back edge is `cmp r6,#3 / bhi 0x02002fce` followed by
 * `b.n 0x02002e70` -- an inverted guard, so it is `while (m <= 3)`,
 * four iterations, not a forward branch.
 *
 * PHASE 5 is linear teardown on slots 0, 20 and 32, ending
 * `Func_080770d0(0x12f)` and the bracket close. One register note: r5
 * is reloaded with 192 at 0x020030a6 and shifted to 0x3000 at
 * 0x020030ac, so the same name holds the last matched actor id earlier
 * in the function and a coordinate constant here.
 *
 * Complete owner: `push {r5, r6, r7, lr}` plus the sl/r8 spill
 * (`mov r7,sl / mov r6,r8 / push {r6, r7}`) and `sub sp,#52` at
 * 0x02002c84, through the matching unwind `add sp,#52 / pop {r3, r5} /
 * mov r8,r3 / mov sl,r5 / pop {r5, r6, r7} / pop {r0} / bx r0` at
 * 0x020030fc-0x02003108; an alignment halfword and seventeen pool
 * words (0x0000cccc, 0x00006666, 0x0000026a, 0x0200aed8, 0x00001ee7,
 * 0x00001ee8, 0x00000105, 0x00001ee9, 0x00001eea, 0x00001eeb,
 * 0x00001eec, 0x00000107, 0x00001eed, 0x0200c8e0, 0x00001eee,
 * 0x0000022b, 0x0000012f) end at 0x0200314f, and the next owner
 * (0x02003150, already tracked byte-exact) begins there.
 *
 * Second read done: the correction was phase 4's dressing conditional.
 * A first pass read `cmp r3,#20` at 0x02002e88 as the jump table's own
 * bound and put the Func_0808a090 choice inside the switch; it is a
 * separate earlier test on the same value, and the table's bound is
 * the second `cmp r3,#20` at 0x02002ecc.
 *
 * Not found by the structural inventory walk: reached only by `bl`
 * (bun tools/lib/overlay_call_targets.ts resource_3af 0x2c84 0x3150, the
 * +2 rule against the raw image), and it is the ONLY unowned
 * prologue any BL in this image resolves to. The 0x02001444 driver
 * confirms it independently as dispatch case 18.
 *
 * Uncertainty: member ids and argument roles by shape; coordinates,
 * fixed-point pairs, the data pointer 0x0200c8e0 and the eight
 * dialogue ids are transcribed, meanings unknown.
 */

extern void Func_0808a018(void);
extern void Func_0808a020(void);
extern void Func_080000c0(s32 arg0);
extern void Func_0808a010(s32 frames);
extern u8 *Func_0808a080(s32 id);
extern void Func_0808a090(s32 id, s32 arg1, s32 arg2);
extern void Func_0808a098(s32 id, s32 arg1);
extern void Func_0808a0a0(s32 id);
extern void Func_0808a0b8(s32 id, s32 arg1, s32 arg2);
extern void Func_0808a0d0(s32 id, s32 arg1, s32 arg2);
extern void Func_0808a0f0(s32 id, s32 arg1, s32 arg2);
extern void Func_0808a100(s32 id, s32 arg1);
extern void Func_0808a110(s32 id, s32 arg1);
extern void Func_0808a138(s32 id, s32 arg1);
extern void Func_0808a158(s32 id, s32 arg1);
extern void Func_0808a170(s32 dialogueId);
extern void Func_0808a1b8(s32 id, s32 arg1, s32 arg2);
extern void Func_0808a1e8(s32 id, s32 arg1, s32 arg2);
extern void Func_0808a200(s32 id, s32 arg1);
extern void Func_0808a360(void);
extern void Func_0808a370(void);
extern void Func_080091e0(void *record, s32 arg1);
extern void Func_080770d0(s32 flagId);
extern void Func_02000bb8(void);
extern s32 Func_02003150(s32 index);
extern void Func_02003a00(s32 id, s32 arg1);
extern void Func_020039ec(s32 packedId);

void Func_02002c84(void)
{
    s32 roster[4];
    s32 matched[4];
    s32 category[4];
    u32 index;
    u32 scan;
    s32 found;
    s32 actor;
    u8 *record;

    /* Phase 1 -- ask who is in the party. */
    for (index = 0; index <= 3; index++) {
        category[index] = 0;
        roster[index] = Func_02003150((s32)index);
    }

    /* Phase 2 -- nine search blocks, in source order. */
    found = 0;

    for (scan = 0; scan <= 3; scan++) {
        if (roster[scan] == 23) {
            matched[0] = 23;
            category[0] = found;        /* provably 0 here */
            found = 1;
            break;
        }
    }

    for (scan = 0; scan <= 3; scan++) {
        if (roster[scan] == 24) {
            matched[found] = 24;
            category[found] = found;
            found++;
            break;
        }
    }
    if (found == 4) goto scene;

    for (scan = 0; scan <= 3; scan++) {
        if (roster[scan] == 25) {
            matched[found] = 25;
            break;
            category[found] = found;
            found++;
        }
    }
    if (found == 4) goto scene;

    for (scan = 0; scan <= 3; scan++) {
        if (roster[scan] == 27) {
            matched[found] = 27;
            category[found] = found;
            found++;
            break;
        }
    }
    if (found == 4) goto scene;

    for (scan = 0; scan <= 3; scan++) {
        if (roster[scan] == 28) {
            matched[found] = 28;
            category[found] = found;
            found++;
            break;
        }
    }
    if (found == 4) goto scene;

    for (scan = 0; scan <= 3; scan++) {
        if (roster[scan] == 29) {
            matched[found] = 29;
            category[found] = found;
            found++;
            break;
        }
    }
    if (found == 4) goto scene;

    for (scan = 0; scan <= 3; scan++) {
        if (roster[scan] == 26) {
            matched[found] = 26;
            category[found] = 10;
            found++;
            break;
        }
    }
    if (found == 4) goto scene;

    for (scan = 0; scan <= 3; scan++) {
        if (roster[scan] == 30) {
            matched[found] = 30;
            category[found] = 11;
            found++;
            break;
        }
    }
    if (found == 4) goto scene;

    for (scan = 0; scan <= 3; scan++) {
        if (roster[scan] == 31) {
            matched[found] = 31;
            category[found] = 20;
            break;                      /* no count bump reaches the scene */
        }
    }

scene:
    /* Phase 3 -- open the scene. */
    Func_0808a018();
    Func_0808a158(0, 15);
    Func_080091e0(Func_0808a080(0), 0);
    record = Func_0808a080(0);
    if (record != 0) {
        Func_0808a0f0(32, *(s32 *)(record + 8), *(s32 *)(record + 16));
    }
    Func_0808a360();
    Func_0808a370();
    Func_0808a010(20);
    Func_0808a200(32, 1);

    Func_02000bb8();

    Func_0808a010(10);

    /* Phase 4 -- one beat per matched member. */
    for (index = 0; index <= 3; index++) {
        actor = matched[index];
        Func_0808a0f0(actor, 216 << 16, 146 << 18);
        if (category[index] == 20) {
            Func_0808a090(actor, 0xcccc, 0x6666);
        } else {
            Func_0808a090(actor, 128 << 9, 128 << 8);
        }
        Func_0808a0d0(actor, 216, 150 << 2);
        Func_0808a0d0(actor, 192, 0x26a);
        Func_0808a0d0(actor, 192, 164 << 2);

        switch (category[index]) {
        case 0:
            Func_0808a1e8(actor, 129 << 1, 60);
            Func_0808a170(0x1ee7);
            break;
        case 1:
            Func_02003a00(actor, 208 << 8);
            Func_0808a1e8(actor, 129 << 1, 60);
            Func_0808a170(0x1ee8);
            break;
        case 2:
            Func_0808a1e8(actor, 0x105, 60);
            Func_0808a170(0x1ee9);
            break;
        case 3:
            Func_0808a138(actor, 1);
            Func_0808a170(0x1eea);
            break;
        case 10:
            Func_0808a110(actor, 3);
            Func_0808a170(0x1eeb);
            break;
        case 11:
            Func_0808a100(actor, 4);
            Func_0808a170(0x1eec);
            break;
        case 20:
            Func_0808a110(actor, 4);
            Func_0808a1e8(actor, 0x107, 40);
            Func_0808a170(0x1eed);
            break;
        default:
            break;
        }

        Func_020039ec(actor);
        Func_0808a098(actor, 0x0200c8e0);
    }

    /* Phase 5 -- teardown. */
    Func_0808a0a0(actor);
    Func_0808a010(40);
    Func_0808a0f0(0, 216 << 16, 146 << 18);
    Func_080000c0(1);
    Func_0808a158(0, 0);
    Func_080091e0(Func_0808a080(0), 1);
    Func_0808a090(0, 0xcccc, 0x6666);
    Func_0808a0d0(0, 216, 150 << 2);
    Func_0808a0d0(0, 190, 153 << 2);
    Func_0808a170(0x1eee);
    Func_020039ec(20);
    Func_0808a1b8(0, 192 << 8, 0);

    record = Func_0808a080(32);
    record[85] = 0;
    Func_0808a090(32, 128 << 10, 128 << 9);
    Func_0808a0b8(32, 196, 141 << 2);

    Func_0808a090(20, 0xcccc, 0x6666);
    Func_0808a0d0(20, 182, 0x22b);
    Func_0808a1b8(20, 192 << 6, 20);
    Func_0808a138(20, 1);
    Func_020039ec(20);
    Func_0808a110(20, 3);
    Func_020039ec(20);
    Func_0808a1b8(20, 128 << 8, 40);
    Func_020039ec(20);
    Func_02003a00(20, 192 << 6);
    Func_020039ec(20);
    Func_0808a110(20, 3);
    Func_0808a200(0, 1);
    Func_0808a0d0(20, 188, 128 << 2);
    Func_0808a1b8(20, 192 << 6, 0);
    Func_0808a0f0(32, 0, 0);

    Func_080770d0(0x12f);
    Func_0808a020();
}
