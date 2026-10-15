#include "mars_room_m.h"

void Func_02001ed4(struct ObjectRuntime *object)
{
    object->motionFlags = 0;
    object->collisionFlags = 0;
    ObjectDispatch_SetSingleChildFlags(object, 0);
    Func_02003118(object, 2);
    Func_02003120(object, gData_020032d4);
    Object_SetPartAttribute(object, 10);
    object->action = 0;
    object->speedLimit = 0;
    object->acceleration = 0;
}
