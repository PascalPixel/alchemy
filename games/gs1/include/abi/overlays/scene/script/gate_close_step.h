#ifndef ALCHEMY_ABI_OVERLAYS_SCENE_SCRIPT_RUN_37F_02000D1C
#define ALCHEMY_ABI_OVERLAYS_SCENE_SCRIPT_RUN_37F_02000D1C

/* Binding layer — not production source. Address / far-call ABI only. */

void Func_020028cc();
void Func_020028e4();
s32 Func_0200293e();
s32 Func_0200293e_a();
s32 Func_02002952();
s32 Func_02002966();
void Func_0200297c();
void Func_02002986();
void Func_02002990();
void Func_02002990_a();
void Func_0200299a();
void Func_020029a4();
void Func_020029d2();
void Func_020029d6();
void Func_020029d6_a();
void Func_020029d6_b();
void Func_020029d6_c();
void Func_020029da();
void Func_020029e0();
void Func_020029e2();
void Func_020029e6();
void Func_020029ec();
void Func_020029f0();
void Func_020029f4();
void Func_020029fa();
void Func_02002a12();
void Func_02002a1a();
void Func_02002a28();
void Func_02002a36();
void Func_02002a40();
void Func_02002a6e();
s32 Func_02002a7e();
s32 Func_02002a7e_a();
void Func_02002a8a();
void Func_02002a92();
s32 Func_02002a9e();
void Func_02002aa0();
void Func_02002ab0();
void Func_02002ab8();
void Func_02002ab8_a();
s32 Func_02002abe();
void Func_02002ac0();
void Func_02002ac4();
void Func_02002ad0();
void Func_02002ad0_a();
void Func_02002ae0();
void Func_02002ae0_a();
void Func_02002ae0_b();
void Func_02002af0();
void Func_02002afa();
void Func_02002afe();
void Func_02002b10();
void Func_02002b16();
void Func_02002b18();
void Func_02002b1a();
void Func_02002b28();
void Func_02002b2a();
void Func_02002b3a();
void Func_02002b42();
void Func_02002b4a();

static __inline__ void Call1(void (*f)(), s32 a0)
{
    f(a0);
}

static __inline__ s32 Value1(s32 (*f)(), s32 a0)
{
    return f(a0);
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

static __inline__ void bump_step(s32 amount)
{
    u8 *work = *(u8 **)Data_03001ebc;

    *(u16 *)(work + 0x1d8) = (u16)(*(u16 *)(work + 0x1d8) + amount);
}

#define Scene_RunGate Func_02000d1c
#define Script_Check7 Func_0200293e
#define Script_Run10 Func_02002990
#define Script_Run11 Func_020029a4
#define Script_Run12 Func_020029d2
#define Script_Run13 Func_020029da
#define Script_Run14 Func_020029e2
#define Script_Run15 Func_020029e0
#define Script_Run16 Func_020029fa
#define Script_Run17 Func_02002a12
#define Script_Run18 Func_02002a1a
#define Script_Run19 Func_02002a28
#define Script_Run20 Func_02002a40
#define Script_Run21 Func_020029e6
#define Script_Run22 Func_02002a6e
#define Script_Run23 Func_020029f4
#define Script_Run24 Func_02002a92
#define Script_Run25 Func_02002a36
#define Script_Run26 Func_02002ab0
#define Script_Run27 Func_02002aa0
#define Script_Run28 Func_02002ad0_a
#define Script_Run29 Func_02002ac0
#define Script_Run30 Func_02002af0
#define Script_Run31 Func_02002ae0_a
#define Script_Run32 Func_02002afe
#define Script_Run33 Func_02002b10
#define Script_Run34 Func_02002b1a
#define Script_Run35 Func_02002b18
#define Script_Run36 Func_02002b2a
#define Script_Run37 Func_02002b3a
#define Script_Run38 Func_02002b42
#define Script_Run39 Func_02002b4a
#define Script_Run40 Func_02002b16
#define Script_Run6 Func_020028cc
#define Script_Run7 Func_020029d6
#define Script_Run8 Func_020028e4
#define Script_Run9 Func_0200297c
#define gWork Data_03001ebc
#define Script_Run(a0, a1, a2, a3) Call4(Func_020029d6_a, a0, a1, a2, a3)
#define Script_Run2(a0, a1) Call2(Func_020029d6_b, a0, a1)
#define Script_Run3(a0, a1, a2, a3) Call4(Func_020029f0, a0, a1, a2, a3)
#define Script_Check(a0) Value1(Func_0200293e_a, a0)
#define Script_Check2(a0) Value1(Func_02002952, a0)
#define Script_Check3(a0) Value1(Func_02002966, a0)
#define Script_Place(a0, a1, a2) Call3(Func_02002986, a0, a1, a2)
#define Script_Place2(a0, a1, a2) Call3(Func_02002990_a, a0, a1, a2)
#define Script_Place3(a0, a1, a2) Call3(Func_0200299a, a0, a1, a2)
#define Script_Place4(a0, a1, a2) Call3(Func_020029d6_c, a0, a1, a2)
#define Script_Place5(a0, a1, a2) Call3(Func_020029ec, a0, a1, a2)
#define Script_Place6(a0, a1, a2) Call3(Func_02002a7e, a0, a1, a2)
#define Script_Place7(a0, a1, a2) Call3(Func_02002a8a, a0, a1, a2)
#define Script_Place8(a0, a1, a2) Call3(Func_02002ab8, a0, a1, a2)
#define Script_Place9(a0, a1, a2) Call3(Func_02002ac4, a0, a1, a2)
#define Script_Place10(a0, a1, a2) Call3(Func_02002ad0, a0, a1, a2)
#define Script_Do(a0) Call1(Func_02002ab8_a, a0)
#define Script_Run4(a0, a1) Call2(Func_02002ae0, a0, a1)
#define Script_Run5(a0, a1, a2, a3) Call4(Func_02002b28, a0, a1, a2, a3)
#define Script_Check4(a0) Value1(Func_02002a7e_a, a0)
#define Script_Check5(a0) Value1(Func_02002a9e, a0)
#define Script_Check6(a0) Value1(Func_02002abe, a0)
#define Script_Do2(a0) Call1(Func_02002ae0_b, a0)
#define Script_Do3(a0) Call1(Func_02002afa, a0)

#endif
