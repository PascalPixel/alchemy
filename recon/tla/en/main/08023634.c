/*
 * Draft: ObjectDispatch_WaitForCommandEnd does not yet match; 6 bytes differ from +0x10.
 * Links as recon/tla/raw/08023634.s.
 */
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
