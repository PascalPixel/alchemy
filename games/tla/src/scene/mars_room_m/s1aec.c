#include "mars_room_m.h"

void Func_02001aec(struct ObjectRuntime *object)
{
    object->motion_flags = 0;
    object->collision_flags = 0;
    ObjectDispatch_SetSingleChildFlags(object, 0);
    Func_02003120(object, Data_020032d4);
    object->action = 0;
    object->speed_limit = 0;
    object->acceleration = 0;
}

void Func_02001b1c(struct MarsRoomMVector *position, s32 value, s32 flag)
{
    struct ObjectRuntime *object = (struct ObjectRuntime *)Value4((s32 (*)())Func_02003128, 366, position->x, position->y, position->z);

    if (object != 0) {
        object->bounce = 0;
        object->gravity = 0;
        object->unknown_4c = value;
        ((void (*)())Func_02001aec)(object, 2);
        object->update = Func_02001988;
        if (flag != 0)
            object->unknown_5c[0] = 2;
        Func_02003118(object, 2);
    }
}
