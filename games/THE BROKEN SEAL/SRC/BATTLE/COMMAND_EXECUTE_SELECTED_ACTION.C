#include "TYPES.H"
#include "BATTLE_EFFECT_RUNTIME.H"

struct BattleActionDefinition { u8 pad00[9]; u8 pp_cost; u8 pad0a[2]; u8 target_mode; };
struct BattleUnitRecord { u8 pad00[58]; s16 pp; };
struct BattleTargetCandidate { u8 pad00[4]; u16 flags; };
struct BattleCommandRuntime {
    u8 pad000[0x170]; s16 result_code; u8 pad172[0x2c]; s16 battle_mode;
    u8 pad1a0[0xb26]; u8 resolving_action;
};

/*
 * Declared struct BattleRuntime * to match the shared extern in
 * battle_effect_runtime.h; the local view of the same storage is obtained by
 * a cast below.
 */
extern struct BattleRuntime *gEventWork;
extern u8 MsgUseAbilityConfirm;
extern u8 MsgNothingHappened[];
extern u8 MsgDoesNotWorkHere[];
extern u8 MsgNotEnoughPp[];
/*
 * Returns u8 * to match the prototype shared with the other callers; the raw
 * pointer is cast to the local action-definition view below.
 */
u8 *BattleAction_Get(s32);
/*
 * Returns void * because callers view the same record through different
 * structs; each casts the shared pointer to its own view locally.
 */
void *Owner_GetStateFar(s32);
void *ObjectTable_Get(s32);
void Battle_InitializeRenderObject(void); void GameFlag_ClearBitFar(s32); s32 GameFlag_TestFar(s32);
void UiWork_PushValueSlotFar(s32, s32); void UiText_ShowPositionedMessageAndWaitFar(s32, s32);
s32 Object_CallSpawnRoutineAtOrigin(s32); void UiWork_FinalizePendingCoreFar(void);
/* Takes an s32 to match the definition of the packed effect argument. */
s32 BattleFx_ExecutePackedAbilityEffect(s32);
void Owner_AdjustSecondValueFar(s32, s32);
/*
 * Matches the shared prototype: s32-returning, with a void * out parameter.
 * Results are cast back to struct BattleTargetCandidate * here.
 */
s32 BattleFx_FindMatchingEvent(s32, s32, void *);
void GameFlag_SetBitFar(s32); s32 BattleEffect_SelectNearbyTargetObject(s32, s32); void BattleEffect_ClearOutOfBoundsObjects(void);
void BattleFx_LoadActionEffectResources(s32, s32); void BattleFx_SetupObjectPair(s32, s32); void EventObject_Initialize(void);
/*
 * Returns s32 to match the shared prototype, although every call site
 * discards the value.
 */
s32 BattleFx_RunEventAction(void *, s32, s32); void BattleFx_DispatchRequestKind(void); void BattleFx_Run(void);
void EffectRuntime_StopCurrentObject(void); void BattleFx_ClearChildValueOnMismatch(void); void BattleEffect_CleanupSceneObjects(void); void BattleEffect_ClearAllObjects(void);

/* Encoded action word: action id, acting unit and a packed-effect bit. */
#define ACTION_ID(word)       ((word) & 0x3ff)
#define ACTION_ACTOR(word)    (((word) >> 10) & 15)
#define ACTION_PACKED_EFFECT  0x2000
#define ACTOR_NONE            15
#define ACTOR_LAST_PARTY      7
#define RESULT_ABILITY_USED   999

#define FLAG_EFFECT_RUN       0x140
#define FLAG_EFFECT_DISPATCH  0x141

/* Names the actor and action, then shows a message. */
#define Command_ShowActionMessage(who, action, msg) \
    (UiWork_PushValueSlotFar((who), 1), UiWork_PushValueSlotFar((action), 4), \
     UiText_ShowPositionedMessageAndWaitFar((s32)(msg), 1))

s32 BattleCommand_ExecuteSelectedAction(u32 encodedAction)
{
    s32 actionId = ACTION_ID(encodedAction);
    struct BattleCommandRuntime *runtime = (struct BattleCommandRuntime *)gEventWork;
    struct BattleTargetCandidate *primary;
    struct BattleTargetCandidate *secondary;
    struct BattleTargetCandidate *tertiary;
    s32 actor;
    s32 targetId;
    s32 specialResult;
    s32 targetMode;
    s32 cost;
    s32 status;

    targetMode = ((struct BattleActionDefinition *)(void *)BattleAction_Get(actionId))->target_mode;
    actor = ACTION_ACTOR(encodedAction);
    ObjectTable_Get(Data_02000240.object_id);
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
        UiText_ShowPositionedMessageAndWaitFar((s32)&MsgUseAbilityConfirm, 13);
        status = Object_CallSpawnRoutineAtOrigin(1);
        UiWork_FinalizePendingCoreFar();
        if (status != 0)
            return 0;
        {
            u16 *work = (u16 *)&Data_02000240;
            s32 a, b;
            a = work[288];
            work[224] = a;
            b = work[289];
            work[225] = b;
        }
        runtime->result_code = RESULT_ABILITY_USED;
        specialResult = 1;
    }
    if (encodedAction & ACTION_PACKED_EFFECT)
        return BattleFx_ExecutePackedAbilityEffect(encodedAction);

    /* Party members pay the action's PP cost up front. */
    if (actor <= ACTOR_LAST_PARTY) {
        cost = ((struct BattleActionDefinition *)(void *)BattleAction_Get(actionId))->pp_cost;
        if (((struct BattleUnitRecord *)Owner_GetStateFar(actor))->pp < cost) {
            Command_ShowActionMessage(actor, actionId, MsgNotEnoughPp);
            if (specialResult)
                runtime->result_code = 0;
            return 0;
        }
        Owner_AdjustSecondValueFar(actor, -cost);
    }

    primary = (struct BattleTargetCandidate *)BattleFx_FindMatchingEvent(0x10000005, targetMode, &targetId);
    secondary = (struct BattleTargetCandidate *)BattleFx_FindMatchingEvent(5, targetMode, &targetId);
    tertiary = (struct BattleTargetCandidate *)BattleFx_FindMatchingEvent(0x50000005, targetMode, &targetId);
    targetId = -1;
    GameFlag_SetBitFar(FLAG_EFFECT_RUN);
    GameFlag_SetBitFar(FLAG_EFFECT_DISPATCH);
    if (primary || secondary || tertiary) {
        targetId = BattleEffect_SelectNearbyTargetObject(Data_02000240.object_id, targetMode);
        if (secondary && (secondary->flags & 0x400)) {
            GameFlag_ClearBitFar(FLAG_EFFECT_RUN);
            GameFlag_ClearBitFar(FLAG_EFFECT_DISPATCH);
        }
    } else
        GameFlag_ClearBitFar(FLAG_EFFECT_DISPATCH);

    if (runtime->battle_mode == 3)
        BattleEffect_ClearOutOfBoundsObjects();
    BattleFx_LoadActionEffectResources(actionId, 0);
    runtime->resolving_action = 1;
    BattleFx_SetupObjectPair(Data_02000240.object_id, targetId);
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
