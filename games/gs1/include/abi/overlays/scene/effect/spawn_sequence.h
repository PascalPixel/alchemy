#ifndef ALCHEMY_ABI_OVERLAYS_SCENE_EFFECT_SPAWN_SEQUENCE
#define ALCHEMY_ABI_OVERLAYS_SCENE_EFFECT_SPAWN_SEQUENCE

/* Binding layer — not production source. Address / far-call ABI only. */

void Func_02001410(void *, s32);
void Func_02001428(void *, s32);
void Func_020014e0(void *, s32);
void Func_0200146c(void *, s32);
void Func_0200153c(void *, s32);
void Func_020014fe();
void Func_02001518();
void Func_02001672();
s32 Func_020015ac();
s32 Func_020015c4();
s32 Func_020015d2();
void Func_02001618();
void Func_02001628();
void Func_020017be();
void Func_020004f0();
s32 Func_02001956(s32 flagId);
void Func_02001998(void);
void Func_02001a3e(s32 messageId);
s32 Func_02001a5e(s32 actorId, s32 mode);
void Func_020019b2(void);
void Func_02001dea(s32 actorId, s32 mode);
void Func_02002156();
s32 Func_02002176();
void Func_02002214();
void Func_02002348();
void Func_02002212();
void Func_0200235a();
void Func_02002366();
void Func_02002372();
void Func_0200223c();
void Func_02002384();
void Func_0200224e();
void Func_02002396();
void Func_02002260();
void Func_020023a8();
void Func_02002272();
void Func_02001cf0();
void Func_020022d4();
s32 Func_020019b4();
void Func_020019c2();
s32 Func_020019cc();
s32 Func_020019ec();
s32 Func_020019ee();
void Func_020019f6();
s32 Func_02001a3e_a();
s32 Func_02001a48();
void Func_02001a66_a();
s32 Func_02001a76();
void Func_02001a80();
s32 Func_02001a84();
s32 Func_02001a92();
void Func_02001aa0();
void Func_02001aa4();
void Func_02001ab0();
s32 Func_02001abe();
void Func_02001ad0();
s32 Func_02001ad4();
s32 Func_02001ae4();
void Func_02001aec();
s32 Func_02001afe();
s32 Func_02001b0e();
void Func_02001b10();
void Func_02001b24();
s32 Func_02001b36();
void Func_02001b3a();
void Func_02001b46();
void Func_02001b4e();
void Func_02001b58();
void Func_02001b66();
s32 Func_02001b70();
void Func_02001b74();
s32 Func_02001b78();
s32 Func_02001b98();
s32 Func_02001ba0();
void Func_02001bb0();
void Func_02001bc8();
void Func_02001be6();
void Func_02001bea();
void Func_02001bfe();
s32 Func_02001c30();
s32 Func_02001c3e();
void Func_02001c4c();
void Func_02001c5e();
s32 Func_02001c68();
void Func_02001c70();
void Func_02001c76();
void Func_02001c78();
s32 Func_02001ca2();
void Func_02001ca4();
void Func_02001ca8();
void Func_02001cac();
void Func_02001cae();
void Func_02001cc6();
void Func_02001cc8();
void Func_02001ccc();
void Func_02001cda();
void Func_02001cda_a();
void Func_02001cdc();
void Func_02001cf2();
void Func_02001cf4();
void Func_02001d00();
void Func_02001d0a();
void Func_02001d12();
void Func_02001cf8();
void Func_02001d2e();
void Func_02001d2e_a();
s32 Func_02001d44();
s32 Func_02001d54();
s32 Func_02001d60();
void Func_02001d66();
s32 Func_02001d68();
void Func_02001d92();
void Func_02001e4e();
s32 Func_020021c0();
void Func_020021dc();
void Func_02002234();
void Func_0200223a();
void Func_02002240();
void Func_020022d6();
void Func_02002424();
s32 Func_02002442();
s32 Func_0200244c();
s32 Func_0200244c_a();
s32 Func_02002458();
s32 Func_02002460();
s32 Func_02002468();
s32 Func_02002470();
s32 Func_02002478();
void Func_0200247a();
void Func_020024a8();
void Func_020024b6();
void Func_020024ea();
s32 Func_020024f2();
void Func_020024f2_a();
void Func_020024f4();
void Func_020024fc();
void Func_020024fe();
void Func_0200251c();
void Func_02002544();
void Func_02002588();
void Func_020025a4();

static __inline__ void Call0(void (*f)())
{
    extern u8 Data_03001ebc[];

    f();
}

static __inline__ void Call1(void (*f)(), s32 a0)
{
    extern u8 Data_03001ebc[];

    f(a0);
}

static __inline__ s32 Value1(s32 (*f)(), s32 a0)
{
    extern u8 Data_03001ebc[];

    return f(a0);
}

static __inline__ void Call3(void (*f)(), s32 a0, s32 a1, s32 a2)
{
    extern u8 Data_03001ebc[];

    f(a0, a1, a2);
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

static __inline__ void Call1_02000968(void (*f)(), s32 a0)
{
    extern u8 Data_03001ebc[];

    f(a0);
}

static __inline__ void Call2(void (*f)(), s32 a0, s32 a1)
{
    extern u8 Data_03001ebc[];

    f(a0, a1);
}

static __inline__ void Call3_02000de8(void (*f)(), s32 a0, s32 a1, s32 a2)
{
    extern u8 Data_03001ebc[];

    f(a0, a1, a2);
}

#define Effect_ActivateNearbyActor Func_0200094c
#define Effect_AdvanceRotatingSprite Func_02000358
#define Effect_Apply Func_02001410
#define Effect_Apply2 Func_020014e0
#define Effect_Apply3 Func_02001428
#define Effect_Apply4 Func_0200146c
#define Effect_Apply5 Func_0200153c
#define Effect_Apply6 Func_02001a5e
#define Effect_Apply7 Func_02001dea
#define Effect_CalculatePositionDistance Func_020003cc
#define Effect_Check26 Func_020015ac
#define Effect_Check27 Func_020015c4
#define Effect_Check28 Func_020015d2
#define Effect_Check29 Func_02001956
#define Effect_Check30 Func_02001a3e_a
#define Effect_Check31 Func_02001ad4
#define Effect_Check32 Func_02001afe
#define Effect_Check33 Func_02001c68
#define Effect_Check34 Func_02001d44
#define Effect_Check35 Func_02001d54
#define Effect_Check36 Func_02002176
#define Effect_Check37 Func_020021c0
#define Effect_Check38 Func_02002442
#define Effect_Check39 Func_0200244c_a
#define Effect_Check40 Func_02002478
#define Effect_Check41 Func_020024f2
#define Effect_DispatchStep Func_02000e4c
#define Effect_Do5 Func_02001a3e
#define Effect_GetInitialValue Func_020005a4
#define Effect_GetPrimaryData Func_0200059c
#define Effect_GetSecondaryData Func_020005a8
#define Effect_GetTertiaryData Func_02001140
#define Effect_PrepareState Func_020005b0
#define Effect_RequestFixedEffect Func_02000314
#define Effect_Run10 Func_0200172e
#define Effect_Run11 Func_02001748
#define Effect_Run12 Func_02001d22
#define Effect_Run13 Func_020019d8
#define Effect_Run14 Func_020019fc
#define Effect_Run15 Func_02001a66
#define Effect_Run16 Func_02001ac6
#define Effect_Run17 Func_02001b2a
#define Effect_Run18 Func_02001b2a_a
#define Effect_Run19 Func_02001b3e
#define Effect_Run20 Func_02001cb0
#define Effect_Run21 Func_02001cd6
#define Effect_Run22 Func_020020b0
#define Effect_Run23 Func_020020ca
#define Effect_Run24 Func_020014fe
#define Effect_Run25 Func_02001518
#define Effect_Run26 Func_02001672
#define Effect_Run27 Func_02001618
#define Effect_Run28 Func_02001628
#define Effect_Run29 Func_020017be
#define Effect_Run30 Func_020004f0
#define Effect_Run31 Func_02001998
#define Effect_Run32 Func_020019b2
#define Effect_Run33 Func_020019c2
#define Effect_Run34 Func_02001a80
#define Effect_Run35 Func_020019f6
#define Effect_Run36 Func_02001ad0
#define Effect_Run37 Func_02001aec
#define Effect_Run38 Func_02001aa0
#define Effect_Run39 Func_02001b24
#define Effect_Run40 Func_02001b4e
#define Effect_Run41 Func_02001b74
#define Effect_Run42 Func_02001b58
#define Effect_Run43 Func_02001bb0
#define Effect_Run44 Func_02001bc8
#define Effect_Run45 Func_02001b46
#define Effect_Run46 Func_02001c4c
#define Effect_Run47 Func_02001bea
#define Effect_Run48 Func_02001c76
#define Effect_Run49 Func_02001b3a
#define Effect_Run50 Func_02001c70
#define Effect_Run51 Func_02001be6
#define Effect_Run52 Func_02001c78
#define Effect_Run53 Func_02001bfe
#define Effect_Run54 Func_02001cc6
#define Effect_Run55 Func_02001d0a
#define Effect_Run56 Func_02001cda
#define Effect_Run57 Func_02001ccc
#define Effect_Run58 Func_02001cda_a
#define Effect_Run59 Func_02001cf2
#define Effect_Run6 Func_020013ca
#define Effect_Run60 Func_02001cf4
#define Effect_Run61 Func_02001cdc
#define Effect_Run62 Func_02001d2e
#define Effect_Run63 Func_02001e4e
#define Effect_Run64 Func_02001d92
#define Effect_Run65 Func_02002156
#define Effect_Run66 Func_02002240
#define Effect_Run67 Func_020022d6
#define Effect_Run68 Func_02002214
#define Effect_Run69 Func_02002348
#define Effect_Run7 Func_02001422
#define Effect_Run70 Func_02002212
#define Effect_Run71 Func_0200235a
#define Effect_Run72 Func_02002366
#define Effect_Run73 Func_02002372
#define Effect_Run74 Func_0200223c
#define Effect_Run75 Func_02002384
#define Effect_Run76 Func_0200224e
#define Effect_Run77 Func_02002396
#define Effect_Run78 Func_02002260
#define Effect_Run79 Func_020023a8
#define Effect_Run8 Func_0200152e
#define Effect_Run80 Func_02002272
#define Effect_Run81 Func_02001cf0
#define Effect_Run82 Func_020022d4
#define Effect_Run83 Func_02002424
#define Effect_Run84 Func_020024f4
#define Effect_Run85 Func_0200247a
#define Effect_Run86 Func_0200251c
#define Effect_Run9 Func_020014f4
#define Effect_RunActorSceneMessage Func_02000cec
#define Effect_ShowActorSetupMessage Func_020005d4
#define Effect_SpawnConfigured Func_0200013c
#define Effect_SpawnPeriodicEffect Func_02000370
#define Effect_SpawnPrimary Func_02000048
#define Effect_SpawnSecondary Func_020000a0
#define Scene_RunPrimarySequence Func_020005f4
#define Scene_RunScene3a0 Func_02001060
#define SetEffectRecordMode Func_02000030
#define gIw Data_03001e40
#define gOv Data_020095a4
#define gOv2 Data_02009740
#define gOv3 Data_02009900
#define gOv4 Data_02009778
#define gOv5 Data_0200978e
#define gOv6 Data_020097a4
#define gOv7 Data_020097ba
#define gOv8 Data_020097d0
#define gWork Data_03001ebc
#define Effect_Check(a0) Value1(Func_020019cc, a0)
#define Effect_Check2(a0) Value1(Func_020019b4, a0)
#define Effect_Check3(a0) Value1(Func_020019ee, a0)
#define Effect_Do(a0) Call1(Func_02001aa4, a0)
#define Effect_Check4(a0) Value1(Func_020019ec, a0)
#define Effect_Check5(a0) Value1(Func_02001a48, a0)
#define Effect_Place(a0, a1, a2) Call3(Func_02001b10, a0, a1, a2)
#define Effect_Run() Call0(Func_02001a66_a)
#define Effect_Check6(a0) Value1(Func_02001a76, a0)
#define Effect_Check7(a0) Value1(Func_02001a84, a0)
#define Effect_Check8(a0) Value1(Func_02001a92, a0)
#define Effect_Place2(a0, a1, a2) Call3(Func_02001ab0, a0, a1, a2)
#define Effect_Place3(a0, a1, a2) Call3((void (*)())Func_02001b2a, a0, a1, a2)
#define Effect_Check9(a0) Value1(Func_02001abe, a0)
#define Effect_Check10(a0) Value1(Func_02001ae4, a0)
#define Effect_Check11(a0) Value1(Func_02001b0e, a0)
#define Effect_Check12(a0) Value1(Func_02001b36, a0)
#define Effect_Check13(a0) Value1(Func_02001b70, a0)
#define Effect_Check14(a0) Value1(Func_02001b78, a0)
#define Effect_Check15(a0) Value1(Func_02001b98, a0)
#define Effect_Check16(a0) Value1(Func_02001ba0, a0)
#define Effect_SetRect(a0, a1, a2, a3, a4, a5) Call6(Func_02001b66, a0, a1, a2, a3, a4, a5)
#define Effect_Place4(a0, a1, a2) Call3(Func_02001cae, a0, a1, a2)
#define Effect_Place5(a0, a1, a2) Call3(Func_02001cc8, a0, a1, a2)
#define Effect_Do2(a0) Call1(Func_02001cac, a0)
#define Effect_Check17(a0) Value1(Func_02001c30, a0)
#define Effect_Check18(a0) Value1(Func_02001c3e, a0)
#define Effect_Place6(a0, a1, a2) Call3(Func_02001c5e, a0, a1, a2)
#define Effect_Place7(a0, a1, a2) Call3(Func_02001d12, a0, a1, a2)
#define Effect_Place8(a0, a1, a2) Call3(Func_02001ca4, a0, a1, a2)
#define Effect_Check19(a0) Value1(Func_02001ca2, a0)
#define Effect_Place9(a0, a1, a2) Call3(Func_02001d00, a0, a1, a2)
#define Effect_Do3(a0) Call1(Func_02001ca8, a0)
#define Effect_Check20(a0) Value1(Func_02001d60, a0)
#define Effect_Check21(a0) Value1(Func_02001d68, a0)
#define Effect_SetRect2(a0, a1, a2, a3, a4, a5) Call6(Func_02001d2e_a, a0, a1, a2, a3, a4, a5)
#define Effect_Run2(a0, a1) Call2(Func_02001cf8, a0, a1)
#define Effect_Do4(a0) Call1_02000968(Func_02001d66, a0)
#define Effect_Place10(a0, a1, a2) Call3_02000de8(Func_020021dc, a0, a1, a2)
#define Effect_Place11(a0, a1, a2) Call3_02000de8(Func_02002234, a0, a1, a2)
#define Effect_Place12(a0, a1, a2) Call3_02000de8(Func_0200223a, a0, a1, a2)
#define Effect_Run3(a0, a1) Call2((void (*)())Func_0200244c, a0, a1)
#define Effect_Check22(a0) Value1(Func_02002458, a0)
#define Effect_Check23(a0) Value1(Func_02002460, a0)
#define Effect_Check24(a0) Value1(Func_02002468, a0)
#define Effect_Check25(a0) Value1(Func_02002470, a0)
#define Effect_Place13(a0, a1, a2) Call3(Func_02002544, a0, a1, a2)
#define Effect_Place14(a0, a1, a2) Call3(Func_020024a8, a0, a1, a2)
#define Effect_Place15(a0, a1, a2) Call3(Func_020024b6, a0, a1, a2)
#define Effect_Place16(a0, a1, a2) Call3(Func_020024ea, a0, a1, a2)
#define Effect_Place17(a0, a1, a2) Call3(Func_020024fe, a0, a1, a2)
#define Effect_Place18(a0, a1, a2) Call3(Func_02002588, a0, a1, a2)
#define Effect_Place19(a0, a1, a2) Call3(Func_020025a4, a0, a1, a2)
#define Effect_Run4(a0, a1) Call2(Func_020024fc, a0, a1)
#define Effect_Run5() Call0(Func_020024f2_a)

#endif
