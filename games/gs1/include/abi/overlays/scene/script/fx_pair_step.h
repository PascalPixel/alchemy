#ifndef ALCHEMY_ABI_OVERLAYS_SCENE_SCRIPT_RUN_3AF_02001C14
#define ALCHEMY_ABI_OVERLAYS_SCENE_SCRIPT_RUN_3AF_02001C14

/* Binding layer — not production source. Address / far-call ABI only. */

void Func_020027fa();
void Func_020056ac();
void Func_02005f22();
void Func_02005f76();
void Func_02005f86();
void Func_02005fbc();
void Func_02005fc2();
void Func_02005fce();
void Func_02005fda();
void Func_02005fe4();
void Func_02006018();
void Func_0200603a();
void Func_0200604a();
void Func_02006068();
void Func_02006070_a();
void Func_02006070_b();
void Func_02006084();
void Func_02006092();
void Func_0200610a();
void Func_02006120();
void Func_0200612c();

static __inline__ void Call1(void (*f)(), s32 a0)
{
    f(a0);
}

static __inline__ void Call2(void (*f)(), s32 a0, s32 a1)
{
    f(a0, a1);
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

#define Scene_RunScript Func_02001c14
#define Script_Run10 Func_02006092
#define Script_Run11 Func_02006120
#define Script_Run12 Func_0200612c
#define Script_Run13 Func_0200610a
#define Script_Run2 Func_02006070_a
#define Script_Run3 Func_02006084
#define Script_Run4 Func_02005f22
#define Script_Run5 Func_020027fa
#define Script_Run6 Func_02006018
#define Script_Run7 Func_02005f86
#define Script_Run8 Func_0200603a
#define Script_Run9 Func_0200604a
#define gWork Data_03001ebc
#define Script_Place(a0, a1, a2) Call3(Func_02005fbc, a0, a1, a2)
#define Script_Place2(a0, a1, a2) Call3(Func_02005f76, a0, a1, a2)
#define Script_Place3(a0, a1, a2) Call3(Func_02005fc2, a0, a1, a2)
#define Script_Place4(a0, a1, a2) Call3(Func_02005fce, a0, a1, a2)
#define Script_Place5(a0, a1, a2) Call3(Func_02005fda, a0, a1, a2)
#define Script_Place6(a0, a1, a2) Call3(Func_02005fe4, a0, a1, a2)
#define Script_Place7(a0, a1, a2) Call3(Func_02006068, a0, a1, a2)
#define Script_Run(a0, a1) Call2(Func_020056ac, a0, a1)
#define Script_Do(a0) Call1(Func_02006070_b, a0)

#endif
