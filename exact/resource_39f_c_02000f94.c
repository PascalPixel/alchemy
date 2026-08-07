#include "types.h"

/*
 * BYTE-EXACT 2026-08-07 with the new -fthumb-blockmove-dest-before-source.
 * The six-argument message is a 24-byte struct passed BY VALUE: the reference's
 * `mov r3, sp / add r2, sp, #24 / ldmia r2!,{r0,r1} / stmia r3!,{r0,r1}' is
 * thumb_expand_movstrqi copying the tail of the aggregate to the outgoing
 * argument area, with the first four words in r0-r3.  Writing the six scalars
 * as one `struct Probe { s32 w[6]; }' argument reproduces that block move;
 * indexing `rec.w[N]' directly rather than through an alias pointer keeps the
 * `add r5, sp, #8' where the reference has it.  The flag only restores the
 * expander's own destination-before-source order, which sched2 transposes.
 */

/*
 * resource_39f owner at 0x02000f94, 168 bytes (0x02000f94-0x0200103b).
 *
 * Reads a six-word probe record through the overlay's own Func_02000474,
 * republishes it through Func_02000608, and - only when the record's second
 * word is 9 and its fifth word scales to 26 whole units - runs a fixed
 * presentation sequence for slot 9 and posts a six-argument message.
 *
 * See resource_39f_c_02000030.c for the link base and the `bl` encoding rule.
 *
 * The epilogue is `add sp, #32 / pop {r5} / pop {r0} / bx r0`, so the owner is
 * void.  Func_0808a020 at 0x02001030 is on the common exit path and runs
 * whether or not the guarded body did.
 */


/* Fills the caller's six-word record; nonzero means it produced one. */

/* Four register arguments plus two stack words. */








/* Returns the slot record whose byte at +35 is written below. */

/* Four register arguments plus two stack words. */



extern void Func_02003d3e();
extern s32 Func_02001416();
extern void Func_020015c2();
extern void Func_02003d64();
extern void Func_02003dd4();
extern void Func_02003daa();
extern void Func_02003dd6();
extern void Func_02003d94();
extern void Func_02003dfc();
extern void Func_02003eba();
extern void Func_02003e4a();
extern u8 * Func_02003dd0();
extern void Func_02003d7a();
extern void Func_02003dde();
struct Probe { s32 w[6]; };

void Func_02000f94(void)
{
    struct Probe rec;

    /* No argument register is written before this branch: the caller's r0-r3
     * reach the import unchanged. */
    Func_02003d3e();

    if (Func_02001416(rec.w) != 0) {
        /* The two high words are copied to the outgoing stack slots by an
         * `ldmia/stmia` pair; the four low words go in r0-r3. */
        Func_020015c2(rec);

        if (rec.w[1] == 9 && (rec.w[4] >> 20) == 26) {
            Func_02003d64(784);            /* 196 << 2 */
            Func_02003dd4(9, 3);
            Func_02003daa(9, 0x4000, 0x8000);   /* 128 << 7, 128 << 8 */
            Func_02003dd6(9, 0, -16);
            Func_02003d94(45);
            Func_02003dfc(9, 8);
            Func_02003eba(240);
            Func_02003e4a(9, 1);

            Func_02003dd0(9)[35] = 2;

            { s32 f1 = 31; s32 g1 = 25; Func_02003d7a(38, 27, 4, 2,  f1, g1); }
        }
    }

    /* Common exit; no argument registers are set. */
    Func_02003dde();
}
