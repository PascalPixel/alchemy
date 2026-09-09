#include "types.h"

#define BattleRuntime_Reset_1(args...) Func_020075d0(args)
#define ObjectMotion_PlaceWithinCameraBounds_1(a0, a1, a2, a3) Call4(Func_020076fa, a0, a1, a2, a3)
#define Object_SetModeById_1(args...) Func_02007678(args)
#define ObjectMotion_SetHorizontalPositionWithTerrain_1(a0, a1, a2) Call3(Func_0200767c, a0, a1, a2)
#define Scene_GetRecord_1(args...) Func_02007622(args)
#define RatioHistory_RecordAndScheduleCallback_1(a0, a1) Call2(Func_020077c6, a0, a1)
#define ObjectMotion_SetHorizontalPositionWithTerrain_2(args...) Func_020076a0(args)
#define ObjectMotion_SetPositionAndReset_1(args...) Func_02007730(args)
#define BattleRuntime_WaitIfModeZero_1(args...) Func_020077a0(args)
#define ObjectMotion_SetSpeedParameters_1(a0, a1, a2) Call3(Func_0200767a, a0, a1, a2)
#define ObjectMotion_SetPositionAndCommit_1(a0, a1, a2) Call3(Func_020076ae, a0, a1, a2)
#define ObjectMotion_SetPositionAndCommit_2(a0, a1, a2) Call3(Func_020076ba, a0, a1, a2)
#define ObjectMotion_SetSpeedParameters_2(a0, a1, a2) Call3(Func_0200769c, a0, a1, a2)
#define ObjectMotion_SetPositionAndCommit_3(a0, a1, a2) Call3(Func_020076d0, a0, a1, a2)
#define ObjectMotion_SetSpeedParameters_3(a0, a1, a2) Call3(Func_020076b2, a0, a1, a2)
#define ObjectMotion_SetPositionAndCommit_4(a0, a1, a2) Call3(Func_020076e6, a0, a1, a2)
#define ObjectMotion_SetSpeedParameters_4(a0, a1, a2) Call3(Func_020076c8, a0, a1, a2)
#define ObjectMotion_SetPositionAndCommit_5(a0, a1, a2) Call3(Func_020076fc, a0, a1, a2)
#define Object_SetModeById_2(args...) Func_02007734(args)
#define BattleRuntime_WaitIfModeZero_2(args...) Func_020076aa(args)
#define ObjectMotion_SetSpeedParameters_5(args...) Func_0200782a(args)
#define SCENE_PHASE (*(s32 *)(*(u8 **)0x03001ebc + 0x1c0))
#define ACTOR 8
#define FieldScene_RunScene371_02001b5c Func_02001b5c
#define FieldScene_RunScene371_02001c08 Func_02001c08
#define FieldScene_RunScene371_02002274 Func_02002274
#define FieldScene_RunScene371_0200281c Func_0200281c
#define FieldScene_RunScene371_02002858 Func_02002858
#define FieldScene_RunActorPresentationSequence Func_02002cb4
#define FieldScene_RunActorEightApproach Func_020032f0
#define FieldScene_RunScene371_0200357c Func_0200357c
#define FieldScene_RunOpeningAuxiliarySequence Func_020036dc
#define FieldScene_RunLateSequence Func_0200395c

extern u8 Data_03001ebc[];
extern u8 Data_0200e79c[];
extern u8 Data_00000000[];
extern u8 Data_02000240[];
extern u8 Data_00000800[];
extern u8 Data_00000c00[];
extern u8 Data_00000d00[];
extern u8 Data_00000e00[];
extern u8 Data_00000f00[];
extern u8 Data_00001000[];
extern u8 Data_02000434[];
extern s32 Data_02000240_t[][1];
extern u8 Data_02002090[];
extern u8 Data_04000208[];
extern u8 Data_0200e7a0[];

void Func_02005d04();
void Func_02005d26();
void Func_02005e44();
s32 Func_02005e5e();
void Func_02005e64();
void Func_02005e72();
void Func_02005ec4();
void Func_02005ec4_a();
void Func_02005ee6();
void Func_02005ef0();
void Func_02005f06();
void Func_02005f6e();
void Func_02005f8e();
void Func_02005fe8();
void Func_02005fea();
void Func_0200600a();
void Func_02006016();
void Func_02005ecc();
void Func_02005eea();
void Func_02005ef0_a();
void Func_02005f0e();
void Func_02005f10();
void Func_02005f12();
void Func_02005f5a();
void Func_02005faa();
void Func_02005fd0();
void Func_02005fd6();
void Func_02005fe8_a();
void Func_0200600c();
void Func_02006024();
void Func_02006070();
void Func_0200607a();
void Func_020060a2();
void Func_020060a2_a();
void Func_020060ae();
void Func_020060e4();
void Func_020060f4();
void Func_0200611c();
void Func_0200641c();
void Func_0200642c();
void Func_02006458();
void Func_020064d2();
void Func_0200655c();
s32 Func_02006576();
void Func_02006598();
void Func_020065f0();
void Func_020065fe();
void Func_02006612();
void Func_0200662a();
void Func_0200662a_a();
void Func_0200664a();
void Func_02006650();
void Func_02006654();
void Func_0200665e();
void Func_02006672();
void Func_02006686();
void Func_02006696();
void Func_020066a4();
void Func_020066b4();
void Func_020066c6();
void Func_020066cc();
void Func_020066d2();
void Func_020066dc();
void Func_020066ea();
void Func_020066ec();
void Func_020066f2();
void Func_020066f6();
void Func_020066fc();
void Func_02006700();
void Func_0200670a();
void Func_02006712();
void Func_02006716();
void Func_0200671c();
void Func_02006722();
void Func_0200673e();
void Func_0200674e();
void Func_02006762();
void Func_02006764();
void Func_02006778();
void Func_0200677e();
void Func_02006798();
void Func_020067dc();
void Func_02006800();
void Func_02006824();
void Func_0200683a();
void Func_02006868();
void Func_02006874();
void Func_0200689c();
void Func_02006afc();
void Func_02006b2e();
void Func_02006bb6();
void Func_02006bd4();
s32 Func_02006bee();
void Func_02006c0a();
void Func_02006b38();
void Func_02006b74();
void Func_02006b8e();
void Func_02006bd0();
void Func_02006c0a_a();
s32 Func_02006c24();
void Func_02006cbc();
void Func_02006cd8();
void Func_02006e8e();
void Func_02006f44();
void Func_02006fa8();
void Func_0200701e();
void Func_0200702c();
void Func_0200704a();
void Func_02007056();
void Func_0200705e();
void Func_02007060();
void Func_020070d6();
void Func_020070dc();
void Func_020070de();
void Func_020070e2();
void Func_020070ec();
void Func_020070f4();
void Func_02007102();
void Func_02007124();
void Func_02007134();
void Func_0200713c();
void Func_02007150();
void Func_02007150_a();
void Func_0200715a();
void Func_0200715e();
void Func_02007164();
void Func_02007164_a();
void Func_0200716c();
void Func_02007172();
void Func_02007176();
void Func_02007178();
void Func_02007192();
void Func_02007194();
void Func_02007198();
void Func_0200719e();
void Func_020071a0();
void Func_020071a0_a();
void Func_020071a0_b();
void Func_020071a2();
void Func_020071b2();
void Func_020071b4();
void Func_020071b8();
void Func_020071c2();
void Func_020071c4();
void Func_020071c6();
void Func_020071ce();
void Func_020071d4();
void Func_020071da();
void Func_020071de();
void Func_020071e2();
void Func_020071f0();
void Func_020071f4();
void Func_020071fe();
void Func_020071fe_a();
void Func_02007202();
void Func_02007212();
void Func_02007212_a();
void Func_02007218();
void Func_0200721e();
void Func_02007220();
void Func_02007228();
void Func_02007238();
void Func_02007238_a();
void Func_02007248();
void Func_0200724a();
void Func_0200724c();
void Func_02007252();
void Func_0200725c();
s32 Func_02007260();
void Func_02007264();
void Func_02007268();
void Func_0200726e();
void Func_0200726e_a();
void Func_02007278();
void Func_0200729e();
void Func_020072a8();
void Func_020072b4();
void Func_020072bc();
void Func_020072c0();
void Func_020072cc();
void Func_02007302();
void Func_0200730a();
void Func_02007314();
void Func_0200731e();
void Func_02007334();
void Func_02007344();
void Func_0200734e();
void Func_02007360();
void Func_02007366();
void Func_02007370();
void Func_02007388();
void Func_02007390();
void Func_020073b8();
void Func_020073ba();
void Func_020073c6();
void Func_020073cc();
void Func_020073dc();
void Func_020073e2();
void Func_020073fa();
void Func_020073fc();
void Func_0200740e();
void Func_02007422();
void Func_0200742c();
void Func_02007484();
void Func_0200749a();
void Func_020074a6();
void Func_020074bc();
void Func_0200751e();
void Func_02007490();
void Func_020074b6();
void Func_020074d6();
void Func_020075d0();
s32 Func_02007622();
void Func_02007678();
void Func_0200767a();
void Func_0200767c();
void Func_0200769c();
void Func_020076a0();
void Func_020076aa();
void Func_020076ae();
void Func_020076b2();
void Func_020076ba();
void Func_020076c8();
void Func_020076d0();
void Func_020076e6();
void Func_020076fa();
void Func_020076fc();
void Func_02007730();
void Func_02007734();
void Func_020077a0();
void Func_020077c6();
void Func_020077f0();
void Func_0200781e();
void Func_0200782a();
void Func_0200772e();
s32 Func_02007778();
void Func_02007802();
void Func_0200780e();
void Func_0200785e();
void Func_0200786a();
s32 Func_0200787e();
s32 Func_020078b4();
s32 Func_020078c0();
void Func_020078e0();
void Func_020078ea();
s32 Func_020078f6();
void Func_020078fc();
void Func_02007946();
void Func_02007988();
void Func_02007a22();
void Func_02007a26();
void Func_02007a4c();
void Func_02007a50();
void Func_02007a7e();
void Func_02007a8a();
void Func_02007aac();
s32 Func_020078d6();
void Func_02007986();
s32 Func_0200798c();
void Func_020079e6();
s32 Func_020079f2();
void Func_02007a16();
s32 Func_02007a1c();
void Func_02007a38();
void Func_02007a5e();
void Func_02007a74();
void Func_02007a7e_a();
void Func_02007a8c();
void Func_02007abc();
void Func_02007aee();
void Func_02007be2();
s32 Func_02007af8();
s32 Func_02007b20();
void Func_02007b9e();
s32 Func_02007bd6();
s32 Func_02007c00();
void Func_02007c0c();
void Func_02007c26();
s32 Func_02007c66();
void Func_02007c70();
void Func_02007cbc();
void Func_02007d7a();
void Func_02007dc4();
s32 Func_02007dd4();
void Func_02007dda();
void Func_02007e52();
void Func_02007e9a();
void Func_02007ee2();
void Func_02007f20();
void Func_02007f2c();
void Func_02007f40();
void Func_02007f5e();
void Func_02007f9c();
void Func_02007f9e();
void Func_02007fe2();
void Func_02008014();
void Func_02008026();
void Func_02008046();
void Func_02008078();
void Func_020080b0();
void Func_020080c0();
void Func_020080e4();
void Func_020080e8();
void Func_02008120();
void Func_02008180();
void Func_0200818c();
void Func_0200818e();
void Func_02008198();
void Func_020081a4();
void Func_02008318();
void Func_0200833a();
void Func_02008344();

/* Loader-relocated overlay calls: each symbol names the pre-relocation call
 * word the image holds. */

/* Resolved engine calls: each pseudo symbol is the per-site call word the
 * overlay image holds (a word can serve two sites with different targets),
 * and the macro names the engine function the site reaches through the
 * overlay veneer and the main-image veneer island, keeping the site's own
 * calling form. Names without a repository binding are provisional.
 */

/* Phase/status word at 0x1c0 of the shared scene work record. */

/* The actor index every call in this scene step operates on. */

/* Call sites spelled through these wrappers pass their constants straight
 * into the argument registers; a direct call precomputes a costly constant
 * into a pseudo that the compiler then shares with later uses in the block.
 * A value-returning call also sets r0 last of its arguments. */
static __inline__ void Call1(void (*f)(), s32 a0)
{
    f(a0);
}

static __inline__ s32 Value1(s32 (*f)(), s32 a0)
{
    return f(a0);
}

static __inline__ void Call3(void (*f)(), s32 a0, s32 a1, s32 a2)
{
    f(a0, a1, a2);
}

static __inline__ void Call4(void (*f)(), s32 a0, s32 a1, s32 a2, s32 a3)
{
    f(a0, a1, a2, a3);
}

/* The scene step counter at 0x1d8 of the shared scene work record. */
static __inline__ void bump_step(s32 amount)
{
    u8 *work = *(u8 **)Data_03001ebc;

    *(u16 *)(work + 0x1d8) = (u16)(*(u16 *)(work + 0x1d8) + amount);
}

static __inline__ void Call2(void (*f)(), s32 a0, s32 a1)
{
    f(a0, a1);
}

static __inline__ s32 Value2(s32 (*f)(), s32 a0, s32 a1)
{
    return f(a0, a1);
}

static __inline__ void Call11(void (*f)(), s32 a0, s32 a1, s32 a2, s32 a3, s32 a4, s32 a5, s32 a6, s32 a7, s32 a8, s32 a9, s32 a10)
{
    f(a0, a1, a2, a3, a4, a5, a6, a7, a8, a9, a10);
}

/* Call sites spelled through these wrappers pass their constants straight
 * into the argument registers; a direct call precomputes a costly constant
 * into a pseudo that the compiler then shares with later uses in the block.
 * A value-returning call also sets r0 last of its arguments. */
static __inline__ void Call2_020032f0(void (*f)(), s32 a0, s32 a1)
{
    f(a0, a1);
}

void FieldScene_RunScene371_02001b5c(void)
{
    u32 i;
    s32 rec7;
    s32 record;

    rec7 = Value1(Func_02005e5e, 8);
    Func_02005e44();
    Call4(Func_02005f6e, -1, -1, -1, 0);
    Func_02005d04(1);
    Func_02005ee6(0, 0, 0);
    Call3(Func_02005ef0, 8, 0x13e80000, 0x9180000);
    *(s32 *)(rec7 + 28) = 0x14000;
    *(s32 *)(rec7 + 24) = 0x14000;
    Func_02005d26(1);
    Func_02005f8e(8, 1);
    Func_02005fea();
    Call3(Func_02005ec4, 8, 0x6666, 0x3333);
    Call3(Func_02005f06, 8, 0x13c8, 0x918);
    Func_0200600a();
    Func_02006016();
    Call1(Func_02005e64, 0x93e);
    Call1(Func_02005e72, 0x927);
    Func_02005fe8(107);
    Func_02005ec4_a();
}

void FieldScene_RunScene371_02001c08(void)
{
    u32 i;
    s32 record;

    Func_02006070();
    Call2(Func_020060a2, 0x10000, 6);
    Func_020060ae();
    Func_0200607a();
    Func_02005faa(8, 2);
    Call1(Func_02005fd0, 0xc66);
    Func_02005fe8_a(8, 0);
    Func_02005f0e(30);
    Func_020060e4(111);
    Func_02005ecc(0, 2);
    Call1(Func_02005eea, 0x16f);
    Call1(Func_02005ef0_a, 0x171);
    Func_020060f4();
    Func_02005fd6(8, 4, 30);
    Call1(Func_0200600c, 0xc67);
    Func_02006024(8, 0);
    Call1(Func_02005f12, 0x16f);
    Call1(Func_02005f10, 0x171);
    Func_0200611c();
    Func_02005f5a(30);
    Func_020060a2_a(12, 6);
}

void FieldScene_RunScene371_02002274(void)
{
    u32 i;
    s32 rec7;
    s32 record;

    rec7 = Value1(Func_02006576, 10);
    Func_0200655c();
    Call4(Func_02006686, -1, -1, -1, 0);
    Func_0200641c(1);
    Func_020065fe(0, 0, 0);
    Func_0200642c(1);
    *(volatile s32 *)(rec7 + 24) = 0x18000;
    *(volatile s32 *)(rec7 + 28) = 0x18000;
    {
        s32 shown = 0x4000;

        *(volatile u16 *)(rec7 + 6) = shown;
    }
    Func_020066f6();
    Func_0200670a();
    Func_02006598(20);
    Call3(Func_0200662a, 10, 0x15680000, 0x8380000);
    Func_02006458(1);
    Func_0200677e(141);
    Call3(Func_020065f0, 10, 0x19999, 0x6666);
    Func_02006650(10, 2);
    Call3(Func_0200662a_a, 10, 0x156d, 0x858);
    Call2(Func_020066ea, 0x6666, 0xccc);
    Call4(Func_02006700, 0x15b80000, -1, 0x8580000, 1);
    Call3(Func_0200664a, 10, 0x159e, 0x858);
    Call3(Func_02006654, 10, 0x15a8, 0x86e);
    Call3(Func_0200665e, 10, 0x15e8, 0x878);
    Func_02006696(10, 1);
    Call1(Func_020067dc, 0x121);
    Func_02006612(20);
    Call3(Func_020066a4, 0, 0x15d80000, 0x8780000);
    Func_020064d2(1);
    Func_020066cc(0, 6, 0);
    Call3(Func_02006672, 0, 0x20000, 0x10000);
    Call3(Func_020066b4, 0, 0x15c8, 0x878);
    Func_0200673e(0, 0, 40);
    Func_02006824(141);
    Func_020066ec(10, 2);
    Call3(Func_020066c6, 10, 0x15f8, 0x878);
    Call3(Func_02006762, 0, 0xe000, 0);
    Call3(Func_020066dc, 10, 0x15f8, 0x838);
    Call3(Func_02006778, 0, 0xc000, 0);
    Call3(Func_020066f2, 10, 0x15bd, 0x838);
    Call3(Func_020066fc, 10, 0x15b8, 0x853);
    Call3(Func_02006798, 0, 0xa000, 0);
    Call3(Func_02006712, 10, 0x1572, 0x858);
    Call3(Func_0200671c, 10, 0x1568, 0x838);
    Func_0200674e(10, 0, 0);
    Call1(Func_0200689c, 0x121);
    Func_020066d2(40);
    Call4(Func_02006800, 0x15d80000, -1, 0x8580000, 1);
    Call3(Func_02006722, 0, 0xcccc, 0x6666);
    Call3(Func_02006764, 0, 0x15d8, 0x858);
    Func_02006868();
    Func_02006874();
    Func_0200683a(20);
    Func_02006716();
}

void FieldScene_RunScene371_0200281c(void)
{
    u32 i;
    s32 record;

    Func_02006afc();
    Func_02006bb6(55, 0, 0);
    Call1(Func_02006bd4, 0x2642);
    Value2(Func_02006bee, *(volatile s32 *)Data_0200e79c, 0);
    Call3(Func_02006c0a, 55, 0x3000, 0);
    Func_02006b2e();
}

void FieldScene_RunScene371_02002858(void)
{
    u32 i;
    s32 record;

    Func_02006b38();
    Func_02006cbc();
    Call1(Func_02006c0a_a, 0x2643);
    Value2(Func_02006c24, *(volatile s32 *)Data_0200e79c, 0);
    Func_02006cd8();
    Call3(Func_02006b8e, 0, 0x10000, 0x8000);
    Call3(Func_02006bd0, 0, 0x1778, 0xd48);
    Func_02006b74();
}

void FieldScene_RunActorPresentationSequence(void)
{
    u32 i;
    s32 record;
    s32 base2_2000240;
    s32 base3_2000240;

    base2_2000240 = (s32)Data_02000240;
    base3_2000240 = (s32)Data_02000240;
    Func_02006f44(*(u8 *)((base2_2000240 + 0x205)), *(u8 *)((base3_2000240 + 0x206)));
    Func_02006fa8();
    Call2(Func_02007164, 0x10000, 0x12c);
    Call4(Func_020070de, -1, -1, -1, 0);
    Func_02007056(5, 19);
    Func_0200705e(8, 5);
    Func_02007060(0, 0, 0);
    Func_02006e8e(1);
    Call2(Func_020071a0, 0x18000, 16);
    *(s32 *)((*(u8 *volatile *)Data_03001ebc + 0x1c0)) = 0x100;
    Call2(Func_02007150, 0x10003, 1);
    *(s32 *)((*(u8 *volatile *)Data_03001ebc + 0x1c8)) = 16;
    Func_02007178();
    Func_020071d4();
    Func_020071a0_a();
    Func_0200701e(40);
    Func_020070d6(5, 1);
    Func_0200702c(20);
    Call1(Func_02007102, 0x2913);
    Func_02007124(5, 0, 20);
    Func_020070f4(8, 2);
    Func_0200704a(20);
    Func_0200713c(8, 0, 20);
    Call3(Func_0200715e, 5, 0x107, 20);
    Func_02007150_a(5, 0, 20);
    Call3(Func_02007172, 8, 0x105, 80);
    Func_02007164_a(8, 0, 10);
    Func_02007134(5, 2);
    Func_02007176(5, 0, 20);
    Call3(Func_02007198, 8, 0x105, 100);
    Call3(Func_020071a2, 5, 0x105, 40);
    Func_02007194(5, 0, 20);
    Func_0200719e(8, 0, 10);
    Call3(Func_020071c2, 5, 0x102, 20);
    Func_020071b4(5, 0, 10);
    Call2(Func_020071de, 8, 0x102);
    Func_020070dc(80);
    Func_020071ce(8, 0, 20);
    Call3(Func_020071f0, 5, 0x105, 80);
    Func_020071e2(8, 0, 120);
    Func_020071b2(5, 1);
    Func_020071f4(5, 0, 40);
    Func_020071fe(8, 0, 20);
    Call3(Func_02007220, 5, 0x105, 40);
    Func_02007212(5, 0, 120);
    Call3(Func_0200716c, 9, 0x6666, 0x3333);
    Call3(Func_020071c6, 9, 0x1ddc0000, 0xd840000);
    Call3(Func_020071b8, 9, 0x1d94, 0xd8c);
    Call3(Func_020071c4, 9, 0x1d88, 0xda0);
    Func_0200715a(20);
    Call3(Func_0200724c, 0x6009, 0, 20);
    Call3(Func_0200726e, 8, 0x101, 0);
    Call3(Func_02007278, 5, 0x101, 60);
    Func_02007218(9, 3);
    Call2(Func_02007268, 0x6009, 0);
    Func_020070ec();
    Func_02007192(20);
    Call2(Func_020071da, 9, 0x200cf7c);
    Func_020071a0_b(80);
    Func_02007238(8, 1);
    Func_02007252(8, 4, 40);
    Func_0200724a(5, 1);
    Func_02007264(5, 4, 60);
    Call3(Func_020072c0, 8, 0x3000, 0);
    Call3(Func_020072cc, 5, 0xb000, 40);
    Call3(Func_0200721e, 8, 0x9999, 0x4ccc);
    Call3(Func_02007228, 5, 0x9999, 0x4ccc);
    Call2(Func_02007238_a, 8, 0x200d01c);
    Func_020071fe_a(20);
    Call2(Func_0200731e, 0xb333, 0x1666);
    Call4(Func_02007334, 0x1e380000, -1, 0xdc80000, 1);
    Func_0200725c(5, 0x200d0a8);
    do {
        Func_020072b4(10, 6);
        Func_020072bc(6, 8);
        Func_020070e2(1);
    } while (*(s16 *)(Func_02007260(5) + 100) == 0);
    Func_02007248(20);
    Call3(Func_02007344, 9, 0x8000, 20);
    Call2(Func_02007366, 8, 0x102);
    Call2(Func_02007370, 5, 0x102);
    Func_0200726e_a(40);
    Func_020073fa();
    Call11(Func_02007390, 5, 7, 13, 2, 12, 8, 9, 4, 4, 3, 0);
    Func_0200729e(20);
    Func_02007202();
    Call2(Func_020073c6, 0x10000, 0x2000);
    Call4(Func_020073dc, 0x1e580000, -1, 0xdc80000, 1);
    Call3(Func_020073b8, 9, 0x3000, 0);
    Call3(Func_0200730a, 8, 0x19999, 0xcccc);
    Call3(Func_02007314, 5, 0x19999, 0xcccc);
    Call3(Func_0200734e, 8, 0x1e7c, 0xdb8);
    Call3(Func_02007360, 5, 0x1e6c, 0xdd8);
    Func_02007388(8, 1);
    Func_02007484();
    Func_02007302(80);
    Func_020073ba(8, 1);
    Func_020073fc(8, 0, 20);
    Func_020073cc(5, 2);
    Call3(Func_0200740e, 0x1005, 0, 40);
    Call3(Func_02007422, 8, 0x8000, 20);
    Func_020073e2(8, 2);
    Func_0200742c(8, 0, 60);
    Func_020072a8();
    Func_0200751e(17);
    Func_020074a6(0, 0);
    Func_020074bc(120);
    Func_02007212_a(120);
    Func_0200749a((s32)Data_00000000, 10);
}

/* Drives actor 8 through a series of position/threshold setup calls and
 * advances the shared scene phase before the scene runs. */
void FieldScene_RunActorEightApproach(void)
{
    u32 i;
    s32 actor;

    BattleRuntime_Reset_1();
    ObjectMotion_PlaceWithinCameraBounds_1(-1, -1, -1, 0);
    Func_02007490(1);
    Object_SetModeById_1(ACTOR, 2);
    ObjectMotion_SetHorizontalPositionWithTerrain_1(ACTOR, 0x13080000, 0x3280000);
    actor = Scene_GetRecord_1(ACTOR);
    {
        /* Write 0xa000 to the halfword at +6 of the actor record. */
        s32 value = 0xa000;

        *(u16 *)(actor + 6) = value;
    }
    Func_020074b6(1);
    RatioHistory_RecordAndScheduleCallback_1(0x13333, 1);
    ObjectMotion_SetHorizontalPositionWithTerrain_2(0, 0, 0);
    ObjectMotion_SetPositionAndReset_1(ACTOR, 1);
    Func_020074d6(1);
    SCENE_PHASE = 0x100;
    BattleRuntime_WaitIfModeZero_1();
    ObjectMotion_SetSpeedParameters_1(ACTOR, 0x6666, 0x3333);
    ObjectMotion_SetPositionAndCommit_1(ACTOR, 0x12d8, 0x2c8);
    ObjectMotion_SetPositionAndCommit_2(ACTOR, 0x12a8, 0x268);
    ObjectMotion_SetSpeedParameters_2(ACTOR, 0x4ccc, 0x2666);
    ObjectMotion_SetPositionAndCommit_3(ACTOR, 0x12a8, 0x1d8);
    ObjectMotion_SetSpeedParameters_3(ACTOR, 0x3333, 0x1999);
    ObjectMotion_SetPositionAndCommit_4(ACTOR, 0x1298, 0x1c8);
    ObjectMotion_SetSpeedParameters_4(ACTOR, 0x1999, 0xccc);
    ObjectMotion_SetPositionAndCommit_5(ACTOR, 0x1298, 0x1b8);
    Object_SetModeById_2(ACTOR, 1);
    BattleRuntime_WaitIfModeZero_2(40);
    Func_0200781e();
    ObjectMotion_SetSpeedParameters_5();
    Func_020077f0(110);
}

void FieldScene_RunScene371_0200357c(void)
{
    u32 i;
    s32 rec7;
    s32 record;

    rec7 = Value1(Func_0200787e, 8);
    Func_0200785e(60);
    Func_0200786a();
    Call2(Func_02007a22, 0x9999, 1);
    *(s32 *)(rec7 + 24) = 0x13333;
    *(s32 *)(rec7 + 28) = 0x13333;
    Func_02007988(8, 1);
    Func_0200772e(1);
    Func_02007946(0, 15);
    record = Func_020078b4(0);
    Func_02007802(record, 0);
    record = Func_020078c0(8);
    Func_0200780e(record, 0);
    Call3(Func_020078e0, 8, 0x6666, 0x3333);
    {
        s32 shown = 0;

        *(u16 *)(rec7 + 100) = shown;
    }
    Value2(Func_020078f6, 8, 0x200d218);
    Value2(Func_02007778, 0x200b4a1, 0xc80);
    *(s32 *)((*(u8 *volatile *)Data_03001ebc + 0x1c0)) = 0x100;
    Call2(Func_02007a26, 0x10003, 1);
    *(s32 *)(*(u8 *volatile *)Data_03001ebc + 0x1c8) = 32;
    Func_02007a4c();
    Func_020078ea(120);
    Call2(Func_02007aac, 0x16666, 0x12c);
    Call1(Func_020078fc, 0x10e);
    *(s32 *)(*(u8 *volatile *)Data_03001ebc + 0x1c8) = 16;
    *(u16 *)0x05000000 = 0x7fff;
    Func_02007a7e();
    Func_02007a8a();
    Func_02007a50(111);
}

void FieldScene_RunOpeningAuxiliarySequence(s32 a0)
{
    s32 rec2;
    u8 *rec7;
    s32 record;
    u8 *p6;
    u8 *base;

    base = Data_02000240;
    p6 = *(volatile s32 *)(base + 500);
    rec7 = Value1(Func_020079f2, (s32)p6);
    rec2 = Value1(Func_0200798c, 0x2f0);
    if (rec2 == 0) {
        Func_020079e6();
        Call2(Func_02007aee, (s32)p6, 0x101);
        Func_02007a7e_a((s32)p6, 9);
        record = Value1(Func_02007a1c, a0);
        if (record != 0) {
            Func_02007a5e((s32)p6, *(s16 *)(record + 10), *(s16 *)(record + 18));
        }
        Func_02007a8c((s32)p6);
        Func_02007be2(244);
        Value2(Func_020078d6, 0x200b679, 0xc80);
        rec7[85] = rec2;
        {
            s32 shifted = *(volatile s32 *)((s32)rec7 + 12) + 0x200000;

            Func_02007986((s32)rec7, *(volatile s32 *)((s32)rec7 + 8), shifted, *(volatile s32 *)((s32)rec7 + 16));
        }
        Func_02007abc((s32)p6);
        *(volatile s32 *)((s32)rec7 + 40) = rec2;
        rec7[85] = 4;
        *(u8 *)(base + 498) = 2;
        Call1(Func_02007a16, 0x2f0);
        Call2(Func_02007a38, 0x2f8, 180);
        Func_02007a74();
        *(u16 *)((*(u8 *volatile *)Data_03001ebc + 0x17c)) = rec2;
    }
}

void FieldScene_RunLateSequence(void)
{
    s32 record;
    s32 sx;
    s32 sy;
    u32 mode;

    record = Func_02007c66(Data_02000240_t[125][0]);
    sx = *(s16 *)(record + 10);
    sy = *(s16 *)(record + 18);
    if (Value2(Func_02007af8, *(volatile s32 *)0x03001e40, 3) == 0) {
        mode = (u32)(Func_02007b20() << 2) >> 16;
        switch (mode) {
        case 0:
            Call4(Func_02007dc4, (sx << 16) - 0x10000, -1, (sy << 16) + 0x10000, 1);
            break;
        case 1:
            Func_02007dc4((sx << 16) + 0x10000, -1, (sy << 16) - 0x10000, 1);
            break;
        case 2:
            Call4(Func_02007dc4, (sx << 16) + 0x10000, -1, (sy << 16) + 0x10000, 1);
            break;
        case 3:
            Call4(Func_02007dda, (sx << 16) - 0x10000, -1, (sy << 16) - 0x10000, 1);
            break;
        }
    }
}
