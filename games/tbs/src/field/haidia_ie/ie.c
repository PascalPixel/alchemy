#include "battle.h"
#include "debug.h"
#include "field.h"
#include "game.h"
#include "graphics.h"
#include "menu.h"
#include "minigame.h"
#include "sound.h"
#include "system.h"
#include "resource.h"
#include "global_cells.h"
#include "haidia.h"
#include "map_scroll.h"
#include "call.h"
#include "text/msg_ids.h"
#include "field_event.h"
#include "field_service.h"








s32 Object_UpdateFacingTowardTarget(struct FacingObject *object)
{
    s32 facing_delta;
    u16 old_facing;
    s32 target_facing;
    struct FacingObject *target;

    target = object->facing_target;
    if (target != NULL) {
        object->facing_flags = (u8)(0xFE & object->facing_flags);
        target_facing = (u16)ArcTan2(target->position_z - object->position_z, target->position_x - object->position_x);
        old_facing = object->facing;
        facing_delta = (s16)(target_facing - old_facing);
        if (facing_delta != 0) {
            if (facing_delta > 0x1000) {
                facing_delta = 0x1000;
            }

            if (facing_delta < -0x1000) {
                facing_delta = -0x1000;
            }
            object->facing = (u16)(old_facing + facing_delta);
        }
    }
    return 1;
}

u8 *HaidiaIe_GetEntrances(void)
{
    return Data_0200afa0;
}

s32 HaidiaIe_GetDefaultResult(void)
{
    return 0;
}

void *SceneData_SelectTableByFlag834(void)
{
    if (Engine_GameFlagIsSet(0x834) != 0) {
        return Data_0200b144;
    }
    return Data_0200b108;
}

void *SceneData_SelectTableByFlags834And87a(void)
{
    if (Engine_GameFlagIsSet(0x834) != 0) {
        return Data_0200b380;
    }
    if (gGameState.entrance == 12) {
        return Data_0200b560;
    }
    if (Engine_GameFlagIsSet(0x87a) != 0) {
        return Data_0200b7d0;
    }
    return Data_0200b170;
}

void Scene_CheckPsynergyStone(void)
{
    Engine_EventBegin();
    Engine_MessageShowCentered((s32)MsgHaidiaCheckedThePsynergyStone, 1);
    Engine_AudioPlayCue(126);
    BattleParty_ApplyDrain(0x3e7, 0);
    Engine_EventWait(10);
    Engine_MessageShowCentered((s32)MsgHaidiaPartyPpRestored, 1);
    UiWork_FinalizePendingCore();
    Engine_GameFlagClear(322);
    Engine_EventEnd();
}

void *SceneData_SelectTableByFlags87a_815_834(void)
{
    if (Engine_GameFlagIsSet(0x87a) != 0) {
        return Data_0200bcec;
    }
    if (Engine_GameFlagIsSet(FLAG_PARTY_LEFT_VALE) != 0) {
        return Data_0200bb3c;
    }
    if (gGameState.entrance == 12) {
        return Data_0200bb30;
    }
    if (Engine_GameFlagIsSet(0x834) != 0) {
        return Data_0200ba64;
    }
    return Data_0200b938;
}

void Villager_AskAboutMeditation(void)
{
    Engine_EventBegin();
    Engine_EventSetMessage((s32)MsgHaidiaMeditateOnMtAlephDaily);
    Engine_ActorFaceEachOther(23, ACTOR_PARTY_LEADER, 2);
    Engine_EventAskYesNo(23, 0);
    Engine_EventEnd();
}

void Villager_RecallThreeYearsAgo(void)
{
    Engine_EventBegin();
    Engine_EventSetMessage((s32)MsgHaidiaADifficultTimeThreeYears);
    Engine_ActorFaceEachOther(24, ACTOR_PARTY_LEADER, 2);
    Engine_EventAskYesNo(24, 0);
    Engine_EventEnd();
}

void Villager_AskAboutTheTravelers(void)
{
    Engine_EventBegin();
    Engine_EventSetMessage((s32)MsgHaidiaDidTheTravelersMeetThe);
    Engine_ActorFaceEachOther(15, ACTOR_PARTY_LEADER, 2);
    Engine_EventAskYesNo(15, 0);
    Engine_EventEnd();
}

void Villager_ShowOffPsynergy(void)
{
    u32 i;
    s32 message;
    s32 shakes;
    s32 *origin;

    Engine_EventBegin();
    if (Engine_GameFlagIsSet(FLAG_PARTY_LEFT_VALE) != 0) {
        message = (s32)MsgHaidiaArentWorriedCrossing;
        Engine_EventSetMessage(message);
        if (Engine_GameFlagIsSet(2) != 0) {
            gEventWork->message++;
        }
        if (Engine_GameFlagIsSet(3) != 0) {
            gEventWork->message++;
        }
        Engine_EventOpenMessage(17, 0);
        if (Engine_EventChooseYesNo(0, 0) == 0) {
            Engine_EventSetMessage(message + 3);
        } else {
            Engine_EventSetMessage(message + 4);
        }
        Engine_EventShowMessage(17, 0);
    } else {
        origin = ((struct MapScrollWork *)gMapWork[0])->origin;
        Engine_EventSetMessage((s32)MsgHaidiaShownNewAbility);
        Engine_ActorFaceEachOther(17, ACTOR_PARTY_LEADER, 0);
        Engine_EventAskYesNo(17, 0);
        Engine_EventWait(20);
        Engine_ActorStartRepeatedMotion(17, 2);
        Engine_EventWait(15);
        SceneState_ApplyPair140And0();
        shakes = 0;
        for (i = 0; i < 40; i++) {
            OverlayObject_UpdateOnFrameBit1((s32)Object_GetById(17));
            Engine_TaskWait(1);
        }
        Engine_TaskAddCallback((s32)FieldScene_RunStep17, 0xc80);
        Engine_AudioPlayCue(107);
        for (i = 0; i != 180; i++) {
            if (Math_RemainderUnsigned(i, 10) == 0) {
                if ((1 & shakes) != 0) {
                    *origin -= 0x10000;
                } else {
                    *origin += 0x10000;
                }
                shakes = shakes + 1;
            }
            Engine_EventWait(1);
        }
        Engine_AudioPlayCue(0x121);
        Scheduler_RemoveCallback((s32)FieldScene_RunStep17);
        Engine_TaskWait(1);
        HaidiaIe_CleanupSceneObjects();
        Engine_ActorSetChildValue(17, 0);
        Engine_EventWait(40);
        Engine_EventSetMessage((s32)MsgHaidiaBeholdPowerPsynergy);
        Engine_EventShowMessage(17, 0);
    }
    Engine_EventEnd();
}

void SceneDialogue_RunFlagGatedMessageStep(void)
{
    Engine_EventBegin();
    if (Engine_GameFlagIsSet(0x87a) != 0) {
        Engine_EventSetMessage((s32)MsgHaidiaAnythingInterestingOnYourTrip);
        Engine_EventOpenMessage(15, 0);
        if (Engine_EventChooseYesNo(0, 0) == 1) {
            Engine_EventShowMessage(15, 0);
        } else {
            struct EventWork *work = gEventWork;

            work->message = work->message + 1;
            Engine_EventAskYesNo(15, 0);
        }
    } else if (Engine_GameFlagIsSet(FLAG_PARTY_LEFT_VALE) != 0) {
        Engine_EventSetMessage((s32)MsgHaidiaYouSawTheWiseOne);
        Engine_EventAskYesNo(11, 0);
    } else {
        Engine_EventSetMessage((s32)MsgHaidiaCanIUsePsynergy);
        Engine_EventAskYesNo(11, 0);
    }
    Engine_EventEnd();
}

void Scene_StoneFellOnTheHut(void)
{
    Engine_EventBegin();
    Engine_ActorSetAnimation(26, 1);
    Engine_ActorFaceActor(26, ACTOR_PARTY_LEADER, 20);
    Engine_ActorFaceActor(26, 21, 40);
    Engine_EventSetMessage((s32)MsgHaidiaTheStoneFellOnThe);
    Event_SayThenWait(26, 20);
    Engine_CameraSetSpeed(0x19999, 0x3333);
    Engine_CameraMoveTo(0x1510000, -1, 0x1100000, 1);
    Engine_EventWait(20);
    Engine_ActorRunRepeatedMotion(26, 2);
    Engine_EventWait(20);
    Engine_ActorFaceActor(26, ACTOR_PARTY_LEADER, 10);
    Event_SayThenWait(26, 40);
    Engine_ActorEnableActionCallback(26, 2);
    Engine_EventEnd();
}

void FieldScene_RunMiddleAuxiliarySequence(void)
{
    u32 i;
    s32 p8;
    struct FieldActor *rec8;
    s32 record;
    s32 v2;

    Engine_EventBegin();
    Actor_WalkToAndWait(ACTOR_PARTY_LEADER, 82, 0x2f8);
    Engine_ActorFaceEachOther(15, ACTOR_PARTY_LEADER, 30);
    Engine_EventSetMessage((s32)MsgHaidiaIHaveSomePsynergyLeft);
    Event_SayThenWait(15, 20);
    SceneActor_SetPairZeroAndValue(15, 0xa000, 20);
    Actor_SetAttachedEffect(15, 0x102);
    Engine_EventWait(20);
    SceneState_ApplyPair140And0();
    for (i = 0; i < 40; i++) {
        OverlayObject_UpdateOnFrameBit1(Object_GetById(15));
        Engine_TaskWait(1);
    }
    FieldScheduler_AddOrUpdateCallback((s32)FieldScene_RunStep15, 0xc80);
    Engine_TaskAddCallback((s32)FieldScene_RunStep20, 0xc80);
    Actor_FaceDirection(ACTOR_PARTY_LEADER, 0xa000, 10);
    rec8 = Object_GetById(20);
    v2 = rec8->motion_flags;
    rec8->motion_flags = 0;
    p8 = v2;
    for (i = 0; i < 40; i++) {
        rec8->y.fixed += 0x1800;
        Engine_TaskWait(1);
    }
    rec8->motion_flags = p8;
    Scheduler_RemoveCallback((s32)FieldScene_RunStep15);
    Scheduler_RemoveCallback((s32)FieldScene_RunStep20);
    Engine_TaskWait(1);
    Event_PlayCue(161);
    Actor_SetChildValue(15, 0);
    Actor_SetChildValue(20, 0);
    Engine_EventWait(40);
    HaidiaIe_CleanupSceneObjects();
    Engine_ActorFaceEachOther(ACTOR_PARTY_LEADER, 15, 30);
    Event_ShowMessage(15, 0);
    Engine_EventEnd();
}

void Villager_PointTheWay(void)
{
    Engine_EventBegin();
    Engine_ActorFaceEachOther(16, ACTOR_PARTY_LEADER, 10);
    if (Engine_GameFlagIsSet(0x840) != 0) {
        Engine_EventSetMessage((s32)MsgHaidiaSureHelp);
        Engine_EventShowMessage(16, 0);
    } else {
        Engine_EventSetMessage((s32)MsgHaidiaWentOffWay);
        Engine_EventShowMessage(16, 0);
    }
    Engine_EventEnd();
}

void Villager_WelcomeBack(void)
{
    u32 i;
    s32 record;

    Engine_EventBegin();
    if (GameFlag_IsSet(0x302) != 0) {
        Engine_EventSetMessage((s32)MsgHaidiaThisIsVale);
    } else {
        Engine_EventSetMessage((s32)MsgHaidiaYouCameBackHome);
        GameFlag_Set(0x302);
    }
    Event_ShowMessage(11, 0);
    Engine_EventEnd();
}

void Scene_PsynergyStoneIsGone(void)
{
    struct Obj *p = Object_GetById(21);
    Engine_EventBegin();
    p->f38 = 0x80000000;
    p->f3c = 0x80000000;
    p->f40 = 0x80000000;
    Engine_ActorSetAnimation(21, 1);
    Engine_ActorStop(21);
    Engine_ActorShowEmote(21, 256, 40);
    p->f06 = 0xb000;
    Engine_EventWait(20);
    Engine_ActorStartRepeatedMotion(21, 2);
    Engine_EventSetMessage((s32)MsgHaidiaThePsynergyStoneIsGone);
    Engine_EventShowMessageAndWait(21, 0, 40);
    Engine_ActorFaceActor(21, ACTOR_PARTY_LEADER, 20);
    Engine_ActorStartRepeatedMotion(21, 2);
    Engine_EventShowMessage(21, 0);
    Engine_GameFlagSet(0x306);
    Engine_ActorStop(21);
    Engine_TaskWait(1);
    Engine_ActorEnableActionCallback(21, Data_0200ae34);
    Engine_EventEnd();
}

void HaidiaIe_RequestBackdropExit(s32 no)
{
    struct EventWork *p;
    if (Engine_GameFlagIsSet(0x834) != 0) {
        BattleFx_SetBlock30ValuesMaxZero();
    }
    p = gEventWork;
    p->start_transition = SCENE_TRANSITION(TRANSITION_BACKDROP_FADE, 0);
    p->transition_frames = 16;
    Engine_EventRequestExit(no);
}

void FieldScene_RunSupplementalSequenceOne(void)
{
    Event_PlayCue(158);
    Map_AnimateCells((s32)&Value_0200beb4, 44, 7);
    Actor_WalkTo(ACTOR_PARTY_LEADER, 248, 0x117);
    HaidiaIe_RequestBackdropExit(1);
}

void FieldScene_RunSupplementalSequenceTwo(void)
{
    Event_PlayCue(188);
    Map_CopyCellsTo(0, 63, 51, 8, 2, 2);
    Engine_TaskWait(10);
    Map_CopyCellsTo(2, 63, 51, 8, 2, 2);
    Engine_TaskWait(10);
    Actor_WalkToAndWait(ACTOR_PARTY_LEADER, 352, 306);
    Engine_ActorSetSpritePriority(ACTOR_PARTY_LEADER, 3);
    Actor_WalkToAndWait(ACTOR_PARTY_LEADER, 352, 296);
    HaidiaIe_RequestBackdropExit(2);
}

void FieldScene_RunSupplementalSequenceThree(void)
{
    Event_PlayCue(158);
    Map_AnimateCells((s32)&Value_0200beb4, 43, 15);
    Actor_WalkTo(ACTOR_PARTY_LEADER, 230, 0x197);
    HaidiaIe_RequestBackdropExit(3);
}

void FieldScene_RunSupplementalSequenceFour(void)
{
    Event_PlayCue(158);
    Map_AnimateCells((s32)&Value_0200beb4, 52, 18);
    Actor_WalkTo(ACTOR_PARTY_LEADER, 374, 0x1a3);
    HaidiaIe_RequestBackdropExit(4);
}

void FieldScene_RunSupplementalSequenceFive(void)
{
    Event_PlayCue(158);
    Map_AnimateCells((s32)&Value_0200beb4, 41, 32);
    Actor_WalkTo(ACTOR_PARTY_LEADER, 200, 0x222);
    HaidiaIe_RequestBackdropExit(5);
}

void FieldScene_RunSupplementalSequenceSix(void)
{
    Event_PlayCue(158);
    Map_AnimateCells((s32)&Value_0200beb4, 35, 36);
    Actor_WalkTo(ACTOR_PARTY_LEADER, 102, 0x263);
    HaidiaIe_RequestBackdropExit(6);
}

void FieldScene_RunSupplementalSequenceSeven(void)
{
    Event_PlayCue(158);
    Map_AnimateCells((s32)&Value_0200beb4, 51, 39);
    Actor_WalkTo(ACTOR_PARTY_LEADER, 358, 0x29e);
    HaidiaIe_RequestBackdropExit(7);
}

void FieldScene_RunStep7BThen8(void)
{
    Engine_AudioPlayCue(123);
    HaidiaIe_RequestBackdropExit(8);
}

void SceneState_ApplyFlag815Branch(void)
{
    if (Engine_GameFlagIsSet(FLAG_PARTY_LEFT_VALE) != 0) {
        Engine_AudioPlayCue(123);
        HaidiaIe_RequestBackdropExit(10);
    }
}

void SceneState_ApplyFlag90b(void)
{
    Engine_GameFlagSet(0x90b);
}

void SceneState_ApplyFlag90c(void)
{
    Engine_GameFlagSet(0x90c);
}

void SceneState_ApplyFlag90d(void)
{
    Engine_GameFlagSet(0x90d);
}

s32 HaidiaIe_RestoreEntryState(void)
{
    extern u8 HaidiaIe_FaceTargetActions[];
    void HaidiaIe_RunActorPairEffectScene();
    void SceneState_SetValues352_365_2116_2117_40();
    void Scene_RunExtendedActorSequence();

    u32 i;
    s32 record;
    s32 v5;

    if (Engine_GameFlagIsSet(0x90b) != 0) {
        Engine_ActorSetPosition(8, 0, 0);
    }
    if (Engine_GameFlagIsSet(0x90c) != 0) {
        Engine_ActorSetPosition(9, 0, 0);
    }
    if (Engine_GameFlagIsSet(0x90d) != 0) {
        Engine_ActorSetPosition(10, 0, 0);
    }
    switch (gGameState.entrance) {
    case 98:
        Engine_GameFlagSet(32);
        Engine_EventRequestExit(50);
        return 0;
    case 99:
        SceneState_SetValues352_365_2116_2117_40();
        return 0;
    case 97:
        HaidiaIe_RunActorPairEffectScene();
        return 0;
    }
    v5 = 192;
    record = Object_GetById(8);
    *(s32 *)(record + 28) = (v5 << 9);
    record = Object_GetById(9);
    *(s32 *)(record + 28) = (v5 << 9);
    record = Object_GetById(10);
    *(s32 *)(record + 28) = (v5 << 9);
    if (Engine_GameFlagIsSet(0x87a) != 0) {
        Engine_MapCopyCellsTo(97, 2, 80, 5, 2, 2);
        Map_CopyCellsTo(42, 53, 42, 54, 3, 1);
        Engine_MapRedraw();
        Engine_TaskWait(1);
    } else {
        if (Engine_GameFlagIsSet(0x834) != 0) {
            FieldFx_StartGroundParticles();
            BattleFx_StartTwelveFrameBlend();
            Call6(Engine_MapCopyCellAttributes, 21, 38, 1, 1, 18, 41);
            record = Engine_GameFlagIsSet(0x840);
            if (record == 0) {
                goto L_02000aaa;
            }
            Engine_ActorSetPosition(17, 0, 0);
            Engine_ActorSetPosition(18, 0, 0);
            Call3(Object_SetTargetAndCallback, 19, 0x10000, (s32)HaidiaIe_FaceTargetActions);
        } else {
            if (Engine_GameFlagIsSet(0x815) != 0) {
                Actor_SetPosition(16, 0xb40000, 0x2380000);
                Engine_MapCopyCellsTo(92, 2, 80, 5, 2, 2);
                Map_CopyCellsTo(42, 53, 42, 54, 3, 1);
                Engine_MapRedraw();
                Engine_TaskWait(1);
            }
        }
        L_02000aaa:;
        if (gGameState.entrance == 12) {
            Scene_RunExtendedActorSequence();
        } else {
            if (Engine_GameFlagIsSet(0x834) != 0) {
                record = Object_GetById(20);
                *(s32 *)(record + 24) = 0x4ccc;
                *(s32 *)(record + 28) = 0x4ccc;
                record = Object_GetById(20);
                Engine_ActorSetSpriteFlags(record, 0);
                record = Object_GetById(21);
                *(s32 *)(record + 24) = 0x9999;
                *(s32 *)(record + 28) = 0x9999;
                Engine_ActorSetAnimation(13, 5);
            } else {
                if (Engine_GameFlagIsSet(0x815) != 0) {
                    Actor_SetPosition(21, 0x14b0000, 0xf90000);
                    record = Object_GetById(21);
                    Engine_ActorSetSpriteFlags(record, 0);
                }
            }
            if (Engine_GameFlagIsSet(0x840) != 0) {
                Engine_ActorSetPosition(26, 0, 0);
                Engine_ActorSetPosition(22, 0, 0);
            }
            if (gGameState.entrance == 19) {
                HaidiaIe_RunActorPairEffectScene();
            } else {
                if (Engine_GameFlagIsSet(0x834) != 0) {
                    record = Engine_GameFlagIsSet(0x842);
                    if (record == 0) {
                        goto L_02000b6a;
                    }
                    HaidiaIe_RunScriptScene();
                } else {
                    L_02000b6a:;
                    if (Engine_GameFlagIsSet(0x834) != 0) {
                        Engine_EventOpenScreen();
                        Engine_EventWaitForScreen();
                        BattleFx_SetBlock30Values128One();
                    }
                }
            }
        }
    }
    L_02000b80:;
    return 0;
}

void FieldScene_RunElderAidEvent(void)
{
    extern u8 HaidiaIe_FaceTargetActions[];

    struct FieldActor *leader;
    s32 skip_reply = 0;
    s32 unk;

    if (Engine_GameFlagIsSet(0x834) != 0 && Engine_GameFlagIsSet(0x840) == 0) {
        Engine_EventBegin();
        Camera_SetSpeed(0x19999, 0x3333);
        Engine_CameraMoveTo(0xc50000, -1, 0x3000000, 1);
        BattleFx_CommitObjectPositionAndWait();
        Engine_EventSetMessage((s32)MsgHaidiaGoAidElders);
        Engine_ActorRunRepeatedMotion(19, 2);
        Event_ShowMessageAndWait(0x4013, 0, 10);
        Actor_SetSpeed(0, 0x10000, 0x8000);
        Actor_SetSpeed(25, 0x10000, 0x8000);
        Actor_WalkToAndWait(0, 179, 0x315);
        leader = Object_GetById(ACTOR_PARTY_LEADER);
        if (leader != NULL) {
            Engine_ActorSetPosition(25, leader->x.fixed, leader->z.fixed);
        }
        Actor_WalkToAndWait(25, 179, 0x324);
        Engine_ActorFaceEachOther(0, 25, 40);
        Engine_ActorFaceDirection(0, 0, 0);
        Engine_ActorFaceDirection(25, 0, 0);
        Engine_ActorSetAnimation(17, 3);
        Engine_ActorSetAnimationAndWait(18, 3);
        Engine_ActorFaceEachOther(17, 18, 0);
        Engine_EventWait(20);
        Engine_ActorStartRepeatedMotion(17, 1);
        Engine_EventShowMessageAndWait(0x4011, 0, 10);
        Engine_ActorSetAnimation(18, 3);
        Engine_EventShowMessageAndWait(18, 0, 10);
        Engine_ActorFaceDirection(17, 0, 0);
        Actor_FaceDirection(18, 0xf000, 10);
        Engine_ActorSetAnimationAndWait(19, 3);
        Event_ShowMessageAndWait(0x4013, 0, 10);
#if defined(TBS_EDITION_ES) || defined(TBS_EDITION_IT)
        Engine_ActorSetSpritePriority(18, 1);
#endif
        Actor_SetSpeed(17, 0x19999, 0xcccc);
        Actor_SetSpeed(18, 0x19999, 0xcccc);
        Engine_ActorEnableActionCallback(17, (s32)HaidiaIe_Actor17Actions);
        Engine_EventWait(20);
#if defined(TBS_EDITION_ES) || defined(TBS_EDITION_IT)
        Object_GetById(18)->priority_flags |= 1;
#endif
        Engine_ActorEnableActionCallback(18, (s32)HaidiaIe_Actor17Actions);
        Actor_FaceDirection(0, 0xc000, 0);
        Actor_FaceDirection(25, 0xc000, 60);
        Engine_ActorEnableActionCallback(0, (s32)HaidiaIe_LeaderStepActions);
        Object_SetActionCallbackAndRefreshById(25, (s32)HaidiaIe_Actor25StepActions);
        Engine_EventWait(20);
        Engine_ActorFaceDirection(0, 0, 0);
        Engine_ActorFaceDirection(25, 0, 10);
        Engine_EventShowMessage(25, 0);
        Actor_FaceDirection(19, 0x8000, 0);
        SceneActor_SetPairZeroAndValue(26, 0x6000, 20);
        Engine_ActorRunRepeatedMotion(26, 2);
        Event_SayThenWait(26, 10);
        Engine_ActorSetAnimation(0, 3);
        Engine_ActorSetAnimationAndWait(25, 3);
        Engine_EventWait(20);
        Engine_ActorRunRepeatedMotion(19, 2);
        Engine_EventOpenMessage(0x4013, 0);
        if (Engine_EventChooseYesNo(0, 0) == 1) {
            skip_reply = 1;
            Engine_ActorSetAnimation(19, 4);
        } else {
            Engine_ActorSetAnimation(19, 3);
            gEventWork->message++;
        }
        Event_ShowMessage(0x4013, 0);
        if (skip_reply != 0) {
            gEventWork->message++;
        }
        unk = 0x4000;
        SceneActor_SetPairZeroAndValue(22, 0x4000, 30);
        Engine_EventShowMessage(22, 0);
        Actor_ShowEmote(19, 0x100, 0);
        Actor_ShowEmote(26, 0x100, 0);
        Actor_ShowEmote(0, 0x100, 0);
        Actor_ShowEmote(25, 0x100, 40);
        Actor_FaceDirection(19, 0xa000, 0);
        Actor_FaceDirection(26, 0xa000, 0);
        Actor_FaceDirection(0, 0xe000, 0);
        SceneActor_SetPairZeroAndValue(25, 0xe000, 10);
        Engine_CameraSetSpeed(0x13333, 0x2666);
        Camera_MoveTo(0xd70000, -1, 0x2f60000, 1);
        BattleFx_CommitObjectPositionAndWait();
        Engine_CameraSetSpeed(0xcccc, 0x1999);
        Camera_MoveTo(0xcd0000, -1, 0x30a0000, 1);
        Engine_ActorEnableActionCallback(22, (s32)HaidiaIe_Actor22Actions);
        Object_RefreshSelectorById(22);
        /* FAKEMATCH: the void result is discarded; Call3 changes argument allocation. */
        Value3(SceneActor_SetPairZeroAndValue, 22, 0x2000, 60);
        Engine_ActorRunRepeatedMotion(19, 2);
        Event_SayThenWait(19, 10);
        Engine_ActorSetAnimationAndWait(22, 3);
        Event_SayThenWait(22, 20);
        Engine_ActorSetAnimationAndWait(19, 3);
        Engine_EventWait(10);
        SceneActor_SetPairZeroAndValue(19, unk, 30);
        Event_SayThenWait(unk + 19, 10);
        SceneActor_SetPairZeroAndValue(26, 0xe000, 30);
        Engine_ActorSetAnimationAndWait(26, 3);
        SceneActor_SetPairZeroAndValue(19, 0x8000, 30);
        Engine_ActorRunRepeatedMotion(19, 2);
        Event_SayThenWait(unk + 19, 10);
        Engine_ActorFaceEachOther(ACTOR_PARTY_LEADER, 25, 40);
        Engine_ActorFaceDirection(ACTOR_PARTY_LEADER, 0, 0);
        SceneActor_SetPairZeroAndValue(25, 0, 20);
        SceneActor_SetPairZeroAndValue(26, 0x8000, 30);
        Engine_ActorSetAnimationAndWait(26, 3);
        Event_SayThenWait(26, 30);
        SceneActor_SetPairZeroAndValue(26, 0xc000, 30);
        Engine_ActorSetAnimationAndWait(26, 3);
        Engine_ActorSetAnimationAndWait(22, 3);
        Engine_ActorSetAnimation(25, 2);
        leader = Object_GetById(ACTOR_PARTY_LEADER);
        if (leader != NULL) {
            Engine_ActorSetDestination(25, leader->x.part.pixel, leader->z.part.pixel);
        }
        Engine_ActorWaitForMove(25);
        Engine_ActorSetPosition(25, 0, 0);
        Engine_ActorSetAnimation(26, 2);
        leader = Object_GetById(ACTOR_PARTY_LEADER);
        if (leader != NULL) {
            Engine_ActorSetDestination(26, leader->x.part.pixel, leader->z.part.pixel);
        }
        Engine_ActorWaitForMove(26);
        Engine_ActorSetPosition(26, 0, 0);
        Engine_ActorSetAnimation(22, 2);
        leader = Object_GetById(ACTOR_PARTY_LEADER);
        if (leader != NULL) {
            Engine_ActorSetDestination(22, leader->x.part.pixel, leader->z.part.pixel);
        }
        Engine_ActorWaitForMove(22);
        Engine_ActorSetPosition(22, 0, 0);
        Call3(Object_SetTargetAndCallback, 19, 0x10000, (s32)HaidiaIe_FaceTargetActions);
        Engine_GameFlagSet(0x840);
        Engine_EventEnd();
    }
}

void HaidiaIe_RunScriptScene(void)
{
    u8 *rec7;

    rec7 = Battle_GetWorkObject1e0();
    Engine_EventBegin();
    Camera_MoveTo(-1, -1, -1, 0);
    Engine_TaskWait(1);
    Engine_CameraMoveTo(0x400000, 0x900000, 0x15e0000, 0);
    Engine_MapRedraw();
    Engine_TaskWait(1);
    Event_PrepareObjectAndApplyValue(1, 0);
    Engine_EventOpenScreen();
    Engine_AudioPlayCue(17);
    BattleFx_SetBlock30Values128One();
    Engine_ActorSetPosition(23, 0x690000, 0x10b0000);
    Engine_TaskWait(1);
    Actor_SetSpeed(0, 0x13333, 0x9999);
    Engine_ActorWalkToAndWait(0, 93, 0x157);
    Engine_EventSetMessage((s32)MsgHaidiaOnlyTwoSurvived);
    Engine_EventShowMessage(23, 0);
    Engine_AudioPlayCue(61);
    rec7[85] = 0;
    Camera_SetSpeed(0x30000, 0x6000);
    Engine_CameraMoveTo(0x6d0000, 0xb00000, 0x1190000, 1);
    BattleFx_CommitObjectPositionAndWait();
    Engine_EventWait(40);
    Actor_SetPosition(24, 0x870000, 0xb10000);
    Actor_SetSpeed(24, 0xcccc, 0x6666);
    Actor_WalkTo(24, 126, 0x102);
    Engine_EventWait(40);
    Engine_ActorFaceDirection(23, 0xd000, 0);
    Engine_ActorWaitForMove(24);
    Engine_ActorSetAnimation(24, 1);
    SceneActor_SetPairZeroAndValue(24, 0x7000, 10);
    Engine_ActorSetAnimationAndWait(23, 3);
    Engine_ActorSetAnimationAndWait(24, 4);
    Event_ShowMessage(0x2018, 0);
    Engine_ActorRunRepeatedMotion(23, 2);
    Event_SayThenWait(0x8017, 30);
    SceneActor_SetPairZeroAndValue(24, 0xb000, 20);
    Event_SayThenWait(0x2018, 10);
    SceneActor_SetPairZeroAndValue(23, 0xb000, 40);
    Engine_EventShowMessage(0x8017, 0);
    Engine_ActorSetAnimationAndWait(24, 4);
    Engine_EventShowMessage(0x2018, 0);
    SceneActor_SetPairZeroAndValue(23, 0xf000, 10);
    Engine_ActorRunRepeatedMotion(23, 2);
    Engine_EventShowMessage(0x8017, 0);
    SceneActor_SetPairZeroAndValue(24, 0x6000, 20);
    Engine_ActorSetAnimationAndWait(24, 3);
    Engine_EventWait(20);
    Event_SayThenWait(0x2018, 20);
    FieldScene_RunGroupChoreography();
    Engine_EventEnd();
}

void FieldScene_RunGroupChoreography(void)
{
    extern u8 HaidiaIe_FaceTargetActions[];

    struct FieldActor *record;
    s32 walk_speed;
    s32 turn_speed;
    s32 approach_speed;

    Engine_ActorSetChildValue(25, 15);
    record = Object_GetById(25);
    Engine_ActorSetSpriteFlags(record, 0);
    Actor_SetPosition(25, 0, 0x14b0000);
    Engine_TaskWait(1);
    Event_ShowMessage(0x1019, 0);
    Actor_ShowEmote(23, 0x100, 0);
    Actor_ShowEmote(24, 0x100, 40);
    Engine_ActorSetPosition(25, 0, 0);
    Actor_FaceDirection(23, 0x5000, 0);
    walk_speed = 0x5000;
    SceneActor_SetPairZeroAndValue(24, walk_speed, 40);
    Engine_CameraSetSpeed(0x18000, 0x3000);
    Engine_CameraMoveTo(0x590000, 0xb00000, 0x1390000, 1);
    BattleFx_CommitObjectPositionAndWait();
    Engine_EventWait(40);
    Actor_FaceDirection(23, 0xe000, 0);
    SceneActor_SetPairZeroAndValue(24, 0x7000, 40);
    Engine_CameraSetSpeed(0xcccc, 0x1999);
    Engine_CameraMoveTo(0x640000, 0x900000, 0x14d0000, 1);
    Actor_SetSpeed(23, 0x10000, 0x8000);
    Actor_SetSpeed(24, 0x10000, 0x8000);
    Actor_WalkTo(23, 105, 0x149);
    Engine_EventWait(10);
    Engine_ActorWalkTo(24, 124, 0x149);
    Engine_ActorWaitForMove(23);
    Engine_ActorSetAnimation(23, 1);
    Engine_ActorFaceDirection(23, walk_speed, 0);
    Engine_ActorWaitForMove(24);
    Engine_ActorSetAnimation(24, 1);
    Engine_ActorFaceDirection(24, walk_speed, 0);
    Engine_ActorSetChildValue(25, 0);
    record = Object_GetById(25);
    Engine_ActorSetSpriteFlags(record, 1);
    Actor_SetPosition(25, 0, 0x14b0000);
    Actor_SetSpeed(25, 0x13333, 0x9999);
    Engine_ActorWalkToAndWait(25, 37, 0x153);
    Engine_EventWait(20);
    Engine_ActorSetAnimationAndWait(23, 3);
    Engine_EventOpenMessage(23, 0);
    Actor_ShowEmote(25, 0x101, 0);
    approach_speed = 0xd000;
    SceneActor_SetPairZeroAndValue(0, approach_speed, 10);
    Engine_EventChooseYesNo(0, 0);
    Engine_EventWait(40);
    Engine_ActorFaceDirection(23, 0, 0);
    turn_speed = 0x8000;
    SceneActor_SetPairZeroAndValue(24, turn_speed, 20);
    Engine_ActorRunRepeatedMotion(25, 2);
    Event_SayThenWait(0x1019, 10);
    Call3(Object_SetTargetAndCallback, 0, 0x10019, (s32)HaidiaIe_FaceTargetActions);
    Actor_WalkToAndWait(25, 93, 0x169);
    SceneActor_SetPairZeroAndValue(25, approach_speed, 40);
    Event_SayThenWait(25, 20);
    Engine_ActorStop(0);
    Engine_ActorFaceDirection(23, 0, 0);
    SceneActor_SetPairZeroAndValue(24, turn_speed, 15);
    Engine_ActorSetAnimation(23, 3);
    Engine_ActorSetAnimationAndWait(24, 3);
    Engine_ActorFaceDirection(23, walk_speed, 0);
    SceneActor_SetPairZeroAndValue(24, walk_speed, 30);
    Engine_ActorSetAnimationAndWait(24, 4);
    Engine_EventShowMessage(0x2018, 0);
    SceneActor_SetPairZeroAndValue(0, approach_speed, 30);
    SceneActor_SetPairZeroAndValue(0, 0x4000, 40);
    Engine_ActorRunRepeatedMotion(23, 2);
    Engine_ActorSetAnimationAndWait(23, 3);
    Event_SayThenWait(23, 20);
    Actor_SetAttachedEffect(0, 0x102);
    Engine_ActorSetAttachedEffect(25, 0x102);
    Engine_EventWait(80);
    Engine_ActorEnableActionCallback(24, (s32)HaidiaIe_Actor24Actions);
    Engine_EventWait(6);
    Engine_ActorEnableActionCallback(23, (s32)HaidiaIe_Actor23Actions);
    Engine_EventWait(20);
    Engine_ActorEnableActionCallback(0, (s32)HaidiaIe_LeaderActions);
    Engine_EventWait(6);
    Object_SetActionCallbackAndRefreshById(25, (s32)HaidiaIe_Actor25Actions);
    /* FAKEMATCH: the do/while loads the linked scene after the store. */
    do {
        gGameState.battle_start = 2;
    } while (0);
    {
        const void *message = &SceneId_HaidiaIe;

        Party_SetFields1ceAnd1d0(message, 19);
        Event_SetDefeatDestination(message, 19);
    }
    BattleFx_SetWeightedResult(12, 4);
    Engine_GameFlagSet(0x11a);
}

void HaidiaIe_RunActorPairEffectScene(void)
{

    u32 i;
    struct FieldActor *record;
    s32 v5;

    Engine_AudioPlayCue(17);
    Engine_EventBegin();
    Camera_MoveTo(-1, -1, -1, 0);
    Engine_TaskWait(1);
    Engine_CameraMoveTo(0x400000, 0x900000, 0x15e0000, 0);
    Engine_MapRedraw();
    Engine_TaskWait(1);
    Actor_SetPosition(0, 0x300000, 0x15a0000);
    Actor_SetPosition(25, 0x4e0000, 0x1660000);
    Actor_SetPosition(23, 0x670000, 0x1560000);
    Actor_SetPosition(24, 0x700000, 0x1680000);
    Actor_FaceDirection(23, 0x8000, 0);
    Actor_FaceDirection(24, 0x8000, 0);
    Engine_ActorSetAnimation(0, 16);
    record = Object_GetById(0);
    record->scale_x = -0x10000;
    record = Object_GetById(0);
    Engine_ActorSetSpriteFlags(record, 0);
    Engine_ActorSetAnimation(25, 7);
    record = Object_GetById(25);
    {
        u8 *motion = (u8 *)record->sprite;
        u16 shown = 0x1555;

        *(u16 *)(motion + 30) = shown;
    }
    record = Object_GetById(25);
    Engine_ActorSetSpriteFlags(record, 0);
    {
        struct EventWork *work = gEventWork;

        work->start_transition = SCENE_TRANSITION(TRANSITION_BACKDROP_FADE, 0);
    }
    Engine_EventOpenScreen();
    BattleFx_SetBlock30Values128One();
    Engine_EventWaitForScreen();
    Engine_EventWait(80);
    Actor_FaceDirection(23, 0x3000, 0);
    Actor_FaceDirection(24, 0xc000, 40);
    Engine_ActorSetAnimationAndWait(23, 3);
    Engine_EventWait(20);
    Engine_ActorSetAnimationAndWait(24, 3);
    Actor_FaceDirection(23, 0x8000, 10);
    Actor_FaceDirection(24, 0x8000, 10);
    Engine_ActorSetSpritePriority(0, 3);
    Engine_ActorSetSpritePriority(25, 3);
    Actor_SetSpeed(23, 0x26666, 0x13333);
    v5 = 128;
    record = Object_GetById(23);
    record->bounce = 0x28f;
    record->gravity = (v5 << 8);
    Engine_ActorEnableActionCallback(23, (s32)Data_0200aa48);
    Engine_EventWait(24);
    Engine_ActorSetSpeed(24, 0x26666, 0x13333);
    record = Object_GetById(24);
    record->bounce = 0x28f;
    record->gravity = (v5 << 8);
    Object_SetActionCallbackAndRefreshById(24, (s32)Data_0200ab2c);
    Engine_EventWait(40);
    BattleFx_SetBlock30ValuesMaxZero();
    BattleFx_SetBlock30Values12Zero();
    Engine_TaskWait(20);
    BattleFx_SetBlock30Values12Zero();
    Engine_TaskWait(60);
    BattleFx_SetBlock30Values12Zero();
    Engine_TaskWait(20);
    BattleFx_SetBlock30ValuesMaxZero();
    Engine_EventWait(40);
    gEventWork->transition_frames = 120;
    Engine_EventCloseScreen();
    Engine_EventWaitForScreen();
    Engine_GameFlagClear(0x834);
    Engine_EventRequestExit(9);
    Engine_EventEnd();
}

void Scene_RunExtendedActorSequence(void)
{
#ifdef MODERN
    // modern GCC holds one type for a name in a file
    extern const u8 HaidiaIe_FaceTargetActions_modern[] __asm__("HaidiaIe_FaceTargetActions");
#define HaidiaIe_FaceTargetActions HaidiaIe_FaceTargetActions_modern
#else
    extern const u8 HaidiaIe_FaceTargetActions[];
#endif

    struct FieldActor *record;
    struct EventWork *work;
    const u8 *base5_200ac00;
    s32 v5;
    const u8 *base5_200ac90;
    s32 v6;
    const u8 *base5_200adf0;

    Owner_RefreshActiveRatios(1);
    Engine_EventBegin();
    Map_CopyCellsTo(42, 53, 42, 54, 3, 1);
    Camera_MoveTo(0xb40000, 0x100000, 0x26a0000, 0);
    Engine_MapRedraw();
    Engine_TaskWait(1);
    record = Object_GetById(22);
    Engine_ActorSetSpriteFlags((s32)record, 0);
    record = Object_GetById(23);
    Engine_ActorSetSpriteFlags((s32)record, 0);
    record = Object_GetById(24);
    Engine_ActorSetSpriteFlags((s32)record, 0);
    record = Object_GetById(25);
    Engine_ActorSetSpriteFlags((s32)record, 0);
    record = Object_GetById(26);
    Engine_ActorSetSpriteFlags((s32)record, 0);
    record = Object_GetById(29);
    Engine_ActorSetSpriteFlags((s32)record, 0);
    Engine_ActorSetSpritePriority(0, 1);
    Engine_ActorSetSpritePriority(1, 1);
    Engine_ActorSetSpritePriority(17, 1);
    Engine_ActorSetSpritePriority(16, 1);
    Engine_ActorSetSpritePriority(15, 1);
    Actor_SetPosition(0, 0xd00000, 0x32e0000);
    Engine_EventOpenScreen();
    Engine_EventWaitForScreen();
    Engine_EventWait(80);
    Actor_ShowEmote(12, 0x101, 40);
    SceneActor_SetPairZeroAndValue(12, 0x7000, 20);
    Event_SetMessage(MSGNO(MsgHaidiaWhysEveryoneHanging));
    Event_SayThenWait(12, 10);
    Actor_ShowEmote(11, 0x102, 20);
    SceneActor_SetPairZeroAndValue(11, 0x1000, 10);
    Event_SayThenWait(11, 10);
    Engine_ActorSetAnimationAndWait(12, 3);
    Engine_EventWait(10);
    Engine_ActorRunRepeatedMotion(11, 2);
    Event_SayThenWait(11, 10);
    Actor_ShowEmote(12, 0x100, 40);
    Actor_SetSpeed(12, 0xcccc, 0x6666);
    Actor_WalkToAndWait(12, 184, 0x26a);
    SceneActor_SetPairZeroAndValue(12, 0x3000, 60);
    Event_SayThenWait(12, 20);
    Actor_SetSpeed(11, 0x10000, 0x8000);
    Actor_WalkToAndWait(11, 168, 0x26a);
    Call3(SceneActor_SetPairZeroAndValue, 11, 0xf000, 10);
    Engine_ActorSetAnimation(11, 4);
    Event_SayThenWait(11, 20);
    SceneActor_SetPairZeroAndValue(12, 0x7000, 10);
    Engine_ActorRunRepeatedMotion(12, 1);
    Engine_EventShowMessage(12, 0);
    Engine_EventWait(10);
    Actor_SetSpeed(30, 0x26666, 0x13333);
    Actor_SetPosition(30, 0x6e0000, 0x2e80000);
    Engine_TaskWait(2);
    Engine_ActorSetAnimation(30, 3);
    Engine_ActorEnableActionCallback(30, (s32)Data_0200ac14);
    Engine_EventWait(40);
    base5_200ac00 = HaidiaIe_FaceTargetActions;
    Call3(Object_SetTargetAndCallback, 11, 0x1001e, (s32)(base5_200ac00));
    Call3(Object_SetTargetAndCallback, 12, 0x1001e, (s32)(base5_200ac00));
    Object_RefreshSelectorById(30);
    Engine_ActorStop(11);
    Engine_ActorStop(12);
    Engine_EventWait(60);
    Actor_ShowEmote(11, 0x105, 0);
    Actor_ShowEmote(12, 0x105, 120);
    Engine_ActorFaceDirection(11, 0x1000, 0);
    SceneActor_SetPairZeroAndValue(12, 0x7000, 80);
    SceneActor_SetPairZeroAndValue(11, 0x5000, 40);
    SceneActor_SetPairZeroAndValue(11, 0x1000, 20);
    Engine_ActorSetAnimationAndWait(11, 3);
    Engine_EventWait(20);
    SceneActor_SetPairZeroAndValue(12, 0x5000, 60);
    SceneActor_SetPairZeroAndValue(12, 0x3000, 40);
    SceneActor_SetPairZeroAndValue(12, 0x5000, 60);
    Actor_ShowEmote(12, 0x101, 80);
    Engine_ActorFaceDirection(11, 0x3000, 0);
    Actor_WalkToAndWait(12, 184, 0x276);
    Engine_EventWait(20);
    SceneActor_SetPairZeroAndValue(12, 0x3000, 20);
    SceneActor_SetPairZeroAndValue(12, 0x5000, 20);
    SceneActor_SetPairZeroAndValue(12, 0x3000, 20);
    Actor_ShowEmote(12, 0x101, 40);
    Actor_ShowEmote(11, 0x101, 40);
    Actor_WalkToAndWait(11, 168, 0x276);
    Engine_EventWait(20);
    SceneActor_SetPairZeroAndValue(11, 0x3000, 40);
    SceneActor_SetPairZeroAndValue(11, 0x5000, 40);
    SceneActor_SetPairZeroAndValue(11, 0x3000, 40);
    Actor_ShowEmote(11, 0x101, 40);
    SceneActor_SetPairZeroAndValue(11, 0x1000, 10);
    Event_SayThenWait(11, 20);
    Engine_ActorSetAnimationAndWait(12, 3);
    Event_SayThenWait(12, 10);
    Actor_ShowEmote(11, 0x100, 20);
    SceneActor_SetPairZeroAndValue(11, 0x5000, 20);
    SceneActor_SetPairZeroAndValue(11, 0x3000, 20);
    SceneActor_SetPairZeroAndValue(11, 0x5000, 20);
    SceneActor_SetPairZeroAndValue(11, 0x5000, 60);
    Engine_ActorSetAnimationAndWait(11, 3);
    Event_SayThenWait(11, 10);
    {
        struct FieldActor *actor = Object_GetById(30);

        if (actor != NULL) {
            Engine_ActorSetPosition(31, actor->x.fixed, actor->z.fixed);
        }
    }
    v5 = 254;
    Engine_TaskWait(2);
    Object_GetById(30)->priority_flags &= v5;
    Object_GetById(31)->priority_flags &= v5;
    Engine_ActorSetSpritePriority(30, 2);
    Engine_ActorSetSpritePriority(31, 2);
    Actor_SetSpeed(31, 0x39999, 0x1cccc);
    Engine_ActorSetAnimation(31, 2);
    base5_200ac90 = Data_0200ac90;
    Engine_ActorEnableActionCallback(31, base5_200ac90);
    Engine_EventWait(20);
    Engine_ActorSetAnimation(30, 3);
    Actor_SetSpeed(30, 0x4cccc, 0x26666);
    Object_SetActionCallbackAndRefreshById(30, base5_200ac90);
    Engine_EventWait(60);
    Engine_ActorRunRepeatedMotion(12, 2);
    SceneActor_SetPairZeroAndValue(12, 0x7000, 10);
    Event_SayThenWait(12, 10);
    Engine_ActorRunRepeatedMotion(11, 1);
    Engine_EventWait(20);
    SceneActor_SetPairZeroAndValue(11, 0x1000, 10);
    Engine_ActorSetAnimationAndWait(11, 3);
    Event_SayThenWait(11, 20);
    Engine_ActorSetAnimation(12, 3);
    Engine_EventWait(10);
    Engine_ActorSetAnimationAndWait(11, 3);
    Engine_EventWait(20);
    Actor_SetSpeed(11, 0x26666, 0x13333);
    Actor_SetSpeed(12, 0x26666, 0x13333);
    Engine_ActorEnableActionCallback(11, (s32)Data_0200acf8);
    Engine_EventWait(10);
    Camera_SetSpeed(0x26666, 0x4ccc);
    v6 = 0;
    Battle_GetWorkObject1e0()->motion_flags = v6;
    Camera_MoveTo(0xd70000, 0x100000, 0x3210000, 1);
    Engine_EventWait(10);
    Engine_ActorEnableActionCallback(12, (s32)Data_0200ad74);
    Object_RefreshSelectorById(12);
    SceneActor_SetPairZeroAndValue(12, 0x3000, 120);
    Engine_ActorRunRepeatedMotion(13, 2);
    Engine_EventWait(20);
    Event_SayThenWait(13, 20);
    Engine_ActorFaceDirection(0, 0, 0);
    SceneActor_SetPairZeroAndValue(1, 0x9000, 20);
    SceneActor_SetPairZeroAndValue(0, 0xc000, 10);
    SceneActor_SetPairZeroAndValue(1, 0xb000, 10);
    Engine_ActorSetAnimation(0, 3);
    Engine_EventWait(10);
    Engine_ActorSetAnimationAndWait(1, 3);
    Engine_EventWait(40);
    Engine_ActorRunRepeatedMotion(16, 2);
    Engine_EventWait(20);
    Event_SayThenWait(16, 10);
    Actor_SetSpeed(16, 0x10000, 0x8000);
    Actor_WalkToAndWait(16, 216, 0x320);
    Actor_FaceDirection(16, 0x4000, 0);
    Engine_PartyGiveItem(180, 0);
    Actor_WalkToAndWait(16, 0x108, 0x320);
    Actor_FaceDirection(16, 0x6000, 0);
    Actor_ShowEmote(1, 0x102, 40);
    Call3(SceneActor_SetPairZeroAndValue, 1, 0xf000, 10);
    Event_SayThenWait(1, 10);
    Engine_ActorSetAnimationAndWait(17, 4);
    Event_SayThenWait(17, 10);
    Engine_ActorFaceDirection(1, 0x1000, 0);
    Actor_ShowEmote(1, 0x103, 20);
    ObjectMotion_Launch(1, 4, 60);
    Engine_ActorRunRepeatedMotion(14, 2);
    Engine_EventWait(20);
    SceneActor_SetPairZeroAndValue(14, 0xd000, 10);
    Event_SayThenWait(14, 60);
    Actor_ShowEmote(1, 0x102, 0);
    Actor_ShowEmote(16, 0x102, 0);
    Actor_ShowEmote(17, 0x102, 0);
    Actor_ShowEmote(18, 0x102, 0);
    Actor_ShowEmote(19, 0x102, 80);
    Actor_ShowEmote(17, 0x100, 0);
    Event_SayThenWait(17, 60);
    Object_LinkObjectAndSetCallback(0, 17);
    Object_LinkObjectAndSetCallback(1, 17);
    Actor_WalkToAndWait(17, 216, 0x320);
    Actor_FaceDirection(17, 0x4000, 0);
    Event_SayThenWait(17, 60);
    Engine_PartyGiveItem(207, 0);
    Engine_ActorStop(0);
    Engine_ActorStop(1);
    Actor_WalkToAndWait(17, 0x110, 0x330);
    Actor_FaceDirection(17, 0x8000, 0);
    Engine_ActorRunRepeatedMotion(1, 2);
    SceneActor_SetPairZeroAndValue(1, 0x9000, 10);
    Event_SayThenWait(1, 10);
    Engine_ActorFaceDirection(14, 0x3000, 0);
    SceneActor_SetPairZeroAndValue(0, 0, 10);
    Actor_ShowEmote(0, 0x101, 60);
    Engine_ActorRunRepeatedMotion(16, 1);
    Event_SayThenWait(16, 10);
    Call3(SceneActor_SetPairZeroAndValue, 1, 0xf000, 10);
    Actor_ShowEmote(1, 0x101, 20);
    Engine_ActorSetAnimationAndWait(16, 4);
    Event_SayThenWait(16, 10);
    Engine_ActorSetAnimationAndWait(18, 3);
    Engine_EventShowMessage(18, 0);
    Engine_ActorFaceDirection(1, 0xd000, 0);
    Engine_ActorSetAnimation(18, 4);
    Engine_EventShowMessage(18, 0);
    Engine_ActorStartRepeatedMotion(18, 3);
    Engine_EventShowMessage(18, 0);
    Engine_ActorSetAnimationAndWait(16, 3);
    Engine_ActorSetAnimation(19, 3);
    Engine_ActorSetAnimation(17, 3);
    Engine_EventWait(10);
    Engine_ActorSetAnimation(24, 3);
    Engine_ActorSetAnimation(18, 3);
    Engine_ActorSetAnimation(27, 3);
    Engine_EventWait(10);
    Engine_ActorSetAnimation(28, 3);
    Engine_EventWait(10);
    Engine_ActorSetAnimation(25, 3);
    Engine_ActorSetAnimation(20, 3);
    Engine_ActorSetAnimationAndWait(21, 3);
    ObjectMotion_Launch(15, 2, 10);
    ObjectMotion_Launch(15, 4, 40);
    Event_SayThenWait(15, 10);
    Engine_ActorFaceDirection(1, 0xb000, 0);
    SceneActor_SetPairZeroAndValue(0, 0xc000, 20);
    SceneActor_SetPairZeroAndValue(15, 0xd000, 10);
    Event_SayThenWait(15, 10);
    SceneActor_SetPairZeroAndValue(15, 0x9000, 20);
    SceneActor_SetPairZeroAndValue(15, 0x5000, 10);
    Engine_ActorSetAnimation(11, 3);
    Engine_ActorSetAnimation(14, 3);
    Engine_ActorSetAnimation(17, 3);
    Engine_ActorSetAnimation(20, 3);
    Engine_ActorSetAnimation(23, 3);
    Engine_ActorSetAnimation(26, 3);
    Engine_ActorSetAnimation(29, 3);
    Engine_EventWait(10);
    Engine_ActorSetAnimation(12, 3);
    Engine_ActorSetAnimation(15, 3);
    Engine_ActorSetAnimation(18, 3);
    Engine_ActorSetAnimation(21, 3);
    Engine_ActorSetAnimation(24, 3);
    Engine_ActorSetAnimation(27, 3);
    Engine_EventWait(10);
    Engine_ActorSetAnimation(13, 3);
    Engine_ActorSetAnimation(16, 3);
    Engine_ActorSetAnimation(19, 3);
    Engine_ActorSetAnimation(22, 3);
    Engine_ActorSetAnimation(25, 3);
    Engine_ActorSetAnimationAndWait(28, 3);
    Engine_EventWait(80);
    ObjectMotion_Launch(11, 4, 0);
    ObjectMotion_Launch(14, 4, 0);
    ObjectMotion_Launch(17, 4, 0);
    ObjectMotion_Launch(20, 4, 0);
    ObjectMotion_Launch(23, 4, 0);
    ObjectMotion_Launch(26, 4, 0);
    ObjectMotion_Launch(29, 4, 0);
    ObjectMotion_Launch(12, 4, 0);
    ObjectMotion_Launch(15, 4, 0);
    ObjectMotion_Launch(18, 4, 0);
    ObjectMotion_Launch(21, 4, 0);
    ObjectMotion_Launch(24, 4, 0);
    ObjectMotion_Launch(27, 4, 0);
    ObjectMotion_Launch(13, 4, 0);
    ObjectMotion_Launch(16, 4, 0);
    ObjectMotion_Launch(19, 4, 0);
    ObjectMotion_Launch(22, 4, 0);
    ObjectMotion_Launch(25, 4, 0);
    ObjectMotion_Launch(28, 4, 0);
    Engine_MessageShowCentered(MSGNO(MsgHaidiaFarewell), 1);
    v5 = 1;
    Engine_EventWait(80);
    Object_GetById(0)->priority_flags |= v5;
    {
        struct FieldActor *actor = Object_GetById(1);
        u8 value = (u8)(v5 | actor->priority_flags);

        actor->priority_flags = value;
    }
    Actor_ShowEmote(0, 0x102, 0);
    Actor_ShowEmote(1, 0x102, 80);
    SceneActor_SetPairZeroAndValue(0, 0x4000, 10);
    SceneActor_SetPairZeroAndValue(1, 0x5000, 20);
    Actor_SetSpeed(0, 0xcccc, 0x6666);
    base5_200adf0 = Data_0200adf0;
    Engine_ActorEnableActionCallback(0, base5_200adf0);
    Engine_EventWait(20);
    Camera_SetSpeed(0x6666, 0xccc);
    Engine_CameraMoveTo(0xd80000, 0x100000, 0x3890000, 1);
    Engine_EventWait(20);
    Actor_SetSpeed(1, 0xcccc, 0x6666);
    Engine_ActorEnableActionCallback(1, base5_200adf0);
    Engine_EventWait(60);
    work = gEventWork;
    work->start_transition = SCENE_TRANSITION(TRANSITION_BACKDROP_FADE, 0);
    work->transition_frames = 60;
    Engine_EventCloseScreen();
    Engine_EventWaitForScreen();
    Engine_ActorStop(0);
    Engine_ActorStop(1);
    Engine_EventRequestExit(10);
    Engine_EventEnd();
}
#ifdef MODERN
#undef HaidiaIe_FaceTargetActions
#endif

void Event_SayThenWait(s32 speaker, s32 frames)
{
    Engine_EventShowMessage(speaker, 0);
    Engine_EventWait(frames);
}

void SceneActor_SetPairZeroAndValue(s32 a, s32 b, s32 c)
{
    Engine_ActorFaceDirection(a, b, 0);
    Engine_EventWait(c);
}

void OverlayObject_UpdateOnFrameBit1(s32 obj)
{
    if ((gFrameCount & 2) != 0) {
        Engine_ObjectSetPartPalettes(obj, 7);
    } else {
        Engine_ObjectSetPartPalettes(obj, 0);
    }
    if ((gFrameCount & 15) == 0) {
        HaidiaIe_SpawnEffectPair(obj);
    }
}

void SceneEffect_UpdateByFrameBits(s32 no)
{
    if ((gFrameCount & 1) != 0) {
        s32 t = Math_RemainderUnsigned(gFrameCount >> 1, 6);
        Engine_ObjectSetPartPalettes(no, t);
    }
    if ((gFrameCount & 15) == 0) {
        HaidiaIe_SpawnEffectPair(no);
    }
}

void SceneEffect_UpdateByFrameBit(s32 no)
{
    if ((gFrameCount & 1) != 0) {
        s32 t = Math_RemainderUnsigned(gFrameCount >> 1, 6);
        Engine_ObjectSetPartPalettes(no, t);
    }
}

void SceneEffect_AnimateVerticalPositive(struct SceneVerticalEffect *effect)
{
    struct VerticalEffectAnchor *anchor;
    s32 frame;
    s32 amplitude;
    s32 offset;

    anchor = effect->anchor;
    effect->frame = effect->frame + 1;
    frame = (s16)effect->frame;
    if (frame > 31) {
        Engine_ObjectDispatchRelease(effect);
        return;
    }
    amplitude = Engine_MathSin(frame << 10);
    effect->amplitude_x = amplitude;
    effect->amplitude_y = amplitude;
    effect->x = anchor->x;
    offset = 0x10000;
    effect->y = effect->y + offset;
    offset = offset - amplitude;
    effect->z = anchor->z + (offset * 4 + offset) + 0x80000;
}

void SceneEffect_AnimateVerticalNegative(struct SceneVerticalEffect *effect)
{
    struct VerticalEffectAnchor *anchor;
    s32 frame;
    s32 amplitude;
    s32 offset;

    anchor = effect->anchor;
    effect->frame = effect->frame + 1;
    frame = (s16)effect->frame;
    if (frame > 31) {
        Engine_ObjectDispatchRelease(effect);
        return;
    }
    amplitude = Engine_MathSin(frame << 10);
    effect->amplitude_x = amplitude;
    effect->amplitude_y = -amplitude;
    effect->x = anchor->x;
    offset = 0x10000;
    effect->y = effect->y + offset;
    offset = offset - amplitude;
    effect->z = anchor->z - (offset * 4 + offset) + 0x100000;
}

void HaidiaIe_SpawnEffectPair(union PairObject *parent)
{
    union PairObject *pair[2];
    union PairObject *child;
    struct PairSprite *part;
    struct FieldSprite *sprite;
    struct PairWork *work = gEffectWork;
    s32 i;

    Engine_AudioPlayCue(131);
    for (i = 0; i < 2; ++i) {
        child = (union PairObject *)Engine_ObjectCreate(26,
            parent->object.actor.x.fixed, parent->object.actor.y.fixed,
            parent->object.actor.z.fixed);
        pair[i] = child;
        if (child != NULL) {
            child->words[5] = parent->words[5];
            part = (struct PairSprite *)child->object.actor.sprite;
            child->object.actor.motion_flags = 0;
            child->object.effect.spin = 0;
            child->link.parent = parent;
            if (part != NULL) {
                sprite = &part->sprite;
                AnimationObjects_SelectAnimation(sprite, 0);
                sprite->flags = 0;
                Resource_ResetEntry(sprite->vram_block);
                sprite->vram_block = work->vram_block;
                sprite->unknown_1d_0 = 1;
                sprite->tile = (gVramBlockCache[sprite->vram_block].offset >> 5) & 0x3ff;
                sprite->full_color = 0;
                sprite->shape = 1;
                ((struct WorldMapOam *)sprite)->size = 2;
                part->detail->field_16 = 0;
            }
        }
    }
    {
        union PairObject *p = pair[0];
        struct FieldSprite *sp = p->object.actor.sprite;

        p->object.actor.update = (void (*)(union FieldObject *))SceneEffect_AnimateVerticalNegative;
        sp->priority = 2;
    }
    {
        struct FieldActor *p = &pair[1]->object.actor;
        struct FieldSprite *sp = p->sprite;

        sp->priority = 2;
        p->update = (void (*)(union FieldObject *))SceneEffect_AnimateVerticalPositive;
        p->priority_flags = 2;
    }
}

void SceneState_ApplyPair140And0(void)
{
    Engine_PsynergyBegin(140, 0);
}

void HaidiaIe_CleanupSceneObjects(void)
{
    BattleEffect_CleanupSceneObjects();
}

void FieldScene_RunStep15(void)
{
    SceneEffect_UpdateByFrameBits((s32)Object_GetById(15));
}

void FieldScene_RunStep17(void)
{
    SceneEffect_UpdateByFrameBits((s32)Object_GetById(17));
}

void FieldScene_RunStep20(void)
{
    SceneEffect_UpdateByFrameBit((s32)Object_GetById(20));
}

void SceneState_SetValues352_365_2116_2117_40(void)
{
    Engine_GameFlagSet(352);
    Engine_GameFlagSet(0x16d);
#if defined(TBS_EDITION_FR) || defined(TBS_EDITION_ES) || defined(TBS_EDITION_IT)
    Engine_GameFlagSet(0x830);
    Engine_GameFlagSet(0x831);
    Engine_GameFlagSet(0x832);
    Engine_GameFlagSet(0x833);
    Engine_GameFlagSet(0x835);
    Engine_GameFlagSet(0x836);
    Engine_GameFlagSet(0x837);
    Engine_GameFlagSet(0x838);
    Engine_GameFlagSet(0x839);
    Engine_GameFlagSet(0x83a);
    Engine_GameFlagSet(0x840);
    Engine_GameFlagSet(0x841);
    Engine_GameFlagSet(0x842);
    Engine_GameFlagSet(0x806);
    Engine_GameFlagSet(0x807);
    Engine_GameFlagSet(0x808);
    Engine_GameFlagSet(0x800);
    Engine_GameFlagSet(0x801);
    Engine_GameFlagSet(0x823);
    Engine_GameFlagSet(0x802);
    Engine_GameFlagSet(0xf01);
    Engine_GameFlagSet(0x81a);
    Engine_GameFlagSet(0x804);
    Engine_GameFlagSet(0xf02);
    Engine_GameFlagSet(0x821);
    Engine_GameFlagSet(0x825);
    Engine_GameFlagSet(0x809);
    Engine_GameFlagSet(0x80a);
    Engine_GameFlagSet(0x818);
    Engine_GameFlagSet(0x80b);
    Engine_GameFlagSet(0x80c);
    Engine_GameFlagSet(0x80d);
    Engine_GameFlagSet(0x80e);
    Engine_GameFlagSet(0x80f);
    Engine_GameFlagSet(0x813);
    Engine_GameFlagSet(0x810);
    Engine_GameFlagSet(0x811);
    Engine_GameFlagSet(0x819);
    Engine_GameFlagSet(0x83b);
    Engine_GameFlagSet(0x83c);
    Engine_GameFlagSet(0x83d);
    Engine_GameFlagSet(0x83e);
    Engine_GameFlagSet(0x83f);
    Engine_GameFlagSet(0x814);
    Engine_GameFlagSet(0x879);
    Engine_GameFlagSet(0x815);
    Engine_GameFlagSet(0x81b);
    Engine_GameFlagSet(0x81d);
    Engine_GameFlagSet(0x87a);
#else
    Engine_GameFlagSet(0x844);
    Engine_GameFlagSet(0x845);
#endif
    Engine_EventRequestExit(40);
}
