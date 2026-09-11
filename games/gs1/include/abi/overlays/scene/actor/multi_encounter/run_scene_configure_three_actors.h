#ifndef ALCHEMY_ABI_OVERLAYS_SCENE_ACTOR_MULTI_ENCOUNTER_RUN_SCENE_CONFIGURE_THREE_ACTORS
#define ALCHEMY_ABI_OVERLAYS_SCENE_ACTOR_MULTI_ENCOUNTER_RUN_SCENE_CONFIGURE_THREE_ACTORS

/* Binding layer — not production source. Address / far-call ABI only. */

void Func_0200661a();
void Func_02006652();
void Func_02006dfc();
void Func_02006e38();
void Func_02006e6c();
void Func_02006e96();
s32 Func_02006e9a();
s32 Func_02006ec0();
s32 Func_02006eda();
void Func_02006ef2();
s32 Func_02006efc();
void Func_02006f1a();
void Func_02006f2c();
void Func_02006f34();
void Func_02006f56();
void Func_02006f60();
void Func_02006f9c();
void Func_02006fa6();
void Func_02006faa();
void Func_02006fb2();
s32 Func_02006fc0();
void Func_02006fd4();
void Func_02006ff4();
s32 Func_0200702c();
void Func_02007040();
void Func_02007054();
void Func_02007090();

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

static __inline__ s32 Value3(s32 (*f)(), s32 a0, s32 a1, s32 a2)
{
    return f(a0, a1, a2);
}

static __inline__ void bump_step(s32 amount)
{
    u8 *work = *(u8 **)Data_03001ebc;

    *(u16 *)(work + 0x1d8) = (u16)(*(u16 *)(work + 0x1d8) + amount);
}

#define Actor_Run Func_02006e38
#define Actor_Run2 Func_02006dfc
#define Actor_Run3 Func_0200661a
#define Actor_Run4 Func_02006652
#define Actor_Run5 Func_02007090
#define gWork Data_03001ebc
#define Battle_Reset_1(args...) Func_02006e6c(args)
#define ObjectGroup_ConfigureChildValue_1(args...) Func_02006f2c(args)
#define Scene_GetRecord_1(args...) Func_02006e9a(args)
#define Object_NotifyLastActiveOfEvent_1(a0) Call1(Func_02006e96, a0)
#define Motion_SetHPosTerrain_1(a0, a1, a2) Call3(Func_02006f1a, a0, a1, a2)
#define Scene_GetRecord_2(args...) Func_02006ec0(args)
#define Motion_SetHPosTerrain_2(a0, a1, a2) Call3(Func_02006f34, a0, a1, a2)
#define Scene_GetRecord_3(args...) Func_02006eda(args)
#define Motion_SetActionVariant_1(a0, a1) Value2(Func_02006fc0, a0, a1)
#define Motion_SetHPosTerrain_3(a0, a1, a2) Call3(Func_02006f56, a0, a1, a2)
#define Scene_GetRecord_4(args...) Func_02006efc(args)
#define Battle_WaitMode0_1(args...) Func_02007040(args)
#define Motion_SetSpeed_1(args...) Func_02007054(args)
#define Battle_WaitMode0_2(args...) Func_02006ef2(args)
#define Motion_Launch_1(args...) Func_02006f9c(args)
#define Motion_Launch_2(args...) Func_02006fa6(args)
#define SceneWork_SetStepValue_1(a0) Call1(Func_02006fd4, a0)
#define Motion_CallWaitAnim_1(args...) Func_02006fb2(args)
#define Motion_SetSpeed_2(a0, a1, a2) Call3(Func_02006f60, a0, a1, a2)
#define Motion_SetPosReset_1(a0, a1, a2) Call3(Func_02006faa, a0, a1, a2)
#define Motion_ArmCb_1(a0, a1, a2) Value3(Func_0200702c, a0, a1, a2)
#define Motion_SetVarCbObj_1(args...) Func_02006ff4(args)
#define SCENE_PHASE(*(s32 *)(*(u8 *volatile *)Data_03001ebc + 0x1c0))

#endif
