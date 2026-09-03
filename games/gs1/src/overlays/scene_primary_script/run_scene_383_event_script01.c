#include "types.h"

#define RunEventScript01 Func_02002fd4

/* Loader-relocated overlay calls: each symbol names the pre-relocation call
 * word the image holds. */
extern u8 Data_0200d17c[];
extern u8 Data_03001ebc[];
void Func_020076ba();
void Func_0200770c();
void Func_02007726();
void Func_020077a2();
void Func_020077e2();
void Func_020077e6();
void Func_020077f6();
void Func_02007824();
void Func_02007848();
void Func_02007860();
void Func_02007870();
void Func_02007890();
void Func_020078b2();
void Func_020078b6();
void Func_020078bc();
void Func_020078d2();
void Func_020078d8();
void Func_020078ec();
void Func_020078f6();
void Func_02007912();
void Func_02007936();
void Func_02007950();
void Func_02007960();
void Func_02007980();
void Func_02007984();
void Func_020079ae();
void Func_020079b8();
void Func_020079fe();
void Func_02007a2a();
void Func_02007a56();
void Func_02007a90();
void Func_02007aa6();
void Func_02007ab0();
void Func_02007abc();
void Func_02007abe();
void Func_02007b18();
void Func_02007b78();
void Func_02007b90();
void Func_02007b96();
void Func_02007b98();
void Func_02007ba0();
void Func_02007bc0();
void Func_02007bd0();
void Func_02007bda();
void Func_02007bf0();
void Func_02007c00();
void Func_02007c04();
void Func_02007c12();
void Func_02007c26();
void Func_02007c3c();
void Func_02007c4a();
void Func_02007c66();
void Func_02007c6a();
void Func_02007c6c();
void Func_02007c7c();
void Func_02007c8e();
void Func_02007cb6();
void Func_02007cbc();
void Func_02007cf0();
void Func_02007d02();
void Func_02007d0e();
void Func_02007d1a();
void Func_02007d24();
s32 Func_02007d56();
s32 Func_02007d8c();
s32 Func_02007d98();
s32 Func_02007da4();
void Func_02007dae();
void Func_02007dba();
s32 Func_02007dc8();
void Func_02007dd6();
void Func_02007df4();
void Func_02007e0a();
void Func_02007e12();
void Func_02007e1a();
void Func_02007e38();
void Func_02007e42();
void Func_02007e44();
void Func_02007e46();
void Func_02007e50();
void Func_02007e5e();
void Func_02007e66();
void Func_02007e82();
void Func_02007e90();
void Func_02007eca();
void Func_02007ece();
void Func_02007ed6();
void Func_02007ed8();
void Func_02007ee2();
void Func_02007ef4();
void Func_02007ef8();
void Func_02007efe();
void Func_02007f0c();
void Func_02007f24();
void Func_02007f2e();
void Func_02007f3e();
void Func_02007f42();
void Func_02007f5c();
void Func_02007f60();
void Func_02007f70();
void Func_02007f82();
void Func_02007f8e();
void Func_02007fa8();
void Func_02007fc2();
void Func_02007fc4();
void Func_02007fce();
void Func_02007fd0();
void Func_02007fd6();
void Func_02007fda();
void Func_02007fe4();
void Func_02008010();
void Func_02008024();
void Func_0200802c();
void Func_0200803c();
void Func_02008044();
void Func_02008054();
void Func_0200805e();
void Func_02008070();
void Func_02008096();
void Func_020080a6();
void Func_020080ca();
void Func_020080ee();
s32 Func_020080f8();
void Func_02008126();
void Func_0200813e();
void Func_02008140();
void Func_02008150();
void Func_02008174();
void Func_02008186();
s32 Func_02008198();
void Func_0200819c();
void Func_020081a2();
void Func_020081a8();
void Func_020081b2();
void Func_020081d0();
void Func_020081fc();
void Func_02008218();
void Func_0200821e();
void Func_0200822c();
void Func_0200822e();
void Func_02008236();
void Func_02008238();
void Func_0200823e();
void Func_02008256();
void Func_02008260();
void Func_0200826a();
void Func_0200826e();
void Func_02008280();
void Func_020082a2();
void Func_020082ac();
void Func_020082b6();
void Func_020082ba();
void Func_020082c0();
void Func_020082dc();
void Func_020082e6();
void Func_020082f0();
void Func_02008300();
void Func_02008302();
void Func_02008306();
void Func_0200831c();
void Func_02008366();
void Func_0200837a();
void Func_02008384();
void Func_020083aa();
void Func_020083ba();
void Func_020083d8();
void Func_020083e4();
void Func_020083f4();
void Func_020083fa();
void Func_02008416();
void Func_0200841a();
void Func_0200842a();
void Func_02008454();
void Func_02008464();
void Func_02008476();
void Func_0200847e();
void Func_02008574();
void Func_02008580();

/* Resolved engine calls: each pseudo symbol is the per-site call word the
 * overlay image holds (a word can serve two sites with different targets),
 * and the macro names the engine function the site reaches through the
 * overlay veneer and the main-image veneer island, keeping the site's own
 * calling form. Names without a repository binding are provisional.
 */
#define Scene_GetRecord_1(a0) Value1(Func_02007d56, a0)
#define BattleRuntime_Reset_1(args...) Func_02007d24(args)
#define ObjectMotion_SetHorizontalPositionWithTerrain_1(a0, a1, a2) Call3(Func_02007dba, a0, a1, a2)
#define ObjectMotion_SetHorizontalPositionWithTerrain_2(a0, a1, a2) Call3(Func_02007dc8, a0, a1, a2)
#define ObjectMotion_SetHorizontalPositionWithTerrain_3(a0, a1, a2) Call3(Func_02007dd6, a0, a1, a2)
#define Scene_GetRecord_2(args...) Func_02007d8c(args)
#define Scene_GetRecord_3(args...) Func_02007d98(args)
#define Scene_GetRecord_4(args...) Func_02007da4(args)
#define Object_SetModeById_1(args...) Func_02007e0a(args)
#define Object_SetModeById_2(args...) Func_02007e12(args)
#define Object_SetModeById_3(args...) Func_02007e1a(args)
#define Scene_GetRecord_5(args...) Func_02007dc8(args)
#define ObjectMotion_EnableActionAndSetCallback_1(args...) Func_02007df4(args)
#define ObjectMotion_SetHorizontalPositionWithTerrain_4(a0, a1, a2) Call3(Func_02007e42, a0, a1, a2)
#define ObjectMotion_SetHorizontalPositionWithTerrain_5(a0, a1, a2) Call3(Func_02007e50, a0, a1, a2)
#define ObjectMotion_SetHorizontalPositionWithTerrain_6(a0, a1, a2) Call3(Func_02007e5e, a0, a1, a2)
#define ObjectMotion_ArmCallback_1(a0, a1, a2) Call3(Func_02007eca, a0, a1, a2)
#define ObjectMotion_ArmCallback_2(a0, a1, a2) Call3(Func_02007ed6, a0, a1, a2)
#define ObjectMotion_ArmCallback_3(a0, a1, a2) Call3(Func_02007ee2, a0, a1, a2)
#define ObjectMotion_SetSpeedParameters_1(a0, a1, a2) Call3(Func_02007e44, a0, a1, a2)
#define ObjectMotion_ArmCallback_4(a0, a1, a2) Call3(Func_02007ef8, a0, a1, a2)
#define ObjectMotion_SetPositionAndReset_1(args...) Func_02007f2e(args)
#define Object_CommitPositionThenWaitIfModeZero_1(args...) Func_02007f42(args)
#define ObjectMotion_EnableActionAndSetCallback_2(args...) Func_02007e82(args)
#define BattleRuntime_WaitIfModeZero_1(args...) Func_02007e38(args)
#define ObjectMotion_EnableActionAndSetCallback_3(args...) Func_02007e90(args)
#define BattleRuntime_WaitIfModeZero_2(args...) Func_02007e46(args)
#define SceneWork_SetStepValue_1(a0) Call1(Func_02007f24, a0)
#define BattleEffect_SpawnLinkedResourceObject_1(a0, a1, a2) Call3(Func_02007f60, a0, a1, a2)
#define BattleRuntime_WaitIfModeZero_3(args...) Func_02007e66(args)
#define ObjectMotion_SetPositionAndReset_2(a0, a1, a2) Call3(Func_02007ef4, a0, a1, a2)
#define ObjectMotion_SetSpeedParameters_2(a0, a1, a2) Call3(Func_02007ece, a0, a1, a2)
#define ObjectMotion_SetPositionAndReset_3(a0, a1, a2) Call3(Func_02007f0c, a0, a1, a2)
#define ObjectMotion_ArmCallback_5(args...) Func_02007f8e(args)
#define ObjectMotion_SetPositionAndReset_4(a0, a1, a2) Call3(Func_02007f24, a0, a1, a2)
#define ObjectMotion_ArmCallback_6(a0, a1, a2) Call3(Func_02007fa8, a0, a1, a2)
#define ObjectMotion_SetPositionAndReset_5(a0, a1, a2) Call3(Func_02007f3e, a0, a1, a2)
#define ObjectMotion_ArmCallback_7(a0, a1, a2) Call3(Func_02007fc2, a0, a1, a2)
#define BattleRuntime_WaitIfModeZero_4(args...) Func_02007ed8(args)
#define ObjectMotion_SetPositionAndReset_6(a0, a1, a2) Call3(Func_02007f70, a0, a1, a2)
#define BattleRuntime_WaitIfModeZero_5(args...) Func_02007efe(args)
#define ObjectMotion_SetAngleToward_1(args...) Func_02007fd0(args)
#define ObjectMotion_SetAngleToward_2(args...) Func_02007fda(args)
#define ObjectMotion_SetAngleToward_3(args...) Func_02007fe4(args)
#define Object_SetModeById_4(args...) Func_02007fce(args)
#define Object_SetModeById_5(args...) Func_02007fd6(args)
#define ObjectMotion_SetPositionAndReset_7(a0, a1, a2) Call3(Func_02007fce, a0, a1, a2)
#define BattleRuntime_WaitIfModeZero_6(args...) Func_02007f5c(args)
#define ObjectMotion_SetVariantCallbackAndInvokeObject_1(args...) Func_02008024(args)
#define ObjectMotion_EnableActionAndSetCallback_4(args...) Func_02007fc4(args)
#define ObjectMotion_SetVariantCallbackAndInvokeObject_2(args...) Func_0200803c(args)
#define BattleRuntime_WaitIfModeZero_7(args...) Func_02007f82(args)
#define ObjectMotion_SetAngleToward_4(args...) Func_02008054(args)
#define ObjectMotion_SetAngleToward_5(args...) Func_0200805e(args)
#define ObjectMotion_SetVariantCallbackAndInvokeObject_3(args...) Func_02008070(args)
#define ObjectMotion_EnableActionAndSetCallback_5(args...) Func_02008010(args)
#define BattleEffect_SpawnLinkedResourceObject_2(a0, a1, a2) Call3(Func_020080ca, a0, a1, a2)
#define BattleRuntime_WaitIfModeZero_8(args...) Func_02007fd0(args)
#define BattleEffect_SpawnLinkedResourceObject_3(a0, a1, a2) Call3(Func_02008126, a0, a1, a2)
#define BattleRuntime_WaitIfModeZero_9(args...) Func_0200802c(args)
#define BattleEffect_SpawnLinkedResourceObject_4(a0, a1, a2) Call3(Func_0200813e, a0, a1, a2)
#define BattleRuntime_WaitIfModeZero_10(args...) Func_02008044(args)
#define Object_SetModeById_6(args...) Func_020080ee(args)
#define ObjectMotion_SetAngleToward_6(args...) Func_0200813e(args)
#define ObjectMotion_SetVariantCallback_1(args...) Func_02008140(args)
#define ObjectMotion_SetVariantCallbackAndInvokeObject_4(args...) Func_02008150(args)
#define BattleRuntime_WaitIfModeZero_11(args...) Func_02008096(args)
#define BattleRuntime_WaitIfModeZero_12(a0, a1) Call2(Func_020081a8, a0, a1)
#define BattleRuntime_WaitIfModeZero_13(args...) Func_020080a6(args)
#define ObjectMotion_SetSpeedParameters_3(a0, a1) Value2(Func_02008198, a0, a1)
#define UiWork_WaitThenFinalizeCapacity_1(a0, a1) Value2(Func_020080f8, a0, a1)
#define BattleRuntime_WaitIfModeZero_14(args...) Func_020080ca(args)
#define ObjectMotion_SetAngleToward_7(args...) Func_0200819c(args)
#define ObjectMotion_SetVariantCallback_2(args...) Func_020081a2(args)
#define ObjectMotion_SetVariantCallbackAndInvokeObject_5(args...) Func_020081b2(args)
#define BattleEffect_SpawnLinkedResourceObject_5(a0, a1, a2) Call3(Func_02008238, a0, a1, a2)
#define BattleRuntime_WaitIfModeZero_15(args...) Func_0200813e(args)
#define ObjectMotion_SetAngleToward_8(args...) Func_02008218(args)
#define ObjectMotion_SetVariantCallback_3(args...) Func_0200821e(args)
#define ObjectMotion_SetVariantCallbackAndInvokeObject_6(args...) Func_0200822e(args)
#define BattleRuntime_WaitIfModeZero_16(args...) Func_02008174(args)
#define BattleEffect_SpawnLinkedResourceObject_6(a0, a1, a2) Call3(Func_02008280, a0, a1, a2)
#define BattleRuntime_WaitIfModeZero_17(args...) Func_02008186(args)
#define SceneWork_SetStepValue_2(a0) Call1(Func_0200826e, a0)
#define Object_SetModeById_7(args...) Func_0200823e(args)
#define ObjectMotion_SetPositionAndReset_8(a0, a1, a2) Call3(Func_02008236, a0, a1, a2)
#define ObjectMotion_ArmCallback_8(a0, a1, a2) Call3(Func_020082ba, a0, a1, a2)
#define BattleRuntime_WaitIfModeZero_18(args...) Func_020081d0(args)
#define ObjectMotion_SetAngleToward_9(args...) Func_020082a2(args)
#define ObjectMotion_SetAngleToward_10(args...) Func_020082ac(args)
#define ObjectMotion_SetVariantCallbackAndInvokeObject_7(args...) Func_020082b6(args)
#define BattleRuntime_WaitIfModeZero_19(args...) Func_020081fc(args)
#define ObjectMotion_SetSpeedParameters_4(a0, a1, a2) Call3(Func_02008256, a0, a1, a2)
#define ObjectMotion_SetSpeedParameters_5(a0, a1, a2) Call3(Func_02008260, a0, a1, a2)
#define ObjectMotion_SetSpeedParameters_6(a0, a1, a2) Call3(Func_0200826a, a0, a1, a2)
#define ObjectMotion_SetHorizontalPositionWithTerrain_7(a0, a1, a2) Call3(Func_020082c0, a0, a1, a2)
#define ObjectMotion_SetPositionAndReset_9(a0, a1, a2) Call3(Func_020082b6, a0, a1, a2)
#define ObjectMotion_SetHorizontalPositionWithTerrain_8(a0, a1, a2) Call3(Func_020082e6, a0, a1, a2)
#define ObjectMotion_SetPositionAndReset_10(a0, a1, a2) Call3(Func_020082dc, a0, a1, a2)
#define ObjectMotion_SetHorizontalPositionWithTerrain_9(a0, a1, a2) Call3(Func_02008302, a0, a1, a2)
#define ObjectMotion_ResetAndSetPositionInMode2_1(a0, a1, a2) Call3(Func_020082f0, a0, a1, a2)
#define ObjectMotion_SetPositionAndReset_11(a0, a1, a2) Call3(Func_02008306, a0, a1, a2)
#define ObjectMotion_CommitCurrentPositionAndActivate_1(args...) Func_0200831c(args)
#define ObjectMotion_SetAngleToward_11(args...) Func_02008366(args)
#define ObjectMotion_SetAngleToward_12(args...) Func_0200837a(args)
#define ObjectMotion_SetAngleToward_13(args...) Func_02008384(args)
#define ObjectMotion_SetVariantCallbackAndInvokeObject_8(args...) Func_020083ba(args)
#define BattleRuntime_WaitIfModeZero_20(args...) Func_02008300(args)
#define Object_SetModeById_8(args...) Func_020083d8(args)
#define Object_SetModeById_9(args...) Func_020083f4(args)
#define ObjectMotion_ResetAndSetPositionInMode2_2(a0, a1, a2) Call3(Func_020083e4, a0, a1, a2)
#define ObjectMotion_SetPositionAndReset_12(a0, a1, a2) Call3(Func_020083fa, a0, a1, a2)
#define ObjectMotion_SetAngleToward_14(args...) Func_02008454(args)
#define ObjectMotion_CommitCurrentPositionAndActivate_2(args...) Func_0200841a(args)
#define ObjectMotion_SetVariantCallbackAndInvokeObject_9(args...) Func_02008464(args)
#define BattleRuntime_WaitIfModeZero_21(args...) Func_020083aa(args)
#define Object_SetModeById_10(args...) Func_02008476(args)
#define Object_SetModeById_11(args...) Func_0200847e(args)
#define ObjectMotion_SetSpeedParameters_7(args...) Func_02008580(args)
#define BattleRuntime_WaitIfModeZero_22(args...) Func_02008416(args)
#define BattleRuntime_ScheduleShoulderButtonModeUpdate_1(args...) Func_0200842a(args)

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

/* The scene step counter at 0x1d8 of the shared scene work record. */
static __inline__ void bump_step(s32 amount)
{
    u8 *work = *(u8 **)Data_03001ebc;

    *(u16 *)(work + 0x1d8) = (u16)(*(u16 *)(work + 0x1d8) + amount);
}

/* Phase/status word at 0x1c0 of the shared scene work record. */
#define SCENE_PHASE (*(s32 *)(*(u8 *volatile *)Data_03001ebc + 0x1c0))

/* Sets up actors 10, 11, 12 (position, pose, flags) and actors 0-2 and 8-9 and
 * 13-14 in several later waves, moving and animating them through a long
 * scripted sequence, branches once on an actor-12 state check, then advances
 * the shared scene phase twice before returning. */
void RunEventScript01(void)
{
    u32 i;
    s32 record;
    u8 *work;
    s32 base5_200d17c;
    u8 *actor12_record;

    record = Scene_GetRecord_1(12);
    actor12_record = *(volatile s32 *)(record + 80);
    BattleRuntime_Reset_1();
    ObjectMotion_SetHorizontalPositionWithTerrain_1(10, 0x3180000, 0x1a00000);
    ObjectMotion_SetHorizontalPositionWithTerrain_2(11, 0x3200000, 0x1900000);
    ObjectMotion_SetHorizontalPositionWithTerrain_3(12, 0x3080000, 0x1880000);
    record = Scene_GetRecord_2(10);
    Func_02007d02(record, 0);
    record = Scene_GetRecord_3(11);
    Func_02007d0e(record, 0);
    record = Scene_GetRecord_4(12);
    Func_02007d1a(record, 0);
    Object_SetModeById_1(10, 9);
    Object_SetModeById_2(11, 9);
    Object_SetModeById_3(12, 9);
    /* Clear the low bit of the flag byte at +35. */
    *(u8 *)(Scene_GetRecord_5(12) + 35) &= 254;
    /* Set flag bits 0x0c of the byte at +9. */
    actor12_record[9] |= 12;
    base5_200d17c = (s32)Data_0200d17c;
    ObjectMotion_EnableActionAndSetCallback_1(10, base5_200d17c);
    ObjectMotion_SetHorizontalPositionWithTerrain_4(0, 0x3180000, 0x1b80000);
    ObjectMotion_SetHorizontalPositionWithTerrain_5(1, 0x3280000, 0x1b00000);
    ObjectMotion_SetHorizontalPositionWithTerrain_6(2, 0x3080000, 0x1b80000);
    ObjectMotion_ArmCallback_1(0, 0xc000, 0);
    ObjectMotion_ArmCallback_2(1, 0xb000, 0);
    ObjectMotion_ArmCallback_3(2, 0xb000, 0);
    ObjectMotion_SetSpeedParameters_1(8, 0xcccc, 0x6666);
    ObjectMotion_ArmCallback_4(8, 0xb000, 0);
    SCENE_PHASE = 0x209;
    ObjectMotion_SetPositionAndReset_1(0, 0);
    Object_CommitPositionThenWaitIfModeZero_1();
    Func_02007dae();
    Func_020076ba();
    ObjectMotion_EnableActionAndSetCallback_2(11, base5_200d17c);
    BattleRuntime_WaitIfModeZero_1(30);
    ObjectMotion_EnableActionAndSetCallback_3(12, base5_200d17c);
    BattleRuntime_WaitIfModeZero_2(30);
    SceneWork_SetStepValue_1(0x12e4);
    Func_0200770c(10, 20);
    BattleEffect_SpawnLinkedResourceObject_1(8, 0x102, 0);
    BattleRuntime_WaitIfModeZero_3(60);
    Func_02007726(8, 20);
    ObjectMotion_SetPositionAndReset_2(8, 0x328, 0x1c8);
    ObjectMotion_SetSpeedParameters_2(1, 0xcccc, 0x6666);
    ObjectMotion_SetPositionAndReset_3(1, 0x318, 0x1b0);
    ObjectMotion_ArmCallback_5(1, 0, 0);
    ObjectMotion_SetPositionAndReset_4(8, 0x328, 0x198);
    ObjectMotion_ArmCallback_6(8, 0x8000, 0);
    ObjectMotion_SetPositionAndReset_5(1, 0x328, 0x1b0);
    ObjectMotion_ArmCallback_7(1, 0xc000, 0);
    BattleRuntime_WaitIfModeZero_4(20);
    Func_020077e2(8, 3, 20);
    Func_020077a2(8, 20);
    ObjectMotion_SetPositionAndReset_6(8, 0x300, 0x198);
    BattleRuntime_WaitIfModeZero_5(20);
    ObjectMotion_SetAngleToward_1(8, 0, 0);
    ObjectMotion_SetAngleToward_2(0, 8, 0);
    ObjectMotion_SetAngleToward_3(1, 8, 0);
    Func_020077f6(2, 8, 40);
    Func_020077e6(8, 30);
    Object_SetModeById_4(0, 3);
    Object_SetModeById_5(1, 3);
    Func_02007848(2, 3, 20);
    ObjectMotion_SetPositionAndReset_7(8, 0x2e8, 0x198);
    BattleRuntime_WaitIfModeZero_6(50);
    ObjectMotion_SetVariantCallbackAndInvokeObject_1(11, 2);
    Func_02007824(11, 20);
    ObjectMotion_EnableActionAndSetCallback_4(11, base5_200d17c);
    ObjectMotion_SetVariantCallbackAndInvokeObject_2(1, 1);
    BattleRuntime_WaitIfModeZero_7(20);
    ObjectMotion_SetAngleToward_4(0, 11, 0);
    ObjectMotion_SetAngleToward_5(1, 11, 0);
    Func_02007870(2, 11, 20);
    Func_02007860(1, 20);
    ObjectMotion_SetVariantCallbackAndInvokeObject_3(12, 2);
    Func_02007870(12, 30);
    ObjectMotion_EnableActionAndSetCallback_5(12, base5_200d17c);
    BattleEffect_SpawnLinkedResourceObject_2(1, 0x103, 0);
    BattleRuntime_WaitIfModeZero_8(60);
    Func_02007890(1, 30);
    Func_020078b2(2, 0, 30);
    Func_020078bc(0, 2, 30);
    Func_020078f6(2, 3, 20);
    Func_020078b6(2, 20);
    Func_020078d8(1, 2, 30);
    Func_02007912(0, 3, 30);
    Func_020078d2(2, 20);
    BattleEffect_SpawnLinkedResourceObject_3(1, 0x100, 0);
    BattleRuntime_WaitIfModeZero_9(60);
    Func_020078ec(1, 30);
    BattleEffect_SpawnLinkedResourceObject_4(2, 0x101, 0);
    BattleRuntime_WaitIfModeZero_10(60);
    Func_02007936(0, 1, 20);
    Object_SetModeById_6(0, 3);
    Func_02007960(1, 3, 20);
    Func_02007984(2, 0);
    Func_0200822c();
    ObjectMotion_SetAngleToward_6(0, 2, 0);
    Func_02007950(1, 2, 10);
    ObjectMotion_SetVariantCallback_1(0, 1);
    ObjectMotion_SetVariantCallbackAndInvokeObject_4(1, 1);
    BattleRuntime_WaitIfModeZero_11(20);
    BattleRuntime_WaitIfModeZero_12(1, 0x102);
    BattleRuntime_WaitIfModeZero_13(60);
    Func_02007980(0, 1, 10);
    ObjectMotion_SetSpeedParameters_3(1, 0);
    if (UiWork_WaitThenFinalizeCapacity_1(0, 0) == 0) {
        BattleRuntime_WaitIfModeZero_14(20);
        ObjectMotion_SetAngleToward_7(0, 2, 0);
        Func_020079ae(1, 2, 20);
        Func_02007a2a();
        ObjectMotion_SetVariantCallback_2(0, 1);
        ObjectMotion_SetVariantCallbackAndInvokeObject_5(1, 1);
        ((void (*)())Func_020080f8)(20);
        Func_020079b8(1, 20);
    } else {
        bump_step(1);
        BattleEffect_SpawnLinkedResourceObject_5(1, 0x102, 0);
        BattleRuntime_WaitIfModeZero_15(60);
        Func_020079fe(1, 20);
        ObjectMotion_SetAngleToward_8(0, 2, 0);
        Func_02007a2a(1, 2, 20);
        Func_02007aa6();
        ObjectMotion_SetVariantCallback_3(0, 1);
        ObjectMotion_SetVariantCallbackAndInvokeObject_6(1, 1);
        BattleRuntime_WaitIfModeZero_16(20);
    }
    BattleEffect_SpawnLinkedResourceObject_6(2, 0x102, 0);
    BattleRuntime_WaitIfModeZero_17(60);
    Func_02007a90(2, 4, 20);
    SceneWork_SetStepValue_2(0x12f2);
    Func_02007a56(2, 20);
    Object_SetModeById_7(0, 3);
    Func_02007ab0(1, 3, 40);
    ObjectMotion_SetPositionAndReset_8(8, 0x328, 0x198);
    ObjectMotion_ArmCallback_8(8, 0x8000, 0);
    BattleRuntime_WaitIfModeZero_18(30);
    ObjectMotion_SetAngleToward_9(0, 8, 0);
    ObjectMotion_SetAngleToward_10(1, 8, 0);
    Func_02007abe(2, 8, 20);
    ObjectMotion_SetVariantCallbackAndInvokeObject_7(8, 1);
    BattleRuntime_WaitIfModeZero_19(20);
    Func_02007abc(8, 20);
    ObjectMotion_SetSpeedParameters_4(9, 0xcccc, 0x6666);
    ObjectMotion_SetSpeedParameters_5(13, 0xcccc, 0x6666);
    ObjectMotion_SetSpeedParameters_6(14, 0xcccc, 0x6666);
    ObjectMotion_SetHorizontalPositionWithTerrain_7(9, 0x2e80000, 0x1980000);
    ObjectMotion_SetPositionAndReset_9(9, 0x300, 0x198);
    Func_02007b18(9, 10, 30);
    ObjectMotion_SetHorizontalPositionWithTerrain_8(13, 0x2e80000, 0x1980000);
    ObjectMotion_SetPositionAndReset_10(13, 0x300, 0x198);
    ObjectMotion_SetHorizontalPositionWithTerrain_9(14, 0x2e80000, 0x1980000);
    ObjectMotion_ResetAndSetPositionInMode2_1(14, 0x310, 0x190);
    ObjectMotion_SetPositionAndReset_11(13, 0x308, 0x1a8);
    ObjectMotion_CommitCurrentPositionAndActivate_1(14);
    ObjectMotion_SetAngleToward_11(13, 10, 0);
    Func_02007b78(14, 10, 20);
    ObjectMotion_SetAngleToward_12(0, 9, 0);
    ObjectMotion_SetAngleToward_13(1, 9, 0);
    Func_02007b96(2, 9, 20);
    Func_02007bd0(9, 4, 20);
    Func_02007b90(9, 20);
    Func_02007b98(11, 20);
    Func_02007ba0(12, 30);
    Func_02007bda(9, 13, 20);
    ObjectMotion_SetVariantCallbackAndInvokeObject_8(13, 1);
    BattleRuntime_WaitIfModeZero_20(20);
    Func_02007bc0(13, 20);
    Func_02007c12(9, 3, 30);
    Func_02007c04(9, 14, 20);
    Func_02007c26(9, 3, 30);
    Func_02007c00(9, 10, 20);
    Func_02007bf0(9, 20);
    Object_SetModeById_8(13, 3);
    Func_02007c4a(14, 3, 20);
    Func_02007c3c(13, 14, 20);
    Object_SetModeById_9(13, 3);
    Func_02007c66(14, 3, 20);
    ObjectMotion_ResetAndSetPositionInMode2_2(14, 0x318, 0x188);
    ObjectMotion_SetPositionAndReset_12(13, 0x310, 0x190);
    ObjectMotion_SetAngleToward_14(13, 12, 0);
    ObjectMotion_CommitCurrentPositionAndActivate_2(14);
    Func_02007c6c(14, 11, 20);
    ObjectMotion_SetVariantCallbackAndInvokeObject_9(13, 1);
    BattleRuntime_WaitIfModeZero_21(20);
    Func_02007c6a(13, 20);
    Func_02007cbc(14, 4, 20);
    Func_02007c7c(14, 30);
    Func_02007cb6(13, 0, 20);
    Func_02007c8e(13, 20);
    Object_SetModeById_10(0, 3);
    Object_SetModeById_11(1, 3);
    Func_02007cf0(2, 3, 50);
    /* Write the field at +0x1c8, then the phase/status word at +0x1c0, of
     * the shared scene work record. */
    work = *(u8 *volatile *)Data_03001ebc;
    *(volatile s32 *)((work + 0x1c8)) = 30;
    *(volatile s32 *)((work + 0x1c0)) = 0x201;
    Func_02008574();
    ObjectMotion_SetSpeedParameters_7();
    BattleRuntime_WaitIfModeZero_22(60);
    BattleRuntime_ScheduleShoulderButtonModeUpdate_1();
}
