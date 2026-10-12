#include "ov6ac.h"

s32 Map_GetTerrainHeight(s32 layer, s32 x, s32 z);
void Func_020031a8(struct ObjectRuntime *object, s32 mode);
void Animation_ApplyChildValues(struct ObjectRuntime *object, s32 value);

s32 Func_02002f34(struct ObjectRuntime *object)
{
    s32 frame = object->action;

    if (frame == 0) {
        object->y = Map_GetTerrainHeight(0, object->x, object->z) + 0x100000;
        object->scale_x = frame;
        object->scale_y = frame;
        object->speed_limit = 0x10000;
        object->acceleration = 0x10000;
        Func_020031a8(object, 1);
    } else if (frame <= 30) {
        if (frame == 30) {
            Func_020032b8(136);
            Func_02003118(object->linked_object, 2);
        }
    } else if (frame <= 60) {
        object->scale_x = 0x10000;
        object->scale_y = 0x10000;
        {
            s32 half = (object->action / 2) & 1;

            Animation_ApplyChildValues(object->linked_object, half * 7);
        }
    } else {
        Func_02003118(object->linked_object, 1);
        Func_020032b8(184);
        object->y = Map_GetTerrainHeight(0, object->x, object->z);
        object->linked_object = 0;
        return 0;
    }
    object->action++;
    return 1;
}

s32 Func_02002fe0(struct ObjectRuntime *object)
{
    s32 frame = object->action;

    if (frame == 0) {
        Func_020032b8(136);
        Func_02003118(object->linked_object, 2);
        object->y = Map_GetTerrainHeight(0, object->x, object->z) + 0x100000;
        object->scale_x = frame;
        object->scale_y = frame;
        object->speed_limit = 0x10000;
        object->acceleration = 0x8000;
        Func_020031a8(object, 1);
    } else if (frame <= 32) {
        object->scale_x = 0x10000;
        object->scale_y = 0x10000;
        {
            s32 half = (object->action / 2) & 1;

            Animation_ApplyChildValues(object->linked_object, half * 7);
        }
    } else {
        Func_02003118(object->linked_object, 1);
        Func_020032b8(184);
        object->y = Map_GetTerrainHeight(0, object->x, object->z);
        object->linked_object = 0;
        return 0;
    }
    object->action++;
    return 1;
}
