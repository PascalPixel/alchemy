#include "types.h"

/*
 * Resource 3ad, cutscene at 0x02001760 (684 bytes, 76 call sites).
 *
 * Complete owner: `push {r5, lr}` at 0x02001760 and `pop {r5} / pop {r0} /
 * bx r0` at 0x02001a00.  The popped branch register is r0, so it holds the
 * return address and the owner is `void`.  One pool word (0x2558, the first
 * dialogue line id) sits at 0x02001a08, past the return and never reached as
 * code.  There is no pool inside the body - the control flow is straight-line
 * apart from three identical null guards.
 *
 * r5 is the dialogue line cursor: 0x2558, then +1, +2, then `adds r5, #3` to
 * 0x255b.  It is the only saved register.
 *
 * Call targets resolved with `tools/overlay_call_targets.ts`; all 76 sites are
 * import veneers and the per-target multiset below is reproduced exactly by the
 * C, target for target:
 *   Func_0808a100 x12, Func_0808a010 x11, Func_0808a1b8 x11, Func_0808a0e8 x7,
 *   Func_0808a0c8 x5,  Func_0808a0f0 x5,  Func_0808a110 x5,  Func_0808a170 x4,
 *   Func_0808a180 x4,  Func_0808a080 x3,  Func_0808a0b8 x3,  Func_0808a0e0 x2,
 *   Func_0808a148 x1,  Func_0808a220 x1,  Func_0808a248 x1,  Func_0808a368 x1.
 * (The inventory row says calls=66; that field predates the corrected overlay
 * `bl` decoding and is a known lower bound.)
 *
 * The three `Func_0808a080(0)` blocks read the actor record's s16 fields at
 * +10 and +18 - the integer parts of the 16.16 coordinate words at +8 and +16.
 * Note the trap here: the offset is materialised in r3 (`movs r3,#18`) and r3
 * is still live across the following `bl`, but Func_0808a0b8 takes three
 * arguments, not four; r3 is the load offset, not a phantom trailing argument.
 *
 * Heading constants are built by shift: 0xa0 << 7 = 0x5000, 0xc0 << 6 = 0x3000,
 * 0x80 << 7 = 0x4000.  Position constants likewise: 0x84 << 2 = 528 and
 * 0xa0 << 2 = 640.  -16 is `movs r1,#16 / negs r1,r1`.
 */

/* Old-style declarations: overlay import arities vary per call site. */
   /* wait n frames */
    /* scene entity record by selector */
   /* move slot to (x, z) */





   /* scene-presentation request */
   /* scene request */
   /* show a dialogue line by id */
   /* wait for the slot's action to finish */
   /* face the slot along a heading */




extern void Func_02003328();
extern void Func_02003318();
extern void Func_02003330();
extern void Func_02003300();
extern void Func_0200332e();
extern void Func_02003346();
extern void Func_02003320();
extern void Func_020032c6();
extern void Func_02003354();
extern void Func_0200336c();
extern void Func_02003380();
extern void Func_02003348();
extern void Func_020032f0();
extern void Func_0200337e();
extern void Func_02003396();
extern void Func_02003366();
extern void Func_0200330c();
extern void Func_02003374();
extern void Func_0200337c();
extern void Func_02003384();
extern void Func_02003332();
extern void Func_0200338c();
extern void Func_020033a4();
extern void Func_02003352();
extern void Func_020033ba();
extern void Func_02003406();
extern void Func_020033cc();
extern void Func_020033d4();
extern void Func_020033dc();
extern void Func_020033e4();
extern void Func_020033d0();
extern void Func_0200339e();
extern void Func_020033e2();
extern void Func_020033f8();
extern void Func_020033f4();
extern void Func_0200340a();
extern void Func_0200347a();
extern void Func_02003486();
extern void Func_02003492();
extern void Func_0200349e();
extern void Func_02003404();
extern void Func_020034b0();
extern void Func_020034bc();
extern void Func_020034c8();
extern void Func_020034d4();
extern void Func_0200343a();
extern void Func_02003494();
extern void Func_020034ae();
extern u8 * Func_0200347c();
extern void Func_020034ce();
extern void Func_020034de();
extern u8 * Func_020034ac();
extern void Func_020034ec();
extern void Func_020034fe();
extern void Func_0200350e();
extern u8 * Func_020034dc();
extern void Func_0200351c();
extern void Func_0200352e();
extern void Func_020034e4();
extern void Func_02003530();
extern void Func_0200353e();
extern void Func_020035de();
extern void Func_02003542();
extern void Func_02003510();
extern void Func_0200360c();
extern void Func_02003602();
void Func_02001760(void)
{
    u8 *actor;
    int line = 0x2558;

    Func_02003328(1, 0x5000, 0);
    Func_02003318(line);
    Func_02003330(1, 0);

    Func_02003300(2, 3);
    Func_0200332e(line + 1);
    Func_02003346(2, 0);

    Func_02003330(13, 2, 0);
    Func_02003320(13, 3);
    Func_020032c6(20);
    Func_02003354(line + 2);
    Func_0200336c(13, 0);

    Func_02003380(12, 0x3000, 0);
    Func_02003348(12, 3);
    Func_020032f0(30);
    Func_0200337e(line);
    line += 3;
    Func_02003396(12, 0);

    Func_02003366(13, 3);
    Func_0200330c(20);

    Func_0200336c(0, 3);
    Func_02003374(1, 3);
    Func_0200337c(2, 3);
    Func_02003384(3, 3);
    Func_02003332(80);

    Func_0200337e(13, -16, 0);
    Func_0200338c(13);
    Func_020033a4(13, 1);
    Func_02003352(40);
    Func_020033ba(13, 3);
    Func_02003406(13, 0x5000, 0);
    Func_0200336c(30);

    Func_020033cc(0, 3);
    Func_020033d4(1, 3);
    Func_020033dc(2, 3);
    Func_020033e4(3, 3);

    Func_020033d0(12, 152, 528);
    Func_0200339e(20);
    Func_020033e2(13, 160, 528);
    Func_020033f8(12);
    Func_020033f4(12, 168, 640);
    Func_0200340a(13);
    Func_02003406(13, 168, 640);

    Func_0200347a(0, 0x5000, 0);
    Func_02003486(2, 0x5000, 0);
    Func_02003492(3, 0x5000, 0);
    Func_0200349e(1, 0x5000, 0);
    Func_02003404(20);

    Func_020034b0(0, 0x4000, 0);
    Func_020034bc(2, 0x4000, 0);
    Func_020034c8(3, 0x4000, 0);
    Func_020034d4(1, 0x4000, 0);
    Func_0200343a(200);

    Func_02003494(13, 0, 0);
    Func_0200349e(12, 0, 0);

    /* Three identical blocks: send slot N to actor 0's current tile. */
    Func_020034ae(1, 2);
    actor = Func_0200347c(0);
    if (actor != 0) {
        Func_0200349e(1, *(s16 *)(actor + 10), *(s16 *)(actor + 18));
    }
    Func_020034bc(1);
    Func_020034ce(1, 0, 0);

    Func_020034de(2, 2);
    actor = Func_020034ac(0);
    if (actor != 0) {
        Func_020034ce(2, *(s16 *)(actor + 10), *(s16 *)(actor + 18));
    }
    Func_020034ec(2);
    Func_020034fe(2, 0, 0);

    Func_0200350e(3, 2);
    actor = Func_020034dc(0);
    if (actor != 0) {
        Func_020034fe(3, *(s16 *)(actor + 10), *(s16 *)(actor + 18));
    }
    Func_0200351c(3);
    Func_0200352e(3, 0, 0);

    Func_020034e4(30);
    Func_02003530(0, -16, 0);
    Func_0200353e(0);
    Func_020035de(0, 1);
    Func_02003542(0, 168, 640);
    Func_02003510(60);

    Func_0200360c();
    Func_02003602(3);
}
