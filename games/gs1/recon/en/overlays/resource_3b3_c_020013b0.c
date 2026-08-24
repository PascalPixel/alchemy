#include "types.h"
/*
 * resource_3b3 owner at 0x020013b0, 118 bytes.
 * The per-frame integrator with all three velocities decayed: each is added
 * into its position, then reduced by the shared helper's result. All three stay
 * live across the calls, which is why two high registers are saved.
 */
struct Sprite_3b3 {
    u8 pad00[30];
    u16 angle;                  /* +30 */
};
struct Effect_3b3 {
    u8 pad00[8];
    s32 position_x;             /* +8  */
    s32 position_y;             /* +12 */
    s32 position_z;             /* +16 */
    u8 pad14[4];
    s32 accum18;                /* +24 */
    s32 accum1c;                /* +28 */
    u8 pad20[16];
    s32 rate30;                 /* +48 */
    s32 rate34;                 /* +52 */
    u8 pad38[12];
    s32 velocity_x;             /* +68 */
    s32 velocity_y;             /* +72 */
    s32 velocity_z;             /* +76 */
    struct Sprite_3b3 *sprite;  /* +80 */
    u8 pad54[16];
    u16 step64;                 /* +100 */
};
s32 Func_02003e08();
s32 Func_02003e14();
s32 Func_02003e22();
void Func_020013b0(struct Effect_3b3 *effect)
{
    s32 velocity_x;
    s32 velocity_y;
    s32 velocity_z;

    velocity_x = effect->velocity_x;
    effect->position_x += velocity_x;

    velocity_y = effect->velocity_y;
    effect->position_y += velocity_y;

    velocity_z = effect->velocity_z;
    effect->position_z += velocity_z;

    effect->velocity_x = velocity_x - Func_02003e08(velocity_x, 10);
    effect->velocity_y = velocity_y - Func_02003e14(velocity_y, 3);
    effect->velocity_z = velocity_z - Func_02003e22(velocity_z, 10);

    effect->accum18 += effect->rate30;
    effect->accum1c += effect->rate34;

    effect->sprite->angle += effect->step64;
}
