#include "types.h"

/*
 * Resource 3ad, cutscene at 0x02001448 (792 bytes, 90 call sites).
 *
 * Complete owner: `push {r5, lr}` at 0x02001448 and `pop {r5} / pop {r0} /
 * bx r0` at 0x0200174c.  The popped branch register is r0, so it holds the
 * return address and the owner is `void`.  Three pool words sit at
 * 0x02001754-0x0200175f (0x105, 0x254e, 0x94f), past the return and never
 * reached as code; the body itself is straight-line apart from three identical
 * null guards, so there is no pool to walk over.
 *
 * r5 is the dialogue line cursor: 0x254e, read at +0..+7, then `adds r5, #8`
 * to 0x2556 for the last line.
 *
 * Sibling of 0x02001760 - the two share the whole second half (the four-slot
 * heading sweeps, the position pairs, and the three "send slot N to actor 0's
 * tile" blocks), differing in dialogue ids, a few position constants and the
 * story flag written at the end.
 *
 * Call targets resolved with `cargo run --release --manifest-path tools/overlay-call-targets/Cargo.toml --`; all 90 sites are
 * import veneers and the per-target multiset below is reproduced exactly by the
 * C, target for target:
 *   Func_0808a010 x12, Func_0808a100 x12, Func_0808a1b8 x10, Func_0808a170 x9,
 *   Func_0808a180 x9,  Func_0808a0e8 x6,  Func_0808a0f0 x5,  Func_0808a110 x5,
 *   Func_0808a0c8 x4,  Func_0808a148 x4,  Func_0808a080 x3,  Func_0808a0b8 x3,
 *   Func_0808a1e8 x2,  and one each of Func_0808a0e0, Func_0808a138,
 *   Func_0808a200, Func_0808a218, Func_0808a220, Func_080770c8.
 * (The inventory row says calls=75, a known lower bound.)
 *
 * As in 0x02001760, the actor's s16 fields at +10 and +18 are the integer parts
 * of the 16.16 coordinate words at +8 and +16, and the r3 holding the load
 * offset survives into the following `bl` without being an argument.
 *
 * Shift-built constants: 0xc0 << 6 = 0x3000, 0xa0 << 7 = 0x5000,
 * 0x80 << 7 = 0x4000, 0x84 << 2 = 528, 0xa0 << 2 = 640; -16 is
 * `movs r1,#16 / negs r1,r1`.
 */

/* Old-style declarations: overlay import arities vary per call site. */
void Func_0200301e();
void Func_02002ff0();
void Func_02003008();
void Func_02003020();
void Func_02003000();
void Func_02003036();
void Func_02002fae();
void Func_0200303c();
void Func_02003054();
void Func_0200303e();
void Func_02003080();
void Func_0200305e();
void Func_02003076();
void Func_02003046();
void Func_02003074();
void Func_0200308c();
void Func_020030a0();
void Func_02003006();
void Func_0200306e();
void Func_0200307e();
void Func_02003086();
void Func_02003034();
void Func_0200308e();
void Func_020030a6();
void Func_020030bc();
void Func_02003108();
void Func_020030ce();
void Func_020030d6();
void Func_020030de();
void Func_020030e6();
void Func_020030d2();
void Func_020030e4();
void Func_020030fa();
void Func_020030f6();
void Func_0200310c();
void Func_0200317c();
void Func_02003188();
void Func_02003194();
void Func_020031a0();
void Func_02003106();
void Func_020031b2();
void Func_020031be();
void Func_020031ca();
void Func_020031d6();
void Func_0200313c();
void Func_02003196();
void Func_02003156();
void Func_020031e4();
void Func_020031fc();
void Func_020031cc();
void Func_02003172();
void Func_02003200();
void Func_02003218();
void Func_020031e8();
void Func_02003216();
void Func_0200322e();
void Func_0200319c();
void Func_02003220();
void Func_02003236();
void Func_0200324e();
u8 *Func_020031e4_b();
void Func_02003206();
void Func_02003224();
void Func_02003246();
u8 *Func_02003214();
void Func_02003254();
void Func_02003266();
void Func_02003276();
u8 *Func_02003244();
void Func_02003284();
void Func_02003296();
void Func_0200324c();
void Func_02003334();
void Func_02003330();
void Func_02003320();
void Func_02003256();
                        /* wait n frames */
                        /* scene entity record by selector */
                        /* move slot to (x, z) */

                     

                        /* scene-presentation request */
                     
                        /* scene request */
                        /* show a dialogue line by id */
                        /* wait for the slot's action to finish */
                        /* face the slot along a heading */
                        /* play animation id for n frames */

                     
                        /* set a story flag */

void Func_02001448(void)
{
    u8 *actor;
    int line = 0x254e;

    Func_0200301e(1, 0x105, 60);
    Func_02002ff0(1, 0, 0);
    Func_02003008(line);
    Func_02003020(1, 0);

    Func_02003000(3, 1);
    Func_0200301e(line + 1);
    Func_02003036(3, 0);

    Func_02003020(2, 13, 0);
    Func_02002fae(60);
    Func_0200303c(line + 2);
    Func_02003054(2, 0);

    Func_0200303e(13, 2, 0);
    Func_02003080(13, 0x105, 70);
    Func_0200305e(line + 3);
    Func_02003076(13, 0);

    Func_02003046(12, 4);
    Func_02003074(line + 4);
    Func_0200308c(12, 0);

    Func_020030a0(12, 0x3000, 0);
    Func_02003006(20);
    Func_0200306e(12, 3);

    Func_0200306e(0, 3);
    Func_02003076(1, 3);
    Func_0200307e(2, 3);
    Func_02003086(3, 3);
    Func_02003034(60);

    Func_02003080(13, -16, 0);
    Func_0200308e(13);
    Func_020030a6(13, 1);
    Func_02003054(40);
    Func_020030bc(13, 3);
    Func_02003108(13, 0x5000, 0);
    Func_0200306e(30);

    Func_020030ce(0, 3);
    Func_020030d6(1, 3);
    Func_020030de(2, 3);
    Func_020030e6(3, 3);

    Func_020030d2(12, 156, 528);
    Func_020030a0(20);
    Func_020030e4(13, 164, 528);
    Func_020030fa(12);
    Func_020030f6(12, 168, 640);
    Func_0200310c(13);
    Func_02003108(13, 168, 640);

    Func_0200317c(0, 0x5000, 0);
    Func_02003188(2, 0x5000, 0);
    Func_02003194(3, 0x5000, 0);
    Func_020031a0(1, 0x5000, 0);
    Func_02003106(20);

    Func_020031b2(0, 0x4000, 0);
    Func_020031be(2, 0x4000, 0);
    Func_020031ca(3, 0x4000, 0);
    Func_020031d6(1, 0x4000, 0);
    Func_0200313c(60);

    Func_02003196(13, 0, 0);
    Func_020031a0(12, 0, 0);
    Func_02003156(110);

    Func_020031e4(line + 5);
    Func_020031fc(1, 0);
    Func_020031cc(3, 3);
    Func_02003172(30);

    Func_02003200(line + 6);
    Func_02003218(3, 0);
    Func_020031e8(2, 3);

    Func_02003216(line + 7);
    Func_0200322e(2, 0);
    Func_0200319c(140);

    Func_02003220(1, 0, 0);
    Func_02003236(line);
    line += 8;
    Func_0200324e(1, 0);

    /* Three identical blocks: send slot N to actor 0's current tile. */
    Func_02003216(1, 2);
    actor = Func_020031e4_b(0);
    if (actor != 0) {
        Func_02003206(1, *(s16 *)(actor + 10), *(s16 *)(actor + 18));
    }
    Func_02003224(1);
    Func_02003236(1, 0, 0);

    Func_02003246(2, 2);
    actor = Func_02003214(0);
    if (actor != 0) {
        Func_02003236(2, *(s16 *)(actor + 10), *(s16 *)(actor + 18));
    }
    Func_02003254(2);
    Func_02003266(2, 0, 0);

    Func_02003276(3, 2);
    actor = Func_02003244(0);
    if (actor != 0) {
        Func_02003266(3, *(s16 *)(actor + 10), *(s16 *)(actor + 18));
    }
    Func_02003284(3);
    Func_02003296(3, 0, 0);

    Func_0200324c(30);
    Func_02003334(0, 1);
    Func_02003330();
    Func_02003320(0, 0);

    Func_02003256(0x94f);
}
