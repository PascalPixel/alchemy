#include "object_runtime.h"

void Object_SetCallback(struct ObjectRuntime *, const void *);
extern const u8 Data_0809ebfc[];

void ObjectMotion_SetVariantCallback(u32 object_id, s32 variant)
{
    struct ObjectRuntime *object;

    object = ObjectTable_Get(object_id);
    if (object != NULL && variant > 0) {
        if (variant > 3) {
            variant = 3;
        }
        Object_SetCallback(object,
            Data_0809ebfc + ((3 - variant) << 7));
    }
}
