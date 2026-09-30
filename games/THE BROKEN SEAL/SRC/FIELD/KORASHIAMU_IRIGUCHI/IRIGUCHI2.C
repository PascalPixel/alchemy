#include "TYPES.H"
#include "CALL.H"

extern u8 MsgKorashiamuHeyYouRe[];
extern struct EventWork *gEventWork;
extern u8 KorashiamuIriguchi_ActionTable1[];
extern u8 KorashiamuIriguchi_ActionTable2[];
extern u8 KorashiamuIriguchi_ActionTable4[];
#define SCENE_WORK ((u8 *)gEventWork)

static __inline__ void bump_step(s32 amount)
{
    u16 *step = (u16 *)((u8 *)gEventWork + 0x1d8);

    *step = *step + amount;
}

#define SCENE_FIELD_1C8 (*(s32 *)(SCENE_WORK + 0x1c8))
void FieldScene_CallPairWith10();
void SceneState_ForwardMaskedHalfwordWith10();
s32 Engine_GameFlagIsSet();
void Engine_GameFlagSet();
void Engine_EventWait();
void Engine_EventBegin();
void Engine_EventEnd();
s32 Engine_EventChooseYesNo();
s32 Object_GetById();
void Engine_ActorSetSpeed();
void Engine_ActorEnableActionCallback();
void Object_SetActionCallbackAndRefreshById();
void Engine_ActorWalkTo();
void Engine_ActorWalkToAndWait();
void Engine_ActorSetPosition();
void Engine_ActorSetAnimation();
void Engine_ActorSetAnimationAndWait();
void Engine_ActorStartRepeatedMotion();
void Engine_ActorRunRepeatedMotion();
void Engine_EventSetMessage();
void Engine_EventOpenMessage();
void Engine_EventShowMessageAndWait();
void Engine_ActorFaceDirection();
void Engine_ActorShowEmote();
void Engine_ActorSetAttachedEffect();
void Engine_CameraSetSpeed();
void Engine_CameraMoveTo();
void Engine_CameraWaitForMove();
u8 *Engine_EventGetViewCenter();
void Engine_EventRequestExit();
void Engine_EventCloseScreen();
void Engine_EventWaitForScreen();

extern u8 MsgKorashiamuDayTiredWaiting[];
extern u8 MsgKorashiamuYouReadyForFinals[];
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

void Engine_ActorStop();
void Engine_TaskWait();
void Engine_EventShowMessage();

/* Runs only while flag 0x962 is set: resets the battle runtime, stages
 * presentation actors 17 to 20, places party actors 0 to 3 (taking optional
 * party record positions), then plays one of three closing sequences chosen by
 * two queries. The later two sequences first advance the scene step counter;
 * every sequence stores 40 in scene work field 0x1c8. */
void Scene_RunBranchingActorPresentation(void)
{
    s32 record;
    s32 value;
    u8 *slot;

    if (Engine_GameFlagIsSet(0x962) == 0) {
        return;
    }

    Engine_EventBegin();
    Engine_ActorRunRepeatedMotion(17, 1);
    Call3(Engine_ActorFaceDirection, 17, 0x3000, 20);
    value = 192;
    Engine_ActorFaceDirection(17, 0, 60);
    Call3(Engine_ActorShowEmote, 17, 0x100, 40);
    SceneState_ForwardMaskedHalfwordWith10(17, (value << 6));
    Engine_EventSetMessage((s32)MsgKorashiamuHeyYouRe);
    Engine_ActorStartRepeatedMotion(17, 2);
    FieldScene_CallPairWith10(17);
    Engine_ActorFaceDirection(18, (value << 6), 0);
    Engine_ActorFaceDirection(19, (value << 6), 0);
    Engine_ActorFaceDirection(20, (value << 6), 0);
    slot = Engine_EventGetViewCenter();
    *(u8 *)(slot + 85) = 0;
    Engine_CameraSetSpeed(0x19999, 0x3333);
    Call4(Engine_CameraMoveTo, 0x1000000, -1, 0xac0000, 1);
    Engine_CameraWaitForMove();
    Engine_EventWait(20);
    Call3(Engine_ActorShowEmote, 18, 0x102, 40);
    FieldScene_CallPairWith10(18);
    Engine_ActorSetAnimationAndWait(17, 3);
    FieldScene_CallPairWith10(17);
    Call2(Engine_ActorSetAttachedEffect, 19, 0x102);
    Engine_EventWait(40);
    SceneState_ForwardMaskedHalfwordWith10(19, 0);
    FieldScene_CallPairWith10(19);
    Call3(Engine_ActorShowEmote, 20, 0x103, 40);
    SceneState_ForwardMaskedHalfwordWith10(20, 0);
    Engine_ActorStartRepeatedMotion(20, 2);
    Engine_EventShowMessageAndWait(20, 0, 20);
    Engine_ActorRunRepeatedMotion(17, 1);
    Engine_EventWait(10);
    Call3(Engine_ActorFaceDirection, 17, 0xb000, 20);
    Engine_EventShowMessageAndWait(17, 0, 20);
    Engine_ActorRunRepeatedMotion(18, 1);
    Engine_ActorSetAnimationAndWait(18, 4);
    Engine_EventWait(20);
    SceneState_ForwardMaskedHalfwordWith10(17, 0x8000);
    Engine_EventShowMessageAndWait(17, 0, 20);
    Engine_ActorRunRepeatedMotion(19, 1);
    Engine_EventWait(40);
    Engine_ActorSetAnimationAndWait(19, 4);
    Engine_EventShowMessageAndWait(17, 0, 20);
    Engine_ActorRunRepeatedMotion(20, 2);
    Engine_EventWait(40);
    Engine_ActorSetAnimationAndWait(20, 3);
    Engine_EventWait(20);
    Engine_ActorSetAnimation(17, 3);
    Engine_ActorSetAnimation(18, 3);
    Engine_ActorSetAnimation(19, 3);
    Engine_ActorSetAnimationAndWait(20, 3);
    Engine_ActorFaceDirection(17, (value << 6), 0);
    Engine_ActorFaceDirection(18, (value << 6), 0);
    Engine_ActorFaceDirection(19, (value << 6), 0);
    SceneState_ForwardMaskedHalfwordWith10(20, (value << 6));
    Call3(Engine_ActorSetSpeed, 17, 0x9999, 0x4ccc);
    Call3(Engine_ActorWalkToAndWait, 17, 0x102, 172);
    Call3(Engine_ActorSetSpeed, 0, 0xcccc, 0x6666);
    Call3(Engine_ActorWalkToAndWait, 0, 0x106, 188);
    Call3(Engine_ActorFaceDirection, 0, 0xc000, 0);
    record = Object_GetById(0);
    if (record != 0) {
        Engine_ActorSetPosition(1, *(s32 *)(record + 8), *(s32 *)(record + 16));
    }
    record = Object_GetById(0);
    if (record != 0) {
        Engine_ActorSetPosition(2, *(s32 *)(record + 8), *(s32 *)(record + 16));
    }
    record = Object_GetById(0);
    if (record != 0) {
        Engine_ActorSetPosition(3, *(s32 *)(record + 8), *(s32 *)(record + 16));
    }
    Call3(Engine_ActorSetSpeed, 1, 0xcccc, 0x6666);
    Call3(Engine_ActorSetSpeed, 2, 0xcccc, 0x6666);
    Call3(Engine_ActorSetSpeed, 3, 0xcccc, 0x6666);
    Engine_ActorWalkTo(1, 246, 200);
    Call3(Engine_ActorWalkTo, 2, 0x106, 200);
    Call3(Engine_ActorWalkToAndWait, 3, 0x116, 200);
    Engine_ActorSetAnimation(2, 1);
    Engine_ActorSetAnimation(1, 1);
    Call3(Engine_ActorFaceDirection, 1, 0xc000, 0);
    Call3(Engine_ActorFaceDirection, 2, 0xc000, 0);
    Call3(Engine_ActorFaceDirection, 3, 0xc000, 40);
    Engine_ActorRunRepeatedMotion(17, 1);
    FieldScene_CallPairWith10(17);
    Engine_ActorStartRepeatedMotion(0, 2);
    Engine_ActorStartRepeatedMotion(1, 2);
    Engine_ActorStartRepeatedMotion(2, 2);
    Engine_ActorRunRepeatedMotion(3, 2);
    Engine_ActorSetAnimationAndWait(17, 3);
    Value2(Engine_EventOpenMessage, 17, 0);
    Call3(Engine_ActorFaceDirection, 1, 0xe000, 0);
    Call3(Engine_ActorFaceDirection, 3, 0xa000, 0);

    if (Engine_EventChooseYesNo(0, 0) == 0) {
        Engine_ActorSetAnimationAndWait(17, 3);
        FieldScene_CallPairWith10(17);
        Engine_ActorEnableActionCallback(1, (s32)KorashiamuIriguchi_ActionTable1);
        Engine_ActorEnableActionCallback(2, (s32)KorashiamuIriguchi_ActionTable1);
        Object_SetActionCallbackAndRefreshById(3, (s32)KorashiamuIriguchi_ActionTable1);
        Engine_CameraSetSpeed(0x6666, 0xccc);
        Engine_CameraMoveTo(0x1000000, -1, 0x640000, 1);
        Call3(Engine_ActorSetSpeed, 17, 0x10000, 0x8000);
        Engine_ActorEnableActionCallback(17, (s32)KorashiamuIriguchi_ActionTable4);
        Engine_EventWait(10);
        Call3(Engine_ActorSetSpeed, 0, 0x10000, 0x8000);
        Engine_ActorEnableActionCallback(0, (s32)KorashiamuIriguchi_ActionTable4);
        Engine_EventWait(80);
        SCENE_FIELD_1C8 = 40;
        Engine_EventCloseScreen();
        Engine_EventWaitForScreen();
    } else {
        bump_step(1);
        Call3(Engine_ActorShowEmote, 17, 0x102, 40);
        FieldScene_CallPairWith10(17);
        Call3(Engine_ActorShowEmote, 18, 0x102, 40);
        FieldScene_CallPairWith10(18);
        Call3(Engine_ActorShowEmote, 19, 0x102, 40);
        FieldScene_CallPairWith10(19);
        Call3(Engine_ActorShowEmote, 20, 0x102, 40);
        FieldScene_CallPairWith10(19);
        Engine_ActorSetAnimationAndWait(17, 4);
        FieldScene_CallPairWith10(17);
        Engine_ActorRunRepeatedMotion(2, 1);
        Engine_EventWait(20);
        FieldScene_CallPairWith10(2);
        Call3(Engine_ActorShowEmote, 3, 0x102, 40);
        FieldScene_CallPairWith10(3);
        Call3(Engine_ActorFaceDirection, 17, 0xb000, 0);
        Engine_ActorFaceDirection(19, 0x8000, 0);
        Engine_ActorFaceDirection(20, 0, 60);
        Engine_ActorFaceDirection(17, (value << 6), 0);
        Engine_ActorFaceDirection(19, (value << 6), 0);
        Engine_ActorFaceDirection(20, (value << 6), 20);
        Engine_ActorSetAnimation(17, 3);
        Engine_ActorSetAnimation(18, 3);
        Engine_ActorSetAnimation(19, 3);
        Engine_ActorSetAnimationAndWait(20, 3);
        Call3(Engine_ActorShowEmote, 3, 0x100, 60);
        Call2(SceneState_ForwardMaskedHalfwordWith10, 3, 0xa000);
        FieldScene_CallPairWith10(3);
        Engine_ActorSetAnimation(1, 4);
        Engine_EventWait(20);
        Value2(Engine_EventOpenMessage, 1, 0);

        record = Engine_EventChooseYesNo(0, 0);
        if (record == 0) {
            Engine_ActorSetAnimationAndWait(17, 3);
            FieldScene_CallPairWith10(17);
            Engine_ActorEnableActionCallback(1, (s32)KorashiamuIriguchi_ActionTable1);
            Engine_ActorEnableActionCallback(2, (s32)KorashiamuIriguchi_ActionTable1);
            Object_SetActionCallbackAndRefreshById(3, (s32)KorashiamuIriguchi_ActionTable1);
            Engine_CameraSetSpeed(0x6666, 0xccc);
            Engine_CameraMoveTo(0x1000000, -1, 0x640000, 1);
            Call3(Engine_ActorSetSpeed, 17, 0x10000, 0x8000);
            Engine_ActorEnableActionCallback(17, (s32)KorashiamuIriguchi_ActionTable4);
            Engine_EventWait(10);
            Call3(Engine_ActorSetSpeed, 0, 0x10000, 0x8000);
            Engine_ActorEnableActionCallback(0, (s32)KorashiamuIriguchi_ActionTable4);
            Engine_EventWait(80);
            SCENE_FIELD_1C8 = 40;
            Engine_EventCloseScreen();
            Engine_EventWaitForScreen();
        } else {
            bump_step(1);
            Engine_EventWait(20);
            Engine_ActorStartRepeatedMotion(1, 2);
            Engine_EventShowMessageAndWait(1, 0, 20);
            Engine_ActorShowEmote(2, 0x102, 60);
            FieldScene_CallPairWith10(2);
            SceneState_ForwardMaskedHalfwordWith10(3, 0x8000);
            Engine_ActorSetAnimation(3, 3);
            FieldScene_CallPairWith10(3);
            Engine_ActorEnableActionCallback(2, (s32)KorashiamuIriguchi_ActionTable2);
            Object_SetActionCallbackAndRefreshById(3, (s32)KorashiamuIriguchi_ActionTable2);
            Engine_EventWait(20);
            Object_SetActionCallbackAndRefreshById(0, (s32)KorashiamuIriguchi_ActionTable2);
            Call3(Engine_ActorWalkToAndWait, 1, 0x106, 188);
            SceneState_ForwardMaskedHalfwordWith10(1, 0xc000);
            Engine_ActorSetAnimationAndWait(1, 3);
            FieldScene_CallPairWith10(1);
            Engine_CameraSetSpeed(0x6666, 0xccc);
            Engine_CameraMoveTo(0x1000000, -1, 0x640000, 1);
            Call3(Engine_ActorSetSpeed, 17, 0x10000, 0x8000);
            Engine_ActorEnableActionCallback(17, (s32)KorashiamuIriguchi_ActionTable4);
            Engine_EventWait(10);
            Call3(Engine_ActorSetSpeed, 1, 0x10000, 0x8000);
            Engine_ActorEnableActionCallback(1, (s32)KorashiamuIriguchi_ActionTable4);
            Engine_EventWait(80);
            SCENE_FIELD_1C8 = 40;
            Engine_EventCloseScreen();
            Engine_EventWaitForScreen();
        }
    }

    Engine_EventRequestExit(2);
    Engine_GameFlagSet(0x93f);
    Engine_EventEnd();
}

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
