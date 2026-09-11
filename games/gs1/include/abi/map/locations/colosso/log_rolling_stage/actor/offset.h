#ifndef ALCHEMY_ABI_MAP_LOCATIONS_COLOSSO_LOG_ROLLING_STAGE_ACTOR_OFFSET
#define ALCHEMY_ABI_MAP_LOCATIONS_COLOSSO_LOG_ROLLING_STAGE_ACTOR_OFFSET

/* Binding layer — not production source. Address / far-call ABI only. */

extern void Func_020055e2(s32, s32);
extern void Func_020053e2(StageActor *, s32);
extern void Func_02005428(StageActor *, s32, s32, s32);
extern void Func_02005436(StageActor *);

#define Actor_Apply Func_020055e2
#define Actor_Apply2 Func_020053e2
#define Actor_Do Func_02005436
#define Actor_Run Func_020054fa
#define Actor_SetMode Func_02005428
#define gCell Data_02000240

#endif
