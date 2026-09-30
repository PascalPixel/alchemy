#include "TYPES.H"
#include "CALL.H"

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

void HaidiaArashi_RunCallOutSequence(void)
{
    s32 record;
    s32 v5;

    Engine_EventOpenMessage(22, 0);
    Engine_ActorFaceEachOther(0, 22, 0);
    v5 = 0;
    if (Engine_EventChooseYesNo(0, 0) == 0) {
        Engine_EventSetMessage((s32)MsgHaidiaThinkForgetThings);
        v5 = 1;
    } else {
        Engine_EventSetMessage((s32)MsgHaidiaRockHitsLose);
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
        Engine_EventSetMessage((s32)MsgHaidiaRightDitchStuff);
    }
    Engine_EventShowMessage(22, 0);
    Engine_ActorSetAnimation(22, 2);
    record = Engine_ActorGet(0);
    if (record != 0) {
        Engine_ActorSetDestination(22, *(s16 *)(record + 10), *(s16 *)(record + 18));
    }
    Engine_ActorWaitForMove(22);
    Engine_ActorSetPosition(22, 0, 0);
    Event_PrepareObjectAndApplyValue(1, 1);
    Engine_GameFlagSet(0x837);
}
