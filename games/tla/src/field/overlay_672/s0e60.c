#include "ov672.h"

void Func_02000e60(void)
{
    Func_02001da0();
    Func_02001ee8(0);
    Ov672_CamMove(0x1880000, -1, 0x780000, 1);
    Func_02001ea8();
    Battle_WaitMode0(20);
    Func_02001e48((s32)MsgIHaveFollowed);
    Ov672_ObjEmoteWait(10, 0x102);
    Func_02001e60(10, 0);
    Ov672_ObjTurn(10, 0x8000);
    Func_02001e60(10, 0);
    Ov672_ObjTurn(10, 0);
    Func_02001e60(10, 0);
    Ov672_ObjTurn(10, 0xd000);
    Motion_SetModeAndWaitAnimation(10, 4);
    Func_02001e60(10, 0);
    GameFlag_SetBit(0x8fe);
    Func_02001da8();
}
