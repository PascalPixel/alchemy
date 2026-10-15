/* Near miss, differ 4 of 40, order only: the native builds the priority
 * mask (movs r3, #3) before it loads the sprite and masks before it loads
 * the attribute byte; the draft loads the sprite first. A direct
 * object->animation cast, a one-pass block and a masked temporary were
 * tried (differ 4, 4 and 15). */
#include "../../../../games/tla/src/scene/kandorean_temple/local.h"

void Func_02000930(struct ObjectRuntime *object, s32 priority)
{
    struct FieldSprite *sprite;

    object->motionFlags = 0;
    object->collisionFlags = 0;
    object->priorityFlags = 2;
    sprite = object->animation;
    sprite->priority = priority;
    ObjectDispatch_SetSingleChildFlags(object, 0);
    Func_02005130(object, 2);
    Func_02005138(object, Data_02005494);
    Object_SetPartAttribute(object, 9);
    object->action = 0;
    object->speedLimit = 0;
    object->acceleration = 0;
}
