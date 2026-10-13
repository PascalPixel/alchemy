#include "local.h"

void Func_02000ed0(void)
{
    s32 priority;

    Func_02001fa8(1, 0, Func_02000e30);
    priority = 0xc80;
    Scheduler_AddOrUpdateCallback(Func_02000e90, priority);
}

void Func_02000ef0(void)
{
    Func_02002190(Data_0200226c);
}

void Func_02000f00(void)
{
    Object_GetById(gPartyState.current_owner)->priority_flags |= 2;
    Func_02002190(Data_0200226c);
}
