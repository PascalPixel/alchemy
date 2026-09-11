#ifndef ALCHEMY_ABI_OVERLAYS_SCENE_EFFECT_PALETTE
#define ALCHEMY_ABI_OVERLAYS_SCENE_EFFECT_PALETTE

/* Binding layer — not production source. Address / far-call ABI only. */

void Func_020019f0();
void Func_02001a14();
void Func_02001ae0(s16);
void Func_02001b0a(s32, s32, s32);
void Func_02001be4(void);
s32 Func_02001bd2(s32);
void Func_02001c7c(s32);
void Func_02001c84(s32);
void Func_02001c9c(s32, s32);
void Func_02001c10(void);
void Func_02001c20(void);
s32 Func_02001c0e(s32);
void Func_02001cb8(s32);
void Func_02001cc0(s32);
void Func_02001cd8(s32, s32);
void Func_02001c4c(void);
void Func_02001c5c(void);
void Func_02001d58(void);
s32 Func_02001c4e(s32);
void Func_020005c2(void);
void Func_02000764(void);
void Func_02001c80(void);
void Func_020012b4(void);
void Func_020015a0(void);
s32 Func_02001980();
void Func_020019e0();
void Func_02001a40();
void Func_02001ab0();
void Func_02001960();
void Func_020019b8();
void Func_020019e8();
void Func_020019c0();
void Func_020019c8();
void Func_02001990();
void Func_020018b8();
void Func_02001a88();
void Func_02001908();
s32 Func_02001910();
void Func_02001a58();
void Func_02001a60();
void Func_02001a68();
s32 Func_02001918();
void Func_020012f4();
void Func_02001a20();
void Func_02001a30();
void Func_02001a08();
void Func_02001a10();
void Func_02001a50();
void Func_02001a38();
s32 Func_02001a28();
s32 Func_020019a8();
void Func_020019f0_a();
void Func_020019f8();
void Func_02001a78();
void Func_020019d0();
s32 Func_02001988();
void Func_0200172c();
void Func_02002b52(struct PaletteEffectFrame *frame);
void Func_02002ce2(s32);
void Func_02002bb4(struct PaletteEffect *, s32);
void Func_02002be4(struct PaletteEffect *, s32, s32, s32);
void Func_02002bd4(struct PaletteEffect *, u8 *);
void Func_02002b7a(s32);
void Func_02002d50(s32);
void Func_02002b8c(s32);
void Func_02002d62(s32);
void Func_02002e82();
void Func_02002eb4();
void Func_02002eda();
s32 Func_020030b0();
s32 Func_020030b8();
void Func_02002fa6();
u16 Func_02002f6c(u16, s32);
void Func_0200302a();
void Func_0200300e();
void Func_02003238();
s32 Func_020030ea();
s32 Func_020030f8();
s32 Func_02003106();
void Func_02001462();
void Func_02001478();
void Func_0200148c();
s32 Func_020014a2();
s32 Func_02001ae2();
void Func_02001af4();
void Func_02001af6();
s32 Func_02001b0c();
void Func_02001b26();
s32 Func_02001b2c();
void Func_02001b94();
void Func_02001bac();
void Func_02001bbe();
void Func_02001bc2();
void Func_02001bd6();
void Func_02001bf2();
void Func_02001c0a();
void Func_02001c10_a();
void Func_02001c30();
void Func_02001c38();
void Func_02001c5a();
void Func_02001c70();
void Func_02001c8a();
void Func_02001ca0();
void Func_02001c1c();
void Func_02001c38_a();
void Func_02001c3a();
s32 Func_02001ca2();
void Func_02001cac();
void Func_02001cb8_a();
void Func_02001ce2();
void Func_02001cea();
s32 Func_02001d04();
void Func_02001d2c();
void Func_02001d3a();
s32 Func_02001d40();
void Func_02001d56();
void Func_02001d5a();
void Func_02001d70();
void Func_02001d80();
void Func_02001d82();
void Func_02001d96();
void Func_02001d96_a();
void Func_02001da0();
void Func_02001da6();
void Func_02001db4();
void Func_02001dca();
void Func_02001dde();
void Func_02001df6();
void Func_02001e18();
void Func_02001e1a();
void Func_02001e2a();
void Func_02001e3a();
void Func_02001e48();
void Func_02001e50();
void Func_02001e58();
void Func_02001e58_a();
void Func_02001e6c();
void Func_02001e9e();
void Func_02001eb4();
void Func_02001ebc();

static __inline__ void Call1(void (*f)(), s32 a0)
{
    extern s32 Data_02009dd0;

    f(a0);
}

static __inline__ s32 Value1(s32 (*f)(), s32 a0)
{
    extern s32 Data_02009dd0;

    return f(a0);
}

static __inline__ void Call2(void (*f)(), s32 a0, s32 a1)
{
    extern s32 Data_02009dd0;

    f(a0, a1);
}

static __inline__ s32 Value2(s32 (*f)(), s32 a0, s32 a1)
{
    extern s32 Data_02009dd0;

    return f(a0, a1);
}

static __inline__ void Call3(void (*f)(), s32 a0, s32 a1, s32 a2)
{
    extern s32 Data_02009dd0;

    f(a0, a1, a2);
}

static __inline__ void Call4(void (*f)(), s32 a0, s32 a1, s32 a2, s32 a3)
{
    extern s32 Data_02009dd0;

    f(a0, a1, a2, a3);
}

static __inline__ void Call1_02000158(void (*f)(), s32 a0)
{
    f(a0);
}

static __inline__ void bump_step(s32 amount)
{
    u8 *work = *(u8 **)Data_03001ebc;

    *(u16 *)(work + 0x1d8) = (u16)(*(u16 *)(work + 0x1d8) + amount);
}

static __inline__ void Call1_020002ec(void (*f)(), s32 a0)
{
    extern u8 Data_02009dd0[];

    f(a0);
}

static __inline__ s32 Value3(s32 (*f)(), s32 a0, s32 a1, s32 a2)
{
    extern u8 Data_02009dd0[];

    return f(a0, a1, a2);
}

#define Effect_Apply Func_02001c9c
#define Effect_Apply2 Func_02001cd8
#define Effect_Apply3 Func_02002bb4
#define Effect_Apply4 Func_02002bd4
#define Effect_Apply5 Func_02002f6c
#define Effect_Check10 Func_02001bd2
#define Effect_Check11 Func_02001c0e
#define Effect_Check12 Func_02001c4e
#define Effect_Check13 Func_02001ca2
#define Effect_Check14 Func_02001d04
#define Effect_Check15 Func_02001980
#define Effect_Check16 Func_02001918
#define Effect_Check17 Func_020030b0
#define Effect_Check18 Func_020030b8
#define Effect_Check19 Func_020030ea
#define Effect_Check20 Func_020030f8
#define Effect_Check21 Func_02003106
#define Effect_Check9 Func_02001b2c
#define Effect_Do10 Func_02001cb8
#define Effect_Do11 Func_02001cc0
#define Effect_Do12 Func_02002b52
#define Effect_Do13 Func_02002ce2
#define Effect_Do14 Func_02002b7a
#define Effect_Do15 Func_02002d50
#define Effect_Do16 Func_02002b8c
#define Effect_Do17 Func_02002d62
#define Effect_Do7 Func_02001ae0
#define Effect_Do8 Func_02001c7c
#define Effect_Do9 Func_02001c84
#define Effect_Place11 Func_02001b0a
#define Effect_Run15 Func_020019b0
#define Effect_Run16 Func_02002b84
#define Effect_Run17 Func_020030bc
#define Effect_Run18 Func_02001d92
#define Effect_Run19 Func_020019f0
#define Effect_Run20 Func_02001a14
#define Effect_Run21 Func_02001af4
#define Effect_Run22 Func_02001462
#define Effect_Run23 Func_02001bac
#define Effect_Run24 Func_02001478
#define Effect_Run25 Func_0200148c
#define Effect_Run26 Func_02001bd6
#define Effect_Run27 Func_02001c0a
#define Effect_Run28 Func_02001c70
#define Effect_Run29 Func_02001af6
#define Effect_Run30 Func_02001c10_a
#define Effect_Run31 Func_02001ca0
#define Effect_Run32 Func_02001b26
#define Effect_Run33 Func_02001bc2
#define Effect_Run34 Func_02001be4
#define Effect_Run35 Func_02001c10
#define Effect_Run36 Func_02001c20
#define Effect_Run37 Func_02001c4c
#define Effect_Run38 Func_02001c5c
#define Effect_Run39 Func_02001d58
#define Effect_Run40 Func_020005c2
#define Effect_Run41 Func_02000764
#define Effect_Run42 Func_02001c80
#define Effect_Run43 Func_02001d96
#define Effect_Run44 Func_02001c1c
#define Effect_Run45 Func_02001dca
#define Effect_Run46 Func_02001c38_a
#define Effect_Run47 Func_02001d96_a
#define Effect_Run48 Func_02001d56
#define Effect_Run49 Func_02001e2a
#define Effect_Run50 Func_02001cac
#define Effect_Run51 Func_02001cb8_a
#define Effect_Run52 Func_02001e6c
#define Effect_Run53 Func_02001db4
#define Effect_Run54 Func_02001d5a
#define Effect_Run55 Func_02001ce2
#define Effect_Run56 Func_02001d82
#define Effect_Run57 Func_02001e1a
#define Effect_Run58 Func_02001da0
#define Effect_Run59 Func_02001ebc
#define Effect_Run60 Func_02001eb4
#define Effect_Run61 Func_02001d3a
#define Effect_Run62 Func_02001e58
#define Effect_Run63 Func_02001dde
#define Effect_Run64 Func_02001e58_a
#define Effect_Run65 Func_02001e50
#define Effect_Run66 Func_02001df6
#define Effect_Run67 Func_02001ab0
#define Effect_Run68 Func_020019e8
#define Effect_Run69 Func_020019c0
#define Effect_Run70 Func_020019c8
#define Effect_Run71 Func_02001990
#define Effect_Run72 Func_020018b8
#define Effect_Run73 Func_02001a88
#define Effect_Run74 Func_02001908
#define Effect_Run75 Func_02001a68
#define Effect_Run76 Func_020012f4
#define Effect_Run77 Func_02001a30
#define Effect_Run78 Func_02001a08
#define Effect_Run79 Func_02001a10
#define Effect_Run80 Func_020019f0_a
#define Effect_Run81 Func_020019f8
#define Effect_Run82 Func_020015a0
#define Effect_Run83 Func_02001a38
#define Effect_Run84 Func_02001a40
#define Effect_Run85 Func_02002e82
#define Effect_Run86 Func_02002eb4
#define Effect_Run87 Func_02002eda
#define Effect_Run88 Func_02002fa6
#define Effect_Run89 Func_0200302a
#define Effect_Run90 Func_0200300e
#define Effect_Run91 Func_02003238
#define Effect_SetMode Func_02002be4
#define PaletteScene_AdjustColor Func_020017d0
#define PaletteScene_AdjustPaletteWindow Func_02001768
#define PaletteScene_AdvanceEffectFrame Func_020011e8
#define PaletteScene_AdvanceOrbit Func_0200152c
#define PaletteScene_AdvanceTransition Func_020012b4
#define PaletteScene_GetActorData Func_02000040
#define PaletteScene_GetEffectData Func_02000048
#define PaletteScene_GetMessageData Func_02000038
#define PaletteScene_GetScriptData Func_02000030
#define PaletteScene_GetState Func_02000080
#define PaletteScene_Initialize Func_02000050
#define PaletteScene_RunActorEightBranch Func_02000284
#define PaletteScene_RunActorNineBranch Func_02000248
#define PaletteScene_RunActorTransitionSequence Func_02000488
#define PaletteScene_RunFlaggedBranch Func_020002c0
#define PaletteScene_SetRecordValue Func_0200172c
#define PaletteScene_SpawnEffect Func_02001218
#define RunEventScript01 Func_020002ec
#define Scene_RunScene395 Func_02000158
#define gIw Data_03001e40
#define gOv Data_02009dd4
#define gOv2 Data_02009dcc
#define gOv3 Data_02009dc0
#define gOv4 Data_02009d9c
#define gOv5 Data_02009dd0
#define gWork Data_03001ebc
#define Effect_Check(a0) Value1(Func_02001ae2, a0)
#define Effect_Do(a0) Call1_02000158(Func_02001b94, a0)
#define Effect_Check2(a0) Value1(Func_02001b0c, a0)
#define Effect_Do2(a0) Call1_02000158(Func_02001bbe, a0)
#define Effect_Check3(a0, a1) Value2(Func_020014a2, a0, a1)
#define Effect_Do3(a0) Call1_02000158(Func_02001bf2, a0)
#define Effect_Run(a0, a1) Call2(Func_02001c5a, a0, a1)
#define Effect_Place(a0, a1, a2) Call3(Func_02001c30, a0, a1, a2)
#define Effect_Run2(a0, a1) Call2(Func_02001c38, a0, a1)
#define Effect_Run3(a0, a1) Call2(Func_02001c8a, a0, a1)
#define Effect_Place2(a0, a1, a2) Value3(Func_02001d40, a0, a1, a2)
#define Effect_Run4(a0, a1) Call2(Func_02001d80, a0, a1)
#define Effect_Run5(a0, a1) Call2(Func_02001c3a, a0, a1)
#define Effect_Run6(a0, a1, a2, a3) Call4(Func_02001da6, a0, a1, a2, a3)
#define Effect_Place3(a0, a1, a2) Call3(Func_02001d2c, a0, a1, a2)
#define Effect_Place4(a0, a1, a2) Call3(Func_02001d70, a0, a1, a2)
#define Effect_Do4(a0) Call1_020002ec(Func_02001cea, a0)
#define Effect_Do5(a0) Call1_020002ec(Func_02001e18, a0)
#define Effect_Place5(a0, a1, a2) Call3(Func_02001e3a, a0, a1, a2)
#define Effect_Run7(a0, a1) Call2(Func_02001e48, a0, a1)
#define Effect_Run8(a0, a1) Call2(Func_02001e9e, a0, a1)
#define Effect_Place6(a0, a1, a2) Call3(Func_020019e0, a0, a1, a2)
#define Effect_Place7(a0, a1, a2) Call3(Func_02001a40, a0, a1, a2)
#define Effect_Run9(a0, a1) Call2(Func_02001960, a0, a1)
#define Effect_Place8(a0, a1, a2) Call3(Func_020019b8, a0, a1, a2)
#define Effect_Check4(a0, a1) Value2(Func_02001910, a0, a1)
#define Effect_Run10(a0, a1) Call2(Func_02001a58, a0, a1)
#define Effect_Run11(a0, a1, a2, a3) Call4(Func_02001a60, a0, a1, a2, a3)
#define Effect_Check5(a0) Value1(Func_02001918, a0)
#define Effect_Do6(a0) Call1(Func_02001a20, a0)
#define Effect_Place9(a0, a1, a2) Call3(Func_02001a50, a0, a1, a2)
#define Effect_Place10(a0, a1, a2) Call3(Func_02001a38, a0, a1, a2)
#define Effect_Run12(a0, a1) Call2(Func_02001a30, a0, a1)
#define Effect_Check6(a0, a1) Value2(Func_02001a28, a0, a1)
#define Effect_Check7(a0, a1) Value2(Func_020019a8, a0, a1)
#define Effect_Run13(a0, a1) Call2(Func_02001a78, a0, a1)
#define Effect_Run14(a0, a1) Call2(Func_020019d0, a0, a1)
#define Effect_Check8(a0) Value1(Func_02001988, a0)

#endif
