#include "types.h"
#include "field_scene.h"
#include "ram_buffer.h"
#include "event_work.h"

void GameFlag_ClearBit(s32 flag);

s32 Scene_PrepareMap(void)
{
    return 0;
}

void Func_0200014c(void)
{
    struct EventWork *work = Ram_HeapSlots->event_work;

    work->screen_effect = 0x209;
    GameFlag_ClearBit(0x12f);
}
