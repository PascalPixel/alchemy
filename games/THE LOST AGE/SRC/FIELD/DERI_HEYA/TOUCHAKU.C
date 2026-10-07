#include "TYPES.H"
#include "RAM_BUFFER.H"
#include "EVENTWRK.H"
#include "SCENE_CALL.H"
#include "PARTY_STATE.H"
#include "SCENE_IDS.H"
#include "DERI.H"

void Func_02000d9c(void);
void Func_02000ca4(s32 mode);

s32 Scene_PrepareMap(void)
{
    if (gPartyState.scene == (s32)&SceneId_DeriHeya2 && GameFlag_Test(0x873)) {
        Func_02000ca4(0);
        Deri_CopyCells(29, 33, 10, 6, 10, 12);
    }
    return 0;
}

void Func_02000c58(void)
{
    struct EventWork *work = Ram_HeapSlots->event_work;

    work->screen_effect = 0x209;
    GameFlag_ClearBit(0x12f);
}

void Func_02000c74(void)
{
    Func_02000d9c();
}
