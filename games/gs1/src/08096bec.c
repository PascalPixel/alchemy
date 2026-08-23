#include "types.h"

struct Object_08096bec {
    u8 padding[8];
    s32 x;
    s32 y;
    s32 z;
};

void RotateVectorByMagnitude(s32, s32, s32 *);
void Object_SetPosition(struct Object_08096bec *, s32, s32, s32);

void Object_SetVelocity(struct Object_08096bec *object, s32 arg1, s32 arg2)
{
    s32 values[3];

    /* 座標3成分を一括変換して書き戻す。 */
    if (object != 0) {
        values[0] = object->x;
        values[1] = object->y;
        values[2] = object->z;
        RotateVectorByMagnitude(arg1, arg2, values);
        Object_SetPosition(object, values[0], values[1], values[2]);
    }
}
