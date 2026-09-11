#ifndef ALCHEMY_ABI_BATTLE_PRESENTATION_TRANS_TIMER
#define ALCHEMY_ABI_BATTLE_PRESENTATION_TRANS_TIMER

/* Binding layer — not production source. Address / far-call ABI only. */

s32 Func_080c0cec(s32, s32, s32, s32);

#define Battle_SetMode Func_080c0cec

#endif
