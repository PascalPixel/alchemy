#ifndef ALCHEMY_ABI_BATTLE_PRESENTATION_LIST_OPPONENT
#define ALCHEMY_ABI_BATTLE_PRESENTATION_LIST_OPPONENT

/* Binding layer — not production source. Address / far-call ABI only. */

s32 Func_080b6b40(s32 side, u16 *out_units);
u32 Func_08004458(void);
void Func_080bd424(void *entry, s32 arg1);

#define Battle_Apply Func_080b6b40
#define Battle_Apply2 Func_080bd424
#define Battle_Run Func_08077008
#define Battle_Run2 Func_08004458

#endif
