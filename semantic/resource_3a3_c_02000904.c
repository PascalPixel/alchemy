#include "types.h"

/*
 * Resource 3a3 scene entry at 0x02000904 (552 bytes,
 * 0x02000904 .. 0x02000b2b, of which 0x02000b0e .. 0x02000b2b is alignment
 * plus the literal pool).
 *
 * Complete owner: `push {r5, r6, r7, lr} / sub sp,#8` at 0x02000904 and the
 * matching `add sp,#8 / pop {r5, r6, r7} / pop {r0} / bx r0` at 0x02000b06.
 * r0 is the popped return address, so the owner returns nothing.
 *
 * This is the Data_02000240[224] == 0x4b scene body dispatched by
 * Func_02000874; Func_02000b2c is the 0x4c one.
 *
 * All 48 call sites are placed.  Per target: Func_0808a080 x8,
 * Func_080770c0 x5, Func_08009180 x3, Func_080091c0 x5, Func_0808a088 x8,
 * Func_0808a5e0 x1, Func_0808a158 x4, Func_0808a100 x1, Func_080000d0 x1,
 * Func_080770d0 x1, Func_0808a0f0 x1, Func_080091e0 x1, Func_0808a1e0 x3,
 * Func_080000c0 x2, Func_0808a018 x1, Func_0808a210 x1, Func_08009128 x1,
 * Func_0808a020 x1.  The inventory row says calls=46; that field counts
 * distinct targets and predates the corrected `bl` decoding, so the check is
 * placed >= row count.
 *
 * SHARED CALL SITE WITH DIFFERENT ARGUMENTS.  The Func_0808a088 site at
 * 0x020009ac is entered from three of the four opening arms with r0 already
 * set to 20, 20 and 17 respectively (the third arm falls into it through
 * `movs r0,#17` at 0x020009aa).  Writing one call per arm would inject two
 * phantom calls, so the id is hoisted into a local and the pair of calls at
 * 0x020009ac/0x020009b2 is spelled once.  The fourth arm bypasses that pair
 * entirely and has its own three Func_0808a088 sites.
 *
 * LINK-BASE WITNESS.  The pool word 0x02008d09 at 0x02000b1c is odd and in
 * the 0x0200_8xxx band: file offset 0x0d09 = `Func_02000d08 + 1`.  It is the
 * first argument of the task installer Func_080000d0, so this scene installs
 * the same object spawner that Func_02000d58 does, here with a period of
 * 3200 (`movs r1,#200 / lsls r1,#4`).
 *
 * Uncertainty: the two literal zeros pushed as Func_080091c0's stack
 * arguments at 0x0200096a / 0x020009a0 / 0x020009c4 come from r5, r6 and r0
 * -- flag-test results that are known to be zero on those paths.  They are
 * written as 0.
 */

/* Imports.  Old-style declarations are mandatory in overlay sources. */
u8 *Func_0808a080();        /* scene/actor record accessor */
s32 Func_080770c0();        /* tests a flag id; nonzero when set */
void Func_08009180();       /* six-argument renderer entry */
void Func_080091c0();       /* six-argument renderer entry */
void Func_0808a088();
void Func_0808a5e0();
void Func_0808a158();
void Func_0808a100();
void Func_080000d0();       /* installs a task callback with a period */
void Func_080770d0();
void Func_0808a0f0();       /* places actor <id> at (x, z) */
void Func_080091e0();
void Func_0808a1e0();
void Func_080000c0();
void Func_0808a018();       /* opens a scripted sequence */
void Func_0808a210();
void Func_08009128();
void Func_0808a020();       /* closes the scripted sequence */

extern s16 Data_02000240[];

void Func_02000904(void)
{
    u8 *origin;
    u8 *record;
    s32 hidden;

    origin = Func_0808a080(0);

    hidden = 0;

    if (Func_080770c0(0x242) != 0) {
        Func_08009180(64, 32, 0, 32, 32, 32);
        hidden = 20;
        Func_080091c0(64, 32, 32, 32, 0, 0);
        goto joined_actors;
    } else if (Func_080770c0(0x241) != 0) {
        Func_08009180(64, 0, 0, 32, 32, 32);
        Func_080091c0(64, 0, 32, 32, 0, 0);
        Func_0808a088(17);
        hidden = 20;
        goto joined_actors;
    } else if (Func_080770c0(0x240) != 0) {
        /* `movs r0,#144 / lsls r0,#2` = 576 = 0x240. */
        Func_08009180(0, 64, 0, 32, 32, 32);
        Func_080091c0(0, 64, 32, 32, 0, 0);
        Func_0808a088(16);
        hidden = 17;
        goto joined_actors;
    }

    goto unjoined_actors;

joined_actors:
    Func_0808a088(hidden);
    Func_0808a088(21);
    goto actors_ready;

unjoined_actors:
    Func_080091c0(0, 32, 32, 32, 0, 0);
    Func_0808a088(15);
    Func_0808a088(16);
    Func_0808a088(17);

actors_ready:
    if (Func_080770c0(0x8ff) != 0) {
        Func_0808a088(18);
    } else {
        Func_0808a5e0(170);
        Func_0808a158(18, 2);
        Func_0808a100(18, 3);
        /* Func_02000d08 + Thumb bit. */
        Func_080000d0(0x02008d09, 3200);
    }

    if (Data_02000240[225] == 3) {
        Func_080770d0(0x12f);
    }

    Func_080091c0(0, 33, 4, 3, 20, 41);

    if (Func_080770c0(0x906) != 0) {
        Func_0808a0f0(19, 0x01680000, 0x00a80000);  /* 180<<17, 168<<16 */
    }

    record = Func_0808a080(19);
    Func_080091e0(record, 0);

    Func_0808a158(22, 15);
    Func_0808a158(23, 15);
    Func_0808a158(24, 15);

    record = Func_0808a080(22);
    record = Func_0808a080(23);
    record[0x59] |= 8;
    record[0x59] |= 8;
    record = Func_0808a080(24);
    record[0x59] |= 8;

    record[0x23] |= 2;
    record = Func_0808a080(22);
    record = Func_0808a080(23);
    record[0x23] |= 2;
    record = Func_0808a080(24);
    record[0x23] |= 2;

    Func_0808a1e0(22, 1);
    Func_0808a1e0(23, 1);
    Func_0808a1e0(24, 1);

    Func_080000c0(1);

    Func_0808a018();
    Func_0808a210(*(s32 *)(origin + 8), *(s32 *)(origin + 12),
                  *(s32 *)(origin + 16), 0);
    Func_08009128();
    Func_0808a020();

    Func_080000c0(1);
}
