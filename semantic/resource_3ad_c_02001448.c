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
 * Call targets resolved with `tools/overlay_call_targets.ts`; all 90 sites are
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
void Func_0808a010();   /* wait n frames */
u8 *Func_0808a080();    /* scene entity record by selector */
void Func_0808a0b8();   /* move slot to (x, z) */
void Func_0808a0c8();
void Func_0808a0e0();
void Func_0808a0e8();
void Func_0808a0f0();
void Func_0808a100();
void Func_0808a110();   /* scene-presentation request */
void Func_0808a138();
void Func_0808a148();   /* scene request */
void Func_0808a170();   /* show a dialogue line by id */
void Func_0808a180();   /* wait for the slot's action to finish */
void Func_0808a1b8();   /* face the slot along a heading */
void Func_0808a1e8();   /* play animation id for n frames */
void Func_0808a200();
void Func_0808a218();
void Func_0808a220();
void Func_080770c8();   /* set a story flag */

void Func_02001448(void)
{
    u8 *actor;
    int line = 0x254e;

    Func_0808a1e8(1, 0x105, 60);
    Func_0808a148(1, 0, 0);
    Func_0808a170(line);
    Func_0808a180(1, 0);

    Func_0808a138(3, 1);
    Func_0808a170(line + 1);
    Func_0808a180(3, 0);

    Func_0808a148(2, 13, 0);
    Func_0808a010(60);
    Func_0808a170(line + 2);
    Func_0808a180(2, 0);

    Func_0808a148(13, 2, 0);
    Func_0808a1e8(13, 0x105, 70);
    Func_0808a170(line + 3);
    Func_0808a180(13, 0);

    Func_0808a110(12, 4);
    Func_0808a170(line + 4);
    Func_0808a180(12, 0);

    Func_0808a1b8(12, 0x3000, 0);
    Func_0808a010(20);
    Func_0808a110(12, 3);

    Func_0808a100(0, 3);
    Func_0808a100(1, 3);
    Func_0808a100(2, 3);
    Func_0808a100(3, 3);
    Func_0808a010(60);

    Func_0808a0e0(13, -16, 0);
    Func_0808a0e8(13);
    Func_0808a100(13, 1);
    Func_0808a010(40);
    Func_0808a110(13, 3);
    Func_0808a1b8(13, 0x5000, 0);
    Func_0808a010(30);

    Func_0808a100(0, 3);
    Func_0808a100(1, 3);
    Func_0808a100(2, 3);
    Func_0808a100(3, 3);

    Func_0808a0c8(12, 156, 528);
    Func_0808a010(20);
    Func_0808a0c8(13, 164, 528);
    Func_0808a0e8(12);
    Func_0808a0c8(12, 168, 640);
    Func_0808a0e8(13);
    Func_0808a0c8(13, 168, 640);

    Func_0808a1b8(0, 0x5000, 0);
    Func_0808a1b8(2, 0x5000, 0);
    Func_0808a1b8(3, 0x5000, 0);
    Func_0808a1b8(1, 0x5000, 0);
    Func_0808a010(20);

    Func_0808a1b8(0, 0x4000, 0);
    Func_0808a1b8(2, 0x4000, 0);
    Func_0808a1b8(3, 0x4000, 0);
    Func_0808a1b8(1, 0x4000, 0);
    Func_0808a010(60);

    Func_0808a0f0(13, 0, 0);
    Func_0808a0f0(12, 0, 0);
    Func_0808a010(110);

    Func_0808a170(line + 5);
    Func_0808a180(1, 0);
    Func_0808a110(3, 3);
    Func_0808a010(30);

    Func_0808a170(line + 6);
    Func_0808a180(3, 0);
    Func_0808a110(2, 3);

    Func_0808a170(line + 7);
    Func_0808a180(2, 0);
    Func_0808a010(140);

    Func_0808a148(1, 0, 0);
    Func_0808a170(line);
    line += 8;
    Func_0808a180(1, 0);

    /* Three identical blocks: send slot N to actor 0's current tile. */
    Func_0808a100(1, 2);
    actor = Func_0808a080(0);
    if (actor != 0) {
        Func_0808a0b8(1, *(s16 *)(actor + 10), *(s16 *)(actor + 18));
    }
    Func_0808a0e8(1);
    Func_0808a0f0(1, 0, 0);

    Func_0808a100(2, 2);
    actor = Func_0808a080(0);
    if (actor != 0) {
        Func_0808a0b8(2, *(s16 *)(actor + 10), *(s16 *)(actor + 18));
    }
    Func_0808a0e8(2);
    Func_0808a0f0(2, 0, 0);

    Func_0808a100(3, 2);
    actor = Func_0808a080(0);
    if (actor != 0) {
        Func_0808a0b8(3, *(s16 *)(actor + 10), *(s16 *)(actor + 18));
    }
    Func_0808a0e8(3);
    Func_0808a0f0(3, 0, 0);

    Func_0808a010(30);
    Func_0808a220(0, 1);
    Func_0808a218();
    Func_0808a200(0, 0);

    Func_080770c8(0x94f);
}
