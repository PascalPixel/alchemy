#ifndef ALCHEMY_ABI_BATTLE_EFFECTS_COLOR_ADVANCE_HUE_CYCLE
#define ALCHEMY_ABI_BATTLE_EFFECTS_COLOR_ADVANCE_HUE_CYCLE

/* Binding layer — not production source. Address / far-call ABI only. */

extern void Func_08097948(s32 angle, s32 *out1, s32 *out2, s32 *out3);

#define Battle_SetMode Func_08097948
#define gIw Data_03001ea8

#endif
