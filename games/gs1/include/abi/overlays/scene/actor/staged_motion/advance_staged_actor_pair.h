#ifndef ALCHEMY_ABI_OVERLAYS_SCENE_ACTOR_STAGED_MOTION_ADVANCE_STAGED_ACTOR_PAIR
#define ALCHEMY_ABI_OVERLAYS_SCENE_ACTOR_STAGED_MOTION_ADVANCE_STAGED_ACTOR_PAIR

/* Binding layer — not production source. Address / far-call ABI only. */

extern s32 Func_020060e4(struct StagedActor *arg0, s32 *arg1);
extern void Func_020060ac(struct StagedActor *arg0, s32 arg1);
extern void Func_0200604c(s32 arg0);
extern void Func_02006312(s32 arg0);
extern void Func_020060f2(struct StagedActor *arg0, s32 arg1, s32 arg2, s32 arg3);
extern void Func_02006102(struct StagedActor *arg0, s32 arg1, s32 arg2, s32 arg3);
extern void Func_02006110(struct StagedActor *arg0);
extern void Func_02006334(void);
extern void Func_02006118(struct StagedActor *arg0, s32 arg1);

#define CanStartStagedActorMove Func_020060e4
#define FindBlockingStagedActor Func_020001a2
#define FindElevatedBlockingStagedActor Func_020001ce
#define FindNextStagedActor Func_02000176
#define FinishStagedActorEffect Func_02006334
#define FinishStagedActorMove Func_02006110
#define GetStagedActor Func_020060b8
#define SelectStagedActorSlot Func_0200604c
#define SetStagedActorMode Func_020060ac
#define SetStagedActorTransition Func_02006118
#define StagedActorStepTable Data_0200e190
#define StartLeadStagedActorMove Func_02006102
#define StartNextStagedActorMove Func_020060f2
#define StartStagedActorEffect Func_02006312

#endif
