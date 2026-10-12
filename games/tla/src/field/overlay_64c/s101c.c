#include "types.h"
#include "iwram_call.h"

extern u8 Data_02003dc0[];
s32 Scheduler_AddOrUpdateCallback(void *callback, s32 priority);
void Func_02000d64(void);

void Func_0200101c(void)
{
    /* FAKEMATCH: the routine address is set before its arguments, the order the native schedules them in; written in the call, the low half of 0x200 moves one step */
    s32 (*clear)(void *, s32) = Iwram_ClearWords;

    clear(Data_02003dc0, 0x200);
    Scheduler_AddOrUpdateCallback(Func_02000d64, 0x480);
}
