#ifndef ALCHEMY_ABI_BATTLE_EFFECTS_BURST_PARTICLES_RUN_MAIN_OBJECT
#define ALCHEMY_ABI_BATTLE_EFFECTS_BURST_PARTICLES_RUN_MAIN_OBJECT

/* Binding layer — not production source. Address / far-call ABI only. */

void Func_08098698(void);
void Func_080091e0(void *, s32);

#define Battle_Apply Func_080091e0
#define Battle_Run Func_08098698

#endif
