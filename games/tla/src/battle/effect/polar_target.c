#include "types.h"
#include "object_runtime.h"

void Vector_AddPolarOffset(s32 radius, s32 angle, s32 *position);

void Motion_SetTargetPositionFromMagnitudeAngle(
    struct ObjectRuntime *object, s32 magnitude, s32 angle)
{
    s32 values[3];

    if (object != 0) {
        values[0] = object->x;
        values[1] = object->y;
        values[2] = object->z;
        Vector_AddPolarOffset(magnitude, angle, values);
        Object_SetPosition(object, values[0], values[1], values[2]);
    }
}
