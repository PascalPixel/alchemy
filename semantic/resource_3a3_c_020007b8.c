#include "types.h"

/*
 * Resource 3a3 actor-18 setup sequence at 0x020007b8 (148 bytes,
 * 0x020007b8 .. 0x0200084b, of which 0x02000838 .. 0x0200084b is the pool).
 *
 * Complete owner: `push {lr}` at 0x020007b8 and the interworking return
 * `pop {r0} / bx r0` at 0x02000834, so the owner returns nothing.
 *
 * All 11 call sites are placed: Func_0808a080 x1, Func_0808a090 x1,
 * Func_0808a0c0 x2, Func_080f9010 x1, Func_0808a088 x1, Func_080091f0 x2,
 * Func_0808a010 x2, Func_0808a100 x1.  (The inventory row says calls=9; that
 * field counts distinct targets and predates the corrected `bl` decoding, so
 * placed >= row count is the check.)
 *
 * LINK-BASE WITNESS.  The pool word at 0x0200083c is 0x020086f1, odd and in
 * the 0x0200_8xxx band: under the proven 0x02008000 overlay link base that is
 * file offset 0x06f1 = `Func_020006f0 + 1`.  It is stored into the record's
 * +0x6c callback slot, which both confirms the base for resource_3a3 (a second
 * independent witness alongside 0x02008c45 in `Func_02000d08`) and names the
 * 0x020006f0 row as this actor's per-frame callback.
 *
 * Uncertainty: r3 still holds 0x020086f1 when Func_0808a090 is entered at
 * 0x020007dc.  That is the documented "stored value register survives into the
 * next bl" artefact -- it is the store's value, not a fourth argument -- so
 * only r0/r1/r2 are asserted here.
 */

/* Imports.  Old-style declarations are mandatory in overlay sources. */
u8 *Func_0808a080();        /* scene/actor record accessor */
void Func_0808a090();
void Func_0808a0c0();
void Func_080f9010();
void Func_0808a088();
void Func_080091f0();
void Func_0808a010();       /* waits the given number of frames */
void Func_0808a100();

void Func_020007b8(void)
{
    u8 *record;

    record = Func_0808a080(18);

    *(u16 *)(record + 100) = 0;
    *(u16 *)(record + 102) = 0;
    *(s32 *)(record + 72) = 0x6666;             /* 16.16: ~0.4 */
    *(u32 *)(record + 108) = 0x020086f1;        /* Func_020006f0 + Thumb bit */

    /* The actor id 18 is reloaded into r0 for each of these; the record
     * pointer is not passed again. */
    Func_0808a090(18, 0x13333, 0x9999);
    Func_0808a0c0(18, 28, 460);                 /* `movs r2,#230 / lsls #1` */
    Func_0808a0c0(18, 24, 448);                 /* `movs r2,#224 / lsls #1` */

    Func_080f9010(229);
    Func_0808a088(18);

    Func_080091f0(0, 0x10000, 0x10000);         /* `movs #128 / lsls #9` */
    Func_0808a010(4);

    Func_080091f0(-1, -1, 0xe666);
    Func_0808a010(40);

    Func_0808a100(18, 1);
}
