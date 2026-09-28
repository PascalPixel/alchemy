#include "OBJECT_DISPATCH.H"

void ObjectDispatch_SetSingleChildField1a(struct DispatchObject *object, u32 value)
{
    if (object != NULL && (object->kind & 0xf) == 1)
        object->target.child->value_1a = value;
}
