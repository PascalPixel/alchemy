#ifndef ALCHEMY_ABI_OVERLAYS_SCENE_STORY_BRANCHING_EVENT_PUSH_ACTOR_AHEAD
#define ALCHEMY_ABI_OVERLAYS_SCENE_STORY_BRANCHING_EVENT_PUSH_ACTOR_AHEAD

/* Binding layer — not production source. Address / far-call ABI only. */

extern s32 Func_02002f88(struct StagedActor *arg0, s32 *arg1);
extern void Func_02002f50(struct StagedActor *arg0, s32 arg1);
extern void Func_02002f40(s32 arg0);
extern void Func_02003156(s32 arg0);
extern void Func_02002f8e(struct StagedActor *arg0, s32 arg1, s32 arg2, s32 arg3);
extern void Func_02002f9e(struct StagedActor *arg0, s32 arg1, s32 arg2, s32 arg3);
extern void Func_02002fac(struct StagedActor *arg0);
extern void Func_02002fbc(struct StagedActor *arg0, s32 arg1);

#define CanStartStagedActorMove Func_02002f88
#define FindBlockingStagedActor Func_020001a2
#define FindElevatedBlockingStagedActor Func_020001ce
#define FindNextStagedActor Func_02000176
#define FinishStagedActorEffect Func_02003178
#define FinishStagedActorMove Func_02002fac
#define GetStagedActor Func_02002f24
#define SelectStagedActorSlot Func_02002f40
#define SetStagedActorMode Func_02002f50
#define SetStagedActorTransition Func_02002fbc
#define StagedActorStepTable Data_0200afd4
#define StartLeadStagedActorMove Func_02002f9e
#define StartNextStagedActorMove Func_02002f8e
#define StartStagedActorEffect Func_02003156

#endif
