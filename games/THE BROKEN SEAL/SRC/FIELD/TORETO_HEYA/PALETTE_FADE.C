#include "HEYA.H"

void SceneEffect_RegisterPaletteFadeCallback(void)
{
    Engine_TaskRemoveCallback(ToretoPalette_ApplyTint);
}

/*
 * The decay of the Z velocity stays a signed divide by sixteen: that shape is
 * what reproduces the negative bias and arithmetic shift in the reference.
 */
void ToretoHeya_AdvanceEffectMotion(struct Effect *effect)
{
    s32 velocity_z;
    struct Sprite *sprite;
    s32 velocity_x;

    /* This block orders the Z load after the Y store; do not flatten it. */
    do {
        velocity_x = effect->velocity_x;
        effect->position[0] += velocity_x;
        effect->position[1] += effect->velocity_y;
    } while (0);
    velocity_z = effect->velocity_z;
    effect->position[2] += velocity_z;

    effect->velocity_x = velocity_x - Math_Divide(velocity_x, 18);
    effect->velocity_z = velocity_z - velocity_z / 16;

    effect->accum18 += effect->rate30;
    effect->accum1c += effect->rate34;

    sprite = effect->sprite;
    sprite->angle += effect->step64;
}
