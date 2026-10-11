#include "ov659.h"

const u8 *Func_0200004c(void)
{
    if (gPartyState.scene == (s32)&SceneId_Ov659Room2)
        return Data_02002828;
    if (gPartyState.entrance == 20)
        return Data_02002750;
    if (FlagChk(0x8ff))
        return Data_02002480;
    return Data_02002240;
}

void Func_020000a4(void)
{
    s32 msg;

    Func_02001d50();
    Func_02001eb8(0);
    msg = (s32)MsgScene659No06;
    Func_02001e18(msg);
    UiText_OpenMessageAtObject(9, 0);
    if (Inventory_PromptAndSetObjectMode(Func_02001ee8(), 0) == 0) {
        Battle_WaitMode0(10);
        Func_02001e18(msg + 1);
    } else {
        Battle_WaitMode0(20);
        Func_02001e18(msg + 2);
    }
    Func_02001e30(9, 0);
    Func_02001d58();
}
