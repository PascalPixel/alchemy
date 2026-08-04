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
void Func_0808a010();   /* wait n frames */
u8 *Func_0808a080();    /* scene entity record by selector */
void Func_0808a0b8();   /* move slot to (x, z) */
void Func_0808a0c8();
void Func_0808a0e0();
void Func_0808a0e8();
void Func_0808a0f0();
void Func_0808a100();
void Func_0808a110();   /* scene-presentation request */
void Func_0808a148();   /* scene request */
void Func_0808a170();   /* show a dialogue line by id */
void Func_0808a180();   /* wait for the slot's action to finish */
void Func_0808a1b8();   /* face the slot along a heading */
void Func_0808a220();
void Func_0808a248();
void Func_0808a368();

void Func_02001760(void)
{
    u8 *actor;
    int line = 0x2558;

    Func_0808a1b8(1, 0x5000, 0);
    Func_0808a170(line);
    Func_0808a180(1, 0);

    Func_0808a110(2, 3);
    Func_0808a170(line + 1);
    Func_0808a180(2, 0);

    Func_0808a148(13, 2, 0);
    Func_0808a110(13, 3);
    Func_0808a010(20);
    Func_0808a170(line + 2);
    Func_0808a180(13, 0);

    Func_0808a1b8(12, 0x3000, 0);
    Func_0808a110(12, 3);
    line += 3;
    Func_0808a010(30);
    Func_0808a170(line);
    Func_0808a180(12, 0);

    Func_0808a110(13, 3);
    Func_0808a010(20);

    Func_0808a100(0, 3);
    Func_0808a100(1, 3);
    Func_0808a100(2, 3);
    Func_0808a100(3, 3);
    Func_0808a010(80);

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

    Func_0808a0c8(12, 152, 528);
    Func_0808a010(20);
    Func_0808a0c8(13, 160, 528);
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
    Func_0808a010(200);

    Func_0808a0f0(13, 0, 0);
    Func_0808a0f0(12, 0, 0);

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
    Func_0808a0e0(0, -16, 0);
    Func_0808a0e8(0);
    Func_0808a220(0, 1);
    Func_0808a0c8(0, 168, 640);
    Func_0808a010(60);

    Func_0808a368();
    Func_0808a248(3);
}
