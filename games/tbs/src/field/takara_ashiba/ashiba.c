#include "battle.h"
#include "debug.h"
#include "field.h"
#include "game.h"
#include "graphics.h"
#include "menu.h"
#include "minigame.h"
#include "sound.h"
#include "system.h"
#include "types.h"
#include "edition.h"
#include "field_event.h"
#include "field_service.h"
#include "field_scene.h"
#include "staged_actor.h"
#include "select_overlay_data_by_runtime_selector.h"
#include "io_reg.h"

enum {
    ITEM_RED_KEY = 243,
    ITEM_BLUE_KEY = 244
};

struct Actor {
    s32 f00;
    s32 f04;
    s32 f08;
    s32 f0c;
    s32 f10;
};

struct Frame {
    s32 f00;
    s32 f04;
    s32 f08;
    s32 f0c;
    s32 f10;
};

void BattleFx_RunPageEffectForSlot(s32 actor, s32 mode, s32 frames);
s32 ArcTan2(s32, s32);
void ObjectDispatch_ApplyValueToChildren();

static __inline__ void SceneState_StoreStep(s16 *field, s32 step)
{
    *field = step;
}

void Engine_MapCopyCells(s32 src_x, s32 src_y, s32 width, s32 height, s32 dest_x, s32 dest_y);

s32 PartyInventory_FindOwner();
void PartyInventory_Remove();
s32 PartyInventory_FindOwner(s32);
void TakaraAshiba_OpenPassage(s32);

void Engine_EventBegin();
void Engine_ActorSetSpeed();
void Engine_ActorSetDestinationOffset();
void Engine_EventWait();
void Engine_ActorWaitForMove();
void Engine_EventEnd();
void Engine_AudioPlayCue();

void battle_owner_69();
void FieldEffect_UpdateGridPlacement();

static __inline__ void LiftBlock(s32 column, u8 lowered)
{
    { s32 fifth = column; s32 last = 42; Engine_MapCopyCellAttributes(61, 36, 1, 1, fifth, last); }
    Object_GetById(8)->motion_flags = lowered;
    Object_GetById(8)->y.fixed = 0x200000;
}

void TakaraAshiba_DispatchByActorEightColumn();

void TakaraAshiba_UpdateBlockRects(void);
void BattleFx_RunRisingObjectSequence(s32, s32, s32);

s32 Korosseo_ShowItemIcon(s32 slot, s32 item);

void SceneActor_SetActor15ModeZero(void)
{
    BattleFx_RunPageEffectForSlot(15, 0, 6);
}

void Resource3b4_EmptyHookA(void)
{
}

const struct SceneEntrance *Scene_GetEntrances(void)
{
    s16 selector = gGameState.scene;

    if (selector == (s32)&SceneId_TakaraAshiba1) {
        return gTakaraAshibaEntrances1;
    }
    if (selector == (s32)&SceneId_TakaraAshiba2) {
        return gTakaraAshibaEntrances2;
    }
    if (selector == (s32)&SceneId_TakaraAshiba3) {
        return gTakaraAshibaEntrances3;
    }
    return gTakaraAshibaEntrancesOther;
}

s32 SceneData_ReturnZero(void)
{
    return 0;
}

u8 *SceneData_GetSceneTable(void)
{
    return TakaraAshiba_SceneTable;
}

const struct ScenePlacement *Scene_GetPlacements(void)
{
    s16 selector = gGameState.scene;

    if (selector == (s32)&SceneId_TakaraAshiba1) {
        return gTakaraAshibaPlacements1;
    }
    if (selector == (s32)&SceneId_TakaraAshiba2) {
        return gTakaraAshibaPlacements2;
    }
    if (selector == (s32)&SceneId_TakaraAshiba3) {
        return gTakaraAshibaPlacements3;
    }
    return gTakaraAshibaPlacementsOther;
}

void Resource3b4_EmptyHookB(void)
{
}

s32 SceneActor_FaceTowardActorZero(struct FieldActor *actor)
{
    struct FieldActor *leader = Object_GetById(0);

    actor->facing = ArcTan2(leader->z.fixed - actor->z.fixed, leader->x.fixed - actor->x.fixed);
    return 0;
}

void TakaraAshiba_StartActorFifteenJump(void)
{
    struct FieldActor *record;

    if (GameFlag_IsSet(0x9c8) == 0) {
        GameFlag_Set(0x9c8);
        Engine_EventBegin();
        Camera_SetSpeed(0x20000, 0x4000);
        Engine_CameraMoveToActor(15, 1);
        Engine_CameraWaitForMove();
        Actor_FaceDirection(15, 0, 20);
        Actor_SetAttachedEffect(15, 0x102);
        Engine_ActorRunRepeatedMotion(15, 2);
        Engine_EventWait(20);
        Actor_SetSpeed(15, 0x10000, 0x8000);
        Audio_PlayCue(152);
        record = Object_GetById(15);
        record->velocity_y = 0x80000;
        Actor_WalkToAndWait(15, 0x248, 0x2a8);
        Actor_FaceDirection(15, 0x4000, 20);
        Engine_EventEnd();
    }
}

void TakaraAshiba_RunActorFifteenSprint(void)
{
    struct FieldActor *rec7;
    s32 rec8;
    struct FieldActor *record;

    if (GameFlag_IsSet(0x9c8) != 0) {
        rec8 = GameFlag_IsSet(0x9c9);
        if (rec8 == 0) {
            GameFlag_Set(0x9c9);
            Engine_EventBegin();
            Camera_SetSpeed(0x20000, 0x4000);
            Engine_CameraMoveToActor(15, 1);
            Engine_CameraWaitForMove();
            Actor_FaceDirection(15, 0x4000, 20);
            Actor_SetAttachedEffect(15, 0x102);
            Engine_ActorRunRepeatedMotion(15, 2);
            Engine_EventWait(20);
            Actor_SetSpeed(15, 0x10000, 0x8000);
            Audio_PlayCue(152);
            record = Object_GetById(15);
            record->velocity_y = 0xa0000;
            Actor_WalkToAndWait(15, 0x248, 0x298);
            Actor_FaceDirection(15, 0x4000, 20);
            Actor_SetAttachedEffect(15, 0x102);
            Engine_EventWait(30);
            Actor_SetSpeed(15, 0x80000, 0x4000);
            Actor_WalkToAndWait(15, 0x298, 0x298);
            Actor_WalkToAndWait(15, 0x2e8, 0x298);
            Actor_WalkToAndWait(15, 0x338, 0x298);
            Engine_EventWait(10);
            Audio_PlayCue(208);
            Work_SetValuesIfNonNegative(0x40000, 0x20000, 0x10000);
            Engine_EventWait(20);
            Work_SetValuesIfNonNegative(-1, -1, 0xe666);
            Engine_EventWait(30);
            Actor_SetPosition(15, 0x3780000, 0x2980000);
            rec7 = Object_GetById(15);
            rec7->sprite->rotation = 0xf800;
            rec7->facing = 0;
            ObjectDispatch_ApplyValueToChildren(rec7, 0);
            Engine_ObjectSetScript(rec7, TakaraAshiba_ActorFifteenMotionScript);
            Engine_EventEnd();
        }
    }
}

void TakaraAshiba_ReturnActorFifteen(void)
{
    struct FieldActor *record;

    if (GameFlag_IsSet(0x9c9) != 0 && GameFlag_IsSet(0x9ca) == 0) {
        GameFlag_Set(0x9ca);
        Engine_EventBegin();
        record = Object_GetById(15);
        record->sprite->rotation = 0;
        ObjectDispatch_ApplyValueToChildren(record, 16);
        Audio_PlayCue(152);
        record = Object_GetById(15);
        record->velocity_y = 0x80000;
        Actor_FaceDirection(15, 0x8000, 30);
        Actor_SetAttachedEffect(15, 0x102);
        Engine_ActorRunRepeatedMotion(15, 2);
        Engine_EventWait(20);
        Actor_SetSpeed(15, 0x10000, 0x8000);
        Audio_PlayCue(152);
        record = Object_GetById(15);
        record->velocity_y = 0x40000;
        Actor_WalkToAndWait(15, 0x370, 0x2a8);
        Engine_EventWait(10);
        Actor_SetAttachedEffect(15, 0x101);
        Actor_SetSpeed(15, 0x20000, 0x10000);
        Actor_WalkToAndWait(15, 0x370, 0x2b8);
        Actor_WalkToAndWait(15, 0x372, 0x2c0);
        Actor_WalkToAndWait(15, 0x370, 0x2c8);
        Actor_WalkToAndWait(15, 0x36e, 0x2d0);
        Actor_WalkToAndWait(15, 0x370, 0x2d8);
        Actor_WalkToAndWait(15, 0x372, 0x2e0);
        Actor_WalkToAndWait(15, 0x370, 0x2e8);
        Actor_WalkToAndWait(15, 0x36e, 0x2f0);
        Actor_WalkToAndWait(15, 0x370, 0x2f8);
        Actor_SetPosition(15, 0x3580000, 0x3380000);
        Engine_EventWait(10);
        Actor_FaceDirection(15, 0xc000, 20);
        Actor_SetAttachedEffect(15, 0x100);
        record = Object_GetById(15);
        record->update = (void (*)(union FieldObject *))SceneActor_FaceTowardActorZero;
        Engine_EventEnd();
    }
}

void FieldScene_CopyActorPosition(void)
{
    struct FieldActor *dst;
    struct FieldActor *src;
    s32 idx;
    s32 tbl;
    s32 idx4;
    struct EventWork *work;

    work = gEventWork;
    if (Engine_GameFlagIsSet(0x9ca) != 0) {
        if (gGameState.paused_object != 15) {
            idx = work->touched_trigger;
            dst = Object_GetById(15);
            src = Object_GetById(0);
            dst->speed = src->speed;
            dst = Object_GetById(15);
            src = Object_GetById(0);
            dst->acceleration = src->speed;
            idx -= 30;
            tbl = (s32)TakaraAshiba_ActorFifteenWalkTargets;
            idx <<= 3;
            idx4 = idx + 4;
            Engine_ActorWalkTo(15, *(s32 *)(tbl + idx), *(s32 *)(tbl + idx4));
        }
    }
}

void FieldScene_RunActor15ZeroStep(void)
{
    Engine_EventBegin();
    Engine_ActorSetAnimation(15, 0);
    Engine_EventEnd();
}

const struct SceneEvent *Scene_GetEvents(void)
{
    s16 selector = gGameState.scene;

    if (selector == (s32)&SceneId_TakaraAshiba1) {
        return gTakaraAshibaEvents1;
    }
    if (selector == (s32)&SceneId_TakaraAshiba2) {
        return gTakaraAshibaEvents2;
    }
    if (selector == (s32)&SceneId_TakaraAshiba3) {
        return gTakaraAshibaEvents3;
    }
    return gTakaraAshibaEventsOther;
}

void TakaraAshiba_CheckLeaderAhead(void)
{
    Engine_LeaderCheckAhead();
}

void TakaraAshiba_OpenPassage(s32 arg)
{
    s32 step = (arg & 0xff) * 4;
    s32 dest = step + 77;
    s32 column = step + 13;

    if (arg & 0x100) {
        Engine_AudioPlayCue(157);
        Work_SetValuesIfNonNegative(0x20000, 0x20000, 0x10000);
        Work_SetValuesIfNonNegative(-1, -1, 0xe666);
        Engine_MapCopyCells(79, 29, 1, 3, dest, 40);
        Engine_TaskWait(40);
    }
    Engine_MapCopyCells(80, 29, 1, 3, dest, 40);
    Engine_MapCopyCellAttributes(column, 40, 1, 1, column, 41);
    Engine_MapCopyCellAttributes(column, 40, 1, 1, column, 42);
}

void TakaraAshiba_OpenPlatformPassage(s32 a0)
{
    u32 i;
    s32 record;

    if ((a0 & 0x100) != 0) {
        Audio_PlayCue(157);
        Work_SetValuesIfNonNegative(0x20000, 0x20000, 0x10000);
        Work_SetValuesIfNonNegative(-1, -1, 0xe666);
        Map_CopyCells(84, 29, 1, 3, 70, 49);
        Engine_TaskWait(60);
    }
    Map_CopyCells(85, 29, 1, 3, 70, 49);
    Map_CopyCellAttributes(6, 49, 1, 1, 6, 50);
    Map_CopyCellAttributes(6, 49, 1, 1, 6, 51);
}

void FieldScene_RunScene3b4SequenceC(void)
{
    u32 i;
    struct FieldActor *record;

    record = Object_GetById(0);
    if (record->facing == 0xc000) {
        if (GameFlag_IsSet(0x9c4) == 0) {
            if (PartyInventory_FindOwner(243) != -1) {
                GameFlag_Set(0x9c4);
                TakaraAshiba_OpenPlatformPassage(0x100);
                PartyInventory_Remove(243);
            }
        }
    }
}

void SceneState_SetSelectorFlagWhenFacingC000(s32 selector)
{
    struct FieldActor *slot = Object_GetById(0);
    s32 flag;

    if (slot->facing != 0xC000) {
        return;
    }
    flag = selector + 2496;
    if (Engine_GameFlagIsSet(flag)!= 0) {
        return;
    }
    if (PartyInventory_FindOwner(244) == -1) {
        return;
    }
    Engine_GameFlagSet(flag);
    TakaraAshiba_OpenPassage(0x100 | selector);
    PartyInventory_Remove(244);
}

void FieldScene_RunIndexedStep0(void)
{
    SceneState_SetSelectorFlagWhenFacingC000(0);
}

void FieldScene_RunIndexedStep1(void)
{
    SceneState_SetSelectorFlagWhenFacingC000(1);
}

void TakaraAshiba_ResetBlockedStepsForSelectorTwo(void)
{
    struct FieldActor *slot;
    s16 *cnt;
    s32 reset;

    SceneState_SetSelectorFlagWhenFacingC000(2);
    slot = Object_GetById(0);
    if (slot->facing == 0x4000) {
        cnt = &gEventWork->blocked_steps;
        if (*cnt > 12) {
            Engine_LeaderCheckAhead();
            reset = 0;
            *cnt = reset;
        }
    }
}

void TakaraAshiba_ResetBlockedStepsForSelectorThree(void)
{
    struct FieldActor *p;
    s16 *cnt;
    s32 zero;

    SceneState_SetSelectorFlagWhenFacingC000(3);
    p = Object_GetById(0);
    if (p->facing == 0x4000) {
        cnt = &gEventWork->blocked_steps;
        if (*cnt > 12) {
            Engine_LeaderCheckAhead();
            zero = 0;
            *cnt = zero;
        }
    }
}

void SceneState_ApplyRectAndPlaceSlot12(void)
{
    s32 a = 25;
    s32 b = 48;
    s32 slot = 12;
    s32 x = 0x1980000;
    s32 z = 0x3080000;

    Engine_MapCopyCells(25, 45, 1, 2, a, b);
    if (Engine_GameFlagIsSet(0xeeb) == 0)
        Engine_ActorSetPosition(slot, x, z);
    Engine_EventWait(1);
}

void ConfigureAndPlaceActorTwelve(void)
{
    s32 a = 25, b = 48;
    Map_CopyCells(24, 48, 1, 2, a, b);
    Actor_SetPosition(12, 0x00080000, 0x00080000);
}

void FieldScene_RunStep8ValueEe7(void)
{
    Engine_ItemShowFound(ITEM_BLUE_KEY, 3);
    Engine_ActorSetAnimation(ACTOR_PARTY_LEADER, 1);
    Engine_PartyGiveItem(ITEM_BLUE_KEY, 0);
    Engine_ActorSetPosition(8, 0, 0);
    Engine_GameFlagSet(0xEE7);
}

void FieldScene_RunStep9ValueEe8(void)
{
    Engine_ItemShowFound(ITEM_BLUE_KEY, 3);
    Engine_ActorSetAnimation(ACTOR_PARTY_LEADER, 1);
    Engine_PartyGiveItem(ITEM_BLUE_KEY, 0);
    Engine_ActorSetPosition(9, 0, 0);
    Engine_GameFlagSet(0xEE8);
}

void FieldScene_RunStep10ValueEe9(void)
{
    Engine_ItemShowFound(ITEM_BLUE_KEY, 3);
    Engine_ActorSetAnimation(ACTOR_PARTY_LEADER, 1);
    Engine_PartyGiveItem(ITEM_BLUE_KEY, 0);
    Engine_ActorSetPosition(0xA, 0, 0);
    Engine_GameFlagSet(0xEE9);
}

void FieldScene_RunStep11ValueEea(void)
{
    Engine_ItemShowFound(ITEM_BLUE_KEY, 3);
    Engine_ActorSetAnimation(ACTOR_PARTY_LEADER, 1);
    Engine_PartyGiveItem(ITEM_BLUE_KEY, 0);
    Engine_ActorSetPosition(0xB, 0, 0);
    Engine_GameFlagSet(0xEEA);
}

void FieldScene_RunStep12ValueEeb(void)
{
    Engine_ItemShowFound(ITEM_RED_KEY, 3);
    Engine_ActorSetAnimation(ACTOR_PARTY_LEADER, 1);
    Engine_PartyGiveItem(ITEM_RED_KEY, 0);
    Engine_ActorSetPosition(0xC, 0, 0);
    Engine_GameFlagSet(0xEEB);
}

void TakaraAshiba_RaiseTriggerOnStand(void)
{
    struct FieldActor *leader = Object_GetById(ACTOR_PARTY_LEADER);
    s32 x = leader->x.fixed / 0x100000;
    s32 z = leader->z.fixed / 0x100000;
    struct EventWork *event = gEventWork;

    if (!GameFlag_IsSet(0x220) && gGameState.cloaked == 0
        && gGameState.paused_object != 8
        && (u32)(x - 21) <= 2 && z >= 10 && z <= 11) {
        Engine_GameFlagSet(0x220);
        event->raised_trigger = 91;
    }
}

void FieldScene_RunSupplementalSequenceOne(void)
{
    u32 i;
    struct FieldActor *record;

    Engine_EventBegin();
    Actor_SetSpeed(ACTOR_PARTY_LEADER, 0x1e666, 0xf333);
    Actor_SetSpeed(8, 0x1e666, 0xf333);
    Audio_PlayCue(188);
    record = Object_GetById(0);
    if (record != 0) {
        Actor_SetDestination(8, record->x.part.pixel, record->z.part.pixel);
    }
    Engine_ActorWaitForMove(8);
    Actor_SetDestinationOffset(ACTOR_PARTY_LEADER, 0, 24);
    Engine_EventWait(4);
    Audio_PlayCue(188);
#if EDITION_INTERNATIONAL
    Actor_SetDestinationOffset(8, 0, 16);
#else
    Actor_SetDestinationOffset(8, 0, 24);
#endif
    Engine_ActorWaitForMove(ACTOR_PARTY_LEADER);
    Actor_SetDestination(8, 0x168, 152);
    Engine_ActorWaitForMove(8);
    Engine_EventEnd();
    GameFlag_Clear(0x220);
}

void SceneState_TriggerColumnTen(void)
{
    struct FieldActor *leader = Object_GetById(ACTOR_PARTY_LEADER);
    s32 x = leader->x.fixed / 0x100000;
    s32 z = leader->z.fixed / 0x100000;
    struct EventWork *work = gEventWork;

    if (GameFlag_IsSet(0x220) == 0 &&
        gGameState.cloaked == 0 && gGameState.paused_object != 9 &&
        x == 10 && (u32)(z - 16) <= 2) {
        GameFlag_Set(0x220);
        SceneState_StoreStep(&work->raised_trigger, 92);
    }
}

void FieldScene_RunScene3b4SequenceA(void)
{
    u32 i;
    struct FieldActor *record;

    Engine_EventBegin();
    Actor_SetSpeed(ACTOR_PARTY_LEADER, 0x1b333, 0xd999);
    Actor_SetSpeed(9, 0x1b333, 0xd999);
    Audio_PlayCue(188);
    record = Object_GetById(0);
    if (record != 0) {
        Actor_SetDestination(9, record->x.part.pixel, record->z.part.pixel);
    }
    Engine_ActorWaitForMove(9);
#if EDITION_INTERNATIONAL
    Actor_SetDestinationOffset(ACTOR_PARTY_LEADER, 0, 24);
#else
    Actor_SetDestinationOffset(ACTOR_PARTY_LEADER, 0, 16);
#endif
    Audio_PlayCue(188);
    Engine_EventWait(4);
    Actor_SetDestinationOffset(9, 0, 16);
    Engine_ActorWaitForMove(ACTOR_PARTY_LEADER);
    Actor_SetDestination(9, 168, 0x108);
    Engine_ActorWaitForMove(9);
    Engine_EventEnd();
    GameFlag_Clear(0x220);
}

void SceneActor_TrackOriginColumnForSlot(s32 no)
{

    struct FieldActor *leader = Object_GetById(ACTOR_PARTY_LEADER);
    s32 col = leader->x.fixed / 0x100000;
    s32 row = leader->z.fixed / 0x100000;
    s32 slot = no + 10;

    if (gGameState.paused_object == slot) return;
    if (col == TakaraAshiba_SlotColumns[no]) return;

    Engine_ActorSetSpeed(slot, 0x48000, 0x24000);
    Engine_AudioPlayCue(188);
    Engine_ActorSetDestination(slot, (col << 4) + 8, 360);

    TakaraAshiba_SlotColumns[no] = col;

    if (row <= 22) {
        Engine_ActorSetDestinationOffset(ACTOR_PARTY_LEADER, 0, 8);
    }
    Engine_ActorWaitForMove(ACTOR_PARTY_LEADER);
}

void FieldScene_RunLateIndexedStep0(void)
{
    SceneActor_TrackOriginColumnForSlot(0);
}

void FieldScene_RunLateIndexedStep1(void)
{
    SceneActor_TrackOriginColumnForSlot(1);
}

void FieldScene_RunLateIndexedStep2(void)
{
    SceneActor_TrackOriginColumnForSlot(2);
}

void FieldScene_RunPrimarySequence(void)
{
    struct FieldActor *record;

    if (gGameState.paused_object != 10) {
        Engine_EventBegin();
        Actor_SetSpeed(ACTOR_PARTY_LEADER, 0x1b333, 0xd999);
        Actor_SetSpeed(10, 0x1b333, 0xd999);
        Audio_PlayCue(188);
        record = Object_GetById(0);
        if (record != 0) {
            Actor_SetDestination(10, record->x.part.pixel, record->z.part.pixel);
        }
        Engine_ActorWaitForMove(10);
    #if EDITION_INTERNATIONAL
    Actor_SetDestinationOffset(ACTOR_PARTY_LEADER, 0, 24);
#else
    Actor_SetDestinationOffset(ACTOR_PARTY_LEADER, 0, 16);
#endif
        Engine_EventWait(4);
        Audio_PlayCue(188);
        Actor_SetDestinationOffset(10, 0, 16);
        Engine_ActorWaitForMove(ACTOR_PARTY_LEADER);
        Actor_SetDestination(10, 0x108, 0x168);
        Engine_ActorWaitForMove(10);
        Engine_EventWait(10);
        Engine_EventEnd();
    }
}

void TakaraAshiba_RunActorElevenFollowScene(void)
{
    struct FieldActor *record;
    s32 v3;

    if (gGameState.paused_object != 11) {
        Engine_EventBegin();
        Actor_SetSpeed(0, 0x1b333, 0xd999);
        Engine_ActorSetSpeed(11, 0x1b333, 0xd999);
        Engine_AudioPlayCue(188);
        v3 = Object_GetById(0)->x.fixed / 0x100000;
        if (v3 > Object_GetById(11)->x.fixed / 0x100000) {
            Engine_ActorSetDestinationOffset(11, 8, 0);
        }
        v3 = Object_GetById(0)->x.fixed / 0x100000;
        if (v3 < Object_GetById(11)->x.fixed / 0x100000) {
            FieldObjectMotion_OffsetPositionAndResetMotion(11, -8, 0);
        }
        Engine_ActorWaitForMove(11);
        record = Object_GetById(0);
        if (record != 0) {
            Engine_ActorSetDestination(11, record->x.part.pixel, record->z.part.pixel);
        }
        Engine_ActorWaitForMove(11);
#if EDITION_INTERNATIONAL
        Engine_ActorSetDestinationOffset(0, 0, 24);
#else
        Engine_ActorSetDestinationOffset(0, 0, 16);
#endif
        Engine_EventWait(4);
        Engine_AudioPlayCue(188);
        Engine_ActorSetDestinationOffset(11, 0, 16);
        Engine_ActorWaitForMove(0);
        Engine_ActorSetDestination(11, 0x158, 0x168);
        Engine_ActorWaitForMove(11);
        Engine_EventWait(10);
        Engine_EventEnd();
    }
}

void SceneState_TriggerColumnNineteen(void)
{
    struct FieldActor *leader = Object_GetById(ACTOR_PARTY_LEADER);
    s32 x = leader->x.fixed / 0x100000;
    s32 z = leader->z.fixed / 0x100000;
    struct EventWork *work = gEventWork;
    struct GameState *state = &gGameState;

    if (state->paused_object != 12 && GameFlag_IsSet(0x220) == 0 &&
        state->cloaked == 0 && x == 19 && (u32)(z - 15) <= 1) {
        GameFlag_Set(0x220);
        SceneState_StoreStep(&work->raised_trigger, 96);
    }
}

void FieldScene_RunScene3b4SequenceB(void)
{
    u32 i;
    struct FieldActor *record;

    Engine_EventBegin();
    Actor_SetSpeed(ACTOR_PARTY_LEADER, 0x1b333, 0xd999);
    Actor_SetSpeed(12, 0x1b333, 0xd999);
    Audio_PlayCue(188);
    record = Object_GetById(0);
    if (record != 0) {
        Actor_SetDestination(12, record->x.part.pixel, record->z.part.pixel);
    }
    Engine_ActorWaitForMove(12);
#if EDITION_INTERNATIONAL
    Actor_SetDestinationOffset(ACTOR_PARTY_LEADER, 0, 24);
#else
    Actor_SetDestinationOffset(ACTOR_PARTY_LEADER, 0, 16);
#endif
    Audio_PlayCue(188);
    Actor_SetDestinationOffset(12, 0, 16);
    Engine_ActorWaitForMove(ACTOR_PARTY_LEADER);
    Actor_SetDestination(12, 0x138, 232);
    Engine_ActorWaitForMove(12);
    Engine_EventEnd();
    GameFlag_Clear(0x220);
}

void SetBlendAlphaCoefficients(void)
{
    u32 coefficient = 208;

    coefficient <<= 4;
    REG_BLDALPHA = coefficient;
}

void SceneEffect_SetBlendAlpha0607(void)
{
    u16 value = 0x0607;

    REG_BLDALPHA = value;
}

void FieldScene_DispatchBySlotZeroFacing(void)
{
    struct FieldActor *slot = Object_GetById(ACTOR_PARTY_LEADER);
    u16 facing = slot->facing;

    if (facing == 0xc000) {
        if (Engine_GameFlagIsSet(0x206) != 0) {
            { s32 fifth = 45; s32 last = 43; Engine_MapCopyCellAttributes(46, 43, 1, 1, fifth, last); }
        }
        Engine_GameFlagClear(0x207);
        battle_owner_69();
    } else if (facing == 0x4000) {
        FieldEffect_UpdateGridPlacement();
    } else if (facing == 0) {
        if (Engine_GameFlagIsSet(0x206) != 0) {
            { s32 fifth = 45; s32 last = 43; Engine_MapCopyCellAttributes(58, 36, 1, 1, fifth, last); }
        }
        Engine_GameFlagSet(0x207);
        Engine_LeaderCheckAhead();
    } else if (facing == 0x8000) {
        if (slot->y.fixed == 0) {
            SceneActor_BranchOnSlotZeroAtTile38();
        } else {
            Engine_LeaderCheckAhead();
        }
    }
}

void FieldScene_NoOpCallback(void) {}

void SceneActor_MarkSlot13AndSetFlag200(void)
{
    struct FieldActor *slot = Object_GetById(13);
    s32 fifth = 40;
    s32 sixth = 55;

    Engine_MapCopyCellAttributes(40, 54, 1, 1, fifth, sixth);
    if (slot != 0) {
        u8 *other = &Object_GetById(13)->motion_flags;
        u8 *flags = &slot->priority_flags;

        *other = 0;
        *flags = 2;
    }
    Engine_GameFlagSet(512);
}

void SceneState_BranchOnActorZeroFacing(void)
{
    struct Slot *slot = Object_GetById(0);

    if (slot->f06 == 0) {
        Engine_LeaderCheckAhead();
    } else {
        SceneActor_BranchOnSlotZeroAtTile38();
    }
}

void TakaraAshiba_RestoreActorEightTerrain(void)
{
    Engine_MapCopyCellAttributes(57, 42, 1, 1, 40, 42);
    Engine_MapCopyCellAttributes(57, 42, 1, 1, 41, 42);
    Engine_MapCopyCellAttributes(58, 42, 1, 1, 42, 42);
    Engine_MapCopyCellAttributes(62, 37, 3, 1, 37, 42);

    Object_GetById(8)->motion_flags = 1;
}

void ActorPresentation_SetSceneCell58AndMarkActorEight(void)
{
    s32 extent = 42;

    Engine_MapCopyCellAttributes(58, 41, 1, 1, extent, extent);
    Object_GetById(8)->priority_flags = 2;
}

void TakaraAshiba_RestoreRightUnderfootCell(void)
{
    s32 w = 41;
    s32 h = 42;

    Engine_MapCopyCellAttributes(44, 42, 1, 1, w, h);
    Object_GetById(8)->priority_flags = 2;
}

void TakaraAshiba_RestoreLeftUnderfootCell(void)
{
    s32 width = 40;
    s32 height = 42;

    Engine_MapCopyCellAttributes(39, 42, 1, 1, width, height);
    Object_GetById(8)->priority_flags = 2;
}

void TakaraAshiba_DispatchByActorEightColumn(void)
{
    struct FieldActor *actor = Object_GetById(8);
    s32 x = actor->x.fixed;
    s32 height = actor->y.fixed;
    s32 column = x / 0x100000;
    u8 lowered;

    if (height == 0) {
        Object_GetById(8)->priority_flags = 2;
    }
    TakaraAshiba_RestoreActorEightTerrain();
    lowered = 0;
    Object_GetById(8)->motion_flags = 3;
    if (column == 40) {
        TakaraAshiba_RestoreLeftUnderfootCell();
    } else if (column == 42) {
        ActorPresentation_SetSceneCell58AndMarkActorEight();
    } else if (column == 41) {
        TakaraAshiba_RestoreRightUnderfootCell();
    } else if (column == 39) {
        LiftBlock(column, lowered);
    } else if (column == 38) {
        LiftBlock(column, lowered);
    } else if (column == 37) {
        LiftBlock(column, lowered);
    }
}

void SceneActor_BranchOnSlotZeroAtTile38(void)
{
    s32 *slot0 = Object_GetById(0);
    s32 *slot8 = Object_GetById(8);
    s32 x0 = slot0[2] / 0x100000;
    s32 x8 = slot8[2] / 0x100000;

    if (x0 == 38 && x8 != 38) {
        u16 facing = ((u16 *)slot0)[3];

        if (facing == 0xc000) {
            battle_owner_69();
            return;
        }
        if (facing == 0x4000) {
            FieldEffect_UpdateGridPlacement();
            return;
        }
    }

    TakaraAshiba_RestoreActorEightTerrain();
    StagedActor_AdvancePair();
    TakaraAshiba_DispatchByActorEightColumn();
}

void TakaraAshiba_ReleaseActorNinePlatform(void)
{
    u32 i;
    s32 record;

    Scheduler_RemoveCallback((s32)ActorPresentation_PlaceActorFourteenOnActorNine);
    Actor_SetPosition(14, 0, 0);
    if (GameFlag_IsSet(0x207) != 0) {
        Map_CopyCellAttributes(58, 36, 1, 1, 45, 43);
    } else {
        Map_CopyCellAttributes(46, 43, 1, 1, 45, 43);
    }
    SceneActor_PassActorNinePositionWithId107();
    GameFlag_Set(0x206);
}

void SceneActor_RunWhenActor9AtTile45x43(void)
{
    s32 *slot = Object_GetById(9);
    s32 x = slot[2] / 0x100000;
    s32 z = slot[4] / 0x100000;

    if (x == 45 && z == 43) {
        TakaraAshiba_ReleaseActorNinePlatform();
    }
}

void FieldScene_RunTwoStepSequence(void)
{
    StagedActor_AdvancePair();
    SceneActor_RunWhenActor9AtTile45x43();
}

void SceneState_ApplyTwoRectsAtRow56(void)
{
    s32 base = 55;

    Engine_MapCopyCellAttributes(38, 56, 1, 1, 38, base);
    Engine_MapCopyCellAttributes(42, 56, 1, 1, 42, base);
}

void TakaraAshiba_RestoreActorTenLeftCell(void)
{
    s32 width = 38;
    s32 height = 55;

    Engine_MapCopyCellAttributes(40, 54, 1, 1, width, height);
    Object_GetById(10)->motion_flags = 0;
}

void TakaraAshiba_RestoreActorTenRightCell(void)
{
    s32 w = 42;
    s32 h = 55;

    Engine_MapCopyCellAttributes(40, 54, 1, 1, w, h);
    Object_GetById(10)->motion_flags = 0;
}

void TakaraAshiba_UpdateBlockRects(void)
{
    struct FieldActor *block = Object_GetById(10);
    s32 y = block->y.fixed / 0x100000;
    s32 x = block->x.fixed / 0x100000;
    s32 z = block->z.fixed / 0x100000;

    if (!GameFlag_IsSet(0x300) && y <= 2) {
        TakaraAshiba_RestoreActorTenLeftCell();
        Engine_GameFlagSet(0x300);
    }
    if (z == 55) {
        if (x == 42) {
            TakaraAshiba_RestoreActorTenRightCell();
        }
        if (x == 38) {
            TakaraAshiba_RestoreActorTenLeftCell();
        }
    } else {
        SceneState_ApplyTwoRectsAtRow56();
    }
}

void FieldScene_RunThreeCallSequenceB(void)
{
    SceneState_ApplyTwoRectsAtRow56();
    StagedActor_AdvancePair();
    TakaraAshiba_UpdateBlockRects();
}

void FieldScene_DispatchByActorZeroFacing(void)
{
    struct Slot *slot = Object_GetById(0);

    if (slot->f06 == 0x4000) {
        BattleFx_RunRisingObjectSequence(0, 6, 0);
    } else {
        FieldScene_RunThreeCallSequenceB();
    }
}

void SceneState_ApplyFourRectsAt48_55(void)
{
    s32 base = 55;

    Engine_MapCopyCellAttributes(48, 55, 1, 1, 49, base);
    Engine_MapCopyCellAttributes(48, 55, 1, 1, 50, base);
    Engine_MapCopyCellAttributes(48, 55, 1, 1, 51, base);
    Engine_MapCopyCellAttributes(48, 55, 1, 1, 52, base);
}

void ActorPresentation_RepaintCellsAtActorsElevenAndTwelve(void)
{
    s32 *slot;
    s32 tile;

    SceneState_ApplyFourRectsAt48_55();

    slot = Object_GetById(11);
    tile = slot[2] / 0x100000;
    Engine_MapCopyCellAttributes(53, 55, 1, 1, tile, 55);
    slot = Object_GetById(12);
    tile = slot[2] / 0x100000;
    Engine_MapCopyCellAttributes(53, 55, 1, 1, tile, 55);
}

void FieldScene_RunSingleStep(void)
{
    ActorPresentation_RepaintCellsAtActorsElevenAndTwelve();
}

void FieldScene_RunThreeCallSequence(void)
{
    SceneState_ApplyFourRectsAt48_55();
    StagedActor_AdvancePair();
    FieldScene_RunSingleStep();
}

void TakaraAshiba_RepaintActorCells(void)
{
    ActorPresentation_RepaintCellsAtActorsElevenAndTwelve();
}

void FieldScene_RunThreeStepSequence(void)
{
    SceneState_ApplyFourRectsAt48_55();
    StagedActor_AdvancePair();
    TakaraAshiba_RepaintActorCells();
}

void ActorPresentation_PlaceActorFourteenOnActorNine(void)
{
    struct Actor *target = Object_GetById(14);
    struct Actor *source = Object_GetById(9);

    target->f0c = 0x200000;
    target->f08 = source->f08;
    target->f10 = source->f10 + 0x10000;
}

void SceneActor_PassActorNinePositionWithId107(void)
{
    struct Frame *frame = Object_GetById(9);

    Engine_MapObjectSetPosition(107, frame->f08, frame->f10 + 0x10000);
}

void SceneActor_PublishMarkerBySlotZeroHeight(void)
{
    struct FieldActor *leader = Object_GetById(0);
    u8 priority;

    if (leader->y.fixed > 0x100000) {
        priority = ACTOR_PRIORITY_UNDERFOOT;
        Object_GetById(8)->priority_flags = priority;
        if (Object_GetById(10)->y.fixed == 0)
            Object_GetById(10)->priority_flags = priority;
        Object_GetById(11)->priority_flags = priority;
    } else {
        if (Object_GetById(10)->y.fixed == 0 && Object_GetById(0)->z.fixed / 0x100000 > 56) {
            Engine_ActorSetSpritePriority(10, 3);
        } else {
            Engine_ActorSetSpritePriority(10, 1);
            Object_GetById(10)->priority_flags = ACTOR_PRIORITY_AUTOMATIC;
        }
        priority = 0;
        Object_GetById(11)->priority_flags = priority;
    }
    Object_GetById(12)->priority_flags = priority;
}

s32 Scene_Initialize(void)
{
    gEventWork->start_transition = SCENE_TRANSITION(TRANSITION_WINDOW, 4);
    if (gGameState.scene == (s32)&SceneId_TakaraAshiba1) {
        TakaraAshiba_InitializeScene1();
    }
    if (gGameState.scene == (s32)&SceneId_TakaraAshiba2) {
        TakaraAshiba_InitializeScene2();
    }
    if (gGameState.scene == (s32)&SceneId_TakaraAshiba3) {
        TakaraAshiba_InitializeScene3();
    }
    return 0;
}

void TakaraAshiba_UpdateItemIcons(void)
{
    if (++TakaraAshiba_IconTimer > 16) {
        TakaraAshiba_IconTimer = 0;
    }
    switch (TakaraAshiba_IconTimer) {
    case 12:
        if (Engine_GameFlagIsSet(0xee7) == 0) {
            Actor_SetPosition(8, 0xe80000, 0x3680000);
        }
        if (Engine_GameFlagIsSet(0xee8) == 0) {
            Actor_SetPosition(9, 0x1280000, 0x3380000);
        }
        if (Engine_GameFlagIsSet(0xee9) == 0) {
            Actor_SetPosition(10, 0x1480000, 0x2f80000);
        }
        if (Engine_GameFlagIsSet(0xeea) == 0) {
            Actor_SetPosition(11, 0x1680000, 0x3680000);
        }
        break;
    case 10:
        Korosseo_ShowItemIcon(8, 0);
        break;
    case 8:
        Korosseo_ShowItemIcon(9, 0);
        break;
    case 6:
        Korosseo_ShowItemIcon(10, 0);
        break;
    case 4:
        Korosseo_ShowItemIcon(11, 0);
        break;
    case 2:
        Korosseo_ShowItemIcon(12, 1);
        break;
    }
}
