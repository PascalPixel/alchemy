#ifndef ALCHEMY_ABI_MAP_LOCATIONS_COLOSSO_LOG_ROLLING_STAGE_EVENT_OPEN_AUX
#define ALCHEMY_ABI_MAP_LOCATIONS_COLOSSO_LOG_ROLLING_STAGE_EVENT_OPEN_AUX

/* Binding layer — not production source. Address / far-call ABI only. */

void Func_0200469e();
void Func_020046b8();
s32 Func_02004710();
void Func_02004bd6();
s32 Func_02004dec();
void Func_02004fcc();
s32 Func_02005048();
s32 Func_020059ea();
void Func_02005c88();
void Func_02005cd0();
void Func_02005d36();
void Func_02006c72();
void Func_02006d50();
void Func_02006d86();
void Func_02006dd8();
void Func_02006de0();
void Func_02006de6();
void Func_02006df6();
void Func_02006e16();
s32 Func_02006e2e();
void Func_02006e56();
void Func_02006e5e();
void Func_02006e6a();
void Func_02006e6c();
void Func_02006e84();
void Func_02006e90();
void Func_02006ea8();
void Func_02006ed0();
void Func_02006eee();
void Func_02006ef0();
void Func_02006f14();
void Func_02006f20();
void Func_02006f3a();
void Func_02006f52();
void Func_02006f64();

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

#define Map_Check2 Func_02006e2e
#define Map_Run10 Func_02006e5e
#define Map_Run11 Func_020046b8
#define Map_Run12 Func_02006dd8
#define Map_Run13 Func_02006de6
#define Map_Run14 Func_02005cd0
#define Map_Run15 Func_02006e6c
#define Map_Run16 Func_02006c72
#define Map_Run17 Func_02006f14
#define Map_Run18 Func_02005d36
#define Map_Run19 Func_02005c88
#define Map_Run20 Func_02006f64
#define Map_Run21 Func_02004fcc
#define Map_Run22 Func_02006f52
#define Map_Run23 Func_02006ea8
#define Map_Run5 Func_02004bd6
#define Map_Run6 Func_02006d50
#define Map_Run7 Func_02006e90
#define Map_Run8 Func_02006d86
#define Map_Run9 Func_02006e56
#define gCell Data_02000240
#define gCell2 Data_02000240_t
#define gOv Data_02000432
#define gWork Data_03001ebc
#define Map_Check(a0, a1) Value2(Func_02004dec, a0, a1)
#define Map_Do(a0) Call1(Func_02006e16, a0)
#define Map_Run(a0, a1) Call2(Func_02006e6a, a0, a1)
#define Map_Run2(a0, a1, a2, a3) Call4(Func_02006e84, a0, a1, a2, a3)
#define Map_Place(a0, a1, a2) Value3(Func_020059ea, a0, a1, a2)
#define Map_Place2(a0, a1, a2) Call3(Func_02006de0, a0, a1, a2)
#define Map_Place3(a0, a1, a2) Call3(Func_02006df6, a0, a1, a2)
#define Map_Place4(a0, a1, a2) Call3(Func_0200469e, a0, a1, a2)
#define Map_Run3(a0, a1) Call2(Func_02006ed0, a0, a1)
#define Map_Run4(a0, a1) Call2(Func_02006eee, a0, a1)
#define Map_Place5(a0, a1, a2) Call3(Func_02006ef0, a0, a1, a2)
#define Map_Place6(a0, a1, a2) Call3(Func_02006f20, a0, a1, a2)
#define Map_Place7(a0, a1, a2) Value3(Func_02004710, a0, a1, a2)
#define Map_Do2(a0) Call1(Func_02006f3a, a0)
#define Map_Place8(a0, a1, a2) Value3(Func_02005048, a0, a1, a2)

#endif
