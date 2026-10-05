#include "OBJECT_LOOKUP.H"
#include "OBJECT_DISPATCH.H"

void ObjectGroup_ApplyIndexedChildValue(struct DispatchObject *);
void Object_SetPartAttribute(struct DispatchObject *, s32);

void ObjectGroup_ConfigureChildValue(s32 object_id, s32 value)
{
    struct DispatchObject *object;

    object = ObjectTable_Get(object_id);
    if (object != NULL) {
        if (value & 0x100) {
            object->update = ObjectGroup_ApplyIndexedChildValue;
            return;
        }
        object->update = NULL;
        Object_SetPartAttribute(object, value);
    }
}
