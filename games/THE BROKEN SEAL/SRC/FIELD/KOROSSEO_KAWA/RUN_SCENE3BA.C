#include "TYPES.H"
#include "FIELD_EVENT.H"
#include "FIELD_SCENE.H"
#include "STAGED_ACTOR.H"
extern u8 MsgKorosseoLogsKeyClearingStage[];
extern u8 MsgKorosseoOperatorBridgeWillAlsoCheer[];
extern u8 MsgKorosseoPlaceNormallyCalledLumberWater[];
extern u8 MsgKorosseoTheyCallBrokenBridge[];
/* FAKEMATCH: calls that cast Korosseo_FadeInCompetitor to another return type keep their original register order. */
s32 Korosseo_FadeInCompetitor();

enum CoordinatorMessage {
    MSG_ROBIN_GOT = 0x96a
};

typedef struct Ctl {
    s16 f0;
    s16 f2;
    s16 f4;
    s16 f6;
    s16 f8;
} Ctl;

typedef struct PartyInteractionRecord {
    u8 padding_00[10];
    s16 x;
    u8 padding_0c[6];
    s16 y;
} PartyInteractionRecord;

typedef struct Rec {
    u8 pad00[216];
    u16 fd8[15];
} Rec;

/* The two mode records the entry point seeds; the halfword at +26 holds the
 * per-mode span in sixtieths. */
struct ModeRecord {
    u8 pad[26];
    u16 span;
};

typedef struct Position3 {
    s32 x;
    s32 y;
    s32 z;
} Position3;

/* The active subject's handle sits 500 bytes into the shared table. */
typedef struct ActiveSubjectSlot {
    u8 pad[500];
    void *handle;
} ActiveSubjectSlot;

extern u8 HexDigits[];

void Korosseo_FinishSoloRound();

void Korosseo_RestoreCompetitor();
typedef void(*SceneTask)(void);
s32 GameFlag_GetByteFar(s32);
s32 SceneDialogue_RunFlagGatedPromptInteraction();

PartyInteractionRecord *GetPartyInteractionRecord(void);
s32 GetPartyMemberCount(void);
Rec *Owner_GetState(s32);

/* Contiguous unnamed leaf-owner run for resource_3ba. */

/*
 * Scene setup for resource_3ba: allocates a scene descriptor, stamps its
 * parameter block, uploads image and palette, and installs the per-frame task.
 */

/* Import veneers, named by the main-image function each one reaches.
 * Old-style declarations: arities vary between call sites in this overlay. */

/* In-image data at file offset 0x3f14 (0x0200bf14 - 0x8000). */

/* The per-frame task this owner installs: in-image code, published below as
 * its entry address plus the Thumb bit. */

/* AUDITED GENERATED CALL SCRIPT for Scene_RunSceneFourCoordinator:
 * A phase-two fast path, full and revisit branches, and all 42 calls across
 * the complete scene-four coordinator. */

/* This overlay's own occupancy lookup for a cell. The record pointer the call
 * sites also load is spelled here, although the lookup itself uses only the
 * position. */

/* In-image direction table: sixteen packed steps, high half x, low half z. */

/* A countdown word this overlay owns at KorosseoKawa_Countdown: each call decrements
 * it by one, and specific values select which sub-sequence runs this call.
 * Reaching 0 restarts the countdown at 120 after running its own branch. */

/* FAKEMATCH: Calls through these inline helpers pass their constants straight
 * into the argument registers; a direct call precomputes a costly constant
 * into a pseudo that the compiler then shares with later uses in the block.
 * A value-returning call also sets r0 last of its arguments. */
s32 SceneDialogue_RunFlagGatedPromptInteraction(s32 a, s32 b);

void FieldScene_RunMiddleSequence(s32 mode, s32 owner, s32 base);

s32 *SceneActor_FindOccupantAheadOfSubject(void);

void SceneState_StoreParamsAndInitTable(s32 a, s32 b, s32 c);

static __inline__ void Call6(void (*f)(), s32 a0, s32 a1, s32 a2, s32 a3, s32 a4, s32 a5)
{
    f(a0, a1, a2, a3, a4, a5);
}

static __inline__ void Call1(void (*f)(), s32 a0)
{
    f(a0);
}

static __inline__ s32 Value1(s32 (*f)(), s32 a0)
{
    return f(a0);
}

static __inline__ void Call2(void (*f)(), s32 a0, s32 a1)
{
    f(a0, a1);
}

static __inline__ void Call4(void (*f)(), s32 a0, s32 a1, s32 a2, s32 a3)
{
    f(a0, a1, a2, a3);
}

static __inline__ void Call3(void (*f)(), s32 a0, s32 a1, s32 a2)
{
    f(a0, a1, a2);
}

static __inline__ s32 Value2(s32 (*f)(), s32 a0, s32 a1)
{
    return f(a0, a1);
}

static __inline__ s32 Value3(s32 (*f)(), s32 a0, s32 a1, s32 a2)
{
    return f(a0, a1, a2);
}

static inline void InitializeActorZero(void)
{
    Actor_SetSpeed(ACTOR_PARTY_LEADER, 0x10000, 0x8000);
}

static inline void InitializeSelectedActor(s32 actorId)
{
    Actor_SetSpeed(actorId, 0x10000, 0x8000);
}

static __inline__ s32 Value0(s32 (*f)())
{
    return f();
}

/* Selects a later line in the current dialogue. */
static __inline__ void AdvanceMessage(s32 amount)
{
    gEventWork->message += amount;
}

void SceneState_ResetCounterAndStartTask(void);
void SceneState_SetMode66AndPassOpeningSequence(void);
void SceneState_WaitUntilWordC41cIs22(void);
void StagedActor_PlacePairAtOffsetAndRun(s32 actor_id, s32 dx, s32 dz);

void FieldScene_RunScene3ba_020015e0(s32 a0)
{

    u32 i;
    s32 rec8;
    s32 record;

    if (gGameState.entrance == 2) {
        Korosseo_FinishSoloRound();
    } else {
        Event_Begin();
        rec8 = Value2(SceneDialogue_RunFlagGatedPromptInteraction, a0, 3);
        if (rec8 == 0) {
            Event_SetMessage((s32)MsgKorosseoTheyCallBrokenBridge);
            SceneState_ResetCounterAndStartTask();
            Camera_SetSpeed(0x30000, 0x6000);
            Camera_MoveTo(0x3480000, -1, 0xd80000, 1);
            ((void (*)())Engine_CameraWaitForMove)();
            Event_ShowMessage(a0, 0);
            SceneState_SetMode66AndPassOpeningSequence();
            Event_Wait(60);
            Event_ShowMessage(a0, 0);
            Value3(Korosseo_FadeInCompetitor, 0, 0x2e0, 200);
            Value3(Engine_ActorFaceDirection, 0, 0, 0);
            SceneState_WaitUntilWordC41cIs22();
            Actor_SetSpeed(ACTOR_PARTY_LEADER, 0x10000, 0x8000);
            Actor_WalkToAndWait(ACTOR_PARTY_LEADER, 0x330, 200);
            Event_Wait(30);
            Actor_ShowEmote(ACTOR_PARTY_LEADER, 0x105, 60);
            Event_ShowMessage(a0, 0);
            Korosseo_RestoreCompetitor(0);
            Camera_FollowActor(ACTOR_PARTY_LEADER, 0);
            SceneState_SendIdBySceneId(a0, 3);
        } else {
            if (rec8 == 1) {
                Event_SetMessage((s32)MsgKorosseoOperatorBridgeWillAlsoCheer);
                Event_ShowMessage(a0, 0);
            }
        }
        Value3(FieldScene_RunMiddleSequence, rec8, a0, 3);
        Event_End();
    }
}

void Scene_RunSceneFourCoordinator(s32 scene)
{

    s32 path;

    if (gGameState.entrance == 2) {
        Korosseo_FinishSoloRound();
        return;
    }
    Event_Begin();
    path = SceneDialogue_RunFlagGatedPromptInteraction(scene, 4);
    if (path == 0) {
        Event_SetMessage((s32)MsgKorosseoPlaceNormallyCalledLumberWater);
        Camera_SetSpeed(196608, 24576);
        Camera_MoveTo(71303168, -1, 11010048, 1);
        Camera_WaitForMove();
        Event_ShowMessage(scene, 0);
        Value3(SceneState_StoreParamsAndInitTable, 120, 72, 0);
        Event_ShowMessage(scene, 0);
        SceneState_ReleaseTableAndResetC6a6();
        Event_Wait(15);
        Value3(Korosseo_FadeInCompetitor, 0, 984, 200);
        Actor_FaceDirection(ACTOR_PARTY_LEADER, 0, 10);
        Event_ShowMessage(scene, 0);
        Actor_FaceDirection(ACTOR_PARTY_LEADER, 16384, 30);
        Actor_ShowEmote(ACTOR_PARTY_LEADER, 262, 60);
        Actor_SetSpeed(ACTOR_PARTY_LEADER, 98304, 49152);
        OverlayObject_PlaceWithScale14000(0, 1000, 192);
        OverlayObject_PlaceWithScale14000(0, 1000, 176);
        Call3(OverlayObject_PlaceWithScale14000, 0, 1016, 168);
        Event_Wait(15);
        Value3(StagedActor_PlacePairAtOffsetAndRun, 18, 160, 0);
        Camera_MoveTo(71303168, -1, 11010048, 1);
        Actor_SetAnimation(ACTOR_PARTY_LEADER, 1);
        Event_Wait(10);
        Actor_SetSpeed(ACTOR_PARTY_LEADER, 65536, 32768);
        Actor_WalkToAndWait(ACTOR_PARTY_LEADER, 1192, 168);
        Event_Wait(10);
        Actor_FaceDirection(ACTOR_PARTY_LEADER, 32768, 30);
        Actor_ShowEmote(ACTOR_PARTY_LEADER, 258, 60);
        Event_ShowMessage(scene, 0);
        Korosseo_RestoreCompetitor(0);
        Camera_FollowActor(ACTOR_PARTY_LEADER, 0);
        Actor_SetPosition(18, 66584576, 11010048);
        SceneState_SendIdBySceneId(scene, 4);
    } else if (path == 1) {
        Event_SetMessage((s32)MsgKorosseoLogsKeyClearingStage);
        Event_ShowMessage(scene, 0);
    }
    Value3(FieldScene_RunMiddleSequence, path, scene, 4);
    Event_End();
}

void SceneActor_PlaceSlots1To3FromWork(void)
{
    {
        s32 x = GameFlag_GetByteFar(896);
        s32 y = GameFlag_GetByteFar(904);

        x <<= 20;
        x += 0x80000;
        y <<= 20;
        y += 0x80000;
        Actor_SetPosition(ACTOR_GERALD, x, y);
    }
    {
        s32 x = GameFlag_GetByteFar(912);
        s32 y = GameFlag_GetByteFar(920);

        x <<= 20;
        x += 0x80000;
        y <<= 20;
        y += 0x80000;
        Actor_SetPosition(ACTOR_IVAN, x, y);
    }
    {
        s32 x = GameFlag_GetByteFar(928);
        s32 y = GameFlag_GetByteFar(936);

        x <<= 20;
        x += 0x80000;
        y <<= 20;
        y += 0x80000;
        Actor_SetPosition(ACTOR_MIA, x, y);
    }
}
void SceneState_SendIdBySceneId(s32 a, s32 b);
void SceneState_ReleaseTableAndResetC6a6(void);
void OverlayObject_PlaceWithScale14000(s32 a, s32 b, s32 c);
