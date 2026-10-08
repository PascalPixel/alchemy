#include "BATTLE.H"
#include "DEBUG.H"
#include "FIELD.H"
#include "GAME.H"
#include "GRAPHICS.H"
#include "MENU.H"
#include "MINIGAME.H"
#include "SOUND.H"
#include "SYSTEM.H"
#include "EDITION.H"
#include "TYPES.H"
#include "FIELD_EVENT.H"
#include "ANIMSPR.H"
#include "FIELD_EFFECT.H"
#include "FIELD_SCENE.H"
#include "FACING_OBJECT.H"
#include "CALL.H"
#include "text/MSG_IDS.H"

void ShindenHeya_SpawnOwnerEffect();


extern s16 Data_02000240[];
void Event_CallWithLastActiveObjectId();
void AudioCommand_WaitForCompletion();




extern struct BattleEffectBuffers *Data_03001ed0;

struct ShrineWork {
    u8 unknown_0000[0xe5a];
    u16 colors[3];
    u8 unknown_0e60[0x2a00 - 0xe60];
    u8 lamps[4];
};

void UiText_ShowCenteredMessage(s32 message, s32 a1, s32 a2);

s32 ShindenHeya_MatchLeaderPriority();
void ShindenHeya_CopyActorPose();
void FieldScene_RunPairedActorChoreography();
void ObjectMotion_WaitForAnimationChange();
void ShindenHeya_SpawnActorSpark();

void ShindenHeya_FollowLeaderOffset();

extern

enum FacingGatedMessage {
    MSG_WIELDERS_PSYNERGY_CALLED_ADEPTS_ADEPTS = 0x1035,
    MSG_WE_HAD_IDEA_TRUE_SANCTUM = 0x1138,
    MSG_ROBIN_WILL_ACCEPT_RESPONSIBILITY_FOR = 0x1162,
    MSG_ARE_YOU_SURE = 0x1164,
    MSG_ONCE_STEP_OUTSIDE_VILLAGE_CANNOT = 0x116c,
    MSG_ACCEPT_ROBIN_CANT_MEAN = 0x1171,
    MSG_MY_CONTROL_OVER_PSYNERGY_HAS = 0x119d,
    MSG_DO_FEEL_ANY_CHANGE_IN = 0x119f,
    MSG_WE_WILL_HELP_ANYTIME_AS = 0x1288,
    MSG_HEALER_MUST_WORRIED_ABOUT_NEVER = 0x1289,
    MSG_WONDER_IF_EVER_SEE_OUR = 0x128b,
    MSG_WAS_HAND_FATE_RETURNED_GOLD = 0x1376,
    MSG_WHEN_STRAY_FROM_YOUR_WORLDLY = 0x1377,
    MSG_CHILD_HAS_AWAKENED_OUR_TEACHINGS = 0x1379,
    MSG_AM_STARTING_FEEL_ONLY_BEGINNING = 0x1408,
    MSG_CURSE_MAY_OVER_BUT_WE = 0x171c,
    MSG_CAME_XIAN_FROM_VERY_DISTANT = 0x1823,
    MSG_WAS_AFTER_EERIE_NIGHT_WHEN = 0x190a,
    MSG_SAVED_ALTIN_FROM_MONSTERS_CLEARLY = 0x1951,
    MSG_PATH_SOL_SANCTUM_STILL_CLOSED = 0x1bfc,
    MSG_ROBIN_YOUR_NEW_FRIENDS_ADEPTS = 0x1bfd,
    MSG_MAY_WRONG_BUT_LATELY_THERE = 0x1ce8,
    MSG_POLISHED_GOLD_STATUE_RETURNED_US = 0x1ce9,
    MSG_DIRTY_GOLDEN_STATUE_CLEANED_UP = 0x1ceb
};

#ifndef MODERN // modern GCC keeps the headers' volatile declaration
extern u32 gFrameCount;
#endif

void SceneEffect_StepEllipseOrbit();


s32 UpdateFacingFromResolvedObject(struct FacingObject *object)
{
    struct FacingObject *target;

    target = ResolveFacingObject(object->unknown_64);
    object->facing = CalculateFacingAngle(
        target->position_z - object->position_z,
        target->position_x - object->position_x
    );
    return 0;
}

void *ShindenHeya_GetEntrances(void)
{
    return &ShindenHeya_TableA;
}

s32 SceneData_ReturnZero(void)
{
    return 0;
}

void *ShindenHeya_GetExits(void)
{
    return &ShindenHeya_TableB;
}

void *SceneData_SelectPlacementTableBySubstate(void)
{

    s16 sub;

    {
        s32 off = 450;

        sub = *(s16 *)((u8 *)Data_02000240 + off);
    }
    switch ((s32)sub) {
    case 1:
    case 2:
        return ShindenHeya_PlacementB;

    case 10:
    case 11:
    case 12:
    case 35:
        return ShindenHeya_PlacementC;

    case 20:
    case 21:
        return ShindenHeya_PlacementD;

    case 29:
    case 32:
        return ShindenHeya_PlacementE;

    default:
        return ShindenHeya_PlacementA;
    }
}

u8 *SceneData_SelectTableBySceneIdAndFlags(void)
{
    extern s16 Data_02000240[];

    s16 *tbl = Data_02000240;
    s32 no = tbl[225];

    switch (no) {
    case 10:
    case 12:
        return ShindenHeya_SceneTableB;
    case 11:
        return ShindenHeya_SceneTableC;
    case 20:
    case 21:
    case 50:
        return ShindenHeya_SceneTableD;
    case 32:
        return ShindenHeya_SceneTableI;
    case 29:
        return ShindenHeya_SceneTableG;
    case 35:
        return ShindenHeya_SceneTableH;
    default:
        break;
    }

    if (Engine_GameFlagIsSet(0x87a) != 0) {
        return ShindenHeya_SceneTableF;
    }
    if (Engine_GameFlagIsSet(FLAG_PARTY_LEFT_VALE) != 0) {
        return ShindenHeya_SceneTableE;
    }
    return ShindenHeya_SceneTableA;
}

void FieldScene_RunActorNineFlagDialogueA(void)
{
    Engine_EventBegin();

    if (Engine_GameFlagIsSet(0x855) != 0) {
        Engine_EventSetMessage((s32)MsgShindenWhenStrayFromYourWorldly);
    } else {
        Engine_EventSetMessage((s32)MsgShindenHealerMustWorriedAboutNever);
    }

    if (gGameState.entrance == 11) {
        Engine_EventSetMessage((s32)MsgShindenPolishedGoldStatueReturnedUs);
    }

    Engine_ActorSetAnimation(9, 1);
    Engine_ActorFaceEachOther(9, ACTOR_PARTY_LEADER, 0);
    Engine_EventWait(2);
    Engine_EventShowMessage(9, 0);
    Engine_EventEnd();
}

void FieldScene_RunActorNineFlagDialogueB(void)
{
    void Engine_ActorStop(s32 id);

    Engine_EventBegin();

    if (Engine_GameFlagIsSet(0x855) == 0) {
        Engine_EventSetMessage((s32)MsgShindenWonderIfEverSeeOur);
    } else {
        Engine_EventSetMessage((s32)MsgShindenChildHasAwakenedOurTeachings);
    }

    if (gGameState.entrance == 11) {
        Engine_EventSetMessage((s32)MsgShindenDirtyGoldenStatueCleanedUp);
    }

    Engine_ActorStop(9);
    Engine_ActorSetAnimation(9, 1);
    Engine_EventWait(2);
    Engine_EventShowMessage(9, 0);
    Engine_ActorEnableActionCallback(9, 2);
    Engine_EventEnd();
}

void FieldScene_RunSupplementalSequenceOne(void)
{
    Engine_EventBegin();
    Engine_EventSetMessage((s32)MsgShindenAreYouSure);
    Engine_EventOpenMessage(8, 0);
    if (Engine_EventChooseYesNo(0, 0) == 0) {
        Engine_ActorSetAnimationAndWait(8, 3);
        Engine_EventWait(20);
    } else {
        Engine_EventWait(20);
        Engine_EventOpenMessage(8, 0);
        if (Engine_EventChooseYesNo(0, 0) == 0) {
            Engine_EventWait(20);
            Engine_EventOpenMessage(8, 0);
            if (Engine_EventChooseYesNo(0, 0) == 0) {
                Engine_EventWait(20);
                if (((struct FacingObject *)Object_GetById(8))->facing >= 0xa000 && ((struct FacingObject *)Object_GetById(8))->facing <= 0xe000) {
                    Actor_SetSpeed(8, 0x8000, 0x4000);
                    Engine_ActorFaceDirection(8, 0, 0);
                    Engine_EventWait(10);
                    ((struct FacingObject *)Object_GetById(8))->facing_flags &= ~1;
                    Engine_ActorWalkToAndWait(8, 152, 120);
                    Engine_EventWait(1);
                    ((struct FacingObject *)Object_GetById(8))->facing_flags |= 1;
                    Engine_EventWait(20);
                    Engine_ActorSetAnimationAndWait(8, 3);
                    Engine_EventWait(20);
                    Engine_ActorWalkToAndWait(ACTOR_PARTY_LEADER, 168, 120);
                    Engine_ActorWalkTo(ACTOR_PARTY_LEADER, 192, 168);
                    Engine_EventWait(20);
                    Engine_ActorWalkToAndWait(8, 168, 120);
                    Engine_ActorFaceDirection(8, 0x3000, 0);
                    Engine_ActorWaitForMove(ACTOR_PARTY_LEADER);
                } else {
                    Engine_ActorWalkTo(ACTOR_PARTY_LEADER, 192, 168);
                    Engine_EventWait(20);
                    Engine_ActorFaceDirection(8, 0x3000, 0);
                    Engine_ActorWaitForMove(ACTOR_PARTY_LEADER);
                }
                FieldScene_RunActorUpdateSequence();
                Engine_ColorBufferApplyTarget(0, 0);
                Engine_ColorBufferInterpolate(120);
                Engine_EventWait(120);
                Engine_AudioPlayCue(86);
                AudioCommand_WaitForCompletion();
                Engine_GameFlagSet(0x9f0);
                Engine_EventRequestExit(30);
            }
        }
    }
    Engine_EventEnd();
}

void FieldScene_RunScene378SequenceB(void)
{
    void Engine_ActorStop();

    u32 i;
    s32 record;

    Engine_EventBegin();
#if EDITION_INTERNATIONAL
    Event_CallWithLastActiveObjectId((s32)ShindenHeya_PlacementSequenceB);
    Engine_TaskWait(1);
#endif
    Engine_EventSetMessage((s32)MsgShindenRobinYourNewFriendsAdepts);
    Engine_EventOpenMessage(9, 0);
    if (Engine_EventChooseYesNo(0, 0) == 0) {
        Engine_EventShowMessage(9, 0);
    } else {
        gEventWork->message++;
#if EDITION_INTERNATIONAL
        Event_ShowTwoMessagesAndWait(2, 16, 1, 24, 1, 3, 7, 16, 1, 14, 0);
#else
        Event_ShowTwoMessagesAndWait(1, 16, 1, 24, 1, 3, 7, 16, 1, 14, 0);
#endif
        Engine_EventShowMessage(9, 0);
    }
    Engine_EventEnd();
}

void FieldScene_RunActorTenCountStep(void)
{

    Engine_EventBegin();
    Engine_ActorFaceActor(10, ACTOR_PARTY_LEADER, 0);
    Engine_EventWait(10);
    Engine_EventSetMessage((s32)MsgShindenDoFeelAnyChangeIn);
    Engine_EventOpenMessage(10, 0);

    if (Engine_EventChooseYesNo(0, 0) == 1) {
        (gEventWork->message)++;
    }

    Engine_EventShowMessage(10, 0);
    Engine_EventEnd();
}

s32 ShindenHeya_ApplyEntryState(void)
{
    gEventWork->start_transition = SCENE_TRANSITION(TRANSITION_WINDOW, 9);
    Engine_ColorBufferApplyTarget(0x10000, 0);
    Engine_ColorBufferInterpolate(1);
    Engine_EventWait(1);
    switch (gGameState.entrance) {
    case 10:
    case 11:
    case 12:
        if (Engine_GameFlagIsSet(0x855) != 0) {
            Actor_SetPosition(10, 0xc80000, 0x500000);
        }
        Engine_GameFlagClear(0x12f);
        break;
    case 20:
        SceneState_ApplyTwoRects();
        if (Engine_GameFlagIsSet(0x109) == 0) {
            FieldScene_RunScriptedSceneSequence();
        }
    case 29:
    case 32:
    case 35:
        Engine_GameFlagClear(0x12f);
        break;
    case 21:
        SceneState_ApplyTwoRects();
        Engine_GameFlagSet(0x201);
        if (Engine_GameFlagIsSet(0x109) == 0) {
            FieldScene_RunScriptedSceneSequence();
        }
        Engine_GameFlagClear(0x12f);
        break;
    }
    return 0;
}

void FieldScene_RunActorEightResetSequence(void)
{
    Engine_EventBegin();
    Engine_CameraSetSpeed(0x10000, 0x2000);
    Engine_CameraMoveToActor(1, 1);
    Engine_CameraWaitForMove();
    Engine_EventWait(20);
    Engine_ActorFaceActor(8, ACTOR_PARTY_LEADER, 0);
    Engine_EventWait(10);
    Engine_ActorSetAnimationAndWait(8, 4);
    Engine_EventWait(20);
    Engine_EventSetMessage((s32)MsgShindenOnceStepOutsideVillageCannot);
    Engine_EventShowMessage(8, 0);
    Engine_GameFlagSet(0x200);
    Engine_EventEnd();
}

void ShindenHeya_RunAltarScene(void)
{
    struct ShrineWork *work;
    u8 zero;

    Engine_EventBegin();
    Engine_ActorSetAnimation(0, 0);
    Engine_ActorSetAnimation(1, 0);
    Engine_ActorSetAnimation(11, 0);
    Engine_ActorSetAnimation(12, 0);
    Engine_ActorSetAnimation(8, 0);
    Engine_ActorSetAnimation(9, 0);
    Engine_ActorSetAnimation(10, 0);
    Engine_ColorBufferApplyTarget(0x10002, 0);
    Engine_ColorBufferInterpolate(120);
    Engine_EventWait(180);
    work = *(struct ShrineWork **)&Data_03001ed0;
    work->colors[0] = 0x7c00;
    work->colors[1] = 0x7c00;
    work->colors[2] = 0x7c00;
    /* FAKEMATCH: the u8 zero local preserves the lamp clear's literal-pool load. */
    zero = 0;
    work->lamps[0] = zero;
    work->lamps[1] = 1;
    work->lamps[2] = 1;
    work->lamps[3] = 1;
    Engine_EventWait(1);
    UiText_ShowCenteredMessage((s32)MsgShindenWorldBeganDrifting, 1, 0);
    Engine_ColorBufferApplyTarget(0, 0);
    Engine_ColorBufferInterpolate(120);
    Engine_EventWait(120);
    Engine_EventWait(60);
    if (ShindenHeya_ChooseRestartOption() == 0) {
        Engine_EventEnd();
        Engine_EventRequestExit(20);
    } else {
        Engine_EventEnd();
        Engine_EventRequestExit(50);
    }
}

s32 ShindenHeya_MatchLeaderPriority(struct FieldActor *actor)
{
    actor->sprite->priority = Object_GetById(0)->sprite->priority;
    actor->sprite->second_priority = Object_GetById(0)->sprite->priority;
    return 0;
}

void FieldScene_RunScriptedSceneSequence(void)
{
    u32 i;

    Engine_EventBegin();
    if (GameFlag_IsSet(0x201) != 0) {
        Engine_ActorFaceEachOther(ACTOR_PARTY_LEADER, 8, 0);
        Actor_FaceActor(GERALD, ACTOR_PARTY_LEADER, 0);
        Actor_FaceActor(0xb, ACTOR_PARTY_LEADER, 0);
        Actor_FaceActor(0xc, ACTOR_PARTY_LEADER, 0);
        Actor_FaceActor(9, ACTOR_PARTY_LEADER, 0);
        Actor_FaceActor(0xa, ACTOR_PARTY_LEADER, 0);
        Camera_MoveTo(0xc00000, -1, 0xa00000, 1);
        Engine_CameraWaitForMove();
        gEventWork->start_transition = SCENE_TRANSITION(TRANSITION_BACKDROP_FADE, 0);
        gEventWork->transition_frames = 0x40;
        Engine_EventOpenScreen();
        Engine_EventWaitForScreen();
        Engine_EventWait(0x78);
        goto dialogue;
    }

    ColorBuffer_ApplyTarget(0x10002, 0);
    Engine_ColorBufferInterpolate(1);
    Engine_EventWait(1);
    Camera_MoveTo(0xc00000, -1, 0xa00000, 1);
    Engine_CameraWaitForMove();
    gEventWork->start_transition = SCENE_TRANSITION(TRANSITION_WINDOW, 9);
    Engine_EventOpenScreen();
    Engine_EventWaitForScreen();
    FieldScene_RunPairedActorChoreography();
    ColorBuffer_ApplyTarget(0x10000, 0);
    Engine_ColorBufferInterpolate(0x3c);
    Engine_EventWait(0x64);
    Engine_ActorSetAnimation(ACTOR_PARTY_LEADER, 1);
    Engine_ActorSetAnimation(GERALD, 1);
    Engine_EventWait(0x1e);
    Actor_FaceActor(ACTOR_PARTY_LEADER, 0xc, 0);
    Actor_FaceActor(GERALD, 0xc, 0);
    Engine_EventWait(0x14);
    Engine_ActorEnableActionCallback(8, 1);
    Engine_ActorEnableActionCallback(0xc, 1);
    Actor_SetSpeed(ACTOR_PARTY_LEADER, 0xcccc, 0x6666);
    Actor_SetSpeed(GERALD, 0xcccc, 0x6666);
    Actor_SetSpeed(0xb, 0xcccc, 0x6666);
    Actor_SetSpeed(0xc, 0xcccc, 0x6666);
    Actor_SetSpeed(9, 0xcccc, 0x6666);
    Actor_SetSpeed(0xa, 0xcccc, 0x6666);
    Actor_SetSpeed(8, 0xcccc, 0x6666);
    Engine_EventWait(0x1e);
    Engine_ActorRunRepeatedMotion(0xc, 2);
    Engine_EventSetMessage((s32)MsgShindenWeHadIdeaTrueSanctum);
    Event_ShowMessage(0xc, 0);
    Engine_EventWait(0xa);
    Engine_ActorSetAnimation(ACTOR_PARTY_LEADER, 3);
    Engine_ActorSetAnimationAndWait(GERALD, 3);
    Engine_EventWait(0x1e);
    Engine_ActorSetAnimationAndWait(0xc, 3);
    Engine_EventWait(0x14);
    Engine_ActorRunRepeatedMotion(0xb, 2);
    Engine_EventWait(0x14);
    Actor_FaceActor(ACTOR_PARTY_LEADER, 0xb, 0);
    Actor_FaceActor(GERALD, 0xb, 0);
    Engine_EventWait(0x14);
    Event_OpenMessage(0xb, 0);
    if (Engine_EventChooseYesNo(0, 0) == 0) {
        Engine_EventWait(0x14);
        Engine_ActorSetAnimationAndWait(0xb, 3);
        Engine_EventWait(0x14);
        Event_ShowMessage(0xb, 0);
        gEventWork->message++;
    } else {
        Engine_EventWait(0x14);
        Engine_ActorSetAnimationAndWait(0xb, 4);
        Engine_EventWait(0x14);
        gEventWork->message++;
        Event_ShowMessage(0xb, 0);
    }
    Engine_EventWait(0x14);
    Engine_ActorRunRepeatedMotion(9, 1);
    Engine_EventWait(0x14);
    Actor_FaceActor(ACTOR_PARTY_LEADER, 9, 0);
    Actor_FaceActor(GERALD, 9, 0);
    Actor_FaceActor(0xb, 9, 0);
    Engine_EventWait(0x14);
    Event_OpenMessage(9, 0);
    if (Engine_EventChooseYesNo(0, 0) == 0) {
        Engine_EventWait(0x14);
        Engine_ActorSetAnimationAndWait(9, 3);
        Engine_EventWait(0x14);
        Event_ShowMessage(9, 0);
        gEventWork->message++;
    } else {
        Engine_EventWait(0x14);
        Engine_ActorSetAnimationAndWait(9, 4);
        Engine_EventWait(0x14);
        gEventWork->message++;
        Event_ShowMessage(9, 0);
    }
    Engine_EventWait(0x14);
    Engine_ActorSetAnimation(ACTOR_PARTY_LEADER, 3);
    Engine_ActorSetAnimation(GERALD, 3);
    Engine_ActorSetAnimation(0xb, 3);
    Engine_ActorSetAnimation(0xc, 3);
    Engine_ActorSetAnimation(9, 3);
    Engine_ActorSetAnimationAndWait(0xa, 3);
    Engine_EventWait(0x1e);
    Actor_ShowEmote(0xc, 0x101, 0);
    Engine_EventWait(0x3c);
    Actor_FaceActor(0xc, 8, 0);
    Engine_EventWait(0x14);
    Actor_WalkToAndWait(0xc, 0xe0, 0x78);
    Engine_EventWait(0xa);
    Event_ShowMessage(0xc, 0);
    Engine_EventWait(0x14);
    Actor_FaceActor(ACTOR_PARTY_LEADER, 8, 0);
    Actor_FaceActor(GERALD, 8, 0);
    Actor_FaceActor(0xb, 8, 0);
    Actor_FaceActor(9, 8, 0);
    Actor_FaceActor(0xa, 8, 0);
    Engine_EventWait(0xa);
    Engine_ActorRunRepeatedMotion(0xb, 1);
    Engine_EventWait(0xa);
    Event_ShowMessage(0xb, 0);
    Engine_EventWait(0x1e);
    Engine_ActorRunRepeatedMotion(8, 3);
    Actor_ShowEmote(8, 0x100, 0);
    Engine_EventWait(0x3c);
    Actor_FaceActor(8, 0xc, 0);
    Engine_EventWait(0x14);
    Engine_ActorSetAnimationAndWait(8, 3);
    Engine_EventWait(0xa);
    Event_ShowMessage(8, 0);
    Engine_EventWait(0x14);
    Actor_FaceDirection(0xc, 0x5000, 0);
    Engine_EventWait(0x3c);
    Actor_FaceActor(0xc, 8, 0);
    Engine_EventWait(0x14);
    Engine_ActorSetAnimationAndWait(0xc, 3);
    Engine_EventWait(0xa);
    Event_ShowMessage(0xc, 0);
    Engine_EventWait(0xa);
    Engine_ActorSetAnimationAndWait(8, 3);
    Engine_EventWait(0x14);
    Actor_FaceDirection(8, 0x3000, 0);
    Engine_EventWait(0xa);
    Event_ShowMessage(8, 0);
    Engine_EventWait(0x14);
    Actor_FaceDirection(0xc, 0x5000, 0);
    Engine_EventWait(0x3c);
    Actor_FaceActor(0xc, 8, 0);
    Engine_EventWait(0x32);
    Actor_ShowEmote(0xc, 0x101, 0);
    Engine_EventWait(0x28);
    Actor_ShowEmote(ACTOR_PARTY_LEADER, 0x101, 0);
    Actor_ShowEmote(GERALD, 0x101, 0);
    Actor_ShowEmote(0xb, 0x101, 0);
    Actor_ShowEmote(9, 0x101, 0);
    Actor_ShowEmote(0xa, 0x101, 0);
    Engine_EventWait(0x3c);
    Engine_ActorRunRepeatedMotion(8, 1);
    Engine_EventWait(0xa);
    Event_ShowMessage(8, 0);
    Engine_EventWait(0xa);
    Actor_ShowEmote(ACTOR_PARTY_LEADER, 0x100, 0);
    Actor_ShowEmote(GERALD, 0x100, 0);
    Actor_ShowEmote(0xb, 0x100, 0);
    Actor_ShowEmote(0xc, 0x100, 0);
    Actor_ShowEmote(9, 0x100, 0);
    Actor_ShowEmote(0xa, 0x100, 0);
    Engine_EventWait(0x3c);
    Engine_ActorRunRepeatedMotion(GERALD, 2);
    Engine_EventWait(0xa);
    Event_ShowMessage(GERALD, 0);
    Engine_EventWait(0x14);
    Engine_ActorSetAnimationAndWait(8, 3);
    Engine_EventWait(0xa);
    Event_ShowMessage(8, 0);
    Engine_EventWait(0xa);
    Actor_ShowEmote(0xc, 0x102, 0);
    Engine_ActorStartRepeatedMotion(ACTOR_PARTY_LEADER, 1);
    Engine_ActorStartRepeatedMotion(GERALD, 1);
    Engine_ActorStartRepeatedMotion(0xb, 1);
    Engine_ActorStartRepeatedMotion(9, 1);
    Engine_ActorRunRepeatedMotion(0xa, 1);
    Engine_EventWait(0xa);
    Engine_ActorSetAnimationAndWait(8, 4);
    Engine_EventWait(0xa);
    Event_ShowMessage(8, 0);
    Engine_EventWait(0x1e);
    Actor_FaceActor(0xc, ACTOR_PARTY_LEADER, 0);
    Actor_FaceActor(0xb, ACTOR_PARTY_LEADER, 0);
    Engine_EventWait(0x14);
    Engine_ActorRunRepeatedMotion(ACTOR_PARTY_LEADER, 2);
    Engine_EventWait(0x3c);
    Engine_ActorRunRepeatedMotion(0xc, 2);
    Engine_EventWait(0x14);
    Actor_FaceActor(0xc, 8, 0);
    Engine_EventWait(0x14);
    Event_ShowMessage(0xc, 0);
    Engine_EventWait(0x1e);
    Engine_ActorRunRepeatedMotion(0xb, 2);
    Engine_EventWait(0x14);
    Actor_FaceActor(8, ACTOR_PARTY_LEADER, 0);
    Engine_EventWait(0x14);
    Event_ShowMessage(0xb, 0);
    Engine_EventWait(0x14);
    Engine_EventWait(0x28);
    Engine_ActorRunRepeatedMotion(8, 2);
    Engine_ActorSetAnimation(8, 0);
    Actor_SetChildValue(8, 0x100);
    ShindenHeya_CopyActorPose();
    Audio_PlayCue(0xc4);
    ShindenHeya_SpawnOwnerEffect(8, 0x1200);
    Engine_EventWait(0x20);
    ShindenHeya_SpawnOwnerEffect(8, 0x1200);
    Engine_EventWait(0x10);
    for (i = 0; i < 6; i++) {
        ShindenHeya_SpawnOwnerEffect(8, 0x1200);
        Engine_EventWait(8);
    }
    Engine_EventWait(8);
    ShindenHeya_SpawnOwnerEffect(8, 0x1200);
    Engine_EventWait(0x20);
    ShindenHeya_SpawnOwnerEffect(8, 0x1200);
    Engine_EventWait(0x60);
    Engine_EventWait(0x20);
    Actor_SetChildValue(8, 0);
    Engine_EventWait(0x1e);
    ShindenHeya_ResetActorUpdate();
    Engine_ActorSetAnimation(8, 1);
    Engine_ActorRunRepeatedMotion(8, 2);
    Engine_EventWait(0x14);
    Event_ShowMessage(8, 0);
    Engine_EventWait(0x14);
    Actor_SetAttachedEffect(0xc, 0x102);
    Actor_SetAttachedEffect(ACTOR_PARTY_LEADER, 0x102);
    Actor_SetAttachedEffect(GERALD, 0x102);
    Actor_SetAttachedEffect(0xb, 0x102);
    Actor_SetAttachedEffect(9, 0x102);
    Actor_SetAttachedEffect(0xa, 0x102);
    Engine_EventWait(0x3c);
    Engine_ActorRunRepeatedMotion(0xa, 1);
    Engine_EventWait(0x14);
    Event_ShowMessage(0xa, 0);
    Engine_EventWait(0x14);
    Engine_ActorFaceEachOther(ACTOR_PARTY_LEADER, GERALD, 0);
    Engine_EventWait(0x3c);
    Engine_ActorSetAnimationAndWait(8, 4);
    Engine_EventWait(0x14);
    Actor_FaceActor(ACTOR_PARTY_LEADER, 8, 0);
    Actor_FaceActor(GERALD, 8, 0);
    Engine_EventWait(0x14);
    Event_ShowMessage(8, 0);
    Engine_EventWait(0x1e);
    Engine_ActorFaceEachOther(ACTOR_PARTY_LEADER, GERALD, 0);
    Engine_EventWait(0x14);
    Engine_ActorSetAnimation(ACTOR_PARTY_LEADER, 3);
    Engine_ActorSetAnimation(GERALD, 3);
    Engine_ActorFaceEachOther(9, 0xa, 0);
    Engine_EventWait(0x14);
    Engine_ActorSetAnimation(9, 3);
    Engine_ActorSetAnimation(0xa, 3);
    ObjectMotion_WaitForAnimationChange(0xa);
    Engine_EventWait(0x1e);
    Engine_ActorFaceEachOther(ACTOR_PARTY_LEADER, 0xb, 0);
    Engine_ActorFaceEachOther(GERALD, 0xc, 0);
    Engine_EventWait(0x14);
    Engine_ActorSetAnimation(ACTOR_PARTY_LEADER, 3);
    Engine_ActorSetAnimation(0xb, 3);
    Engine_ActorSetAnimation(GERALD, 3);
    Engine_ActorSetAnimation(0xc, 3);
    ObjectMotion_WaitForAnimationChange(0xc);
    Engine_EventWait(0x3c);
    Engine_ActorRunRepeatedMotion(8, 2);
    Engine_ActorSetAnimation(8, 0);
    Actor_SetChildValue(8, 0x100);
    ShindenHeya_CopyActorPose();
    Audio_PlayCue(0xc4);
    ShindenHeya_SpawnOwnerEffect(8, 0x1200);
    Engine_EventWait(0x20);
    ShindenHeya_SpawnOwnerEffect(8, 0x1200);
    Actor_FaceActor(ACTOR_PARTY_LEADER, 8, 0);
    Actor_FaceActor(GERALD, 8, 0);
    Actor_FaceActor(0xb, 8, 0);
    Actor_FaceActor(0xc, 8, 0);
    Actor_FaceActor(9, 8, 0);
    Actor_FaceActor(0xa, 8, 0);
    Engine_EventWait(0x10);
    Engine_ActorStartRepeatedMotion(ACTOR_PARTY_LEADER, 1);
    Engine_ActorStartRepeatedMotion(GERALD, 1);
    Engine_ActorStartRepeatedMotion(0xb, 1);
    Engine_ActorStartRepeatedMotion(0xc, 1);
    Engine_ActorStartRepeatedMotion(9, 1);
    Engine_ActorStartRepeatedMotion(0xa, 1);
    for (i = 0; i < 6; i++) {
        ShindenHeya_SpawnOwnerEffect(8, 0x1200);
        Engine_EventWait(8);
    }
    Engine_EventWait(8);
    ShindenHeya_SpawnOwnerEffect(8, 0x1200);
    Engine_EventWait(0x20);
    ShindenHeya_SpawnOwnerEffect(8, 0x1200);
    Engine_EventWait(0x80);
    Actor_SetChildValue(8, 0);
    Engine_EventWait(0x1e);
    ShindenHeya_ResetActorUpdate();
    Engine_ActorSetAnimation(8, 1);
    Engine_ActorRunRepeatedMotion(8, 2);
    Engine_EventWait(0x1e);
    Actor_ShowEmote(0xc, 0x105, 0);
    Engine_EventWait(0x14);
    Engine_ActorFaceEachOther(9, 0xa, 0);
    Engine_EventWait(0x28);
    Actor_FaceActor(9, 8, 0);
    Actor_FaceActor(0xa, 8, 0);
    Event_ShowMessage(0xc, 0);
    Engine_EventWait(0x14);
    Engine_ActorRunRepeatedMotion(0xb, 2);
    Engine_EventWait(0x14);
    Event_ShowMessage(0xb, 0);
    Engine_EventWait(0x14);
    Engine_ActorStartRepeatedMotion(ACTOR_PARTY_LEADER, 1);
    Engine_ActorStartRepeatedMotion(GERALD, 1);
    Engine_ActorFaceEachOther(ACTOR_PARTY_LEADER, GERALD, 0);
    Engine_ActorStartRepeatedMotion(9, 1);
    Engine_ActorStartRepeatedMotion(0xa, 1);
    Engine_ActorFaceEachOther(9, 0xa, 0);
    Actor_SetAttachedEffect(0xc, 0x102);
    Engine_EventWait(0x3c);
    Event_ShowMessage(8, 0);
    Engine_EventWait(0x14);
    Actor_FaceActor(ACTOR_PARTY_LEADER, 8, 0);
    Actor_FaceActor(GERALD, 8, 0);
    Actor_FaceActor(9, 8, 0);
    Actor_FaceActor(0xa, 8, 0);
    Engine_EventWait(0x14);
    Event_ShowMessage(GERALD, 0);
    Engine_EventWait(0x14);
    Engine_ActorSetAnimationAndWait(8, 3);
    Engine_EventWait(0x14);
    Event_ShowMessage(8, 0);
    Engine_EventWait(0x14);
    Actor_ShowEmote(ACTOR_PARTY_LEADER, 0x101, 0);
    Actor_ShowEmote(GERALD, 0x101, 0);
    Actor_ShowEmote(0xb, 0x101, 0);
    Actor_ShowEmote(0xc, 0x101, 0);
    Actor_ShowEmote(9, 0x101, 0);
    Actor_ShowEmote(0xa, 0x101, 0);
    Engine_EventWait(0x3c);
    Engine_ActorSetAnimationAndWait(8, 4);
    Engine_EventWait(0x14);
    Event_ShowMessage(8, 0);
    Engine_EventWait(0x14);
    Engine_ActorSetAnimation(ACTOR_PARTY_LEADER, 3);
    Engine_ActorSetAnimation(GERALD, 3);
    Engine_ActorSetAnimation(0xb, 3);
    Engine_ActorSetAnimation(0xc, 3);
    Engine_ActorSetAnimation(9, 3);
    Engine_ActorSetAnimation(0xa, 3);
    ObjectMotion_WaitForAnimationChange(0xa);
    Engine_EventWait(0x14);
    Engine_ActorRunRepeatedMotion(0xc, 1);
    Engine_EventWait(0x14);
    Event_ShowMessage(0xc, 0);
    Engine_EventWait(0x14);
    Actor_FaceActor(8, 0xc, 0);
    Engine_EventWait(0x14);
    Engine_ActorSetAnimationAndWait(8, 3);
    Engine_EventWait(0x14);
    Event_ShowMessage(8, 0);
    Engine_EventWait(0x32);
    Actor_FaceActor(8, ACTOR_PARTY_LEADER, 0);
    Engine_EventWait(0x14);
    Event_ShowMessage(8, 0);
    Engine_EventWait(0x1e);
    Actor_ShowEmote(ACTOR_PARTY_LEADER, 0x102, 0);
    Actor_ShowEmote(GERALD, 0x102, 0);
    Actor_ShowEmote(0xb, 0x102, 0);
    Actor_ShowEmote(0xc, 0x102, 0);
    Actor_ShowEmote(9, 0x102, 0);
    Actor_ShowEmote(0xa, 0x102, 0);
    Engine_EventWait(0x50);
    Engine_ActorRunRepeatedMotion(0xb, 1);
    Engine_EventWait(0x14);
    Event_ShowMessage(0xb, 0);
    Engine_EventWait(0x14);
    Engine_ActorSetAnimationAndWait(8, 4);
    Engine_EventWait(0x14);
    Actor_ShowEmote(ACTOR_PARTY_LEADER, 0x101, 0);
    Actor_ShowEmote(GERALD, 0x101, 0);
    Actor_ShowEmote(0xb, 0x101, 0);
    Actor_ShowEmote(0xc, 0x101, 0);
    Actor_ShowEmote(9, 0x101, 0);
    Actor_ShowEmote(0xa, 0x101, 0);
    Engine_EventWait(0x50);
    Engine_ActorRunRepeatedMotion(8, 1);
    Engine_EventWait(0x14);
    Event_ShowMessage(8, 0);
    Engine_EventWait(0x14);
    Actor_SetAttachedEffect(ACTOR_PARTY_LEADER, 0x102);
    Actor_SetAttachedEffect(GERALD, 0x102);
    Actor_SetAttachedEffect(0xb, 0x102);
    Actor_SetAttachedEffect(0xc, 0x102);
    Actor_SetAttachedEffect(9, 0x102);
    Actor_SetAttachedEffect(0xa, 0x102);
    Engine_EventWait(0x3c);
    Engine_ActorSetAnimationAndWait(0xc, 4);
    Engine_EventWait(0x14);
    Event_ShowMessage(0xc, 0);
    Engine_EventWait(0x14);
    Engine_ActorRunRepeatedMotion(8, 1);
    Engine_EventWait(0x14);
    Engine_ActorSetAnimationAndWait(8, 4);
    Engine_EventWait(0x14);
    Event_ShowMessage(8, 0);
    Engine_EventWait(0x28);
    Engine_ActorFaceEachOther(ACTOR_PARTY_LEADER, GERALD, 0);
    Engine_ActorFaceEachOther(9, 0xa, 0);
    Actor_FaceActor(0xc, ACTOR_PARTY_LEADER, 0);
    Engine_EventWait(0x3c);
    Actor_FaceActor(ACTOR_PARTY_LEADER, 8, 0);
    Actor_FaceActor(GERALD, 8, 0);
    Actor_FaceActor(0xb, 8, 0);
    Actor_FaceActor(0xc, 8, 0);
    Actor_FaceActor(9, 8, 0);
    Actor_FaceActor(0xa, 8, 0);
    Engine_EventWait(0x14);
    Engine_ActorRunRepeatedMotion(0xb, 1);
    Engine_EventWait(0x14);
    Event_ShowMessage(0xb, 0);
    Engine_EventWait(0x3c);
    Actor_WalkToAndWait(8, 0xc8, 0x88);
    Actor_FaceActor(ACTOR_PARTY_LEADER, 8, 0);
    Actor_FaceActor(0xc, ACTOR_PARTY_LEADER, 0);
    Engine_ActorFaceEachOther(8, GERALD, 0);
    Engine_EventWait(0x28);
    Engine_ActorSetAnimationAndWait(8, 3);
    Engine_EventWait(0x32);
    Actor_FaceActor(8, ACTOR_PARTY_LEADER, 0);
    Engine_EventWait(0x28);
    Engine_ActorSetAnimationAndWait(8, 3);
    Engine_EventWait(0x32);
    Engine_ActorRunRepeatedMotion(8, 1);
    Engine_EventWait(0x14);
    Event_ShowMessage(8, 0);
    Engine_EventWait(0x1e);
    Actor_ShowEmote(ACTOR_PARTY_LEADER, 0x102, 0);
    Actor_ShowEmote(GERALD, 0x102, 0);
    Engine_EventWait(0x3c);
    Actor_SetAttachedEffect(0xb, 0x102);
    Engine_ActorRunRepeatedMotion(0xb, 1);
    Event_ShowMessage(0xb, 0);
    Engine_EventWait(0x1e);
    Engine_ActorSetAnimationAndWait(8, 3);
    Engine_EventWait(0x1e);
    Actor_ShowEmote(0xc, 0x102, 0);
    Engine_ActorStartRepeatedMotion(0xc, 1);
    Engine_EventWait(0x14);
    Event_ShowMessage(0xc, 0);
    Engine_EventWait(0x1e);
    Actor_WalkToAndWait(8, 0xa8, 0x78);
    Actor_FaceActor(ACTOR_PARTY_LEADER, 8, 0);
    Actor_FaceActor(GERALD, 8, 0);
    Actor_FaceActor(0xc, 8, 0);
    Actor_FaceDirection(8, 0xd000, 0);
    Engine_EventWait(0x14);
    Engine_ActorSetAnimationAndWait(8, 4);
    Engine_EventWait(0x14);
    Event_ShowMessage(8, 0);
    Engine_EventWait(0x1e);
    Actor_ShowEmote(ACTOR_PARTY_LEADER, 0x102, 0);
    Actor_ShowEmote(GERALD, 0x102, 0);
    Engine_EventWait(0x3c);
    Engine_ActorSetAnimationAndWait(8, 3);
    Engine_EventWait(0xa);
    Event_ShowMessage(8, 0);
    Engine_EventWait(0xa);
    Actor_FaceActor(8, ACTOR_PARTY_LEADER, 0);
    Engine_EventWait(0x14);
    Event_ShowMessage(8, 0);
    Engine_EventWait(0x14);
    Actor_FaceActor(0xc, ACTOR_PARTY_LEADER, 0);
    Actor_SetAttachedEffect(GERALD, 0x102);
    Engine_ActorRunRepeatedMotion(GERALD, 2);
    Actor_FaceActor(GERALD, ACTOR_PARTY_LEADER, 0);
    Engine_EventWait(0x14);
    Event_ShowMessage(GERALD, 0);
    Engine_EventWait(0x1e);
dialogue:
    Engine_EventSetMessage((s32)MsgShindenRobinWillAcceptResponsibilityFor);
    Engine_ActorRunRepeatedMotion(8, 1);
    Engine_EventWait(0x14);
    Event_OpenMessage(8, 0);
    if (Engine_EventChooseYesNo(0, 0) == 0) {
        FieldScene_RunActorUpdateSequence();
        ColorBuffer_ApplyTarget(0, 0);
        Engine_ColorBufferInterpolate(0x78);
        Engine_EventWait(0x78);
        Audio_PlayCue(0x56);
        AudioCommand_WaitForCompletion();
        GameFlag_Set(0x9f0);
        Engine_EventRequestExit(0x1e);
    } else {
        u8 *actor;
        s32 flags;

        Engine_EventWait(0x1e);
        Engine_ActorRunRepeatedMotion(0xc, 1);
        Engine_ActorSetAnimationAndWait(0xc, 4);
        Engine_EventWait(0x14);
        Event_ShowMessage(0xc, 0);
        Engine_EventWait(0x14);
        actor = &Object_GetById(0xa)->priority_flags;
        flags = 254;
        flags &= *actor;
        *actor = flags;
        actor = (u8 *)Object_GetById(0xa);
        ((struct FieldActor *)actor)->update =
            (void (*)(union FieldObject *))ShindenHeya_MatchLeaderPriority;
    }
    Engine_EventEnd();
}

void FieldScene_RunActorUpdateSequence(void)
{

    u32 i;
    struct FieldActor *record;

    Actor_SetAttachedEffect(1, 0x102);
    Engine_ActorRunRepeatedMotion(GERALD, 2);
    Engine_EventWait(20);
    Actor_SetSpeed(0, 0xcccc, 0x6666);
    Actor_SetSpeed(1, 0xcccc, 0x6666);
    Actor_SetSpeed(11, 0xcccc, 0x6666);
    Actor_SetSpeed(12, 0xcccc, 0x6666);
    Actor_SetSpeed(9, 0xcccc, 0x6666);
    Actor_SetSpeed(10, 0xcccc, 0x6666);
    Actor_SetSpeed(8, 0xcccc, 0x6666);
    Camera_MoveTo(0xc00000, -1, 0xa00000, 1);
    Engine_CameraWaitForMove();
    Actor_FaceActor(GERALD, ACTOR_PARTY_LEADER, 0);
    Actor_FaceActor(12, ACTOR_PARTY_LEADER, 0);
    Actor_FaceActor(9, ACTOR_PARTY_LEADER, 0);
    Actor_FaceActor(10, ACTOR_PARTY_LEADER, 0);
    Engine_ActorFaceEachOther(ACTOR_PARTY_LEADER, 11, 0);
    Engine_EventWait(10);
    Engine_ActorSetAnimation(ACTOR_PARTY_LEADER, 3);
    Engine_ActorSetAnimationAndWait(11, 3);
    Engine_EventWait(60);
    Engine_ActorSetAnimation(8, 3);
    Engine_ActorSetAnimation(12, 3);
    Engine_ActorSetAnimation(9, 3);
    Engine_ActorSetAnimationAndWait(10, 3);
    Engine_EventWait(50);
    Actor_FaceActor(ACTOR_PARTY_LEADER, 8, 0);
    Actor_FaceActor(11, 8, 0);
    Actor_FaceActor(12, 8, 0);
    Actor_FaceActor(9, 8, 0);
    Actor_FaceActor(10, 8, 0);
    Engine_EventWait(30);
    Engine_ActorSetAnimationAndWait(GERALD, 4);
    Engine_EventWait(20);
    Engine_EventSetMessage((s32)MsgShindenAcceptRobinCantMean);
    Event_ShowMessage(GERALD, 0);
    Engine_EventWait(20);
    Actor_FaceActor(12, GERALD, 0);
    Engine_ActorStartRepeatedMotion(12, 2);
    Engine_ActorShowEmote(12, 0x103, 0);
    Engine_EventWait(60);
    Event_ShowMessage(12, 0);
    Engine_EventWait(20);
    Engine_ActorRunRepeatedMotion(GERALD, 2);
    Engine_EventWait(20);
    Actor_FaceActor(GERALD, 12, 0);
    Engine_EventWait(20);
    Engine_ActorSetAnimationAndWait(GERALD, 3);
    Engine_EventWait(40);
    Actor_FaceActor(GERALD, 8, 0);
    Actor_FaceActor(12, 8, 0);
    Engine_EventWait(20);
    Engine_ActorSetAnimationAndWait(8, 3);
    Engine_EventWait(20);
    Engine_ActorFaceDirection(8, 0xd000, 0);
    Engine_EventWait(30);
    Engine_ActorSetAnimationAndWait(8, 3);
    Engine_EventWait(30);
    Event_ShowMessage(8, 0);
    Engine_EventWait(80);
    Audio_PlayCue(17);
    Engine_ColorBufferApplyTarget(0x10005, 1);
    Engine_ColorBufferInterpolate(60);
    Engine_EventWait(40);
    Camera_SetSpeed(0x6666, 0xccc);
    Camera_MoveTo(0xc00000, -1, 0x680000, 1);
    Engine_EventWait(120);
    Audio_PlayCue(21);
    Audio_PlayCue(0x134);
    Actor_SetPosition(13, 0xc80000, 0x80000);
    Actor_SetSpeed(13, 0x6666, 0x3333);
    Actor_MoveToAndWait(13, 200, 72);
    Audio_PlayCue(0x120);
    Engine_EventWait(30);
    Engine_ActorRunRepeatedMotion(8, 2);
    Engine_ActorSetAnimation(8, 0);
    ShindenHeya_CopyActorPose();
    Actor_FaceActor(ACTOR_PARTY_LEADER, 13, 0);
    Actor_FaceActor(GERALD, 13, 0);
    Actor_FaceActor(11, 13, 0);
    Actor_FaceActor(12, 13, 0);
    Actor_FaceActor(9, 13, 0);
    Actor_FaceActor(10, 13, 0);
    Engine_ActorStartRepeatedMotion(ACTOR_PARTY_LEADER, 2);
    Engine_ActorStartRepeatedMotion(GERALD, 2);
    Engine_ActorStartRepeatedMotion(11, 2);
    Engine_ActorStartRepeatedMotion(12, 2);
    Engine_ActorStartRepeatedMotion(9, 2);
    Engine_ActorStartRepeatedMotion(10, 2);
    Engine_EventWait(40);
    Event_ShowMessage(13, 0);
    Engine_EventWait(20);
    Engine_EventWait(40);
    Event_ShowMessage(13, 0);
    Engine_EventWait(60);
    Engine_ActorSetChildValue(13, 0x100);
    Audio_PlayCue(17);
    Audio_PlayCue(0x134);
    for (i = 0; i < 32; i++) {
        ShindenHeya_SpawnActorSpark(13);
        Engine_EventWait(4);
        record = Object_GetById(13);
        record->scale_x += -0x28f;
        record = Object_GetById(13);
        record->scale_y += -0x28f;
    }
    Audio_PlayCue(0x120);
    Actor_SetChildValue(13, 0);
    Actor_SetPosition(13, 0, 0);
    Engine_EventWait(40);
    Camera_MoveTo(0xc00000, -1, 0xa00000, 1);
    Engine_CameraWaitForMove();
    Engine_ColorBufferApplyTarget(0x10000, 0);
    Engine_ColorBufferInterpolate(60);
    Engine_EventWait(120);
    ShindenHeya_ResetActorUpdate();
    Engine_ActorSetAnimation(8, 1);
    Audio_PlayCue(2);
    Engine_EventWait(60);
    Engine_ActorRunRepeatedMotion(8, 2);
    Engine_EventWait(20);
    Engine_ActorStartRepeatedMotion(ACTOR_PARTY_LEADER, 1);
    Engine_ActorStartRepeatedMotion(GERALD, 1);
    Engine_ActorStartRepeatedMotion(11, 1);
    Engine_ActorStartRepeatedMotion(12, 1);
    Engine_ActorStartRepeatedMotion(9, 1);
    Engine_ActorRunRepeatedMotion(10, 1);
    Engine_EventWait(30);
    Event_ShowMessage(11, 0);
    Engine_EventWait(30);
    Actor_FaceActor(8, ACTOR_PARTY_LEADER, 0);
    Engine_EventWait(30);
    Actor_FaceActor(12, 8, 0);
    Engine_EventWait(20);
    Actor_FaceActor(ACTOR_PARTY_LEADER, 8, 0);
    Actor_FaceActor(GERALD, 8, 0);
    Actor_FaceActor(11, 8, 0);
    Actor_FaceActor(9, 8, 0);
    Actor_FaceActor(10, 8, 0);
    Engine_EventWait(20);
    Event_ShowMessage(12, 0);
    Engine_EventWait(20);
    Actor_FaceActor(8, 12, 0);
    Engine_EventWait(20);
    Engine_ActorSetAnimationAndWait(8, 3);
    Engine_EventWait(20);
    Engine_ActorSetAnimation(ACTOR_PARTY_LEADER, 3);
    Engine_ActorSetAnimationAndWait(GERALD, 3);
    Engine_EventWait(20);
    Actor_FaceActor(8, ACTOR_PARTY_LEADER, 0);
    Engine_EventWait(20);
    Engine_ActorSetAnimationAndWait(8, 3);
    Engine_EventWait(30);
    Event_ShowMessage(8, 0);
    Engine_EventWait(20);
    Actor_ShowEmote(0, 0x102, 0);
    Actor_ShowEmote(1, 0x102, 0);
    Actor_ShowEmote(11, 0x102, 0);
    Actor_ShowEmote(12, 0x102, 0);
    Actor_ShowEmote(9, 0x102, 0);
    Actor_ShowEmote(10, 0x102, 0);
    Engine_EventWait(30);
    Actor_ShowEmote(8, 0x102, 0);
    Engine_EventWait(30);
    Event_ShowMessage(8, 0);
    Engine_EventWait(20);
    Engine_ActorSetAnimation(ACTOR_PARTY_LEADER, 3);
    Engine_ActorSetAnimation(GERALD, 3);
    Engine_ActorSetAnimation(11, 3);
    Engine_ActorSetAnimation(12, 3);
    Engine_ActorSetAnimation(9, 3);
    Engine_ActorSetAnimationAndWait(10, 3);
    Engine_EventWait(20);
    Engine_ActorSetAnimationAndWait(8, 3);
    Engine_EventWait(180);
    Actor_ShowEmote(12, 0x105, 0);
    Engine_EventWait(60);
    Engine_ActorFaceEachOther(ACTOR_PARTY_LEADER, GERALD, 0);
    Engine_ActorFaceEachOther(9, 10, 0);
    Engine_EventWait(40);
    Actor_FaceActor(ACTOR_PARTY_LEADER, 8, 0);
    Actor_FaceActor(GERALD, 8, 0);
    Actor_FaceActor(9, 8, 0);
    Actor_FaceActor(10, 8, 0);
    Engine_EventWait(40);
    Engine_ActorRunRepeatedMotion(GERALD, 2);
    Engine_EventWait(20);
    Event_ShowMessage(GERALD, 0);
    Engine_EventWait(30);
    Actor_ShowEmote(8, 0x101, 0);
    Engine_EventWait(60);
    Engine_ActorStartRepeatedMotion(ACTOR_PARTY_LEADER, 1);
    Engine_ActorStartRepeatedMotion(GERALD, 1);
    Engine_ActorStartRepeatedMotion(11, 1);
    Engine_ActorStartRepeatedMotion(12, 1);
    Engine_ActorStartRepeatedMotion(9, 1);
    Engine_ActorRunRepeatedMotion(10, 1);
    Engine_EventWait(30);
    Event_ShowMessage(11, 0);
    Engine_EventWait(20);
    Actor_ShowEmote(8, 0x101, 0);
    Engine_EventWait(80);
    Actor_ShowEmote(12, 0x102, 0);
    Engine_EventWait(60);
    Event_ShowMessage(12, 0);
    Engine_EventWait(30);
    Actor_ShowEmote(8, 0x105, 0);
    Engine_EventWait(60);
    Engine_ActorRunRepeatedMotion(8, 1);
    Engine_ActorShowEmote(8, 0x106, 0);
    Engine_EventWait(20);
    Engine_ActorStartRepeatedMotion(ACTOR_PARTY_LEADER, 1);
    Engine_ActorStartRepeatedMotion(GERALD, 1);
    Engine_ActorStartRepeatedMotion(11, 1);
    Engine_ActorStartRepeatedMotion(12, 1);
    Engine_ActorStartRepeatedMotion(9, 1);
    Engine_ActorRunRepeatedMotion(10, 1);
    Engine_EventWait(40);
    Event_ShowMessage(8, 0);
    Engine_EventWait(20);
    Engine_ActorSetAnimation(ACTOR_PARTY_LEADER, 3);
    Engine_ActorSetAnimation(GERALD, 3);
    Engine_ActorSetAnimation(11, 3);
    Engine_ActorSetAnimation(12, 3);
    Engine_ActorSetAnimation(9, 3);
    Engine_ActorSetAnimationAndWait(10, 3);
    Engine_EventWait(30);
    Event_ShowMessage(8, 0);
    Engine_EventWait(30);
    Engine_ActorSetAnimation(ACTOR_PARTY_LEADER, 3);
    Engine_ActorSetAnimation(GERALD, 3);
    Engine_ActorSetAnimation(11, 3);
    Engine_ActorSetAnimation(12, 3);
    Engine_ActorSetAnimation(9, 3);
    Engine_ActorSetAnimationAndWait(10, 3);
    Engine_EventWait(120);
    Engine_ActorRunRepeatedMotion(GERALD, 1);
    Engine_EventWait(20);
    Event_ShowMessage(GERALD, 0);
    Engine_EventWait(30);
    Engine_ActorSetAnimationAndWait(8, 3);
    Engine_EventWait(20);
    Event_ShowMessage(8, 0);
    Engine_EventWait(20);
    Actor_ShowEmote(0, 0x105, 0);
    Actor_ShowEmote(1, 0x105, 0);
    Actor_ShowEmote(11, 0x105, 0);
    Actor_ShowEmote(12, 0x100, 0);
    Actor_ShowEmote(9, 0x105, 0);
    Actor_ShowEmote(10, 0x105, 0);
    Engine_EventWait(60);
    Engine_ActorRunRepeatedMotion(12, 1);
    Event_ShowMessage(12, 0);
    Engine_EventWait(20);
    Actor_FaceActor(8, 12, 0);
    Engine_EventWait(20);
    Engine_ActorSetAnimationAndWait(8, 4);
    Engine_EventWait(20);
    Event_ShowMessage(8, 0);
    Engine_EventWait(20);
    Actor_ShowEmote(0, 0x100, 0);
    Actor_ShowEmote(1, 0x100, 0);
    Actor_ShowEmote(11, 0x100, 0);
    Actor_ShowEmote(12, 0x100, 0);
    Actor_ShowEmote(9, 0x100, 0);
    Actor_ShowEmote(10, 0x100, 0);
    Engine_EventWait(60);
    Actor_FaceActor(8, ACTOR_PARTY_LEADER, 0);
    Engine_EventWait(20);
    Engine_ActorSetAnimationAndWait(8, 3);
    Event_ShowMessage(8, 0);
    Engine_EventWait(40);
    Actor_WalkToAndWait(8, 168, 176);
    Actor_FaceActor(ACTOR_PARTY_LEADER, 8, 0);
    Actor_FaceActor(GERALD, 8, 0);
    Actor_FaceActor(11, 8, 0);
    Actor_FaceActor(12, 8, 0);
    Actor_FaceActor(9, 8, 0);
    Actor_FaceActor(10, 8, 0);
    Actor_WalkToAndWait(8, 200, 200);
    Actor_FaceActor(ACTOR_PARTY_LEADER, 8, 0);
    Actor_FaceActor(GERALD, 8, 0);
    Actor_FaceActor(12, 8, 0);
    Actor_FaceDirection(11, 0, 0);
    Actor_FaceDirection(9, 0x8000, 0);
    Actor_FaceDirection(10, 0x8000, 0);
    Actor_WalkTo(8, 200, 0x110);
    Engine_EventWait(40);
    Actor_FaceDirection(11, 0x3000, 0);
    Actor_FaceDirection(9, 0x5000, 0);
    Actor_FaceDirection(10, 0x5000, 0);
    Engine_ActorWaitForMove(8);
    Actor_SetPosition(8, 0, 0);
    Engine_EventWait(60);
    Actor_ShowEmote(0, 0x102, 0);
    Actor_ShowEmote(1, 0x102, 0);
    Actor_ShowEmote(11, 0x102, 0);
    Actor_ShowEmote(12, 0x102, 0);
    Actor_ShowEmote(9, 0x102, 0);
    Actor_ShowEmote(10, 0x102, 0);
    Engine_EventWait(60);
    Engine_EventWait(120);
    Actor_FaceActor(ACTOR_PARTY_LEADER, 9, 0);
    Actor_FaceActor(GERALD, 10, 0);
    Actor_FaceActor(11, 9, 0);
    Actor_FaceActor(12, 9, 0);
    Engine_EventWait(120);
    Actor_ShowEmote(9, 0x105, 0);
    Engine_EventWait(60);
    Actor_FaceActor(9, ACTOR_PARTY_LEADER, 0);
    Engine_EventWait(10);
    Engine_ActorRunRepeatedMotion(9, 2);
    Engine_EventWait(10);
    Actor_FaceDirection(9, 0x5000, 0);
    Engine_EventWait(20);
    Actor_ShowEmote(9, 0x102, 0);
    Engine_EventWait(80);
    Actor_FaceActor(9, 10, 0);
    Engine_ActorRunRepeatedMotion(9, 1);
    Engine_EventWait(20);
    Event_ShowMessage(9, 0);
    Engine_EventWait(20);
    Actor_FaceDirection(10, 0x5000, 0);
    Engine_EventWait(20);
    Engine_ActorShowEmote(10, 0x101, 0);
    Engine_EventWait(60);
    Engine_ActorRunRepeatedMotion(9, 2);
    Engine_EventWait(20);
    Actor_FaceActor(10, ACTOR_PARTY_LEADER, 0);
    Engine_EventWait(30);
    Engine_ActorRunRepeatedMotion(10, 2);
    Engine_EventWait(30);
    Engine_ActorFaceDirection(10, 0x5000, 0);
    Engine_EventWait(20);
    Event_ShowMessage(10, 0);
    Engine_EventWait(20);
    Engine_ActorSetAnimation(9, 3);
    Engine_ActorSetAnimationAndWait(10, 3);
    Engine_EventWait(20);
    Actor_WalkTo(9, 200, 0x110);
    Actor_WalkToAndWait(10, 200, 0x110);
    Actor_SetPosition(9, 0, 0);
    Actor_SetPosition(10, 0, 0);
    Engine_ActorShowEmote(12, 0x105, 0);
    Engine_EventWait(60);
    Actor_WalkToAndWait(12, 200, 136);
    Engine_ActorFaceEachOther(ACTOR_PARTY_LEADER, 12, 0);
    Actor_FaceActor(GERALD, 12, 0);
    Actor_FaceActor(11, 12, 0);
    Engine_EventWait(30);
    Engine_ActorSetAnimationAndWait(12, 4);
    Engine_EventWait(20);
    Event_ShowMessage(12, 0);
    Engine_EventWait(30);
    Engine_ActorSetAnimation(ACTOR_PARTY_LEADER, 3);
    Engine_ActorSetAnimationAndWait(GERALD, 3);
    Engine_EventWait(20);
    Event_ShowMessage(11, 0);
    Engine_EventWait(20);
    Actor_WalkToAndWait(11, 168, 168);
    Actor_FaceActor(11, 12, 0);
    Engine_EventWait(20);
    Actor_FaceActor(ACTOR_PARTY_LEADER, 11, 0);
    Actor_FaceActor(GERALD, 11, 0);
    Engine_EventWait(20);
    Engine_ActorSetAnimationAndWait(11, 4);
    Engine_EventWait(20);
    Event_ShowMessage(11, 0);
    Engine_EventWait(20);
    Actor_ShowEmote(0, 0x102, 0);
    Actor_ShowEmote(1, 0x102, 0);
    Engine_EventWait(60);
    Engine_ActorSetAnimationAndWait(12, 4);
    Engine_EventWait(20);
    Actor_FaceActor(ACTOR_PARTY_LEADER, 12, 0);
    Actor_FaceActor(GERALD, 12, 0);
    Engine_EventWait(20);
    Event_ShowMessage(12, 0);
    Engine_EventWait(20);
    Engine_ActorRunRepeatedMotion(11, 2);
    Engine_EventWait(20);
    Event_ShowMessage(11, 0);
    Engine_EventWait(20);
    Actor_ShowEmote(0, 0x102, 0);
    Actor_ShowEmote(1, 0x102, 0);
    Engine_EventWait(60);
    Engine_ActorShowEmote(12, 0x102, 0);
    Engine_EventWait(60);
    Event_ShowMessage(12, 0);
    Engine_EventWait(20);
    Engine_ActorStartRepeatedMotion(ACTOR_PARTY_LEADER, 2);
    Engine_ActorStartRepeatedMotion(GERALD, 2);
    Actor_ShowEmote(0, 0x100, 0);
    Engine_ActorShowEmote(1, 0x100, 0);
    Engine_EventWait(60);
    Engine_ActorSetAnimationAndWait(11, 3);
    Engine_EventWait(20);
    Event_ShowMessage(11, 0);
    Engine_EventWait(20);
    Actor_FaceActor(ACTOR_PARTY_LEADER, 11, 0);
    Engine_EventWait(30);
    Engine_ActorSetAnimation(11, 3);
    Engine_EventWait(30);
    Engine_ActorSetAnimation(12, 3);
}

void ShindenHeya_CopyActorPose(void)
{
    u32 i;
    struct FieldActor *rec7;
    struct FieldActor *record;
    struct AnimationObject *p5;

    record = Object_GetById(8);
    if (record != 0) {
        Engine_ActorSetPosition(14, record->x.fixed, record->z.fixed);
    }
    Engine_ActorSetAnimation(14, 0);
    rec7 = Object_GetById(14);
    record = Object_GetById(8);
    rec7->facing = record->facing;
    record = Object_GetById(14);
    record->update = (void (*)(union FieldObject *))ShindenHeya_FollowLeaderOffset;
    record = Object_GetById(14);
    p5 = (struct AnimationObject *)record->sprite;
    {
        for (i = 0; i < p5->count; i++) {
            struct AnimationEntry *e = p5->entries[i];

            if (e != 0 && e->script != 0) {
                e->param = 10;
            }
        }
    }
    p5->dirty = 1;
    Object_GetById(14)->priority_flags &= 254;
    ((struct FieldSprite *)p5)->priority = 2;
}

void ShindenHeya_ResetActorUpdate(void)
{
    struct FieldActor *state = Object_GetById(14);
    state->update = 0;
    Engine_ActorSetPosition(14, 0, 0);
}

void ShindenHeya_FollowLeaderOffset(struct FieldActor *actor)
{
    struct FieldActor *leader;

    leader = Object_GetById(8);
    actor->target_x = actor->x.fixed = leader->x.fixed;
    actor->target_y = actor->y.fixed = leader->y.fixed;
    actor->target_z = actor->z.fixed = leader->z.fixed - 0x20000;
    switch (gFrameCount & 3) {
    case 0:
        actor->target_x = actor->x.fixed = leader->x.fixed - 0x38000;
        break;
    case 1:
        actor->target_x = actor->x.fixed = leader->x.fixed + 0x30000;
        break;
    case 2:
        actor->target_y = actor->y.fixed = leader->y.fixed + 0x20000;
        break;
    case 3:
        actor->target_z = actor->z.fixed = leader->z.fixed;
        break;
    }
}

void SceneEffect_StepEllipseOrbit(u8 *obj)
{
    u8 *anchor = *(u8 **)(obj + 104);
    u16 *angle = &((struct FieldActor *)obj)->timer;
    u16 theta = *angle;
    s32 x;
    s32 z;
    s32 tmp;

    x = ((struct FieldActor *)anchor)->x.fixed + Engine_MathCos(theta)* 14;
    ((struct FieldActor *)obj)->x.fixed = x;

    z = ((struct FieldActor *)anchor)->z.fixed + Engine_MathSin(theta)* 10;
    tmp = ((struct FieldActor *)obj)->x.fixed;
    ((struct FieldActor *)obj)->z.fixed = z;
    ((struct FieldActor *)obj)->target_z = z;
    ((struct FieldActor *)obj)->target_x = tmp;

    *angle = (u16)(*angle + ((struct FieldActor *)obj)->work);
}

void ShindenHeya_SpawnOwnerEffect(s32 a0, s32 a1)
{
    u32 i;
    s32 p8;
    s32 p8b;
    struct FieldActor *rec;
    u8 *rec8;
    u8 *p5;

    p8 = a1;
    rec = Object_GetById(a0);
    if (rec != 0) {
        rec8 = Object_Create(0x11d, rec->x.fixed, rec->y.fixed + 0x2d0000, rec->z.fixed);
        if ((s32)rec8 != 0) {
            p5 = (u8 *)((struct FieldActor *)rec8)->sprite;
            Engine_ObjectSetScript((s32)rec8, (s32)ShindenHeya_OwnerEffectScript);
            {
                s32 zero = 0;

                ((struct FieldActor *)rec8)->motion_flags = zero;
                ((struct FieldActor *)rec8)->timer = zero;
            }
            ((struct FieldActor *)rec8)->work = p8;
            ((struct FieldActor *)rec8)->update =
                (void (*)(union FieldObject *))SceneEffect_StepEllipseOrbit;
            { u16 v = 0; p5[38] = v; }
            /* FAKEMATCH: byte-bitfield aliasing preserves the anchor-store order and mov13/neg sequence. */
            {
                struct SpritePriorityByte {
                    u8 unknown_00[9];
                    u8 low : 2;
                    u8 priority : 2;
                };
                u8 priority = ((struct SpritePriorityByte *)(*(s32 *)&rec->sprite))->priority;

                *(s32 *)((s32)rec8 + 104) = (s32)rec;
                ((struct SpritePriorityByte *)p5)->priority = priority;
            }
        }
    }
}
