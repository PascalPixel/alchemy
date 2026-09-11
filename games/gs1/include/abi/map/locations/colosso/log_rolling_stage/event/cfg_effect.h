#ifndef ALCHEMY_ABI_MAP_LOCATIONS_COLOSSO_LOG_ROLLING_STAGE_EVENT_CFG_EFFECT
#define ALCHEMY_ABI_MAP_LOCATIONS_COLOSSO_LOG_ROLLING_STAGE_EVENT_CFG_EFFECT

/* Binding layer — not production source. Address / far-call ABI only. */

extern void Func_02005340(StageEffect *, s32);
extern void Func_02005350(StageEffect *, s32);
extern void Func_02005426(s32);

#define Map_Apply Func_02005340
#define Map_Apply2 Func_02005350
#define Map_Do Func_02005426
#define Map_Run Func_02005462

#endif
