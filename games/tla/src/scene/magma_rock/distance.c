#include "types.h"
#include "ram_buffer.h"
#include "iwram_call.h"
#include "object_runtime.h"

struct EventObjects {
    u8 unknown_00[20];
    struct ObjectRuntime *objects[64];
};

struct ObjectRuntime *Engine_ObjectCreate(s32 kind, s32 x, s32 y, s32 z);
void Object_SetSpritePriority(struct ObjectRuntime *object, s32 priority);
void ObjectDispatch_SetSingleChildFlags(struct ObjectRuntime *object, s32 flags);
void Object_SetPartAttribute(struct ObjectRuntime *object, s32 value);
void Func_02003b84(s32 a, s32 b);

void Func_02000594(void)
{
    Func_02003b84(8, 56);
}

s32 Func_020005a0(s32 *a, s32 *b)
{
    s32 dx = (a[0] - b[0]) >> 16;
    s32 dz = (a[2] - b[2]) >> 16;

    return Iwram_Sqrt(dx * dx + dz * dz);
}
