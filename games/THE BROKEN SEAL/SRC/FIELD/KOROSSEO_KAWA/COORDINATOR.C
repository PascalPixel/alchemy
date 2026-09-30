#include "TYPES.H"
#include "FIELD_EVENT.H"
#include "FIELD_SCENE.H"
#include "STAGED_ACTOR.H"
#include "CALL.H"
/* FAKEMATCH: calls that cast Object_GetById to another return type keep their original register order. */
struct FieldActor *Object_GetById();

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
s32 Scheduler_AddOrUpdateCallback(s32, s32);

s32 Map_GetTerrainHeightFar(s32, s32, s32);

void GameFlag_SetByte(s32, s32);

void Object_SetMoveTarget(struct FieldActor *, s32, s32, s32);
void Script_WaitForEventTimeout(struct FieldActor *);

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

s32 SceneDialogue_RunFlagGatedPromptInteraction(s32 a, s32 b);

void FieldScene_RunMiddleSequence(s32 mode, s32 owner, s32 base);

s32 *SceneActor_FindOccupantAheadOfSubject(void);

void SceneState_StoreParamsAndInitTable(s32 a, s32 b, s32 c);

static inline void InitializeActorZero(void)
{
    Actor_SetSpeed(ACTOR_PARTY_LEADER, 0x10000, 0x8000);
}

static inline void InitializeSelectedActor(s32 actorId)
{
    Actor_SetSpeed(actorId, 0x10000, 0x8000);
}

/* Selects a later line in the current dialogue. */
static __inline__ void AdvanceMessage(s32 amount)
{
    gEventWork->message += amount;
}

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
        Actor_SetAnimation(16, 10); /* object 16, action 10 */
        break;
    case 60:
        Map_UpdateCellRect(92, 33, 2, 2, 50, 38); /* main:080091c8 */
        Map_UpdateCellRect(92, 33, 2, 2, 54, 38); /* main:080091c8 */
        Map_CopyCellAttributes(50, 25, 6, 1, 50, 12); /* main:080091c0 */
        Actor_SetAnimation(16, 11); /* object 16, action 11 */
        break;
    case 6:
        Map_UpdateCellRect(92, 31, 2, 2, 50, 38); /* main:080091c8 */
        Map_UpdateCellRect(92, 31, 2, 2, 54, 38); /* main:080091c8 */
        Actor_SetAnimation(16, 10); /* object 16, action 10 */
        break;
    case 0:
        Map_UpdateCellRect(92, 29, 2, 2, 50, 38); /* main:080091c8 */
        Map_UpdateCellRect(92, 29, 2, 2, 54, 38); /* main:080091c8 */
        Actor_SetAnimation(16, 12); /* object 16, action 12 */
        Map_CopyCellAttributes(50, 24, 6, 1, 50, 12); /* main:080091c0 */
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

    Task_Wait(10);
    i = 0;
    if (KorosseoKawa_Countdown != 22) {
        do {
            Task_Wait(1);
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

        Map_CopyCellAttributes(27, 13, 3, 1, x, y);
    }
    actor = Object_GetById(9);
    blocked = Map_GetTerrainHeightFar(0, actor->x.fixed, actor->z.fixed);
    if (actor->y.fixed == 0 && blocked == 0) {
        actor->priority_flags = ACTOR_PRIORITY_UNDERFOOT;
        actor->motion_flags = 0;
        {
            s32 x = actor->x.fixed >> 20;
            s32 y = actor->z.fixed >> 20;

            Map_CopyCellAttributes(14, 13, 1, 1, x, y);
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

        Map_CopyCellAttributes(14, 13, 1, 1, x, y);
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

        Map_CopyCellAttributes(47, 24, 1, 1, x, y);
    }
    GameFlag_Set(0x303);
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
    Event_Begin();
    Camera_SetSpeed(0x20000, 0x4000);
    Camera_MoveTo(0x2580000, -1, 0xc80000, 1);
    Object_SetAnimation((s32)rec7, 3);
    Camera_WaitForMove();
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
    Event_Wait(45);
    Map_CopyCellAttributes(43, 12, 1, 1, 41, 12);
    Event_End();
}

void StagedActor_PlacePairAtOffsetAndRun(s32 actor_id, s32 dx, s32 dz)
{
    struct FieldActor *leader;
    struct FieldActor *actor;
    s32 x;
    s32 z;

    leader = Object_GetById(gGameState.selected_actor);
    actor = Object_GetById(actor_id);
    Event_Begin();
    {
        x = ((leader->x.fixed + (dx << 16)) & 0xFFF00000) + 0x80000;
        z = ((leader->z.fixed + (dz << 16)) & 0xFFF00000) + 0x80000;

        leader->speed = 0x10000;
        leader->acceleration = 0x8000;
        Object_SetMoveTarget(leader, x, leader->y.fixed, z);
    }
    Object_SetAnimation(leader, 27);
    {
        x = ((actor->x.fixed + (dx << 16)) & 0xFFF00000) + 0x80000;
        z = ((actor->z.fixed + (dz << 16)) & 0xFFF00000) + 0x80000;

        actor->speed = 0x10000;
        actor->acceleration = 0x8000;
        Object_SetMoveTarget(actor, x, actor->y.fixed, z);
    }
    if (dx < 0 || dz < 0) {
        Object_SetAnimation(actor, 4);
    } else {
        Object_SetAnimation(actor, 3);
    }
    Audio_PlayCue(226);
    Script_WaitForEventTimeout(leader);
    Object_SetAnimation(actor, 2);
    Audio_PlayCue(288);
    Event_End();
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
    Map_CopyCellAttributes(67, 8, 3, 1, 64, row);
    StagedActor_PlacePairAtOffsetAndRun(17, 0, offset);
    actor = Object_GetById(17);
    row = actor->z.fixed >> 20;
    Map_CopyCellAttributes(64, 24, 3, 1, 64, row);
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
    Map_CopyCellAttributes(72, 9, 1, 3, column, 9);
    StagedActor_PlacePairAtOffsetAndRun(18, offset, 0);
    actor = Object_GetById(18);
    column = actor->x.fixed >> 20;
    Map_CopyCellAttributes(63, 25, 1, 3, column, 9);
}
void StagedActor_PushActorAhead(void);

void ObjectDispatch_InitFromTable4WithArgument(s32 table, struct FieldActor *object);
u8 *Runtime_AllocateBlock(s32 id, s32 size);

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
    Event_Begin();
    Actor_SetAnimation(leader_id, 8);
    Event_Wait(6);
    actor->speed = 0x8000;
    actor->acceleration = 0x3333;
    Audio_PlayCue(239);
    Object_SetAnimation(actor, 2);
    Object_SetMoveTarget(actor, actor->x.fixed - 0x300000, 0, actor->z.fixed);
    Event_Wait(6);
    Actor_SetAnimation(leader_id, 2);
    ObjectDispatch_InitFromTable4WithArgument(*(s32 *)(Runtime_AllocateBlock(27, 0xccc) + 480), actor);
    Actor_SetSpeed(leader_id, 0x4ccc, 0x3333);
    Object_SetMoveTarget(leader, leader->x.fixed - 0x180000, 0, leader->z.fixed);
    Actor_WaitForMove(leader_id);
    Actor_SetAnimation(leader_id, 1);
    Script_WaitForEventTimeout(actor);
    Object_SetAnimation(actor, 1);
    Audio_PlayCue(288);
    Audio_PlayCue(213);
    Event_Wait(15);
    Event_End();
    Map_CopyCellAttributes(37, 7, 1, 4, 34, 7);
    Map_CopyCellAttributes(36, 7, 1, 4, 37, 7);
    none = GameFlag_IsSet(0x301);
    if (none != 0) {
        Event_Begin();
        Camera_SetSpeed(0x20000, 0x4000);
        Camera_MoveTo(0x2280000, -1, 0xc80000, 1);
        Camera_WaitForMove();
        Map_CopyCells(96, 29, 1, 3, 34, 38);
        Event_Wait(3);
        Map_CopyCells(97, 29, 1, 3, 34, 38);
        Event_Wait(3);
        Map_CopyCells(98, 29, 1, 3, 34, 38);
        Event_Wait(3);
        Map_CopyCells(99, 29, 1, 3, 34, 38);
        Event_Wait(3);
        Map_CopyCells(100, 29, 1, 3, 34, 38);
        Event_Wait(15);
        Event_End();
        return;
    }
    GameFlag_Set(0x301);
    Event_Begin();
    Camera_SetSpeed(0x20000, 0x4000);
    Camera_MoveTo(0x2580000, -1, 0xc80000, 1);
    Camera_WaitForMove();
    actor = Object_GetById(13);
    actor->motion_flags = none;
    actor->acceleration = 0x6666;
    actor->speed = 0xcccc;
    Object_SetMoveTarget(actor, actor->x.fixed, 0x80000, actor->z.fixed);
    Object_SetAnimation(actor, 3);
    Map_CopyCells(96, 29, 1, 3, 34, 38);
    Event_Wait(3);
    Map_CopyCells(97, 29, 1, 3, 34, 38);
    Event_Wait(3);
    Map_CopyCells(98, 29, 1, 3, 34, 38);
    Event_Wait(3);
    Map_CopyCells(99, 29, 1, 3, 34, 38);
    Event_Wait(3);
    Map_CopyCells(100, 29, 1, 3, 34, 38);
    actor = Object_GetById(14);
    actor->motion_flags = none;
    actor->acceleration = 0x6666;
    actor->speed = 0xcccc;
    Object_SetMoveTarget(actor, actor->x.fixed, 0x200000, actor->z.fixed);
    Script_WaitForEventTimeout(actor);
    Event_Wait(15);
    Event_End();
    Map_CopyCellAttributes(43, 12, 1, 1, 41, 12);
}
