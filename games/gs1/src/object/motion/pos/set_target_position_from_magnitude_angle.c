#include "types.h"

struct Object_08096bec {
    u8 padding[8];
    s32 x;
    s32 y;
    s32 z;
};

void RotateVectorByMagnitude(s32, s32, s32 *);
void Object_SetPosition(struct Object_08096bec *, s32, s32, s32);

void ObjectMotion_SetTargetPositionFromMagnitudeAngle(
    struct Object_08096bec *object, s32 magnitude, s32 angle)
{
    s32 values[3];

    /* 座標3成分を一括変換して書き戻す。 */
    if (object != 0) {
        values[0] = object->x;
        values[1] = object->y;
        values[2] = object->z;
        RotateVectorByMagnitude(magnitude, angle, values);
        Object_SetPosition(object, values[0], values[1], values[2]);
    }
}
