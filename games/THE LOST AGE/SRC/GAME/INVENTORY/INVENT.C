#include "TYPES.H"
#include "OBJECT_RUNTIME.H"
#include "SYSTEM.H"

void Inventory_ReservedNoOp(void)
{
}

void Object_WaitUntilChildValueDiffers(s32 object_id, s32 value)
{
    struct ObjectRuntime *obj = ObjectTable_Get(object_id);

    if (obj != 0 && obj->animation_kind == 1) {
        s32 cnt = 0;
        u8 *anim = obj->animation;

        while (cnt <= 89) {
            WaitFrames(1);
            if (value != anim[24]) {
                break;
            }
            cnt++;
        }
    }
}
