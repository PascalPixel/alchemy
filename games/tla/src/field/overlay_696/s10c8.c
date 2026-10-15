#include "ov696.h"

void Func_020010c8(void)
{
    if (Func_02000f94()) {
        Func_02004a7c(8);
    } else {
        Func_020048c4();
        Func_02004a24(0);
        if (GameFlag_Test(0x9fe))
            Func_0200498c((s32)MsgShamanMoapaDefeatIncredible);
        else
            Func_0200498c((s32)MsgShamanTribeForbidsTalking);
        Func_020049a4(8, 0);
        Func_020048cc();
    }
}

void Func_02001118(void)
{
    Func_020048c4();
    Func_02004a24(0);
    Func_0200498c((s32)MsgShamanCheckOtherMountainSide);
    Func_020049ac(13, 0);
    Func_020048cc();
}

void Func_0200113c(void)
{
    Func_020048c4();
    Func_02004a24(0);
    if (Ov696_FlagCheck(0x9ed)) {
        Func_0200498c((s32)MsgShamanMoapaNeverForgetsVictor);
        Func_020049a4(11, 0);
    } else {
        Func_0200498c((s32)MsgShamanMoapaAsksVictorsNames);
        UiText_OpenMessageAtObject(11, 0);
        if (Inventory_PromptAndSetObjectMode(gPartyState.current_owner, 0) == 1) {
            Func_020049a4(11, 0);
        } else {
            ((struct Ov696EventWork *)Ram_HeapSlots->event_work)->field_1c4++;
            Func_020049a4(11, 0);
            Ov696_FlagSet(0x9ed);
        }
    }
    Func_020048cc();
}

void Func_020011c0(void)
{
    if (Func_02000f94()) {
        Func_02004a74(29, 12);
    } else {
        Func_020048c4();
        Func_02004a24(0);
        Func_0200498c((s32)MsgShamanWelcomeContigoHero);
        Func_020049a4(12, 0);
        Func_020048cc();
    }
}

void Func_020011f8(void)
{
    ObjectMotion_EnableActionAndResetMotion(17);
    WaitFrames(1);
    ObjectMotion_Launch(17, 4, 20);
    Engine_AudioPlayCue(229);
    WaitFrames(2);
    Object_SetActionCallbackAndRefreshById(16, Data_020050f8);
}

void Func_0200122c(void)
{
    Func_020048c4();
    Func_02004a24(0);
    if (GameFlag_Test(0x301)) {
        if (GameFlag_Test(0xa21))
            Func_0200498c((s32)MsgShamanFortuneAvoidNortheast);
        else
            Func_0200498c((s32)MsgShamanFortuneAvoidSouth);
        Func_020049a4(17, 0);
    } else {
        Engine_AudioPlayCue(0);
        if (GameFlag_Test(0xa21))
            Func_0200498c((s32)MsgShamanFortuneOfferAfterTrial);
        else
            Func_0200498c((s32)MsgShamanFortuneOfferBeforeTrial);
        UiText_OpenMessageAtObject(17, 0);
        if (Inventory_PromptAndSetObjectMode(gPartyState.current_owner, 0) == 1) {
            Func_020049a4(17, 0);
        } else {
            ((struct Ov696EventWork *)Ram_HeapSlots->event_work)->field_1c4++;
            Func_020049a4(17, 0);
            Engine_AudioPlayCue(11);
            Object_SetModeById(17, 5);
            Func_020049a4(17, 0);
            Func_020011f8();
            Object_SetModeById(17, 6);
            Func_020049a4(17, 0);
            Func_020011f8();
            Object_SetModeById(17, 5);
            Func_020049a4(17, 0);
            Func_020011f8();
            Battle_WaitMode0(20);
            Object_SetModeById(17, 1);
            Object_SetModeById(17, 3);
            Func_020049a4(17, 0);
            Ov696_ObjEmoteWait(17, 0x102);
            Battle_WaitMode0(40);
            Func_020049a4(17, 0);
            Motion_SetVarCbAndRefresh(17, 1);
            Func_020049a4(17, 0);
            GameFlag_SetBit(0x301);
        }
        Func_02004a2c();
    }
    Func_020048cc();
}

void Func_02001380(void)
{
    if (GameFlag_Test(0x200)) {
        Func_0200498c((s32)MsgShamanTrialRulesAreSimple);
    } else {
        Engine_AudioPlayCue(78);
        Func_020048c4();
        Func_02004a24(0);
        Ov696_CamSpeed(0x19999, 0x3333);
        Ov696_CamMove(0xe00000, -1, 0x1180000, 1);
        Ov696_ObjSpeed(gPartyState.current_owner, 0x13333, 0x9999);
        Ov696_ObjWalk(gPartyState.current_owner, 182, 280);
        Ov696_ObjArm(gPartyState.current_owner, 0, 0);
        Engine_AudioPlayCue(76);
        Ov696_ObjArm(15, 0x8000, 0);
        Ov696_ObjArm(16, 0x8000, 0);
        Ov696_ObjTurn(17, 0x8000);
        Motion_SetModeAndWaitAnimation(15, 3);
        Func_0200498c((s32)MsgShamanListenCarefully);
        Ov696_MessagePut(0x200f, 0);
        Ov696_ObjTurn(15, 0xc000);
        Func_020049e4(0x33333, 0x6666);
        Ov696_CamMove(0xe00000, -1, 0xb60000, 1);
        Func_020049f4();
        Ov696_MessagePut(0x200f, 0);
        Ov696_ObjEmoteWait(15, 0x102);
        Ov696_MessagePut(0x200f, 0);
        Ov696_CamMove(0xe00000, -1, 0x1180000, 1);
        Func_020049f4();
        Ov696_ObjTurn(15, 0x8000);
        Ov696_MessagePut(0x200f, 0);
        Ov696_CamMove(0xb20000, -1, 0xdc0000, 1);
        Func_020049f4();
        Battle_WaitMode0(10);
        Ov696_CamMove(0x10e0000, -1, 0xdc0000, 1);
        Func_020049f4();
        Battle_WaitMode0(20);
        Func_020049a4(15, 0);
        Ov696_CamMove(0xe20000, -1, 0x1180000, 1);
        Func_020049f4();
        Ov696_ObjTurn(15, 0x8000);
        Ov696_MessagePut(0x200f, 0);
    }
    Ov696_ObjTurn(15, 0xc000);
    Ov696_MessagePut(0x200f, 0);
    Motion_SetModeAndWaitAnimation(15, 3);
    Ov696_ObjArm(16, 0x2000, 0);
    Ov696_ObjTurn(17, 0x6000);
    Object_SetModeById(16, 3);
    Motion_SetModeAndWaitAnimation(17, 3);
    Ov696_ObjSpeed(16, 0x19999, 0xcccc);
    Ov696_ObjSpeed(17, 0x19999, 0xcccc);
    Ov696_ObjWalk2(16, 184, 256);
    Ov696_ObjWalk(17, 264, 256);
    Ov696_ObjArm(gPartyState.current_owner, 0xc000, 0);
    Ov696_ObjArm(15, 0xc000, 0);
    Func_020049e4(0x26666, 0x4ccc);
    Ov696_CamMove(0xe00000, -1, 0xb60000, 1);
    Ov696_ObjWalk2(16, 184, 128);
    Ov696_ObjWalk(17, 264, 128);
    Object_SetModeById(16, 1);
    WaitFrames(1);
    Func_02004934(16, 0, 0);
    Func_02004934(17, 0, 0);
    Event_FinishScreenTransition();
    Event_WaitTransitionFrames();
    GameFlag_SetBit(0x9e0);
    GameFlag_SetBit(0x11b);
    Func_02004a04(10);
}

void Func_02001644(void)
{
    Func_020048c4();
    Func_02004a24(0);
    Func_020049e4(0x33333, 0x6666);
    Ov696_CamMove(0xb60000, -1, 0x1300000, 1);
    Func_020049f4();
    Func_0200498c((s32)MsgShamanGiveUpTrialPrompt);
    UiText_OpenMessageAtObject(15, 0);
    Object_SetModeById(gPartyState.current_owner, 1);
    Ov696_ObjArm(gPartyState.current_owner, 0xe000, 0);
    if (Inventory_PromptAndSetObjectMode(gPartyState.current_owner, 0) == 0) {
        Func_020049a4(15, 0);
        Func_020049e4(0x4cccc, 0x9999);
        Ov696_CamMove(0xa80000, -1, 0x1700000, 1);
        Ov696_ObjSpeed(gPartyState.current_owner, 0x19999, 0xcccc);
        Ov696_ObjWalk(gPartyState.current_owner, 168, 368);
        GameFlag_ClearBit(0x201);
    } else {
        ((struct Ov696EventWork *)Ram_HeapSlots->event_work)->field_1c4++;
        Func_020049a4(15, 0);
        Func_020049e4(0x13333, 0x2666);
        Ov696_CamMove(0xa80000, -1, 0x1180000, 1);
        Ov696_ObjSpeed(gPartyState.current_owner, 0x19999, 0xcccc);
        Ov696_ObjWalk(gPartyState.current_owner, 168, 280);
    }
    Func_020048cc();
}

void Func_02001764(void)
{
    Func_020048c4();
    Func_02004a24(0);
    Func_020049e4(0x26666, 0x4ccc);
    Ov696_CamMove(0xe00000, -1, 0x1180000, 1);
    Ov696_ObjSpeed(gPartyState.current_owner, 0x1cccc, 0xe666);
    Ov696_ObjWalk(gPartyState.current_owner, 182, 280);
    Ov696_ObjArm(gPartyState.current_owner, 0, 0);
    Ov696_ObjArm(16, 0x8000, 0);
    Ov696_ObjArm(17, 0x8000, 0);
    Ov696_ObjTurn(15, 0x8000);
    Func_0200498c((s32)MsgShamanChallengeTrialRoadPrompt);
    Ov696_MessageOpen(0x200f, 0);
    if (Inventory_PromptAndSetObjectMode(gPartyState.current_owner, 0) == 1) {
        Ov696_MessagePut(0x200f, 0);
        Func_0200289c();
    } else {
        ((struct Ov696EventWork *)Ram_HeapSlots->event_work)->field_1c4++;
        Ov696_MessageOpen(0x200f, 0);
        if (Inventory_PromptAndSetObjectMode(gPartyState.current_owner, 0) == 0) {
            Engine_AudioPlayCue(76);
            Ov696_MessagePut(0x200f, 0);
            GameFlag_SetBit(0x200);
            Func_02001380();
        } else {
            ((struct Ov696EventWork *)Ram_HeapSlots->event_work)->field_1c4++;
            Func_0200290c();
        }
    }
    Func_020048cc();
}
