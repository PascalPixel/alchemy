#ifndef ALCHEMY_ABI_BATTLE_EFFECTS_ORBITING_PARTICLES_RUN
#define ALCHEMY_ABI_BATTLE_EFFECTS_ORBITING_PARTICLES_RUN

/* Binding layer — not production source. Address / far-call ABI only. */

void Func_08009240(struct OrbitingParticle *particle, s32 battle_mode);
s32 Func_08096b28(void *resource, s32 battle_mode, s32 size);
void Func_08097384(void);

#define Battle_Apply Func_08009240
#define Battle_Place Func_08096b28
#define Battle_Run Func_0808e4b4
#define Battle_Run2 Func_08097384
#define UpdateOrbitingParticleLeft Func_08099070
#define UpdateOrbitingParticleRight Func_080990cc
#define gCell Data_02000240
#define gIw Data_03001f30

#endif
