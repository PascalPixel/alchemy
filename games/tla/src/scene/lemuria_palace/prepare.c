#include "types.h"
#include "field_scene.h"
#include "ram_buffer.h"
#include "object_runtime.h"
#include "party_state.h"
#include "call.h"

void                  Scheduler_AddOrUpdateCallback(void (*callback)(void), s32 priority);
s32                   GameFlag_Test(s32 flag);
struct ObjectRuntime *Object_GetById(s32 id);
void                  Func_020000f4(void);
void                  Func_02002024(s32 actor, s32 a, s32 b);

// The room's setup: it schedules the palette cycle Func_020000f4.
s32 Func_02000158(void)
{
    *(s32 *)((u8 *)Ram_HeapSlots->eventWork + 0x1ac) = 0x201;
    Value2((s32 (*)())Scheduler_AddOrUpdateCallback, (s32)Func_020000f4, 0xc80);
    ObjectMotion_SetActionVariant(8, 3);
    if (gPartyState.entrance == 6)
        Object_GetById(gPartyState.currentOwner)->priorityFlags |= 32;
    if (GameFlag_Test(0x96a))
    {
        Func_02002024(7, 0, 0);
        Func_02002024(15, 0, 0);
    }
    return 0;
}

s32 Scene_PrepareMap(void)
{
    return 0;
}
