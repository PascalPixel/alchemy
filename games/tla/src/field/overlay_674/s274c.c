#include "ov674.h"

void Func_0200274c(void)
{
    struct Ov674EventWork *work = Ram_HeapSlots->event_work;

    Func_02003e64();
    Func_02003fa4(0);
    Func_020034ec();
    Func_02003ffc(123);
    Event_FinishScreenTransition();
    Event_WaitTransitionFrames();
    Func_02003f7c(work->exit);
}
