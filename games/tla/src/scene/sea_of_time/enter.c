#include "sea_of_time.h"

void Func_02001068(void)
{
    Func_02001fac();
    Func_02002044(0);
    if (!SeaOfTime_FlagCheck(0x8dd)) {
        Func_02002004(4);
        SeaOfTime_FlagSet(0x8dd);
    } else {
        Func_02002004(1);
    }
    Event_FinishScreenTransition();
    Event_WaitTransitionFrames();
    Func_02001fb4();
}
