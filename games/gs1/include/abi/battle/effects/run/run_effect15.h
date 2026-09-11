#ifndef ALCHEMY_ABI_BATTLE_EFFECTS_RUN_RUN_EFFECT15
#define ALCHEMY_ABI_BATTLE_EFFECTS_RUN_RUN_EFFECT15

/* Binding layer — not production source. Address / far-call ABI only. */

void Func_080916b0(void);
void Func_080933f8(s32, s32, s32, s32);
void Func_08097384(void);
void Func_080030f8(s32);
void Func_08092adc(s32, s32, s32);
void Func_080f9010(s32);
void Func_08009080(void *, s32);
void Func_08009240(void *, s32);
void Func_080091e0(void *, s32);
void Func_080974d8(struct EffectPosition *);
void Func_0809ba90(void *, s32, s32, s32);
void Func_0809ba7c(void *, const void *);
void Func_0809ba70(void *, s32);
u32 Func_08004458(void);
void Func_08009248(void *, s32);
void Func_0809748c(void);
void Func_08096b88(void);
void Func_0809b0b0(void);
void Func_0809b0dc(void);
void Func_0809b11c(void);

#define Battle_Apply Func_08009080
#define Battle_Apply2 Func_08009240
#define Battle_Apply3 Func_080091e0
#define Battle_Apply4 Func_0809ba7c
#define Battle_Apply5 Func_0809ba70
#define Battle_Apply6 Func_08009248
#define Battle_Do Func_080030f8
#define Battle_Do2 Func_080f9010
#define Battle_Do3 Func_080974d8
#define Battle_Place Func_08092adc
#define Battle_Run Func_080916b0
#define Battle_Run2 Func_08097384
#define Battle_Run3 Func_08096b88
#define Battle_Run4 Func_0809b0b0
#define Battle_Run5 Func_0809b0dc
#define Battle_Run6 Func_0809b11c
#define Battle_Run7 Func_08004458
#define Battle_Run8 Func_0809748c
#define Battle_SetMode Func_080933f8
#define Battle_SetMode2 Func_0809ba90
#define gCell Data_02000240
#define gIw Data_03001f30

#endif
