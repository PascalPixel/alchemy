#include "TYPES.H"
#include "CALL.H"
#include "FIELDOBJ.H"
#include "FIELD_EVENT.H"
#include "FIELD_SCENE.H"
#include "text/MSG_IDS.H"

extern u8 KorashiamuIriguchi_ActionTable1[];
extern u8 KorashiamuIriguchi_ActionTable2[];
extern u8 KorashiamuIriguchi_ActionTable4[];

void FieldScene_CallPairWith10();
void SceneState_ForwardMaskedHalfwordWith10();
void Object_SetActionCallbackAndRefreshById();

extern u8 KorashiamuIriguchi_ActionTable5[];
extern u8 KorashiamuIriguchi_ActionTable6[];
extern u8 KorashiamuIriguchi_ActionTable7[];
extern u8 KorashiamuIriguchi_ActionTable8[];
extern u8 KorashiamuIriguchi_ActionTable9[];

void Scene_RunBranchingActorPresentation(void)
{
    s32 record;

    if (Engine_GameFlagIsSet(0x962) == 0) {
        return;
    }

    Engine_EventBegin();
    Engine_ActorRunRepeatedMotion(17, 1);
    Actor_FaceDirection(17, 0x3000, 20);
    Engine_ActorFaceDirection(17, FACING_EAST, 60);
    Actor_ShowEmote(17, 256, 40);
    SceneState_ForwardMaskedHalfwordWith10(17, 0x3000);
    Engine_EventSetMessage((s32)MsgKorashiamuHeyYouRe);
    Engine_ActorStartRepeatedMotion(17, 2);
    FieldScene_CallPairWith10(17);
    Engine_ActorFaceDirection(18, 0x3000, 0);
    Engine_ActorFaceDirection(19, 0x3000, 0);
    Engine_ActorFaceDirection(20, 0x3000, 0);
    Engine_EventGetViewCenter()->motion_flags = 0;
    Engine_CameraSetSpeed(0x19999, 0x3333);
    Camera_MoveTo(0x1000000, -1, 0xac0000, 1);
    Engine_CameraWaitForMove();
    Engine_EventWait(20);
    Actor_ShowEmote(18, 258, 40);
    FieldScene_CallPairWith10(18);
    Engine_ActorSetAnimationAndWait(17, 3);
    FieldScene_CallPairWith10(17);
    Actor_SetAttachedEffect(19, 258);
    Engine_EventWait(40);
    SceneState_ForwardMaskedHalfwordWith10(19, 0);
    FieldScene_CallPairWith10(19);
    Actor_ShowEmote(20, 259, 40);
    SceneState_ForwardMaskedHalfwordWith10(20, 0);
    Engine_ActorStartRepeatedMotion(20, 2);
    Engine_EventShowMessageAndWait(20, 0, 20);
    Engine_ActorRunRepeatedMotion(17, 1);
    Engine_EventWait(10);
    Actor_FaceDirection(17, 0xb000, 20);
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
    Engine_ActorFaceDirection(17, 0x3000, 0);
    Engine_ActorFaceDirection(18, 0x3000, 0);
    Engine_ActorFaceDirection(19, 0x3000, 0);
    SceneState_ForwardMaskedHalfwordWith10(20, 0x3000);
    Actor_SetSpeed(17, 0x9999, 0x4ccc);
    Actor_WalkToAndWait(17, 258, 172);
    Actor_SetSpeed(0, 0xcccc, 0x6666);
    Actor_WalkToAndWait(0, 262, 188);
    Actor_FaceDirection(0, FACING_NORTH, 0);
    record = Object_GetById(0);
    if (record != 0) {
        Engine_ActorSetPosition(1, ((struct FieldActor *)record)->x.fixed, ((struct FieldActor *)record)->z.fixed);
    }
    record = Object_GetById(0);
    if (record != 0) {
        Engine_ActorSetPosition(2, ((struct FieldActor *)record)->x.fixed, ((struct FieldActor *)record)->z.fixed);
    }
    record = Object_GetById(0);
    if (record != 0) {
        Engine_ActorSetPosition(3, ((struct FieldActor *)record)->x.fixed, ((struct FieldActor *)record)->z.fixed);
    }
    Actor_SetSpeed(1, 0xcccc, 0x6666);
    Actor_SetSpeed(2, 0xcccc, 0x6666);
    Actor_SetSpeed(3, 0xcccc, 0x6666);
    Engine_ActorWalkTo(1, 246, 200);
    Actor_WalkTo(2, 262, 200);
    Actor_WalkToAndWait(3, 278, 200);
    Engine_ActorSetAnimation(2, 1);
    Engine_ActorSetAnimation(1, 1);
    Actor_FaceDirection(1, FACING_NORTH, 0);
    Actor_FaceDirection(2, FACING_NORTH, 0);
    Actor_FaceDirection(3, FACING_NORTH, 40);
    Engine_ActorRunRepeatedMotion(17, 1);
    FieldScene_CallPairWith10(17);
    Engine_ActorStartRepeatedMotion(0, 2);
    Engine_ActorStartRepeatedMotion(1, 2);
    Engine_ActorStartRepeatedMotion(2, 2);
    Engine_ActorRunRepeatedMotion(3, 2);
    Engine_ActorSetAnimationAndWait(17, 3);
    /* FAKEMATCH: the void result is discarded; Call2 changes argument allocation. */
    Event_OpenMessage(17, 0);
    Actor_FaceDirection(1, FACING_NORTHEAST, 0);
    Actor_FaceDirection(3, FACING_NORTHWEST, 0);

    if (Engine_EventChooseYesNo(0, 0) == 0) {
        Engine_ActorSetAnimationAndWait(17, 3);
        FieldScene_CallPairWith10(17);
        Engine_ActorEnableActionCallback(1, KorashiamuIriguchi_ActionTable1);
        Engine_ActorEnableActionCallback(2, KorashiamuIriguchi_ActionTable1);
        Object_SetActionCallbackAndRefreshById(3, KorashiamuIriguchi_ActionTable1);
        Engine_CameraSetSpeed(0x6666, 0xccc);
        Engine_CameraMoveTo(0x1000000, -1, 0x640000, 1);
        Actor_SetSpeed(17, 0x10000, 0x8000);
        Engine_ActorEnableActionCallback(17, KorashiamuIriguchi_ActionTable4);
        Engine_EventWait(10);
        Actor_SetSpeed(0, 0x10000, 0x8000);
        Engine_ActorEnableActionCallback(0, KorashiamuIriguchi_ActionTable4);
        Engine_EventWait(80);
        gEventWork->transition_frames = 40;
        Engine_EventCloseScreen();
        Engine_EventWaitForScreen();
    } else {
        Event_SkipMessages(1);
        Actor_ShowEmote(17, 258, 40);
        FieldScene_CallPairWith10(17);
        Actor_ShowEmote(18, 258, 40);
        FieldScene_CallPairWith10(18);
        Actor_ShowEmote(19, 258, 40);
        FieldScene_CallPairWith10(19);
        Actor_ShowEmote(20, 258, 40);
        FieldScene_CallPairWith10(19);
        Engine_ActorSetAnimationAndWait(17, 4);
        FieldScene_CallPairWith10(17);
        Engine_ActorRunRepeatedMotion(2, 1);
        Engine_EventWait(20);
        FieldScene_CallPairWith10(2);
        Actor_ShowEmote(3, 258, 40);
        FieldScene_CallPairWith10(3);
        Actor_FaceDirection(17, 0xb000, 0);
        Engine_ActorFaceDirection(19, FACING_WEST, 0);
        Engine_ActorFaceDirection(20, FACING_EAST, 60);
        Engine_ActorFaceDirection(17, 0x3000, 0);
        Engine_ActorFaceDirection(19, 0x3000, 0);
        Engine_ActorFaceDirection(20, 0x3000, 20);
        Engine_ActorSetAnimation(17, 3);
        Engine_ActorSetAnimation(18, 3);
        Engine_ActorSetAnimation(19, 3);
        Engine_ActorSetAnimationAndWait(20, 3);
        Actor_ShowEmote(3, 256, 60);
        Call2(SceneState_ForwardMaskedHalfwordWith10, 3, 0xa000);
        FieldScene_CallPairWith10(3);
        Engine_ActorSetAnimation(1, 4);
        Engine_EventWait(20);
        /* FAKEMATCH: the void result is discarded; Call2 changes argument allocation. */
        Event_OpenMessage(1, 0);

        record = Engine_EventChooseYesNo(0, 0);
        if (record == 0) {
            Engine_ActorSetAnimationAndWait(17, 3);
            FieldScene_CallPairWith10(17);
            Engine_ActorEnableActionCallback(1, KorashiamuIriguchi_ActionTable1);
            Engine_ActorEnableActionCallback(2, KorashiamuIriguchi_ActionTable1);
            Object_SetActionCallbackAndRefreshById(3, KorashiamuIriguchi_ActionTable1);
            Engine_CameraSetSpeed(0x6666, 0xccc);
            Engine_CameraMoveTo(0x1000000, -1, 0x640000, 1);
            Actor_SetSpeed(17, 0x10000, 0x8000);
            Engine_ActorEnableActionCallback(17, KorashiamuIriguchi_ActionTable4);
            Engine_EventWait(10);
            Actor_SetSpeed(0, 0x10000, 0x8000);
            Engine_ActorEnableActionCallback(0, KorashiamuIriguchi_ActionTable4);
            Engine_EventWait(80);
            gEventWork->transition_frames = 40;
            Engine_EventCloseScreen();
            Engine_EventWaitForScreen();
        } else {
            Event_SkipMessages(1);
            Engine_EventWait(20);
            Engine_ActorStartRepeatedMotion(1, 2);
            Engine_EventShowMessageAndWait(1, 0, 20);
            Engine_ActorShowEmote(2, 258, 60);
            FieldScene_CallPairWith10(2);
            SceneState_ForwardMaskedHalfwordWith10(3, 0x8000);
            Engine_ActorSetAnimation(3, 3);
            FieldScene_CallPairWith10(3);
            Engine_ActorEnableActionCallback(2, KorashiamuIriguchi_ActionTable2);
            Object_SetActionCallbackAndRefreshById(3, KorashiamuIriguchi_ActionTable2);
            Engine_EventWait(20);
            Object_SetActionCallbackAndRefreshById(0, KorashiamuIriguchi_ActionTable2);
            Actor_WalkToAndWait(1, 262, 188);
            SceneState_ForwardMaskedHalfwordWith10(1, 0xc000);
            Engine_ActorSetAnimationAndWait(1, 3);
            FieldScene_CallPairWith10(1);
            Engine_CameraSetSpeed(0x6666, 0xccc);
            Engine_CameraMoveTo(0x1000000, -1, 0x640000, 1);
            Actor_SetSpeed(17, 0x10000, 0x8000);
            Engine_ActorEnableActionCallback(17, KorashiamuIriguchi_ActionTable4);
            Engine_EventWait(10);
            Actor_SetSpeed(1, 0x10000, 0x8000);
            Engine_ActorEnableActionCallback(1, KorashiamuIriguchi_ActionTable4);
            Engine_EventWait(80);
            gEventWork->transition_frames = 40;
            Engine_EventCloseScreen();
            Engine_EventWaitForScreen();
        }
    }

    Engine_EventRequestExit(2);
    Engine_GameFlagSet(0x93f);
    Engine_EventEnd();
}

void KorashiamuIriguchi_RunGatherScene(void)
{
    struct FieldActor *actor;
    s32 script;

    if (Engine_GameFlagIsSet(0x234) != 0) {
        Engine_GameFlagSet(0x235);
        Engine_EventBegin();
        Actor_SetSpeed(0, 0x10000, 0x8000);
        Actor_WalkToAndWait(0, 872, 120);
        SceneState_ForwardMaskedHalfwordWith10(0, 0xa000);
        Camera_SetSpeed(0x19999, 0x3333);
        Camera_MoveTo(0x3300000, -1, 0x700000, 1);
        Engine_CameraWaitForMove();
        Actor_FaceDirection(8, 0x3000, 20);
        Engine_ActorRunRepeatedMotion(8, 1);
        Engine_EventSetMessage((s32)MsgKorashiamuYouReadyForFinals);
        /* FAKEMATCH: the void result is discarded; Call2 changes argument allocation. */
        Event_OpenMessage(0x8008, 0);
        if (Engine_EventChooseYesNo(0, 0) == 0) {
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
            Engine_ActorSetAttachedEffect(11, 258);
            Engine_EventWait(60);
            Engine_ActorStartRepeatedMotion(11, 2);
            FieldScene_CallPairWith10(0x800b);
            Engine_ActorStop(13);
            Engine_TaskWait(1);
            Engine_ActorStartRepeatedMotion(13, 2);
            Engine_ActorStartRepeatedMotion(14, 2);
            Engine_ActorRunRepeatedMotion(16, 2);
            Engine_EventWait(20);
            Engine_ActorFaceDirection(13, FACING_EAST, 0);
            Engine_ActorFaceDirection(14, FACING_WEST, 0);
            Engine_ActorFaceDirection(16, 0xb000, 40);
            Engine_ActorSetAnimation(13, 3);
            Engine_ActorSetAnimation(14, 3);
            Engine_ActorSetAnimationAndWait(16, 3);
            Actor_SetSpeed(13, 0x19999, 0xcccc);
            Actor_SetSpeed(14, 0x19999, 0xcccc);
            Actor_SetSpeed(16, 0x19999, 0xcccc);
            Engine_ActorEnableActionCallback(13, KorashiamuIriguchi_ActionTable5);
            Engine_ActorEnableActionCallback(16, KorashiamuIriguchi_ActionTable7);
            Engine_EventWait(20);
            Engine_ActorFaceDirection(15, 0xd000, 0);
            Engine_ActorFaceDirection(17, 0xb000, 0);
            Engine_ActorFaceDirection(0, FACING_NORTHWEST, 0);
            Engine_ActorFaceDirection(12, 0xd000, 0);
            Engine_ActorFaceDirection(18, 0xb000, 0);
            Object_SetActionCallbackAndRefreshById(14, KorashiamuIriguchi_ActionTable6);
            Engine_EventWait(20);
            Engine_ActorFaceDirection(8, FACING_EAST, 0);
            Engine_ActorFaceDirection(11, FACING_WEST, 40);
            Engine_ActorSetAnimation(8, 3);
            Engine_ActorSetAnimationAndWait(11, 3);
            Engine_CameraMoveTo(0x3280000, -1, 0x560000, 1);
            Engine_CameraWaitForMove();
            Engine_ActorRunRepeatedMotion(8, 2);
            FieldScene_CallPairWith10(8);
            Engine_ActorSetSpeed(8, 0x10000, 0x8000);
            Object_GetById(8)->steering_flags &= ~ACTOR_FACES_MOTION;
            Engine_ActorWalkToAndWait(8, 792, 72);
            Engine_EventWait(1);
            Object_GetById(8)->steering_flags |= ACTOR_FACES_MOTION;
            SceneState_ForwardMaskedHalfwordWith10(8, 0);
            Engine_ActorStop(13);
            actor = Object_GetById(13);
            actor->update = NULL;
            actor->timer = 0;
            actor->work = 0;
            actor->velocity_x = 0;
            actor->velocity_y = 0;
            actor->velocity_z = 0;
            actor->target_x = ACTOR_NO_TARGET;
            actor->target_y = ACTOR_NO_TARGET;
            actor->target_z = ACTOR_NO_TARGET;
            Engine_TaskWait(1);
            script = (s32)KorashiamuIriguchi_ActionTable8;
            Engine_ActorEnableActionCallback(15, script);
            Engine_EventWait(20);
            Engine_ActorEnableActionCallback(13, script);
            Engine_EventWait(20);
            Engine_ActorEnableActionCallback(17, script);
            Engine_EventWait(20);
            Engine_ActorEnableActionCallback(14, script);
            Engine_EventWait(20);
            Engine_ActorEnableActionCallback(16, script);
            Engine_EventWait(20);
            Engine_ActorEnableActionCallback(12, script);
            Engine_EventWait(20);
            Engine_ActorEnableActionCallback(18, script);
            Engine_EventWait(60);
            Engine_ActorEnableActionCallback(0, script);
            Engine_EventWait(80);
            Engine_EventRequestExit(66);
        } else {
            gEventWork->message++;
            Engine_EventShowMessage(0x8008, 0);
        }
        Engine_EventEnd();
    }
}
