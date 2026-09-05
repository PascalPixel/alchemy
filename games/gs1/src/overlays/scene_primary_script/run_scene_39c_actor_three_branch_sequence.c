#include "types.h"

/* Loader-relocated overlay calls: each symbol names the pre-relocation call
 * word the image holds. */
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
void Func_02008270();
void Func_0200827e();
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

/* Resolved engine calls: each pseudo symbol is the per-site call word the
 * overlay image holds (a word can serve two sites with different targets),
 * and the macro names the engine function the site reaches through the
 * overlay veneer and the main-image veneer island, keeping the site's own
 * calling form. Names without a repository binding are provisional.
 */
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
#define ObjectMotion_CallThenWaitForAnimationChange_1(args...) Func_02008270(args)
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
#define BattleRuntime_WaitIfModeZero_6(args...) Func_02008270(args)
#define ObjectMotion_SetVariantCallbackAndInvokeObject_1(args...) Func_02008318(args)
#define BattleRuntime_WaitIfModeZero_7(args...) Func_0200827e(args)
#define ObjectMotion_CallThenWaitForAnimationChange_3(args...) Func_0200830e(args)
#define BattleRuntime_WaitIfModeZero_8(args...) Func_0200828c(args)
#define BattleRuntime_RunThenWaitIfModeZero_5(args...) Func_02008376(args)
#define GameFlag_Set_1(a0) Call1(Func_0200827e, a0)
#define BattleRuntime_ScheduleShoulderButtonModeUpdate_1(args...) Func_020082b2(args)

/* Call sites spelled through these wrappers pass their constants straight
 * into the argument registers; a direct call precomputes a costly constant
 * into a pseudo that the compiler then shares with later uses in the block.
 * A value-returning call also sets r0 last of its arguments. */

static __inline__ void Call1(void (*f)(), s32 a0)
{
    f(a0);
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

static __inline__ void Call4(void (*f)(), s32 a0, s32 a1, s32 a2, s32 a3)
{
    f(a0, a1, a2, a3);
}

/* Pointer to the shared scene work record. */
#define SCENE_WORK (*(u8 **)0x03001ebc)

/* The scene step counter at 0x1d8 of the shared scene work record. */
static __inline__ void bump_step(s32 amount)
{
    u8 *work = SCENE_WORK;

    *(u16 *)(work + 0x1d8) = (u16)(*(u16 *)(work + 0x1d8) + amount);
}

/* Values below that look like 16.16 fixed-point fractions, named by that
 * fraction's decimal value. */
#define FX16_0_1 0x1999
#define FX16_0_25 0x4000
#define FX16_0_4 0x6666
#define FX16_0_5 0x8000
#define FX16_0_75 0xc000
#define FX16_0_8 0xcccc
#define FX16_1_0 0x10000

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
