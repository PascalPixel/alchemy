#ifndef ALCHEMY_ABI_OVERLAYS_SCENE_SCRIPT_RUN_39C_02001340
#define ALCHEMY_ABI_OVERLAYS_SCENE_SCRIPT_RUN_39C_02001340

/* Binding layer — not production source. Address / far-call ABI only. */

s32 Func_02006cd6();
void Func_02006d7e();
void Func_02006dfc();
void Func_02006e3c();
s32 Func_02006e42();
void Func_02006e82();
void Func_02006e94();
void Func_02006ea6();
void Func_02006eb2();
void Func_02006ebe();
void Func_02006ec4();
void Func_02006ef8();
void Func_02006fe2();

static __inline__ void Call1(void (*f)(), s32 a0)
{
    f(a0);
}

static __inline__ s32 Value2(s32 (*f)(), s32 a0, s32 a1)
{
    return f(a0, a1);
}

static __inline__ void Call3(void (*f)(), s32 a0, s32 a1, s32 a2)
{
    f(a0, a1, a2);
}

#define Scene_RunSel Func_02001340
#define Script_Check2 Func_02006e42
#define Script_Run Func_02006eb6
#define Script_Run2 Func_02006dfc
#define Script_Run3 Func_02006e94
#define Script_Run4 Func_02006fe2
#define Script_Run5 Func_02006ef8
#define Script_Run6 Func_02006eb2
#define gWork Data_03001ebc
#define Script_Check(a0, a1) Value2(Func_02006cd6, a0, a1)
#define Script_Place(a0, a1, a2) Call3(Func_02006e3c, a0, a1, a2)
#define Script_Place2(a0, a1, a2) Call3(Func_02006ea6, a0, a1, a2)
#define Script_Place3(a0, a1, a2) Call3(Func_02006ebe, a0, a1, a2)
#define Script_Place4(a0, a1, a2) Call3(Func_02006ec4, a0, a1, a2)
#define Script_Do(a0) Call1(Func_02006e82, a0)
#define Script_Do2(a0) Call1(Func_02006d7e, a0)

#endif
