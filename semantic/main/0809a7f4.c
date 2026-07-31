#include "types.h"

/*
 * Per-frame step of a particle that circles its anchor.  The particle's angle
 * is the halfword at +6; the step places it on a circle of radius 128 << 12
 * around the anchor block hanging off +0x68, advances the angle by 128 << 4,
 * and counts the step in the halfword at +0x64.
 *
 * On the 121st step the particle is handed to its successor: the handler word
 * at +0x6c becomes 0x0809a739, the Thumb pointer to Func_0809a738, the step
 * and sub-step counters at +0x64 and +0x66 are cleared, +0x48 is loaded with
 * 0x00001999, +0x28 with 192 << 10, and the angle is reseeded from the random
 * source at Func_08004458.  `lsls r3,#16` before the compare is the usual
 * halfword compare, so the test is on the truncated counter.
 *
 * The two `mov ip, pc ; bx r5` sequences at 0x0809a818 and 0x0809a828 call
 * the pooled 0x03000118, the ARM fixed-point multiply relocated into IWRAM.
 * `mov ip, pc` sets the return address to the halfword after each `bx`
 * (0x0809a81c and 0x0809a82c), and both landing sites consume r0 as that
 * call's result in the same frame with the live registers intact - the
 * ordinary indirect call already established for 0x0800ebec, not retained
 * assembly.
 *
 * Excluded (12 bytes): the three-word pool at 0x0809a884 holding 0x03000118,
 * 0x0809a739 and 0x00001999; there is no alignment filler, the code ends
 * flush at 0x0809a884.  The single epilogue is at 0x0809a876, reached both
 * by the counter test and by the hand-off arm.  No direct caller; published
 * as a Thumb pointer at 0x0809aa94.
 */

/* 0x03000118, the IWRAM-relocated ARM fixed-point multiply. */
typedef s32 (*Multiply_0809a7f4)(s32 a, s32 b);

struct Anchor_0809a7f4 {
    u8 unknown_00[8];
    s32 x_08;
    u8 unknown_0c[4];
    s32 z_10;
};

struct Particle_0809a7f4 {
    u8 unknown_00[6];
    u16 angle_06;
    s32 x_08;
    u8 unknown_0c[4];
    s32 z_10;
    u8 unknown_14[0x14];
    s32 speed_28;
    u8 unknown_2c[0x1c];
    s32 value_48;
    u8 unknown_4c[0x18];
    u16 steps_64;
    u16 substep_66;
    struct Anchor_0809a7f4 *anchor_68;
    u32 handler_6c;
};

s32 Func_0800231c(s32 angle);
s32 Func_08002322(s32 angle);
u32 Func_08004458(void);

void Func_0809a7f4(struct Particle_0809a7f4 *particle)
{
    Multiply_0809a7f4 multiply = (Multiply_0809a7f4)0x03000118;
    struct Anchor_0809a7f4 *anchor = particle->anchor_68;
    s32 radius = 128 << 12;
    s32 angle = particle->angle_06;

    particle->x_08 = anchor->x_08 + multiply(radius, Func_0800231c(angle));
    particle->z_10 = anchor->z_10 + multiply(radius, Func_08002322(angle));
    particle->angle_06 += 128 << 4;

    particle->steps_64++;
    if ((u16)particle->steps_64 == 121) {
        particle->handler_6c = 0x0809a739;
        particle->steps_64 = 0;
        particle->substep_66 = 0;
        particle->value_48 = 0x00001999;
        particle->speed_28 = 192 << 10;
        particle->angle_06 = (u16)Func_08004458();
    }
}
