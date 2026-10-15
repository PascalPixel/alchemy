#include "types.h"
#include "field_effect.h"

void Effect_AdvanceMotionLocal(union FieldObject *object)
{
    struct FieldEffect *effect = &object->effect;
    s32                 velocityX;
    s32                 velocityZ;
    s32                 scaleRateX;
    struct FieldSprite *sprite;

    velocityX = effect->velocityX;
    effect->x += velocityX;
    effect->y += effect->velocityY;
    velocityZ = effect->velocityZ;
    effect->z += velocityZ;

    effect->velocityX = velocityX - velocityX / 18;
    effect->velocityZ = velocityZ - velocityZ / 16;
    /* FAKEMATCH: the ARM9 scheduling would load the scale rate before storing the Z velocity. */
    __asm__("");

    scaleRateX = effect->scaleRateX;
    effect->scaleX += scaleRateX;
    sprite = effect->sprite;
    effect->scaleY += effect->scaleRateY;
    /* FAKEMATCH: and would address the spin before storing the Y scale. */
    __asm__("");
    sprite->rotation += effect->spin;
}
