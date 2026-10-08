#include "types.h"
#include "field_effect.h"

void BattleFx_UpdateObjectMotionScaleAndLinkedAngle(union FieldObject *object)
{
    object->effect.x += object->effect.velocity_x;
    object->effect.y += object->effect.velocity_y;
    object->effect.z += object->effect.velocity_z;
    object->effect.scale_x += object->effect.scale_rate_x;
    object->effect.scale_y += object->effect.scale_rate_y;
    object->effect.sprite->rotation += object->effect.spin;
}
