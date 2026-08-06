#include "types.h"

/*
 * Resource 3b1 object reaction handler at 0x02000284.
 *
 * OWNER AND POOL MAP, from a control-flow walk:
 *   0x02000284-0x020002d5  code
 *   0x020002d6-0x020002db  DATA: a SIX-BYTE interior pool — one alignment
 *                          halfword plus the single word 0x00000000
 *   0x020002dc-0x020002ef  code (the third arm and the epilogue)
 *   0x020002f0-0x020002f3  DATA: the trailing pool word 0x00000000
 * The halfword at 0x020002dc immediately after the six-byte pool is live
 * code AND the target of the `bne` at 0x020002b8, so reading the customary
 * eight-to-twelve-byte pool length here would have eaten a real instruction.
 * The 112-byte advertised span is exact; 102 of it is code.
 *
 * Signature.  The prologue is `push {r5, r6, r7, lr}` plus `mov r7, r8` /
 * `push {r7}` to save r8; the epilogue restores r8 and then does
 * `pop {r0} / bx r0`, so the popped value is the return address and the
 * owner is `void`.  One argument, the object.
 *
 * Link base 0x02008000 (see resource_3b1_c_0200012c.c for the jump-table
 * witness); 0x02008031 and 0x020080b1 in this overlay's pools are
 * Func_02000030 + 1 and Func_020000b0 + 1, both of which have byte-exact
 * sources in assets/code.
 *
 * Import naming uses `true_target_offset = stored_displacement + 2` against
 * the veneer band at 0x02006450+, never the disassembler's annotation.
 *
 * Two registers hold constants that a reading by eye turns into variables:
 * r6 is the object's own +91 byte, and the guard means every store of r6
 * below happens only when it is 0; and r3 is loaded from a pool word that is
 * literally 0.  Both are written as 0 here, with the provenance noted, rather
 * than as names that would suggest they could be anything else.
 */

  /* veneer 0x64e8 — actor record accessor */
  /* veneer 0x6450 */

extern u8  * Func_02006778();
extern void Func_0200671c();
void Func_02000284(u8 *object)
{
    u8 *actor;
    u8 *stage;

    actor = Func_02006778(9);

    /* Reached only when this byte is 0; the zero stores below are this
     * register, not fresh constants. */
    if (object[91] != 0) {
        return;
    }

    stage = actor + 99;

    if (*stage == 1) {
        /* 208 << 8. */
        object[98] = 1;
        *(u16 *) (object + 6) = 0xd000;
        /* The stored 0 is the pool word at 0x020002d8. */
        *stage = 0;
        return;
    }

    if (*stage == 2) {
        if (object[98] != 0) {
            Func_0200671c(object, 3);
        }
        *stage = 0;
        object[98] = 0;
        return;
    }

    if (*stage == 3) {
        *(u16 *) (object + 6) = 0;
        /* The stored 0 is the pool word at 0x020002f0. */
        *stage = 0;
    }
}
