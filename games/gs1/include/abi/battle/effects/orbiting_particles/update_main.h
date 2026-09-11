#ifndef ALCHEMY_ABI_BATTLE_EFFECTS_ORBITING_PARTICLES_UPDATE_MAIN
#define ALCHEMY_ABI_BATTLE_EFFECTS_ORBITING_PARTICLES_UPDATE_MAIN

/* Binding layer — not production source. Address / far-call ABI only. */

void Func_08009240(struct OrbitingParticle *, s32);

#define Battle_Apply Func_08009240

#endif
