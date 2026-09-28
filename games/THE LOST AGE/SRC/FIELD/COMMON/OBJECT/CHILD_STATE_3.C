#include "OBJECT_DISPATCH.H"

void Animation_ApplyChildValues(struct DispatchObject *object)
{
    if (object != NULL && object->kind == 1)
        Animation_ApplyChildValuesToRecord(object->target.child);
}
