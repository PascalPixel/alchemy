/* Draft, differ 4: the listing loads the sprite and its attribute byte one instruction later, around movs r3, #3 and ands. */
#include "../../../../games/tla/src/scene/aqua_rock_inside/local.h"

void Func_02003994(struct ObjectRuntime *object, s32 priority)
{
    object->motion_flags = 0;
    object->collision_flags = 0;
    object->priority_flags = 2;
    ((struct FieldSprite *)object->animation)->priority = priority;
    ObjectDispatch_SetSingleChildFlags(object, 0);
    Func_02003cc4(object, 2);
    Func_02003ccc(object, Data_020044c8);
    Object_SetPartAttribute(object, 10);
    object->action = 0;
    object->speed_limit = 0;
    object->acceleration = 0;
}
