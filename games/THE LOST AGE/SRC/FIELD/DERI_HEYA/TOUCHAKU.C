#include "TYPES.H"
#include "RAM_BUFFER.H"
#include "EVENTWRK.H"
#include "SCENE_CALL.H"

void Func_02000d9c(void);

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
