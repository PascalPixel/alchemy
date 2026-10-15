#include "types.h"
#include "field_effect.h"

static void Effect_AdvanceMotion(union FieldObject *object)
{
    struct FieldEffect *effect = &object->effect;

    effect->x += effect->velocityX;
    effect->y += effect->velocityY;
    effect->z += effect->velocityZ;
    effect->velocityX -= effect->velocityX / 18;
    effect->velocityZ -= effect->velocityZ / 16;
    effect->scaleX += effect->scaleRateX;
    effect->scaleY += effect->scaleRateY;
    effect->sprite->rotation += effect->spin;
}
