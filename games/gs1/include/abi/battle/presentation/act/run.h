#ifndef ALCHEMY_ABI_BATTLE_PRESENTATION_ACT_RUN
#define ALCHEMY_ABI_BATTLE_PRESENTATION_ACT_RUN

/* Binding layer — not production source. Address / far-call ABI only. */

void Func_08015118(void);
s32 Func_080b8888(s16 *);
s32 Func_080b8c1c(s16 *);
s32 Func_080b88d0(s16 *);
void Func_08015220(void);

#define Battle_Check Func_080b8888
#define Battle_Check2 Func_080b8c1c
#define Battle_Check3 Func_080b88d0
#define Battle_Run Func_08015118
#define Battle_Run2 Func_08015220
#define gIw Data_03001f00

#endif
