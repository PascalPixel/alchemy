#ifndef ALCHEMY_ABI_MAP_LOCATIONS_COLOSSO_LOG_ROLLING_STAGE_FX_SPAWN_PARTICLE
#define ALCHEMY_ABI_MAP_LOCATIONS_COLOSSO_LOG_ROLLING_STAGE_FX_SPAWN_PARTICLE

/* Binding layer — not production source. Address / far-call ABI only. */

extern s32 Func_02005c1a(s32, s32);
extern void Func_02005e84(s32, s32, s32);
extern s32 Func_02005e3a(s32);
extern void Func_02005d80(s32, s32);
extern void Func_02005e8a(s32, s32);

#define Map_Apply Func_02005c1a
#define Map_Apply2 Func_02005d80
#define Map_Apply3 Func_02005e8a
#define Map_Check Func_02005e3a
#define Map_Place Func_02005e84
#define Map_Run Func_02005de2
#define Map_Run2 Func_02005e1a
#define Map_Run3 Func_02005e26

#endif
