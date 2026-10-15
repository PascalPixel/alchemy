#include "types.h"
#include "object_runtime.h"
#include "party_state.h"
#include "call.h"

extern u8 MsgHereTheIntruder[];

struct ObjectRuntime *Object_GetById(s32 id);
void Battle_WaitMode0(s32 frames);
void GameFlag_SetBit(s32 flag);
void Event_StartScreenTransition(void);
void Event_WaitTransitionFrames(void);
void Motion_SetModeAndWaitAnimation(s32 actor, s32 mode);
void Object_LinkObjectAndSetCallback(s32 actor, s32 target);
void ObjectMotion_SetAngleToward(s32 actor, s32 target, s32 frames);
void ObjectMotion_SetPositionAndReset(s32 actor, s32 x, s32 z);
void ObjectMotion_ResetAndSetPositionInMode2(s32 actor, s32 x, s32 z);
void Func_02000ad4(s32 actor);
void Func_02001084(void);
void Func_0200108c(void);
void Func_020010d4(s32 actor, s32 x, s32 z);
void Func_02001104(const u8 *message);
void Func_0200110c(s32 actor, s32 flags);
void Func_0200111c(s32 actor, s32 a, s32 b);
void Func_02001164(s32 value);

void Func_02000af0(void)
{
    s32 owner = gPartyState.currentOwner;

    /* FAKEMATCH: the placements go through call.h's Call3 and Func_0200111c through Value3, as in Func_02000960, which build each call's constants again and set its arguments up in the native's order (all direct: differ 126; the placements direct: 64; Func_0200111c direct: 12). */
    Func_02001084();
    Func_02001164(0);
    Call3((void (*)())Func_020010d4, 23, 0xf80000, 0x1d80000);
    Call3((void (*)())Func_020010d4, 24, 0x1080000, 0x1d80000);
    Call3((void (*)())Func_020010d4, 25, 0xeb0000, 0x1e30000);
    Call3((void (*)())Func_020010d4, 26, 0xe80000, 0x1f00000);
    Call3((void (*)())Func_020010d4, 27, 0x1150000, 0x1e30000);
    Call3((void (*)())Func_020010d4, 28, 0x1180000, 0x1f00000);
    Func_02000ad4(owner);
    Func_02000ad4(23);
    Func_02000ad4(24);
    Func_02000ad4(25);
    Func_02000ad4(26);
    Func_02000ad4(27);
    Func_02000ad4(28);
    Event_StartScreenTransition();
    Event_WaitTransitionFrames();
    Func_02001104(MsgHereTheIntruder);
    Value3((s32 (*)())Func_0200111c, 27, 259, 30);
    Func_0200110c(27, 0);
    ObjectMotion_SetAngleToward(27, 25, 0);
    Battle_WaitMode0(10);
    Motion_SetModeAndWaitAnimation(25, 3);
    ObjectMotion_SetAngleToward(27, owner, 0);
    Battle_WaitMode0(10);
    Value3((s32 (*)())Func_0200111c, 27, 263, 30);
    Func_0200110c(27, 0);
    Motion_SetModeAndWaitAnimation(27, 3);
    Func_0200110c(27, 0);
    ObjectMotion_SetAngleToward(25, 23, 0);
    Value3((s32 (*)())Func_0200111c, 25, 256, 30);
    Func_0200110c(25, 0);
    Call3((void (*)())ObjectMotion_ResetAndSetPositionInMode2, 23, 248, 536);
    Battle_WaitMode0(1);
    Call3((void (*)())ObjectMotion_ResetAndSetPositionInMode2, 24, 264, 536);
    Object_LinkObjectAndSetCallback(25, owner);
    Object_LinkObjectAndSetCallback(26, owner);
    Object_LinkObjectAndSetCallback(27, owner);
    Object_LinkObjectAndSetCallback(28, owner);
    Battle_WaitMode0(2);
    Object_GetById(owner)->actionFlags &= ~1;
    Call3((void (*)())ObjectMotion_SetPositionAndReset, owner, 256, 552);
    Battle_WaitMode0(1);
    Object_GetById(owner)->actionFlags |= 1;
    Object_SetModeById(23, 1);
    Object_SetModeById(24, 1);
    Battle_WaitMode0(20);
    Object_SetModeById(23, 4);
    Battle_WaitMode0(3);
    Motion_SetModeAndWaitAnimation(24, 4);
    Battle_WaitMode0(20);
    Call3((void (*)())ObjectMotion_ResetAndSetPositionInMode2, 23, 248, 488);
    Battle_WaitMode0(2);
    Call3((void (*)())ObjectMotion_SetPositionAndReset, 24, 264, 488);
    Call3((void (*)())ObjectMotion_ResetAndSetPositionInMode2, 23, 248, 440);
    Call3((void (*)())ObjectMotion_ResetAndSetPositionInMode2, 24, 264, 440);
    Call3((void (*)())ObjectMotion_ResetAndSetPositionInMode2, 25, 232, 456);
    Call3((void (*)())ObjectMotion_ResetAndSetPositionInMode2, 26, 232, 472);
    Call3((void (*)())ObjectMotion_ResetAndSetPositionInMode2, 27, 280, 456);
    Call3((void (*)())ObjectMotion_SetPositionAndReset, 28, 280, 472);
    Func_020010d4(23, 0, 0);
    Func_020010d4(24, 0, 0);
    Func_020010d4(25, 0, 0);
    Func_020010d4(26, 0, 0);
    Func_020010d4(27, 0, 0);
    Func_020010d4(28, 0, 0);
    Func_0200108c();
    GameFlag_SetBit(0x300);
}
