#ifndef ALCHEMY_ABI_OVERLAYS_SCENE_STATE_INTERACTION_INTERACTION
#define ALCHEMY_ABI_OVERLAYS_SCENE_STATE_INTERACTION_INTERACTION

/* Binding layer — not production source. Address / far-call ABI only. */

void Func_02001bfc(s32, s32, s32 *);
void Func_02001c38(s32 *, s32, s32, s32);
void Func_020004e6();
s32 Func_020006ae();
void Func_02000726();
s32 Func_0200146e();
void Func_0200148c();
void Func_020014da();
s32 Func_02000722();
void Func_0200079a();
s32 Func_020014e6();
void Func_02001500();
void Func_0200154e();
void Func_02000600();
void Func_020008c6();
s32 Func_020008f6();
s32 Func_02001596();
s32 Func_020015a8();
void Func_020015c4();
void Func_02001602();
void Func_02000906();
s32 Func_02000936();
s32 Func_020015d6();
s32 Func_020015e8();
void Func_02001604();
void Func_02001642();
s32 Func_02001836();
void Func_02001882();
void Func_0200188c();
void Func_02001890();
s32 Func_02001898();
void Func_020018ba();
void Func_020018fe();
void Func_0200191a();
void Func_02001920();
void Func_02001926();
void Func_02001948();
void Func_0200195e();
void Func_0200199a();
void Func_020019b6();
double Func_02001bd2();
void Func_02001bde();
double Func_02001c1c();
double Func_02001c2a();
s32 Func_02001cd6();
void Func_02001084();
void Func_020019a4();
s32 Func_020019b0();
void Func_020019b4();
void Func_020019ba();
void Func_020019ca();
void Func_020019d8();
void Func_020019ee();
void Func_020019fa();
void Func_02001a10();
void Func_02001a18();
void Func_02001a2e();
void Func_02001a44();
void Func_02001a4e();
s32 Func_0200138a();
s32 Func_020013ba();
void Func_020015b0();
s32 Func_02001af6();
s32 Func_02001b36();
s32 Func_02001b40();
s32 Func_02001b48();
s32 Func_02001b58();
void Func_02001b6a();
void Func_02001b8a_a();
void Func_02001b92();
void Func_02001bb8();
void Func_02001bc0();
void Func_02001bd0();
s32 Func_02001bd6();
s32 Func_02001bf6();
void Func_02001c66();
void Func_02001c82();
void Func_02000aa0(void)
{
    void Func_02001b8a();
void Func_02001476(s32, s32, s32, s32, s32, s32);
void Func_0200148a(s32, s32, s32, s32, s32, s32);
void Func_020014a4(s32, s32, s32, s32, s32, s32);
void Func_0200168e(s32, s32, s32, s32, s32, s32);
void Func_020016e4(s32);
void Func_0200172e(s32, s32, s32, s32, s32, s32);
void Func_0200174a(u8 *, s32);
void Func_0200178e(s32);
void Func_02001cf8();
void Func_02001cb6();
void Func_02001d40();
void Func_02001dc8();
void Func_02001d08();
void Func_020013cc(s32);
void Func_020013cc_a(s32, s32);
void Func_020015fa(s32, s32, s32, s32, s32, s32);
void Func_02001652(s32);
void Func_020016da(s32, s32, s32, s32, s32, s32);
void Func_020016f6(u8 *, s32);
void Func_02001738(s32);
void Func_0200177e(s32, s32, s32, s32, s32, s32);
void Func_0200179a(u8 *, s32);
void Func_020017dc(s32);

static __inline__ s32 Value0(s32 (*f)())
{
    return f();
}

static __inline__ void Call1(void (*f)(), s32 a0)
{
    f(a0);
}

static __inline__ s32 Value1(s32 (*f)(), s32 a0)
{
    return f(a0);
}

static __inline__ void bump_step(s32 amount)
{
    u8 *work = *(u8 **)Data_03001ebc;

    *(u16 *)(work + 0x1d8) = (u16)(*(u16 *)(work + 0x1d8) + amount);
}

static __inline__ void Call1_02000754(void (*f)(), s32 a0)
{
    double Func_02001b8a();

    f(a0);
}

static __inline__ void Call2(void (*f)(), s32 a0, s32 a1)
{
    double Func_02001b8a();

    f(a0, a1);
}

static __inline__ s32 Value2(s32 (*f)(), s32 a0, s32 a1)
{
    double Func_02001b8a();

    return f(a0, a1);
}

static __inline__ void Call3(void (*f)(), s32 a0, s32 a1, s32 a2)
{
    double Func_02001b8a();

    f(a0, a1, a2);
}

static __inline__ s32 Value4(s32 (*f)(), s32 a0, s32 a1, s32 a2, s32 a3)
{
    double Func_02001b8a();

    return f(a0, a1, a2, a3);
}

static __inline__ void Call6(void (*f)(), s32 a0, s32 a1, s32 a2, s32 a3, s32 a4, s32 a5)
{
    double Func_02001b8a();

    f(a0, a1, a2, a3, a4, a5);
}

static __inline__ void Call1_020008a8(void (*f)(), s32 a0)
{
    f(a0);
}

static __inline__ void Call1_020009c0(void (*f)(), s32 a0)
{
    f(a0);
}

static __inline__ void Call1_02000aa0(void (*f)(), s32 a0)
{
    void Func_02001b8a();

    f(a0);
}

#define Actor_InitSlots10To15AndStartTask Func_02000c08
#define Actor_IsActor10AtTile16x12 Func_02000458
#define Actor_MarkSlot21AndSetFlag205 Func_02000690
#define Actor_SetupActors11To14AndInstallTask Func_02000c50
#define Actor_SetupSlotTwenty Func_02000640
#define Actor_TransformAndApplyRecordPosition Func_02000b54
#define GetActorPosition Func_0200146a
#define IsActor9AtTile15x54 Func_02000334
#define Resource3a7_NoOpCallback Func_020004cc
#define SceneData_FindActiveSlotAtCell Func_02000134
#define Scene_RunFlag9a9GuardedScene Func_02000368
#define Scene_RunGuardedStep9AA Func_020004d0
#define Scene_RunGuardedStep9AAAfterSetup Func_0200048c
#define Scene_RunOpeningAuxiliarySequence Func_020009c0
#define Scene_RunScene3a7SequenceA Func_020003e0
#define Scene_RunScene3a7SequenceB Func_020008a8
#define Scene_RunSupplementalSequenceOne Func_02000754
#define State_Apply Func_0200174a
#define State_Apply2 Func_020013cc_a
#define State_Apply3 Func_020016f6
#define State_Apply4 Func_0200179a
#define State_ApplyFlag300 Func_020002bc
#define State_ApplyRectAndMarkActor16 Func_0200050c
#define State_ApplyRectAndSetupActor19 Func_020005ec
#define State_ApplyThreeRects Func_02000398
#define State_Check16 Func_02001836
#define State_Check17 Func_02001cd6
#define State_Check18 Func_02001af6
#define State_Check19 Func_02001b48
#define State_ConfigureRegion26_30AndClearActor18Mode Func_020005a0
#define State_Do10 Func_020013cc
#define State_Do11 Func_02001652
#define State_Do12 Func_02001738
#define State_Do13 Func_020017dc
#define State_Do8 Func_020016e4
#define State_Do9 Func_0200178e
#define State_IsStateAtMostOneB Func_02001544
#define State_IsStateAtMostOneBB Func_02001740
#define State_Place3 Func_02001bfc
#define State_Run10 Func_02000726
#define State_Run11 Func_02001b22
#define State_Run12 Func_02001c06
#define State_Run13 Func_02001c1a
#define State_Run14 Func_0200154e
#define State_Run15 Func_0200079a
#define State_Run16 Func_02000600
#define State_Run17 Func_02001602
#define State_Run18 Func_020008c6
#define State_Run19 Func_02001642
#define State_Run20 Func_02000906
#define State_Run21 Func_02001b8a
#define State_Run22 Func_0200188c
#define State_Run23 Func_020018fe
#define State_Run24 Func_02001c1c
#define State_Run25 Func_02001c2a
#define State_Run26 Func_02001bde
#define State_Run27 Func_02001bd2
#define State_Run28 Func_02001920
#define State_Run29 Func_02001948
#define State_Run30 Func_0200191a
#define State_Run31 Func_0200195e
#define State_Run32 Func_0200199a
#define State_Run33 Func_02001a18
#define State_Run34 Func_020019fa
#define State_Run35 Func_02001a10
#define State_Run36 Func_02001a4e
#define State_Run37 Func_02001a2e
#define State_Run38 Func_02001a44
#define State_Run39 Func_02001084
#define State_Run40 Func_020015b0
#define State_Run41 Func_02001b8a_a
#define State_Run42 Func_02001b92
#define State_Run43 Func_02001bb8
#define State_Run44 Func_02001bc0
#define State_Run45 Func_02001bd0
#define State_Run46 Func_02001c66
#define State_Run47 Func_02001c82
#define State_Run48 Func_020016d8
#define State_Run49 Func_020016f8
#define State_Run50 Func_02001778
#define State_Run51 Func_020017a0
#define State_Run52 Func_02001d42
#define State_Run53 Func_02001d8a
#define State_Run54 Func_02001cf8
#define State_Run55 Func_02001d4e
#define State_Run56 Func_02001cb6
#define State_Run57 Func_02001d40
#define State_Run58 Func_02001d96
#define State_Run59 Func_02001dc8
#define State_Run60 Func_02001d08
#define State_Run61 Func_02001644
#define State_Run62 Func_02001664
#define State_Run63 Func_02001724
#define State_Run64 Func_0200174c
#define State_Run65 Func_020017c8
#define State_Run66 Func_020017f0
#define State_Run7 Func_0200158e
#define State_Run8 Func_020004e6
#define State_Run9 Func_020014da
#define State_SetFlag953 Func_020002cc
#define State_SetMode Func_02001c38
#define State_SetRect10 Func_0200172e
#define State_SetRect11 Func_020015fa
#define State_SetRect12 Func_020016da
#define State_SetRect13 Func_0200177e
#define State_SetRect6 Func_02001476
#define State_SetRect7 Func_0200148a
#define State_SetRect8 Func_020014a4
#define State_SetRect9 Func_0200168e
#define gOv Data_0200a224
#define gOv2 Data_0200a228
#define gOv3 Data_0200a22c
#define gOv4 Data_0200a230
#define gWork Data_03001ebc
#define State_Check(a0) Value1(Func_0200146e, a0)
#define State_Run() Value0(Func_020006ae)
#define State_Do(a0) Call1(Func_0200148c, a0)
#define State_Check2(a0) Value1(Func_020014e6, a0)
#define State_Run2() Value0(Func_02000722)
#define State_Do2(a0) Call1(Func_02001500, a0)
#define State_Check3(a0) Value1(Func_02001596, a0)
#define State_Run3() Value0(Func_020008f6)
#define State_Check4(a0) Value1(Func_020015a8, a0)
#define State_Do3(a0) Call1(Func_020015c4, a0)
#define State_Check5(a0) Value1(Func_020015d6, a0)
#define State_Run4() Value0(Func_02000936)
#define State_Check6(a0) Value1(Func_020015e8, a0)
#define State_Do4(a0) Call1(Func_02001604, a0)
#define State_Place(a0, a1, a2) Call3(Func_02001882, a0, a1, a2)
#define State_Place2(a0, a1, a2) Call3(Func_02001890, a0, a1, a2)
#define State_Run5(a0, a1) Call2(Func_020018ba, a0, a1)
#define State_Check7(a0, a1) Value2(Func_02001898, a0, a1)
#define State_SetRect(a0, a1, a2, a3, a4, a5) Call6(Func_02001926, a0, a1, a2, a3, a4, a5)
#define State_Do5(a0) Call1_02000754(Func_020019b6, a0)
#define State_Check8(a0) Value1(Func_020019b0, a0)
#define State_Run6(a0, a1) Call2(Func_020019b4, a0, a1)
#define State_Do6(a0) Call1_020008a8(Func_020019ca, a0)
#define State_SetRect2(a0, a1, a2, a3, a4, a5) Call6(Func_020019a4, a0, a1, a2, a3, a4, a5)
#define State_SetRect3(a0, a1, a2, a3, a4, a5) Call6(Func_020019ba, a0, a1, a2, a3, a4, a5)
#define State_SetRect4(a0, a1, a2, a3, a4, a5) Call6(Func_020019d8, a0, a1, a2, a3, a4, a5)
#define State_SetRect5(a0, a1, a2, a3, a4, a5) Call6(Func_020019ee, a0, a1, a2, a3, a4, a5)
#define State_Check9(a0) Value1(Func_02001b40, a0)
#define State_Check10(a0) Value1(Func_0200138a, a0)
#define State_Check11(a0) Value1(Func_02001b58, a0)
#define State_Do7(a0) Call1_020009c0(Func_02001b6a, a0)
#define State_Check12(a0) Value1(Func_020013ba, a0)
#define State_Check13(a0) Value1(Func_02001bd6, a0)
#define State_Check14(a0, a1) Value2(Func_02001b36, a0, a1)
#define State_Check15(a0) Value1(Func_02001bf6, a0)

#endif
