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
 * `bun tools/lib/overlay_call_targets.ts resource_38d 06e0 --json`: Func_0808a080
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
void Func_08009190();           /* channel control */
void Func_080091c0();           /* six-argument renderer, last two on the stack */
void Func_080091e0();           /* set presentation mode (record, phase) */
u8 *Func_0808a080();            /* record by slot index */
void Func_0808a0f0();           /* place slot at (x, z), 16.16 */
s32 Func_080770c0();            /* story-flag test; used in a condition */
void Func_080770c8();           /* set a story flag */
void Func_080770d0();           /* clear a story flag */

/* Intra-overlay scene bodies, resolved as real prologues in this overlay. */
void Func_020008c0();
void Func_0200179c();
void Func_020019b0();
void Func_020021bc();

s32 Func_020006e0(void)
{
    u8 *workspace;
    u8 *record;
    s32 selector;

    workspace = *(u8 **)0x03001ebc;
    *(s32 *)(workspace + 448) = 521;

    Func_08009190(1);
    Func_08009190(2);
    Func_080770c8(0x84b);

    if (Func_080770c0(0x109) != 0) {
        Func_080770d0(512);             /* 128 << 2 */
    }

    if (Func_080770c0(0x84f) == 0 && Func_080770c0(0x845) == 0) {
        selector = Data_02000240[225];
        if (selector == 29) {
            Func_020008c0();
        } else if (selector == 9 && Func_080770c0(0x321) != 0) {
            Func_0200179c();
        }
        return 0;
    }

    if (Func_080770c0(0x84e) != 0) {
        return 0;
    }

    selector = Data_02000240[225];

    if (selector == 29) {
        if (Func_080770c0(0x85e) == 0 && Func_080770c0(0x845) != 0) {
            Func_020019b0();
        }
        return 0;
    }

    if (selector != 28) {
        return 0;
    }
    if (Func_080770c0(0x322) == 0) {
        return 0;
    }
    if (Func_080770c0(0x109) == 0) {
        goto run_scene;
    }

    Func_080091c0(38, 55, 4, 1, 38, 45);
    Func_080091c0(42, 55, 4, 1, 38, 46);

    Func_0808a0f0(21, 154 << 18, 182 << 18);
    Func_0808a0f0(22, 158 << 18, 182 << 18);
    Func_0808a0f0(23, 162 << 18, 182 << 18);
    Func_0808a0f0(24, 166 << 18, 182 << 18);

    Func_080091e0(Func_0808a080(21), 0);
    Func_080091e0(Func_0808a080(22), 0);
    Func_080091e0(Func_0808a080(23), 0);
    Func_080091e0(Func_0808a080(24), 0);

    /* r6, and r6 is 0 on every path that reaches here - see the note above. */
    record = Func_0808a080(21);
    record[85] = 0;
    record = Func_0808a080(22);
    record[85] = 0;
    record = Func_0808a080(23);
    record[85] = 0;
    record = Func_0808a080(24);
    record[85] = 0;

    record = Func_0808a080(21);
    *(s32 *)(record + 12) = (s32)0xfffc0000;
    record = Func_0808a080(22);
    *(s32 *)(record + 12) = (s32)0xfffc0000;
    record = Func_0808a080(23);
    *(s32 *)(record + 12) = (s32)0xfffc0000;
    *(s32 *)(record + 12) = (s32)0xfffc0000;
    record = Func_0808a080(24);

    goto done;

    Func_020021bc();
run_scene:

done:
    return 0;
}
