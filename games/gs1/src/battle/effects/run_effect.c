#include "types.h"
#include "scene.h"

/* battle/effects/run/run_effect.c */
struct BattleEffectRequest {
    u8 reserved_000[0x14];
    void *object;
    s16 source_id;
    s16 target_id;
    u8 reserved_01c[2];
    s16 battle_mode;
    u8 running;
};

struct BattleEffectState {
    u8 reserved_000[0xCB8];
    s16 active;
};

struct BattleEffectGlobals {
    u8 reserved_000[0x1F4];
    s32 selected_object;
    u8 reserved_1f8[0x52];
    s16 selected_id;
};

extern struct BattleEffectRequest *gIw;
extern struct BattleEffectGlobals gCell;

void RunBattleEffect01(void);
void RunSceneTransitionEffect(s32 source_id, s32 target_id);
void RunBattleEffect03(void);
void RunBattleEffect04(void);
void RunBattleEffect05(void);
void BattleFx_RunOrbitingParticles(void);
void RunBattleEffect07(void);
void RunBattleEffect08(void);
void RunBattleEffect10(void);
void RunBattleEffect11(void);
void BattleFx_RunBurstParticles(void);
void RunBattleEffect13(void);
void RunBattleEffect14(void);
void BattleFx_RunEffect15(void);
void RunBattleEffect16(void);
void BattleFx_ResumeObject(s32 obj_id);

s32 BattleFx_FilterObjectIdByFlags(s32 obj_id);

void BattleFx_SetupObjectPair(s32 selected_object, s32 obj_id);

void BattleFx_PauseObject(s32 obj_id);

void ResetSceneTransitionEffect(void);

void BattleFx_Run(void)
{
    struct BattleEffectRequest *request;
    struct BattleEffectState *battle;
    s32 battle_mode;
    s32 target_id;
    s32 obj_id;

    request = gIw;
    battle = *(struct BattleEffectState **)((u8 *)&gIw - 0x74);
    battle_mode = request->battle_mode;
    target_id = request->target_id;

    switch (battle_mode) {
    case 1:
        RunBattleEffect01();
        return;
    case 7:
        RunBattleEffect07();
        return;
    case 11:
        RunBattleEffect11();
        return;
    case 4:
        RunBattleEffect04();
        return;
    case 5:
        RunBattleEffect05();
        return;
    case 14:
        RunBattleEffect14();
        return;
    case 6:
        BattleFx_RunOrbitingParticles();
        return;
    case 3:
        RunBattleEffect03();
        return;
    case 12:
        BattleFx_RunBurstParticles();
        return;
    case 13:
        RunBattleEffect13();
        return;
    case 9:
        if (gCell.selected_id != -1) {
            BattleFx_ResumeObject(gCell.selected_id);
            gCell.selected_id = -1;
        }

        obj_id = FunctionHead_0808df1c(gCell.selected_object, battle_mode);
        obj_id = BattleFx_FilterObjectIdByFlags(obj_id);
        if (Battle_Check(obj_id)!= 0) {
            BattleFx_SetupObjectPair(gCell.selected_object, obj_id);
            FunctionHead_0809ab98(obj_id);
            BattleFx_PauseObject(obj_id);
            gCell.selected_id = obj_id;
        } else {
            FunctionHead_0809abb4();
        }
        return;
    case 2:
        if (battle->active != 0)
            ResetSceneTransitionEffect();
        RunSceneTransitionEffect(request->source_id, target_id);
        return;
    case 8:
        RunBattleEffect08();
        return;
    case 10:
        RunBattleEffect10();
        return;
    case 15:
        BattleFx_RunEffect15();
        return;
    case 16:
        RunBattleEffect16();
        return;
    }
}
