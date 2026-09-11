#ifndef ALCHEMY_ABI_MAP_LOCATIONS_COLOSSO_LOG_ROLLING_STAGE_EVENT_SEQ_A
#define ALCHEMY_ABI_MAP_LOCATIONS_COLOSSO_LOG_ROLLING_STAGE_EVENT_SEQ_A

/* Binding layer — not production source. Address / far-call ABI only. */

void Func_0200640a();
void Func_02006436();
s32 Func_02006996();
s32 Func_020069c2();
void Func_02007e44();
void Func_02007fa2();
void Func_02006a56();
void Func_02006a5e();
void Func_02007fc2();
void Func_02007fcc();
void Func_02006a76();
void Func_02007fea();
void Func_02006aa6();
void Func_02008010();
void Func_020080d0();
void Func_020080e4();
void Func_020080dc();
void Func_0200811c();
void Func_02006bba();
void Func_02008150();
void Func_02006be4();
void Func_02006bec();
void Func_0200817a();
void Func_0200817c();
void Func_02008186();
s32 Func_020081b0();
void Func_020081b2();
void Func_020081be();
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
void Func_02002ba8();
void Func_02002d84();

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

#define Map_Run10 Func_02007fc2
#define Map_Run11 Func_02007fcc
#define Map_Run12 Func_0200817c
#define Map_Run13 Func_02006436
#define Map_Run14 Func_02007fea
#define Map_Run15 Func_02007e44
#define Map_Run16 Func_020081b2
#define Map_Run17 Func_02008010
#define Map_Run18 Func_0200817a
#define Map_Run19 Func_02008186
#define Map_Run2 Func_02006b14
#define Map_Run20 Func_02002ba8
#define Map_Run21 Func_02006be4
#define Map_Run22 Func_02006aa6
#define Map_Run23 Func_02006a56
#define Map_Run24 Func_02006a5e
#define Map_Run25 Func_02006bba
#define Map_Run26 Func_02006bec
#define Map_Run27 Func_02006d36
#define Map_Run28 Func_02006a76
#define Map_Run29 Func_02006c4e
#define Map_Run3 Func_02006cde
#define Map_Run30 Func_02006c5c
#define Map_Run31 Func_02006cea
#define Map_Run32 Func_02006c80
#define Map_Run33 Func_02002d84
#define Map_Run34 Func_02006d42
#define Map_Run35 Func_02006d62
#define Map_Run36 Func_02006d00
#define Map_Run37 Func_02006cb2
#define Map_Run38 Func_02006c62
#define Map_Run39 Func_02006c6a
#define Map_Run4 Func_0200811c
#define Map_Run5 Func_020080d0
#define Map_Run6 Func_020080e4
#define Map_Run7 Func_02007fa2
#define Map_Run8 Func_02008150
#define Map_Run9 Func_0200640a
#define gCell Data_02000240
#define gCell2 Data_02000240_t
#define gOv Data_0200da50
#define gOv2 Data_0200cce2
#define gOv3 Data_02002090
#define gOv4 Data_0200cbfc
#define gOv5 Data_0200cc28
#define gOv6 Data_0200cca4
#define gVal Data_00000000
#define gWork Data_03001ebc
#define Map_Check(a0, a1) Value2(Func_02006996, a0, a1)
#define Map_Check2(a0, a1) Value2(Func_020069c2, a0, a1)
#define Map_Place(a0, a1, a2) Call3(Func_020080dc, a0, a1, a2)
#define Map_Run() Value0(Func_020081b0)
#define Map_Do(a0) Call1(Func_020081be, a0)
#define Map_Place2(a0, a1, a2) Call3(Func_02006db2, a0, a1, a2)

#endif
