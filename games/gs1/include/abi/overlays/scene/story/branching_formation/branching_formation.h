#ifndef ALCHEMY_ABI_OVERLAYS_SCENE_STORY_BRANCHING_FORMATION_BRANCHING_FORMATION
#define ALCHEMY_ABI_OVERLAYS_SCENE_STORY_BRANCHING_FORMATION_BRANCHING_FORMATION

/* Binding layer — not production source. Address / far-call ABI only. */

s32 Func_02002d84(struct StagedActor *actor, s32 *position);
void Func_02002d5c(struct StagedActor *actor, s32 mode);
void Func_02002d44(s32 actor_index);
void Func_02002eea(s32 effect_id);
void Func_02002da2(struct StagedActor *actor, s32 x, s32 y, s32 z);
void Func_02002db2(struct StagedActor *actor, s32 x, s32 y, s32 z);
void Func_02002dc0(struct StagedActor *actor);
void Func_02002f0c(void);
void Func_02002dc8(struct StagedActor *actor, s32 mode);
s32 Func_02002f06(Ent *, Desc *);
void Func_0200358e(u8 *object, s32 mode);
u32 Func_0200359e(void);
void Func_020036d2(u8 *object, s32 mode);
void Func_020036dc(u8 *object, s32 mode);
void Func_0200367a(void);
void Func_02003724(void);
s32 Func_02000eea(struct PlacementResult_02000a68 *res);
void Func_02001096(struct PlacementResult_02000a68 res);
void Func_02003728(s32, s32);
void Func_0200371c(s32, s32, s32);
void Func_020036ba(s32);
void Func_020037e0(s32);
void Func_02003748(s32, s32);
void Func_020036c0(s32, s32, s32, s32, s32, s32);
s32 Func_02000d3c(s32, s32, s32, s32, s32, s32);
void Func_020036fc(s32);
void Func_020036f8(u8 *, s32);
void Func_02003750(void);
void Func_020036f0(struct StagedActorEffect *actor, s32 mode);
void Func_020036d6(s32 actor_index);
void Func_0200387c(s32 effect_id);
void Func_02003704(struct StagedActorEffect *actor, s32 mode);
void Func_0200376e(struct StagedActorEffect *actor, s32 mode);
s32 Func_020037ec(s32 layer, s32 cell_x, s32 cell_z);
void Func_0200373c(struct StagedActorEffect *actor, s32 mode);
void Func_0200378c(struct StagedActorEffect *actor, s32 mode);
void Func_02003830(s32 frames, s32 mode);
void Func_02003744(s32 mode);
void Func_02003756(s32 frames);
void Func_0200376c(s32 mode);
void Func_02003810(void);
void Func_0200176a(u32 *arg0);
s32 Func_02000ef4();
void Func_02001530();
void Func_02001582();
void Func_02001588();
void Func_02001a22();
s32 Func_0200386c();
void Func_02003878();
s32 Func_020038a6();
void Func_020038a8();
s32 Func_020038d2();
s32 Func_020038da();
s32 Func_020038e0();
void Func_02003976();
void Func_02003980();
void Func_0200398a();
void Func_02003994();
void Func_0200399e();
void Func_02005434(s32 arg0, s32 arg1);
s32 Func_0200537a(s32 arg0);
void Func_02005454(s32 a, s32 b, s32 c);
s32 Func_02005392(s32 a);
void Func_02005376(struct Struct2798 *p);
void Func_02005506(s32);
void Func_020053d6(struct Obj *, s32);
void Func_0200540e(struct Obj *, s32, s32, s32);
void Func_020053f6(struct Obj *, u8 *);
void Func_02005524(s32 arg0, s32 arg1);
void Func_0200552c(s32 arg0, s32 arg1);
void Func_0200563a(s32 arg0);
s32 Func_020054ca(struct Struct288c *arg0, u8 *arg1);
s32 Func_0200371e(struct StagedActorEffect *actor,
                         struct StagedActorEffectRequest *request);
void Func_02002c0c();
void Func_02002cec();
void Func_02002b84();
s32 Func_02002bf4();
u8 * Func_02002c24();
void Func_02002bec();
void Func_02002c34();
void Func_02002c7c();
void Func_02002bc4();
void Func_02002d04();
void Func_02002d0c();
void Func_02002c2c();
void Func_02002c64();
void Func_02002c3c();
void Func_02002780();
void Func_02002cdc();
void Func_02002c04();
void Func_02002ccc();
void Func_02002ca4();
void Func_02002cb4();
void Func_02002768();
void Func_02002c8c();
void Func_02002d24();
void Func_02002cf4();
void Func_02002cfc();
s32 Func_02002b8c();
void Func_02002c6c();
void Func_02002c84();
s32 Func_02002b94();
void Func_02002c9c();
void Func_02002cd4();
void Func_02002c44();
void Func_02002c4c();
s32 Func_02002cbc();
s32 Func_02002c1c();
void Func_02002c94();
void Func_02002cc4();
void Func_02002ce4();
void Func_02002c54();
void Func_02002c74();
s32 Func_02002bfc();
void Func_02002d14();
void Func_02002c14();

static __inline__ s32 Value1(s32 (*f)(), s32 a0)
{
    extern s16 Data_02000240[];

    return f(a0);
}

static __inline__ void Call6(void (*f)(), s32 a0, s32 a1, s32 a2, s32 a3, s32 a4, s32 a5)
{
    extern s16 Data_02000240[];

    f(a0, a1, a2, a3, a4, a5);
}

static __inline__ void Call1(void (*f)(), s32 a0)
{
    f(a0);
}

static __inline__ void Call2(void (*f)(), s32 a0, s32 a1)
{
    f(a0, a1);
}

static __inline__ void Call3(void (*f)(), s32 a0, s32 a1, s32 a2)
{
    f(a0, a1, a2);
}

static __inline__ void Call4(void (*f)(), s32 a0, s32 a1, s32 a2, s32 a3)
{
    f(a0, a1, a2, a3);
}

static __inline__ s32 Value1(s32 (*f)(), s32 a0)
{
    return f(a0);
}

static __inline__ s32 Value2(s32 (*f)(), s32 a0, s32 a1)
{
    return f(a0, a1);
}

#define Actor_AlternateSlots13To16Field0c Func_02002ad8
#define Actor_CheckRegionTrigger Func_0200288c
#define Actor_PassSubjectOffsetPosition Func_02000c14
#define Actor_ResetMotionWhenAheadBlocked Func_020002a8
#define Actor_SetPairZeroAndValue Func_02002780
#define Actor_UpdateRandomCounterMode Func_020009fc
#define AdvanceStagedActorEffect Func_02003744
#define BeginStagedActorEffect Func_02003750
#define CanStartStagedActorEffect Func_0200371e
#define CanStartStagedActorMove Func_02002d84
#define DrawPlacement Func_02000a68
#define Effect_AdvanceAngleAndFinishWhenParked Func_02002798
#define Effect_SpawnObject26EveryEightFrames Func_020027c8
#define FindBlockingStagedActor Func_020001a2
#define FindElevatedBlockingStagedActor Func_020001ce
#define FindNextStagedActor Func_02000176
#define FindStagedActorProbePosition Func_02000474
#define FinishStagedActorEffect Func_02003810
#define FinishStagedActorMove Func_02002dc0
#define GetStagedActor Func_02002cf8
#define GetStagedActorEffect Func_0200374e
#define OvObj_ClearPendingAndRestoreMode Func_020009dc
#define OvObj_SelectValueByFrameBit1 Func_02002864
#define PlacementResult Func_02000a68
#define PrepareStagedActorEffect Func_0200376e
#define RestoreStagedActorEffect Func_0200376c
#define SceneData_GetTableB294 Func_02000c60
#define SceneData_GetTableb06c Func_02000a4c
#define SceneData_GetTableb0cc Func_02000a58
#define SceneData_GetTableb0e4 Func_02000a60
#define SceneData_ReturnZero Func_02000a54
#define Scene_RunBranchingFormationPresentation Func_02000d3c
#define Scene_RunScene391 Func_02000c68
#define Scene_RunSplitPairSteps Func_02002768
#define Scene_RunTile10x20Transition Func_02000a68
#define SelectStagedActorEffectSlot Func_020036d6
#define SelectStagedActorSlot Func_02002d44
#define SetStagedActorEffectMode Func_020036f0
#define SetStagedActorEffectTransition Func_0200378c
#define SetStagedActorMode Func_02002d5c
#define SetStagedActorMotionMode Func_02003704
#define SetStagedActorTransition Func_02002dc8
#define StagedActorStepTable Data_0200ad68
#define StagedActor_PushActorAhead Func_020000c4
#define StagedActor_RunStepEffect Func_02000b1c
#define StartLeadStagedActorMove Func_02002db2
#define StartNextStagedActorMove Func_02002da2
#define StartStagedActorEffect Func_02002eea
#define StartStagedActorEffectMove Func_0200373c
#define StartStagedActorEffectSound Func_0200387c
#define Story_Apply Func_0200358e
#define Story_Apply10 Func_02005524
#define Story_Apply11 Func_0200552c
#define Story_Apply12 Func_020054ca
#define Story_Apply2 Func_020036d2
#define Story_Apply3 Func_020036dc
#define Story_Apply4 Func_02003728
#define Story_Apply5 Func_02003748
#define Story_Apply6 Func_020036f8
#define Story_Apply7 Func_02005434
#define Story_Apply8 Func_020053d6
#define Story_Apply9 Func_020053f6
#define Story_BytePtr Func_02002c24
#define Story_Check10 Func_02000eea
#define Story_Check11 Func_020038a6
#define Story_Check12 Func_02000ef4
#define Story_Check13 Func_020038da
#define Story_Check14 Func_0200537a
#define Story_Check15 Func_02005392
#define Story_Do10 Func_02005506
#define Story_Do11 Func_0200563a
#define Story_Do4 Func_02001096
#define Story_Do5 Func_020036ba
#define Story_Do6 Func_020037e0
#define Story_Do7 Func_020036fc
#define Story_Do8 Func_0200176a
#define Story_Do9 Func_02005376
#define Story_EntOp Func_02002f06
#define Story_Place8 Func_0200371c
#define Story_Place9 Func_02005454
#define Story_Run10 Func_02003848
#define Story_Run11 Func_020053a6
#define Story_Run12 Func_02005702
#define Story_Run13 Func_0200572a
#define Story_Run14 Func_02005752
#define Story_Run15 Func_0200577a
#define Story_Run16 Func_02000030
#define Story_Run17 Func_0200006c
#define Story_Run18 Func_02000244
#define Story_Run19 Func_0200359e
#define Story_Run20 Func_0200367a
#define Story_Run21 Func_02003724
#define Story_Run22 Func_02001530
#define Story_Run23 Func_020038a8
#define Story_Run24 Func_02001582
#define Story_Run25 Func_02001588
#define Story_Run26 Func_02001a22
#define Story_Run27 Func_02003976
#define Story_Run28 Func_02003980
#define Story_Run29 Func_0200398a
#define Story_Run30 Func_02003994
#define Story_Run31 Func_0200399e
#define Story_Run32 Func_02002c0c
#define Story_Run33 Func_02002b84
#define Story_Run34 Func_02002bec
#define Story_Run35 Func_02002c34
#define Story_Run36 Func_02002bc4
#define Story_Run37 Func_02002d04
#define Story_Run38 Func_02002d0c
#define Story_Run39 Func_02002c7c
#define Story_Run40 Func_02002c3c
#define Story_Run41 Func_02002c04
#define Story_Run42 Func_02002ccc
#define Story_Run43 Func_02002ca4
#define Story_Run44 Func_02002c8c
#define Story_Run45 Func_02002d24
#define Story_Run46 Func_02002cfc
#define Story_Run47 Func_02002c2c
#define Story_Run48 Func_02002c84
#define Story_Run49 Func_02002c9c
#define Story_Run50 Func_02002cd4
#define Story_Run51 Func_02002c44
#define Story_Run52 Func_02002c4c
#define Story_Run53 Func_02002c94
#define Story_Run54 Func_02002cc4
#define Story_Run55 Func_02002c54
#define Story_Run56 Func_02002c74
#define Story_Run57 Func_02002d14
#define Story_Run58 Func_02002c14
#define Story_Run7 Func_02000342
#define Story_Run8 Func_020036ee
#define Story_Run9 Func_0200372a
#define Story_SetMode Func_0200540e
#define Story_SetRect2 Func_020036c0
#define TestStagedActorEffectCell Func_020037ec
#define WaitSceneFrames Func_02003756
#define WaitStagedActorEffect Func_02003830
#define gCam Data_03001e70
#define gCell Data_02000240
#define gIw Data_03001e40
#define gOv Data_0200ada8
#define gOv10 Data_0200a7c9
#define gOv11 Data_0200a975
#define gOv12 Data_0200aad9
#define gOv13 Data_0200ae20
#define gOv14 Data_0200ae54
#define gOv15 Data_0200ae88
#define gOv16 Data_0200aebc
#define gOv17 Data_0200af48
#define gOv18 Data_0200af6c
#define gOv19 Data_0200afc8
#define gOv2 Data_0200b398
#define gOv20 Data_0200b024
#define gOv3 Data_0200b2d0
#define gOv4 Data_0200b394
#define gOv5 Data_0200b390
#define gOv6 Data_0200b2e4
#define gOv7 Data_0200b384
#define gOv8 Data_0200b388
#define gOv9 Data_0200b38c
#define gUnk Data_02010000
#define gWork Data_03001ebc
#define SceneTransition_Phase Data_02000240[225]
#define Story_Check(a0) Value1(Func_0200386c, a0)
#define Story_SetRect(a0, a1, a2, a3, a4, a5) Call6(Func_02003878, a0, a1, a2, a3, a4, a5)
#define Story_Check2(a0) Value1(Func_020038d2, a0)
#define Story_Check3(a0) Value1(Func_020038e0, a0)
#define Story_Run(a0, a1, a2, a3) Call4(Func_02002cec, a0, a1, a2, a3)
#define Story_Check4(a0) Value1(Func_02002bf4, a0)
#define Story_Place(a0, a1, a2) Call3(Func_02002c7c, a0, a1, a2)
#define Story_Do(a0) Call1(Func_02002b84, a0)
#define Story_Place2(a0, a1, a2) Call3(Func_02002c2c, a0, a1, a2)
#define Story_Place3(a0, a1, a2) Call3(Func_02002c64, a0, a1, a2)
#define Story_Run2(a0, a1) Call2(Func_02002c34, a0, a1)
#define Story_Place4(a0, a1, a2) Call3(Func_02002cdc, a0, a1, a2)
#define Story_Do2(a0) Call1(Func_02002cb4, a0)
#define Story_Run3(a0, a1) Call2(Func_02002cf4, a0, a1)
#define Story_Check5(a0, a1) Value2(Func_02002b8c, a0, a1)
#define Story_Place5(a0, a1, a2) Call3(Func_02002c6c, a0, a1, a2)
#define Story_Check6(a0) Value1(Func_02002b94, a0)
#define Story_Do3(a0) Call1(Func_02002c04, a0)
#define Story_Check7(a0, a1) Value2(Func_02002cbc, a0, a1)
#define Story_Check8(a0, a1) Value2(Func_02002c1c, a0, a1)
#define Story_Place6(a0, a1, a2) Call3(Func_02002c94, a0, a1, a2)
#define Story_Run4(a0, a1) Call2(Func_02002ca4, a0, a1)
#define Story_Place7(a0, a1, a2) Call3(Func_02002ccc, a0, a1, a2)
#define Story_Run5(a0, a1) Call2(Func_02002ce4, a0, a1)
#define Story_Run6(a0, a1) Call2(Func_02002c9c, a0, a1)
#define Story_Check9(a0) Value1(Func_02002bfc, a0)

#endif
