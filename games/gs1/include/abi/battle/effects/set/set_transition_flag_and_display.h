#ifndef ALCHEMY_ABI_BATTLE_EFFECTS_SET_SET_TRANSITION_FLAG_AND_DISPLAY
#define ALCHEMY_ABI_BATTLE_EFFECTS_SET_SET_TRANSITION_FLAG_AND_DISPLAY

/* Binding layer — not production source. Address / far-call ABI only. */

void Func_0800387c(u32 first, u32 second);
void Func_080b5038(s32, u16, s32);

#define Battle_Apply Func_0800387c
#define Battle_Place Func_080b5038

#endif
