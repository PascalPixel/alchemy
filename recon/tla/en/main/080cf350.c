/* Near miss, differ 2: the native copies the owner out of r0 after the motionFlags
 * byte store; this draft copies it one instruction earlier. Tried: store order, a result
 * temporary, empty and "+l"/"+r" asm barriers after the stores, a one-pass block,
 * two minutes of permute. */
#include "types.h"
#include "object_runtime.h"

s32 EventRuntime_GetControlledOwner(void);
void WaitFrames(s32);
void ObjectDispatch_SetSpritePriorityFar(struct ObjectRuntime *object, s32 priority);
void ObjectDispatch_InitializeFar(struct ObjectRuntime *object, const u8 *script);
extern const u8 Data_080f0074[];

void Func_080cf350(struct ObjectRuntime *object)
{
    struct ObjectRuntime *owner;

    if (object == 0)
        return;

    owner = ObjectTable_Get(EventRuntime_GetControlledOwner());
    object->acceleration = 0x10000;
    object->speedLimit = 0x20000;
    object->motionFlags = 0;
    Object_SetPosition(object, owner->x, owner->y + 0x240000, owner->z);
    ObjectDispatch_SetSpritePriorityFar(object, 1);
    WaitFrames(3);
    Object_SetMode(owner, 28);
    ObjectDispatch_InitializeFar(object, Data_080f0074);
    owner->angle = 0x4000;
}
