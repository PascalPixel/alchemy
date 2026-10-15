#include "mars_room_m.h"

void Func_02001ed4(struct ObjectRuntime *object)
{
    object->motion_flags = 0;
    object->collision_flags = 0;
    ObjectDispatch_SetSingleChildFlags(object, 0);
    Func_02003118(object, 2);
    Func_02003120(object, Data_020032d4);
    Object_SetPartAttribute(object, 10);
    object->action = 0;
    object->speed_limit = 0;
    object->acceleration = 0;
}
