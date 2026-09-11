#ifndef ALCHEMY_ABI_OVERLAYS_SCENE_SCRIPT_RUN_3AF_02004218
#define ALCHEMY_ABI_OVERLAYS_SCENE_SCRIPT_RUN_3AF_02004218

/* Binding layer — not production source. Address / far-call ABI only. */

void Func_020084aa();
void Func_020084b2();
s32 Func_0200854e();
void Func_020085a8();
void Func_0200863e();

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

#define Scene_RunScript Func_02004218
#define Script_Check Func_0200854e
#define Script_Run2 Func_020084b2
#define Script_Run3 Func_020084aa
#define gWork Data_03001ebc
#define Script_Run(a0, a1, a2, a3) Call4(Func_0200863e, a0, a1, a2, a3)
#define Script_Place(a0, a1, a2) Call3(Func_020085a8, a0, a1, a2)

#endif
