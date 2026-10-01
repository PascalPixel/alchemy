/* NONMATCHING: 2026-10-01 brief Wave2 direct-call adapter attempt.
 * Removing Iriguchi_SetSpeed from games/THE BROKEN SEAL/SRC/FIELD/BABI_IRIGUCHI/IRIGUCHI.H changes:
 * games/THE BROKEN SEAL/SRC/FIELD/BABI_IRIGUCHI/IRIGUCHI.C FieldScene_RunBranchingActorSequence: push {lr} => push {r5, r6, lr} (885/880 assembly lines).
 * games/THE BROKEN SEAL/SRC/FIELD/BABI_IRIGUCHI/IRIGUCHI.C FieldScene_RunActorEventSequence: push {lr} => push {r5, r6, lr} (943/936 assembly lines).
 * Production source retains this helper with its measured reason.
 */
#ifndef IRIGUCHI_H
#define IRIGUCHI_H

/* The staged-actor code linked here names the imports by the main image's
   names; the entrance's scenes reach them by the same names. */
#define FIELD_STAGED_ACTOR_IMPORTS
#include "TYPES.H"
#include "FIELD_EVENT.H"

/* Preserve the original measured FIELD_EVENT adapter context of this draft. */
static inline void Event_Begin(void)
{
    Engine_EventBegin();
}

static inline void Event_End(void)
{
    Engine_EventEnd();
}

static inline void Event_SetMessage(s32 message)
{
    Engine_EventSetMessage(message);
}

static inline s32 Event_ChooseYesNo(s32 actor, s32 flags)
{
    return Engine_EventChooseYesNo(actor, flags);
}

static inline void Message_ShowCentered(s32 message, s32 flags)
{
    Engine_MessageShowCentered(message, flags);
}

static inline void Event_RequestExit(s32 exit)
{
    Engine_EventRequestExit(exit);
}

static inline void Event_OpenScreen(void)
{
    Engine_EventOpenScreen();
}

static inline void Event_CloseScreen(void)
{
    Engine_EventCloseScreen();
}

static inline void Event_WaitForScreen(void)
{
    Engine_EventWaitForScreen();
}

static inline void Actor_FaceEachOther(s32 actor, s32 other, s32 frames)
{
    Engine_ActorFaceEachOther(actor, other, frames);
}

static inline void Actor_SetAnimationAndWait(s32 actor, s32 animation)
{
    Engine_ActorSetAnimationAndWait(actor, animation);
}

static inline void Actor_StartRepeatedMotion(s32 actor, s32 repeats)
{
    Engine_ActorStartRepeatedMotion(actor, repeats);
}

static inline void Actor_RunRepeatedMotion(s32 actor, s32 repeats)
{
    Engine_ActorRunRepeatedMotion(actor, repeats);
}

static inline void Actor_SetSpritePriority(s32 actor, s32 priority)
{
    Engine_ActorSetSpritePriority(actor, priority);
}

static inline void Actor_SetSpriteFlags(struct FieldActor *actor, s32 flags)
{
    Engine_ActorSetSpriteFlags(actor, flags);
}

static inline void Camera_FollowActor(s32 actor, s32 keep_position)
{
    Engine_CameraFollowActor(actor, keep_position);
}

static inline void Camera_WaitForMove(void)
{
    Engine_CameraWaitForMove();
}

static inline void Map_Redraw(void)
{
    Engine_MapRedraw();
}

static inline s32 Math_Sin(s32 angle)
{
    return Engine_MathSin(angle);
}

static inline s32 Math_Cos(s32 angle)
{
    return Engine_MathCos(angle);
}

static inline void Actor_Jump(s32 actor, s32 height, s32 frames)
{
    Engine_ActorJump(actor, height, frames);
}

static inline void MapRender_WaitForValues(void)
{
    Engine_MapRenderWaitForValues();
}

#include "FIELD_SCENE.H"

#define NULL ((void *)0)
#define FIELD_AT_OFFSET(base, type, offset)     (*(type)((u8 *)(base) + (offset)))

#include "FIELD_EFFECT.H"
#include "STAGED_ACTOR.H"
#include "SPAWN_CONFIGURED_EFFECT.H"
#include "SELECT_OVERLAY_DATA_BY_RUNTIME_SELECTOR.H"
extern u8 gEffectWork[];

struct Owner {
    u8 unk0[9];
    u8 unk9_0 : 2;
    u8 mode : 2;
    u8 unk9_4 : 4;
};

/*
 * Per-frame integrator for an effect record -- resource_3c5. It adds the
 * velocity at +68/+72/+76 into the position at +8/+12/+16, decays two of the
 * three velocities, accumulates the rates at +48/+52, and turns the sprite.
 */

/* Every field is named by offset; the layout is not verified. */
struct MotionEffect {
    u8 pad00[8];
    s32 position[3];
    u8 pad14[4];
    s32 accum18;
    s32 accum1c;
    u8 pad20[3];
    u8 flatla3;
    u8 pad24[12];
    s32 rate30;
    s32 rate34;
    u8 pad38[12];
    s32 velocity[3];
    struct Sprite *sprite;
    u8 pad54;
    u8 mode55;
    u8 pad56[14];
    u16 step64;
    u8 pad66[6];
    u32 callback;
};

/* The entrance's exits, in the overlay's data. */
extern u8 gBabiIriguchiExits[];

/* DOOR.C: the door of truth's two leaves slide apart or together. */
void BabiIriguchi_OpenTruthDoor(void);
void BabiIriguchi_CloseTruthDoor(void);

/* The occupant one square ahead of an actor. */
struct StagedActor *BabiIriguchi_FindActorAhead(struct StagedActor *actor);

/* Main-image services reached through the overlay's import veneers. The
 * staged-actor code linked into this overlay names several of them, so the
 * entrance's scenes call them by those names too. */
void WaitFrames(s32 frames);
void Battle_WaitMode0(s32 frames);
void ObjectMotion_SetSpeedParameters(s32 actor, s32 speed, s32 acceleration);
void ObjectMotion_OffsetPositionAndResetMotion(s32 actor, s32 dx, s32 dz);
void ObjectMotion_CommitCurrentPositionAndActivate(s32 actor);
void Object_SetModeById(s32 actor, s32 mode);
void Map_CopyCellAttributeRect(s32 src_x, s32 src_y, s32 width, s32 height,
                               s32 dest_x, s32 dest_y);
void BattleFx_SetPhaseRequest();
void Battle_ResetEffectCounter();
void Motion_LaunchFromFocusedObject();
void BattleFx_RunRisingObjectSequence();
s32 FieldEffect_UpdateGridPlacement();
s32 battle_owner_69();
void BabiIriguchi_JumpFromLedge();

/* The event services the entrance's scripts use, wrapped as the field event
 * header wraps its own but reaching the imports by the names above. */
static __inline__ void Iriguchi_Wait(s32 frames)
{
    Battle_WaitMode0(frames);
}

static __inline__ void Iriguchi_TaskWait(s32 frames)
{
    WaitFrames(frames);
}







static __inline__ void Iriguchi_CopyCellAttributes(s32 src_x, s32 src_y, s32 width, s32 height,
                                                   s32 dest_x, s32 dest_y)
{
    /* FAKEMATCH: forwarding through this helper preserves measured instruction order in its callers; see the retained direct-call draft. */
    Map_CopyCellAttributeRect(src_x, src_y, width, height, dest_x, dest_y);
}

static __inline__ s32 Iriguchi_Divide(s32 dividend, s32 divisor)
{
    return dividend / divisor;
}

/*
 * Distance between two three-component 16.16 fixed-point positions.
 *
 * Each argument walks three consecutive 16.16 words in x, y, z order. The
 * per-axis deltas are taken in fixed point, shifted down to integers, squared,
 * and summed; the total is passed to the resident IWRAM integer square root.
 *
 * Expressions are preserved exactly as reconstructed: the walking-pointer form
 * is load-bearing for byte-identity and must not become struct field access.
 */

/* Copy the low two mode bits into the object's owner record. */

/* Contiguous unnamed leaf-owner run for resource_3c5. */

/*
 * Configure this overlay's scene parameters, enable the configuration, and
 * apply the final magnitude.  The complete owner is the saved-link function
 * at 0x020027f4-0x02002820; its stack frame holds arguments five and six for
 * the first call.
 */


/* The scene step counter at 0x1d8 of the shared scene work record. */
static __inline__ void bump_step_02001238(s32 amount)
{
    gEventWork->message += amount;
}

#endif

#include "TYPES.H"
#include "CALL.H"
#include "STAGED_ACTOR.H"

void Effect_AdvanceMotion(struct MotionEffect *effect);
void OverlayObject_WaitUntilIdle(s32 *obj);
extern const struct SceneEntrance gBabiIriguchiEntrances3[];
extern const struct SceneEntrance gBabiIriguchiEntrances2[];
extern const struct SceneEntrance gBabiIriguchiEntrances1[];
extern const struct SceneEntrance gBabiIriguchiEntrancesOther[];
extern const struct SceneRegion gBabiIriguchiRegions3[];
extern const struct ScenePlacement gBabiIriguchiPlacements3[];
extern const struct ScenePlacement gBabiIriguchiPlacements2[];
extern const struct ScenePlacement gBabiIriguchiPlacements1[];
extern const struct ScenePlacement gBabiIriguchiPlacementsOther[];
void Engine_EventBegin();
void Engine_ActorShowEmote();
void ObjectMotion_SetSpeedParameters();
void Engine_ActorJump();
void ObjectMotion_OffsetPositionAndResetMotion();
void ObjectMotion_CommitCurrentPositionAndActivate();
void Engine_ActorFaceDirection();
void SceneState_ApplyRectsAtActors8And9();
void Engine_EventEnd();
extern u8 MsgBabiSeemsLocked[];
extern u8 MsgBabiTheDoor[];
extern u8 MsgBabiYoureSureTheyWentThrough[];
extern u8 MsgBabiTruthDoorOpenThoseSeeing[];
extern u8 MsgFieldFlippedSwitch[];
void FieldScene_RunActorEventSequence(void);

extern s32 StagedActor_DirectionSteps[];
s32 Map_GetTerrainHeight(s32 mode, s32 x, s32 z);

s32 battle_owner_69(void);
s32 FieldEffect_UpdateGridPlacement(void);
void SceneActor_PushObjectAheadIfLevel(void);
extern const struct SceneEvent gBabiIriguchiEvents3[];
extern const struct SceneEvent gBabiIriguchiEvents2[];
extern const struct SceneEvent gBabiIriguchiEvents1[];
extern const struct SceneEvent gBabiIriguchiEventsOther[];

void SceneState_SetValue8Mode66(void)
{
    BattleFx_SetPhaseRequest(8, 66);
}

void OverlayObject_WaitUntilIdle(s32 *obj)
{
    s32 i = 60;

    while (i != 0) {
        Iriguchi_TaskWait(1);
        i--;
        if (obj[10] == 0) {
            break;
        }
    }
}

/*
 * The decay of the Z velocity stays a signed divide by sixteen: that shape is
 * what reproduces the negative bias and arithmetic shift in the reference.
 */
void Effect_AdvanceMotion(struct MotionEffect *effect)
{
    s32 velocity_z;
    struct Sprite *sprite;
    s32 velocity_x;

    /* This block orders the Z load after the Y store; do not flatten it. */
    /* FAKEMATCH: removing this one-pass block changes instruction scheduling; see its retained draft. */
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

/* The leader drops into the entrance from high above: raised far over the
   floor with the words at +68 and +72 set, the motion byte at +85 lets the
   fall play once the screen has opened. */

/* The arrival by entrance 3 the scene start plays until its flag is set: the
   leader lands in a ring of seventeen sparks and recovers. */
void FieldScene_RunSupplementalSequenceOne(void)
{
    u32 i;
    struct FieldActor *leader;
    struct EffectOptions options;
    s32 vec[3];

    Event_Begin();
    Camera_MoveTo(-1, -1, -1, 0);
    Map_Redraw();
    Iriguchi_TaskWait(1);
    *(s32 *)((u8 *)Object_GetById(0) + 12) = 0x820000;
    *(s32 *)((u8 *)Object_GetById(0) + 72) = 0x8000;
    *(s32 *)((u8 *)Object_GetById(0) + 68) = 0;
    *(u8 *)((u8 *)Object_GetById(0) + 85) = 0;
    Event_OpenScreen();
    Event_WaitForScreen();
    Iriguchi_Wait(30);
    Audio_PlayCue(204);
    *(u8 *)((u8 *)Object_GetById(0) + 85) = 3;
    Iriguchi_Wait(24);
    leader = Object_GetById(ACTOR_PARTY_LEADER);
    options.palette = 7;
    options.update = (void (*)(union FieldObject *))Effect_AdvanceMotion;
    options.start_scale_x = 0xcccc;
    options.start_scale_y = 0xcccc;
    for (i = 0; i < 17; i++) {
        vec[0] = Math_Cos(i << 12);
        vec[1] = 0;
        vec[2] = Math_Sin(i << 12);
        vec[0] += vec[0] / 2;
        Effect_Spawn(leader->x.fixed, leader->y.fixed, leader->z.fixed, vec[0], vec[1], vec[2],
                      EFFECT_USE_UPDATE | EFFECT_USE_START_SCALE | EFFECT_USE_PALETTE | 1, &options);
    }
    Audio_PlayCue(188);
    Actor_SetAttachedEffect(ACTOR_PARTY_LEADER, 0x101);
    Object_SetModeById(ACTOR_PARTY_LEADER, 22);
    Work_SetValuesIfNonNegative(0x50000, 0x50000, 0x10000);
    Work_SetValuesIfNonNegative(-1, -1, 0xe666);
    MapRender_WaitForValues();
    Actor_SetAttachedEffect(ACTOR_PARTY_LEADER, 0x100);
    *(s32 *)((u8 *)Object_GetById(0) + 72) = 0x10000;
    *(s32 *)((u8 *)Object_GetById(0) + 68) = 0x4000;
    Event_End();
}

/* The leader falls in, hidden, and at once leaves by the exit given. */
void FieldScene_RunScene3c5SequenceA(s32 exit)
{
    u8 *leader;

    Event_Begin();
    Camera_MoveTo(-1, -1, -1, 0);
    Map_Redraw();
    Iriguchi_TaskWait(1);
    leader = (u8 *)Actor_Get(ACTOR_PARTY_LEADER);
    *(s32 *)(leader + 12) = 0x820000;
    leader = (u8 *)Actor_Get(ACTOR_PARTY_LEADER);
    *(s32 *)(leader + 72) = 0x4000;
    leader = (u8 *)Actor_Get(ACTOR_PARTY_LEADER);
    *(s32 *)(leader + 68) = 0;
    *(u8 *)((u8 *)Object_GetById(0) + 85) = 0;
    Actor_SetSpriteFlags(Actor_Get(ACTOR_PARTY_LEADER), 0);
    Event_OpenScreen();
    Event_WaitForScreen();
    Iriguchi_Wait(10);
    Audio_PlayCue(204);
    *(u8 *)((u8 *)Object_GetById(0) + 85) = 3;
    leader = (u8 *)Actor_Get(ACTOR_PARTY_LEADER);
    *(s32 *)(leader + 40) = -0x50000;
    OverlayObject_WaitUntilIdle((s32 *)Actor_Get(ACTOR_PARTY_LEADER));
    Actor_SetChildValue(ACTOR_PARTY_LEADER, 15);
    Event_RequestExit(exit);
    Event_End();
}

/* Where the party appears in the scene it enters. */
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

/* Only the third scene has regions. */
const struct SceneRegion *Scene_GetRegions(void)
{
    if (gGameState.scene == (s32)&SceneId_BabiIriguchi3) {
        return gBabiIriguchiRegions3;
    }
    return 0;
}

/* The entrance's exits, which the main image asks for through the overlay's
 * entry veneers. */
u8 *BabiIriguchi_GetExits(void)
{
    return gBabiIriguchiExits;
}

/* The actors placed in each of the entrance's scenes. */
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
    s32 rec7;

    rec7 = (s32)Object_GetById(0);
    Engine_EventBegin();
    if ((*(s32 *)(rec7 + 8) >> 20) != 6) {
        if ((*(s32 *)(rec7 + 8) >> 20) != 18) {
            goto done;
        }
    }
    if ((*(s32 *)(rec7 + 16) >> 20) == 20) {
        *(s32 *)(rec7 + 56) = -0x80000000;
        *(s32 *)(rec7 + 64) = -0x80000000;
        Call3(Engine_ActorShowEmote, 0, 0x100, 20);
        Call3(ObjectMotion_SetSpeedParameters, 0, 0x20000, 0x10000);
        Engine_ActorJump(0, 4, 0);
{ u16 dir = *(u16 *)(rec7 + 6); if ((u16)(dir + 0x4fff) > 0x1fff && (u16)(dir - 0x3001) > 0x1fff) goto step_down; }
        ObjectMotion_OffsetPositionAndResetMotion(0, 16, 0);
        ObjectMotion_CommitCurrentPositionAndActivate(0);
        Call3(Engine_ActorFaceDirection, 0, 0x8000, 20);
        goto done;
        step_down:;
        Call3(ObjectMotion_OffsetPositionAndResetMotion, 0, 0, -16);
        ObjectMotion_CommitCurrentPositionAndActivate(0);
        Call3(Engine_ActorFaceDirection, 0, 0x4000, 20);
    }
    done:;
    SceneState_ApplyRectsAtActors8And9();
    Engine_EventEnd();
}

void FieldScene_RunFourCallSequence(void)
{
    Event_Begin();
    StagedActor_AdvancePair();
    BabiIriguchi_JumpFromLedge();
    Event_End();
}

void SceneState_BranchOnActorEightOrNineTile(void)
{
    s32 *p = Actor_Get(9);

    if ((((s32 *)Object_GetById(0))[2] >> 20) <= 12) {
        p = Actor_Get(8);
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

/* resource_3c5 owner at 0x02001158, 42 bytes. */
void ResetSceneParametersAndFinishSetup(void)
{
    Camera_MoveTo(-1, -1, -1, 0);
    BattleFx_RunRisingObjectSequence(0, 6, 0);
    Event_CloseScreen();
    Event_WaitForScreen();
}

void FieldScene_RunStep11(void)
{
    Event_Begin();
    ResetSceneParametersAndFinishSetup();
    Event_RequestExit(11);
    Event_End();
}

void FieldScene_RunStep12WithPosition(void)
{
    Event_Begin();
    OverlayObject_SpawnConfiguredObject(0x1d00000, 0, 0x1220000, 223);
    ResetSceneParametersAndFinishSetup();
    Event_RequestExit(12);
    Event_End();
}

void FieldScene_RunStep13WithTwoPositions(void)
{
    Event_Begin();
    OverlayObject_SpawnConfiguredObject(0x8f0000, 0, 0x1220000, 223);
    OverlayObject_SpawnConfiguredObject(0x790000, 0, 0x11e0000, 253);
    ResetSceneParametersAndFinishSetup();
    Event_RequestExit(13);
    Event_End();
}

void FieldScene_RunStep15(void)
{
    Event_Begin();
    ResetSceneParametersAndFinishSetup();
    Event_RequestExit(15);
    Event_End();
}

void FieldScene_RunStepWithValue2693(void)
{
    Event_Begin();
    Object_SetModeById(ACTOR_PARTY_LEADER, 1);
    Message_ShowCentered((s32)MsgBabiSeemsLocked, 1);
    Event_End();
}

void FieldScene_RunBranchingActorSequence(void)
{
    s32 record;

    GameFlag_Set(0x988);
    GameFlag_Set(0x98a);
    Event_Begin();
    Battle_ResetEffectCounter();
    Event_SetMessage((s32)MsgBabiYoureSureTheyWentThrough);
    ObjectMotion_SetSpeedParameters(ACTOR_PARTY_LEADER, 0x10000, 0x8000);
    Actor_WalkToAndWait(ACTOR_PARTY_LEADER, 0x128, 0x160);
    Actor_FaceDirection(ACTOR_PARTY_LEADER, 0xc000, 0);
    Iriguchi_Wait(10);
    Call4(Motion_LaunchFromFocusedObject, 10, 16, 0, 0xc000);
    Call4(Motion_LaunchFromFocusedObject, 1, -8, 16, 0xc000);
    Call4(Motion_LaunchFromFocusedObject, 2, 8, 16, 0xc000);
    Call4(Motion_LaunchFromFocusedObject, 3, 24, 16, 0xc000);
    ObjectMotion_CommitCurrentPositionAndActivate(ACTOR_MIA);
    Iriguchi_Wait(20);
    Event_ShowMessage(14, 0);
    Iriguchi_Wait(10);
    Camera_SetSpeed(0x30000, 0x6000);
    Camera_MoveTo(0x1180000, -1, 0x1200000, 1);
    Camera_WaitForMove();
    Iriguchi_Wait(20);
    Actor_SetAnimationAndWait(11, 3);
    Iriguchi_Wait(30);
    Event_ShowMessage(11, 0);
    Iriguchi_Wait(10);
    Actor_FaceDirection(14, 0xc000, 0);
    Actor_FaceDirection(11, 0xc000, 0);
    Iriguchi_Wait(30);
    Iriguchi_Wait(10);
    Actor_RunRepeatedMotion(14, 2);
    Iriguchi_Wait(20);
    Event_ShowMessage(14, 0);
    Iriguchi_Wait(10);
    Actor_StartRepeatedMotion(13, 2);
    Actor_RunRepeatedMotion(12, 2);
    Iriguchi_Wait(40);
    Actor_StartRepeatedMotion(13, 2);
    Actor_RunRepeatedMotion(12, 2);
    Iriguchi_Wait(40);
    Actor_StartRepeatedMotion(13, 2);
    Actor_RunRepeatedMotion(12, 2);
    Iriguchi_Wait(40);
    Actor_ShowEmote(12, 0x102, 50);
    Event_ShowMessage(12, 0);
    Iriguchi_Wait(10);
    Actor_FaceDirection(13, 0, 0);
    Iriguchi_Wait(25);
    Actor_RunRepeatedMotion(13, 2);
    Iriguchi_Wait(20);
    Event_ShowMessage(13, 0);
    Iriguchi_Wait(10);
    Actor_FaceDirection(12, 0x8000, 0);
    Iriguchi_Wait(20);
    Actor_SetAnimationAndWait(13, 4);
    Iriguchi_Wait(20);
    Event_ShowMessage(13, 0);
    Iriguchi_Wait(10);
    Actor_SetAnimationAndWait(11, 4);
    Iriguchi_Wait(20);
    Event_ShowMessage(11, 0);
    Iriguchi_Wait(20);
    Actor_ShowEmote(13, 0x107, 40);
    Iriguchi_Wait(10);
    Actor_FaceDirection(13, 0x4000, 0);
    Iriguchi_Wait(20);
    Event_ShowMessage(13, 0);
    Iriguchi_Wait(20);
    Actor_ShowEmote(13, 0x101, 75);
    Actor_ShowEmote(14, 0x101, 60);
    Actor_FaceDirection(12, 0x4000, 0);
    Iriguchi_Wait(20);
    Actor_FaceDirection(11, 0x4000, 0);
    Iriguchi_Wait(20);
    Actor_FaceDirection(14, 0x4000, 0);
    Iriguchi_Wait(30);
    Event_ShowMessage(14, 0);
    Camera_MoveTo(0x1180000, -1, 0x1400000, 1);
    Camera_WaitForMove();
    Iriguchi_Wait(20);
    Actor_SetAnimationAndWait(10, 3);
    Iriguchi_Wait(30);
    Event_ShowMessage(10, 0);
    Iriguchi_Wait(10);
    Actor_SetAnimationAndWait(10, 4);
    Iriguchi_Wait(20);
    Event_ShowMessage(10, 0);
    Iriguchi_Wait(20);
    Actor_ShowEmote(14, 0x105, 60);
    Event_ShowMessage(14, 0);
    Iriguchi_Wait(10);
    Actor_SetAnimationAndWait(ACTOR_MIA, 4);
    Iriguchi_Wait(20);
    Event_ShowMessage(ACTOR_MIA, 0);
    Iriguchi_Wait(10);
    Actor_RunRepeatedMotion(ACTOR_IVAN, 2);
    Iriguchi_Wait(20);
    Event_ShowMessage(ACTOR_IVAN, 0);
    Iriguchi_Wait(10);
    Actor_RunRepeatedMotion(ACTOR_GERALD, 2);
    Iriguchi_Wait(20);
    Event_OpenMessage(ACTOR_GERALD, 0);
    if (Event_ChooseYesNo(0, 0) == 0) {
        Iriguchi_Wait(30);
        Actor_FaceDirection(10, 0x8000, 0);
        Iriguchi_Wait(30);
        Actor_SetAnimationAndWait(10, 3);
        Iriguchi_Wait(30);
        Event_ShowMessage(10, 0);
        bump_step_02001238(1);
    } else {
        Iriguchi_Wait(30);
        Actor_FaceDirection(10, 0x8000, 0);
        Iriguchi_Wait(30);
        Actor_SetAnimationAndWait(10, 4);
        Iriguchi_Wait(20);
        bump_step_02001238(1);
        Event_ShowMessage(10, 0);
    }
    Iriguchi_Wait(10);
    Actor_ShowEmote(14, 0x101, 60);
    ObjectMotion_SetSpeedParameters(14, 0x10000, 0x8000);
    Actor_WalkByAndWait(14, 0, 16);
    Iriguchi_Wait(20);
    Event_ShowMessage(14, 0);
    Iriguchi_Wait(10);
    Actor_FaceDirection(10, 0xc000, 0);
    Iriguchi_Wait(35);
    Event_ShowMessage(10, 0);
    Iriguchi_Wait(10);
    Actor_RunRepeatedMotion(14, 2);
    Iriguchi_Wait(20);
    Event_ShowMessage(14, 0);
    Iriguchi_Wait(20);
    Actor_ShowEmote(ACTOR_MIA, 0x102, 40);
    Event_ShowMessage(ACTOR_MIA, 0);
    Iriguchi_Wait(10);
    Actor_RunRepeatedMotion(ACTOR_IVAN, 2);
    Iriguchi_Wait(20);
    Event_ShowMessage(ACTOR_IVAN, 0);
    Iriguchi_Wait(10);
    Actor_SetAnimationAndWait(ACTOR_GERALD, 3);
    Iriguchi_Wait(30);
    Event_ShowMessage(ACTOR_GERALD, 0);
    Iriguchi_Wait(20);
    Actor_ShowEmote(14, 0x100, 40);
    Event_OpenMessage(14, 0);
    if (Event_ChooseYesNo(0, 0) == 0) {
        Iriguchi_Wait(30);
        Actor_SetAnimationAndWait(14, 4);
        Battle_WaitMode0(20);
        Event_ShowMessage(14, 0);
        bump_step_02001238(1);
    } else {
        Iriguchi_Wait(30);
        Engine_ActorSetAnimationAndWait(14, 4);
        Iriguchi_Wait(20);
        bump_step_02001238(1);
        Event_ShowMessage(14, 0);
    }
    Iriguchi_Wait(10);
    Actor_ShowEmote(10, 0x102, 50);
    Event_ShowMessage(10, 0);
    Iriguchi_Wait(10);
    Actor_RunRepeatedMotion(13, 2);
    Iriguchi_Wait(20);
    ObjectMotion_SetSpeedParameters(13, 0x14ccc, 0xa666);
    Actor_WalkByAndWait(13, 0, 16);
    Iriguchi_Wait(20);
    Event_ShowMessage(13, 0);
    Iriguchi_Wait(10);
    ObjectMotion_SetSpeedParameters(12, 0x14ccc, 0xa666);
    Actor_WalkByAndWait(12, 0, 16);
    Iriguchi_Wait(20);
    Actor_ShowEmote(12, 0x107, 50);
    Event_ShowMessage(12, 0);
    Iriguchi_Wait(10);
    Actor_SetAnimationAndWait(14, 3);
    Iriguchi_Wait(30);
    Iriguchi_Wait(10);
    Actor_ShowEmote(10, 0x102, 60);
    Actor_FaceDirection(10, 0x8000, 0);
    Iriguchi_Wait(25);
    Event_ShowMessage(10, 0);
    Iriguchi_Wait(10);
    Actor_RunRepeatedMotion(10, 2);
    Iriguchi_Wait(20);
    Event_OpenMessage(10, 0);
    if (Event_ChooseYesNo(0, 0) == 0) {
        Iriguchi_Wait(30);
        Actor_SetAnimationAndWait(10, 3);
        Battle_WaitMode0(30);
        Event_ShowMessage(10, 0);
        bump_step_02001238(1);
    } else {
        Iriguchi_Wait(30);
        Actor_SetAnimationAndWait(10, 4);
        Iriguchi_Wait(20);
        bump_step_02001238(1);
        Event_ShowMessage(10, 0);
    }
    Iriguchi_Wait(10);
    Actor_FaceDirection(10, 0xc000, 0);
    Iriguchi_Wait(35);
    Actor_SetAnimationAndWait(14, 3);
    Iriguchi_Wait(30);
    Actor_FaceDirection(14, 0xb000, 0);
    Iriguchi_Wait(40);
    Event_ShowMessage(14, 0);
    Iriguchi_Wait(10);
    Actor_FaceEachOther(12, 13, 50);
    Object_SetModeById(12, 3);
    Actor_SetAnimationAndWait(13, 3);
    Iriguchi_Wait(30);
    Actor_FaceDirection(12, 0x4000, 0);
    Actor_FaceDirection(13, 0x4000, 0);
    Iriguchi_Wait(20);
    Object_SetModeById(12, 3);
    Actor_SetAnimationAndWait(13, 3);
    Iriguchi_Wait(30);
    ObjectMotion_SetSpeedParameters(12, 0x10000, 0x8000);
    ObjectMotion_SetSpeedParameters(13, 0x10000, 0x8000);
    Actor_WalkBy(12, 32, 0);
    Actor_WalkByAndWait(13, 32, 0);
    Actor_WalkBy(12, 0, 16);
    Actor_WalkByAndWait(13, 16, 0);
    Actor_WalkTo(13, 0x158, 0x138);
    Actor_WalkToAndWait(12, 0x158, 0x150);
    Object_SetModeById(13, 1);
    Actor_FaceDirection(12, 0x8000, 0);
    Actor_FaceDirection(13, 0x8000, 0);
    Iriguchi_Wait(10);
    Actor_FaceDirection(14, 0x4000, 0);
    Iriguchi_Wait(20);
    Event_ShowMessage(14, 0);
    Iriguchi_Wait(10);
    ObjectMotion_SetSpeedParameters(14, 0x10000, 0x8000);
    Actor_WalkToAndWait(14, 0x148, 0x138);
    Actor_FaceDirection(14, 0x8000, 0);
    Iriguchi_Wait(30);
    ObjectMotion_SetSpeedParameters(11, 0x10000, 0x8000);
    Actor_WalkToAndWait(11, 0x148, 0x148);
    Actor_FaceDirection(11, 0x8000, 0);
    Iriguchi_Wait(20);
    ObjectMotion_SetSpeedParameters(ACTOR_GERALD, 0x13333, 0x9999);
    ObjectMotion_SetSpeedParameters(ACTOR_IVAN, 0x13333, 0x9999);
    ObjectMotion_SetSpeedParameters(ACTOR_MIA, 0x13333, 0x9999);
    Object_SetModeById(ACTOR_GERALD, 2);
    record = Object_GetById(ACTOR_PARTY_LEADER);
    if (record != 0) {
        Actor_SetDestination(ACTOR_GERALD, *(s16 *)(record + 10), *(s16 *)(record + 18));
    }
    ObjectMotion_CommitCurrentPositionAndActivate(ACTOR_GERALD);
    Actor_SetPosition(ACTOR_GERALD, 0, 0);
    Object_SetModeById(ACTOR_IVAN, 2);
    record = Object_GetById(ACTOR_PARTY_LEADER);
    if (record != 0) {
        Actor_SetDestination(ACTOR_IVAN, *(s16 *)(record + 10), *(s16 *)(record + 18));
    }
    ObjectMotion_CommitCurrentPositionAndActivate(ACTOR_IVAN);
    Actor_SetPosition(ACTOR_IVAN, 0, 0);
    Object_SetModeById(ACTOR_MIA, 2);
    record = Object_GetById(ACTOR_PARTY_LEADER);
    if (record != 0) {
        Actor_SetDestination(ACTOR_MIA, *(s16 *)(record + 10), *(s16 *)(record + 18));
    }
    ObjectMotion_CommitCurrentPositionAndActivate(ACTOR_MIA);
    Actor_SetPosition(ACTOR_MIA, 0, 0);
    Iriguchi_Wait(10);
    Event_End();
}

void FieldScene_RunActorEventSequence(void)
{
    s32 record;

    GameFlag_Set(0x989);
    Event_Begin();
    Battle_ResetEffectCounter();
    Event_SetMessage((s32)MsgBabiTheDoor);
    ObjectMotion_SetSpeedParameters(ACTOR_PARTY_LEADER, 0x10000, 0x8000);
    Actor_WalkToAndWait(ACTOR_PARTY_LEADER, 0x128, 0x138);
    Actor_FaceDirection(ACTOR_PARTY_LEADER, 0, 0);
    Iriguchi_Wait(10);
    Motion_LaunchFromFocusedObject(1, 0, 16, 0);
    Call4(Motion_LaunchFromFocusedObject, 2, -16, -8, 0);
    Motion_LaunchFromFocusedObject(3, -16, 24, 0);
    ObjectMotion_CommitCurrentPositionAndActivate(ACTOR_MIA);
    Iriguchi_Wait(20);
    Camera_SetSpeed(0x30000, 0x6000);
    Camera_MoveTo(0x1180000, -1, 0x1480000, 1);
    Camera_WaitForMove();
    Iriguchi_Wait(10);
    Iriguchi_Wait(10);
    Actor_FaceDirection(10, 0xb000, 0);
    Iriguchi_Wait(10);
    Actor_ShowEmote(10, 0x100, 40);
    Event_ShowMessage(10, 0);
    Actor_Jump(10, 4, 13);
    Actor_Jump(10, 4, 30);
    Iriguchi_Wait(10);
    Actor_ShowEmote(11, 0x100, 0);
    Actor_ShowEmote(12, 0x100, 0);
    Actor_ShowEmote(13, 0x100, 0);
    Actor_ShowEmote(14, 0x100, 40);
    Iriguchi_Wait(10);
    Actor_FaceDirection(14, 0xb000, 0);
    Actor_FaceDirection(11, 0xb000, 0);
    Actor_FaceDirection(12, 0xb000, 0);
    Actor_FaceDirection(13, 0xb000, 0);
    Iriguchi_Wait(30);
    Actor_RunRepeatedMotion(14, 2);
    Iriguchi_Wait(20);
    Event_ShowMessage(14, 0);
    Iriguchi_Wait(20);
    Actor_ShowEmote(13, 0x102, 40);
    Event_ShowMessage(13, 0);
    Iriguchi_Wait(20);
    Actor_ShowEmote(12, 0x101, 50);
    Actor_FaceDirection(12, 0x8000, 0);
    Iriguchi_Wait(25);
    Event_ShowMessage(12, 0);
    Iriguchi_Wait(10);
    Actor_FaceDirection(14, 0x8000, 0);
    Actor_FaceDirection(11, 0x8000, 0);
    Actor_FaceDirection(13, 0x8000, 0);
    Iriguchi_Wait(30);
    Iriguchi_Wait(10);
    Actor_RunRepeatedMotion(ACTOR_MIA, 2);
    Iriguchi_Wait(20);
    Event_ShowMessage(ACTOR_MIA, 0);
    Iriguchi_Wait(10);
    Actor_RunRepeatedMotion(ACTOR_IVAN, 2);
    Iriguchi_Wait(20);
    Event_ShowMessage(ACTOR_IVAN, 0);
    Iriguchi_Wait(10);
    Actor_ShowEmote(ACTOR_GERALD, 0x102, 40);
    Event_ShowMessage(ACTOR_GERALD, 0);
    Iriguchi_Wait(20);
    Actor_SetAnimationAndWait(10, 4);
    Iriguchi_Wait(20);
    Event_ShowMessage(10, 0);
    Iriguchi_Wait(10);
    Actor_RunRepeatedMotion(14, 2);
    Iriguchi_Wait(25);
    Event_ShowMessage(14, 0);
    Iriguchi_Wait(10);
    Actor_RunRepeatedMotion(10, 2);
    Iriguchi_Wait(20);
    ObjectMotion_SetSpeedParameters(10, 0x10000, 0x8000);
    Actor_WalkByAndWait(10, 0, -40);
    Actor_FaceDirection(10, 0, 0);
    Iriguchi_Wait(20);
    Event_ShowMessage(10, 0);
    Iriguchi_Wait(10);
    Actor_SetAnimationAndWait(14, 3);
    Iriguchi_Wait(30);
    Iriguchi_Wait(10);
    Actor_FaceDirection(13, 0x4000, 0);
    Actor_FaceDirection(12, 0xc000, 0);
    Iriguchi_Wait(30);
    Object_SetModeById(12, 3);
    Actor_SetAnimationAndWait(13, 3);
    Iriguchi_Wait(30);
    Actor_FaceDirection(13, 0x8000, 0);
    Actor_FaceDirection(12, 0x8000, 0);
    Iriguchi_Wait(20);
    Actor_SetAnimationAndWait(10, 3);
    Iriguchi_Wait(30);
    Camera_FollowActor(10, 1);
    Actor_WalkByAndWait(10, 0, -32);
    BabiIriguchi_CloseTruthDoor();
    GameFlag_Clear(0x301);
    Camera_MoveTo(-1, -1, -1, 0);
    Event_Begin();
    Actor_ShowEmote(10, 0x102, 40);
    Event_ShowMessage(10, 0);
    Iriguchi_Wait(10);
    Actor_FaceDirection(14, 0xb000, 0);
    Actor_FaceDirection(11, 0xb000, 0);
    Actor_FaceDirection(12, 0xb000, 0);
    Actor_FaceDirection(13, 0xb000, 0);
    Actor_FaceDirection(ACTOR_PARTY_LEADER, 0xc000, 0);
    Actor_FaceDirection(ACTOR_GERALD, 0xc000, 0);
    Actor_FaceDirection(ACTOR_IVAN, 0xc000, 0);
    Actor_FaceDirection(ACTOR_MIA, 0xc000, 0);
    Iriguchi_Wait(30);
    Actor_ShowEmote(11, 0x100, 0);
    Actor_ShowEmote(12, 0x100, 0);
    Actor_ShowEmote(13, 0x100, 0);
    Actor_ShowEmote(14, 0x100, 70);
    Camera_MoveTo(0x1180000, -1, 0x1380000, 1);
    Camera_WaitForMove();
    Iriguchi_Wait(10);
    Actor_ShowEmote(12, 0x102, 40);
    Event_ShowMessage(12, 0);
    Iriguchi_Wait(10);
    Actor_SetAnimationAndWait(13, 4);
    Iriguchi_Wait(20);
    Event_ShowMessage(13, 0);
    Iriguchi_Wait(10);
    Actor_RunRepeatedMotion(ACTOR_IVAN, 2);
    Iriguchi_Wait(30);
    Actor_FaceDirection(ACTOR_IVAN, 0x2000, 0);
    Iriguchi_Wait(20);
    Event_ShowMessage(ACTOR_IVAN, 0);
    Iriguchi_Wait(10);
    Actor_FaceDirection(10, 0x4000, 0);
    Iriguchi_Wait(30);
    Actor_FaceDirection(ACTOR_MIA, 0xe000, 0);
    Iriguchi_Wait(30);
    Actor_ShowEmote(ACTOR_MIA, 0x102, 40);
    Actor_FaceDirection(ACTOR_PARTY_LEADER, 0x3000, 0);
    Iriguchi_Wait(20);
    Event_ShowMessage(ACTOR_MIA, 0);
    Iriguchi_Wait(10);
    Actor_SetAnimationAndWait(ACTOR_GERALD, 4);
    Iriguchi_Wait(20);
    Event_ShowMessage(ACTOR_GERALD, 0);
    Iriguchi_Wait(20);
    Actor_ShowEmote(14, 0x103, 50);
    Actor_FaceDirection(14, 0x8000, 0);
    Iriguchi_Wait(20);
    Event_OpenMessage(14, 0);
    Actor_FaceDirection(ACTOR_PARTY_LEADER, 0, 0);
    Actor_FaceDirection(11, 0x8000, 0);
    Actor_FaceDirection(12, 0x8000, 0);
    Actor_FaceDirection(13, 0x8000, 0);
    Actor_FaceDirection(ACTOR_IVAN, 0x1000, 0);
    Actor_FaceDirection(ACTOR_MIA, 0xe000, 0);
    if (Event_ChooseYesNo(0, 0) == 0) {
        Iriguchi_Wait(30);
        Actor_RunRepeatedMotion(14, 2);
        Iriguchi_Wait(20);
        Event_ShowMessage(14, 0);
        bump_step_02001238(1);
    } else {
        Iriguchi_Wait(30);
        Actor_RunRepeatedMotion(14, 2);
        Iriguchi_Wait(20);
        bump_step_02001238(1);
        Event_ShowMessage(14, 0);
    }
    Iriguchi_Wait(10);
    Actor_RunRepeatedMotion(10, 2);
    Iriguchi_Wait(20);
    Actor_WalkByAndWait(10, 0, 16);
    Actor_FaceDirection(10, 0x2000, 0);
    Iriguchi_Wait(20);
    Event_ShowMessage(10, 0);
    Iriguchi_Wait(10);
    Actor_FaceDirection(14, 0xa000, 0);
    Iriguchi_Wait(20);
    Actor_SetAnimationAndWait(14, 4);
    Iriguchi_Wait(20);
    Event_ShowMessage(14, 0);
    Iriguchi_Wait(10);
    Actor_SetAnimationAndWait(10, 3);
    Iriguchi_Wait(30);
    Event_ShowMessage(10, 0);
    Iriguchi_Wait(10);
    Actor_RunRepeatedMotion(14, 2);
    Iriguchi_Wait(20);
    Event_OpenMessage(14, 0);
    Iriguchi_Wait(40);
    Actor_FaceDirection(10, 0x5000, 0);
    Iriguchi_Wait(20);
    Actor_ShowEmote(10, 0x101, 60);
    if (Event_ChooseYesNo(0, 0) == 0) {
        Iriguchi_Wait(30);
        Actor_FaceDirection(10, 0x2000, 0);
        Iriguchi_Wait(20);
        Actor_SetAnimationAndWait(10, 3);
        Iriguchi_Wait(30);
        Event_ShowMessage(10, 0);
        bump_step_02001238(1);
    } else {
        Iriguchi_Wait(30);
        Actor_FaceDirection(10, 0x2000, 0);
        Iriguchi_Wait(20);
        Actor_SetAnimationAndWait(10, 4);
        Iriguchi_Wait(20);
        bump_step_02001238(1);
        Event_ShowMessage(10, 0);
    }
    Iriguchi_Wait(10);
    Actor_ShowEmote(14, 0x102, 50);
    Event_ShowMessage(14, 0);
    Iriguchi_Wait(15);
    Actor_SetAnimationAndWait(10, 3);
    Iriguchi_Wait(30);
    Iriguchi_Wait(20);
    Actor_RunRepeatedMotion(14, 2);
    Iriguchi_Wait(40);
    Actor_FaceDirection(14, 0x8000, 0);
    Iriguchi_Wait(20);
    Event_ShowMessage(14, 0);
    Iriguchi_Wait(10);
    Actor_SetAnimationAndWait(14, 3);
    Iriguchi_Wait(30);
    Event_ShowMessage(14, 0);
    Iriguchi_Wait(10);
    Actor_FaceDirection(10, 0x5000, 0);
    Iriguchi_Wait(25);
    Actor_RunRepeatedMotion(10, 2);
    Iriguchi_Wait(20);
    Actor_WalkToAndWait(10, 0x138, 0x138);
    Actor_FaceDirection(ACTOR_PARTY_LEADER, 0, 0);
    Actor_FaceDirection(10, 0x8000, 0);
    Iriguchi_Wait(25);
    Event_ShowMessage(10, 0);
    Iriguchi_Wait(10);
    Actor_FaceEachOther(ACTOR_PARTY_LEADER, ACTOR_GERALD, 0);
    Actor_FaceEachOther(ACTOR_MIA, ACTOR_IVAN, 0);
    Iriguchi_Wait(30);
    Object_SetModeById(ACTOR_PARTY_LEADER, 3);
    Object_SetModeById(ACTOR_GERALD, 3);
    Object_SetModeById(ACTOR_MIA, 3);
    Actor_SetAnimationAndWait(ACTOR_IVAN, 3);
    Iriguchi_Wait(30);
    ObjectMotion_SetSpeedParameters(ACTOR_GERALD, 0x13333, 0x9999);
    ObjectMotion_SetSpeedParameters(ACTOR_IVAN, 0x13333, 0x9999);
    ObjectMotion_SetSpeedParameters(ACTOR_MIA, 0x13333, 0x9999);
    Object_SetModeById(ACTOR_GERALD, 2);
    record = Object_GetById(ACTOR_PARTY_LEADER);
    if (record != 0) {
        Actor_SetDestination(ACTOR_GERALD, *(s16 *)(record + 10), *(s16 *)(record + 18));
    }
    ObjectMotion_CommitCurrentPositionAndActivate(ACTOR_GERALD);
    Actor_SetPosition(ACTOR_GERALD, 0, 0);
    Object_SetModeById(ACTOR_IVAN, 2);
    record = Object_GetById(ACTOR_PARTY_LEADER);
    if (record != 0) {
        Actor_SetDestination(ACTOR_IVAN, *(s16 *)(record + 10), *(s16 *)(record + 18));
    }
    ObjectMotion_CommitCurrentPositionAndActivate(ACTOR_IVAN);
    Actor_SetPosition(ACTOR_IVAN, 0, 0);
    Object_SetModeById(ACTOR_MIA, 2);
    record = Object_GetById(ACTOR_PARTY_LEADER);
    if (record != 0) {
        Actor_SetDestination(ACTOR_MIA, *(s16 *)(record + 10), *(s16 *)(record + 18));
    }
    ObjectMotion_CommitCurrentPositionAndActivate(ACTOR_MIA);
    Actor_SetPosition(ACTOR_MIA, 0, 0);
    Iriguchi_Wait(10);
    Actor_FaceDirection(ACTOR_PARTY_LEADER, 0xc000, 0);
    Actor_FaceDirection(10, 0xb000, 0);
    Actor_FaceDirection(14, 0xb000, 0);
    Actor_FaceDirection(11, 0xb000, 0);
    Actor_FaceDirection(12, 0xb000, 0);
    Actor_FaceDirection(13, 0xb000, 0);
    Iriguchi_Wait(30);
    Event_End();
}

void ActorPresentation_SetSceneCellByFlag985(void)
{
    if (GameFlag_IsSet(0x985) == 0) {
        s32 k5 = 17, k6 = 78;

        Engine_MapCopyCellsTo(36, 78, 1, 2, k5, k6);
    } else {
        s32 k5 = 17, k6 = 78;

        Engine_MapCopyCellsTo(34, 78, 1, 2, k5, k6);
    }
}

void SceneState_ApplyRectAt32x78(void)
{
    {
        s32 k5 = 17, k6 = 78;

        Engine_MapCopyCellsTo(32, 78, 1, 2, k5, k6);
    }
}

/* The door of truth: actors 8 and 9 are its two leaves. Flag 0x985 records
   it open; a switch opens it for one who sees with a true heart. */

/* Slide the leaves apart and open the passage; the first time, the scene
   that follows plays. */
void BabiIriguchi_OpenTruthDoor(void)
{
    if (GameFlag_IsSet(0x985) == 0) {
        GameFlag_Set(0x985);
        Audio_PlayCue(157);
        Event_Begin();
        Actor_SetDestination(8, 0x118, 240);
        Actor_SetDestination(9, 0x148, 240);
        ObjectMotion_CommitCurrentPositionAndActivate(8);
        ObjectMotion_CommitCurrentPositionAndActivate(9);
        Iriguchi_CopyCellAttributes(81, 14, 4, 1, 17, 14);
        Event_End();
        if (GameFlag_IsSet(0x989) == 0) {
            FieldScene_RunActorEventSequence();
        }
    }
}

/* Slide the leaves together, close the passage and toggle flag 0x301. */
void BabiIriguchi_CloseTruthDoor(void)
{
    if (GameFlag_IsSet(0x985) != 0) {
        GameFlag_Clear(0x985);
        Audio_PlayCue(157);
        Event_Begin();
        Actor_SetDestination(8, 0x128, 240);
        Actor_SetDestination(9, 0x138, 240);
        ObjectMotion_CommitCurrentPositionAndActivate(8);
        ObjectMotion_CommitCurrentPositionAndActivate(9);
        Iriguchi_CopyCellAttributes(0, 14, 4, 1, 17, 14);
        Event_End();
        if (GameFlag_IsSet(0x301) != 0) {
            GameFlag_Clear(0x301);
        } else {
            GameFlag_Set(0x301);
        }
    }
}

/* The door's switch: while the word at +0xcb8 of the event work is set the
   leader flips it and the door opens; otherwise the door's words show. */
void BabiIriguchi_FlipTruthDoorSwitch(void)
{
    u8 *work = (u8 *)gEventWork;
    s16 *seeing;

    Event_Begin();
    seeing = (s16 *)(work + 0xcb8);
    if (seeing[0] != 0) {
        if (GameFlag_IsSet(0x985) == 0) {
            /* FAKEMATCH: forced temporaries; the destination stays in two
               saved registers across both copies, where plain constants
               are built again for each call. */
            s32 dest_x = 17, dest_y = 78;

            Message_ShowCentered((s32)MsgFieldFlippedSwitch, 1);
            Audio_PlayCue(155);
            Engine_MapCopyCellsTo(35, 78, 1, 2, dest_x, dest_y);
            Iriguchi_Wait(10);
            Engine_MapCopyCellsTo(34, 78, 1, 2, dest_x, dest_y);
            Iriguchi_Wait(10);
            BabiIriguchi_OpenTruthDoor();
        }
    } else {
        Event_SetMessage((s32)MsgBabiTruthDoorOpenThoseSeeing);
        Event_ShowMessage(-1, 0);
    }
    Event_End();
}

/* The terrain lookup consumes both computed coordinates, not just the mode.
 * Omitting x/z left their live argument registers unexplained in the draft.
 * Exact complete 72-byte owner, including both final pool words. */
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

void SceneState_SetRuntimeByte34(void)
{
    FIELD_AT_OFFSET(*(void **)gEffectWork, s8 *, 0x34) = 1;
}

void ActorPresentation_PlaceActorTwelveAtTile20And12(void)
{
    s32 *p = Actor_Get(12);
    s32 a = p[2] >> 20;

    if (a == 20) {
        s32 b = p[4] >> 20;

        if (b == 12) {
            ((u8 *)p)[85] = 2;
            p[5] = 0x300000;
            ((u8 *)p)[35] = 2;
            {
                s32 k5 = a, k6 = b;

                Iriguchi_CopyCellAttributes(38, 12, 1, 1, k5, k6);
            }
        }
    }
}

void SceneActor_PushObjectAheadIfLevel(void)
{
    struct StagedActor *p = (struct StagedActor *)Actor_Get(ACTOR_PARTY_LEADER);
    struct StagedActor *q = BabiIriguchi_FindActorAhead(p);
    s32 diff;

    if (q == 0) {
        return;
    }

    diff = q->y - p->y;

    if (diff >= 0) {
        /* Written with an empty arm on purpose: the reference branches away on
         * the *return* condition (`bge`), and spelling this as a plain
         * `if (diff >= 0x80000) return;` inverts it to `blt`. Arm order
         * decides the branch sense; no flag moves it. */
        if (diff < 0x80000) {
        } else {
            return;
        }
    } else if (p->y - q->y >= 0x80000) {
        return;
    }

    StagedActor_AdvancePair();
}

/* The facing check reads the game state as rows of bytes, not through the
 * field event header's structure. */

/* With the party leader facing north or south and either the byte at 498 of
 * the game state set or no actor ahead, runs the grid placement for that
 * facing; unless that placement reports zero, pushes the object ahead when
 * the byte is clear. */
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

/* What each of the entrance's scenes answers. */
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
    /* The two stack arguments each need their own local: the reference builds
     * both into separate registers before storing either, and a literal pair
     * lets the compiler reuse one register for both. */
    s32 a = 18;
    s32 b = 7;

    Iriguchi_CopyCellAttributes(82, 7, 1, 2, a, b);
    Iriguchi_TaskWait(1);
    GameFlag_Set(768);
}

void SceneState_ApplyRectsAtActors8And9(void)
{
    s32 *p = Actor_Get(8);

    Actor_SetSpritePriority(8, 1);
    Actor_SetSpritePriority(9, 1);
    {
        s32 k5 = 5, k6 = 19;

        Iriguchi_CopyCellAttributes(69, 19, 3, 3, k5, k6);
    }
    {
        s32 k5 = 17, k6 = 19;

        Iriguchi_CopyCellAttributes(69, 19, 3, 3, k5, k6);
    }
    {
        s32 k5 = p[2] >> 20, k6 = p[4] >> 20;

        Iriguchi_CopyCellAttributes(3, 3, 1, 1, k5, k6);
    }
    {
        s32 *q = Actor_Get(9);
        s32 k5 = q[2] >> 20, k6 = q[4] >> 20;

        Iriguchi_CopyCellAttributes(3, 3, 1, 1, k5, k6);
    }
}
