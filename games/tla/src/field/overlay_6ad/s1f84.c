#include "room.h"

extern const u8 Data_02005d8c[];

void Func_02001e20(void);
void Func_02001ffc(void);
void Func_02001f84(struct ObjectRuntime *object, s32 unused)
{
    object->motion_flags = 0;
    object->collision_flags = 0;
    ObjectDispatch_SetSingleChildFlags(object, 0);
    Func_02005490(object, Data_02005d8c);
    object->action = 0;
    object->speed_limit = 0;
    object->acceleration = 0;
}

void Func_02001fb4(s32 *position, s32 speed, s32 mode)
{
    struct ObjectRuntime *object = Func_02005498_(366, position[0], position[1], position[2]);

    if (object != NULL) {
        object->bounce = 0;
        object->gravity = 0;
        object->unknown_4c = speed;
        Func_02001f84(object, 2);
        object->update = Func_02001e20;
        if (mode)
            object->unknown_5c[0] = 2;
        Func_02005480(object, 2);
    }
}
