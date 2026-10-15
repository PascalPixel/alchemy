#include "ov687.h"

void Func_02001068(void)
{
    Func_02001fac();
    Func_02002044(0);
    if (!Ov687_FlagCheck(0x8dd)) {
        Func_02002004(4);
        Ov687_FlagSet(0x8dd);
    } else {
        Func_02002004(1);
    }
    Event_FinishScreenTransition();
    Event_WaitTransitionFrames();
    Func_02001fb4();
}
