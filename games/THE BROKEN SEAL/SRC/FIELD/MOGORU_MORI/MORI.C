#include "MAPCOPY.H"
#include "EDITION.H"
#include "MORI.H"
#include "TYPES.H"
#include "FIELD_EVENT.H"
#include "FIELD_SCENE.H"
#include "CALL.H"
#include "STAGED_ACTOR.H"
#include "SCENE_IDS.H"
#include "text/MSG_IDS.H"

TEXT_MESSAGE_ENUM(MsgMogoruMoriBrokenSign);

s32 Engine_MathCos(s32 angle);
s32 Engine_MathSin(s32 angle);

/* The IWRAM divide, reached through this overlay's import veneer. */

struct Vec {
    s32 x;
    s32 y;
    s32 z;
};

extern const struct SceneEntrance gMogoruMoriEntrances1[];
extern const struct SceneEntrance gMogoruMoriEntrances2[];
extern const struct SceneEntrance gMogoruMoriEntrances3[];
extern const struct SceneEntrance gMogoruMoriEntrancesOther[];
extern const struct ScenePlacement gMogoruMoriPlacements1[];
extern const struct ScenePlacement gMogoruMoriPlacements2[];
extern const struct ScenePlacement gMogoruMoriPlacements3[];
extern const struct ScenePlacement gMogoruMoriPlacementsOther[];

void Engine_EventBegin(void);
void Object_SetModeById(s32 actor, s32 animation);
void ObjectMotion_SetSpeedParameters(s32 actor, s32 speed, s32 acceleration);
void ObjectMotion_OffsetPositionAndResetMotion(s32 actor, s32 dx, s32 dz);
void Battle_WaitMode0(s32 frames);
void Audio_PlayCue(s32 cue);
void Engine_ActorSetSpritePriority(s32 actor, s32 value);
void Engine_EventEnd(void);

void Engine_EventBegin();
void FieldScene_RunSixCallSetupSequence();
void FieldScene_RunScene39f_02000d90();
void Battle_WaitMode0();
void Effect_Spawn();
void Engine_CameraFollowActor();
void Engine_ActorFaceEachOther();
void Engine_ActorStartRepeatedMotion();
void Engine_ActorSetAttachedEffect();
void Engine_ActorFaceActor();
void Engine_ActorSetPosition();
void Engine_EventEnd();

void Engine_ActorSetChildValue();
void Engine_ActorSetSpriteFlags();
void Engine_CameraSetSpeed();
void Engine_CameraMoveTo();
void Engine_CameraWaitForMove();
void Engine_WorkSetValuesIfNonNegative();
void MogoruMori_SpawnPuffRing();
void Engine_MapWaitWorkValuesBelow256();
void Engine_ActorFaceDirection();
s32 Engine_MathCos();
s32 Engine_MathSin();
void Audio_PlayCue();
void Engine_ActorRunRepeatedMotion();
void Party_SetFields1ceAnd1d0();
void BattleFx_SetWeightedResult();
extern struct GameState gGameState;

extern const struct SceneEvent gMogoruMoriEvents1[];
extern const struct SceneEvent gMogoruMoriEvents2[];
extern const struct SceneEvent gMogoruMoriEvents3[];
extern const struct SceneEvent gMogoruMoriEventsOther[];
extern struct EventWork *gEventWork;
s32 Engine_GameFlagIsSet();
void WaitFrames();
void ObjectMotion_SetSpeedParameters();
void Engine_ActorWalkTo();
void Engine_EventOpenScreen();
void Engine_EventWaitForScreen();
void ObjectMotion_CommitCurrentPositionAndActivate();
void Engine_ActorShowEmote();
void InitializeOrbitingEffect();
void Object_SetModeById();
s32 StagedActor_FillGridAttributeRectangle(u32, s32, s32, u32, u32, s32);
u8 *OverlayObject_SpawnConfiguredWithMode15(s32, s32, s32, s32);
void FieldScene_RunSupplementalSequenceOne();
void Engine_ActorEnableActionCallback();
extern u8 MogoruMori_ActorScript[];

/* Five sites of one import. */
void SceneState_SetValue18Mode2(void)
{
    BattleFx_SetPhaseRequest(18, 2);
}

/* Mogall Forest: when the leader can step onto the next slot, play the hop
 * and move the leader there. */
/* 0x02004918 serves two imports: the two-argument mode select and the
 * zero-argument bracket close. */
s32 SceneActor_TryRunSlotZeroMoveStep(s16 *arg)
{
    s32 *p = Object_GetById(ACTOR_PARTY_LEADER);
    u8 *f = (u8 *)p + 0x55;
    s32 saved = *f;

    s32 r = Object_CheckMovementCollision(p, arg);

    if (r == 0) {
        s32 m;

        Engine_EventBegin();
        Object_SetMode(p, 6);
        WaitFrames(6);
        Audio_PlayCue(152);
        Object_SetMode(p, 7);
        p[12] = 0x30000;
        p[13] = 0x20000;
        p[10] = 0x40000;
        m = 0x7e;
        m &= *f;
        *f = m;
        Engine_ActorSetSpriteFlags(p, 0);
        Engine_ActorMoveToAndWait(ACTOR_PARTY_LEADER, arg[1], arg[5]);
        Object_SetMode(p, 6);
        Engine_ActorSetSpriteFlags(p, 1);
        *f = saved;
        Engine_EventEnd();
        return 1;
    }
    return 0;
}

s32 OverlayObject_ApplyField100(s32 a)
{
    ObjectGroup_SetChildValue(a, *(s16 *)(a + 100));
    return 0;
}

s32 OverlayObject_ApplyZero(s32 a)
{
    Engine_ActorSetSpriteFlags(a, 0);
    return 0;
}

/* Mogall Forest: slide an actor to a point with the swish of cue 152,
 * lifting it by the given height while it moves. */
void FieldScene_RunScene39f_02000d90(s32 a0, s32 a1, s32 a2, s32 a3)
{
    s32 rec7;

    rec7 = (s32)Object_GetById(a0);
    Engine_ActorSetSpritePriority(a0, 1);
    ObjectMotion_SetSpeedParameters(a0, 0x30000, 0x18000);
    Audio_PlayCue(152);
    *(s32 *)(rec7 + 40) = a3;
    *(s32 *)(rec7 + 72) = 0x8000;
    *(s32 *)(rec7 + 68) = 0;
    Engine_ActorSetSpriteFlags(rec7, 0);
    Engine_ActorMoveToAndWait(a0, a1, a2);
    Engine_ActorSetPosition(a0, a1 << 16, a2 << 16);
    Engine_ActorSetSpriteFlags(rec7, 1);
    *(s32 *)(rec7 + 72) = 0x10000;
}

/* A ring of seventeen puffs bursts out around the actor. */
void MogoruMori_SpawnPuffRing(s32 id)
{
    struct Vec dir;
    struct EffectOptions params;
    struct EffectOptions *p;
    struct Vec *v;
    struct FieldActor *actor;
    u32 i;
    s32 x;
    s32 z;

    actor = Object_GetById(id);
    Audio_PlayCue(188);
    p = &params;
    p->priority = 1;
    for (i = 0; i <= 16; i++) {
        v = &dir;
        v->x = Engine_MathCos(i << 12);
        v->y = 0;
        z = Engine_MathSin(i << 12);
        x = v->x;
        v->z = z;
        x += x / 3;
        v->x = x;
        Effect_Spawn(actor->x.fixed, 0x100000, actor->z.fixed, x, v->y + 0x1999, z, 0x20000, p);
    }
}

void FieldScene_RunSixCallSetupSequence(s32 no, s32 val)
{
    s32 v0 = 0x20000;
    s32 v1 = 0x4000;

    Engine_CameraSetSpeed(v0, v1);
    Engine_CameraMoveToActor(no, 1);
    Engine_CameraWaitForMove();
    Battle_WaitMode0(30);
    MogoruMori_SpawnPuffRing(no);
    Engine_ActorSetChildValue(no, val);
}

/* Where the party appears in each of the forest's three areas. */
const struct SceneEntrance *Scene_GetEntrances(void)
{
    s16 selector = gGameState.scene;

    if (selector == (s32)&SceneId_MogoruMori1) {
        return gMogoruMoriEntrances1;
    }
    if (selector == (s32)&SceneId_MogoruMori2) {
        return gMogoruMoriEntrances2;
    }
    if (selector == (s32)&SceneId_MogoruMori3) {
        return gMogoruMoriEntrances3;
    }
    return gMogoruMoriEntrancesOther;
}

/* Complete four-byte leaf: movs r0,#0 followed by bx lr. */
s32 SceneData_ReturnZero(void)
{
    return 0;
}

/* Complete eight-byte literal-address getter, including its sole pool word. */
u8 *SceneData_GetTableB5bc(void)
{
    return MogoruMori_SceneTable;
}

/* The actors placed in each area. */
const struct ScenePlacement *Scene_GetPlacements(void)
{
    s16 selector = gGameState.scene;

    if (selector == (s32)&SceneId_MogoruMori1) {
        return gMogoruMoriPlacements1;
    }
    if (selector == (s32)&SceneId_MogoruMori2) {
        return gMogoruMoriPlacements2;
    }
    if (selector == (s32)&SceneId_MogoruMori3) {
        return gMogoruMoriPlacements3;
    }
    return gMogoruMoriPlacementsOther;
}

/* Mogoru Forest: when the probe finds actor 9 in map column 26, set flag
 * 0x310, step actor 9 back, play cue 240 and copy the cleared cells. */
void MogoruMori_RunProbedActorNineScene(void)
{
    struct StagedActorProbe probe;
    s32 fifth;
    s32 sixth;

    Engine_EventBegin();
    if (StagedActor_FindClearPosition(&probe) != 0) {
        SceneActor_MoveAndRedraw(probe);
        if (probe.actor_slot == 9 && (probe.position_z >> 20) == 26) {
            Engine_GameFlagSet(0x310);
            Object_SetModeById(9, 3);
            Call3(ObjectMotion_SetSpeedParameters, 9, 0x4000, 0x8000);
            ObjectMotion_OffsetPositionAndResetMotion(9, 0, -16);
            Battle_WaitMode0(45);
            Object_SetModeById(9, 8);
            Audio_PlayCue(240);
            Engine_ActorSetSpritePriority(9, 1);
            Object_GetById(9)->priority_flags = 2;
            fifth = 31;
            sixth = 25;
            Map_CopyCellAttributeRect(38, 27, 4, 2, fifth, sixth);
        }
    }
    Engine_EventEnd();
}

/* The five tile-painting calls take (layer, x, z, width, height, value) and
 * all reach the same routine, but each keeps its own call word: the encoding
 * is per site, so they must not be collapsed onto one alias. */
void SceneActor_PassOffsetPointOfActorZero(void)
{
    s32 v[3];
#if EDITION_INTERNATIONAL
    s32 *p = Object_GetById(ACTOR_PARTY_LEADER);

    v[0] = (p[2] & 0xfff00000) + 0x80000;
    v[1] = p[3];
    v[2] = (p[4] & 0xfff00000) + 0xffe80000;
#else
    v[0] = ((struct FieldActor *)Object_GetById(ACTOR_PARTY_LEADER))->x.fixed;
    v[1] = ((struct FieldActor *)Object_GetById(ACTOR_PARTY_LEADER))->y.fixed;
    v[2] = ((struct FieldActor *)Object_GetById(ACTOR_PARTY_LEADER))->z.fixed - 0x200000;
#endif
    SceneActor_TryRunSlotZeroMoveStep(v);
}

void SceneActor_BobActorZeroWhenAheadClear(void)
{
    s32 pos[3];
    s32 *actor = Object_GetById(ACTOR_PARTY_LEADER);
    u8 *fp = (u8 *)actor + 0x55;
    s32 saved = *fp;

#if EDITION_INTERNATIONAL
    pos[0] = (actor[2] & 0xfff00000) + 0x80000;
    pos[1] = actor[3];
    pos[2] = (actor[4] & 0xfff00000) + 0x280000;
#else
    pos[0] = ((struct FieldActor *)Object_GetById(ACTOR_PARTY_LEADER))->x.fixed;
    pos[1] = ((struct FieldActor *)Object_GetById(ACTOR_PARTY_LEADER))->y.fixed;
    pos[2] = ((struct FieldActor *)Object_GetById(ACTOR_PARTY_LEADER))->z.fixed + 0x200000;
#endif
    if (SceneActor_TryRunSlotZeroMoveStep(pos)!= 0) {
        Engine_EventBegin();
        *fp = 0;
        Object_SetModeById(9, 7);
        actor[3] += -0x10000;
        actor[5] += -0x10000;
        WaitFrames(2);
        actor[3] += -0x10000;
        actor[5] += -0x10000;
        WaitFrames(10);
        actor[3] += 0x10000;
        actor[5] += 0x10000;
        WaitFrames(4);
        actor[3] += 0x10000;
        actor[5] += 0x10000;
        *fp = saved;
        Engine_EventEnd();
    }
}

void FieldScene_RunScriptedSteps0And17E6(void)
{
    Engine_EventBegin();
    Object_SetModeById(ACTOR_PARTY_LEADER, 1);
    Engine_MessageShowCentered(MsgMogoruMoriBrokenSign, 1);
    Engine_EventEnd();
}

void FieldScene_RunActor10WaypointSequence(void)
{
    u8 *slot;

    slot = Object_GetById(10);

    /* r0 still holds the record returned above. */
    Engine_EventBegin();

    FieldScene_RunSixCallSetupSequence(10, 1);
    FieldScene_RunScene39f_02000d90(10, 88, 120, 0x60000);        /* 192 << 11 */

    Effect_Spawn(*(s32 *)(slot + 8), *(s32 *)(slot + 12),
                  *(s32 *)(slot + 16) + 0x180000,   /* 192 << 13 */
                  0, 0, 0, 1, 0);

    Engine_CameraFollowActor(10, 1);
    Engine_ActorFaceEachOther(10, ACTOR_PARTY_LEADER, 0);
    Battle_WaitMode0(20);
    Engine_ActorStartRepeatedMotion(10, 2);
    Engine_ActorSetAttachedEffect(10, 258);                     /* 129 << 1 */
    Battle_WaitMode0(60);

    /* Three waypoints, each at height 0x30000 (192 << 10). */
    FieldScene_RunScene39f_02000d90(10, 88, 152, 0x30000);
    Engine_ActorFaceActor(ACTOR_PARTY_LEADER, 10, 0);
    Battle_WaitMode0(10);

    FieldScene_RunScene39f_02000d90(10, 120, 192, 0x30000);
    Engine_ActorFaceActor(ACTOR_PARTY_LEADER, 10, 0);
    Battle_WaitMode0(10);

    FieldScene_RunScene39f_02000d90(10, 120, 240, 0x30000);
    Engine_ActorFaceActor(ACTOR_PARTY_LEADER, 10, 0);
    Battle_WaitMode0(10);

    Engine_GameFlagSet(768);                       /* 192 << 2 */
    Engine_ActorSetPosition(13, 0, 0);
    Engine_ActorSetPosition(10, 0, 0);

    /* Common exit; no argument registers are set. */
    Engine_EventEnd();
}

/* Mogall Forest: actor 11 hops out of the trees, greets the party and
 * hops back to the leader. */
/*
 * A full cutscene beat for slot 11: opens the slot, places it at (408, 456),
 * publishes an eight-argument piece, runs the presentation, then re-places the
 * slot on the party's current heading readings and sets the engine byte at
 * gGameState + 0x22b to 3.  The 228-byte owner includes an alignment
 * halfword and its four pool words.
 */
void FieldScene_RunActorElevenPresentationBeat(void)
{
    u8 *slot;
    s32 offset;

    slot = Object_GetById(11);

    /* Reads the record left in r0 by the call above; it must not be respelled
     * as a fresh fetch. */
    Engine_EventBegin();

    FieldScene_RunSixCallSetupSequence(11, 0);
    FieldScene_RunScene39f_02000d90(11, 408, 456, 0x60000);   /* 204 << 1, 228 << 1, 192 << 11 */

    Effect_Spawn(*(s32 *)(slot + 8), *(s32 *)(slot + 12),
                  *(s32 *)(slot + 16) + 0x180000,   /* 192 << 13 */
                  0, 0, 0, 1, 0);

    Engine_CameraFollowActor(11, 1);
    Engine_ActorFaceEachOther(11, ACTOR_PARTY_LEADER, 0);
    Battle_WaitMode0(30);
    Engine_ActorStartRepeatedMotion(11, 2);
    Engine_ActorShowEmote(11, 0x103, 0);
    Audio_PlayCue(147);
    Battle_WaitMode0(60);

    /* Two signed halfwords of slot 0, each read after its own fetch of the
     * record. */
    FieldScene_RunScene39f_02000d90(11,
                  *(s16 *)((u8 *)Object_GetById(0) + 10),
                  *(s16 *)((u8 *)Object_GetById(0) + 18),
                  0x40000);                          /* 128 << 11 */

    Battle_WaitMode0(10);
    Engine_GameFlagSet(0x301);
    Engine_ActorSetPosition(14, 0, 0);

    /* FAKEMATCH: the byte offset held in a forced temporary keeps the
     * game-state address and the offset apart. */
    offset = 0x22b;
    ((u8 *)&gGameState)[offset] = 3;

    BattleFx_SetWeightedResult(53, 0);

    /* Common exit; no argument registers are set. */
    Engine_EventEnd();
}

void SceneActor_RunActorTwelveThreeWaypointMotion(void)
{
    u8 *slot;

    slot = Object_GetById(12);

    /* r0 still holds the record returned above. */
    Engine_EventBegin();

    FieldScene_RunSixCallSetupSequence(12, 1);
    FieldScene_RunScene39f_02000d90(12, 536, 344, 0x70000);       /* 134 << 2, 172 << 1, 224 << 11 */

    Effect_Spawn(*(s32 *)(slot + 8), *(s32 *)(slot + 12),
                  *(s32 *)(slot + 16) + 0x100000,   /* 128 << 13 */
                  0, 0, 0, 1, 0);

    Engine_CameraFollowActor(12, 1);
    Engine_ActorFaceEachOther(12, ACTOR_PARTY_LEADER, 0);
    Battle_WaitMode0(20);
    Engine_ActorStartRepeatedMotion(12, 2);
    Engine_ActorSetAttachedEffect(12, 258);                     /* 129 << 1 */
    Battle_WaitMode0(60);

    /* Three waypoints, each at height 0x30000 (192 << 10); the X literals are
     * 146 << 2, 158 << 2 and 170 << 2 and the Z is the same 172 << 1. */
    FieldScene_RunScene39f_02000d90(12, 584, 344, 0x30000);
    Engine_ActorFaceActor(ACTOR_PARTY_LEADER, 12, 0);
    Battle_WaitMode0(6);

    FieldScene_RunScene39f_02000d90(12, 632, 344, 0x30000);
    Engine_ActorFaceActor(ACTOR_PARTY_LEADER, 12, 0);
    Battle_WaitMode0(6);

    FieldScene_RunScene39f_02000d90(12, 680, 344, 0x30000);
    Engine_ActorFaceActor(ACTOR_PARTY_LEADER, 12, 0);
    Battle_WaitMode0(6);

    Engine_GameFlagSet(0x302);
    Engine_ActorSetPosition(15, 0, 0);

    /* Common exit; no argument registers are set. */
    Engine_EventEnd();
}

void FieldScene_RunStepFD4WithActor181(s32 a)
{
    Engine_EventBegin();
    Engine_ActorSetPosition(16, 0, 0);
    Engine_GameFlagSet(4052);
    Engine_ItemShowFound(ITEM_NUT, 3);
    Engine_PartyGiveItem(ITEM_NUT, 0);
    Engine_EventEnd();
}

/* Mogall Forest: after the probe moves actor 8 or actor 10, copy the cells
 * it opened; actor 10 at column 35 settles and sets flag 0x311. */
void FieldScene_RunProbedActorEightOrTenScene(void)
{
    struct StagedActorProbe probe;
    s32 fifth;
    s32 sixth;
    s32 height;
    s32 value;

    /* No argument register is written before this branch. */
    Engine_EventBegin();

    if (StagedActor_FindClearPosition(&probe) != 0) {
        SceneActor_MoveAndRedraw(probe);

        if (probe.actor_slot == 8 && (probe.position_z >> 20) == 23) {
            fifth = 35;
            sixth = 68;
            Map_CopyCellAttributeRect(35, 67, 4, 1, fifth, sixth);
        } else if (probe.actor_slot == 10 && (probe.position_x >> 20) == 35) {
            /* Written here, not at the call: the reference keeps it in a
             * callee-saved register across the whole sequence. */
            value = 0;
            Engine_GameFlagSet(0x311);
            Object_SetModeById(10, 3);
            ObjectMotion_OffsetPositionAndResetMotion(10, -16, 6);
            Battle_WaitMode0(30);
            Object_SetModeById(10, 8);
            Audio_PlayCue(240);

            Object_GetById(10)->priority_flags = 2;

            fifth = 34;
            sixth = 30;
            Map_CopyCellAttributeRect(44, 30, 2, 4, fifth, sixth);
            height = 4;
            StagedActor_FillGridAttributeRectangle(2, 35, 30, 1, height, value);
        }
    }

    /* Common exit; no argument registers are set. */
    Engine_EventEnd();
}

void SceneState_ApplyCrossRectsAroundActor11(void)
{
    s32 x;
    s32 z;

    /* No argument register is written before this branch. */
    Engine_EventBegin();

    /* Both coordinates are 16.16 fixed point reduced to whole tiles with
     * `asrs #20`, i.e. 16 fractional bits plus a 16-unit tile pitch. */
    x = ((s32 *)Object_GetById(11))[2] >> 20;
    z = ((s32 *)Object_GetById(11))[4] >> 20;

    StagedActor_FillGridAttributeRectangle(2, x, z, 1, 1, 0xff);
    StagedActor_FillGridAttributeRectangle(2, x + 1, z, 1, 1, 0);
    StagedActor_FillGridAttributeRectangle(2, x - 1, z, 1, 1, 0);
    StagedActor_FillGridAttributeRectangle(2, x, z + 1, 1, 1, 0);
    StagedActor_FillGridAttributeRectangle(2, x, z - 1, 1, 1, 0);

    if (x == 36 && z == 24) {
        u8 *p = (u8 *)Object_GetById(11);

        p[85] = 0;
        *(s32 *)(p + 20) = (s32)0xfffe0000;
        *(s32 *)(p + 12) = (s32)0xfffe0000;
    }

    /* Common exit; no argument registers are set. */
    Engine_EventEnd();
}

/* Mogoru Forest: actor 12 hops up on the branch in four steps, with the
 * leader watching each one, then leaves; flag 0x303 records it. */
void MogoruMori_RunBranchHopScene(void)
{
    s32 rec7;

    rec7 = (s32)Object_GetById(12);
    Engine_EventBegin();
    FieldScene_RunSixCallSetupSequence(12, 1);
    FieldScene_RunScene39f_02000d90(12, 0x188, 104, 0x70000);
    Battle_WaitMode0(10);
    Effect_Spawn(*(s32 *)(rec7 + 8), *(s32 *)(rec7 + 12), (*(s32 *)(rec7 + 16) + 0x40000), 0, 0, 0, 1, 0);
    Engine_CameraFollowActor(12, 1);
    Engine_ActorFaceEachOther(12, 0, 0);
    Battle_WaitMode0(20);
    Engine_ActorStartRepeatedMotion(12, 2);
    Engine_ActorSetAttachedEffect(12, 0x102);
    Battle_WaitMode0(60);
    FieldScene_RunScene39f_02000d90(12, 0x1a8, 120, 0x30000);
    Engine_ActorFaceActor(0, 12, 0);
    Battle_WaitMode0(6);
    FieldScene_RunScene39f_02000d90(12, 0x1a8, 168, 0x30000);
    Engine_ActorFaceActor(0, 12, 0);
    Battle_WaitMode0(6);
    FieldScene_RunScene39f_02000d90(12, 0x1a8, 208, 0x30000);
    Engine_ActorFaceActor(0, 12, 0);
    Battle_WaitMode0(6);
    FieldScene_RunScene39f_02000d90(12, 0x1a8, 232, 0x30000);
    Engine_ActorFaceActor(0, 12, 0);
    Battle_WaitMode0(6);
    Engine_ActorSetPosition(12, 0, 0);
    Engine_GameFlagSet(0x303);
    Engine_ActorSetPosition(15, 0, 0);
    Engine_EventEnd();
}

void FieldScene_RunActorThirteenPresentationBeat(void)
{
    u8 *slot;

    slot = Object_GetById(13);

    /* r0 still holds the record returned above. */
    Engine_EventBegin();

    FieldScene_RunSixCallSetupSequence(13, 1);
    FieldScene_RunScene39f_02000d90(13, 456, 104, 0x70000);       /* 228 << 1, 224 << 11 */
    Battle_WaitMode0(10);

    Effect_Spawn(*(s32 *)(slot + 8), *(s32 *)(slot + 12),
                  *(s32 *)(slot + 16) + 0x40000,    /* 128 << 11 */
                  0, 0, 0, 1, 0);

    Engine_CameraFollowActor(13, 1);
    Engine_ActorFaceEachOther(13, ACTOR_PARTY_LEADER, 0);
    Battle_WaitMode0(20);
    Engine_ActorStartRepeatedMotion(13, 2);
    Engine_ActorSetAttachedEffect(13, 258);                     /* 129 << 1 */
    Battle_WaitMode0(60);

    FieldScene_RunScene39f_02000d90(13, 472, 136, 0x30000);       /* 236 << 1, 192 << 10 */
    Engine_ActorFaceActor(ACTOR_PARTY_LEADER, 13, 0);
    Battle_WaitMode0(6);

    FieldScene_RunScene39f_02000d90(13, 504, 136, 0x33333);       /* 252 << 1, pooled height */
    Engine_ActorFaceActor(ACTOR_PARTY_LEADER, 13, 0);
    Battle_WaitMode0(6);

    FieldScene_RunScene39f_02000d90(13, 552, 136, 0x38000);       /* 138 << 2, 224 << 10 */
    Engine_ActorFaceActor(ACTOR_PARTY_LEADER, 13, 0);
    Battle_WaitMode0(6);

    FieldScene_RunScene39f_02000d90(13, 584, 136, 0x38000);       /* 146 << 2 */
    Engine_ActorFaceActor(ACTOR_PARTY_LEADER, 13, 0);
    Battle_WaitMode0(6);

    Engine_ActorSetPosition(13, 0, 0);
    Engine_GameFlagSet(772);                         /* 193 << 2 */
    Engine_ActorSetPosition(16, 0, 0);

    /* Common exit; no argument registers are set. */
    Engine_EventEnd();
}

void FieldScene_RunScene39f_02001818(void)
{
    u32 i;
    s32 record;

    Engine_EventBegin();
    FieldScene_RunSixCallSetupSequence(14, 1);
    FieldScene_RunScene39f_02000d90(14, 0x1a8, 0x1e0, 0x79999);
    Battle_WaitMode0(2);
    MogoruMori_SpawnPuffRing(14);
    Engine_ActorSetChildValue(14, 15);
    record = Object_GetById(14);
    Engine_ActorSetSpriteFlags(record, 0);
    Battle_WaitMode0(30);
    Engine_GameFlagSet(0x305);
    Engine_ActorSetPosition(17, 0x1a80000, 0x1e00000);
    Engine_EventEnd();
}

void SceneActor_RunActorFourteenFourWaypointMotion(void)
{
    u8 *slot;

    slot = Object_GetById(14);

    /* r0 still holds the record returned above. */
    Engine_EventBegin();

    FieldScene_RunSixCallSetupSequence(14, 1);
    FieldScene_RunScene39f_02000d90(14, 392, 504, 0x60000);       /* 196 << 1, 252 << 1, 192 << 11 */
    Battle_WaitMode0(10);

    Effect_Spawn(*(s32 *)(slot + 8), *(s32 *)(slot + 12),
                  *(s32 *)(slot + 16) + 0x40000,    /* 128 << 11 */
                  0, 0, 0, 1, 0);

    Engine_CameraFollowActor(14, 1);
    Engine_ActorFaceEachOther(14, ACTOR_PARTY_LEADER, 0);
    Battle_WaitMode0(20);
    Engine_ActorStartRepeatedMotion(14, 2);
    Engine_ActorSetAttachedEffect(14, 258);                     /* 129 << 1 */
    Battle_WaitMode0(60);

    /* Four waypoints; Z is 132 << 2 and the height 192 << 10 throughout. */
    FieldScene_RunScene39f_02000d90(14, 360, 528, 0x30000);
    Engine_ActorFaceActor(ACTOR_PARTY_LEADER, 14, 0);
    Battle_WaitMode0(6);

    FieldScene_RunScene39f_02000d90(14, 328, 528, 0x30000);
    Engine_ActorFaceActor(ACTOR_PARTY_LEADER, 14, 0);
    Battle_WaitMode0(6);

    FieldScene_RunScene39f_02000d90(14, 288, 528, 0x30000);
    Engine_ActorFaceActor(ACTOR_PARTY_LEADER, 14, 0);
    Battle_WaitMode0(6);

    FieldScene_RunScene39f_02000d90(14, 256, 528, 0x30000);
    Engine_ActorFaceActor(ACTOR_PARTY_LEADER, 14, 0);
    Battle_WaitMode0(6);

    Engine_CameraFollowActor(ACTOR_PARTY_LEADER, 1);
    Engine_ActorSetPosition(14, 0, 0);
    Battle_WaitMode0(30);
    Engine_GameFlagSet(0x306);
    Engine_ActorSetPosition(17, 0, 0);

    /* Common exit; no argument registers are set. */
    Engine_EventEnd();
}

/* Mogall Forest: after the probe moves an actor, copy the cells it opened;
 * when actor 9 reaches column 42 or actor 11 column 40, raise its priority,
 * set flag 0x312 or 0x313 and play its landing. */
/* Mogoru Forest: after a probed actor moves, copy the cells it opened; when
 * actor 9 reaches column 42 or actor 11 column 40, raise its priority, set
 * flag 0x312 or 0x313 and play its landing. */
void MogoruMori_RunProbedLandingScene(void)
{
    struct StagedActorProbe probe;
    s32 landed;

    Engine_EventBegin();
    landed = 0;
    if (StagedActor_FindClearPosition(&probe) != 0) {
        SceneActor_MoveAndRedraw(probe);
        if (probe.actor_slot == 9)
            goto nine;
        if (probe.actor_slot == 11)
            goto eleven;
        goto other;
    nine:
        Map_CopyCellAttributeRect(38, 68, 1, 4, probe.position_x >> 20, 68);
        if (probe.position_x >> 20 == 42) {
            Map_CopyCellAttributeRect(26, 20, 2, 4, probe.position_x >> 20, 23);
            Engine_ActorSetSpritePriority(9, 1);
            landed = 1;
            Engine_GameFlagSet(0x312);
        }
        goto join;
    eleven:
        if (probe.position_x >> 20 == 40) {
            Map_CopyCellAttributeRect(26, 20, 2, 4, probe.position_x >> 20, 32);
            Engine_ActorSetSpritePriority(11, 1);
            landed = 1;
            Engine_GameFlagSet(0x313);
        }
    join:
        if (landed == 0) {
            Engine_EventEnd();
            return;
        }
        Object_SetModeById(probe.actor_slot, 3);
        ObjectMotion_OffsetPositionAndResetMotion(probe.actor_slot, 18, 6);
        Battle_WaitMode0(30);
        Object_SetModeById(probe.actor_slot, 8);
        Audio_PlayCue(240);
        Object_GetById(probe.actor_slot)->priority_flags = 2;
        goto end;
    other:
        if (probe.actor_slot == 8)
            Map_CopyCellAttributeRect(42, 49, 1, 4, probe.position_x >> 20, 49);
    }
end:
    Engine_EventEnd();
}

void SceneActor_BobActorZeroWhenTargetClear(void)
{
    u8 *record;
    u8 *mode;
    u8 saved;
    s32 target[3];

    record = Object_GetById(ACTOR_PARTY_LEADER);
    mode = record + 85;
    saved = *mode;

    target[0] = *(s32 *)((u8 *)Object_GetById(0) + 8) + (s32)0xffe00000;
    target[1] = *(s32 *)((u8 *)Object_GetById(0) + 12);
    target[2] = *(s32 *)((u8 *)Object_GetById(0) + 16);

    if (SceneActor_TryRunSlotZeroMoveStep(target)!= 0) {
        /* r0 still holds the nonzero result of the test above. */
        Engine_EventBegin();

        *mode = 0;
        Object_SetModeById(11, 7);

        *(s32 *)(record + 12) += (s32)0xffff0000;
        *(s32 *)(record + 20) += (s32)0xffff0000;
        WaitFrames(2);

        *(s32 *)(record + 12) += (s32)0xffff0000;
        *(s32 *)(record + 20) += (s32)0xffff0000;
        WaitFrames(10);

        *(s32 *)(record + 12) += 0x10000;
        *(s32 *)(record + 20) += 0x10000;
        WaitFrames(4);

        *(s32 *)(record + 12) += 0x10000;
        *(s32 *)(record + 20) += 0x10000;

        *mode = saved;
        Engine_EventEnd();
    }
}

/*
 * Parking step for actor slot 13 in resource_39f. It marks the actor's own
 * tile 0xff, clears the four orthogonally adjacent tiles, and once the actor
 * stands on tile (45, 6) clears the record's mode byte and writes -2.0 in
 * 16.16 into the words at +12 and +20.
 *
 * The 176-byte owner at 0x02001b84 runs past its code to include an alignment
 * halfword and the pool word 0xfffe0000 at 0x02001c30.
 */
void SceneActor_MarkActorThirteenTileAndPark(void)
{
    s32 x;
    s32 z;

    /* No argument register is written before this branch. */
    Engine_EventBegin();

    x = ((s32 *)Object_GetById(13))[2] >> 20;
    z = ((s32 *)Object_GetById(13))[4] >> 20;

    StagedActor_FillGridAttributeRectangle(2, x, z, 1, 1, 0xff);
    StagedActor_FillGridAttributeRectangle(2, x + 1, z, 1, 1, 0);
    StagedActor_FillGridAttributeRectangle(2, x - 1, z, 1, 1, 0);
    StagedActor_FillGridAttributeRectangle(2, x, z + 1, 1, 1, 0);
    StagedActor_FillGridAttributeRectangle(2, x, z - 1, 1, 1, 0);

    if (x == 45 && z == 6) {
        u8 *record = (u8 *)Object_GetById(13);

        record[85] = 0;
        *(s32 *)(record + 20) = (s32)0xfffe0000;
        *(s32 *)(record + 12) = (s32)0xfffe0000;
    }

    /* Common exit; no argument registers are set. */
    Engine_EventEnd();
}

void FieldScene_RunSupplementalSequenceOne(void)
{

    s32 x;
    s32 y;
    u8 *record;

    Engine_EventBegin();
    record = Object_GetById(14);
    x = *(s32 *)(record + 8);
    record = Object_GetById(14);
    y = *(s32 *)(record + 16);
    x >>= 20;
    y >>= 20;
    StagedActor_FillGridAttributeRectangle(2, x, y, 1, 1, 255);
    StagedActor_FillGridAttributeRectangle(2, x + 1, y, 1, 1, 0);
    StagedActor_FillGridAttributeRectangle(2, x - 1, y, 1, 1, 0);
    StagedActor_FillGridAttributeRectangle(2, x, y + 1, 1, 1, 0);
    StagedActor_FillGridAttributeRectangle(2, x, y - 1, 1, 1, 0);
    record = Object_GetById(14);
    if ((*(s32 *)(record + 16) >> 20) == 27) {
        record = Object_GetById(14);
        record[85] = 0;
        *(s32 *)(record + 20) = -0x20000;
        *(s32 *)(record + 12) = -0x20000;
        Engine_GameFlagSet(0x214);
        StagedActor_FillGridAttributeRectangle(2, 43, 23, 1, 1, 255);
    }
    Engine_EventEnd();
}

/* Mogall Forest: actor 15 hops out of the trees, greets the party and
 * hops back to the leader, then sets flag 0x307. */
/*
 * Actor presentation beat for overlay resource_39f.  The twin at 0x02001d04
 * is the same beat for slot 15.
 */
void FieldScene_RunScene39fSequenceA(void)
{

    s32 rec7;
    s32 big;
    s32 first;
    s32 shown;
    s32 second;
    s32 base3_2000240;

    rec7 = Object_GetById(15);
    big = 0x80000;
    Engine_EventBegin();
    FieldScene_RunSixCallSetupSequence(15, 0);
    FieldScene_RunScene39f_02000d90(15, 0x1d8, 104, big);
    Battle_WaitMode0(10);
    Effect_Spawn(*(s32 *)(rec7 + 8), *(s32 *)(rec7 + 12), (*(s32 *)(rec7 + 16) + big), 0, 0, 0, 1, 0);
    Engine_CameraFollowActor(15, 1);
    Engine_ActorFaceEachOther(15, ACTOR_PARTY_LEADER, 0);
    Battle_WaitMode0(30);
    Engine_ActorStartRepeatedMotion(15, 2);
    Engine_ActorShowEmote(15, 0x103, 0);
    Audio_PlayCue(147);
    Battle_WaitMode0(60);
    first = Object_GetById(ACTOR_PARTY_LEADER);
    shown = *(s16 *)(first + 10);
    second = Object_GetById(ACTOR_PARTY_LEADER);
    FieldScene_RunScene39f_02000d90(15, shown, *(s16 *)(second + 18), 0x60000);
    /* FAKEMATCH: called as returning a doubleword, the wait keeps the
     * registers the game keeps across it. */
    ((s64 (*)())Battle_WaitMode0)(10);
    Engine_GameFlagSet(0x307);
    /* FAKEMATCH: the game-state address held in a forced temporary keeps
     * it apart from the byte offset. */
    base3_2000240 = (s32)&gGameState;
    *(u8 *)((base3_2000240 + 0x22b)) = 3;
    BattleFx_SetWeightedResult(53, 0);
    Engine_EventEnd();
}

void FieldScene_RunSlot16WaypointSequence(void)
{
    u8 *slot;

    slot = Object_GetById(16);

    /* r0 still holds the record returned above. */
    Engine_EventBegin();

    FieldScene_RunSixCallSetupSequence(16, 1);
    FieldScene_RunScene39f_02000d90(16, 456, 152, 0x60000);       /* 228 << 1, 192 << 11 */
    Battle_WaitMode0(10);

    Effect_Spawn(*(s32 *)(slot + 8), *(s32 *)(slot + 12),
                  *(s32 *)(slot + 16) + 0x40000,    /* 128 << 11 */
                  0, 0, 0, 1, 0);

    Engine_CameraFollowActor(16, 1);
    Engine_ActorFaceEachOther(16, ACTOR_PARTY_LEADER, 0);
    Battle_WaitMode0(20);
    Engine_ActorStartRepeatedMotion(16, 2);
    Engine_ActorSetAttachedEffect(16, 258);                     /* 129 << 1 */
    Battle_WaitMode0(60);

    /* Three waypoints at height 0x30000 (192 << 10). */
    FieldScene_RunScene39f_02000d90(16, 448, 192, 0x30000);       /* 224 << 1 */
    Engine_ActorFaceActor(ACTOR_PARTY_LEADER, 16, 0);
    Battle_WaitMode0(6);

    FieldScene_RunScene39f_02000d90(16, 424, 208, 0x30000);       /* 212 << 1 */
    Engine_ActorFaceActor(ACTOR_PARTY_LEADER, 16, 0);
    Battle_WaitMode0(6);

    FieldScene_RunScene39f_02000d90(16, 424, 224, 0x30000);
    Engine_ActorFaceActor(ACTOR_PARTY_LEADER, 16, 0);
    Battle_WaitMode0(6);

    Engine_CameraFollowActor(ACTOR_PARTY_LEADER, 1);
    Engine_ActorSetPosition(16, 0, 0);
    Battle_WaitMode0(30);
    Engine_GameFlagSet(776);                         /* 194 << 2 */
    Engine_ActorSetPosition(20, 0, 0);

    /* Common exit; no argument registers are set. */
    Engine_EventEnd();
}

void FieldScene_RunActor17CameraSequence(void)
{
    u8 *slot;

    slot = Object_GetById(17);

    /* r0 still holds the record returned above. */
    Engine_EventBegin();

    FieldScene_RunSixCallSetupSequence(17, 1);
    FieldScene_RunScene39f_02000d90(17, 392, 104, 0x60000);       /* 196 << 1, 192 << 11 */
    Battle_WaitMode0(10);

    Effect_Spawn(*(s32 *)(slot + 8), *(s32 *)(slot + 12),
                  *(s32 *)(slot + 16) + 0x40000,    /* 128 << 11 */
                  0, 0, 0, 1, 0);

    Engine_CameraFollowActor(17, 1);
    Engine_ActorFaceEachOther(17, ACTOR_PARTY_LEADER, 0);
    Battle_WaitMode0(20);
    Engine_ActorStartRepeatedMotion(17, 2);
    Engine_ActorSetAttachedEffect(17, 258);                     /* 129 << 1 */
    Battle_WaitMode0(60);

    FieldScene_RunScene39f_02000d90(17, 376, 152, 0x60000);       /* 188 << 1 */
    Engine_ActorFaceActor(ACTOR_PARTY_LEADER, 17, 0);
    Battle_WaitMode0(10);

    FieldScene_RunScene39f_02000d90(17, 328, 160, 0x30000);       /* 164 << 1, 192 << 10 */
    Engine_ActorFaceActor(ACTOR_PARTY_LEADER, 17, 0);
    Battle_WaitMode0(6);

    FieldScene_RunScene39f_02000d90(17, 296, 160, 0x30000);       /* 148 << 1 */
    Engine_ActorFaceActor(ACTOR_PARTY_LEADER, 17, 0);
    Battle_WaitMode0(6);

    Engine_CameraFollowActor(ACTOR_PARTY_LEADER, 1);
    Engine_ActorSetPosition(17, 0, 0);
    Battle_WaitMode0(30);
    Engine_GameFlagSet(0x309);
    Engine_ActorSetPosition(21, 0, 0);

    /* Common exit; no argument registers are set. */
    Engine_EventEnd();
}

void FieldScene_RunScene39f_02002004(void)
{
    u32 i;
    s32 record;

    Engine_EventBegin();
    FieldScene_RunSixCallSetupSequence(18, 1);
    Engine_CameraMoveTo(0x2e80000, -1, 0x1f80000, 1);
    FieldScene_RunScene39f_02000d90(18, 0x2e8, 0x1f8, 0x90000);
    MogoruMori_SpawnPuffRing(18);
    Engine_ActorSetChildValue(18, 15);
    record = Object_GetById(18);
    Engine_ActorSetSpriteFlags(record, 0);
    Battle_WaitMode0(30);
    Engine_GameFlagSet(0x30a);
    Engine_ActorSetPosition(22, 0x2e80000, 0x1f80000);
    Engine_EventEnd();
}

void FieldScene_RunActorEighteenEffectSequence(void)
{
    u8 *slot;

    slot = Object_GetById(18);

    /* r0 still holds the record returned above. */
    Engine_EventBegin();

    FieldScene_RunSixCallSetupSequence(18, 1);
    FieldScene_RunScene39f_02000d90(18, 712, 536, 0x60000);       /* 178 << 2, 134 << 2, 192 << 11 */
    Battle_WaitMode0(10);

    Effect_Spawn(*(s32 *)(slot + 8), *(s32 *)(slot + 12),
                  *(s32 *)(slot + 16) + 0x40000,    /* 128 << 11 */
                  0, 0, 0, 1, 0);

    Engine_CameraFollowActor(18, 1);
    Engine_ActorFaceEachOther(18, ACTOR_PARTY_LEADER, 0);
    Battle_WaitMode0(20);
    Engine_ActorStartRepeatedMotion(18, 2);
    Engine_ActorSetAttachedEffect(18, 258);                     /* 129 << 1 */
    Battle_WaitMode0(60);

    FieldScene_RunScene39f_02000d90(18, 712, 568, 0x60000);       /* 142 << 2 */
    Engine_ActorFaceActor(ACTOR_PARTY_LEADER, 18, 0);
    Battle_WaitMode0(10);

    FieldScene_RunScene39f_02000d90(18, 712, 600, 0x30000);       /* 150 << 2, 192 << 10 */
    Engine_ActorFaceActor(ACTOR_PARTY_LEADER, 18, 0);
    Battle_WaitMode0(6);

    FieldScene_RunScene39f_02000d90(18, 736, 640, 0x30000);       /* X += 24, 160 << 2 */
    Engine_ActorFaceActor(ACTOR_PARTY_LEADER, 18, 0);
    Battle_WaitMode0(6);

    FieldScene_RunScene39f_02000d90(18, 736, 704, 0x30000);       /* 176 << 2 */
    Engine_ActorFaceActor(ACTOR_PARTY_LEADER, 18, 0);
    Battle_WaitMode0(6);

    Engine_CameraFollowActor(ACTOR_PARTY_LEADER, 1);
    Engine_ActorSetPosition(18, 0, 0);
    Battle_WaitMode0(30);
    Engine_GameFlagSet(0x30b);

    /* Common exit; no argument registers are set. */
    Engine_EventEnd();
}

void FieldScene_RunScene39f_020021b0(void)
{
    s32 rec7;

    rec7 = Object_GetById(18);
    Engine_EventBegin();
    Engine_ActorSetPosition(18, 0x880000, 0x1680000);
    FieldScene_RunSixCallSetupSequence(18, 1);
    FieldScene_RunScene39f_02000d90(18, 136, 0x198, 0x80000);
    Battle_WaitMode0(10);
    Effect_Spawn(*(s32 *)(rec7 + 8), *(s32 *)(rec7 + 12), (*(s32 *)(rec7 + 16) + 0x40000), 0, 0, 0, 1, 0);
    Engine_ActorFaceDirection(18, 0xc000, 40);
    Engine_ActorSetAttachedEffect(18, 0x102);
    Engine_ActorRunRepeatedMotion(18, 2);
    Engine_CameraFollowActor(18, 1);
    FieldScene_RunScene39f_02000d90(18, 136, 0x1b8, 0x60000);
    Engine_ActorFaceActor(ACTOR_PARTY_LEADER, 18, 0);
    Battle_WaitMode0(10);
    FieldScene_RunScene39f_02000d90(18, 136, 0x1d8, 0x30000);
    Engine_ActorFaceActor(ACTOR_PARTY_LEADER, 18, 0);
    Battle_WaitMode0(6);
    FieldScene_RunScene39f_02000d90(18, 136, 0x1f8, 0x30000);
    Engine_ActorFaceActor(ACTOR_PARTY_LEADER, 18, 0);
    Battle_WaitMode0(6);
    Engine_CameraFollowActor(ACTOR_PARTY_LEADER, 1);
    Engine_ActorSetPosition(18, 0, 0);
    Battle_WaitMode0(60);
    Engine_GameFlagSet(0x89d);
    Engine_EventEnd();
}

/* Run the closing choreography of the room's scene. */
void MogoruMori_RunClosingChoreography(void)
{
    struct Vec dir;
    struct Vec *v;
    struct FieldActor *actor;
    u32 i;
    s32 x;
    s32 z;
    s32 zero;
    s32 game;

    actor = Object_GetById(18);
    Engine_EventBegin();
    Engine_ActorSetChildValue(18, 15);
    Engine_ActorSetSpriteFlags(Object_GetById(18), 0);
    Call3(Engine_ActorSetPosition, 18, 0x880000, 0x1680000);
    Engine_CameraSetSpeed(0x8000, 0x1000);
    Call4(Engine_CameraMoveTo, 0x880000, -1, 0x1880000, 1);
    Engine_CameraWaitForMove();
    Battle_WaitMode0(60);
    Call3(Engine_WorkSetValuesIfNonNegative, 0x50000, 0x50000, 0x10000);
    MogoruMori_SpawnPuffRing(18);
    Engine_ActorStartRepeatedMotion(0, 2);
    Call3(Engine_WorkSetValuesIfNonNegative, -1, -1, 0xe666);
    Engine_MapWaitWorkValuesBelow256();
    Engine_ActorFaceDirection(0, 0xc000, 20);
    Battle_WaitMode0(40);
    Call3(Engine_WorkSetValuesIfNonNegative, 0x50000, 0x50000, 0x10000);
    MogoruMori_SpawnPuffRing(18);
    Call3(Engine_WorkSetValuesIfNonNegative, -1, -1, 0xe666);
    Engine_MapWaitWorkValuesBelow256();
    Battle_WaitMode0(40);
    MogoruMori_SpawnPuffRing(18);
    actor->scale_x = 0x13333;
    actor->scale_y = 0x13333;
    Engine_ActorSetChildValue(18, 5);
    FieldScene_RunScene39f_02000d90(18, 136, 0x188, 0xf0000);
    Battle_WaitMode0(15);
    Call3(Engine_WorkSetValuesIfNonNegative, 0x50000, 0x50000, 0x10000);
    for (i = 0; i <= 16; i++) {
        v = &dir;
        /* FAKEMATCH: zero is set inside the loop (always entered) so it is
         * materialized with the counter. */
        zero = 0;
        v->x = Engine_MathCos(i << 12);
        v->y = zero;
        z = Engine_MathSin(i << 12);
        x = v->x;
        v->z = z;
        x += x / 2;
        v->x = x;
        Effect_Spawn(actor->x.fixed, 0, actor->z.fixed, x, v->y, z, 1, (const struct EffectOptions *)zero);
    }
    Battle_WaitMode0(30);
    Call3(Engine_WorkSetValuesIfNonNegative, -1, -1, 0xe666);
    Engine_MapWaitWorkValuesBelow256();
    Audio_PlayCue(148);
    Engine_ActorRunRepeatedMotion(18, 2);
    Battle_WaitMode0(20);
    {
        s32 px = Object_GetById(0)->x.part.pixel;

        FieldScene_RunScene39f_02000d90(18, px, Object_GetById(0)->z.part.pixel - 16, 0x80000);
    }
    Battle_WaitMode0(10);
    /* FAKEMATCH: an empty do-while here moves px into r1 after the
     * other arguments of the call above. */
    do { } while (0);
    game = (s32)&gGameState;
    *(u8 *)(game + 0x22b) = 3;
    Party_SetFields1ceAnd1d0((s32)&SceneId_MogoruMori3, 15);
    BattleFx_SetWeightedResult(53, 1);
    Engine_EventEnd();
}

/* What each area answers. */
const struct SceneEvent *Scene_GetEvents(void)
{
    s16 selector = gGameState.scene;

    if (selector == (s32)&SceneId_MogoruMori1) {
        return gMogoruMoriEvents1;
    }
    if (selector == (s32)&SceneId_MogoruMori2) {
        return gMogoruMoriEvents2;
    }
    if (selector == (s32)&SceneId_MogoruMori3) {
        return gMogoruMoriEvents3;
    }
    return gMogoruMoriEventsOther;
}

/* Stores 0x204 in the scene work word at 448, then by area of Mogoru Forest
 * (SceneId_MogoruMori1 to 3) and entry number applies the flag-dependent
 * actor placements and states for that entry. Returns 0. */
s32 FieldScene_RunSceneEntryHook(void)
{
    u8 *rec;
    s32 mode;
    s32 step;
    s32 pos;
    s32 tmp;

    /* FAKEMATCH: one scoped offset of 448 serves both the event work's start
       transition and the game state's scene, so the constant is loaded once
       into a register the two accesses share. */
    {
        u8 *work = (u8 *)gEventWork;
        s32 off = 448;

        *(s32 *)(work + off) = 0x204;
        mode = *(s16 *)((u8 *)&gGameState + off);
    }
    if (mode == (s32)&SceneId_MogoruMori1) {
        step = *(s16 *)((u8 *)&gGameState + 450);
        switch (step) {
        case 1:
        case 2:
        case 3:
        case 4:
            if (Value1(Engine_GameFlagIsSet, 0x89c) == 0) {
                Engine_EventBegin();
                WaitFrames(1);
                Engine_ActorSetChildValue(10, 1);
                Call3(Engine_ActorSetPosition, 10, 0x5c0000, 0x780000);
                Call3(Engine_ActorFaceDirection, 10, 0xd000, 0);
                Call3(ObjectMotion_SetSpeedParameters, 0, 0x6666, 0x3333);
                Engine_ActorWalkTo(0, 136, 64);
                Engine_EventOpenScreen();
                Engine_EventWaitForScreen();
                ObjectMotion_CommitCurrentPositionAndActivate(0);
                Battle_WaitMode0(30);
                Call3(Engine_ActorShowEmote, 10, 256, 0);
                Engine_ActorRunRepeatedMotion(10, 2);
                Battle_WaitMode0(30);
                FieldScene_RunScene39f_02000d90(10, 136, 116, 0x70000);
                MogoruMori_SpawnPuffRing(10);
                Engine_ActorSetChildValue(10, 15);
                Engine_ActorSetSpriteFlags((u8 *)Object_GetById(10), 0);
                Engine_GameFlagSet(0x89c);
                Battle_WaitMode0(60);
                Engine_EventEnd();
            }
            if (Engine_GameFlagIsSet(0x109) == 0) {
                break;
            }
            if (Engine_GameFlagIsSet(768) != 0) {
                break;
            }
            Engine_ActorSetChildValue(10, 15);
            Call3(Engine_ActorSetPosition, 10, 0x880000, 0x740000);
            break;

        case 7:
        case 8:
        case 9:
            rec = (u8 *)Object_GetById(0);
            if (rec != 0) {
                Engine_ActorSetPosition(16, *(s32 *)(rec + 8), *(s32 *)(rec + 16));
            }
            rec = (u8 *)Object_GetById(16);
            *(s32 *)(rec + 108) = 0;
            if (Engine_GameFlagIsSet(0x109) != 0) {
                rec = (u8 *)Object_GetById(16);
                *(s32 *)(rec + 12) = 0x200000;
            }
            WaitFrames(1);
            Engine_ActorSetPosition(16, 0x2780000, 0x1b80000);
            if (Engine_GameFlagIsSet(0xfd4) == 0) {
                InitializeOrbitingEffect(16);
            }
            Engine_ActorSetChildValue(11, 15);
            Engine_ActorSetChildValue(12, 15);
            Engine_ActorSetSpriteFlags((u8 *)Object_GetById(11), 0);
            Engine_ActorSetSpriteFlags((u8 *)Object_GetById(12), 0);
            FieldScene_RedrawActorFootprint(8);
            if (Engine_GameFlagIsSet(784) == 0) {
                FieldScene_RedrawActorFootprint(9);
                break;
            }
            WaitFrames(1);
            Call3(Engine_ActorSetPosition, 9, 0x2100000, 0x1980000);
            Object_SetModeById(9, 4);
            Call6(Map_CopyCellAttributeRect, 38, 27, 4, 2, 31, 25);
            *(u8 *)((u8 *)Object_GetById(9) + 35) = 2;
            break;
        }
    } else if (mode == (s32)&SceneId_MogoruMori2) {
        step = *(s16 *)((u8 *)&gGameState + 450);
        switch (step) {
        case 3:
        case 4:
        case 5:
        case 6:
            if (Engine_GameFlagIsSet(0x303) == 0) {
                Engine_ActorSetChildValue(12, 15);
                Engine_ActorSetSpriteFlags((u8 *)Object_GetById(12), 0);
            }
            if (Engine_GameFlagIsSet(772) != 0) {
                break;
            }
            Engine_ActorSetChildValue(13, 15);
            Engine_ActorSetSpriteFlags((u8 *)Object_GetById(13), 0);
            break;

        case 10:
        case 11:
        case 12:
            if (Engine_GameFlagIsSet(0x311) == 0) {
                FieldScene_RedrawActorFootprint(10);
            } else {
                s32 attr = 0;

                WaitFrames(1);
                Call3(Engine_ActorSetPosition, 10, 0x2280000, 0x1fe0000);
                Object_SetModeById(10, 4);
                *(u8 *)((u8 *)Object_GetById(10) + 35) = 2;
                Call6(Map_CopyCellAttributeRect, 44, 30, 2, 4, 34, 30);
#if EDITION_INTERNATIONAL
                StagedActor_FillGridAttributeRectangle(0, 35, 29, 1, 4, attr);
#endif
            }
            FieldScene_RedrawActorFootprint(8);
            FieldScene_RedrawActorFootprint(9);
            pos = *(s32 *)((u8 *)Object_GetById(11) + 8);
            tmp = *(s32 *)((u8 *)Object_GetById(11) + 16);
            pos >>= 20;
            StagedActor_FillGridAttributeRectangle(2, pos, tmp >> 20, 1, 1, 255);
            WaitFrames(1);
            Engine_ActorSetChildValue(11, 6);
            {
                u8 *obj = (u8 *)Object_GetById(8);
                u32 mask = 8;
                mask = mask | obj[89];
                obj[89] = mask;
            }
            if (Engine_GameFlagIsSet(0x306) != 0) {
                break;
            }
            Engine_ActorSetChildValue(14, 15);
            Engine_ActorSetSpriteFlags((u8 *)Object_GetById(14), 0);
            if (Engine_GameFlagIsSet(0x305) == 0) {
                break;
            }
            Call3(Engine_ActorSetPosition, 14, 0x1a80000, 0x1e00000);
            Call3(Engine_ActorSetPosition, 17, 0x1a80000, 0x1e00000);
            break;
        }
    } else if (mode == (s32)&SceneId_MogoruMori3) {
        step = *(s16 *)((u8 *)&gGameState + 450);
        switch (step) {
        case 3:
        case 4:
        case 5:
        case 6:
            WaitFrames(1);
            if (Engine_GameFlagIsSet(0x307) == 0) {
                Engine_ActorSetChildValue(15, 15);
                Engine_ActorSetSpriteFlags((u8 *)Object_GetById(15), 0);
                Engine_ActorSetSpriteFlags((u8 *)Object_GetById(19), 0);
            }
            if (Engine_GameFlagIsSet(776) == 0) {
                Engine_ActorSetChildValue(16, 15);
                Engine_ActorSetSpriteFlags((u8 *)Object_GetById(16), 0);
                Engine_ActorSetSpriteFlags((u8 *)Object_GetById(20), 0);
            }
            if (Engine_GameFlagIsSet(0x309) != 0) {
                break;
            }
            Engine_ActorSetChildValue(17, 15);
            Engine_ActorSetSpriteFlags((u8 *)Object_GetById(17), 0);
            Engine_ActorSetSpriteFlags((u8 *)Object_GetById(21), 0);
            break;

        case 7:
            pos = *(s32 *)((u8 *)Object_GetById(13) + 8);
            tmp = *(s32 *)((u8 *)Object_GetById(13) + 16);
            pos >>= 20;
            StagedActor_FillGridAttributeRectangle(2, pos, tmp >> 20, 1, 1, 255);
            Engine_ActorSetChildValue(13, 6);
            WaitFrames(1);
            {
                u8 *obj = (u8 *)Object_GetById(8);
                u32 mask = 8;
                mask = mask | obj[89];
                obj[89] = mask;
            }
            FieldScene_RedrawActorFootprint(8);
            break;

        case 8:
        case 9:
        case 10:
        case 11:
            OverlayObject_SpawnConfiguredWithMode15(0x2de0000, 0, 0x1720000, 223);
            OverlayObject_SpawnConfiguredWithMode15(0x2f20000, 0, 0x1720000, 223);
            FieldScene_RedrawActorFootprint(10);
            FieldScene_RedrawActorFootprint(12);
            if (Engine_GameFlagIsSet(0x312) == 0) {
                FieldScene_RedrawActorFootprint(9);
            } else {
                WaitFrames(1);
                Object_SetModeById(9, 4);
                Engine_ActorSetPosition(9, 0x2ba0000, 0x18e0000);
                {
                    u8 *obj = (u8 *)Object_GetById(9);
                    u32 mask = 2;
                    mask = mask | obj[35];
                    obj[35] = mask;
                }
                Call6(Map_CopyCellAttributeRect, 26, 20, 2, 4, 42, 23);
                Engine_GameFlagSet(532);
                Call3(Engine_ActorSetPosition, 14, 0x2780000, 0x1b80000);
                Engine_ActorSetSpriteFlags((u8 *)Object_GetById(14), 0);
            }
            if (Engine_GameFlagIsSet(0x313) == 0) {
                FieldScene_RedrawActorFootprint(11);
            } else {
                WaitFrames(1);
                Object_SetModeById(11, 4);
                Engine_ActorSetPosition(11, 0x29a0000, 0x2260000);
                *(u8 *)((u8 *)Object_GetById(11) + 35) = 2;
                Call6(Map_CopyCellAttributeRect, 26, 20, 2, 4, 40, 32);
            }
            pos = *(s32 *)((u8 *)Object_GetById(14) + 8);
            tmp = *(s32 *)((u8 *)Object_GetById(14) + 16);
            pos >>= 20;
            StagedActor_FillGridAttributeRectangle(2, pos, tmp >> 20, 1, 1, 255);
            Engine_ActorSetChildValue(14, 6);
            WaitFrames(1);
            {
                u8 *obj = (u8 *)Object_GetById(9);
                u32 mask = 8;
                mask = mask | obj[89];
                obj[89] = mask;
            }
            if (Engine_GameFlagIsSet(0x30b) == 0) {
                Engine_ActorSetChildValue(18, 15);
                Engine_ActorSetSpriteFlags((u8 *)Object_GetById(18), 0);
                if (Engine_GameFlagIsSet(0x30a) != 0) {
                    Call3(Engine_ActorSetPosition, 22, 0x2e80000, 0x1f80000);
                    Call3(Engine_ActorSetPosition, 18, 0x2e80000, 0x1f80000);
                }
#if defined(TBS_EDITION_DE) || defined(TBS_EDITION_ES) || defined(TBS_EDITION_FR) || defined(TBS_EDITION_IT)
            } else {
                Engine_ActorSetPosition(18, 0, 0);
#endif
            }
            FieldScene_RunSupplementalSequenceOne();
            break;

        case 12:
        case 13:
            Engine_ActorEnableActionCallback(18, MogoruMori_ActorScript);
            if (Engine_GameFlagIsSet(0x893) == 0) {
                break;
            }
            if (Engine_GameFlagIsSet(0x89e) == 0) {
                break;
            }
            Engine_GameFlagSet(0x88f);
            break;

        case 15:
            Engine_GameFlagSet(0x89e);
            break;
        }
    }
    return 0;
}

s32 UpdateOrbitingSceneObject(s32 *p)
{
    s16 *q = (s16 *)p[20];
    s32 a, b;
    s32 d = Engine_MathSin(p[12]) * 2;
    if (d > 0)
        d = -d;
    p[2] = p[14] + Engine_MathCos(p[12]) * 2;
    p[3] = p[15] + d;
    q[15] = Engine_MathCos(p[12] + 0x8000) / 8;
    a = Engine_RandomNext();
    b = Engine_RandomNext();
    p[12] = p[12] + ((((u32)a << 9) >> 16) + (((u32)b << 9) >> 16)) + 0x400;
    return 0;
}

void InitializeOrbitingEffect(s32 id)
{
    OrbitingSceneObject *actor;
    OrbitingSceneObjectSprite *sprite;
    u8 *transfer;
    s32 zero;

    actor = (OrbitingSceneObject *)Object_GetById(id);
    sprite = actor->sprite;
    sprite->flags_09_mode = 1;
    sprite->flags_05_bit_5 = 0;
    sprite->flags_09_high = 0;

    zero = 0;
    sprite->state = zero;
    Engine_ActorSetSpriteFlags(actor, zero);
    actor->active = zero;
    actor->mode = zero;

    if (Engine_GameFlagIsSet(0x109) == 0)
        actor->y += 0x200000;

    actor->flags_23 &= 0xfe;
    actor->visible = 1;

    transfer = AllocateEffectTransfer(17, 0x608);
    Engine_ItemLoadIcon(ITEM_NUT);
    transfer += 0x400;
    Engine_VramLoad(sprite->palette, 128, transfer);
    Engine_HeapRelease(17);

    actor->orbit_center_x = actor->x;
    actor->orbit_angle = zero;
    actor->orbit_center_y = actor->y;
    actor->active = 1;
    actor->callback = (u32)UpdateOrbitingSceneObject;
    actor->state = zero;
}
