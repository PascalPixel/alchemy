#ifndef ALCHEMY_ABI_BATTLE_RUN_SIMPLE_PRESENTATION
#define ALCHEMY_ABI_BATTLE_RUN_SIMPLE_PRESENTATION

/* Binding layer — not production source. Address / far-call ABI only. */

void Func_080c10e8(u16 *actors, s32 mode);
void Func_080b9d34(void *input, struct BattlePresentationWork *work);
s32 Func_08009260(s32 battle_value, s32 second, s32 third);
void Func_080b8178(s32 id);
void Func_080c9008(struct BattlePresentationWork *work);
void Func_080bb938(void);

#define Battle_Apply Func_080c10e8
#define Battle_Apply2 Func_080b9d34
#define Battle_Do Func_080b8178
#define Battle_Do2 Func_080c9008
#define Battle_Place Func_08009260
#define Battle_Run Func_080bb938
#define gIw Data_03001f00

#endif
