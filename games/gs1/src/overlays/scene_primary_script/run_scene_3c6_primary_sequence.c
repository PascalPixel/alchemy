#include "types.h"

#define FieldScene_RunPrimarySequence Func_02000218

/* Loader-relocated overlay calls: each symbol names the pre-relocation call
 * word the image holds. */
void Func_0200174c();
void Func_02001782();
void Func_0200179e();
void Func_020017c0();
void Func_020017ca();
void Func_020017dc();
void Func_020017e4();
void Func_020017e8();
void Func_020017ea();
void Func_020017ee();
void Func_020017f8();
void Func_020017fc();
void Func_0200180a();
void Func_0200181c();
void Func_02001832();
void Func_02001840();
void Func_0200184c();
void Func_0200184e();
void Func_02001850();
void Func_0200185c();
void Func_02001864();
void Func_02001866();
void Func_0200186e();
void Func_02001872();
void Func_02001878();
void Func_0200187c();
void Func_0200187e();
void Func_02001882();
void Func_0200188a();
void Func_02001898();
void Func_020018a6();
void Func_020018b4();
void Func_020018c2();
void Func_020018c4();
s32 Func_020018cc();
void Func_020018ce();
void Func_020018d0();
void Func_020018e6();
void Func_020018e8();
void Func_020018f2();
void Func_020018f8();
void Func_0200190c();
void Func_02001918();
void Func_0200191a();
void Func_0200191e();
void Func_02001920();
void Func_02001932();
void Func_02001940();
void Func_02001942();
void Func_02001944();
void Func_0200194e();
void Func_0200195c();
void Func_0200195e();
void Func_0200196a();
void Func_02001974();
void Func_0200197a();
void Func_0200198c();
void Func_0200198e();
void Func_0200199c();
void Func_020019aa();
void Func_020019b6();
void Func_020019bc();
void Func_020019c2();
void Func_020019c6();
void Func_020019ca();
void Func_020019d8();
void Func_020019de();
void Func_020019e4();
void Func_020019f8();
void Func_02001a08();
void Func_02001a10();
void Func_02001a14();
void Func_02001a18();
void Func_02001a1e();
void Func_02001a24();
void Func_02001a2a();
void Func_02001a38();
void Func_02001a3c();
void Func_02001a3e();
void Func_02001a4a();
void Func_02001a54();
void Func_02001a58();
void Func_02001a66();
void Func_02001a6e();
void Func_02001a74();
void Func_02001a84();
void Func_02001a92();
void Func_02001aa0();
void Func_02001aae();
void Func_02001ab2();
void Func_02001abc();
void Func_02001aca();
void Func_02001ada();
void Func_02001adc();
void Func_02001ae4();
void Func_02001aee();
void Func_02001af2();
void Func_02001af4();
void Func_02001af6();
void Func_02001b00();
void Func_02001b0e();
void Func_02001b10();
void Func_02001b12();
void Func_02001b1c();
void Func_02001b20();
void Func_02001b2e();
void Func_02001b36();
void Func_02001b3a();
void Func_02001b3c();
void Func_02001b3e();
void Func_02001b58();
void Func_02001b6c();
void Func_02001b7a();
void Func_02001b82();
void Func_02001b8c();
void Func_02001b96();
void Func_02001b9c();
void Func_02001ba0();
void Func_02001bb6();
void Func_02001bb8();
void Func_02001bc4();
void Func_02001bd2();
void Func_02001be0();
void Func_02001be6();
void Func_02001bee();
void Func_02001c08();
void Func_02001c16();
void Func_02001c24();
void Func_02001c2a();
void Func_02001c34();
void Func_02001c46();
void Func_02001c4c();
void Func_02001c4e();
void Func_02001c58();
void Func_02001c5e();
void Func_02001c62();
void Func_02001c66();
void Func_02001c68();
void Func_02001c6c();
void Func_02001c7a();
void Func_02001c7c();
void Func_02001c88();
void Func_02001c92();
void Func_02001c96();
void Func_02001c98();
void Func_02001cb0();
void Func_02001cca();
void Func_02001cce();
void Func_02001cd0();
u8 *Scene_GetRecord_1();
void Func_02001ce8();
void Func_02001cea();
void Func_02001cee();
void Func_02001cfa();
void Func_02001cfe();
void Func_02001d00();
void Func_02001d08();
void Func_02001d10();
void Func_02001d18();
void Func_02001d24();
void Func_02001d26();
void Func_02001d38();

/* Resolved engine calls: each pseudo symbol is the per-site call word the
 * overlay image holds (a word can serve two sites with different targets),
 * and the macro names the engine function the site reaches through the
 * overlay veneer and the main-image veneer island, keeping the site's own
 * calling form. Names without a repository binding are provisional.
 */
#define BattleRuntime_Reset_1(args...) Func_0200174c(args)
#define SceneWork_SetStepValue_1(a0) Call1(Func_020017ea, a0)
#define ObjectMotion_SetHorizontalPositionWithTerrain_1(a0, a1, a2) Call3(Func_020017c0, a0, a1, a2)
#define ObjectMotion_ArmCallback_1(a0, a1, a2) Call3(Func_0200181c, a0, a1, a2)
#define Object_SetModeById_1(args...) Func_020017dc(args)
#define Object_SetModeById_2(args...) Func_020017e4(args)
#define BattleRuntime_WaitIfModeZero_1(args...) Func_02001850(args)
#define ObjectMotion_SetSpeedParameters_1(args...) Func_0200185c(args)
#define BattleRuntime_WaitIfModeZero_2(args...) Func_02001782(args)
#define ObjectMotion_RealignToTrackedObjectAndArmCallback_1(a0, a1, a2, a3) Call4(Func_02001882, a0, a1, a2, a3)
#define ObjectMotion_CommitCurrentPositionAndActivate_1(args...) Func_020017f8(args)
#define BattleRuntime_WaitIfModeZero_3(args...) Func_0200179e(args)
#define BattleEffect_SpawnLinkedResourceObject_1(a0, a1, a2) Call3(Func_02001872, a0, a1, a2)
#define ObjectMotion_SetSpeedParameters_2(a0, a1, a2) Call3(Func_020017e8, a0, a1, a2)
#define ObjectMotion_CommitPositionAndActivate_1(a0, a1, a2) Call3(Func_020018c4, a0, a1, a2)
#define BattleRuntime_WaitIfModeZero_4(args...) Func_020017ca(args)
#define ObjectMotion_SetAngleToward_1(args...) Func_02001864(args)
#define ObjectMotion_SetAngleToward_2(args...) Func_0200186e(args)
#define ObjectMotion_SetAngleToward_3(args...) Func_02001878(args)
#define BattleRuntime_WaitIfModeZero_5(args...) Func_020017ee(args)
#define ObjectMotion_SetVariantCallbackAndInvokeObject_1(args...) Func_0200187e(args)
#define BattleRuntime_WaitIfModeZero_6(args...) Func_020017fc(args)
#define BattleEvent_RunActionAndWait_1(args...) Func_020018b4(args)
#define BattleRuntime_WaitIfModeZero_7(args...) Func_0200180a(args)
#define ObjectMotion_ArmCallback_2(a0, a1, a2) Call3(Func_020018ce, a0, a1, a2)
#define BattleRuntime_WaitIfModeZero_8(args...) Func_0200181c(args)
#define ObjectMotion_SetSpeedParameters_3(a0, a1) Value2(Func_020018cc, a0, a1)
#define UiWork_WaitThenFinalizeCapacity_1(args...) Func_0200184c(args)
#define BattleRuntime_WaitIfModeZero_9(args...) Func_02001832(args)
#define ObjectMotion_SetVariantCallbackAndInvokeObject_2(args...) Func_020018c2(args)
#define BattleRuntime_WaitIfModeZero_10(args...) Func_02001840(args)
#define BattleEvent_RunActionAndWait_2(args...) Func_020018f8(args)
#define BattleRuntime_WaitIfModeZero_11(args...) Func_0200184e(args)
#define ObjectMotion_SetAngleToward_4(args...) Func_020018e8(args)
#define ObjectMotion_SetVariantCallbackAndInvokeObject_3(args...) Func_020018e8(args)
#define BattleRuntime_WaitIfModeZero_12(args...) Func_02001866(args)
#define BattleEvent_RunActionAndWait_3(args...) Func_0200191e(args)
#define Object_SetModeById_3(args...) Func_020018e6(args)
#define BattleRuntime_WaitIfModeZero_13(args...) Func_0200187c(args)
#define ObjectMotion_SetVariantCallbackAndInvokeObject_4(args...) Func_0200190c(args)
#define BattleRuntime_WaitIfModeZero_14(args...) Func_0200188a(args)
#define BattleEvent_RunActionAndWait_4(args...) Func_02001942(args)
#define BattleRuntime_WaitIfModeZero_15(args...) Func_02001898(args)
#define ObjectMotion_CallThenWaitForAnimationChange_1(args...) Func_02001918(args)
#define BattleRuntime_WaitIfModeZero_16(args...) Func_020018a6(args)
#define BattleEvent_RunActionAndWait_5(args...) Func_0200195e(args)
#define BattleRuntime_WaitIfModeZero_17(args...) Func_020018b4(args)
#define ObjectMotion_SetVariantCallbackAndInvokeObject_5(args...) Func_02001944(args)
#define BattleRuntime_WaitIfModeZero_18(args...) Func_020018c2(args)
#define BattleEvent_RunActionAndWait_6(args...) Func_0200197a(args)
#define BattleRuntime_WaitIfModeZero_19(args...) Func_020018d0(args)
#define ObjectMotion_SetAngleToward_5(args...) Func_0200196a(args)
#define ObjectMotion_SetAngleToward_6(args...) Func_02001974(args)
#define Object_SetModeById_4(args...) Func_0200195c(args)
#define BattleRuntime_WaitIfModeZero_20(args...) Func_020018f2(args)
#define BattleEffect_SpawnLinkedResourceObject_2(a0, a1, a2) Call3(Func_020019c6, a0, a1, a2)
#define BattleEvent_RunActionAndWait_7(args...) Func_020019b6(args)
#define BattleRuntime_WaitIfModeZero_21(args...) Func_0200190c(args)
#define ObjectMotion_CallThenWaitForAnimationChange_2(args...) Func_0200198c(args)
#define BattleRuntime_WaitIfModeZero_22(args...) Func_0200191a(args)
#define BattleRuntime_WaitIfModeZero_23(args...) Func_02001920(args)
#define ObjectMotion_ArmCallback_3(a0, a1, a2) Call3(Func_020019e4, a0, a1, a2)
#define BattleRuntime_WaitIfModeZero_24(args...) Func_02001932(args)
#define ObjectMotion_SetVariantCallbackAndInvokeObject_6(args...) Func_020019c2(args)
#define BattleRuntime_WaitIfModeZero_25(args...) Func_02001940(args)
#define BattleEvent_RunActionAndWait_8(args...) Func_020019f8(args)
#define BattleRuntime_WaitIfModeZero_26(args...) Func_0200194e(args)
#define ObjectMotion_SetVariantCallbackAndInvokeObject_7(args...) Func_020019de(args)
#define BattleRuntime_WaitIfModeZero_27(args...) Func_0200195c(args)
#define BattleEvent_RunActionAndWait_9(args...) Func_02001a14(args)
#define BattleRuntime_WaitIfModeZero_28(args...) Func_0200196a(args)
#define BattleEffect_SpawnLinkedResourceObject_3(a0, a1, a2) Call3(Func_02001a3e, a0, a1, a2)
#define ObjectMotion_SetAngleToward_7(args...) Func_02001a10(args)
#define BattleEvent_RunActionAndWait_10(args...) Func_02001a38(args)
#define BattleRuntime_WaitIfModeZero_29(args...) Func_0200198e(args)
#define ObjectMotion_SetVariantCallbackAndInvokeObject_8(args...) Func_02001a1e(args)
#define BattleRuntime_WaitIfModeZero_30(args...) Func_0200199c(args)
#define BattleEvent_RunActionAndWait_11(args...) Func_02001a54(args)
#define BattleRuntime_WaitIfModeZero_31(args...) Func_020019aa(args)
#define ObjectMotion_ArmCallback_4(a0, a1, a2) Call3(Func_02001a6e, a0, a1, a2)
#define BattleRuntime_WaitIfModeZero_32(args...) Func_020019bc(args)
#define BattleEvent_RunActionAndWait_12(args...) Func_02001a74(args)
#define BattleRuntime_WaitIfModeZero_33(args...) Func_020019ca(args)
#define ObjectMotion_CallThenWaitForAnimationChange_3(args...) Func_02001a4a(args)
#define BattleRuntime_WaitIfModeZero_34(args...) Func_020019d8(args)
#define BattleRuntime_WaitIfModeZero_35(args...) Func_020019de(args)
#define BattleEffect_SpawnLinkedResourceObject_4(a0, a1, a2) Call3(Func_02001ab2, a0, a1, a2)
#define ObjectMotion_SetSpeedParameters_4(a0, a1, a2) Call3(Func_02001a24, a0, a1, a2)
#define ObjectMotion_SetPositionAndReset_1(a0, a1, a2) Call3(Func_02001a4a, a0, a1, a2)
#define BattleRuntime_WaitIfModeZero_36(args...) Func_02001a08(args)
#define ObjectMotion_ArmCallback_5(args...) Func_02001aca(args)
#define BattleRuntime_WaitIfModeZero_37(args...) Func_02001a18(args)
#define ObjectMotion_ArmCallback_6(a0, a1, a2) Call3(Func_02001adc, a0, a1, a2)
#define BattleRuntime_WaitIfModeZero_38(args...) Func_02001a2a(args)
#define ObjectMotion_ArmCallback_7(a0, a1, a2) Call3(Func_02001aee, a0, a1, a2)
#define BattleRuntime_WaitIfModeZero_39(args...) Func_02001a3c(args)
#define BattleEvent_RunActionAndWait_13(args...) Func_02001af4(args)
#define BattleRuntime_WaitIfModeZero_40(args...) Func_02001a4a(args)
#define ObjectMotion_SetVariantCallbackAndInvokeObject_9(args...) Func_02001ada(args)
#define BattleRuntime_WaitIfModeZero_41(args...) Func_02001a58(args)
#define BattleEvent_RunActionAndWait_14(args...) Func_02001b10(args)
#define BattleRuntime_WaitIfModeZero_42(args...) Func_02001a66(args)
#define BattleEffect_SpawnLinkedResourceObject_5(a0, a1, a2) Call3(Func_02001b3a, a0, a1, a2)
#define ObjectMotion_ArmCallback_8(a0, a1, a2) Call3(Func_02001b36, a0, a1, a2)
#define BattleRuntime_WaitIfModeZero_43(args...) Func_02001a84(args)
#define BattleEvent_RunActionAndWait_15(args...) Func_02001b3c(args)
#define BattleRuntime_WaitIfModeZero_44(args...) Func_02001a92(args)
#define ObjectMotion_CallThenWaitForAnimationChange_4(args...) Func_02001b12(args)
#define BattleRuntime_WaitIfModeZero_45(args...) Func_02001aa0(args)
#define BattleEvent_RunActionAndWait_16(args...) Func_02001b58(args)
#define BattleRuntime_WaitIfModeZero_46(args...) Func_02001aae(args)
#define ObjectMotion_SetVariantCallbackAndInvokeObject_10(args...) Func_02001b3e(args)
#define BattleRuntime_WaitIfModeZero_47(args...) Func_02001abc(args)
#define ObjectMotion_SetSpeedParameters_5(a0, a1, a2) Call3(Func_02001af6, a0, a1, a2)
#define ObjectMotion_SetPositionAndReset_2(a0, a1, a2) Call3(Func_02001b1c, a0, a1, a2)
#define ObjectMotion_ArmCallback_9(args...) Func_02001b96(args)
#define BattleRuntime_WaitIfModeZero_48(args...) Func_02001ae4(args)
#define BattleEvent_RunActionAndWait_17(args...) Func_02001b9c(args)
#define BattleRuntime_WaitIfModeZero_49(args...) Func_02001af2(args)
#define ObjectMotion_SetVariantCallbackAndInvokeObject_11(args...) Func_02001b82(args)
#define BattleRuntime_WaitIfModeZero_50(args...) Func_02001b00(args)
#define BattleEvent_RunActionAndWait_18(args...) Func_02001bb8(args)
#define BattleRuntime_WaitIfModeZero_51(args...) Func_02001b0e(args)
#define ObjectMotion_ArmCallback_10(a0, a1, a2) Call3(Func_02001bd2, a0, a1, a2)
#define BattleRuntime_WaitIfModeZero_52(args...) Func_02001b20(args)
#define ObjectMotion_CallThenWaitForAnimationChange_5(args...) Func_02001ba0(args)
#define BattleRuntime_WaitIfModeZero_53(args...) Func_02001b2e(args)
#define BattleEvent_RunActionAndWait_19(args...) Func_02001be6(args)
#define BattleRuntime_WaitIfModeZero_54(args...) Func_02001b3c(args)
#define ObjectMotion_SetSpeedParameters_6(a0, a1, a2) Call3(Func_02001b8c, a0, a1, a2)
#define ObjectMotion_CommitPositionAndActivate_2(args...) Func_02001c66(args)
#define BattleRuntime_WaitIfModeZero_55(args...) Func_02001b6c(args)
#define BattleEvent_RunActionAndWait_20(args...) Func_02001c24(args)
#define BattleRuntime_WaitIfModeZero_56(args...) Func_02001b7a(args)
#define BattleEffect_SpawnLinkedResourceObject_6(a0, a1, a2) Call3(Func_02001c4e, a0, a1, a2)
#define BattleEffect_SpawnLinkedResourceObject_7(a0, a1, a2) Call3(Func_02001c58, a0, a1, a2)
#define ObjectMotion_SetAngleToward_8(args...) Func_02001c2a(args)
#define ObjectMotion_SetAngleToward_9(args...) Func_02001c34(args)
#define ObjectMotion_ArmCallback_11(a0, a1, a2) Call3(Func_02001c68, a0, a1, a2)
#define BattleRuntime_WaitIfModeZero_57(args...) Func_02001bb6(args)
#define ObjectMotion_SetVariantCallbackAndInvokeObject_12(args...) Func_02001c46(args)
#define BattleRuntime_WaitIfModeZero_58(args...) Func_02001bc4(args)
#define BattleEvent_RunActionAndWait_21(args...) Func_02001c7c(args)
#define BattleRuntime_WaitIfModeZero_59(args...) Func_02001bd2(args)
#define ObjectMotion_SetVariantCallbackAndInvokeObject_13(args...) Func_02001c62(args)
#define BattleRuntime_WaitIfModeZero_60(args...) Func_02001be0(args)
#define BattleEvent_RunActionAndWait_22(args...) Func_02001c98(args)
#define BattleRuntime_WaitIfModeZero_61(args...) Func_02001bee(args)
#define ObjectMotion_ArmCallback_12(args...) Func_02001cb0(args)
#define ObjectMotion_SetAngleToward_10(args...) Func_02001c92(args)
#define BattleRuntime_WaitIfModeZero_62(args...) Func_02001c08(args)
#define ObjectMotion_SetVariantCallbackAndInvokeObject_14(args...) Func_02001c98(args)
#define BattleRuntime_WaitIfModeZero_63(args...) Func_02001c16(args)
#define BattleEvent_RunActionAndWait_23(args...) Func_02001cce(args)
#define BattleRuntime_WaitIfModeZero_64(args...) Func_02001c24(args)
#define ObjectMotion_ArmCallback_13(a0, a1, a2) Call3(Func_02001ce8, a0, a1, a2)
#define ObjectMotion_SetAngleToward_11(args...) Func_02001cca(args)
#define ObjectMotion_ArmCallback_14(a0, a1, a2) Call3(Func_02001cfe, a0, a1, a2)
#define BattleRuntime_WaitIfModeZero_65(args...) Func_02001c4c(args)
#define ObjectMotion_ArmCallback_15(a0, a1, a2) Call3(Func_02001d10, a0, a1, a2)
#define BattleRuntime_WaitIfModeZero_66(args...) Func_02001c5e(args)
#define ObjectMotion_SetVariantCallbackAndInvokeObject_15(args...) Func_02001cee(args)
#define BattleRuntime_WaitIfModeZero_67(args...) Func_02001c6c(args)
#define BattleEvent_RunActionAndWait_24(args...) Func_02001d24(args)
#define BattleRuntime_WaitIfModeZero_68(args...) Func_02001c7a(args)
#define ObjectMotion_CallThenWaitForAnimationChange_6(args...) Func_02001cfa(args)
#define BattleRuntime_WaitIfModeZero_69(args...) Func_02001c88(args)
#define ObjectMotion_CallThenWaitForAnimationChange_7(args...) Func_02001d08(args)
#define BattleRuntime_WaitIfModeZero_70(args...) Func_02001c96(args)
#define ObjectMotion_SetSpeedParameters_7(a0, a1, a2) Call3(Func_02001cd0, a0, a1, a2)
#define Object_SetModeById_5(args...) Func_02001d18(args)
#define Scene_GetRecord_1(args...) Func_02001cd6(args)
#define ObjectMotion_ResetAndSetPosition_1(args...) Func_02001d00(args)
#define ObjectMotion_CommitCurrentPositionAndActivate_2(args...) Func_02001d26(args)
#define ObjectMotion_SetHorizontalPositionWithTerrain_2(args...) Func_02001d38(args)
#define BattleRuntime_WaitIfModeZero_71(args...) Func_02001cd6(args)
#define BattleRuntime_ScheduleShoulderButtonModeUpdate_1(args...) Func_02001cea(args)
u8 *Func_02001cd6();
u8 *Func_02001cd6();

/* Remaining raw call sites, named individually from the engine source their
 * main-image function reaches. */

/* Call sites spelled through these wrappers pass their constants straight
 * into the argument registers; a direct call precomputes a costly constant
 * into a pseudo that the compiler then shares with later uses in the block.
 * A value-returning call also sets r0 last of its arguments. */

static __inline__ void Call1(void (*f)(), s32 a0)
{
    f(a0);
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

/* Runs the fixed call sequence for scene 3c6: a long, paced series of setup
 * and per-entity calls (position/pose/property triples keyed by entity id,
 * interleaved with timed single-argument steps), ending with a lookup of a
 * record whose s16 fields at +10 and +18 feed the last positioning call. */
void FieldScene_RunPrimarySequence(void)
{
    u32 i;
    u8 *record;

    BattleRuntime_Reset_1();
    SceneWork_SetStepValue_1(0x2694);
    ObjectMotion_SetHorizontalPositionWithTerrain_1(0, 0xf80000, 0x1a80000);
    ObjectMotion_ArmCallback_1(0, 0xc000, 0);
    Object_SetModeById_1(8, 0);
    Object_SetModeById_2(9, 0);
    BattleRuntime_WaitIfModeZero_1(); /* main:0808a360 */
    ObjectMotion_SetSpeedParameters_1(); /* main:0808a370 */
    BattleRuntime_WaitIfModeZero_2(20);
    ObjectMotion_RealignToTrackedObjectAndArmCallback_1(22, 8, -16, 0xc000);
    ObjectMotion_CommitCurrentPositionAndActivate_1(22);
    BattleRuntime_WaitIfModeZero_3(20);
    BattleEffect_SpawnLinkedResourceObject_1(22, 0x102, 60);
    ObjectMotion_SetSpeedParameters_2(22, 0x10000, 0x8000);
    ObjectMotion_CommitPositionAndActivate_1(22, 0, -16);
    BattleRuntime_WaitIfModeZero_4(10);
    ObjectMotion_SetAngleToward_1(22, 8, 40);
    ObjectMotion_SetAngleToward_2(22, 9, 40);
    ObjectMotion_SetAngleToward_3(22, 8, 40);
    BattleRuntime_WaitIfModeZero_5(10);
    ObjectMotion_SetVariantCallbackAndInvokeObject_1(22, 2);
    BattleRuntime_WaitIfModeZero_6(20);
    BattleEvent_RunActionAndWait_1(22, 0);
    BattleRuntime_WaitIfModeZero_7(10);
    ObjectMotion_ArmCallback_2(22, 0x4000, 0);
    BattleRuntime_WaitIfModeZero_8(30);
    ObjectMotion_SetSpeedParameters_3(22, 0); /* main:0808a178 */
    UiWork_WaitThenFinalizeCapacity_1(0, 0);
    BattleRuntime_WaitIfModeZero_9(30);
    ObjectMotion_SetVariantCallbackAndInvokeObject_2(8, 2);
    BattleRuntime_WaitIfModeZero_10(20);
    BattleEvent_RunActionAndWait_2(8, 0);
    BattleRuntime_WaitIfModeZero_11(10);
    ObjectMotion_SetAngleToward_4(22, 8, 30);
    ObjectMotion_SetVariantCallbackAndInvokeObject_3(22, 2);
    BattleRuntime_WaitIfModeZero_12(20);
    BattleEvent_RunActionAndWait_3(22, 0);
    Object_SetModeById_3(8, 1);
    BattleRuntime_WaitIfModeZero_13(20);
    ObjectMotion_SetVariantCallbackAndInvokeObject_4(8, 2);
    BattleRuntime_WaitIfModeZero_14(30);
    BattleEvent_RunActionAndWait_4(8, 0);
    BattleRuntime_WaitIfModeZero_15(10);
    ObjectMotion_CallThenWaitForAnimationChange_1(22, 3);
    BattleRuntime_WaitIfModeZero_16(20);
    BattleEvent_RunActionAndWait_5(22, 0);
    BattleRuntime_WaitIfModeZero_17(10);
    ObjectMotion_SetVariantCallbackAndInvokeObject_5(9, 2);
    BattleRuntime_WaitIfModeZero_18(20);
    BattleEvent_RunActionAndWait_6(9, 0);
    BattleRuntime_WaitIfModeZero_19(10);
    ObjectMotion_SetAngleToward_5(22, 9, 0);
    ObjectMotion_SetAngleToward_6(0, 9, 30);
    Object_SetModeById_4(9, 1);
    BattleRuntime_WaitIfModeZero_20(10);
    BattleEffect_SpawnLinkedResourceObject_2(22, 0x108, 40);
    BattleEvent_RunActionAndWait_7(22, 0);
    BattleRuntime_WaitIfModeZero_21(10);
    ObjectMotion_CallThenWaitForAnimationChange_2(0, 3);
    BattleRuntime_WaitIfModeZero_22(20);
    BattleRuntime_WaitIfModeZero_23(10);
    ObjectMotion_ArmCallback_3(22, 0x4000, 0);
    BattleRuntime_WaitIfModeZero_24(30);
    ObjectMotion_SetVariantCallbackAndInvokeObject_6(22, 2);
    BattleRuntime_WaitIfModeZero_25(20);
    BattleEvent_RunActionAndWait_8(22, 0);
    BattleRuntime_WaitIfModeZero_26(10);
    ObjectMotion_SetVariantCallbackAndInvokeObject_7(9, 2);
    BattleRuntime_WaitIfModeZero_27(30);
    BattleEvent_RunActionAndWait_9(9, 0);
    BattleRuntime_WaitIfModeZero_28(10);
    BattleEffect_SpawnLinkedResourceObject_3(22, 0x102, 50);
    ObjectMotion_SetAngleToward_7(22, 9, 20);
    BattleEvent_RunActionAndWait_10(22, 0);
    BattleRuntime_WaitIfModeZero_29(10);
    ObjectMotion_SetVariantCallbackAndInvokeObject_8(8, 2);
    BattleRuntime_WaitIfModeZero_30(20);
    BattleEvent_RunActionAndWait_11(8, 0);
    BattleRuntime_WaitIfModeZero_31(10);
    ObjectMotion_ArmCallback_4(22, 0x4000, 0);
    BattleRuntime_WaitIfModeZero_32(30);
    BattleEvent_RunActionAndWait_12(22, 0);
    BattleRuntime_WaitIfModeZero_33(10);
    ObjectMotion_CallThenWaitForAnimationChange_3(0, 3);
    BattleRuntime_WaitIfModeZero_34(20);
    BattleRuntime_WaitIfModeZero_35(20);
    BattleEffect_SpawnLinkedResourceObject_4(22, 0x102, 50);
    ObjectMotion_SetSpeedParameters_4(22, 0x1cccc, 0xe666);
    ObjectMotion_SetPositionAndReset_1(22, 0x100, 0x168);
    BattleRuntime_WaitIfModeZero_36(10);
    ObjectMotion_ArmCallback_5(22, 0, 0);
    BattleRuntime_WaitIfModeZero_37(30);
    ObjectMotion_ArmCallback_6(22, 0x8000, 0);
    BattleRuntime_WaitIfModeZero_38(30);
    ObjectMotion_ArmCallback_7(22, 0xc000, 0);
    BattleRuntime_WaitIfModeZero_39(30);
    BattleEvent_RunActionAndWait_13(22, 0);
    BattleRuntime_WaitIfModeZero_40(10);
    ObjectMotion_SetVariantCallbackAndInvokeObject_9(9, 2);
    BattleRuntime_WaitIfModeZero_41(20);
    BattleEvent_RunActionAndWait_14(9, 0);
    BattleRuntime_WaitIfModeZero_42(10);
    BattleEffect_SpawnLinkedResourceObject_5(22, 0x100, 40);
    ObjectMotion_ArmCallback_8(22, 0x2000, 0);
    BattleRuntime_WaitIfModeZero_43(20);
    BattleEvent_RunActionAndWait_15(22, 0);
    BattleRuntime_WaitIfModeZero_44(10);
    ObjectMotion_CallThenWaitForAnimationChange_4(9, 3);
    BattleRuntime_WaitIfModeZero_45(30);
    BattleEvent_RunActionAndWait_16(9, 0);
    BattleRuntime_WaitIfModeZero_46(20);
    ObjectMotion_SetVariantCallbackAndInvokeObject_10(22, 2);
    BattleRuntime_WaitIfModeZero_47(20);
    ObjectMotion_SetSpeedParameters_5(22, 0x19999, 0xcccc);
    ObjectMotion_SetPositionAndReset_2(22, 0x100, 0x180);
    ObjectMotion_ArmCallback_9(22, 0, 0);
    BattleRuntime_WaitIfModeZero_48(20);
    BattleEvent_RunActionAndWait_17(22, 0);
    BattleRuntime_WaitIfModeZero_49(10);
    ObjectMotion_SetVariantCallbackAndInvokeObject_11(8, 2);
    BattleRuntime_WaitIfModeZero_50(20);
    BattleEvent_RunActionAndWait_18(8, 0);
    BattleRuntime_WaitIfModeZero_51(10);
    ObjectMotion_ArmCallback_10(22, 0x8000, 0);
    BattleRuntime_WaitIfModeZero_52(40);
    ObjectMotion_CallThenWaitForAnimationChange_5(22, 3);
    BattleRuntime_WaitIfModeZero_53(20);
    BattleEvent_RunActionAndWait_19(22, 0);
    BattleRuntime_WaitIfModeZero_54(10);
    ObjectMotion_SetSpeedParameters_6(22, 0x13333, 0x9999);
    ObjectMotion_CommitPositionAndActivate_2(22, 0, 16);
    BattleRuntime_WaitIfModeZero_55(10);
    BattleEvent_RunActionAndWait_20(22, 0);
    BattleRuntime_WaitIfModeZero_56(10);
    BattleEffect_SpawnLinkedResourceObject_6(0, 0x102, 80);
    BattleEffect_SpawnLinkedResourceObject_7(22, 0x101, 80);
    ObjectMotion_SetAngleToward_8(22, 8, 40);
    ObjectMotion_SetAngleToward_9(22, 9, 40);
    ObjectMotion_ArmCallback_11(22, 0x4000, 0);
    BattleRuntime_WaitIfModeZero_57(30);
    ObjectMotion_SetVariantCallbackAndInvokeObject_12(22, 2);
    BattleRuntime_WaitIfModeZero_58(20);
    BattleEvent_RunActionAndWait_21(22, 0);
    BattleRuntime_WaitIfModeZero_59(10);
    ObjectMotion_SetVariantCallbackAndInvokeObject_13(9, 2);
    BattleRuntime_WaitIfModeZero_60(20);
    BattleEvent_RunActionAndWait_22(9, 0);
    BattleRuntime_WaitIfModeZero_61(10);
    ObjectMotion_ArmCallback_12(22, 0, 0);
    ObjectMotion_SetAngleToward_10(0, 9, 30);
    BattleRuntime_WaitIfModeZero_62(10);
    ObjectMotion_SetVariantCallbackAndInvokeObject_14(8, 2);
    BattleRuntime_WaitIfModeZero_63(20);
    BattleEvent_RunActionAndWait_23(8, 0);
    BattleRuntime_WaitIfModeZero_64(10);
    ObjectMotion_ArmCallback_13(22, 0x8000, 0);
    ObjectMotion_SetAngleToward_11(0, 8, 70);
    ObjectMotion_ArmCallback_14(22, 0x4000, 0);
    BattleRuntime_WaitIfModeZero_65(40);
    ObjectMotion_ArmCallback_15(0, 0xc000, 0);
    BattleRuntime_WaitIfModeZero_66(40);
    ObjectMotion_SetVariantCallbackAndInvokeObject_15(22, 2);
    BattleRuntime_WaitIfModeZero_67(20);
    BattleEvent_RunActionAndWait_24(22, 0);
    BattleRuntime_WaitIfModeZero_68(10);
    ObjectMotion_CallThenWaitForAnimationChange_6(0, 3);
    BattleRuntime_WaitIfModeZero_69(20);
    ObjectMotion_CallThenWaitForAnimationChange_7(22, 3);
    BattleRuntime_WaitIfModeZero_70(30);
    ObjectMotion_SetSpeedParameters_7(22, 0x13333, 0x9999);
    Object_SetModeById_5(22, 2);
    record = Scene_GetRecord_1(0);
    if (record != 0) {
        /* Pass the record's s16 fields at +10 and +18 through to entity 22. */
        ObjectMotion_ResetAndSetPosition_1(22, *(s16 *)(record + 10), *(s16 *)(record + 18));
    }
    ObjectMotion_CommitCurrentPositionAndActivate_2(22);
    ObjectMotion_SetHorizontalPositionWithTerrain_2(22, 0, 0);
    BattleRuntime_WaitIfModeZero_71(10);
    BattleRuntime_ScheduleShoulderButtonModeUpdate_1();
}
