#include "tundaria_tower.h"

void Func_020006d4(union FieldObject *object)
{
    object->effect.x += object->effect.velocity_x;
    object->effect.y += object->effect.velocity_y;
    object->effect.z += object->effect.velocity_z;
    object->effect.scale_x += object->effect.scale_rate_x;
    object->effect.scale_y += object->effect.scale_rate_y;
    object->effect.velocity_y -= 0x4ccc;
}
