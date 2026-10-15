#include "../../../../games/tla/src/scene/yallam/yallam.h"

s32 Func_02000088(struct ObjectRuntime *object, struct ObjectRuntime *target, s32 range, s32 force)
{
    s32 result = 0;
    s32 *targetPos;
    s32 *objectPos;

    if (BYTE_AT(object, 99) != 0) {
        object->movement_state = 1;
        return 1;
    }
    targetPos = &target->x;
    objectPos = &object->x;
    if (SceneActor_CalculateFixedPointDistance(targetPos, objectPos) < range || force != 0) {
        u32 angle = (u16)ArcTan2(target->z - object->z, *targetPos - *objectPos);
        u32 left = (angle - 0x1000) & 0xf000;
        u32 right = (angle + 0x1000) & 0xf000;
        u32 ahead = angle & 0xf000;
        u32 facing = object->angle & 0xf000;

        if (ahead == facing || right == facing || left == facing || force != 0) {
            object->movement_state = 1;
            Func_02002448(object, 1);
            result = 1;
        }
    } else {
        object->movement_state = 0;
        Func_02002448(object, 2);
    }
    return result;
}
