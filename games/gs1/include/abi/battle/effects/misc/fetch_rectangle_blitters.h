#ifndef ALCHEMY_ABI_BATTLE_EFFECTS_MISC_FETCH_RECTANGLE_BLITTERS
#define ALCHEMY_ABI_BATTLE_EFFECTS_MISC_FETCH_RECTANGLE_BLITTERS

/* Binding layer — not production source. Address / far-call ABI only. */

s32 Func_080ed408(s32, s32, s32, s32, s32);

#define Battle_SetRange Func_080ed408
#define gIw Data_03001e50

#endif
