#include "types.h"
#include "io_reg.h"
#include "mem.h"
#include "iwram_call.h"
#include "callback_scheduler.h"

void Func_08127038(void);
void Func_08126e00(void);

void Func_081272b0(void)
{
    Iwram_ClearWords((void *)0x06004000, 0x4000);
    Runtime_ReleaseHeapBlock(188);
    Runtime_ReleaseHeapBlock(104);
    Runtime_ReleaseHeapBlock(96);
    Runtime_ReleaseHeapBlock(92);
    REG_DISPCNT = 0x1341;
    Scheduler_RemoveCallback(Func_08127038);
    Scheduler_RemoveCallback(Func_08126e00);
}

void Func_08127304(void)
{
}
