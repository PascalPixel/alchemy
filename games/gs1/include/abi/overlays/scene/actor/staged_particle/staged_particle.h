#ifndef ALCHEMY_ABI_OVERLAYS_SCENE_ACTOR_STAGED_PARTICLE_STAGED_PARTICLE
#define ALCHEMY_ABI_OVERLAYS_SCENE_ACTOR_STAGED_PARTICLE_STAGED_PARTICLE

/* Binding layer — not production source. Address / far-call ABI only. */

s32 Func_02000f8c(struct StagedActor *arg0, s32 *arg1);
void Func_02000f84(struct StagedActor *arg0, s32 arg1);
void Func_02000f54(s32 arg0);
void Func_0200104a(s32 arg0);
void Func_02000faa(struct StagedActor *arg0, s32 arg1, s32 arg2, s32 arg3);
void Func_02000fba(struct StagedActor *arg0, s32 arg1, s32 arg2, s32 arg3);
void Func_02000fc8(struct StagedActor *arg0);
void Func_0200106c(void);
void Func_02000ff0(struct StagedActor *arg0, s32 arg1);
s32 Func_0200110e(struct StagedActor *, struct StagedActorProbePoint *);
s32 Func_020007de(s32 *, s32 *, s32 *);
s32 Func_02001348(u8 *, s32 *);
void Func_0200181a(void);
void Func_02001846(void);
s32 Func_02000e7a(Query *result);
void Func_02001026(Query result);
void Func_0200184c();
void Func_0200184c_a();
void Func_02001878(void);
void Func_0200188e(s32, s32, s32);
void Func_020018bc(s32, s32);
s32 Func_0200187c(s32, s32);
void Func_020008c0(s32 id);
s32 Func_020018a2(s32 flagId);
void Func_0200164e();
u16 Func_02001614(u16, s32);
void Func_020016d2();
void Func_020016b6();
void Func_02001978();
s32 Func_020019fa(s32);
s32 Func_02001a10(s32);
s32 Func_02001a2a(s32);
s32 Func_02001a28(void);
s32 Func_02001a2e(void);
void Func_02001ae6(OrbitingSceneObject *, s32);
s32 Func_02001b0c(s32);
void Func_02001b3c(s32);
s32 Func_02001b14(u8, s32, u8 *);
void Func_02001b12(s32);

#define ActorProbeOffsets Data_02008f20
#define ActorSearchStep Data_02008ec8
#define Actor_Apply Func_020018bc
#define Actor_Apply2 Func_0200187c
#define Actor_Apply3 Func_02001614
#define Actor_CalculateFixedPointPositionDistance Func_02000030
#define Actor_Check Func_02000e7a
#define Actor_Check2 Func_020018a2
#define Actor_Check3 Func_020019fa
#define Actor_Check4 Func_02001a10
#define Actor_Check5 Func_02001a2a
#define Actor_Check6 Func_02001a28
#define Actor_Check7 Func_02001a2e
#define Actor_Do Func_02001026
#define Actor_Far Data_02008f20
#define Actor_Far10 Func_02001524
#define Actor_Far11 Func_020014ec
#define Actor_Far12 Func_020014f6
#define Actor_Far13 Func_020015b4
#define Actor_Far14 Func_02001510
#define Actor_Far15 Func_0200158e
#define Actor_Far16 Func_020015a6
#define Actor_Far17 Func_02001590
#define Actor_Far18 Func_020015bc
#define Actor_Far19 Func_020015d4
#define Actor_Far2 Data_02008ec8
#define Actor_Far20 Func_020015ec
#define Actor_Far21 Func_020015ca
#define Actor_Far22 Func_0200162c
#define Actor_Far23 Func_02001632
#define Actor_Far24 Func_02001590_a
#define Actor_Far25 Func_020015fa
#define Actor_Far26 Func_02001656
#define Actor_Far27 Func_02001708
#define Actor_Far3 Func_02001460
#define Actor_Far4 Func_0200146c
#define Actor_Far5 Func_02001504
#define Actor_Far6 Func_0200152c
#define Actor_Far7 Func_020014ea
#define Actor_Far8 Func_02001544
#define Actor_Far9 Func_0200153a
#define Actor_FindActorAtFixedPointPosition Func_0200006c
#define Actor_Place Func_0200188e
#define Actor_Run Func_02000608
#define Actor_Run10 Func_02001880
#define Actor_Run11 Func_0200173a
#define Actor_Run12 Func_0200134c
#define Actor_Run13 Func_02001352
#define Actor_Run14 Func_02001358
#define Actor_Run15 Func_02001564
#define Actor_Run16 Func_0200164e
#define Actor_Run17 Func_020016d2
#define Actor_Run18 Func_020016b6
#define Actor_Run19 Func_02001978
#define Actor_Run2 Func_020018d2
#define Actor_Run3 Func_020018e0
#define Actor_Run4 Func_020018ee
#define Actor_Run5 Func_0200181a
#define Actor_Run6 Func_02001846
#define Actor_Run7 Func_0200184c
#define Actor_Run8 Func_0200184c_a
#define Actor_Run9 Func_02001878
#define Actor_RunPlacementQuery Func_020009f8
#define AllocateEffectTransfer Func_02001aec
#define CanStartStagedActorMove Func_02000f8c
#define CheckStagedActorProbePosition Func_0200110e
#define ConfigurePaletteTransfer Func_02001b14
#define Effect_AdjustColorChannels Func_02000b24
#define Effect_AdjustPaletteWindow Func_02000abc
#define Effect_InitOrbitingParticle Func_02000cb4
#define Effect_UpdateOrbitingParticle Func_02000c4c
#define FindBlockingStagedActor Func_020001a2
#define FindElevatedBlockingStagedActor Func_020001ce
#define FindNextStagedActor Func_02000176
#define FindStagedActorAtProbe Func_02000342
#define FinishStagedActorEffect Func_0200106c
#define FinishStagedActorMove Func_02000fc8
#define GetOrbitingSceneObject Func_02001af4
#define GetStagedActor Func_02000f08
#define IsGameFlagSet Func_02001b0c
#define LoadEffectResource Func_02001b3c
#define NormalizeOrbitingSceneObject Func_02001ae6
#define Particle Func_02000c4c
#define RefreshStagedActor Func_0200157c
#define ReleaseEffectTransfer Func_02001b12
#define ResolveActorSearchContext Func_020007de
#define SceneData_GetTable8f80 Func_020009dc
#define SceneData_GetTable8fe0 Func_020009e8
#define SceneData_GetTable8ff0 Func_020009f0
#define SceneData_GetTable9068 Func_02000a60
#define SceneData_ReturnZero Func_020009e4
#define Scene_SetupActor11Effect181 Func_02000a2c
#define Scene_SetupEntryActors8To11 Func_02000a68
#define SelectStagedActorSlot Func_02000f54
#define SetStagedActorMode Func_02000f84
#define SetStagedActorTransition Func_02000ff0
#define StagedActorDirectionSteps Data_02008ec8
#define StagedActorProbeValues Data_02008f08
#define StagedActorStepTable Data_02008ec8
#define StagedActor_AdvancePair Func_020000c4
#define StagedActor_CheckProbe Func_020002a8
#define StagedActor_FindClearPosition Func_02000474
#define StartLeadStagedActorMove Func_02000fba
#define StartNextStagedActorMove Func_02000faa
#define StartStagedActorEffect Func_0200104a
#define State_FillGridAttributeRectangle Func_02000244
#define TestActorPosition Func_02001348
#define Value Func_02008c4d
#define gCam Data_03001e70
#define gUnk Data_02010000
#define gWork Data_03001ebc

#endif
