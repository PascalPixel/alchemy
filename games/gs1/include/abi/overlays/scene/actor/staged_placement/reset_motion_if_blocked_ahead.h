#ifndef ALCHEMY_ABI_OVERLAYS_SCENE_ACTOR_STAGED_PLACEMENT_RESET_MOTION_IF_BLOCKED_AHEAD
#define ALCHEMY_ABI_OVERLAYS_SCENE_ACTOR_STAGED_PLACEMENT_RESET_MOTION_IF_BLOCKED_AHEAD

/* Binding layer — not production source. Address / far-call ABI only. */

extern s32 Func_0200171a(
    struct StagedActor *, struct StagedActorProbePoint *);

#define CheckStagedActorProbePosition Func_0200171a
#define FindStagedActorAtProbe Func_02000342
#define StagedActorDirectionSteps Data_02009554
#define StagedActorProbeValues Data_02009594

#endif
