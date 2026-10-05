#include "OBJECT_DISPATCH.H"

void ObjectDispatch_WaitForCommandEnd(struct DispatchObject *object)
{
    s32 cnt = 0;

    if (object->value_00 != 0 &&
        ((const s32 *)object->value_00)[object->value_04] != 17) {
        do {
            WaitFrames(1);
            cnt++;
            if (cnt > 599)
                break;
        } while (object->value_00 != 0 &&
                 ((const s32 *)object->value_00)[object->value_04] != 17);
    }
}

void ObjectDispatch_SetSingleChildField1a(struct DispatchObject *object, u32 value)
{
    if (object != NULL && (object->kind & 0xf) == 1)
        object->target.child->value_1a = value;
}
