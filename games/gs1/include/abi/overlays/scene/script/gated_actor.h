#ifndef ALCHEMY_ABI_OVERLAYS_SCENE_SCRIPT_GATED_ACTOR
#define ALCHEMY_ABI_OVERLAYS_SCENE_SCRIPT_GATED_ACTOR

/* Binding layer — not production source. Address / far-call ABI only. */

s32 Func_0200264a();
void Func_02002668();
void Func_020027a6();
void Func_02002674();
void Func_02002d0a();
s32 Func_0200299e();
void Func_020026bc();
void Func_02002ce0();
void Func_020028fe();
void Func_02002706();
void Func_02002cfe();
void Func_0200273e();
void Func_02002788();
void Func_0200285a();
void Func_02002b9c();
void Func_02002890();
void Func_02002b4c();
void Func_02002a90();
void Func_02002d8e();
void Func_02002730();
s32 Func_02002b20();
void Func_0200288e();
void Func_02002792();
void Func_02002780();
void Func_02002884();
void Func_020029fa();
void Func_02002e46();
void Func_020027ca();

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

#define Scene_RunGatedActorSequence Func_02000be0
#define Script_Run10 Func_0200288e
#define Script_Run11 Func_0200273e
#define Script_Run12 Func_02002788
#define Script_Run13 Func_02002792
#define Script_Run14 Func_020027a6
#define Script_Run15 Func_0200285a
#define Script_Run16 Func_02002890
#define Script_Run17 Func_02002a90
#define Script_Run18 Func_02002b4c
#define Script_Run19 Func_02002b9c
#define Script_Run20 Func_02002ce0
#define Script_Run21 Func_02002cfe
#define Script_Run22 Func_02002e46
#define Script_Run23 Func_02002d0a
#define Script_Run3 Func_020028a8
#define Script_Run4 Func_020028a8_a
#define Script_Run5 Func_020028a8_b
#define Script_Run6 Func_020026e0
#define Script_Run7 Func_02002674
#define Script_Run8 Func_020027ca
#define Script_Run9 Func_02002730
#define gOv Data_02009b85
#define gOv2 Data_02009ce0
#define gVal Data_00000f85
#define gVal2 Data_00000f91
#define Script_Check(a0) Value1(Func_0200264a, a0)
#define Script_Do(a0) Call1(Func_02002668, a0)
#define Script_Place(a0, a1, a2) Call3(Func_020026bc, a0, a1, a2)
#define Script_Place2(a0, a1, a2) Call3(Func_02002780, a0, a1, a2)
#define Script_Place3(a0, a1, a2) Call3(Func_02002706, a0, a1, a2)
#define Script_Place4(a0, a1, a2) Call3(Func_02002792, a0, a1, a2)
#define Script_Place5(a0, a1, a2) Call3(Func_020028fe, a0, a1, a2)
#define Script_Run(a0, a1) Call2(Func_02002884, a0, a1)
#define Script_Run2(a0, a1, a2, a3) Call4(Func_020029fa, a0, a1, a2, a3)
#define Script_Check2(a0, a1) Value2(Func_02002b20, a0, a1)
#define Script_Check3(a0, a1) Value2(Func_0200299e, a0, a1)
#define Script_Place6(a0, a1, a2) Call3(Func_0200288e, a0, a1, a2)
#define Script_Do2(a0) Call1(Func_02002730, a0)
#define Script_Place7(a0, a1, a2) Call3(Func_02002d8e, a0, a1, a2)

#endif
