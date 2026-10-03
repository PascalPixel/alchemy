#include "TYPES.H"
#include "EDITION.H"
#include "FIELD_EVENT.H"
#include "FIELD_SCENE.H"
#include "STAGED_ACTOR.H"
#include "SELECT_OVERLAY_DATA_BY_RUNTIME_SELECTOR.H"
#include "CALL.H"

enum {
    /* Message 0x182 + 243. */
    ITEM_RED_KEY = 243,
    /* Message 0x182 + 244. */
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

struct Slot {
    u16 f00;
    u16 f02;
    u16 f04;
    u16 f06;
};

void BattleFx_RunPageEffectForSlot(s32 actor, s32 mode, s32 frames);
s32 ArcTan2(s32, s32);
void ObjectDispatch_ApplyValueToChildren();

s32 *Engine_GetTriggerActor(s32 slot);
s32 Engine_TestTriggerFlag(s32 flag);
void Engine_SetTriggerFlag(s32 flag);

static __inline__ void SceneState_StoreStep(s16 *field, s32 step)
{
    *field = step;
}

void FieldScene_RunScene3b4_02002188(void);
void FieldScene_RunScene3b4_02002290(void);
void FieldScene_RunScene3b4_02002334(void);

extern const struct SceneEntrance gTakaraAshibaEntrances1[];
extern const struct SceneEntrance gTakaraAshibaEntrances2[];
extern const struct SceneEntrance gTakaraAshibaEntrances3[];
extern const struct SceneEntrance gTakaraAshibaEntrancesOther[];

extern const struct ScenePlacement gTakaraAshibaPlacements1[];
extern const struct ScenePlacement gTakaraAshibaPlacements2[];
extern const struct ScenePlacement gTakaraAshibaPlacements3[];
extern const struct ScenePlacement gTakaraAshibaPlacementsOther[];

extern s16 Data_02000240[];

extern const struct SceneEvent gTakaraAshibaEvents1[];
extern const struct SceneEvent gTakaraAshibaEvents2[];
extern const struct SceneEvent gTakaraAshibaEvents3[];
extern const struct SceneEvent gTakaraAshibaEventsOther[];

void Map_CopyMetatileCellsRect(s32 src_x, s32 src_y, s32 width, s32 height, s32 dest_x, s32 dest_y);

s32 PartyInventory_FindOwnerFar();
void PartyInventory_RemoveFar();
s32 PartyInventory_FindOwnerFar(s32);
void TakaraAshiba_OpenPassage(s32);

extern s32 TakaraAshiba_SlotColumns[];
extern u8 TakaraAshiba_SceneTable[];
extern const s32 TakaraAshiba_ActorFifteenMotionScript[];
extern const s32 TakaraAshiba_ActorFifteenWalkTargets[];

void Battle_Reset();
void ObjectMotion_SetSpeedParameters();
void ObjectMotion_OffsetPositionAndResetMotion();
void Battle_WaitMode0();
void ObjectMotion_ResetAndSetPosition();
void ObjectMotion_CommitCurrentPositionAndActivate();
void BattleFx_FinishAction();
void AudioCommand_Play();

void battle_owner_69();
void FieldEffect_UpdateGridPlacement();
void SceneActor_BranchOnSlotZeroAtTile38(void);

/* Repaints the lifted block's cell at a column and raises actor 8 to it. */
static __inline__ void LiftBlock(s32 column, u8 lowered)
{
    { s32 fifth = column; s32 last = 42; Engine_MapCopyCellAttributes(61, 36, 1, 1, fifth, last); }
    ((u8 *)Object_GetById(8))[85] = lowered;
    *(s32 *)((u8 *)Object_GetById(8) + 12) = 0x200000;
}

void ActorPresentation_PlaceActorFourteenOnActorNine(void);
void Scheduler_RemoveCallbackFar();
void TakaraAshiba_DispatchByActorEightColumn();
void SceneState_ApplyFourRectsAndSetActor8Byte85(void);
void SceneActor_PassActorNinePositionWithId107(void);

void SceneState_ApplyRectAndClearSlotTenByte85(void);
void SceneState_ApplyRectAndClearActor10Byte85(void);
void SceneState_ApplyTwoRectsAtRow56(void);

void TakaraAshiba_UpdateBlockRects(void);
void BattleFx_RunRisingObjectSequence(s32, s32, s32);

s32 Korosseo_ShowItemIcon(s32 slot, s32 item);
extern s32 TakaraAshiba_IconTimer;

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

/* resource_3b4 actor presentation: cell repaints for slots 11 and 12. */

/*
 * Func_ names below are loader-relocated call words in this overlay's import
 * veneer table, not runtime addresses.  The declarations are old-style
 * because the same imports are reached with differing argument counts from
 * different call sites.
 */

/*
 * Actor presentation for resource_3b4.
 *
 * A Func_ name in the import veneer band 0x02002468-0x0200261f names the
 * main-image address held in the veneer's trailing word, not a runtime
 * address the call reaches directly.  Declarations are old-style because
 * those imports are reached with differing argument counts from different
 * call sites.
 */

/* Slot record lookup, then the mode imports. */

/* Complete 16-byte actor-15 mode wrapper before the no-op leaf at 0x9ec. */
void SceneActor_SetActor15ModeZero(void)
{
    BattleFx_RunPageEffectForSlot(15, 0, 6);
}

/* Complete four-byte no-op leaf plus its alignment halfword. */
void Resource3b4_EmptyHookA(void)
{
}

/* Where the party appears on each of the island's three platforms. */
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

/* Complete four-byte leaf: movs r0,#0 followed by bx lr. */
s32 SceneData_ReturnZero(void)
{
    return 0;
}

/* Complete eight-byte literal-address getter, including its sole pool word. */
u8 *SceneData_GetSceneTable(void)
{
    return TakaraAshiba_SceneTable;
}

/* The actors placed on each of the island's three platforms. */
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

/* Complete two-byte empty hook plus its alignment halfword. */
void Resource3b4_EmptyHookB(void)
{
}

/*
 * Complete 40-byte heading update: face the supplied entity towards entity 0,
 * store the resulting angle in its +6 halfword and report zero.
 */
s32 SceneActor_FaceTowardActorZero(u8 *obj)
{
    u8 *p = Engine_GetTriggerActor(0);
    *(u16 *)(obj + 6) = (u16)ArcTan2(
        *(s32 *)(p + 16) - *(s32 *)(obj + 16),
        *(s32 *)(p + 8) - *(s32 *)(obj + 8));
    return 0;
}

void FieldScene_RunScene3b4_02000ad0(void)
{
    s32 record;

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
        record = Engine_GetTriggerActor(15);
        *(s32 *)(record + 40) = 0x80000;
        Actor_WalkToAndWait(15, 0x248, 0x2a8);
        Actor_FaceDirection(15, 0x4000, 20);
        Engine_EventEnd();
    }
}

void FieldScene_RunScene3b4_02000b68(void)
{
    s32 rec7;
    s32 rec8;
    s32 record;

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
            record = Engine_GetTriggerActor(15);
            *(s32 *)(record + 40) = 0xa0000;
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
            rec7 = Engine_GetTriggerActor(15);
            {
                s32 target = *(s32 *)(rec7 + 80);
                s32 shown = 0xf800;

                *(u16 *)(target + 30) = shown;
            }
            *(u16 *)(rec7 + 6) = 0;
            ObjectDispatch_ApplyValueToChildren(rec7, 0);
            Engine_ObjectSetScript(rec7, (s32)TakaraAshiba_ActorFifteenMotionScript);
            Engine_EventEnd();
        }
    }
}

void FieldScene_RunScene3b4_02000ccc(void)
{
    s32 record;

    if (GameFlag_IsSet(0x9c9) != 0 && GameFlag_IsSet(0x9ca) == 0) {
        GameFlag_Set(0x9ca);
        Engine_EventBegin();
        record = (s32)Object_GetById(15);
        *(u16 *)(*(s32 *)(record + 80) + 30) = 0;
        ObjectDispatch_ApplyValueToChildren(record, 16);
        Audio_PlayCue(152);
        record = Engine_GetTriggerActor(15);
        *(s32 *)(record + 40) = 0x80000;
        Actor_FaceDirection(15, 0x8000, 30);
        Actor_SetAttachedEffect(15, 0x102);
        Engine_ActorRunRepeatedMotion(15, 2);
        Engine_EventWait(20);
        Actor_SetSpeed(15, 0x10000, 0x8000);
        Audio_PlayCue(152);
        record = Engine_GetTriggerActor(15);
        *(s32 *)(record + 40) = 0x40000;
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
        record = Engine_GetTriggerActor(15);
        *(s32 *)(record + 108) = (s32)SceneActor_FaceTowardActorZero;
        Engine_EventEnd();
    }
}

void FieldScene_CopyActorPosition(void)
{
    s32 dst;
    s32 src;
    s32 idx;
    s32 tbl;
    s32 idx4;
    struct EventWork *work;

    work = gEventWork;
    if (Engine_GameFlagIsSet(0x9ca) != 0) {
        if (Data_02000240[293] != 15) {
            idx = work->touched_trigger;
            dst = (s32)Object_GetById(15);
            src = (s32)Object_GetById(0);
            *(s32 *)(dst + 48) = *(s32 *)(src + 48);
            dst = (s32)Object_GetById(15);
            src = (s32)Object_GetById(0);
            *(s32 *)(dst + 52) = *(s32 *)(src + 48);
            idx -= 30;
            tbl = (s32)TakaraAshiba_ActorFifteenWalkTargets;
            idx <<= 3;
            idx4 = idx + 4;
            Engine_ActorWalkTo(15, *(s32 *)(tbl + idx), *(s32 *)(tbl + idx4));
        }
    }
}

/* Contiguous unnamed leaf-owner run for resource_3b4. */
void FieldScene_RunActor15ZeroStep(void)
{
    Engine_EventBegin();
    Engine_ActorSetAnimation(15, 0);
    Engine_EventEnd();
}

/* What each of the island's three platforms answers. */
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
void FieldScene_CallHelper3500(void)
{
    Engine_LeaderCheckAhead();
}

/* Passage n (low byte of arg, four cells apart): with bit 8 set, play cue 157, set the work values and show the 1x3 tiles from (79, 29) for 40 frames first; then draw the tiles from (80, 29) and copy the row-40 cell attributes down to rows 41 and 42. */
void TakaraAshiba_OpenPassage(s32 arg)
{
    s32 step = (arg & 0xff) * 4;
    s32 dest = step + 77;
    s32 column = step + 13;

    if (arg & 0x100) {
        Engine_AudioPlayCue(157);
        Work_SetValuesIfNonNegative(0x20000, 0x20000, 0x10000);
        Work_SetValuesIfNonNegative(-1, -1, 0xe666);
        Map_CopyMetatileCellsRect(79, 29, 1, 3, dest, 40);
        Engine_TaskWait(40);
    }
    Map_CopyMetatileCellsRect(80, 29, 1, 3, dest, 40);
    Engine_MapCopyCellAttributes(column, 40, 1, 1, column, 41);
    Engine_MapCopyCellAttributes(column, 40, 1, 1, column, 42);
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
void FieldScene_RunScene3b4_02000fdc(s32 a0)
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
    s32 record;

    record = Engine_GetTriggerActor(0);
    if (*(u16 *)(record + 6) == 0xc000) {
        if (GameFlag_IsSet(0x9c4) == 0) {
            if (PartyInventory_FindOwnerFar(243) != -1) {
                GameFlag_Set(0x9c4);
                FieldScene_RunScene3b4_02000fdc(0x100);
                PartyInventory_RemoveFar(243);
            }
        }
    }
}

/* Four sites of one import, so four names. */
void SceneState_SetSelectorFlagWhenFacingC000(s32 selector)
{
    u8 *slot = Engine_GetTriggerActor(0);
    s32 flag;

    if (*(u16 *)(slot + 6) != 0xC000) {
        return;
    }
    flag = selector + 2496;
    if (Engine_GameFlagIsSet(flag)!= 0) {
        return;
    }
    if (PartyInventory_FindOwnerFar(244) == -1) {
        return;
    }
    Engine_GameFlagSet(flag);
    TakaraAshiba_OpenPassage(0x100 | selector);
    PartyInventory_RemoveFar(244);
}

void FieldScene_RunIndexedStep0(void)
{
    SceneState_SetSelectorFlagWhenFacingC000(0);
}

void FieldScene_RunIndexedStep1(void)
{
    SceneState_SetSelectorFlagWhenFacingC000(1);
}

void SceneState_ResetCounter412OnHeading4000B(void)
{
    extern u8 *Data_03001ebc;

    u8 *slot;
    s16 *cnt;
    s32 reset;

    SceneState_SetSelectorFlagWhenFacingC000(2);
    slot = Engine_GetTriggerActor(0);
    if (*(u16 *)(slot + 6) == 0x4000) {
        cnt = (s16 *)(Data_03001ebc + 412);
        if (*cnt > 12) {
            Engine_LeaderCheckAhead();
            reset = 0;
            *cnt = reset;
        }
    }
}

void SceneState_ResetCounter412OnHeading4000(void)
{
    extern u8 *Data_03001ebc;

    u8 *p;
    s16 *cnt;
    s32 zero;

    SceneState_SetSelectorFlagWhenFacingC000(3);
    p = Engine_GetTriggerActor(0);
    if (*(u16 *)(p + 6) == 0x4000) {
        cnt = (s16 *)(Data_03001ebc + 412);
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

/* Once the leader stands in cells x 21-23, z 10-11 while not cloaked (and the
 * game-state halfword at +0x24a is not 8), sets flag 0x220 and raises
 * trigger 91; flag 0x220 keeps it from firing again. */
void TakaraAshiba_RaiseTriggerOnStand(void)
{
    struct FieldActor *leader = (s32)Object_GetById(0);
    s32 x = leader->x.fixed / 0x100000;
    s32 z = leader->z.fixed / 0x100000;
    struct EventWork *event = gEventWork;

    if (!Value1(Engine_GameFlagIsSet, 0x220) && gGameState.cloaked == 0
        && *(s16 *)((u8 *)&gGameState + 0x24a) != 8
        && (u32)(x - 21) <= 2 && z >= 10 && z <= 11) {
        Engine_GameFlagSet(0x220);
        event->raised_trigger = 91;
    }
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
void FieldScene_RunSupplementalSequenceOne(void)
{
    u32 i;
    s32 record;

    Engine_EventBegin();
    Actor_SetSpeed(ACTOR_PARTY_LEADER, 0x1e666, 0xf333);
    Actor_SetSpeed(8, 0x1e666, 0xf333);
    Audio_PlayCue(188);
    record = Engine_GetTriggerActor(0);
    if (record != 0) {
        Actor_SetDestination(8, *(s16 *)(record + 10), *(s16 *)(record + 18));
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
    extern u8 *Data_03001ebc;
    s32 *pos = Engine_GetTriggerActor(0);
    s32 x = pos[2] / 0x100000;
    s32 z = pos[4] / 0x100000;
    u8 *work = Data_03001ebc;

    if (Value1(Engine_TestTriggerFlag, 0x220) == 0 &&
        Data_02000240[0x24c / 2] == 0 && Data_02000240[0x24a / 2] != 9 &&
        x == 10 && (u32)(z - 16) <= 2) {
        Call1(Engine_SetTriggerFlag, 0x220);
        SceneState_StoreStep((s16 *)(work + 386), 92);
    }
}

void FieldScene_RunScene3b4SequenceA(void)
{
    u32 i;
    s32 record;

    Engine_EventBegin();
    Actor_SetSpeed(ACTOR_PARTY_LEADER, 0x1b333, 0xd999);
    Actor_SetSpeed(9, 0x1b333, 0xd999);
    Audio_PlayCue(188);
    record = Engine_GetTriggerActor(0);
    if (record != 0) {
        Actor_SetDestination(9, *(s16 *)(record + 10), *(s16 *)(record + 18));
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

    s32 *pos = Engine_GetTriggerActor(0);
    s32 col = pos[2] / 0x100000;   /* +8  */
    s32 row = pos[4] / 0x100000;      /* +16 */
    s32 slot = no + 10;

    if (Data_02000240[293] == slot) return;
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
    u32 i;
    s32 record;
    s32 base3_2000240;

    base3_2000240 = (s32)Data_02000240;
    if (*(s16 *)((base3_2000240 + 0x24a)) != 10) {
        Engine_EventBegin();
        Actor_SetSpeed(ACTOR_PARTY_LEADER, 0x1b333, 0xd999);
        Actor_SetSpeed(10, 0x1b333, 0xd999);
        Audio_PlayCue(188);
        record = Engine_GetTriggerActor(0);
        if (record != 0) {
            Actor_SetDestination(10, *(s16 *)(record + 10), *(s16 *)(record + 18));
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

/* Unless the scene is 11, bring actor 11 level with the leader (0.85
 * speed), play cue 188 twice around a facing beat and walk it to (0x158,
 * 0x168). */
void TakaraAshiba_RunActorElevenFollowScene(void)
{
    s32 record;
    s32 base3_2000240;
    s32 v3;

    base3_2000240 = (s32)((u8 *)Data_02000240);
    if (*(s16 *)((base3_2000240 + 0x24a)) != 11) {
        Battle_Reset();
        Call3(ObjectMotion_SetSpeedParameters, 0, 0x1b333, 0xd999);
        ObjectMotion_SetSpeedParameters(11, 0x1b333, 0xd999);
        AudioCommand_Play(188);
        v3 = *(s32 *)((s32)Object_GetById(0) + 8) / 0x100000;
        if (v3 > *(s32 *)((s32)Object_GetById(11) + 8) / 0x100000) {
            ObjectMotion_OffsetPositionAndResetMotion(11, 8, 0);
        }
        v3 = *(s32 *)((s32)Object_GetById(0) + 8) / 0x100000;
        if (v3 < *(s32 *)((s32)Object_GetById(11) + 8) / 0x100000) {
            Call3(ObjectMotion_OffsetPositionAndResetMotion, 11, -8, 0);
        }
        ObjectMotion_CommitCurrentPositionAndActivate(11);
        record = (s32)Object_GetById(0);
        if (record != 0) {
            ObjectMotion_ResetAndSetPosition(11, *(s16 *)(record + 10), *(s16 *)(record + 18));
        }
        ObjectMotion_CommitCurrentPositionAndActivate(11);
#if EDITION_INTERNATIONAL
        ObjectMotion_OffsetPositionAndResetMotion(0, 0, 24);
#else
        ObjectMotion_OffsetPositionAndResetMotion(0, 0, 16);
#endif
        Battle_WaitMode0(4);
        AudioCommand_Play(188);
        ObjectMotion_OffsetPositionAndResetMotion(11, 0, 16);
        ObjectMotion_CommitCurrentPositionAndActivate(0);
        ObjectMotion_ResetAndSetPosition(11, 0x158, 0x168);
        ObjectMotion_CommitCurrentPositionAndActivate(11);
        Battle_WaitMode0(10);
        BattleFx_FinishAction();
    }
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
void SceneState_TriggerColumnNineteen(void)
{
    extern u8 *Data_03001ebc;
    s32 *pos = Engine_GetTriggerActor(0);
    s32 x = pos[2] / 0x100000;
    s32 z = pos[4] / 0x100000;
    u8 *work = Data_03001ebc;
    s16 *state = Data_02000240;

    if (state[0x24a / 2] != 12 && Value1(Engine_TestTriggerFlag, 0x220) == 0 &&
        state[0x24c / 2] == 0 && x == 19 && (u32)(z - 15) <= 1) {
        Call1(Engine_SetTriggerFlag, 0x220);
        SceneState_StoreStep((s16 *)(work + 386), 96);
    }
}

void FieldScene_RunScene3b4SequenceB(void)
{
    u32 i;
    s32 record;

    Engine_EventBegin();
    Actor_SetSpeed(ACTOR_PARTY_LEADER, 0x1b333, 0xd999);
    Actor_SetSpeed(12, 0x1b333, 0xd999);
    Audio_PlayCue(188);
    record = Engine_GetTriggerActor(0);
    if (record != 0) {
        Actor_SetDestination(12, *(s16 *)(record + 10), *(s16 *)(record + 18));
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
    *(u16 *)0x04000052 = coefficient;
}

/* Complete blend-alpha setter through return and its two pool words. */
void SceneEffect_SetBlendAlpha0607(void)
{
    u16 value = 0x0607;

    *(volatile u16 *)0x04000052 = value;
}

/*
 * Scene state interaction for resource_3b4.
 *
 * A Func_ name in the import veneer band 0x02002468-0x0200261f names the
 * main-image address held in the veneer's trailing word, not a runtime
 * address the call reaches directly.  Declarations are old-style because
 * those imports are reached with differing argument counts from different
 * call sites.
 */

/* Slot record lookup, then the scene flag test, clear and set imports. */

/*
 * Dispatch on slot 0's halfword at +6, the facing field, which takes the
 * values 0, 0x4000, 0x8000 and 0xc000.  The 160-byte owner includes its two
 * pool words, 0x206 and 0x207.  Two arms repaint one collision cell, but
 * only while scene flag 0x206 is set, and then move flag 0x207.  The 0x8000
 * arm does no flag work and branches on slot 0's height word at +12, read
 * from the record pointer already in hand rather than a fresh lookup.
 */
void FieldScene_DispatchBySlotZeroFacing(void)
{
    s32 *slot = Engine_GetTriggerActor(0);
    u16 facing = *(u16 *)((u8 *)slot + 6);

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
        if (slot[3] == 0) {          /* +12 */
            SceneActor_BranchOnSlotZeroAtTile38();
        } else {
            Engine_LeaderCheckAhead();
        }
    }
}

/* Deliberate no-op callback. */
void FieldScene_NoOpCallback(void) {}

void SceneActor_MarkSlot13AndSetFlag200(void)
{
    u8 *slot = Engine_GetTriggerActor(13);
    s32 fifth = 40;
    s32 sixth = 55;

    Engine_MapCopyCellAttributes(40, 54, 1, 1, fifth, sixth);
    if (slot != 0) {
        u8 *other = (u8 *)Object_GetById(13) + 85;
        u8 *flags = slot + 35;

        *other = 0;
        *flags = 2;
    }
    Engine_GameFlagSet(512);
}

/*
 * Facing target scene for resource_3b4.
 *
 * A Func_ name in the import veneer band 0x02002468-0x0200261f names the
 * main-image address held in the veneer's trailing word, not a runtime
 * address the call reaches directly.  Declarations are old-style because
 * those imports are reached with differing argument counts from different
 * call sites.
 */

/* Slot record lookup, then the notification and step imports. */
void SceneState_BranchOnActorZeroFacing(void)
{
    struct Slot *slot = Engine_GetTriggerActor(0);

    if (slot->f06 == 0) {
        Engine_LeaderCheckAhead();
    } else {
        SceneActor_BranchOnSlotZeroAtTile38();
    }
}

void SceneState_ApplyFourRectsAndSetActor8Byte85(void)
{
    Engine_MapCopyCellAttributes(57, 42, 1, 1, 40, 42);
    Engine_MapCopyCellAttributes(57, 42, 1, 1, 41, 42);
    Engine_MapCopyCellAttributes(58, 42, 1, 1, 42, 42);
    Engine_MapCopyCellAttributes(62, 37, 3, 1, 37, 42);

    ((u8 *)Object_GetById(8))[85] = 1;
}

void ActorPresentation_SetSceneCell58AndMarkActorEight(void)
{
    s32 extent = 42;
    u8 *entry;

    Engine_MapCopyCellAttributes(58, 41, 1, 1, extent, extent);
    entry = (u8 *)Object_GetById(8) + 35;
    *entry = 2;
}

void SceneState_ApplyRectAndSetActor8Byte35(void)
{
    s32 w = 41;
    s32 h = 42;
    u8 *p;

    Engine_MapCopyCellAttributes(44, 42, 1, 1, w, h);
    p = (u8 *)Object_GetById(8) + 35;
    *p = 2;
}

void SceneState_ApplyRectAndSetSlotEightByte35(void)
{
    s32 width = 40;
    s32 height = 42;
    u8 *entry;

    Engine_MapCopyCellAttributes(39, 42, 1, 1, width, height);
    entry = (u8 *)Object_GetById(8) + 35;
    *entry = 2;
}

/* Acts on the map column actor 8 stands in: marks it when it rests on the
 * ground, repaints the four gate cells, then opens the passage for columns
 * 40 to 42, or lifts the block in columns 37 to 39. Each of those three
 * columns lifts it on its own branch. */
void TakaraAshiba_DispatchByActorEightColumn(void)
{
    u8 *actor = (u8 *)Object_GetById(8);
    s32 x = *(s32 *)(actor + 8);
    s32 height = *(s32 *)(actor + 12);
    s32 column = x / 0x100000;
    u8 lowered;

    if (height == 0) {
        ((u8 *)Object_GetById(8))[35] = 2;
    }
    SceneState_ApplyFourRectsAndSetActor8Byte85();
    lowered = 0;
    ((u8 *)Object_GetById(8))[85] = 3;
    if (column == 40) {
        SceneState_ApplyRectAndSetSlotEightByte35();
    } else if (column == 42) {
        ActorPresentation_SetSceneCell58AndMarkActorEight();
    } else if (column == 41) {
        SceneState_ApplyRectAndSetActor8Byte35();
    } else if (column == 39) {
        LiftBlock(column, lowered);
    } else if (column == 38) {
        LiftBlock(column, lowered);
    } else if (column == 37) {
        LiftBlock(column, lowered);
    }
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

/*
 * Compare the X tiles of slots 0 and 8, each the word at +8 divided by
 * 0x100000.  The 100-byte owner includes its one pool word, 0x000fffff,
 * which that truncating signed division reads.  The special cases apply only
 * when slot 0 sits on tile 38 and slot 8 does not; then slot 0's halfword at
 * +6 selects one of two notifications.  Every other case, an unrecognised
 * halfword included, runs the three-step ordinary path.
 */
void SceneActor_BranchOnSlotZeroAtTile38(void)
{
    s32 *slot0 = Engine_GetTriggerActor(0);
    s32 *slot8 = Engine_GetTriggerActor(8);
    s32 x0 = slot0[2] / 0x100000;
    s32 x8 = slot8[2] / 0x100000;

    if (x0 == 38 && x8 != 38) {
        u16 facing = ((u16 *)slot0)[3];

        /*
         * The facing value is still in r0 at both branches, but whether
         * either callee reads it is unverified, so no argument is passed.
         */
        if (facing == 0xc000) {
            battle_owner_69();
            return;
        }
        if (facing == 0x4000) {
            FieldEffect_UpdateGridPlacement();
            return;
        }
    }

    SceneState_ApplyFourRectsAndSetActor8Byte85();
    StagedActor_AdvancePair();
    TakaraAshiba_DispatchByActorEightColumn();
}

void FieldScene_RunScene3b4_02001bc4(void)
{
    u32 i;
    s32 record;

    Scheduler_RemoveCallbackFar((s32)ActorPresentation_PlaceActorFourteenOnActorNine);
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
    s32 *slot = Engine_GetTriggerActor(9);
    s32 x = slot[2] / 0x100000;
    s32 z = slot[4] / 0x100000;

    if (x == 45 && z == 43) {
        FieldScene_RunScene3b4_02001bc4();
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

void SceneState_ApplyRectAndClearSlotTenByte85(void)
{
    s32 width = 38;
    s32 height = 55;
    u8 *entry;

    Engine_MapCopyCellAttributes(40, 54, 1, 1, width, height);
    entry = (u8 *)Object_GetById(10) + 85;
    *entry = 0;
}

void SceneState_ApplyRectAndClearActor10Byte85(void)
{
    s32 w = 42;
    s32 h = 55;
    u8 *p;

    Engine_MapCopyCellAttributes(40, 54, 1, 1, w, h);
    p = (u8 *)Object_GetById(10) + 85;
    *p = 0;
}

/* Tracks block 10: once it has sunk to cell height 2 or below, applies its
 * rectangle and sets flag 0x300; on row 55 applies the rectangle for columns
 * 42 and 38, and anywhere else the two rectangles at row 56. */
void TakaraAshiba_UpdateBlockRects(void)
{
    struct FieldActor *block = (s32)Object_GetById(10);
    s32 y = block->y.fixed / 0x100000;
    s32 x = block->x.fixed / 0x100000;
    s32 z = block->z.fixed / 0x100000;

    if (!Value1(Engine_GameFlagIsSet, 0x300) && y <= 2) {
        SceneState_ApplyRectAndClearSlotTenByte85();
        Engine_GameFlagSet(0x300);
    }
    if (z == 55) {
        if (x == 42) {
            SceneState_ApplyRectAndClearActor10Byte85();
        }
        if (x == 38) {
            SceneState_ApplyRectAndClearSlotTenByte85();
        }
    } else {
        SceneState_ApplyTwoRectsAtRow56();
    }
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
void FieldScene_RunThreeCallSequenceB(void)
{
    SceneState_ApplyTwoRectsAtRow56();
    StagedActor_AdvancePair();
    TakaraAshiba_UpdateBlockRects();
}

void FieldScene_DispatchByActorZeroFacing(void)
{
    struct Slot *slot = Engine_GetTriggerActor(0);

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

/*
 * Repaint the four cells, then one cell for each of slots 11 and 12 at that
 * slot's own X tile.  The 92-byte owner at 0x02001df8 includes two bytes of
 * alignment and the pool word 0x000fffff.  The tile divisions must stay
 * spelled `/ 0x100000': the reference biases a negative value before the
 * arithmetic shift, which is exactly this truncating signed division.
 */
void ActorPresentation_RepaintCellsAtActorsElevenAndTwelve(void)
{
    s32 *slot;
    s32 tile;

    SceneState_ApplyFourRectsAt48_55();

    slot = Engine_GetTriggerActor(11);
    tile = slot[2] / 0x100000;
    Engine_MapCopyCellAttributes(53, 55, 1, 1, tile, 55);
    slot = Engine_GetTriggerActor(12);
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

void FieldScene_CallHelper3c70(void)
{
    ActorPresentation_RepaintCellsAtActorsElevenAndTwelve();
}

void FieldScene_RunThreeStepSequence(void)
{
    SceneState_ApplyFourRectsAt48_55();
    StagedActor_AdvancePair();
    FieldScene_CallHelper3c70();
}

void ActorPresentation_PlaceActorFourteenOnActorNine(void)
{
    struct Actor *target = Engine_GetTriggerActor(14);
    struct Actor *source = Engine_GetTriggerActor(9);

    target->f0c = 0x200000;
    target->f08 = source->f08;
    target->f10 = source->f10 + 0x10000;
}

void SceneActor_PassActorNinePositionWithId107(void)
{
    struct Frame *frame = Engine_GetTriggerActor(9);

    Engine_MapObjectSetPosition(107, frame->f08, frame->f10 + 0x10000);
}

/*
 * Publish one marker byte at +35 to slots 8, 10, 11 and 12 according to slot
 * 0's height word at +12.  The 156-byte owner includes its one pool word,
 * 0x000fffff, read by the tile division.  The marker local is what carries
 * the value 2 across the high path, which branches over the clear to 0.
 * Slot 11's record is fetched once on each path rather than once before
 * them, and that duplication is what reproduces the reference.
 */
void SceneActor_PublishMarkerBySlotZeroHeight(void)
{
    s32 *slot0 = Engine_GetTriggerActor(0);
    u8 marker;

    if (slot0[3] > 0x100000) {                 /* +12 */
        marker = 2;
        ((u8 *)Engine_GetTriggerActor(8))[35] = marker;
        if (Engine_GetTriggerActor(10)[3] == 0) {
            ((u8 *)Engine_GetTriggerActor(10))[35] = marker;
        }
        ((u8 *)Engine_GetTriggerActor(11))[35] = marker;
    } else {
        if (Engine_GetTriggerActor(10)[3] == 0 &&
            Engine_GetTriggerActor(0)[4] / 0x100000 > 56) {   /* +16 */
            Engine_ActorSetSpritePriority(10, 3);
        } else {
            Engine_ActorSetSpritePriority(10, 1);
            ((u8 *)Engine_GetTriggerActor(10))[35] = 1;
        }
        marker = 0;
        ((u8 *)Engine_GetTriggerActor(11))[35] = marker;
    }

    ((u8 *)Engine_GetTriggerActor(12))[35] = marker;
}

/* The platforms' scene start, entry veneer 0: the screen opens through the
 * window, and each platform runs its own opening. */
s32 Scene_Initialize(void)
{
    gEventWork->start_transition = SCENE_TRANSITION(TRANSITION_WINDOW, 4);
    if (gGameState.scene == (s32)&SceneId_TakaraAshiba1) {
        FieldScene_RunScene3b4_02002188();
    }
    if (gGameState.scene == (s32)&SceneId_TakaraAshiba2) {
        FieldScene_RunScene3b4_02002290();
    }
    if (gGameState.scene == (s32)&SceneId_TakaraAshiba3) {
        FieldScene_RunScene3b4_02002334();
    }
    return 0;
}

/* Run a sixteen-frame cycle: on frame 12 return the actors whose flags are clear to their marks, and on the even frames before it show their item icons in turn. */
void TakaraAshiba_UpdateItemIcons(void)
{
    if (++TakaraAshiba_IconTimer > 16) {
        TakaraAshiba_IconTimer = 0;
    }
    switch (TakaraAshiba_IconTimer) {
    case 12:
        if (Engine_GameFlagIsSet(0xee7) == 0) {
            Call3(Engine_ActorSetPosition, 8, 0xe80000, 0x3680000);
        }
        if (Engine_GameFlagIsSet(0xee8) == 0) {
            Call3(Engine_ActorSetPosition, 9, 0x1280000, 0x3380000);
        }
        if (Engine_GameFlagIsSet(0xee9) == 0) {
            Call3(Engine_ActorSetPosition, 10, 0x1480000, 0x2f80000);
        }
        if (Engine_GameFlagIsSet(0xeea) == 0) {
            Call3(Engine_ActorSetPosition, 11, 0x1680000, 0x3680000);
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
