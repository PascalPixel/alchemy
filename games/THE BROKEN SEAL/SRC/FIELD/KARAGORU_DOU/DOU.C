#include "TYPES.H"
#include "FIELD_EVENT.H"
#include "FIELD_SCENE.H"
#include "STAGED_ACTOR_PAIR_SCENE.H"
#include "STAGED_ACTOR.H"
#include "FIELD_EFFECT.H"

extern const struct SceneEntrance gKaragoruDouEntrances1[];
extern const struct SceneEntrance gKaragoruDouEntrances2[];
extern const struct SceneEntrance gKaragoruDouEntrances3[];
extern const struct SceneEntrance gKaragoruDouEntrancesOther[];
extern const u32 gKaragoruDouExits[];
extern const struct ScenePlacement gKaragoruDouPlacements1[];
extern const struct ScenePlacement gKaragoruDouPlacements1Flag96f[];
extern const struct ScenePlacement gKaragoruDouPlacements2[];
extern const struct ScenePlacement gKaragoruDouPlacements3[];
extern const struct ScenePlacement gKaragoruDouPlacementsOther[];
extern const struct SceneEvent gKaragoruDouEvents1[];
extern const struct SceneEvent gKaragoruDouEvents1Flag96f[];
extern const struct SceneEvent gKaragoruDouEvents2[];
extern const struct SceneEvent gKaragoruDouEvents3[];
extern const struct SceneEvent gKaragoruDouEventsOther[];

/* The cave links the staged-actor code, so this scene reaches these engine
   imports by the names that code gives them, beside the Engine_ names the
   cave's other scenes use. */
void Battle_WaitMode0(s32 frames);
void ObjectMotion_SetSpeedParameters(s32 actor, s32 speed, s32 acceleration);
void ObjectMotion_CommitCurrentPositionAndActivate(s32 actor);
void ObjectMotion_OffsetPositionAndResetMotion(s32 actor, s32 dx, s32 dz);
void Object_SetModeById(s32 actor, s32 animation);
extern u8 MsgKaragoruWhyGoingBackRobinDo[];

extern u8 MsgKaragoruIveBeenWaitingForRobin[];
extern u8 MsgKaragoruDoYouWishCrossInto[];

struct EffectRecord {
    u8 pad[9];
    u8 flags_lo : 2;
    u8 mode : 2;
    u8 flags_hi : 4;
};

struct EffectWork {
    u8 pad[80];
    struct EffectRecord *record;
};

struct HeightTrackedObject {
    u8 pad00[12];
    s32 height;                 /* +12 */
};

/* Moves the next dialogue line on by amount messages. */
static __inline__ void bump_step(s32 amount)
{
    gEventWork->message += amount;
}

extern u8 MsgKaragoruWarriorsHaveBeenFightingWhile[];
extern u8 MsgKaragoruWeMissedColossoBecauseWe[];

union SpinningActor {
    union FieldObject object;
    struct StagedActor staged;
};

void Map_WaitWorkValuesBelow256(void);

enum StagedPairMessage {
    MSG_WARRIORS_HAVE_BEEN_FIGHTING_WHILE = 0x23d2,
    MSG_WE_MISSED_COLOSSO_BECAUSE_WE = 0x23d5,
    MSG_IVE_BEEN_WAITING_FOR_ROBIN = 0x23d9,
    MSG_WHY_GOING_BACK_ROBIN_DO = 0x23da
};

extern u8 LinkedMessage_DoYouWishCrossInto[];

/* Where the party appears in each of the cave's three scenes. */
const struct SceneEntrance *Scene_GetEntrances(void)
{
    s16 scene = gGameState.scene;

    if (scene == (s32)&SceneId_KaragoruDou1) {
        return gKaragoruDouEntrances1;
    }
    if (scene == (s32)&SceneId_KaragoruDou2) {
        return gKaragoruDouEntrances2;
    }
    if (scene == (s32)&SceneId_KaragoruDou3) {
        return gKaragoruDouEntrances3;
    }
    return gKaragoruDouEntrancesOther;
}

/* The cave has no regions. */
const struct SceneRegion *Scene_GetRegions(void) { return 0; }

/* Every cave scene leaves through one exit table. */
const u32 *Scene_GetExits(void)
{
    return gKaragoruDouExits;
}

/* The actors placed in the cave; the first scene changes once flag 0x96f
   is set. */
const struct ScenePlacement *Scene_GetPlacements(void)
{
    s16 scene = gGameState.scene;

    if (scene == (s32)&SceneId_KaragoruDou1) {
        if (GameFlag_IsSet(0x96f) != 0) {
            return gKaragoruDouPlacements1Flag96f;
        }
        return gKaragoruDouPlacements1;
    }
    if (scene == (s32)&SceneId_KaragoruDou2) {
        return gKaragoruDouPlacements2;
    }
    if (scene == (s32)&SceneId_KaragoruDou3) {
        return gKaragoruDouPlacements3;
    }
    return gKaragoruDouPlacementsOther;
}

/* What the cave answers, chosen as its placements are. */
const struct SceneEvent *Scene_GetEvents(void)
{
    s16 scene = gGameState.scene;

    if (scene == (s32)&SceneId_KaragoruDou1) {
        if (GameFlag_IsSet(0x96f) != 0) {
            return gKaragoruDouEvents1Flag96f;
        }
        return gKaragoruDouEvents1;
    }
    if (scene == (s32)&SceneId_KaragoruDou2) {
        return gKaragoruDouEvents2;
    }
    if (scene == (s32)&SceneId_KaragoruDou3) {
        return gKaragoruDouEvents3;
    }
    return gKaragoruDouEventsOther;
}

/* Actor 11 catches up with a leader turning back while flag 0x9a0 is set
   and asks why. Going back sends the party to the Tolbi house at entrance
   30; staying on, he follows the leader out and the leader steps on. */
void FieldScene_RunScene3beSequenceB(void)
{
    struct FieldActor *leader;

    if (GameFlag_IsSet(0x98a) == 0 && GameFlag_IsSet(0x9a0) != 0) {
        Engine_EventBegin();
        ObjectMotion_SetSpeedParameters(11, 0x10000, 0x8000);
        leader = Actor_Get(ACTOR_PARTY_LEADER);
        if (leader != 0) {
            Actor_SetPosition(11, *(s32 *)((u8 *)leader + 8), *(s32 *)((u8 *)leader + 16));
        }
        ObjectMotion_OffsetPositionAndResetMotion(11, -8, 16);
        ObjectMotion_CommitCurrentPositionAndActivate(11);
        Actor_FaceDirection(11, 0xd000, 0);
        Battle_WaitMode0(10);
        Actor_FaceActor(ACTOR_PARTY_LEADER, 11, 0);
        Engine_EventSetMessage((s32)MsgKaragoruWhyGoingBackRobinDo);
        Event_OpenMessage(11, 0);
        if (Engine_EventChooseYesNo(0, 0) == 0) {
            Event_ShowMessage(11, 0);
            Actor_WalkTo(11, 152, 232);
            GameFlag_Clear(0x9a0);
            ObjectMotion_CommitCurrentPositionAndActivate(11);
            Object_SetModeById(11, 1);
            gGameState.saved_scene = (s32)&SceneId_TorebiHeya;
            gGameState.saved_entrance = 30;
        } else {
            gEventWork->message += 1;
            Event_ShowMessage(11, 0);
            Object_SetModeById(11, 2);
            leader = Actor_Get(ACTOR_PARTY_LEADER);
            if (leader != 0) {
                Actor_SetDestination(11, *(s16 *)((u8 *)leader + 10), *(s16 *)((u8 *)leader + 18));
            }
            ObjectMotion_CommitCurrentPositionAndActivate(11);
            Actor_SetPosition(11, 0, 0);
            Battle_WaitMode0(30);
            Object_SetModeById(ACTOR_PARTY_LEADER, 2);
            ObjectMotion_OffsetPositionAndResetMotion(ACTOR_PARTY_LEADER, 0, 16);
            ObjectMotion_CommitCurrentPositionAndActivate(ACTOR_PARTY_LEADER);
            Object_SetModeById(ACTOR_PARTY_LEADER, 1);
        }
        Engine_EventEnd();
    }
}

void ActorPresentation_RunActorElevenRecoveryScene(void)
{
    Engine_EventBegin();
    Actor_FaceActor(11, ACTOR_PARTY_LEADER, 0);
    Actor_FaceActor(ACTOR_PARTY_LEADER, 11, 0);
    Engine_ActorSetAnimation(ACTOR_PARTY_LEADER, 1);
    Engine_EventWait(10);
    Engine_ActorFaceEachOther(ACTOR_PARTY_LEADER, 11, 0);
    Engine_EventSetMessage((s32)MsgKaragoruIveBeenWaitingForRobin);
    Event_ShowMessage(11, 0);
    Engine_ActorSetAnimation(11, 2);
    {
        s16 *position = Actor_Get(ACTOR_PARTY_LEADER);

        if (position != 0)
            Actor_SetDestination(11, position[5], position[9]);
    }
    Engine_ActorWaitForMove(11);
    Actor_SetPosition(11, 0, 0);
    Engine_EventWait(20);
    GameFlag_Set(2464);
    Engine_EventEnd();
}

void KaragoruDou_AskToCross(void)
{
    s32 base;

    base = (s32)MsgKaragoruDoYouWishCrossInto;
    Engine_EventSetMessage(base);
    Event_OpenMessage(8, 0);
    if (Engine_EventChooseYesNo(0, 0) == 0) {
        if (GameFlag_IsSet(0x950) != 0) {
            if (GameFlag_IsSet(0x96f) == 0) {
                Engine_EventSetMessage((base + 8));
            }
        }
        Event_ShowMessage(8, 0);
    } else {
        bump_step(1);
        Event_ShowMessage(8, 0);
    }
}

void ActorPresentation_SelectActorNineScript(void)
{
    if (GameFlag_IsSet(2384) != 0 && GameFlag_IsSet(2415) == 0)
        Engine_EventSetMessage((s32)MsgKaragoruWeMissedColossoBecauseWe);
    else
        Engine_EventSetMessage((s32)MsgKaragoruWarriorsHaveBeenFightingWhile);
    Event_ShowMessage(9, 0);
}

void FieldScene_RunScene3be_02001080(void)
{
    extern u8 *Data_03001ebc;

    u8 *work;

    work = Data_03001ebc;
    Engine_EventBegin();
    if (GameFlag_IsSet(0x204) != 0) {
        GameFlag_Clear(0x9a3);
        GameFlag_Clear(0x9a5);
        GameFlag_Clear(0x9a4);
        GameFlag_Clear(0x9a6);
        GameFlag_Set(0x9a5);
        GameFlag_Set(0x9a4);
    }
    Engine_EventRequestExit(*(s16 *)(work + 0x16c));
    Engine_EventCloseScreen();
    Engine_EventWaitForScreen();
    Engine_EventEnd();
}

void StagedActorPairScene_RunStep(void)
{
    Engine_LeaderCheckAhead();
}

void ActorPresentation_RunActorEightThresholdScene(void)
{
    Actor_Get(8);
    Engine_EventBegin();
    {
        s32 *actor = Actor_Get(8);

        if ((actor[2] >> 20) <= 30) {
            StagedActorPairScene_RunSpinningLeap(8);
            {
                s32 x = 27;
                s32 y = 19;

                Map_CopyCellAttributes(29, 19, 1, 1, x, y);
            }
            GameFlag_Set(2466);
        }
    }
    Engine_EventEnd();
}

void StagedActorPairScene_RunUpdate(void)
{
    StagedActor_AdvancePair();
    ActorPresentation_RunActorNineThresholdScene();
}

void ActorPresentation_RunActorNineThresholdScene(void)
{
    Engine_EventBegin();
    if ((((s32 *)Object_GetById(9))[2] >> 20) > 42) {
        s32 x = 107;
        s32 y = 17;

        Map_CopyCellAttributes(108, 17, 1, 1, x, y);
        Engine_EventWait(8);
        Actor_SetPosition(9, 0, 0);
        Actor_SetPosition(10, 45613056, 18874368);
        Engine_ActorSetAnimation(10, 3);
        Audio_PlayCue(154);
        GameFlag_Set(2469);
    }
    Engine_EventEnd();
}

void StagedActorPairScene_NoopActorCallback(void){}

void StagedActorPairScene_RotateActorPart(u8 *actor)
{
    u8 *sprite_part = *(u8 **)(actor + 80);

    *(u16 *)(sprite_part + 30) -= 0x400;
}

void StagedActorPairScene_WaitForHeight(struct HeightTrackedObject *object,
                                       s32 limit)
{
    extern u8 Data_03001ebc[];

    s32 frames = 40;

    while (frames != 0) {
        Engine_TaskWait(1);
        frames--;
        if (object->height <= limit) {
            break;
        }
    }
}

void StagedActorPairScene_RunSpinningLeap(s32 id)
{
    union SpinningActor *work;
    u32 cnt;
    s32 velocity[3];
    s32 angle;

    work = (union SpinningActor *)Object_GetById(id);
    work->object.actor.motion_flags = 0;
    for (cnt = 0; cnt < 9; cnt++) {
        Engine_TaskWait(1);
        work->object.actor.sprite->rotation -= 0x100;
        work->object.actor.x.fixed -= Engine_MathCos(work->object.actor.sprite->rotation) / 2;
        work->staged.unknown_38 = 0x80000000;
    }
    work->object.actor.update = (void (*)(union FieldObject *))StagedActorPairScene_RotateActorPart;
    Engine_AudioPlayCue(136);
    Actor_SetSpeed(id, 0x20000, 0x10000);
    Actor_SetDestination(id, 472, 288);
    work->object.effect.velocity_y = 0xcccc;
    work->object.actor.motion_flags = 3;
    work->object.actor.unknown_22 = 0;
    Engine_ActorWaitForMove(id);
    StagedActorPairScene_WaitForHeight(work, 0x200000);
    Work_SetValuesIfNonNegative(0x50000, 0x50000, 0x10000);
    Work_SetValuesIfNonNegative(-1, -1, 0xe666);
    for (cnt = 0; cnt < 17; cnt++) {
        angle = cnt << 12;
        velocity[0] = Engine_MathCos(angle);
        velocity[1] = 0;
        velocity[2] = Engine_MathSin(angle);
        velocity[0] -= velocity[0] / 4;
        velocity[2] -= velocity[2] / 2;
        Effect_Spawn(work->object.actor.x.fixed, work->object.actor.y.fixed,
                     work->object.actor.z.fixed, velocity[0], velocity[1], velocity[2], 0, 0);
    }
    Actor_SetDestination(id, 440, 308);
    Engine_ActorWaitForMove(id);
    StagedActorPairScene_WaitForHeight(work, 0x200000);
    work->object.actor.update = 0;
    work->object.actor.sprite->rotation = 0x1000;
    Engine_AudioPlayCue(154);
    Engine_ActorSetAnimation(id, 3);
    Map_WaitWorkValuesBelow256();
    Engine_EventWait(10);
    Engine_ActorSetAnimation(id, 2);
}

void StagedActorPairScene_NoopSceneCallback(void){}

void StagedActorPairScene_RunActorTwelveCommand(void)
{
    Actor_SetPosition(12, 0, 0);
}

/* The cave's scene start. Entering the first scene sets flag 0x144 and
   hides actor 11 while flag 0x9a0 is set. In the third, entrance 1 opens
   its passage, flags 0x9a2 and 0x9a5 move the actors their events have
   moved, and actor 12's sprite flags are cleared. */
s32 Scene_Initialize(void)
{
    struct FieldActor *actor;

    if (gGameState.scene == (s32)&SceneId_KaragoruDou1) {
        GameFlag_Set(0x144);
        if (GameFlag_IsSet(0x9a0) != 0) {
            Actor_SetPosition(11, 0, 0);
        }
    }
    if (gGameState.scene == (s32)&SceneId_KaragoruDou3) {
        if (gGameState.entrance == 1) {
            Map_CopyCellAttributes(108, 17, 1, 1, 107, 17);
        }
        if (GameFlag_IsSet(0x9a2) != 0) {
            Actor_SetPosition(8, 0x1b80000, 0x1340000);
            Engine_ActorSetAnimation(8, 2);
            Map_CopyCellAttributes(29, 19, 1, 1, 27, 19);
        }
        if (GameFlag_IsSet(0x9a5) != 0) {
            Actor_SetPosition(9, 0, 0);
            Actor_SetPosition(10, 0x2b80000, 0x1200000);
            Engine_ActorSetAnimation(10, 2);
        }
        actor = Actor_Get(12);
        Engine_ActorSetSpriteFlags(actor, 0);
    }
    return 0;
}
