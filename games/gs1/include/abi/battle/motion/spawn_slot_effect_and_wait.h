#ifndef ALCHEMY_ABI_BATTLE_MOTION_SPAWN_SLOT_EFFECT_AND_WAIT
#define ALCHEMY_ABI_BATTLE_MOTION_SPAWN_SLOT_EFFECT_AND_WAIT

/* Binding layer — not production source. Address / far-call ABI only. */

s32 Func_08009070(void *, s32);

#define Battle_Apply Func_08009070
#define Battle_Run Func_08009048

#endif
