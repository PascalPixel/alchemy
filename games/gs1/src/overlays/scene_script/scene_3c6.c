#include "types.h"

#define SceneData_SelectSecondaryTableByFlag9a7 Func_020011e8

#define SceneState_SetWord1c8To16AndForward16c Func_020001f0

#define SceneActor_UpdateObjectWithCue28be Func_020000c4

#define SceneData_SelectTableByFlag9a7 Func_02000054

#define SceneData_GetTable98c4 Func_0200004c

#define SceneData_ReturnZero Func_02000048

#define SceneData_GetTable975c Func_02000040

#define SceneActor_SetActor23Params2And6 Func_02000030

#define NULL ((void *)0)
#define FIELD_AT_OFFSET(base, type, offset)     (*(type)((u8 *)(base) + (offset)))
#define SceneActor_SetupActorForTable9638 Func_02000078
#define SceneActor_SetupActor18Event Func_0200010c
#define FieldScene_RunPrimarySequence Func_02000218
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
#define FieldScene_RunSecondarySequence Func_020007c4
#define GameFlag_Set_1(a0) Call1(Func_02001cec, a0)
#define BattleRuntime_Reset_1_020007c4(args...) Func_02001d00(args)
#define SceneWork_SetStepValue_1_020007c4(a0) Call1(Func_02001d9e, a0)
#define ObjectMotion_SetPositionAndReset_1_020007c4(a0, a1, a2) Call3(Func_02001d52, a0, a1, a2)
#define ObjectMotion_ArmCallback_1_020007c4(args...) Func_02001dcc(args)
#define ObjectMotion_RealignToTrackedObjectAndArmCallback_1_020007c4(a0, a1, a2, a3) Call4(Func_02001e12, a0, a1, a2, a3)
#define ObjectMotion_RealignToTrackedObjectAndArmCallback_2(a0, a1, a2, a3) Call4(Func_02001e22, a0, a1, a2, a3)
#define ObjectMotion_RealignToTrackedObjectAndArmCallback_3(a0, a1, a2, a3) Call4(Func_02001e30, a0, a1, a2, a3)
#define ObjectMotion_CommitCurrentPositionAndActivate_1_020007c4(args...) Func_02001da6(args)
#define BattleRuntime_WaitIfModeZero_1_020007c4(args...) Func_02001d4c(args)
#define ObjectMotion_SetSpeedParameters_1_020007c4(args...) Func_02001dfc(args)
#define BattleRuntime_WaitIfModeZero_2_020007c4(args...) Func_02001d5a(args)
#define ObjectMotion_ArmCallback_2_020007c4(a0, a1, a2) Call3(Func_02001e1e, a0, a1, a2)
#define BattleRuntime_WaitIfModeZero_3_020007c4(args...) Func_02001d6c(args)
#define UiWork_WaitThenFinalizeCapacity_1_020007c4(args...) Func_02001d94(args)
#define UiWork_WaitThenFinalizeCapacity_2(a0, a1) Value2(Func_02001d94, a0, a1)
#define BattleRuntime_WaitIfModeZero_4_020007c4(args...) Func_02001d7e(args)
#define ObjectMotion_CallThenWaitForAnimationChange_1_020007c4(args...) Func_02001dfe(args)
#define BattleRuntime_WaitIfModeZero_5_020007c4(args...) Func_02001d8c(args)
#define BattleEvent_RunActionAndWait_1_020007c4(args...) Func_02001e44(args)
#define BattleRuntime_WaitIfModeZero_6_020007c4(args...) Func_02001db8(args)
#define ObjectMotion_CallThenWaitForAnimationChange_2_020007c4(args...) Func_02001e38(args)
#define BattleRuntime_WaitIfModeZero_7_020007c4(args...) Func_02001dc6(args)
#define BattleEvent_RunActionAndWait_2_020007c4(args...) Func_02001e8e(args)
#define BattleRuntime_WaitIfModeZero_8_020007c4(args...) Func_02001de4(args)
#define BattleEffect_SpawnLinkedResourceObject_1_020007c4(a0, a1, a2) Call3(Func_02001eb8, a0, a1, a2)
#define BattleEvent_RunActionAndWait_3_020007c4(args...) Func_02001ea8(args)
#define BattleRuntime_WaitIfModeZero_9_020007c4(args...) Func_02001dfe(args)
#define Object_SetModeById_1_020007c4(args...) Func_02001e76(args)
#define BattleRuntime_WaitIfModeZero_10_020007c4(args...) Func_02001e0c(args)
#define Object_SetModeById_2_020007c4(args...) Func_02001e84(args)
#define Object_SetModeById_3_020007c4(args...) Func_02001e8c(args)
#define ObjectMotion_CallThenWaitForAnimationChange_3_020007c4(args...) Func_02001e9c(args)
#define BattleRuntime_WaitIfModeZero_11_020007c4(args...) Func_02001e2a(args)
#define ObjectMotion_CommitPositionAndActivate_1_020007c4(a0, a1, a2) Call3(Func_02001f36, a0, a1, a2)
#define ObjectMotion_OffsetPositionAndReset_1(a0, a1, a2) Call3(Func_02001f3a, a0, a1, a2)
#define ObjectMotion_OffsetPositionAndReset_2(a0, a1, a2) Call3(Func_02001f46, a0, a1, a2)
#define ObjectMotion_CommitPositionAndActivate_2_020007c4(a0, a1, a2) Call3(Func_02001f5a, a0, a1, a2)
#define Object_SetModeById_4_020007c4(args...) Func_02001ed2(args)
#define Object_SetModeById_5_020007c4(args...) Func_02001eda(args)
#define ObjectMotion_ArmCallback_3_020007c4(a0, a1, a2) Call3(Func_02001f2e, a0, a1, a2)
#define ObjectMotion_ArmCallback_4_020007c4(a0, a1, a2) Call3(Func_02001f3a, a0, a1, a2)
#define ObjectMotion_ArmCallback_5_020007c4(a0, a1, a2) Call3(Func_02001f46, a0, a1, a2)
#define ObjectMotion_ArmCallback_6_020007c4(a0, a1, a2) Call3(Func_02001f52, a0, a1, a2)
#define ObjectMotion_ArmCallback_7_020007c4(a0, a1, a2) Call3(Func_02001f5e, a0, a1, a2)
#define BattleRuntime_WaitIfModeZero_12_020007c4(args...) Func_02001eac(args)
#define ObjectMotion_SetSpeedParameters_2_020007c4(a0, a1, a2) Call3(Func_02001ee6, a0, a1, a2)
#define Scene_GetRecord_1_020007c4(args...) Func_02001ee4(args)
#define ObjectMotion_SetActionVariant_1(args...) Func_02001f8a(args)
#define Audio_PlayCue_1(args...) Func_02001fec(args)
#define BattleRuntime_WaitIfModeZero_13_020007c4(args...) Func_02001eea(args)
#define ObjectMotion_SetHorizontalPositionWithTerrain_1_020007c4(a0, a1, a2) Call3(Func_02001f60, a0, a1, a2)
#define BattleRuntime_WaitIfModeZero_14_020007c4(args...) Func_02001efe(args)
#define ObjectMotion_CommitPositionAndActivate_3(args...) Func_02002008(args)
#define Audio_PlayCue_2(args...) Func_02002026(args)
#define BattleRuntime_WaitIfModeZero_15_020007c4(args...) Func_02001f24(args)
#define BattleEvent_RunActionAndWait_4_020007c4(args...) Func_02001fdc(args)
#define BattleRuntime_WaitIfModeZero_16_020007c4(args...) Func_02001f32(args)
#define Object_SetModeById_6(args...) Func_02001faa(args)
#define Object_SetModeById_7(args...) Func_02001fb2(args)
#define Object_SetModeById_8(args...) Func_02001fba(args)
#define ObjectMotion_CallThenWaitForAnimationChange_4_020007c4(args...) Func_02001fca(args)
#define BattleRuntime_WaitIfModeZero_17_020007c4(args...) Func_02001f58(args)
#define ObjectMotion_CommitPositionAndActivate_4(args...) Func_02002062(args)
#define ObjectMotion_ArmCallback_8_020007c4(a0, a1, a2) Call3(Func_02002026, a0, a1, a2)
#define BattleRuntime_WaitIfModeZero_18_020007c4(args...) Func_02001f74(args)
#define BattleRuntime_WaitIfModeZero_19_020007c4(args...) Func_02001f7a(args)
#define ObjectMotion_CallThenWaitForAnimationChange_5_020007c4(args...) Func_02001ffa(args)
#define BattleRuntime_WaitIfModeZero_20_020007c4(args...) Func_02001f88(args)
#define BattleEvent_RunActionAndWait_5_020007c4(args...) Func_02002040(args)
#define BattleRuntime_WaitIfModeZero_21_020007c4(args...) Func_02001f96(args)
#define ObjectMotion_CallThenWaitForAnimationChange_6_020007c4(args...) Func_02002016(args)
#define BattleRuntime_WaitIfModeZero_22_020007c4(args...) Func_02001fa4(args)
#define BattleEvent_RunActionAndWait_6_020007c4(args...) Func_0200205c(args)
#define BattleRuntime_WaitIfModeZero_23_020007c4(args...) Func_02001fb2(args)
#define ObjectMotion_SetVariantCallbackAndInvokeObject_1_020007c4(args...) Func_02002042(args)
#define BattleRuntime_WaitIfModeZero_24_020007c4(args...) Func_02001fc0(args)
#define BattleEvent_RunActionAndWait_7_020007c4(args...) Func_02002078(args)
#define BattleRuntime_WaitIfModeZero_25_020007c4(args...) Func_02001fce(args)
#define ObjectMotion_SetVariantCallbackAndInvokeObject_2_020007c4(args...) Func_0200205e(args)
#define BattleRuntime_WaitIfModeZero_26_020007c4(args...) Func_02001fdc(args)
#define BattleEvent_RunActionAndWait_8_020007c4(args...) Func_02002094(args)
#define BattleRuntime_WaitIfModeZero_27_020007c4(args...) Func_02001fea(args)
#define BattleEffect_SpawnLinkedResourceObject_2_020007c4(a0, a1, a2) Call3(Func_020020be, a0, a1, a2)
#define BattleEvent_RunActionAndWait_9_020007c4(args...) Func_020020ae(args)
#define BattleRuntime_WaitIfModeZero_28_020007c4(args...) Func_02002004(args)
#define BattleEffect_SpawnLinkedResourceObject_3_020007c4(a0, a1, a2) Call3(Func_020020d8, a0, a1, a2)
#define BattleEvent_RunActionAndWait_10_020007c4(args...) Func_020020c8(args)
#define BattleRuntime_WaitIfModeZero_29_020007c4(args...) Func_0200201e(args)
#define ObjectMotion_CallThenWaitForAnimationChange_7_020007c4(args...) Func_0200209e(args)
#define BattleRuntime_WaitIfModeZero_30_020007c4(args...) Func_0200202c(args)
#define BattleEvent_RunActionAndWait_11_020007c4(args...) Func_020020e4(args)
#define BattleRuntime_WaitIfModeZero_31_020007c4(args...) Func_0200203a(args)
#define Object_LinkPair_1(args...) Func_020020dc(args)
#define Object_LinkPair_2(args...) Func_020020e6(args)
#define BattleRuntime_WaitIfModeZero_32_020007c4(args...) Func_02002054(args)
#define ObjectMotion_ArmCallback_9_020007c4(a0, a1, a2) Call3(Func_02002118, a0, a1, a2)
#define ObjectMotion_ArmCallback_10_020007c4(a0, a1, a2) Call3(Func_02002124, a0, a1, a2)
#define ObjectMotion_ArmCallback_11_020007c4(a0, a1, a2) Call3(Func_02002130, a0, a1, a2)
#define ObjectMotion_ArmCallback_12_020007c4(a0, a1, a2) Call3(Func_0200213c, a0, a1, a2)
#define BattleRuntime_WaitIfModeZero_33_020007c4(args...) Func_0200208a(args)
#define BattleRuntime_WaitIfModeZero_34_020007c4(args...) Func_02002090(args)
#define ObjectMotion_CallThenWaitForAnimationChange_8(args...) Func_02002110(args)
#define BattleRuntime_WaitIfModeZero_35_020007c4(args...) Func_0200209e(args)
#define BattleEvent_RunActionAndWait_12_020007c4(args...) Func_02002156(args)
#define BattleRuntime_WaitIfModeZero_36_020007c4(args...) Func_020020ac(args)
#define BattleEffect_SpawnLinkedResourceObject_4_020007c4(a0, a1, a2) Call3(Func_02002180, a0, a1, a2)
#define BattleEvent_RunActionAndWait_13_020007c4(args...) Func_02002170(args)
#define BattleRuntime_WaitIfModeZero_37_020007c4(args...) Func_020020c6(args)
#define ObjectMotion_CallThenWaitForAnimationChange_9(args...) Func_02002146(args)
#define BattleRuntime_WaitIfModeZero_38_020007c4(args...) Func_020020d4(args)
#define BattleEvent_RunActionAndWait_14_020007c4(args...) Func_0200218c(args)
#define BattleRuntime_WaitIfModeZero_39_020007c4(args...) Func_020020e2(args)
#define ObjectMotion_ArmCallback_13_020007c4(a0, a1, a2) Call3(Func_020021a6, a0, a1, a2)
#define BattleRuntime_WaitIfModeZero_40_020007c4(args...) Func_020020f4(args)
#define ObjectMotion_SetSpeedParameters_3_020007c4(a0, a1) Value2(Func_020021a4, a0, a1)
#define UiWork_WaitThenFinalizeCapacity_3(args...) Func_02002124(args)
#define UiWork_WaitThenFinalizeCapacity_4(a0, a1) Value2(Func_02002124, a0, a1)
#define BattleRuntime_WaitIfModeZero_41_020007c4(args...) Func_0200210e(args)
#define ObjectMotion_CallThenWaitForAnimationChange_10(args...) Func_0200218e(args)
#define BattleRuntime_WaitIfModeZero_42_020007c4(args...) Func_0200211c(args)
#define BattleEvent_RunActionAndWait_15_020007c4(args...) Func_020021d4(args)
#define BattleRuntime_WaitIfModeZero_43_020007c4(args...) Func_02002148(args)
#define ObjectMotion_CallThenWaitForAnimationChange_11(args...) Func_020021c8(args)
#define BattleRuntime_WaitIfModeZero_44_020007c4(args...) Func_02002156(args)
#define BattleEvent_RunActionAndWait_16_020007c4(args...) Func_0200221e(args)
#define BattleRuntime_WaitIfModeZero_45_020007c4(args...) Func_02002174(args)
#define BattleEffect_SpawnLinkedResourceObject_5_020007c4(a0, a1, a2) Call3(Func_02002248, a0, a1, a2)
#define BattleEvent_RunActionAndWait_17_020007c4(args...) Func_02002238(args)
#define BattleRuntime_WaitIfModeZero_46_020007c4(args...) Func_0200218e(args)
#define ObjectMotion_ArmCallback_14_020007c4(a0, a1, a2) Call3(Func_02002252, a0, a1, a2)
#define BattleEffect_SpawnLinkedResourceObject_6_020007c4(a0, a1, a2) Call3(Func_0200226e, a0, a1, a2)
#define BattleEvent_RunActionAndWait_18_020007c4(args...) Func_0200225e(args)
#define BattleRuntime_WaitIfModeZero_47_020007c4(args...) Func_020021b4(args)
#define ObjectMotion_SetHorizontalPositionWithTerrain_2_020007c4(a0, a1, a2) Call3(Func_0200222a, a0, a1, a2)
#define BattleEvent_RunActionAndWait_19_020007c4(a0, a1) Call2(Func_0200227c, a0, a1)
#define BattleRuntime_WaitIfModeZero_48_020007c4(args...) Func_020021d2(args)
#define BattleEffect_SpawnLinkedResourceObject_7_020007c4(a0, a1, a2) Call3(Func_020022a6, a0, a1, a2)
#define ObjectMotion_Launch_1(args...) Func_02002268(args)
#define ObjectMotion_Launch_2(args...) Func_02002272(args)
#define BattleEvent_RunActionAndWait_20_020007c4(args...) Func_020022aa(args)
#define BattleRuntime_WaitIfModeZero_49_020007c4(args...) Func_02002200(args)
#define ObjectMotion_CallThenWaitForAnimationChange_12(args...) Func_02002280(args)
#define BattleRuntime_WaitIfModeZero_50_020007c4(args...) Func_0200220e(args)
#define BattleEvent_RunActionAndWait_21_020007c4(args...) Func_020022c6(args)
#define GameFlag_IsSet_1(args...) Func_0200220c(args)
#define GameFlag_IsSet_2(a0) Value1(Func_0200220c, a0)
#define SceneWork_SetStepValue_2(a0) Call1(Func_020022ca, a0)
#define BattleRuntime_WaitIfModeZero_51_020007c4(args...) Func_02002230(args)
#define ObjectMotion_SetVariantCallbackAndInvokeObject_3_020007c4(args...) Func_020022c0(args)
#define BattleRuntime_WaitIfModeZero_52_020007c4(args...) Func_0200223e(args)
#define BattleEvent_RunActionAndWait_22_020007c4(args...) Func_020022f6(args)
#define BattleRuntime_WaitIfModeZero_53_020007c4(args...) Func_0200224c(args)
#define ObjectMotion_CallThenWaitForAnimationChange_13(args...) Func_020022cc(args)
#define BattleRuntime_WaitIfModeZero_54_020007c4(args...) Func_0200225a(args)
#define BattleEvent_RunActionAndWait_23_020007c4(args...) Func_02002312(args)
#define BattleRuntime_WaitIfModeZero_55_020007c4(args...) Func_02002268(args)
#define BattleEffect_SpawnLinkedResourceObject_8(a0, a1, a2) Call3(Func_0200233c, a0, a1, a2)
#define BattleEvent_RunActionAndWait_24_020007c4(args...) Func_0200232c(args)
#define BattleRuntime_WaitIfModeZero_56_020007c4(args...) Func_02002282(args)
#define ObjectMotion_SetVariantCallbackAndInvokeObject_4_020007c4(args...) Func_02002312(args)
#define BattleRuntime_WaitIfModeZero_57_020007c4(args...) Func_02002290(args)
#define BattleEvent_RunActionAndWait_25(args...) Func_02002348(args)
#define BattleRuntime_WaitIfModeZero_58_020007c4(args...) Func_0200229e(args)
#define ObjectMotion_CallThenWaitForAnimationChange_14(args...) Func_0200231e(args)
#define BattleRuntime_WaitIfModeZero_59_020007c4(args...) Func_020022ac(args)
#define BattleEvent_RunActionAndWait_26(args...) Func_02002364(args)
#define BattleRuntime_WaitIfModeZero_60_020007c4(args...) Func_020022ba(args)
#define ObjectMotion_CallThenWaitForAnimationChange_15(args...) Func_0200233a(args)
#define BattleRuntime_WaitIfModeZero_61_020007c4(args...) Func_020022c8(args)
#define ObjectMotion_SetSpeedParameters_4_020007c4(a0, a1, a2) Call3(Func_02002302, a0, a1, a2)
#define ObjectMotion_OffsetPositionAndResetMotion_1(args...) Func_02002334(args)
#define ObjectMotion_CommitPositionAndActivate_5(args...) Func_020023e6(args)
#define BattleRuntime_WaitIfModeZero_62_020007c4(args...) Func_020022ec(args)
#define BattleRuntime_WaitIfModeZero_63_020007c4(args...) Func_02002314(args)
#define Scene_GetRecord_2(args...) Func_02002342(args)
#define ObjectMotion_CommitPositionAndActivate_6(a0, a1, a2) Call3(Func_02002430, a0, a1, a2)
#define Scene_GetRecord_3(args...) Func_0200235e(args)
#define ObjectMotion_ArmCallback_15_020007c4(a0, a1, a2) Call3(Func_02002404, a0, a1, a2)
#define BattleRuntime_WaitIfModeZero_64_020007c4(args...) Func_02002352(args)
#define BattleRuntime_WaitIfModeZero_65_020007c4(args...) Func_02002358(args)
#define BattleEffect_SpawnLinkedResourceObject_9(a0, a1, a2) Call3(Func_0200242c, a0, a1, a2)
#define BattleEvent_RunActionAndWait_27(args...) Func_0200241c(args)
#define BattleRuntime_WaitIfModeZero_66_020007c4(args...) Func_02002372(args)
#define ObjectMotion_CallThenWaitForAnimationChange_16(args...) Func_020023f2(args)
#define BattleRuntime_WaitIfModeZero_67_020007c4(args...) Func_02002380(args)
#define BattleEvent_RunActionAndWait_28(args...) Func_02002438(args)
#define BattleRuntime_WaitIfModeZero_68_020007c4(args...) Func_0200238e(args)
#define Object_SetModeById_9(args...) Func_02002406(args)
#define Object_SetModeById_10(args...) Func_0200240e(args)
#define Object_SetModeById_11(args...) Func_02002416(args)
#define ObjectMotion_CallThenWaitForAnimationChange_17(args...) Func_02002426(args)
#define BattleRuntime_WaitIfModeZero_69_020007c4(args...) Func_020023b4(args)
#define BattleRuntime_WaitIfModeZero_70_020007c4(args...) Func_020023ba(args)
#define ObjectMotion_CallThenWaitForAnimationChange_18(args...) Func_0200243a(args)
#define BattleRuntime_WaitIfModeZero_71_020007c4(args...) Func_020023c8(args)
#define ObjectMotion_CommitPositionAndActivate_7(a0, a1, a2) Call3(Func_020024d4, a0, a1, a2)
#define ObjectMotion_ArmCallback_16(a0, a1, a2) Call3(Func_02002498, a0, a1, a2)
#define BattleRuntime_WaitIfModeZero_72(args...) Func_020023e6(args)
#define Audio_PlayCue_3(args...) Func_02002508(args)
#define BattleRuntime_WaitIfModeZero_73(args...) Func_02002406(args)
#define ObjectMotion_CommitPositionAndActivate_8(a0, a1, a2) Call3(Func_02002512, a0, a1, a2)
#define ObjectMotion_SetHorizontalPositionWithTerrain_3(args...) Func_02002484(args)
#define BattleRuntime_WaitIfModeZero_74(args...) Func_02002422(args)
#define Audio_PlayCue_4(args...) Func_02002540(args)
#define BattleRuntime_WaitIfModeZero_75(args...) Func_0200243e(args)
#define ObjectMotion_ArmCallback_17(a0, a1, a2) Call3(Func_02002502, a0, a1, a2)
#define ObjectMotion_SetAngleToward_1_020007c4(args...) Func_020024e4(args)
#define ObjectMotion_SetAngleToward_2_020007c4(args...) Func_020024ee(args)
#define BattleRuntime_WaitIfModeZero_76(args...) Func_02002464(args)
#define BattleEvent_RunActionAndWait_29(args...) Func_0200251c(args)
#define BattleRuntime_WaitIfModeZero_77(args...) Func_02002472(args)
#define ObjectMotion_SetVariantCallbackAndInvokeObject_5_020007c4(args...) Func_02002502(args)
#define BattleRuntime_WaitIfModeZero_78(args...) Func_02002480(args)
#define BattleEvent_RunActionAndWait_30(args...) Func_02002538(args)
#define BattleRuntime_WaitIfModeZero_79(args...) Func_0200248e(args)
#define ObjectMotion_CallThenWaitForAnimationChange_19(args...) Func_0200250e(args)
#define BattleRuntime_WaitIfModeZero_80(args...) Func_0200249c(args)
#define BattleEvent_RunActionAndWait_31(args...) Func_02002554(args)
#define BattleRuntime_WaitIfModeZero_81(args...) Func_020024aa(args)
#define Object_SetModeById_12(args...) Func_02002522(args)
#define BattleRuntime_WaitIfModeZero_82(args...) Func_020024b8(args)
#define Object_SetModeById_13(args...) Func_02002530(args)
#define Object_SetModeById_14(args...) Func_02002538(args)
#define ObjectMotion_CallThenWaitForAnimationChange_20(args...) Func_02002548(args)
#define BattleRuntime_WaitIfModeZero_83(args...) Func_020024d6(args)
#define ObjectMotion_SetSpeedParameters_5_020007c4(a0, a1, a2) Call3(Func_02002510, a0, a1, a2)
#define ObjectMotion_SetSpeedParameters_6_020007c4(a0, a1, a2) Call3(Func_0200251a, a0, a1, a2)
#define ObjectMotion_SetSpeedParameters_7_020007c4(a0, a1, a2) Call3(Func_02002524, a0, a1, a2)
#define Object_SetModeById_15(args...) Func_0200256c(args)
#define Scene_GetRecord_4(a0) Value1(Func_0200252a, a0)
#define ObjectMotion_ResetAndSetPosition_1_020007c4(args...) Func_02002554(args)
#define ObjectMotion_CommitCurrentPositionAndActivate_2_020007c4(args...) Func_02002598(args)
#define ObjectMotion_SetHorizontalPositionWithTerrain_4(args...) Func_020025aa(args)
#define Object_SetModeById_16(args...) Func_020025ba(args)
#define Scene_GetRecord_5(a0) Value1(Func_02002578, a0)
#define ObjectMotion_ResetAndSetPosition_2(args...) Func_020025a2(args)
#define ObjectMotion_CommitCurrentPositionAndActivate_3(args...) Func_020025c8(args)
#define ObjectMotion_SetHorizontalPositionWithTerrain_5(args...) Func_020025da(args)
#define Object_SetModeById_17(args...) Func_020025ea(args)
#define Scene_GetRecord_6(a0) Value1(Func_020025a8, a0)
#define ObjectMotion_ResetAndSetPosition_3(args...) Func_020025d2(args)
#define ObjectMotion_CommitCurrentPositionAndActivate_4(args...) Func_020025f8(args)
#define ObjectMotion_SetHorizontalPositionWithTerrain_6(args...) Func_0200260a(args)
#define BattleRuntime_ScheduleShoulderButtonModeUpdate_1_020007c4(args...) Func_020025bc(args)
#define SCENE_STEP (*(u16 *)(*(u8 **)0x03001ebc + 0x1d8))
#define REC_S16(rec, off) (*(s16 *)((rec) + (off)))
#define FieldScene_RunScene3c6SequenceA Func_02001090
#define SceneWork_SetStepValue_1_02001090(a0) Call1(Func_0200265e, a0)
#define BattleRuntime_WaitIfModeZero_1_02001090(args...) Func_020025c4(args)
#define ObjectMotion_SetVariantCallbackAndInvokeObject_1_02001090(args...) Func_02002654(args)
#define BattleRuntime_WaitIfModeZero_2_02001090(args...) Func_020025d2(args)
#define BattleEvent_RunActionAndWait_1_02001090(args...) Func_0200268a(args)
#define BattleRuntime_WaitIfModeZero_3_02001090(args...) Func_020025e0(args)
#define ObjectMotion_CallThenWaitForAnimationChange_1_02001090(args...) Func_02002660(args)
#define BattleRuntime_WaitIfModeZero_4_02001090(args...) Func_020025ee(args)
#define BattleEvent_RunActionAndWait_2_02001090(args...) Func_020026a6(args)
#define BattleRuntime_WaitIfModeZero_5_02001090(args...) Func_020025fc(args)
#define BattleEffect_SpawnLinkedResourceObject_1_02001090(a0, a1, a2) Call3(Func_020026d0, a0, a1, a2)
#define BattleEffect_SpawnLinkedResourceObject_2_02001090(a0, a1, a2) Call3(Func_020026dc, a0, a1, a2)
#define BattleEffect_SpawnLinkedResourceObject_3_02001090(a0, a1, a2) Call3(Func_020026e8, a0, a1, a2)
#define BattleEffect_SpawnLinkedResourceObject_4_02001090(a0, a1, a2) Call3(Func_020026f4, a0, a1, a2)
#define BattleRuntime_WaitIfModeZero_6_02001090(args...) Func_02002632(args)
#define ObjectMotion_SetVariantCallbackAndInvokeObject_2_02001090(args...) Func_020026c2(args)
#define BattleRuntime_WaitIfModeZero_7_02001090(args...) Func_02002640(args)
#define BattleEvent_RunActionAndWait_3_02001090(args...) Func_020026f8(args)
#define BattleRuntime_WaitIfModeZero_8_02001090(args...) Func_0200264e(args)
#define ObjectMotion_CallThenWaitForAnimationChange_2_02001090(args...) Func_020026ce(args)
#define BattleRuntime_WaitIfModeZero_9_02001090(args...) Func_0200265c(args)
#define BattleEvent_RunActionAndWait_4_02001090(args...) Func_02002714(args)
#define BattleRuntime_WaitIfModeZero_10_02001090(args...) Func_0200266a(args)
#define BattleEffect_SpawnLinkedResourceObject_5_02001090(a0, a1, a2) Call3(Func_0200273e, a0, a1, a2)
#define BattleEvent_RunActionAndWait_5_02001090(args...) Func_0200272e(args)
#define BattleRuntime_WaitIfModeZero_11_02001090(args...) Func_02002684(args)
#define ObjectMotion_CallThenWaitForAnimationChange_3_02001090(args...) Func_02002704(args)
#define BattleRuntime_WaitIfModeZero_12_02001090(args...) Func_02002692(args)
#define BattleEvent_RunActionAndWait_6_02001090(args...) Func_0200274a(args)
#define BattleRuntime_WaitIfModeZero_13_02001090(args...) Func_020026a0(args)
#define ObjectMotion_ArmCallback_1_02001090(a0, a1, a2) Call3(Func_02002764, a0, a1, a2)
#define BattleRuntime_WaitIfModeZero_14_02001090(args...) Func_020026b2(args)
#define ObjectMotion_ArmCallback_2_02001090(a0, a1, a2) Call3(Func_02002776, a0, a1, a2)
#define BattleRuntime_WaitIfModeZero_15_02001090(args...) Func_020026c4(args)
#define ObjectMotion_CallThenWaitForAnimationChange_4_02001090(args...) Func_02002744(args)
#define BattleRuntime_WaitIfModeZero_16_02001090(args...) Func_020026d2(args)
#define BattleEvent_RunActionAndWait_7_02001090(args...) Func_0200278a(args)
#define FieldScene_RunSequenceB Func_020011bc

extern u8 Value_000028be;

void Func_0200164a(s32, s32, s32);
s32 Func_02001572(s32);
s32 Func_020015ce();
void Func_020015d4();
s32 Func_020015da();
void Func_02001614();
void Func_0200165a();
void Func_02001672();
void Func_02001696(s32);
s32 Func_020016a6(s32, s32);
s32 Func_02001626(s32, s32);
void Func_02001610(s32);
void Func_020016b6(s32);
void Func_020016be(s32);
void Func_020016d6(s32, s32);
void Func_02001632();
void Func_020016e0();
void Func_020016f8();
void Func_02001724();
void Func_02001762(s32, s32, s32);
void Func_02001804(s32);
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
u8 *Func_02001cd6();
void Func_02001cec();
void Func_02001d4c();
void Func_02001d52();
void Func_02001d5a();
void Func_02001d6c();
void Func_02001d7e();
void Func_02001d8c();
s32 Func_02001d94();
void Func_02001d9e();
void Func_02001da6();
void Func_02001db8();
void Func_02001dc6();
void Func_02001dcc();
void Func_02001de4();
void Func_02001dfc();
void Func_02001dfe();
void Func_02001e0c();
void Func_02001e12();
void Func_02001e1e();
void Func_02001e22();
void Func_02001e2a();
void Func_02001e30();
void Func_02001e38();
void Func_02001e44();
void Func_02001e76();
void Func_02001e84();
void Func_02001e8c();
void Func_02001e8e();
void Func_02001e9c();
void Func_02001ea8();
void Func_02001eac();
void Func_02001eb6();
void Func_02001eb8();
void Func_02001ed2();
void Func_02001eda();
void Func_02001ee6();
void Func_02001eea();
void Func_02001ef0();
void Func_02001efe();
void Func_02001f24();
void Func_02001f2e();
void Func_02001f32();
void Func_02001f36();
void Func_02001f3a();
void Func_02001f46();
void Func_02001f52();
void Func_02001f58();
void Func_02001f5a();
void Func_02001f5e();
void Func_02001f60();
void Func_02001f74();
void Func_02001f7a();
void Func_02001f88();
void Func_02001f8a();
void Func_02001f96();
void Func_02001fa4();
void Func_02001faa();
void Func_02001fb2();
void Func_02001fba();
void Func_02001fc0();
void Func_02001fca();
void Func_02001fce();
void Func_02001fdc();
void Func_02001fea();
void Func_02001fec();
void Func_02001ffa();
void Func_02002004();
void Func_02002008();
void Func_02002016();
void Func_0200201e();
void Func_02002026();
void Func_0200202c();
void Func_0200203a();
void Func_02002040();
void Func_02002042();
void Func_02002054();
void Func_0200205c();
void Func_0200205e();
void Func_02002062();
void Func_02002078();
void Func_0200208a();
void Func_02002090();
void Func_02002094();
void Func_0200209e();
void Func_020020ac();
void Func_020020ae();
void Func_020020be();
void Func_020020c6();
void Func_020020c8();
void Func_020020d4();
void Func_020020d8();
void Func_020020dc();
void Func_020020e2();
void Func_020020e4();
void Func_020020e6();
void Func_020020f4();
void Func_0200210e();
void Func_02002110();
void Func_02002118();
void Func_0200211c();
s32 Func_02002124();
void Func_02002130();
void Func_0200213c();
void Func_02002146();
void Func_02002148();
void Func_02002156();
void Func_02002170();
void Func_02002174();
void Func_02002180();
void Func_0200218c();
void Func_0200218e();
s32 Func_020021a4();
void Func_020021a6();
void Func_020021b4();
void Func_020021c8();
void Func_020021d2();
void Func_020021d4();
void Func_02002200();
s32 Func_0200220c();
void Func_0200220e();
void Func_0200221e();
void Func_0200222a();
void Func_02002230();
void Func_02002238();
void Func_0200223e();
void Func_02002248();
void Func_0200224c();
void Func_02002252();
void Func_0200225a();
void Func_0200225e();
void Func_02002268();
void Func_0200226e();
void Func_02002272();
void Func_0200227c();
void Func_02002280();
void Func_02002282();
void Func_02002290();
void Func_0200229e();
void Func_020022a6();
void Func_020022aa();
void Func_020022ac();
void Func_020022ba();
void Func_020022c0();
void Func_020022c6();
void Func_020022c8();
void Func_020022ca();
void Func_020022cc();
void Func_020022ec();
void Func_020022f6();
void Func_02002302();
void Func_02002312();
void Func_02002314();
void Func_0200231e();
void Func_02002326();
void Func_0200232c();
void Func_02002334();
void Func_0200233a();
void Func_0200233c();
s32 Func_02002342();
void Func_02002348();
void Func_02002352();
void Func_02002358();
void Func_02002484();
void Func_02002364();
void Func_02002372();
void Func_02002380();
void Func_0200238e();
void Func_020023b4();
void Func_020023ba();
void Func_020023c8();
void Func_020023d2();
void Func_020023e6();
void Func_020023f2();
void Func_02002404();
void Func_02002406();
void Func_0200240a();
void Func_0200240e();
void Func_02002416();
void Func_0200241c();
void Func_02002422();
void Func_02002426();
void Func_0200242c();
void Func_02002430();
void Func_02002438();
void Func_0200243a();
void Func_0200243e();
void Func_02002464();
void Func_02002472();
void Func_02002480();
void Func_0200248e();
void Func_02002498();
void Func_0200249c();
void Func_020024aa();
void Func_020024b8();
void Func_020024d4();
void Func_020024d6();
void Func_020024e4();
void Func_020024ee();
void Func_02002502();
void Func_02002508();
void Func_0200250e();
void Func_02002510();
void Func_02002512();
void Func_0200251a();
void Func_0200251c();
void Func_02002522();
void Func_02002524();
s32 Func_0200252a();
void Func_02002530();
void Func_02002538();
void Func_02002540();
void Func_02002548();
void Func_02002554();
void Func_0200256c();
s32 Func_02002578();
void Func_02002598();
void Func_020025a2();
s32 Func_020025a8();
void Func_020025aa();
void Func_020025ba();
void Func_020025bc();
void Func_020025c8();
void Func_020025d2();
void Func_020025da();
void Func_020025ea();
void Func_020025f8();
void Func_0200260a();
u8 *Func_0200235e();
void Func_020025c4();
void Func_020025e0();
void Func_020025ee();
void Func_020025fc();
void Func_02002632();
void Func_02002640();
void Func_0200264e();
void Func_02002654();
void Func_0200265c();
void Func_0200265e();
void Func_02002660();
void Func_0200266a();
void Func_02002684();
void Func_0200268a();
void Func_02002692();
void Func_020026a0();
void Func_020026a6();
void Func_020026b2();
void Func_020026c2();
void Func_020026c4();
void Func_020026ce();
void Func_020026d0();
void Func_020026d2();
void Func_020026dc();
void Func_020026e8();
void Func_020026f4();
void Func_020026f8();
void Func_02002704();
void Func_02002714();
void Func_0200272e();
void Func_0200273e();
void Func_02002744();
void Func_0200274a();
void Func_02002764();
void Func_02002776();
void Func_0200278a();
void Func_020026f0();
void Func_02002716();
void Func_0200278e();
void Func_020027a6();
void Func_02002802();
s32 Func_02002706(s32);

/* Contiguous unnamed leaf-owner run for resource_3c6. */

/*
 * resource_3c6 owner at 0x02000040, 8 bytes: `ldr r0, [pc, #0] / bx lr` plus the
 * one-word literal pool at 0x2000044 holding 0x200975c.
 *
 * LEAF RESIDUE. Published at image offset 0xc; sweep B resolved that
 * word and, before 2026-08-01, discarded it for not opening with a `push`.
 *
 * THE SPAN IS 8 BYTES, NOT 4. The pool word sits past the `bx lr`, and the
 * `pc`-relative load at 0x02000040 reads it, so it belongs to this owner.
 * Recording 4 would orphan a word and manufacture a phantom gap.
 *
 * The pool word is an ADDRESS -- 0x200975c is image offset
 * 0x175c under the base + 0x8000 spelling -- loaded and returned
 * without being dereferenced, so this is a getter for an in-image table.
 *
 * One of the 191 rows sharing this exact body across the tree, and every
 * one of them returns a DIFFERENT address. Identical bytes are not
 * identical semantics; this row's pool word was resolved on its own.
 */

/*
 * resource_3c6 owner at 0x02000048, 4 bytes: `movs r0, #0 / bx lr`.
 *
 * LEAF RESIDUE. Published at image offset 0x2c; sweep B resolved that
 * word and, before 2026-08-01, discarded it for not opening with a `push`.
 * A leaf never does -- it saves no register and returns with `bx lr`.
 *
 * Complete owner: both instructions. No prologue, no stack frame, no
 * literal pool, no callees, no argument read.
 *
 * One of the 70 rows sharing this exact body across the tree. The body is
 * shared; the identity is not -- this row is bounded by ITS overlay's
 * neighbours and published from ITS overlay's table.
 */

/*
 * resource_3c6 owner at 0x0200004c, 8 bytes: `ldr r0, [pc, #0] / bx lr` plus the
 * one-word literal pool at 0x2000050 holding 0x20098c4.
 *
 * LEAF RESIDUE. Published at image offset 0x14; sweep B resolved that
 * word and, before 2026-08-01, discarded it for not opening with a `push`.
 *
 * THE SPAN IS 8 BYTES, NOT 4. The pool word sits past the `bx lr`, and the
 * `pc`-relative load at 0x0200004c reads it, so it belongs to this owner.
 * Recording 4 would orphan a word and manufacture a phantom gap.
 *
 * The pool word is an ADDRESS -- 0x20098c4 is image offset
 * 0x18c4 under the base + 0x8000 spelling -- loaded and returned
 * without being dereferenced, so this is a getter for an in-image table.
 *
 * One of the 191 rows sharing this exact body across the tree, and every
 * one of them returns a DIFFERENT address. Identical bytes are not
 * identical semantics; this row's pool word was resolved on its own.
 */

/* Loader-relocated overlay calls: each symbol names the pre-relocation call
 * word the image holds. */

/* Call sites spelled through these wrappers pass their constants straight
 * into the argument registers; a direct call precomputes a costly constant
 * into a pseudo that the compiler then shares with later uses in the block.
 * A value-returning call also sets r0 last of its arguments. */

/* The scene step counter at 0x1d8 of the shared scene work record. */

/* Resolved engine calls: each pseudo symbol is the per-site call word the
 * overlay image holds (a word can serve two sites with different targets),
 * and the macro names the engine function the site reaches through the
 * overlay veneer and the main-image veneer island, keeping the site's own
 * calling form. Names without a repository binding are provisional.
 */

/* Remaining raw call sites, named individually from the engine source their
 * main-image function reaches. */

/* Runs the fixed call sequence for scene 3c6: a long, paced series of setup
 * and per-entity calls (position/pose/property triples keyed by entity id,
 * interleaved with timed single-argument steps), ending with a lookup of a
 * record whose s16 fields at +10 and +18 feed the last positioning call. */

/* The step counter, addressed directly for the sites that read-modify-write
 * it inline instead of going through bump_step. */

/* A signed 16-bit field read out of an actor record returned by one of the
 * Func_...() record lookups below. */

/* Drives actors 0-3, 22 and 25 through a long timed sequence of pose, move,
 * and sprite-flag calls, gated by two condition checks that each pick one of
 * two call sequences and both bump the shared scene step counter. */

/* Sets the s16 field at offset 0x1d8 of the shared scene work record
 * (the scene step counter, per bump_step below) directly to a value. */

/* Calls ObjectMotion_SetVariantCallback(), then forwards arg0 into an
 * unnamed follow-up call. */

/* Calls an unnamed function, then ObjectMotion_WaitForAnimationChange(arg0). */

/* Runs a fixed, unbranching sequence of overlay calls with constant
 * arguments; no loop, no stored result, no use of the scene work record. */

static __inline__ void Call1(void (*f)(), s32 a0)
{
    extern u8 Data_03001ebc[];
    void Func_02001686();

    f(a0);
}

static __inline__ s32 Value1(s32 (*f)(), s32 a0)
{
    extern u8 Data_03001ebc[];
    void Func_02001686();

    return f(a0);
}

static __inline__ void Call3(void (*f)(), s32 a0, s32 a1, s32 a2)
{
    extern u8 Data_03001ebc[];
    void Func_02001686();

    f(a0, a1, a2);
}

static __inline__ void bump_step(s32 amount)
{
    extern u8 Data_03001ebc[];
    void Func_02001686();

    u8 *work = *(u8 **)Data_03001ebc;

    *(u16 *)(work + 0x1d8) = (u16)(*(u16 *)(work + 0x1d8) + amount);
}

static __inline__ void PlaceActor(s32 actor, s32 x, s32 y)
{
    void Func_02001686(s32, s32, s32);

    Func_02001686(actor, x, y);
}

static __inline__ void SetOffset(s32 actor, s32 offset, s32 zero)
{
    void Func_02001686(s32, s32, s32);

    Func_02001762(actor, offset, zero);
}

static __inline__ s32 Value2(s32 (*f)(), s32 a0, s32 a1)
{
    u8 *Func_02001ee4();

    return f(a0, a1);
}

static __inline__ void Call4(void (*f)(), s32 a0, s32 a1, s32 a2, s32 a3)
{
    u8 *Func_02001ee4();

    f(a0, a1, a2, a3);
}

static __inline__ void Call2(void (*f)(), s32 a0, s32 a1)
{
    s32 Func_02001ee4();

    f(a0, a1);
}

static __inline__ void Call6(void (*f)(), s32 a0, s32 a1, s32 a2, s32 a3, s32 a4, s32 a5)
{
    s32 Func_02001ee4();

    f(a0, a1, a2, a3, a4, a5);
}

void SceneActor_SetActor23Params2And6(void) {
    Func_0200164a(0x17, 2, 6);
}

u8 *SceneData_GetTable975c(void)
{
    return (u8 *)0x0200975c;
}

s32 SceneData_ReturnZero(void)
{
    return 0;
}

u8 *SceneData_GetTable98c4(void)
{
    return (u8 *)0x020098c4;
}

s32 SceneData_SelectTableByFlag9a7(void)
{
    if (Func_02001572(0x9A7) != 0) {
        return 0x02009A98;
    }
    return 0x02009900;
}

void SceneActor_SetupActorForTable9638(s32 a0)
{
    extern u8 Data_03001ebc[];
    void Func_02001686();

    u32 i;
    s32 record;

    record = Func_020015ce();
    *(volatile s32 *)(record + 24) = 0x10000;
    record = Value1(Func_020015da, a0);
    *(volatile s32 *)(record + 28) = 0x10000;
    Call1(Func_0200165a, 0x26af);
    Func_02001672(a0, 0);
    Call3(Func_02001686, a0, 0xc000, 0);
    Func_020015d4(20);
    Func_02001614(a0, 0x2009638);
}

void SceneActor_UpdateObjectWithCue28be(s32 obj)
{
    s32 cue = (s32) &Value_000028be;
    Func_02001696(cue);
    Func_020016a6(obj, 0);
    if (Func_02001626(0, 0) == 0) {
        Func_02001610(10);
        Func_020016b6(cue + 1);
    } else {
        Func_020016be(cue + 2);
    }
    Func_020016d6(obj, 0);
}

void SceneActor_SetupActor18Event(void)
{
    void Func_02001686(s32, s32, s32);

    Func_02001632(2491);
    Func_020016e0(0x28b8);
    Func_020016f8(18, 0);
    PlaceActor(18, 65536, 32768);
    SetOffset(18, -16, 0);
    Func_02001724(18, 0, 0);
    Func_02001672(10);
}

void SceneState_SetWord1c8To16AndForward16c(void)
{
    extern u8 *Data_03001ebc;

    u8 *work = Data_03001ebc;
    s16 *p = (s16 *)(work + 0x16C);
    s32 n = *p;

    *(s32 *)(work + 0x1C8) = 16;
    Func_02001804(n);
}

void FieldScene_RunPrimarySequence(void)
{
    u8 *Func_02001ee4();

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

void FieldScene_RunSecondarySequence(void)
{
    s32 Func_02001ee4();

    u32 i;
    u8 *record;

    GameFlag_Set_1(0x9ba);
    BattleRuntime_Reset_1_020007c4();
    SceneWork_SetStepValue_1_020007c4(0x288e);
    ObjectMotion_SetPositionAndReset_1_020007c4(0, 104, 0x178);
    ObjectMotion_ArmCallback_1_020007c4(0, 0, 0);
    ObjectMotion_RealignToTrackedObjectAndArmCallback_1_020007c4(1, -32, 0, 0);
    ObjectMotion_RealignToTrackedObjectAndArmCallback_2(3, -16, 16, 0xe000);
    ObjectMotion_RealignToTrackedObjectAndArmCallback_3(2, 0, 16, 0xc000);
    ObjectMotion_CommitCurrentPositionAndActivate_1_020007c4(1);
    BattleRuntime_WaitIfModeZero_1_020007c4(30);
    ObjectMotion_SetSpeedParameters_1_020007c4(1, 0);
    BattleRuntime_WaitIfModeZero_2_020007c4(10);
    ObjectMotion_ArmCallback_2_020007c4(0, 0x8000, 0);
    BattleRuntime_WaitIfModeZero_3_020007c4(10);
    /* Branch selected by UiWork_WaitThenFinalizeCapacity_1_020007c4(0, 0); either path bumps the step
     * counter once, in a different spot in its four calls. */
    if (UiWork_WaitThenFinalizeCapacity_2(0, 0) == 0) {
        BattleRuntime_WaitIfModeZero_4_020007c4(20);
        ObjectMotion_CallThenWaitForAnimationChange_1_020007c4(3, 4);
        BattleRuntime_WaitIfModeZero_5_020007c4(20);
        BattleEvent_RunActionAndWait_1_020007c4(3, 0);
        SCENE_STEP += 1;
    } else {
        BattleRuntime_WaitIfModeZero_6_020007c4(20);
        ObjectMotion_CallThenWaitForAnimationChange_2_020007c4(3, 4);
        BattleRuntime_WaitIfModeZero_7_020007c4(20);
        SCENE_STEP += 1;
        BattleEvent_RunActionAndWait_2_020007c4(3, 0);
    }
    BattleRuntime_WaitIfModeZero_8_020007c4(10);
    BattleEffect_SpawnLinkedResourceObject_1_020007c4(2, 0x108, 40);
    BattleEvent_RunActionAndWait_3_020007c4(2, 0);
    BattleRuntime_WaitIfModeZero_9_020007c4(10);
    Object_SetModeById_1_020007c4(0, 3);
    BattleRuntime_WaitIfModeZero_10_020007c4(40);
    Object_SetModeById_2_020007c4(1, 3);
    Object_SetModeById_3_020007c4(2, 3);
    ObjectMotion_CallThenWaitForAnimationChange_3_020007c4(3, 3);
    BattleRuntime_WaitIfModeZero_11_020007c4(30);
    ObjectMotion_CommitPositionAndActivate_1_020007c4(0, -16, 0);
    ObjectMotion_OffsetPositionAndReset_1(2, 0, -16);
    ObjectMotion_OffsetPositionAndReset_2(3, 0, -8);
    ObjectMotion_CommitPositionAndActivate_2_020007c4(0, 0, -16);
    Object_SetModeById_4_020007c4(3, 1);
    Object_SetModeById_5_020007c4(2, 1);
    ObjectMotion_ArmCallback_3_020007c4(0, 0xc000, 0);
    ObjectMotion_ArmCallback_4_020007c4(1, 0xc000, 0);
    ObjectMotion_ArmCallback_5_020007c4(3, 0xc000, 0);
    ObjectMotion_ArmCallback_6_020007c4(2, 0xc000, 0);
    ObjectMotion_ArmCallback_7_020007c4(22, 0x4000, 0);
    BattleRuntime_WaitIfModeZero_12_020007c4(80);
    ObjectMotion_SetSpeedParameters_2_020007c4(22, 0xcccc, 0x6666);
    /* Set the byte at offset 85 of actor 22's record (from Func_02001ee4) to 2. */
    *(u8 *)(Scene_GetRecord_1_020007c4(22) + 85) = 2;
    ObjectMotion_SetActionVariant_1(22, 2);
    Call6(Func_02001eb6, 34, 0, 1, 2, 4, 18);
    Audio_PlayCue_1(158);
    BattleRuntime_WaitIfModeZero_13_020007c4(20);
    ObjectMotion_SetHorizontalPositionWithTerrain_1_020007c4(22, 0x480000, 0x1380000);
    BattleRuntime_WaitIfModeZero_14_020007c4(20);
    ObjectMotion_CommitPositionAndActivate_3(22, 0, 16);
    Call6(Func_02001ef0, 32, 0, 1, 2, 4, 18);
    Audio_PlayCue_2(159);
    BattleRuntime_WaitIfModeZero_15_020007c4(20);
    BattleEvent_RunActionAndWait_4_020007c4(22, 0);
    BattleRuntime_WaitIfModeZero_16_020007c4(10);
    Object_SetModeById_6(0, 3);
    Object_SetModeById_7(1, 3);
    Object_SetModeById_8(2, 3);
    ObjectMotion_CallThenWaitForAnimationChange_4_020007c4(3, 3);
    BattleRuntime_WaitIfModeZero_17_020007c4(40);
    ObjectMotion_CommitPositionAndActivate_4(22, 16, 0);
    ObjectMotion_ArmCallback_8_020007c4(22, 0x4000, 0);
    BattleRuntime_WaitIfModeZero_18_020007c4(20);
    BattleRuntime_WaitIfModeZero_19_020007c4(10);
    ObjectMotion_CallThenWaitForAnimationChange_5_020007c4(22, 3);
    BattleRuntime_WaitIfModeZero_20_020007c4(30);
    BattleEvent_RunActionAndWait_5_020007c4(22, 0);
    BattleRuntime_WaitIfModeZero_21_020007c4(10);
    ObjectMotion_CallThenWaitForAnimationChange_6_020007c4(3, 3);
    BattleRuntime_WaitIfModeZero_22_020007c4(30);
    BattleEvent_RunActionAndWait_6_020007c4(3, 0);
    BattleRuntime_WaitIfModeZero_23_020007c4(10);
    ObjectMotion_SetVariantCallbackAndInvokeObject_1_020007c4(22, 2);
    BattleRuntime_WaitIfModeZero_24_020007c4(20);
    BattleEvent_RunActionAndWait_7_020007c4(22, 0);
    BattleRuntime_WaitIfModeZero_25_020007c4(10);
    ObjectMotion_SetVariantCallbackAndInvokeObject_2_020007c4(1, 2);
    BattleRuntime_WaitIfModeZero_26_020007c4(20);
    BattleEvent_RunActionAndWait_8_020007c4(1, 0);
    BattleRuntime_WaitIfModeZero_27_020007c4(10);
    BattleEffect_SpawnLinkedResourceObject_2_020007c4(22, 0x102, 40);
    BattleEvent_RunActionAndWait_9_020007c4(22, 0);
    BattleRuntime_WaitIfModeZero_28_020007c4(10);
    BattleEffect_SpawnLinkedResourceObject_3_020007c4(2, 0x100, 40);
    BattleEvent_RunActionAndWait_10_020007c4(2, 0);
    BattleRuntime_WaitIfModeZero_29_020007c4(10);
    ObjectMotion_CallThenWaitForAnimationChange_7_020007c4(22, 3);
    BattleRuntime_WaitIfModeZero_30_020007c4(30);
    BattleEvent_RunActionAndWait_11_020007c4(22, 0);
    BattleRuntime_WaitIfModeZero_31_020007c4(10);
    Object_LinkPair_1(1, 0, 0);
    Object_LinkPair_2(3, 2, 0);
    BattleRuntime_WaitIfModeZero_32_020007c4(60);
    ObjectMotion_ArmCallback_9_020007c4(0, 0xc000, 0);
    ObjectMotion_ArmCallback_10_020007c4(1, 0xc000, 0);
    ObjectMotion_ArmCallback_11_020007c4(2, 0xc000, 0);
    ObjectMotion_ArmCallback_12_020007c4(3, 0xc000, 0);
    BattleRuntime_WaitIfModeZero_33_020007c4(40);
    BattleRuntime_WaitIfModeZero_34_020007c4(10);
    ObjectMotion_CallThenWaitForAnimationChange_8(22, 3);
    BattleRuntime_WaitIfModeZero_35_020007c4(30);
    BattleEvent_RunActionAndWait_12_020007c4(22, 0);
    BattleRuntime_WaitIfModeZero_36_020007c4(10);
    BattleEffect_SpawnLinkedResourceObject_4_020007c4(3, 0x102, 40);
    BattleEvent_RunActionAndWait_13_020007c4(3, 0);
    BattleRuntime_WaitIfModeZero_37_020007c4(10);
    ObjectMotion_CallThenWaitForAnimationChange_9(22, 3);
    BattleRuntime_WaitIfModeZero_38_020007c4(20);
    BattleEvent_RunActionAndWait_14_020007c4(22, 0);
    BattleRuntime_WaitIfModeZero_39_020007c4(10);
    ObjectMotion_ArmCallback_13_020007c4(1, 0xe000, 0);
    BattleRuntime_WaitIfModeZero_40_020007c4(20);
    ObjectMotion_SetSpeedParameters_3_020007c4(1, 0);
    /* Same branch-and-bump shape as above, gated by UiWork_WaitThenFinalizeCapacity_3(0, 0). */
    if (UiWork_WaitThenFinalizeCapacity_4(0, 0) == 0) {
        BattleRuntime_WaitIfModeZero_41_020007c4(20);
        ObjectMotion_CallThenWaitForAnimationChange_10(22, 4);
        BattleRuntime_WaitIfModeZero_42_020007c4(20);
        BattleEvent_RunActionAndWait_15_020007c4(22, 0);
        SCENE_STEP += 1;
    } else {
        BattleRuntime_WaitIfModeZero_43_020007c4(20);
        ObjectMotion_CallThenWaitForAnimationChange_11(22, 4);
        BattleRuntime_WaitIfModeZero_44_020007c4(20);
        SCENE_STEP += 1;
        BattleEvent_RunActionAndWait_16_020007c4(22, 0);
    }
    BattleRuntime_WaitIfModeZero_45_020007c4(10);
    BattleEffect_SpawnLinkedResourceObject_5_020007c4(2, 0x102, 40);
    BattleEvent_RunActionAndWait_17_020007c4(2, 0);
    BattleRuntime_WaitIfModeZero_46_020007c4(10);
    ObjectMotion_ArmCallback_14_020007c4(1, 0xc000, 0);
    BattleEffect_SpawnLinkedResourceObject_6_020007c4(22, 0x106, 50);
    BattleEvent_RunActionAndWait_18_020007c4(22, 0);
    BattleRuntime_WaitIfModeZero_47_020007c4(20);
    ObjectMotion_SetHorizontalPositionWithTerrain_2_020007c4(25, 0x580000, 0x14c0000);
    BattleEvent_RunActionAndWait_19_020007c4(-1, 0);
    BattleRuntime_WaitIfModeZero_48_020007c4(10);
    BattleEffect_SpawnLinkedResourceObject_7_020007c4(3, 0x100, 40);
    ObjectMotion_Launch_1(3, 4, 13);
    ObjectMotion_Launch_2(3, 4, 30);
    BattleEvent_RunActionAndWait_20_020007c4(3, 0);
    BattleRuntime_WaitIfModeZero_49_020007c4(10);
    ObjectMotion_CallThenWaitForAnimationChange_12(22, 3);
    BattleRuntime_WaitIfModeZero_50_020007c4(30);
    BattleEvent_RunActionAndWait_21_020007c4(22, 0);
    /* One extra call, run only when GameFlag_IsSet_1(0x9bf) is non-zero. */
    if (GameFlag_IsSet_2(0x9bf) != 0) {
        Func_02001d8c();
    }
    SceneWork_SetStepValue_2(0x28a5);
    BattleRuntime_WaitIfModeZero_51_020007c4(10);
    ObjectMotion_SetVariantCallbackAndInvokeObject_3_020007c4(22, 2);
    BattleRuntime_WaitIfModeZero_52_020007c4(20);
    BattleEvent_RunActionAndWait_22_020007c4(22, 0);
    BattleRuntime_WaitIfModeZero_53_020007c4(10);
    ObjectMotion_CallThenWaitForAnimationChange_13(22, 3);
    BattleRuntime_WaitIfModeZero_54_020007c4(30);
    BattleEvent_RunActionAndWait_23_020007c4(22, 0);
    BattleRuntime_WaitIfModeZero_55_020007c4(10);
    BattleEffect_SpawnLinkedResourceObject_8(1, 0x100, 40);
    BattleEvent_RunActionAndWait_24_020007c4(1, 0);
    BattleRuntime_WaitIfModeZero_56_020007c4(10);
    ObjectMotion_SetVariantCallbackAndInvokeObject_4_020007c4(22, 2);
    BattleRuntime_WaitIfModeZero_57_020007c4(20);
    BattleEvent_RunActionAndWait_25(22, 0);
    BattleRuntime_WaitIfModeZero_58_020007c4(20);
    ObjectMotion_CallThenWaitForAnimationChange_14(2, 3);
    BattleRuntime_WaitIfModeZero_59_020007c4(30);
    BattleEvent_RunActionAndWait_26(2, 0);
    BattleRuntime_WaitIfModeZero_60_020007c4(10);
    ObjectMotion_CallThenWaitForAnimationChange_15(22, 3);
    BattleRuntime_WaitIfModeZero_61_020007c4(30);
    ObjectMotion_SetSpeedParameters_4_020007c4(25, 0xcccc, 0x6666);
    ObjectMotion_OffsetPositionAndResetMotion_1(25, 0, 16);
    ObjectMotion_CommitPositionAndActivate_5(22, 0, 16);
    BattleRuntime_WaitIfModeZero_62_020007c4(30);
    Call3((void (*)())Func_0200235e, 25, 0, 0);
    SCENE_STEP += 1;
    Func_02002326(242, 0);
    BattleRuntime_WaitIfModeZero_63_020007c4(10);
    /* Clear bit 0 of the flag byte at offset 90 of the record from
     * Func_02002342, then set it back via a second record accessor. */
    *(u8 *)(Scene_GetRecord_2(22) + 90) &= 254;
    ObjectMotion_CommitPositionAndActivate_6(22, 0, -16);
    {
        u8 *record = Scene_GetRecord_3(22);
        u8 value = *(volatile u8 *)&record[90];

        record[90] = (u8)(value | 1);
    }
    ObjectMotion_ArmCallback_15_020007c4(22, 0x4100, 0);
    BattleRuntime_WaitIfModeZero_64_020007c4(30);
    BattleRuntime_WaitIfModeZero_65_020007c4(10);
    BattleEffect_SpawnLinkedResourceObject_9(22, 0x100, 40);
    BattleEvent_RunActionAndWait_27(22, 0);
    BattleRuntime_WaitIfModeZero_66_020007c4(10);
    ObjectMotion_CallThenWaitForAnimationChange_16(22, 3);
    BattleRuntime_WaitIfModeZero_67_020007c4(30);
    BattleEvent_RunActionAndWait_28(22, 0);
    BattleRuntime_WaitIfModeZero_68_020007c4(20);
    Object_SetModeById_9(0, 3);
    Object_SetModeById_10(1, 3);
    Object_SetModeById_11(2, 3);
    ObjectMotion_CallThenWaitForAnimationChange_17(3, 3);
    BattleRuntime_WaitIfModeZero_69_020007c4(50);
    BattleRuntime_WaitIfModeZero_70_020007c4(10);
    ObjectMotion_CallThenWaitForAnimationChange_18(22, 3);
    BattleRuntime_WaitIfModeZero_71_020007c4(30);
    ObjectMotion_CommitPositionAndActivate_7(22, -16, 0);
    ObjectMotion_ArmCallback_16(22, 0xc000, 0);
    BattleRuntime_WaitIfModeZero_72(20);
    Call6(Func_020023d2, 34, 0, 1, 2, 4, 18);
    Audio_PlayCue_3(158);
    BattleRuntime_WaitIfModeZero_73(10);
    ObjectMotion_CommitPositionAndActivate_8(22, 0, -16);
    ObjectMotion_SetHorizontalPositionWithTerrain_3(22, 0, 0);
    BattleRuntime_WaitIfModeZero_74(10);
    Call6(Func_0200240a, 32, 0, 1, 2, 4, 18);
    Audio_PlayCue_4(159);
    BattleRuntime_WaitIfModeZero_75(50);
    ObjectMotion_ArmCallback_17(0, 0x4000, 0);
    ObjectMotion_SetAngleToward_1_020007c4(1, 0, 0);
    ObjectMotion_SetAngleToward_2_020007c4(2, 0, 0);
    BattleRuntime_WaitIfModeZero_76(20);
    BattleEvent_RunActionAndWait_29(1, 0);
    BattleRuntime_WaitIfModeZero_77(10);
    ObjectMotion_SetVariantCallbackAndInvokeObject_5_020007c4(3, 2);
    BattleRuntime_WaitIfModeZero_78(20);
    BattleEvent_RunActionAndWait_30(3, 0);
    BattleRuntime_WaitIfModeZero_79(10);
    ObjectMotion_CallThenWaitForAnimationChange_19(2, 3);
    BattleRuntime_WaitIfModeZero_80(30);
    BattleEvent_RunActionAndWait_31(2, 0);
    BattleRuntime_WaitIfModeZero_81(10);
    Object_SetModeById_12(0, 3);
    BattleRuntime_WaitIfModeZero_82(40);
    Object_SetModeById_13(1, 3);
    Object_SetModeById_14(2, 3);
    ObjectMotion_CallThenWaitForAnimationChange_20(3, 3);
    BattleRuntime_WaitIfModeZero_83(30);
    ObjectMotion_SetSpeedParameters_5_020007c4(1, 0x13333, 0x9999);
    ObjectMotion_SetSpeedParameters_6_020007c4(3, 0x13333, 0x9999);
    ObjectMotion_SetSpeedParameters_7_020007c4(2, 0x13333, 0x9999);
    Object_SetModeById_15(1, 2);
    record = Scene_GetRecord_4(0);
    if (record != 0) {
        ObjectMotion_ResetAndSetPosition_1_020007c4(1, REC_S16(record, 10), REC_S16(record, 18));
    }
    ObjectMotion_CommitCurrentPositionAndActivate_2_020007c4(1);
    ObjectMotion_SetHorizontalPositionWithTerrain_4(1, 0, 0);
    Object_SetModeById_16(3, 2);
    record = Scene_GetRecord_5(0);
    if (record != 0) {
        ObjectMotion_ResetAndSetPosition_2(3, REC_S16(record, 10), REC_S16(record, 18));
    }
    ObjectMotion_CommitCurrentPositionAndActivate_3(3);
    ObjectMotion_SetHorizontalPositionWithTerrain_5(3, 0, 0);
    Object_SetModeById_17(2, 2);
    record = Scene_GetRecord_6(0);
    if (record != 0) {
        ObjectMotion_ResetAndSetPosition_3(2, REC_S16(record, 10), REC_S16(record, 18));
    }
    ObjectMotion_CommitCurrentPositionAndActivate_4(2);
    ObjectMotion_SetHorizontalPositionWithTerrain_6(2, 0, 0);
    Call1((void (*)())Func_020025a8, 10);
    BattleRuntime_ScheduleShoulderButtonModeUpdate_1_020007c4();
}

void FieldScene_RunScene3c6SequenceA(void)
{
    u32 i;
    u8 *record;

    SceneWork_SetStepValue_1_02001090(0x28b0);
    BattleRuntime_WaitIfModeZero_1_02001090(20);
    ObjectMotion_SetVariantCallbackAndInvokeObject_1_02001090(22, 2);
    BattleRuntime_WaitIfModeZero_2_02001090(20);
    BattleEvent_RunActionAndWait_1_02001090(22, 0);
    BattleRuntime_WaitIfModeZero_3_02001090(10);
    ObjectMotion_CallThenWaitForAnimationChange_1_02001090(22, 4);
    BattleRuntime_WaitIfModeZero_4_02001090(20);
    BattleEvent_RunActionAndWait_2_02001090(22, 0);
    BattleRuntime_WaitIfModeZero_5_02001090(10);
    BattleEffect_SpawnLinkedResourceObject_1_02001090(0, 0x100, 0);
    BattleEffect_SpawnLinkedResourceObject_2_02001090(1, 0x100, 0);
    BattleEffect_SpawnLinkedResourceObject_3_02001090(3, 0x100, 0);
    BattleEffect_SpawnLinkedResourceObject_4_02001090(2, 0x100, 55);
    BattleRuntime_WaitIfModeZero_6_02001090(10);
    ObjectMotion_SetVariantCallbackAndInvokeObject_2_02001090(1, 2);
    BattleRuntime_WaitIfModeZero_7_02001090(20);
    BattleEvent_RunActionAndWait_3_02001090(1, 0);
    BattleRuntime_WaitIfModeZero_8_02001090(10);
    ObjectMotion_CallThenWaitForAnimationChange_2_02001090(22, 4);
    BattleRuntime_WaitIfModeZero_9_02001090(20);
    BattleEvent_RunActionAndWait_4_02001090(22, 0);
    BattleRuntime_WaitIfModeZero_10_02001090(10);
    BattleEffect_SpawnLinkedResourceObject_5_02001090(3, 0x102, 40);
    BattleEvent_RunActionAndWait_5_02001090(3, 0);
    BattleRuntime_WaitIfModeZero_11_02001090(10);
    ObjectMotion_CallThenWaitForAnimationChange_3_02001090(1, 3);
    BattleRuntime_WaitIfModeZero_12_02001090(30);
    BattleEvent_RunActionAndWait_6_02001090(1, 0);
    BattleRuntime_WaitIfModeZero_13_02001090(10);
    ObjectMotion_ArmCallback_1_02001090(2, 0x8000, 0);
    BattleRuntime_WaitIfModeZero_14_02001090(65);
    ObjectMotion_ArmCallback_2_02001090(2, 0xc000, 0);
    BattleRuntime_WaitIfModeZero_15_02001090(40);
    ObjectMotion_CallThenWaitForAnimationChange_4_02001090(2, 3);
    BattleRuntime_WaitIfModeZero_16_02001090(30);
    BattleEvent_RunActionAndWait_7_02001090(2, 0);
}

void FieldScene_RunSequenceB(void)
{
    extern u8 Data_03001ebc[];

    u32 i;
    s32 record;

    Func_020026f0();
    Call1(Func_0200278e, 0x28b7);
    Func_020027a6(1, 0);
    Call3(Func_02002802, 0, 0, -16);
    Func_02002716();
}

s32 SceneData_SelectSecondaryTableByFlag9a7(void)
{
    if (Func_02002706(0x9A7) != 0) {
        return 0x02009EE4;
    }
    return 0x02009D04;
}
