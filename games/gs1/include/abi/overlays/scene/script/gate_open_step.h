#ifndef ALCHEMY_ABI_OVERLAYS_SCENE_SCRIPT_RUN_37F_0200092C
#define ALCHEMY_ABI_OVERLAYS_SCENE_SCRIPT_RUN_37F_0200092C

/* Binding layer — not production source. Address / far-call ABI only. */

void Func_0200248a();
void Func_020024ca();
s32 Func_020024d6();
void Func_020024de();
s32 Func_020024f0();
s32 Func_020024f0_a();
void Func_02002502();
void Func_02002514();
void Func_02002540();
s32 Func_02002542();
void Func_02002552();
void Func_02002564();
void Func_020025c4();

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

static __inline__ void Call6(void (*f)(), s32 a0, s32 a1, s32 a2, s32 a3, s32 a4, s32 a5)
{
    f(a0, a1, a2, a3, a4, a5);
}

#define Scene_RunGate Func_0200092c
#define gOv Data_0200a69c
#define gWork Data_03001ebc
#define Script_Do(a0) Call1(Func_020024ca, a0)
#define Script_Check(a0) Value1(Func_020024d6, a0)
#define Script_Run(a0, a1) Call2(Func_0200248a, a0, a1)
#define Script_Check2(a0) Value1(Func_020024f0, a0)
#define Script_SetRect(a0, a1, a2, a3, a4, a5) Call6(Func_020024de, a0, a1, a2, a3, a4, a5)
#define Script_SetRect2(a0, a1, a2, a3, a4, a5) Call6(Func_020024f0_a, a0, a1, a2, a3, a4, a5)
#define Script_SetRect3(a0, a1, a2, a3, a4, a5) Call6(Func_02002502, a0, a1, a2, a3, a4, a5)
#define Script_SetRect4(a0, a1, a2, a3, a4, a5) Call6(Func_02002514, a0, a1, a2, a3, a4, a5)
#define Script_Check3(a0) Value1(Func_02002542, a0)
#define Script_Place(a0, a1, a2) Call3(Func_020025c4, a0, a1, a2)
#define Script_SetRect5(a0, a1, a2, a3, a4, a5) Call6(Func_02002540, a0, a1, a2, a3, a4, a5)
#define Script_SetRect6(a0, a1, a2, a3, a4, a5) Call6(Func_02002552, a0, a1, a2, a3, a4, a5)
#define Script_SetRect7(a0, a1, a2, a3, a4, a5) Call6(Func_02002564, a0, a1, a2, a3, a4, a5)

#endif
