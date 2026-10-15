/* Near miss, differ 2 of 36 (ported from The Broken Seal's
   Object_SetTargetAndCallback): the native loads the cleared flags' 0 from
   the pool (ldr r1) before it loads the target's acceleration; this draft
   loads the acceleration first. The flags cleared earlier, the speed limit
   first or a block before the acceleration differ 4 to 22; permute (3 min)
   found nothing. */
#include "types.h"
#include "object_runtime.h"

void ObjectDispatch_InitializeFar(struct ObjectRuntime *object, const void *callback);

void Func_080d3780(u32 objectId, s32 targetId, const void *callback)
{
    struct ObjectRuntime *first = ObjectTable_Get(objectId);
    struct ObjectRuntime *second = ObjectTable_Get(targetId & 0xff);

    if (first != NULL && second != NULL) {
        first->linkedObject = second;
        if (!(targetId & 0x10000)) {
            first->action = 40;
            first->acceleration = second->acceleration * 2;
            first->speedLimit = second->speedLimit;
            first->collisionFlags = 0;
        }
        ObjectDispatch_InitializeFar(first, callback);
    }
}
