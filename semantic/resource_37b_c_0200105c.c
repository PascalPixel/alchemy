#include "types.h"

/*
 * Resource 37b scene entry at 0x0200105c.
 *
 * Complete owner: `push {r5, r6, lr}` / `push {r8}` / `sub sp, #8` at
 * 0x0200105c through the matching return at 0x0200149c..0x020014a6
 * (`add sp, #8` / `pop {r3}` / `mov r8, r3` / `pop {r5, r6}` / `pop {r1}` /
 * `bx r1`).  The popped register is r1, NOT r0, so r0 survives the epilogue
 * and IS the result: the owner returns s32, and every path reaches the
 * `movs r0, #0` at 0x0200149a, so the value is always 0.
 *
 * Two literal pools sit inside the span and are handled as data:
 *  - 0x020013f8-0x0200141f, which the `b.n 0x02001434` at 0x020013f6 branches
 *    over.  Register state is carried ACROSS it: r5 is loaded with the pool
 *    word 0x02000240 at 0x020013c8, before the pool, and is used again at
 *    0x02001438, after it.  That is why the second `Data_02000240` halfword
 *    test below reloads nothing.
 *  - 0x020014a8-0x020014b7, after the `bx r1`.  The span (1,116 bytes) ends at
 *    0x020014b8, which is exactly the next prologue, so both pools are inside
 *    it and neither is decoded as an instruction.
 *
 * Call sites resolved with `cargo run --release --manifest-path tools/overlay-call-targets/Cargo.toml --` (an overlay `bl`
 * stores `target_offset - 2`).  72 sites, 14 distinct targets: 69 import
 * veneers and 3 real in-overlay prologues — 0x020014b8, 0x02000150 and
 * 0x02000614.  The inventory reports calls=69, a lower bound.  Completeness
 * was taken as a multiset over the tool's per-site list against the span's
 * `bl` halfword pairs: 72 = 72, with per-import counts
 * 21 (Func_08009180), 22 (Func_0808a0f0), 14 (Func_080770c0),
 * 3 (Func_080770c8), 3 (Func_0808a080) and 1 each for the remaining six.
 * Each of those sites appears exactly once in the C below; note that the two
 * story-flag ids 0x0816 and 0x0817 are each tested TWICE on the falling path,
 * which the control flow preserves rather than collapses.
 *
 * 0x03001ebc is the overlay workspace pointer (the same one the neighbouring
 * byte-exact sources in assets/code load directly).  The owner's first act is
 * to store 516 at workspace + 448.  Note the encoding: r2 holds 448 as the
 * offset and is then advanced by 68 to become the stored VALUE, which is the
 * documented "one register is both displacement and value" trap; the value is
 * 516, not 448 + 68 applied to the address.
 *
 * 0x02000240 is a RAM global, not an in-image address: it is outside the
 * 0x0200_8xxx..0x0200_bxxx band that the confirmed 0x02008000 link base maps
 * into this image.  Its halfword at +450 is read unsigned once (for the
 * `(u16)(v - 3) > 1` guard, spelled in assembly as `subs #3 / lsls #16 /
 * cmp / bls`) and signed twice (`ldrsh`) for the scene-id tests.
 *
 * Uncertainties recorded as found:
 *  - Bit 1 of the byte at +35 of record 18 is set and bit 0 later cleared,
 *    with the byte at +89 zeroed in between; the field's meaning is not
 *    established.
 *  - Func_0808a2f8 (0x02001496) is reached with no argument register set and
 *    is spelled with none.
 */

/* Imports.  Old-style declarations: arity varies per site in this overlay. */
void Func_0200252c(void);
void Func_020034b8();
u8 *Func_020034ee();
u8 *Func_02003506();
void Func_020034b4();
u8 *Func_02003512();
void Func_020035b4();
void Func_0200357c();
void Func_02003586();
s32 Func_0200351c();
void Func_0200359e();
void Func_020035ac();
void Func_020035ba();
void Func_020035c8();
void Func_020035d6();
void Func_0200354a();
void Func_020035f8();
void Func_02003568();
void Func_0200357a();
s32 Func_020035be();
s32 Func_020035c8_b();
void Func_0200364a();
void Func_02003658();
void Func_020035cc();
void Func_0200367a();
void Func_020035f0();
void Func_0200369e();
void Func_02003610();
void Func_02003620();
void Func_0200363c();
s32 Func_0200366c();
void Func_020036ee();
void Func_02003662();
s32 Func_02003698();
void Func_0200371a();
void Func_0200368e();
s32 Func_020036c4();
void Func_02003746();
void Func_020036ba();
void Func_020036ca();
void Func_020036de();
s32 Func_02003714();
void Func_02003796();
void Func_0200370a();
void Func_0200372e();
s32 Func_02003764();
void Func_020037e6();
void Func_0200375a();
void Func_0200376a();
void Func_0200377c();
s32 Func_020037b2();
void Func_02003834();
void Func_020037a8();
void Func_020037b8();
void Func_020037ca();
s32 Func_02003810();
void Func_0200388e();
void Func_02003898();
s32 Func_02003858();
void Func_0200157c();
void Func_0200386e();
s32 Func_0200387a();
void Func_020038f8();
void Func_02003902();
s32 Func_0200389a();
void Func_02001a82();
void Func_020038b0();
s32 Func_020038ae();
void Func_020039e0();
void Func_020039d4();

                     

                     

                     

/* In-overlay callees. */

                     

/* RAM globals. */
extern u8 Data_02000240[];

s32 Func_0200105c(void)
{
    u8 *record;
    u8 *workspace;
    s32 first;
    s32 second;
    s32 slot;
    s32 mode;

    workspace = *(u8 **)0x03001ebc;
    *(u32 *)(workspace + 448) = 516;

    Func_0200252c();
    Func_020034b8(324);

    record = Func_020034ee(18);
    record[35] |= 2;
    record[89] = 0;

    Func_020034b4(Func_02003506(18), 0);

    record = Func_02003512(18);
    record[35] &= 0xfe;

    Func_020035b4(18, 1);

    if ((u16)(*(u16 *)(Data_02000240 + 450) - 3) > 1) {
        Func_0200357c(5, 0, 0);
        Func_02003586(1, 0, 0);
    }

    if (Func_0200351c(0x818) != 0) {
        Func_0200359e(18, 0x1200000, 0xb20000);
        Func_020035ac(17, 0x6480000, 0x6480000);
        Func_020035ba(10, 0xe80000, 0x780000);
        Func_020035c8(12, 0x1580000, 0x780000);
        Func_020035d6(10, 0xe80000, 0x780000);
        Func_0200354a(0, 59, 15, 38, 4, 3);
        Func_020035f8(12, 0x1580000, 0x780000);
        Func_02003568(4, 59, 17, 38, 4, 3);
        first = 0;
        second = 1;
        slot = 17;
        mode = 7;
        Func_0200357a(8, 60, 17, 39, 2, 2);
        goto publish;
    }

    if (Func_020035be(0x816) != 0 && Func_020035c8_b(0x817) != 0) {
        Func_0200364a(10, 0xe80000, 0x780000);
        Func_02003658(12, 0x1580000, 0x780000);
        Func_020035cc(0, 28, 17, 8, 2, 1);
        Func_0200367a(10, 0xe80000, 0x780000);
        Func_020035f0(0, 59, 15, 38, 4, 3);
        Func_0200369e(12, 0x1580000, 0x780000);
        Func_02003610(4, 59, 17, 38, 4, 3);
        Func_02003620(8, 60, 17, 39, 2, 2);
        first = 0;
        second = 0;
        slot = 17;
        mode = 8;
        goto publish;
    }

    goto retest;

publish:
    Func_0200363c(first, second, 2, 1, slot, mode);
    goto common;

retest:
    /* The two flags are re-tested here; this is a second pair of call sites,
     * not a reuse of the results above. */
    if (Func_0200366c(0x816) != 0) {
        Func_020036ee(10, 0xe80000, 0x780000);
        Func_02003662(0, 59, 15, 38, 4, 3);
    }
    if (Func_02003698(0x817) != 0) {
        Func_0200371a(12, 0x1580000, 0x780000);
        Func_0200368e(4, 59, 17, 38, 4, 3);
    }
    goto common;

common:
    if (Func_020036c4(0x80b) != 0) {
        Func_02003746(9, 0x1f80000, 0x980000);
        Func_020036ba(2, 28, 34, 10, 2, 1);
        Func_020036ca(2, 30, 16, 10, 2, 1);
        Func_020036de(0, 55, 32, 40, 4, 3);
    }
    if (Func_02003714(0x80c) != 0) {
        Func_02003796(11, 0x2880000, 0x980000);
        Func_0200370a(4, 28, 36, 10, 2, 1);
        Func_0200371a(4, 30, 18, 10, 2, 1);
        Func_0200372e(4, 55, 36, 40, 4, 3);
    }
    if (Func_02003764(0x80d) != 0) {
        Func_020037e6(13, 0x1f80000, 0xc80000);
        Func_0200375a(2, 29, 34, 11, 2, 1);
        Func_0200376a(2, 31, 16, 11, 2, 1);
        Func_0200377c(0, 58, 32, 43, 4, 1);
    }
    if (Func_020037b2(0x80e) != 0) {
        Func_02003834(15, 0x2880000, 0xc80000);
        Func_020037a8(4, 29, 36, 11, 2, 1);
        Func_020037b8(4, 31, 18, 11, 2, 1);
        Func_020037ca(4, 58, 36, 43, 4, 1);
    }

    if (*(s16 *)(Data_02000240 + 450) == 3) {
        if (Func_02003810(0x30a) != 0) {
            Func_0200388e(1, 0, 0);
            Func_02003898(5, 0, 0);
        } else if (Func_02003858(0x109) == 0) {
            Func_0200157c();
            Func_0200386e(0x30a);
        }
    }

    if (*(s16 *)(Data_02000240 + 450) == 4) {
        if (Func_0200387a(0x30b) != 0) {
            Func_020038f8(1, 0, 0);
            Func_02003902(5, 0, 0);
        } else if (Func_0200389a(0x109) == 0) {
            Func_02001a82();
            Func_020038b0(0x30b);
        }
    }

    if (Func_020038ae(0x814) != 0) {
        Func_020039e0(141);
        Func_020038b0(0x10000, 0x10000, 0x10000);
        Func_020039d4();
    }

    return 0;
}
