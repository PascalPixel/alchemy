#ifndef ALCHEMY_ABI_BATTLE_PRESENTATION_ACT_RUN_APPROACH
#define ALCHEMY_ABI_BATTLE_PRESENTATION_ACT_RUN_APPROACH

/* Binding layer — not production source. Address / far-call ABI only. */

s32 Func_080b8808(u32);
void Func_080c9008(struct Work_080b8b48 *);

#define Battle_Check Func_080b8808
#define Battle_Do Func_080c9008
#define gIw Data_03001f00

#endif
