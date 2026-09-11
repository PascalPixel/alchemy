#ifndef ALCHEMY_ABI_BATTLE_PRESENTATION_ACTOR_SET_MODES
#define ALCHEMY_ABI_BATTLE_PRESENTATION_ACTOR_SET_MODES

/* Binding layer — not production source. Address / far-call ABI only. */

void Func_0800387c(u32, u32);
s32 Func_080041d8(u32, s32);
void Func_08004278(u32);
u32 Func_080b6c08(s32, s16 *);
void Func_080c0f98(s32, s32);
void Func_080c1054(void);

#define Actor_Apply Func_0800387c
#define Actor_Apply2 Func_080b6c08
#define Actor_Apply3 Func_080c0f98
#define Actor_Apply4 Func_080041d8
#define Actor_Do Func_08004278
#define Actor_Run Func_080c1054
#define gBattleWork Data_03001e74

#endif
