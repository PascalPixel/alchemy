#include "object_runtime.h"

void Func_080030f8(s32);

void Func_08092504(u32 object_id)
{
    struct ObjectRuntime *object;
    u8 *ptr;
    volatile s32 saved;
    s32 i;

    object = Func_0808ba1c(object_id);
    if (object != NULL && object->animation_kind == 1) {
        ptr = object->animation;
        saved = ptr[36];
        for (i = 0; i <= 89; i++) {
            Func_080030f8(1);
            if (saved != ptr[36]) {
                break;
            }
        }
    }
}
