#include "local.h"

void Func_02000cec(void)
{
    s32 n = --gData_02003460;

    Func_02002838_(50, (n & 1) * 5 + 59, 13, 5, 18, 15);
    if (gData_02003460 == 0)
        Scheduler_RemoveCallbackFar(Func_02000cec);
}
