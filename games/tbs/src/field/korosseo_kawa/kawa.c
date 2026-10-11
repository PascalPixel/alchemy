#include "battle.h"
#include "keystate.h"
#include "debug.h"
#include "field.h"
#include "game.h"
#include "graphics.h"
#include "menu.h"
#include "minigame.h"
#include "sound.h"
#include "system.h"
#include "mem.h"
#include "types.h"
#include "callback_scheduler.h"
#include "field_event.h"
#include "field_service.h"
#include "field_scene.h"
#include "staged_actor.h"
#include "kawa.h"
#include "call.h"
#include "resource_ids.h"
#include "text/msg_ids.h"

// 戦闘開始の種類 (battle/party/party.c と同じ)


#define ObjectDispatch_InitFromTable4WithArgument ((void (*)(s32,struct FieldActor*))ObjectDispatch_InitFromTable4WithArgument)
#define FieldScene_BuildSceneDescriptorAndInstallTask ((s32 (*)(s32,s32,s32,s32,s32,s32,s32))FieldScene_BuildSceneDescriptorAndInstallTask)

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

struct ModeRecord {
    u8 pad[26];
    u16 span;
};

typedef struct ActiveSubjectSlot {
    u8 pad[500];
    void *handle;
} ActiveSubjectSlot;

#define KEYS_HELD gKeysHeld // VBlank で書く
extern u8 HexDigits[];
void Script_WaitForEventTimeout();
typedef void(*SceneTask)(void);

s32 Map_GetTerrainHeight(s32, s32, s32);
void GameFlag_SetByte(s32, s32);
void Script_WaitForEventTimeout(struct FieldActor *);
PartyInteractionRecord *GetPartyInteractionRecord(void);
s32 GetPartyMemberCount(void);
s32 SceneDialogue_RunFlagGatedPromptInteraction(s32 a, s32 b);

s32 FieldScene_RunMiddleSequence();
s32 SceneState_StoreParamsAndInitTable();
s32 Korosseo_FadeInCompetitor();

void StagedActor_PushActorAhead(void);
s32 FieldScene_RunFlag211ApproachScene();
void Object_RefreshSelectorById(s32 actor);
s32 BattleFx_SetWeightedResult();
s32 Party_SetFields1ceAnd1d0();
void Event_SetDefeatDestination(s32 scene, s32 entrance);
void Owner_RefreshActiveRatios();
void FieldScene_RunLateSequence(s32 a0);
s32 Map_GetTerrainHeight(s32 layer, s32 x, s32 z);
s32 Korosseo_ShowItemIcon(s32 slot, s32 item);
void Korosseo_SelectSoloCompetitor(s32 index);
void Object_LinkObjectAndSetCallback(s32 actor, s32 leader);
void FieldScene_RunOpeningAuxiliarySequence(void);

void Korosseo_FinishSoloRound();
void Engine_EventBegin();
s32 SceneDialogue_RunFlagGatedPromptInteraction();
void Engine_EventSetMessage();
void Engine_CameraSetSpeed();
void Engine_CameraMoveTo();
void Engine_CameraWaitForMove();
void Engine_EventShowMessage();
void Engine_ActorSetSpeed();
void Engine_ActorWalkToAndWait();
void Engine_EventWait();
void Engine_ActorShowEmote();
void Engine_ActorFaceDirection();
s32 OverlayObject_PlaceWithScale14000();
void StagedActor_PushActorAhead();
void Engine_ActorSetAnimation();
void Korosseo_RestoreCompetitor();
void Engine_CameraFollowActor();
void Engine_ActorSetPosition();
void SceneState_SendIdBySceneId();
void Engine_EventEnd();
void SceneState_InitTableWordsAndLoad3200();
void KorosseoKawa_ReleaseMarker();
void Engine_ObjectSetPosition();

void SceneState_SendIdBySceneId(s32 a, s32 b);
void KorosseoKawa_ReleaseMarker(void);

u8 *KorosseoKawa_GetEntrances(void)
{

    return KorosseoKawa_SceneTableA;
}

s32 get_default_result(void)
{
    return 0;
}

u8 *KorosseoKawa_GetExits(void)
{

    return KorosseoKawa_SceneTableB;
}

u8 *KorosseoKawa_GetPlacements(void)
{

    return KorosseoKawa_SceneTableC;
}

void FieldScene_RunOpeningAuxiliarySequence(void)
{
    extern s32 KorosseoKawa_Countdown;

    switch ((u32)KorosseoKawa_Countdown) {
    case 66:
        Engine_MapCopyCellsTo(92, 31, 2, 2, 50, 38);
        Engine_MapCopyCellsTo(92, 31, 2, 2, 54, 38);
        Engine_ActorSetAnimation(16, 10);
        break;
    case 60:
        Engine_MapCopyCellsTo(92, 33, 2, 2, 50, 38);
        Engine_MapCopyCellsTo(92, 33, 2, 2, 54, 38);
        Engine_MapCopyCellAttributes(50, 25, 6, 1, 50, 12);
        Engine_ActorSetAnimation(16, 11);
        break;
    case 6:
        Engine_MapCopyCellsTo(92, 31, 2, 2, 50, 38);
        Engine_MapCopyCellsTo(92, 31, 2, 2, 54, 38);
        Engine_ActorSetAnimation(16, 10);
        break;
    case 0:
        Engine_MapCopyCellsTo(92, 29, 2, 2, 50, 38);
        Engine_MapCopyCellsTo(92, 29, 2, 2, 54, 38);
        Engine_ActorSetAnimation(16, 12);
        Engine_MapCopyCellAttributes(50, 24, 6, 1, 50, 12);
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
    Engine_TaskRemoveCallback(task);
    task();
}

void SceneState_SetMode66AndPassOpeningSequence(void)
{
    extern s32 KorosseoKawa_Countdown;

    s32 value = 66;
    s32 *mode = (s32 *)&KorosseoKawa_Countdown;

    *mode = value;
    Engine_TaskAddCallback((s32)FieldScene_RunOpeningAuxiliarySequence, 0xC80);
}

void KorosseoKawa_WaitForCountdown(void)
{
#ifdef MODERN
    // modern GCC holds one type for a name in a file
    extern u32 KorosseoKawa_Countdown_modern __asm__("KorosseoKawa_Countdown");
#define KorosseoKawa_Countdown KorosseoKawa_Countdown_modern
#else
    extern u32 KorosseoKawa_Countdown;
#endif

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
#ifdef MODERN
#undef KorosseoKawa_Countdown
#endif

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
    blocked = Map_GetTerrainHeight(0, actor->x.fixed, actor->z.fixed);
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

void KorosseoKawa_RaisePlatformPair(void)
{
#ifdef MODERN
    // modern GCC holds one type for a name in a file
    extern u8 KorosseoKawa_Countdown_bytes[] __asm__("KorosseoKawa_Countdown");
#define KorosseoKawa_Countdown KorosseoKawa_Countdown_bytes
#else
    extern u8 KorosseoKawa_Countdown[];
#endif

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
    ((struct FieldActor *)rec7)->motion_flags = none;
    ((struct FieldActor *)rec7)->acceleration = 0x6666;
    ((struct FieldActor *)rec7)->speed = 0xcccc;
    FieldObject_SetMoveTarget((s32)rec7, ((struct FieldActor *)rec7)->x.fixed, 0x80000, ((struct FieldActor *)rec7)->z.fixed);
    rec7 = Actor_Get(14);
    ((struct FieldActor *)rec7)->motion_flags = none;
    ((struct FieldActor *)rec7)->acceleration = 0x6666;
    ((struct FieldActor *)rec7)->speed = 0xcccc;
    ((void (*)(struct FieldActor *, s32, s32, s32))Engine_ObjectSetPosition)((s32)rec7, ((struct FieldActor *)rec7)->x.fixed, 0x200000, ((struct FieldActor *)rec7)->z.fixed);
    Script_WaitForEventTimeout((s32)rec7);
    Engine_EventWait(45);
    Map_CopyCellAttributes(43, 12, 1, 1, 41, 12);
    Engine_EventEnd();
}
#ifdef MODERN
#undef KorosseoKawa_Countdown
#endif

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
        ((void (*)(struct FieldActor *, s32, s32, s32))Engine_ObjectSetPosition)(leader, x, leader->y.fixed, z);
    }
    Object_SetMode(leader, 27);
    {
        x = ((actor->x.fixed + (dx << 16)) & 0xFFF00000) + 0x80000;
        z = ((actor->z.fixed + (dz << 16)) & 0xFFF00000) + 0x80000;

        actor->speed = 0x10000;
        actor->acceleration = 0x8000;
        ((void (*)(struct FieldActor *, s32, s32, s32))Engine_ObjectSetPosition)(actor, x, actor->y.fixed, z);
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
    if ((KEYS_HELD & 32) != 0) {
        direction = -1;
    }
    if ((KEYS_HELD & 16) != 0) {
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
    Event_PlayCue(239);
    Object_SetMode(actor, 2);
    ((void (*)(struct FieldActor *, s32, s32, s32))Engine_ObjectSetPosition)(actor, actor->x.fixed - 0x300000, 0, actor->z.fixed);
    Engine_EventWait(6);
    Engine_ActorSetAnimation(leader_id, 2);
    ObjectDispatch_InitFromTable4WithArgument(*(s32 *)(Runtime_AllocateBlock(27, 0xccc) + 480), actor);
    Actor_SetSpeed(leader_id, 0x4ccc, 0x3333);
    ((void (*)(struct FieldActor *, s32, s32, s32))Engine_ObjectSetPosition)(leader, leader->x.fixed - 0x180000, 0, leader->z.fixed);
    Engine_ActorWaitForMove(leader_id);
    Engine_ActorSetAnimation(leader_id, 1);
    Script_WaitForEventTimeout(actor);
    Object_SetMode(actor, 1);
    Event_PlayCue(288);
    Event_PlayCue(213);
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
    ((void (*)(struct FieldActor *, s32, s32, s32))Engine_ObjectSetPosition)(actor, actor->x.fixed, 0x80000, actor->z.fixed);
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
    ((void (*)(struct FieldActor *, s32, s32, s32))Engine_ObjectSetPosition)(actor, actor->x.fixed, 0x200000, actor->z.fixed);
    Script_WaitForEventTimeout(actor);
    Engine_EventWait(15);
    Engine_EventEnd();
    Map_CopyCellAttributes(43, 12, 1, 1, 41, 12);
}

void SceneState_ApplyValue768(void)
{

    Engine_GameFlagSet(768);
}

void KorosseoKawa_RunStageStart(void)
{
    s32 approach;
    s32 n;
    s32 base;

    base = 0;
    Resource3ba_NoOpCallback();
    Engine_EventBegin();
    approach = FieldScene_RunFlag211ApproachScene(120, 127);
    KorosseoKawa_WaitForStageReady();
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
    /* FAKEMATCH: base is 0 from the top of the function, so 0 - approach + 1 is not folded into 1 - approach. */
    BattleFx_SetWeightedResult(72, base - approach + 1);
    Battle_SetStart(3);
    Party_SetFields1ceAnd1d0((s32)&SceneId_KorosseoKawa, 4);
    Event_SetDefeatDestination((s32)&SceneId_KorosseoKawa, 5);
    GameFlag_Set(0x11a);
}

void FieldScene_RunBranchedStep(void)
{

    if (SceneActor_FindOccupantAheadOfSubject() == 0) {
        Engine_LeaderCheckAhead();
    } else {
        FieldScene_RunTwoCallSequence();
    }
}

u8 *KorosseoKawa_GetEvents(void)
{

    return KorosseoKawa_SceneTableD;
}

void KorosseoKawa_RunPostMatchScene(s32 a0)
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
            o->terrain_id = 1;
        }
    }
}

s32 KorosseoKawa_ApplyEntryState(void)
{
    struct FieldActor *actor;
    s32 hit;
    s32 x;
    s32 z;
    u8 zero;
    s32 col;

    gEventWork->start_transition = 0;
    GameFlag_Set(0x144);
    actor = Object_GetById(9);
    hit = Map_GetTerrainHeight(0, actor->x.fixed, actor->z.fixed);
    if (actor->y.fixed == 0 && hit == 0) {
        actor->priority_flags = 2;
        actor->motion_flags = hit;
        Map_CopyCellAttributes(14, 13, 1, 1, actor->x.fixed >> 20, actor->z.fixed >> 20);
    }
    x = GameFlag_GetByte(0x310);
    if (x == 0) {
        x = 25;
    }
    actor = Object_GetById(10);
    actor->x.fixed = (x << 20) + 0x80000;
    actor->motion_flags = 0;
    actor->priority_flags = 2;
    Map_CopyCellAttributes(14, 13, 1, 1, x, 12);
    ((s32 (*)(void ( *)(void), s32))Engine_TaskAddCallback)(FieldScene_RunOpeningAuxiliarySequence, 0xc80);
    actor = Object_GetById(15);
    actor->terrain_id = 1;
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
    switch (gGameState.entrance) {
    case 1:
        FieldScene_BuildSceneDescriptorAndInstallTask(0, 8, 4, 0x5180000, 0xc00000, 24, 25);
        Map_CopyCellsTo(127, 0, 1, 2, 19, 2);
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
        ((s32 (*)(void ( *)(void), s32))Engine_TaskAddCallback)(SceneState_RaiseTriggerOnPsynergyTarget, 0xc80);
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
        KorosseoKawa_RunPostMatchScene(1);
        Engine_EventRequestExit(4);
        break;
    case 5:
        KorosseoKawa_RunPostMatchScene(-1);
        Engine_EventRequestExit(5);
        break;
    }
    return 0;
}

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
    Actor_SetSpeed(GERALD, 0x10000, 0x8000);
    Actor_SetSpeed(IWAN, 0x10000, 0x8000);
    Actor_SetSpeed(MEARI, 0x10000, 0x8000);
    Engine_ActorSetPosition(0, x << 16, (z << 16) - 0x300000);
    Actor_SetPosition(GERALD, (x << 16) - 0x100000, (z << 16) - 0x280000);
    Actor_SetPosition(IWAN, (x << 16) + 0x100000, (z << 16) - 0x280000);
    Actor_SetPosition(MEARI, x << 16, (z << 16) - 0x200000);
    Actor_SetPosition(a0, x << 16, (z << 16) - 0x500000);
    actor = Object_GetById(0);
    actor->facing = 0xc000;
    Engine_CameraFollowActor(ACTOR_PARTY_LEADER, 0);
    Engine_EventOpenScreen();
    Engine_EventWaitForScreen();
    Engine_EventSetMessage((s32)MsgKorosseoSiteFirstFinalsBattle);
    Event_ShowMessage(a0, 0);
    Actor_ShowEmote(MEARI, 0x101, 60);
    Event_ShowMessage(MEARI, 0);
    Engine_ActorStartRepeatedMotion(a0, 3);
    Event_ShowMessage(a0, 0);
    Actor_ShowEmote(IWAN, 0x101, 60);
    Event_ShowMessage(IWAN, 0);
    Actor_FaceActor(a0, IWAN, 0);
    Engine_EventWait(20);
    Engine_ActorSetAnimationAndWait(a0, 3);
    Event_ShowMessage(a0, 0);
    Actor_ShowEmote(GERALD, 0x101, 60);
    Event_ShowMessage(GERALD, 0);
    Actor_ShowEmote(MEARI, 0x101, 60);
    Event_ShowMessage(MEARI, 0);
    Actor_ShowEmote(a0, 0x102, 60);
    if (Event_AskYesNo(a0, 0) == 0) {
        do {
            Engine_EventSetMessage((s32)MsgKorosseoWarriorsEnterFinalsWithoutAny);
            Engine_ActorSetAnimation(IWAN, 3);
            Engine_EventWait(2);
            Engine_ActorSetAnimation(GERALD, 3);
            Engine_EventWait(2);
            Engine_ActorSetAnimation(MEARI, 3);
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
    Actor_FaceDirection(GERALD, 0, 0);
    Actor_FaceDirection(IWAN, 0x8000, 0);
    Engine_ActorRunRepeatedMotion(GERALD, 2);
    Event_ShowMessage(GERALD, 0);
    Engine_ActorRunRepeatedMotion(IWAN, 2);
    Event_ShowMessage(IWAN, 0);
    Engine_ActorSetAnimationAndWait(MEARI, 3);
    Event_ShowMessage(MEARI, 0);
    Engine_ActorSetAnimation(MEARI, 3);
    Engine_EventWait(1);
    Engine_ActorSetAnimation(GERALD, 3);
    Engine_EventWait(2);
    Engine_ActorSetAnimation(IWAN, 3);
    Engine_EventWait(1);
    Engine_ActorSetAnimationAndWait(ACTOR_PARTY_LEADER, 3);
    Engine_EventWait(6);
    Engine_ActorSetAnimation(GERALD, 2);
    actor = Object_GetById(0);
    if (actor != 0) {
        Actor_SetDestination(GERALD, actor->x.part.pixel, actor->z.part.pixel);
    }
    Engine_ActorSetAnimation(IWAN, 2);
    actor = Object_GetById(0);
    if (actor != 0) {
        Actor_SetDestination(IWAN, actor->x.part.pixel, actor->z.part.pixel);
    }
    Engine_ActorSetAnimation(MEARI, 2);
    actor = Object_GetById(0);
    if (actor != 0) {
        Actor_SetDestination(MEARI, actor->x.part.pixel, actor->z.part.pixel);
    }
    Actor_WalkToAndWait(a0, x - 16, z - 64);
    Actor_SetPosition(GERALD, 0, 0);
    Actor_SetPosition(IWAN, 0, 0);
    Actor_SetPosition(MEARI, 0, 0);
    Actor_WalkToAndWait(a0, x - 16, z - 16);
    Actor_WalkToAndWait(a0, x, z);
    Actor_FaceDirection(a0, 0xc000, 10);
    Engine_EventEnd();
}

void KorosseoKawa_RunStageIntro(s32 scene)
{
    s32 path;

    if (gGameState.entrance == 2) {
        Korosseo_FinishSoloRound();
    } else {
        Engine_EventBegin();
        path = SceneDialogue_RunFlagGatedPromptInteraction(scene, 1);
        if (path == 0) {
            Engine_EventSetMessage((s32)MsgKorosseoCallRockChallenge);
            Camera_SetSpeed(0x30000, 0x6000);
            Camera_MoveTo(PIXELS(328), -1, PIXELS(168), 1);
            Engine_CameraWaitForMove();
            Engine_EventShowMessage(scene, 0);
            Korosseo_FadeInCompetitor(0, 280, 200);
            Actor_SetSpeed(ACTOR_PARTY_LEADER, 0x10000, 0x8000);
            Actor_WalkToAndWait(ACTOR_PARTY_LEADER, 360, 200);
            Engine_EventWait(30);
            Actor_ShowEmote(ACTOR_PARTY_LEADER, 0x102, 60);
            Engine_EventShowMessage(scene, 0);
            Actor_WalkToAndWait(ACTOR_PARTY_LEADER, 312, 200);
            Engine_EventWait(30);
            Actor_FaceDirection(ACTOR_PARTY_LEADER, 0xc000, 10);
            Actor_ShowEmote(ACTOR_PARTY_LEADER, 0x106, 60);
            Actor_SetSpeed(ACTOR_PARTY_LEADER, 0x18000, 0xc000);
            OverlayObject_PlaceWithScale14000(0, 296, 184);
            OverlayObject_PlaceWithScale14000(0, 296, 152);
            OverlayObject_PlaceWithScale14000(0, 312, 152);
            Actor_FaceDirection(ACTOR_PARTY_LEADER, 0x4000, 15);
            StagedActor_PushActorAhead();
            OverlayObject_ResetMotionFields(0);
            StagedActor_PushActorAhead();
            OverlayObject_ResetMotionFields(0);
            Actor_SetSpeed(ACTOR_PARTY_LEADER, 0x18000, 0xc000);
            Actor_WalkToAndWait(ACTOR_PARTY_LEADER, 304, 184);
            Engine_ActorWalkToAndWait(ACTOR_PARTY_LEADER, 296, 192);
            Engine_ActorWalkToAndWait(ACTOR_PARTY_LEADER, 296, 200);
            Engine_ActorFaceDirection(ACTOR_PARTY_LEADER, 0, 15);
            StagedActor_PushActorAhead();
            OverlayObject_ResetMotionFields(0);
            StagedActor_PushActorAhead();
            OverlayObject_ResetMotionFields(0);
            Engine_ActorSetAnimation(ACTOR_PARTY_LEADER, 1);
            Engine_EventShowMessage(scene, 0);
            Korosseo_RestoreCompetitor(0);
            Engine_CameraFollowActor(ACTOR_PARTY_LEADER, 0);
            Actor_SetPosition(9, 0x1380000, 0xa80000);
            SceneState_SendIdBySceneId(scene, 1);
        } else if (path == 1) {
            Engine_EventSetMessage((s32)MsgKorosseoObjectiveStageClear);
            Engine_EventShowMessage(scene, 0);
        }
        FieldScene_RunMiddleSequence(path, scene, 1);
        Engine_EventEnd();
    }
}

void Korosseo_RunPipeworksIntro(s32 a0)
{
    s32 rec4;
    struct FieldActor *second;
    struct FieldActor *first;

    if (gGameState.entrance == 2) {
        Korosseo_FinishSoloRound();
    } else {
        Engine_EventBegin();
        rec4 = SceneDialogue_RunFlagGatedPromptInteraction(a0, 2);
        if (rec4 == 0) {
            Engine_EventSetMessage((s32)MsgKorosseoAreaCalledPipeworks);
            Engine_CameraSetSpeed(0x30000, 0x6000);
            Camera_MoveTo(0x2500000, -1, 0x780000, 1);
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
            KorosseoKawa_ReleaseMarker();
            Engine_TaskWait(2);
            first = Object_GetById(13);
            first->motion_flags = 0;
            first->acceleration = 0x6666;
            first->speed = 0xcccc;
            Engine_ObjectSetPosition(first, first->x.fixed, 0x80000, first->z.fixed);
            second = Object_GetById(14);
            second->motion_flags = 0;
            second->acceleration = 0x6666;
            second->speed = 0xcccc;
            Engine_ObjectSetPosition(second, second->x.fixed, 0x200000, second->z.fixed);
            Script_WaitForEventTimeout((s32)second);
            Engine_EventWait(45);
            first = Object_GetById(13);
            first->motion_flags = 0;
            first->acceleration = 0x6666;
            first->speed = 0xcccc;
            Engine_ObjectSetPosition(first, first->x.fixed, 0x180000, first->z.fixed);
            second = (struct FieldActor *)Actor_Get(14);
            second->motion_flags = 0;
            second->acceleration = 0x6666;
            second->speed = 0xcccc;
            Engine_ObjectSetPosition(second, second->x.fixed, 0, second->z.fixed);
            Script_WaitForEventTimeout((s32)second);
            Engine_EventWait(15);
            Engine_EventShowMessage(a0, 0);
            SceneState_StoreParamsAndInitTable(56, 64, 0);
            Engine_EventWait(30);
            SceneState_InitTableWordsAndLoad3200(160, 96, 10);
            Engine_EventWait(40);
            SceneState_InitTableWordsAndLoad3200(56, 64, 10);
            Engine_EventWait(70);
            Engine_EventShowMessage(a0, 0);
            KorosseoKawa_ReleaseMarker();
            Engine_TaskWait(2);
            Engine_CameraFollowActor(0, 0);
            SceneState_SendIdBySceneId(a0, 2);
        } else if (rec4 == 1) {
            Engine_EventSetMessage((s32)MsgKorosseoObjectiveMakeGood);
            Engine_EventShowMessage(a0, 0);
        }
        FieldScene_RunMiddleSequence(rec4, a0, 2);
        Engine_EventEnd();
    }
}

void Korosseo_RunBrokenBridgeIntro(s32 scene)
{
    s32 path;

    if (gGameState.entrance == 2) {
        Korosseo_FinishSoloRound();
    } else {
        Engine_EventBegin();
        path = SceneDialogue_RunFlagGatedPromptInteraction(scene, 3);
        if (path == 0) {
            Engine_EventSetMessage((s32)MsgKorosseoTheyCallBrokenBridge);
            SceneState_ResetCounterAndStartTask();
            Camera_SetSpeed(0x30000, 0x6000);
            Camera_MoveTo(PIXELS(840), -1, PIXELS(216), 1);
            Engine_CameraWaitForMove();
            Event_ShowMessage(scene, 0);
            SceneState_SetMode66AndPassOpeningSequence();
            Engine_EventWait(60);
            Event_ShowMessage(scene, 0);
            Korosseo_FadeInCompetitor(0, 736, 200);
            /* FAKEMATCH: the discarded result sets r0 after both other arguments. */
            Value3(Engine_ActorFaceDirection, ACTOR_PARTY_LEADER, 0, 0);
            KorosseoKawa_WaitForCountdown();
            Actor_SetSpeed(ACTOR_PARTY_LEADER, 0x10000, 0x8000);
            Actor_WalkToAndWait(ACTOR_PARTY_LEADER, 816, 200);
            Engine_EventWait(30);
            Actor_ShowEmote(ACTOR_PARTY_LEADER, 0x105, 60);
            Event_ShowMessage(scene, 0);
            Korosseo_RestoreCompetitor(0);
            Engine_CameraFollowActor(ACTOR_PARTY_LEADER, 0);
            SceneState_SendIdBySceneId(scene, 3);
        } else if (path == 1) {
            Engine_EventSetMessage((s32)MsgKorosseoOperatorBridgeWillAlsoCheer);
            Event_ShowMessage(scene, 0);
        }
        FieldScene_RunMiddleSequence(path, scene, 3);
        Engine_EventEnd();
    }
}

void Korosseo_RunLumberWaterIntro(s32 scene)
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
        Camera_SetSpeed(0x30000, 0x6000);
        Camera_MoveTo(PIXELS(1088), -1, PIXELS(168), 1);
        Engine_CameraWaitForMove();
        Event_ShowMessage(scene, 0);
        SceneState_StoreParamsAndInitTable(120, 72, 0);
        Event_ShowMessage(scene, 0);
        KorosseoKawa_ReleaseMarker();
        Engine_EventWait(15);
        Korosseo_FadeInCompetitor(0, 984, 200);
        Actor_FaceDirection(ACTOR_PARTY_LEADER, 0, 10);
        Event_ShowMessage(scene, 0);
        Actor_FaceDirection(ACTOR_PARTY_LEADER, 0x4000, 30);
        Actor_ShowEmote(ACTOR_PARTY_LEADER, 0x106, 60);
        Actor_SetSpeed(ACTOR_PARTY_LEADER, 0x18000, 0xc000);
        OverlayObject_PlaceWithScale14000(0, 1000, 192);
        OverlayObject_PlaceWithScale14000(0, 1000, 176);
        OverlayObject_PlaceWithScale14000(0, 1016, 168);
        Engine_EventWait(15);
        /* FAKEMATCH: the void result is discarded; Call3 changes argument allocation. */
        Value3(StagedActor_PlacePairAtOffsetAndRun, 18, 160, 0);
        Camera_MoveTo(0x4400000, -1, 0xa80000, 1);
        Engine_ActorSetAnimation(ACTOR_PARTY_LEADER, 1);
        Engine_EventWait(10);
        Actor_SetSpeed(ACTOR_PARTY_LEADER, 0x10000, 0x8000);
        Actor_WalkToAndWait(ACTOR_PARTY_LEADER, 1192, 168);
        Engine_EventWait(10);
        Actor_FaceDirection(ACTOR_PARTY_LEADER, 0x8000, 30);
        Actor_ShowEmote(ACTOR_PARTY_LEADER, 0x102, 60);
        Event_ShowMessage(scene, 0);
        Korosseo_RestoreCompetitor(0);
        Engine_CameraFollowActor(ACTOR_PARTY_LEADER, 0);
        Actor_SetPosition(18, 0x3f80000, 0xa80000);
        SceneState_SendIdBySceneId(scene, 4);
    } else if (path == 1) {
        Engine_EventSetMessage((s32)MsgKorosseoLogsKeyClearingStage);
        Event_ShowMessage(scene, 0);
    }
    FieldScene_RunMiddleSequence(path, scene, 4);
    Engine_EventEnd();
}

void SceneActor_PlaceSlots1To3FromWork(void)
{
    {
        s32 x = GameFlag_GetByte(896);
        s32 y = GameFlag_GetByte(904);

        x <<= 20;
        x += 0x80000;
        y <<= 20;
        y += 0x80000;
        Engine_ActorSetPosition(GERALD, x, y);
    }
    {
        s32 x = GameFlag_GetByte(912);
        s32 y = GameFlag_GetByte(920);

        x <<= 20;
        x += 0x80000;
        y <<= 20;
        y += 0x80000;
        Engine_ActorSetPosition(IWAN, x, y);
    }
    {
        s32 x = GameFlag_GetByte(928);
        s32 y = GameFlag_GetByte(936);

        x <<= 20;
        x += 0x80000;
        y <<= 20;
        y += 0x80000;
        Engine_ActorSetPosition(MEARI, x, y);
    }
}
