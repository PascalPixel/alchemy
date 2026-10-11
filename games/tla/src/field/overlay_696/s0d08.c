#include "ov696.h"

void Func_02000d08(void)
{
    Func_020048c4();
    Func_02004a24(0);
    UiText_ShowPositionedMessageAndWait((s32)MsgShamanEntranceLocked, 1);
    Func_020048cc();
}

void Func_02000d28(void)
{
    Func_020048c4();
    Func_02004a24(0);
    UiText_ShowPositionedMessageAndWait((s32)MsgShamanEntranceClosed, 1);
    Func_020048cc();
}

void Func_02000d48(void)
{
    Func_020048c4();
    Func_02004a24(0);
    UiText_ShowPositionedMessageAndWait((s32)MsgShamanTrialRoadPermissionSign, 1);
    Func_020048cc();
}

void Func_02000d68(void)
{
    Func_020048c4();
    Func_02004a24(0);
    Func_0200498c((s32)MsgShamanGuardRepeatRulesBeforeTrial);
    UiText_OpenMessageAtObject(17, 0);
    if (Inventory_PromptAndSetObjectMode(gPartyState.current_owner, 0) == 0) {
        Motion_SetModeAndWaitAnimation(15, 3);
        Ov696_CamSpeed(0x19999, 0x3333);
        Ov696_CamMove(0xe00000, -1, 0x1180000, 1);
        Ov696_ObjSpeed(gPartyState.current_owner, 0x19999, 0xcccc);
        Ov696_ObjWalk(gPartyState.current_owner, 246, 292);
        Ov696_ObjWalk(gPartyState.current_owner, 204, 292);
        Ov696_ObjWalk(gPartyState.current_owner, 182, 280);
        Ov696_ObjTurn(gPartyState.current_owner, 0);
        Ov696_MesPut(0x200f, 0);
        GameFlag_SetBit(0x200);
        Func_02001380();
    } else {
        ((struct Ov696EventWork *)Ram_HeapSlots->event_work)->field_1c4++;
        Func_020049a4(17, 0);
    }
    Func_020048cc();
}

void Func_02000e3c(void)
{
    Func_020048c4();
    Func_02004a24(0);
    Func_0200498c((s32)MsgShamanOtherGuardRepeatRulesBeforeTrial);
    UiText_OpenMessageAtObject(16, 0);
    if (Inventory_PromptAndSetObjectMode(gPartyState.current_owner, 0) == 0) {
        Motion_SetModeAndWaitAnimation(15, 3);
        Ov696_CamSpeed(0x19999, 0x3333);
        Ov696_CamMove(0xe00000, -1, 0x1180000, 1);
        Ov696_ObjSpeed(gPartyState.current_owner, 0x19999, 0xcccc);
        Ov696_ObjWalk(gPartyState.current_owner, 182, 280);
        Ov696_ObjTurn(gPartyState.current_owner, 0);
        Ov696_MesPut(0x200f, 0);
        GameFlag_SetBit(0x200);
        Func_02001380();
    } else {
        ((struct Ov696EventWork *)Ram_HeapSlots->event_work)->field_1c4++;
        Func_020049a4(16, 0);
    }
    Func_020048cc();
}

void Func_02000ef8(void)
{
    struct Ov696EventWork *work = Ram_HeapSlots->event_work;

    Func_020048c4();
    Func_02004a24(0);
    Func_0200498c((s32)MsgShamanOutsidersLeaveTown);
    Func_020049a4(15, 0);
    work->field_15a = 1;
    Func_020048cc();
}

void Func_02000f2c(void)
{
    struct Ov696EventWork *work = Ram_HeapSlots->event_work;

    Func_020048c4();
    Func_02004a24(0);
    Func_0200498c((s32)MsgShamanFearMoapaPower);
    Func_020049a4(17, 0);
    work->field_15a = 1;
    Func_020048cc();
}

void Func_02000f60(void)
{
    struct Ov696EventWork *work = Ram_HeapSlots->event_work;

    Func_020048c4();
    Func_02004a24(0);
    Func_0200498c((s32)MsgShamanSpeakToMoapa);
    Func_020049a4(16, 0);
    work->field_15a = 1;
    Func_020048cc();
}

s32 Func_02000f94(void)
{
    u16 turn;
    s32 facing;

    turn = Object_GetById(gPartyState.current_owner)->facing + 0x5fff;
    facing = 1;
    if (turn >= 0x3fff)
        facing = 0;
    return facing;
}
