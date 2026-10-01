#include "TYPES.H"
#include "CALLBACK_SCHEDULER.H"
#include "FIELD_EVENT.H"
#include "VINASU.H"
/* Bracketed scenes, the pair's defeat and the multi-actor presentation. */
#include "CHOJO.H"
#include "SCENE_IDS.H"
#include "CALL.H"
#include "FIELD_SCENE.H"
#include "STAGED_ACTOR.H"
#include "MAP_SCROLL.H"
#include "TBS_EDITION.H"

extern u8 MsgVinasuPairDefeated[];
void SceneEffect_SpawnParticlesAboveActor(void);

extern u8 Data_0200e088[];
extern u8 Data_0200e130[];
void SceneEffect_SpawnParticlesAboveActor();
void VinasuChojo_RunTransitionStep();

/* The transition task's frame count and step, past the overlay's image. */
extern s32 VinasuChojo_TransitionTimer;
extern s32 VinasuChojo_TransitionStep;
extern u8 Data_0200e0d0[];
extern u8 Data_0200e0f4[];
void Engine_EventWait();
void VinasuChojo_ShowMessage();
void Object_SetActionCallbackAndRefreshById();
void SceneActor_ParkRecord();
void Event_SetPairWork1c0();
void ObjectTable_Snapshot();

/* FAKEMATCH: the shared zero lives in a one-halfword struct so it is a
 * HImode register; its pool load then has the movhi reach of 64 bytes the
 * reference pool placement needs. */
struct Half {
    u16 v;
};

extern u8 MsgVinasuNoooo[];

enum {
    PARTICLE_SOURCE_ACTOR = 23
};

void SceneEffect_SpawnParticlesBesideActor(void);

extern u8 MsgVinasuDespiteLongTiring[];
extern u8 MsgVinasuWhyHappeningProtectVenusLighthouse[];

struct SceneWork {
    u8 unknown_000[0x1c0];
    s32 request;
    u8 unknown_1c4[4];
    s32 setup;
};

extern struct SceneWork *Data_03001ebc;
void OverlayObject_DecayRecordField1e(u8 *);
extern u8 Data_0200e324[];
extern u8 Data_0200e360[];
extern u8 Data_0200e074[];
extern u8 Data_0200e3c0[];
extern u8 Data_0200e39c[];
void ObjectDispatch_StopCallbacksAndHideLayers();
void UiText_ShowCenteredMessage();
void Graphics_EnableObjLayerAndCallbacks();
void VinasuChojo_FaceActor();
void Object_RefreshSelectorById();

extern const u8 VinasuChojo_RiseActionScript[];
extern const s32 VinasuChojo_RiseParticleScript[];
void SceneEffect_UpdateCounterDrivenOrbit(u8 *actor);
void SceneEffect_AdvanceGatedRiseCounter(u8 *obj);

extern struct MapScrollWork *gMapWork;
extern const s32 VinasuChojo_BesideParticleScript[];
void FieldScene_RunScene3c9_02005b90(union FieldObject *object);

/* Once the second actor stops, grow and slide the pair into their shared form. */
void VinasuChojo_UpdateBeamActors(void)
{
    struct FieldActor *first;
    struct FieldActor *second;
    s32 stopped;

    first = Object_GetById(ACTOR_FIRST_OF_PAIR);
    second = Object_GetById(ACTOR_SECOND_OF_PAIR);
    /* FAKEMATCH: the dead first test and the word temporaries below reproduce the
     * reference's leftover loads and stores. */
    stopped = first->target_x == ACTOR_NO_TARGET && first->target_y == first->target_x && first->target_z == first->target_y;
    if (second->target_x == ACTOR_NO_TARGET && second->target_y == second->target_x && second->target_z == second->target_y) {
        stopped = 1;
    } else {
        stopped = 0;
    }
    if (stopped != 0) {
        {
            s32 shown = 0;
        
            first->facing = shown;
        }
        {
            s32 shown = 0;
        
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

/*
 * Brackets a scripted scene: opens it, runs two of this overlay's own
 * steps, sets the story flag, writes the workspace phase and timer, then
 * closes. The 72-byte owner includes its alignment halfword and one pool
 * word. No incoming argument is read before being overwritten, so this is
 * void.
 */
void FieldScene_RunBracketedSceneWithFlag282(void)
{

    u8 *workspace;

    Engine_EventBegin();
    FieldScene_RestageParty();
    FieldScene_RunScene3c9_02004b28();
    /* The flag id is built as 141 << 1 rather than folded. */
    GameFlag_Set(141 << 1);

    workspace = ((u8 *)Data_03001ebc);
    *(s32 *)(workspace + 448) = 512;
    *(s32 *)(workspace + 456) = 24;

    Engine_EventCloseScreen();
    Engine_EventWaitForScreen();
    Engine_EventRequestExit(1);
    Engine_EventEnd();
}

void FieldScene_RunScene3c9_02003924(void)
{
    u32 i;
    s32 rec4;
    s32 record;

    rec4 = Object_GetById(ACTOR_PARTY_LEADER);
    Engine_EventBegin();
    *((u8 *)Engine_EventGetViewCenter() + 85) = 0;
    Map_CopyCellsTo(102, 4, 74, 4, 18, 23);
    Map_CopyCellsTo(39, 72, 11, 72, 16, 20);
    Map_CopyCellAttributes(19, 6, 3, 7, 22, 6);
    Map_CopyCellAttributes(19, 6, 3, 7, 13, 6);
    Map_CopyCellAttributes(19, 6, 3, 7, 22, 13);
    Map_CopyCellAttributes(19, 6, 3, 7, 13, 13);
    Engine_TaskWait(1);
    Camera_MoveTo(0xc00000, -0x400000, 0xee0000, 0);
    Engine_TaskWait(1);
    Engine_MapRedraw();
    Engine_TaskWait(1);
    Engine_ActorDestroy(20);
    Engine_ActorDestroy(19);
    Engine_ActorSetAnimation(ACTOR_PARTY_LEADER, 19);
    record = Actor_Get(ACTOR_PARTY_LEADER);
    Engine_ActorSetSpriteFlags(record, 0);
    *(s32 *)(rec4 + 8) = 0x15a0000;
    *(s32 *)(rec4 + 16) = 0xcd0000;
    *(s32 *)(rec4 + 12) = 0x200000;
    {
        s32 shown = 0x6000;

        *(u16 *)(rec4 + 6) = shown;
    }
    SceneActor_ParkRecord(rec4);
    Engine_ActorSetAnimation(ACTOR_GERALD, 18);
    record = Actor_Get(ACTOR_GERALD);
    Engine_ActorSetSpriteFlags(record, 0);
    record = Actor_Get(ACTOR_GERALD);
    *(s32 *)(record + 8) = 0x1640000;
    *(s32 *)(record + 16) = 0xc00000;
    {
        s32 shown = 0xa000;

        *(u16 *)(record + 6) = shown;
    }
    *(s32 *)(record + 12) = 0x200000;
    SceneActor_ParkRecord((u8 *)record);
    Engine_ActorSetAnimation(ACTOR_IVAN, 18);
    record = Actor_Get(ACTOR_IVAN);
    Engine_ActorSetSpriteFlags(record, 0);
    record = Actor_Get(ACTOR_IVAN);
    *(s32 *)(record + 8) = 0x1680000;
    {
        s32 shown = 0x2000;

        *(u16 *)(record + 6) = shown;
    }
    *(s32 *)(record + 12) = 0x200000;
    *(s32 *)(record + 16) = 0xde0000;
    SceneActor_ParkRecord((u8 *)record);
    Engine_ActorSetAnimation(ACTOR_MIA, 18);
    record = Actor_Get(ACTOR_MIA);
    Engine_ActorSetSpriteFlags(record, 0);
    record = Actor_Get(ACTOR_MIA);
    *(s32 *)(record + 8) = 0x14e0000;
    {
        s32 shown = 0x8000;

        *(u16 *)(record + 6) = shown;
    }
    *(s32 *)(record + 12) = 0x200000;
    *(s32 *)(record + 16) = 0xde0000;
    SceneActor_ParkRecord((u8 *)record);
    Actor_SetPosition(21, 0xc40000, 0xdc0000);
    Engine_ActorSetAnimation(21, 5);
    Actor_SetPosition(6, 0xbc0000, 0x13c0000);
    Engine_ActorSetAnimation(6, 5);
    record = Actor_Get(6);
    Engine_ActorSetSpriteFlags(record, 0);
    record = Actor_Get(8);
    *(s32 *)(record + 8) += -0x100000;
    SceneActor_ParkRecord((u8 *)record);
    record = Object_GetById(9);
    *(s32 *)(record + 8) += -0x100000;
    SceneActor_ParkRecord((u8 *)record);
    record = Actor_Get(10);
    *(s32 *)(record + 8) += 0x100000;
    SceneActor_ParkRecord((u8 *)record);
    record = Object_GetById(11);
    *(s32 *)(record + 8) += 0x100000;
    SceneActor_ParkRecord((u8 *)record);
    Work_SetValuesIfNonNegative(0x10000, 0x10000, 0x10000);
    record = Actor_Get(23);
    Engine_ActorSetSpriteFlags(record, 0);
    *(u8 *)((u8 *)Object_GetById(23) + 85) = 4;
    Actor_SetChildValue(23, 4);
    record = Actor_Get(23);
    *(s32 *)(record + 12) = 0x280000;
    ((void (*)())Engine_TaskAddCallback)((s32)SceneEffect_SpawnParticlesBesideActor, 0xc80);
    gEventWork->start_transition = SCENE_TRANSITION(TRANSITION_WINDOW, 0);
    gEventWork->transition_frames = 24;
    Engine_EventOpenScreen();
    Engine_EventWaitForScreen();
    Engine_EventWait(40);
    Scene_RunExtendedActorTransition();
    GameFlag_Set(0x9a7);
    Engine_EventRequestExit(2);
}

/*
 * The pair's defeat. Actors 0 to 3 are placed facing northwest, actors 21
 * and 6 between north and northeast, and the pair between south and
 * southeast. Actors 24 and 25 are prepared, the particle task starts and the
 * screen opens. The pair speak, then each falls away in three poses and is
 * removed.
 */
void FieldScene_RunPairDefeat(void)
{
    struct FieldActor *actor;

    Audio_PlayCue(141);
    Map_CopyCellAttributes(17, 10, 4, 2, 17, 8);
    Actor_Get(ACTOR_PARTY_LEADER)->facing = FACING_NORTHWEST;
    Actor_Get(ACTOR_GERALD)->facing = FACING_NORTHWEST;
    Actor_SetPosition(ACTOR_GERALD, PIXELS(328), PIXELS(168));
    Actor_Get(ACTOR_IVAN)->facing = FACING_NORTHWEST;
    Actor_SetPosition(ACTOR_IVAN, PIXELS(340), PIXELS(196));
    Actor_Get(ACTOR_MIA)->facing = FACING_NORTHWEST;
    Actor_SetPosition(ACTOR_MIA, PIXELS(326), PIXELS(204));
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

    Engine_ActorRunRepeatedMotion(ACTOR_GERALD, 1);
    Engine_EventWait(20);
    Event_ShowMessageAndWait(ACTOR_GERALD, 0, 20);
    Engine_ActorRunRepeatedMotion(ACTOR_IVAN, 1);
    Engine_EventWait(20);
    Event_OpenMessage(ACTOR_IVAN, 0);
    Actor_FaceDirection(ACTOR_IVAN, 0xc000, 0);
    VinasuChojo_FaceActor(0, 0x4000);
    count_flag = 0;
    if (Engine_EventChooseYesNo(0, 0) == 0) {
        Engine_EventWait(20);
        Engine_ActorSetAnimationAndWait(ACTOR_IVAN, 4);
        count_flag = 1;
    } else {
        Engine_EventWait(20);
        Engine_ActorRunRepeatedMotion(ACTOR_IVAN, 1);
        *(u16 *)((*(s32 *)&gEventWork + 0x1d8)) += 1;
    }
    VinasuChojo_ShowMessage(2);
    if (count_flag != 0) {
        *(u16 *)((*(s32 *)&gEventWork + 0x1d8)) += 1;
    }
    Actor_SetSpeed(ACTOR_GERALD, 0x19999, 0xcccc);
    request_a = 0x1001;
    Actor_WalkToAndWait(ACTOR_GERALD, 0x141, 174);
    VinasuChojo_FaceActor(1, 0x2000);
    VinasuChojo_ShowMessage(request_a);
    Actor_FaceDirection(ACTOR_PARTY_LEADER, 0xa000, 0);
    Actor_FaceDirection(ACTOR_IVAN, 0xa000, 0);
    VinasuChojo_FaceActor(3, 0xc000);
    VinasuChojo_FaceActor(1, 0x4000);
    VinasuChojo_ShowMessage(request_a);
    Actor_ShowEmote(ACTOR_GERALD, 0x105, 40);
    Actor_FaceDirection(ACTOR_GERALD, 0x2000, 20);
    VinasuChojo_ShowMessage(request_a);
    Actor_FaceDirection(ACTOR_GERALD, 0xa000, 40);
    Event_ShowMessageAndWait(0x8001, 0, 40);
    Actor_FaceDirection(ACTOR_GERALD, 0x2000, 20);
    VinasuChojo_ShowMessage(request_a);
    Audio_PlayCue(17);
    Engine_ActorSetAnimation(ACTOR_MIA, 4);
    VinasuChojo_ShowMessage(3);
    Actor_FaceDirection(ACTOR_GERALD, 0x4000, 80);
    Actor_FaceDirection(ACTOR_MIA, 0x6000, 80);
    Actor_ShowEmote(ACTOR_MIA, 0x100, 40);
    Actor_FaceDirection(ACTOR_PARTY_LEADER, 0x6000, 60);
    Engine_ActorRunRepeatedMotion(ACTOR_PARTY_LEADER, 2);
    VinasuChojo_FaceActor(2, 0xc000);
    Actor_ShowEmote(ACTOR_IVAN, 0x101, 60);
    Actor_FaceDirection(ACTOR_IVAN, 0x6000, 40);
    Engine_ActorJump(ACTOR_IVAN, 4, 60);
    VinasuChojo_FaceActor(1, 0x2000);
    Actor_ShowEmote(ACTOR_GERALD, 0x101, 40);
    VinasuChojo_ShowMessage(request_a);
    Actor_FaceDirection(ACTOR_GERALD, 0x6000, 40);
    Actor_ShowEmote(ACTOR_GERALD, 0x100, 0);
    Engine_ActorJump(ACTOR_GERALD, 4, 40);
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
    Engine_ActorStartRepeatedMotion(ACTOR_MIA, 2);
    VinasuChojo_ShowMessage(request_b);
    Engine_ActorRunRepeatedMotion(21, 2);
    Engine_EventWait(20);
    VinasuChojo_ShowMessage(0x2002);
    VinasuChojo_FaceActor(21, 0xe000);
    Engine_ActorRunRepeatedMotion(ACTOR_GERALD, 1);
    VinasuChojo_ShowMessage(1);
    Engine_ActorRunRepeatedMotion(21, 1);
    Event_ShowMessageAndWait(21, 0, 20);
    VinasuChojo_ShowMessage(request_b);
    Engine_ActorSetAnimationAndWait(21, 4);
    VinasuChojo_ShowMessage(21);
    Actor_SetAttachedEffect(ACTOR_IVAN, 0x102);
    VinasuChojo_ShowMessage(0x2002);
    Engine_ActorSetAnimation(21, 3);
    VinasuChojo_ShowMessage(21);
    Actor_ShowEmote(ACTOR_GERALD, 0x103, 40);
    Engine_ActorStartRepeatedMotion(ACTOR_GERALD, 2);
    VinasuChojo_ShowMessage(1);
    Engine_ActorSetAnimation(21, 4);
    VinasuChojo_ShowMessage(21);
    Actor_FaceDirection(21, 0x5000, 20);
    Engine_ActorJump(ACTOR_MIA, 4, 20);
    Event_ShowMessageAndWait(request_b, 0, 20);
    Actor_ShowEmote(21, 0x103, 40);
}

void VinasuChojo_RunActorTransition(void)
{
    struct Half zero;
    struct FieldActor *actor26;
    struct FieldActor *actor27;
    struct FieldActor *actor28;
    u8 *record;
    u8 *action_start;
    s32 none;
    u8 *action_next;
    u8 *action_end;

    Engine_AudioPlayCue(19);
    Engine_AudioPlayCue(0x120);
    Call3(Engine_WorkSetValuesIfNonNegative, 0x30000, 0x30000, 0x10000);
    Call3(Engine_ActorShowEmote, 0, 0x100, 0);
    Call3(Engine_ActorShowEmote, 1, 0x100, 0);
    Call3(Engine_ActorShowEmote, 2, 0x100, 0);
    Call3(Engine_ActorShowEmote, 3, 0x100, 0);
    Call3(Engine_ActorShowEmote, 21, 0x100, 0);
    Call3(Engine_ActorShowEmote, 6, 0x100, 10);
    Call3(Engine_ActorFaceDirection, 0, 0xa000, 0);
    Call3(Engine_ActorFaceDirection, 1, 0xa000, 0);
    Call3(Engine_ActorFaceDirection, 2, 0xa000, 0);
    Call3(Engine_ActorFaceDirection, 3, 0xa000, 0);
    Call3(Engine_ActorFaceDirection, 21, 0xd000, 0);
    Call3(Engine_ActorFaceDirection, 6, 0xd000, 0);
    Call3(Engine_WorkSetValuesIfNonNegative, 0x20000, 0x20000, 0x10000);
    Engine_EventWait(10);
    Call3(Engine_WorkSetValuesIfNonNegative, 0x10000, 0x10000, 0x10000);
    Call4(Engine_CameraMoveTo, 0x1300000, 0x200000, 0xb40000, 1);
    Engine_CameraWaitForMove();
    record = Object_GetById(24);
    *(s32 *)(record + 24) = 0x1999;
    record = Object_GetById(25);
    *(s32 *)(record + 24) = 0x1999;
    action_start = Data_0200e088;
    Engine_ActorEnableActionCallback(24, action_start);
    Engine_ActorEnableActionCallback(25, action_start);
    Engine_AudioPlayCue(145);
    Call3(Engine_WorkSetValuesIfNonNegative, 0x60000, 0x60000, 0x10000);
    Call2(Engine_ColorBufferApplyTarget, 0x4063ff, 0);
    Engine_ColorBufferInterpolate(16);
    Engine_TaskWait(20);
    Engine_ColorBufferApplyTarget(0x7fff, 0);
    Engine_ColorBufferInterpolate(24);
    Engine_TaskWait(60);
    Engine_AudioPlayCue(141);
    Engine_GameFlagSet(0x236);
    record = Object_GetById(24);
    *(s32 *)(record + 12) = -0x600000;
    record = Object_GetById(25);
    *(s32 *)(record + 12) = -0x400000;
    Engine_ActorSetChildValue(26, 7);
    record = Object_GetById(26);
    Engine_ActorSetSpriteFlags(record, 0);
    actor26 = Object_GetById(26);
    actor26->scale_y = -0x10000;
    record = Object_GetById(24);
    actor26->scale_x = *(s32 *)(record + 24);
    none = 0;
    actor26->motion_flags = none;
    actor26->x.fixed = 0x1300000;
    actor26->y.fixed = -0x200000;
    actor26->z.fixed = 0x600000;
    Engine_ActorSetChildValue(27, 7);
    record = Object_GetById(27);
    Engine_ActorSetSpriteFlags(record, 0);
    actor27 = Object_GetById(27);
    actor27->scale_y = -0x10000;
    record = Object_GetById(24);
    actor27->scale_x = *(s32 *)(record + 24);
    actor27->motion_flags = none;
    actor27->x.fixed = 0x1300000;
    actor27->y.fixed = none;
    actor27->z.fixed = 0x600000;
    Engine_ActorSetChildValue(28, 7);
    record = Object_GetById(28);
    Engine_ActorSetSpriteFlags(record, 0);
    actor28 = Object_GetById(28);
    actor28->scale_y = -0x10000;
    record = Object_GetById(24);
    actor28->scale_x = *(s32 *)(record + 24);
    actor28->motion_flags = none;
    actor28->x.fixed = 0x1300000;
    actor28->y.fixed = 0x200000;
    actor28->z.fixed = 0x600000;
    Call6(Engine_MapCopyCellsTo, 102, 4, 74, 4, 18, 23);
    Call6(Engine_MapCopyCellsTo, 39, 72, 11, 72, 16, 21);
    Engine_MapCopyCellAttributes(19, 6, 3, 7, 22, 6);
    Engine_MapCopyCellAttributes(19, 6, 3, 7, 13, 6);
    Engine_MapCopyCellAttributes(19, 6, 3, 7, 22, 13);
    Engine_MapCopyCellAttributes(19, 6, 3, 7, 13, 13);
    Engine_TaskWait(1);
    record = Object_GetById(8);
    *(s32 *)(record + 8) += -0x100000;
    SceneActor_ParkRecord(record);
    record = Object_GetById(9);
    *(s32 *)(record + 8) += -0x100000;
    SceneActor_ParkRecord(record);
    record = Object_GetById(10);
    *(s32 *)(record + 8) += 0x100000;
    SceneActor_ParkRecord(record);
    record = Object_GetById(11);
    *(s32 *)(record + 8) += 0x100000;
    SceneActor_ParkRecord(record);
    record = Object_GetById(0);
    *(s32 *)(record + 8) += 0x100000;
    *(s32 *)(record + 16) += 0x100000;
    SceneActor_ParkRecord(record);
    record = Object_GetById(1);
    *(s32 *)(record + 8) += 0x100000;
    *(s32 *)(record + 16) += 0x100000;
    SceneActor_ParkRecord(record);
    record = Object_GetById(2);
    *(s32 *)(record + 8) += 0x100000;
    *(s32 *)(record + 16) += 0x100000;
    SceneActor_ParkRecord(record);
    record = Object_GetById(3);
    *(s32 *)(record + 8) += 0x100000;
    *(s32 *)(record + 16) += 0x100000;
    SceneActor_ParkRecord(record);
    Call3(Engine_ActorSetPosition, 21, 0xc40000, 0xdc0000);
    Engine_ActorSetAnimation(21, 5);
    Call3(Engine_ActorSetPosition, 6, 0xbc0000, 0x13c0000);
    Engine_ActorSetAnimation(6, 5);
    record = Object_GetById(6);
    Engine_ActorSetSpriteFlags(record, 0);
    Call3(Engine_WorkSetValuesIfNonNegative, 0x40000, 0x40000, 0x10000);
    Engine_ColorBufferApplyTarget(0x4063ff, 0);
    Engine_ColorBufferInterpolate(120);
    action_next = Data_0200e0d0;
    Engine_ActorEnableActionCallback(24, action_next);
    Engine_ActorEnableActionCallback( 25, action_next);
    Engine_ActorEnableActionCallback( 26, action_next);
    Engine_ActorEnableActionCallback( 27, action_next);
    Engine_ActorEnableActionCallback(28, action_next);
    Engine_TaskWait(120);
    Call3(Engine_WorkSetValuesIfNonNegative, 0x30000, 0x30000, 0x10000);
    Engine_ColorBufferApplyTarget(0x203210, 0);
    Engine_ColorBufferInterpolate(120);
    Engine_TaskWait(120);
    Call3(Engine_WorkSetValuesIfNonNegative, 0x20000, 0x20000, 0x10000);
    Call2(Engine_ColorBufferApplyTarget, 0x10000, 0);
    Engine_ColorBufferInterpolate(120);
    Engine_TaskWait(120);
    Call3(Engine_WorkSetValuesIfNonNegative, 0x10000, 0x10000, 0x10000);
    record = Object_GetById(24);
    action_end = Data_0200e0f4;
    *(s32 *)(record + 28) = 0x51e;
    Engine_ActorEnableActionCallback(25, action_end);
    Engine_ActorEnableActionCallback( 26, action_end);
    Engine_ActorEnableActionCallback( 27, action_end);
    Object_SetActionCallbackAndRefreshById(28, action_end);
    Engine_AudioPlayCue(0x121);
    Engine_ActorSetChildValue(24, 15);
    Engine_EventWait(20);
    Object_SetActionCallbackAndRefreshById(24, (s32)Data_0200e130);
    Scheduler_RemoveCallback((u32)((s32)SceneEffect_SpawnParticlesAboveActor));
    Engine_ActorJump(2, 2, 20);
    VinasuChojo_ShowMessage(2);
    Call3(Engine_ActorFaceDirection, 1, 0x6000, 20);
    Call2(Engine_ActorSetAttachedEffect, 1, 0x102);
    Engine_EventWait(20);
    Engine_ActorStartRepeatedMotion(1, 2);
    VinasuChojo_ShowMessage(1);
    Call3(Engine_ActorSetSpeed, 3, 0xcccc, 0x6666);
    Engine_ActorWalkToAndWait(3, 0x146, 220);
    Engine_EventWait(40);
    Engine_ActorSetAttachedEffect(3, 0x102);
    VinasuChojo_ShowMessage(3);
    record = ((u8 *)Value1(Object_GetById, 0));
    record[98] = none;
    *(u8 *)((record + 98) + 1) = 1;
    *(s32 *)(record + 76) = *(s32 *)(record + 12);
    zero.v = 0;
    {
        s32 shown = 0xa000;

        *(u16 *)(record + 6) = shown;
    }
    record = ((u8 *)Value1(Object_GetById, 1));
    record[98] = zero.v;
    *(u8 *)((record + 98) + 1) = 1;
    {
        s32 shown = 0xa000;

        *(u16 *)(record + 6) = shown;
    }
    *(s32 *)(record + 76) = *(s32 *)(record + 12);
    record = ((u8 *)Value1(Object_GetById, 2));
    record[98] = zero.v;
    *(u8 *)((record + 98) + 1) = 1;
    {
        s32 shown = 0xa000;

        *(u16 *)(record + 6) = shown;
    }
    *(s32 *)(record + 76) = *(s32 *)(record + 12);
    record = ((u8 *)Value1(Object_GetById, 3));
    record[98] = zero.v;
    *(u8 *)((record + 98) + 1) = 1;
    {
        s32 shown = 0xa000;

        *(u16 *)(record + 6) = shown;
    }
    *(s32 *)(record + 76) = *(s32 *)(record + 12);
    record = ((u8 *)Value1(Object_GetById, 21));
    record[98] = zero.v;
    *(u8 *)((record + 98) + 1) = 1;
    *(s32 *)(record + 76) = *(s32 *)(record + 12);
    record = ((u8 *)Value1(Object_GetById, 6));
    record[98] = zero.v;
    *(u8 *)((record + 98) + 1) = 1;
    *(s32 *)(record + 76) = *(s32 *)(record + 12);
    *(u8 *)((u8 *)Object_GetById(23) + 85) = zero.v;
    record = Object_GetById(23);
    Engine_ActorSetSpriteFlags(record, 0);
    Engine_ActorSetChildValue(23, 7);
    Engine_ActorSetSpritePriority(23, 2);
    VinasuChojo_TransitionStep = none;
    VinasuChojo_TransitionTimer = 240;
    ((void (*)())Engine_TaskAddCallback)((s32)VinasuChojo_RunTransitionStep, 0xc80);
    do {
        Engine_TaskWait(1);
    } while (Engine_GameFlagIsSet(0x237) == 0);
    Engine_GameFlagSet(0x101);
    Engine_EventWait(30);
    Engine_GameFlagSet(0x11a);
    ObjectTable_Snapshot();
    Event_SetPairWork1c0((s32)&SceneId_WorldMap, 91);
    { s32 white = 0x7fff; *(u16 *)0x05000000 = white; }
    *(s32 *)((*(s32 *)&gEventWork + 0x1c8)) = 1;
    Engine_EventCloseScreen();
    Engine_EventWaitForScreen();
}

/* Restaging the party. */

/*
 * Restages the aerie after the pair's defeat: both of the pair are removed,
 * map cells are copied, the camera and actors are refreshed, and actors 0 to
 * 3 are placed with their rise stopped. The rise counters of actors 21 and 6
 * are cleared, the particle task for actor 23 starts, and the palette is
 * blended in from white over 40 frames.
 */
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
    Engine_ActorSetAnimation(ACTOR_GERALD, 18);
    Engine_ActorSetAnimation(ACTOR_IVAN, 18);
    Engine_ActorSetAnimation(ACTOR_MIA, 18);
    Engine_ActorSetSpriteFlags(Actor_Get(ACTOR_PARTY_LEADER), 0);
    Engine_ActorSetSpriteFlags(Actor_Get(ACTOR_GERALD), 0);
    Engine_ActorSetSpriteFlags(Actor_Get(ACTOR_IVAN), 0);
    Engine_ActorSetSpriteFlags(Actor_Get(ACTOR_MIA), 0);

    actor = Actor_Get(ACTOR_PARTY_LEADER);
    actor->x.fixed = PIXELS(346);
    actor->y.fixed = PIXELS(32);
    actor->z.fixed = PIXELS(205);
    SceneActor_ParkRecord((u8 *)actor);
    actor->rise_enabled = 0;
    actor->velocity_y = 0x20000;

    actor = Actor_Get(ACTOR_GERALD);
    actor->x.fixed = PIXELS(356);
    actor->y.fixed = PIXELS(32);
    actor->z.fixed = PIXELS(192);
    SceneActor_ParkRecord((u8 *)actor);
    actor->rise_enabled = 0;
    actor->velocity_y = 0x20000;

    actor = Actor_Get(ACTOR_IVAN);
    actor->x.fixed = PIXELS(360);
    actor->y.fixed = PIXELS(32);
    actor->z.fixed = PIXELS(222);
    SceneActor_ParkRecord((u8 *)actor);
    actor->rise_enabled = 0;
    actor->velocity_y = 0x20000;

    actor = Actor_Get(ACTOR_MIA);
    actor->x.fixed = PIXELS(334);
    actor->y.fixed = PIXELS(32);
    actor->z.fixed = PIXELS(222);
    SceneActor_ParkRecord((u8 *)actor);
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

void FieldScene_RunScene3c9_02004b28(void)
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

/* Stages the long actor transition, takes one of two query-selected branches
 * that both advance the scene step counter by three, then posts the next scene
 * request and clears two halfwords of the record whose pointer cell lies 48
 * bytes before the scene work cell. */
void Scene_RunExtendedActorTransition(void)
{
    u8 *rec;
    u8 *record;
    s32 none;
    s32 turn;
    s32 step_pending;
    u8 *group_action;
    u8 *closing_action;
    s32 cell;
    struct SceneWork *work;

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
    Engine_ActorEnableActionCallback(6, Data_0200e324);
    Engine_EventWait(20);
    Actor_SetAttachedEffect(21, 0x102);
    Engine_ActorStartRepeatedMotion(21, 3);
    Object_RefreshSelectorById(6);
    Engine_EventWait(160);
    Engine_ActorRunRepeatedMotion(21, 1);
    Engine_EventWait(20);
    record = Object_GetById(21);
    {
        s32 shown = 0x5000;

        *(u16 *)(record + 6) = shown;
    }
    Engine_ActorSetAnimation(21, 0);
    Engine_EventWait(80);
    Engine_ActorSetAnimationAndWait(21, 4);
    Engine_EventWait(40);
    Event_ShowMessageAndWait(21, 0, 40);
    Engine_ActorSetAnimation(21, 5);
    Engine_EventWait(10);
    record = Object_GetById(21);
    Engine_ActorSetSpriteFlags(record, 0);
    Engine_ActorSetAnimation(21, 0);
    Engine_ActorJump(21, 6, 0);
    Actor_SetSpeed(21, 0x30000, 0x18000);
    *(u8 *)((u8 *)Object_GetById(21) + 90) &= 254;
    turn = 128;
    rec = Object_GetById(21);
    *(void (**)(u8 *))(rec + 108) = OverlayObject_DecayRecordField1e;
    *(u16 *)(rec + 6) = (turn << 8);
    Actor_MoveToAndWait(21, 184, 237);
    Object_SetActionCallbackAndRefreshById(21, Data_0200e360);
    Engine_EventWait(120);
    VinasuChojo_ShowMessage(0);
    Audio_PlayCue(72);
    Camera_SetSpeed( 0x40000, (turn << 8));
    Camera_MoveTo(0x1560000, 0x200000, 0xd40000, 1);
    Engine_CameraWaitForMove();
    Engine_EventWait(20);
    Engine_ActorRunRepeatedMotion(ACTOR_PARTY_LEADER, 2);
    Engine_EventWait(40);
    record = Object_GetById(0);
    Engine_ActorSetSpriteFlags(record, 1);
    Engine_ActorSetAnimation(ACTOR_PARTY_LEADER, 1);
    Engine_ActorJump(ACTOR_PARTY_LEADER, 6, 60);
    {
        u8 *record = Object_GetById(0);
        s32 shown = 10;

        *(u16 *)(record + 100) = shown;
    }
    Engine_ActorEnableActionCallback(0, Data_0200e074);
    Actor_ShowEmote(ACTOR_GERALD, 0x100, 80);
    Event_ShowMessageAndWait(ACTOR_GERALD, 0, 40);
    Engine_ActorRunRepeatedMotion(ACTOR_IVAN, 1);
    Engine_EventWait(20);
    VinasuChojo_ShowMessage(2);
    Actor_ShowEmote(ACTOR_MIA, 0x102, 40);
    VinasuChojo_ShowMessage(3);
    record = Object_GetById(1);
    Engine_ActorSetSpriteFlags(record, 1);
    Engine_ActorSetAnimation(ACTOR_GERALD, 1);
    Engine_ActorJump(ACTOR_GERALD, 6, 60);
    Engine_ActorRunRepeatedMotion(ACTOR_IVAN, 1);
    Engine_EventWait(20);
    record = Object_GetById(2);
    Engine_ActorSetSpriteFlags(record, 1);
    Engine_ActorSetAnimation(ACTOR_IVAN, 1);
    Engine_ActorJump(ACTOR_IVAN, 6, 40);
    record = Object_GetById(3);
    Engine_ActorSetSpriteFlags(record, 1);
    Engine_ActorSetAnimation(ACTOR_MIA, 1);
    Engine_ActorJump(ACTOR_MIA, 6, 60);
    Actor_FaceDirection(ACTOR_GERALD, 0x6000, 60);
    Engine_ActorSetAnimation(ACTOR_GERALD, 4);
    VinasuChojo_ShowMessage(1);
    Actor_FaceDirection(ACTOR_IVAN, 0x6000, 20);
    Engine_ActorSetAnimation(ACTOR_IVAN, 4);
    VinasuChojo_ShowMessage(2);
    VinasuChojo_FaceActor(3, 0x6000);
    Engine_ActorSetAnimation(ACTOR_MIA, 4);
    Event_ShowMessageAndWait(ACTOR_MIA, 0, 80);
    VinasuChojo_FaceActor(1, 0x4000);
    Event_ShowMessageAndWait(ACTOR_GERALD, 0, 40);
    Actor_FaceDirection(ACTOR_IVAN, 0xc000, 20);
    Event_ShowMessageAndWait(ACTOR_IVAN, 0, 40);
    Engine_ActorStartRepeatedMotion(ACTOR_GERALD, 1);
    Engine_ActorRunRepeatedMotion(ACTOR_IVAN, 1);
    Engine_EventWait(20);
    Actor_FaceDirection(ACTOR_GERALD, 0x6000, 0);
    Actor_FaceDirection(ACTOR_IVAN, 0x6000, 20);
    Engine_ActorRunRepeatedMotion(ACTOR_MIA, 1);
    Actor_FaceDirection(ACTOR_MIA, 0xe000, 20);
    VinasuChojo_ShowMessage(3);
    Actor_ShowEmote(ACTOR_GERALD, 0x100, 20);
    VinasuChojo_FaceActor(1, 0x4000);
    VinasuChojo_ShowMessage(1);
    Actor_FaceDirection(ACTOR_IVAN, 0xa000, 0);
    Actor_SetSpeed(ACTOR_GERALD, 0x19999, 0xcccc);
    Actor_WalkToAndWait(ACTOR_GERALD, 0x14d, 194);
    Actor_WalkToAndWait(ACTOR_GERALD, 0x14d, 206);
    Actor_FaceDirection(ACTOR_GERALD, 0, 20);
    Engine_ActorRunRepeatedMotion(ACTOR_GERALD, 1);
    Event_ShowMessageAndWait(ACTOR_GERALD, 0, 80);
    Engine_ActorStop(ACTOR_PARTY_LEADER);
    Engine_TaskWait(1);
    *(s32 *)(rec + 24) = 0x10000;
    *(s32 *)(rec + 28) = 0x10000;
    Engine_EventWait(40);
    Actor_ShowEmote(ACTOR_PARTY_LEADER, 0x100, 80);
    VinasuChojo_FaceActor( 1, 0x2000);
    Engine_ActorJump(ACTOR_GERALD, 4, 0);
    VinasuChojo_ShowMessage(1);
    VinasuChojo_FaceActor( 1, 0xe000);
    Event_OpenMessage(ACTOR_GERALD, 0);
    if (Engine_EventChooseYesNo(0, 0) == 0) {
        Engine_EventWait(20);
        Engine_ActorSetAnimationAndWait(ACTOR_GERALD, 3);
        VinasuChojo_ShowMessage(1);
        Engine_ActorRunRepeatedMotion(ACTOR_IVAN, 1);
        VinasuChojo_ShowMessage(2);
        Engine_ActorSetAnimation(ACTOR_MIA, 4);
        VinasuChojo_ShowMessage(3);
        step_pending = 1;
    } else {
        *(u16 *)((*(s32 *)&gEventWork + 0x1d8)) += 3;
        Engine_EventWait(60);
        VinasuChojo_ShowMessage(1);
        Engine_ActorRunRepeatedMotion(ACTOR_IVAN, 1);
        VinasuChojo_ShowMessage(2);
        Engine_ActorSetAnimation(ACTOR_MIA, 4);
        VinasuChojo_ShowMessage(3);
    }
    if (step_pending != 0) {
        *(u16 *)((*(s32 *)&gEventWork + 0x1d8)) += 3;
    }
    VinasuChojo_FaceActor( 0, 0x4000);
    Actor_SetAttachedEffect(ACTOR_PARTY_LEADER, 0x102);
    Engine_EventWait(80);
    Audio_PlayCue(17);
    Actor_ShowEmote(ACTOR_GERALD, 0x100, 20);
    Actor_FaceDirection(ACTOR_GERALD, 0xa000, 20);
    Actor_FaceDirection(ACTOR_GERALD, 0xe000, 40);
    Actor_FaceDirection(ACTOR_GERALD, 0x8000, 20);
    Actor_ShowEmote(ACTOR_IVAN, 0x101, 40);
    VinasuChojo_ShowMessage(2);
    Actor_FaceDirection(ACTOR_PARTY_LEADER, 0x6000, 0);
    Actor_FaceDirection(ACTOR_GERALD, 0x2000, 20);
    Engine_ActorSetAnimation(ACTOR_GERALD, 4);
    VinasuChojo_ShowMessage(1);
    Actor_FaceDirection(ACTOR_IVAN, 0xc000, 0);
    Actor_FaceDirection(ACTOR_MIA, 0x8000, 40);
    Actor_FaceDirection(ACTOR_IVAN, 0xa000, 0);
    Actor_FaceDirection(ACTOR_MIA, 0xc000, 40);
    Actor_SetAttachedEffect(ACTOR_MIA, 0x102);
    Engine_EventWait(60);
    VinasuChojo_ShowMessage(3);
    Audio_PlayCue(141);
    Work_SetValuesIfNonNegative(0x60000, 0x60000, 0x10000);
    Engine_EventWait(40);
    Audio_PlayCue(145);
    Map_CopyCellsTo(110, 105, 74, 4, 18, 23);
    Map_CopyCellsTo(92, 86, 83, 4, 8, 23);
    Map_CopyCellsTo(75, 28, 75, 4, 8, 23);
    Map_CopyCellsTo(92, 86, 11, 72, 16, 20);
    Map_CopyCellsTo(19, 92, 19, 68, 8, 21);
    rec = Object_GetById(0);
    ((struct StagedActor *)rec)->z.value += -0x200000;
    none = 0;
    ((struct StagedActor *)rec)->vertical_motion_direction = none;
    SceneActor_ParkRecord(rec);
    rec = Object_GetById(1);
    ((struct StagedActor *)rec)->x.value += -0x40000;
    ((struct StagedActor *)rec)->z.value += -0x200000;
    ((struct StagedActor *)rec)->vertical_motion_direction = none;
    SceneActor_ParkRecord(rec);
    rec = Object_GetById(2);
    ((struct StagedActor *)rec)->x.value += -0x40000;
    ((struct StagedActor *)rec)->z.value += -0x200000;
    ((struct StagedActor *)rec)->vertical_motion_direction = none;
    SceneActor_ParkRecord(rec);
    rec = Object_GetById(3);
    ((struct StagedActor *)rec)->x.value += -0x40000;
    ((struct StagedActor *)rec)->z.value += -0x120000;
    ((struct StagedActor *)rec)->vertical_motion_direction = none;
    SceneActor_ParkRecord(rec);
    record = Object_GetById(23);
    *(s32 *)(record + 12) = 0x380000;
    Camera_MoveTo(0x1520000, 0x200000, 0xb40000, 0);
    Engine_MapRedraw();
    Engine_TaskWait(1);
    Actor_ShowEmote(ACTOR_PARTY_LEADER, 0x100, 0);
    Actor_ShowEmote(ACTOR_GERALD, 0x100, 0);
    Actor_ShowEmote(ACTOR_IVAN, 0x100, 0);
    Actor_ShowEmote(ACTOR_MIA, 0x100, 0);
    Engine_ActorJump(ACTOR_PARTY_LEADER, 6, 0);
    Engine_ActorJump(ACTOR_GERALD, 6, 0);
    Engine_ActorJump(ACTOR_IVAN, 6, 0);
    Engine_ActorJump(ACTOR_MIA, 6, 0);
    group_action = Data_0200e3c0;
    Engine_ActorEnableActionCallback(ACTOR_PARTY_LEADER, group_action);
    Engine_ActorEnableActionCallback( 1, group_action);
    Engine_ActorEnableActionCallback( 2, group_action);
    Engine_ActorEnableActionCallback( 3, group_action);
    Work_SetValuesIfNonNegative(0x30000, 0x30000, 0x10000);
    Engine_EventWait(80);
    Work_SetValuesIfNonNegative(0x60000, 0x60000, 0x10000);
    Engine_EventWait(40);
    Audio_PlayCue(145);
    Map_CopyCellAttributes(110, 106, 18, 14, 10, 5);
    Map_CopyCellsTo(110, 105, 74, 4, 18, 23);
    Map_CopyCellsTo(92, 86, 11, 68, 16, 20);
    rec = Object_GetById(0);
    *(s32 *)(rec + 8) += -0x100000;
    SceneActor_ParkRecord(rec);
    rec = Object_GetById(1);
    *(s32 *)(rec + 8) += -0x100000;
    SceneActor_ParkRecord(rec);
    rec = Object_GetById(2);
    *(s32 *)(rec + 8) += -0x100000;
    SceneActor_ParkRecord(rec);
    rec = Object_GetById(3);
    *(s32 *)(rec + 8) += -0x100000;
    SceneActor_ParkRecord(rec);
    rec = Object_GetById(8);
    *(s32 *)(rec + 8) += 0x100000;
    SceneActor_ParkRecord(rec);
    rec = Object_GetById(9);
    *(s32 *)(rec + 8) += 0x100000;
    SceneActor_ParkRecord(rec);
    rec = Object_GetById(10);
    *(s32 *)(rec + 8) += -0x100000;
    SceneActor_ParkRecord(rec);
    rec = Object_GetById(11);
    *(s32 *)(rec + 8) += -0x100000;
    SceneActor_ParkRecord(rec);
    Camera_MoveTo(0x1420000, 0x200000, 0xb40000, 0);
    Engine_MapRedraw();
    Engine_TaskWait(1);
    Actor_SetAttachedEffect(ACTOR_PARTY_LEADER, 0x102);
    Actor_SetAttachedEffect(ACTOR_GERALD, 0x102);
    Actor_SetAttachedEffect(ACTOR_IVAN, 0x102);
    Actor_SetAttachedEffect(ACTOR_MIA, 0x102);
    Engine_ActorJump(ACTOR_PARTY_LEADER, 6, 0);
    Engine_ActorJump(ACTOR_GERALD, 6, 0);
    Engine_ActorJump(ACTOR_IVAN, 6, 0);
    Engine_ActorJump(ACTOR_MIA, 6, 0);
    Work_SetValuesIfNonNegative(-1, -1, 0xe666);
    Engine_EventWait(80);
    Audio_PlayCue(0x121);
    Engine_EventWait(40);
    Engine_ActorStop(ACTOR_PARTY_LEADER);
    Engine_ActorStop(ACTOR_GERALD);
    Engine_ActorStop(ACTOR_IVAN);
    Engine_ActorStop(ACTOR_MIA);
    Engine_EventWait(120);
    Actor_ShowEmote(ACTOR_IVAN, 0x105, 120);
    Engine_ActorRunRepeatedMotion(ACTOR_IVAN, 1);
    Actor_FaceDirection(ACTOR_IVAN, 0xa000, 20);
    VinasuChojo_ShowMessage(2);
    Engine_ActorSetAnimationAndWait(ACTOR_GERALD, 3);
    Actor_FaceDirection(ACTOR_GERALD, 0x2000, 0);
    VinasuChojo_ShowMessage(1);
    Actor_FaceDirection(ACTOR_MIA, 0xe000, 20);
    Engine_ActorSetAnimation(ACTOR_MIA, 4);
    VinasuChojo_ShowMessage(3);
    Engine_ActorJump(ACTOR_IVAN, 2, 20);
    VinasuChojo_ShowMessage(2);
    Engine_ActorSetAnimation(ACTOR_GERALD, 3);
    Engine_ActorSetAnimation(ACTOR_IVAN, 3);
    Engine_ActorSetAnimationAndWait(ACTOR_MIA, 3);
    Engine_EventWait(20);
    VinasuChojo_FaceActor(1, 0);
    VinasuChojo_ShowMessage(1);
    Actor_FaceDirection(ACTOR_PARTY_LEADER, 0x8000, 20);
    Engine_ActorSetAnimationAndWait(ACTOR_PARTY_LEADER, 3);
    Engine_EventWait(20);
    Actor_SetSpeed(ACTOR_GERALD, 0x10000, 0x8000);
    Actor_SetSpeed(ACTOR_IVAN, 0x10000, 0x8000);
    Actor_SetSpeed(ACTOR_MIA, 0x10000, 0x8000);
    closing_action = Data_0200e39c;
    Engine_ActorEnableActionCallback(ACTOR_GERALD, closing_action);
    Engine_ActorEnableActionCallback( 2, closing_action);
    Engine_ActorEnableActionCallback(ACTOR_MIA, closing_action);
    Engine_EventWait(60);
    Actor_SetSpeed(ACTOR_PARTY_LEADER, 0x10000, 0x8000);
    Actor_WalkToAndWait(ACTOR_PARTY_LEADER, 0x110, 216);
    Actor_WalkTo(ACTOR_PARTY_LEADER, 0x110, 254);
    Engine_EventWait(80);
    cell = (u32)&Data_03001ebc;
    work = *(struct SceneWork **)cell;
    work->request = 0x201;
    work->setup = 16;
    Engine_EventCloseScreen();
    cell -= 48;
    Engine_EventWaitForScreen();
    Engine_EventWait(80);
    Graphics_EnableObjLayerAndCallbacks();
    *(u16 *)((*(s32 *)cell + RENDER_RESULT_OFS)) = none;
    *(u16 *)((*(s32 *)cell + RENDER_RESULT_OFS + 2)) = none;
    UiText_ShowCenteredMessage((s32)MsgVinasuDespiteLongTiring, 0, 0);
    ObjectDispatch_StopCallbacksAndHideLayers();
    Engine_EventWait(80);
}

/* Parking an actor record. */

/*
 * Park a record: stamp the sentinel 0x80000000 into the three mirror fields at
 * +56, +60 and +64, zero the three at +36, +40 and +44, and clear the u16
 * angle at +100.  The 24-byte owner at 0x02005688 has no prologue, no stack
 * use and no literal pool.  0x80000000 is read as a sentinel because it is
 * stamped into three position-family fields at once and then tested for
 * equality; the roles of +36, +40 and +44 are not established.
 */
void SceneActor_ParkRecord(u8 *record)
{
    /* The sentinel is built as 128 shifted left by 24, not pooled. */
    *(s32 *)(record + 56) = (s32)0x80000000;
    *(s32 *)(record + 60) = (s32)0x80000000;
    *(s32 *)(record + 64) = (s32)0x80000000;

    *(s32 *)(record + 36) = 0;
    *(s32 *)(record + 40) = 0;
    *(s32 *)(record + 44) = 0;

    *(u16 *)(record + 100) = 0;
}

/*
 * The summit's rising transition, one step a frame: the flash and scroll
 * of its first steps, actor 23 rising from below the view while particles
 * stream up beside it, then the white flash and the flag that ends it. The
 * party's rise counters advance every frame.
 */
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
        SceneActor_ParkRecord((u8 *)center);
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
            effect->unknown_64 = angle;
            effect->unknown_66 = 0;
            effect->rise_counter = (u32)Random_Next() >> 13;
            effect->update = (void (*)(union FieldObject *))SceneEffect_UpdateCounterDrivenOrbit;
            effect->speed = Engine_MathSin((u32)(Random_Next() * 0xffff) >> 20) * 24;
            effect->speed = center->speed >> 16;
            sprite->flags = 0;
            sprite->priority = 1;
        }
    }
    VinasuChojo_TransitionStep++;
    SceneEffect_AdvanceGatedRiseCounter((u8 *)Actor_Get(0));
    SceneEffect_AdvanceGatedRiseCounter((u8 *)Actor_Get(1));
    SceneEffect_AdvanceGatedRiseCounter((u8 *)Actor_Get(2));
    SceneEffect_AdvanceGatedRiseCounter((u8 *)Actor_Get(3));
    SceneEffect_AdvanceGatedRiseCounter((u8 *)Actor_Get(21));
    SceneEffect_AdvanceGatedRiseCounter((u8 *)Actor_Get(6));
}

/* The gated rise counter. */
void SceneEffect_AdvanceGatedRiseCounter(u8 *obj)
{
    if (*(u8 *)(obj + 99) != 0) {
        u8 counter = *(u8 *)(obj + 98);

        *(u32 *)(obj + 12) = *(u32 *)(obj + 76) + ((u32)(counter >> 2) << 16);

        SceneActor_ParkRecord(obj);

        {
            if (*(u8 *)(obj + 98) != 0) {
                if (*(u8 *)(obj + 98) <= 31) {
                    ++*(u8 *)(obj + 98);
                }
            }
        }
    }
}

/*
 * Keeps actor 23 beside the view while the view is high enough, pulsing its
 * size on alternate frames, and every sixteenth frame releases a particle
 * that circles it.
 */
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
    map = gMapWork;
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
    effect->unknown_64 = angle;
    effect->unknown_66 = phase;
    *(struct FieldActor **)effect->unknown_68 = center;
    effect->update = FieldScene_RunScene3c9_02005b90;
    effect->speed = Engine_MathSin((drift & 0xfffff) >> 4) * 24 >> 16;
    /* FAKEMATCH: the byte store outside the sprite record keeps the game's order. */
    *(u8 *)((u8 *)sprite + 38) = 0;
    sprite->priority = center->sprite->priority;
}

/* The closing scene step. */
void FieldScene_RunScene3c9_02005b90(union FieldObject *object)
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
