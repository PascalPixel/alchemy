#include "types.h"

#define BattleRuntime_Reset_1(args...) Func_02005890(args)
#define ObjectMotion_SetSpeedParameters_1(a0, a1, a2) Call3(Func_020058da, a0, a1, a2)
#define ObjectMotion_SetSpeedParameters_2(a0, a1, a2) Call3(Func_020058e4, a0, a1, a2)
#define ObjectMotion_SetSpeedParameters_3(a0, a1, a2) Call3(Func_020058ee, a0, a1, a2)
#define Audio_PlayCue_1(args...) Func_02005a94(args)
#define ObjectMotion_SetPositionAndReset_1(a0, a1, a2) Call3(Func_02005932, a0, a1, a2)
#define ObjectMotion_ArmCallback_1(a0, a1, a2) Call3(Func_020059b6, a0, a1, a2)
#define ObjectMotion_SetHorizontalPositionWithTerrain_1(a0, a1, a2) Call3(Func_02005964, a0, a1, a2)
#define ObjectMotion_SetPositionAndReset_2(a0, a1, a2) Call3(Func_0200595a, a0, a1, a2)
#define ObjectMotion_ArmCallback_2(a0, a1, a2) Call3(Func_020059de, a0, a1, a2)
#define GameFlag_IsSet_1(a0) Value1(Func_020058ce, a0)
#define GameFlag_Set_1(a0) Call1(Func_020058e4_a, a0)
#define BattleRuntime_WaitIfModeZero_1(args...) Func_02005912(args)
#define SceneWork_SetStepValue_1(a0) Call1(Func_020059f4, a0)
#define Audio_PlayCue_2(args...) Func_02005b0a(args)
#define BattleRuntime_WaitIfModeZero_2(args...) Func_02005928(args)
#define ObjectMotion_SetVariantCallback_1(args...) Func_020059fa(args)
#define ObjectMotion_SetVariantCallbackAndInvokeObject_1(args...) Func_02005a0a(args)
#define BattleRuntime_WaitIfModeZero_3(args...) Func_02005950(args)
#define BattleRuntime_WaitIfModeZero_4(args...) Func_0200595e(args)
#define ObjectMotion_ArmCallback_3(a0, a1, a2) Call3(Func_02005a70, a0, a1, a2)
#define ObjectMotion_ArmCallback_4(a0, a1, a2) Call3(Func_02005a7c, a0, a1, a2)
#define BattleRuntime_WaitIfModeZero_5(args...) Func_02005992(args)
#define BattleRuntime_WaitIfModeZero_6(args...) Func_020059a0(args)
#define ObjectMotion_SetVariantCallbackAndInvokeObject_2(args...) Func_02005a6c(args)
#define Object_SetModeById_1(args...) Func_02005a54(args)
#define ObjectMotion_SetVariantCallbackAndInvokeObject_3(args...) Func_02005aa0(args)
#define BattleRuntime_WaitIfModeZero_7(args...) Func_020059e6(args)
#define BattleRuntime_WaitIfModeZero_8(args...) Func_020059f8(args)
#define ObjectMotion_SetPositionAndReset_3(a0, a1, a2) Call3(Func_02005a7e, a0, a1, a2)
#define BattleRuntime_WaitIfModeZero_9(args...) Func_02005a0c(args)
#define ObjectMotion_ArmCallback_5(a0, a1, a2) Call3(Func_02005b12, a0, a1, a2)
#define ObjectMotion_ArmCallback_6(a0, a1, a2) Call3(Func_02005b1e, a0, a1, a2)
#define ObjectMotion_SetVariantCallbackAndInvokeObject_4(args...) Func_02005af6(args)
#define BattleRuntime_WaitIfModeZero_10(args...) Func_02005a3c(args)
#define ObjectMotion_ArmCallback_7(a0, a1, a2) Call3(Func_02005b42, a0, a1, a2)
#define BattleRuntime_WaitIfModeZero_11(args...) Func_02005a58(args)
#define BattleEvent_RunActionAndWait_1(args...) Func_02005b48(args)
#define ObjectMotion_SetSpeedParameters_4(args...) Func_02005b48_a(args)
#define Audio_PlayCue_3(args...) Func_02005c58(args)
#define SceneWork_SetStepValue_2(a0) Call1(Func_02005b4e, a0)
#define ObjectMotion_SetSpeedParameters_5(args...) Func_02005b5e(args)
#define UiWork_WaitThenFinalizeCapacity_1(args...) Func_02005abe(args)
#define GameFlag_Set_2(a0) Call1(Func_02005a74, a0)
#define Object_SetModeById_2(args...) Func_02005b34(args)
#define Scene_GetRecord_1(args...) Func_02005ae2(args)
#define ObjectMotion_ResetAndSetPosition_1(args...) Func_02005b14(args)
#define ObjectMotion_CommitCurrentPositionAndActivate_1(args...) Func_02005b42_a(args)
#define ObjectMotion_SetHorizontalPositionWithTerrain_2(args...) Func_02005b54(args)
#define BattleEvent_RunActionAndWait_2(args...) Func_02005bb6(args)
#define Object_SetModeById_3(args...) Func_02005b6e(args)
#define Scene_GetRecord_2(args...) Func_02005b1c(args)
#define ObjectMotion_ResetAndSetPosition_2(args...) Func_02005b4e_a(args)
#define ObjectMotion_CommitCurrentPositionAndActivate_2(args...) Func_02005b7c(args)
#define ObjectMotion_SetHorizontalPositionWithTerrain_3(args...) Func_02005b8e(args)
#define Audio_PlayCueForPartyMember_1(args...) Func_02005cca(args)
#define BattleRuntime_ScheduleShoulderButtonModeUpdate_1(args...) Func_02005b16(args)
#define ObjectMotion_SetSpeedParameters_1_020019a4(a0, a1, a2) Call3(Func_02006736, a0, a1, a2)
#define ObjectMotion_ResetAndSetPositionInMode2_1(a0, a1, a2) Call3(Func_0200676c, a0, a1, a2)
#define Audio_PlayCue_1_020019a4(a0) Value1(Func_020068f8, a0)
#define SCENE_WORD_1C8 (*(u32 *)(*(u8 **)0x03001ebc + 456))
#define ObjectMotion_SetSpeedParameters_1_020019e4(a0, a1, a2) Call3(Func_02006776, a0, a1, a2)
#define ObjectMotion_SetPositionAndReset_1_020019e4(a0, a1, a2) Call3(Func_020067b4, a0, a1, a2)
#define GameFlag_IsSet_1_020019e4(args...) Func_0200671a(args)
#define BattleRuntime_Reset_1_020019e4() Call0(Func_02006752)
#define SceneWork_SetStepValue_1_020019e4(a0) Value1(Func_02006828, a0)
#define BattleEvent_RunActionAndWait_1_020019e4(a0, a1) Call2(Func_02006840, a0, a1)
#define BattleRuntime_ScheduleShoulderButtonModeUpdate_1_020019e4() Call0(Func_0200676c_a)
#define Audio_PlayCue_1_020019e4(a0) Value1(Func_02006958, a0)
#define SCENE_WORK_FIELD_456 (*(u32 *)(*(u8 **)0x03001ebc + 456))
#define Scene_GetRecord_1_02001a4c(args...) Func_020067d0(args)
#define Scene_GetRecord_2_02001a4c(args...) Func_020067d8(args)
#define BattleRuntime_Reset_1_02001a4c() Call0(Func_020067a6)
#define ObjectMotion_SetSpeedParameters_1_02001a4c(a0, a1, a2) Call3(Func_020067f0, a0, a1, a2)
#define ObjectMotion_SetSpeedParameters_2_02001a4c(a0, a1, a2) Call3(Func_020067fa, a0, a1, a2)
#define ObjectMotion_SetSpeedParameters_3_02001a4c(a0, a1, a2) Call3(Func_02006804, a0, a1, a2)
#define ObjectMotion_SetPositionAndReset_1_02001a4c(a0, a1, a2) Call3(Func_02006840_a, a0, a1, a2)
#define ObjectMotion_SetPositionAndReset_2_02001a4c(a0, a1, a2) Call3(Func_0200684c, a0, a1, a2)
#define BattleRuntime_WaitIfModeZero_1_02001a4c(a0) Call1(Func_020067da, a0)
#define BattleEffect_SpawnLinkedResourceObject_1(a0, a1, a2) Call3(Func_020068e6, a0, a1, a2)
#define BattleEffect_SpawnLinkedResourceObject_2(a0, a1, a2) Call3(Func_020068f2, a0, a1, a2)
#define BattleRuntime_WaitIfModeZero_2_02001a4c(a0) Call1(Func_020067f8, a0)
#define ObjectMotion_SetVariantCallbackAndInvokeObject_1_02001a4c(a0, a1) Call2(Func_020068ca, a0, a1)
#define BattleRuntime_WaitIfModeZero_3_02001a4c(a0) Call1(Func_02006810, a0)
#define SceneWork_SetStepValue_1_02001a4c(a0) Value1(Func_020068ee, a0)
#define BattleRuntime_WaitIfModeZero_4_02001a4c(a0, a1) Call2(Func_02006930, a0, a1)
#define BattleRuntime_WaitIfModeZero_5_02001a4c(a0) Call1(Func_0200682e, a0)
#define ObjectMotion_SetVariantCallbackAndInvokeObject_2_02001a4c(a0, a1) Call2(Func_020068fe, a0, a1)
#define ObjectMotion_SetSpeedParameters_4_02001a4c(a0, a1, a2) Call3(Func_0200689c, a0, a1, a2)
#define ObjectMotion_SetSpeedParameters_5_02001a4c(a0, a1, a2) Call3(Func_020068aa, a0, a1, a2)
#define ObjectMotion_EnableActionAndSetCallback_1(a0, a1) Value2(Func_020068ba, a0, a1)
#define ObjectMotion_EnableActionAndSetCallback_2(a0, a1) Value2(Func_020068c2, a0, a1)
#define Object_LookupAndStep_1(a0) Call1(Func_020068d0, a0)
#define GameFlag_Set_1_02001a4c(a0) Value1(Func_02006872, a0)
#define GameFlag_Set_2_02001a4c(a0) Call1(Func_0200687a, a0)
#define BattleRuntime_ScheduleShoulderButtonModeUpdate_1_02001a4c() Value0(Func_020068c4)
#define RATIO_HI 52428
#define RATIO_LO 26214
#define FieldScene_RunScene383_02000428 Func_02000428
#define FieldScene_RunFlag856DialogueBranch Func_0200052c
#define FieldScene_RunScene383SequenceC Func_0200082c
#define FieldScene_RunScene383_0200091c Func_0200091c
#define FieldScene_RunSetupSequence Func_02000b48
#define FieldScene_RunOpeningSequenceHead Func_020019a4
#define FieldScene_RunOpeningSequenceSecond Func_020019e4
#define FieldScene_RunOpeningSequenceThird Func_02001a4c

struct SceneRecord {
    u8 pad[100];
    u16 value100;
};

extern u8 Data_03001ebc[];
extern u8 Data_00001360[];
extern u8 Value_00000854;
extern u8 Value_000012c3;

void Func_020007f4();
void Func_02005170();
void Func_02005194();
void Func_02005246();
void Func_02005270();
void Func_02005274(void);
s32 Func_0200524a(s32);
s32 Func_02005254(s32);
void Func_0200535e(s32);
void Func_020008a4(s32);
void Func_02005292(s32);
void Func_02004b9c(s32, s32, s32);
void Func_02005282(s32);
void Func_02005382(s32);
void Func_0200538a(s32);
void Func_020008d0(s32);
void Func_020052cc(void);
void Func_02004e3e();
void Func_02004e50();
void Func_02004e90();
void Func_02004ea8();
void Func_02004ea8_a();
void Func_02004eca();
s32 Func_0200554a();
void Func_02005574();
void Func_020055a8();
void Func_020055b6();
s32 Func_020055d6();
void Func_020055dc();
void Func_02005604();
void Func_02005618();
void Func_02005634();
void Func_02005650();
void Func_02005656();
s32 Func_02005658();
void Func_02005670();
void Func_02005692();
void Func_020056c0();
void Func_020056e2();
void Func_020056f2();
void Func_02005724();
void Func_02004f4e();
s32 Func_02005644();
void Func_02005664();
void Func_020056a0();
void Func_020056ae();
s32 Func_020056b8();
void Func_020056bc();
s32 Func_020056ce();
void Func_020056d2();
void Func_020056d8();
void Func_02005710();
void Func_0200571e();
void Func_02005722();
void Func_02005746();
void Func_0200574e();
void Func_0200575e();
void Func_0200575e_a();
void Func_0200576c();
void Func_0200576e();
void Func_02005776();
void Func_02005790();
void Func_020057b0();
void Func_020057b8();
void Func_020057c8();
void Func_0200580e();
void Func_02005822();
void Func_0200586a();
void Func_02001b36();
void Func_020051f2();
void Func_02005222();
void Func_02005228();
void Func_02005232();
void Func_0200525c();
void Func_0200525e();
void Func_0200526c();
void Func_02005274_a();
void Func_02005290();
void Func_020052aa();
void Func_020052b6();
void Func_020052c6();
void Func_020052d0();
void Func_020052ec();
void Func_020052fe();
void Func_02005346();
void Func_02005890();
s32 Func_020058ce();
void Func_020058da();
void Func_020058e4();
void Func_020058e4_a();
void Func_020058ee();
void Func_02005912();
void Func_02005928();
void Func_02005932();
void Func_02005950();
void Func_0200595a();
void Func_0200595e();
void Func_02005964();
void Func_02005992();
void Func_020059a0();
void Func_020059b6();
void Func_020059de();
void Func_020059e6();
void Func_020059f4();
void Func_020059f8();
void Func_020059fa();
void Func_02005a0a();
void Func_02005a0c();
void Func_02005a3c();
void Func_02005a54();
void Func_02005a58();
void Func_02005a6c();
void Func_02005a70();
void Func_02005a74();
void Func_02005a7c();
void Func_02005a7e();
void Func_02005a94();
void Func_02005aa0();
s32 Func_02005abe();
void Func_02005af6();
void Func_02005b0a();
void Func_02005b12();
void Func_02005b14();
void Func_02005b16();
void Func_02005b1e();
void Func_02005b34();
void Func_02005b42();
void Func_02005b42_a();
void Func_02005b48();
void Func_02005b48_a();
void Func_02005b4e();
void Func_02005b4e_a();
void Func_02005b54();
void Func_02005b5e();
void Func_02005b6e();
void Func_02005b7c();
void Func_02005b8e();
void Func_02005bb6();
void Func_02005c58();
void Func_02005cca();
u8 *Func_02005ae2_a();
u8 *Func_02005b1c();
void Func_02006736();
void Func_0200676c();
void Func_02006846();
void Func_020068f8();
s32 Func_0200671a();
void Func_02006752();
void Func_0200676c_a();
void Func_02006776();
void Func_020067b4();
void Func_02006828();
void Func_02006840();
void Func_020068a6();
void Func_02006958();
void Func_020060d2();
void Func_020060d6();
void Func_020060ee();
void Func_020060fe();
void Func_020067a6();
void Func_020067da();
void Func_020067dc();
void Func_020067f0();
void Func_020067f8();
void Func_020067fa();
void Func_02006804();
void Func_02006810();
void Func_0200682e();
void Func_02006834();
void Func_02006840_a();
void Func_0200684c();
void Func_02006872();
void Func_0200687a();
void Func_0200689c();
void Func_020068aa();
void Func_020068ba();
void Func_020068c2();
void Func_020068c4();
void Func_020068ca();
void Func_020068d0();
void Func_020068e6();
void Func_020068ee();
void Func_020068f2();
void Func_020068fe();
void Func_02006930();
void *Func_020067d0();

/* Resolved engine calls: each pseudo symbol is the per-site call word the
 * overlay image holds (a word can serve two sites with different targets),
 * and the macro names the engine function the site reaches through the
 * overlay veneer and the main-image veneer island, keeping the site's own
 * calling form. Names without a repository binding are provisional.
 */

/* Word at +456 of the shared scene work record. */

/* Field at +456 of the shared scene work record, addressed through the
 * loader-fixed pointer at 0x03001ebc. */

/* Pair of ratio-like arguments shared by three setup calls below (each
 * applied to a different index: 0, 1, 2). */

/* Loader-relocated overlay calls: each symbol names the pre-relocation call
 * word the image holds. */

/* Call sites spelled through these wrappers pass their constants straight
 * into the argument registers; a direct call precomputes a costly constant
 * into a pseudo that the compiler then shares with later uses in the block.
 * A value-returning call also sets r0 last of its arguments. */
static __inline__ void Call1(void (*f)(), s32 a0)
{
    f(a0);
}

static __inline__ void Call3(void (*f)(), s32 a0, s32 a1, s32 a2)
{
    f(a0, a1, a2);
}

/* The scene step counter at 0x1d8 of the shared scene work record. */
static __inline__ void bump_step(s32 amount)
{
    u8 *work = *(u8 **)Data_03001ebc;

    *(u16 *)(work + 0x1d8) = (u16)(*(u16 *)(work + 0x1d8) + amount);
}

/* Call sites spelled through these wrappers pass their constants straight
 * into the argument registers; a direct call precomputes a costly constant
 * into a pseudo that the compiler then shares with later uses in the block.
 * A value-returning call also sets r0 last of its arguments. */
static __inline__ s32 Value0(s32 (*f)())
{
    return f();
}

static __inline__ void Call1_0200082c(void (*f)(), s32 a0)
{
    f(a0);
}

static __inline__ s32 Value1(s32 (*f)(), s32 a0)
{
    return f(a0);
}

static __inline__ s32 Value2(s32 (*f)(), s32 a0, s32 a1)
{
    return f(a0, a1);
}

static __inline__ void Call1_0200091c(void (*f)(), s32 a0)
{
    f(a0);
}

static __inline__ s32 Value3(s32 (*f)(), s32 a0, s32 a1, s32 a2)
{
    return f(a0, a1, a2);
}

static __inline__ void Call1_020019a4(void (*f)(), s32 a0)
{
    f(a0);
}

static __inline__ void Call0(void (*f)())
{
    f();
}

static __inline__ void Call1_020019e4(void (*f)(), s32 a0)
{
    f(a0);
}

static __inline__ void Call2(void (*f)(), s32 a0, s32 a1)
{
    f(a0, a1);
}

static __inline__ s32 Value0_02001a4c(s32 (*f)())
{
    void *Func_02005ae2();
    void *Func_020067d8();

    return f();
}

static __inline__ void Call1_02001a4c(void (*f)(), s32 a0)
{
    void *Func_02005ae2();
    void *Func_020067d8();

    f(a0);
}

static __inline__ void Call6(void (*f)(), s32 a0, s32 a1, s32 a2, s32 a3, s32 a4, s32 a5)
{
    void *Func_02005ae2();
    void *Func_020067d8();

    f(a0, a1, a2, a3, a4, a5);
}

void FieldScene_RunScene383_02000428(void)
{
    u32 i;
    s32 record;

    Func_02005170();
    Call1(Func_02005246, 0x1253);
    Func_020007f4(15);
    Call3(Func_02005270, 15, 0x8000, 0);
    Func_02005194();
}

void FieldScene_RunFlag856DialogueBranch(void)
{
    s32 g;
    g = 0x851;
    Func_02005274();
    if (Func_0200524a(0x856) != 0) {
        if (Func_02005254(g) == 0) {
            Func_0200535e(0x1276);
            Func_020008a4(16);
            Func_02005292(10);
            Func_02004b9c(16, 3, 20);
            Func_02005282(g);
        } else {
            Func_02005382(0x1278);
        }
    } else {
        Func_0200538a(0x1250);
    }
    Func_020008d0(16);
    Func_020052cc();
}

void FieldScene_RunScene383SequenceC(void)
{
    u32 i;
    s32 record;

    Func_02005574();
    if (Value1(Func_0200554a, 0x857) == 0) {
        Func_02005656((s32)Data_00001360);
        Func_02004e3e(16, 20);
        Func_02004e90(16, 3, 20);
        Func_02004e50(16, 30);
        Func_02005692(16, 0, 0);
        Func_020055a8(30);
        Func_02005670(16, 2);
        Func_020055b6(30);
        Func_02004ea8(0, 16, 20);
        Func_02004eca(16, 3, 20);
        bump_step(1);
        if (Value0(Func_020055d6) == 0) {
            Func_020056c0(((s32)Data_00001360 + 3));
            Func_02004ea8_a(16, 20);
            Func_02005604();
            goto L_02000906;
        }
        Call1_0200082c(Func_020055dc, 0x857);
        Func_02005634(189, 0);
    }
    Call1_0200082c(Func_020056e2, 0x1364);
    Func_020056f2(16, 0);
    Func_02005618(20);
    if (Value2(Func_02005658, 0, 0) != 0) {
        bump_step(1);
    }
    Func_02005724(16, 0);
    Func_02005650();
    L_02000906:;
}

void FieldScene_RunScene383_0200091c(void)
{
    u32 i;
    s32 record;

    Func_02005664();
    Func_02004f4e(18, 0, 2);
    if (Value1(Func_02005644, 0x85b) == 0) {
        Call1_0200091c(Func_0200574e, 0x137c);
        Func_0200575e(18, 0);
    } else {
        Call1_0200091c(Func_0200575e_a, 0x1385);
        Func_0200576e(18, 0);
    }
    if (Value2(Func_020056ce, 0, 0) == 0) {
        Func_020056a0(20);
        Func_02005790(18, 0);
        Func_020056ae(20);
        Func_02005776(18, 2);
        Func_020056bc(20);
        if (Value0(Func_020056b8) == 0) {
            Func_0200576c(18, 4);
            Func_020056d2(20);
            Call1_0200091c(Func_020057b0, 0x1384);
            Func_020057c8(18, 0);
            goto L_020009ec;
        }
        Func_0200586a(231, 3);
        Func_02005722(231, 0);
        Call1_0200091c(Func_020056d8, 0x85b);
    } else {
        bump_step(1);
        Func_02005710(20);
        Func_020057b8(18, 3);
        Func_0200571e(20);
        Func_0200580e(18, 0);
    }
    L_020009ec:;
    Call3(Func_02005822, 18, 0x4000, 0);
    Func_02005746();
}

/* Configures actors 0, 1, 2 (position, movement, and animation timing), then
 * branches on whether actor 0 is already set up: one path sets up actors 0-2
 * with poses and movement, the other only advances actor 2's animation. Both
 * paths converge to check actor 0's record and, depending on that check,
 * configure either actor 2 or actor 1 from it before the scene finishes. */
void FieldScene_RunSetupSequence(void)
{
    u8 *Func_02005ae2();
    u8 *Func_020067d8();

    u8 *record;

    BattleRuntime_Reset_1();
    ObjectMotion_SetSpeedParameters_1(0, 0xcccc, 0x6666);
    ObjectMotion_SetSpeedParameters_2(1, 0xcccc, 0x6666);
    ObjectMotion_SetSpeedParameters_3(2, 0xcccc, 0x6666);
    Audio_PlayCue_1(19);
    ObjectMotion_SetPositionAndReset_1(0, 0x180, 0x198);
    ObjectMotion_ArmCallback_1(0, 0xc000, 0);
    ObjectMotion_SetHorizontalPositionWithTerrain_1(1, 0x1800000, 0x1980000);
    ObjectMotion_SetPositionAndReset_2(1, 0x170, 0x198);
    ObjectMotion_ArmCallback_2(1, 0xd000, 20);
    if (GameFlag_IsSet_1(0x850) != 0) {
    } else {
        GameFlag_Set_1(0x850);
        Func_02005228(2, 0);
        BattleRuntime_WaitIfModeZero_1(40);
        Func_0200525e();
        SceneWork_SetStepValue_1(0x1256);
        Audio_PlayCue_2(60);
        BattleRuntime_WaitIfModeZero_2(30);
        Func_02005232(2, 3, 30);
        Func_020051f2(2, 30);
        ObjectMotion_SetVariantCallback_1(0, 1);
        ObjectMotion_SetVariantCallbackAndInvokeObject_1(1, 1);
        BattleRuntime_WaitIfModeZero_3(20);
        Func_02005274_a(2, 0);
        BattleRuntime_WaitIfModeZero_4(40);
        Func_020052aa();
        Func_02005222(2, 30);
        Func_0200525c(0, 1, 50);
        ObjectMotion_ArmCallback_3(0, 0xc000, 0);
        ObjectMotion_ArmCallback_4(1, 0xd000, 0);
        BattleRuntime_WaitIfModeZero_5(20);
        Func_020052b6(2, 0);
        BattleRuntime_WaitIfModeZero_6(40);
        Func_020052ec();
        ObjectMotion_SetVariantCallbackAndInvokeObject_2(2, 1);
        Func_0200526c(2, 50);
        Object_SetModeById_1(0, 3);
        Func_020052c6(1, 3, 20);
        Func_020052d0(2, 3, 20);
        Func_02005290(2, 40);
        ObjectMotion_SetVariantCallbackAndInvokeObject_3(2, 1);
        BattleRuntime_WaitIfModeZero_7(30);
        Call3((void (*)())Func_02005ae2_a, 2, 0xc000, 0);
        BattleRuntime_WaitIfModeZero_8(30);
        ObjectMotion_SetPositionAndReset_3(2, 0x178, 0x178);
        BattleRuntime_WaitIfModeZero_9(40);
        Func_020052fe(0, 1, 50);
        ObjectMotion_ArmCallback_5(0, 0xc000, 0);
        ObjectMotion_ArmCallback_6(1, 0xd000, 0);
        ObjectMotion_SetVariantCallbackAndInvokeObject_4(2, 1);
        BattleRuntime_WaitIfModeZero_10(50);
        Func_02005346(2, 3, 30);
        ObjectMotion_ArmCallback_7(2, 0x4000, 0);
        BattleRuntime_WaitIfModeZero_11(10);
        BattleEvent_RunActionAndWait_1(2, 0);
        ObjectMotion_SetSpeedParameters_4(2, 0);
        goto L_join_setup_paths;
    }
    Audio_PlayCue_3(60);
    SceneWork_SetStepValue_2(0x125d);
    ObjectMotion_SetSpeedParameters_5(2, 0);
    L_join_setup_paths:;
    if (UiWork_WaitThenFinalizeCapacity_1(0, 0) == 0) {
        Func_02001b36();
        GameFlag_Set_2(0x856);
        Object_SetModeById_2(2, 2);
        record = Scene_GetRecord_1(0);
        if (record != 0) {
            /* Read the two s16 fields at +10 and +18 of the record. */
            ObjectMotion_ResetAndSetPosition_1(2, *(s16 *)(record + 10), *(s16 *)(record + 18));
        }
        ObjectMotion_CommitCurrentPositionAndActivate_1(2);
        ObjectMotion_SetHorizontalPositionWithTerrain_2(2, 0, 0);
    } else {
        BattleEvent_RunActionAndWait_2(2, 0);
    }
    Object_SetModeById_3(1, 2);
    record = Scene_GetRecord_2(0);
    if (record != 0) {
        /* Read the two s16 fields at +10 and +18 of the record. */
        ObjectMotion_ResetAndSetPosition_2(1, *(s16 *)(record + 10), *(s16 *)(record + 18));
    }
    ObjectMotion_CommitCurrentPositionAndActivate_2(1);
    ObjectMotion_SetHorizontalPositionWithTerrain_3(1, 0, 0);
    Audio_PlayCueForPartyMember_1();
    BattleRuntime_ScheduleShoulderButtonModeUpdate_1();
}

/* Sets up the opening sequence: two calls with fixed argument pairs, a
 * write to the scene work record, and two more calls with fixed args. */
void FieldScene_RunOpeningSequenceHead(void)
{
    ObjectMotion_SetSpeedParameters_1_020019a4(0, 32768, 16384); /* object_id 0, speed_limit 32768, acceleration 16384 */
    ObjectMotion_ResetAndSetPositionInMode2_1(0, 728, 408); /* object_id 0, x 728, z 408 */
    SCENE_WORD_1C8 = 16;
    Audio_PlayCue_1_020019a4(123);
    Call1_020019a4(Func_02006846, 15); /* main:0808a248 */
}

/* Runs the second part of an opening sequence: two parameterised calls, then
 * (only if a flag read via Value_00000854 is clear) a short block of setup
 * calls, then unconditionally sets a scene work field and two more calls. */
void FieldScene_RunOpeningSequenceSecond(void)
{
    ObjectMotion_SetSpeedParameters_1_020019e4(0, 32768, 16384); /* object 0, speed_limit 32768, acceleration 16384 */
    ObjectMotion_SetPositionAndReset_1_020019e4(0, 744, 408); /* object 0, x 744, z 408 */
    if (GameFlag_IsSet_1_020019e4((s32)&Value_00000854) == 0) {
        BattleRuntime_Reset_1_020019e4();
        SceneWork_SetStepValue_1_020019e4((s32)&Value_000012c3);
        BattleEvent_RunActionAndWait_1_020019e4(8, 0); /* action 8 */
        BattleRuntime_ScheduleShoulderButtonModeUpdate_1_020019e4();
    }
    SCENE_WORK_FIELD_456 = 16;
    Audio_PlayCue_1_020019e4(123);
    Call1_020019e4(Func_020068a6, 14); /* main:0808a248 */
}

/* Third scene step: sets up actors 24 and 25 (fetching each one's record),
 * runs a shared series of configuration calls touching actors 0-2, 10, 14,
 * 20, 24 and 25, then marks the two fetched records with a byte flag. */
void FieldScene_RunOpeningSequenceThird(void)
{
    void *Func_02005ae2();
    void *Func_020067d8();

    void *actor24;
    void *actor25;

    actor24 = Scene_GetRecord_1_02001a4c(24);
    actor25 = Scene_GetRecord_2_02001a4c(25);
    BattleRuntime_Reset_1_02001a4c();
    ObjectMotion_SetSpeedParameters_1_02001a4c(0, RATIO_HI, RATIO_LO);
    ObjectMotion_SetSpeedParameters_2_02001a4c(1, RATIO_HI, RATIO_LO);
    ObjectMotion_SetSpeedParameters_3_02001a4c(2, RATIO_HI, RATIO_LO);
    ObjectMotion_SetPositionAndReset_1_02001a4c(0, 232, 696);
    ObjectMotion_SetPositionAndReset_2_02001a4c(0, 200, 696);
    BattleRuntime_WaitIfModeZero_1_02001a4c(10);
    BattleEffect_SpawnLinkedResourceObject_1(25, 256, 0);
    BattleEffect_SpawnLinkedResourceObject_2(24, 256, 0);
    BattleRuntime_WaitIfModeZero_2_02001a4c(60);
    Call3(Func_020060d2, 25, 0, 10);
    ObjectMotion_SetVariantCallbackAndInvokeObject_1_02001a4c(24, 2);
    BattleRuntime_WaitIfModeZero_3_02001a4c(20);
    SceneWork_SetStepValue_1_02001a4c(4758);
    Call2(Func_020060d6, 24, 20);
    BattleRuntime_WaitIfModeZero_4_02001a4c(25, 258); /* main:0808a1f0 */
    BattleRuntime_WaitIfModeZero_5_02001a4c(60);
    Call2(Func_020060ee, 25, 20);
    ObjectMotion_SetVariantCallbackAndInvokeObject_2_02001a4c(24, 1);
    Call2(Func_020060fe, 24, 30);
    ObjectMotion_SetSpeedParameters_4_02001a4c(24, 262144, 131072);
    ObjectMotion_SetSpeedParameters_5_02001a4c(25, 229376, 114688);
    ObjectMotion_EnableActionAndSetCallback_1(25, 33609776);
    ObjectMotion_EnableActionAndSetCallback_2(24, 33609056);
    Object_LookupAndStep_1(24);
    Call6(Func_02006834, 14, 45, 3, 1, 14, 44); /* main:080091c0 */
    GameFlag_Set_1_02001a4c(2130);
    GameFlag_Set_2_02001a4c(768);
    Call2(Func_020067dc, 33598369, 3200); /* main:080000d0 */
    /* Byte flag at +100 of each record: 1 for actor 24's, 3 for actor 25's. */
    ((struct SceneRecord *)actor24)->value100 = 1;
    ((struct SceneRecord *)actor25)->value100 = 3;
    BattleRuntime_ScheduleShoulderButtonModeUpdate_1_02001a4c();
}
