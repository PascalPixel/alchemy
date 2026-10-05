#include "PROTO.H"
#include "FIELDRUN.H"
#include "GLOBAL_CELLS.H"
#include "TYPES.H"
#include "BATTLE_EFFECT_RUNTIME.H"
#include "BATTLE_RUNTIME.H"
#include "DMA.H"
#include "GAME_STATE.H"
#include "SCRIPT_OBJECT_RUNTIME.H"
#include "OBJECT_RUNTIME.H"
#include "IO_REG.H"
#include "BATTLE_CALC.H"
#include "text/MSG_IDS.H"

struct BattleCommandRuntime {
    u8 pad000[0x170]; s16 result_code; u8 pad172[0x2c]; s16 battle_mode;
    u8 pad1a0[0xb26]; u8 resolving_action;
};

extern struct BattleRuntime *gEventWork;

void *ObjectTable_Get(s32);
void GameFlag_ClearBitFar(s32);
void UiWork_PushValueSlotFar(s32, s32);
void UiText_ShowPositionedMessageAndWaitFar(s32, s32);
void UiWork_FinalizePendingCoreFar(void);


s32 BattleFx_FindMatchingEvent(s32, s32, void *);
void GameFlag_SetBitFar(s32);
void BattleFx_LoadActionEffectResources(s32, s32);
void BattleFx_SetupObjectPair(s32, s32);
void EventObject_Initialize(void);

void BattleFx_DispatchRequestKind(void);
void BattleFx_Run(void);
void EffectRuntime_StopCurrentObject(void);
void BattleEffect_CleanupSceneObjects(void);

#define ACTION_ID(word)       ((word) & 0x3ff)
#define ACTION_ACTOR(word)    (((word) >> 10) & 15)
#define ACTION_PACKED_EFFECT  0x2000
#define ACTOR_NONE            15
#define ACTOR_LAST_PARTY      7
#define RESULT_ABILITY_USED   999
#define FLAG_EFFECT_RUN       0x140
#define FLAG_EFFECT_DISPATCH  0x141

#define Command_ShowActionMessage(who, action, msg) \
    (UiWork_PushValueSlotFar((who), 1), UiWork_PushValueSlotFar((action), 4), \
     UiText_ShowPositionedMessageAndWaitFar((s32)(msg), 1))

s32 Map_GetTerrainHeightFar(s32, s32, s32);

struct MarkerMap {
    u8 unknown_00[0x10];
    u8 *markers;
};

struct MarkerSlot {
    struct ScriptObjectRuntime *object;
    u8 id;
    u8 unknown_05;
    u8 column;
    u8 row;
};

struct MarkerWork {
    u8 unknown_000[0x11c];
    struct MarkerSlot slots[10];
};

struct ScriptObjectRuntime *Object_CreateFar(s32 kind, s32 x, s32 y, s32 z);
void ObjectDispatch_SetSingleChildFlagsFar(struct ScriptObjectRuntime *object, s32 value);
void ObjectDispatch_ReleaseFar(struct ScriptObjectRuntime *object);
void Object_SetMode(struct ScriptObjectRuntime *object, s32 mode);
void Object_ResetMotion(struct ScriptObjectRuntime *object);

s32 BattleCommand_ExecuteSelectedAction(u32 encodedAction)
{
    s32 actionId = ACTION_ID(encodedAction);
    struct BattleCommandRuntime *runtime = (struct BattleCommandRuntime *)gEventWork;
    const struct SceneEvent *primary;
    const struct SceneEvent *secondary;
    const struct SceneEvent *tertiary;
    s32 actor;
    s32 targetId;
    s32 specialResult;
    s32 targetMode;
    s32 cost;
    s32 status;

    targetMode = BattleAction_Get(actionId)->type_0c;
    actor = ACTION_ACTOR(encodedAction);
    ObjectTable_Get(gGameState.selected_actor);
    specialResult = 0;
    Battle_InitializeRenderObject();
    GameFlag_ClearBitFar(0x145);
    if (actor == ACTOR_NONE)
        actor = 0;

    if (GameFlag_TestFar(0x17e)) {
        Command_ShowActionMessage(actor, actionId, MsgNothingHappened);
        return 0;
    }
    if (runtime->battle_mode == 3 && actionId == 0x90) {
        Command_ShowActionMessage(actor, 0x90, MsgNothingHappened);
        return 0;
    }
    if (actionId == 0x95) {
        if (GameFlag_TestFar(0x144)) {
            Command_ShowActionMessage(actor, 0x95, MsgDoesNotWorkHere);
            return 0;
        }
        UiWork_PushValueSlotFar(0x95, 4);
        UiText_ShowPositionedMessageAndWaitFar((s32)MsgUseAbilityConfirm, 13);
        status = Object_CallSpawnRoutineAtOrigin(1);
        UiWork_FinalizePendingCoreFar();
        if (status != 0)
            return 0;
        {
            struct GameState *work = &gGameState;
            s32 a, b;
            a = (u16)work->retreat_scene;
            work->scene = a;
            b = (u16)work->retreat_entrance;
            work->entrance = b;
        }
        runtime->result_code = RESULT_ABILITY_USED;
        specialResult = 1;
    }
    if (encodedAction & ACTION_PACKED_EFFECT)
        return BattleFx_ExecutePackedAbilityEffect(encodedAction);

    if (actor <= ACTOR_LAST_PARTY) {
        cost = BattleAction_Get(actionId)->pp_cost;
        if (Owner_GetStateFar(actor)->pp < cost) {
            Command_ShowActionMessage(actor, actionId, MsgNotEnoughPp);
            if (specialResult)
                runtime->result_code = 0;
            return 0;
        }
        Owner_AdjustSecondValueFar(actor, -cost);
    }

    primary = (const struct SceneEvent *)BattleFx_FindMatchingEvent(0x10000005, targetMode, &targetId);
    secondary = (const struct SceneEvent *)BattleFx_FindMatchingEvent(5, targetMode, &targetId);
    tertiary = (const struct SceneEvent *)BattleFx_FindMatchingEvent(0x50000005, targetMode, &targetId);
    targetId = -1;
    GameFlag_SetBitFar(FLAG_EFFECT_RUN);
    GameFlag_SetBitFar(FLAG_EFFECT_DISPATCH);
    if (primary || secondary || tertiary) {
        targetId = BattleEffect_SelectNearbyTargetObject(gGameState.selected_actor, targetMode);
        if (secondary && (secondary->trigger & 0x400)) {
            GameFlag_ClearBitFar(FLAG_EFFECT_RUN);
            GameFlag_ClearBitFar(FLAG_EFFECT_DISPATCH);
        }
    } else
        GameFlag_ClearBitFar(FLAG_EFFECT_DISPATCH);

    if (runtime->battle_mode == 3)
        BattleEffect_ClearOutOfBoundsObjects();
    BattleFx_LoadActionEffectResources(actionId, 0);
    runtime->resolving_action = 1;
    BattleFx_SetupObjectPair(gGameState.selected_actor, targetId);
    EventObject_Initialize();

    BattleFx_RunEventAction(primary, actor, targetId);
    if (GameFlag_TestFar(FLAG_EFFECT_RUN)) {
        if (GameFlag_TestFar(FLAG_EFFECT_DISPATCH))
            BattleFx_DispatchRequestKind();
        else
            BattleFx_Run();
    }
    EffectRuntime_StopCurrentObject();
    BattleFx_RunEventAction(secondary, actor, targetId);
    if (GameFlag_TestFar(FLAG_EFFECT_RUN))
        BattleFx_ClearChildValueOnMismatch();
    GameFlag_ClearBitFar(FLAG_EFFECT_RUN);
    GameFlag_ClearBitFar(FLAG_EFFECT_DISPATCH);
    runtime->resolving_action = 0;
    BattleEffect_CleanupSceneObjects();
    if (runtime->battle_mode == 3)
        BattleEffect_ClearAllObjects();
    return 0;
}

s32 BattleFx_HasMatchingEvent5(s32 effectId)
{
    s32 local;
    s32 result = BattleFx_FindMatchingEvent(0x70000005, (u16)effectId, &local);
    return (u32)((-result) | result) >> 31;
}

u32 BattleFx_HasTrigger(u16 effectId)
{
    s32 result;

    result = Event_FindFacingTrigger(effectId);
    return (u32)((0 - result) | result) >> 0x1F;
}

void ObjectMotion_SnapToTerrain(void *object)
{
    struct ObjectRuntime *state = object;
    s32 height;

    height = Map_GetTerrainHeightFar(0, state->x, state->z);
    state->y = height;
    state->terrain_height = height;
}

void Battle_PlaceMapMarkers(void)
{
    u32 id;
    struct MarkerMap *map = gMapWork[0];
    struct MarkerWork *work = gMapWork[27 - 8];
    s32 count = 0;
    u8 *list = map->markers;
    struct MarkerSlot *slot = work->slots;
    volatile u32 fill;
    const struct SceneEvent *event;
    struct ScriptObjectRuntime *object;
    u32 column;
    u32 row;

    fill = 0;
    Dma_Set((void *)&fill, slot, 0x85000000 | (sizeof(struct MarkerSlot) * 10 / 4), REG_DMA3);
    if (list == NULL)
        return;

    column = *list++;
    row = *list++;
    while (column != 255 || row != 255) {
        id = *list++;
        if (id < 100 || id > 239)
            goto next;
        event = gOverlayArea.events();
        for (; event->control != -1; event++) {
            if (event->trigger != id)
                continue;
            if ((event->control & 0x1ff) == 19) {
                object = Object_CreateFar(20, (column << 20) + 0x80000, 0, (row << 20) + 0x80000);
                if (object == NULL)
                    continue;
                ObjectMotion_SnapToTerrain(object);
                ObjectDispatch_SetSingleChildFlagsFar(object, 0);
                if (GameFlag_TestFar(event->condition)) {
                    if ((event->value & 0xfff00000) == 0x500000) {
                        ObjectDispatch_ReleaseFar(object);
                        continue;
                    }
                    Object_SetMode(object, 2);
                }
                Object_ResetMotion(object);
                object->home_x = object->x / 0x10000;
                object->home_z = object->z / 0x10000;
                object->unknown_23 = 1;
                object->flags_59 = 1;
                slot->id = event->trigger;
                slot->object = object;
                slot->column = object->x / 0x100000;
                slot->row = object->z / 0x100000;
                slot++;
                count++;
                if (count > 9)
                    return;
            } else if ((event->control & 0x1ff) == 3) {
                if ((event->value & 0xfff00000) != 0x300000)
                    continue;
                if (GameFlag_TestFar(event->condition))
                    continue;
                object = Object_CreateFar(28, (column << 20) + 0x80000, 0, (row << 20) + 0x80000);
                if (object == NULL)
                    continue;
                ObjectMotion_SnapToTerrain(object);
                ObjectDispatch_SetSingleChildFlagsFar(object, 0);
                Object_ResetMotion(object);
                Object_SetMode(object, 1);
                object->home_x = object->x / 0x10000;
                object->home_z = object->z / 0x10000;
                object->flags_59 = 1;
                object->unknown_23 = 1;
                slot->object = object;
                slot->id = event->trigger;
                slot->column = object->x / 0x100000;
                slot->row = object->z / 0x100000;
                slot++;
                count++;
                if (count > 9)
                    return;
            } else {
                continue;
            }
            break;
        }
    next:
        column = *list++;
        row = *list++;
    }
}
