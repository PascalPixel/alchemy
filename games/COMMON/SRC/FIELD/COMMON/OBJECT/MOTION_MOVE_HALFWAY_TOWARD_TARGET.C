#include "OBJECT_RUNTIME.H"

void ObjectMotion_MoveHalfwayTowardTarget(struct ObjectRuntime *object)
{
    struct ObjectRuntime *target = object->linked_object;

    object->x += (target->x - object->x) / 2;
    /* FAKEMATCH: ⚓️'s ARM9 scheduling would load the target's y before the object's own. */
    __asm__("");
    object->y += (target->y - object->y) / 2;
    /* FAKEMATCH: and the target's z before the object's own. */
    __asm__("");
    object->z += (target->z - object->z) / 2;
}
