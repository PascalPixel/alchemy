#ifndef ALCHEMY_ABI_BATTLE_EFFECTS_RUN_RUN_VISIBILITY_TRANSITION
#define ALCHEMY_ABI_BATTLE_EFFECTS_RUN_RUN_VISIBILITY_TRANSITION

/* Binding layer — not production source. Address / far-call ABI only. */

s32 Func_0800430c(void);
s32 Func_080043e0(void);
void Func_080936a0(s32, s32);

#define Battle_Apply Func_080936a0
#define Battle_Check Func_0800430c
#define Battle_Check2 Func_080043e0

#endif
