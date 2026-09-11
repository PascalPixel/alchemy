#ifndef ALCHEMY_ABI_BATTLE_PRESENTATION_ACT_RUN_ENTRIES
#define ALCHEMY_ABI_BATTLE_PRESENTATION_ACT_RUN_ENTRIES

/* Binding layer — not production source. Address / far-call ABI only. */

void Func_080bb65c(void);
s32 Func_080bb938();
s32 Func_080bbb0c(void *, s32);
void Func_080c10e8(u16 *, s32);

#define Battle_Apply Func_080c10e8
#define Battle_Apply2 Func_080bbb0c
#define Battle_Check Func_080bb938
#define Battle_Run Func_080bb65c

#endif
