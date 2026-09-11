#ifndef ALCHEMY_ABI_OVERLAYS_SCENE_ACTOR_STAGED_MOTION_FIND_CLEAR_ACTOR_POSITION
#define ALCHEMY_ABI_OVERLAYS_SCENE_ACTOR_STAGED_MOTION_FIND_CLEAR_ACTOR_POSITION

/* Binding layer — not production source. Address / far-call ABI only. */

extern s32 Func_020007de(s32 *, s32 *, s32 *);
extern s32 Func_020064a0(u8 *, s32 *);

#define Actor_Apply Func_020064a0
#define Actor_Place Func_020007de
#define gOv Data_0200e1e8
#define gOv2 Data_0200e190

#endif
