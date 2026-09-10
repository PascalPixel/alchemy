#include "types.h"

#define Scene_GetRecord_1(args...) Func_0200568a(args)
#define GameFlag_IsSet_1(a0) Value1(Func_0200565a, a0)
#define BattleRuntime_Reset_1(args...) Func_02005684(args)
#define ObjectMotion_PlaceWithinCameraBounds_1(a0, a1, a2, a3) Call4(Func_0200578a, a0, a1, a2, a3)
#define ObjectMotion_SetHorizontalPositionWithTerrain_1(args...) Func_02005712(args)
#define ObjectGroup_ConfigureChildValue_1(args...) Func_0200575a(args)
#define Scene_GetRecord_2(args...) Func_020056d8(args)
#define BattleRuntime_WaitIfModeZero_1(args...) Func_020057fa(args)
#define ObjectMotion_SetSpeedParameters_1(args...) Func_0200580e(args)
#define Audio_PlayCue_1(args...) Func_0200583c(args)
#define ObjectMotion_SetSpeedParameters_2(a0, a1, a2) Call3(Func_02005702, a0, a1, a2)
#define ObjectMotion_CommitPositionAndActivate_1(args...) Func_0200583c_a(args)
#define ObjectGroup_ConfigureChildValue_2(args...) Func_02005794(args)
#define Scene_GetRecord_3(args...) Func_02005712_a(args)
#define ObjectMotion_CommitPositionAndActivate_2(args...) Func_0200585a(args)
#define BattleEffect_PlayQueuedSound_1(args...) Func_02005874(args)
#define BattleRuntime_ScheduleShoulderButtonModeUpdate_1(args...) Func_02005720(args)
#define BattleRuntime_Reset_1_02002f30(args...) Func_02007dc8(args)
#define ObjectMotion_SetSpeedParameters_1_02002f30(a0, a1, a2) Call3(Func_02007df6, a0, a1, a2)
#define ObjectMotion_SetPositionAndReset_1(a0, a1, a2) Call3(Func_02007e2c, a0, a1, a2)
#define ObjectMotion_ArmCallback_1(a0, a1, a2) Call3(Func_02007eb8, a0, a1, a2)
#define BattleEffect_RunRisingObjectSequence_1(args...) Func_02007e92(args)
#define BattleRuntime_WaitIfModeZero_1_02002f30(args...) Func_02007e08(args)
#define BattleRuntime_ScheduleShoulderButtonModeUpdate_1_02002f30(args...) Func_02007e22(args)
#define FieldScene_RunOpeningAuxiliarySequence Func_020007d8
#define FieldScene_RunSupplementalSequenceOne Func_020009c8
#define FieldScene_RunActorEightTenStepLoop Func_02000f38
#define FieldScene_RunScene3c8SequenceC Func_02001150
#define FieldScene_RunScene3c8SequenceD Func_02001218
#define FieldScene_RunFlag986ActorOneScene Func_020014f4
#define FieldScene_RunScene3c8SequenceB Func_02002f30
#define FieldScene_RunStep230With44 Func_02004600
#define FieldScene_RunScene3c8_02004a2c Func_02004a2c

typedef struct {
    u8 filler0[8];
    s32 unk8;
    u8 fillerC[4];
    s32 unk10;
} Struct_A;

typedef struct {
    u8 filler0[10];
    s16 unkA;
    u8 fillerC[6];
    s16 unk12;
} Struct_B;

extern u8 Data_03001ebc[];
extern u8 Value_00000986;

s32 Func_0200565a();
void Func_02005676();
void Func_02005684();
u8 *Func_0200568a();
void Func_020056b0();
u8 *Func_020056d8();
void Func_02005702();
u8 *Func_02005712();
void Func_02005720();
void Func_0200575a();
void Func_0200578a();
void Func_02005794();
void Func_020057fa();
void Func_0200580e();
void Func_0200583c();
void Func_0200583c_a();
void Func_0200585a();
void Func_02005874();
u8 *Func_02005712_a();
void Func_02000b9a();
void Func_0200579c();
void Func_020057e6();
s32 Func_02005812();
s32 Func_02005816();
void Func_02005870();
s32 Func_02005882();
void Func_020058a8();
void Func_020058b8();
void Func_02005906();
void Func_02005914();
void Func_02005920();
void Func_02005972();
void Func_0200597e();
void Func_0200599e();
void Func_020059d6();
void Func_020059ea();
void Func_02005a0e();
void Func_02005a1e();
void Func_02005a42();
void Func_02005a8e();
void Func_02005ac6();
void Func_02005dd2(void);
void Func_02005e5a(s32, s32);
void Func_02005e90(s32);
void Func_02005eae(s32, s32, s32);
void Func_02005e96(s32, s32);
void Func_02005d0c(s32);
void Func_02005ea4(s32, s32);
void Func_02005d1a(s32);
void Func_02005e04(s32);
void Func_02005e86(s32, s32, s32);
void Func_02005dda(s32, s32, s32, s32, s32, s32);
void Func_02005e46(void);
void Func_02005ff0();
s32 Func_02006002();
void Func_02006020();
void Func_0200602e();
void Func_02006036();
s32 Func_0200603c();
void Func_0200604e();
s32 Func_02006050();
void Func_02006064();
u8 *Func_02006074();
void Func_02006098();
void Func_020060a4();
void Func_020060a6();
void Func_020060fc();
void Func_0200610c();
void Func_0200612c();
void Func_02006154();
void Func_020060ae();
void Func_020060c6();
void Func_020060ec();
void Func_020060f2();
void Func_020060f8();
s32 Func_020060fa();
s32 Func_02006104();
void Func_02006106();
void Func_0200610c_a();
void Func_02006116();
void Func_02006120();
s32 Func_02006126();
s32 Func_02006126_a();
void Func_02006128();
void Func_02006136();
void Func_02006136_a();
void Func_02006138();
void Func_02006146();
void Func_02006146_a();
void Func_02006156();
void Func_02006166();
void Func_02006176();
void Func_02006186();
void Func_02006196();
void Func_020061a6();
void Func_020061c8();
void Func_020061dc();
void Func_020061ec();
void Func_020061fc();
void Func_02006200();
void Func_02006200_a();
void Func_0200620c();
void Func_0200620e();
void Func_02006216();
void Func_0200621c();
void Func_0200621c_a();
void Func_02006220();
void Func_0200622c();
void Func_02006236();
void Func_0200623c();
void Func_0200624c();
void Func_0200625c();
void Func_02006262();
void Func_0200626c();
void Func_0200627c();
void Func_02006302();
void Func_02006328();
void Func_02006350();
void Func_020063fe();
void Func_02006416();
void Func_02006416_a();
void Func_02006422();
void Func_0200642c();
void Func_02006430();
void Func_0200643c();
void Func_0200638c(void);
Struct_A *Func_020063aa(s32);
s32 Func_02006382();
void Func_02006394();
Struct_A *Func_020063ca(s32);
void Func_02006420(s32, s32, s32);
void Func_020063ea();
void Func_020063ea_a();
void Func_0200641e(s32, s32, s32);
void Func_0200642a(s32, s32, s32);
void Func_02006484(s32, s32, s32);
void Func_02006472(s32, s32);
void Func_020063f8(s32);
void Func_020064b6(s32);
void Func_020064d0(s32, s32, s32);
void Func_020064e2(s32, s32, s32);
void Func_020064fe(s32, s32, s32);
void Func_020064c0(s32, s32, s32);
void Func_0200642e(s32);
void Func_020064be(s32, s32);
void Func_020064be_a(s32, s32);
void Func_0200643c_a(s32);
void Func_0200650e(s32, s32, s32);
void Func_020064d6(s32, s32);
void Func_0200645c(s32);
void Func_020064b8(s32, s32, s32);
void Func_020064e0(s32, s32);
Struct_B *Func_02006496(s32);
void Func_020064c8(s32, s32, s32);
void Func_020064ee(s32);
void Func_02006500(s32, s32, s32);
void Func_020064ac(void);
s32 Func_02002fc4();
void Func_02007dc8();
void Func_02007df6();
void Func_02007e08();
void Func_02007e22();
void Func_02007e2c();
void Func_02007e92();
void Func_02007eb8();
void Func_02007f26();
void Func_02008b2a();
void Func_02009808();
void Func_02009852();
void Func_0200987a();
void Func_02009894();
void Func_020098b0();
void Func_020098c8();
void Func_020098dc();
s32 Func_020098e6();
s32 Func_020098ee();
void Func_02009920();
void Func_0200992c();
void Func_0200994e();
void Func_020099ac();
void Func_020099de();
void Func_020099ec();
void Func_020099f6();
void Func_02009a0a();
void Func_02009a1c();
void Func_02009a48();
void Func_02009a56();
void Func_02009a5c();
void Func_02009a8a();
void Func_02009ac6();
void Func_02009ad6();
void Func_02009ade();
void Func_02009b5c();
void Func_02009b7c();
void Func_02009b84();

/* Loader-relocated overlay calls: each symbol names the pre-relocation call
 * word the image holds. */

/* Resolved engine calls: each pseudo symbol is the per-site call word the
 * overlay image holds (a word can serve two sites with different targets),
 * and the macro names the engine function the site reaches through the
 * overlay veneer and the main-image veneer island, keeping the site's own
 * calling form. Names without a repository binding are provisional.
 */

/* Call sites spelled through these wrappers pass their constants straight
 * into the argument registers; a direct call precomputes a costly constant
 * into a pseudo that the compiler then shares with later uses in the block.
 * A value-returning call also sets r0 last of its arguments. */
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

static __inline__ void Call8(void (*f)(), s32 a0, s32 a1, s32 a2, s32 a3, s32 a4, s32 a5, s32 a6, s32 a7)
{
    f(a0, a1, a2, a3, a4, a5, a6, a7);
}

/* Call sites spelled through these wrappers pass their constants straight
 * into the argument registers; a direct call precomputes a costly constant
 * into a pseudo that the compiler then shares with later uses in the block.
 * A value-returning call also sets r0 last of its arguments. */
static __inline__ void Call1(void (*f)(), s32 a0)
{
    f(a0);
}

static __inline__ s32 Value1_02001218(s32 (*f)(), s32 a0)
{
    return f(a0);
}

static __inline__ void Call6(void (*f)(), s32 a0, s32 a1, s32 a2, s32 a3, s32 a4, s32 a5)
{
    f(a0, a1, a2, a3, a4, a5);
}

/* Call sites spelled through these wrappers pass their constants straight
 * into the argument registers; a direct call precomputes a costly constant
 * into a pseudo that the compiler then shares with later uses in the block.
 * A value-returning call also sets r0 last of its arguments. */
static __inline__ void Call3_02002f30(void (*f)(), s32 a0, s32 a1, s32 a2)
{
    f(a0, a1, a2);
}

static __inline__ s32 Value1_02004a2c(s32 (*f)(), s32 a0)
{
    return f(a0);
}

/* Runs a guarded one-shot setup on an entry record: only fires the first
 * time (while a global 0x109 lookup is still unset), positions the entry
 * from its own stored coordinates, drives an effect/param sequence, then
 * writes a stage byte and an override field on the entry before returning. */
void FieldScene_RunOpeningAuxiliarySequence(void)
{
    u32 i;
    u8 *entry;
    u8 *guard;
    u8 *sub;

    entry = Scene_GetRecord_1(0);
    guard = GameFlag_IsSet_1(0x109);
    if (guard == 0) {
        BattleRuntime_Reset_1();
        ObjectMotion_PlaceWithinCameraBounds_1(-1, -1, -1, 0);
        /* Stage byte at +85 of the entry record. */
        entry[85] = guard;
        /* Position, from the entry's own s16 coordinates at +10/+18
         * (converted to 16.16 fixed point; the y term is offset by -16.0). */
        ObjectMotion_SetHorizontalPositionWithTerrain_1(0, (*(s16 *)(entry + 10) << 16), ((*(s16 *)(entry + 18) << 16) + -0x100000));
        ObjectGroup_ConfigureChildValue_1(0, 15);
        sub = Scene_GetRecord_2(0);
        Func_02005676(sub, 0);
        BattleRuntime_WaitIfModeZero_1();
        ObjectMotion_SetSpeedParameters_1();
        Audio_PlayCue_1(228);
        /* Override field at +108 of the entry record; holds an EWRAM
         * address while the effect sequence below runs. */
        *(s32 *)(entry + 108) = 0x20086a1;
        ObjectMotion_SetSpeedParameters_2(0, 0x6666, 0x3333);
        ObjectMotion_CommitPositionAndActivate_1(0, 0, 8);
        ObjectGroup_ConfigureChildValue_2(0, 0);
        sub = Scene_GetRecord_3(0);
        Func_020056b0(sub, 1);
        ObjectMotion_CommitPositionAndActivate_2(0, 0, 8);
        entry[85] = 3;
        /* Restore the +108 override field to the original (unset) value. */
        *(s32 *)(entry + 108) = guard;
        BattleEffect_PlayQueuedSound_1();
        BattleRuntime_ScheduleShoulderButtonModeUpdate_1();
    }
}

void FieldScene_RunSupplementalSequenceOne(s32 a0)
{
    s32 rec;
    s32 v8;
    s32 sh;
    s32 record;
    s32 half;
    s32 w0;
    s32 w1;
    s32 w2;
    s32 w4;
    u8 *p10;
    u8 *p16;
    u8 slot28[40];
    u8 slot16[12];

    rec = Value1(Func_02005882, 0);
    Func_02005870();
    Call4(Func_02005972, -1, -1, -1, 0);
    Func_020057e6();
    Func_0200579c(1);
    *(volatile s32 *)(rec + 12) = 0x820000;
    *(volatile s32 *)(rec + 72) = 0x8000;
    *(volatile s32 *)(rec + 68) = 0;
    *(volatile u8 *)(rec + 85) = 0;
    Func_020059d6();
    Func_020059ea();
    Func_020058a8(30);
    Func_02005a1e(204);
    *(volatile u8 *)(rec + 85) = 3;
    Func_020058b8(24);
    p10 = slot28;
    *(s32 *)(p10 + 4) = 7;
    *(s32 *)(p10 + 36) = 0x200896d;
    *(s32 *)(p10 + 8) = 0xcccc;
    *(s32 *)(p10 + 12) = 0xcccc;
    v8 = 0;
    p16 = slot16;
    do {
        sh = v8 << 12;
        *(s32 *)(p16) = Func_02005812(sh);
        *(s32 *)(p16 + 4) = 0;
        record = Func_02005816(sh);
        *(s32 *)(p16 + 8) = record;
        half = *(s32 *)p16;
        *(s32 *)(p16) = half + half / 2;
        w0 = *(volatile s32 *)(rec + 8);
        w2 = *(volatile s32 *)(rec + 16);
        w1 = *(volatile s32 *)(rec + 12);
        w4 = *(volatile s32 *)(p16 + 4);
        Func_02000b9a(w0, w1, w2, (*(s32 *)p16), w4, record, 0x1090001, (s32)p10);
        v8 = (v8 + 1);
    } while ((u32)v8 <= 16);
    Func_02005a8e(188);
    Call2(Func_02005a0e, 0, 0x101);
    Func_0200599e(0, 22);
    Call3(Func_02005906, 0x50000, 0x50000, 0x10000);
    Call3(Func_02005914, -1, -1, 0xe666);
    Func_02005920();
    Call2(Func_02005a42, 0, 0x100);
    Func_02005ac6();
    *(volatile s32 *)(rec + 72) = 0x10000;
    *(volatile s32 *)(rec + 68) = 0x4000;
    Func_0200597e();
}

void FieldScene_RunActorEightTenStepLoop(void)
{
    u32 n;
    u32 w;
    s32 a;
    s32 b;

    Func_02005dd2();
    Func_02005e5a(8, 3);
    Func_02005e90(0x266d);
    n = 10;
    w = 8;
    Func_02005eae(8, 0, 20);
    do {
        Func_02005e96(8, 15);
        Func_02005d0c(2);
        Func_02005ea4(8, 0);
        Func_02005d1a(w);
        if (w > 3) {
            w--;
        }
        n--;
    } while (n != 0);
    Func_02005e04(0x981);
    Func_02005e86(8, 0, 0);
    a = 7;
    b = 16;
    Func_02005dda(7, 17, 2, 1, a, b);
    Func_02005e46();
}

void FieldScene_RunScene3c8SequenceC(void)
{
    u32 i;
    s32 rec7;
    u8 *record;
    s32 v2;

    rec7 = Value1(Func_02006002, 0);
    Func_02005ff0();
    Call2(Func_02006020, 0, 0x200d21c);
    Func_0200602e(0);
    Func_020060a6(0, 6);
    *(volatile s32 *)(rec7 + 40) = 0x40000;
    Call3(Func_02006036, 0, 0x40000, 0x20000);
    if ((*(volatile s32 *)(rec7 + 16) >> 20) <= 54) {
        *(u8 *)(Func_0200603c(0) + 90) &= 254;
        v2 = 210;
    } else {
        *(u8 *)(Func_02006050(0) + 90) &= 254;
        v2 = 238;
    }
    Func_02006098(0, *(s16 *)(rec7 + 10), (v2 << 2));
    Func_0200604e(1);
    {
        u8 *record = Func_02006074(0);
        u8 value = *(volatile u8 *)&record[90];

        record[90] = (u8)(value | 1);
    }
    Func_02006064(20);
    *(volatile s32 *)(rec7 + 108) = 0x20085e5;
    Call3(Func_02006154, 0, 0x102, 60);
    Func_020060fc(0, 4);
    Func_0200612c(0, 0);
    Func_0200610c(0, 4);
    *(volatile s32 *)(rec7 + 108) = 0;
    Func_020060a4();
}

void FieldScene_RunScene3c8SequenceD(void)
{
    u32 i;
    s32 record;
    u8 *p7;

    p7 = *(volatile s32 *)Data_03001ebc;
    {
        volatile u16 *target = (volatile u16 *)((s32)p7 + 0xcba);
        s32 shown = 0;

        *target = shown;
    }
    {
        s32 shown = 1;

        *(volatile u16 *)(((s32)p7 + 0xcb6)) = shown;
    }
    Func_020060c6();
    Func_02006136(0, 1);
    Call2(Func_020060ae, 0x2688, 1);
    Call2(Func_02006200, 0x10000, 0);
    Call2(Func_02006200_a, 0x10005, 0);
    Func_02006216(120);
    Func_020060ec(100);
    Func_02006262(142);
    Func_020060f8(30);
    Call2(Func_02006220, 0x7fff, 0);
    Func_02006236(60);
    Func_0200610c_a(70);
    if (Value1_02001218(Func_020060fa, 0x982) == 0) {
        if (Value1_02001218(Func_02006104, 0x983) == 0) {
            if ((*(volatile s32 *)0x03001e40 & 1) != 0) {
                Call1(Func_02006120, 0x982);
            } else {
                Call1(Func_02006128, 0x983);
            }
        }
    }
    if (Value1_02001218(Func_02006126, 0x982) == 0) {
        Call1(Func_02006138, 0x982);
        Call1(Func_02006146, 0x983);
        Call6(Func_020060f2, 103, 27, 89, 27, 7, 8);
        Call6(Func_02006106, 41, 90, 27, 92, 3, 2);
        Call6(Func_02006116, 41, 90, 29, 93, 3, 2);
        Call6(Func_02006126_a, 41, 90, 27, 94, 3, 2);
        Call6(Func_02006136_a, 41, 90, 27, 96, 3, 2);
        Call6(Func_02006146_a, 41, 90, 29, 97, 3, 2);
        Call6(Func_02006156, 41, 96, 25, 91, 3, 2);
        Call6(Func_02006166, 41, 92, 25, 93, 3, 2);
        Call6(Func_02006176, 41, 96, 25, 95, 3, 2);
        Call6(Func_02006186, 41, 96, 25, 97, 3, 2);
        Call6(Func_02006196, 41, 96, 27, 96, 3, 2);
        Call6(Func_020061a6, 41, 96, 29, 97, 3, 2);
    } else {
        Call1(Func_0200620e, 0x983);
        Call1(Func_0200621c, 0x982);
        Call6(Func_020061c8, 111, 27, 89, 27, 7, 8);
        Call6(Func_020061dc, 41, 90, 25, 91, 3, 2);
        Call6(Func_020061ec, 41, 90, 25, 93, 3, 2);
        Call6(Func_020061fc, 41, 90, 25, 95, 3, 2);
        Call6(Func_0200620c, 41, 90, 25, 97, 3, 2);
        Call6(Func_0200621c_a, 41, 90, 27, 96, 3, 2);
        Call6(Func_0200622c, 41, 90, 29, 97, 3, 2);
        Call6(Func_0200623c, 41, 94, 27, 92, 3, 2);
        Call6(Func_0200624c, 41, 96, 29, 93, 3, 2);
        Call6(Func_0200625c, 41, 94, 27, 94, 3, 2);
        Call6(Func_0200626c, 41, 96, 27, 96, 3, 2);
        Call6(Func_0200627c, 41, 96, 29, 97, 3, 2);
    }
    Call2(Func_02006416, 0x10000, 0);
    Func_0200642c(20);
    Func_02006302(40);
    Call2(Func_020063fe, 0x8000, 0x1000);
    Call4(Func_02006416_a, 0x1c80000, -1, 0x21e0000, 1);
    Func_02006422();
    Func_02006328(50);
    Call4(Func_02006430, 0x1c80000, -1, 0x1a70000, 1);
    Func_0200643c();
    Func_02006350();
    {
        volatile u16 *target = (volatile u16 *)((s32)p7 + 0xcb6);
        s32 shown = 0;

        *target = shown;
    }
}

void FieldScene_RunFlag986ActorOneScene(void)
{
    Struct_A *o;
    Struct_B *u;
    s32 g;
    s32 m1;
    s32 m2;
    s32 h;
    s32 k;

    g = 0x986;
    m1 = 0xcccc;
    m2 = 0x6666;
    h = 0x100;
    k = 0x338;
    Func_0200638c();
    o = Func_020063aa(12);
    if (o->unk8 >> 20 == 53) {
        if (Func_02006382(g) == 0) {
            Func_02006394(g);
            o = Func_020063ca(0);
            if (o != 0) {
                Func_02006420(1, o->unk8, o->unk10);
            }
            Func_020063ea(1, m1, m2);
            Func_0200641e(1, k, 88);
            Func_0200642a(1, k, 104);
            Func_02006484(1, 0, 0);
            Func_020063ea_a(20);
            Func_02006472(1, 4);
            Func_020063f8(20);
            Func_020064b6(0x2691);
            Func_020064d0(1, 0, 20);
            Func_020064e2(1, 0, 10);
            Func_020064fe(1, h, 60);
            Func_020064c0(1, 0, 0);
            Func_0200642e(20);
            Func_020064be(1, 2);
            Func_0200643c_a(20);
            Func_0200650e(1, 0, 20);
            Func_020064be_a(0, 3);
            Func_020064d6(1, 3);
            Func_0200645c(30);
            Func_020064b8(1, k, 88);
            Func_020064e0(1, 2);
            u = Func_02006496(0);
            if (u != 0) {
                Func_020064c8(1, u->unkA, u->unk12);
            }
            Func_020064ee(1);
            Func_02006500(1, 0, 0);
            Func_020064ac();
        }
    }
}

/* Runs a fixed sequence of setup calls with literal parameters; most share
 * a leading 0 argument. */
void FieldScene_RunScene3c8SequenceB(void)
{
    BattleRuntime_Reset_1_02002f30();
    ObjectMotion_SetSpeedParameters_1_02002f30(0, 0x8000, 0x4000);
    ObjectMotion_SetPositionAndReset_1(0, 0x208, 0x2c8);
    ObjectMotion_ArmCallback_1(0, 0x4000, 10);
    (void)Func_02002fc4(0x2080000, 0, 0x3100000, 223);
    BattleEffect_RunRisingObjectSequence_1(0, 6, 0);
    BattleRuntime_WaitIfModeZero_1_02002f30(60);
    Func_02007f26(20); /* main:0808a248 */
    BattleRuntime_ScheduleShoulderButtonModeUpdate_1_02002f30();
}

void FieldScene_RunStep230With44(void)
{
    Func_02008b2a(0x230, 44);
}

void FieldScene_RunScene3c8_02004a2c(void)
{
    u32 i;
    u8 *p8;
    s32 rec;
    u8 *rec8;
    s32 record;
    s32 none;
    s32 v2;
    s32 slot0;

    rec = Value1_02004a2c(Func_020098e6, 0);
    rec8 = Value1_02004a2c(Func_020098ee, 20);
    Func_020098dc();
    Call4(Func_020099de, -1, -1, -1, 0);
    Func_02009852();
    Func_02009808(1);
    *(s32 *)(rec + 12) = 0x820000;
    *(s32 *)(rec + 72) = 0x8000;
    none = 0;
    *(s32 *)(rec + 68) = none;
    p8 = rec + 85;
    *p8 = none;
    Func_02009a48();
    Func_02009a5c();
    Func_02009a8a(204);
    Func_02009920(30);
    *p8 = 3;
    Func_0200992c(24);
    Call2(Func_02009a1c, 0, 0x101);
    Func_020099ac(0, 22);
    *p8 &= 254;
    *(s32 *)((s32)rec8 + 12) += -0x30000;
    *(s32 *)(rec + 12) += -0x30000;
    *(s32 *)(rec + 20) += -0x30000;
    Func_0200987a(2);
    *(s32 *)((s32)rec8 + 12) += -0x20000;
    *(s32 *)(rec + 12) += -0x20000;
    *(s32 *)(rec + 20) += -0x20000;
    Func_02009894(10);
    *(s32 *)((s32)rec8 + 12) += 0x20000;
    *(s32 *)(rec + 12) += 0x20000;
    *(s32 *)(rec + 20) += 0x20000;
    Func_020098b0(4);
    *(s32 *)((s32)rec8 + 12) += 0x20000;
    *(s32 *)(rec + 12) += 0x20000;
    *(s32 *)(rec + 20) += 0x20000;
    Func_020098c8(4);
    *(s32 *)((s32)rec8 + 12) += 0x10000;
    *(s32 *)(rec + 12) += 0x10000;
    *(s32 *)(rec + 20) += 0x10000;
    *p8 = none;
    rec8[85] = none;
    Call2(Func_02009ac6, 0, 0x100);
    Func_02009a56(0, 1);
    Func_020099ec(40);
    *(s32 *)(rec + 108) = 0x200c969;
    Func_020099f6(60);
    Func_02009ad6(0, 1);
    Func_02009ade(20, 1);
    Func_02009b7c(17);
    Call1(Func_02009b84, 0x134);
    Call1(Func_02009a0a, 0x101);
    v2 = 0;
    do {
        *(s32 *)(rec + 12) += 0x10000;
        *(s32 *)(rec + 20) += 0x10000;
        *(s32 *)((s32)rec8 + 12) += 0x10000;
        slot0 = v2;
        Func_0200994e(1);
        v2 = slot0;
        v2 = (v2 + 1);
    } while ((u32)v2 <= 127);
    Func_02009b5c(21);
}
