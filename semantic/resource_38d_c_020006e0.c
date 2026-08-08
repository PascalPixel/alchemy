#include "types.h"

/*
 * resource_38d owner at 0x020006e0, 480 bytes: code 0x020006e0-0x02000891
 * (0x02000892 is the two-byte alignment `movs r0, r0`) and the eleven-word
 * literal pool at 0x02000894-0x020008bf, which is exactly the end of the row.
 * A control-flow walk from the prologue reaches the pool only through
 * `ldr rN, [pc, #imm]`: every branch in the owner goes forward to 0x02000888
 * or earlier, and the last instruction before the pool is `bx r1`.
 *
 * The overlay's scene router.  It publishes the scene word into the workspace,
 * re-enables channels 1 and 2, sets story flag 0x84b, and then selects one of
 * four scene bodies from the scene selector - the signed halfword at
 * Data_02000240 + 450 - crossed with five story flags.  The fifth outcome is
 * inline: it places the four-actor group 21-24, presents them, clears a byte
 * in each record and drops each record's word at +12 to -4.0 in 16.16.
 *
 * Sibling evidence used here: `assets/code/resource_38d_c_02000100.c` reads
 * the neighbouring element Data_02000240[224] the same way (`ldrsh` through a
 * zero index register), which fixes both the type and the fact that
 * 0x02000240 is an external EWRAM global rather than overlay data - it is
 * below the overlay's own 0x02008000 link base.  The workspace pointer at
 * 0x03001ebc is the one the rest of the overlay loads directly.
 *
 * The store at 0x020006ee is the documented "one register is both value and
 * displacement" shape: r2 is built as 224 << 1 = 448 and used as the
 * displacement, then `adds r2, #73` makes it 521 and that is the word stored.
 * The owner at 0x020005f4 performs the identical store.
 *
 * Call accounting: 39 `bl` sites, all resolved with
 * `cargo run --release --manifest-path tools/overlay-call-targets/Cargo.toml -- resource_38d 06e0 --json`: Func_0808a080
 * x12, Func_080770c0 x9, Func_080091e0 x4, Func_0808a0f0 x4, Func_08009190
 * x2, Func_080091c0 x2, and one each of Func_080770c8, Func_080770d0,
 * Func_020008c0, Func_0200179c, Func_020019b0, Func_020021bc - 12 distinct
 * targets, of which four are intra-overlay prologues in this same overlay.
 * Every site appears below exactly once and the per-target multisets match.
 * The inventory row says calls=36, the documented lower bound.  No `call_via`
 * slot: r3 is written only as workspace scratch and as Func_080091c0's fourth
 * register argument, never with an IWRAM code address.
 *
 * Func_080091c0 is the established six-argument renderer: four register
 * arguments plus two stack words at [sp, #0] and [sp, #4], which the prologue
 * reserves with `sub sp, #8`.  Both sites here write [sp, #0] from r5 = 38 and
 * [sp, #4] from a fresh immediate, so the calls are (38, 55, 4, 1, 38, 45) and
 * (42, 55, 4, 1, 38, 46).
 *
 * The epilogue is `add sp, #8 / pop {r5, r6} / pop {r1} / bx r1` with N != 0,
 * so r0 survives and IS the result; `movs r0, #0` at the join means every path
 * returns 0.  The prologue is `push {r5, r6, lr}` and no incoming register is
 * read before it is overwritten, so the owner takes no arguments.
 *
 * UNCERTAINTIES:
 *  - r6 holds the result of Func_080770c0(0x84e) and is stored into each
 *    record's byte at +85.  Every path that reaches those stores has already
 *    branched on r6 == 0, so the stored value is provably 0; it is written as
 *    0 below with this note rather than as a live variable.
 *  - The four positions are (154, 158, 162, 166) << 18 on X against a common
 *    182 << 18 on Z, i.e. a row of four at a fixed spacing.  16.16 is the
 *    established unit for Func_0808a0f0; the shift is left explicit.
 *  - Record offsets +12 and +85 are taken from the stores themselves; +12 is
 *    the 16.16 word the sibling overlays model as the Y coordinate, but that
 *    identification is not proven here.
 *  - Func_080091e0's second argument is the literal 0 at all four sites.
 */

extern s16 Data_02000240[];     /* external EWRAM globals, not overlay data */

/* Imports. Old-style declarations: one name can take different argument
 * counts at different sites in this overlay. */
void Func_02002b26();
void Func_02002b2c();
void Func_02002b5a();
s32 Func_02002b58();
void Func_02002b74();
s32 Func_02002b6a();
s32 Func_02002b74_b();
void Func_02000ffc();
s32 Func_02002b9a();
void Func_02001ef0();
s32 Func_02002bac();
s32 Func_02002bca();
s32 Func_02002bd6();
void Func_0200213e();
s32 Func_02002bea();
s32 Func_02002bf4();
void Func_02002bf4_b();
void Func_02002c06();
void Func_02002cd4();
void Func_02002ce2();
void Func_02002cf0();
void Func_02002cfe();
u8 *Func_02002ca4();
void Func_02002c52();
u8 *Func_02002cb0();
void Func_02002c5e();
u8 *Func_02002cbc();
void Func_02002c6a();
u8 *Func_02002cc8();
void Func_02002c76();
u8 *Func_02002cd4_b();
u8 *Func_02002cde();
u8 *Func_02002ce8();
u8 *Func_02002cf2();
u8 *Func_02002cfc();
u8 *Func_02002d06();
u8 *Func_02002d0e();
u8 *Func_02002d16();
void Func_02002a42();
                                /* channel control */
                                /* six-argument renderer, last two on the stack */
                                /* set presentation mode (record, phase) */
                                /* record by slot index */
                                /* place slot at (x, z), 16.16 */
                                /* story-flag test; used in a condition */
                                /* set a story flag */
                                /* clear a story flag */

/* Intra-overlay scene bodies, resolved as real prologues in this overlay. */

                     

s32 Func_020006e0(void)
{
    u8 *workspace;
    u8 *record;
    s32 selector;

    workspace = *(u8 **)0x03001ebc;
    *(s32 *)(workspace + 448) = 521;

    Func_02002b26(1);
    Func_02002b2c(2);
    Func_02002b5a(0x84b);

    if (Func_02002b58(0x109) != 0) {
        Func_02002b74(512);             /* 128 << 2 */
    }

    if (Func_02002b6a(0x84f) == 0 && Func_02002b74_b(0x845) == 0) {
        selector = Data_02000240[225];
        if (selector == 29) {
            Func_02000ffc();
        } else if (selector == 9 && Func_02002b9a(0x321) != 0) {
            Func_02001ef0();
        }
        return 0;
    }

    if (Func_02002bac(0x84e) != 0) {
        return 0;
    }

    selector = Data_02000240[225];

    if (selector == 29) {
        if (Func_02002bca(0x85e) == 0 && Func_02002bd6(0x845) != 0) {
            Func_0200213e();
        }
        return 0;
    }

    if (selector != 28) {
        return 0;
    }
    if (Func_02002bea(0x322) == 0) {
        return 0;
    }
    if (Func_02002bf4(0x109) == 0) {
        goto run_scene;
    }

    Func_02002bf4_b(38, 55, 4, 1, 38, 45);
    Func_02002c06(42, 55, 4, 1, 38, 46);

    Func_02002cd4(21, 154 << 18, 182 << 18);
    Func_02002ce2(22, 158 << 18, 182 << 18);
    Func_02002cf0(23, 162 << 18, 182 << 18);
    Func_02002cfe(24, 166 << 18, 182 << 18);

    Func_02002c52(Func_02002ca4(21), 0);
    Func_02002c5e(Func_02002cb0(22), 0);
    Func_02002c6a(Func_02002cbc(23), 0);
    Func_02002c76(Func_02002cc8(24), 0);

    /* r6, and r6 is 0 on every path that reaches here - see the note above. */
    record = Func_02002cd4_b(21);
    record[85] = 0;
    record = Func_02002cde(22);
    record[85] = 0;
    record = Func_02002ce8(23);
    record[85] = 0;
    record = Func_02002cf2(24);
    record[85] = 0;

    record = Func_02002cfc(21);
    *(s32 *)(record + 12) = (s32)0xfffc0000;
    record = Func_02002d06(22);
    *(s32 *)(record + 12) = (s32)0xfffc0000;
    record = Func_02002d0e(23);
    *(s32 *)(record + 12) = (s32)0xfffc0000;
    *(s32 *)(record + 12) = (s32)0xfffc0000;
    record = Func_02002d16(24);

    goto done;

    Func_02002a42();
run_scene:

done:
    return 0;
}
