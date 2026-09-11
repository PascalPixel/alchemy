#ifndef ALCHEMY_ABI_BATTLE_EFFECTS_PARTICLES_SPAWN_BURST
#define ALCHEMY_ABI_BATTLE_EFFECTS_PARTICLES_SPAWN_BURST

/* Binding layer — not production source. Address / far-call ABI only. */

extern s32 Func_08002304(s32, s32);

#define Battle_Apply Func_08002304
#define Battle_Run Func_080090c8
#define gRom Data_0809fbec
#define gRom2 Data_0809fc04
#define gRom3 Data_080925e1

#endif
