#include "scene.h"
#include "abi/object/motion/act/set_variant_callback.h"
#include "object_runtime.h"

void Object_SetCallback(struct ObjectRuntime *, const void *);
extern const u8 gRom[];

void Motion_SetVarCb(u32 object_id, s32 variant)
{
    struct ObjectRuntime *object;

    object = ObjectTable_Get(object_id);
    if (object != NULL && variant > 0) {
        if (variant > 3) {
            variant = 3;
        }
        Object_SetCallback(object,
            gRom + ((3 - variant) << 7));
    }
}
