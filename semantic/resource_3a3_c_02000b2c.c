#include "types.h"

/*
 * Resource 3a3 scene actor placement at 0x02000b2c (224 bytes,
 * 0x02000b2c .. 0x02000c0b, of which 0x02000bf0 .. 0x02000c0b is the pool).
 *
 * Complete owner: `push {lr}` at 0x02000b2c and the interworking return
 * `pop {r0} / bx r0` at 0x02000bec, so the owner returns nothing.
 *
 * One of the two scene bodies dispatched by Func_02000874 (the other is
 * Func_02000904); it is selected when Data_02000240[224] is 0x4c.  Each of the
 * three gate flags 0x240 / 0x241 / 0x242 is the one Func_02000874 sets, so the
 * "already handled" case is the flag being set here.
 *
 * All 14 call sites are placed (row reports calls=14): Func_080770c0 x3,
 * Func_0808a0f0 x5, Func_0808a080 x6.
 *
 * The trailing three blocks all perform the same `record[0x59] |= 4` bit set.
 * They are separate call sites with different actor ids, so they are written
 * out separately rather than merged.  Note the asymmetry, which is in the
 * bytes and not a transcription slip: the actor-15 result is used without a
 * null check (it is reached only on the else arm of a flag test that has
 * already produced the record), while the actor-17 and actor-16 results are
 * both checked against 0.
 */

/* Imports.  Old-style declarations are mandatory in overlay sources. */
s32 Func_080770c0();        /* tests a flag id; nonzero when set */
void Func_0808a0f0();       /* places actor <id> at (x, z) */
u8 *Func_0808a080();        /* scene/actor record accessor */

void Func_02000b2c(void)
{
    u8 *record;

    /* `movs r0,#144 / lsls r0,#2` = 576 = 0x240. */
    if (Func_080770c0(0x240) == 0) {
        Func_0808a0f0(8, 0x03280000, 0x02d70000);       /* 202<<18 */
        record = Func_0808a080(8);
        *(u16 *)(record + 6) = 0x3000;                  /* 192<<6 */
        Func_0808a0f0(9, 0x031a0000, 0x03390000);
    }

    if (Func_080770c0(0x241) == 0) {
        Func_0808a0f0(10, 0x02300000, 0x02c60000);      /* 140<<18 */
        record = Func_0808a080(10);
        *(u16 *)(record + 6) = 0x1000;                  /* 128<<5 */
        Func_0808a0f0(11, 0x02400000, 0x02c60000);      /* 144<<18 */
    }

    if (Func_080770c0(0x242) == 0) {
        Func_0808a0f0(15, 0x01270000, 0x02e80000);      /* 186<<18 */
        record = Func_0808a080(15);
        *(u16 *)(record + 6) = 0xb000;                  /* 176<<8 */
    } else {
        record = Func_0808a080(15);
        record[0x59] |= 4;
    }

    record = Func_0808a080(17);
    if (record != 0) {
        record[0x59] |= 4;
    }

    record = Func_0808a080(16);
    if (record != 0) {
        record[0x59] |= 4;
    }
}
