#include "object_runtime.h"

void Func_08009098(struct ObjectRuntime *, const void *);
extern const u8 Data_0809ebfc[];

void Func_0809259c(u32 object_id, s32 variant)
{
    struct ObjectRuntime *object;

    object = GetObject(object_id);
    if (object != NULL && variant > 0) {
        if (variant > 3) {
            variant = 3;
        }
        Func_08009098(object,
            Data_0809ebfc + ((3 - variant) << 7));
    }
}
