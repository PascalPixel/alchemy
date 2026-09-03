#include "types.h"

#define FieldScene_RunScene383SequenceB Func_02001ba0

/* Loader-relocated overlay calls: each symbol names the pre-relocation call
 * word the image holds. */
extern u8 Data_00001299[];
void Func_02006290();
void Func_020062bc();
void Func_020062ec();
void Func_020062f0();
void Func_020062fe();
void Func_0200631e();
void Func_02006348();
void Func_0200634e();
void Func_02006352();
void Func_02006378();
void Func_02006388();
void Func_020063b8();
void Func_020063fa();
void Func_02006838();
void Func_020068ea();
void Func_020068fa();
s32 Func_02006924();
s32 Func_0200692c();
void Func_02006962();
void Func_0200696c();
void Func_02006980();
void Func_0200698a();
void Func_0200698c();
void Func_02006996();
void Func_020069a0();
void Func_020069dc();
void Func_020069fe();
void Func_02006a02();
void Func_02006a10();
void Func_02006a12();
void Func_02006a28();
s32 Func_02006a50();
void Func_02006a60();
void Func_02006a7e();
void Func_02006a88();
void Func_02006a8c();
void Func_02006a9c();
void Func_02006aa8();
void Func_02006aaa();
void Func_02006aae();
void Func_02006ac0();
void Func_02006ae0();
s32 Func_02006af0();
void Func_02006b0c();
void Func_02006b12();
void Func_02006b14();
void Func_02006b3c();
void Func_02006b5a();
void Func_02006b76();
void Func_02006b80();
void Func_02006b88();
void Func_02006b8a();
void Func_02006b9c();
void Func_02006b9e();
void Func_02006ba8();
void Func_02006bac();
void Func_02006bb6();
void Func_02006bc4();
void Func_02006bca();
void Func_02006bd2();
void Func_02006c3c();
void Func_02006c48();

/* Resolved engine calls: each pseudo symbol is the per-site call word the
 * overlay image holds (a word can serve two sites with different targets),
 * and the macro names the engine function the site reaches through the
 * overlay veneer and the main-image veneer island, keeping the site's own
 * calling form. Names without a repository binding are provisional.
 */
#define Scene_GetRecord_1(a0) Value1(Func_02006924, a0)
#define Scene_GetRecord_2(a0) Value1(Func_0200692c, a0)
#define BattleRuntime_Reset_1(args...) Func_020068fa(args)
#define GameFlag_Clear_1(a0) Call1(Func_020068ea, a0)
#define ObjectMotion_EnableActionAndSetCallback_1(args...) Func_02006962(args)
#define ObjectMotion_EnableActionAndSetCallback_2(args...) Func_0200696c(args)
#define ObjectMotion_EnableActionAndSetCallback_3(args...) Func_02006980(args)
#define ObjectMotion_EnableActionAndSetCallback_4(args...) Func_0200698a(args)
#define ObjectMotion_SetSpeedParameters_1(a0, a1, a2) Call3(Func_0200698c, a0, a1, a2)
#define ObjectMotion_SetSpeedParameters_2(a0, a1, a2) Call3(Func_02006996, a0, a1, a2)
#define ObjectMotion_SetSpeedParameters_3(a0, a1, a2) Call3(Func_020069a0, a0, a1, a2)
#define ObjectMotion_SetPositionAndReset_1(a0, a1, a2) Call3(Func_020069dc, a0, a1, a2)
#define ObjectMotion_SetHorizontalPositionWithTerrain_1(a0, a1, a2) Call3(Func_02006a02, a0, a1, a2)
#define ObjectMotion_SetHorizontalPositionWithTerrain_2(a0, a1, a2) Call3(Func_02006a10, a0, a1, a2)
#define ObjectMotion_ResetAndSetPositionInMode2_1(a0, a1, a2) Call3(Func_020069fe, a0, a1, a2)
#define ObjectMotion_SetPositionAndReset_2(a0, a1, a2) Call3(Func_02006a12, a0, a1, a2)
#define ObjectMotion_CommitCurrentPositionAndActivate_1(args...) Func_02006a28(args)
#define ObjectMotion_ArmCallback_1(a0, a1, a2) Call3(Func_02006a9c, a0, a1, a2)
#define ObjectMotion_SetAngleToward_1(args...) Func_02006a7e(args)
#define ObjectMotion_SetVariantCallbackAndInvokeObject_1(args...) Func_02006a88(args)
#define SceneWork_SetStepValue_1(args...) Func_02006aa8(args)
#define BattleEvent_RunActionAndWait_1(args...) Func_02006ac0(args)
#define ObjectMotion_SetAngleToward_2(args...) Func_02006aaa(args)
#define Object_SetModeById_1(args...) Func_02006a8c(args)
#define ObjectMotion_SetSpeedParameters_4(a0, a1) Value2(Func_02006af0, a0, a1)
#define UiWork_WaitThenFinalizeCapacity_1(a0, a1) Value2(Func_02006a50, a0, a1)
#define SceneWork_SetStepValue_2(args...) Func_02006b12(args)
#define ObjectMotion_SetAngleToward_3(args...) Func_02006b0c(args)
#define BattleEffect_SpawnLinkedResourceObject_1(a0, a1, a2) Call3(Func_02006b5a, a0, a1, a2)
#define BattleRuntime_WaitIfModeZero_1(args...) Func_02006a60(args)
#define ObjectMotion_SetAngleToward_4(args...) Func_02006b3c(args)
#define BattleEffect_SpawnLinkedResourceObject_2(a0, a1, a2) Call3(Func_02006b9c, a0, a1, a2)
#define BattleEffect_SpawnLinkedResourceObject_3(a0, a1, a2) Call3(Func_02006ba8, a0, a1, a2)
#define BattleRuntime_WaitIfModeZero_2(args...) Func_02006aae(args)
#define SceneWork_SetStepValue_3(a0) Call1(Func_02006b9e, a0)
#define BattleEvent_RunActionAndWait_2(args...) Func_02006bb6(args)
#define ObjectMotion_ArmCallback_2(a0, a1, a2) Call3(Func_02006bca, a0, a1, a2)
#define BattleRuntime_WaitIfModeZero_3(args...) Func_02006ae0(args)
#define Object_SetModeById_2(args...) Func_02006b80(args)
#define Object_SetModeById_3(args...) Func_02006b88(args)
#define ObjectMotion_ResetAndSetPositionInMode2_2(a0, a1, a2) Call3(Func_02006b76, a0, a1, a2)
#define ObjectMotion_SetPositionAndReset_3(a0, a1, a2) Call3(Func_02006b8a, a0, a1, a2)
#define ObjectMotion_SetHorizontalPositionWithTerrain_3(args...) Func_02006bac(args)
#define ObjectMotion_SetHorizontalPositionWithTerrain_4(args...) Func_02006bb6(args)
#define ObjectMotion_SetHorizontalPositionWithTerrain_5(a0, a1, a2) Call3(Func_02006bc4, a0, a1, a2)
#define ObjectMotion_SetHorizontalPositionWithTerrain_6(a0, a1, a2) Call3(Func_02006bd2, a0, a1, a2)
#define ObjectMotion_ArmCallback_3(args...) Func_02006c3c(args)
#define ObjectMotion_ArmCallback_4(a0, a1, a2) Call3(Func_02006c48, a0, a1, a2)
#define BattleRuntime_ScheduleShoulderButtonModeUpdate_1(args...) Func_02006b80(args)

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

static __inline__ void Call6(void (*f)(), s32 a0, s32 a1, s32 a2, s32 a3, s32 a4, s32 a5)
{
    f(a0, a1, a2, a3, a4, a5);
}

/* The scene step counter at 0x1d8 of the shared scene work record. */
static __inline__ void bump_step(s32 amount)
{
    u8 *work = *(u8 **)0x03001ebc;

    *(u16 *)(work + 0x1d8) = (u16)(*(u16 *)(work + 0x1d8) + amount);
}

/* Sets up actors 24 and 25 (pose picked from a per-actor threshold field at
 * +100), positions and moves actors 0-2 and 24-25, and advances the shared
 * scene step counter partway through. */
void FieldScene_RunScene383SequenceB(void)
{
    s32 actor24;
    s32 actor25;
    s32 record_00001299;

    actor24 = Scene_GetRecord_1(24);
    actor25 = Scene_GetRecord_2(25);
    BattleRuntime_Reset_1();
    Call1(Func_02006838, 0x200aba1);
    GameFlag_Clear_1(0x300);
    if (*(s16 *)(actor24 + 100) <= 3) {
        ObjectMotion_EnableActionAndSetCallback_1(24, 0x200d678);
    } else {
        ObjectMotion_EnableActionAndSetCallback_2(24, 0x200d650);
    }
    if (*(s16 *)(actor25 + 100) <= 2) {
        ObjectMotion_EnableActionAndSetCallback_3(25, 0x200d768);
    } else {
        ObjectMotion_EnableActionAndSetCallback_4(25, 0x200d650);
    }
    ObjectMotion_SetSpeedParameters_1(0, 0xcccc, 0x6666);
    ObjectMotion_SetSpeedParameters_2(1, 0xcccc, 0x6666);
    ObjectMotion_SetSpeedParameters_3(2, 0xcccc, 0x6666);
    ObjectMotion_SetPositionAndReset_1(0, 248, 0x2d8);
    ObjectMotion_SetHorizontalPositionWithTerrain_1(2, 0xf80000, 0x2d80000);
    ObjectMotion_SetHorizontalPositionWithTerrain_2(1, 0xf80000, 0x2d80000);
    ObjectMotion_ResetAndSetPositionInMode2_1(2, 0x108, 0x2e8);
    ObjectMotion_SetPositionAndReset_2(1, 232, 0x2e8);
    ObjectMotion_CommitCurrentPositionAndActivate_1(2);
    ObjectMotion_ArmCallback_1(0, 0x4000, 0);
    ObjectMotion_SetAngleToward_1(1, 0, 0);
    Func_02006290(2, 0, 30);
    ObjectMotion_SetVariantCallbackAndInvokeObject_1(2, 1);
    record_00001299 = (s32)Data_00001299;
    SceneWork_SetStepValue_1(record_00001299);
    BattleEvent_RunActionAndWait_1(2, 0);
    ObjectMotion_SetAngleToward_2(0, 2, 0);
    Func_020062bc(1, 2, 20);
    Object_SetModeById_1(0, 3);
    Func_020062fe(1, 3, 20);
    Func_020062f0(0, 1, 10);
    ObjectMotion_SetSpeedParameters_4(1, 0);
    if (UiWork_WaitThenFinalizeCapacity_1(0, 0) != 0) {
        bump_step(1);
    }
    Func_020062ec(1, 30);
    SceneWork_SetStepValue_2((record_00001299 + 4));
    ObjectMotion_SetAngleToward_3(0, 2, 0);
    Func_0200631e(1, 2, 50);
    BattleEffect_SpawnLinkedResourceObject_1(2, 0x100, 0);
    BattleRuntime_WaitIfModeZero_1(60);
    Func_02006352(0, 1, 50);
    ObjectMotion_SetAngleToward_4(0, 2, 0);
    Func_0200634e(1, 2, 30);
    Func_02006388(2, 3, 10);
    Func_02006348(2, 20);
    BattleEffect_SpawnLinkedResourceObject_2(0, 0x102, 0);
    BattleEffect_SpawnLinkedResourceObject_3(1, 0x102, 0);
    BattleRuntime_WaitIfModeZero_2(60);
    Func_020063b8(2, 3, 20);
    Func_02006378(2, 30);
    SceneWork_SetStepValue_3(0x129f);
    BattleEvent_RunActionAndWait_2(1, 0);
    ObjectMotion_ArmCallback_2(0, 0x4000, 0);
    BattleRuntime_WaitIfModeZero_3(40);
    Object_SetModeById_2(0, 3);
    Object_SetModeById_3(1, 3);
    Func_020063fa(2, 3, 50);
    ObjectMotion_ResetAndSetPositionInMode2_2(2, 248, 0x2d8);
    ObjectMotion_SetPositionAndReset_3(1, 248, 0x2d8);
    ObjectMotion_SetHorizontalPositionWithTerrain_3(1, 0, 0);
    ObjectMotion_SetHorizontalPositionWithTerrain_4(2, 0, 0);
    ObjectMotion_SetHorizontalPositionWithTerrain_5(24, 0x680000, 0x2b80000);
    ObjectMotion_SetHorizontalPositionWithTerrain_6(25, 0x780000, 0x2b80000);
    ObjectMotion_ArmCallback_3(24, 0, 0);
    ObjectMotion_ArmCallback_4(25, 0x8000, 0);
    Call6(Func_02006b14, 14, 50, 3, 1, 14, 44);
    BattleRuntime_ScheduleShoulderButtonModeUpdate_1();
}
