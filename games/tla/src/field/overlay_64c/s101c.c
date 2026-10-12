#include "types.h"
#include "iwram_call.h"

extern u8 Data_02003dc0[];
void Scheduler_AddOrUpdateCallback(void *callback, s32 priority);
void Func_02000d64(void);

void Func_0200101c(void)
{
    /* FAKEMATCH: the routine address and the priority are set before their arguments, the order the native schedules them in; written in the calls, each low half of a split constant moves one step */
    s32 (*clear)(void *, s32) = Iwram_ClearWords;
    s32 priority;

    clear(Data_02003dc0, 0x200);
    priority = 0x480;
    Scheduler_AddOrUpdateCallback(Func_02000d64, priority);
}
