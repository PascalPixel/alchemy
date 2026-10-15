#include "room.h"

extern const u8 Data_020056a0[];

void Func_02001ae0(void);
void Func_02001cbc(void);
void Func_02001f90(struct ObjectRuntime *object);

void Func_02001c44(struct ObjectRuntime *object, s32 unused)
{
    object->motionFlags = 0;
    object->collisionFlags = 0;
    ObjectDispatch_SetSingleChildFlags(object, 0);
    Func_02004bd0(object, Data_020056a0);
    object->action = 0;
    object->speedLimit = 0;
    object->acceleration = 0;
}

void Func_02001c74(s32 *position, s32 speed, s32 mode)
{
    struct ObjectRuntime *object = Func_02004bd8_(366, position[0], position[1], position[2]);

    if (object != NULL) {
        object->bounce = 0;
        object->gravity = 0;
        object->unk4c = speed;
        Func_02001c44(object, 2);
        object->update = Func_02001ae0;
        if (mode)
            object->unk5c[0] = 2;
        Func_02004bc0(object, 2);
    }
}
