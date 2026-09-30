#include "TYPES.H"
#include "CALL.H"
extern u8 MsgHaidiaRockslideDestroyedFence[];
extern u8 MsgHaidiaSaveYourselves[];

extern u8 Sukureta_StrangerActions[];
extern u8 Sukureta_Actor11RockslideActions[];

s32 Engine_GameFlagIsSet();
void Engine_EventBegin();
s32 Engine_ActorRunRepeatedMotion();
void Engine_EventSetMessage();
void Engine_EventShowMessage();
void Engine_EventEnd();
void Engine_ActorStop();
void Engine_EventShowMessageAndWait();
void Engine_ActorShowEmote();
void Engine_CameraMoveTo();
void Engine_ActorWalkToAndWait();
void Engine_ActorFaceDirection();
s32 Engine_ActorGet();
void Engine_ActorSetPosition();
void Engine_EventWait();
s32 Engine_EventOpenMessage();
s32 Engine_EventChooseYesNo();
void Engine_GameFlagSet();
void Engine_ActorFaceActor();
void Engine_ActorSetAnimation();
void Engine_ActorJump();
void Engine_ActorSetAnimationAndWait();
void Object_SetTargetAndCallback();
void Object_SetActionCallbackAndRefreshById();
void Engine_ActorSetDestination();
void Engine_ActorWaitForMove();

void HaidiaSukureta_RunActorSequence(void)
{
    s32 record;
    s32 base;

    if (Value1(Engine_GameFlagIsSet, 0x839) == 0) {
        if (Value1(Engine_GameFlagIsSet, 0x82f) != 0) {
            Engine_EventBegin();
            Engine_ActorRunRepeatedMotion(11, 2);
            Engine_EventSetMessage((s32)MsgHaidiaSaveYourselves);
            Engine_EventShowMessage(11, 0);
            Engine_EventEnd();
        } else {
            Engine_EventBegin();
            Engine_ActorStop(11);
            Engine_ActorRunRepeatedMotion(11, 1);
            base = (s32)MsgHaidiaRockslideDestroyedFence;
            Engine_EventSetMessage(base);
            Engine_EventShowMessageAndWait(11, 0, 20);
            Call3(Engine_ActorShowEmote, 0, 0x100, 30);
            Engine_CameraMoveTo(0x620000, -1, 0x11b0000, 1);
            Call3(Engine_ActorWalkToAndWait, 0, 94, 0x125);
            Call3(Engine_ActorFaceDirection, 0, 0xa000, 0);
            record = Engine_ActorGet(0);
            if (record != 0) {
                Engine_ActorSetPosition(1, *(s32 *)(record + 8), *(s32 *)(record + 16));
            }
            Call3(Engine_ActorWalkToAndWait, 1, 110, 0x117);
            Call3(Engine_ActorFaceDirection, 1, 0xa000, 40);
            Engine_ActorRunRepeatedMotion(11, 2);
            Engine_EventWait(40);
            Engine_EventOpenMessage(11, 0);
            if (Engine_EventChooseYesNo(0, 0) == 0) {
                Engine_ActorRunRepeatedMotion(11, 2);
                Engine_EventWait(20);
                Engine_EventSetMessage((base + 2));
                Engine_EventShowMessage(11, 0);
                Engine_GameFlagSet(0x82f);
            } else {
                Engine_ActorRunRepeatedMotion(11, 2);
                Engine_EventWait(20);
                Engine_EventSetMessage((base + 3));
                Engine_EventShowMessageAndWait(11, 0, 40);
                Engine_ActorFaceActor(11, 0, 0);
                Engine_ActorSetAnimation(11, 1);
                Engine_ActorJump(11, 4, 40);
                Engine_ActorSetAnimation(11, 6);
                Call3(Engine_ActorShowEmote, 11, 0x101, 40);
                Engine_EventShowMessageAndWait(11, 0, 10);
                Engine_ActorSetAnimation(11, 1);
                Engine_EventWait(10);
                Engine_ActorSetAnimationAndWait(11, 3);
                Engine_EventShowMessageAndWait(11, 0, 10);
                Engine_ActorSetAnimationAndWait(11, 3);
                Call3(Object_SetTargetAndCallback, 0, 0x1000b, (s32)Sukureta_StrangerActions);
                Call3(Object_SetTargetAndCallback, 1, 0x1000b, (s32)Sukureta_StrangerActions);
                Object_SetActionCallbackAndRefreshById(11, (s32)Sukureta_Actor11RockslideActions);
                Engine_ActorStop(0);
                Engine_ActorStop(1);
                Call3(Engine_ActorFaceDirection, 0, 0x4000, 0);
                Call3(Engine_ActorFaceDirection, 1, 0x4000, 60);
                Call3(Engine_ActorShowEmote, 0, 0x105, 0);
                Engine_ActorShowEmote(1, 0x105, 120);
                Engine_GameFlagSet(0x839);
            }
            Engine_ActorSetAnimation(1, 2);
            record = Engine_ActorGet(0);
            if (record != 0) {
                Engine_ActorSetDestination(1, *(s16 *)(record + 10), *(s16 *)(record + 18));
            }
            Engine_ActorWaitForMove(1);
            Engine_ActorSetPosition(1, 0, 0);
            Engine_EventEnd();
        }
    }
}
