#include "types.h"
/*
 * resource_3c9 owner at 0x020003a0, 94 bytes.
 * The per-frame integrator: adds the +68/+72/+76 velocity into the +8/+12/+16
 * position, decays the x and z velocities through the shared helper, folds the
 * two rates at +48/+52 into +24/+28 and advances the sprite angle.
 */
struct Sprite_3c9 {
    u8 pad00[30];
    u16 angle;                  /* +30 */
};
struct Effect_3c9 {
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
    struct Sprite_3c9 *sprite;  /* +80 */
    u8 pad54[16];
    u16 step64;                 /* +100 */
};
s32 Func_02005fb2();
s32 Func_02005fc0();
void Func_020003a0(struct Effect_3c9 *effect)
{
    s32 velocity_x;
    s32 velocity_z;

    velocity_x = effect->velocity_x;
    effect->position_x += velocity_x;
    effect->position_y += effect->velocity_y;
    velocity_z = effect->velocity_z;
    effect->position_z += velocity_z;

    effect->velocity_x = velocity_x - Func_02005fb2(velocity_x, 22);
    effect->velocity_z = velocity_z - Func_02005fc0(velocity_z, 20);

    effect->accum18 += effect->rate30;
    effect->accum1c += effect->rate34;

    effect->sprite->angle += effect->step64;
}
