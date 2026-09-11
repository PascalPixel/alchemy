#include "types.h"

#define BattleRuntime_Reset_1(args...) Func_0200813c(args)
#define ObjectMotion_SetSpeedParameters_1(a0, a1, a2) Call3(Func_0200816e, a0, a1, a2)
#define ObjectMotion_SetSpeedParameters_2(a0, a1, a2) Call3(Func_02008178, a0, a1, a2)
#define SceneWork_SetStepValue_1(a0) Call1(Func_0200821e, a0)
#define BattleRuntime_RunThenWaitIfModeZero_1(args...) Func_02008238(args)
#define ObjectMotion_SetPositionAndReset_1(a0, a1, a2) Call3(Func_020081c6, a0, a1, a2)
#define BattleEffect_SpawnLinkedResourceObject_1(a0, a1, a2) Call3(Func_0200826a, a0, a1, a2)
#define ObjectMotion_ArmCallback_1(a0, a1, a2) Call3(Func_02008266, a0, a1, a2)
#define Object_SetModeById_1(args...) Func_02008206(args)
#define Scene_GetRecord_1(args...) Func_020081b4(args)
#define BattleRuntime_WaitIfModeZero_1(args...) Func_02008196(args)
#define BattleRuntime_RunThenWaitIfModeZero_2(args...) Func_02008280(args)
#define Object_SetModeById_2(args...) Func_02008228(args)
#define Scene_GetRecord_2(args...) Func_020081d6(args)
#define BattleRuntime_WaitIfModeZero_2(args...) Func_020081ba(args)
#define ObjectMotion_ArmCallback_2(a0, a1, a2) Call3(Func_020082ae, a0, a1, a2)
#define ObjectMotion_SetSpeedParameters_3(a0, a1) Value2(Func_020082a6, a0, a1)
#define UiWork_WaitThenFinalizeCapacity_1(a0, a1) Value2(Func_020081f6, a0, a1)
#define BattleRuntime_WaitIfModeZero_3(args...) Func_020081e0(args)
#define ObjectMotion_CallThenWaitForAnimationChange_1(args...) Func_02008270_a(args)
#define BattleRuntime_RunThenWaitIfModeZero_3(args...) Func_020082d2(args)
#define BattleRuntime_WaitIfModeZero_4(args...) Func_0200821a(args)
#define ObjectMotion_CallThenWaitForAnimationChange_2(args...) Func_020082aa(args)
#define BattleRuntime_RunThenWaitIfModeZero_4(args...) Func_0200830c(args)
#define BattleRuntime_WaitIfModeZero_5(args...) Func_02008232(args)
#define ObjectMotion_ArmCallback_3(a0, a1, a2) Call3(Func_02008326, a0, a1, a2)
#define ObjectMotion_SetSpeedLimitAndAcceleration_1(a0, a1) Call2(Func_02008346, a0, a1)
#define ObjectMotion_PlaceWithinCameraBounds_1(a0, a1, a2, a3) Call4(Func_02008360, a0, a1, a2, a3)
#define ObjectMotion_SetPositionAndReset_2(a0, a1, a2) Call3(Func_020082c6, a0, a1, a2)
#define Object_CommitPositionThenWaitIfModeZero_1(args...) Func_0200837a(args)
#define BattleRuntime_WaitIfModeZero_6(args...) Func_02008270_b(args)
#define ObjectMotion_SetVariantCallbackAndInvokeObject_1(args...) Func_02008318(args)
#define BattleRuntime_WaitIfModeZero_7(args...) Func_0200827e_a(args)
#define ObjectMotion_CallThenWaitForAnimationChange_3(args...) Func_0200830e(args)
#define BattleRuntime_WaitIfModeZero_8(args...) Func_0200828c(args)
#define BattleRuntime_RunThenWaitIfModeZero_5(args...) Func_02008376(args)
#define GameFlag_Set_1(a0) Call1(Func_0200827e_b, a0)
#define BattleRuntime_ScheduleShoulderButtonModeUpdate_1(args...) Func_020082b2(args)
#define SCENE_WORK (*(u8 **)0x03001ebc)
#define FX16_0_1 0x1999
#define FX16_0_25 0x4000
#define FX16_0_4 0x6666
#define FX16_0_5 0x8000
#define FX16_0_75 0xc000
#define FX16_0_8 0xcccc
#define FX16_1_0 0x10000
#define FieldScene_RunScene39b_02001208 Func_02003788
#define FieldScene_RunFourSteps Func_02001bd8
#define FieldScene_RunScene39cSequenceA Func_02001d3c
#define FieldScene_RunActorThreeBranchSequence Func_02002684
#define FieldScene_RunScriptedSteps0And1576 Func_020035b8
#define FieldScene_RunScriptedSteps0And953 Func_020035dc
#define FieldScene_RunFlag881Dialogue Func_02003600
#define FieldScene_RunActor184Sequence Func_02003660
#define FieldScene_RunScene39cSequenceB Func_020036ac
#define FieldScene_RunScene39c_02004f44 Func_02004f44

extern u8 Data_00000036[];
extern u8 Data_02000240[];
extern s16 Data_02000240_t[][1];

void Func_02007690(void);
void Func_02001f88(void);
void Func_020076a0(void);
void Func_020037d8(void);
void Func_02007790();
void Func_02007798();
s32 Func_020077c8();
void Func_020077e6();
void Func_02007800();
void Func_02007826();
s32 Func_0200782c();
s32 Func_0200783c();
s32 Func_02007844();
void Func_0200785c();
void Func_020079f6();
void Func_0200813c();
void Func_0200816e();
void Func_02008178();
void Func_02008196();
s32 Func_020081b4();
void Func_020081ba();
void Func_020081c6();
s32 Func_020081d6();
void Func_020081e0();
s32 Func_020081f6();
void Func_02008206();
void Func_0200821a();
void Func_0200821e();
void Func_02008228();
void Func_02008232();
void Func_02008238();
void Func_02008266();
void Func_0200826a();
void Func_02008270_a();
void Func_02008270_b();
void Func_0200827e_a();
void Func_0200827e_b();
void Func_02008280();
void Func_0200828c();
s32 Func_020082a6();
void Func_020082aa();
void Func_020082ae();
void Func_020082b2();
void Func_020082c6();
void Func_020082d2();
void Func_0200830c();
void Func_0200830e();
void Func_02008318();
void Func_02008326();
void Func_02008346();
void Func_02008360();
void Func_02008376();
void Func_0200837a();
void Func_02009070(void);
void Func_020090f0(s32, s32);
void Func_02009018(s32, s32);
void Func_0200908c(void);
void Func_02009094(void);
void Func_02009114(s32, s32);
void Func_0200903c(s32, s32);
void Func_020090b0(void);
void Func_020090b8(void);
void Func_02009138(s32, s32);
s32 Func_02009096(s32);
void Func_0200906a(s32, s32);
void Func_02009074(s32, s32);
s32 Func_020090aa(s32);
void Func_02009104(void);
void Func_02009118(void);
void Func_020092ce(s32);
void Func_0200927e(s32, s32);
void Func_0200860a(s32, s32);
s32 Func_020090fc(s32);
void Func_020090da(s32, s32);
void Func_020090e2_a(s32, s32);
void Func_020090e2_b(s32, s32);
void Func_0200912a(s32);
void Func_0200915e(void);
s32 Func_02003756();
void Func_02009126();
void Func_02009128();
void Func_02009166();
s32 Func_020091ca();
s32 Func_020091d6();
void Func_020091de();
s32 Func_020091fc();
void Func_02009202();
void Func_0200922a();
void Func_02009272();
void Func_02009296();
void Func_020092a4();
void Func_020092be();
void Func_020092d2();
void Func_02009328();
s32 Func_0200921a();
void Func_0200920e();
s32 Func_02009262();
void Func_02009254();
void Func_02009248();
s32 Func_020092b0();
void Func_020092da();
s32 Func_020092f2();
s32 Func_020092ea();
void Func_02009300();
void Func_02009342();
void Func_0200936a();
void Func_0200937c();
void Func_020093da();
void Func_020093ee();
void Func_02009454_a();
void Func_02009454_b();
void Func_02009482();
void Func_0200949c();
void Func_0200aa7a();
void Func_0200ab8a();
void Func_0200ab9a();
void Func_0200aba0();
void Func_0200abba();
void Func_0200abbc();

/* Loader-relocated overlay calls: each symbol names the pre-relocation call
 * word the image holds. */

/* Resolved engine calls: each pseudo symbol is the per-site call word the
 * overlay image holds (a word can serve two sites with different targets),
 * and the macro names the engine function the site reaches through the
 * overlay veneer and the main-image veneer island, keeping the site's own
 * calling form. Names without a repository binding are provisional.
 */

/* Pointer to the shared scene work record. */

/* Values below that look like 16.16 fixed-point fractions, named by that
 * fraction's decimal value. */

/* Call sites spelled through these wrappers pass their constants straight
 * into the argument registers; a direct call precomputes a costly constant
 * into a pseudo that the compiler then shares with later uses in the block.
 * A value-returning call also sets r0 last of its arguments. */
static __inline__ void Call1(void (*f)(), s32 a0)
{
    extern u8 Data_03001ebc[];

    f(a0);
}

static __inline__ s32 Value1(s32 (*f)(), s32 a0)
{
    extern u8 Data_03001ebc[];

    return f(a0);
}

static __inline__ void Call3(void (*f)(), s32 a0, s32 a1, s32 a2)
{
    extern u8 Data_03001ebc[];

    f(a0, a1, a2);
}

static __inline__ void Call6(void (*f)(), s32 a0, s32 a1, s32 a2, s32 a3, s32 a4, s32 a5)
{
    extern u8 Data_03001ebc[];

    f(a0, a1, a2, a3, a4, a5);
}

/* The scene step counter at 0x1d8 of the shared scene work record. */
static __inline__ void bump_step(s32 amount)
{
    extern u8 Data_03001ebc[];

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

static __inline__ void Call4(void (*f)(), s32 a0, s32 a1, s32 a2, s32 a3)
{
    f(a0, a1, a2, a3);
}

/* The scene step counter at 0x1d8 of the shared scene work record. */
static __inline__ void bump_step_02002684(s32 amount)
{
    u8 *work = SCENE_WORK;

    *(u16 *)(work + 0x1d8) = (u16)(*(u16 *)(work + 0x1d8) + amount);
}

/* Call sites spelled through these wrappers pass their constants straight
 * into the argument registers; a direct call precomputes a costly constant
 * into a pseudo that the compiler then shares with later uses in the block.
 * A value-returning call also sets r0 last of its arguments. */
static __inline__ void Call3_020036ac(void (*f)(), s32 a0, s32 a1, s32 a2)
{
    extern u8 Data_03001ebc[];

    f(a0, a1, a2);
}

/* Call sites spelled through these wrappers pass their constants straight
 * into the argument registers; a direct call precomputes a costly constant
 * into a pseudo that the compiler then shares with later uses in the block.
 * A value-returning call also sets r0 last of its arguments. */
static __inline__ s32 Value1_02003788(s32 (*f)(), s32 a0)
{
    extern u8 Data_03001ebc[];

    return f(a0);
}

static __inline__ s32 Value3(s32 (*f)(), s32 a0, s32 a1, s32 a2)
{
    extern u8 Data_03001ebc[];

    return f(a0, a1, a2);
}

/* Call sites spelled through these wrappers pass their constants straight
 * into the argument registers; a direct call precomputes a costly constant
 * into a pseudo that the compiler then shares with later uses in the block.
 * A value-returning call also sets r0 last of its arguments. */
static __inline__ void Call3_02004f44(void (*f)(), s32 a0, s32 a1, s32 a2)
{
    f(a0, a1, a2);
}

void FieldScene_RunFourSteps(void)
{
    Func_02007690();
    Func_02001f88();
    Func_020076a0();
    Func_020037d8();
}

void FieldScene_RunScene39cSequenceA(void)
{
    extern u8 Data_03001ebc[];

    u32 i;
    s32 rec7;
    s32 record;

    if (Value1(Func_020077c8, 0x256) != 0) {
        Func_02007800();
        Call1(Func_020077e6, 0x256);
        record = Func_0200782c(0);
        *(volatile s32 *)(record + 12) += 0x20000;
        rec7 = Value1(Func_0200783c, 0);
        record = Value1(Func_02007844, 0);
        *(volatile s32 *)(rec7 + 60) = *(volatile s32 *)(record + 12);
        Func_02007826(5);
        Call6(Func_02007790, 8, 29, 10, 23, 1, 1);
        Func_020079f6(217);
        Call3(Func_02007798, 0x200e08e, 10, 18);
        Func_0200785c();
    }
}

/* Sets up actor 3 and actor 20, branches on the result of a query call
 * against actor 0, then runs a shared closing sequence over both actors. */
void FieldScene_RunActorThreeBranchSequence(void)
{
    u32 i;
    s32 record;

    BattleRuntime_Reset_1();
    ObjectMotion_SetSpeedParameters_1(3, FX16_0_8, FX16_0_4);
    ObjectMotion_SetSpeedParameters_2(0, FX16_0_8, FX16_0_4);
    SceneWork_SetStepValue_1(0x1577);
    BattleRuntime_RunThenWaitIfModeZero_1(3, 0, 20);
    ObjectMotion_SetPositionAndReset_1(3, 0x348, 0x288);
    BattleEffect_SpawnLinkedResourceObject_1(3, 0x100, 60);
    ObjectMotion_ArmCallback_1(3, FX16_0_5, 20);
    Object_SetModeById_1(3, 16);
    record = Scene_GetRecord_1(3);
    /* Set the +24 field of actor 3's record to -1.0 in 16.16 fixed point. */
    *(s32 *)(record + 24) = -FX16_1_0;
    BattleRuntime_WaitIfModeZero_1(20);
    BattleRuntime_RunThenWaitIfModeZero_2(3, 0, 20);
    Object_SetModeById_2(3, 1);
    record = Scene_GetRecord_2(3);
    /* Set the +24 field of actor 3's record to 1.0 in 16.16 fixed point. */
    *(s32 *)(record + 24) = FX16_1_0;
    BattleRuntime_WaitIfModeZero_2(20);
    ObjectMotion_ArmCallback_2(3, FX16_0_25, 20);
    ObjectMotion_SetSpeedParameters_3(3, 0);
    if (UiWork_WaitThenFinalizeCapacity_1(0, 0) == 0) {
        BattleRuntime_WaitIfModeZero_3(20);
        ObjectMotion_CallThenWaitForAnimationChange_1(3, 3);
        BattleRuntime_RunThenWaitIfModeZero_3(3, 0, 20);
        *(u16 *)((SCENE_WORK + 0x1d8)) += 1;
    } else {
        *(u16 *)((SCENE_WORK + 0x1d8)) += 1;
        BattleRuntime_WaitIfModeZero_4(20);
        ObjectMotion_CallThenWaitForAnimationChange_2(3, 4);
        BattleRuntime_RunThenWaitIfModeZero_4(3, 0, 20);
    }
    BattleRuntime_WaitIfModeZero_5(20);
    ObjectMotion_ArmCallback_3(3, FX16_0_75, 20);
    ObjectMotion_SetSpeedLimitAndAcceleration_1(FX16_0_8, FX16_0_1);
    ObjectMotion_PlaceWithinCameraBounds_1(0x3480000, -1, 0x2780000, 1);
    ObjectMotion_SetPositionAndReset_2(3, 0x348, 0x278);
    Object_CommitPositionThenWaitIfModeZero_1();
    BattleRuntime_WaitIfModeZero_6(20);
    ObjectMotion_SetVariantCallbackAndInvokeObject_1(3, 2);
    BattleRuntime_WaitIfModeZero_7(10);
    ObjectMotion_CallThenWaitForAnimationChange_3(3, 4);
    BattleRuntime_WaitIfModeZero_8(20);
    BattleRuntime_RunThenWaitIfModeZero_5(3, 0, 20);
    GameFlag_Set_1(0x870);
    BattleRuntime_ScheduleShoulderButtonModeUpdate_1();
}

void FieldScene_RunScriptedSteps0And1576(void)
{
    Func_02009070();
    Func_020090f0(0, 1);
    Func_02009018(0x1576, 1);
    Func_0200908c();
}

void FieldScene_RunScriptedSteps0And953(void)
{
    Func_02009094();
    Func_02009114(0, 1);
    Func_0200903c(0x953, 1);
    Func_020090b0();
}

void FieldScene_RunFlag881Dialogue(void)
{
    extern u8 *Data_03001ebc;

    Func_020090b8();
    Func_02009138(0, 1);
    if (Func_02009096(0x881) == 0)
        Func_0200906a(0x1636, 1);
    else
        Func_02009074(0x1635, 1);
    if (Func_020090aa(0xb9) != -1) {
        s16 *slot = (s16 *)Data_03001ebc + 185;
        s32 one = 1;

        *slot = one;
    }
    Func_02009104();
}

void FieldScene_RunActor184Sequence(void)
{
    Func_02009118();
    Func_020092ce(0x53);
    Func_0200927e(0xb8, 3);
    Func_0200860a(0xb9, 0xb8);
    Func_020090da(Func_020090fc(0xb8), 1);
    Func_020090e2_a(0xb8, 2);
    Func_020090e2_b(0x1638, 1);
    Func_0200912a(512);
    Func_0200915e();
}

void FieldScene_RunScene39cSequenceB(void)
{
    extern u8 Data_03001ebc[];

    u32 i;
    s32 record;

    Func_02009166();
    if (Data_02000240_t[224][0] == (s32)Data_00000036) {
        Call3_020036ac(Func_020091de, 0, 0x1d8, 0x258);
        Call3_020036ac(Func_02009272, 0, 0x4000, 10);
        Call4(Func_020092a4, 0x1d00000, -1, 0x2900000, 1);
        record = Func_020091ca(0);
        Func_02009128(record, 0);
        record = Func_020091d6(0);
        (void)Func_02003756(*(volatile s32 *)(record + 8), 0, 0x2be0000, 223);
        Call6(Func_02009126, 92, 46, 92, 40, 3, 2);
        record = Func_020091fc(0);
        *(volatile s32 *)(record + 72) = 0x8000;
        Func_020092d2(0, 2);
        Call3_020036ac(Func_02009296, 0, 6, -1);
        *(s32 *)((*(u8 *volatile *)Data_03001ebc + 0x1c0)) = 0x203;
        Func_02009202(60);
        Func_02009328(8);
    } else {
        Call3_020036ac(Func_020092be, 0, 6, -1);
    }
    Func_0200922a();
}

void FieldScene_RunScene39b_02001208(void)
{
    extern u8 Data_03001ebc[];

    u32 i;
    u8 *rec7;
    s32 rec8;
    s32 record;

    rec7 = Value1_02003788(Func_02009262, 0);
    rec8 = Value1_02003788(Func_0200921a, 0x109);
    if (rec8 == 0) {
        Func_02009254();
        Call4(Func_0200936a, -1, -1, -1, 0);
        rec7[85] = rec8;
        Value3(Func_020092f2, 0, (*(s16 *)((s32)rec7 + 10) << 16), ((*(s16 *)((s32)rec7 + 18) << 16) + -0x100000));
        Func_02009342(0, 15);
        record = Func_020092b0(0);
        Func_0200920e(record, 0);
        Func_020093da();
        Func_020093ee();
        Func_02009454_a(228);
        *(volatile s32 *)((s32)rec7 + 108) = 0x2008cd1;
        Call3(Func_020092da, 0, 0x6666, 0x3333);
        Func_02009454_b(0, 0, 8);
        Func_0200937c(0, 0);
        record = Func_020092ea(0);
        Func_02009248(record, 1);
        {
            u8 *rec9 = (u8 *)*(volatile s32 *)((s32)rec7 + 80);
            s32 mask = -13;
            rec9[9] = (mask & rec9[9]) | 4;
        }
        Func_02009482(0, 0, 10);
        rec7[85] = 3;
        *(volatile s32 *)((s32)rec7 + 108) = rec8;
        Func_0200949c();
        Func_02009300();
    }
}

void FieldScene_RunScene39c_02004f44(void)
{
    u8 *p5;
    u8 *addr;
    u8 v;

    p5 = *(volatile s32 *)0x03001f30;
    Call3_02004f44(Func_0200aa7a, 11, 0x3480000, 0x2580000);
    Func_0200ab8a(93, 1);
    Func_0200ab9a(3, 11);
    addr = p5 + 0x71c;
    v = *addr | 8;
    *addr = v;
    Func_0200abba();
    Func_0200aba0(1);
    Func_0200abbc();
}
