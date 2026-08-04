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
 * Call sites resolved with `tools/overlay_call_targets.ts` (an overlay `bl`
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
u8 *Func_0808a080();
s32 Func_080770c0();
void Func_080770c8();
void Func_0808a0f0();
void Func_0808a1e0();
void Func_0808a2f8();
void Func_0808a5e0();
void Func_08009180();
void Func_080091c0();
void Func_080091e0();
void Func_080091f0();

/* In-overlay callees. */
void Func_020014b8(void);
void Func_02000150();
void Func_02000614();

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

    Func_020014b8();
    Func_080770c8(324);

    record = Func_0808a080(18);
    record[89] = 0;
    record[35] |= 2;

    Func_080091e0(Func_0808a080(18), 0);

    record = Func_0808a080(18);
    record[35] &= 0xfe;

    Func_0808a1e0(18, 1);

    if ((u16)(*(u16 *)(Data_02000240 + 450) - 3) > 1) {
        Func_0808a0f0(5, 0, 0);
        Func_0808a0f0(1, 0, 0);
    }

    if (Func_080770c0(0x818) != 0) {
        Func_0808a0f0(18, 0x1200000, 0xb20000);
        Func_0808a0f0(17, 0x6480000, 0x6480000);
        Func_0808a0f0(10, 0xe80000, 0x780000);
        Func_0808a0f0(12, 0x1580000, 0x780000);
        Func_0808a0f0(10, 0xe80000, 0x780000);
        Func_08009180(0, 59, 15, 38, 4, 3);
        Func_0808a0f0(12, 0x1580000, 0x780000);
        Func_08009180(4, 59, 17, 38, 4, 3);
        Func_08009180(8, 60, 17, 39, 2, 2);
        first = 0;
        second = 1;
        slot = 17;
        mode = 7;
        goto publish;
    }

    if (Func_080770c0(0x816) != 0 && Func_080770c0(0x817) != 0) {
        Func_0808a0f0(10, 0xe80000, 0x780000);
        Func_0808a0f0(12, 0x1580000, 0x780000);
        Func_08009180(0, 28, 17, 8, 2, 1);
        Func_0808a0f0(10, 0xe80000, 0x780000);
        Func_08009180(0, 59, 15, 38, 4, 3);
        Func_0808a0f0(12, 0x1580000, 0x780000);
        Func_08009180(4, 59, 17, 38, 4, 3);
        Func_08009180(8, 60, 17, 39, 2, 2);
        first = 0;
        second = 0;
        slot = 17;
        mode = 8;
        goto publish;
    }

    goto retest;

publish:
    Func_080091c0(first, second, 2, 1, slot, mode);
    goto common;

retest:
    /* The two flags are re-tested here; this is a second pair of call sites,
     * not a reuse of the results above. */
    if (Func_080770c0(0x816) != 0) {
        Func_0808a0f0(10, 0xe80000, 0x780000);
        Func_08009180(0, 59, 15, 38, 4, 3);
    }
    if (Func_080770c0(0x817) != 0) {
        Func_0808a0f0(12, 0x1580000, 0x780000);
        Func_08009180(4, 59, 17, 38, 4, 3);
    }
    goto common;

common:
    if (Func_080770c0(0x80b) != 0) {
        Func_0808a0f0(9, 0x1f80000, 0x980000);
        Func_08009180(2, 28, 34, 10, 2, 1);
        Func_08009180(2, 30, 16, 10, 2, 1);
        Func_08009180(0, 55, 32, 40, 4, 3);
    }
    if (Func_080770c0(0x80c) != 0) {
        Func_0808a0f0(11, 0x2880000, 0x980000);
        Func_08009180(4, 28, 36, 10, 2, 1);
        Func_08009180(4, 30, 18, 10, 2, 1);
        Func_08009180(4, 55, 36, 40, 4, 3);
    }
    if (Func_080770c0(0x80d) != 0) {
        Func_0808a0f0(13, 0x1f80000, 0xc80000);
        Func_08009180(2, 29, 34, 11, 2, 1);
        Func_08009180(2, 31, 16, 11, 2, 1);
        Func_08009180(0, 58, 32, 43, 4, 1);
    }
    if (Func_080770c0(0x80e) != 0) {
        Func_0808a0f0(15, 0x2880000, 0xc80000);
        Func_08009180(4, 29, 36, 11, 2, 1);
        Func_08009180(4, 31, 18, 11, 2, 1);
        Func_08009180(4, 58, 36, 43, 4, 1);
    }

    if (*(s16 *)(Data_02000240 + 450) == 3) {
        if (Func_080770c0(0x30a) != 0) {
            Func_0808a0f0(1, 0, 0);
            Func_0808a0f0(5, 0, 0);
        } else if (Func_080770c0(0x109) == 0) {
            Func_02000150();
            Func_080770c8(0x30a);
        }
    }

    if (*(s16 *)(Data_02000240 + 450) == 4) {
        if (Func_080770c0(0x30b) != 0) {
            Func_0808a0f0(1, 0, 0);
            Func_0808a0f0(5, 0, 0);
        } else if (Func_080770c0(0x109) == 0) {
            Func_02000614();
            Func_080770c8(0x30b);
        }
    }

    if (Func_080770c0(0x814) != 0) {
        Func_0808a5e0(141);
        Func_080091f0(0x10000, 0x10000, 0x10000);
        Func_0808a2f8();
    }

    return 0;
}
