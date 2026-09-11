#ifndef ALCHEMY_ABI_BATTLE_EFFECTS_OBJECTS_START_EFFECT_22
#define ALCHEMY_ABI_BATTLE_EFFECTS_OBJECTS_START_EFFECT_22

/* Binding layer — not production source. Address / far-call ABI only. */

void Func_08015250(s32 value);
s32 Func_08003fa4(u32 slot, u32 size, const void *source);
void Func_08002dd8(s32 value);
void Func_0808f0d8(struct EffectObject_0808f1c0 *object);
void Func_080030f8(s32 frames);
void Func_08009080(struct EffectObject_0808f1c0 *object, s32 mode);
void Func_080090d0(struct EffectObject_0808f1c0 *object);

#define BattleFx_StartEffectObject22 Func_0808f1c0
#define Battle_Apply Func_08009080
#define Battle_Do Func_08015250
#define Battle_Do2 Func_08002dd8
#define Battle_Do3 Func_0808f0d8
#define Battle_Do4 Func_080030f8
#define Battle_Do5 Func_080090d0
#define Battle_Place Func_08003fa4
#define Battle_Run Func_080090c8
#define Battle_Run2 Func_080048b0
#define ObjectTable_Get Func_0808ba1c
#define gCell Data_02000240
#define gWork Data_03001ebc

#endif
