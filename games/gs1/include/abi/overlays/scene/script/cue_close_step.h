#ifndef ALCHEMY_ABI_OVERLAYS_SCENE_SCRIPT_RUN_38D_0200179C
#define ALCHEMY_ABI_OVERLAYS_SCENE_SCRIPT_RUN_38D_0200179C

/* Binding layer — not production source. Address / far-call ABI only. */

void Func_02003c10();
void Func_02003c1a();
void Func_02003c20();
void Func_02003c26();
void Func_02003c3a();
void Func_02003c44();
void Func_02003c5e();
void Func_02003c60();
void Func_02003c66();
void Func_02003ca4_a();
void Func_02003ca4_b();
void Func_02003cbc();
void Func_02003d02();
void Func_02003d04();
void Func_02003d14();
void Func_02003d3c();
void Func_02003d46_a();
void Func_02003d46_b();
void Func_02003d50();
void Func_02003d5e();
void Func_02003d64_a();
void Func_02003d64_b();
void Func_02003d82();
void Func_02003d84();
void Func_02003d86();
void Func_02003da2();
void Func_02003da4();
void Func_02003da8();
void Func_02003daa_a();
void Func_02003daa_b();
void Func_02003db2();
void Func_02003dc8();
void Func_02003dda_a();
void Func_02003dda_b();
void Func_02003dea();
void Func_02003dee();
void Func_02003df2();
void Func_02003e00();
void Func_02003e06();
s32 Func_02003e0a();
void Func_02003e14();
void Func_02003e18();
void Func_02003e36();
void Func_02003e40();
void Func_02003e48_a();
void Func_02003e48_b();

static __inline__ void Call1(void (*f)(), s32 a0)
{
    f(a0);
}

static __inline__ void Call2(void (*f)(), s32 a0, s32 a1)
{
    f(a0, a1);
}

static __inline__ s32 Value2(s32 (*f)(), s32 a0, s32 a1)
{
    return f(a0, a1);
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

#define Scene_RunCue Func_0200179c
#define Script_Run10 Func_02003c1a
#define Script_Run11 Func_02003c60
#define Script_Run12 Func_02003c66
#define Script_Run13 Func_02003ca4_b
#define Script_Run14 Func_02003d64_b
#define Script_Run15 Func_02003da4
#define Script_Run16 Func_02003da2
#define Script_Run17 Func_02003dea
#define Script_Run18 Func_02003db2
#define Script_Run19 Func_02003dda_b
#define Script_Run20 Func_02003e14
#define Script_Run21 Func_02003d46_b
#define Script_Run22 Func_02003e06
#define Script_Run23 Func_02003e40
#define Script_Run24 Func_02003e00
#define Script_Run25 Func_02003e48_b
#define Script_Run26 Func_02003e18
#define Script_Run27 Func_02003da8
#define Script_Run3 Func_02003c10
#define Script_Run4 Func_02003d3c
#define Script_Run5 Func_02003d50
#define Script_Run6 Func_02003c3a
#define Script_Run7 Func_02003daa_a
#define Script_Run8 Func_02003c20
#define Script_Run9 Func_02003c26
#define gWork Data_03001ebc
#define Script_Place(a0, a1, a2) Call3(Func_02003d14, a0, a1, a2)
#define Script_Place2(a0, a1, a2) Call3(Func_02003c5e, a0, a1, a2)
#define Script_Place3(a0, a1, a2) Call3(Func_02003ca4_a, a0, a1, a2)
#define Script_Run(a0, a1, a2, a3) Call4(Func_02003d64_a, a0, a1, a2, a3)
#define Script_Do(a0) Call1(Func_02003c44, a0)
#define Script_Place4(a0, a1, a2) Call3(Func_02003d04, a0, a1, a2)
#define Script_Place5(a0, a1, a2) Call3(Func_02003cbc, a0, a1, a2)
#define Script_Place6(a0, a1, a2) Call3(Func_02003d02, a0, a1, a2)
#define Script_Do2(a0) Call1(Func_02003d82, a0)
#define Script_Place7(a0, a1, a2) Call3(Func_02003dc8, a0, a1, a2)
#define Script_Place8(a0, a1, a2) Call3(Func_02003d46_a, a0, a1, a2)
#define Script_Place9(a0, a1, a2) Call3(Func_02003dda_a, a0, a1, a2)
#define Script_Place10(a0, a1, a2) Call3(Func_02003d5e, a0, a1, a2)
#define Script_Place11(a0, a1, a2) Call3(Func_02003df2, a0, a1, a2)
#define Script_Check(a0, a1) Value2(Func_02003e0a, a0, a1)
#define Script_Place12(a0, a1, a2) Call3(Func_02003e36, a0, a1, a2)
#define Script_Run2(a0, a1) Call2(Func_02003e48_a, a0, a1)
#define Script_Place13(a0, a1, a2) Call3(Func_02003daa_b, a0, a1, a2)
#define Script_Place14(a0, a1, a2) Call3(Func_02003dee, a0, a1, a2)
#define Script_Do3(a0) Call1(Func_02003d86, a0)
#define Script_Do4(a0) Call1(Func_02003d84, a0)

#endif
