#include "room.h"

void Func_02004c68(struct ObjectRuntime *object, s32 mode);

s32 Func_0200308c(struct ObjectRuntime *object)
{
    s32 frame = object->action;

    if (frame == 0) {
        object->y = Map_GetTerrainHeight(0, object->x, object->z) + 0x100000;
        object->scale_x = frame;
        object->scale_y = frame;
        object->speed_limit = 0x10000;
        object->acceleration = 0x10000;
        Func_02004c68(object, 1);
    } else if (frame <= 30) {
        if (frame == 30) {
            Func_02004de0(136);
            Func_02004bc0(object->linked_object, 2);
        }
    } else if (frame <= 60) {
        object->scale_x = 0x10000;
        object->scale_y = 0x10000;
        {
            s32 half = (object->action / 2) & 1;

            Animation_ApplyChildValues(object->linked_object, half * 7);
        }
    } else {
        Func_02004bc0(object->linked_object, 1);
        Func_02004de0(184);
        object->y = Map_GetTerrainHeight(0, object->x, object->z);
        object->linked_object = 0;
        return 0;
    }
    object->action++;
    return 1;
}

s32 Func_02003138(struct ObjectRuntime *object)
{
    s32 frame = object->action;

    if (frame == 0) {
        Func_02004de0(136);
        Func_02004bc0(object->linked_object, 2);
        object->y = Map_GetTerrainHeight(0, object->x, object->z) + 0x100000;
        object->scale_x = frame;
        object->scale_y = frame;
        object->speed_limit = 0x10000;
        object->acceleration = 0x8000;
        Func_02004c68(object, 1);
    } else if (frame <= 32) {
        object->scale_x = 0x10000;
        object->scale_y = 0x10000;
        {
            s32 half = (object->action / 2) & 1;

            Animation_ApplyChildValues(object->linked_object, half * 7);
        }
    } else {
        Func_02004bc0(object->linked_object, 1);
        Func_02004de0(184);
        object->y = Map_GetTerrainHeight(0, object->x, object->z);
        object->linked_object = 0;
        return 0;
    }
    object->action++;
    return 1;
}
