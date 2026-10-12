#include "../../../../games/tla/src/field/overlay_676/ov676.h"

s32 Func_02000088(struct ObjectRuntime *object, struct ObjectRuntime *target, s32 range, s32 force)
{
    s32 result = 0;

    if (BYTE_AT(object, 99) != 0) {
        object->movement_state = 1;
        return 1;
    }
    if (SceneActor_CalculateFixedPointDistance(&target->x, &object->x) < range || force != 0) {
        u16 angle = ArcTan2(target->z - object->z, target->x - object->x);
        s32 left = (angle - 0x1000) & 0xf000;
        s32 right = (angle + 0x1000) & 0xf000;
        s32 ahead = angle & 0xf000;
        s32 facing = object->angle & 0xf000;

        if (ahead == facing || right == facing || left == facing || force != 0) {
            object->movement_state = 1;
            Func_02002448(object, 1);
            result = 1;
        }
    } else {
        object->movement_state = result;
        Func_02002448(object, 2);
    }
    return result;
}
