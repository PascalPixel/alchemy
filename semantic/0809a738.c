#include "types.h"

/*
 * Per-frame step of the drifting phase of a particle - the successor handler
 * that 0x0809a7f4 installs at +0x6c once its own 121 steps are up.
 *
 * Each step moves the particle along its current heading by a radius of
 * 128 << 10 plus a random amount, then turns the heading by 0xfff0.  The
 * halfword at +0x66 is a countdown of extra turns: while it is non-zero it is
 * decremented and the heading gets a further 128 << 4; when it reaches zero a
 * random draw reloads it - one in 2^16 of the draws (bits 11..26 clear) sets
 * a new countdown of 8 plus bits 12..27 of a second draw, the rest leave it
 * at zero.
 *
 * The step counter at +0x64 is advanced every frame; on the 101st step the
 * particle is handed to Func_08009098 with the descriptor at 0x0809f0b0.
 * `lsls r3,#16` before the compare is the usual halfword compare, so the test
 * is on the truncated counter.
 *
 * The two `mov ip, pc ; bx r5` sequences at 0x0809a760 and 0x0809a770 call
 * the pooled 0x03000118, the ARM fixed-point multiply relocated into IWRAM.
 * `mov ip, pc` sets the return address to the halfword after each `bx`
 * (0x0809a764 and 0x0809a774), and both landing sites consume r0 as that
 * call's result in the same frame with the live registers intact - the
 * ordinary indirect call already established for 0x0800ebec, not retained
 * assembly.  The `movs r0, r0` at 0x0809a75e in front of the first is
 * alignment filler.
 *
 * Excluded (14 bytes): the 2-byte `movs r0, r0` alignment filler at
 * 0x0809a7e6 and the three-word pool at 0x0809a7e8 holding 0x03000118,
 * 0x0000fff0 and 0x0809f0b0.  The single epilogue is at 0x0809a7da, reached
 * from every arm.  No direct caller; published as a Thumb pointer at
 * 0x0809a888, the pool word 0x0809a739 of 0x0809a7f4.
 */

/* 0x03000118, the IWRAM-relocated ARM fixed-point multiply. */
typedef s32 (*Multiply_0809a738)(s32 a, s32 b);

struct Particle_0809a738 {
    u8 unknown_00[6];
    u16 angle_06;
    s32 x_08;
    u8 unknown_0c[4];
    s32 z_10;
    u8 unknown_14[0x50];
    u16 steps_64;
    s16 turns_66;
};

extern u8 Data_0809f0b0[];

s32 Func_0800231c(s32 angle);
s32 Func_08002322(s32 angle);
u32 Func_08004458(void);
void Func_08009098(struct Particle_0809a738 *particle, void *descriptor);

void Func_0809a738(struct Particle_0809a738 *particle)
{
    Multiply_0809a738 multiply = (Multiply_0809a738)0x03000118;
    s32 radius = (s32)Func_08004458() + (128 << 10);
    s32 angle = particle->angle_06;
    s32 step_x = multiply(radius, Func_0800231c(angle));
    s32 step_z = multiply(radius, Func_08002322(angle));

    particle->x_08 += step_x;
    particle->z_10 += step_z;
    particle->angle_06 += 0xfff0;

    if (particle->turns_66 != 0) {
        particle->turns_66--;
        particle->angle_06 += 128 << 4;
    } else if ((Func_08004458() << 5) >> 16 == 0) {
        particle->turns_66 = (s16)(((Func_08004458() << 4) >> 16) + 8);
    }

    particle->steps_64++;
    if ((u16)particle->steps_64 == 101) {
        Func_08009098(particle, Data_0809f0b0);
    }
}
