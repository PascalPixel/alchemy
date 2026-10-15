#include "shaman_village.h"

void Func_020046bc(void);

void Func_020046ec(void)
{
    struct ShamanVillageEventWork *work = Ram_HeapSlots->eventWork;

    work->screenEffect = 521;
    if (GameFlag_Test(0x9fe))
    {
        Scheduler_AddOrUpdateCallback(Func_020046bc, 0x480);
    }
    else
    {
        Func_02004934(9, 0, 0);
        Func_02004934(10, 0, 0);
    }
}
