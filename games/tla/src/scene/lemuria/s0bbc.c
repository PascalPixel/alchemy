#include "types.h"

void Scheduler_AddOrUpdateCallback(void (*callback)(void), s32 priority);
void Func_02000b28(void);
void Func_02000ad8(void);

void Func_02000bbc(void)
{
    s32 priority;

    priority = 0x480;
    Scheduler_AddOrUpdateCallback(Func_02000b28, priority);
    priority = 0xc80;
    Scheduler_AddOrUpdateCallback(Func_02000ad8, priority);
}
