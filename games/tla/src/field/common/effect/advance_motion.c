#include "types.h"
#include "field_effect.h"

void Effect_AdvanceMotion(union FieldObject *object)
{
    struct FieldEffect *effect = &object->effect;
    s32 velocity_x;
    s32 velocity_z;
    s32 scale_rate_x;
    struct FieldSprite *sprite;

    velocity_x = effect->velocity_x;
    effect->x += velocity_x;
    effect->y += effect->velocity_y;
    velocity_z = effect->velocity_z;
    effect->z += velocity_z;

    effect->velocity_x = velocity_x - velocity_x / 18;
    effect->velocity_z = velocity_z - velocity_z / 16;
    /* FAKEMATCH: the ARM9 scheduling would load the scale rate before storing the Z velocity. */
    __asm__("");

    scale_rate_x = effect->scale_rate_x;
    effect->scale_x += scale_rate_x;
    sprite = effect->sprite;
    effect->scale_y += effect->scale_rate_y;
    /* FAKEMATCH: and would address the spin before storing the Y scale. */
    __asm__("");
    sprite->rotation += effect->spin;
}
