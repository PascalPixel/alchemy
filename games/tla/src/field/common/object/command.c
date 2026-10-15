#include "object_dispatch.h"

void ObjectDispatch_WaitForCommandEnd(struct DispatchObject *object)
{
    s32 cnt = 0;

    if (object->value00 != 0 && ((const s32 *)object->value00)[object->value04] != 17)
    {
        do
        {
            WaitFrames(1);
            cnt++;
            if (cnt > 599)
                break;
        } while (object->value00 != 0 && ((const s32 *)object->value00)[object->value04] != 17);
    }
}

void ObjectDispatch_SetChildByteValue(struct DispatchObject *object, u32 value)
{
    if (object != NULL && (object->kind & 0xf) == 1)
        object->target.child->value1a = value;
}
