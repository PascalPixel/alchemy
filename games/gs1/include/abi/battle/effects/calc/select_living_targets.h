#ifndef ALCHEMY_ABI_BATTLE_EFFECTS_CALC_SELECT_LIVING_TARGETS
#define ALCHEMY_ABI_BATTLE_EFFECTS_CALC_SELECT_LIVING_TARGETS

/* Binding layer — not production source. Address / far-call ABI only. */

void Func_080b5010(s16 *targets, s32 mode);

#define Battle_Apply Func_080b5010

#endif
