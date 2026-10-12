/* Near miss, differ 4 of 30: the listing builds the mask 3 and masks the layer before it
 * loads the animation and its flag byte; this draft loads them first. A barrier on the layer or on
 * the animation pointer does not move them. */
#include "../../../../games/tla/src/field/overlay_6ac/ov6ac.h"

void Func_02001cc4(struct ObjectRuntime *object, s32 layer)
{
    object->motion_flags = 0;
    object->collision_flags = 0;
    ((struct Ov6acAnimation *)object->animation)->layer = layer;
    ObjectDispatch_SetSingleChildFlags(object, 0);
    Func_02003120(object, Data_020032d4);
    object->action = 0;
    object->speed_limit = 0;
    object->acceleration = 0;
}
