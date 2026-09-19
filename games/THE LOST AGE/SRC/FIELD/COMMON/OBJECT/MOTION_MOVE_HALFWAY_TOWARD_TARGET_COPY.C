#include "OBJECT_RUNTIME.H"

void ObjectMotion_MoveHalfwayTowardTargetCopy(struct ObjectRuntime *object)
{
    struct ObjectRuntime *target = object->linked_object;

    object->x += (target->x - object->x) / 2;
    object->y += (target->y - object->y) / 2;
    object->z += (target->z - object->z) / 2;
}
