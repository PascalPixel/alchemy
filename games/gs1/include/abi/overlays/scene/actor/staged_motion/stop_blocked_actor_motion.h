#ifndef ALCHEMY_ABI_OVERLAYS_SCENE_ACTOR_STAGED_MOTION_STOP_BLOCKED_ACTOR_MOTION
#define ALCHEMY_ABI_OVERLAYS_SCENE_ACTOR_STAGED_MOTION_STOP_BLOCKED_ACTOR_MOTION

/* Binding layer — not production source. Address / far-call ABI only. */

extern s32 Func_02006266(StagedActor *, StagedMoveTarget *);

#define Actor_Apply Func_02006266
#define Actor_Run Func_02000342
#define gOv Data_0200e190
#define gOv2 Data_0200e1d0

#endif
