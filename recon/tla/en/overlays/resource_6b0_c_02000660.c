/* Near miss (9 instructions differ): the native returns the remaining
 * radius != 0 as negs, orrs, lsrs #31 before it stores the radius; the
 * draft compares and branches. An explicit (left | -left) >> 31 gives that
 * tail but reorders the first loads; a u8 return changes nothing. */
#include "types.h"
#include "object_runtime.h"

#define BYTE_AT(base, offset) (*(u8 *)((u8 *)(base) + (offset)))

extern const u8 Data_02003674[];

void Vector_AddPolarOffsetFar(s32 radius, s32 angle, s32 *position);
struct ObjectRuntime *Engine_ObjectCreate(s32 kind, s32 x, s32 y, s32 z);
void Object_SetPartAttribute(struct ObjectRuntime *object, s32 attribute);
void Object_SetSpritePriority(struct ObjectRuntime *object, s32 priority);
void ObjectDispatch_SetSingleChildFlags(struct ObjectRuntime *object, s32 flags);
void Animation_SetStateFlags(struct ObjectRuntime *object, s32 flags);
void Func_020032e0(struct ObjectRuntime *object, s32 mode);
void Func_020032e8(struct ObjectRuntime *object, const u8 *script);

s32 Func_02000660(struct ObjectRuntime *object)
{
    object->action += object->unknown_66;
    object->x = object->bounce;
    object->y = object->gravity;
    object->z = 0x980000;
    Vector_AddPolarOffsetFar(object->unknown_4c, object->action, &object->x);
    object->unknown_4c -= 0x10000;
    return object->unknown_4c != 0;
}
