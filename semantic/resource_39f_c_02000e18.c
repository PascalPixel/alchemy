#include "types.h"

/*
 * resource_39f owner at 0x02000e18, 144 bytes: code 0x02000e18-0x02000ea3 and
 * the pool word 0x00001999 at 0x02000ea4.
 *
 * Seventeen-step build loop.  For each step it asks two ROM dispatch entries
 * for a pair of values derived from `step << 12`, advances a running cursor by
 * an IWRAM helper's result, and publishes one piece through the overlay's own
 * eight-argument Func_02000ae8 (four register arguments and four stack words).
 *
 * See resource_39f_c_02000030.c for the link base and the `bl` encoding rule.
 * Three of the imports here reach the ROM's fixed dispatch stub table at
 * 0x080000c0 (8 bytes per entry, `ldr r4,1f / bx r4 / .word target+1`, source
 * asm/080000c0.s):
 *   veneer 0x02002d04 -> 0x08000120 = dispatch[12] -> Func_0800231c
 *   veneer 0x02002cfc -> 0x08000118 = dispatch[11] -> Func_08002322
 * and veneer 0x02002ce4 reaches 0x03000380, an ARM-mode helper relocated into
 * IWRAM (the same family as the 0x030001d8 square root used at 0x02000030).
 *
 * The epilogue is `add sp, #68 / pop {r3, r5, r6} / ... / pop {r0} / bx r0`, so
 * the owner is void.
 */

/* Returns the subject record whose words at +8 and +16 are read in the loop. */


       /* ROM dispatch[12] -> Func_0800231c */
       /* ROM dispatch[11] -> Func_08002322 */
       /* relocated IWRAM helper */
      /* four register arguments plus four stack words */

extern u8 * Func_02003be2();
extern void Func_02003ce2();
extern s32 Func_02003b4a();
extern s32 Func_02003b4e();
extern s32 Func_02003b44();
extern void Func_02001970();
void Func_02000e18(s32 subject)
{
    u8 *record;
    s32 flag;              /* sp+16, published as the eighth argument */
    s32 cursor;            /* r7[0] at sp+56 */
    s32 spare;             /* r7[1] at sp+60 */
    s32 extent;            /* r7[2] at sp+64 */
    s32 step;

    /* r0-r3 are untouched by the prologue, so the incoming arguments reach
     * this import unchanged. */
    record = Func_02003be2(subject);

    Func_02003ce2(188);

    flag = 1;

    for (step = 0; step <= 16; step++) {
        s32 key = step << 12;

        cursor = Func_02003b4a(key);
        spare = 0;
        extent = Func_02003b4e(key);

        /* The helper's result advances the cursor; its second argument is the
         * literal 3. */
        cursor = cursor + Func_02003b44(cursor, 3);

        /* Register arguments r0-r3, then the four stack words at sp+0..sp+12.
         * `spare` is written 0 immediately above and reloaded here, so the
         * first stack word is always 0x1999 in this loop; it is kept as a read
         * of the local because that is what the assembly does. */
        Func_02001970(*(s32 *)(record + 8), 0x100000, *(s32 *)(record + 16),
                      cursor, spare + 0x1999, extent, 0x20000, &flag);
    }
}
