#include "types.h"

/*
 * Resource 37b cutscene at 0x02000150.
 *
 * Complete owner: `push {r5, r6, r7, lr}` at 0x02000150 through the matching
 * interworking return `pop {r5, r6, r7} / pop {r0} / bx r0` at 0x0200060a.
 * The popped register is r0, so it holds the return address: the owner is
 * `void`.  There is no stack frame beyond the saved registers.
 *
 * TWO literal pools sit inside the span, and both matter:
 *  - 0x02000552-0x02000597 (a two-byte alignment `nop` at 0x02000552 then 17
 *    words).  The `b.n 0x02000598` at 0x02000550 branches over it MID-SEQUENCE,
 *    so register state must be carried ACROSS it: r7 still holds the pool word
 *    0x03001ebc loaded at 0x02000156 and is dereferenced at 0x020005f4, and r6
 *    still holds 16 — assigned at 0x020002c0, more than 800 bytes earlier — and
 *    is the value stored at 0x02000604.  Neither is reloaded after the pool.
 *  - 0x02000610-0x02000613, after the `bx r0`.  The span (1,220 bytes) ends at
 *    0x02000614, exactly the next prologue, so both pools are inside it.
 * None of these words is modelled as an instruction.
 *
 * Note also that r6 is REASSIGNED mid-owner: it is 448 from 0x0200015e (used
 * as the store displacement for the two workspace writes at 0x02000164 and
 * 0x020002be) and becomes 16 at 0x020002c0 (used as a stored VALUE from then
 * on).  This is the documented long-lived-alias trap; the register is tracked
 * per use below, not as one variable.
 *
 * Call sites resolved with `tools/lib/overlay_call_targets.ts` (an overlay `bl`
 * stores `target_offset - 2`).  127 sites, 27 distinct targets: 116 import
 * veneers and 11 sites of one real in-overlay routine, 0x020023a4 — a
 * two-argument helper (`push {r5, lr}` at 0x020023a4, `pop {r5} / pop {r0} /
 * bx r0` at 0x020023b4) that lives among the veneers but is genuine code, not
 * an eight-byte `ldr r4 / bx r4 / .word` entry.  The inventory reports
 * calls=117, a lower bound.  Completeness was taken as a multiset over the
 * tool's per-site list against the span's `bl` halfword pairs: 127 = 127, and
 * every site appears exactly once below.  Per-target counts, for the record:
 * 27 Func_0808a010, 11 Func_020023a4, 9 Func_0808a1b8, 8 Func_0808a210,
 * 7 Func_0808a0f0, 7 Func_0808a110, 5 Func_0808a208, 5 Func_0808a370,
 * 4 each Func_0808a090/0f0-class helpers 0808a0d0, 0808a100, 0808a1e8,
 * 0808a218, 3 each Func_080000c0, Func_0808a128, Func_0808a138,
 * Func_0808a360, 2 each Func_08009128, Func_0808a080, Func_0808a0b8,
 * Func_0808a0e8, Func_0808a150, Func_0808a368, and 1 each Func_0808a018,
 * Func_0808a020, Func_0808a170, Func_080770d0.
 *
 * 0x03001ebc is the overlay workspace pointer.  The owner brackets itself with
 * writes to workspace + 448 and workspace + 456 (256/32 on entry, 0x202/16 in
 * the middle, 516/16 on exit).  At 0x020005fa the same register-reuse trap as
 * in 0x0200105c appears: r2 is first the displacement 448, then advanced to
 * 516 as the stored VALUE, then reduced by 60 to 456 as the next displacement.
 *
 * Uncertainties recorded as found:
 *  - Func_0808a018 (0x02000152), Func_0808a218 (four sites) and
 *    Func_08009128 (two sites) are reached with no argument register set by
 *    this owner and are spelled with none.  In the Func_08009128 cases the
 *    registers still hold the previous call's arguments; whether the callee
 *    reads them is unverified, and the dataflow is left as written.
 *  - Func_0808a080(0) returns the scene-state record (same accessor as
 *    0x02001624 / 0x02000c8c); the s16 fields at +10 and +18 read here are the
 *    integer parts of the 16.16 words at +8 and +16, which is the documented
 *    halfword-coordinate-view idiom.
 *  - The many pooled constants passed to Func_0808a208/Func_0808a210 are
 *    fade/blend parameters; their exact meaning is not established.
 */

/* Imports.  Old-style declarations: arity varies per site in this overlay. */
void Func_020025a8();
void Func_020026b2();
void Func_020026c6();
void Func_020025c4();
void Func_0200262a();
void Func_020025d8();
void Func_0200266e();
void Func_0200253e();
void Func_0200264c();
void Func_020026c4();
void Func_020026dc();
void Func_02002642();
void Func_0200262c();
void Func_020026a6();
void Func_0200263c();
void Func_0200259c();
void Func_02002652();
void Func_02002706();
void Func_02002664();
void Func_02002734();
void Func_0200274c();
void Func_02002758();
void Func_02002686();
void Func_02002766();
void Func_02002772();
void Func_020026a0();
void Func_02002710();
void Func_020026ae();
void Func_02002762();
void Func_020026c0();
void Func_0200273a();
void Func_02002700();
void Func_0200272c();
void Func_020026ea();
void Func_0200264a();
void Func_020027a6();
void Func_02002704();
void Func_02002812();
void Func_0200281e();
void Func_020027f6();
void Func_020026da();
void Func_0200282c();
void Func_02002840();
void Func_0200273e();
void Func_020027a4();
void Func_020026c2();
void Func_0200285e();
void Func_0200286a();
void Func_020027c8();
void Func_02002850();
void Func_020026fa();
void Func_02002886();
void Func_0200289a();
void Func_02002798();
void Func_02002868();
void Func_02002880();
void Func_0200288c();
void Func_020027ba();
void Func_0200283a();
void Func_02002722();
void Func_02002732();
void Func_0200285a();
void Func_020027e8();
void Func_02002858();
void Func_02002750();
void Func_020028bc();
void Func_0200280a();
void Func_020028be();
void Func_0200281c();
void Func_0200277c();
void Func_020028e6();
void Func_020028f0();
void Func_020028fa();
void Func_02002848();
void Func_020028fc();
void Func_020028ca();
void Func_020027c2();
void Func_0200287a();
void Func_0200290c();
void Func_0200288a();
void Func_020027f2();
void Func_0200294e();
void Func_0200295a();
void Func_02002966();
void Func_020028c4();
void Func_0200292c();
void Func_02002934();
void Func_02002944();
void Func_020028e2();
void Func_02002952();
void Func_0200284a();
void Func_020029c2();
void Func_020029dc();
void Func_020029c0();
void Func_0200291e();
void Func_02002998();
void Func_0200297c();
void Func_0200293a();
void Func_0200298e();
void Func_020029a8();
void Func_02002a20();
void Func_02002a3a();
void Func_02002a46();
void Func_02002974();
void Func_020029aa();
void Func_020029b8();
void Func_020029f8();
s32 *Func_02002a06();
void Func_02002a28();
void Func_02002a4e();
void Func_02002a60();
void Func_02002a70();
s32 *Func_02002a36();
void Func_02002a58();
void Func_02002a7e();
void Func_02002a90();
void Func_02002a36_b();
void Func_02002a64();

                     

                     

                     

                     

                     

                     

                     

                     

/* In-overlay helper at 0x020023a4 (two arguments). */

void Func_02000150(void)
{
    u8 *workspace;
    s32 *record;

    Func_020025a8();

    workspace = *(u8 **)0x03001ebc;
    *(u32 *)(workspace + 456) = 32;
    *(u32 *)(workspace + 448) = 256;

    Func_020026b2();
    Func_020026c6();
    Func_020025c4(20);

    Func_0200262a(8, 0x2400000, 0xe80000);
    Func_020025d8(1);
    Func_0200266e(0x101a);
    Func_0200253e(8, 6);
    Func_0200264c(8, 0x2400000, 0x1180000);
    Func_020026c4(0xcccc, 0x1999);
    Func_020026dc(0x23e0000, -1, 0xb40000, 1);
    Func_02002642(8, 0x10000, 0x8000);
    Func_0200266e(8, 0x240, 216);
    Func_0200262c(20);
    Func_020026a6(5, 2, 0);
    Func_0200263c(30);
    Func_0200259c(5, 6);
    Func_020026c4(8, 2);
    Func_02002652(6);
    Func_02002706(8, 0x9000, 0);
    Func_02002664(10);
    Func_02002734(0x59999, 0xb333);
    Func_0200274c(0x11f0000, -1, 0xb00000, 1);
    Func_02002758();
    Func_02002686(60);
    Func_02002766(0x23e0000, -1, 0xb40000, 1);
    Func_02002772();
    Func_020026a0(20);
    Func_02002710(8, 3);
    Func_020026ae(10);
    Func_02002762(8, 0xc000, 0);
    Func_020026c0(10);
    Func_0200273a(8, 6, 0);
    Func_02002700(8, 0x30000, 0x20000);
    Func_0200272c(8, 0x240, 184);
    Func_020026ea(40);
    Func_0200264a(8, 6);
    Func_020027a6(8, 0x8000, 0);
    Func_02002704(40);

    /* Mid-cutscene workspace change; 16 stays live in r6 to the very end. */
    workspace = *(u8 **)0x03001ebc;
    *(u32 *)(workspace + 448) = 0x202;
    *(u32 *)(workspace + 456) = 16;

    Func_02002812();
    Func_0200281e();
    Func_020027f6(0x11f0000, -1, 0xb00000, 0);
    Func_020026da();
    Func_020026a0(1);
    Func_0200282c();
    Func_02002840();
    Func_0200273e(40);
    Func_020027a4(8, 0x1a80000, 0xc80000);
    Func_020026c2(1);
    Func_020026b2(8, 20);
    Func_0200285e();
    Func_0200286a();
    Func_020027c8(8, 0x2400000, 0xb80000);
    Func_02002850(0x23e0000, -1, 0xb40000, 0);
    Func_02002734();
    Func_020026fa(1);
    Func_02002886();
    Func_0200289a();
    Func_02002798(20);
    Func_02002868(0x13333, 0x2666);
    Func_02002880(0x23e0000, -1, 0x9d0000, 1);
    Func_0200288c();
    Func_020027ba(20);
    Func_0200283a(1, 2);
    Func_02002722(1, 20);
    Func_0200283a(5, 3);
    Func_02002732(5, 6);
    Func_0200285a(8, 2);
    Func_020027e8(40);
    Func_02002858(8, 4);
    Func_02002750(8, 80);
    Func_020028bc(8, 258, 0);
    Func_0200280a(60);
    Func_020028be(8, 0xc000, 0);
    Func_0200281c(30);
    Func_0200277c(8, 20);
    Func_020028e6(0, 0x101, 0);
    Func_020028f0(1, 0x101, 0);
    Func_020028fa(5, 0x101, 0);
    Func_02002848(60);
    Func_020028fc(8, 0xc000, 0);
    Func_0200285a(30);
    Func_020028ca(8, 4);
    Func_020027c2(8, 6);
    Func_020028fc(0, 1, 0);
    Func_0200287a(40);
    Func_0200290c(0, 5, 0);
    Func_0200288a(40);
    Func_020028fa(8, 3);
    Func_020027f2(8, 6);
    Func_0200294e(0, 0x4000, 0);
    Func_0200295a(5, 0x4000, 0);
    Func_02002966(1, 0x4000, 0);
    Func_020028c4(40);
    Func_0200292c(1, 3);
    Func_02002934(5, 3);
    Func_02002944(0, 3);
    Func_020028e2(20);
    Func_02002952(8, 3);
    Func_0200284a(8, 6);
    Func_020029c2(0x19999, 0x3333);
    Func_020029dc(0x2400000, -1, 0xd70000, 1);
    Func_020029c0(8, 0x4000, 0);
    Func_0200291e(10);
    Func_02002998(8, 6, 0);
    Func_0200297c(8, 0x240, 217);
    Func_0200293a(20);
    Func_0200298e(8, 0x240, 0x141);
    Func_020029a8(8, 0, 0);
    Func_02002a20(0x39999, 0x7333);
    Func_02002a3a(0x2400000, -1, 0x880000, 1);
    Func_02002a46();
    Func_02002974(20);
    Func_020029aa(1, 0x10000, 0x8000);
    Func_020029b8(5, 0x10000, 0x8000);
    Func_020029f8(1, 2);

    record = Func_02002a06(0);
    if (record != 0) {
        Func_02002a28(1, *(s16 *)((u8 *)record + 10),
                      *(s16 *)((u8 *)record + 18));
    }
    Func_02002a4e(1);
    Func_02002a60(1, 0, 0);

    Func_02002a70(5, 2);
    record = Func_02002a36(0);
    if (record != 0) {
        Func_02002a58(5, *(s16 *)((u8 *)record + 10),
                      *(s16 *)((u8 *)record + 18));
    }
    Func_02002a7e(5);
    Func_02002a90(5, 0, 0);

    Func_02002a36_b(0x12f);

    workspace = *(u8 **)0x03001ebc;
    *(u32 *)(workspace + 456) = 16;
    *(u32 *)(workspace + 448) = 516;

    Func_02002a64();
}
