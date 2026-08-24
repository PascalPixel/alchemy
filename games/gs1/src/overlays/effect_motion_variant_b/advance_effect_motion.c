#include "types.h"

/*
 * resource_3c5 owner at 0x02000cf0, 90 bytes.
 *
 * The per-frame integrator for an effect record: it adds the +68/+72/+76
 * velocity into the +8/+12/+16 position, decays two of the three velocities,
 * accumulates the two rates at +48/+52 into +24/+28, and advances the sprite's
 * angle by the record's per-frame step.
 *
 * The record layout is the one established by resource_3c5_c_02000ae8.c, whose
 * comment names this shape directly: "a leaf routine that adds the +68/+72/+76
 * velocity into the +8/+12/+16 position".  The three position words at +8, +12
 * and +16 fall inside that source's leading pad and are named here.
 *
 * `velocity_z -= velocity_z / 16` is spelled as a signed divide: the reference
 * biases by 15 when the value is negative and then arithmetic-shifts by 4,
 * which is what the compiler emits for a division by a power of two.
 */

struct Sprite {
    u8 pad00[9];
    u8 flags9;
    u8 pad0a[20];
    u16 angle;
    u8 pad20[6];
    u8 state26;
};

struct Effect {
    u8 pad00[8];
    s32 position[3];
    u8 pad14[4];
    s32 accum18;
    s32 accum1c;
    u8 pad20[3];
    u8 flags23;
    u8 pad24[12];
    s32 rate30;
    s32 rate34;
    u8 pad38[12];
    s32 velocity[3];
    struct Sprite *sprite;
    u8 pad54;
    u8 mode55;
    u8 pad56[14];
    u16 step64;
    u8 pad66[6];
    u32 callback;
};

/* Relocated IWRAM helper: turns a value and a duration into a per-frame step. */
s32 Func_02003a8a();

void Func_02000cf0(struct Effect *effect)
{
    s32 velocity_z;
    struct Sprite *sprite;
    s32 velocity_x;

    /* Macro-shaped block keeps the following Z load after the Y store. */
    do {
        velocity_x = effect->velocity[0];
        effect->position[0] += velocity_x;
        effect->position[1] += effect->velocity[1];
    } while (0);
    velocity_z = effect->velocity[2];
    effect->position[2] += velocity_z;

    effect->velocity[0] = velocity_x - Func_02003a8a(velocity_x, 18);
    effect->velocity[2] = velocity_z - velocity_z / 16;

    effect->accum18 += effect->rate30;
    effect->accum1c += effect->rate34;

    sprite = effect->sprite;
    sprite->angle += effect->step64;
}
