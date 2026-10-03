#include "RUNTIME_MEM.H"
#include "TYPES.H"
#include "CALLBACK_SCHEDULER.H"
#include "FIELD_EVENT.H"
#include "FIELD_SCENE.H"
#include "STAGED_ACTOR.H"
#include "KAWA.H"
#include "CALL.H"
#include "RESOURCE_IDS.H"


enum CoordinatorMessage {
    MSG_ROBIN_GOT = 0x96a,
    MSG_WOULD_LIKE_FRIEND_CHEER_FOR = 0x207d,
    MSG_IF_KNOW_WHO_WANT_CHEER = 0x207e,
    MSG_ROBIN_WILL_CHEER_FOR_WAY = 0x207f,
    MSG_DO_YOUR_BEST = 0x2083,
    MSG_UNFORTUNATELY_WE_HAVE_FULL_HOUSE = 0x2084,
    MSG_MATCH_ABOUT_BEGIN_PLEASE_TAKE = 0x2085,
    MSG_OPERATOR_BRIDGE_WILL_ALSO_CHEER = 0x2094,
    MSG_THEY_CALL_BROKEN_BRIDGE = 0x2095,
    MSG_LOGS_KEY_CLEARING_STAGE = 0x2098,
    MSG_PLACE_NORMALLY_CALLED_LUMBER_WATER = 0x2099,
    MSG_SITE_FIRST_FINALS_BATTLE = 0x20cb,
    MSG_ASK_ATTENDANTS_FOR_EXPLANATIONS_STAGES = 0x20d4,
    MSG_WARRIORS_ENTER_FINALS_WITHOUT_ANY = 0x20d5,
    MSG_ROBIN_YOURE_CONTESTANT_IN_FINALS = 0x20e1,
    MSG_ROBIN_DID_GET_GOOD_LOOK = 0x20e5,
    MSG_WAIT_SHOULDNT_DECIDE_WHERE_BEST = 0x20e8
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


/* The active subject's handle sits 500 bytes into the shared table. */
typedef struct ActiveSubjectSlot {
    u8 pad[500];
    void *handle;
} ActiveSubjectSlot;

extern u8 KorosseoKawa_SceneTableA[];
extern u8 KorosseoKawa_SceneTableB[];
extern u8 KorosseoKawa_SceneTableC[];
extern u8 LinkedMessage_WouldYouLikeHearDescription;
extern volatile u32 Data_03001ae8;
extern u8 HexDigits[];
void Map_UpdateCellRect();
void Object_SetMoveTarget();
void Script_WaitForEventTimeout();
typedef void(*SceneTask)(void);
void Scheduler_RemoveCallbackFar(SceneTask);
s32 Map_GetTerrainHeightFar(s32, s32, s32);
void GameFlag_SetByte(s32, s32);
void Object_SetMoveTarget(struct FieldActor *, s32, s32, s32);
void Script_WaitForEventTimeout(struct FieldActor *);
PartyInteractionRecord *GetPartyInteractionRecord(void);
s32 GetPartyMemberCount(void);
Rec *Owner_GetState(s32);
s32 SceneDialogue_RunFlagGatedPromptInteraction(s32 a, s32 b);
void FieldScene_RunMiddleSequence(s32 mode, s32 owner, s32 base);
void SceneState_StoreParamsAndInitTable(s32 a, s32 b, s32 c);

static inline void InitializeActorZero(void)
{
    Engine_ActorSetSpeed(ACTOR_PARTY_LEADER, 0x10000, 0x8000);
}

static inline void InitializeSelectedActor(s32 actorId)
{
    Engine_ActorSetSpeed(actorId, 0x10000, 0x8000);
}

/* Selects a later line in the current dialogue. */
static __inline__ void AdvanceMessage(s32 amount)
{
    gEventWork->message += amount;
}

void StagedActor_PushActorAhead(void);
void ObjectDispatch_InitFromTable4WithArgument(s32 table, struct FieldActor *object);
void Resource3ba_NoOpCallback(void);
s32 FieldScene_RunFlag211ApproachScene();
void SceneState_WaitUntilWord1000IsNine(void);
void Object_RefreshSelectorById(s32 actor);
s32 BattleFx_SetWeightedResult();
s32 Party_SetFields1ceAnd1d0();
void Event_SetPair1d4(s32 scene, s32 entrance);
extern u8 KorosseoKawa_SceneTableD[];
void Owner_RefreshActiveRatios();
void FieldScene_RunTwoCallSequence(void);
void FieldScene_RunLateSequence(s32 a0);
s32 Map_GetTerrainHeightFar(s32 layer, s32 x, s32 z);
s32 GameFlag_GetByteFar(s32 flag);
void Map_UpdateCellRect(s32 a0, s32 a1, s32 a2, s32 a3, s32 a4, s32 a5);
s32 Korosseo_ShowItemIcon(s32 slot, s32 item);
s32 FieldScene_BuildSceneDescriptorAndInstallTask(s32 a0, s32 a1, s32 a2, s32 a3, s32 a4, s32 a5, s32 a6);
void Korosseo_SelectSoloCompetitor(s32 index);
void SceneActor_PlaceSlots1To3FromWork(void);
void FieldScene_RunTwoArmSequence(s32 arm);
void FieldScene_RunCommandSequence(s32 actor);
void FieldScene_RunSixSteps380To3A8(void);
void FieldScene_RunScene3ba_02000974(s32 direction);
void SceneState_InitControlWhenFlag109Clear(s32 value);
void Object_LinkObjectAndSetCallback(s32 actor, s32 leader);
void FieldScene_RunOpeningAuxiliarySequence(void);
void SceneState_SetStateHalfword386To99WhenMatched(void);


extern u8 MsgKorosseoAskAttendantsForExplanationsStages[];
extern u8 MsgKorosseoRobinYoureContestantInFinals[];
extern u8 MsgKorosseoSiteFirstFinalsBattle[];
extern u8 MsgKorosseoWarriorsEnterFinalsWithoutAny[];
extern u8 MsgKorosseoCallRockChallenge[];
extern u8 MsgKorosseoObjectiveStageClear[];
void Korosseo_FinishSoloRound();
void Engine_EventBegin();
s32 SceneDialogue_RunFlagGatedPromptInteraction();
void Engine_EventSetMessage();
void Engine_CameraSetSpeed();
void Engine_CameraMoveTo();
void Engine_CameraWaitForMove();
void Engine_EventShowMessage();
void Korosseo_FadeInCompetitor(s32 actor, s32 x, s32 z);
void Engine_ActorSetSpeed();
void Engine_ActorWalkToAndWait();
void Engine_EventWait();
void Engine_ActorShowEmote();
void Engine_ActorFaceDirection();
s32 OverlayObject_PlaceWithScale14000();
void StagedActor_PushActorAhead();
void OverlayObject_ResetMotionFields();
void Engine_ActorSetAnimation();
void Korosseo_RestoreCompetitor();
void Engine_CameraFollowActor();
void Engine_ActorSetPosition();
void SceneState_SendIdBySceneId();
void Engine_EventEnd();
extern u8 MsgKorosseoAreaCalledPipeworks[];
extern u8 MsgKorosseoObjectiveMakeGood[];
void SceneState_StoreParamsAndInitTable();
void SceneState_InitTableWordsAndLoad3200();
void SceneState_ReleaseTableAndResetC6a6();
void Engine_ObjectSetPosition();

extern u8 MsgKorosseoLogsKeyClearingStage[];
extern u8 MsgKorosseoOperatorBridgeWillAlsoCheer[];
extern u8 MsgKorosseoPlaceNormallyCalledLumberWater[];
extern u8 MsgKorosseoTheyCallBrokenBridge[];
s32 GameFlag_GetByteFar(s32);
void SceneState_ResetCounterAndStartTask(void);
void SceneState_SetMode66AndPassOpeningSequence(void);
void SceneState_WaitUntilWordC41cIs22(void);
void StagedActor_PlacePairAtOffsetAndRun(s32 actor_id, s32 dx, s32 dz);
void SceneState_SendIdBySceneId(s32 a, s32 b);
void SceneState_ReleaseTableAndResetC6a6(void);

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

/* Contiguous unnamed leaf-owner run for resource_3ba. */
u8 *SceneData_GetTableC194(void)
{

    return KorosseoKawa_SceneTableA;
}

s32 get_default_result(void)
{
    return 0;
}

u8 *SceneData_GetTablec1dc(void)
{

    return KorosseoKawa_SceneTableB;
}

u8 *SceneData_GetTablec1f4(void)
{

    return KorosseoKawa_SceneTableC;
}

/* Runs one branch of a scripted auxiliary sequence selected by the current
 * countdown value, then advances (or, from 0, restarts) the countdown. */
void FieldScene_RunOpeningAuxiliarySequence(void)
{
    extern s32 KorosseoKawa_Countdown;

    switch ((u32)KorosseoKawa_Countdown) {
    case 66:
        Map_UpdateCellRect(92, 31, 2, 2, 50, 38); /* main:080091c8 */
        Map_UpdateCellRect(92, 31, 2, 2, 54, 38); /* main:080091c8 */
        Engine_ActorSetAnimation(16, 10); /* object 16, action 10 */
        break;
    case 60:
        Map_UpdateCellRect(92, 33, 2, 2, 50, 38); /* main:080091c8 */
        Map_UpdateCellRect(92, 33, 2, 2, 54, 38); /* main:080091c8 */
        Engine_MapCopyCellAttributes(50, 25, 6, 1, 50, 12); /* main:080091c0 */
        Engine_ActorSetAnimation(16, 11); /* object 16, action 11 */
        break;
    case 6:
        Map_UpdateCellRect(92, 31, 2, 2, 50, 38); /* main:080091c8 */
        Map_UpdateCellRect(92, 31, 2, 2, 54, 38); /* main:080091c8 */
        Engine_ActorSetAnimation(16, 10); /* object 16, action 10 */
        break;
    case 0:
        Map_UpdateCellRect(92, 29, 2, 2, 50, 38); /* main:080091c8 */
        Map_UpdateCellRect(92, 29, 2, 2, 54, 38); /* main:080091c8 */
        Engine_ActorSetAnimation(16, 12); /* object 16, action 12 */
        Engine_MapCopyCellAttributes(50, 24, 6, 1, 50, 12); /* main:080091c0 */
        KorosseoKawa_Countdown = 120;
        break;
    }
    KorosseoKawa_Countdown = KorosseoKawa_Countdown - 1;
}

void SceneState_ResetCounterAndStartTask(void)
{
    extern s32 KorosseoKawa_Countdown;

    SceneTask task;

    KorosseoKawa_Countdown = 0;
    task = (SceneTask)FieldScene_RunOpeningAuxiliarySequence;
    Scheduler_RemoveCallbackFar(task);
    task();
}

void SceneState_SetMode66AndPassOpeningSequence(void)
{
    extern s32 KorosseoKawa_Countdown;

    s32 value = 66;
    s32 *mode = (s32 *)&KorosseoKawa_Countdown;

    *mode = value;
    Scheduler_AddOrUpdateCallback((s32)FieldScene_RunOpeningAuxiliarySequence, 0xC80);
}

void SceneState_WaitUntilWordC41cIs22(void)
{
    extern u32 KorosseoKawa_Countdown;

    s32 i;

    Engine_TaskWait(10);
    i = 0;
    if (KorosseoKawa_Countdown != 22) {
        do {
            Engine_TaskWait(1);
            i++;
            if (i > 119) {
                break;
            }
        } while (KorosseoKawa_Countdown != 22);
    }
}

void SceneState_ApplyRectsForActorsNineAndTen(void)
{

    struct FieldActor *actor;
    s32 blocked;

    {
        s32 x = 23;
        s32 y = 12;

        Engine_MapCopyCellAttributes(27, 13, 3, 1, x, y);
    }
    actor = Object_GetById(9);
    blocked = Map_GetTerrainHeightFar(0, actor->x.fixed, actor->z.fixed);
    if (actor->y.fixed == 0 && blocked == 0) {
        actor->priority_flags = ACTOR_PRIORITY_UNDERFOOT;
        actor->motion_flags = 0;
        {
            s32 x = actor->x.fixed >> 20;
            s32 y = actor->z.fixed >> 20;

            Engine_MapCopyCellAttributes(14, 13, 1, 1, x, y);
        }
    }
    actor = Object_GetById(10);
    {
        s32 x = actor->x.fixed >> 20;

        GameFlag_SetByte(784, x);
    }
    {
        s32 x = actor->x.fixed >> 20;
        s32 y = actor->z.fixed >> 20;

        Engine_MapCopyCellAttributes(14, 13, 1, 1, x, y);
    }
}

void FieldScene_RunTwoCallSequence(void)
{

    StagedActor_PushActorAhead();
    SceneState_ApplyRectsForActorsNineAndTen();
}

void SceneState_ApplyRectAndSend303(void)
{
    extern s32 KorosseoKawa_Countdown;

    {
        s32 x = 47;
        s32 y = 12;

        Engine_MapCopyCellAttributes(47, 24, 1, 1, x, y);
    }
    Engine_GameFlagSet(0x303);
}

void FieldScene_RunScene3ba_02000270(void)
{
    extern u8 KorosseoKawa_Countdown[];

    u32 i;
    u8 *rec7;
    s32 record;
    s32 none;

    GameFlag_Set(0x301);
    rec7 = Object_GetById(13);
    Engine_EventBegin();
    Camera_SetSpeed(0x20000, 0x4000);
    Camera_MoveTo(0x2580000, -1, 0xc80000, 1);
    Object_SetMode((s32)rec7, 3);
    Engine_CameraWaitForMove();
    none = 0;
    rec7[85] = none;
    *(s32 *)((s32)rec7 + 52) = 0x6666;
    *(s32 *)((s32)rec7 + 48) = 0xcccc;
    Call4(Object_SetMoveTarget, (s32)rec7, *(s32 *)((s32)rec7 + 8), 0x80000, *(s32 *)((s32)rec7 + 16));
    rec7 = Value1(Object_GetById, 14);
    rec7[85] = none;
    *(s32 *)((s32)rec7 + 52) = 0x6666;
    *(s32 *)((s32)rec7 + 48) = 0xcccc;
    Object_SetMoveTarget((s32)rec7, *(s32 *)((s32)rec7 + 8), 0x200000, *(s32 *)((s32)rec7 + 16));
    Script_WaitForEventTimeout((s32)rec7);
    Engine_EventWait(45);
    Map_CopyCellAttributes(43, 12, 1, 1, 41, 12);
    Engine_EventEnd();
}

void StagedActor_PlacePairAtOffsetAndRun(s32 actor_id, s32 dx, s32 dz)
{
    struct FieldActor *leader;
    struct FieldActor *actor;
    s32 x;
    s32 z;

    leader = Object_GetById(gGameState.selected_actor);
    actor = Object_GetById(actor_id);
    Engine_EventBegin();
    {
        x = ((leader->x.fixed + (dx << 16)) & 0xFFF00000) + 0x80000;
        z = ((leader->z.fixed + (dz << 16)) & 0xFFF00000) + 0x80000;

        leader->speed = 0x10000;
        leader->acceleration = 0x8000;
        Object_SetMoveTarget(leader, x, leader->y.fixed, z);
    }
    Object_SetMode(leader, 27);
    {
        x = ((actor->x.fixed + (dx << 16)) & 0xFFF00000) + 0x80000;
        z = ((actor->z.fixed + (dz << 16)) & 0xFFF00000) + 0x80000;

        actor->speed = 0x10000;
        actor->acceleration = 0x8000;
        Object_SetMoveTarget(actor, x, actor->y.fixed, z);
    }
    if (dx < 0 || dz < 0) {
        Object_SetMode(actor, 4);
    } else {
        Object_SetMode(actor, 3);
    }
    Engine_AudioPlayCue(226);
    Script_WaitForEventTimeout(leader);
    Object_SetMode(actor, 2);
    Engine_AudioPlayCue(288);
    Engine_EventEnd();
}

void SceneActor_ShiftActorSeventeenByLeaderRow(void)
{
    struct FieldActor *actor;
    s32 row;
    s32 offset;

    actor = Object_GetById(gGameState.selected_actor);
    row = actor->z.fixed >> 20;
    offset = -48;
    if (row <= 8) {
        offset = 48;
    }
    Engine_MapCopyCellAttributes(67, 8, 3, 1, 64, row);
    StagedActor_PlacePairAtOffsetAndRun(17, 0, offset);
    actor = Object_GetById(17);
    row = actor->z.fixed >> 20;
    Engine_MapCopyCellAttributes(64, 24, 3, 1, 64, row);
}

void SceneActor_ShiftActorEighteenByInputAndLeaderColumn(void)
{
    struct FieldActor *actor;
    s32 column;
    s32 offset;
    s32 direction;

    actor = Object_GetById(gGameState.selected_actor);
    column = actor->x.fixed >> 20;
    if ((Data_03001ae8 & 32) != 0) {
        direction = -1;
    }
    if ((Data_03001ae8 & 16) != 0) {
        direction = 1;
    }
    actor = Object_GetById(17);
    offset = actor->z.fixed >> 20;
    if (column == 63) {
        if (offset == 11) {
            return;
        }
        offset = 160;
    } else if (column == 67) {
        if (offset == 11 && direction == -1) {
            return;
        }
        offset = 96;
    } else {
        if (offset == 11) {
            offset = 96;
        } else {
            offset = 160;
        }
        offset = -offset;
    }
    Engine_MapCopyCellAttributes(72, 9, 1, 3, column, 9);
    StagedActor_PlacePairAtOffsetAndRun(18, offset, 0);
    actor = Object_GetById(18);
    column = actor->x.fixed >> 20;
    Engine_MapCopyCellAttributes(63, 25, 1, 3, column, 9);
}

void KorosseoKawa_RaisePipes(void)
{
    s32 leader_id;
    struct FieldActor *leader;
    struct FieldActor *actor;
    s32 none;

    leader_id = gGameState.selected_actor;
    leader = Object_GetById(leader_id);
    actor = Object_GetById(12);
    GameFlag_Set(0x302);
    Engine_EventBegin();
    Engine_ActorSetAnimation(leader_id, 8);
    Engine_EventWait(6);
    actor->speed = 0x8000;
    actor->acceleration = 0x3333;
    Audio_PlayCue(239);
    Object_SetMode(actor, 2);
    Object_SetMoveTarget(actor, actor->x.fixed - 0x300000, 0, actor->z.fixed);
    Engine_EventWait(6);
    Engine_ActorSetAnimation(leader_id, 2);
    ObjectDispatch_InitFromTable4WithArgument(*(s32 *)(Runtime_AllocateBlock(27, 0xccc) + 480), actor);
    Actor_SetSpeed(leader_id, 0x4ccc, 0x3333);
    Object_SetMoveTarget(leader, leader->x.fixed - 0x180000, 0, leader->z.fixed);
    Engine_ActorWaitForMove(leader_id);
    Engine_ActorSetAnimation(leader_id, 1);
    Script_WaitForEventTimeout(actor);
    Object_SetMode(actor, 1);
    Audio_PlayCue(288);
    Audio_PlayCue(213);
    Engine_EventWait(15);
    Engine_EventEnd();
    Map_CopyCellAttributes(37, 7, 1, 4, 34, 7);
    Map_CopyCellAttributes(36, 7, 1, 4, 37, 7);
    none = GameFlag_IsSet(0x301);
    if (none != 0) {
        Engine_EventBegin();
        Camera_SetSpeed(0x20000, 0x4000);
        Camera_MoveTo(0x2280000, -1, 0xc80000, 1);
        Engine_CameraWaitForMove();
        Map_CopyCells(96, 29, 1, 3, 34, 38);
        Engine_EventWait(3);
        Map_CopyCells(97, 29, 1, 3, 34, 38);
        Engine_EventWait(3);
        Map_CopyCells(98, 29, 1, 3, 34, 38);
        Engine_EventWait(3);
        Map_CopyCells(99, 29, 1, 3, 34, 38);
        Engine_EventWait(3);
        Map_CopyCells(100, 29, 1, 3, 34, 38);
        Engine_EventWait(15);
        Engine_EventEnd();
        return;
    }
    GameFlag_Set(0x301);
    Engine_EventBegin();
    Camera_SetSpeed(0x20000, 0x4000);
    Camera_MoveTo(0x2580000, -1, 0xc80000, 1);
    Engine_CameraWaitForMove();
    actor = Object_GetById(13);
    actor->motion_flags = none;
    actor->acceleration = 0x6666;
    actor->speed = 0xcccc;
    Object_SetMoveTarget(actor, actor->x.fixed, 0x80000, actor->z.fixed);
    Object_SetMode(actor, 3);
    Map_CopyCells(96, 29, 1, 3, 34, 38);
    Engine_EventWait(3);
    Map_CopyCells(97, 29, 1, 3, 34, 38);
    Engine_EventWait(3);
    Map_CopyCells(98, 29, 1, 3, 34, 38);
    Engine_EventWait(3);
    Map_CopyCells(99, 29, 1, 3, 34, 38);
    Engine_EventWait(3);
    Map_CopyCells(100, 29, 1, 3, 34, 38);
    actor = Object_GetById(14);
    actor->motion_flags = none;
    actor->acceleration = 0x6666;
    actor->speed = 0xcccc;
    Object_SetMoveTarget(actor, actor->x.fixed, 0x200000, actor->z.fixed);
    Script_WaitForEventTimeout(actor);
    Engine_EventWait(15);
    Engine_EventEnd();
    Map_CopyCellAttributes(43, 12, 1, 1, 41, 12);
}

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
void SceneState_ApplyValue768(void)
{

    Engine_GameFlagSet(768);
}

/* The river stage's start: after the approach scene and once the stage is
   ready, actor 8 and the leader walk to the start line and face each
   other. The approach's outcome goes to the weighted result, the river's
   scene is stored with entrance 4 and 5 in the two scene pairs, the stage
   byte at 0x22b becomes 3 and flag 0x11a is set. */
void KorosseoKawa_RunStageStart(void)
{
    s32 approach;
    s32 n;
    s32 base;
    u8 *state;

    base = 0;
    Resource3ba_NoOpCallback();
    Engine_EventBegin();
    approach = FieldScene_RunFlag211ApproachScene(120, 127);
    SceneState_WaitUntilWord1000IsNine();
    n = 9;
    do {
        Object_RefreshSelectorById(8);
        n = n - 1;
    } while (n >= 0);
    Actor_SetSpeed(8, 0x10000, 0x8000);
    Actor_WalkTo(8, 0x528, 192);
    Actor_SetSpeed(ACTOR_PARTY_LEADER, 0x10000, 0x8000);
    Actor_WalkToAndWait(ACTOR_PARTY_LEADER, 0x508, 192);
    Engine_ActorSetAnimation(8, 1);
    Engine_ActorFaceEachOther(0, 8, 0);
    Engine_EventWait(10);
    Engine_ActorSetAnimation(8, 3);
    Engine_ActorSetAnimationAndWait(0, 3);
    Engine_EventWait(20);
    Actor_SetSpeed(ACTOR_PARTY_LEADER, 0x20000, 0x10000);
    Actor_SetSpeed(8, 0x20000, 0x10000);
    Actor_WalkTo(ACTOR_PARTY_LEADER, 0x510, 192);
    Actor_WalkToAndWait(8, 0x520, 192);
    Engine_ActorSetAnimation(0, 16);
    Engine_ActorSetAnimation(8, 9);
    Engine_EventWait(10);
    /* FAKEMATCH: base is 0 from the top of the function, so 0 - approach
     * + 1 is not folded into 1 - approach. */
    BattleFx_SetWeightedResult(72, base - approach + 1);
    state = (u8 *)&gGameState;
    /* FAKEMATCH: the do-while keeps the stage flag store ahead of the
     * scene's pool load. */
    do {
        state[0x22b] = 3;
    } while (0);
    Party_SetFields1ceAnd1d0((s32)&SceneId_KorosseoKawa, 4);
    Event_SetPair1d4((s32)&SceneId_KorosseoKawa, 5);
    GameFlag_Set(0x11a);
}


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
void FieldScene_RunBranchedStep(void)
{

    if (SceneActor_FindOccupantAheadOfSubject() == 0) {
        Engine_LeaderCheckAhead();
    } else {
        FieldScene_RunTwoCallSequence();
    }
}

u8 *SceneData_GetTablec420(void)
{

    return KorosseoKawa_SceneTableD;
}

void FieldScene_RunScene3ba_02000974(s32 a0)
{

    u32 i;
    s32 record;

    Engine_ActorDestroy(24);
    Engine_ActorDestroy(25);
    Owner_RefreshActiveRatios(1);
    Engine_EventBegin();
    Actor_SetPosition(8, 0x5280000, 0xc00000);
    Actor_SetPosition(ACTOR_PARTY_LEADER, 0x5080000, 0xc00000);
    Actor_FaceActor(8, 0x4000, 0);
    Actor_FaceActor(ACTOR_PARTY_LEADER, 0x4000, 0);
    if (a0 < 0) {
        Engine_ActorSetAnimation(8, 10);
        Engine_ActorSetAnimation(ACTOR_PARTY_LEADER, 35);
    } else {
        Engine_ActorSetAnimation(8, 8);
        Engine_ActorSetAnimation(ACTOR_PARTY_LEADER, 28);
    }
    Engine_TaskWait(1);
    Camera_MoveTo(0x5180000, 0, 0x800000, 0);
    FieldScene_RunLateSequence(a0);
    Engine_EventEnd();
}

void SceneActor_MarkObjectAtTiles94To95(s32 actor)
{
    struct FieldActor *o;
    s32 x;
    s32 y;

    o = Object_GetById(actor);
    if (o != 0) {
        x = o->x.fixed >> 19;
        y = o->z.fixed >> 19;
        if (x >= 94 && x <= 95 && y > 23 && y <= 26) {
            o->unknown_22 = 1;
        }
    }
}

/* River arena entry: record the arrival, set the logs, ledges and item icons by the story flags, then start the entrance's opening scene. */
s32 KorosseoKawa_ApplyEntryState(void)
{
    /* FAKEMATCH: the halfword row view retains the base-plus-index
     * entrance address form also used by the wall stage. */
    union GameStateRows {
        u8 bytes[512][2];
        s16 halves[512][1];
    };
    struct FieldActor *actor;
    s32 hit;
    s32 x;
    s32 z;
    s32 zero;
    s32 col;

    gEventWork->start_transition = 0;
    GameFlag_Set(0x144);
    actor = Object_GetById(9);
    hit = Map_GetTerrainHeightFar(0, actor->x.fixed, actor->z.fixed);
    if (actor->y.fixed == 0 && hit == 0) {
        actor->priority_flags = 2;
        actor->motion_flags = hit;
        Map_CopyCellAttributes(14, 13, 1, 1, actor->x.fixed >> 20, actor->z.fixed >> 20);
    }
    x = GameFlag_GetByteFar(0x310);
    if (x == 0) {
        x = 25;
    }
    actor = Object_GetById(10);
    actor->x.fixed = (x << 20) + 0x80000;
    actor->motion_flags = 0;
    actor->priority_flags = 2;
    Map_CopyCellAttributes(14, 13, 1, 1, x, 12);
    ((s32 (*)(void ( *)(void), s32))Scheduler_AddOrUpdateCallback)(FieldScene_RunOpeningAuxiliarySequence, 0xc80);
    actor = Object_GetById(15);
    actor->unknown_22 = 1;
    zero = 0;
    if (Engine_GameFlagIsSet(0x303)) {
        Object_SetMode(actor, 4);
        Engine_ActorSetSpriteFlags(actor, 0);
        actor->collision_flags = zero;
        actor->priority_flags = 3;
        Map_CopyCellAttributes(47, 24, 1, 1, 47, 12);
    }
    actor = Object_GetById(17);
    col = actor->z.fixed >> 20;
    actor->motion_flags = zero;
    actor->priority_flags = 2;
    Map_CopyCellAttributes(64, 24, 3, 1, 64, col);
    actor = Object_GetById(18);
    col = actor->x.fixed >> 20;
    actor->motion_flags = zero;
    actor->priority_flags = 2;
    Map_CopyCellAttributes(63, 25, 1, 3, col, 9);
    if (Engine_GameFlagIsSet(0x302)) {
        Map_CopyCellAttributes(37, 7, 1, 4, 34, 7);
        Map_CopyCellAttributes(36, 7, 1, 4, 37, 7);
        Map_CopyCells(100, 29, 1, 3, 34, 38);
    }
    actor = Object_GetById(13);
    if (Engine_GameFlagIsSet(0x301)) {
        Map_CopyCellAttributes(43, 12, 1, 1, 41, 12);
        actor->motion_flags = zero;
        actor->acceleration = 0x6666;
        actor->speed = 0xcccc;
        actor->y.fixed = 0x80000;
        Object_SetMode(actor, 3);
    } else {
        Object_SetMode(actor, 2);
    }
    Object_GetById(14)->priority_flags = 2;
    Korosseo_ShowItemIcon(24, 120);
    Korosseo_ShowItemIcon(25, 127);
    switch (((union GameStateRows *)&gGameState)->halves[225][0]) {
    case 1:
        FieldScene_BuildSceneDescriptorAndInstallTask(0, 8, 4, 0x5180000, 0xc00000, 24, 25);
        Call6(Map_UpdateCellRect, 127, 0, 1, 2, 19, 2);
        Engine_ActorDestroy(19);
        Engine_ActorDestroy(20);
        Engine_ActorDestroy(21);
        Engine_ActorDestroy(22);
        Engine_ActorDestroy(23);
        if (!Engine_GameFlagIsSet(0x109)) {
            Engine_AudioPlayCue(17);
            Korosseo_SelectSoloCompetitor(0);
            SceneActor_PlaceSlots1To3FromWork();
            SceneActor_MarkObjectAtTiles94To95(1);
            SceneActor_MarkObjectAtTiles94To95(2);
            SceneActor_MarkObjectAtTiles94To95(3);
            FieldScene_RunTwoArmSequence(1);
        }
        Object_LinkObjectAndSetCallback(1, 0);
        Object_LinkObjectAndSetCallback(2, 0);
        Object_LinkObjectAndSetCallback(3, 0);
        SceneState_InitControlWhenFlag109Clear((s32)&ResourceId_RivalPathA);
        break;
    case 2:
        ((s32 (*)(void ( *)(void), s32))Scheduler_AddOrUpdateCallback)(SceneState_SetStateHalfword386To99WhenMatched, 0xc80);
        Engine_ActorDestroy(24);
        Engine_ActorDestroy(25);
        if (!Engine_GameFlagIsSet(0x109)) {
            SceneActor_PlaceSlots1To3FromWork();
            Korosseo_SelectSoloCompetitor(1);
            FieldScene_RunTwoArmSequence(0);
        }
        break;
    case 3:
        if (!Engine_GameFlagIsSet(0x109)) {
            FieldScene_RunCommandSequence(19);
            FieldScene_RunSixSteps380To3A8();
        }
        break;
    case 4:
        FieldScene_RunScene3ba_02000974(1);
        Engine_EventRequestExit(4);
        break;
    case 5:
        FieldScene_RunScene3ba_02000974(-1);
        Engine_EventRequestExit(5);
        break;
    }
    return 0;
}

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
void FieldScene_RunCommandSequence(s32 a0)
{
    struct FieldActor *actor;
    s32 x;
    s32 z;

    actor = Object_GetById(a0);
    x = actor->x.part.pixel;
    z = actor->z.part.pixel;
    Engine_EventBegin();
    Actor_SetSpeed(a0, 0x10000, 0x8000);
    Actor_SetSpeed(ACTOR_PARTY_LEADER, 0x10000, 0x8000);
    Actor_SetSpeed(ACTOR_GERALD, 0x10000, 0x8000);
    Actor_SetSpeed(ACTOR_IVAN, 0x10000, 0x8000);
    Actor_SetSpeed(ACTOR_MIA, 0x10000, 0x8000);
    Engine_ActorSetPosition(0, x << 16, (z << 16) - 0x300000);
    Actor_SetPosition(ACTOR_GERALD, (x << 16) - 0x100000, (z << 16) - 0x280000);
    Actor_SetPosition(ACTOR_IVAN, (x << 16) + 0x100000, (z << 16) - 0x280000);
    Actor_SetPosition(ACTOR_MIA, x << 16, (z << 16) - 0x200000);
    Actor_SetPosition(a0, x << 16, (z << 16) - 0x500000);
    actor = (struct FieldActor *)Object_GetById(0);
    actor->facing = 0xc000;
    Engine_CameraFollowActor(ACTOR_PARTY_LEADER, 0);
    Engine_EventOpenScreen();
    Engine_EventWaitForScreen();
    Engine_EventSetMessage((s32)MsgKorosseoSiteFirstFinalsBattle);
    Event_ShowMessage(a0, 0);
    Actor_ShowEmote(ACTOR_MIA, 0x101, 60);
    Event_ShowMessage(ACTOR_MIA, 0);
    Engine_ActorStartRepeatedMotion(a0, 3);
    Event_ShowMessage(a0, 0);
    Actor_ShowEmote(ACTOR_IVAN, 0x101, 60);
    Event_ShowMessage(ACTOR_IVAN, 0);
    Actor_FaceActor(a0, ACTOR_IVAN, 0);
    Engine_EventWait(20);
    Engine_ActorSetAnimationAndWait(a0, 3);
    Event_ShowMessage(a0, 0);
    Actor_ShowEmote(ACTOR_GERALD, 0x101, 60);
    Event_ShowMessage(ACTOR_GERALD, 0);
    Actor_ShowEmote(ACTOR_MIA, 0x101, 60);
    Event_ShowMessage(ACTOR_MIA, 0);
    Actor_ShowEmote(a0, 0x102, 60);
    if (Event_AskYesNo(a0, 0) == 0) {
        do {
            Engine_EventSetMessage((s32)MsgKorosseoWarriorsEnterFinalsWithoutAny);
            Engine_ActorSetAnimation(ACTOR_IVAN, 3);
            Engine_EventWait(2);
            Engine_ActorSetAnimation(ACTOR_GERALD, 3);
            Engine_EventWait(2);
            Engine_ActorSetAnimation(ACTOR_MIA, 3);
            Engine_EventWait(1);
            Engine_ActorSetAnimationAndWait(ACTOR_PARTY_LEADER, 3);
            Engine_ActorSetAnimationAndWait(a0, 3);
            Event_ShowMessage(a0, 0);
            Actor_FaceDirection(a0, 0xa000, 0);
            Engine_EventWait(20);
            Event_ShowMessage(a0, 0);
            Camera_SetSpeed(0x30000, 0x6000);
            Camera_MoveTo(0x1380000, -1, 0x680000, 1);
            Engine_CameraWaitForMove();
            Event_ShowMessage(a0, 0);
            Camera_SetSpeed(0x18000, 0x3000);
            Camera_MoveTo(0x3080000, -1, 0x680000, 1);
            Event_ShowMessage(a0, 0);
            Engine_CameraWaitForMove();
            Event_ShowMessage(a0, 0);
            Camera_SetSpeed(0x30000, 0x6000);
            Camera_MoveTo(0x4d80000, -1, 0xa80000, 1);
            Engine_CameraWaitForMove();
            Actor_FaceActor(a0, 0x6000, 0);
            Event_ShowMessage(a0, 0);
            Camera_MoveTo(0x5180000, -1, 0xa80000, 1);
            Engine_CameraWaitForMove();
            Actor_FaceActor(a0, ACTOR_PARTY_LEADER, 0);
            Event_ShowMessage(a0, 0);
            Event_ShowMessage(a0, 0);
            Event_ShowMessage(a0, 0);
            Engine_CameraFollowActor(ACTOR_PARTY_LEADER, 0);
            Engine_ActorRunRepeatedMotion(a0, 2);
        } while (Event_AskYesNo(a0, 0) != 0);
        Engine_ActorRunRepeatedMotion(a0, 2);
        Engine_EventSetMessage((s32)MsgKorosseoAskAttendantsForExplanationsStages);
        Event_ShowMessage(a0, 0);
    }
    Engine_EventSetMessage((s32)MsgKorosseoRobinYoureContestantInFinals);
    Engine_ActorRunRepeatedMotion(a0, 2);
    Event_ShowMessage(a0, 0);
    Actor_FaceDirection(ACTOR_PARTY_LEADER, 0x4000, 0);
    Actor_FaceDirection(ACTOR_GERALD, 0, 0);
    Actor_FaceDirection(ACTOR_IVAN, 0x8000, 0);
    Engine_ActorRunRepeatedMotion(ACTOR_GERALD, 2);
    Event_ShowMessage(ACTOR_GERALD, 0);
    Engine_ActorRunRepeatedMotion(ACTOR_IVAN, 2);
    Event_ShowMessage(ACTOR_IVAN, 0);
    Engine_ActorSetAnimationAndWait(ACTOR_MIA, 3);
    Event_ShowMessage(ACTOR_MIA, 0);
    Engine_ActorSetAnimation(ACTOR_MIA, 3);
    Engine_EventWait(1);
    Engine_ActorSetAnimation(ACTOR_GERALD, 3);
    Engine_EventWait(2);
    Engine_ActorSetAnimation(ACTOR_IVAN, 3);
    Engine_EventWait(1);
    Engine_ActorSetAnimationAndWait(ACTOR_PARTY_LEADER, 3);
    Engine_EventWait(6);
    Engine_ActorSetAnimation(ACTOR_GERALD, 2);
    actor = (struct FieldActor *)Object_GetById(0);
    if (actor != 0) {
        Actor_SetDestination(ACTOR_GERALD, actor->x.part.pixel, actor->z.part.pixel);
    }
    Engine_ActorSetAnimation(ACTOR_IVAN, 2);
    actor = (struct FieldActor *)Object_GetById(0);
    if (actor != 0) {
        Actor_SetDestination(ACTOR_IVAN, actor->x.part.pixel, actor->z.part.pixel);
    }
    Engine_ActorSetAnimation(ACTOR_MIA, 2);
    actor = (struct FieldActor *)Object_GetById(0);
    if (actor != 0) {
        Actor_SetDestination(ACTOR_MIA, actor->x.part.pixel, actor->z.part.pixel);
    }
    Actor_WalkToAndWait(a0, x - 16, z - 64);
    Actor_SetPosition(ACTOR_GERALD, 0, 0);
    Actor_SetPosition(ACTOR_IVAN, 0, 0);
    Actor_SetPosition(ACTOR_MIA, 0, 0);
    Actor_WalkToAndWait(a0, x - 16, z - 16);
    Actor_WalkToAndWait(a0, x, z);
    Actor_FaceDirection(a0, 0xc000, 10);
    Engine_EventEnd();
}

/* Colosso river stage: unless the stage is already cleared, walk the player
 * through the introduction, push the pillars and hand over to the stage. */
void KorosseoKawa_RunStageIntro(s32 a0)
{
    s32 rec;
    s32 record;

    if (gGameState.entrance == 2) {
        Korosseo_FinishSoloRound();
    } else {
        Engine_EventBegin();
        rec = SceneDialogue_RunFlagGatedPromptInteraction(a0, 1);
        if (rec != 0) {
        } else {
            Engine_EventSetMessage((s32)MsgKorosseoCallRockChallenge);
            Camera_SetSpeed(0x30000, 0x6000);
            Camera_MoveTo(0x1480000, -1, 0xa80000, 1);
            Engine_CameraWaitForMove();
            Engine_EventShowMessage(a0, 0);
            {
                /* FAKEMATCH: native coordinates in r1/r2 precede r0 setup. */
                register s32 fx asm("r1") = 0x118;
                /* FAKEMATCH: native coordinates in r1/r2 precede r0 setup. */
                register s32 fz asm("r2") = 200;
                /* FAKEMATCH: direct and Call3 void forms move r0 ahead of coordinates. */
                asm("" : : "r"(fx), "r"(fz) : "r0");
                Korosseo_FadeInCompetitor(0, fx, fz);
            }
            Actor_SetSpeed(0, 0x10000, 0x8000);
            Actor_WalkToAndWait(0, 0x168, 200);
            Engine_EventWait(30);
            Actor_ShowEmote(0, 0x102, 60);
            Engine_EventShowMessage(a0, 0);
            Actor_WalkToAndWait(0, 0x138, 200);
            Engine_EventWait(30);
            Actor_FaceDirection(0, 0xc000, 10);
            Actor_ShowEmote(0, 0x106, 60);
            Actor_SetSpeed(0, 0x18000, 0xc000);
            OverlayObject_PlaceWithScale14000(0, 0x128, 184);
            OverlayObject_PlaceWithScale14000(0, 0x128, 152);
            OverlayObject_PlaceWithScale14000(0, 0x138, 152);
            Actor_FaceDirection(0, 0x4000, 15);
            StagedActor_PushActorAhead();
            OverlayObject_ResetMotionFields(0);
            StagedActor_PushActorAhead();
            OverlayObject_ResetMotionFields(0);
            Actor_SetSpeed(0, 0x18000, 0xc000);
            Actor_WalkToAndWait(0, 0x130, 184);
            Engine_ActorWalkToAndWait(0, 0x128, 192);
            Engine_ActorWalkToAndWait(0, 0x128, 200);
            Engine_ActorFaceDirection(0, 0, 15);
            StagedActor_PushActorAhead();
            OverlayObject_ResetMotionFields(0);
            StagedActor_PushActorAhead();
            OverlayObject_ResetMotionFields(0);
            Engine_ActorSetAnimation(0, 1);
            Engine_EventShowMessage(a0, 0);
            Korosseo_RestoreCompetitor(0);
            Engine_CameraFollowActor(0, 0);
            Actor_SetPosition(9, 0x1380000, 0xa80000);
            SceneState_SendIdBySceneId(a0, 1);
            goto L_020013c2;
        }
        if (rec == 1) {
            Engine_EventSetMessage((s32)MsgKorosseoObjectiveStageClear);
            Engine_EventShowMessage(a0, 0);
        }
        L_020013c2:;
        ((s32 (*)())FieldScene_RunMiddleSequence)(rec, a0, 1);
        Engine_EventEnd();
    }
}

/* The Pipeworks stage: actor a0 shows the course while actors 13 and 14 are
 * raised and lowered twice, then the stage opens. After a solo round the
 * question whether the party is done cheering comes instead. */
void Korosseo_RunPipeworksIntro(s32 a0)
{
    s32 rec4;
    u8 *rec7;
    u8 *record;

    if (gGameState.entrance == 2) {
        Korosseo_FinishSoloRound();
    } else {
        Engine_EventBegin();
        rec4 = SceneDialogue_RunFlagGatedPromptInteraction(a0, 2);
        if (rec4 == 0) {
            Engine_EventSetMessage((s32)MsgKorosseoAreaCalledPipeworks);
            Engine_CameraSetSpeed(0x30000, 0x6000);
            Call4((void (*)())Engine_CameraMoveTo, 0x2500000, -1, 0x780000, 1);
            Engine_CameraWaitForMove();
            Engine_EventWait(60);
            Engine_CameraSetSpeed(0x18000, 0x3000);
            Engine_CameraMoveTo(0x2600000, -1, 0xd80000, 1);
            Engine_CameraWaitForMove();
            Engine_EventShowMessage(a0, 0);
            SceneState_StoreParamsAndInitTable(56, 64, 0);
            Engine_EventWait(60);
            SceneState_InitTableWordsAndLoad3200(160, 96, 10);
            Engine_EventWait(70);
            Engine_EventShowMessage(a0, 0);
            SceneState_ReleaseTableAndResetC6a6();
            Engine_TaskWait(2);
            record = (u8 *)Object_GetById(13);
            record[85] = 0;
            *(s32 *)(record + 52) = 0x6666;
            *(s32 *)(record + 48) = 0xcccc;
            Engine_ObjectSetPosition((s32)record, *(s32 *)(record + 8), 0x80000, *(s32 *)(record + 16));
            rec7 = (u8 *)Object_GetById(14);
            rec7[85] = 0;
            *(s32 *)(rec7 + 52) = 0x6666;
            *(s32 *)(rec7 + 48) = 0xcccc;
            Engine_ObjectSetPosition((s32)rec7, *(s32 *)(rec7 + 8), 0x200000, *(s32 *)(rec7 + 16));
            Script_WaitForEventTimeout((s32)rec7);
            Engine_EventWait(45);
            record = (u8 *)Object_GetById(13);
            record[85] = 0;
            *(s32 *)(record + 52) = 0x6666;
            *(s32 *)(record + 48) = 0xcccc;
            Engine_ObjectSetPosition((s32)record, *(s32 *)(record + 8), 0x180000, *(s32 *)(record + 16));
            rec7 = (u8 *)Value1((s32 (*)())Object_GetById, 14);
            rec7[85] = 0;
            *(s32 *)(rec7 + 52) = 0x6666;
            *(s32 *)(rec7 + 48) = 0xcccc;
            Engine_ObjectSetPosition((struct FieldActor *)rec7, *(s32 *)(rec7 + 8), 0, *(s32 *)(rec7 + 16));
            Script_WaitForEventTimeout((s32)rec7);
            Engine_EventWait(15);
            Engine_EventShowMessage(a0, 0);
            SceneState_StoreParamsAndInitTable(56, 64, 0);
            Engine_EventWait(30);
            SceneState_InitTableWordsAndLoad3200(160, 96, 10);
            Engine_EventWait(40);
            SceneState_InitTableWordsAndLoad3200(56, 64, 10);
            Engine_EventWait(70);
            Engine_EventShowMessage(a0, 0);
            SceneState_ReleaseTableAndResetC6a6();
            Engine_TaskWait(2);
            Engine_CameraFollowActor(0, 0);
            SceneState_SendIdBySceneId(a0, 2);
        } else if (rec4 == 1) {
            Engine_EventSetMessage((s32)MsgKorosseoObjectiveMakeGood);
            Engine_EventShowMessage(a0, 0);
        }
        ((s32 (*)())FieldScene_RunMiddleSequence)(rec4, a0, 2);
        Engine_EventEnd();
    }
}

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
void FieldScene_RunScene3ba_020015e0(s32 a0)
{

    u32 i;
    s32 rec8;
    s32 record;

    if (gGameState.entrance == 2) {
        Korosseo_FinishSoloRound();
    } else {
        Engine_EventBegin();
        rec8 = SceneDialogue_RunFlagGatedPromptInteraction(a0, 3);
        if (rec8 == 0) {
            Engine_EventSetMessage((s32)MsgKorosseoTheyCallBrokenBridge);
            SceneState_ResetCounterAndStartTask();
            Camera_SetSpeed(0x30000, 0x6000);
            Camera_MoveTo(0x3480000, -1, 0xd80000, 1);
            Engine_CameraWaitForMove();
            Event_ShowMessage(a0, 0);
            SceneState_SetMode66AndPassOpeningSequence();
            Engine_EventWait(60);
            Event_ShowMessage(a0, 0);
            {
                /* FAKEMATCH: native coordinates in r1/r2 precede r0 setup. */
                register s32 fx asm("r1") = 0x2e0;
                /* FAKEMATCH: native coordinates in r1/r2 precede r0 setup. */
                register s32 fz asm("r2") = 200;
                /* FAKEMATCH: direct and Call3 void forms move r0 ahead of coordinates. */
                asm("" : : "r"(fx), "r"(fz) : "r0");
                Korosseo_FadeInCompetitor(0, fx, fz);
            }
            /* FAKEMATCH: the void result is discarded; Call3 changes argument allocation. */
            Value3(Engine_ActorFaceDirection, 0, 0, 0);
            SceneState_WaitUntilWordC41cIs22();
            Actor_SetSpeed(ACTOR_PARTY_LEADER, 0x10000, 0x8000);
            Actor_WalkToAndWait(ACTOR_PARTY_LEADER, 0x330, 200);
            Engine_EventWait(30);
            Actor_ShowEmote(ACTOR_PARTY_LEADER, 0x105, 60);
            Event_ShowMessage(a0, 0);
            Korosseo_RestoreCompetitor(0);
            Engine_CameraFollowActor(ACTOR_PARTY_LEADER, 0);
            SceneState_SendIdBySceneId(a0, 3);
        } else {
            if (rec8 == 1) {
                Engine_EventSetMessage((s32)MsgKorosseoOperatorBridgeWillAlsoCheer);
                Event_ShowMessage(a0, 0);
            }
        }
        /* FAKEMATCH: the void result is discarded; Call3 changes argument allocation. */
        Value3(FieldScene_RunMiddleSequence, rec8, a0, 3);
        Engine_EventEnd();
    }
}

void Scene_RunSceneFourCoordinator(s32 scene)
{

    s32 path;

    if (gGameState.entrance == 2) {
        Korosseo_FinishSoloRound();
        return;
    }
    Engine_EventBegin();
    path = SceneDialogue_RunFlagGatedPromptInteraction(scene, 4);
    if (path == 0) {
        Engine_EventSetMessage((s32)MsgKorosseoPlaceNormallyCalledLumberWater);
        Camera_SetSpeed(196608, 24576);
        Camera_MoveTo(71303168, -1, 11010048, 1);
        Engine_CameraWaitForMove();
        Event_ShowMessage(scene, 0);
        /* FAKEMATCH: the void result is discarded; Call3 changes argument allocation. */
        Value3(SceneState_StoreParamsAndInitTable, 120, 72, 0);
        Event_ShowMessage(scene, 0);
        SceneState_ReleaseTableAndResetC6a6();
        Engine_EventWait(15);
        {
            /* FAKEMATCH: native coordinates in r1/r2 precede r0 setup. */
            register s32 fx asm("r1") = 984;
            /* FAKEMATCH: native coordinates in r1/r2 precede r0 setup. */
            register s32 fz asm("r2") = 200;
            /* FAKEMATCH: direct and Call3 void forms move r0 ahead of coordinates. */
            asm("" : : "r"(fx), "r"(fz) : "r0");
            Korosseo_FadeInCompetitor(0, fx, fz);
        }
        Actor_FaceDirection(ACTOR_PARTY_LEADER, 0, 10);
        Event_ShowMessage(scene, 0);
        Actor_FaceDirection(ACTOR_PARTY_LEADER, 16384, 30);
        Actor_ShowEmote(ACTOR_PARTY_LEADER, 262, 60);
        Actor_SetSpeed(ACTOR_PARTY_LEADER, 98304, 49152);
        OverlayObject_PlaceWithScale14000(0, 1000, 192);
        OverlayObject_PlaceWithScale14000(0, 1000, 176);
        OverlayObject_PlaceWithScale14000(0, 1016, 168);
        Engine_EventWait(15);
        /* FAKEMATCH: the void result is discarded; Call3 changes argument allocation. */
        Value3(StagedActor_PlacePairAtOffsetAndRun, 18, 160, 0);
        Camera_MoveTo(71303168, -1, 11010048, 1);
        Engine_ActorSetAnimation(ACTOR_PARTY_LEADER, 1);
        Engine_EventWait(10);
        Actor_SetSpeed(ACTOR_PARTY_LEADER, 65536, 32768);
        Actor_WalkToAndWait(ACTOR_PARTY_LEADER, 1192, 168);
        Engine_EventWait(10);
        Actor_FaceDirection(ACTOR_PARTY_LEADER, 32768, 30);
        Actor_ShowEmote(ACTOR_PARTY_LEADER, 258, 60);
        Event_ShowMessage(scene, 0);
        Korosseo_RestoreCompetitor(0);
        Engine_CameraFollowActor(ACTOR_PARTY_LEADER, 0);
        Actor_SetPosition(18, 66584576, 11010048);
        SceneState_SendIdBySceneId(scene, 4);
    } else if (path == 1) {
        Engine_EventSetMessage((s32)MsgKorosseoLogsKeyClearingStage);
        Event_ShowMessage(scene, 0);
    }
    /* FAKEMATCH: the void result is discarded; Call3 changes argument allocation. */
    Value3(FieldScene_RunMiddleSequence, path, scene, 4);
    Engine_EventEnd();
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
        Engine_ActorSetPosition(ACTOR_GERALD, x, y);
    }
    {
        s32 x = GameFlag_GetByteFar(912);
        s32 y = GameFlag_GetByteFar(920);

        x <<= 20;
        x += 0x80000;
        y <<= 20;
        y += 0x80000;
        Engine_ActorSetPosition(ACTOR_IVAN, x, y);
    }
    {
        s32 x = GameFlag_GetByteFar(928);
        s32 y = GameFlag_GetByteFar(936);

        x <<= 20;
        x += 0x80000;
        y <<= 20;
        y += 0x80000;
        Engine_ActorSetPosition(ACTOR_MIA, x, y);
    }
}
