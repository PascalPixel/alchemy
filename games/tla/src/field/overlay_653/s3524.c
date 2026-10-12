#include "local.h"

void Func_02003524(void)
{
    Func_020051c0();
    Func_02005370(0);
    Object_GetById(gPartyState.current_owner)->motion_flags = 0;
    ((struct EventWork1ac *)Ram_HeapSlots->event_work)->value = 0x209;
    Event_FinishScreenTransition();
    Event_WaitTransitionFrames();
}

void Func_02003564(void)
{
    Func_02003524();
    Func_020052f8(3);
}

void Func_02003574(void)
{
    Func_02003524();
    Func_020052f8(4);
}
