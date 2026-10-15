#include "madra_interiors.h"

const u8 *Func_0200004c(void)
{
    if (gPartyState.scene == (s32)&SceneId_Ov659Room2)
        return Data_02002828;
    if (gPartyState.entrance == 20)
        return Data_02002750;
    if (FlagCheck(0x8ff))
        return Data_02002480;
    return Data_02002240;
}

void Func_020000a4(void)
{
    s32 msg;

    Func_02001d50();
    Func_02001eb8(0);
    msg = (s32)MsgMadraInteriors06;
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

void Func_020000fc(void)
{
    u16 facing = Object_GetById(gPartyState.currentOwner)->angle;

    /* FAKEMATCH: the one-pass block keeps the quadrant offset after the facing load. */
    do {
    if ((u16)((facing + 0x2000) & ~0x3fff) == 0xc000) {
        Func_02001f08(1, 22);
    } else {
#ifdef TLA_EDITION_JA
        // 日本語版は選択肢なし
        if (FlagCheck(0x8ff))
            Func_02001e18((s32)MsgMadraInteriors155);
        else if (FlagCheck(0x8ab))
            Func_02001e18((s32)MsgMadraInteriors87);
        else
            Func_02001e18((s32)MsgMadraInteriors39);
        Func_02001e30(22, 0);
#else
        if (FlagCheck(0x8ff)) {
            s32 msg = (s32)MsgMadraInteriors155;

            Func_02001e18(msg);
            UiText_OpenMessageAtObject(22, 0);
            if (Inventory_PromptAndSetObjectMode(Func_02001ee8(), 0) == 0) {
                Battle_WaitMode0(10);
                Func_02001e18(msg + 1);
            } else {
                Battle_WaitMode0(20);
                Func_02001e18(msg + 2);
            }
            Func_02001e30(22, 0);
        } else {
            if (FlagCheck(0x8ab))
                Func_02001e18((s32)MsgMadraInteriors87);
            else
                Func_02001e18((s32)MsgMadraInteriors39);
            Func_02001e30(22, 0);
        }
#endif
    }
    } while (0);
}
