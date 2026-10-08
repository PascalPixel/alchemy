#include "BATTLE.H"
#include "DEBUG.H"
#include "FIELD.H"
#include "GAME.H"
#include "GRAPHICS.H"
#include "MENU.H"
#include "MINIGAME.H"
#include "SOUND.H"
#include "SYSTEM.H"
#include "GLOBAL_CELLS.H"
#include "TYPES.H"
#include "CALLBACK_SCHEDULER.H"
#include "FIELD_EVENT.H"
#include "VINASU.H"
#include "CHOJO.H"
#include "SCENE_IDS.H"
#include "CALL.H"
#include "FIELD_SCENE.H"
#include "STAGED_ACTOR.H"
#include "MAP_SCROLL.H"
#include "TBS_EDITION.H"
#include "IO_REG.H"
#include "WINDOW.H"
#include "text/MSG_IDS.H"

#define VinasuChojo_BeamOpenAction ((u8 (*))&VinasuChojo_BeamOpenAction)
#define VinasuChojo_ScaleStepAction ((u8 (*))&VinasuChojo_ScaleStepAction)

void SceneEffect_SpawnParticlesAboveActor(void);

void SceneEffect_SpawnParticlesAboveActor();
void VinasuChojo_RunTransitionStep();

void Object_SetActionCallbackAndRefreshById();
void Event_SetDestination();
void ObjectTable_Snapshot();


enum {
    PARTICLE_SOURCE_ACTOR = 23
};

void SceneEffect_SpawnParticlesBesideActor(void);


void ObjectDispatch_StopCallbacksAndHideLayers();
void UiText_ShowCenteredMessage();
void Graphics_EnableObjLayerAndCallbacks();
void Object_RefreshSelectorById();



void VinasuChojo_UpdateBeamActors(void)
{
    struct FieldActor *first;
    struct FieldActor *second;
    s32 stopped;

    first = Object_GetById(ACTOR_FIRST_OF_PAIR);
    second = Object_GetById(ACTOR_SECOND_OF_PAIR);
    /* FAKEMATCH: the dead first test and the word temporaries below reproduce the reference's leftover loads and stores. 2026-10-02: removing only the overwritten first test removes real loads and exchanges r5/r6 roles. */
    stopped = first->target_x == ACTOR_NO_TARGET && first->target_y == first->target_x && first->target_z == first->target_y;
    if (second->target_x == ACTOR_NO_TARGET && second->target_y == second->target_x && second->target_z == second->target_y) {
        stopped = 1;
    } else {
        stopped = 0;
    }
    if (stopped != 0) {
        {
            u16 shown = 0;

            first->facing = shown;
        }
        {
            u16 shown = 0;

            second->facing = shown;
        }
        if (Engine_GameFlagIsSet(FLAG_PAIR_GROWING) != 0) {
            Engine_ActorSetChildValue(ACTOR_FIRST_OF_PAIR, 7);
            Engine_ActorSetChildValue(ACTOR_SECOND_OF_PAIR, 7);
            if (first->scale_x >= 0x14000) {
                goto slide;
            }
            first->scale_x += 0x200;
            first->scale_y += 0x200;
            second->scale_x += 0x200;
            second->scale_y += 0x200;
        } else {
            if ((*(s32 *)&gFrameCount & 2) != 0) {
                Engine_ActorSetChildValue(ACTOR_FIRST_OF_PAIR, 15);
                Engine_ActorSetChildValue(ACTOR_SECOND_OF_PAIR, 0);
            } else {
                Engine_ActorSetChildValue(ACTOR_FIRST_OF_PAIR, 0);
                Engine_ActorSetChildValue(ACTOR_SECOND_OF_PAIR, 15);
            }
        }
        slide:;
        if (Engine_GameFlagIsSet(FLAG_PAIR_SLIDING) != 0) {
            if (first->x.fixed < PIXELS(312)) {
                first->x.fixed += 0x1000;
                second->x.fixed += 0x1000;
            }
            if (first->z.fixed > PIXELS(182)) {
                first->z.fixed += -0x1000;
                second->z.fixed += -0x1000;
            }
        }
    }
}

void FieldScene_RunThreeStepsInBracket(void)
{
    Engine_EventBegin();
    FieldScene_RunPairDefeat();
    FieldScene_RunMultiActorPresentation();
    VinasuChojo_RunActorTransition();
    Engine_EventEnd();
}

void FieldScene_RunBracketedSceneWithFlag282(void)
{
    Engine_EventBegin();
    FieldScene_RestageParty();
    VinasuChojo_TryToPreventFall();
    Engine_GameFlagSet(141 << 1);

    gEventWork->start_transition = SCENE_TRANSITION(TRANSITION_WINDOW, 0);
    gEventWork->transition_frames = 24;

    Engine_EventCloseScreen();
    Engine_EventWaitForScreen();
    Engine_EventRequestExit(1);
    Engine_EventEnd();
}

void VinasuChojo_RunLeapEntrance(void)
{
    struct FieldActor *leader;
    struct FieldActor *actor;

    leader = Object_GetById(ACTOR_PARTY_LEADER);
    Engine_EventBegin();
    Engine_EventGetViewCenter()->motion_flags = 0;
    Map_CopyCellsTo(102, 4, 74, 4, 18, 23);
    Map_CopyCellsTo(39, 72, 11, 72, 16, 20);
    Map_CopyCellAttributes(19, 6, 3, 7, 22, 6);
    Map_CopyCellAttributes(19, 6, 3, 7, 13, 6);
    Map_CopyCellAttributes(19, 6, 3, 7, 22, 13);
    Map_CopyCellAttributes(19, 6, 3, 7, 13, 13);
    Engine_TaskWait(1);
    Camera_MoveTo(PIXELS(192), -PIXELS(64), PIXELS(238), 0);
    Engine_TaskWait(1);
    Engine_MapRedraw();
    Engine_TaskWait(1);
    Engine_ActorDestroy(ACTOR_FIRST_OF_PAIR);
    Engine_ActorDestroy(ACTOR_SECOND_OF_PAIR);
    Engine_ActorSetAnimation(ACTOR_PARTY_LEADER, 19);
    Engine_ActorSetSpriteFlags(Actor_Get(ACTOR_PARTY_LEADER), 0);
    leader->x.fixed = PIXELS(346);
    leader->z.fixed = PIXELS(205);
    leader->y.fixed = PIXELS(32);
    leader->facing = FACING_SOUTHWEST;
    SceneActor_ParkRecord(leader);
    Engine_ActorSetAnimation(GERALD, 18);
    Engine_ActorSetSpriteFlags(Actor_Get(GERALD), 0);
    actor = Actor_Get(GERALD);
    actor->x.fixed = PIXELS(356);
    actor->z.fixed = PIXELS(192);
    actor->facing = FACING_NORTHWEST;
    actor->y.fixed = PIXELS(32);
    SceneActor_ParkRecord(actor);
    Engine_ActorSetAnimation(IWAN, 18);
    Engine_ActorSetSpriteFlags(Actor_Get(IWAN), 0);
    actor = Actor_Get(IWAN);
    actor->x.fixed = PIXELS(360);
    actor->facing = FACING_SOUTHEAST;
    actor->y.fixed = PIXELS(32);
    actor->z.fixed = PIXELS(222);
    SceneActor_ParkRecord(actor);
    Engine_ActorSetAnimation(MEARI, 18);
    Engine_ActorSetSpriteFlags(Actor_Get(MEARI), 0);
    actor = Actor_Get(MEARI);
    actor->x.fixed = PIXELS(334);
    actor->facing = FACING_WEST;
    actor->y.fixed = PIXELS(32);
    actor->z.fixed = PIXELS(222);
    SceneActor_ParkRecord(actor);
    Actor_SetPosition(21, PIXELS(196), PIXELS(220));
    Engine_ActorSetAnimation(21, 5);
    Actor_SetPosition(6, PIXELS(188), PIXELS(316));
    Engine_ActorSetAnimation(6, 5);
    Engine_ActorSetSpriteFlags(Actor_Get(6), 0);
    actor = Actor_Get(8);
    actor->x.fixed -= PIXELS(16);
    SceneActor_ParkRecord(actor);
    actor = Object_GetById(9);
    actor->x.fixed -= PIXELS(16);
    SceneActor_ParkRecord(actor);
    actor = Actor_Get(10);
    actor->x.fixed += PIXELS(16);
    SceneActor_ParkRecord(actor);
    actor = Object_GetById(11);
    actor->x.fixed += PIXELS(16);
    SceneActor_ParkRecord(actor);
    Work_SetValuesIfNonNegative(0x10000, 0x10000, 0x10000);
    Engine_ActorSetSpriteFlags(Actor_Get(PARTICLE_SOURCE_ACTOR), 0);
    Object_GetById(PARTICLE_SOURCE_ACTOR)->motion_flags = 4;
    Actor_SetChildValue(PARTICLE_SOURCE_ACTOR, 4);
    Actor_Get(PARTICLE_SOURCE_ACTOR)->y.fixed = PIXELS(40);
    Engine_TaskAddCallback(SceneEffect_SpawnParticlesBesideActor, TASK_PRIORITY_SCENE);
    gEventWork->start_transition = SCENE_TRANSITION(TRANSITION_WINDOW, 0);
    gEventWork->transition_frames = 24;
    Engine_EventOpenScreen();
    Engine_EventWaitForScreen();
    Engine_EventWait(40);
    Scene_RunExtendedActorTransition();
    GameFlag_Set(0x9a7);
    Engine_EventRequestExit(2);
}

void FieldScene_RunPairDefeat(void)
{
    struct FieldActor *actor;

    Audio_PlayCue(141);
    Map_CopyCellAttributes(17, 10, 4, 2, 17, 8);
    Actor_Get(ACTOR_PARTY_LEADER)->facing = FACING_NORTHWEST;
    Actor_Get(GERALD)->facing = FACING_NORTHWEST;
    Actor_SetPosition(GERALD, PIXELS(328), PIXELS(168));
    Actor_Get(IWAN)->facing = FACING_NORTHWEST;
    Actor_SetPosition(IWAN, PIXELS(340), PIXELS(196));
    Actor_Get(MEARI)->facing = FACING_NORTHWEST;
    Actor_SetPosition(MEARI, PIXELS(326), PIXELS(204));
    Actor_Get(21)->facing = FACING_NORTH + FACING_STEP;
    Actor_SetPosition(21, PIXELS(200), PIXELS(216));
    Actor_Get(6)->facing = FACING_NORTH + FACING_STEP;
    Actor_SetPosition(6, PIXELS(200), PIXELS(216));
    Actor_Get(ACTOR_FIRST_OF_PAIR)->facing = FACING_SOUTHEAST + FACING_STEP;
    Actor_SetPosition(ACTOR_FIRST_OF_PAIR, PIXELS(310), PIXELS(158));
    Actor_Get(ACTOR_SECOND_OF_PAIR)->facing = FACING_SOUTHEAST + FACING_STEP;
    Actor_SetPosition(ACTOR_SECOND_OF_PAIR, PIXELS(292), PIXELS(158));

    Engine_ActorSetSpriteFlags(Actor_Get(24), 0);
    Actor_SetChildValue(24, 7);
    Engine_ActorSetSpritePriority(24, 1);
    actor = Actor_Get(24);
    actor->scale_y = -0x10000;
    actor->scale_x = 0x3333;
    actor->motion_flags = 0;
    actor->x.fixed = PIXELS(304);
    actor->y.fixed = PIXELS(2);
    actor->z.fixed = PIXELS(96);

    Engine_ActorSetSpriteFlags(Actor_Get(25), 0);
    Actor_SetChildValue(25, 7);
    Engine_ActorSetSpritePriority(25, 1);
    actor = Actor_Get(25);
    actor->scale_y = -0x10000;
    actor->scale_x = 0x3333;
    actor->motion_flags = 0;
    actor->x.fixed = PIXELS(304);
    actor->y.fixed = PIXELS(34);
    actor->z.fixed = PIXELS(96);

    Engine_TaskAddCallback(SceneEffect_SpawnParticlesAboveActor, TASK_PRIORITY_SCENE);
    Engine_EventGetViewCenter()->motion_flags = 0;
    Camera_MoveTo(PIXELS(304), PIXELS(32), PIXELS(180), 0);
    Engine_TaskWait(1);
    Engine_MapRedraw();
    Engine_TaskWait(1);
    Work_SetValuesIfNonNegative(0x10000, 0x10000, 0x10000);
    Engine_EventOpenScreen();
    Engine_EventWaitForScreen();
    Engine_EventWait(40);
    Engine_ActorRunRepeatedMotion(ACTOR_FIRST_OF_PAIR, 2);
    Engine_EventWait(10);
    Engine_EventSetMessage((s32)MsgVinasuPairDefeated);
    VinasuChojo_ShowMessage(ACTOR_FIRST_OF_PAIR);
    Engine_ActorRunRepeatedMotion(ACTOR_SECOND_OF_PAIR, 3);
    Engine_EventWait(20);
    Event_ShowMessageAndWait(ACTOR_SECOND_OF_PAIR, 0, 40);
    Audio_PlayCue(17);

    actor = Actor_Get(ACTOR_FIRST_OF_PAIR);
    actor->x.fixed = PIXELS(308);
    actor->y.fixed = PIXELS(28);
    actor->z.fixed = PIXELS(152);
    Engine_ActorSetAnimation(ACTOR_FIRST_OF_PAIR, 10);
    Engine_EventWait(20);
    actor->x.fixed = PIXELS(306);
    actor->y.fixed = PIXELS(28);
    actor->z.fixed = PIXELS(152);
    Engine_ActorSetAnimation(ACTOR_FIRST_OF_PAIR, 11);
    Engine_EventWait(12);
    actor->x.fixed = PIXELS(304);
    actor->y.fixed = PIXELS(21);
    actor->z.fixed = PIXELS(152);
    Engine_ActorSetAnimation(ACTOR_FIRST_OF_PAIR, 12);
    Engine_EventWait(8);
    Engine_ActorDestroy(ACTOR_FIRST_OF_PAIR);

    actor = Actor_Get(ACTOR_SECOND_OF_PAIR);
    actor->x.fixed = PIXELS(294);
    actor->y.fixed = PIXELS(28);
    actor->z.fixed = PIXELS(152);
    Engine_ActorSetAnimation(ACTOR_SECOND_OF_PAIR, 8);
    Engine_EventWait(20);
    actor->x.fixed = PIXELS(300);
    actor->y.fixed = PIXELS(27);
    actor->z.fixed = PIXELS(152);
    Engine_ActorSetAnimation(ACTOR_SECOND_OF_PAIR, 9);
    Engine_EventWait(12);
    actor->x.fixed = PIXELS(304);
    actor->y.fixed = PIXELS(17);
    actor->z.fixed = PIXELS(152);
    Engine_ActorSetAnimation(ACTOR_SECOND_OF_PAIR, 10);
    Engine_EventWait(8);
    Engine_ActorDestroy(ACTOR_SECOND_OF_PAIR);
    Engine_EventWait(160);
}

void FieldScene_RunMultiActorPresentation(void)
{
    s32 count_flag;
    s32 request_a;
    s32 request_b;

    Engine_ActorRunRepeatedMotion(GERALD, 1);
    Engine_EventWait(20);
    Event_ShowMessageAndWait(GERALD, 0, 20);
    Engine_ActorRunRepeatedMotion(IWAN, 1);
    Engine_EventWait(20);
    Event_OpenMessage(IWAN, 0);
    Actor_FaceDirection(IWAN, 0xc000, 0);
    VinasuChojo_FaceActor(0, 0x4000);
    count_flag = 0;
    if (Engine_EventChooseYesNo(0, 0) == 0) {
        Engine_EventWait(20);
        Engine_ActorSetAnimationAndWait(IWAN, 4);
        count_flag = 1;
    } else {
        Engine_EventWait(20);
        Engine_ActorRunRepeatedMotion(IWAN, 1);
        gEventWork->message += 1;
    }
    VinasuChojo_ShowMessage(2);
    if (count_flag != 0) {
        gEventWork->message += 1;
    }
    Actor_SetSpeed(GERALD, 0x19999, 0xcccc);
    request_a = 0x1001;
    Actor_WalkToAndWait(GERALD, 0x141, 174);
    VinasuChojo_FaceActor(1, 0x2000);
    VinasuChojo_ShowMessage(request_a);
    Actor_FaceDirection(ACTOR_PARTY_LEADER, 0xa000, 0);
    Actor_FaceDirection(IWAN, 0xa000, 0);
    VinasuChojo_FaceActor(3, 0xc000);
    VinasuChojo_FaceActor(1, 0x4000);
    VinasuChojo_ShowMessage(request_a);
    Actor_ShowEmote(GERALD, 0x105, 40);
    Actor_FaceDirection(GERALD, 0x2000, 20);
    VinasuChojo_ShowMessage(request_a);
    Actor_FaceDirection(GERALD, 0xa000, 40);
    Event_ShowMessageAndWait(0x8001, 0, 40);
    Actor_FaceDirection(GERALD, 0x2000, 20);
    VinasuChojo_ShowMessage(request_a);
    Audio_PlayCue(17);
    Engine_ActorSetAnimation(MEARI, 4);
    VinasuChojo_ShowMessage(3);
    Actor_FaceDirection(GERALD, 0x4000, 80);
    Actor_FaceDirection(MEARI, 0x6000, 80);
    Actor_ShowEmote(MEARI, 0x100, 40);
    Actor_FaceDirection(ACTOR_PARTY_LEADER, 0x6000, 60);
    Engine_ActorRunRepeatedMotion(ACTOR_PARTY_LEADER, 2);
    VinasuChojo_FaceActor(2, 0xc000);
    Actor_ShowEmote(IWAN, 0x101, 60);
    Actor_FaceDirection(IWAN, 0x6000, 40);
    Engine_ActorJump(IWAN, 4, 60);
    VinasuChojo_FaceActor(1, 0x2000);
    Actor_ShowEmote(GERALD, 0x101, 40);
    VinasuChojo_ShowMessage(request_a);
    Actor_FaceDirection(GERALD, 0x6000, 40);
    Actor_ShowEmote(GERALD, 0x100, 0);
    Engine_ActorJump(GERALD, 4, 40);
    VinasuChojo_ShowMessage(1);
    Actor_SetSpeed(21, 0xcccc, 0x6666);
    Actor_SetDestination(21, 200, 188);
    Actor_SetDestination(6, 200, 204);
    Camera_SetSpeed(0x33333, 0x6666);
    Camera_MoveTo(0xfc0000, 0, 0xbe0000, 1);
    Engine_CameraWaitForMove();
    Engine_EventWait(40);
    Audio_PlayCue(23);
    Actor_SetAttachedEffect(21, 0x102);
    VinasuChojo_ShowMessage(21);
    Engine_ActorRunRepeatedMotion(21, 1);
    Engine_EventWait(20);
    VinasuChojo_ShowMessage(21);
    Engine_ActorSetAnimationAndWait(21, 4);
    VinasuChojo_ShowMessage(21);
    Actor_FaceDirection(21, 0x3000, 20);
    VinasuChojo_ShowMessage(21);
    Actor_SetAttachedEffect(6, 0x102);
    request_b = 0x2003;
    Engine_EventWait(20);
    Engine_ActorStartRepeatedMotion(MEARI, 2);
    VinasuChojo_ShowMessage(request_b);
    Engine_ActorRunRepeatedMotion(21, 2);
    Engine_EventWait(20);
    VinasuChojo_ShowMessage(0x2002);
    VinasuChojo_FaceActor(21, 0xe000);
    Engine_ActorRunRepeatedMotion(GERALD, 1);
    VinasuChojo_ShowMessage(1);
    Engine_ActorRunRepeatedMotion(21, 1);
    Event_ShowMessageAndWait(21, 0, 20);
    VinasuChojo_ShowMessage(request_b);
    Engine_ActorSetAnimationAndWait(21, 4);
    VinasuChojo_ShowMessage(21);
    Actor_SetAttachedEffect(IWAN, 0x102);
    VinasuChojo_ShowMessage(0x2002);
    Engine_ActorSetAnimation(21, 3);
    VinasuChojo_ShowMessage(21);
    Actor_ShowEmote(GERALD, 0x103, 40);
    Engine_ActorStartRepeatedMotion(GERALD, 2);
    VinasuChojo_ShowMessage(1);
    Engine_ActorSetAnimation(21, 4);
    VinasuChojo_ShowMessage(21);
    Actor_FaceDirection(21, 0x5000, 20);
    Engine_ActorJump(MEARI, 4, 20);
    Event_ShowMessageAndWait(request_b, 0, 20);
    Actor_ShowEmote(21, 0x103, 40);
}

void VinasuChojo_RunActorTransition(void)
{
    struct FieldActor *lower;
    struct FieldActor *middle;
    struct FieldActor *upper;
    struct FieldActor *actor;

    Engine_AudioPlayCue(19);
    Engine_AudioPlayCue(0x120);
    Work_SetValuesIfNonNegative(0x30000, 0x30000, 0x10000);
    Actor_ShowEmote(ACTOR_PARTY_LEADER, 0x100, 0);
    Actor_ShowEmote(GERALD, 0x100, 0);
    Actor_ShowEmote(IWAN, 0x100, 0);
    Actor_ShowEmote(MEARI, 0x100, 0);
    Actor_ShowEmote(21, 0x100, 0);
    Actor_ShowEmote(6, 0x100, 10);
    Actor_FaceDirection(ACTOR_PARTY_LEADER, FACING_NORTHWEST, 0);
    Actor_FaceDirection(GERALD, FACING_NORTHWEST, 0);
    Actor_FaceDirection(IWAN, FACING_NORTHWEST, 0);
    Actor_FaceDirection(MEARI, FACING_NORTHWEST, 0);
    Actor_FaceDirection(21, FACING_NORTH + FACING_STEP, 0);
    Actor_FaceDirection(6, FACING_NORTH + FACING_STEP, 0);
    Work_SetValuesIfNonNegative(0x20000, 0x20000, 0x10000);
    Engine_EventWait(10);
    Work_SetValuesIfNonNegative(0x10000, 0x10000, 0x10000);
    Camera_MoveTo(PIXELS(304), PIXELS(32), PIXELS(180), 1);
    Engine_CameraWaitForMove();
    Object_GetById(24)->scale_x = 0x1999;
    Object_GetById(25)->scale_x = 0x1999;
    Engine_ActorEnableActionCallback(24, VinasuChojo_BeamOpenAction);
    Engine_ActorEnableActionCallback(25, VinasuChojo_BeamOpenAction);
    Engine_AudioPlayCue(145);
    Work_SetValuesIfNonNegative(0x60000, 0x60000, 0x10000);
    ColorBuffer_ApplyTarget(0x4063ff, 0);
    Engine_ColorBufferInterpolate(16);
    Engine_TaskWait(20);
    Engine_ColorBufferApplyTarget(0x7fff, 0);
    Engine_ColorBufferInterpolate(24);
    Engine_TaskWait(60);
    Engine_AudioPlayCue(141);
    Engine_GameFlagSet(0x236);
    Object_GetById(24)->y.fixed = -PIXELS(96);
    Object_GetById(25)->y.fixed = -PIXELS(64);
    Engine_ActorSetChildValue(26, 7);
    Engine_ActorSetSpriteFlags(Object_GetById(26), 0);
    lower = Object_GetById(26);
    lower->scale_y = -0x10000;
    lower->scale_x = Object_GetById(24)->scale_x;
    lower->motion_flags = 0;
    lower->x.fixed = PIXELS(304);
    lower->y.fixed = -PIXELS(32);
    lower->z.fixed = PIXELS(96);
    Engine_ActorSetChildValue(27, 7);
    Engine_ActorSetSpriteFlags(Object_GetById(27), 0);
    middle = Object_GetById(27);
    middle->scale_y = -0x10000;
    middle->scale_x = Object_GetById(24)->scale_x;
    middle->motion_flags = 0;
    middle->x.fixed = PIXELS(304);
    middle->y.fixed = 0;
    middle->z.fixed = PIXELS(96);
    Engine_ActorSetChildValue(28, 7);
    Engine_ActorSetSpriteFlags(Object_GetById(28), 0);
    upper = Object_GetById(28);
    upper->scale_y = -0x10000;
    upper->scale_x = Object_GetById(24)->scale_x;
    upper->motion_flags = 0;
    upper->x.fixed = PIXELS(304);
    upper->y.fixed = PIXELS(32);
    upper->z.fixed = PIXELS(96);
    Map_CopyCellsTo(102, 4, 74, 4, 18, 23);
    Map_CopyCellsTo(39, 72, 11, 72, 16, 21);
    Engine_MapCopyCellAttributes(19, 6, 3, 7, 22, 6);
    Engine_MapCopyCellAttributes(19, 6, 3, 7, 13, 6);
    Engine_MapCopyCellAttributes(19, 6, 3, 7, 22, 13);
    Engine_MapCopyCellAttributes(19, 6, 3, 7, 13, 13);
    Engine_TaskWait(1);
    actor = Object_GetById(8);
    actor->x.fixed -= PIXELS(16);
    SceneActor_ParkRecord(actor);
    actor = Object_GetById(9);
    actor->x.fixed -= PIXELS(16);
    SceneActor_ParkRecord(actor);
    actor = Object_GetById(10);
    actor->x.fixed += PIXELS(16);
    SceneActor_ParkRecord(actor);
    actor = Object_GetById(11);
    actor->x.fixed += PIXELS(16);
    SceneActor_ParkRecord(actor);
    actor = Object_GetById(ACTOR_PARTY_LEADER);
    actor->x.fixed += PIXELS(16);
    actor->z.fixed += PIXELS(16);
    SceneActor_ParkRecord(actor);
    actor = Object_GetById(GERALD);
    actor->x.fixed += PIXELS(16);
    actor->z.fixed += PIXELS(16);
    SceneActor_ParkRecord(actor);
    actor = Object_GetById(IWAN);
    actor->x.fixed += PIXELS(16);
    actor->z.fixed += PIXELS(16);
    SceneActor_ParkRecord(actor);
    actor = Object_GetById(MEARI);
    actor->x.fixed += PIXELS(16);
    actor->z.fixed += PIXELS(16);
    SceneActor_ParkRecord(actor);
    Actor_SetPosition(21, PIXELS(196), PIXELS(220));
    Engine_ActorSetAnimation(21, 5);
    Actor_SetPosition(6, PIXELS(188), PIXELS(316));
    Engine_ActorSetAnimation(6, 5);
    Engine_ActorSetSpriteFlags(Object_GetById(6), 0);
    Work_SetValuesIfNonNegative(0x40000, 0x40000, 0x10000);
    Engine_ColorBufferApplyTarget(0x4063ff, 0);
    Engine_ColorBufferInterpolate(120);
    Engine_ActorEnableActionCallback(24, VinasuChojo_BeamRiseAction);
    Engine_ActorEnableActionCallback(25, VinasuChojo_BeamRiseAction);
    Engine_ActorEnableActionCallback(26, VinasuChojo_BeamRiseAction);
    Engine_ActorEnableActionCallback(27, VinasuChojo_BeamRiseAction);
    Engine_ActorEnableActionCallback(28, VinasuChojo_BeamRiseAction);
    Engine_TaskWait(120);
    Work_SetValuesIfNonNegative(0x30000, 0x30000, 0x10000);
    Engine_ColorBufferApplyTarget(0x203210, 0);
    Engine_ColorBufferInterpolate(120);
    Engine_TaskWait(120);
    Work_SetValuesIfNonNegative(0x20000, 0x20000, 0x10000);
    ColorBuffer_ApplyTarget(0x10000, 0);
    Engine_ColorBufferInterpolate(120);
    Engine_TaskWait(120);
    Work_SetValuesIfNonNegative(0x10000, 0x10000, 0x10000);
    Object_GetById(24)->scale_y = 0x51e;
    Engine_ActorEnableActionCallback(25, VinasuChojo_BeamCloseAction);
    Engine_ActorEnableActionCallback(26, VinasuChojo_BeamCloseAction);
    Engine_ActorEnableActionCallback(27, VinasuChojo_BeamCloseAction);
    Object_SetActionCallbackAndRefreshById(28, VinasuChojo_BeamCloseAction);
    Engine_AudioPlayCue(0x121);
    Engine_ActorSetChildValue(24, 15);
    Engine_EventWait(20);
    Object_SetActionCallbackAndRefreshById(24, VinasuChojo_BeamEndAction);
    Scheduler_RemoveCallback((u32)SceneEffect_SpawnParticlesAboveActor);
    Engine_ActorJump(IWAN, 2, 20);
    VinasuChojo_ShowMessage(IWAN);
    Actor_FaceDirection(GERALD, FACING_SOUTHWEST, 20);
    Actor_SetAttachedEffect(GERALD, 0x102);
    Engine_EventWait(20);
    Engine_ActorStartRepeatedMotion(GERALD, 2);
    VinasuChojo_ShowMessage(GERALD);
    Actor_SetSpeed(MEARI, 0xcccc, 0x6666);
    Engine_ActorWalkToAndWait(MEARI, 326, 220);
    Engine_EventWait(40);
    Engine_ActorSetAttachedEffect(MEARI, 0x102);
    VinasuChojo_ShowMessage(MEARI);
    actor = Actor_Get(ACTOR_PARTY_LEADER);
    actor->rise_counter = 0;
    actor->rise_enabled = 1;
    actor->rise_base_y = actor->y.fixed;
    actor->facing = FACING_NORTHWEST;
    actor = Actor_Get(GERALD);
    actor->rise_counter = 0;
    actor->rise_enabled = 1;
    actor->facing = FACING_NORTHWEST;
    actor->rise_base_y = actor->y.fixed;
    actor = Actor_Get(IWAN);
    actor->rise_counter = 0;
    actor->rise_enabled = 1;
    actor->facing = FACING_NORTHWEST;
    actor->rise_base_y = actor->y.fixed;
    actor = Actor_Get(MEARI);
    actor->rise_counter = 0;
    actor->rise_enabled = 1;
    actor->facing = FACING_NORTHWEST;
    actor->rise_base_y = actor->y.fixed;
    actor = Actor_Get(21);
    actor->rise_counter = 0;
    actor->rise_enabled = 1;
    actor->rise_base_y = actor->y.fixed;
    actor = Actor_Get(6);
    actor->rise_counter = 0;
    actor->rise_enabled = 1;
    actor->rise_base_y = actor->y.fixed;
    Object_GetById(PARTICLE_SOURCE_ACTOR)->motion_flags = 0;
    Engine_ActorSetSpriteFlags(Object_GetById(PARTICLE_SOURCE_ACTOR), 0);
    Engine_ActorSetChildValue(PARTICLE_SOURCE_ACTOR, 7);
    Engine_ActorSetSpritePriority(PARTICLE_SOURCE_ACTOR, 2);
    VinasuChojo_TransitionStep = 0;
    VinasuChojo_TransitionTimer = 240;
    Engine_TaskAddCallback(VinasuChojo_RunTransitionStep, TASK_PRIORITY_SCENE);
    do {
        Engine_TaskWait(1);
    } while (Engine_GameFlagIsSet(0x237) == 0);
    Engine_GameFlagSet(0x101);
    Engine_EventWait(30);
    Engine_GameFlagSet(0x11a);
    ObjectTable_Snapshot();
    Event_SetDestination((s32)&SceneId_WorldMap, 91);
    {
        u16 white = 0x7fff;

        PLTT_BACKDROP = white;
    }
    gEventWork->transition_frames = 1;
    Engine_EventCloseScreen();
    Engine_EventWaitForScreen();
}

void FieldScene_RestageParty(void)
{
    struct FieldActor *actor;

    Engine_ActorDestroy(ACTOR_FIRST_OF_PAIR);
    Engine_ActorDestroy(ACTOR_SECOND_OF_PAIR);
    Audio_PlayCue(141);
    Map_CopyCellAttributes(17, 10, 4, 2, 17, 8);
    Map_CopyCellsTo(102, 4, 74, 4, 18, 23);
    Map_CopyCellsTo(39, 72, 11, 72, 16, 21);
    Map_CopyCellAttributes(19, 6, 3, 7, 22, 6);
    Map_CopyCellAttributes(19, 6, 3, 7, 13, 6);
    Map_CopyCellAttributes(19, 6, 3, 7, 22, 13);
    Map_CopyCellAttributes(19, 6, 3, 7, 13, 13);
    Work_SetValuesIfNonNegative(0x20000, 0x20000, 0x10000);
    Camera_MoveTo(-1, -1, -1, 0);
    Engine_ActorsRefresh();
    Engine_MapRedraw();
    Engine_TaskWait(1);
    Audio_PlayCue(SOUND_ITEM_BREAK);
    Engine_ActorSetAnimation(ACTOR_PARTY_LEADER, 19);
    Engine_ActorSetAnimation(GERALD, 18);
    Engine_ActorSetAnimation(IWAN, 18);
    Engine_ActorSetAnimation(MEARI, 18);
    Engine_ActorSetSpriteFlags(Actor_Get(ACTOR_PARTY_LEADER), 0);
    Engine_ActorSetSpriteFlags(Actor_Get(GERALD), 0);
    Engine_ActorSetSpriteFlags(Actor_Get(IWAN), 0);
    Engine_ActorSetSpriteFlags(Actor_Get(MEARI), 0);

    actor = Actor_Get(ACTOR_PARTY_LEADER);
    actor->x.fixed = PIXELS(346);
    actor->y.fixed = PIXELS(32);
    actor->z.fixed = PIXELS(205);
    SceneActor_ParkRecord(actor);
    actor->rise_enabled = 0;
    actor->velocity_y = 0x20000;

    actor = Actor_Get(GERALD);
    actor->x.fixed = PIXELS(356);
    actor->y.fixed = PIXELS(32);
    actor->z.fixed = PIXELS(192);
    SceneActor_ParkRecord(actor);
    actor->rise_enabled = 0;
    actor->velocity_y = 0x20000;

    actor = Actor_Get(IWAN);
    actor->x.fixed = PIXELS(360);
    actor->y.fixed = PIXELS(32);
    actor->z.fixed = PIXELS(222);
    SceneActor_ParkRecord(actor);
    actor->rise_enabled = 0;
    actor->velocity_y = 0x20000;

    actor = Actor_Get(MEARI);
    actor->x.fixed = PIXELS(334);
    actor->y.fixed = PIXELS(32);
    actor->z.fixed = PIXELS(222);
    SceneActor_ParkRecord(actor);
    actor->rise_enabled = 0;
    actor->velocity_y = 0x20000;

    Actor_Get(21)->rise_counter = 0;
    Actor_Get(6)->rise_counter = 0;
    Actor_Get(PARTICLE_SOURCE_ACTOR)->motion_flags |= 4;
    Actor_SetChildValue(PARTICLE_SOURCE_ACTOR, 4);
    Engine_TaskAddCallback(SceneEffect_SpawnParticlesBesideActor, TASK_PRIORITY_SCENE);
    gEventWork->transition_frames = 1;
    Engine_EventOpenScreen();
    ColorBuffer_ApplySource(0x7fff, 0);
    ColorBuffer_ApplyTarget(0x10000, 0);
    Engine_ColorBufferInterpolate(40);
    Engine_TaskWait(60);
}

void VinasuChojo_TryToPreventFall(void)
{
    u32 i;
    s32 record;

    Engine_EventSetMessage((s32)MsgVinasuNoooo);
    VinasuChojo_ShowMessage(21);
    Audio_PlayCue(62);
    Work_SetValuesIfNonNegative(0x10000, 0x10000, 0x10000);
    Camera_SetSpeed(0x4cccc, 0x9999);
    Camera_SetSpeed(0x40000, 0x8000);
    Camera_MoveTo(0xc00000, -0x400000, 0xee0000, 1);
    Engine_CameraWaitForMove();
    Engine_EventWait(40);
    Engine_ActorRunRepeatedMotion(21, 1);
    Event_ShowMessageAndWait(0x2015, 0, 40);
    Engine_ActorRunRepeatedMotion(6, 3);
    VinasuChojo_ShowMessage(6);
    Actor_SetAttachedEffect(21, 0x102);
    Engine_EventWait(60);
    Event_ShowMessageAndWait(0x2015, 0, 80);
    Actor_SetAttachedEffect(6, 0x102);
    Engine_EventWait(40);
    Engine_ActorStartRepeatedMotion(6, 2);
    VinasuChojo_ShowMessage(6);
}

void Scene_RunExtendedActorTransition(void)
{
    struct FieldActor *actor;
    s32 step_pending;
    void **cell;

    step_pending = 0;
    Engine_EventSetMessage((s32)MsgVinasuWhyHappeningProtectVenusLighthouse);
    Actor_SetAttachedEffect(21, 0x102);
    Engine_EventWait(20);
    Event_ShowMessageAndWait(0x2015, 0, 20);
    Engine_ActorRunRepeatedMotion(6, 2);
    Event_ShowMessageAndWait(6, 0, 20);
    Engine_ActorSetAnimation(6, 6);
    Engine_EventWait(10);
    Engine_ActorStartRepeatedMotion(21, 2);
    Event_ShowMessageAndWait(0x2015, 0, 40);
    Engine_ActorSetAnimation(6, 7);
    Event_ShowMessageAndWait(6, 0, 20);
    Audio_PlayCue(17);
    Engine_ActorRunRepeatedMotion(6, 2);
    Engine_ActorEnableActionCallback(6, VinasuChojo_StepBackAction);
    Engine_EventWait(20);
    Actor_SetAttachedEffect(21, 0x102);
    Engine_ActorStartRepeatedMotion(21, 3);
    Object_RefreshSelectorById(6);
    Engine_EventWait(160);
    Engine_ActorRunRepeatedMotion(21, 1);
    Engine_EventWait(20);
    Object_GetById(21)->facing = FACING_SOUTH + FACING_STEP;
    Engine_ActorSetAnimation(21, 0);
    Engine_EventWait(80);
    Engine_ActorSetAnimationAndWait(21, 4);
    Engine_EventWait(40);
    Event_ShowMessageAndWait(21, 0, 40);
    Engine_ActorSetAnimation(21, 5);
    Engine_EventWait(10);
    Engine_ActorSetSpriteFlags(Object_GetById(21), 0);
    Engine_ActorSetAnimation(21, 0);
    Engine_ActorJump(21, 6, 0);
    Actor_SetSpeed(21, 0x30000, 0x18000);
    Object_GetById(21)->steering_flags &= ~ACTOR_FACES_MOTION;
    actor = Object_GetById(21);
    actor->update = (void (*)(union FieldObject *))VinasuChojo_StepSpriteRotation;
    actor->facing = FACING_WEST;
    Actor_MoveToAndWait(21, 184, 237);
    Object_SetActionCallbackAndRefreshById(21, VinasuChojo_LeapAction);
    Engine_EventWait(120);
    VinasuChojo_ShowMessage(ACTOR_PARTY_LEADER);
    Audio_PlayCue(72);
    Camera_SetSpeed(0x40000, 0x8000);
    Camera_MoveTo(PIXELS(342), PIXELS(32), PIXELS(212), 1);
    Engine_CameraWaitForMove();
    Engine_EventWait(20);
    Engine_ActorRunRepeatedMotion(ACTOR_PARTY_LEADER, 2);
    Engine_EventWait(40);
    Engine_ActorSetSpriteFlags(Object_GetById(ACTOR_PARTY_LEADER), 1);
    Engine_ActorSetAnimation(ACTOR_PARTY_LEADER, 1);
    Engine_ActorJump(ACTOR_PARTY_LEADER, 6, 60);
    Object_GetById(ACTOR_PARTY_LEADER)->timer = 10;
    Engine_ActorEnableActionCallback(ACTOR_PARTY_LEADER, VinasuChojo_ScaleStepAction);
    Actor_ShowEmote(GERALD, 0x100, 80);
    Event_ShowMessageAndWait(GERALD, 0, 40);
    Engine_ActorRunRepeatedMotion(IWAN, 1);
    Engine_EventWait(20);
    VinasuChojo_ShowMessage(IWAN);
    Actor_ShowEmote(MEARI, 0x102, 40);
    VinasuChojo_ShowMessage(MEARI);
    Engine_ActorSetSpriteFlags(Object_GetById(GERALD), 1);
    Engine_ActorSetAnimation(GERALD, 1);
    Engine_ActorJump(GERALD, 6, 60);
    Engine_ActorRunRepeatedMotion(IWAN, 1);
    Engine_EventWait(20);
    Engine_ActorSetSpriteFlags(Object_GetById(IWAN), 1);
    Engine_ActorSetAnimation(IWAN, 1);
    Engine_ActorJump(IWAN, 6, 40);
    Engine_ActorSetSpriteFlags(Object_GetById(MEARI), 1);
    Engine_ActorSetAnimation(MEARI, 1);
    Engine_ActorJump(MEARI, 6, 60);
    Actor_FaceDirection(GERALD, FACING_SOUTHWEST, 60);
    Engine_ActorSetAnimation(GERALD, 4);
    VinasuChojo_ShowMessage(GERALD);
    Actor_FaceDirection(IWAN, FACING_SOUTHWEST, 20);
    Engine_ActorSetAnimation(IWAN, 4);
    VinasuChojo_ShowMessage(IWAN);
    VinasuChojo_FaceActor(MEARI, FACING_SOUTHWEST);
    Engine_ActorSetAnimation(MEARI, 4);
    Event_ShowMessageAndWait(MEARI, 0, 80);
    VinasuChojo_FaceActor(GERALD, FACING_SOUTH);
    Event_ShowMessageAndWait(GERALD, 0, 40);
    Actor_FaceDirection(IWAN, FACING_NORTH, 20);
    Event_ShowMessageAndWait(IWAN, 0, 40);
    Engine_ActorStartRepeatedMotion(GERALD, 1);
    Engine_ActorRunRepeatedMotion(IWAN, 1);
    Engine_EventWait(20);
    Actor_FaceDirection(GERALD, FACING_SOUTHWEST, 0);
    Actor_FaceDirection(IWAN, FACING_SOUTHWEST, 20);
    Engine_ActorRunRepeatedMotion(MEARI, 1);
    Actor_FaceDirection(MEARI, FACING_NORTHEAST, 20);
    VinasuChojo_ShowMessage(MEARI);
    Actor_ShowEmote(GERALD, 0x100, 20);
    VinasuChojo_FaceActor(GERALD, FACING_SOUTH);
    VinasuChojo_ShowMessage(GERALD);
    Actor_FaceDirection(IWAN, FACING_NORTHWEST, 0);
    Actor_SetSpeed(GERALD, 0x19999, 0xcccc);
    Actor_WalkToAndWait(GERALD, 333, 194);
    Actor_WalkToAndWait(GERALD, 333, 206);
    Actor_FaceDirection(GERALD, FACING_EAST, 20);
    Engine_ActorRunRepeatedMotion(GERALD, 1);
    Event_ShowMessageAndWait(GERALD, 0, 80);
    Engine_ActorStop(ACTOR_PARTY_LEADER);
    Engine_TaskWait(1);
    actor->scale_x = 0x10000;
    actor->scale_y = 0x10000;
    Engine_EventWait(40);
    Actor_ShowEmote(ACTOR_PARTY_LEADER, 0x100, 80);
    VinasuChojo_FaceActor(GERALD, FACING_SOUTHEAST);
    Engine_ActorJump(GERALD, 4, 0);
    VinasuChojo_ShowMessage(GERALD);
    VinasuChojo_FaceActor(GERALD, FACING_NORTHEAST);
    Event_OpenMessage(GERALD, 0);
    if (Engine_EventChooseYesNo(0, 0) == 0) {
        Engine_EventWait(20);
        Engine_ActorSetAnimationAndWait(GERALD, 3);
        VinasuChojo_ShowMessage(GERALD);
        Engine_ActorRunRepeatedMotion(IWAN, 1);
        VinasuChojo_ShowMessage(IWAN);
        Engine_ActorSetAnimation(MEARI, 4);
        VinasuChojo_ShowMessage(MEARI);
        step_pending = 1;
    } else {
        gEventWork->message += 3;
        Engine_EventWait(60);
        VinasuChojo_ShowMessage(GERALD);
        Engine_ActorRunRepeatedMotion(IWAN, 1);
        VinasuChojo_ShowMessage(IWAN);
        Engine_ActorSetAnimation(MEARI, 4);
        VinasuChojo_ShowMessage(MEARI);
    }
    if (step_pending != 0) {
        gEventWork->message += 3;
    }
    VinasuChojo_FaceActor(ACTOR_PARTY_LEADER, FACING_SOUTH);
    Actor_SetAttachedEffect(ACTOR_PARTY_LEADER, 0x102);
    Engine_EventWait(80);
    Audio_PlayCue(17);
    Actor_ShowEmote(GERALD, 0x100, 20);
    Actor_FaceDirection(GERALD, FACING_NORTHWEST, 20);
    Actor_FaceDirection(GERALD, FACING_NORTHEAST, 40);
    Actor_FaceDirection(GERALD, FACING_WEST, 20);
    Actor_ShowEmote(IWAN, 0x101, 40);
    VinasuChojo_ShowMessage(IWAN);
    Actor_FaceDirection(ACTOR_PARTY_LEADER, FACING_SOUTHWEST, 0);
    Actor_FaceDirection(GERALD, FACING_SOUTHEAST, 20);
    Engine_ActorSetAnimation(GERALD, 4);
    VinasuChojo_ShowMessage(GERALD);
    Actor_FaceDirection(IWAN, FACING_NORTH, 0);
    Actor_FaceDirection(MEARI, FACING_WEST, 40);
    Actor_FaceDirection(IWAN, FACING_NORTHWEST, 0);
    Actor_FaceDirection(MEARI, FACING_NORTH, 40);
    Actor_SetAttachedEffect(MEARI, 0x102);
    Engine_EventWait(60);
    VinasuChojo_ShowMessage(MEARI);
    Audio_PlayCue(141);
    Work_SetValuesIfNonNegative(0x60000, 0x60000, 0x10000);
    Engine_EventWait(40);
    Audio_PlayCue(145);
    Map_CopyCellsTo(110, 105, 74, 4, 18, 23);
    Map_CopyCellsTo(92, 86, 83, 4, 8, 23);
    Map_CopyCellsTo(75, 28, 75, 4, 8, 23);
    Map_CopyCellsTo(92, 86, 11, 72, 16, 20);
    Map_CopyCellsTo(19, 92, 19, 68, 8, 21);
    actor = Object_GetById(ACTOR_PARTY_LEADER);
    actor->z.fixed -= PIXELS(32);
    actor->work = 0;
    SceneActor_ParkRecord(actor);
    actor = Object_GetById(GERALD);
    actor->x.fixed -= PIXELS(4);
    actor->z.fixed -= PIXELS(32);
    actor->work = 0;
    SceneActor_ParkRecord(actor);
    actor = Object_GetById(IWAN);
    actor->x.fixed -= PIXELS(4);
    actor->z.fixed -= PIXELS(32);
    actor->work = 0;
    SceneActor_ParkRecord(actor);
    actor = Object_GetById(MEARI);
    actor->x.fixed -= PIXELS(4);
    actor->z.fixed -= PIXELS(18);
    actor->work = 0;
    SceneActor_ParkRecord(actor);
    Object_GetById(23)->y.fixed = PIXELS(56);
    Camera_MoveTo(PIXELS(338), PIXELS(32), PIXELS(180), 0);
    Engine_MapRedraw();
    Engine_TaskWait(1);
    Actor_ShowEmote(ACTOR_PARTY_LEADER, 0x100, 0);
    Actor_ShowEmote(GERALD, 0x100, 0);
    Actor_ShowEmote(IWAN, 0x100, 0);
    Actor_ShowEmote(MEARI, 0x100, 0);
    Engine_ActorJump(ACTOR_PARTY_LEADER, 6, 0);
    Engine_ActorJump(GERALD, 6, 0);
    Engine_ActorJump(IWAN, 6, 0);
    Engine_ActorJump(MEARI, 6, 0);
    Engine_ActorEnableActionCallback(ACTOR_PARTY_LEADER, VinasuChojo_PartyHeadingAction);
    Engine_ActorEnableActionCallback(GERALD, VinasuChojo_PartyHeadingAction);
    Engine_ActorEnableActionCallback(IWAN, VinasuChojo_PartyHeadingAction);
    Engine_ActorEnableActionCallback(MEARI, VinasuChojo_PartyHeadingAction);
    Work_SetValuesIfNonNegative(0x30000, 0x30000, 0x10000);
    Engine_EventWait(80);
    Work_SetValuesIfNonNegative(0x60000, 0x60000, 0x10000);
    Engine_EventWait(40);
    Audio_PlayCue(145);
    Map_CopyCellAttributes(110, 106, 18, 14, 10, 5);
    Map_CopyCellsTo(110, 105, 74, 4, 18, 23);
    Map_CopyCellsTo(92, 86, 11, 68, 16, 20);
    actor = Object_GetById(ACTOR_PARTY_LEADER);
    actor->x.fixed -= PIXELS(16);
    SceneActor_ParkRecord(actor);
    actor = Object_GetById(GERALD);
    actor->x.fixed -= PIXELS(16);
    SceneActor_ParkRecord(actor);
    actor = Object_GetById(IWAN);
    actor->x.fixed -= PIXELS(16);
    SceneActor_ParkRecord(actor);
    actor = Object_GetById(MEARI);
    actor->x.fixed -= PIXELS(16);
    SceneActor_ParkRecord(actor);
    actor = Object_GetById(8);
    actor->x.fixed += PIXELS(16);
    SceneActor_ParkRecord(actor);
    actor = Object_GetById(9);
    actor->x.fixed += PIXELS(16);
    SceneActor_ParkRecord(actor);
    actor = Object_GetById(10);
    actor->x.fixed -= PIXELS(16);
    SceneActor_ParkRecord(actor);
    actor = Object_GetById(11);
    actor->x.fixed -= PIXELS(16);
    SceneActor_ParkRecord(actor);
    Camera_MoveTo(PIXELS(322), PIXELS(32), PIXELS(180), 0);
    Engine_MapRedraw();
    Engine_TaskWait(1);
    Actor_SetAttachedEffect(ACTOR_PARTY_LEADER, 0x102);
    Actor_SetAttachedEffect(GERALD, 0x102);
    Actor_SetAttachedEffect(IWAN, 0x102);
    Actor_SetAttachedEffect(MEARI, 0x102);
    Engine_ActorJump(ACTOR_PARTY_LEADER, 6, 0);
    Engine_ActorJump(GERALD, 6, 0);
    Engine_ActorJump(IWAN, 6, 0);
    Engine_ActorJump(MEARI, 6, 0);
    Work_SetValuesIfNonNegative(-1, -1, 0xe666);
    Engine_EventWait(80);
    Audio_PlayCue(0x121);
    Engine_EventWait(40);
    Engine_ActorStop(ACTOR_PARTY_LEADER);
    Engine_ActorStop(GERALD);
    Engine_ActorStop(IWAN);
    Engine_ActorStop(MEARI);
    Engine_EventWait(120);
    Actor_ShowEmote(IWAN, 0x105, 120);
    Engine_ActorRunRepeatedMotion(IWAN, 1);
    Actor_FaceDirection(IWAN, FACING_NORTHWEST, 20);
    VinasuChojo_ShowMessage(IWAN);
    Engine_ActorSetAnimationAndWait(GERALD, 3);
    Actor_FaceDirection(GERALD, FACING_SOUTHEAST, 0);
    VinasuChojo_ShowMessage(GERALD);
    Actor_FaceDirection(MEARI, FACING_NORTHEAST, 20);
    Engine_ActorSetAnimation(MEARI, 4);
    VinasuChojo_ShowMessage(MEARI);
    Engine_ActorJump(IWAN, 2, 20);
    VinasuChojo_ShowMessage(IWAN);
    Engine_ActorSetAnimation(GERALD, 3);
    Engine_ActorSetAnimation(IWAN, 3);
    Engine_ActorSetAnimationAndWait(MEARI, 3);
    Engine_EventWait(20);
    VinasuChojo_FaceActor(GERALD, FACING_EAST);
    VinasuChojo_ShowMessage(GERALD);
    Actor_FaceDirection(ACTOR_PARTY_LEADER, FACING_WEST, 20);
    Engine_ActorSetAnimationAndWait(ACTOR_PARTY_LEADER, 3);
    Engine_EventWait(20);
    Actor_SetSpeed(GERALD, 0x10000, 0x8000);
    Actor_SetSpeed(IWAN, 0x10000, 0x8000);
    Actor_SetSpeed(MEARI, 0x10000, 0x8000);
    Engine_ActorEnableActionCallback(GERALD, VinasuChojo_PartyExitAction);
    Engine_ActorEnableActionCallback(IWAN, VinasuChojo_PartyExitAction);
    Engine_ActorEnableActionCallback(MEARI, VinasuChojo_PartyExitAction);
    Engine_EventWait(60);
    Actor_SetSpeed(ACTOR_PARTY_LEADER, 0x10000, 0x8000);
    Actor_WalkToAndWait(ACTOR_PARTY_LEADER, 272, 216);
    Actor_WalkTo(ACTOR_PARTY_LEADER, 272, 254);
    Engine_EventWait(80);
    cell = (void **)&gEventWork;
    ((struct EventWork *)*cell)->start_transition = SCENE_TRANSITION(TRANSITION_WINDOW, 1);
    ((struct EventWork *)*cell)->transition_frames = 16;
    Engine_EventCloseScreen();
    /* FAKEMATCH: the window work is reached 12 cells below the event work's cell, as the scene's single pool word shows; naming gWindowWork adds a word. */
    cell -= 12;
    Engine_EventWaitForScreen();
    Engine_EventWait(80);
    Graphics_EnableObjLayerAndCallbacks();
    ((struct UiRenderWork *)*cell)->result[0] = 0;
    ((struct UiRenderWork *)*cell)->result[1] = 0;
    UiText_ShowCenteredMessage((s32)MsgVinasuDespiteLongTiring, 0, 0);
    ObjectDispatch_StopCallbacksAndHideLayers();
    Engine_EventWait(80);
}

void SceneActor_ParkRecord(struct FieldActor *actor)
{
    actor->target_x = ACTOR_NO_TARGET;
    actor->target_y = ACTOR_NO_TARGET;
    actor->target_z = ACTOR_NO_TARGET;

    actor->velocity_x = 0;
    actor->velocity_y = 0;
    actor->velocity_z = 0;

    actor->timer = 0;
}

void VinasuChojo_RunTransitionStep(void)
{
    struct FieldActor *center;
    struct FieldActor *effect;
    struct FieldSprite *sprite;
    s32 spawn;
    u32 rise;
    s32 angle;

    center = Actor_Get(23);
    spawn = 0;
    switch (VinasuChojo_TransitionStep) {
    case 0:
        Audio_PlayCue(220);
        Work_SetValuesIfNonNegative(0x60000, 0x60000, 0x10000);
        ColorBuffer_ApplyTarget(0x2063ff, 1);
        Engine_ColorBufferInterpolate(8);
        break;
    case 8:
        ColorBuffer_ApplyTarget(0x10000, 1);
        Engine_ColorBufferInterpolate(8);
        break;
    case 16:
        Work_SetValuesIfNonNegative(0x10000, 0x10000, 0x10000);
        break;
    case 24:
        center->x.fixed = 152 << 17;
        center->y.fixed = -0x1680000;
        center->z.fixed = 164 << 16;
        center->scale_x = 0x10000;
        center->scale_y = 0x10000;
        SceneActor_ParkRecord(center);
        Engine_ActorEnableActionCallback(23, VinasuChojo_RiseActionScript);
        break;
    case 25:
        VinasuChojo_TransitionStep--;
        if (center->y.fixed > 0) {
            ColorBuffer_ApplyTarget(0x203210, 0);
            Engine_ColorBufferInterpolate(16);
            VinasuChojo_TransitionStep++;
            Actor_Get(0)->rise_counter = 1;
            Actor_Get(1)->rise_counter = 1;
            Actor_Get(2)->rise_counter = 1;
            Actor_Get(3)->rise_counter = 1;
            Actor_Get(21)->rise_counter = 1;
            Actor_Get(6)->rise_counter = 1;
        } else {
            if ((gFrameCount & 7) == 0)
                Audio_PlayCue(246);
            spawn = 1;
            center->y.fixed += 0x24000;
        }
        break;
    case 26:
        VinasuChojo_TransitionStep--;
        if (center->y.fixed > (160 << 14)) {
            ColorBuffer_ApplyTarget(0x10000, 0);
            Engine_ColorBufferInterpolate(40);
            VinasuChojo_TransitionStep++;
        } else {
            if ((gFrameCount & 7) == 0)
                Audio_PlayCue(246);
            spawn = 1;
            center->y.fixed += 0x24000;
        }
        break;
    case 27:
    case 28:
    case 29:
    case 30:
    case 31:
    case 32:
    case 33:
    case 34:
        spawn = 1;
        break;
    case 36:
        Audio_PlayCue(187);
        ColorBuffer_ApplyTarget(0x7fff, 0);
        Engine_ColorBufferInterpolate(12);
        break;
    case 48:
        Engine_ActorStop(23);
        GameFlag_Set(0x237);
        break;
    }
    if (spawn) {
        rise = ((u32)(Random_Next() * 80) >> 16) << 16;
        effect = Object_Create(284, center->x.fixed, center->y.fixed - rise + (s32)0xfff80000, center->z.fixed);
        if (effect != 0) {
            sprite = effect->sprite;
            Object_SetScript(effect, VinasuChojo_RiseParticleScript);
            ObjectGroup_SetChildValue(effect, 1);
            effect->motion_flags = 0;
            angle = Random_Next() & 0xffff000;
            effect->timer = angle;
            effect->work = 0;
            effect->rise_counter = (u32)Random_Next() >> 13;
            effect->update = (void (*)(union FieldObject *))SceneEffect_UpdateCounterDrivenOrbit;
            effect->speed = Engine_MathSin((u32)(Random_Next() * 0xffff) >> 20) * 24;
            effect->speed = center->speed >> 16;
            sprite->flags = 0;
            sprite->priority = 1;
        }
    }
    VinasuChojo_TransitionStep++;
    SceneEffect_AdvanceGatedRiseCounter(Actor_Get(0));
    SceneEffect_AdvanceGatedRiseCounter(Actor_Get(1));
    SceneEffect_AdvanceGatedRiseCounter(Actor_Get(2));
    SceneEffect_AdvanceGatedRiseCounter(Actor_Get(3));
    SceneEffect_AdvanceGatedRiseCounter(Actor_Get(21));
    SceneEffect_AdvanceGatedRiseCounter(Actor_Get(6));
}

void SceneEffect_AdvanceGatedRiseCounter(struct FieldActor *actor)
{
    if (actor->rise_enabled != 0) {
        u8 counter = actor->rise_counter;

        actor->y.fixed = actor->rise_base_y + ((u32)(counter >> 2) << 16);

        SceneActor_ParkRecord(actor);

        if (actor->rise_counter != 0) {
            if (actor->rise_counter <= 31) {
                ++actor->rise_counter;
            }
        }
    }
}

void SceneEffect_SpawnParticlesBesideActor(void)
{
    struct FieldActor *center;
    struct FieldActor *effect;
    struct FieldSprite *sprite;
    struct MapScrollWork *map;
    s32 drift;
    s32 phase;
    s32 angle;

    center = Actor_Get(23);
    map = gMapWork[0];
    drift = (u32)(Random_Next() * 48) >> 16 << 16;
    if ((s16)(map->view_y >> 16) <= 129) {
        if (gFrameCount & 1) {
            Actor_SetPosition(23, 152 << 17, 164 << 16);
            Actor_Get(23)->scale_x = 0x10000;
            Actor_Get(23)->scale_y = 0x10000;
        } else {
            Actor_SetPosition(23, 152 << 17, 171 << 16);
            Actor_Get(23)->scale_x = 0x14ccc;
            Actor_Get(23)->scale_y = 0x14ccc;
        }
    } else {
        Actor_SetPosition(23, 0, 0);
    }
    if (center == 0)
        return;
    phase = gFrameCount & 15;
    if (phase != 0)
        return;
    effect = Object_Create(284, center->x.fixed + 0x80000, center->y.fixed + drift + 0x80000,
                                 center->z.fixed);
    drift /= 0x60000;
    drift <<= 16;
    if (effect == 0)
        return;
    sprite = effect->sprite;
    Object_SetScript(effect, VinasuChojo_BesideParticleScript);
    ObjectGroup_SetChildValue(effect, 5);
    effect->motion_flags = phase;
    angle = Random_Next() & 0xffff000;
    effect->timer = angle;
    effect->work = phase;
    *(struct FieldActor **)effect->unknown_68 = center;
    effect->update = VinasuChojo_UpdateOrbitingEffect;
    effect->speed = Engine_MathSin((drift & 0xfffff) >> 4) * 24 >> 16;
    /* FAKEMATCH: the byte store outside the sprite record keeps the game's order. */
    *(u8 *)((u8 *)sprite + 38) = 0;
    sprite->priority = center->sprite->priority;
}

void VinasuChojo_UpdateOrbitingEffect(union FieldObject *object)
{
    struct FieldEffect *anchor;
    s32 spin;

    anchor = *(struct FieldEffect **)((u8 *)object + 104);
    spin = object->effect.spin;
    object->effect.x = anchor->x + Engine_MathCos(spin) * (*(s32 *)((u8 *)object + 48) + 28);
    object->effect.z = (Engine_MathSin(spin) << 4) + 0xa40000;
    *(s32 *)((u8 *)object + 56) = object->effect.x;
    *(s32 *)((u8 *)object + 64) = object->effect.z;
    object->effect.spin -= 0x200;
}
