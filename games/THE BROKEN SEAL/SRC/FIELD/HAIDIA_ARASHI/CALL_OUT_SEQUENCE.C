#include "TYPES.H"

extern u8 MsgHaidiaKnowRightOk[];
extern u8 MsgHaidiaRightDitchStuff[];
extern u8 MsgHaidiaRockHitsLose[];
extern u8 MsgHaidiaThinkForgetThings[];
s32 Engine_EventOpenMessage();
void Engine_ActorFaceEachOther();
s32 Engine_EventChooseYesNo();
void Engine_EventSetMessage();
void Engine_EventWait();
void Engine_EventShowMessageAndWait();
void Engine_ActorSetAttachedEffect();
void Engine_ActorSetAnimation();
void Engine_ActorFaceActor();
void Engine_ActorSetAnimationAndWait();
void Engine_EventShowMessage();
s32 Engine_ActorGet();
void Engine_ActorSetDestination();
void Engine_ActorWaitForMove();
void Engine_ActorSetPosition();
void Event_PrepareObjectAndApplyValue();
void Engine_GameFlagSet();

/* Call sites spelled through these wrappers pass their constants straight
 * into the argument registers; a direct call precomputes a costly constant
 * into a pseudo that the compiler then shares with later uses in the block.
 * A value-returning call also sets r0 last of its arguments. */

static __inline__ void Call1(void (*f)(), s32 a0)
{
    f(a0);
}

static __inline__ s32 Value1(s32 (*f)(), s32 a0)
{
    return f(a0);
}

static __inline__ void Call2(void (*f)(), s32 a0, s32 a1)
{
    f(a0, a1);
}

static __inline__ s32 Value2(s32 (*f)(), s32 a0, s32 a1)
{
    return f(a0, a1);
}

void HaidiaArashi_RunCallOutSequence(void)
{
    s32 record;
    s32 v5;

    Value2(Engine_EventOpenMessage, 22, 0);
    Engine_ActorFaceEachOther(0, 22, 0);
    v5 = 0;
    if (Value2(Engine_EventChooseYesNo, 0, 0) == 0) {
        Call1(Engine_EventSetMessage, (s32)MsgHaidiaThinkForgetThings);
        v5 = 1;
    } else {
        Call1(Engine_EventSetMessage, (s32)MsgHaidiaRockHitsLose);
    }
    Engine_EventWait(20);
    Engine_EventShowMessageAndWait(22, 0, 40);
    Call2(Engine_ActorSetAttachedEffect, 22, 0x100);
    Engine_ActorSetAnimation(21, 3);
    Engine_ActorSetAnimation(22, 1);
    Engine_EventWait(40);
    Engine_ActorFaceActor(22, 0, 0);
    Engine_EventWait(20);
    Engine_ActorSetAnimationAndWait(22, 3);
    if (v5 != 0) {

        Engine_EventSetMessage((s32)MsgHaidiaKnowRightOk);
    } else {
        Call1(Engine_EventSetMessage, (s32)MsgHaidiaRightDitchStuff);
    }
    Engine_EventShowMessage(22, 0);
    Engine_ActorSetAnimation(22, 2);
    record = Value1(Engine_ActorGet, 0);
    if (record != 0) {
        Engine_ActorSetDestination(22, *(s16 *)(record + 10), *(s16 *)(record + 18));
    }
    Engine_ActorWaitForMove(22);
    Engine_ActorSetPosition(22, 0, 0);
    Event_PrepareObjectAndApplyValue(1, 1);
    Call1(Engine_GameFlagSet, 0x837);
}
