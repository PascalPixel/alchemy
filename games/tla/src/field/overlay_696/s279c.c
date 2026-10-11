#include "ov696.h"

void Func_0200279c(void)
{
    Func_020048c4();
    Func_02004a24(0);
    Func_0200498c((s32)MsgShamanShowMeSomething);
    Func_020049a4(15, 0);
    Func_020048cc();
}

void Func_020027c0(void)
{
    Func_020048c4();
    Func_02004a24(0);
    Func_0200498c((s32)MsgShamanCannotBuyGift);
    Func_020049a4(16, 0);
    Func_020048cc();
}

void Func_020027e4(void)
{
    Func_020048c4();
    Func_02004a24(0);
    Func_0200498c((s32)MsgShamanShowItToMoapa);
    Func_020049a4(17, 0);
    Func_020048cc();
}

void Func_02002808(void)
{
    if (GameFlag_Test(0x9fe)) {
        Engine_AudioPlayCue(0);
        Func_020048c4();
        Func_02004a24(0);
        Func_0200498c((s32)MsgShamanFortuneGreeting);
        Object_SetModeById(17, 5);
        Func_020049a4(17, 0);
        Ov696_ObjArm(gPartyState.current_owner, 0xc000, 20);
        Func_020011f8();
        Object_SetModeById(17, 1);
        GameFlag_SetBit(0x203);
        Func_020048cc();
    }
}

void Func_02002878(void)
{
    Ov696_BlendStart(0, 0, 1, 2, 2, 17);
    GameFlag_SetBit(0x205);
}

void Func_0200289c(void)
{
    Ov696_ObjSpeed(gPartyState.current_owner, 0x19999, 0xcccc);
    Object_AttachWorkTargetToObject(gPartyState.current_owner, 1);
    Ov696_ObjWalk(gPartyState.current_owner, 168, 304);
    Ov696_ObjArm(16, 0x4000, 0);
    Ov696_ObjArm(17, 0x4000, 0);
    Ov696_ObjArm(15, 0x4000, 0);
    Ov696_ObjWalk(gPartyState.current_owner, 168, 368);
    GameFlag_ClearBit(0x201);
}
