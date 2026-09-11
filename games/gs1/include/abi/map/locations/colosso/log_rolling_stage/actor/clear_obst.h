#ifndef ALCHEMY_ABI_MAP_LOCATIONS_COLOSSO_LOG_ROLLING_STAGE_ACTOR_CLEAR_OBST
#define ALCHEMY_ABI_MAP_LOCATIONS_COLOSSO_LOG_ROLLING_STAGE_ACTOR_CLEAR_OBST

/* Binding layer — not production source. Address / far-call ABI only. */

extern s32 Func_02004f26(s32, s32, s32);
extern void Func_02004f60(s32, s32, s32, s32, s32, s32);
extern void Func_02004f7a(s32, s32, s32, s32, s32, s32);
extern void Func_02004ffa(s32);

#define Actor_Do Func_02004ffa
#define Actor_Place Func_02004f26
#define Actor_Run Func_02005002
#define Actor_SetRect Func_02004f60
#define Actor_SetRect2 Func_02004f7a

#endif
