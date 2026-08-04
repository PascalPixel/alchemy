#include "types.h"

/*
 * One step of a wander script: pick a random step that stays clear of the
 * world and keeps the actor inside its leash, and commit to it.
 *
 * The three script words at program[step + 1 ..  step + 3] are the base
 * distance, the spread the random draw is scaled by, and the leash radius.
 * The radius is taken to whole units with the usual truncating shift - add
 * 0xffff when negative, then arithmetic-shift right by 16 - and squared once,
 * outside the loop.
 *
 * Up to seven candidates are tried.  Each is the actor's own position pushed
 * out by base + random * spread along a heading jittered from the actor's
 * own by two quarter-scale random draws.  A candidate is rejected if
 * Func_0800d924 or Func_080120dc objects to it, and then again if
 * Func_080120dc objects to the probe a further 128 << 12 out at the heading
 * turned by +128 << 6 or by -128 << 6.  The straight-ahead probe at that
 * further distance is built but not tested, exactly as the code has it.
 * Finally the candidate must land within the leash radius of the anchor held
 * in the halfwords at +0x64 and +0x66.
 *
 * On success the actor is moved there by Func_0800d14c, the script index
 * advances by four, and the step reports 1.  If all seven candidates fail the
 * actor turns 128 << 8 - half a turn - the byte pair at +0x5e is set to 1,
 * and the step reports 0.
 *
 * `ldr r3,[pc,#300]` loads the pooled 0x03000118, the ARM fixed-point
 * multiply relocated into IWRAM, and `mov ip, pc ; bx r3` at 0x0800ddd0
 * calls it: `mov ip, pc` sets the return address to the halfword after the
 * `bx`, 0x0800ddd4, which consumes r0 as that call's result in the same frame
 * with the live registers intact - the ordinary indirect call already
 * established for 0x0800ebec, not retained assembly.
 *
 * The object is the 0x70-byte record of 0x0800ebec; the halfwords this owner
 * reads at +0x64 and +0x66 are the leash anchor, which is the cell-coordinate
 * reading of Object_0800c150 rather than the animation timer and phase that
 * 0x0800ebec names them.
 *
 * Excluded (12 bytes): the three-word pool at 0x0800def8 holding 0x0000ffff,
 * 0x03000118 and 0xffffe000; there is no alignment filler, the code ends
 * flush at 0x0800def8.  The single epilogue is at 0x0800dee6, reached by the
 * success arm and by the give-up arm.  No direct caller; published as a
 * Thumb pointer at 0x08013644.
 */

/* 0x03000118, the IWRAM-relocated ARM fixed-point multiply. */
typedef s32 (*Multiply_0800dd70)(s32 a, s32 b);

struct Actor_0800dd70 {
    const s32 *program_00;
    s16 step_04;
    u16 heading_06;
    s32 position_08[3];
    u8 unknown_14[0x4a];
    u16 blocked_5e;
    u8 unknown_60[4];
    s16 anchor_x_64;
    s16 anchor_z_66;
    u8 unknown_68[8];
};

u32 Func_08004458(void);
void Func_0800447c(s32 distance, s32 angle, s32 position[3]);
s32 Func_0800d924(const struct Actor_0800dd70 *actor, const s32 position[3]);
s32 Func_080120dc(const struct Actor_0800dd70 *actor, const s32 position[3]);
void Func_0800d14c(struct Actor_0800dd70 *actor, s32 x, s32 y, s32 z);

s32 Func_0800dd70(struct Actor_0800dd70 *actor)
{
    Multiply_0800dd70 multiply = (Multiply_0800dd70)0x03000118;
    const s32 *entry = &actor->program_00[actor->step_04 + 1];
    s32 base = entry[0];
    s32 spread = entry[1];
    s32 radius = entry[2];
    s32 leash;
    s32 attempt;

    if (radius < 0) {
        radius += 0xffff;
    }
    radius >>= 16;
    leash = radius * radius;

    for (attempt = 1; attempt <= 7; attempt++) {
        s32 probe[3];
        s32 far[3];
        s32 distance;
        s32 heading;
        s32 left;
        s32 right;
        s32 dx;
        s32 dz;
        s32 x;
        s32 z;

        probe[0] = actor->position_08[0];
        probe[1] = actor->position_08[1];
        probe[2] = actor->position_08[2];

        distance = base + multiply((s32)Func_08004458(), spread);

        left = (s32)Func_08004458() >> 2;
        right = (s32)Func_08004458() >> 2;
        heading = actor->heading_06 + left - right;

        Func_0800447c(distance, heading, probe);

        if (Func_0800d924(actor, probe) != 0) {
            continue;
        }
        if (Func_080120dc(actor, probe) != 0) {
            continue;
        }

        distance += 128 << 12;

        far[0] = actor->position_08[0];
        far[1] = actor->position_08[1];
        far[2] = actor->position_08[2];
        Func_0800447c(distance, heading, far);

        far[0] = actor->position_08[0];
        far[1] = actor->position_08[1];
        far[2] = actor->position_08[2];
        Func_0800447c(distance, heading + (128 << 6), far);
        if (Func_080120dc(actor, far) != 0) {
            continue;
        }

        far[0] = actor->position_08[0];
        far[1] = actor->position_08[1];
        far[2] = actor->position_08[2];
        Func_0800447c(distance, heading - (128 << 6), far);
        if (Func_080120dc(actor, far) != 0) {
            continue;
        }

        x = probe[0];
        if (x < 0) {
            x += 0xffff;
        }
        dx = (x >> 16) - actor->anchor_x_64;

        z = probe[2];
        if (z < 0) {
            z += 0xffff;
        }
        dz = (z >> 16) - actor->anchor_z_66;

        if (dx * dx + dz * dz <= leash) {
            Func_0800d14c(actor, probe[0], probe[1], probe[2]);
            actor->step_04 += 4;
            return 1;
        }
    }

    actor->heading_06 += 128 << 8;
    actor->blocked_5e = 1;
    return 0;
}
