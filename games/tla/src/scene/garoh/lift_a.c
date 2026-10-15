#include "garoh.h"

extern u8 gData_02005450[];
void Func_02000a68(void);

void Func_02000e00(void)
{
    Scheduler_AddOrUpdateCallback(Func_02000a68, 1152);
    if (Flag_IsSet(0x310)) {
        if (Flag_IsSet(0x318))
            Func_020052cc(0x10003, 1);
        else
            Func_020052cc(0x10007, 1);
    }
    Func_0200536c(gData_02005450);
    Func_020052d4(1);
    Event_WaitTransitionFrames();
}

void Func_02000e58(void)
{
    Func_02005374(gData_02005450);
}

void Func_02000e68(void)
{
    Func_02005374(gData_02005450);
}

