#include "local.h"

void Func_02003628(void)
{
    void *buffer;
    s32 priority;

    /* FAKEMATCH: the native builds the priority before loading the callback. As a plain constant the load comes before the priority's last add (differ 2); the same barrier after the assignment, a one-pass block around it or an empty barrier before the call fix that and move the size's shift above the buffer copy (differ 2). Naming the priority here, before it is set, is the one place measured that keeps both. */
    __asm__ volatile("" : "+r"(priority));
    Iwram_ClearWords(&Data_02005970, 0x3a4);
    buffer = MemAlloc_(0x400);
    Func_02003c7c(Data_02004351, buffer);
    Data_02005970.vram_entry = Resource_FindFreeEntry();
    /* FAKEMATCH: the native stores the entry before it starts the size; without the barrier the size's first half comes first (differ 2). */
    __asm__("");
    VramBlock_LoadCached_(Data_02005970.vram_entry, 0x400, buffer);
    MemFree(buffer);
    priority = 0x41c;
    Scheduler_AddOrUpdateCallback(Func_020034fc, priority);
    Func_02003ef4(220);
}
