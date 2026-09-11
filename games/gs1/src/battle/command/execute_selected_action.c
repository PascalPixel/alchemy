#include "types.h"
#include "scene.h"
#include "battle_effect_runtime.h"

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
extern struct BattleRuntime *gWork;
extern u8 Value_00000920;
/*
 * Returns u8 * to match the prototype shared with the other callers; the raw
 * pointer is cast to the local action-definition view below.
 */
u8 *Battle_Run(s32);
/*
 * Returns void * because callers view the same record through different
 * structs; each casts the shared pointer to its own view locally.
 */
void *Battle_unk2_4(s32);
void *Battle_unk3_4(s32);
void Battle_unk4_4(void); void Battle_unk5_2(s32); s32 Battle_unk6_2(s32);
void Battle_unk7_2(s32, s32); void Battle_unk8_2(s32, s32);
s32 Battle_unk9_2(s32); void Battle_unk10_2(void);
/* Takes an s32 to match the definition of the packed effect argument. */

/*
 * Matches the shared prototype: s32-returning, with a void * out parameter.
 * Results are cast back to struct BattleTargetCandidate * here.
 */

void Battle_unk11_2(s32); s32 Battle_unk12_2(s32, s32); void Battle_unk13_2(void);
void Battle_unk14(s32, s32); void Battle_unk15(s32, s32); void Battle_unk16(void);
/*
 * Returns s32 to match the shared prototype, although every call site
 * discards the value.
 */
s32 Battle_unk17(void *, s32, s32); void Battle_unk18(void); void Battle_unk19(void);
void Battle_unk20(void); void Battle_unk21(void); void Battle_unk22(void); void Battle_unk23(void);

s32 BattleCommand_ExecuteSelectedAction(u32 encodedAction)
{
    s32 actionId = encodedAction & 0x3ff;
    struct BattleCommandRuntime *runtime = (struct BattleCommandRuntime *)gWork;
    struct BattleTargetCandidate *primary;
    struct BattleTargetCandidate *secondary;
    struct BattleTargetCandidate *tertiary;
    s32 actor;
    s32 targetId;
    s32 specialResult;
    s32 targetMode;
    s32 cost;
    s32 status;

    targetMode = ((struct BattleActionDefinition *)(void *)Battle_Run(actionId))->target_mode;
    actor = (encodedAction >> 10) & 15;
    Battle_unk3_4(gCell.object_id);
    specialResult = 0;
    Battle_unk4_4();
    Battle_unk5_2(0x145);
    if (actor == 15) actor = 0;

    if (Battle_unk6_2(0x17e)) {
        Battle_unk7_2(actor, 1); Battle_unk7_2(actionId, 4); Battle_unk8_2(0x91f, 1);
        return 0;
    }
    if (runtime->battle_mode == 3 && actionId == 0x90) {
        Battle_unk7_2(actor, 1); Battle_unk7_2(0x90, 4); Battle_unk8_2(0x91f, 1);
        return 0;
    }
    if (actionId == 0x95) {
        if (Battle_unk6_2(0x144)) {
            Battle_unk7_2(actor, 1); Battle_unk7_2(0x95, 4); Battle_unk8_2(0x921, 1);
            return 0;
        }
        Battle_unk7_2(0x95, 4); Battle_unk8_2((s32)&Value_00000920, 13);
        status = Battle_unk9_2(1); Battle_unk10_2();
        if (status != 0) return 0;
        {
            u16 *work = (u16 *)&gCell;
            s32 a, b;
            a = work[288];
            work[224] = a;
            b = work[289];
            work[225] = b;
        }
        runtime->result_code = 999;
        specialResult = 1;
    }
    if (encodedAction & 0x2000) return Battle_Check(encodedAction);

    if (actor <= 7) {
        cost = ((struct BattleActionDefinition *)(void *)Battle_Run(actionId))->pp_cost;
        if (((struct BattleUnitRecord *)Battle_unk2_4(actor))->pp < cost) {
            Battle_unk7_2(actor, 1); Battle_unk7_2(actionId, 4); Battle_unk8_2(0x91e, 1);
            if (specialResult)runtime->result_code = 0;
            return 0;
        }
        Battle_Apply(actor, -cost);
    }

    primary = (struct BattleTargetCandidate *)Battle_Place(0x10000005, targetMode, &targetId);
    secondary = (struct BattleTargetCandidate *)Battle_Place(5, targetMode, &targetId);
    tertiary = (struct BattleTargetCandidate *)Battle_Place(0x50000005, targetMode, &targetId);
    targetId = -1;
    Battle_unk11_2(0x140); Battle_unk11_2(0x141);
    if (primary || secondary || tertiary) {
        targetId = Battle_unk12_2(gCell.object_id, targetMode);
        if (secondary && (secondary->flags & 0x400)) {
            Battle_unk5_2(0x140); Battle_unk5_2(0x141);
        }
    } else Battle_unk5_2(0x141);

    if (runtime->battle_mode == 3) Battle_unk13_2();
    Battle_unk14(actionId, 0); runtime->resolving_action = 1;
    Battle_unk15(gCell.object_id, targetId); Battle_unk16();
    Battle_unk17(primary, actor, targetId);
    if (Battle_unk6_2(0x140)) {
        if (Battle_unk6_2(0x141)) Battle_unk18(); else Battle_unk19();
    }
    Battle_unk20(); Battle_unk17(secondary, actor, targetId);
    if (Battle_unk6_2(0x140)) Battle_unk21();
    Battle_unk5_2(0x140); Battle_unk5_2(0x141); runtime->resolving_action = 0;
    Battle_unk22();
    if (runtime->battle_mode == 3) Battle_unk23();
    return 0;
}
