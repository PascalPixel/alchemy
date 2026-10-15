#include "mars_room_m.h"

void Func_02001aec(struct ObjectRuntime *object)
{
    object->motionFlags = 0;
    object->collisionFlags = 0;
    ObjectDispatch_SetSingleChildFlags(object, 0);
    Func_02003120(object, gData_020032d4);
    object->action = 0;
    object->speedLimit = 0;
    object->acceleration = 0;
}

void Func_02001b1c(struct MarsRoomMVector *position, s32 value, s32 flag)
{
    struct ObjectRuntime *object = (struct ObjectRuntime *)Value4(
        (s32 (*)())Func_02003128, 366, position->x, position->y, position->z);

    if (object != 0)
    {
        object->bounce = 0;
        object->gravity = 0;
        object->unk4c = value;
        ((void (*)())Func_02001aec)(object, 2);
        object->update = Func_02001988;
        if (flag != 0)
            object->unk5c[0] = 2;
        Func_02003118(object, 2);
    }
}
