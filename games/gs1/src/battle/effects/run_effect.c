#include "types.h"

struct BattleEffectRequest {
    u8 padding_000[0x18];
    s16 sourceId;
    s16 targetId;
    u8 padding_01c[2];
    s16 mode;
};

struct BattleEffectState {
    u8 padding_000[0xCB8];
    s16 active;
};

struct BattleEffectGlobals {
    u8 padding_000[0x1F4];
    s32 selectedObject;
    u8 padding_1f8[0x52];
    s16 selectedId;
};

extern struct BattleEffectRequest *Data_03001f30;
extern struct BattleEffectGlobals Data_02000240;

#define RunBattleEffect01 Func_0809802c
#define RunSceneTransitionEffect Func_08097540
#define RunBattleEffect03 Func_080994d0
#define RunBattleEffect04 Func_08098cd8
#define RunBattleEffect05 Func_080999f0
#define RunOrbitingParticleEffect Func_08099160
#define RunBattleEffect07 Func_08098954
#define RunBattleEffect08 Func_080983a0
#define RunBattleEffect10 Func_08099838
#define RunBattleEffect11 Func_0809a294
#define RunBurstParticleEffect Func_080985fc
#define RunBattleEffect13 Func_0809ae64
#define RunBattleEffect14 Func_0809a8c4
#define RunBattleEffect15 Func_0809b208
#define RunBattleEffect16 Func_0809b698
void RunBattleEffect01(void);
void RunSceneTransitionEffect(s32 sourceId, s32 targetId);
void RunBattleEffect03(void);
void RunBattleEffect04(void);
void RunBattleEffect05(void);
void RunOrbitingParticleEffect(void);
void RunBattleEffect07(void);
void RunBattleEffect08(void);
void RunBattleEffect10(void);
void RunBattleEffect11(void);
void RunBurstParticleEffect(void);
void RunBattleEffect13(void);
void RunBattleEffect14(void);
void RunBattleEffect15(void);
void RunBattleEffect16(void);
void Func_0809ade8(s32 objectId);
s32 Func_0808df1c(s32 objectId, s32 mode);
s32 Func_0809ae3c(s32 objectId);
s32 Func_0808d5a4(s32 objectId);
void Func_080970f8(s32 selectedObject, s32 objectId);
void Func_0809ab98(s32 objectId);
void Func_0809ad90(s32 objectId);
void Func_0809abb4(void);
#define ResetSceneTransitionEffect Func_080984c0
void ResetSceneTransitionEffect(void);

#define RunBattleEffect Func_08096810
void RunBattleEffect(void)
{
    struct BattleEffectRequest *request;
    struct BattleEffectState *battle;
    s32 mode;
    s32 targetId;
    s32 objectId;

    request = Data_03001f30;
    battle = *(struct BattleEffectState **)((u8 *)&Data_03001f30 - 0x74);
    mode = request->mode;
    targetId = request->targetId;

    switch (mode) {
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
        RunOrbitingParticleEffect();
        return;
    case 3:
        RunBattleEffect03();
        return;
    case 12:
        RunBurstParticleEffect();
        return;
    case 13:
        RunBattleEffect13();
        return;
    case 9:
        if (Data_02000240.selectedId != -1) {
            Func_0809ade8(Data_02000240.selectedId);
            Data_02000240.selectedId = -1;
        }

        objectId = Func_0808df1c(Data_02000240.selectedObject, mode);
        objectId = Func_0809ae3c(objectId);
        if (Func_0808d5a4(objectId) != 0) {
            Func_080970f8(Data_02000240.selectedObject, objectId);
            Func_0809ab98(objectId);
            Func_0809ad90(objectId);
            Data_02000240.selectedId = objectId;
        } else {
            Func_0809abb4();
        }
        return;
    case 2:
        if (battle->active != 0)
            ResetSceneTransitionEffect();
        RunSceneTransitionEffect(request->sourceId, targetId);
        return;
    case 8:
        RunBattleEffect08();
        return;
    case 10:
        RunBattleEffect10();
        return;
    case 15:
        RunBattleEffect15();
        return;
    case 16:
        RunBattleEffect16();
        return;
    }
}
