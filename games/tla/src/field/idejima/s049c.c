#include "types.h"
#include "object_runtime.h"
#include "party_state.h"
#include "ram_buffer.h"
#include "call.h"
#include "text/msg_ids.h"

struct ObjectRuntime *Object_GetById(s32 id);
void ObjectDispatch_SetSingleChildFlags(struct ObjectRuntime *object, s32 flags);
s32 GameFlag_Test(s32 flag);
void GameFlag_SetBit(s32 flag);
void Engine_EventBegin(void);
void Engine_EventEnd();
void Engine_EventSetMessage();
void Engine_EventShowMessage();
void Engine_ActorSetAnimation();
void Engine_AudioPlayCue(s32 cue);
s32 Engine_ActorShowEmote();
void Event_FinishScreenTransition(void);
void Event_WaitTransitionFrames(void);
void Event_PrepareObjectAndApplyValue();
void Battle_WaitMode0();
void Motion_SetVarCbAndRefresh();
void Motion_SetModeAndWaitAnimation();
void Object_LinkPair();
void ObjectMotion_Launch();
void ObjectMotion_ArmCallback();
void ObjectMotion_SetAngleToward();
void ObjectMotion_ResetAndSetPosition();
void ObjectMotion_CommitCurrentPositionAndActivate();
void ObjectMotion_SetSpeedParameters(s32 actor, s32 speed, s32 acceleration);
void ObjectMotion_SetPositionAndReset(s32 actor, s32 x, s32 z);
void Func_02003190();
void Func_02003228();
void Func_02003298();
void Func_02003310(s32 skip);
void Func_020032e8(s32 countdown);

// A scene of actor 6 and the leader that ends by setting flag 0x865.
void Func_0200049c(void)
{
    struct ObjectRuntime *leader;

    Engine_EventBegin();
    Func_02003310(0);
    Motion_SetVarCbAndRefresh(6, 2);
    Engine_EventSetMessage(MsgIdejimaSecondGroan);
    Func_02003298(6, 0, 40);
    Engine_EventShowMessage(6, 0);
    Engine_ActorSetAnimation(6, 18);
    Engine_EventShowMessage(6, 0);
    Call6(Func_02003190, 38, 31, 2, 1, 14, 40);
    Object_GetById(6)->motion_flags = 3;
    ObjectMotion_Launch(6, 4, 0);
    Engine_ActorSetAnimation(6, 1);
    ObjectDispatch_SetSingleChildFlags(Object_GetById(6), 1);
    Battle_WaitMode0(20);
    Object_LinkPair(6, gPartyState.current_owner, 0);
    Engine_EventShowMessage(6, 0);
    Motion_SetModeAndWaitAnimation(gPartyState.current_owner, 3);
    Engine_ActorShowEmote(6, 0x105, 20);
    Engine_EventShowMessage(6, 0);
    Call3(ObjectMotion_ArmCallback, 6, 0xe000, 20);
    Call3(ObjectMotion_ArmCallback, 6, 0xa000, 40);
    Call3(ObjectMotion_ArmCallback, 6, 0x6000, 0);
    Engine_ActorShowEmote(6, 0x100, 20);
    Engine_EventShowMessage(6, 0);
    ObjectMotion_SetAngleToward(6, gPartyState.current_owner, 0);
    Battle_WaitMode0(20);
    Engine_ActorSetAnimation(6, 3);
    Engine_EventShowMessage(6, 0);
    Motion_SetModeAndWaitAnimation(gPartyState.current_owner, 3);
    Engine_EventShowMessage(6, 0);
    Call3(ObjectMotion_SetSpeedParameters, 6, 0x19999, 0xcccc);
    Call3(ObjectMotion_SetPositionAndReset, 6, 212, 680);
    Call3(ObjectMotion_ArmCallback, 6, 0xe000, 20);
    Engine_EventShowMessage(6, 0);
    Engine_ActorSetAnimation(6, 2);
    leader = Object_GetById(gPartyState.current_owner);
    if (leader != 0)
        ObjectMotion_ResetAndSetPosition(6, ((s16 *)&leader->x)[1], ((s16 *)&leader->z)[1]);
    ObjectMotion_CommitCurrentPositionAndActivate(6);
    Func_02003228(6, 0, 0);
    Event_PrepareObjectAndApplyValue(6, 1);
    GameFlag_SetBit(0x865);
    Engine_EventEnd();
}
static inline s32 FlagChk(s32 flag)
{
    /* FAKEMATCH: scene calls reach the engine through one inline layer. */
    return GameFlag_Test(flag);
}

static inline void FlagSet(s32 flag)
{
    /* FAKEMATCH: scene calls reach the engine through one inline layer. */
    GameFlag_SetBit(flag);
}

static inline void ActorPlace(s32 actor, s32 x, s32 z)
{
    /* FAKEMATCH: scene calls reach the engine through one inline layer. */
    ObjectMotion_SetPositionAndReset(actor, x, z);
}

void Func_02000640(void)
{
    Engine_EventBegin();
    Func_02003310(0);
    Engine_AudioPlayCue(123);
    Event_FinishScreenTransition();
    Event_WaitTransitionFrames();
    Func_020032e8(3);
}

void Func_02000664(void)
{
    if (!FlagChk(0x200)) {
        ObjectMotion_SetSpeedParameters(gPartyState.current_owner, 0xcccc, 0x6666);
        ActorPlace(gPartyState.current_owner, 152, 756);
        FlagSet(0x200);
    }
}
