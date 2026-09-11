#ifndef ALCHEMY_ABI_OVERLAYS_SCENE_ACTOR_TRANSITION_RUN_SCENE_PRIMARY_SEQUENCE
#define ALCHEMY_ABI_OVERLAYS_SCENE_ACTOR_TRANSITION_RUN_SCENE_PRIMARY_SEQUENCE

/* Binding layer — not production source. Address / far-call ABI only. */

void Func_02002f30();
void Func_02002f72();
s32 Func_02003a7e();
void Func_02003aa4();

static __inline__ void Call1(void (*f)(), s32 a0)
{
    f(a0);
}

static __inline__ s32 Value1(s32 (*f)(), s32 a0)
{
    return f(a0);
}

static __inline__ void bump_step(s32 amount)
{
    u8 *work = *(u8 **)Data_03001ebc;

    *(u16 *)(work + 0x1d8) = (u16)(*(u16 *)(work + 0x1d8) + amount);
}

#define Actor_Run Func_02001750
#define Actor_Run2 Func_02002f30
#define Actor_Run3 Func_02002f72
#define gCell Data_02000240
#define gCell2 Data_02000240_t
#define gVal Data_00000034
#define gWork Data_03001ebc
#define Actor_Check(a0) Value1(Func_02003a7e, a0)
#define Actor_Do(a0) Call1(Func_02003aa4, a0)

#endif
