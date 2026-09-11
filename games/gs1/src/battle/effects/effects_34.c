#include "types.h"
#include "scene.h"

/* battle/effects/run/run_effect.c */
struct BattleEffectRequest {
    u8 reserved_000[0x18];
    s16 source_id;
    s16 target_id;
    u8 reserved_01c[2];
    s16 battle_mode;
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

        obj_id = Battle_Apply(gCell.selected_object, battle_mode);
        obj_id = BattleFx_FilterObjectIdByFlags(obj_id);
        if (Battle_Check(obj_id)!= 0) {
            BattleFx_SetupObjectPair(gCell.selected_object, obj_id);
            Battle_Do(obj_id);
            BattleFx_PauseObject(obj_id);
            gCell.selected_id = obj_id;
        } else {
            Battle_Run();
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

/* battle/effects/set/dispatch_request_kind.c */
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
    u8 reserved_000[0xcb8];
    s16 active;
};

struct BattleEffectGlobals {
    u8 reserved_000[0x24a];
    s16 selected_id;
};

extern struct BattleEffectRequest *gIw;
extern struct BattleEffectGlobals gCell;

void BattleFx_DispatchRequestKind(void)
{
    struct BattleEffectRequest *request = gIw;
    struct BattleEffectState *battle = *(struct BattleEffectState **)((u8 *)&gIw - 0x74);
    s32 battle_mode = request->battle_mode;
    s32 target_id = request->target_id;

    request->running = 0;
    switch (battle_mode) {
    case 2:
        if (battle->active != 0)
            Battle_Run();
        if (gCell.selected_id != request->target_id)
            *(u8 *)((u8 *)request->object + 91) = 1;
        Battle_Apply(request->source_id, target_id);
        break;
    case 1:
        Battle_Do(target_id);
        break;
    case 7:
        Battle_unk2_2(target_id);
        break;
    case 11:
        Battle_unk3_2(target_id);
        break;
    case 4:
        Battle_unk4_2(target_id);
        break;
    case 5:
        Battle_unk5(target_id);
        break;
    case 6:
        Battle_unk6(target_id);
        break;
    case 12:
        Battle_unk7(target_id);
        break;
    case 9:
        if (gCell.selected_id != -1) {
            Battle_unk8(gCell.selected_id);
            gCell.selected_id = -1;
        }
        Battle_unk9(target_id);
        gCell.selected_id = target_id;
        Battle_unk10(target_id);
        break;
    case 3:
        Battle_unk11(target_id);
        break;
    case 14:
        Battle_unk12(target_id);
        break;
    case 13:
        Battle_unk13(target_id);
        break;
    case 8:
        Battle_unk2_4();
        break;
    case 10:
        Battle_unk3_4();
        break;
    case 15:
        Battle_unk4_4();
        break;
    case 16:
        Battle_unk5_2();
        break;
    }
}

/* battle/effects/misc/clear_child_value_on_mismatch.c */
struct Child_08096ab0 {
    u8 padding[91];
    u8 value;
};

struct State_08096ab0 {
    u8 padding_00[20];
    struct Child_08096ab0 *child;
    u8 padding_18[2];
    s16 value;
    u8 padding_1c[2];
    s16 mode;
};

struct Global_08096ab0 {
    u8 padding[0x24a];
    s16 value;
};

extern struct State_08096ab0 *gIw;
extern struct Global_08096ab0 gCell;

void BattleFx_ClearChildValueOnMismatch(void)
{
    struct State_08096ab0 *state = gIw;

    if (state->mode == 2) {
        Battle_Run();
        if (gCell.value != state->value) {
            state->child->value = 0;
        }
    }
}
