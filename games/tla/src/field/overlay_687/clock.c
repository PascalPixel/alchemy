#include "ov687.h"

void Func_02000c50(void)
{
    u32 *state = gSceneState;

    if (gFrameCount - Data_020022c8 > 120)
        ((struct Ov687EventWork *)Ram_HeapSlots->event_work)->alarm = 201;
    *state++ = Data_020022c8;
    *state++ = Data_020022cc;
    *state = gFrameCount;
}
