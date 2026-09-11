#ifndef ALCHEMY_ABI_OVERLAYS_SCENE_STATE_TASK_RUN_SCENE_SCENE_THREE_COORDINATOR
#define ALCHEMY_ABI_OVERLAYS_SCENE_STATE_TASK_RUN_SCENE_SCENE_THREE_COORDINATOR

/* Binding layer — not production source. Address / far-call ABI only. */

s32 Func_02001930();
void Func_02003346();
s32 Func_0200355c();
void Func_02003750();
s32 Func_020037cc();
void Func_0200417a();
void Func_020043fe();
s32 Func_020046d8();
s32 Func_020046e2();
void Func_020046f6();
void Func_02005490();
void Func_020054c6();
void Func_02005508();
void Func_0200553e();
void Func_02005544();
void Func_02005546();
void Func_02005570();
void Func_02005590();
void Func_02005592();
void Func_020055a8();
void Func_020055ac();
void Func_020055ae();
void Func_020055b8();
void Func_020055ba();
void Func_020055d2();
void Func_020055d8();
void Func_020055da();
void Func_020055e2();
void Func_020055e6();
void Func_020055fc();
void Func_02005620();
void Func_02005652();
void Func_02005654();
void Func_0200565c();
void Func_02005676();
void Func_0200568e();
void Func_02005692();

static __inline__ void Call1(void (*f)(), s32 a0)
{
    f(a0);
}

static __inline__ void Call2(void (*f)(), s32 a0, s32 a1)
{
    f(a0, a1);
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

static __inline__ void Call4(void (*f)(), s32 a0, s32 a1, s32 a2, s32 a3)
{
    f(a0, a1, a2, a3);
}

static __inline__ void bump_step(s32 amount)
{
    u8 *work = *(u8 **)Data_03001ebc;

    *(u16 *)(work + 0x1d8) = (u16)(*(u16 *)(work + 0x1d8) + amount);
}

#define State_Run10 Func_020055e6
#define State_Run11 Func_020055a8
#define State_Run12 Func_0200417a
#define State_Run13 Func_02005508
#define State_Run14 Func_020055e2
#define State_Run15 Func_02005544
#define State_Run16 Func_020055d2
#define State_Run17 Func_02005570
#define State_Run18 Func_02005590
#define State_Run19 Func_02005654
#define State_Run20 Func_020043fe
#define State_Run21 Func_02005692
#define State_Run22 Func_02003750
#define State_Run23 Func_0200568e
#define State_Run24 Func_020055fc
#define State_Run6 Func_02003346
#define State_Run7 Func_02005490
#define State_Run8 Func_020055b8
#define State_Run9 Func_020054c6
#define gCell Data_02000240
#define gCell2 Data_02000240_t
#define gWork Data_03001ebc
#define State_Check(a0, a1) Value2(Func_0200355c, a0, a1)
#define State_Do(a0) Call1(Func_0200553e, a0)
#define State_Run(a0, a1) Call2(Func_02005592, a0, a1)
#define State_Run2(a0, a1, a2, a3) Call4(Func_020055ac, a0, a1, a2, a3)
#define State_Run3(a0, a1) Call2(Func_020055ba, a0, a1)
#define State_Run4(a0, a1, a2, a3) Call4(Func_020055d8, a0, a1, a2, a3)
#define State_Place(a0, a1, a2) Call3(Func_02005546, a0, a1, a2)
#define State_Place2(a0, a1, a2) Value3(Func_020046d8, a0, a1, a2)
#define State_Place3(a0, a1, a2) Value3(Func_020046e2, a0, a1, a2)
#define State_Place4(a0, a1, a2) Call3(Func_020046f6, a0, a1, a2)
#define State_Place5(a0, a1, a2) Value3(Func_02001930, a0, a1, a2)
#define State_Run5(a0, a1, a2, a3) Call4(Func_02005652, a0, a1, a2, a3)
#define State_Place6(a0, a1, a2) Call3(Func_020055ae, a0, a1, a2)
#define State_Place7(a0, a1, a2) Call3(Func_020055da, a0, a1, a2)
#define State_Place8(a0, a1, a2) Call3(Func_0200565c, a0, a1, a2)
#define State_Place9(a0, a1, a2) Call3(Func_02005620, a0, a1, a2)
#define State_Do2(a0) Call1(Func_02005676, a0)
#define State_Place10(a0, a1, a2) Value3(Func_020037cc, a0, a1, a2)

#endif
