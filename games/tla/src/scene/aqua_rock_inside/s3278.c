#include "local.h"

void Func_02003278(void)
{
    void *buffer;
    s32   priority;

    Iwram_ClearWords(&gData_0200536c, 0x604);
    buffer = MemAlloc_(0xc80);
    Func_02003c7c(gData_0200414c, buffer);
    gData_0200536c.vramEntry = Resource_FindFreeEntry();
    /* FAKEMATCH: the native stores the entry before it starts the size; without the barrier the size's first half comes first (differ 2). */
    __asm__("");
    VramBlock_LoadCached_(gData_0200536c.vramEntry, 0xc80, buffer);
    MemFree(buffer);
    priority = 0x41c;
    Scheduler_AddOrUpdateCallback(Func_020030f4, priority);
}
