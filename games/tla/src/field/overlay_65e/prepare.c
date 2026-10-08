#include "types.h"
#include "field_scene.h"
#include "object_runtime.h"
#include "party_state.h"
#include "ram_buffer.h"
#include "eventwrk.h"
#include "call.h"
#include "text/msg_ids.h"

void GameFlag_ClearBit();

s32 Scene_PrepareMap(void)
{
    return 0;
}

void Func_02000454(void)
{
    *(s32 *)((u8 *)((s32)Ram_HeapSlots->event_work + 0x1ac) + 0) = 0x209;
    Call1((void (*)())GameFlag_ClearBit, 0x12f);
    return;
}
