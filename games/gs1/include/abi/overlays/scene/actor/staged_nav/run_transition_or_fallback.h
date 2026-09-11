#ifndef ALCHEMY_ABI_OVERLAYS_SCENE_ACTOR_STAGED_NAV_RUN_TRANSITION_OR_FALLBACK
#define ALCHEMY_ABI_OVERLAYS_SCENE_ACTOR_STAGED_NAV_RUN_TRANSITION_OR_FALLBACK

/* Binding layer — not production source. Address / far-call ABI only. */

void Func_02004038(void);
void Func_02004050(void);
int Func_02002798(void);
void Func_020018e0(void);

#define Actor_Run Func_02004038
#define Actor_Run2 Func_02002798
#define Actor_Run3 Func_020018e0
#define Actor_Run4 Func_02004050

#endif
