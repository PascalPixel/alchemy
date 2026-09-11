#ifndef ALCHEMY_ABI_OVERLAYS_SCENE_STORY_REGION
#define ALCHEMY_ABI_OVERLAYS_SCENE_STORY_REGION

/* Binding layer — not production source. Address / far-call ABI only. */

void Func_02000c7c_a();
void Func_02000d6a_a();
void Func_0200101a_a();
void Func_02000a8e(s32, s32, s32, s32, s32, s32);
void Func_02000aae(s32, s32, s32, s32, s32, s32);
void Func_02000ac6(s32, s32, s32, s32, s32, s32);
void Func_02000ae2(s32, s32, s32, s32, s32, s32);
void Func_02000afa(s32, s32, s32, s32, s32, s32);
void Func_02000b16(s32, s32, s32, s32, s32, s32);
void Func_02000b5c(void);
s32 Func_02000b7e(s32, s32, s32);
void Func_02000b54(s32);
void Func_02000b78(void);
void Func_02000bec(void);
s32 Func_02000c24(s32, s32);
void Func_02000bf2(s32);
void Func_02000c30(s32, s32);
void Func_02000c14(void);
void Func_02000c18(void);
s32 Func_02000c50(s32, s32);
void Func_02000c1e(s32);
void Func_02000c5c(s32, s32);
void Func_02000c44(void);
void Func_02000c4a(s32);
void Func_02000c98(s32);
void Func_02000c88(s32, s32);
void Func_02000d90(s32);
void Func_02000936(void);
void Func_020009d2(void);
s32 Func_02000e04();
void Func_02000e04_a(s32, s32, s32, s32, s32, s32);
void Func_02000e64();
void Func_02000e86(s32, s32);
void Func_02000e8e(s32, s32);
s32 Func_02000e54(s32);
void Func_02000e9a(s32, s32, s32);
void Func_02000e68(u8 *, s32);
s32 Func_020011c0();
void Func_020011a4();
void Func_020011a4_a(s32);
void Func_0200124a();
void Func_020011ca();
void Func_020011dc();
void Func_020011ea();
void Func_020011f0();
void Func_02000be8(void);
void Func_02000b7a();
void Func_02000b82();
void Func_02000b90();
void Func_02000ba0();
void Func_02000ba4();
s32 Func_02000bb6();
void Func_02000bc8();
void Func_02000be4();
void Func_02000bfc();
s32 Func_02000c84();
s32 Func_02000c8c();
void Func_02000c8c_a(s32);
void Func_02000c9c();
s32 Func_02000ca4();
void Func_02000cae();
s32 Func_02000cb0();
s32 Func_02000cba();
void Func_02000cd2();
void Func_02000ce0();
void Func_02000ce2();
void Func_02000ce4();
s32 Func_02000d0e();
s32 Func_02000d0e_a(s32);
void Func_02000d1e();
void Func_02000d26();
void Func_02000d28();
void Func_02000d2e();
void Func_02000d34();
void Func_02000d36();
void Func_02000d38();
void Func_02000d72();
void Func_02000d7c();
void Func_02000d82();
void Func_02000e94();
void Func_02000ea0();
void Func_02000eac();
void Func_02000eb8();
void Func_02000ec4();
void Func_02000ed0();
void Func_02000edc();
void Func_02000ee8();
void Func_02000ef4();
void Func_02000f36();
void Func_02000f3c();
void Func_02000f42();
void Func_02000f44();
void Func_02000f48();
void Func_02000f4c();
void Func_02000f4e();
void Func_02000f54();
void Func_02000f54_a(s32);
void Func_02000f5a();
void Func_02000f5c();
void Func_02000f60();
void Func_02000f64();
void Func_02000f66();
void Func_02000f6c();
void Func_02000f6c_a(s32);
void Func_02000f72();
void Func_02000f74();
void Func_02000f78();
void Func_02000f7c();
void Func_02000f7e();
void Func_02000f84();
void Func_02000f84_a(s32);
void Func_02000f8a();
void Func_02000f8c();
void Func_02000f94();
void Func_02000f9c();
void Func_02000fa4();
void Func_02000fac();
void Func_02000fb4();
s32 Func_02000fd4();
void Func_02001014();
s32 Func_02001020();
void Func_0200102a();
void Func_0200103a();
s32 Func_02001058();
void Func_02001060();
void Func_02001070();
void Func_02001072();
void Func_02001076();
void Func_0200108a();
void Func_02001090();
void Func_02001092();
void Func_020010a2();
void Func_020010a4();
s32 Func_020010c0();
void Func_020010d4();
s32 Func_020010d8();
void Func_02001108();
void Func_02001120();

static __inline__ void Call1(void (*f)(), s32 a0)
{
    extern u8 Data_03001ebc[];

    f(a0);
}

static __inline__ void Call6(void (*f)(), s32 a0, s32 a1, s32 a2, s32 a3, s32 a4, s32 a5)
{
    extern u8 Data_03001ebc[];

    f(a0, a1, a2, a3, a4, a5);
}

static __inline__ void bump_step(s32 amount)
{
    extern u8 Data_03001ebc[];

    u8 *work = *(u8 **)Data_03001ebc;

    *(u16 *)(work + 0x1d8) = (u16)(*(u16 *)(work + 0x1d8) + amount);
}

static __inline__ s32 Value1(s32 (*f)(), s32 a0)
{
    extern u8 Data_03001ebc[];
    void Func_02000c7c();
    void Func_02000d6a();

    return f(a0);
}

static __inline__ void Call3(void (*f)(), s32 a0, s32 a1, s32 a2)
{
    extern u8 Data_03001ebc[];
    void Func_0200101a();

    f(a0, a1, a2);
}

#define ActorDraw_SetSceneCell31AndFlag305 Func_02000424
#define Actor_FindSlotAtTile Func_020007c4
#define DefaultOverlayData Data_02008e9c
#define FinishSceneSetup Func_02000e64
#define PrimaryOverlayData Data_02008ea8
#define Resource398_ImportBankNoOp Func_02000904
#define RunGuardedSceneSetup Func_020004b4
#define RunSceneCommand Func_02000e04
#define RuntimeSelectorTable Data_02000240
#define SceneData_GetTable8bcc Func_02000098
#define SceneData_SelectByRuntimeSelector Func_02000040
#define SceneData_SelectDataByRuntimeSelector Func_020000f4
#define SceneData_SelectSecondaryDataByRuntimeSelector Func_020000a0
#define Scene_DispatchByScenarioId Func_0200046c
#define Scene_RunActor17Steps28AndD2 Func_020002d8
#define Scene_RunActor9Flag882Scene Func_020001f0
#define Scene_RunActorFifteenScene Func_02000280
#define Scene_RunActorSixteenScene Func_020002ac
#define Scene_RunScene398SequenceA Func_02000214
#define Scene_RunScene398SequenceB Func_02000304
#define Scene_RunScene398SequenceC Func_02000538
#define Scene_RunTwoLayoutSteps Func_020001bc
#define SecondaryOverlayData Data_02008efc
#define StagedActor_PushActorAhead Func_020007f8
#define State_ApplyArgMode0AndReturnZero Func_02000030
#define State_ApplyTwoRects Func_02000188
#define State_ClearRuntimeByte17 Func_0200045c
#define State_ConfigureRegion0_0_21x14 Func_02000168
#define State_ConfigureRegion1_0_21x14 Func_02000148
#define State_SetGlobalByte17 Func_0200044c
#define State_SetRuntimeWord448To516 Func_020004e8
#define Story_Apply Func_02000c24
#define Story_Apply2 Func_02000c30
#define Story_Apply3 Func_02000c50
#define Story_Apply4 Func_02000c5c
#define Story_Apply5 Func_02000c88
#define Story_Apply6 Func_02000e86
#define Story_Apply7 Func_02000e8e
#define Story_Apply8 Func_02000e68
#define Story_Check10 Func_02000bb6
#define Story_Check11 Func_02000c8c
#define Story_Check12 Func_02000e54
#define Story_Check13 Func_02001020
#define Story_Check14 Func_020011c0
#define Story_Do10 Func_02000c1e
#define Story_Do11 Func_02000c4a
#define Story_Do12 Func_02000c98
#define Story_Do13 Func_02000d90
#define Story_Do14 Func_02000f54_a
#define Story_Do15 Func_02000f6c_a
#define Story_Do16 Func_02000f84_a
#define Story_Do17 Func_020011a4_a
#define Story_Do8 Func_02000b54
#define Story_Do9 Func_02000bf2
#define Story_Place3 Func_02000b7e
#define Story_Place4 Func_02000e9a
#define Story_Run Func_02000e9a_b
#define Story_Run10 Func_02000f02
#define Story_Run100 Func_020011ea
#define Story_Run101 Func_020011f0
#define Story_Run102 Func_02000be8
#define Story_Run11 Func_02000f0e
#define Story_Run12 Func_02000f1a
#define Story_Run13 Func_02000f26
#define Story_Run14 Func_0200102c
#define Story_Run15 Func_02001036
#define Story_Run16 Func_0200106c
#define Story_Run17 Func_02000c7c
#define Story_Run18 Func_02000d6a
#define Story_Run19 Func_0200101a
#define Story_Run2 Func_02001180
#define Story_Run20 Func_02000976
#define Story_Run21 Func_02000094
#define Story_Run22 Func_02000b5c
#define Story_Run23 Func_02000b78
#define Story_Run24 Func_02000b82
#define Story_Run25 Func_02000ba4
#define Story_Run26 Func_02000b90
#define Story_Run27 Func_02000bc8
#define Story_Run28 Func_02000bfc
#define Story_Run29 Func_02000be4
#define Story_Run3 Func_02000ff6
#define Story_Run30 Func_02000c40
#define Story_Run31 Func_02000bec
#define Story_Run32 Func_02000c14
#define Story_Run33 Func_02000c40_a
#define Story_Run34 Func_02000c6c
#define Story_Run35 Func_02000c18
#define Story_Run36 Func_02000c6c_a
#define Story_Run37 Func_02000c44
#define Story_Run38 Func_02000ce2
#define Story_Run39 Func_02000ce0
#define Story_Run4 Func_02000bc2
#define Story_Run40 Func_02000d2e
#define Story_Run41 Func_02000d1e
#define Story_Run42 Func_02000d28
#define Story_Run43 Func_02000d36
#define Story_Run44 Func_02000d34
#define Story_Run45 Func_02000d82
#define Story_Run46 Func_02000d72
#define Story_Run47 Func_02000d7c
#define Story_Run48 Func_02000976_a
#define Story_Run49 Func_02000936
#define Story_Run5 Func_02000ec6
#define Story_Run50 Func_020009d2
#define Story_Run51 Func_02000e94
#define Story_Run52 Func_02000ea0
#define Story_Run53 Func_02000eac
#define Story_Run54 Func_02000eb8
#define Story_Run55 Func_02000ec4
#define Story_Run56 Func_02000ed0
#define Story_Run57 Func_02000edc
#define Story_Run58 Func_02000ee8
#define Story_Run59 Func_02000ef4
#define Story_Run6 Func_02000ed2
#define Story_Run60 Func_02000f44
#define Story_Run61 Func_02000f4c
#define Story_Run62 Func_02000f54
#define Story_Run63 Func_02000f5c
#define Story_Run64 Func_02000f64
#define Story_Run65 Func_02000f6c
#define Story_Run66 Func_02000f74
#define Story_Run67 Func_02000f7c
#define Story_Run68 Func_02000f84
#define Story_Run69 Func_02000f8c
#define Story_Run7 Func_02000ede
#define Story_Run70 Func_02000f94
#define Story_Run71 Func_02000f9c
#define Story_Run72 Func_02000fa4
#define Story_Run73 Func_02000fac
#define Story_Run74 Func_02000fb4
#define Story_Run75 Func_02000f36
#define Story_Run76 Func_02000f3c
#define Story_Run77 Func_02000f42
#define Story_Run78 Func_02000f48
#define Story_Run79 Func_02000f4e
#define Story_Run8 Func_02000eea
#define Story_Run80 Func_02000f5a
#define Story_Run81 Func_02000f60
#define Story_Run82 Func_02000f66
#define Story_Run83 Func_02000f72
#define Story_Run84 Func_02000f78
#define Story_Run85 Func_02000f7e
#define Story_Run86 Func_02000f8a
#define Story_Run87 Func_0200101a_a
#define Story_Run88 Func_0200102a
#define Story_Run89 Func_02001070
#define Story_Run9 Func_02000ef6
#define Story_Run90 Func_02001076
#define Story_Run91 Func_0200103a
#define Story_Run92 Func_0200108a
#define Story_Run93 Func_02001092
#define Story_Run94 Func_020010a4
#define Story_Run95 Func_020010d4
#define Story_Run96 Func_020011a4
#define Story_Run97 Func_0200124a
#define Story_Run98 Func_020011ca
#define Story_Run99 Func_020011dc
#define Story_SetRect11 Func_02000a8e
#define Story_SetRect12 Func_02000aae
#define Story_SetRect13 Func_02000ac6
#define Story_SetRect14 Func_02000ae2
#define Story_SetRect15 Func_02000afa
#define Story_SetRect16 Func_02000b16
#define Story_SetRect17 Func_02000e04_a
#define TertiaryOverlayData Data_02008f80
#define gOv Data_02009064
#define gWork Data_03001ebc
#define Story_Do(a0) Call1(Func_02000b7a, a0)
#define Story_SetRect(a0, a1, a2, a3, a4, a5) Call6(Func_02000ba0, a0, a1, a2, a3, a4, a5)
#define Story_Check(a0) Value1(Func_02000c84, a0)
#define Story_Do2(a0) Call1(Func_02000c7c_a, a0)
#define Story_Do3(a0) Call1(Func_02000c8c_a, a0)
#define Story_Do4(a0) Call1(Func_02000c9c, a0)
#define Story_Do5(a0) Call1(Func_02000cae, a0)
#define Story_Check2(a0) Value1(Func_02000ca4, a0)
#define Story_Check3(a0) Value1(Func_02000cb0, a0)
#define Story_Check4(a0) Value1(Func_02000cba, a0)
#define Story_SetRect2(a0, a1, a2, a3, a4, a5) Call6(Func_02000cd2, a0, a1, a2, a3, a4, a5)
#define Story_SetRect3(a0, a1, a2, a3, a4, a5) Call6(Func_02000ce4, a0, a1, a2, a3, a4, a5)
#define Story_Do6(a0) Call1(Func_02000d0e, a0)
#define Story_Check5(a0) Value1(Func_02000d0e_a, a0)
#define Story_SetRect4(a0, a1, a2, a3, a4, a5) Call6(Func_02000d26, a0, a1, a2, a3, a4, a5)
#define Story_SetRect5(a0, a1, a2, a3, a4, a5) Call6(Func_02000d38, a0, a1, a2, a3, a4, a5)
#define Story_Do7(a0) Call1(Func_02000d6a_a, a0)
#define Story_Check6(a0) Value1(Func_02000fd4, a0)
#define Story_SetRect6(a0, a1, a2, a3, a4, a5) Call6(Func_02001014, a0, a1, a2, a3, a4, a5)
#define Story_Check7(a0) Value1(Func_02001058, a0)
#define Story_SetRect7(a0, a1, a2, a3, a4, a5) Call6(Func_02001060, a0, a1, a2, a3, a4, a5)
#define Story_SetRect8(a0, a1, a2, a3, a4, a5) Call6(Func_02001072, a0, a1, a2, a3, a4, a5)
#define Story_SetRect9(a0, a1, a2, a3, a4, a5) Call6(Func_02001090, a0, a1, a2, a3, a4, a5)
#define Story_SetRect10(a0, a1, a2, a3, a4, a5) Call6(Func_020010a2, a0, a1, a2, a3, a4, a5)
#define Story_Check8(a0) Value1(Func_020010c0, a0)
#define Story_Place(a0, a1, a2) Call3(Func_02001108, a0, a1, a2)
#define Story_Check9(a0) Value1(Func_020010d8, a0)
#define Story_Place2(a0, a1, a2) Call3(Func_02001120, a0, a1, a2)

#endif
