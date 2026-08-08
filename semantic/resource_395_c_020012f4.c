#include "types.h"

/*
 * Resource 395 gesture dispatcher at 0x020012f4 (568 bytes,
 * 0x020012f4-0x0200152b).
 *
 * Complete owner: `push {lr}` prologue at 0x020012f4 and the single
 * `pop {r0} / bx r0` interworking return at 0x0200151e.  The popped return
 * address lands in r0, so nothing is returned and the owner is `void`.  There
 * is no frame and no callee-saved register is touched, so every arm reaches
 * the one shared tail at 0x02001518.  The next owner (Func_0200152c, already
 * converted) begins at 0x0200152c, which confirms the end boundary.
 *
 * POOL / TABLE MAP, derived from a CONTROL-FLOW WALK, not from the listing.
 * This owner is the exact case the HANDOVER warns about: BOTH of its embedded
 * jump tables decode as entirely plausible `str rN,[sp,#imm]` pairs, and the
 * SECOND one ends mid-row — the table at 0x02001460 runs to 0x02001477 and
 * the very next byte, 0x02001478, is table entry 0's body, with no branch and
 * no label between them.  A heuristic that ran a table to the next branch
 * target would swallow live code.  Walking from the prologue gives:
 *   0x0200130a          two-byte alignment `nop` (never executed)
 *   0x0200130c-0x0200133b   TABLE A, 12 entries, base pool word 0x0200930c
 *   0x0200133c-0x0200145f   code
 *   0x02001460-0x02001477   TABLE B, 6 entries, base pool word 0x02009460
 *   0x02001478-0x02001521   code
 *   0x02001522              two-byte alignment `nop`
 *   0x02001524-0x0200152b   the owner's two pool words
 *
 * Both table bases confirm the 0x02008000 link base: 0x0200930c against the
 * table's physical file offset 0x130c, and 0x02009460 against 0x1460.  Every
 * entry is even, as jump-table entries must be, because the `mov pc, r3`
 * dispatches at 0x02001308 and 0x0200145e do not interwork.
 *
 * Table A (r1 = 0..11), as file offsets: 0x133c 0x134e 0x1352 0x1364 0x1376
 * 0x13b2 0x13ca 0x144a 0x13e2 0x13fa 0x1412 0x144a.  Entries 7 and 11 are the
 * SAME body, so they are written as grouped `case` labels rather than as two
 * copies — duplicating them would inject a phantom call.
 * Table B (r1 = 0..5): 0x1478 0x14f8 0x1490 0x14a8 0x14c0 0x1502.
 *
 * All 57 call sites were resolved with cargo run --release --manifest-path tools/overlay-call-targets/Cargo.toml --
 * (--annotate for the listing, site->target from --json, never from the
 * summary histogram); overlay_show.ts's own `bl` annotations are wrong on
 * every overlay because an overlay `bl` stores target_offset - 2.  The tool
 * reports sites=57 distinct_targets=2 veneer=57 unknown=0.
 *
 * PLACED MULTISET, checked PER TARGET rather than by total:
 *   Func_0808a100  33 sites  = 33 C call expressions
 *   Func_080000c0  24 sites  = 24 C call expressions
 * The per-target split is what makes this owner safe: a total-only check would
 * not notice a Func_0808a100 written where a Func_080000c0 belongs, and this
 * body is 57 calls to two imports in long alternating runs.  The inventory's
 * calls=1 for this row is the documented undercount — its scan followed the
 * first dispatch into table A and read the table as code.
 *
 * FIVE SHARED CALL SITES, which is the whole reason this file uses `goto`.
 * Each is one `bl` reached from arms of BOTH tables, differing only in the
 * slot left in r0 by the arm:
 *   0x02001488 -> Func_0808a100(slot, 3)   A0 branches in; B0 falls through
 *   0x020014a0 -> Func_0808a100(slot, 5)   A2 branches in; B2 falls through
 *   0x020014b8 -> Func_0808a100(slot, 4)   A3 branches in; B3 falls through
 *   0x020014fa -> Func_0808a100(slot, 1)   A1 and A4 branch in; B1 falls
 *                                          through, B4 branches in
 *   0x0200144a -> Func_0808a100(8, 6)      A7 and A11 dispatch to it; A10
 *                                          falls through into it
 * Writing the natural per-arm call at each of these would have inflated the
 * multiset by nine.  The slot is hoisted into a local exactly as the assembly
 * hoists it into r0, and the arms `goto` the shared site.  The four `join`
 * labels below are those `bl`s, one apiece; `join_slot6` is 0x0200144a, which
 * is only ever reached with slot 8.
 *
 * Behaviour: pick a presentation slot from the first argument — slot 8 when it
 * is 10, slot 9 otherwise — then play the gesture named by the second argument
 * as a sequence of Func_0808a100 requests six frames apart, and settle for
 * twelve frames.  The two tables are the same vocabulary for the two slots;
 * slot 8 has six extra gestures (6, 7, 8, 9, 10, 11) that slot 9 does not.
 * Out-of-range gestures fall straight to the twelve-frame settle, and so does
 * every slot-9 gesture above 5.
 *
 * Uncertainties, recorded rather than smoothed over:
 *  - both range tests are unsigned (`bls`/`bhi`), so a negative gesture id
 *    takes the settle-only path; written with a u32 cast rather than a
 *    signed comparison.
 *  - the first argument is compared only against 10; nothing here establishes
 *    what other values mean, so it is left as a plain s32 and the else-branch
 *    is written as the default it is, not as "argument == some other id".
 *  - Func_0808a100's second argument is an opaque request id in this overlay's
 *    converted siblings too; no meaning is asserted for the individual values.
 *  - old-style declarations are used because one import can take different
 *    argument counts at different sites.
 */

void Func_02002d32();
void Func_02002c50();
void Func_02002d48();
void Func_02002c66();
void Func_02002d5a();
void Func_02002c78();
void Func_02002d6c();
void Func_02002c8a();
void Func_02002d7a();
void Func_02002c98();
void Func_02002d88();
void Func_02002ca6();
void Func_02002d96();
void Func_02002cb4();
void Func_02002da8();
void Func_02002cc6();
void Func_02002db6();
void Func_02002dc0();
void Func_02002cde();
void Func_02002dce();
void Func_02002dd8();
void Func_02002cf6();
void Func_02002de6();
void Func_02002df0();
void Func_02002d0e();
void Func_02002dfe();
void Func_02002e08();
void Func_02002d26();
void Func_02002e16();
void Func_02002d34();
void Func_02002e24();
void Func_02002d42();
void Func_02002e32();
void Func_02002d50();
void Func_02002e40();
void Func_02002e6e();
void Func_02002d8c();
void Func_02002e7c();
void Func_02002e86();
void Func_02002da4();
void Func_02002e94();
void Func_02002e9e();
void Func_02002dbc();
void Func_02002eac();
void Func_02002eb6();
void Func_02002dd4();
void Func_02002ec4();
void Func_02002de2();
void Func_02002ed2();
void Func_02002ee0();
void Func_02002eee();
void Func_02002ef8();
void Func_02002f06();
                                /* per-slot scene request (slot, request) */
                                /* frame wait */

void Func_020012f4(s32 kind, s32 gesture)
{
    s32 slot;

    if (kind == 10) {
        if ((u32)gesture > 11) goto settle;
        switch (gesture) {
        case 0:  goto table_a_0;
        case 1:  slot = 8; goto join_req1;
        case 2:  goto table_a_2;
        case 3:  goto table_a_3;
        case 4:  goto table_a_4;
        case 5:  goto table_a_5;
        case 6:  goto table_a_6;
        case 7:  goto join_slot6;
        case 8:  goto table_a_8;
        case 9:  goto table_a_9;
        case 10: goto table_a_10;
        case 11: goto join_slot6;
        }
        goto settle;
    }

    goto table_b_dispatch;

table_a_0:
    Func_02002d32(8, 1);
    Func_02002c50(6);
    slot = 8;
    goto join_req3;                         /* 0x02001488 */

table_a_2:
    Func_02002d48(8, 1);
    Func_02002c66(6);
    slot = 8;
    goto join_req5;                         /* 0x020014a0 */

table_a_3:
    Func_02002d5a(8, 1);
    Func_02002c78(6);
    slot = 8;
    goto join_req4;                         /* 0x020014b8 */

table_a_4:
    Func_02002d6c(8, 1);
    Func_02002c8a(6);
    Func_02002d7a(8, 3);
    Func_02002c98(6);
    Func_02002d88(8, 1);
    Func_02002ca6(6);
    Func_02002d96(8, 3);
    slot = 8;
    Func_02002cb4(6);
    goto join_req1;

table_a_5:
    Func_02002da8(8, 1);
    Func_02002cc6(6);
    Func_02002db6(8, 2);
    goto settle;

table_a_6:
    Func_02002dc0(8, 6);
    Func_02002cde(6);
    Func_02002dce(8, 8);
    goto settle;

table_a_8:
    Func_02002dd8(8, 6);
    Func_02002cf6(6);
    Func_02002de6(8, 9);
    goto settle;

table_a_9:
    Func_02002df0(8, 6);
    Func_02002d0e(6);
    Func_02002dfe(8, 10);
    goto settle;

table_a_10:
    Func_02002e08(8, 6);
    Func_02002d26(6);
    Func_02002e16(8, 8);
    Func_02002d34(6);
    Func_02002e24(8, 6);
    Func_02002d42(6);
    Func_02002e32(8, 8);
    Func_02002d50(6);
    goto join_slot6;

join_slot6:                                 /* 0x0200144a */
    Func_02002e40(8, 6);
    goto settle;

table_b_dispatch:
    if ((u32)gesture > 5) goto settle;
    switch (gesture) {
    case 0: goto table_b_0;
    case 1: slot = 9; goto join_req1;
    case 2: goto table_b_2;
    case 3: goto table_b_3;
    case 4: goto table_b_4;
    case 5: goto table_b_5;
    }
    goto settle;

table_b_0:
    Func_02002e6e(9, 1);
    Func_02002d8c(6);
    slot = 9;
join_req3:                                  /* 0x02001488 */
    Func_02002e7c(slot, 3);
    goto settle;

table_b_2:
    Func_02002e86(9, 1);
    Func_02002da4(6);
    slot = 9;
join_req5:                                  /* 0x020014a0 */
    Func_02002e94(slot, 5);
    goto settle;

table_b_3:
    Func_02002e9e(9, 1);
    Func_02002dbc(6);
    slot = 9;
join_req4:                                  /* 0x020014b8 */
    Func_02002eac(slot, 4);
    goto settle;

table_b_4:
    Func_02002eb6(9, 1);
    Func_02002dd4(6);
    Func_02002ec4(9, 3);
    Func_02002de2(6);
    Func_02002ed2(9, 1);
    Func_02002df0(6);
    Func_02002ee0(9, 3);
    Func_02002dfe(6);
    slot = 9;
    goto join_req1;

join_req1:                                  /* 0x020014fa */
    Func_02002eee(slot, 1);
    goto settle;

table_b_5:
    Func_02002ef8(9, 1);
    Func_02002e16(6);
    Func_02002f06(9, 2);

settle:                                     /* 0x02001518 */
    Func_02002e24(12);
}
