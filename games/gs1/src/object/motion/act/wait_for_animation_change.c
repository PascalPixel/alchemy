#include "object_runtime.h"

void WaitFrames(s32);

void ObjectMotion_WaitForAnimationChange(u32 object_id)
{
    struct ObjectRuntime *object;
    u8 *ptr;
    volatile s32 saved;
    s32 i;

    object = ObjectTable_Get(object_id);
    if (object != NULL && object->animation_kind == 1) {
        ptr = object->animation;
        saved = ptr[36];
        for (i = 0; i <= 89; i++) {
            WaitFrames(1);
            if (saved != ptr[36]) {
                break;
            }
        }
    }
}
