#ifndef ALCHEMY_ABI_BATTLE_EFFECTS_MISC_FINISH_ACTION
#define ALCHEMY_ABI_BATTLE_EFFECTS_MISC_FINISH_ACTION

/* Binding layer — not production source. Address / far-call ABI only. */

void Func_080772f0(void);
void Func_0809335c(s32 value, s32 enabled);

#define Battle_Apply Func_0809335c
#define Battle_Run Func_080772f0
#define gCell Data_02000240

#endif
