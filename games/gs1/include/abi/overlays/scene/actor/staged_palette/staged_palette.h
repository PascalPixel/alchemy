#ifndef ALCHEMY_ABI_OVERLAYS_SCENE_ACTOR_STAGED_PALETTE_STAGED_PALETTE
#define ALCHEMY_ABI_OVERLAYS_SCENE_ACTOR_STAGED_PALETTE_STAGED_PALETTE

/* Binding layer — not production source. Address / far-call ABI only. */

s32 Func_02000fec(struct StagedActor *arg0, s32 *arg1);
void Func_02000fe4();
void Func_02000fe4_a(s32);
void Func_02001092(s32 arg0);
void Func_0200100a(struct StagedActor *arg0, s32 arg1, s32 arg2, s32 arg3);
void Func_0200101a(struct StagedActor *arg0, s32 arg1, s32 arg2, s32 arg3);
void Func_02001028(struct StagedActor *arg0);
void Func_020010b4(void);
void Func_02001050(struct StagedActor *arg0, s32 arg1);
s32 Func_0200116e(struct StagedActor *, struct StagedActorProbePoint *);
s32 Func_020007de(s32 *, s32 *, s32 *);
s32 Func_020013a8(u8 *, s32 *);
void Func_02001872(void);
s32 Func_02000e7a(struct PlacementResult *result);
void Func_02001026(struct PlacementResult result);
void Func_02001918_a(s32);
void Func_020018b8(s32, s32, s32, s32, s32, s32);
s32 Func_02000ccc(s32, s32, s32, s32, s32, s32);
void Func_0200191a(void);
s32 Func_02001916(struct StagedActorEffect *actor,
                         struct StagedActorEffectRequest *request);
void Func_02001948(void);
void Func_02001908(struct StagedActorEffect *actor, s32 mode);
void Func_02001906(s32 actor_index);
void Func_0200191c(struct StagedActorEffect *actor, s32 mode);
void Func_02001966(struct StagedActorEffect *actor, s32 mode);
void Func_02001954(struct StagedActorEffect *actor, s32 mode);
void Func_02001984(struct StagedActorEffect *actor, s32 mode);
void Func_020019e8(s32 frames, s32 mode);
void Func_02001974(s32 mode);
void Func_02001986(s32 frames);
void Func_0200199c(s32 mode);
void Func_02001a08(void);
s32 Func_020014ce();
s32 Func_02001a70();
void Func_02001a7c();
s32 Func_02000e90();
void Func_02001aac();
void Func_0200151e();
void Func_02001524();
s32 Func_02001ac6();
void Func_020019ee(void);
void Func_02001a72(void);
void Func_02001a56(void);
void Func_02001b98(s32, s32);
s32 Func_02001b32();
s32 Func_02001b40();
s32 Func_02001b4e();

#define ActorProbeOffsets Data_02008f68
#define ActorSearchStep Data_02008f10
#define Actor_Apply Func_02001b98
#define Actor_Check Func_02000e7a
#define Actor_Check2 Func_02001b32
#define Actor_Check3 Func_02001b40
#define Actor_Check4 Func_02001b4e
#define Actor_Do Func_02000fe4_a
#define Actor_Do2 Func_02001026
#define Actor_Do3 Func_02001918_a
#define Actor_Far Data_02008f68
#define Actor_Far10 Func_0200157c_a
#define Actor_Far11 Func_0200154c
#define Actor_Far12 Func_02001556
#define Actor_Far13 Func_020015fc
#define Actor_Far14 Func_02001570
#define Actor_Far15 Func_020015e6
#define Actor_Far16 Func_020015f6
#define Actor_Far17 Func_020015e0
#define Actor_Far18 Func_02001614
#define Actor_Far19 Func_0200162c
#define Actor_Far2 Data_02008f10
#define Actor_Far20 Func_0200163c
#define Actor_Far21 Func_0200161a
#define Actor_Far22 Func_02001674
#define Actor_Far23 Func_0200167a
#define Actor_Far24 Func_020015f0
#define Actor_Far25 Func_0200165a
#define Actor_Far26 Func_020016b6
#define Actor_Far27 Func_02001750
#define Actor_Far3 Func_020014b0
#define Actor_Far4 Func_020014bc
#define Actor_Far5 Func_02001554
#define Actor_Far6 Func_0200157c
#define Actor_Far7 Func_02001542
#define Actor_Far8 Func_0200159c
#define Actor_Far9 Func_0200158a
#define Actor_Run Func_02000608
#define Actor_Run10 Func_02001900
#define Actor_Run11 Func_020018de
#define Actor_Run12 Func_02001872
#define Actor_Run13 Func_0200191a
#define Actor_Run14 Func_020019b4_b
#define Actor_Run15 Func_020019ee
#define Actor_Run16 Func_02001a72
#define Actor_Run17 Func_02001a56
#define Actor_Run2 Func_0808a018
#define Actor_Run3 Func_0808a020
#define Actor_Run4 Func_020018e0
#define Actor_Run5 Func_020018dc
#define Actor_Run6 Func_020018b2
#define Actor_Run7 Func_02001918
#define Actor_Run8 Func_020018f2
#define Actor_Run9 Func_020018ee
#define Actor_SetRect Func_020018b8
#define Actor_SetRect2 Func_02000ccc
#define AdvanceStagedActorEffect Func_02001974
#define ApplyResource393Position Func_0200168a
#define BeginStagedActorEffect Func_02001948
#define CanStartStagedActorEffect Func_02001916
#define CanStartStagedActorMove Func_02000fec
#define CheckStagedActorProbePosition Func_0200116e
#define DrawSceneBeatRectangle Func_02001a7c
#define FillSceneTileAttributes Func_02000e90
#define FindBlockingStagedActor Func_020001a2
#define FindElevatedBlockingStagedActor Func_020001ce
#define FindNextStagedActor Func_02000176
#define FindStagedActorAtProbe Func_02000342
#define FinishStagedActorEffect Func_02001a08
#define FinishStagedActorMove Func_02001028
#define GetResource393Object Func_02001a38
#define GetSceneBeatSubject Func_02001aa2
#define GetScenePresentationSubject Func_02001ad6
#define GetStagedActor Func_02000f58
#define GetStagedActorEffect Func_0200193e
#define IsSceneFlag0201Set Func_02001a70
#define IsSceneFlag0845Set Func_02001ac6
#define PrepareStagedActorEffect Func_02001966
#define RefreshStagedActor Func_020015dc
#define ResolveActorSearchContext Func_020007de
#define Resource393SharedWork Data_02000240
#define RestoreStagedActorEffect Func_0200199c
#define RunPhase516Followup Func_02001900_a
#define RunSceneBeat10 Func_020014ce
#define RunSceneBeat8 Func_0200151e
#define RunSceneBeat9 Func_02001524
#define SelectStagedActorEffectSlot Func_02001906
#define SelectStagedActorSlot Func_02000fe4
#define SetScenePresentationMode Func_02001aac
#define SetStagedActorEffectMode Func_02001908
#define SetStagedActorEffectTransition Func_02001984
#define SetStagedActorMode Func_02000fe4
#define SetStagedActorMotionMode Func_0200191c
#define SetStagedActorTransition Func_02001050
#define StagedActorDirectionSteps Data_02008f10
#define StagedActorProbeValues Data_02008f50
#define StagedActorStepTable Data_02008f10
#define StartLeadStagedActorMove Func_0200101a
#define StartNextStagedActorMove Func_0200100a
#define StartStagedActorEffect Func_02001092
#define StartStagedActorEffectMove Func_02001954
#define StartStagedActorEffectSound Func_020019b4
#define TestActorPosition Func_020013a8
#define TestStagedActorEffectCell Func_020019b4_a
#define WaitSceneFrames Func_02001986
#define WaitStagedActorEffect Func_020019e8
#define gCam Data_03001e70
#define gUnk Data_02010000
#define gWork Data_03001ebc

#endif
