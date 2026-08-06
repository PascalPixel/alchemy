#include "types.h"

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
void Func_02000f94(void)
{
    s32 probe[6];

    /* No argument register is written before this branch: the caller's r0-r3
     * reach the import unchanged. */
    Func_02003d3e();

    if (Func_02001416(probe) != 0) {
        /* The two high words are copied to the outgoing stack slots by an
         * `ldmia/stmia` pair; the four low words go in r0-r3. */
        Func_020015c2(probe[0], probe[1], probe[2], probe[3],
                      probe[4], probe[5]);

        if (probe[1] == 9 && (probe[4] >> 20) == 26) {
            Func_02003d64(784);            /* 196 << 2 */
            Func_02003dd4(9, 3);
            Func_02003daa(9, 0x4000, 0x8000);   /* 128 << 7, 128 << 8 */
            Func_02003dd6(9, 0, -16);
            Func_02003d94(45);
            Func_02003dfc(9, 8);
            Func_02003eba(240);
            Func_02003e4a(9, 1);

            Func_02003dd0(9)[35] = 2;

            Func_02003d7a(38, 27, 4, 2, 31, 25);
        }
    }

    /* Common exit; no argument registers are set. */
    Func_02003dde();
}
