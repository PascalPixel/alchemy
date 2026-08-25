#include "types.h"

struct BattleActionDefinition { u8 pad00[9]; u8 pp_cost; u8 pad0a[2]; u8 target_mode; };
struct BattleUnitRecord { u8 pad00[58]; s16 pp; };
struct BattleTargetCandidate { u8 pad00[4]; u16 flags; };
struct BattleCommandRuntime {
    u8 pad000[0x170]; s16 result_code; u8 pad172[0x2c]; s16 battle_mode;
    u8 pad1a0[0xb26]; u8 resolving_action;
};

extern struct BattleCommandRuntime *Data_03001ebc;
struct BattleActionDefinition *Func_08077080(s32);
struct BattleUnitRecord *Func_08077008(s32);
void *Func_0808ba1c(s32);
void Func_08091660(void); void Func_080770d0(s32); s32 Func_080770c0(s32);
void Func_08015120(s32, s32); void Func_08015040(s32, s32);
s32 Func_08091d84(s32); void Func_08015140(void); s32 Func_0808e5d8(u32);
void Func_08077120(s32, s32); struct BattleTargetCandidate *Func_0808e4b4(u32, s32, s32 *);
void Func_080770c8(s32); s32 Func_0808df1c(s32, s32); void Func_0808b8e8(void);
void Func_08096fb0(s32, s32); void Func_080970f8(s32, s32); void Func_0809728c(void);
void Func_08096b28(void *, s32, s32); void Func_08096960(void); void Func_08096810(void);
void Func_08097174(void); void Func_08096ab0(void); void Func_08097194(void); void Func_0808b98c(void);

#define BattleCommand_ExecuteSelectedAction Func_0808e680
s32 BattleCommand_ExecuteSelectedAction(u32 encodedAction)
{
    struct BattleCommandRuntime *runtime = Data_03001ebc;
    struct BattleActionDefinition *action;
    struct BattleTargetCandidate *primary;
    struct BattleTargetCandidate *secondary;
    struct BattleTargetCandidate *tertiary;
    s32 actionId = encodedAction & 0x3ff;
    s32 actor = (encodedAction >> 10) & 15;
    s32 targetId = -1;
    s32 specialResult = 0;
    s32 targetMode;

    action = Func_08077080(actionId);
    targetMode = action->target_mode;
    Func_0808ba1c(*(s32 *)0x02000434);
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
        Func_08015120(0x95, 4); Func_08015040(0x920, 13);
        specialResult = Func_08091d84(1); Func_08015140();
        if (specialResult != 0) return 0;
        *(u16 *)0x02000400 = *(u16 *)0x02000480;
        *(u16 *)0x02000402 = *(u16 *)0x02000482;
        runtime->result_code = 999;
        specialResult = 1;
    }
    if (encodedAction & 0x2000) return Func_0808e5d8(encodedAction);

    if (actor <= 7 && Func_08077008(actor)->pp < action->pp_cost) {
        Func_08015120(actor, 1); Func_08015120(actionId, 4); Func_08015040(0x91e, 1);
        if (specialResult) runtime->result_code = 0;
        return 0;
    }
    if (actor <= 7) Func_08077120(actor, -action->pp_cost);

    primary = Func_0808e4b4(0x10000005, targetMode, &targetId);
    secondary = Func_0808e4b4(5, targetMode, &targetId);
    tertiary = Func_0808e4b4(0x50000005, targetMode, &targetId);
    targetId = -1;
    Func_080770c8(0x140); Func_080770c8(0x141);
    if (primary || secondary || tertiary) {
        targetId = Func_0808df1c(*(s32 *)0x02000434, targetMode);
        if (secondary && (secondary->flags & 0x400)) {
            Func_080770d0(0x140); Func_080770d0(0x141);
        }
    } else Func_080770d0(0x141);

    if (runtime->battle_mode == 3) Func_0808b8e8();
    Func_08096fb0(actionId, 0); runtime->resolving_action = 1;
    Func_080970f8(*(s32 *)0x02000434, targetId); Func_0809728c();
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
