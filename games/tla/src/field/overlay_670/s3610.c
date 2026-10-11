#include "ov670.h"

void Func_02003610(void)
{
    Scheduler_RemoveCallbackFar(Func_02002ccc);
    WaitFrames(1);
    Func_02005bb4();
    Battle_WaitMode0(40);
    if (Data_02007950 != 0)
        Func_02003504();
}
