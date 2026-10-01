#include "OBJECT_LOOKUP.H"
#include "OBJECT_DISPATCH.H"

void ObjectGroup_ApplyIndexedChildValue(struct DispatchObject *);
void Object_SetPartAttribute(struct DispatchObject *, s32);

void ObjectGroup_ConfigureChildValue(s32 object_id, s32 value)
{
    s32 flags;
    struct DispatchObject *object;

    object = ObjectTable_Get(object_id);
    if (object != NULL) {
        flags = 0x100 & value;
        if (flags != 0) {
            *(void (**)(struct DispatchObject *))((u8 *)object + 0x6c) =
                ObjectGroup_ApplyIndexedChildValue;
            return;
        }
        *(s32 *)((u8 *)object + 0x6c) = flags;
        Object_SetPartAttribute(object, value);
    }
}
