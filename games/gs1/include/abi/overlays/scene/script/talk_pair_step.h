#ifndef ALCHEMY_ABI_OVERLAYS_SCENE_SCRIPT_RUN_39A_02001AD0
#define ALCHEMY_ABI_OVERLAYS_SCENE_SCRIPT_RUN_39A_02001AD0

/* Binding layer — not production source. Address / far-call ABI only. */

void Func_02003e10();
void Func_02003e3e();
void Func_02003e54();
s32 Func_02003e58();
void Func_02003e78();
void Func_02003e7a();
void Func_02003e7a_a();
void Func_02003e88();

static __inline__ void Call3(void (*f)(), s32 a0, s32 a1, s32 a2)
{
    f(a0, a1, a2);
}

static __inline__ s32 Value3(s32 (*f)(), s32 a0, s32 a1, s32 a2)
{
    return f(a0, a1, a2);
}

static __inline__ void bump_step(s32 amount)
{
    u8 *work = *(u8 **)Data_03001ebc;

    *(u16 *)(work + 0x1d8) = (u16)(*(u16 *)(work + 0x1d8) + amount);
}

#define Scene_RunTalk Func_02001ad0
#define Script_Run Func_02003e10
#define Script_Run2 Func_02003e7a
#define Script_Run3 Func_02003e7a_a
#define Script_Run4 Func_02003e78
#define Script_Run5 Func_02003e88
#define Script_Run6 Func_02003e54
#define gWork Data_03001ebc
#define Script_Place(a0, a1, a2) Call3(Func_02003e3e, a0, a1, a2)
#define Script_Place2(a0, a1, a2) Value3(Func_02003e58, a0, a1, a2)

#endif
