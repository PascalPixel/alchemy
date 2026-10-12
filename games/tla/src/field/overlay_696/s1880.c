#include "ov696.h"

extern const u8 Data_02004c5c[];
extern const u8 Data_02004cac[];

void Func_02001880(void)
{
    struct Ov696Object *owner;
    struct Ov696Object *shaman;
    struct Ov696Object *target;

    owner = Object_GetById(gPartyState.current_owner);
    shaman = Object_GetById(15);
    Func_020048c4();
    Func_02004a24(0);
    ObjectMotion_EnableActionAndResetMotion(16);
    ObjectMotion_EnableActionAndResetMotion(17);
    ObjectMotion_EnableActionAndResetMotion(15);
    Ov696_ObjArm(15, 0xa000, 0);
    Ov696_ObjTurn(16, 0x2000);
    Motion_SetModeAndWaitAnimation(16, 3);
    Ov696_ObjSpeed(16, 0x19999, 0xcccc);
    Object_SetModeById(16, 2);
    target = Object_GetById(15);
    if (target != NULL)
        ObjectMotion_ResetAndSetPosition(16, ((s16 *)target)[5], ((s16 *)target)[9]);
    ObjectMotion_CommitCurrentPositionAndActivate(16);
    Func_02004934(16, 0, 0);
    Object_SetModeById(15, 3);
    Func_0200498c((s32)MsgShamanGuardsTakeOtherPath);
    Func_020049a4(15, 0);
    ObjectMotion_SetAngleToward(gPartyState.current_owner, 15, 0);
    Ov696_ObjArm(17, 0x6000, 0);
    Ov696_ObjTurn(15, 0xe000);
    Motion_SetModeAndWaitAnimation(17, 3);
    Object_SetModeById(17, 2);
    target = Object_GetById(15);
    if (target != NULL)
        ObjectMotion_ResetAndSetPosition(17, ((s16 *)target)[5], ((s16 *)target)[9]);
    ObjectMotion_CommitCurrentPositionAndActivate(17);
    Func_02004934(17, 0, 0);
    Ov696_ObjTurn(15, 0xa000);
    Func_020049a4(15, 0);
    Ov696_CamMove(-1, -1, -1, 0);
    Ov696_CamSpeed(0x19999, 0x3333);
    Ov696_CamMove(0xe00000, -1, 0xc80000, 1);
    owner->unknown_63 = 0;
    shaman->unknown_63 = 0;
    ObjectMotion_EnableActionAndSetCallback(gPartyState.current_owner, Data_02004c5c);
    ObjectMotion_EnableActionAndSetCallback(15, Data_02004cac);
    do {
        WaitFrames(1);
    } while (owner->unknown_63 == 0 || shaman->unknown_63 == 0);
    /* FAKEMATCH: one more use of the shaman's flag ranks its address ahead of the owner's, so it takes r5 and the owner's r6 as the native has; plain, the owner's address, tested in both natural loops, is allocated first (differ 36). Measured against it: explicit pointers in either order, a for loop with break, nested do loops and a goto loop. */
    __asm__("" : : "m"(shaman->unknown_63));
    Func_02003040();
    Ov696_CamSpeed(0xcccc, 0x1999);
    Ov696_CamMove(0xe00000, -1, 0xa80000, 1);
    Ov696_ObjSpeed(gPartyState.current_owner, 0x19999, 0xcccc);
    Ov696_ObjSpeed(15, 0x19999, 0xcccc);
    ObjectMotion_ResetAndSetPositionInMode2(gPartyState.current_owner, 184, 128);
    Ov696_ObjWalk(15, 264, 128);
    Object_SetModeById(gPartyState.current_owner, 1);
    WaitFrames(1);
    Func_02004934(gPartyState.current_owner, 0, 0);
    Func_02004934(15, 0, 0);
    Event_FinishScreenTransition();
    Event_WaitTransitionFrames();
    Func_020048b4();
    Func_02004a04(11);
}

void Func_02001aa4(void)
{
    struct Ov696Object *owner;
    struct Ov696Object *shaman;
    struct Ov696Object *target;

    owner = Object_GetById(gPartyState.current_owner);
    shaman = Object_GetById(15);
    Func_020048c4();
    Func_02004a24(0);
    ObjectMotion_EnableActionAndResetMotion(16);
    ObjectMotion_EnableActionAndResetMotion(17);
    ObjectMotion_EnableActionAndResetMotion(15);
    Ov696_ObjArm(15, 0xe000, 0);
    Ov696_ObjTurn(17, 0x6000);
    Motion_SetModeAndWaitAnimation(17, 3);
    Ov696_ObjSpeed(17, 0x19999, 0xcccc);
    Object_SetModeById(17, 2);
    target = Object_GetById(15);
    if (target != NULL)
        ObjectMotion_ResetAndSetPosition(17, ((s16 *)target)[5], ((s16 *)target)[9]);
    ObjectMotion_CommitCurrentPositionAndActivate(17);
    Func_02004934(17, 0, 0);
    Object_SetModeById(15, 3);
    Func_0200498c((s32)MsgShamanGuardsTakeOtherPath);
    Func_020049a4(15, 0);
    ObjectMotion_SetAngleToward(gPartyState.current_owner, 15, 0);
    Ov696_ObjArm(16, 0x2000, 0);
    Ov696_ObjTurn(15, 0xa000);
    Motion_SetModeAndWaitAnimation(16, 3);
    Object_SetModeById(16, 2);
    target = Object_GetById(15);
    if (target != NULL)
        ObjectMotion_ResetAndSetPosition(16, ((s16 *)target)[5], ((s16 *)target)[9]);
    ObjectMotion_CommitCurrentPositionAndActivate(16);
    Func_02004934(16, 0, 0);
    Ov696_ObjTurn(15, 0xe000);
    Func_020049a4(15, 0);
    Ov696_CamMove(-1, -1, -1, 0);
    Ov696_CamSpeed(0x19999, 0x3333);
    Ov696_CamMove(0xe00000, -1, 0xc80000, 1);
    owner->unknown_63 = 0;
    shaman->unknown_63 = 0;
    ObjectMotion_EnableActionAndSetCallback(15, Data_02004c5c);
    ObjectMotion_EnableActionAndSetCallback(gPartyState.current_owner, Data_02004cac);
    do {
        WaitFrames(1);
    } while (owner->unknown_63 == 0 || shaman->unknown_63 == 0);
    /* FAKEMATCH: one more use of the shaman's flag ranks its address ahead of the owner's (r5, then r6), as in Func_02001880 above. */
    __asm__("" : : "m"(shaman->unknown_63));
    Func_02003040();
    Ov696_CamSpeed(0xcccc, 0x1999);
    Ov696_CamMove(0xe00000, -1, 0xa80000, 1);
    Ov696_ObjSpeed(gPartyState.current_owner, 0x19999, 0xcccc);
    Ov696_ObjSpeed(15, 0x19999, 0xcccc);
    ObjectMotion_ResetAndSetPositionInMode2(15, 184, 128);
    Ov696_ObjWalk(gPartyState.current_owner, 264, 128);
    Object_SetModeById(15, 1);
    WaitFrames(1);
    Func_02004934(15, 0, 0);
    Func_02004934(gPartyState.current_owner, 0, 0);
    Event_FinishScreenTransition();
    Event_WaitTransitionFrames();
    Func_020048b4();
    Func_02004a04(12);
}
