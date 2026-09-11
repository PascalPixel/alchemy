#ifndef ALCHEMY_ABI_OVERLAYS_SCENE_ACTOR_TRANSITION_RUN_ACTOR_8_TRANSITION
#define ALCHEMY_ABI_OVERLAYS_SCENE_ACTOR_TRANSITION_RUN_ACTOR_8_TRANSITION

/* Binding layer — not production source. Address / far-call ABI only. */

extern void Func_02003182(s32);
extern s32 Func_02000efe(s32, s32, s32);
extern void Func_02003194(s32);
extern void Func_020030b2(s32);
extern void Func_02002ff0(s32);
extern void Func_02002370(void);

#define Actor_Do Func_02003182
#define Actor_Do2 Func_02003194
#define Actor_Do3 Func_020030b2
#define Actor_Do4 Func_02002ff0
#define Actor_Place Func_02000efe
#define Actor_Run Func_02002370

#endif
