#include "BATTLE.H"
#include "DEBUG.H"
#include "FIELD.H"
#include "GAME.H"
#include "GRAPHICS.H"
#include "MENU.H"
#include "MINIGAME.H"
#include "SOUND.H"
#include "SYSTEM.H"
#include "FX_SCENE.H"
#include "IRIGUCHI.H"
#include "TYPES.H"
#include "CALL.H"
#include "STAGED_ACTOR.H"
#include "FIELD_EVENT.H"
#include "FIELD_SERVICE.H"
#include "FIELD_SCENE.H"
#include "text/MSG_IDS.H"

void Effect_AdvanceMotion(struct MotionEffect *effect);
void OverlayObject_WaitUntilIdle(s32 *obj);
void SceneState_ApplyRectsAtActors8And9();

extern s32 StagedActor_DirectionSteps[];
s32 Map_GetTerrainHeight(s32 mode, s32 x, s32 z);

s32 battle_owner_69(void);
s32 FieldEffect_UpdateGridPlacement(void);

void SceneState_SetValue8Mode66(void)
{
    BattleFx_SetPhaseRequest(8, 66);
}

void OverlayObject_WaitUntilIdle(s32 *obj)
{
    s32 i = 60;

    while (i != 0) {
        WaitFrames(1);
        i--;
        if (obj[10] == 0) {
            break;
        }
    }
}

void Effect_AdvanceMotion(struct MotionEffect *effect)
{
    s32 velocity_z;
    struct Sprite *sprite;
    s32 velocity_x;

    /* FAKEMATCH: removing this one-pass block changes instruction scheduling. */
    do {
        velocity_x = effect->velocity[0];
        effect->position[0] += velocity_x;
        effect->position[1] += effect->velocity[1];
    } while (0);
    velocity_z = effect->velocity[2];
    effect->position[2] += velocity_z;

    effect->velocity[0] = velocity_x - Iriguchi_Divide(velocity_x, 18);
    effect->velocity[2] = velocity_z - velocity_z / 16;

    effect->accum18 += effect->rate30;
    effect->accum1c += effect->rate34;

    sprite = effect->sprite;
    sprite->angle += effect->step64;
}

void FieldScene_RunSupplementalSequenceOne(void)
{
    u32 i;
    struct FieldActor *leader;
    struct EffectOptions options;
    s32 vec[3];

    Engine_EventBegin();
    Camera_MoveTo(-1, -1, -1, 0);
    Engine_MapRedraw();
    WaitFrames(1);
    Object_GetById(0)->y.fixed = 0x820000;
    Object_GetById(0)->gravity = 0x8000;
    Object_GetById(0)->bounce = 0;
    Object_GetById(0)->motion_flags = 0;
    Engine_EventOpenScreen();
    Engine_EventWaitForScreen();
    Battle_WaitMode0(30);
    Audio_PlayCue(204);
    Object_GetById(0)->motion_flags = 3;
    Battle_WaitMode0(24);
    leader = Object_GetById(ACTOR_PARTY_LEADER);
    options.palette = 7;
    options.update = (void (*)(union FieldObject *))Effect_AdvanceMotion;
    options.start_scale_x = 0xcccc;
    options.start_scale_y = 0xcccc;
    for (i = 0; i < 17; i++) {
        vec[0] = Engine_MathCos(i << 12);
        vec[1] = 0;
        vec[2] = Engine_MathSin(i << 12);
        vec[0] += vec[0] / 2;
        Effect_Spawn(leader->x.fixed, leader->y.fixed, leader->z.fixed, vec[0], vec[1], vec[2],
                      EFFECT_USE_UPDATE | EFFECT_USE_START_SCALE | EFFECT_USE_PALETTE | 1, &options);
    }
    Audio_PlayCue(188);
    Actor_SetAttachedEffect(ACTOR_PARTY_LEADER, 0x101);
    Object_SetModeById(ACTOR_PARTY_LEADER, 22);
    Work_SetValuesIfNonNegative(0x50000, 0x50000, 0x10000);
    Work_SetValuesIfNonNegative(-1, -1, 0xe666);
    Engine_MapRenderWaitForValues();
    Actor_SetAttachedEffect(ACTOR_PARTY_LEADER, 0x100);
    Object_GetById(0)->gravity = 0x10000;
    Object_GetById(0)->bounce = 0x4000;
    Engine_EventEnd();
}

void FieldScene_RunScene3c5SequenceA(s32 exit)
{
    struct FieldActor *leader;

    Engine_EventBegin();
    Camera_MoveTo(-1, -1, -1, 0);
    Engine_MapRedraw();
    WaitFrames(1);
    leader = Actor_Get(ACTOR_PARTY_LEADER);
    leader->y.fixed = 0x820000;
    leader = Actor_Get(ACTOR_PARTY_LEADER);
    leader->gravity = 0x4000;
    leader = Actor_Get(ACTOR_PARTY_LEADER);
    leader->bounce = 0;
    Object_GetById(0)->motion_flags = 0;
    Engine_ActorSetSpriteFlags(Actor_Get(ACTOR_PARTY_LEADER), 0);
    Engine_EventOpenScreen();
    Engine_EventWaitForScreen();
    Battle_WaitMode0(10);
    Audio_PlayCue(204);
    Object_GetById(0)->motion_flags = 3;
    leader = Actor_Get(ACTOR_PARTY_LEADER);
    leader->velocity_y = -0x50000;
    OverlayObject_WaitUntilIdle((s32 *)Actor_Get(ACTOR_PARTY_LEADER));
    Actor_SetChildValue(ACTOR_PARTY_LEADER, 15);
    Engine_EventRequestExit(exit);
    Engine_EventEnd();
}

const struct SceneEntrance *Scene_GetEntrances(void)
{
    s16 scene = gGameState.scene;

    if (scene == (s32)&SceneId_BabiIriguchi3) {
        return gBabiIriguchiEntrances3;
    }
    if (scene == (s32)&SceneId_BabiIriguchi2) {
        return gBabiIriguchiEntrances2;
    }
    if (scene == (s32)&SceneId_BabiIriguchi1) {
        return gBabiIriguchiEntrances1;
    }
    return gBabiIriguchiEntrancesOther;
}

const struct SceneRegion *Scene_GetRegions(void)
{
    if (gGameState.scene == (s32)&SceneId_BabiIriguchi3) {
        return gBabiIriguchiRegions3;
    }
    return 0;
}

u8 *BabiIriguchi_GetExits(void)
{
    return gBabiIriguchiExits;
}

const struct ScenePlacement *Scene_GetPlacements(void)
{
    s16 scene = gGameState.scene;

    if (scene == (s32)&SceneId_BabiIriguchi3) {
        return gBabiIriguchiPlacements3;
    }
    if (scene == (s32)&SceneId_BabiIriguchi2) {
        return gBabiIriguchiPlacements2;
    }
    if (scene == (s32)&SceneId_BabiIriguchi1) {
        return gBabiIriguchiPlacements1;
    }
    return gBabiIriguchiPlacementsOther;
}

void BabiIriguchi_JumpFromLedge(void)
{
    struct FieldActor *rec7;

    rec7 = Object_GetById(0);
    Engine_EventBegin();
    if ((rec7->x.fixed >> 20) != 6) {
        if ((rec7->x.fixed >> 20) != 18) {
            goto done;
        }
    }
    if ((rec7->z.fixed >> 20) == 20) {
        rec7->target_x = -0x80000000;
        rec7->target_z = -0x80000000;
        Actor_ShowEmote(0, 0x100, 20);
        Actor_SetSpeed(0, 0x20000, 0x10000);
        Engine_ActorJump(0, 4, 0);
{ u16 dir = rec7->facing; if ((u16)(dir + 0x4fff) > 0x1fff && (u16)(dir - 0x3001) > 0x1fff) goto step_down; }
        ObjectMotion_OffsetPositionAndResetMotion(0, 16, 0);
        ObjectMotion_CommitCurrentPositionAndActivate(0);
        Actor_FaceDirection(0, FACING_WEST, 20);
        goto done;
        step_down:;
        FieldObjectMotion_OffsetPositionAndResetMotion(0, 0, -16);
        ObjectMotion_CommitCurrentPositionAndActivate(0);
        Actor_FaceDirection(0, FACING_SOUTH, 20);
    }
    done:;
    SceneState_ApplyRectsAtActors8And9();
    Engine_EventEnd();
}

void FieldScene_RunFourCallSequence(void)
{
    Engine_EventBegin();
    StagedActor_AdvancePair();
    BabiIriguchi_JumpFromLedge();
    Engine_EventEnd();
}

void SceneState_BranchOnActorEightOrNineTile(void)
{
    s32 *p = Object_GetById(9);

    if ((Object_GetById(0)->x.fixed >> 20) <= 12) {
        p = Object_GetById(8);
        if ((p[2] >> 20) == 6) {
            if ((p[4] >> 20) == 20) {
                FieldScene_RunFourCallSequence();
                return;
            }
        }
    } else {
        if ((p[2] >> 20) == 18) {
            if ((p[4] >> 20) == 20) {
                FieldScene_RunFourCallSequence();
                return;
            }
        }
    }
    FieldEffect_UpdateGridPlacement();
}

void ResetSceneParametersAndFinishSetup(void)
{
    Camera_MoveTo(-1, -1, -1, 0);
    BattleFx_RunRisingObjectSequence(0, 6, 0);
    Engine_EventCloseScreen();
    Engine_EventWaitForScreen();
}

void FieldScene_RunStep11(void)
{
    Engine_EventBegin();
    ResetSceneParametersAndFinishSetup();
    Engine_EventRequestExit(11);
    Engine_EventEnd();
}

void FieldScene_RunStep12WithPosition(void)
{
    Engine_EventBegin();
    OverlayObject_SpawnConfiguredObject(0x1d00000, 0, 0x1220000, 223);
    ResetSceneParametersAndFinishSetup();
    Engine_EventRequestExit(12);
    Engine_EventEnd();
}

void FieldScene_RunStep13WithTwoPositions(void)
{
    Engine_EventBegin();
    OverlayObject_SpawnConfiguredObject(0x8f0000, 0, 0x1220000, 223);
    OverlayObject_SpawnConfiguredObject(0x790000, 0, 0x11e0000, 253);
    ResetSceneParametersAndFinishSetup();
    Engine_EventRequestExit(13);
    Engine_EventEnd();
}

void FieldScene_RunStep15(void)
{
    Engine_EventBegin();
    ResetSceneParametersAndFinishSetup();
    Engine_EventRequestExit(15);
    Engine_EventEnd();
}

void FieldScene_RunStepWithValue2693(void)
{
    Engine_EventBegin();
    Object_SetModeById(ACTOR_PARTY_LEADER, 1);
    Engine_MessageShowCentered((s32)MsgBabiSeemsLocked, 1);
    Engine_EventEnd();
}

void FieldScene_RunBranchingActorSequence(void)
{
    struct FieldActor *record;

    GameFlag_Set(0x988);
    GameFlag_Set(0x98a);
    Engine_EventBegin();
    Battle_ResetEffectCounter();
    Engine_EventSetMessage((s32)MsgBabiYoureSureTheyWentThrough);
    Actor_SetSpeed(ACTOR_PARTY_LEADER, 0x10000, 0x8000);
    Actor_WalkToAndWait(ACTOR_PARTY_LEADER, 0x128, 0x160);
    Actor_FaceDirection(ACTOR_PARTY_LEADER, FACING_NORTH, 0);
    Battle_WaitMode0(10);
    FieldMotion_LaunchFromFocusedObject(10, 16, 0, 0xc000);
    FieldMotion_LaunchFromFocusedObject(1, -8, 16, 0xc000);
    FieldMotion_LaunchFromFocusedObject(2, 8, 16, 0xc000);
    FieldMotion_LaunchFromFocusedObject(3, 24, 16, 0xc000);
    ObjectMotion_CommitCurrentPositionAndActivate(MEARI);
    Battle_WaitMode0(20);
    Event_ShowMessage(14, 0);
    Battle_WaitMode0(10);
    Camera_SetSpeed(0x30000, 0x6000);
    Camera_MoveTo(0x1180000, -1, 0x1200000, 1);
    Engine_CameraWaitForMove();
    Battle_WaitMode0(20);
    Engine_ActorSetAnimationAndWait(11, 3);
    Battle_WaitMode0(30);
    Event_ShowMessage(11, 0);
    Battle_WaitMode0(10);
    Actor_FaceDirection(14, FACING_NORTH, 0);
    Actor_FaceDirection(11, FACING_NORTH, 0);
    Battle_WaitMode0(30);
    Battle_WaitMode0(10);
    Engine_ActorRunRepeatedMotion(14, 2);
    Battle_WaitMode0(20);
    Event_ShowMessage(14, 0);
    Battle_WaitMode0(10);
    Engine_ActorStartRepeatedMotion(13, 2);
    Engine_ActorRunRepeatedMotion(12, 2);
    Battle_WaitMode0(40);
    Engine_ActorStartRepeatedMotion(13, 2);
    Engine_ActorRunRepeatedMotion(12, 2);
    Battle_WaitMode0(40);
    Engine_ActorStartRepeatedMotion(13, 2);
    Engine_ActorRunRepeatedMotion(12, 2);
    Battle_WaitMode0(40);
    Actor_ShowEmote(12, 0x102, 50);
    Event_ShowMessage(12, 0);
    Battle_WaitMode0(10);
    Actor_FaceDirection(13, FACING_EAST, 0);
    Battle_WaitMode0(25);
    Engine_ActorRunRepeatedMotion(13, 2);
    Battle_WaitMode0(20);
    Event_ShowMessage(13, 0);
    Battle_WaitMode0(10);
    Actor_FaceDirection(12, FACING_WEST, 0);
    Battle_WaitMode0(20);
    Engine_ActorSetAnimationAndWait(13, 4);
    Battle_WaitMode0(20);
    Event_ShowMessage(13, 0);
    Battle_WaitMode0(10);
    Engine_ActorSetAnimationAndWait(11, 4);
    Battle_WaitMode0(20);
    Event_ShowMessage(11, 0);
    Battle_WaitMode0(20);
    Actor_ShowEmote(13, 0x107, 40);
    Battle_WaitMode0(10);
    Actor_FaceDirection(13, FACING_SOUTH, 0);
    Battle_WaitMode0(20);
    Event_ShowMessage(13, 0);
    Battle_WaitMode0(20);
    Actor_ShowEmote(13, 0x101, 75);
    Actor_ShowEmote(14, 0x101, 60);
    Actor_FaceDirection(12, FACING_SOUTH, 0);
    Battle_WaitMode0(20);
    Actor_FaceDirection(11, FACING_SOUTH, 0);
    Battle_WaitMode0(20);
    Actor_FaceDirection(14, FACING_SOUTH, 0);
    Battle_WaitMode0(30);
    Event_ShowMessage(14, 0);
    Camera_MoveTo(0x1180000, -1, 0x1400000, 1);
    Engine_CameraWaitForMove();
    Battle_WaitMode0(20);
    Engine_ActorSetAnimationAndWait(10, 3);
    Battle_WaitMode0(30);
    Event_ShowMessage(10, 0);
    Battle_WaitMode0(10);
    Engine_ActorSetAnimationAndWait(10, 4);
    Battle_WaitMode0(20);
    Event_ShowMessage(10, 0);
    Battle_WaitMode0(20);
    Actor_ShowEmote(14, 0x105, 60);
    Event_ShowMessage(14, 0);
    Battle_WaitMode0(10);
    Engine_ActorSetAnimationAndWait(MEARI, 4);
    Battle_WaitMode0(20);
    Event_ShowMessage(MEARI, 0);
    Battle_WaitMode0(10);
    Engine_ActorRunRepeatedMotion(IWAN, 2);
    Battle_WaitMode0(20);
    Event_ShowMessage(IWAN, 0);
    Battle_WaitMode0(10);
    Engine_ActorRunRepeatedMotion(GERALD, 2);
    Battle_WaitMode0(20);
    Event_OpenMessage(GERALD, 0);
    if (Engine_EventChooseYesNo(0, 0) == 0) {
        Battle_WaitMode0(30);
        Actor_FaceDirection(10, FACING_WEST, 0);
        Battle_WaitMode0(30);
        Engine_ActorSetAnimationAndWait(10, 3);
        Battle_WaitMode0(30);
        Event_ShowMessage(10, 0);
        gEventWork->message++;
    } else {
        Battle_WaitMode0(30);
        Actor_FaceDirection(10, FACING_WEST, 0);
        Battle_WaitMode0(30);
        Engine_ActorSetAnimationAndWait(10, 4);
        Battle_WaitMode0(20);
        gEventWork->message++;
        Event_ShowMessage(10, 0);
    }
    Battle_WaitMode0(10);
    Actor_ShowEmote(14, 0x101, 60);
    Actor_SetSpeed(14, 0x10000, 0x8000);
    Actor_WalkByAndWait(14, 0, 16);
    Battle_WaitMode0(20);
    Event_ShowMessage(14, 0);
    Battle_WaitMode0(10);
    Actor_FaceDirection(10, FACING_NORTH, 0);
    Battle_WaitMode0(35);
    Event_ShowMessage(10, 0);
    Battle_WaitMode0(10);
    Engine_ActorRunRepeatedMotion(14, 2);
    Battle_WaitMode0(20);
    Event_ShowMessage(14, 0);
    Battle_WaitMode0(20);
    Actor_ShowEmote(MEARI, 0x102, 40);
    Event_ShowMessage(MEARI, 0);
    Battle_WaitMode0(10);
    Engine_ActorRunRepeatedMotion(IWAN, 2);
    Battle_WaitMode0(20);
    Event_ShowMessage(IWAN, 0);
    Battle_WaitMode0(10);
    Engine_ActorSetAnimationAndWait(GERALD, 3);
    Battle_WaitMode0(30);
    Event_ShowMessage(GERALD, 0);
    Battle_WaitMode0(20);
    Actor_ShowEmote(14, 0x100, 40);
    Event_OpenMessage(14, 0);
    if (Engine_EventChooseYesNo(0, 0) == 0) {
        Battle_WaitMode0(30);
        Engine_ActorSetAnimationAndWait(14, 4);
        Battle_WaitMode0(20);
        Event_ShowMessage(14, 0);
        gEventWork->message++;
    } else {
        Battle_WaitMode0(30);
        Engine_ActorSetAnimationAndWait(14, 4);
        Battle_WaitMode0(20);
        gEventWork->message++;
        Event_ShowMessage(14, 0);
    }
    Battle_WaitMode0(10);
    Actor_ShowEmote(10, 0x102, 50);
    Event_ShowMessage(10, 0);
    Battle_WaitMode0(10);
    Engine_ActorRunRepeatedMotion(13, 2);
    Battle_WaitMode0(20);
    Actor_SetSpeed(13, 0x14ccc, 0xa666);
    Actor_WalkByAndWait(13, 0, 16);
    Battle_WaitMode0(20);
    Event_ShowMessage(13, 0);
    Battle_WaitMode0(10);
    Actor_SetSpeed(12, 0x14ccc, 0xa666);
    Actor_WalkByAndWait(12, 0, 16);
    Battle_WaitMode0(20);
    Actor_ShowEmote(12, 0x107, 50);
    Event_ShowMessage(12, 0);
    Battle_WaitMode0(10);
    Engine_ActorSetAnimationAndWait(14, 3);
    Battle_WaitMode0(30);
    Battle_WaitMode0(10);
    Actor_ShowEmote(10, 0x102, 60);
    Actor_FaceDirection(10, FACING_WEST, 0);
    Battle_WaitMode0(25);
    Event_ShowMessage(10, 0);
    Battle_WaitMode0(10);
    Engine_ActorRunRepeatedMotion(10, 2);
    Battle_WaitMode0(20);
    Event_OpenMessage(10, 0);
    if (Engine_EventChooseYesNo(0, 0) == 0) {
        Battle_WaitMode0(30);
        Engine_ActorSetAnimationAndWait(10, 3);
        Battle_WaitMode0(30);
        Event_ShowMessage(10, 0);
        gEventWork->message++;
    } else {
        Battle_WaitMode0(30);
        Engine_ActorSetAnimationAndWait(10, 4);
        Battle_WaitMode0(20);
        gEventWork->message++;
        Event_ShowMessage(10, 0);
    }
    Battle_WaitMode0(10);
    Actor_FaceDirection(10, FACING_NORTH, 0);
    Battle_WaitMode0(35);
    Engine_ActorSetAnimationAndWait(14, 3);
    Battle_WaitMode0(30);
    Actor_FaceDirection(14, 0xb000, 0);
    Battle_WaitMode0(40);
    Event_ShowMessage(14, 0);
    Battle_WaitMode0(10);
    Engine_ActorFaceEachOther(12, 13, 50);
    Object_SetModeById(12, 3);
    Engine_ActorSetAnimationAndWait(13, 3);
    Battle_WaitMode0(30);
    Actor_FaceDirection(12, FACING_SOUTH, 0);
    Actor_FaceDirection(13, FACING_SOUTH, 0);
    Battle_WaitMode0(20);
    Object_SetModeById(12, 3);
    Engine_ActorSetAnimationAndWait(13, 3);
    Battle_WaitMode0(30);
    Actor_SetSpeed(12, 0x10000, 0x8000);
    Actor_SetSpeed(13, 0x10000, 0x8000);
    Actor_WalkBy(12, 32, 0);
    Actor_WalkByAndWait(13, 32, 0);
    Actor_WalkBy(12, 0, 16);
    Actor_WalkByAndWait(13, 16, 0);
    Actor_WalkTo(13, 0x158, 0x138);
    Actor_WalkToAndWait(12, 0x158, 0x150);
    Object_SetModeById(13, 1);
    Actor_FaceDirection(12, FACING_WEST, 0);
    Actor_FaceDirection(13, FACING_WEST, 0);
    Battle_WaitMode0(10);
    Actor_FaceDirection(14, FACING_SOUTH, 0);
    Battle_WaitMode0(20);
    Event_ShowMessage(14, 0);
    Battle_WaitMode0(10);
    Actor_SetSpeed(14, 0x10000, 0x8000);
    Actor_WalkToAndWait(14, 0x148, 0x138);
    Actor_FaceDirection(14, FACING_WEST, 0);
    Battle_WaitMode0(30);
    Actor_SetSpeed(11, 0x10000, 0x8000);
    Actor_WalkToAndWait(11, 0x148, 0x148);
    Actor_FaceDirection(11, FACING_WEST, 0);
    Battle_WaitMode0(20);
    Actor_SetSpeed(GERALD, 0x13333, 0x9999);
    Actor_SetSpeed(IWAN, 0x13333, 0x9999);
    Actor_SetSpeed(MEARI, 0x13333, 0x9999);
    Object_SetModeById(GERALD, 2);
    record = Object_GetById(ACTOR_PARTY_LEADER);
    if (record != 0) {
        Actor_SetDestination(GERALD, record->x.part.pixel, record->z.part.pixel);
    }
    ObjectMotion_CommitCurrentPositionAndActivate(GERALD);
    Actor_SetPosition(GERALD, 0, 0);
    Object_SetModeById(IWAN, 2);
    record = Object_GetById(ACTOR_PARTY_LEADER);
    if (record != 0) {
        Actor_SetDestination(IWAN, record->x.part.pixel, record->z.part.pixel);
    }
    ObjectMotion_CommitCurrentPositionAndActivate(IWAN);
    Actor_SetPosition(IWAN, 0, 0);
    Object_SetModeById(MEARI, 2);
    record = Object_GetById(ACTOR_PARTY_LEADER);
    if (record != 0) {
        Actor_SetDestination(MEARI, record->x.part.pixel, record->z.part.pixel);
    }
    ObjectMotion_CommitCurrentPositionAndActivate(MEARI);
    Actor_SetPosition(MEARI, 0, 0);
    Battle_WaitMode0(10);
    Engine_EventEnd();
}

void FieldScene_RunActorEventSequence(void)
{
    struct FieldActor *record;

    GameFlag_Set(0x989);
    Engine_EventBegin();
    Battle_ResetEffectCounter();
    Engine_EventSetMessage((s32)MsgBabiTheDoor);
    Actor_SetSpeed(ACTOR_PARTY_LEADER, 0x10000, 0x8000);
    Actor_WalkToAndWait(ACTOR_PARTY_LEADER, 0x128, 0x138);
    Actor_FaceDirection(ACTOR_PARTY_LEADER, FACING_EAST, 0);
    Battle_WaitMode0(10);
    Motion_LaunchFromFocusedObject(1, 0, 16, 0);
    FieldMotion_LaunchFromFocusedObject(2, -16, -8, 0);
    Motion_LaunchFromFocusedObject(3, -16, 24, 0);
    ObjectMotion_CommitCurrentPositionAndActivate(MEARI);
    Battle_WaitMode0(20);
    Camera_SetSpeed(0x30000, 0x6000);
    Camera_MoveTo(0x1180000, -1, 0x1480000, 1);
    Engine_CameraWaitForMove();
    Battle_WaitMode0(10);
    Battle_WaitMode0(10);
    Actor_FaceDirection(10, 0xb000, 0);
    Battle_WaitMode0(10);
    Actor_ShowEmote(10, 0x100, 40);
    Event_ShowMessage(10, 0);
    Engine_ActorJump(10, 4, 13);
    Engine_ActorJump(10, 4, 30);
    Battle_WaitMode0(10);
    Actor_ShowEmote(11, 0x100, 0);
    Actor_ShowEmote(12, 0x100, 0);
    Actor_ShowEmote(13, 0x100, 0);
    Actor_ShowEmote(14, 0x100, 40);
    Battle_WaitMode0(10);
    Actor_FaceDirection(14, 0xb000, 0);
    Actor_FaceDirection(11, 0xb000, 0);
    Actor_FaceDirection(12, 0xb000, 0);
    Actor_FaceDirection(13, 0xb000, 0);
    Battle_WaitMode0(30);
    Engine_ActorRunRepeatedMotion(14, 2);
    Battle_WaitMode0(20);
    Event_ShowMessage(14, 0);
    Battle_WaitMode0(20);
    Actor_ShowEmote(13, 0x102, 40);
    Event_ShowMessage(13, 0);
    Battle_WaitMode0(20);
    Actor_ShowEmote(12, 0x101, 50);
    Actor_FaceDirection(12, FACING_WEST, 0);
    Battle_WaitMode0(25);
    Event_ShowMessage(12, 0);
    Battle_WaitMode0(10);
    Actor_FaceDirection(14, FACING_WEST, 0);
    Actor_FaceDirection(11, FACING_WEST, 0);
    Actor_FaceDirection(13, FACING_WEST, 0);
    Battle_WaitMode0(30);
    Battle_WaitMode0(10);
    Engine_ActorRunRepeatedMotion(MEARI, 2);
    Battle_WaitMode0(20);
    Event_ShowMessage(MEARI, 0);
    Battle_WaitMode0(10);
    Engine_ActorRunRepeatedMotion(IWAN, 2);
    Battle_WaitMode0(20);
    Event_ShowMessage(IWAN, 0);
    Battle_WaitMode0(10);
    Actor_ShowEmote(GERALD, 0x102, 40);
    Event_ShowMessage(GERALD, 0);
    Battle_WaitMode0(20);
    Engine_ActorSetAnimationAndWait(10, 4);
    Battle_WaitMode0(20);
    Event_ShowMessage(10, 0);
    Battle_WaitMode0(10);
    Engine_ActorRunRepeatedMotion(14, 2);
    Battle_WaitMode0(25);
    Event_ShowMessage(14, 0);
    Battle_WaitMode0(10);
    Engine_ActorRunRepeatedMotion(10, 2);
    Battle_WaitMode0(20);
    Actor_SetSpeed(10, 0x10000, 0x8000);
    Actor_WalkByAndWait(10, 0, -40);
    Actor_FaceDirection(10, FACING_EAST, 0);
    Battle_WaitMode0(20);
    Event_ShowMessage(10, 0);
    Battle_WaitMode0(10);
    Engine_ActorSetAnimationAndWait(14, 3);
    Battle_WaitMode0(30);
    Battle_WaitMode0(10);
    Actor_FaceDirection(13, FACING_SOUTH, 0);
    Actor_FaceDirection(12, FACING_NORTH, 0);
    Battle_WaitMode0(30);
    Object_SetModeById(12, 3);
    Engine_ActorSetAnimationAndWait(13, 3);
    Battle_WaitMode0(30);
    Actor_FaceDirection(13, FACING_WEST, 0);
    Actor_FaceDirection(12, FACING_WEST, 0);
    Battle_WaitMode0(20);
    Engine_ActorSetAnimationAndWait(10, 3);
    Battle_WaitMode0(30);
    Engine_CameraFollowActor(10, 1);
    Actor_WalkByAndWait(10, 0, -32);
    BabiIriguchi_CloseTruthDoor();
    GameFlag_Clear(0x301);
    Camera_MoveTo(-1, -1, -1, 0);
    Engine_EventBegin();
    Actor_ShowEmote(10, 0x102, 40);
    Event_ShowMessage(10, 0);
    Battle_WaitMode0(10);
    Actor_FaceDirection(14, 0xb000, 0);
    Actor_FaceDirection(11, 0xb000, 0);
    Actor_FaceDirection(12, 0xb000, 0);
    Actor_FaceDirection(13, 0xb000, 0);
    Actor_FaceDirection(ACTOR_PARTY_LEADER, FACING_NORTH, 0);
    Actor_FaceDirection(GERALD, FACING_NORTH, 0);
    Actor_FaceDirection(IWAN, FACING_NORTH, 0);
    Actor_FaceDirection(MEARI, FACING_NORTH, 0);
    Battle_WaitMode0(30);
    Actor_ShowEmote(11, 0x100, 0);
    Actor_ShowEmote(12, 0x100, 0);
    Actor_ShowEmote(13, 0x100, 0);
    Actor_ShowEmote(14, 0x100, 70);
    Camera_MoveTo(0x1180000, -1, 0x1380000, 1);
    Engine_CameraWaitForMove();
    Battle_WaitMode0(10);
    Actor_ShowEmote(12, 0x102, 40);
    Event_ShowMessage(12, 0);
    Battle_WaitMode0(10);
    Engine_ActorSetAnimationAndWait(13, 4);
    Battle_WaitMode0(20);
    Event_ShowMessage(13, 0);
    Battle_WaitMode0(10);
    Engine_ActorRunRepeatedMotion(IWAN, 2);
    Battle_WaitMode0(30);
    Actor_FaceDirection(IWAN, FACING_SOUTHEAST, 0);
    Battle_WaitMode0(20);
    Event_ShowMessage(IWAN, 0);
    Battle_WaitMode0(10);
    Actor_FaceDirection(10, FACING_SOUTH, 0);
    Battle_WaitMode0(30);
    Actor_FaceDirection(MEARI, FACING_NORTHEAST, 0);
    Battle_WaitMode0(30);
    Actor_ShowEmote(MEARI, 0x102, 40);
    Actor_FaceDirection(ACTOR_PARTY_LEADER, 0x3000, 0);
    Battle_WaitMode0(20);
    Event_ShowMessage(MEARI, 0);
    Battle_WaitMode0(10);
    Engine_ActorSetAnimationAndWait(GERALD, 4);
    Battle_WaitMode0(20);
    Event_ShowMessage(GERALD, 0);
    Battle_WaitMode0(20);
    Actor_ShowEmote(14, 0x103, 50);
    Actor_FaceDirection(14, FACING_WEST, 0);
    Battle_WaitMode0(20);
    Event_OpenMessage(14, 0);
    Actor_FaceDirection(ACTOR_PARTY_LEADER, FACING_EAST, 0);
    Actor_FaceDirection(11, FACING_WEST, 0);
    Actor_FaceDirection(12, FACING_WEST, 0);
    Actor_FaceDirection(13, FACING_WEST, 0);
    Actor_FaceDirection(IWAN, 0x1000, 0);
    Actor_FaceDirection(MEARI, FACING_NORTHEAST, 0);
    if (Engine_EventChooseYesNo(0, 0) == 0) {
        Battle_WaitMode0(30);
        Engine_ActorRunRepeatedMotion(14, 2);
        Battle_WaitMode0(20);
        Event_ShowMessage(14, 0);
        gEventWork->message++;
    } else {
        Battle_WaitMode0(30);
        Engine_ActorRunRepeatedMotion(14, 2);
        Battle_WaitMode0(20);
        gEventWork->message++;
        Event_ShowMessage(14, 0);
    }
    Battle_WaitMode0(10);
    Engine_ActorRunRepeatedMotion(10, 2);
    Battle_WaitMode0(20);
    Actor_WalkByAndWait(10, 0, 16);
    Actor_FaceDirection(10, FACING_SOUTHEAST, 0);
    Battle_WaitMode0(20);
    Event_ShowMessage(10, 0);
    Battle_WaitMode0(10);
    Actor_FaceDirection(14, FACING_NORTHWEST, 0);
    Battle_WaitMode0(20);
    Engine_ActorSetAnimationAndWait(14, 4);
    Battle_WaitMode0(20);
    Event_ShowMessage(14, 0);
    Battle_WaitMode0(10);
    Engine_ActorSetAnimationAndWait(10, 3);
    Battle_WaitMode0(30);
    Event_ShowMessage(10, 0);
    Battle_WaitMode0(10);
    Engine_ActorRunRepeatedMotion(14, 2);
    Battle_WaitMode0(20);
    Event_OpenMessage(14, 0);
    Battle_WaitMode0(40);
    Actor_FaceDirection(10, 0x5000, 0);
    Battle_WaitMode0(20);
    Actor_ShowEmote(10, 0x101, 60);
    if (Engine_EventChooseYesNo(0, 0) == 0) {
        Battle_WaitMode0(30);
        Actor_FaceDirection(10, FACING_SOUTHEAST, 0);
        Battle_WaitMode0(20);
        Engine_ActorSetAnimationAndWait(10, 3);
        Battle_WaitMode0(30);
        Event_ShowMessage(10, 0);
        gEventWork->message++;
    } else {
        Battle_WaitMode0(30);
        Actor_FaceDirection(10, FACING_SOUTHEAST, 0);
        Battle_WaitMode0(20);
        Engine_ActorSetAnimationAndWait(10, 4);
        Battle_WaitMode0(20);
        gEventWork->message++;
        Event_ShowMessage(10, 0);
    }
    Battle_WaitMode0(10);
    Actor_ShowEmote(14, 0x102, 50);
    Event_ShowMessage(14, 0);
    Battle_WaitMode0(15);
    Engine_ActorSetAnimationAndWait(10, 3);
    Battle_WaitMode0(30);
    Battle_WaitMode0(20);
    Engine_ActorRunRepeatedMotion(14, 2);
    Battle_WaitMode0(40);
    Actor_FaceDirection(14, FACING_WEST, 0);
    Battle_WaitMode0(20);
    Event_ShowMessage(14, 0);
    Battle_WaitMode0(10);
    Engine_ActorSetAnimationAndWait(14, 3);
    Battle_WaitMode0(30);
    Event_ShowMessage(14, 0);
    Battle_WaitMode0(10);
    Actor_FaceDirection(10, 0x5000, 0);
    Battle_WaitMode0(25);
    Engine_ActorRunRepeatedMotion(10, 2);
    Battle_WaitMode0(20);
    Actor_WalkToAndWait(10, 0x138, 0x138);
    Actor_FaceDirection(ACTOR_PARTY_LEADER, FACING_EAST, 0);
    Actor_FaceDirection(10, FACING_WEST, 0);
    Battle_WaitMode0(25);
    Event_ShowMessage(10, 0);
    Battle_WaitMode0(10);
    Engine_ActorFaceEachOther(ACTOR_PARTY_LEADER, GERALD, 0);
    Engine_ActorFaceEachOther(MEARI, IWAN, 0);
    Battle_WaitMode0(30);
    Object_SetModeById(ACTOR_PARTY_LEADER, 3);
    Object_SetModeById(GERALD, 3);
    Object_SetModeById(MEARI, 3);
    Engine_ActorSetAnimationAndWait(IWAN, 3);
    Battle_WaitMode0(30);
    Actor_SetSpeed(GERALD, 0x13333, 0x9999);
    Actor_SetSpeed(IWAN, 0x13333, 0x9999);
    Actor_SetSpeed(MEARI, 0x13333, 0x9999);
    Object_SetModeById(GERALD, 2);
    record = Object_GetById(ACTOR_PARTY_LEADER);
    if (record != 0) {
        Actor_SetDestination(GERALD, record->x.part.pixel, record->z.part.pixel);
    }
    ObjectMotion_CommitCurrentPositionAndActivate(GERALD);
    Actor_SetPosition(GERALD, 0, 0);
    Object_SetModeById(IWAN, 2);
    record = Object_GetById(ACTOR_PARTY_LEADER);
    if (record != 0) {
        Actor_SetDestination(IWAN, record->x.part.pixel, record->z.part.pixel);
    }
    ObjectMotion_CommitCurrentPositionAndActivate(IWAN);
    Actor_SetPosition(IWAN, 0, 0);
    Object_SetModeById(MEARI, 2);
    record = Object_GetById(ACTOR_PARTY_LEADER);
    if (record != 0) {
        Actor_SetDestination(MEARI, record->x.part.pixel, record->z.part.pixel);
    }
    ObjectMotion_CommitCurrentPositionAndActivate(MEARI);
    Actor_SetPosition(MEARI, 0, 0);
    Battle_WaitMode0(10);
    Actor_FaceDirection(ACTOR_PARTY_LEADER, FACING_NORTH, 0);
    Actor_FaceDirection(10, 0xb000, 0);
    Actor_FaceDirection(14, 0xb000, 0);
    Actor_FaceDirection(11, 0xb000, 0);
    Actor_FaceDirection(12, 0xb000, 0);
    Actor_FaceDirection(13, 0xb000, 0);
    Battle_WaitMode0(30);
    Engine_EventEnd();
}

void ActorPresentation_SetSceneCellByFlag985(void)
{
    if (Engine_GameFlagIsSet(0x985) == 0) {
        s32 k5 = 17, k6 = 78;

        Engine_MapCopyCellsLayered(36, 78, 1, 2, k5, k6);
    } else {
        s32 k5 = 17, k6 = 78;

        Engine_MapCopyCellsLayered(34, 78, 1, 2, k5, k6);
    }
}

void SceneState_ApplyRectAt32x78(void)
{
    {
        s32 k5 = 17, k6 = 78;

        Engine_MapCopyCellsLayered(32, 78, 1, 2, k5, k6);
    }
}

void BabiIriguchi_OpenTruthDoor(void)
{
    if (GameFlag_IsSet(0x985) == 0) {
        GameFlag_Set(0x985);
        Audio_PlayCue(157);
        Engine_EventBegin();
        Actor_SetDestination(8, 0x118, 240);
        Actor_SetDestination(9, 0x148, 240);
        ObjectMotion_CommitCurrentPositionAndActivate(8);
        ObjectMotion_CommitCurrentPositionAndActivate(9);
        Map_CopyCellAttributes(81, 14, 4, 1, 17, 14);
        Engine_EventEnd();
        if (GameFlag_IsSet(0x989) == 0) {
            FieldScene_RunActorEventSequence();
        }
    }
}

void BabiIriguchi_CloseTruthDoor(void)
{
    if (GameFlag_IsSet(0x985) != 0) {
        GameFlag_Clear(0x985);
        Audio_PlayCue(157);
        Engine_EventBegin();
        Actor_SetDestination(8, 0x128, 240);
        Actor_SetDestination(9, 0x138, 240);
        ObjectMotion_CommitCurrentPositionAndActivate(8);
        ObjectMotion_CommitCurrentPositionAndActivate(9);
        Map_CopyCellAttributes(0, 14, 4, 1, 17, 14);
        Engine_EventEnd();
        if (GameFlag_IsSet(0x301) != 0) {
            GameFlag_Clear(0x301);
        } else {
            GameFlag_Set(0x301);
        }
    }
}

void BabiIriguchi_FlipTruthDoorSwitch(void)
{
    u8 *work = (u8 *)gEventWork;
    s16 *seeing;

    Engine_EventBegin();
    seeing = (s16 *)(work + 0xcb8);
    if (seeing[0] != 0) {
        if (Engine_GameFlagIsSet(0x985) == 0) {
            /* FAKEMATCH: forced temporaries; the destination stays in two saved registers across both copies, where plain constants are built again for each call. */
            s32 dest_x = 17, dest_y = 78;

            Engine_MessageShowCentered((s32)MsgFieldFlippedSwitch, 1);
            Audio_PlayCue(155);
            Engine_MapCopyCellsLayered(35, 78, 1, 2, dest_x, dest_y);
            Battle_WaitMode0(10);
            Engine_MapCopyCellsLayered(34, 78, 1, 2, dest_x, dest_y);
            Battle_WaitMode0(10);
            BabiIriguchi_OpenTruthDoor();
        }
    } else {
        Engine_EventSetMessage((s32)MsgBabiTruthDoorOpenThoseSeeing);
        Engine_EventShowMessage(-1, 0);
    }
    Engine_EventEnd();
}

struct StagedActor *BabiIriguchi_FindActorAhead(struct StagedActor *actor)
{
    s32 pos[3];
    s32 *p = pos;
    s32 step = StagedActor_DirectionSteps[actor->direction_and_kind >> 12];

    {
        s32 x = actor->x.value;
        s32 z = actor->z.value;

        x += -0x10000 & step;
        z += step << 16;
        p[0] = x;
        p[2] = z;
    }
    p[1] = Map_GetTerrainHeight(actor->transition_mode, p[0], p[2]);
    return StagedActor_FindAtTile(p, actor);
}

void BabiIriguchi_EnableEffectChildOption(void)
{
    ((struct BattleFxScene *)*(void **)gEffectWork)->child_option = 1;
}

void ActorPresentation_PlaceActorTwelveAtTile20And12(void)
{
    s32 *p = Object_GetById(12);
    s32 a = p[2] >> 20;

    if (a == 20) {
        s32 b = p[4] >> 20;

        if (b == 12) {
            ((u8 *)p)[85] = 2;
            p[5] = 0x300000;
            ((u8 *)p)[35] = 2;
            {
                s32 k5 = a, k6 = b;

                Map_CopyCellAttributeRect(38, 12, 1, 1, k5, k6);
            }
        }
    }
}

void SceneActor_PushObjectAheadIfLevel(void)
{
    struct StagedActor *p = (struct StagedActor *)Object_GetById(ACTOR_PARTY_LEADER);
    struct StagedActor *q = BabiIriguchi_FindActorAhead(p);
    s32 diff;

    if (q == 0) {
        return;
    }

    diff = q->y - p->y;

    if (diff >= 0) {

        if (diff < 0x80000) {
        } else {
            return;
        }
    } else if (p->y - q->y >= 0x80000) {
        return;
    }

    StagedActor_AdvancePair();
}

void SceneActor_RunSlotZeroFacingCheck(void)
{
    struct StagedActor *p = (struct StagedActor *)Object_GetById(0);
    struct StagedActor *ahead = BabiIriguchi_FindActorAhead(p);
    s32 m = (p->direction_and_kind + 0x2000) & 0xc000;
    s32 r = -1;

    if (gGameState.movement_mode == 1 || ahead == 0) {
        if (m == 0xc000) {
            r = battle_owner_69();
        }
        if (m == 0x4000) {
            r = FieldEffect_UpdateGridPlacement();
        }
    }
    if (r != 0) {
        if (gGameState.movement_mode != 1) {
            SceneActor_PushObjectAheadIfLevel();
        }
    }
}

const struct SceneEvent *Scene_GetEvents(void)
{
    s16 scene = gGameState.scene;

    if (scene == (s32)&SceneId_BabiIriguchi3) {
        return gBabiIriguchiEvents3;
    }
    if (scene == (s32)&SceneId_BabiIriguchi2) {
        return gBabiIriguchiEvents2;
    }
    if (scene == (s32)&SceneId_BabiIriguchi1) {
        return gBabiIriguchiEvents1;
    }
    return gBabiIriguchiEventsOther;
}

void SceneState_ConfigureRegion82_7AndApply768(void)
{

    s32 a = 18;
    s32 b = 7;

    Map_CopyCellAttributeRect(82, 7, 1, 2, a, b);
    WaitFrames(1);
    Engine_GameFlagSet(768);
}

void SceneState_ApplyRectsAtActors8And9(void)
{
    s32 *p = Object_GetById(8);

    Engine_ActorSetSpritePriority(8, 1);
    Engine_ActorSetSpritePriority(9, 1);
    {
        s32 k5 = 5, k6 = 19;

        Map_CopyCellAttributeRect(69, 19, 3, 3, k5, k6);
    }
    {
        s32 k5 = 17, k6 = 19;

        Map_CopyCellAttributeRect(69, 19, 3, 3, k5, k6);
    }
    {
        s32 k5 = p[2] >> 20, k6 = p[4] >> 20;

        Map_CopyCellAttributeRect(3, 3, 1, 1, k5, k6);
    }
    {
        s32 *q = Object_GetById(9);
        s32 k5 = q[2] >> 20, k6 = q[4] >> 20;

        Map_CopyCellAttributeRect(3, 3, 1, 1, k5, k6);
    }
}

#if defined(TBS_EDITION_DE) || defined(TBS_EDITION_ES) || \
    defined(TBS_EDITION_FR) || defined(TBS_EDITION_IT)
void BabiIriguchi_MatchLeaderPriority(void)
{
    struct FieldSprite *sprite;
    struct FieldActor *actor;
    s32 priority;

    sprite = Object_GetById(ACTOR_PARTY_LEADER)->sprite;
    actor = Object_GetById(8);
    if (actor->z.fixed < Object_GetById(ACTOR_PARTY_LEADER)->z.fixed) {
        Engine_ActorSetSpritePriority(8, sprite->priority);
    } else {
        priority = sprite->priority;
        if (priority != 1)
            priority--;
        Engine_ActorSetSpritePriority(8, priority);
        Object_GetById(8)->priority_flags |= 1;
    }
    actor = Object_GetById(9);
    if (actor->z.fixed < Object_GetById(ACTOR_PARTY_LEADER)->z.fixed) {
        Engine_ActorSetSpritePriority(9, sprite->priority);
    } else {
        priority = sprite->priority;
        if (priority != 1)
            priority--;
        Engine_ActorSetSpritePriority(9, priority);
        Object_GetById(9)->priority_flags |= 1;
    }
}
#endif

struct RowActor {
    u8 unknown_00[8];
    union FieldCoordinate x;
    union FieldCoordinate y;
    union FieldCoordinate z;
    u8 unknown_14[0x0f];
    u8 priority_flags;
    u8 unknown_24[0x2c];
    struct FieldSprite *sprite;
    u8 unknown_54[5];
    u8 collision_flags;
    u8 unknown_5a[4];
    u16 delay;
};

s32 BabiIriguchi_SetupScene(void)
{
    struct FieldActor *actor;
    struct RowActor *row;
    s32 scene;
    /* FAKEMATCH: the game compares the second and third scenes through a copy of the scene in r12, made after Retreat's stores; a plain second variable is folded back into the first. */
    register s32 copy asm("r12");
    struct GameState *state;
    s32 x;
    s32 z;
    u8 zero;

    WaitFrames(1);
    gEventWork->start_transition = SCENE_TRANSITION(TRANSITION_WINDOW, 4);
    state = &gGameState;
    scene = state->scene;
    if (scene != (s32)&SceneId_BabiIriguchi4) {
        state->retreat_entrance = 1;
        state->retreat_scene = (s32)&SceneId_BabiIriguchi3;
        copy = scene;
    }
    if (scene == (s32)&SceneId_BabiIriguchi4) {
        row = (struct RowActor *)Object_GetById(12);
        x = row->x.fixed >> 20;
        if (x == 20) {
            z = row->z.fixed >> 20;
            if (z == 12) {
                Map_CopyCellAttributes(38, 12, 1, 1, x, z);
            }
        }
    } else if (copy == (s32)&SceneId_BabiIriguchi3) {
        Actor_SetChildValue(8, 6);
        Actor_SetChildValue(9, 6);
#if defined(TBS_EDITION_DE) || defined(TBS_EDITION_ES) || \
    defined(TBS_EDITION_FR) || defined(TBS_EDITION_IT)
        ((union FieldObject *)Object_GetById(8))->actor.update =
            (void (*)(union FieldObject *))BabiIriguchi_MatchLeaderPriority;
        ((union FieldObject *)Object_GetById(9))->actor.update =
            (void (*)(union FieldObject *))BabiIriguchi_MatchLeaderPriority;
#endif
        if (state->entrance == 5 && GameFlag_IsSet(0x109) == 0) {
            Actor_SetPosition(9, 0x1380000, 0x1480000);
        }
        SceneState_ApplyRectsAtActors8And9();
        actor = Object_GetById(9);
        actor->y.fixed = Object_GetById(9)->terrain_height;
        if (GameFlag_IsSet(0x300) != 0) {
            Object_SetModeById(10, 4);
            Object_GetById(10)->collision_flags = 0xfe;
            SceneState_ConfigureRegion82_7AndApply768();
        }
        row = (struct RowActor *)Object_GetById(11);
        row->collision_flags = 0;
        row->priority_flags = 0;
        row->delay = 0;
        ((u8 *)row->sprite)[9] |= 12;
        zero = 0;
        ((u8 *)row->sprite)[38] = zero;
        row->sprite->rotation = 0xc000;
        Object_SetModeById(11, 0);
        row = (struct RowActor *)Object_GetById(12);
        row->collision_flags = zero;
        row->priority_flags = zero;
        row->delay = 30;
        ((u8 *)row->sprite)[9] |= 12;
        ((u8 *)row->sprite)[38] = zero;
        row->sprite->rotation = 0x4000;
        Object_SetModeById(12, 0);
        row = (struct RowActor *)Object_GetById(13);
        row->collision_flags = zero;
        row->priority_flags = zero;
        row->delay = 60;
        ((u8 *)row->sprite)[9] |= 12;
        ((u8 *)row->sprite)[38] = zero;
        row->sprite->rotation = 0x8000;
        Object_SetModeById(13, 0);
        row = (struct RowActor *)Object_GetById(14);
        row->collision_flags = zero;
        row->priority_flags = zero;
        row->delay = 90;
        ((u8 *)row->sprite)[9] |= 12;
        ((u8 *)row->sprite)[38] = zero;
        row->sprite->rotation = 0x8000;
        Object_SetModeById(14, 0);
    } else if (copy == (s32)&SceneId_BabiIriguchi2) {
        switch (state->entrance) {
        case 10:
            GameFlag_Set(0x980);
        case 11:
            if (GameFlag_IsSet(0x980) != 0) {
                Map_CopyCellsTo(120, 7, 109, 7, 1, 3);
                Map_CopyCellAttributes(45, 8, 1, 1, 45, 9);
            }
            break;
        case 14:
            OverlayObject_SpawnConfiguredObject(0x1b80000, 0, 0x1220000, 223);
            Map_CopyCellAttributes(22, 13, 1, 1, 27, 13);
            FieldScene_RunScene3c5SequenceA(14);
            break;
        case 16:
            OverlayObject_SpawnConfiguredObject(0x1c00000, 0, 0x1220000, 223);
            Map_CopyCellAttributes(22, 12, 1, 1, 28, 10);
            FieldScene_RunScene3c5SequenceA(16);
            break;
        case 17:
            OverlayObject_SpawnConfiguredObject(0xe80000, 0, 0x2520000, 223);
            Map_CopyCellAttributes(22, 12, 1, 1, 14, 33);
            FieldScene_RunScene3c5SequenceA(17);
            break;
        case 12:
        case 13:
        case 15:
            if (GameFlag_IsSet(0x109) == 0) {
                FieldScene_RunSupplementalSequenceOne();
            }
            break;
        }
    } else if (scene == (s32)&SceneId_BabiIriguchi1) {
        Object_GetById(8)->steering_flags &= ~ACTOR_FACES_MOTION;
        Object_GetById(9)->steering_flags &= ~ACTOR_FACES_MOTION;
        Actor_SetSpeed(8, 0x10000, 0x8000);
        Actor_SetSpeed(9, 0x10000, 0x8000);
        if (GameFlag_IsSet(0x109) == 0) {
            if (state->entrance == 1) {
                GameFlag_Set(0x301);
            } else {
                GameFlag_Clear(0x301);
            }
        }
        if (GameFlag_IsSet(0x988) == 0) {
            Actor_SetPosition(10, -0x400000, -0x400000);
            Actor_SetPosition(11, 0x1180000, 0x1280000);
            Actor_SetPosition(12, 0x1380000, 0xf80000);
            Actor_SetPosition(13, 0x1280000, 0xf80000);
            Actor_SetPosition(14, 0x1400000, 0x1280000);
            Actor_FaceDirection(11, FACING_EAST, 0);
            Actor_FaceDirection(12, FACING_NORTH, 0);
            Actor_FaceDirection(13, FACING_NORTH, 0);
            Actor_FaceDirection(14, FACING_WEST, 0);
            Battle_WaitMode0(5);
        } else if (GameFlag_IsSet(0x989) != 0) {
            Actor_SetPosition(10, 0x1380000, 0x1380000);
            Actor_FaceDirection(10, 0xb000, 0);
            Actor_FaceDirection(11, 0xb000, 0);
            Actor_FaceDirection(12, 0xb000, 0);
            Actor_FaceDirection(13, 0xb000, 0);
            Actor_FaceDirection(14, 0xb000, 0);
            Battle_WaitMode0(5);
        }
        if (GameFlag_IsSet(0x985) != 0) {
            Actor_SetPosition(8, 0x1180000, 0xf00000);
            Actor_SetPosition(9, 0x1480000, 0xf00000);
            Actor_FaceDirection(8, FACING_WEST, 0);
            Actor_FaceDirection(9, FACING_EAST, 0);
            Map_CopyCellAttributes(81, 14, 4, 1, 17, 14);
        }
        if (gGameState.entrance == 3) {
            if (GameFlag_IsSet(0x109) == 0) {
                FieldScene_RunSupplementalSequenceOne();
            }
        }
    } else {
        Object_SetModeById(12, 2);
    }
    return 0;
}
