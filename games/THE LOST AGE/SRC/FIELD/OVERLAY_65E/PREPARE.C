#include "TYPES.H"
#include "FIELD_SCENE.H"
#include "OBJECT_RUNTIME.H"
#include "PARTY_STATE.H"
#include "RAM_BUFFER.H"
#include "EVENTWRK.H"
#include "CALL.H"
#include "text/MSG_IDS.H"

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
