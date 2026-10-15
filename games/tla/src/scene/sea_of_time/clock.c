#include "sea_of_time.h"

void Func_02000c50(void)
{
    u32 *state = gSceneState;

    if (gFrameCount - gData_020022c8 > 120)
        ((struct SeaOfTimeEventWork *)Ram_HeapSlots->eventWork)->alarm = 201;
    *state++ = gData_020022c8;
    *state++ = gData_020022cc;
    *state = gFrameCount;
}
