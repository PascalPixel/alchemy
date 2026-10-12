#include "local.h"

void Func_02005028(void)
{
    Func_020051c0();
    Func_02005370(0);
    Event_StartScreenTransition();
    Event_WaitTransitionFrames();
    Battle_WaitMode0(20);
    Func_020053d8(121);
    Func_02005170(41, 0, 20, 0, 1, 1);
    Battle_WaitMode0(10);
    Func_02005170(41, 1, 20, 1, 1, 1);
    Battle_WaitMode0(10);
    Func_02005170(41, 2, 20, 2, 1, 1);
    Battle_WaitMode0(10);
    Func_02005170(41, 3, 20, 3, 1, 1);
    Battle_WaitMode0(10);
    GameFlag_SetBit(0x92d);
    Func_020051c8();
}
