#include "shaman_village.h"

void Func_020010c8(void)
{
    if (Func_02000f94())
    {
        Func_02004a7c(8);
    }
    else
    {
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
    if (ShamanVillage_FlagCheck(0x9ed))
    {
        Func_0200498c((s32)MsgShamanMoapaNeverForgetsVictor);
        Func_020049a4(11, 0);
    }
    else
    {
        Func_0200498c((s32)MsgShamanMoapaAsksVictorsNames);
        UiText_OpenMessageAtObject(11, 0);
        if (Inventory_PromptAndSetObjectMode(gPartyState.currentOwner, 0) == 1)
        {
            Func_020049a4(11, 0);
        }
        else
        {
            ((struct ShamanVillageEventWork *)Ram_HeapSlots->eventWork)->field1c4++;
            Func_020049a4(11, 0);
            ShamanVillage_FlagSet(0x9ed);
        }
    }
    Func_020048cc();
}

void Func_020011c0(void)
{
    if (Func_02000f94())
    {
        Func_02004a74(29, 12);
    }
    else
    {
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
    Object_SetActionCallbackAndRefreshById(16, gData_020050f8);
}

void Func_0200122c(void)
{
    Func_020048c4();
    Func_02004a24(0);
    if (GameFlag_Test(0x301))
    {
        if (GameFlag_Test(0xa21))
            Func_0200498c((s32)MsgShamanFortuneAvoidNortheast);
        else
            Func_0200498c((s32)MsgShamanFortuneAvoidSouth);
        Func_020049a4(17, 0);
    }
    else
    {
        Engine_AudioPlayCue(0);
        if (GameFlag_Test(0xa21))
            Func_0200498c((s32)MsgShamanFortuneOfferAfterTrial);
        else
            Func_0200498c((s32)MsgShamanFortuneOfferBeforeTrial);
        UiText_OpenMessageAtObject(17, 0);
        if (Inventory_PromptAndSetObjectMode(gPartyState.currentOwner, 0) == 1)
        {
            Func_020049a4(17, 0);
        }
        else
        {
            ((struct ShamanVillageEventWork *)Ram_HeapSlots->eventWork)->field1c4++;
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
            ShamanVillage_ObjEmoteWait(17, 0x102);
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
    if (GameFlag_Test(0x200))
    {
        Func_0200498c((s32)MsgShamanTrialRulesAreSimple);
    }
    else
    {
        Engine_AudioPlayCue(78);
        Func_020048c4();
        Func_02004a24(0);
        ShamanVillage_CamSpeed(0x19999, 0x3333);
        ShamanVillage_CamMove(0xe00000, -1, 0x1180000, 1);
        ShamanVillage_ObjSpeed(gPartyState.currentOwner, 0x13333, 0x9999);
        ShamanVillage_ObjWalk(gPartyState.currentOwner, 182, 280);
        ShamanVillage_ObjArm(gPartyState.currentOwner, 0, 0);
        Engine_AudioPlayCue(76);
        ShamanVillage_ObjArm(15, 0x8000, 0);
        ShamanVillage_ObjArm(16, 0x8000, 0);
        ShamanVillage_ObjTurn(17, 0x8000);
        Motion_SetModeAndWaitAnimation(15, 3);
        Func_0200498c((s32)MsgShamanListenCarefully);
        ShamanVillage_MessagePut(0x200f, 0);
        ShamanVillage_ObjTurn(15, 0xc000);
        Func_020049e4(0x33333, 0x6666);
        ShamanVillage_CamMove(0xe00000, -1, 0xb60000, 1);
        Func_020049f4();
        ShamanVillage_MessagePut(0x200f, 0);
        ShamanVillage_ObjEmoteWait(15, 0x102);
        ShamanVillage_MessagePut(0x200f, 0);
        ShamanVillage_CamMove(0xe00000, -1, 0x1180000, 1);
        Func_020049f4();
        ShamanVillage_ObjTurn(15, 0x8000);
        ShamanVillage_MessagePut(0x200f, 0);
        ShamanVillage_CamMove(0xb20000, -1, 0xdc0000, 1);
        Func_020049f4();
        Battle_WaitMode0(10);
        ShamanVillage_CamMove(0x10e0000, -1, 0xdc0000, 1);
        Func_020049f4();
        Battle_WaitMode0(20);
        Func_020049a4(15, 0);
        ShamanVillage_CamMove(0xe20000, -1, 0x1180000, 1);
        Func_020049f4();
        ShamanVillage_ObjTurn(15, 0x8000);
        ShamanVillage_MessagePut(0x200f, 0);
    }
    ShamanVillage_ObjTurn(15, 0xc000);
    ShamanVillage_MessagePut(0x200f, 0);
    Motion_SetModeAndWaitAnimation(15, 3);
    ShamanVillage_ObjArm(16, 0x2000, 0);
    ShamanVillage_ObjTurn(17, 0x6000);
    Object_SetModeById(16, 3);
    Motion_SetModeAndWaitAnimation(17, 3);
    ShamanVillage_ObjSpeed(16, 0x19999, 0xcccc);
    ShamanVillage_ObjSpeed(17, 0x19999, 0xcccc);
    ShamanVillage_ObjWalk2(16, 184, 256);
    ShamanVillage_ObjWalk(17, 264, 256);
    ShamanVillage_ObjArm(gPartyState.currentOwner, 0xc000, 0);
    ShamanVillage_ObjArm(15, 0xc000, 0);
    Func_020049e4(0x26666, 0x4ccc);
    ShamanVillage_CamMove(0xe00000, -1, 0xb60000, 1);
    ShamanVillage_ObjWalk2(16, 184, 128);
    ShamanVillage_ObjWalk(17, 264, 128);
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
    ShamanVillage_CamMove(0xb60000, -1, 0x1300000, 1);
    Func_020049f4();
    Func_0200498c((s32)MsgShamanGiveUpTrialPrompt);
    UiText_OpenMessageAtObject(15, 0);
    Object_SetModeById(gPartyState.currentOwner, 1);
    ShamanVillage_ObjArm(gPartyState.currentOwner, 0xe000, 0);
    if (Inventory_PromptAndSetObjectMode(gPartyState.currentOwner, 0) == 0)
    {
        Func_020049a4(15, 0);
        Func_020049e4(0x4cccc, 0x9999);
        ShamanVillage_CamMove(0xa80000, -1, 0x1700000, 1);
        ShamanVillage_ObjSpeed(gPartyState.currentOwner, 0x19999, 0xcccc);
        ShamanVillage_ObjWalk(gPartyState.currentOwner, 168, 368);
        GameFlag_ClearBit(0x201);
    }
    else
    {
        ((struct ShamanVillageEventWork *)Ram_HeapSlots->eventWork)->field1c4++;
        Func_020049a4(15, 0);
        Func_020049e4(0x13333, 0x2666);
        ShamanVillage_CamMove(0xa80000, -1, 0x1180000, 1);
        ShamanVillage_ObjSpeed(gPartyState.currentOwner, 0x19999, 0xcccc);
        ShamanVillage_ObjWalk(gPartyState.currentOwner, 168, 280);
    }
    Func_020048cc();
}

void Func_02001764(void)
{
    Func_020048c4();
    Func_02004a24(0);
    Func_020049e4(0x26666, 0x4ccc);
    ShamanVillage_CamMove(0xe00000, -1, 0x1180000, 1);
    ShamanVillage_ObjSpeed(gPartyState.currentOwner, 0x1cccc, 0xe666);
    ShamanVillage_ObjWalk(gPartyState.currentOwner, 182, 280);
    ShamanVillage_ObjArm(gPartyState.currentOwner, 0, 0);
    ShamanVillage_ObjArm(16, 0x8000, 0);
    ShamanVillage_ObjArm(17, 0x8000, 0);
    ShamanVillage_ObjTurn(15, 0x8000);
    Func_0200498c((s32)MsgShamanChallengeTrialRoadPrompt);
    ShamanVillage_MessageOpen(0x200f, 0);
    if (Inventory_PromptAndSetObjectMode(gPartyState.currentOwner, 0) == 1)
    {
        ShamanVillage_MessagePut(0x200f, 0);
        Func_0200289c();
    }
    else
    {
        ((struct ShamanVillageEventWork *)Ram_HeapSlots->eventWork)->field1c4++;
        ShamanVillage_MessageOpen(0x200f, 0);
        if (Inventory_PromptAndSetObjectMode(gPartyState.currentOwner, 0) == 0)
        {
            Engine_AudioPlayCue(76);
            ShamanVillage_MessagePut(0x200f, 0);
            GameFlag_SetBit(0x200);
            Func_02001380();
        }
        else
        {
            ((struct ShamanVillageEventWork *)Ram_HeapSlots->eventWork)->field1c4++;
            Func_0200290c();
        }
    }
    Func_020048cc();
}
