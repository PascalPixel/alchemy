#include "TYPES.H"
#include "CALL.H"
extern u8 MsgKorashiamuDayTiredWaiting[];
extern u8 MsgKorashiamuYouReadyForFinals[];
extern struct EventWork *gEventWork;
extern u8 KorashiamuIriguchi_ActionTable5[];
extern u8 KorashiamuIriguchi_ActionTable6[];
extern u8 KorashiamuIriguchi_ActionTable7[];
extern u8 KorashiamuIriguchi_ActionTable8[];
extern u8 KorashiamuIriguchi_ActionTable9[];

struct ActorMotion {
    u8 pad0[100];
    u16 step[2];
    u8 pad104[4];
    s32 target;
};

s32 Engine_GameFlagIsSet();
void Engine_GameFlagSet();
void Engine_EventBegin();
void Engine_ActorSetSpeed();
void Engine_ActorWalkToAndWait();
void SceneState_ForwardMaskedHalfwordWith10();
void Engine_CameraSetSpeed();
void Engine_CameraMoveTo();
void Engine_CameraWaitForMove();
void Engine_ActorFaceDirection();
void Engine_ActorRunRepeatedMotion();
void Engine_EventSetMessage();
s32 Engine_EventOpenMessage();
s32 Engine_EventChooseYesNo();
void Engine_EventWait();
void FieldScene_CallPairWith10();
void Engine_ActorSetAnimationAndWait();
void Engine_ActorSetAnimation();
void Engine_ActorStartRepeatedMotion();
void Engine_ActorSetAttachedEffect();
void Engine_ActorStop();
void Engine_TaskWait();
void Engine_ActorEnableActionCallback();
void Object_SetActionCallbackAndRefreshById();
s32 Object_GetById();
void Engine_EventRequestExit();
void Engine_EventShowMessage();
void Engine_EventEnd();

/* Colosso entrance: once flag 0x234 is set, gather the competitors, let the
 * player answer the entry question and send everyone into the arena. */
void KorashiamuIriguchi_RunGatherScene(void)
{
    s32 rec;
    u8 *record;
    s32 base5_200af24;

    if (Engine_GameFlagIsSet(0x234) == 0) {
    } else {
        Engine_GameFlagSet(0x235);
        Engine_EventBegin();
        Call3(Engine_ActorSetSpeed, 0, 0x10000, 0x8000);
        Call3(Engine_ActorWalkToAndWait, 0, 0x368, 120);
        SceneState_ForwardMaskedHalfwordWith10(0, 0xa000);
        Call2(Engine_CameraSetSpeed, 0x19999, 0x3333);
        Call4(Engine_CameraMoveTo, 0x3300000, -1, 0x700000, 1);
        Engine_CameraWaitForMove();
        Call3(Engine_ActorFaceDirection, 8, 0x3000, 20);
        Engine_ActorRunRepeatedMotion(8, 1);
        Engine_EventSetMessage((s32)MsgKorashiamuYouReadyForFinals);
        Value2(Engine_EventOpenMessage, 0x8008, 0);
        rec = Engine_EventChooseYesNo(0, 0);
        if (rec != 0) {
        } else {
            Engine_EventWait(20);
            SceneState_ForwardMaskedHalfwordWith10(12, 0xd000);
            Engine_ActorRunRepeatedMotion(12, 1);
            Engine_EventSetMessage((s32)MsgKorashiamuDayTiredWaiting);
            FieldScene_CallPairWith10(0x400c);
            SceneState_ForwardMaskedHalfwordWith10(17, 0);
            SceneState_ForwardMaskedHalfwordWith10(0, 0x8000);
            Engine_ActorSetAnimationAndWait(17, 3);
            Engine_ActorSetAnimationAndWait(0, 3);
            Engine_ActorSetAnimation(15, 1);
            Engine_ActorFaceDirection(15, 0xd000, 20);
            Engine_ActorRunRepeatedMotion(15, 1);
            FieldScene_CallPairWith10(15);
            SceneState_ForwardMaskedHalfwordWith10(16, 0x8000);
            Engine_ActorSetAnimationAndWait(16, 3);
            Engine_ActorRunRepeatedMotion(17, 2);
            SceneState_ForwardMaskedHalfwordWith10(17, 0xa000);
            FieldScene_CallPairWith10(0x4011);
            SceneState_ForwardMaskedHalfwordWith10(18, 0xb000);
            Engine_ActorStartRepeatedMotion(18, 2);
            FieldScene_CallPairWith10(0x4012);
            SceneState_ForwardMaskedHalfwordWith10(11, 0x3000);
            Engine_ActorSetAttachedEffect(11, 0x102);
            Engine_EventWait(60);
            Engine_ActorStartRepeatedMotion(11, 2);
            FieldScene_CallPairWith10(0x800b);
            Engine_ActorStop(13);
            Engine_TaskWait(1);
            Engine_ActorStartRepeatedMotion(13, 2);
            Engine_ActorStartRepeatedMotion(14, 2);
            Engine_ActorRunRepeatedMotion(16, 2);
            Engine_EventWait(20);
            Engine_ActorFaceDirection(13, 0, 0);
            Engine_ActorFaceDirection(14, 0x8000, 0);
            Engine_ActorFaceDirection(16, 0xb000, 40);
            Engine_ActorSetAnimation(13, 3);
            Engine_ActorSetAnimation(14, 3);
            Engine_ActorSetAnimationAndWait(16, 3);
            Call3(Engine_ActorSetSpeed, 13, 0x19999, 0xcccc);
            Call3(Engine_ActorSetSpeed, 14, 0x19999, 0xcccc);
            Call3(Engine_ActorSetSpeed, 16, 0x19999, 0xcccc);
            Engine_ActorEnableActionCallback(13, (s32)KorashiamuIriguchi_ActionTable5);
            Engine_ActorEnableActionCallback(16, (s32)KorashiamuIriguchi_ActionTable7);
            Engine_EventWait(20);
            Engine_ActorFaceDirection(15, 0xd000, 0);
            Engine_ActorFaceDirection(17, 0xb000, 0);
            Engine_ActorFaceDirection(0, 0xa000, 0);
            Engine_ActorFaceDirection(12, 0xd000, 0);
            Engine_ActorFaceDirection(18, 0xb000, 0);
            Object_SetActionCallbackAndRefreshById(14, (s32)KorashiamuIriguchi_ActionTable6);
            Engine_EventWait(20);
            Engine_ActorFaceDirection(8, 0, 0);
            Engine_ActorFaceDirection(11, 0x8000, 40);
            Engine_ActorSetAnimation(8, 3);
            Engine_ActorSetAnimationAndWait(11, 3);
            Engine_CameraMoveTo(0x3280000, -1, 0x560000, 1);
            Engine_CameraWaitForMove();
            Engine_ActorRunRepeatedMotion(8, 2);
            FieldScene_CallPairWith10(8);
            Engine_ActorSetSpeed(8, 0x10000, 0x8000);
            *(u8 *)(Object_GetById(8) + 90) &= 254;
            Engine_ActorWalkToAndWait(8, 0x318, 72);
            Engine_EventWait(1);
            {
                u8 *record = Object_GetById(8);
                u8 value = *(volatile u8 *)&record[90];
            
                record[90] = (u8)(value | 1);
            }
            SceneState_ForwardMaskedHalfwordWith10(8, 0);
            Engine_ActorStop(13);
            record = Object_GetById(13);
            *(s32 *)((s32)record + 108) = rec;
            ((struct ActorMotion *)record)->step[0] = rec;
            ((struct ActorMotion *)record)->step[1] = rec;
            *(s32 *)((s32)record + 36) = rec;
            *(s32 *)((s32)record + 40) = rec;
            *(s32 *)((s32)record + 44) = rec;
            *(s32 *)((s32)record + 56) = -0x80000000;
            *(s32 *)((s32)record + 60) = -0x80000000;
            *(s32 *)((s32)record + 64) = -0x80000000;
            Engine_TaskWait(1);
            base5_200af24 = (s32)KorashiamuIriguchi_ActionTable8;
            Engine_ActorEnableActionCallback(15, base5_200af24);
            Engine_EventWait(20);
            Engine_ActorEnableActionCallback(13, base5_200af24);
            Engine_EventWait(20);
            Engine_ActorEnableActionCallback(17, base5_200af24);
            Engine_EventWait(20);
            Engine_ActorEnableActionCallback(14, base5_200af24);
            Engine_EventWait(20);
            Engine_ActorEnableActionCallback(16, base5_200af24);
            Engine_EventWait(20);
            Engine_ActorEnableActionCallback(12, base5_200af24);
            Engine_EventWait(20);
            Engine_ActorEnableActionCallback(18, base5_200af24);
            Engine_EventWait(60);
            Engine_ActorEnableActionCallback(0, base5_200af24);
            Engine_EventWait(80);
            Engine_EventRequestExit(66);
            goto L_0200115c;
        }
        *(u16 *)((*(s32 *)&gEventWork + 0x1d8)) += 1;
        Engine_EventShowMessage(0x8008, 0);
        L_0200115c:;
        Engine_EventEnd();
    }
}
