#ifndef ALCHEMY_ABI_OVERLAYS_SCENE_ACTOR_STAGED_PAIR_STAGED_PAIR
#define ALCHEMY_ABI_OVERLAYS_SCENE_ACTOR_STAGED_PAIR_STAGED_PAIR

/* Binding layer — not production source. Address / far-call ABI only. */

void Func_02002464();
void Func_0200247e();
void Func_020024c6();
void Func_020024cc();
void Func_020024d0();
void Func_020024e6();
void Func_020024e8();
void Func_020024f0();
void Func_020024f6();
void Func_02002504();
void Func_02002506();
void Func_02002516();
void Func_0200251e();
s32 Func_02002544();
s32 Func_0200254e();
void Func_020025e8();
void Func_020025f0();
void Func_02002608();
void Func_02002314();
void Func_020025fc();
void Func_02002624();
void Func_02002638();
void Func_0200263a();
void Func_0200265e();
void Func_0200264c();
void Func_0200267a();
void Func_0200269a();
void Func_020026b6();
void Func_020026da();
void Func_020026f4();
void Func_02002702(s32, s32, s32);
void Func_02002712();
void Func_02002780();
s32 Func_02002234();
s32 Func_0200229c();
s32 Func_020022f2();
s32 Func_02002300();
void Func_0200232a();
s32 Func_02002356();
void Func_02002358();
void Func_02002368();
void Func_02002390();
void Func_02002394();
void Func_0200239e();
s32 Func_020023a0();
void Func_020023d2();
void Func_020023d6();
void Func_020023f0();
void Func_020023f2();
void Func_020023f4();
s32 Func_02002400();
void Func_0200240c();
s32 Func_02002412();
void Func_0200241a();
void Func_0200241c();
void Func_02002434();
void Func_02002444();
void Func_0200244e();
void Func_02002452();
void Func_02002464_a();
void Func_0200246c();
void Func_0200247a();
void Func_02002492();
void Func_0200239e_a();
void Func_0200246c_a();
s32 Func_020024f0_a();
s32 Func_020024fa();
s32 Func_02002514();
void Func_02002564();
s32 Func_02002574();
void Func_02002596();
void Func_020025ae();
void Func_020025c8();
s32 Func_0200258c();
void Func_020025a4();
void Func_020025a6();
void Func_020025ac();
void Func_020025b2();
void Func_020025b6();
void Func_020025b8();
void Func_020025bc();
void Func_020025f6();
void Func_02002672();
void Func_02002686();
void Func_02002692();
s32 Func_020028b4();
s32 Func_020028b4_a();
void Func_020028c4();
void Func_020028f8();
s32 Func_020028fa();
void Func_0200292a();
s32 Func_0200292e();
void Func_0200293e();
void Func_02002974();
void Func_02002984();
s32 Func_02002990();
void Func_020029a4();
void Func_020029b2();
void Func_020029c2();
void Func_0200155c(void *, s32);
void Func_02001574(void *, s32);
void Func_0200160c(void *, s32);
void Func_020015b8(void *, s32);
void Func_02001668(void *, s32);
void Func_02001642();
void Func_0200165c();
void Func_0200179e();
s32 Func_02001700();
s32 Func_02001718();
s32 Func_02001726();
void Func_0200175c();
void Func_0200176c();
s32 Func_02001958(struct StagedActor *arg0, s32 *arg1);
void Func_02001940(struct StagedActor *arg0, s32 arg1);
void Func_02001930(s32 arg0);
void Func_02001a7e(s32 arg0);
void Func_02001976(struct StagedActor *arg0, s32 arg1, s32 arg2, s32 arg3);
void Func_02001986(struct StagedActor *arg0, s32 arg1, s32 arg2, s32 arg3);
void Func_02001994(struct StagedActor *arg0);
void Func_02001aa0(void);
void Func_020019ac(struct StagedActor *arg0, s32 arg1);
s32 Func_02001ada(MovingActor *, ScenePosition *);
s32 Func_02000da6(s32 *, s32 *, s32 *);
s32 Func_02001d14(u8 *, s32 *);
void Func_020026a8(void);
void Func_020014f4(void);
void Func_020022a8(void);
void Func_02002674();
void Func_020028f2(int, int, int);

static __inline__ void Call1(void (*f)(), s32 a0)
{
    f(a0);
}

static __inline__ s32 Value1(s32 (*f)(), s32 a0)
{
    return f(a0);
}

static __inline__ s32 Value2(s32 (*f)(), s32 a0, s32 a1)
{
    return f(a0, a1);
}

static __inline__ void Call3(void (*f)(), s32 a0, s32 a1, s32 a2)
{
    f(a0, a1, a2);
}

static __inline__ void bump_step(s32 amount)
{
    u8 *work = *(u8 **)Data_03001ebc;

    *(u16 *)(work + 0x1d8) = (u16)(*(u16 *)(work + 0x1d8) + amount);
}

static __inline__ s32 Value1_02000fd0(s32 (*f)(), s32 a0)
{
    return f(a0);
}

static __inline__ void Call6(void (*f)(), s32 a0, s32 a1, s32 a2, s32 a3, s32 a4, s32 a5)
{
    f(a0, a1, a2, a3, a4, a5);
}

#define ActorDraw_RunActorEightThresholdScene Func_020010fc
#define ActorDraw_RunActorElevenRecoveryScene Func_02000f44
#define ActorDraw_RunActorNineThresholdScene Func_02001158
#define ActorDraw_SelectActorNineScript Func_02001040
#define Actor_Apply Func_0200155c
#define Actor_Apply2 Func_0200160c
#define Actor_Apply3 Func_02001574
#define Actor_Apply4 Func_020015b8
#define Actor_Apply5 Func_02001668
#define Actor_Apply6 Func_02001ada
#define Actor_Apply7 Func_02001d14
#define Actor_Check17 Func_02002544
#define Actor_Check18 Func_0200254e
#define Actor_Check19 Func_02002990
#define Actor_Check20 Func_02001700
#define Actor_Check21 Func_02001718
#define Actor_Check22 Func_02001726
#define Actor_Place6 Func_02002702
#define Actor_Place7 Func_02000da6
#define Actor_Place8 Func_020028f2
#define Actor_Run Func_020024c4
#define Actor_Run10 Func_02002506
#define Actor_Run11 Func_0200251e
#define Actor_Run12 Func_020024f6
#define Actor_Run13 Func_020024e6
#define Actor_Run14 Func_02002504
#define Actor_Run15 Func_02002516
#define Actor_Run16 Func_020024cc
#define Actor_Run17 Func_020024c4_a
#define Actor_Run18 Func_020024e8
#define Actor_Run19 Func_020025e8
#define Actor_Run2 Func_02002642
#define Actor_Run20 Func_020025f0
#define Actor_Run21 Func_02002608
#define Actor_Run22 Func_02002638
#define Actor_Run23 Func_02002624
#define Actor_Run24 Func_02002314
#define Actor_Run25 Func_020025fc
#define Actor_Run26 Func_0200263a
#define Actor_Run27 Func_0200265e
#define Actor_Run28 Func_0200267a
#define Actor_Run29 Func_0200264c
#define Actor_Run3 Func_02002698
#define Actor_Run30 Func_0200269a
#define Actor_Run31 Func_020026f4
#define Actor_Run32 Func_02002712
#define Actor_Run33 Func_02002780
#define Actor_Run34 Func_020026b6
#define Actor_Run35 Func_020026da
#define Actor_Run36 Func_02000d20
#define Actor_Run37 Func_0200232a
#define Actor_Run38 Func_02002394
#define Actor_Run39 Func_0200239e
#define Actor_Run4 Func_02002464
#define Actor_Run40 Func_02002368
#define Actor_Run41 Func_020023d2
#define Actor_Run42 Func_0200241c
#define Actor_Run43 Func_020023d6
#define Actor_Run44 Func_020023f4
#define Actor_Run45 Func_0200240c
#define Actor_Run46 Func_0200246c
#define Actor_Run47 Func_02002444
#define Actor_Run48 Func_02002434
#define Actor_Run49 Func_02002452
#define Actor_Run5 Func_020024c6
#define Actor_Run50 Func_02002464_a
#define Actor_Run51 Func_0200241a
#define Actor_Run52 Func_0200247a
#define Actor_Run53 Func_0200246c_a
#define Actor_Run54 Func_0200247a_a
#define Actor_Run55 Func_02002492
#define Actor_Run56 Func_0200244e
#define Actor_Run57 Func_02002564
#define Actor_Run58 Func_02002596
#define Actor_Run59 Func_020025ae
#define Actor_Run6 Func_020024d0
#define Actor_Run60 Func_020025c8
#define Actor_Run61 Func_020025a4
#define Actor_Run62 Func_02002672
#define Actor_Run63 Func_02002686
#define Actor_Run64 Func_02002692
#define Actor_Run65 Func_020025f6
#define Actor_Run66 Func_0200292a
#define Actor_Run67 Func_02002984
#define Actor_Run68 Func_020029a4
#define Actor_Run69 Func_020029c2
#define Actor_Run7 Func_020024d0_a
#define Actor_Run70 Func_0200293e
#define Actor_Run71 Func_0200150e
#define Actor_Run72 Func_02001566
#define Actor_Run73 Func_02001692
#define Actor_Run74 Func_02001638
#define Actor_Run75 Func_0200090a
#define Actor_Run76 Func_02001642
#define Actor_Run77 Func_0200165c
#define Actor_Run78 Func_0200179e
#define Actor_Run79 Func_0200175c
#define Actor_Run8 Func_0200247e
#define Actor_Run80 Func_0200176c
#define Actor_Run81 Func_020026a8
#define Actor_Run82 Func_020014f4
#define Actor_Run83 Func_020022a8
#define Actor_Run84 Func_02002674
#define Actor_Run9 Func_020024f0
#define CanStartStagedActorMove Func_02001958
#define FindBlockingStagedActor Func_0200076a
#define FindElevatedBlockingStagedActor Func_02000796
#define FindNextStagedActor Func_0200073e
#define FinishStagedActorEffect Func_02001aa0
#define FinishStagedActorMove Func_02001994
#define GetStagedActor Func_020018ec
#define Scene_RunScene3be Func_02001394
#define Scene_RunScene3beSequenceA Func_02000d88
#define Scene_RunScene3beSequenceB Func_02000df0
#define SelectStagedActorSlot Func_02001930
#define SetStagedActorMode Func_02001940
#define SetStagedActorTransition Func_020019ac
#define StagedActorPairScene_AdvancePair Func_020003a8
#define StagedActorPairScene_FindClearPosition Func_02000758
#define StagedActorPairScene_GetInitialValue Func_02000d14
#define StagedActorPairScene_GetVariantData Func_02000cc0
#define StagedActorPairScene_RotateActorPart Func_020011c8
#define StagedActorPairScene_RunActorTwelveCommand Func_02001384
#define StagedActorPairScene_RunStep Func_020010f0
#define StagedActorPairScene_RunUpdate Func_02001148
#define StagedActorPairScene_SpawnConfiguredEffect Func_0200013c
#define StagedActorPairScene_SpawnPrimaryEffect Func_02000048
#define StagedActorPairScene_SpawnSecondaryEffect Func_020000a0
#define StagedActorPairScene_StopBlockedMotion Func_0200058c
#define StagedActorPairScene_WaitForHeight Func_020011d8
#define StagedActorStepTable Data_020096c0
#define StartLeadStagedActorMove Func_02001986
#define StartNextStagedActorMove Func_02001976
#define StartStagedActorEffect Func_02001a7e
#define gCell Data_02000240
#define gCell2 Data_02000240_t
#define gOv Data_0200995c
#define gOv10 Data_02009ce0
#define gOv11 Data_02009778
#define gOv12 Data_02009700
#define gOv13 Data_02009718
#define gOv14 Data_020097b4
#define gOv15 Data_020097fc
#define gOv16 Data_02009874
#define gOv17 Data_02009784
#define gOv2 Data_02009974
#define gOv3 Data_020099d4
#define gOv4 Data_02009a4c
#define gOv5 Data_02009aac
#define gOv6 Data_02009b3c
#define gOv7 Data_02009b48
#define gOv8 Data_02009bcc
#define gOv9 Data_02009c80
#define gVal Data_00000088
#define gVal2 Data_00000098
#define gVal3 Data_0000009d
#define gVal4 Data_0000009e
#define gVal5 Data_000023cc
#define gWork Data_03001ebc
#define Actor_Check(a0) Value1(Func_02002234, a0)
#define Actor_Check2(a0) Value1(Func_0200229c, a0)
#define Actor_Check3(a0) Value1(Func_020022f2, a0)
#define Actor_Check4(a0) Value1(Func_02002300, a0)
#define Actor_Place(a0, a1, a2) Call3(Func_02002358, a0, a1, a2)
#define Actor_Check5(a0) Value1(Func_02002356, a0)
#define Actor_Place2(a0, a1, a2) Call3(Func_02002390, a0, a1, a2)
#define Actor_Place3(a0, a1, a2) Call3(Func_020023f2, a0, a1, a2)
#define Actor_Do(a0) Call1(Func_020023f0, a0)
#define Actor_Check6(a0, a1) Value2(Func_02002400, a0, a1)
#define Actor_Check7(a0, a1) Value2(Func_020023a0, a0, a1)
#define Actor_Do2(a0) Call1(Func_0200239e_a, a0)
#define Actor_Check8(a0) Value1(Func_02002412, a0)
#define Actor_Check9(a0, a1) Value2(Func_02002574, a0, a1)
#define Actor_Check10(a0, a1) Value2(Func_02002514, a0, a1)
#define Actor_Check11(a0) Value1_02000fd0(Func_020024f0_a, a0)
#define Actor_Check12(a0) Value1_02000fd0(Func_020024fa, a0)
#define Actor_Check13(a0) Value1(Func_0200258c, a0)
#define Actor_Do3(a0) Call1(Func_020025a6, a0)
#define Actor_Do4(a0) Call1(Func_020025ac, a0)
#define Actor_Do5(a0) Call1(Func_020025b2, a0)
#define Actor_Do6(a0) Call1(Func_020025b8, a0)
#define Actor_Do7(a0) Call1(Func_020025b6, a0)
#define Actor_Do8(a0) Call1(Func_020025bc, a0)
#define Actor_Do9(a0) Call1(Func_020028b4, a0)
#define Actor_Check14(a0) Value1(Func_020028b4_a, a0)
#define Actor_SetRect(a0, a1, a2, a3, a4, a5) Call6(Func_020028c4, a0, a1, a2, a3, a4, a5)
#define Actor_Check15(a0) Value1(Func_020028fa, a0)
#define Actor_Place4(a0, a1, a2) Call3(Func_02002974, a0, a1, a2)
#define Actor_SetRect2(a0, a1, a2, a3, a4, a5) Call6(Func_020028f8, a0, a1, a2, a3, a4, a5)
#define Actor_Check16(a0) Value1(Func_0200292e, a0)
#define Actor_Place5(a0, a1, a2) Call3(Func_020029b2, a0, a1, a2)

#endif
