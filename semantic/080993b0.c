#include "types.h"

/*
 * Per-frame update for a trailing effect emitter.  Two independent halves:
 *
 *   - while the emitter's phase at +0x64 is not -1, the emitter is parked at
 *     a fixed offset from the anchor block at 0x03001f30, with its x swung by
 *     a sine of the phase, and the phase steps once around a 64-entry cycle;
 *   - every third frame (the counter at 0x03001e40 modulo 3) a new drifting
 *     particle is spawned near the emitter, given the update handler
 *     Func_080992f0 and an animation, and released.
 *
 * The spawn point is the emitter's own position raised by 0x20000, then
 * pushed out along a random heading by a random distance; both randoms come
 * from Func_08004458, and Func_0800447c does the push.
 *
 * `ldr r3,[pc,#208]` loads 0x03000118 - the ARM fixed-point multiply
 * relocated into IWRAM - and the following `mov ip, pc ; bx r3` calls it.
 * `mov ip, pc` sets the return address to the instruction after the `bx`, so
 * control resumes in the same frame with every live register intact: an
 * ordinary indirect call, the shape already resolved at 0x0800ebec, not
 * retained assembly.
 *
 * Reached only as a published Thumb pointer at 0x0809966c.
 */

/* 0x03000118, the IWRAM-relocated ARM fixed-point multiply. */
typedef s32 (*Multiply_080993b0)(s32 a, s32 b);

/* The emitter and the particles it spawns share one object layout. */
struct Object_080993b0 {
    u8 unknown_00[8];
    s32 position_08[3];
    u8 unknown_14[4];
    s32 marker_18;
    s32 marker_1c;
    u8 unknown_20[0x18];
    s32 base_x_38;
    u8 unknown_3c[0xc];
    s32 field_48;
    u8 unknown_4c[9];
    u8 kind_55;
    u8 unknown_56[8];
    s16 size_5e;
    u8 unknown_60[4];
    s16 phase_64;
    u8 unknown_66[6];
    void (*handler_6c)(struct Object_080993b0 *object);
};

/* Anchor the emitter parks against; only the position triple is read. */
struct Anchor_080993b0 {
    s32 unknown_00;
    s32 position_04[3];
};

s32 Func_08002304(u32 value, u32 divisor);
s32 Func_08002322(s32 angle);
s32 Func_08004458(void);
void Func_0800447c(s32 distance, s32 heading, s32 position[3]);
void Func_08009098(struct Object_080993b0 *object, const void *program);
void Func_08009240(struct Object_080993b0 *object, s32 animation);
struct Object_080993b0 *Func_08096c80(s32 resource, s32 x, s32 y, s32 z);
void Func_080992f0(struct Object_080993b0 *object);

void Func_080993b0(struct Object_080993b0 *emitter)
{
    Multiply_080993b0 multiply = (Multiply_080993b0)0x03000118;
    struct Anchor_080993b0 *anchor =
        *(struct Anchor_080993b0 **)0x03001f30;
    struct Object_080993b0 *particle;
    s32 spawn[3];
    s32 phase;
    s32 heading;
    s32 distance;

    if (emitter->phase_64 != -1) {
        emitter->position_08[0] = anchor->position_04[0] +
            multiply(192 << 11, Func_08002322(emitter->phase_64 << 10));
        emitter->position_08[1] = anchor->position_04[1] + (128 << 13);
        emitter->position_08[2] = anchor->position_04[2];

        phase = (s16)(emitter->phase_64 + 1) + 64;
        emitter->phase_64 = phase % 64;
    }

    if (Func_08002304(*(u32 *)0x03001e40, 3) != 0) {
        return;
    }

    spawn[0] = emitter->position_08[0];
    spawn[1] = emitter->position_08[1] + (128 << 10);
    spawn[2] = emitter->position_08[2];

    distance = Func_08004458() * 6;
    heading = Func_08004458();
    Func_0800447c(distance, heading, spawn);

    particle = Func_08096c80(0x11d, spawn[0], spawn[1], spawn[2]);
    if (particle == NULL) {
        return;
    }

    particle->handler_6c = Func_080992f0;
    particle->marker_1c = 0x9999;
    particle->marker_18 = 0x9999;
    particle->kind_55 = 2;
    particle->field_48 = 229 << 1;
    particle->phase_64 = (u32)Func_08004458() >> 9;
    particle->base_x_38 = particle->position_08[0];
    Func_08009240(particle, 9);
    particle->size_5e = 72;
    Func_08009098(particle, (const void *)0x0809f0b0);
}
