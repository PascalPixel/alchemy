#include "types.h"

/*
 * Resource 3ce script-command handler at 0x02000b10 (168 bytes,
 * 0x02000b10-0x02000bb7).
 *
 * Role, settled BEFORE any disassembly (call-graph-first, HANDOVER §0):
 * `assets/code/resource_3ce_overlay.s` carries the pool word 0x02008b11, which
 * under the proven 0x02008000 link base is 0x02000b10 + the Thumb bit — this
 * owner's own entry — and it appears in the overlay's script-command table as
 * the triple `0xffff000a / 0x02008b11 / 0x00000000`.  The same table names
 * 0x02000244 (0xffff0001), 0x0200029c (0xffff0002), 0x0200021c (0xffff0003),
 * 0x02000208 (0xffff0008), 0x020008c4 (0xffff0009), 0x02000f14 (0xffff000b),
 * 0x020001f8 (0xffff000c), 0x0200117c (0xffff000d), 0x02001170 (0xffff000f)
 * and 0x02000bb8 (0xffff0005) — nine of which are already tracked byte-exact
 * siblings.  So this is a no-argument command handler, command id 0x0a.
 *
 * Link base proof for this overlay: `assets/code/resource_3ce_overlay.s`
 * holds 31 distinct `0x0200[89ab]xxx` words, and FIFTEEN of the odd ones
 * resolve at `word - 0x8000 - 1` onto already-tracked byte-exact sibling
 * entries — 0x02008131 -> Func_02000130, 0x02008149 -> Func_02000148,
 * 0x02008161 -> Func_02000160, 0x02008181 -> Func_02000180,
 * 0x02008199 -> Func_02000198, 0x020081b9 -> Func_020001b8,
 * 0x020081d9 -> Func_020001d8, 0x020081f9 -> Func_020001f8,
 * 0x02008209 -> Func_02000208, 0x0200821d -> Func_0200021c,
 * 0x02008229 -> Func_02000228, 0x0200822d -> Func_0200022c,
 * 0x0200823d -> Func_0200023c, 0x02008245 -> Func_02000244,
 * 0x020088c5 -> Func_020008c4 (plus 0x02008bb9, 0x02008f15, 0x020090a9,
 * 0x02009171, 0x0200917d).  That is the strongest witness class available and
 * it is not in doubt here.
 *
 * Complete owner: `push {r5, r6, lr}` prologue at 0x02000b10 and the matching
 * interworking return `pop {r5, r6} / pop {r0} / bx r0` at 0x02000ba8.  r0
 * holds the popped return address, so nothing is returned: the owner is
 * `void`.
 *
 * Pool map, derived by walking the owner's control flow from the prologue: the
 * body is entirely straight-line — there is not one branch instruction between
 * the prologue and the return — so every halfword from 0x02000b10 to
 * 0x02000bad is reached, the 2-byte zero word at 0x02000bae is the alignment
 * word, and 0x02000bb0-0x02000bb7 is the single trailing literal pool:
 *   0x00000c1b 0x00000131
 * Neither word decodes as a BL prefix, so no phantom call site is introduced.
 *
 * All 15 call sites were resolved with tools/overlay_call_targets.ts
 * (true_target_offset = stored_displacement + 2); the disassembler's printed
 * `bl` targets are the usual overlay artefact and are NOT the callees.  This
 * overlay resolves to veneers only (sites=243, prologue=0 across the whole
 * image), so every call here is a main-image import.
 * sites=15, distinct_targets=5, against the inventory's calls=15.
 *
 * Per-target multiset, tool count == C call count, checked PER TARGET:
 *   Func_08015040 1   Func_08077008 2   Func_08077010 4
 *   Func_08077118 4   Func_08077120 4
 *
 * Func_08077008 is the party-slot record accessor: its result is used as a
 * base for `strb` at +0x131 and at +304/+320, so it returns a pointer.  The
 * offsets are built by the `movs r2,#0xa0 / lsls r2,#1` and
 * `movs r2,#0x98 / lsls r2,#1` shape (320 and 304); r6 holds the constant
 * 0x131 across both records and r5 the constant 1 — both are long-lived
 * registers, not per-site values.
 *
 * Behaviour: the handler applies a fixed set of negative deltas to the four
 * battle slots through two different appliers (Func_08077118 with -100, -100,
 * -33, -100 and Func_08077120 with -50, -40, -35, -20), then marks slot 0's
 * record with 1 at +0x131 and +320 and slot 1's record with 1 at +304 and 2 at
 * +0x131, then refreshes all four slots through Func_08077010 in the order
 * 0, 1, 3, 2.  The refresh order is NOT 0,1,2,3 and is preserved as written.
 *
 * Uncertainties: 0x00000c1b is an id passed to Func_08015040 with a second
 * argument of 1, not an address under the link base; the record field widths
 * are `strb`, so they are bytes, but their meanings are not established.
 * Arities are taken from the registers written before each site.
 */

/* Old-style declarations: arities vary per call site across this overlay. */





/* Used for its return value. */


extern void Func_02001cc8();
extern void Func_02001d62();
extern void Func_02001d6c();
extern void Func_02001d76();
extern void Func_02001d80();
extern void Func_02001d92();
extern void Func_02001d9c();
extern void Func_02001da6();
extern void Func_02001db0();
extern u8 * Func_02001d86();
extern u8 * Func_02001d9a();
extern void Func_02001db4();
extern void Func_02001dba();
extern void Func_02001dc0();
extern void Func_02001dc6();
void Func_02000b10(void)
{
    u8 *record;
    s32 one = 1;                /* r5 */
    s32 mark = 0x131;           /* r6 */

    Func_02001cc8(0xc1b, 1);

    Func_02001d62(0, -100);
    Func_02001d6c(1, -100);
    Func_02001d76(2, -33);
    Func_02001d80(3, -100);

    Func_02001d92(0, -50);
    Func_02001d9c(1, -40);
    Func_02001da6(2, -35);
    Func_02001db0(3, -20);

    record = Func_02001d86(0);
    record[mark] = (u8)one;
    record[320] = (u8)one;      /* 160 << 1 */

    record = Func_02001d9a(1);
    record[304] = (u8)one;      /* 152 << 1 */
    record[mark] = 2;

    Func_02001db4(0);
    Func_02001dba(1);
    Func_02001dc0(3);
    Func_02001dc6(2);
}
