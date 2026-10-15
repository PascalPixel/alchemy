#include "types.h"
#include "ram_buffer.h"
#include "iwram_call.h"
#include "object_runtime.h"

struct EventObjects
{
    u8                    unk00[20];
    struct ObjectRuntime *objects[64];
};

struct ObjectRuntime *Engine_ObjectCreate(s32 kind, s32 x, s32 y, s32 z);
void                  Object_SetSpritePriority(struct ObjectRuntime *object, s32 priority);
void                  ObjectDispatch_SetSingleChildFlags(struct ObjectRuntime *object, s32 flags);
void                  Object_SetPartAttribute(struct ObjectRuntime *object, s32 value);
void                  Func_02003b84(s32 a, s32 b);

struct ObjectRuntime *Func_020000d0(s32 x, s32 y, s32 z, s32 kind)
{
    struct ObjectRuntime *object = Engine_ObjectCreate(kind, x, y, z);

    if (object != NULL)
    {
        Object_SetSpritePriority(object, 1);
        object->motionFlags = 0;
        object->collisionFlags = 8;
        ObjectDispatch_SetSingleChildFlags(object, 0);
        Object_SetPartAttribute(object, 15);
        object->priorityFlags |= 34;
        return object;
    }
    return NULL;
}
