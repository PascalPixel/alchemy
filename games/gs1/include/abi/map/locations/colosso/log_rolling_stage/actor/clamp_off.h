#ifndef ALCHEMY_ABI_MAP_LOCATIONS_COLOSSO_LOG_ROLLING_STAGE_ACTOR_CLAMP_OFF
#define ALCHEMY_ABI_MAP_LOCATIONS_COLOSSO_LOG_ROLLING_STAGE_ACTOR_CLAMP_OFF

/* Binding layer — not production source. Address / far-call ABI only. */

extern void Func_02005440(StageActor *, s32);
extern void Func_02005486(StageActor *, s32, s32, s32);
extern void Func_02005494(StageActor *);
extern void Func_0200566e(s32, s32);
extern void Func_02005620(s32, s32, s32);

#define Actor_Apply Func_02005440
#define Actor_Apply2 Func_0200566e
#define Actor_Do Func_02005494
#define Actor_Place Func_02005620
#define Actor_Run Func_02005556
#define Actor_SetMode Func_02005486
#define gCell Data_02000240

#endif
