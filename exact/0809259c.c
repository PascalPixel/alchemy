#include "object_runtime.h"

void Func_08009098(struct ObjectRuntime *, const void *);

void Func_0809259c(u32 object_id, s32 variant)
{
    struct ObjectRuntime *object;

    object = Func_0808ba1c(object_id);
    if (object != NULL && variant > 0) {
        if (variant > 3) {
            variant = 3;
        }
        Func_08009098(object,
            (const void *)(((3 - variant) << 7) + 0x0809EBFC));
    }
}
