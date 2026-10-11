// near miss: differ 20 (listing 470 instructions, draft 470); the wait loop: the native loads Data_02007950's address inside the loop; loop.c hoists it in the draft
#include "../../../../games/tla/src/field/overlay_670/ov670.h"

void Func_02005c7c(void);
void Func_02002eb0(void);
void Func_020032b0(void);
void Func_020032c8(void);
void Func_020032e0(void);
void Func_02003610(void);
void ObjectMotion_ResetAndSetPositionInMode2(s32 actor, s32 x, s32 z);

void Func_02000d74(void)
{
    Func_020032c8();
    Func_02005c9c(0);
    Func_02005b64(17, 0);
    Ov670_ObjArm(7, 0, 0);
    Ov670_ObjArm(gPartyState.current_owner, 0, 0);
    Ov670_ObjArm(26, 0, 0);
    Ov670_ObjArm(5, 0, 0);
    Ov670_ObjArm(6, 0, 10);
    Func_02005ba4(0x4cccc, 0x9999);
    Ov670_CamMove(0x1080000, -1, 0x1840000, 1);
    Func_02005bb4();
    Battle_WaitMode0(40);
    Ov670_ObjArm(17, 0x3000, 20);
    Motion_SetModeAndWaitAnimation(17, 3);
    Func_02005b64(0x4011, 0);
    Func_02005c9c(26);
    ObjectDispatch_SetSingleChildFlags(Object_GetById(27), 0);
    Object_GetById(27)->motion_flags = 0;
    Ov670_ObjSetPos(Object_GetById(27), 0x1090000, 0x180000, 0x1580000);
    Func_02005ba4(0x6666, 0xccc);
    Ov670_CamMove(0x1080000, -1, 0x1540000, 1);
    Ov670_ObjSpeed(17, 0x3333, 0x1999);
    Ov670_ObjWalk(17, 0x108, 0x176);
    Ov670_TaskAdd(Func_02002eb0, 0x480);
    Battle_WaitMode0(20);
    Scheduler_RemoveCallbackFar(Func_02002ccc);
    Func_020032e0();
    OV670_TASK_ADD(Func_02002ccc, 0x480);
    Func_02005b64(7, 0);
    Func_02005b64(17, 0);
    Func_02005c9c(11);
    Func_02003610();
    Battle_WaitMode0(160);
    Func_02005b5c(17, 0, 80);
    Func_02005c9c(0x240);
    Func_02003640(1, 1);
    Battle_WaitMode0(20);
    Func_02005b5c(17, 0, 80);
    Func_02005c9c(161);
    Func_02003640(0, 1);
    Battle_WaitMode0(20);
    Ov670_ObjEmoteWait(17, 0x102);
    Battle_WaitMode0(20);
    Func_02005b5c(17, 0, 20);
    Scheduler_RemoveCallbackFar(Func_02002eb0);
    WaitFrames(1);
    Func_02005b0c(27, 0, 0);
    Ov670_ObjArm(17, 0xd000, 0);
    if (Data_02007950 != 0) {
        while (Data_02007950->y > 0x240000) {
            Ov670_ObjArm(17, 0xd000, 0);
            Data_02007950->y += 0xffffc000;
            Ov670_WaitFrames(1);
        }
    }
    Ov670_ObjTurn(17, 0x3000);
    Func_02005b64(17, 0);
    Func_02005ba4(0x4cccc, 0x9999);
    Ov670_CamMove(0x1080000, 0x100000, 0x1a40000, 1);
    Ov670_ObjSpeed(17, 0x19999, 0xcccc);
    Ov670_ObjWalk(17, 0x108, 0x188);
    Battle_WaitMode0(20);
    Func_02005b64(0x8011, 0);
    Func_02005c9c(148);
    Func_02005c9c(11);
    Func_020032b0();
    Battle_WaitMode0(80);
    Ov670_CamMove(0x580000, 0x200000, 0x1880000, 1);
    Func_02005bb4();
    Battle_WaitMode0(40);
    Func_02005b64(7, 0);
    Ov670_ObjEmoteWait(7, 0x102);
    Func_02005b64(7, 0);
    Motion_SetModeAndWaitAnimation(7, 4);
    Func_02005b64(7, 0);
    ObjectMotion_SetActionVariant(7, 2);
    Ov670_ObjSpeed(7, 0x19999, 0xcccc);
    Ov670_ObjWalk(7, 152, 0x182);
    Ov670_ObjArm(7, 0xe000, 40);
    Ov670_ObjArm(7, 0x2000, 20);
    Ov670_ObjArm(7, 0xe000, 20);
    Ov670_ObjArm(7, 0x2000, 40);
    Func_02005b64(7, 0);
    Ov670_ObjWalk(7, 124, 0x164);
    Ov670_ObjArm(7, 0xe000, 20);
    Ov670_ObjArm(7, 0xa000, 20);
    Ov670_ObjArm(7, 0xe000, 20);
    Func_02005b64(0x2007, 0);
    Ov670_ObjArm(5, 0x2000, 0);
    Func_02005b64(5, 0);
    Ov670_ObjArm(gPartyState.current_owner, 0xe000, 40);
    Ov670_ObjSpeed(26, 0xcccc, 0x6666);
    ObjectMotion_ResetAndSetPositionInMode2(26, 104, 0x170);
    Battle_WaitMode0(20);
    Func_02005ba4(0xcccc, 0x1999);
    Ov670_CamMove(0x6f0000, -1, 0x1640000, 1);
    ObjectMotion_CommitCurrentPositionAndActivate(26);
    Object_SetModeById(26, 1);
    Ov670_ObjArm(26, 0xd000, 40);
    Func_02005b94(26, 0x100, 20);
    Func_02005b64(26, 0);
    Ov670_ObjArm(gPartyState.current_owner, 0xc000, 0);
    Ov670_ObjArm(5, 0xc000, 0);
    Ov670_ObjArm(6, 0xc000, 0);
    Ov670_ObjArm(26, 0x5000, 20);
    Ov670_MesOpen(0x601a, 0);
    if (Inventory_PromptAndSetObjectMode(gPartyState.current_owner, 0) == 0) {
        Ov670_MesPut(0x601a, 0);
        ((struct Ov670EventWork *)Ram_HeapSlots->event_work)->field_1c4++;
    } else {
        ((struct Ov670EventWork *)Ram_HeapSlots->event_work)->field_1c4++;
        Ov670_MesPut(0x601a, 0);
        Func_02005c7c();
    }
}
