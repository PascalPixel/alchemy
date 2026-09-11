#ifndef ALCHEMY_ABI_OVERLAYS_SCENE_SCRIPT_RUN_39C_020010C0
#define ALCHEMY_ABI_OVERLAYS_SCENE_SCRIPT_RUN_39C_020010C0

/* Binding layer — not production source. Address / far-call ABI only. */

void Func_02006b38();
s32 Func_02006b4c();
s32 Func_02006b56();
s32 Func_02006b60();
void Func_02006b6e();
void Func_02006b72();
void Func_02006b7a();
void Func_02006b98();
void Func_02006bb4();
void Func_02006bd8();
void Func_02006d08();
void Func_02006d16();
void Func_02006d22();
void Func_02006d66();
void Func_02006d88();

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

static __inline__ void bump_step(s32 amount)
{
    u8 *work = *(u8 **)Data_03001ebc;

    *(u16 *)(work + 0x1d8) = (u16)(*(u16 *)(work + 0x1d8) + amount);
}

#define Scene_RunSel Func_020010c0
#define Script_Run Func_02006b98
#define Script_Run2 Func_02006d66
#define Script_Run3 Func_02006bb4
#define Script_Run4 Func_02006d16
#define Script_Run5 Func_02006d22
#define Script_Run6 Func_02006b7a
#define Script_Run7 Func_02006d08
#define gWork Data_03001ebc
#define Script_Check(a0) Value1(Func_02006b4c, a0)
#define Script_Check2(a0) Value1(Func_02006b56, a0)
#define Script_Check3(a0) Value1(Func_02006b60, a0)
#define Script_Do(a0) Call1(Func_02006b72, a0)
#define Script_Place(a0, a1, a2) Call3(Func_02006b38, a0, a1, a2)
#define Script_Do2(a0) Call1(Func_02006d88, a0)
#define Script_Place2(a0, a1, a2) Call3(Func_02006b6e, a0, a1, a2)
#define Script_Do3(a0) Call1(Func_02006bd8, a0)

#endif
