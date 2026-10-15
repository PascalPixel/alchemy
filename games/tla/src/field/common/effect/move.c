#include "types.h"
#include "field_effect.h"

void BattleFx_UpdateObjectMotionScaleAndLinkedAngle(union FieldObject *object)
{
    s32                 x;
    s32                 velocityX;
    struct FieldSprite *sprite;

    x = object->effect.x;
    velocityX = object->effect.velocityX;
    sprite = object->effect.sprite;

    object->effect.x = x + velocityX;
    object->effect.y += object->effect.velocityY;
    object->effect.z += object->effect.velocityZ;
    object->effect.scaleX += object->effect.scaleRateX;
    object->effect.scaleY += object->effect.scaleRateY;
    sprite->rotation += object->effect.spin;
}
