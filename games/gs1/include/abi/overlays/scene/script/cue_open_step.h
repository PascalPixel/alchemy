#ifndef ALCHEMY_ABI_OVERLAYS_SCENE_SCRIPT_RUN_38D_020005F4
#define ALCHEMY_ABI_OVERLAYS_SCENE_SCRIPT_RUN_38D_020005F4

/* Binding layer — not production source. Address / far-call ABI only. */

s32 Func_02002a4a();
void Func_02002a72();
void Func_02002aac();
void Func_02002ab6();
void Func_02002abc();
void Func_02002af6();
void Func_02002afa();
void Func_02002afc();
void Func_02002b16();
void Func_02002b20();
void Func_02002b34();
void Func_02002b3e();
void Func_02002b46();
void Func_02002b58();
void Func_02002b64();
void Func_02002b76();
void Func_02002b82();
void Func_02002b8e();
void Func_02002b9a();
void Func_02002bbc();

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

#define Scene_RunCue Func_020005f4
#define Script_Run Func_02002a72
#define Script_Run10 Func_02002b82
#define Script_Run11 Func_02002b20
#define Script_Run2 Func_02002b34
#define Script_Run3 Func_02002b3e
#define Script_Run4 Func_02002b46
#define Script_Run5 Func_02002aac
#define Script_Run6 Func_02002b76
#define Script_Run7 Func_02002abc
#define Script_Run8 Func_02002b64
#define Script_Run9 Func_02002bbc
#define gWork Data_03001ebc
#define Script_Check(a0) Value1(Func_02002a4a, a0)
#define Script_Place(a0, a1, a2) Call3(Func_02002ab6, a0, a1, a2)
#define Script_Place2(a0, a1, a2) Call3(Func_02002afa, a0, a1, a2)
#define Script_Place3(a0, a1, a2) Call3(Func_02002b8e, a0, a1, a2)
#define Script_Do(a0) Call1(Func_02002b9a, a0)
#define Script_Place4(a0, a1, a2) Call3(Func_02002b16, a0, a1, a2)
#define Script_Place5(a0, a1, a2) Call3(Func_02002b58, a0, a1, a2)
#define Script_Do2(a0) Call1(Func_02002af6, a0)
#define Script_Do3(a0) Call1(Func_02002afc, a0)

#endif
