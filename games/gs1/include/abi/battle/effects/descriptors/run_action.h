#ifndef ALCHEMY_ABI_BATTLE_EFFECTS_DESCRIPTORS_RUN_ACTION
#define ALCHEMY_ABI_BATTLE_EFFECTS_DESCRIPTORS_RUN_ACTION

/* Binding layer — not production source. Address / far-call ABI only. */

extern s32 Func_080915dc(s32);
extern u32 Func_08004458(void);
extern void Func_080916b0(void);
extern void Func_08092b94(s32);
extern void Func_08092f84(s32, s32);
extern void Func_08091750(void);
extern void Func_08009088(struct BattleActionObject *, s32);
extern void Func_08092848(s32, s32, s32);
extern void Func_08015058(s32);
extern void Func_08091660(void);
extern void Func_08093a6c(struct BattleActionObject *, void *);
extern void Func_08009098(struct BattleActionObject *, void *);
extern void Func_0809ade8(s32);

#define Battle_Apply Func_08092f84
#define Battle_Apply2 Func_08009088
#define Battle_Apply3 Func_08093a6c
#define Battle_Apply4 Func_08009098
#define Battle_Check Func_080915dc
#define Battle_Do Func_08092b94
#define Battle_Do2 Func_08015058
#define Battle_Do3 Func_0809ade8
#define Battle_Place Func_08092848
#define Battle_Run Func_0808d394
#define Battle_Run2 Func_0808d48c
#define Battle_Run3 Func_08092054
#define Battle_Run4 Func_08004458
#define Battle_Run5 Func_080916b0
#define Battle_Run6 Func_08091750
#define Battle_Run7 Func_08091660
#define gCell Data_02000240

#endif
