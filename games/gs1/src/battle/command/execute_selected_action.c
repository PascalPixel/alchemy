#include "types.h"
#include "battle_effect_runtime.h"

struct BattleActionDefinition { u8 pad00[9]; u8 pp_cost; u8 pad0a[2]; u8 target_mode; };
struct BattleUnitRecord { u8 pad00[58]; s16 pp; };
struct BattleTargetCandidate { u8 pad00[4]; u16 flags; };
struct BattleCommandRuntime {
    u8 pad000[0x170]; s16 result_code; u8 pad172[0x2c]; s16 battle_mode;
    u8 pad1a0[0xb26]; u8 resolving_action;
};

/* Declared struct BattleRuntime * to match the canonical extern in
 * battle_effect_runtime.h / main:0808e23c; this owner's own local view is
 * obtained with a cast below. */
extern struct BattleRuntime *Data_03001ebc;
extern u8 Value_00000920;
/* u8 *-returning to match the prototype the exact owner battle_owner_23.c
 * uses under its Ability_GetData alias; this owner casts the raw pointer to
 * its own local view below. */
u8 *Func_08077080(s32);
/* Declared void *-returning: main:0808e23c shares this symbol through a
 * different local view (struct BattleUnitObject, for the abilities table);
 * both callers cast the shared pointer to their own struct locally. */
void *Func_08077008(s32);
void *Func_0808ba1c(s32);
void Func_08091660(void); void Func_080770d0(s32); s32 Func_080770c0(s32);
void Func_08015120(s32, s32); void Func_08015040(s32, s32);
s32 Func_08091d84(s32); void Func_08015140(void);
/* s32 parameter to match the exact owner's definition (battle_owner_23.c
 * defines Func_0808e5d8(s32 packedEffect)). */
s32 Func_0808e5d8(s32);
void Func_08077120(s32, s32);
/* Matches the prototype agreed by the exact owners (battle_owner_23.c,
 * runtime_owner_207.c): s32-returning, u16 second parameter, void * out
 * parameter. Results here are cast back to struct BattleTargetCandidate *. */
s32 Func_0808e4b4(s32, s32, void *);
void Func_080770c8(s32); s32 Func_0808df1c(s32, s32); void Func_0808b8e8(void);
void Func_08096fb0(s32, s32); void Func_080970f8(s32, s32); void Func_0809728c(void);
/* s32-returning to match the prototype in the exact owner battle_owner_23.c
 * (the return value is discarded at every call site, here and there). */
s32 Func_08096b28(void *, s32, s32); void Func_08096960(void); void Func_08096810(void);
void Func_08097174(void); void Func_08096ab0(void); void Func_08097194(void); void Func_0808b98c(void);

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
    Func_08091660();
    Func_080770d0(0x145);
    if (actor == 15) actor = 0;

    if (Func_080770c0(0x17e)) {
        Func_08015120(actor, 1); Func_08015120(actionId, 4); Func_08015040(0x91f, 1);
        return 0;
    }
    if (runtime->battle_mode == 3 && actionId == 0x90) {
        Func_08015120(actor, 1); Func_08015120(0x90, 4); Func_08015040(0x91f, 1);
        return 0;
    }
    if (actionId == 0x95) {
        if (Func_080770c0(0x144)) {
            Func_08015120(actor, 1); Func_08015120(0x95, 4); Func_08015040(0x921, 1);
            return 0;
        }
        Func_08015120(0x95, 4); Func_08015040((s32)&Value_00000920, 13);
        status = Func_08091d84(1); Func_08015140();
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
    if (encodedAction & 0x2000) return Func_0808e5d8(encodedAction);

    if (actor <= 7) {
        cost = ((struct BattleActionDefinition *)(void *)Func_08077080(actionId))->pp_cost;
        if (((struct BattleUnitRecord *)Func_08077008(actor))->pp < cost) {
            Func_08015120(actor, 1); Func_08015120(actionId, 4); Func_08015040(0x91e, 1);
            if (specialResult) runtime->result_code = 0;
            return 0;
        }
        Func_08077120(actor, -cost);
    }

    primary = (struct BattleTargetCandidate *)Func_0808e4b4(0x10000005, targetMode, &targetId);
    secondary = (struct BattleTargetCandidate *)Func_0808e4b4(5, targetMode, &targetId);
    tertiary = (struct BattleTargetCandidate *)Func_0808e4b4(0x50000005, targetMode, &targetId);
    targetId = -1;
    Func_080770c8(0x140); Func_080770c8(0x141);
    if (primary || secondary || tertiary) {
        targetId = Func_0808df1c(Data_02000240.object_id, targetMode);
        if (secondary && (secondary->flags & 0x400)) {
            Func_080770d0(0x140); Func_080770d0(0x141);
        }
    } else Func_080770d0(0x141);

    if (runtime->battle_mode == 3) Func_0808b8e8();
    Func_08096fb0(actionId, 0); runtime->resolving_action = 1;
    Func_080970f8(Data_02000240.object_id, targetId); Func_0809728c();
    Func_08096b28(primary, actor, targetId);
    if (Func_080770c0(0x140)) {
        if (Func_080770c0(0x141)) Func_08096960(); else Func_08096810();
    }
    Func_08097174(); Func_08096b28(secondary, actor, targetId);
    if (Func_080770c0(0x140)) Func_08096ab0();
    Func_080770d0(0x140); Func_080770d0(0x141); runtime->resolving_action = 0;
    Func_08097194();
    if (runtime->battle_mode == 3) Func_0808b98c();
    return 0;
}
