#include "types.h"
#include "field_effect.h"

void Effect_MoveLocal(union FieldObject *object)
{
    s32 x;
    s32 velocity_x;
    struct FieldSprite *sprite;

    x = object->effect.x;
    velocity_x = object->effect.velocity_x;
    sprite = object->effect.sprite;

    object->effect.x = x + velocity_x;
    object->effect.y += object->effect.velocity_y;
    object->effect.z += object->effect.velocity_z;
    object->effect.scale_x += object->effect.scale_rate_x;
    object->effect.scale_y += object->effect.scale_rate_y;
    sprite->rotation += object->effect.spin;
}
