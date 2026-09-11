#ifndef ALCHEMY_ABI_MAP_LOCATIONS_HEIDIA_VILLAGE_SCENE_SEQ_TAIL
#define ALCHEMY_ABI_MAP_LOCATIONS_HEIDIA_VILLAGE_SCENE_SEQ_TAIL

/* Binding layer — not production source. Address / far-call ABI only. */

void Func_020076a0();
void Func_020076f8();
void Func_0200771e();
void Func_0200772c();
void Func_0200773a();
void Func_0200774a();
void Func_02007752();
void Func_0200775a();
void Func_0200778c();
void Func_02007834();

static __inline__ void Call0(void (*f)())
{
    f();
}

static __inline__ void Call2(void (*f)(), s32 a0, s32 a1)
{
    f(a0, a1);
}

static __inline__ void Call3(void (*f)(), s32 a0, s32 a1, s32 a2)
{
    f(a0, a1, a2);
}

#define Battle_Reset_1() Call0(Func_020076a0)
#define Motion_SetHPosTerrain_1(a0, a1, a2) Call3(Func_0200771e, a0, a1, a2)
#define Motion_SetHPosTerrain_2(a0, a1, a2) Call3(Func_0200772c, a0, a1, a2)
#define Motion_SetHPosTerrain_3(a0, a1, a2) Call3(Func_0200773a, a0, a1, a2)
#define Object_SetModeById_1(a0, a1) Call2(Func_0200774a, a0, a1)
#define Object_SetModeById_2(a0, a1) Call2(Func_02007752, a0, a1)
#define Object_SetModeById_3(a0, a1) Call2(Func_0200775a, a0, a1)
#define Motion_SetAngleToward_1(a0, a1, a2) Call3(Func_0200778c, a0, a1, a2)
#define Battle_SchedShoulder_1() Call0(Func_020076f8)
#define Battle_WaitMode0_1() Call0(Func_02007834)

#endif
