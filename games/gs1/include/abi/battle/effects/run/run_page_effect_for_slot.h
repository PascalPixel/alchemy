#ifndef ALCHEMY_ABI_BATTLE_EFFECTS_RUN_RUN_PAGE_EFFECT_FOR_SLOT
#define ALCHEMY_ABI_BATTLE_EFFECTS_RUN_RUN_PAGE_EFFECT_FOR_SLOT

/* Binding layer — not production source. Address / far-call ABI only. */

s32 Func_08077288(s32, void *);
void Func_0808c44c(void);
void Func_0808b8e8(void);
void Func_08096140(s32);
void Func_080965a8(s32);
void Func_08095dd0(s32);
void Func_0808b98c(void);
void Func_08015358(s32, s32, void *);
void Func_0808c4c0(void);
void Func_08091750(void);

#define Battle_Apply Func_08077288
#define Battle_Do Func_08096140
#define Battle_Do2 Func_080965a8
#define Battle_Do3 Func_08095dd0
#define Battle_Place Func_08015358
#define Battle_Run Func_0808c44c
#define Battle_Run2 Func_0808b8e8
#define Battle_Run3 Func_0808b98c
#define Battle_Run4 Func_0808c4c0
#define Battle_Run5 Func_08091750

#endif
