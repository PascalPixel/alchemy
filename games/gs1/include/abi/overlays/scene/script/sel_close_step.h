#ifndef ALCHEMY_ABI_OVERLAYS_SCENE_SCRIPT_RUN_39C_020014CC
#define ALCHEMY_ABI_OVERLAYS_SCENE_SCRIPT_RUN_39C_020014CC

/* Binding layer — not production source. Address / far-call ABI only. */

void Func_02006f40();
void Func_02006f48();
s32 Func_02006f58();
void Func_02006f90();
s32 Func_02006fb2_a();
s32 Func_02006fb2_b();
void Func_02006fb6();
s32 Func_02006fbc();
s32 Func_02006fe4();
s32 Func_02006ff2();
s32 Func_02006ffa();
void Func_0200700c();
void Func_020071a6();

static __inline__ void Call1(void (*f)(), s32 a0)
{
    f(a0);
}

static __inline__ s32 Value1(s32 (*f)(), s32 a0)
{
    return f(a0);
}

static __inline__ void Call3(void (*f)(), s32 a0, s32 a1, s32 a2)
{
    f(a0, a1, a2);
}

static __inline__ void Call6(void (*f)(), s32 a0, s32 a1, s32 a2, s32 a3, s32 a4, s32 a5)
{
    f(a0, a1, a2, a3, a4, a5);
}

#define Scene_RunSel Func_020014cc
#define Script_Check6 Func_02006fb2_b
#define Script_Check7 Func_02006fe4
#define Script_Run Func_02006fb6
#define Script_Run2 Func_020071a6
#define Script_Run3 Func_0200700c
#define Script_Check(a0) Value1(Func_02006f58, a0)
#define Script_Check2(a0) Value1(Func_02006fb2_a, a0)
#define Script_Check3(a0) Value1(Func_02006fbc, a0)
#define Script_Do(a0) Call1(Func_02006f90, a0)
#define Script_Check4(a0) Value1(Func_02006ff2, a0)
#define Script_Check5(a0) Value1(Func_02006ffa, a0)
#define Script_SetRect(a0, a1, a2, a3, a4, a5) Call6(Func_02006f40, a0, a1, a2, a3, a4, a5)
#define Script_Place(a0, a1, a2) Call3(Func_02006f48, a0, a1, a2)

#endif
