#include "types.h"

#define FieldScene_RunScene3aeSequenceA Func_02000ba0

/* Loader-relocated overlay calls: each symbol names the pre-relocation call
 * word the image holds. */
extern u8 Data_03001ebc[];
void Func_02002124();
void Func_02002148();
s32 Func_0200215c();
void Func_02002170();
void Func_0200218e();
void Func_020021b6();
void Func_020021bc();
void Func_020021c0();
void Func_020021e2();
void Func_020021e8();
void Func_020021fe();
void Func_02002216();
void Func_02002230();
void Func_02002232();
void Func_02002234();
void Func_0200223e();
void Func_02002242();
void Func_02002244();
void Func_0200224a();
void Func_0200224c();
void Func_02002258();
void Func_02002266();
void Func_0200226c();
void Func_02002272();
void Func_02002278();
void Func_0200228a();
void Func_0200228c();
void Func_02002298();
void Func_0200229e();
void Func_020022aa();
void Func_020022c2();
void Func_020022dc();
void Func_020022e2();
void Func_020022e8();
void Func_020022ee();
void Func_020022f6();
void Func_020022fc();
void Func_02002306();
void Func_0200230a();
void Func_02002310();
void Func_02002316();
void Func_02002318();
void Func_02002342();
void Func_0200234a();
void Func_0200234c();
void Func_02002352();
s32 Func_02002358();
void Func_0200236c();
void Func_02002374();
void Func_02002382();
s32 Func_02002386();
void Func_02002392();
void Func_02002398();
s32 Func_0200239a();
void Func_020023a0();
s32 Func_020023a4();
void Func_020023a6();
void Func_020023aa();
void Func_020023b0();
void Func_020023b2();

/* Resolved engine calls: each pseudo symbol is the per-site call word the
 * overlay image holds (a word can serve two sites with different targets),
 * and the macro names the engine function the site reaches through the
 * overlay veneer and the main-image veneer island, keeping the site's own
 * calling form. Names without a repository binding are provisional.
 */
#define BattleRuntime_Reset_1(args...) Func_02002124(args)
#define ObjectMotion_SetHorizontalPositionWithTerrain_1(a0, a1, a2) Call3(Func_0200218e, a0, a1, a2)
#define Scene_GetRecord_1(args...) Func_0200215c(args)
#define BattleRuntime_WaitIfModeZero_1(args...) Func_0200223e(args)
#define ObjectMotion_SetSpeedParameters_1(args...) Func_0200224a(args)
#define BattleRuntime_WaitIfModeZero_2(args...) Func_02002148(args)
#define ObjectMotion_RealignToTrackedObjectAndArmCallback_1(a0, a1, a2, a3) Call4(Func_02002278, a0, a1, a2, a3)
#define ObjectMotion_CommitCurrentPositionAndActivate_1(args...) Func_020021b6(args)
#define ObjectMotion_ArmCallback_1(a0, a1, a2) Call3(Func_02002232, a0, a1, a2)
#define BattleRuntime_WaitIfModeZero_3(args...) Func_02002170(args)
#define SceneWork_SetStepValue_1(a0) Call1(Func_02002216, a0)
#define ObjectMotion_ArmCallback_2(a0, a1, a2) Call3(Func_0200224a, a0, a1, a2)
#define ObjectMotion_SetSpeedParameters_2(a0, a1, a2) Call3(Func_020021bc, a0, a1, a2)
#define ObjectMotion_SetPositionAndReset_1(a0, a1, a2) Call3(Func_020021e8, a0, a1, a2)
#define ObjectMotion_ArmCallback_3(a0, a1, a2) Call3(Func_0200226c, a0, a1, a2)
#define ObjectMotion_PlaceWithinCameraBounds_1(a0, a1, a2, a3) Call4(Func_0200229e, a0, a1, a2, a3)
#define Object_CommitPositionThenWaitIfModeZero_1(args...) Func_020022aa(args)
#define BattleRuntime_WaitIfModeZero_4(args...) Func_020021c0(args)
#define ObjectMotion_Launch_1(args...) Func_02002242(args)
#define ObjectMotion_Launch_2(args...) Func_0200224c(args)
#define BattleEvent_RunActionAndWait_1(args...) Func_0200228c(args)
#define BattleRuntime_WaitIfModeZero_5(args...) Func_020021e2(args)
#define ObjectMotion_PlaceWithinCameraBounds_2(a0, a1, a2, a3) Call4(Func_020022dc, a0, a1, a2, a3)
#define Object_CommitPositionThenWaitIfModeZero_2(args...) Func_020022e8(args)
#define BattleRuntime_WaitIfModeZero_6(args...) Func_020021fe(args)
#define BattleEffect_SpawnLinkedResourceObject_1(a0, a1, a2) Call3(Func_020022e2, a0, a1, a2)
#define ObjectMotion_SetSpeedParameters_3(a0, a1, a2) Call3(Func_02002244, a0, a1, a2)
#define ObjectMotion_SetPositionAndReset_2(a0, a1, a2) Call3(Func_02002272, a0, a1, a2)
#define ObjectMotion_ArmCallback_4(a0, a1, a2) Call3(Func_020022f6, a0, a1, a2)
#define BattleRuntime_WaitIfModeZero_7(args...) Func_02002234(args)
#define ObjectMotion_ArmCallback_5(args...) Func_02002306(args)
#define BattleRuntime_WaitIfModeZero_8(args...) Func_02002244(args)
#define BattleRuntime_WaitIfModeZero_9(args...) Func_0200224a(args)
#define ObjectMotion_CallThenWaitForAnimationChange_1(args...) Func_020022c2(args)
#define BattleRuntime_WaitIfModeZero_10(args...) Func_02002258(args)
#define BattleEvent_RunActionAndWait_2(args...) Func_02002310(args)
#define BattleRuntime_WaitIfModeZero_11(args...) Func_02002266(args)
#define BattleEffect_SpawnLinkedResourceObject_2(a0, a1, a2) Call3(Func_0200234a, a0, a1, a2)
#define BattleRuntime_WaitIfModeZero_12(args...) Func_02002278(args)
#define ObjectMotion_ArmCallback_6(a0, a1, a2) Call3(Func_0200234c, a0, a1, a2)
#define BattleRuntime_WaitIfModeZero_13(args...) Func_0200228a(args)
#define BattleEvent_RunActionAndWait_3(args...) Func_02002342(args)
#define BattleRuntime_WaitIfModeZero_14(args...) Func_02002298(args)
#define ObjectMotion_ArmCallback_7(a0, a1, a2) Call3(Func_0200236c, a0, a1, a2)
#define BattleRuntime_WaitIfModeZero_15(args...) Func_020022aa(args)
#define ObjectMotion_SetSpeedParameters_4(a0, a1, a2) Call3(Func_020022e8, a0, a1, a2)
#define ObjectMotion_SetPositionAndReset_3(a0, a1, a2) Call3(Func_02002316, a0, a1, a2)
#define ObjectMotion_ArmCallback_8(args...) Func_02002398(args)
#define ObjectMotion_ArmCallback_9(a0, a1, a2) Call3(Func_020023a4, a0, a1, a2)
#define ObjectMotion_ArmCallback_10(a0, a1, a2) Call3(Func_020023b0, a0, a1, a2)
#define BattleRuntime_WaitIfModeZero_16(args...) Func_020022ee(args)
#define BattleEvent_RunActionAndWait_4(args...) Func_020023a6(args)
#define BattleRuntime_WaitIfModeZero_17(args...) Func_020022fc(args)
#define ObjectMotion_CallThenWaitForAnimationChange_2(args...) Func_02002374(args)
#define BattleRuntime_WaitIfModeZero_18(args...) Func_0200230a(args)
#define ObjectMotion_CallThenWaitForAnimationChange_3(args...) Func_02002382(args)
#define BattleRuntime_WaitIfModeZero_19(args...) Func_02002318(args)
#define ObjectMotion_SetSpeedParameters_5(a0, a1, a2) Call3(Func_02002352, a0, a1, a2)
#define Object_SetModeById_1(args...) Func_02002392(args)
#define Scene_GetRecord_2(a0) Value1(Func_02002358, a0)
#define ObjectMotion_ResetAndSetPosition_1(args...) Func_02002382(args)
#define ObjectMotion_CommitCurrentPositionAndActivate_2(args...) Func_020023a0(args)
#define ObjectMotion_SetHorizontalPositionWithTerrain_2(args...) Func_020023b2(args)
#define BattleRuntime_WaitIfModeZero_20(args...) ((void (*)())Func_02002358)(args)
#define Scene_GetRecord_3(args...) Func_02002386(args)
#define ObjectMotion_EnableActionAndSetCallback_1(a0, a1) Value2(Func_020023a4, a0, a1)
#define Scene_GetRecord_4(a0) Value1(Func_0200239a, a0)
#define BattleRuntime_ScheduleShoulderButtonModeUpdate_1(args...) Func_020023aa(args)

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

static __inline__ s32 Value2(s32 (*f)(), s32 a0, s32 a1)
{
    return f(a0, a1);
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

/* Runs a fixed sequence of scene calls (positioning, timing, and animation
 * triggers) for scene 3ae, sequence A. */
void FieldScene_RunScene3aeSequenceA(void)
{
    u32 i;
    s32 record;

    BattleRuntime_Reset_1();
    Func_02002230();
    ObjectMotion_SetHorizontalPositionWithTerrain_1(8, 0x1480000, 0x5900000);
    /* Set the flag byte at +91 of record 8. */
    *(u8 *)(Scene_GetRecord_1(8) + 91) = 1;
    BattleRuntime_WaitIfModeZero_1();
    ObjectMotion_SetSpeedParameters_1();
    BattleRuntime_WaitIfModeZero_2(20);
    ObjectMotion_RealignToTrackedObjectAndArmCallback_1(1, -16, 0, 0x8000);
    ObjectMotion_CommitCurrentPositionAndActivate_1(1);
    ObjectMotion_ArmCallback_1(0, 0xa000, 0);
    BattleRuntime_WaitIfModeZero_3(20);
    SceneWork_SetStepValue_1(0x1f89);
    ObjectMotion_ArmCallback_2(0, 0xa000, 0);
    ObjectMotion_SetSpeedParameters_2(1, 0x19999, 0xcccc);
    ObjectMotion_SetPositionAndReset_1(1, 232, 0x590);
    ObjectMotion_ArmCallback_3(1, 0x8000, 0);
    ObjectMotion_PlaceWithinCameraBounds_1(0xb80000, -1, 0x5a00000, 1);
    Object_CommitPositionThenWaitIfModeZero_1();
    BattleRuntime_WaitIfModeZero_4(10);
    ObjectMotion_Launch_1(1, 6, 15);
    ObjectMotion_Launch_2(1, 6, 40);
    BattleEvent_RunActionAndWait_1(1, 0);
    BattleRuntime_WaitIfModeZero_5(20);
    ObjectMotion_PlaceWithinCameraBounds_2(0x1080000, -1, 0x5a80000, 1);
    Object_CommitPositionThenWaitIfModeZero_2();
    BattleRuntime_WaitIfModeZero_6(20);
    BattleEffect_SpawnLinkedResourceObject_1(8, 0x100, 50);
    ObjectMotion_SetSpeedParameters_3(8, 0x13333, 0x9999);
    ObjectMotion_SetPositionAndReset_2(8, 0x108, 0x590);
    ObjectMotion_ArmCallback_4(8, 0x8000, 0);
    BattleRuntime_WaitIfModeZero_7(10);
    ObjectMotion_ArmCallback_5(1, 0, 0);
    BattleRuntime_WaitIfModeZero_8(20);
    BattleRuntime_WaitIfModeZero_9(10);
    ObjectMotion_CallThenWaitForAnimationChange_1(8, 4);
    BattleRuntime_WaitIfModeZero_10(10);
    BattleEvent_RunActionAndWait_2(8, 0);
    BattleRuntime_WaitIfModeZero_11(20);
    BattleEffect_SpawnLinkedResourceObject_2(1, 0x102, 40);
    BattleRuntime_WaitIfModeZero_12(30);
    ObjectMotion_ArmCallback_6(1, 0x8000, 0);
    BattleRuntime_WaitIfModeZero_13(50);
    BattleEvent_RunActionAndWait_3(1, 0);
    BattleRuntime_WaitIfModeZero_14(20);
    ObjectMotion_ArmCallback_7(1, 0x2000, 0);
    BattleRuntime_WaitIfModeZero_15(30);
    ObjectMotion_SetSpeedParameters_4(1, 0x10000, 0x8000);
    ObjectMotion_SetPositionAndReset_3(1, 0x108, 0x5b8);
    ObjectMotion_ArmCallback_8(1, 0, 0);
    ObjectMotion_ArmCallback_9(0, 0x8000, 0);
    ObjectMotion_ArmCallback_10(8, 0x4000, 0);
    BattleRuntime_WaitIfModeZero_16(30);
    BattleEvent_RunActionAndWait_4(1, 0);
    BattleRuntime_WaitIfModeZero_17(10);
    ObjectMotion_CallThenWaitForAnimationChange_2(0, 3);
    BattleRuntime_WaitIfModeZero_18(30);
    ObjectMotion_CallThenWaitForAnimationChange_3(1, 3);
    BattleRuntime_WaitIfModeZero_19(30);
    ObjectMotion_SetSpeedParameters_5(1, 0x13333, 0x9999);
    Object_SetModeById_1(1, 2);
    /* If a record is returned, pass its s16 fields at +10 and +18 back in as
     * arguments. */
    record = Scene_GetRecord_2(0);
    if (record != 0) {
        ObjectMotion_ResetAndSetPosition_1(1, *(s16 *)(record + 10), *(s16 *)(record + 18));
    }
    ObjectMotion_CommitCurrentPositionAndActivate_2(1);
    ObjectMotion_SetHorizontalPositionWithTerrain_2(1, 0, 0);
    /* Same routine as above, called directly instead of through the Value
     * wrapper. */
    BattleRuntime_WaitIfModeZero_20(20);
    /* Clear the flag byte at +91 of record 8. */
    *(u8 *)(Scene_GetRecord_3(8) + 91) = 0;
    ObjectMotion_EnableActionAndSetCallback_1(8, 2);
    record = Scene_GetRecord_4(8);
    /* Store the integer part of the 16.16 fixed-point fields at +8 and +16
     * into the halfwords at +100 and +102. */
    {
        s32 shown = *(s32 *)(record + 8) / 0x10000;

        *(u16 *)(record + 100) = shown;
    }
    {
        s32 shown = *(s32 *)(record + 16) / 0x10000;

        *(u16 *)(record + 102) = shown;
    }
    BattleRuntime_ScheduleShoulderButtonModeUpdate_1();
}
