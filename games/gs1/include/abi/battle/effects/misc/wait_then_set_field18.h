#ifndef ALCHEMY_ABI_BATTLE_EFFECTS_MISC_WAIT_THEN_SET_FIELD18
#define ALCHEMY_ABI_BATTLE_EFFECTS_MISC_WAIT_THEN_SET_FIELD18

/* Binding layer — not production source. Address / far-call ABI only. */

void Func_080b5078(s32, s16, s32, s32);
s32 Func_080d4604(void *, s32);

#define Battle_Apply Func_080d4604
#define Battle_SetMode Func_080b5078

#endif
