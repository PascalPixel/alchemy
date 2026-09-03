#include "types.h"

#define FieldScene_RunPrimaryActorSequence Func_02002f14

/* Loader-relocated overlay calls: each symbol names the pre-relocation call
 * word the image holds. */
extern u8 Data_00001c60[]; /* Buffer passed by address to Func_0200918e. */
void Func_02008ed4();
s32 Func_02008f26();
void Func_02008f54();
void Func_02008f80();
void Func_02008f8c();
void Func_02008fe2();
void Func_02008fe8();
void Func_02008ff2();
void Func_02009004();
void Func_0200900a();
void Func_02009014();
void Func_02009016();
void Func_02009034();
void Func_02009042();
void Func_0200904a();
void Func_0200904e();
void Func_02009054();
void Func_02009056();
void Func_0200905a();
void Func_02009080();
void Func_02009082();
void Func_02009088();
void Func_0200909e();
s32 Func_020090aa();
void Func_020090b0();
void Func_020090bc();
void Func_020090c0();
void Func_020090cc();
void Func_020090d0();
void Func_020090d8();
void Func_020090e6();
void Func_020090f4();
void Func_02009100();
void Func_02009108();
void Func_0200910c();
void Func_0200911a();
void Func_0200912a();
void Func_0200913c();
s32 Func_02009140();
void Func_0200914e();
s32 Func_02009152();
void Func_0200915a();
void Func_02009166();
void Func_0200916a();
void Func_02009178();
void Func_0200917a();
void Func_0200918e();
void Func_02009192();
void Func_02009194();
void Func_0200919a();
void Func_020091a0();
void Func_020091a6();
void Func_020091a8();
void Func_020091b0();
void Func_020091ba();
void Func_020091c0();
void Func_020091ea();

/* Resolved engine calls: each pseudo symbol is the per-site call word the
 * overlay image holds (a word can serve two sites with different targets),
 * and the macro names the engine function the site reaches through the
 * overlay veneer and the main-image veneer island, keeping the site's own
 * calling form. Names without a repository binding are provisional.
 */
#define BattleRuntime_Reset_1(args...) Func_02008ed4(args)
#define ObjectMotion_PlaceWithinCameraBounds_1(a0, a1, a2, a3) Call4(Func_0200900a, a0, a1, a2, a3)
#define ObjectMotion_SetPositionAndReset_1(a0, a1, a2) Call3(Func_02008f54, a0, a1, a2)
#define ObjectMotion_ArmCallback_1(a0, a1, a2) Call3(Func_02008fe8, a0, a1, a2)
#define Scene_GetRecord_1(a0) Value1(Func_02008f26, a0)
#define ObjectMotion_SetHorizontalPositionWithTerrain_1(args...) Func_02008f8c(args)
#define ObjectMotion_SetPositionAndReset_2(a0, a1, a2) Call3(Func_02008f80, a0, a1, a2)
#define ObjectMotion_ArmCallback_2(a0, a1, a2) Call3(Func_02009014, a0, a1, a2)
#define SceneWork_SetStepValue_1(a0) Call1(Func_02008ff2, a0)
#define BattleEvent_RunActionAndWait_1(args...) Func_0200900a(args)
#define ObjectMotion_SetVariantCallbackAndInvokeObject_1(args...) Func_02008fe2(args)
#define BattleEffect_SpawnLinkedResourceObject_1(a0, a1, a2) Call3(Func_0200904e, a0, a1, a2)
#define ObjectMotion_ArmCallback_3(a0, a1, a2) Call3(Func_02009042, a0, a1, a2)
#define ObjectMotion_ArmCallback_4(a0, a1, a2) Call3(Func_0200904e, a0, a1, a2)
#define ObjectMotion_ArmCallback_5(a0, a1, a2) Call3(Func_0200905a, a0, a1, a2)
#define BattleRuntime_RunThenWaitIfModeZero_1(args...) Func_02009054(args)
#define Object_SetModeById_1(args...) Func_02009004(args)
#define ObjectMotion_CallThenWaitForAnimationChange_1(args...) Func_02009014(args)
#define ObjectMotion_SetVariantCallbackAndInvokeObject_2(args...) Func_02009034(args)
#define ObjectMotion_ArmCallback_6(a0, a1, a2) Call3(Func_02009088, a0, a1, a2)
#define BattleRuntime_RunThenWaitIfModeZero_2(args...) Func_02009082(args)
#define ObjectMotion_SetVariantCallback_1(args...) Func_0200904a(args)
#define BattleEffect_SpawnLinkedResourceObject_2(a0, a1, a2) Call3(Func_020090bc, a0, a1, a2)
#define BattleRuntime_RunThenWaitIfModeZero_3(args...) Func_0200909e(args)
#define ObjectMotion_CallThenWaitForAnimationChange_2(args...) Func_02009056(args)
#define BattleRuntime_RunThenWaitIfModeZero_4(args...) Func_020090b0(args)
#define ObjectMotion_ArmCallback_7(a0, a1, a2) Call3(Func_020090cc, a0, a1, a2)
#define ObjectMotion_ArmCallback_8(a0, a1, a2) Call3(Func_020090d8, a0, a1, a2)
#define ObjectMotion_CallThenWaitForAnimationChange_3(args...) Func_02009080(args)
#define BattleEvent_RunActionAndWait_2(args...) Func_020090d0(args)
#define ObjectMotion_ArmCallback_9(a0, a1, a2) Call3(Func_020090f4, a0, a1, a2)
#define ObjectMotion_ArmCallback_10(a0, a1, a2) Call3(Func_02009100, a0, a1, a2)
#define ObjectMotion_SetVariantCallbackAndInvokeObject_3(args...) Func_020090c0(args)
#define BattleRuntime_WaitIfModeZero_1(args...) Func_02009016(args)
#define BattleRuntime_RunThenWaitIfModeZero_5(args...) Func_02009108(args)
#define ObjectMotion_CallThenWaitForAnimationChange_4(args...) Func_020090c0(args)
#define BattleRuntime_RunThenWaitIfModeZero_6(args...) Func_0200911a(args)
#define BattleEffect_SpawnLinkedResourceObject_3(a0, a1, a2) Call3(Func_0200914e, a0, a1, a2)
#define BattleEffect_SpawnLinkedResourceObject_4(a0, a1, a2) Call3(Func_0200915a, a0, a1, a2)
#define BattleRuntime_RunThenWaitIfModeZero_7(args...) Func_0200913c(args)
#define ObjectMotion_SetVariantCallbackAndInvokeObject_4(args...) Func_0200910c(args)
#define BattleRuntime_RunThenWaitIfModeZero_8(args...) Func_0200914e(args)
#define ObjectMotion_ArmCallback_11(a0, a1, a2) Call3(Func_0200916a, a0, a1, a2)
#define ObjectMotion_SetSpeedParameters_1(a0, a1) Value2(Func_02009152, a0, a1)
#define UiWork_WaitThenFinalizeCapacity_1(a0, a1) Value2(Func_020090aa, a0, a1)
#define BattleEffect_SpawnLinkedResourceObject_5(a0, a1, a2) Call3(Func_020091a0, a0, a1, a2)
#define BattleRuntime_RunThenWaitIfModeZero_9(args...) Func_02009194(args)
#define ObjectMotion_ArmCallback_12(a0, a1, a2) Call3(Func_020091b0, a0, a1, a2)
#define SceneWork_SetStepValue_2(args...) Func_0200918e(args)
#define BattleEvent_RunActionAndWait_3(args...) Func_020091a6(args)
#define ObjectMotion_CallThenWaitForAnimationChange_5(args...) Func_02009166(args)
#define BattleRuntime_RunThenWaitIfModeZero_10(args...) Func_020091c0(args)
#define ObjectMotion_CallThenWaitForAnimationChange_6(args...) Func_02009178(args)
#define BattleRuntime_WaitIfModeZero_2(args...) Func_020090e6(args)
#define ObjectMotion_ArmCallback_13(a0, a1, a2) Call3(Func_020091ea, a0, a1, a2)
#define ObjectMotion_CallThenWaitForAnimationChange_7(args...) Func_02009192(args)
#define ObjectMotion_CallThenWaitForAnimationChange_8(args...) Func_0200919a(args)
#define Object_SetModeById_2(args...) Func_0200919a(args)
#define Scene_GetRecord_2(a0) Value1(Func_02009140, a0)
#define ObjectMotion_ResetAndSetPosition_1(args...) Func_0200917a(args)
#define ObjectMotion_CommitCurrentPositionAndActivate_1(args...) Func_020091a8(args)
#define ObjectMotion_SetHorizontalPositionWithTerrain_2(args...) Func_020091ba(args)
#define GameFlag_Set_1(a0) Call1(Func_0200912a, a0)
#define BattleRuntime_ScheduleShoulderButtonModeUpdate_1(args...) Func_0200914e(args)

/* Newly identified engine calls: each macro below names the site's own
 * engine function from its reconstructed C source, and keeps the site's
 * own calling form. */

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
    u8 *work = *(u8 **)0x03001ebc;

    *(u16 *)(work + 0x1d8) = (u16)(*(u16 *)(work + 0x1d8) + amount);
}

/* Runs the actor 0 / 1 / 9 setup sequence: position, speed and animation
 * calls in a fixed order. Two steps look up a record for one actor and
 * copy a coordinate pair out of it (offsets +8/+16 as s32, then +10/+18
 * as s16) into the call configuring another actor. */
void FieldScene_RunPrimaryActorSequence(void)
{
    u32 i;
    s32 record;

    BattleRuntime_Reset_1();
    ObjectMotion_PlaceWithinCameraBounds_1(0x1650000, -1, 0x2e20000, 1);
    ObjectMotion_SetPositionAndReset_1(0, 0x16f, 0x2e9);
    ObjectMotion_ArmCallback_1(0, 0xa000, 0);
    record = Scene_GetRecord_1(0);
    if (record != 0) {
        /* Copy the s32 coordinate pair at +8/+16 of the looked-up record. */
        ObjectMotion_SetHorizontalPositionWithTerrain_1(1, *(s32 *)(record + 8), *(s32 *)(record + 16));
    }
    ObjectMotion_SetPositionAndReset_2(1, 0x15a, 0x2e9);
    ObjectMotion_ArmCallback_2(1, 0xd000, 20);
    SceneWork_SetStepValue_1(0x1c53);
    BattleEvent_RunActionAndWait_1(1, 0);
    ObjectMotion_SetVariantCallbackAndInvokeObject_1(9, 2);
    BattleEffect_SpawnLinkedResourceObject_1(9, 0x100, 0);
    ObjectMotion_ArmCallback_3(9, 0x3000, 10);
    ObjectMotion_ArmCallback_4(9, 0x5000, 10);
    ObjectMotion_ArmCallback_5(9, 0x3000, 40);
    BattleRuntime_RunThenWaitIfModeZero_1(9, 0, 20);
    Object_SetModeById_1(0, 3);
    ObjectMotion_CallThenWaitForAnimationChange_1(1, 3);
    ObjectMotion_SetVariantCallbackAndInvokeObject_2(9, 1);
    ObjectMotion_ArmCallback_6(9, 0x5000, 10);
    BattleRuntime_RunThenWaitIfModeZero_2(9, 0, 20);
    ObjectMotion_SetVariantCallback_1(1, 1);
    BattleEffect_SpawnLinkedResourceObject_2(1, 0x103, 40);
    BattleRuntime_RunThenWaitIfModeZero_3(1, 0, 10);
    ObjectMotion_CallThenWaitForAnimationChange_2(9, 3);
    BattleRuntime_RunThenWaitIfModeZero_4(9, 0, 20);
    ObjectMotion_ArmCallback_7(0, 0x8000, 0);
    ObjectMotion_ArmCallback_8(1, 0x1000, 40);
    ObjectMotion_CallThenWaitForAnimationChange_3(9, 4);
    BattleEvent_RunActionAndWait_2(9, 0);
    ObjectMotion_ArmCallback_9(0, 0xb000, 0);
    ObjectMotion_ArmCallback_10(1, 0xd000, 10);
    ObjectMotion_SetVariantCallbackAndInvokeObject_3(1, 2);
    BattleRuntime_WaitIfModeZero_1(20);
    BattleRuntime_RunThenWaitIfModeZero_5(1, 0, 10);
    ObjectMotion_CallThenWaitForAnimationChange_4(9, 3);
    BattleRuntime_RunThenWaitIfModeZero_6(9, 0, 20);
    BattleEffect_SpawnLinkedResourceObject_3(0, 0x102, 0);
    BattleEffect_SpawnLinkedResourceObject_4(1, 0x102, 80);
    BattleRuntime_RunThenWaitIfModeZero_7(1, 0, 20);
    ObjectMotion_SetVariantCallbackAndInvokeObject_4(9, 2);
    BattleRuntime_RunThenWaitIfModeZero_8(9, 0, 20);
    ObjectMotion_ArmCallback_11(1, 0x1000, 20);
    ObjectMotion_SetSpeedParameters_1(1, 0); /* main:0808a178 */
    if (UiWork_WaitThenFinalizeCapacity_1(0, 0) == 0) {
        BattleEffect_SpawnLinkedResourceObject_5(1, 0x105, 60);
    } else {
        bump_step(1);
    }
    BattleRuntime_RunThenWaitIfModeZero_9(1, 0, 20);
    ObjectMotion_ArmCallback_12(1, 0xd000, 10);
    SceneWork_SetStepValue_2((s32)Data_00001c60);
    BattleEvent_RunActionAndWait_3(1, 0);
    ObjectMotion_CallThenWaitForAnimationChange_5(9, 3);
    BattleRuntime_RunThenWaitIfModeZero_10(9, 0, 20);
    ObjectMotion_CallThenWaitForAnimationChange_6(1, 3);
    BattleRuntime_WaitIfModeZero_2(20);
    ObjectMotion_ArmCallback_13(1, 0x1000, 20);
    ObjectMotion_CallThenWaitForAnimationChange_7(1, 3);
    ObjectMotion_CallThenWaitForAnimationChange_8(0, 3);
    Object_SetModeById_2(1, 2);
    record = Scene_GetRecord_2(0);
    if (record != 0) {
        /* Copy the s16 coordinate pair at +10/+18 of the looked-up record. */
        ObjectMotion_ResetAndSetPosition_1(1, *(s16 *)(record + 10), *(s16 *)(record + 18));
    }
    ObjectMotion_CommitCurrentPositionAndActivate_1(1);
    ObjectMotion_SetHorizontalPositionWithTerrain_2(1, 0, 0);
    GameFlag_Set_1(0x304);
    BattleRuntime_ScheduleShoulderButtonModeUpdate_1();
}
