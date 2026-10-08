#include "types.h"

extern u8 gPartyState[];
void Scheduler_AddOrUpdateCallback(void (*callback)(void), s32 priority);
void Func_080deba4(void);

void Func_080dec8c(void)
{
    if ((s8)gPartyState[0x26d] != 0)
        Scheduler_AddOrUpdateCallback(Func_080deba4, 0x480);
}
