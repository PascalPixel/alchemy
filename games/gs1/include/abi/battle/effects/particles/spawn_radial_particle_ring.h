#ifndef ALCHEMY_ABI_BATTLE_EFFECTS_PARTICLES_SPAWN_RADIAL_PARTICLE_RING
#define ALCHEMY_ABI_BATTLE_EFFECTS_PARTICLES_SPAWN_RADIAL_PARTICLE_RING

/* Binding layer — not production source. Address / far-call ABI only. */

s32 Func_0800231c(s32);
s32 Func_08002322(s32);
void Func_0809a484(s32, s32, s32, s32, s32, s32, s32, s32 *);

#define Battle_Check Func_0800231c
#define Battle_Check2 Func_08002322
#define Battle_SetRect Func_0809a484
#define gRom Data_0809a65d

#endif
