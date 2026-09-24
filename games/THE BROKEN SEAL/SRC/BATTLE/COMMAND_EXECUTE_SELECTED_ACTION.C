#include "TYPES.H"
#include "BATTLE_EFFECT_RUNTIME.H"
#define PARTY_STATE gGameState

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
extern u8 Value_00000920;
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
void Func_08096fb0(s32, s32); void BattleFx_SetupObjectPair(s32, s32); void EventObject_Initialize(void);
/*
 * Returns s32 to match the shared prototype, although every call site
 * discards the value.
 */
s32 BattleFx_RunEventAction(void *, s32, s32); void BattleFx_DispatchRequestKind(void); void BattleFx_Run(void);
void EffectRuntime_StopCurrentObject(void); void BattleFx_ClearChildValueOnMismatch(void); void BattleEffect_CleanupSceneObjects(void); void Func_0808b98c(void);

s32 BattleCommand_ExecuteSelectedAction(u32 encodedAction)
{
    s32 actionId = encodedAction & 0x3ff;
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
    actor = (encodedAction >> 10) & 15;
    ObjectTable_Get(PARTY_STATE.object_id);
    specialResult = 0;
    Battle_InitializeRenderObject();
    GameFlag_ClearBitFar(0x145);
    if (actor == 15) actor = 0;

    if (GameFlag_TestFar(0x17e)) {
        UiWork_PushValueSlotFar(actor, 1); UiWork_PushValueSlotFar(actionId, 4); UiText_ShowPositionedMessageAndWaitFar(0x91f, 1);
        return 0;
    }
    if (runtime->battle_mode == 3 && actionId == 0x90) {
        UiWork_PushValueSlotFar(actor, 1); UiWork_PushValueSlotFar(0x90, 4); UiText_ShowPositionedMessageAndWaitFar(0x91f, 1);
        return 0;
    }
    if (actionId == 0x95) {
        if (GameFlag_TestFar(0x144)) {
            UiWork_PushValueSlotFar(actor, 1); UiWork_PushValueSlotFar(0x95, 4); UiText_ShowPositionedMessageAndWaitFar(0x921, 1);
            return 0;
        }
        UiWork_PushValueSlotFar(0x95, 4); UiText_ShowPositionedMessageAndWaitFar((s32)&Value_00000920, 13);
        status = Object_CallSpawnRoutineAtOrigin(1); UiWork_FinalizePendingCoreFar();
        if (status != 0) return 0;
        {
            u16 *work = (u16 *)&PARTY_STATE;
            s32 a, b;
            a = work[288];
            work[224] = a;
            b = work[289];
            work[225] = b;
        }
        runtime->result_code = 999;
        specialResult = 1;
    }
    if (encodedAction & 0x2000) return BattleFx_ExecutePackedAbilityEffect(encodedAction);

    if (actor <= 7) {
        cost = ((struct BattleActionDefinition *)(void *)BattleAction_Get(actionId))->pp_cost;
        if (((struct BattleUnitRecord *)Owner_GetStateFar(actor))->pp < cost) {
            UiWork_PushValueSlotFar(actor, 1); UiWork_PushValueSlotFar(actionId, 4); UiText_ShowPositionedMessageAndWaitFar(0x91e, 1);
            if (specialResult)runtime->result_code = 0;
            return 0;
        }
        Owner_AdjustSecondValueFar(actor, -cost);
    }

    primary = (struct BattleTargetCandidate *)BattleFx_FindMatchingEvent(0x10000005, targetMode, &targetId);
    secondary = (struct BattleTargetCandidate *)BattleFx_FindMatchingEvent(5, targetMode, &targetId);
    tertiary = (struct BattleTargetCandidate *)BattleFx_FindMatchingEvent(0x50000005, targetMode, &targetId);
    targetId = -1;
    GameFlag_SetBitFar(0x140); GameFlag_SetBitFar(0x141);
    if (primary || secondary || tertiary) {
        targetId = BattleEffect_SelectNearbyTargetObject(PARTY_STATE.object_id, targetMode);
        if (secondary && (secondary->flags & 0x400)) {
            GameFlag_ClearBitFar(0x140); GameFlag_ClearBitFar(0x141);
        }
    } else GameFlag_ClearBitFar(0x141);

    if (runtime->battle_mode == 3) BattleEffect_ClearOutOfBoundsObjects();
    Func_08096fb0(actionId, 0); runtime->resolving_action = 1;
    BattleFx_SetupObjectPair(PARTY_STATE.object_id, targetId); EventObject_Initialize();
    BattleFx_RunEventAction(primary, actor, targetId);
    if (GameFlag_TestFar(0x140)) {
        if (GameFlag_TestFar(0x141)) BattleFx_DispatchRequestKind(); else BattleFx_Run();
    }
    EffectRuntime_StopCurrentObject(); BattleFx_RunEventAction(secondary, actor, targetId);
    if (GameFlag_TestFar(0x140)) BattleFx_ClearChildValueOnMismatch();
    GameFlag_ClearBitFar(0x140); GameFlag_ClearBitFar(0x141); runtime->resolving_action = 0;
    BattleEffect_CleanupSceneObjects();
    if (runtime->battle_mode == 3) Func_0808b98c();
    return 0;
}
