#ifndef ALCHEMY_ABI_OVERLAYS_SCENE_SCRIPT_RUN_3AF_02001A98
#define ALCHEMY_ABI_OVERLAYS_SCENE_SCRIPT_RUN_3AF_02001A98

/* Binding layer — not production source. Address / far-call ABI only. */

void Func_02003760();
void Func_02005d10();
void Func_02005d88();
void Func_02005da0();
void Func_02005dc2();
s32 Func_02005e16();
void Func_02005e2a();
s32 Func_02005e2e();
void Func_02005e34();
void Func_02005e3e();
void Func_02005e48();
void Func_02005e52();
void Func_02005e5c();
void Func_02005e66();
void Func_02005e70();
void Func_02005e88();
void Func_02005ec2();
void Func_02005f4e();

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

#define Scene_RunScript Func_02001a98
#define Script_Check Func_02005e16
#define Script_Check2 Func_02005e2e
#define Script_Run10 Func_02005e5c
#define Script_Run11 Func_02005e66
#define Script_Run12 Func_02005e70
#define Script_Run13 Func_02005dc2
#define Script_Run14 Func_02005da0
#define Script_Run15 Func_02003760
#define Script_Run3 Func_02005d88
#define Script_Run4 Func_02005d10
#define Script_Run5 Func_02005e2a
#define Script_Run6 Func_02005e34
#define Script_Run7 Func_02005e3e
#define Script_Run8 Func_02005e48
#define Script_Run9 Func_02005e52
#define gWork Data_03001ebc
#define Script_Run(a0, a1, a2, a3) Call4(Func_02005ec2, a0, a1, a2, a3)
#define Script_Place(a0, a1, a2) Call3(Func_02005e88, a0, a1, a2)
#define Script_Run2(a0, a1, a2, a3) Call4(Func_02005f4e, a0, a1, a2, a3)

#endif
