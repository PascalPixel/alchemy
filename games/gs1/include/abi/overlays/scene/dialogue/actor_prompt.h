#ifndef ALCHEMY_ABI_OVERLAYS_SCENE_DIALOGUE_ACTOR_PROMPT
#define ALCHEMY_ABI_OVERLAYS_SCENE_DIALOGUE_ACTOR_PROMPT

/* Binding layer — not production source. Address / far-call ABI only. */

void Func_0200099c(s32, s32);
s32 Func_020009a8();
s32 Func_020009da();
s32 Func_020009fc();
s32 Func_02000a16();
void Func_02000a40();
s32 Func_02000a4c();
void Func_02000a4e();
void Func_02000a54();
void Func_02000a58();
void Func_02000a80();
void Func_02000ab6();
s32 Func_02000ae6();
void Func_02000b00();
void Func_02000b08();
void Func_02000b10();
void Func_02000b18();
void Func_02000b1e();
void Func_02000b20();
void Func_02000b20_a();
void Func_02000b34();
void Func_02000b36();
s32 Func_02000b52();
s32 Func_02000bc0();
void Func_02000c16();
void Func_02000c46();
void Func_02000c78(s32);
void Func_02000b9e(s32);
void Func_02000ba4(s32);
void Func_02000c7a(s32);
void Func_02000bd6();
void Func_02000c04();
void Func_02000c0e();
void Func_02000c12();
void Func_02000c20();
void Func_02000c2e();
void Func_02000c42();
void Func_02000c5c();
void Func_02000c82();
void Func_02000cbe();
void Func_02000cca();
void Func_02000c62();
void Func_02000c66();
void Func_02000c90();
s32 Func_02000ca6();
void Func_02000cb4();
void Func_02000ccc();
void Func_02000cec();
void Func_02000cee();
void Func_02000d30();
void Func_02000d6a();
void Func_02000d6a_a();
void Func_02000d74();
void Func_02000d7e();
void Func_02000d82();
void Func_02000d88();
void Func_02000d8c();
void Func_02000d96();
void Func_02000d98();
void Func_02000d98_a();
void Func_02000da2();
void Func_02000daa();
void Func_02000dc4();
void Func_02000dc8();
void Func_02000dce();
void Func_02000ddc();
void Func_02000df0();
s32 Func_02000df6();
s32 Func_02000df6_a();
void Func_02000dfc();
void Func_02000dfe();
void Func_02000e14();
void Func_02000e14_a();
void Func_02000e16();
void Func_02000e28();
void Func_02000e2e();
void Func_02000e44();
void Func_02000e46();
void Func_02000e4c();
void Func_02000e58();
void Func_02000e62();
void Func_02000e76();
void Func_02000e92();
void Func_02000ea0();
void Func_02000eae();
void Func_02000ec4();
void Func_02000ece();
void Func_02000ece_a();
void Func_02000ed8();
void Func_02000edc();
void Func_02000ee8();
void Func_02000ef6();
void Func_02000f00();
void Func_02000f06();
void Func_02000f12();
void Func_02000f20();
void Func_02000f22();
void Func_02000f24();
void Func_02000f28();
void Func_02000f36();
void Func_02000f4a();
void Func_02000f4c();
void Func_02000f4e();
void Func_02000f5c();
void Func_02000f68();
s32 Func_02000f6e();
s32 Func_02000f6e_a();
s32 Func_02000f6e_b();
void Func_02000f76();
void Func_02000f80();
void Func_02000f8c();
void Func_02000f90();
void Func_02000f92();
void Func_02000f98();
void Func_02000fa0();
void Func_02000fa8();
void Func_02000fb0();
void Func_02000fb2();
void Func_02000fc8();
void Func_02000fca();
void Func_02000fe0();
void Func_02000fe6();
void Func_02000fd0();
void Func_02001060();
void Func_02001068();
s32 Func_02001078();
s32 Func_02001008();
void Func_02001094();
void Func_0200106c();
void Func_020010a4();
void Func_020010be();
void Func_0200103a();
void Func_0200104c(void);
void Func_020010d2(s32);
void Func_020010f4(s32, s32, s32);
void Func_020010de(s32, s32);
void Func_0200104a(u8 *, s32);
void Func_02001038(s32);
void Func_020010f8(s32, s32);
void Func_02001064(u8 *, s32);
void Func_02001054(s32);
void Func_0200111a(s32, s32);
void Func_02001086(u8 *, s32);
void Func_02001074(s32);
void Func_02001134(s32, s32);
void Func_020010a0(u8 *, s32);
void Func_02001090(s32);
void Func_0200112e(s32, s32, s32);
void Func_020010cc(s32);
void Func_020010f0(void);

static __inline__ void Call1(void (*f)(), s32 a0)
{
    extern u8 Data_02000240[];
    extern u8 Data_03001ebc[];

    f(a0);
}

static __inline__ s32 Value1(s32 (*f)(), s32 a0)
{
    extern u8 Data_02000240[];
    extern u8 Data_03001ebc[];

    return f(a0);
}

static __inline__ s32 Value3(s32 (*f)(), s32 a0, s32 a1, s32 a2)
{
    extern u8 Data_02000240[];
    extern u8 Data_03001ebc[];

    return f(a0, a1, a2);
}

static __inline__ void Call6(void (*f)(), s32 a0, s32 a1, s32 a2, s32 a3, s32 a4, s32 a5)
{
    extern u8 Data_02000240[];
    extern u8 Data_03001ebc[];

    f(a0, a1, a2, a3, a4, a5);
}

static __inline__ void bump_step(s32 amount)
{
    extern u8 Data_02000240[];
    extern u8 Data_03001ebc[];

    u8 *work = *(u8 **)Data_03001ebc;

    *(u16 *)(work + 0x1d8) = (u16)(*(u16 *)(work + 0x1d8) + amount);
}

static __inline__ void Call0(void (*f)())
{
    f();
}

static __inline__ void Call2(void (*f)(), s32 a0, s32 a1)
{
    f(a0, a1);
}

static __inline__ void Call3(void (*f)(), s32 a0, s32 a1, s32 a2)
{
    f(a0, a1, a2);
}

#define Dialogue_ShowMessages8fbAnd8fc Func_020002f8
#define SceneData_GetPrimaryTable Func_02000040
#define SceneData_GetSecondaryTable Func_0200008c
#define SceneData_GetTable8B48 Func_02000084
#define SceneData_GetTertiaryTable Func_020000e4
#define SceneData_ReturnZero Func_02000080
#define Scene_RunActorNinePromptDialogue Func_02000730
#define Scene_RunActorTenRepeatedMotion Func_020007ac
#define Scene_RunPrimarySequence Func_02000334
#define Scene_RunScene3c3SequenceA Func_020003c4
#define Scene_SetActor13Value1A Func_02000030
#define Talk_Apply Func_0200099c
#define Talk_Apply2 Func_020010de
#define Talk_Apply3 Func_0200104a
#define Talk_Apply4 Func_020010f8
#define Talk_Apply5 Func_02001064
#define Talk_Apply6 Func_0200111a
#define Talk_Apply7 Func_02001086
#define Talk_Apply8 Func_02001134
#define Talk_Apply9 Func_020010a0
#define Talk_Check10 Func_02000b52
#define Talk_Check11 Func_02000df6
#define Talk_Check12 Func_02000f6e
#define Talk_Check13 Func_02001078
#define Talk_Check14 Func_02001008
#define Talk_Check9 Func_02000ae6
#define Talk_Do10 Func_020010d2
#define Talk_Do11 Func_02001038
#define Talk_Do12 Func_02001054
#define Talk_Do13 Func_02001074
#define Talk_Do14 Func_02001090
#define Talk_Do15 Func_020010cc
#define Talk_Do6 Func_02000c78
#define Talk_Do7 Func_02000b9e
#define Talk_Do8 Func_02000ba4
#define Talk_Do9 Func_02000c7a
#define Talk_Place29 Func_020010f4
#define Talk_Place30 Func_0200112e
#define Talk_Run10 Func_02000a58
#define Talk_Run11 Func_02000b00
#define Talk_Run12 Func_02000b08
#define Talk_Run13 Func_02000b10
#define Talk_Run14 Func_02000b18
#define Talk_Run15 Func_02000b20
#define Talk_Run16 Func_02000288
#define Talk_Run17 Func_02000c16
#define Talk_Run18 Func_02000b1e
#define Talk_Run19 Func_02000b34
#define Talk_Run20 Func_02000c46
#define Talk_Run21 Func_02000fb6
#define Talk_Run22 Func_02000c66
#define Talk_Run23 Func_02000cec
#define Talk_Run24 Func_02000c62
#define Talk_Run25 Func_02000d8c
#define Talk_Run26 Func_02000da2
#define Talk_Run27 Func_02000d6a_a
#define Talk_Run28 Func_02000daa
#define Talk_Run29 Func_02000d96
#define Talk_Run30 Func_02000dc8
#define Talk_Run31 Func_02000ddc
#define Talk_Run32 Func_02000e16
#define Talk_Run33 Func_02000e28
#define Talk_Run34 Func_02000e14_a
#define Talk_Run35 Func_02000e46
#define Talk_Run36 Func_02000dc4
#define Talk_Run37 Func_02000e44
#define Talk_Run38 Func_02000e76
#define Talk_Run39 Func_02000e58
#define Talk_Run4 Func_02001084
#define Talk_Run40 Func_02000e92
#define Talk_Run41 Func_02000eae_a
#define Talk_Run42 Func_02000e2e
#define Talk_Run43 Func_02000ed8
#define Talk_Run44 Func_02000f00
#define Talk_Run45 Func_02000f22
#define Talk_Run46 Func_02000f12
#define Talk_Run47 Func_02000f4c
#define Talk_Run48 Func_02000f36
#define Talk_Run49 Func_02000ec4
#define Talk_Run5 Func_0200109e
#define Talk_Run50 Func_02000f4e
#define Talk_Run51 Func_02000edc
#define Talk_Run52 Func_02000f68
#define Talk_Run53 Func_02000ef6
#define Talk_Run54 Func_02000fa0
#define Talk_Run55 Func_02000f80
#define Talk_Run56 Func_02000fb2
#define Talk_Run57 Func_02000f92
#define Talk_Run58 Func_02000f20
#define Talk_Run59 Func_02000fca
#define Talk_Run6 Func_020010c0
#define Talk_Run60 Func_02000fe0
#define Talk_Run61 Func_02000fa8
#define Talk_Run62 Func_02000f90
#define Talk_Run63 Func_02000fc8
#define Talk_Run64 Func_02000fb0
#define Talk_Run65 Func_02000fb6_a
#define Talk_Run66 Func_02000fd0
#define Talk_Run67 Func_02001060
#define Talk_Run68 Func_02001068
#define Talk_Run69 Func_02001094
#define Talk_Run7 Func_020010da
#define Talk_Run70 Func_0200106c
#define Talk_Run71 Func_020010a4
#define Talk_Run72 Func_020010be
#define Talk_Run73 Func_0200103a
#define Talk_Run74 Func_0200104c
#define Talk_Run75 Func_020010f0
#define Talk_Run8 Func_0200092a
#define Talk_Run9 Func_02000124
#define gCell Data_02000240
#define gOv Data_02008a40
#define gOv10 Data_02008d10
#define gOv2 Data_02008ad0
#define gOv3 Data_02008998
#define gOv4 Data_02008ba8
#define gOv5 Data_02008c98
#define gOv6 Data_02008c50
#define gOv7 Data_02008b90
#define gOv8 Data_02008ddc
#define gOv9 Data_02008e54
#define gVal Data_00000069
#define gVal2 Data_000000a9
#define gVal3 Data_000000aa
#define gWork Data_03001ebc
#define Battle_Reset_1() Call0(Func_02000bd6)
#define Motion_SetSpeed_1(a0, a1, a2) Call3(Func_02000c04, a0, a1, a2)
#define Motion_SetSpeed_2(a0, a1, a2) Call3(Func_02000c12, a0, a1, a2)
#define Motion_ResetPosMode2_1(a0, a1, a2) Call3(Func_02000c2e, a0, a1, a2)
#define Motion_SetPosReset_1(a0, a1, a2) Call3(Func_02000c42, a0, a1, a2)
#define Motion_ArmCb_1(a0, a1, a2) Call3(Func_02000cbe, a0, a1, a2)
#define Motion_ArmCb_2(a0, a1, a2) Call3(Func_02000cca, a0, a1, a2)
#define Object_SetModeById_1(a0, a1) Call2(Func_02000c82, a0, a1)
#define Battle_SchedShoulder_1() Call0(Func_02000c5c)
#define Talk_Check(a0) Value1(Func_020009a8, a0)
#define Talk_Check2(a0) Value1(Func_020009da, a0)
#define Talk_Check3(a0) Value1(Func_020009fc, a0)
#define Talk_Check4(a0) Value1(Func_02000a16, a0)
#define Talk_Do(a0) Call1(Func_02000a40, a0)
#define Talk_Do2(a0) Call1(Func_02000a4e, a0)
#define Talk_Check5(a0) Value1(Func_02000a4c, a0)
#define Talk_SetRect(a0, a1, a2, a3, a4, a5) Call6(Func_02000a54, a0, a1, a2, a3, a4, a5)
#define Talk_Do3(a0) Call1(Func_02000a80, a0)
#define Talk_SetRect2(a0, a1, a2, a3, a4, a5) Call6(Func_02000ab6, a0, a1, a2, a3, a4, a5)
#define Talk_SetRect3(a0, a1, a2, a3, a4, a5) Call6(Func_02000b20_a, a0, a1, a2, a3, a4, a5)
#define Talk_SetRect4(a0, a1, a2, a3, a4, a5) Call6(Func_02000b36, a0, a1, a2, a3, a4, a5)
#define Talk_Place(a0, a1, a2) Value3(Func_02000bc0, a0, a1, a2)
#define Talk_SetRect5(a0, a1, a2, a3, a4, a5) Call6(Func_02000c0e, a0, a1, a2, a3, a4, a5)
#define Talk_SetRect6(a0, a1, a2, a3, a4, a5) Call6(Func_02000c20, a0, a1, a2, a3, a4, a5)
#define Talk_Place2(a0, a1, a2) Call3(Func_02000c90, a0, a1, a2)
#define Talk_Place3(a0, a1, a2) Call3(Func_02000cb4, a0, a1, a2)
#define Talk_Place4(a0, a1, a2) Call3(Func_02000d30, a0, a1, a2)
#define Talk_Check6(a0) Value1(Func_02000ca6, a0)
#define Talk_Place5(a0, a1, a2) Call3(Func_02000ccc, a0, a1, a2)
#define Talk_Place6(a0, a1, a2) Call3(Func_02000cee, a0, a1, a2)
#define Talk_Place7(a0, a1, a2) Call3(Func_02000d6a, a0, a1, a2)
#define Talk_Place8(a0, a1, a2) Call3(Func_02000d7e, a0, a1, a2)
#define Talk_Place9(a0, a1, a2) Call3(Func_02000d82, a0, a1, a2)
#define Talk_Place10(a0, a1, a2) Call3(Func_02000d98, a0, a1, a2)
#define Talk_Do4(a0) Call1(Func_02000d88, a0)
#define Talk_Place11(a0, a1, a2) Call3(Func_02000dce, a0, a1, a2)
#define Talk_Place12(a0, a1, a2) Call3(Func_02000d74, a0, a1, a2)
#define Talk_Place13(a0, a1, a2) Call3(Func_02000df0, a0, a1, a2)
#define Talk_Place14(a0, a1, a2) Call3(Func_02000dfc, a0, a1, a2)
#define Talk_Place15(a0, a1, a2) Call3(Func_02000d98_a, a0, a1, a2)
#define Talk_Place16(a0, a1, a2) Call3(Func_02000e14, a0, a1, a2)
#define Talk_Place17(a0, a1, a2) Call3(Func_02000e4c, a0, a1, a2)
#define Talk_Place18(a0, a1, a2) Call3(Func_02000e62, a0, a1, a2)
#define Talk_Place19(a0, a1, a2) Call3(Func_02000dfe, a0, a1, a2)
#define Talk_Run(a0, a1) Call2(Func_02000ea0, a0, a1)
#define Talk_Place20(a0, a1, a2) Call3(Func_02000eae, a0, a1, a2)
#define Talk_Check7(a0) Value1(Func_02000df6_a, a0)
#define Talk_Place21(a0, a1, a2) Call3(Func_02000ece, a0, a1, a2)
#define Talk_Run2(a0, a1) Call2(Func_02000ee8, a0, a1)
#define Talk_Place22(a0, a1, a2) Call3(Func_02000f06, a0, a1, a2)
#define Talk_Place23(a0, a1, a2) Call3(Func_02000f24, a0, a1, a2)
#define Talk_Place24(a0, a1, a2) Call3(Func_02000f28, a0, a1, a2)
#define Talk_Place25(a0, a1, a2) Call3(Func_02000ece_a, a0, a1, a2)
#define Talk_Place26(a0, a1, a2) Call3(Func_02000f4a, a0, a1, a2)
#define Talk_Run3(a0, a1) Call2(Func_02000f76, a0, a1)
#define Talk_Place27(a0, a1, a2) Call3(Func_02000f98, a0, a1, a2)
#define Talk_Place28(a0, a1, a2) Call3(Func_02000fe6, a0, a1, a2)
#define Talk_Check8(a0) Value1(Func_02000f6e_a, a0)
#define Talk_SetRect7(a0, a1, a2, a3, a4, a5) Call6(Func_02000f5c, a0, a1, a2, a3, a4, a5)
#define Talk_SetRect8(a0, a1, a2, a3, a4, a5) Call6(Func_02000f6e_b, a0, a1, a2, a3, a4, a5)
#define Talk_Do5(a0) Call1(Func_02000f8c, a0)

#endif
