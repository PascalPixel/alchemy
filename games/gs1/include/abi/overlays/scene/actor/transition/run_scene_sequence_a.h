#ifndef ALCHEMY_ABI_OVERLAYS_SCENE_ACTOR_TRANSITION_RUN_SCENE_SEQUENCE_A
#define ALCHEMY_ABI_OVERLAYS_SCENE_ACTOR_TRANSITION_RUN_SCENE_SEQUENCE_A

/* Binding layer — not production source. Address / far-call ABI only. */

void Func_02003ab0();
void Func_02003ae4();
s32 Func_02003b02();
void Func_02003b18();
void Func_02003b20();
void Func_02003b34();
void Func_02003b84();

static __inline__ void Call3(void (*f)(), s32 a0, s32 a1, s32 a2)
{
    f(a0, a1, a2);
}

static __inline__ void bump_step(s32 amount)
{
    u8 *work = *(u8 **)Data_03001ebc;

    *(u16 *)(work + 0x1d8) = (u16)(*(u16 *)(work + 0x1d8) + amount);
}

#define Actor_Check Func_02003b02
#define Actor_Run Func_02003ae4
#define Actor_Run2 Func_02003ab0
#define Actor_Run3 Func_02003b84
#define Actor_Run4 Func_02003b18
#define gWork Data_03001ebc
#define Actor_Place(a0, a1, a2) Call3(Func_02003b20, a0, a1, a2)
#define Actor_Place2(a0, a1, a2) Call3(Func_02003b34, a0, a1, a2)

#endif
