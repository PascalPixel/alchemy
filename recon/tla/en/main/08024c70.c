/* Near miss, differ 21 of 29: the native keeps 0 in r2 for both byte stores,
   has no result register and returns 0 from a tail after the common exit;
   this draft keeps a done flag in r6. An else-if that returns 0 directly
   (differ 24) puts the 0 return before the step increment. */
#include "types.h"
#include "object_runtime.h"

#define BYTE_AT(base, offset) (*(u8 *)((u8 *)(base) + (offset)))

s32 Object_IsTargetUnset(struct ObjectRuntime *object);

s32 Func_08024c70(struct ObjectRuntime *object)
{
    s32 done = 0;

    if (BYTE_AT(object, 96) > 59) {
        BYTE_AT(object, 96) = 0;
        object->target_x = 0x80000000;
        object->target_y = 0x80000000;
        object->target_z = 0x80000000;
        object->unknown_56 = 0;
        done = 1;
    } else if (Object_IsTargetUnset(object)) {
        done = 1;
    }
    if (done) {
        object->step++;
        return 1;
    }
    return 0;
}
