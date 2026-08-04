#include "types.h"

/*
 * Resource 37b cutscene at 0x02000614.
 *
 * Complete owner: `push {lr}` at 0x02000614 through `pop {r0} / bx r0` at
 * 0x02000c58.  The popped register is r0, so it holds the return address and
 * the owner is `void`.  No stack frame and no saved work registers: every
 * value used here is rebuilt at its point of use.
 *
 * TWO literal pools sit inside the span:
 *  - 0x02000a16-0x02000a4b (a two-byte alignment `nop` then 13 words), which
 *    the `b.n 0x02000a4c` at 0x02000a14 branches over MID-CALL-SETUP: r0 is
 *    set to 0 at 0x02000a12, *before* the branch, and is the first argument of
 *    the Func_0808a070 call at 0x02000a4e, *after* it.  Register state is
 *    therefore carried across the pool and the call is
 *    `Func_0808a070(0, 0)`, not a fragment.
 *  - 0x02000c5c-0x02000c8b, after the `bx r0`.  The span (1,656 bytes) ends at
 *    0x02000c8c, exactly the next prologue, so both pools are inside it.
 * Neither pool is decoded as an instruction.  Note also that
 * `overlay_show.ts`'s "pool words referenced" listing for this span includes
 * 0x02000d78, which is OUTSIDE the owner: the pool word 0x00004ccc at
 * 0x02000a44 disassembles as `ldr r4, [pc, #816]`.  That is the documented
 * inverse trap — the listing is not authoritative, and the pool map here comes
 * from the owner's own control flow.
 *
 * Call sites resolved with `tools/overlay_call_targets.ts` (an overlay `bl`
 * stores `target_offset - 2`).  167 sites, 28 distinct targets: 149 import
 * veneers and 18 sites of the real in-overlay two-argument helper at
 * 0x020023a4 (the same one 0x02000150 calls eleven times).  The inventory
 * reports calls=153, a lower bound.  Completeness was taken as a multiset over
 * the tool's per-site list against the span's `bl` halfword pairs: 167 = 167,
 * with per-target counts 36 Func_0808a010, 18 Func_020023a4,
 * 16 Func_0808a1b8, 10 Func_0808a110, 9 Func_0808a210, 8 Func_0808a0d0,
 * 8 Func_0808a128, 7 Func_0808a090, 7 Func_0808a138, 6 Func_0808a100,
 * 6 Func_0808a208, 5 Func_0808a1e8, 4 Func_0808a0f0, 4 Func_0808a170,
 * 4 Func_0808a218, 3 Func_0808a150, 2 each Func_0808a080, Func_0808a0b8,
 * Func_0808a0e8, Func_0808a148, and 1 each Func_080770d0, Func_0808a018,
 * Func_0808a020, Func_0808a070, Func_0808a0c8, Func_0808a178,
 * Func_0808a360, Func_0808a370.  Every site appears exactly once below.
 *
 * 0x03001ebc is the overlay workspace pointer; the owner brackets itself with
 * writes of 256/32 on entry and 516/16 on exit to workspace + 448 and + 456.
 * The entry encoding is the canonical form of the documented displacement/value
 * trap: r3 holds 448 as the store displacement, is reduced by 192 to 256 as
 * the stored VALUE, then advanced by 200 to 456 as the next displacement.  The
 * exit encoding does the same in the other direction (448 -> 516 -> 456).
 *
 * Uncertainties recorded as found:
 *  - Func_0808a018 (0x02000616) and the four Func_0808a218 sites are reached
 *    with no argument register set and are spelled with none.
 *  - Func_0808a070(0, 0) returns a value tested only for zero; it selects
 *    between script ids 0x1010 and 0x1011.  What it queries is not
 *    established.
 *  - Func_0808a080(0) returns the scene-state record; the s16 fields at +10
 *    and +18 are the integer parts of the 16.16 words at +8 and +16 (the
 *    documented halfword-coordinate view).
 *  - This owner is the sibling of 0x02000150: same bracketing, same helper,
 *    largely the same import set with different ids and timings.  The two were
 *    read together and agree on every shared idiom.
 */

/* Imports.  Old-style declarations: arity varies per site in this overlay. */
s32 *Func_0808a080();
s32 Func_0808a070();
void Func_0808a010();
void Func_0808a018();
void Func_0808a020();
void Func_0808a090();
void Func_0808a0b8();
void Func_0808a0c8();
void Func_0808a0d0();
void Func_0808a0e8();
void Func_0808a0f0();
void Func_0808a100();
void Func_0808a110();
void Func_0808a128();
void Func_0808a138();
void Func_0808a148();
void Func_0808a150();
void Func_0808a170();
void Func_0808a178();
void Func_0808a1b8();
void Func_0808a1e8();
void Func_0808a208();
void Func_0808a210();
void Func_0808a218();
void Func_0808a360();
void Func_0808a370();
void Func_080770d0();

/* In-overlay helper at 0x020023a4 (two arguments). */
void Func_020023a4();

void Func_02000614(void)
{
    u8 *workspace;
    s32 *record;

    Func_0808a018();

    workspace = *(u8 **)0x03001ebc;
    *(u32 *)(workspace + 448) = 256;
    *(u32 *)(workspace + 456) = 32;

    Func_0808a360();
    Func_0808a370();
    Func_0808a010(20);

    Func_0808a0f0(8, 0x2400000, 0x1280000);
    Func_0808a010(1);
    Func_0808a170(0x1004);
    Func_020023a4(8, 6);
    Func_0808a208(0xcccc, 0x1999);
    Func_0808a210(0x23e0000, -1, 0xb40000, 1);
    Func_0808a090(8, 0x10000, 0x8000);
    Func_0808a0d0(8, 0x240, 216);
    Func_0808a010(20);
    Func_0808a128(5, 2, 0);
    Func_0808a010(30);
    Func_020023a4(5, 6);
    Func_0808a138(8, 2);
    Func_0808a010(6);
    Func_0808a1b8(8, 0x9000, 0);
    Func_0808a010(10);
    Func_0808a208(0x59999, 0xb333);
    Func_0808a210(0x11f0000, -1, 0xb00000, 1);
    Func_0808a218();
    Func_0808a010(60);
    Func_0808a210(0x23e0000, -1, 0xb40000, 1);
    Func_0808a218();
    Func_0808a010(20);
    Func_0808a110(8, 3);
    Func_0808a010(10);
    Func_0808a1b8(8, 0xc000, 0);
    Func_0808a010(10);
    Func_0808a128(8, 6, 0);
    Func_0808a090(8, 0x30000, 0x20000);
    Func_0808a0d0(8, 0x240, 184);
    Func_0808a010(40);
    Func_0808a110(8, 3);
    Func_020023a4(8, 6);
    Func_0808a100(1, 3);
    Func_0808a100(5, 3);
    Func_0808a110(0, 3);
    Func_0808a010(40);
    Func_0808a138(8, 3);
    Func_020023a4(8, 6);
    Func_0808a128(0, 2, 0);
    Func_0808a128(1, 2, 0);
    Func_0808a128(5, 2, 0);
    Func_0808a010(30);
    Func_0808a138(1, 2);
    Func_020023a4(1, 6);
    Func_0808a138(8, 1);
    Func_0808a110(8, 4);
    Func_020023a4(8, 6);
    Func_0808a150(0, 5, 0);
    Func_0808a010(40);
    Func_0808a148(8, 0, 0);
    Func_0808a148(8, 5, 0);
    Func_0808a010(40);
    Func_0808a128(8, 6, 0);
    Func_0808a0d0(8, 0x240, 216);
    Func_0808a1b8(8, 0x8000, 0);
    Func_0808a010(10);
    Func_0808a090(8, 0x13333, 0x9999);
    Func_0808a0c8(8, 432, 200);
    Func_0808a010(20);
    Func_0808a208(0x26666, 0x4ccc);
    Func_0808a210(0x1200000, -1, 0xab0000, 1);
    Func_0808a218();
    Func_0808a010(80);
    Func_0808a100(8, 1);
    Func_0808a210(0x23e0000, -1, 0xb40000, 1);
    Func_0808a010(20);
    Func_0808a1b8(8, 0, 0);
    Func_0808a010(30);
    Func_0808a0d0(8, 0x240, 216);
    Func_0808a1b8(8, 0xc000, 0);
    Func_0808a010(10);
    Func_0808a1b8(0, 0x4000, 0);
    Func_0808a1b8(5, 0x4000, 0);
    Func_0808a1b8(1, 0x4000, 0);
    Func_0808a208(0xcccc, 0x1999);
    Func_0808a210(0x23e0000, -1, 0xab0000, 1);
    Func_0808a128(8, 6, 0);
    Func_0808a090(8, 0x30000, 0x20000);
    Func_0808a0d0(8, 0x240, 184);
    Func_0808a010(80);
    Func_020023a4(8, 6);
    Func_0808a110(8, 4);
    Func_020023a4(8, 20);
    Func_0808a1e8(5, 258, 0);
    Func_0808a010(40);
    Func_0808a138(5, 2);
    Func_020023a4(5, 6);
    Func_0808a110(8, 3);
    Func_0808a1b8(8, 0x8000, 0);
    Func_0808a010(40);
    Func_0808a1b8(8, 0xc000, 0);
    Func_0808a010(30);
    Func_020023a4(8, 6);
    Func_0808a1e8(0, 0x101, 0);
    Func_0808a1e8(1, 0x101, 0);
    Func_0808a1e8(5, 0x101, 0);
    Func_0808a010(60);
    Func_0808a110(8, 4);
    Func_020023a4(8, 6);
    Func_0808a150(1, 0, 0);
    Func_0808a010(40);
    Func_0808a150(5, 0, 0);
    Func_0808a010(40);
    Func_0808a178(8, 0);
    Func_0808a1b8(0, 0x4000, 0);
    Func_0808a1b8(5, 0x4000, 0);
    Func_0808a1b8(1, 0x4000, 0);

    /* r0 = 0 is set before the `b.n` that hops the literal pool. */
    if (Func_0808a070(0, 0) == 0) {
        Func_0808a170(0x1010);
    } else {
        Func_0808a170(0x1011);
    }

    Func_020023a4(8, 6);
    Func_0808a170(0x1012);
    Func_0808a138(8, 2);
    Func_020023a4(8, 6);
    Func_0808a1e8(1, 258, 0);
    Func_0808a010(60);
    Func_020023a4(1, 6);
    Func_0808a110(8, 3);
    Func_020023a4(8, 6);
    Func_0808a1b8(8, 0x4000, 0);
    Func_0808a010(20);
    Func_0808a128(8, 6, 0);
    Func_0808a0d0(8, 0x240, 216);
    Func_0808a010(40);
    Func_0808a208(0xcccc, 0x1999);
    Func_0808a210(0x23e0000, -1, 0xbf0000, 1);
    Func_0808a090(8, 0x10000, 0x8000);
    Func_0808a0d0(8, 0x240, 232);
    Func_0808a010(40);
    Func_0808a138(8, 2);
    Func_0808a010(40);
    Func_020023a4(8, 6);
    Func_0808a1b8(8, 0xc000, 0);
    Func_0808a010(30);
    Func_020023a4(8, 6);
    Func_0808a110(8, 3);
    Func_0808a210(0x2400000, -1, 0xd70000, 1);
    Func_0808a0d0(8, 0x23e, 0x143);
    Func_0808a0f0(8, 0, 0);
    Func_0808a208(0x39999, 0x7333);
    Func_0808a210(0x2400000, -1, 0x880000, 1);
    Func_0808a218();
    Func_0808a010(20);
    Func_0808a1b8(5, 0, 0);
    Func_0808a010(10);
    Func_020023a4(5, 6);
    Func_0808a100(1, 3);
    Func_0808a110(5, 3);
    Func_0808a090(1, 0x10000, 0x8000);
    Func_0808a090(5, 0x10000, 0x8000);
    Func_0808a100(5, 2);

    record = Func_0808a080(0);
    if (record != 0) {
        Func_0808a0b8(5, *(s16 *)((u8 *)record + 10),
                      *(s16 *)((u8 *)record + 18));
    }
    Func_0808a0e8(5);
    Func_0808a0f0(5, 0, 0);

    Func_0808a100(1, 2);
    record = Func_0808a080(0);
    if (record != 0) {
        Func_0808a0b8(1, *(s16 *)((u8 *)record + 10),
                      *(s16 *)((u8 *)record + 18));
    }
    Func_0808a0e8(1);
    Func_0808a0f0(1, 0, 0);

    Func_080770d0(0x12f);

    workspace = *(u8 **)0x03001ebc;
    *(u32 *)(workspace + 448) = 516;
    *(u32 *)(workspace + 456) = 16;

    Func_0808a020();
}
