#ifndef ALCHEMY_ABI_OVERLAYS_SCENE_STATE_TASK_RUN_SCENE_LATE_SEQUENCE
#define ALCHEMY_ABI_OVERLAYS_SCENE_STATE_TASK_RUN_SCENE_LATE_SEQUENCE

/* Binding layer — not production source. Address / far-call ABI only. */

void Func_02004eda();
void Func_02004f06();
s32 Func_02005466();
s32 Func_02005492();
void Func_0200690c();
void Func_02006a42();
void Func_02006a56();
void Func_02006a5e();
void Func_02006a62();
void Func_02006a6c();
void Func_02006a76();
void Func_02006a8a();
void Func_02006aa6();
void Func_02006ab0();
void Func_02006b48();
void Func_02006b5c();
void Func_02006b64();
void Func_02006b94();
void Func_02006bba();
void Func_02006bc8();
void Func_02006be4();
void Func_02006bec();
void Func_02006bf2();
void Func_02006bf4();
void Func_02006bfe();
s32 Func_02006c28();
void Func_02006c2a();
void Func_02006c36();
void Func_02006c4e();
void Func_02006c5c();
void Func_02006c62();
void Func_02006c6a();
void Func_02006c80();
void Func_02006cb2();
void Func_02006cea();
void Func_02006d00();
void Func_02006d36();
void Func_02006d42();
void Func_02006d62();
void Func_02006db2();

static __inline__ s32 Value0(s32 (*f)())
{
    return f();
}

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

static __inline__ void bump_step(s32 amount)
{
    u8 *work = *(u8 **)Data_03001ebc;

    *(u16 *)(work + 0x1d8) = (u16)(*(u16 *)(work + 0x1d8) + amount);
}

#define State_Run10 Func_02006a62
#define State_Run11 Func_02006a6c
#define State_Run12 Func_02006bf4
#define State_Run13 Func_02004f06
#define State_Run14 Func_02006a8a
#define State_Run15 Func_0200690c
#define State_Run16 Func_02006c2a
#define State_Run17 Func_02006ab0
#define State_Run18 Func_02006bf2
#define State_Run19 Func_02006bfe
#define State_Run2 Func_02006b14
#define State_Run20 Func_02002ba8
#define State_Run21 Func_02006be4
#define State_Run22 Func_02006aa6
#define State_Run23 Func_02006a56
#define State_Run24 Func_02006a5e
#define State_Run25 Func_02006bba
#define State_Run26 Func_02006bec
#define State_Run27 Func_02006d36
#define State_Run28 Func_02006a76
#define State_Run29 Func_02006c4e
#define State_Run3 Func_02006cde
#define State_Run30 Func_02006c5c
#define State_Run31 Func_02006cea
#define State_Run32 Func_02006c80
#define State_Run33 Func_02002d84
#define State_Run34 Func_02006d42
#define State_Run35 Func_02006d62
#define State_Run36 Func_02006d00
#define State_Run37 Func_02006cb2
#define State_Run38 Func_02006c62
#define State_Run39 Func_02006c6a
#define State_Run4 Func_02006b94
#define State_Run5 Func_02006b48
#define State_Run6 Func_02006b5c
#define State_Run7 Func_02006a42
#define State_Run8 Func_02006bc8
#define State_Run9 Func_02004eda
#define gCell Data_02000240
#define gCell2 Data_02000240_t
#define gOv Data_0200cac8
#define gOv2 Data_0200c0f6
#define gOv3 Data_02002090
#define gOv4 Data_0200cbfc
#define gOv5 Data_0200cc28
#define gOv6 Data_0200cca4
#define gVal Data_00000000
#define gWork Data_03001ebc
#define State_Check(a0, a1) Value2(Func_02005466, a0, a1)
#define State_Check2(a0, a1) Value2(Func_02005492, a0, a1)
#define State_Place(a0, a1, a2) Call3(Func_02006b64, a0, a1, a2)
#define State_Run() Value0(Func_02006c28)
#define State_Do(a0) Call1(Func_02006c36, a0)
#define State_Place2(a0, a1, a2) Call3(Func_02006db2, a0, a1, a2)

#endif
