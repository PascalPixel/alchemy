#ifndef ALCHEMY_ABI_OVERLAYS_SCENE_SCRIPT_RUN_3AF_02001B58
#define ALCHEMY_ABI_OVERLAYS_SCENE_SCRIPT_RUN_3AF_02001B58

/* Binding layer — not production source. Address / far-call ABI only. */

void Func_02003818();
void Func_02005dc4();
void Func_02005dec();
void Func_02005dfe();
void Func_02005e10();
void Func_02005e20();
void Func_02005e2e();
void Func_02005e48();
void Func_02005e58();
void Func_02005e5e();
s32 Func_02005e90();
void Func_02005edc();
void Func_02005ee2_a();
void Func_02005ee2_b();
s32 Func_02005ee4();
s32 Func_02005efc();
void Func_02005f22();
void Func_02005f2a();
void Func_02005f34();
void Func_02005f3e();
void Func_02005f56();
void Func_02005f9c();

static __inline__ void Call1(void (*f)(), s32 a0)
{
    f(a0);
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

#define Scene_RunScript Func_02001b58
#define Script_Check Func_02005e90
#define Script_Check2 Func_02005ee4
#define Script_Check3 Func_02005efc
#define Script_Run Func_02005e48
#define Script_Run10 Func_02005ee2_b
#define Script_Run11 Func_02005e10
#define Script_Run12 Func_02005f2a
#define Script_Run13 Func_02005f34
#define Script_Run14 Func_02005f3e
#define Script_Run15 Func_02005e58
#define Script_Run16 Func_02003818
#define Script_Run2 Func_02005dc4
#define Script_Run3 Func_02005f22
#define Script_Run4 Func_02005e2e
#define Script_Run5 Func_02005dec
#define Script_Run6 Func_02005f9c
#define Script_Run7 Func_02005e20
#define Script_Run8 Func_02005dfe
#define Script_Run9 Func_02005edc
#define gWork Data_03001ebc
#define Script_Do(a0) Call1(Func_02005e5e, a0)
#define Script_Place(a0, a1, a2) Call3(Func_02005ee2_a, a0, a1, a2)
#define Script_Place2(a0, a1, a2) Call3(Func_02005f56, a0, a1, a2)

#endif
