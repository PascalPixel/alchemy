#include "types.h"
#include "object_runtime.h"

#define BYTE_AT(base, offset) (*(u8 *)((u8 *)(base) + (offset)))

extern const u8 gData_02003674[];

struct ObjectRuntime *Engine_ObjectCreate(s32 kind, s32 x, s32 y, s32 z);
void Object_SetPartAttribute(struct ObjectRuntime *object, s32 attribute);
void Object_SetSpritePriority(struct ObjectRuntime *object, s32 priority);
void ObjectDispatch_SetSingleChildFlags(struct ObjectRuntime *object, s32 flags);
void Animation_SetStateFlags(struct ObjectRuntime *object, s32 flags);
void Func_020032e0(struct ObjectRuntime *object, s32 mode);
void Func_020032e8(struct ObjectRuntime *object, const u8 *script);

void Func_020006a4(struct ObjectRuntime *source)
{
    struct ObjectRuntime *object = Engine_ObjectCreate(672, source->x, source->y, source->z);

    object->motionFlags = 0;
    Object_SetPartAttribute(object, BYTE_AT(source, 98));
    Object_SetSpritePriority(object, 2);
    Func_020032e0(object, 1);
    ObjectDispatch_SetSingleChildFlags(object, 0);
    Func_020032e8(object, gData_02003674);
    Animation_SetStateFlags(object, 1);
}
