#include "types.h"

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

extern struct BattleEffectRequest *Data_03001f30;
extern struct BattleEffectGlobals Data_02000240;
#define RunBattleEffect01 Func_0809802c

void RunBattleEffect01(void);
void RunSceneTransitionEffect(s32 source_id, s32 target_id);
void RunBattleEffect03(void);
void RunBattleEffect04(void);
void RunBattleEffect05(void);
void BattleEffect_RunOrbitingParticles(void);
void RunBattleEffect07(void);
void RunBattleEffect08(void);
void RunBattleEffect10(void);
void RunBattleEffect11(void);
void BattleEffect_RunBurstParticles(void);
void RunBattleEffect13(void);
void RunBattleEffect14(void);
void BattleEffect_RunEffect15(void);
void RunBattleEffect16(void);
void BattleEffect_ResumeObject(s32 obj_id);
s32 Func_0808df1c(s32 obj_id, s32 battle_mode);
s32 BattleEffect_FilterObjectIdByFlags(s32 obj_id);
s32 Func_0808d5a4(s32 obj_id);
void BattleEffect_SetupObjectPair(s32 selected_object, s32 obj_id);
void Func_0809ab98(s32 obj_id);
void BattleEffect_PauseObject(s32 obj_id);
void Func_0809abb4(void);
void ResetSceneTransitionEffect(void);

void BattleEffect_Run(void)
{
    struct BattleEffectRequest *request;
    struct BattleEffectState *battle;
    s32 battle_mode;
    s32 target_id;
    s32 obj_id;

    request = Data_03001f30;
    battle = *(struct BattleEffectState **)((u8 *)&Data_03001f30 - 0x74);
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
        BattleEffect_RunOrbitingParticles();
        return;
    case 3:
        RunBattleEffect03();
        return;
    case 12:
        BattleEffect_RunBurstParticles();
        return;
    case 13:
        RunBattleEffect13();
        return;
    case 9:
        if (Data_02000240.selected_id != -1) {
            BattleEffect_ResumeObject(Data_02000240.selected_id);
            Data_02000240.selected_id = -1;
        }

        obj_id = Func_0808df1c(Data_02000240.selected_object, battle_mode);
        obj_id = BattleEffect_FilterObjectIdByFlags(obj_id);
        if (Func_0808d5a4(obj_id) != 0) {
            BattleEffect_SetupObjectPair(Data_02000240.selected_object, obj_id);
            Func_0809ab98(obj_id);
            BattleEffect_PauseObject(obj_id);
            Data_02000240.selected_id = obj_id;
        } else {
            Func_0809abb4();
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
        BattleEffect_RunEffect15();
        return;
    case 16:
        RunBattleEffect16();
        return;
    }
}
