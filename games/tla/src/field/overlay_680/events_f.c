#include "types.h"

s32 Scheduler_AddOrUpdateCallback(void (*callback)(void), s32 priority);
void Func_02000ab4(void);

void Func_02000dd0(void)
{
    Scheduler_AddOrUpdateCallback(Func_02000ab4, 0x480);
}
