#include "types.h"

/*
 * resource_399 scripted step at 0x02000384, 396 bytes.
 *
 * Complete owner: `push {lr}` at 0x02000384 and the interworking return
 * `pop {r0} / bx r0` at 0x020004d6.  The popped register is r0, so it holds
 * the return address — the owner is `void`.  Bytes 0x020004da-0x0200050f are
 * alignment plus the thirteen-word literal pool, reached only by
 * `ldr rN, [pc, #imm]`; all three arms branch over it to the shared epilogue.
 *
 * Call targets resolved with `bun tools/overlay_call_targets.ts resource_399
 * 0384 --annotate` (an overlay `bl` stores `target - 2`, so the disassembler's
 * own annotations are wrong).  41 sites, 22 distinct imports.  Per-target
 * multiset against the assembly, which is what the C below spells:
 *   Func_080770c0 x2, Func_080770c8 x1, Func_0808a018 x3, Func_0808a020 x3,
 *   Func_0808a010 x3, Func_0808a080 x2, Func_0808a090 x1, Func_0808a098 x2,
 *   Func_0808a0a8 x1, Func_0808a0b0 x1, Func_0808a100 x2, Func_0808a128 x1,
 *   Func_0808a130 x3, Func_0808a148 x1, Func_0808a170 x3, Func_0808a180 x3,
 *   Func_0808a188 x2, Func_0808a190 x1, Func_0808a1b8 x2, Func_0808a1e8 x1,
 *   Func_0808a1f0 x1, Func_08009178 x2.
 * (The inventory's calls=40 predates the corrected `bl` decoding and is a
 * lower bound, not a check.)
 *
 * Shape: a three-way scene variant chosen by two story flags, 0x881 first and
 * 0x82b second, each arm its own Func_0808a018/Func_0808a020 section.  The
 * third arm is the long one and ends by setting flag 0x82c, so the flags read
 * as "this scene has already played" markers of the usual kind.
 *
 * LINK BASE.  Every pointer-shaped pool word is EVEN and lies in the
 * 0x0200axxx/0x0200bxxx band, so under the proven 0x02008000 overlay link base
 * they are in-image data at `value - 0x8000`: 0x31c0 and 0x31d6 (camera
 * targets, in the same table region the owner at 0x020006c0 indexes through
 * the word table at 0x3190), and 0x24f4, 0x2564, 0x2508 (animation scripts,
 * the established second argument of Func_0808a098/Func_0808a0b0).
 *
 * Constants built by shifts, as usual for 16.16 values and angles:
 * `movs #128 ; lsls #1` = 256, `movs #208 ; lsls #8` = 0xd000,
 * `movs #129 ; lsls #1` = 258 (the same pose id the owner at 0x020002b8
 * uses), `movs #192 ; lsls #6` = 0x3000.
 *
 * Uncertainty: the halfword written at +100 of channel 9's record, and the
 * bit-1 toggle `record[89] ^= 2` on channel 8's, are reproduced as written —
 * nothing in this overlay reads either field back, so their meaning is not
 * established here.  Note the toggle really is an exclusive-or, not a set:
 * `ldrb / eors #2 / strb`.
 */

extern u8 Data_0200a4f4[];      /* in-image animation script, file offset 0x24f4 */
extern u8 Data_0200a508[];      /* in-image animation script, file offset 0x2508 */
extern u8 Data_0200a564[];      /* in-image animation script, file offset 0x2564 */
extern u8 Data_0200b1c0[];      /* in-image camera target, file offset 0x31c0 */
extern u8 Data_0200b1d6[];      /* in-image camera target, file offset 0x31d6 */

/* Imports, named by the main-image address in the trailing word of the overlay
 * veneer each call site reaches.  Old-style declarations are mandatory. */
           /* frame wait */
           /* scripted-section open */
           /* scripted-section close */
           /* 16.16 pair setter */
           /* (selector, script) */

           /* (selector, script) */



           /* scene request */
           /* message line */



           /* orientation over frames */


           /* camera placement */
           /* story-flag set */
            /* scene record for a selector, or 0 */
            /* story-flag query */

extern s32 Func_0200259a();
extern void Func_020025c2();
extern void Func_0200267c();
extern void Func_020025ca();
extern void Func_020026a8();
extern void Func_020026c8();
extern void Func_020025ec();
extern s32 Func_020025cc();
extern void Func_020025f4();
extern void Func_02002684();
extern void Func_020025b6();
extern void Func_020026dc();
extern void Func_020026ec();
extern void Func_020026a4();
extern void Func_020025d6();
extern void Func_02002632();
extern void Func_02002630();
extern u8 * Func_0200264e();
extern void Func_0200266c();
extern void Func_0200271a();
extern void Func_0200272a();
extern void Func_02002690();
extern void Func_02002764();
extern void Func_02002760();
extern void Func_02002718();
extern void Func_02002762();
extern void Func_020026b2();
extern void Func_020026b4();
extern void Func_0200269a();
extern void Func_02002744();
extern void Func_02002754();
extern void Func_020027c6();
extern void Func_020026bc();
extern void Func_020027ae();
extern void Func_020027ca();
extern void Func_02002782();
extern void Func_020027c2();
extern u8 * Func_02002708();
extern void Func_020026e8();
extern void Func_0200270c();
void Func_02000384(void)
{
    u8 *record;

    if (Func_0200259a(0x881) != 0) {
        Func_020025c2();
        Func_0200267c(9, 0, 0);
        Func_020025ca(10);
        Func_020026a8(0x1644);
        Func_020026c8(9, 0);
        Func_020025ec();
        return;
    }

    if (Func_020025cc(0x82b) != 0) {
        Func_020025f4();
        Func_02002684(9, 7);
        Func_020025b6(Data_0200b1c0, 10, 69);
        return;
        Func_020026dc(0x156c);
        Func_020026ec(9, 0);
        Func_020026a4(9, 8);
        Func_020025d6(Data_0200b1d6, 10, 69);
        Func_02002632();
    }

    Func_02002630();

    *(u16 *)(record + 100) = 10;
    record = Func_0200264e(9);

    Func_0200266c(9, Data_0200a4f4);
    Func_0200271a(0x1534);
    Func_0200272a(9, 0);

    Func_02002690(8);
    Func_02002764(8, 256, 40);
    Func_02002760(8, 0xd000, 10);
    Func_02002718(8, 2);
    Func_02002762(8, 0, 20);

    Func_020026b2(0, Data_0200a564);
    Func_020026b4(8, 0x19999, 0xcccc);
    Func_020026dc(8, Data_0200a508);
    Func_0200269a(40);

    Func_02002744(8, 2, 0);
    Func_02002754(8, 2);
    Func_020027c6(8, 258);
    Func_020026bc(60);

    Func_020027ae(8, 0, 10);
    Func_020027ca(8, 0x3000, 20);
    Func_02002782(8, 2);
    Func_020027c2(8, 0);

    record = Func_02002708(8);
    record[89] ^= 2;

    Func_020026e8(0x82c);
    Func_0200270c();
}
