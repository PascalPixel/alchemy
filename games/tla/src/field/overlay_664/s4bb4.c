#include "ov664.h"

extern u8 MsgCheckedThePsynergy[];

void Func_02004bb4(void)
{
    const u8 *message;

    Func_02005e14();
    Func_02005f2c(0);
    message = MsgCheckedThePsynergy;
    UiText_ShowPositionedMessageAndWait(message, 1);
    Func_02005f94(126);
    Func_02005f34(999, 0);
    Battle_WaitMode0(10);
    GameFlag_ClearBit(322);
    UiText_ShowPositionedMessageAndWait(message + 1, 1);
    Func_02005e1c();
}
