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
 * Call sites resolved with `tools/lib/overlay_call_targets.ts` (an overlay `bl`
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
void Func_02002a6c();
void Func_02002b76();
void Func_02002b8a();
void Func_02002a88();
void Func_02002aee();
void Func_02002a9c();
void Func_02002b32();
void Func_02002a02();
void Func_02002b7a();
void Func_02002b92();
void Func_02002af8();
void Func_02002b24();
void Func_02002ae2();
void Func_02002b5c();
void Func_02002af2();
void Func_02002a52();
void Func_02002b08();
void Func_02002bbc();
void Func_02002b1a();
void Func_02002bea();
void Func_02002c02();
void Func_02002c0e();
void Func_02002b3c();
void Func_02002c1c();
void Func_02002c28();
void Func_02002b56();
void Func_02002bc6();
void Func_02002b64();
void Func_02002c18();
void Func_02002bf0();
void Func_02002bb6();
void Func_02002be2();
void Func_02002ba0();
void Func_02002c10();
void Func_02002c20();
void Func_02002c30();
void Func_02002bce();
void Func_02002c4e();
void Func_02002b36();
void Func_02002c58();
void Func_02002c62();
void Func_02002c6c();
void Func_02002c82();
void Func_02002b6a();
void Func_02002c92();
void Func_02002c8a();
void Func_02002b82();
void Func_02002cbc();
void Func_02002c3a();
void Func_02002cc4();
void Func_02002cce();
void Func_02002c54();
void Func_02002cb2();
void Func_02002d1e();
void Func_02002c7c();
void Func_02002cae();
void Func_02002cd2();
void Func_02002c98();
void Func_02002d68();
void Func_02002d82();
void Func_02002d8e();
void Func_02002d24();
void Func_02002da4();
void Func_02002cda();
void Func_02002d8c();
void Func_02002cea();
void Func_02002d3e();
void Func_02002daa();
void Func_02002d08();
void Func_02002dbc();
void Func_02002dc8();
void Func_02002dd4();
void Func_02002dfc();
void Func_02002e14();
void Func_02002dbe();
void Func_02002d84();
void Func_02002db0();
void Func_02002d6e();
void Func_02002de6();
void Func_02002cde();
void Func_02002e4a();
void Func_02002d98();
void Func_02002e18();
void Func_02002d00();
void Func_02002e64();
void Func_02002dc2();
void Func_02002e76();
void Func_02002d34();
void Func_02002e9e();
void Func_02002ea8();
void Func_02002eb2();
void Func_02002e00();
void Func_02002e70();
void Func_02002ea2();
void Func_02002e20();
void Func_02002e30();
void Func_02002ed0();
void Func_02002eec();
void Func_02002ef8();
void Func_02002f04();
s32 Func_02002eb4();
void Func_02002f36();
void Func_02002f3e();
void Func_02002e0e();
void Func_02002f4c();
void Func_02002f3c();
void Func_02002e24();
void Func_02002f90();
void Func_02002ede();
void Func_02002e3e();
void Func_02002f56();
void Func_02002e4e();
void Func_02002faa();
void Func_02002f08();
void Func_02002f82();
void Func_02002f66();
void Func_02002f24();
void Func_02002ff4();
void Func_0200300c();
void Func_02002f72();
void Func_02002f9e();
void Func_02002f5c();
void Func_02002fdc();
void Func_02002f6a();
void Func_02002eca();
void Func_02003026();
void Func_02002f84();
void Func_02002ee4();
void Func_02002ffc();
void Func_02003076();
void Func_02002ff8();
void Func_02003012();
void Func_0200308a();
void Func_020030a4();
void Func_020030b0();
void Func_02002fde();
void Func_02003090();
void Func_02002fee();
void Func_02002f4e();
void Func_0200305e();
void Func_0200306e();
void Func_0200303c();
void Func_0200304a();
s32 *Func_02003050();
void Func_02003072();
void Func_02003098();
void Func_020030aa();
void Func_020030ba();
s32 *Func_02003080();
void Func_020030a2();
void Func_020030c8();
void Func_020030da();
void Func_02003080_b();
void Func_020030b2();

                     

                     

                     

                     

                     

                     

                     

                     

                     

/* In-overlay helper at 0x020023a4 (two arguments). */

void Func_02000614(void)
{
    u8 *workspace;
    s32 *record;

    Func_02002a6c();

    *(u32 *)(workspace + 456) = 32;
    *(u32 *)(workspace + 448) = 256;
    workspace = *(u8 **)0x03001ebc;

    Func_02002b76();
    Func_02002b8a();
    Func_02002a88(20);

    Func_02002aee(8, 0x2400000, 0x1280000);
    Func_02002a9c(1);
    Func_02002b32(0x1004);
    Func_02002a02(8, 6);
    Func_02002b7a(0xcccc, 0x1999);
    Func_02002b92(0x23e0000, -1, 0xb40000, 1);
    Func_02002af8(8, 0x10000, 0x8000);
    Func_02002b24(8, 0x240, 216);
    Func_02002ae2(20);
    Func_02002b5c(5, 2, 0);
    Func_02002af2(30);
    Func_02002a52(5, 6);
    Func_02002b7a(8, 2);
    Func_02002b08(6);
    Func_02002bbc(8, 0x9000, 0);
    Func_02002b1a(10);
    Func_02002bea(0x59999, 0xb333);
    Func_02002c02(0x11f0000, -1, 0xb00000, 1);
    Func_02002c0e();
    Func_02002b3c(60);
    Func_02002c1c(0x23e0000, -1, 0xb40000, 1);
    Func_02002c28();
    Func_02002b56(20);
    Func_02002bc6(8, 3);
    Func_02002b64(10);
    Func_02002c18(8, 0xc000, 0);
    Func_02002b76(10);
    Func_02002bf0(8, 6, 0);
    Func_02002bb6(8, 0x30000, 0x20000);
    Func_02002be2(8, 0x240, 184);
    Func_02002ba0(40);
    Func_02002c10(8, 3);
    Func_02002b08(8, 6);
    Func_02002c18(1, 3);
    Func_02002c20(5, 3);
    Func_02002c30(0, 3);
    Func_02002bce(40);
    Func_02002c4e(8, 3);
    Func_02002b36(8, 6);
    Func_02002c58(0, 2, 0);
    Func_02002c62(1, 2, 0);
    Func_02002c6c(5, 2, 0);
    Func_02002c02(30);
    Func_02002c82(1, 2);
    Func_02002b6a(1, 6);
    Func_02002c92(8, 1);
    Func_02002c8a(8, 4);
    Func_02002b82(8, 6);
    Func_02002cbc(0, 5, 0);
    Func_02002c3a(40);
    Func_02002cc4(8, 0, 0);
    Func_02002cce(8, 5, 0);
    Func_02002c54(40);
    Func_02002cce(8, 6, 0);
    Func_02002cb2(8, 0x240, 216);
    Func_02002d1e(8, 0x8000, 0);
    Func_02002c7c(10);
    Func_02002cae(8, 0x13333, 0x9999);
    Func_02002cd2(8, 432, 200);
    Func_02002c98(20);
    Func_02002d68(0x26666, 0x4ccc);
    Func_02002d82(0x1200000, -1, 0xab0000, 1);
    Func_02002d8e();
    Func_02002cbc(80);
    Func_02002d24(8, 1);
    Func_02002da4(0x23e0000, -1, 0xb40000, 1);
    Func_02002cda(20);
    Func_02002d8c(8, 0, 0);
    Func_02002cea(30);
    Func_02002d3e(8, 0x240, 216);
    Func_02002daa(8, 0xc000, 0);
    Func_02002d08(10);
    Func_02002dbc(0, 0x4000, 0);
    Func_02002dc8(5, 0x4000, 0);
    Func_02002dd4(1, 0x4000, 0);
    Func_02002dfc(0xcccc, 0x1999);
    Func_02002e14(0x23e0000, -1, 0xab0000, 1);
    Func_02002dbe(8, 6, 0);
    Func_02002d84(8, 0x30000, 0x20000);
    Func_02002db0(8, 0x240, 184);
    Func_02002d6e(80);
    Func_02002cce(8, 6);
    Func_02002de6(8, 4);
    Func_02002cde(8, 20);
    Func_02002e4a(5, 258, 0);
    Func_02002d98(40);
    Func_02002e18(5, 2);
    Func_02002d00(5, 6);
    Func_02002e18(8, 3);
    Func_02002e64(8, 0x8000, 0);
    Func_02002dc2(40);
    Func_02002e76(8, 0xc000, 0);
    Func_02002dd4(30);
    Func_02002d34(8, 6);
    Func_02002e9e(0, 0x101, 0);
    Func_02002ea8(1, 0x101, 0);
    Func_02002eb2(5, 0x101, 0);
    Func_02002e00(60);
    Func_02002e70(8, 4);
    Func_02002d68(8, 6);
    Func_02002ea2(1, 0, 0);
    Func_02002e20(40);
    Func_02002eb2(5, 0, 0);
    Func_02002e30(40);
    Func_02002ed0(8, 0);
    Func_02002eec(0, 0x4000, 0);
    Func_02002ef8(5, 0x4000, 0);
    Func_02002f04(1, 0x4000, 0);

    /* r0 = 0 is set before the `b.n` that hops the literal pool. */
    if (Func_02002eb4(0, 0) == 0) {
        Func_02002f36(0x1010);
    } else {
        Func_02002f3e(0x1011);
    }

    Func_02002e0e(8, 6);
    Func_02002f4c(0x1012);
    Func_02002f3c(8, 2);
    Func_02002e24(8, 6);
    Func_02002f90(1, 258, 0);
    Func_02002ede(60);
    Func_02002e3e(1, 6);
    Func_02002f56(8, 3);
    Func_02002e4e(8, 6);
    Func_02002faa(8, 0x4000, 0);
    Func_02002f08(20);
    Func_02002f82(8, 6, 0);
    Func_02002f66(8, 0x240, 216);
    Func_02002f24(40);
    Func_02002ff4(0xcccc, 0x1999);
    Func_0200300c(0x23e0000, -1, 0xbf0000, 1);
    Func_02002f72(8, 0x10000, 0x8000);
    Func_02002f9e(8, 0x240, 232);
    Func_02002f5c(40);
    Func_02002fdc(8, 2);
    Func_02002f6a(40);
    Func_02002eca(8, 6);
    Func_02003026(8, 0xc000, 0);
    Func_02002f84(30);
    Func_02002ee4(8, 6);
    Func_02002ffc(8, 3);
    Func_02003076(0x2400000, -1, 0xd70000, 1);
    Func_02002ff8(8, 0x23e, 0x143);
    Func_02003012(8, 0, 0);
    Func_0200308a(0x39999, 0x7333);
    Func_020030a4(0x2400000, -1, 0x880000, 1);
    Func_020030b0();
    Func_02002fde(20);
    Func_02003090(5, 0, 0);
    Func_02002fee(10);
    Func_02002f4e(5, 6);
    Func_0200305e(1, 3);
    Func_0200306e(5, 3);
    Func_0200303c(1, 0x10000, 0x8000);
    Func_0200304a(5, 0x10000, 0x8000);
    Func_0200308a(5, 2);

    record = Func_02003050(0);
    if (record != 0) {
        Func_02003072(5, *(s16 *)((u8 *)record + 10),
                      *(s16 *)((u8 *)record + 18));
    }
    Func_02003098(5);
    Func_020030aa(5, 0, 0);

    Func_020030ba(1, 2);
    record = Func_02003080(0);
    if (record != 0) {
        Func_020030a2(1, *(s16 *)((u8 *)record + 10),
                      *(s16 *)((u8 *)record + 18));
    }
    Func_020030c8(1);
    Func_020030da(1, 0, 0);

    Func_02003080_b(0x12f);

    workspace = *(u8 **)0x03001ebc;
    *(u32 *)(workspace + 456) = 16;
    *(u32 *)(workspace + 448) = 516;

    Func_020030b2();
}
