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
extern struct BattleRuntime *Data_03001ebc;
extern u8 Value_00000920;
/*
 * Returns u8 * to match the prototype shared with the other callers; the raw
 * pointer is cast to the local action-definition view below.
 */
u8 *Func_08077080(s32);
/*
 * Returns void * because callers view the same record through different
 * structs; each casts the shared pointer to its own view locally.
 */
void *Func_08077008(s32);
void *Func_0808ba1c(s32);
void Func_08091660(void); void Func_080770d0(s32); s32 Func_080770c0(s32);
#define BattleFlag_Test Func_080770c0
#define Battle_InitializeRenderObject Func_08091660
void Func_08015120(s32, s32); void Func_08015040(s32, s32);
s32 Func_08091d84(s32); void Func_08015140(void);
#define Object_CallSpawnRoutineAtOrigin Func_08091d84
/* Takes an s32 to match the definition of the packed effect argument. */
s32 Func_0808e5d8(s32);
#define BattleFx_ExecutePackedAbilityEffect Func_0808e5d8
void Func_08077120(s32, s32);
/*
 * Matches the shared prototype: s32-returning, with a void * out parameter.
 * Results are cast back to struct BattleTargetCandidate * here.
 */
s32 Func_0808e4b4(s32, s32, void *);
#define BattleFx_FindMatchingEvent Func_0808e4b4
void Func_080770c8(s32); s32 BattleEffect_SelectNearbyTargetObject(s32, s32); void BattleEffect_ClearOutOfBoundsObjects(void);
void Func_08096fb0(s32, s32); void BattleFx_SetupObjectPair(s32, s32); void Func_0809728c(void);
/*
 * Returns s32 to match the shared prototype, although every call site
 * discards the value.
 */
s32 Func_08096b28(void *, s32, s32); void BattleFx_DispatchRequestKind(void); void Func_08096810(void);
#define BattleFx_RunEventAction Func_08096b28
void Func_08097174(void); void BattleFx_ClearChildValueOnMismatch(void); void BattleEffect_CleanupSceneObjects(void); void Func_0808b98c(void);
#define EffectRuntime_StopCurrentObject Func_08097174

s32 BattleCommand_ExecuteSelectedAction(u32 encodedAction)
{
    s32 actionId = encodedAction & 0x3ff;
    struct BattleCommandRuntime *runtime = (struct BattleCommandRuntime *)Data_03001ebc;
    struct BattleTargetCandidate *primary;
    struct BattleTargetCandidate *secondary;
    struct BattleTargetCandidate *tertiary;
    s32 actor;
    s32 targetId;
    s32 specialResult;
    s32 targetMode;
    s32 cost;
    s32 status;

    targetMode = ((struct BattleActionDefinition *)(void *)Func_08077080(actionId))->target_mode;
    actor = (encodedAction >> 10) & 15;
    Func_0808ba1c(Data_02000240.object_id);
    specialResult = 0;
    Battle_InitializeRenderObject();
    Func_080770d0(0x145);
    if (actor == 15) actor = 0;

    if (BattleFlag_Test(0x17e)) {
        Func_08015120(actor, 1); Func_08015120(actionId, 4); Func_08015040(0x91f, 1);
        return 0;
    }
    if (runtime->battle_mode == 3 && actionId == 0x90) {
        Func_08015120(actor, 1); Func_08015120(0x90, 4); Func_08015040(0x91f, 1);
        return 0;
    }
    if (actionId == 0x95) {
        if (BattleFlag_Test(0x144)) {
            Func_08015120(actor, 1); Func_08015120(0x95, 4); Func_08015040(0x921, 1);
            return 0;
        }
        Func_08015120(0x95, 4); Func_08015040((s32)&Value_00000920, 13);
        status = Object_CallSpawnRoutineAtOrigin(1); Func_08015140();
        if (status != 0) return 0;
        {
            u16 *work = (u16 *)&Data_02000240;
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
        cost = ((struct BattleActionDefinition *)(void *)Func_08077080(actionId))->pp_cost;
        if (((struct BattleUnitRecord *)Func_08077008(actor))->pp < cost) {
            Func_08015120(actor, 1); Func_08015120(actionId, 4); Func_08015040(0x91e, 1);
            if (specialResult)runtime->result_code = 0;
            return 0;
        }
        Func_08077120(actor, -cost);
    }

    primary = (struct BattleTargetCandidate *)BattleFx_FindMatchingEvent(0x10000005, targetMode, &targetId);
    secondary = (struct BattleTargetCandidate *)BattleFx_FindMatchingEvent(5, targetMode, &targetId);
    tertiary = (struct BattleTargetCandidate *)BattleFx_FindMatchingEvent(0x50000005, targetMode, &targetId);
    targetId = -1;
    Func_080770c8(0x140); Func_080770c8(0x141);
    if (primary || secondary || tertiary) {
        targetId = BattleEffect_SelectNearbyTargetObject(Data_02000240.object_id, targetMode);
        if (secondary && (secondary->flags & 0x400)) {
            Func_080770d0(0x140); Func_080770d0(0x141);
        }
    } else Func_080770d0(0x141);

    if (runtime->battle_mode == 3) BattleEffect_ClearOutOfBoundsObjects();
    Func_08096fb0(actionId, 0); runtime->resolving_action = 1;
    BattleFx_SetupObjectPair(Data_02000240.object_id, targetId); Func_0809728c();
    BattleFx_RunEventAction(primary, actor, targetId);
    if (BattleFlag_Test(0x140)) {
        if (BattleFlag_Test(0x141)) BattleFx_DispatchRequestKind(); else Func_08096810();
    }
    EffectRuntime_StopCurrentObject(); BattleFx_RunEventAction(secondary, actor, targetId);
    if (BattleFlag_Test(0x140)) BattleFx_ClearChildValueOnMismatch();
    Func_080770d0(0x140); Func_080770d0(0x141); runtime->resolving_action = 0;
    BattleEffect_CleanupSceneObjects();
    if (runtime->battle_mode == 3) Func_0808b98c();
    return 0;
}
