#include "types.h"

#define CalculateFixedPointPositionDistance Func_02000030
#define StagedActorStepTable Data_0200beb4
#define GetStagedActor Func_02003d9c
#define FindNextStagedActor Func_02000176
#define FindBlockingStagedActor Func_020001a2
#define FindElevatedBlockingStagedActor Func_020001ce
#define CanStartStagedActorMove Func_02003df8
#define SetStagedActorMode Func_02003de0
#define SelectStagedActorSlot Func_02003dc0
#define StartStagedActorEffect Func_02004036
#define StartNextStagedActorMove Func_02003e0e
#define StartLeadStagedActorMove Func_02003e1e
#define FinishStagedActorMove Func_02003e2c
#define FinishStagedActorEffect Func_02004048
#define SetStagedActorTransition Func_02003e4c
#define StagedActor_AdvancePair Func_020000c4
#define SceneActor_ResetMotionIfBlockedAhead Func_020002a8
#define TestActorPosition Func_020041b4
#define ActorProbeOffsets Data_0200bf0c
#define ActorSearchStep Data_0200beb4
#define SceneActor_FindClearActorPosition Func_02000474
#define Data_0200e1e8 Data_0200bf0c
#define Data_0200e190 Data_0200beb4
#define Func_02006610 Func_020042f4
#define Func_0200661c Func_02004300
#define Func_020066b4 Func_02004398
#define Func_02006714 Func_020043f0
#define Func_0200668a Func_0200437e
#define Func_0200672c Func_02004408
#define Func_020066ea Func_020043ce
#define Func_020066c4 Func_020043b8
#define RefreshStagedActor Func_020043e0
#define Func_02006614 Func_02004348
#define Func_0200661e Func_02004352
#define Func_0200687c Func_020045a0
#define Func_02006658 Func_02004374
#define Func_02006776 Func_02004452
#define Func_0200678e Func_0200446a
#define Func_02006740 Func_02004424
#define Func_020067a4 Func_02004480
#define Func_020067bc Func_02004498
#define Func_020067d4 Func_020044b0
#define Func_0200677a Func_0200445e
#define Func_020068f4 Func_02004618
#define Func_020068fa Func_0200461e
#define Func_020066b8 Func_020043ec
#define Func_02006752 Func_0200445e_a
#define Func_020067ae Func_020044ba
#define Func_020069d0 Func_020046e4
#define GameFlag_Set_1(a0) Value1(Func_020068dc, a0)
#define Audio_PlayCue_1(a0) Value1(Func_02006ac2, a0)
#define BattleRuntime_Reset_1() Value0(Func_02006906)
#define SceneWork_SetStepValue_1(a0) Call1(Func_020069c0, a0)
#define ObjectMotion_SetVariantCallbackAndInvokeObject_1(a0, a1) Value2(Func_020069a8, a0, a1)
#define BattleRuntime_WaitIfModeZero_1(a0) Value1(Func_02006916, a0)
#define BattleEvent_RunActionAndWait_1(a0, a1) Value2(Func_020069e6, a0, a1)
#define BattleRuntime_WaitIfModeZero_2(a0) Value1(Func_02006924, a0)
#define ObjectMotion_PlaceWithinCameraBounds_1(a0, a1, a2, a3) Call4(Func_02006a26, a0, a1, a2, a3)
#define ObjectMotion_SetSpeedParameters_1(a0, a1, a2) Call3(Func_02006968, a0, a1, a2)
#define ObjectMotion_SetPositionAndReset_1(a0, a1, a2) Call3(Func_020069a2, a0, a1, a2)
#define ObjectMotion_ArmCallback_1(a0, a1, a2) Call3(Func_02006a26_a, a0, a1, a2)
#define ObjectMotion_RealignToTrackedObjectAndArmCallback_1(a0, a1, a2, a3) Call4(Func_02006af6, a0, a1, a2, a3)
#define ObjectMotion_RealignToTrackedObjectAndArmCallback_2(a0, a1, a2, a3) Call4(Func_02006b04, a0, a1, a2, a3)
#define ObjectMotion_RealignToTrackedObjectAndArmCallback_3(a0, a1, a2, a3) Value4(Func_02006b12, a0, a1, a2, a3)
#define ObjectMotion_CommitCurrentPositionAndActivate_1(a0) Value1(Func_020069f0, a0)
#define BattleRuntime_WaitIfModeZero_3(a0) Value1(Func_0200698e, a0)
#define BattleEffect_SpawnLinkedResourceObject_1(a0, a1, a2) Value3(Func_02006a72, a0, a1, a2)
#define BattleRuntime_WaitIfModeZero_4(a0) Value1(Func_020069a0, a0)
#define BattleEvent_RunActionAndWait_2(a0, a1) Call2(Func_02006a70, a0, a1)
#define ObjectMotion_SetVariantCallbackAndInvokeObject_2(a0, a1) Value2(Func_02006a48, a0, a1)
#define BattleRuntime_WaitIfModeZero_5(a0) Value1(Func_020069b6, a0)
#define BattleEvent_RunActionAndWait_3(a0, a1) Value2(Func_02006a86, a0, a1)
#define BattleRuntime_WaitIfModeZero_6(a0) Value1(Func_020069c4, a0)
#define BattleEffect_SpawnLinkedResourceObject_2(a0, a1, a2) Call3(Func_02006aa8, a0, a1, a2)
#define BattleEvent_RunActionAndWait_4(a0, a1) Value2(Func_02006aa0, a0, a1)
#define BattleRuntime_WaitIfModeZero_7(a0) Value1(Func_020069de, a0)
#define Object_LinkPair_1(a0, a1, a2) Call3(Func_02006a90, a0, a1, a2)
#define ObjectMotion_ArmCallback_2(a0, a1, a2) Call3(Func_02006ac4, a0, a1, a2)
#define ObjectMotion_ArmCallback_3(a0, a1, a2) Value3(Func_02006ad0, a0, a1, a2)
#define BattleRuntime_WaitIfModeZero_8(a0) Value1(Func_02006a06, a0)
#define BattleEvent_RunActionAndWait_5(a0, a1) Value2(Func_02006ad6, a0, a1)
#define BattleRuntime_WaitIfModeZero_9(a0) Value1(Func_02006a14, a0)
#define BattleRuntime_WaitIfModeZero_10(a0, a1) Call2(Func_02006afe, a0, a1)
#define ObjectMotion_SetVariantCallbackAndInvokeObject_3(a0, a1) Value2(Func_02006abe, a0, a1)
#define BattleRuntime_WaitIfModeZero_11(a0) Value1(Func_02006a2c, a0)
#define BattleEvent_RunActionAndWait_6(a0, a1) Value2(Func_02006afc, a0, a1)
#define BattleRuntime_WaitIfModeZero_12(a0) Value1(Func_02006a3a, a0)
#define Object_LinkPair_2(a0, a1, a2) Call3(Func_02006aec, a0, a1, a2)
#define ObjectMotion_ArmCallback_4(a0, a1, a2) Call3(Func_02006b20, a0, a1, a2)
#define ObjectMotion_ArmCallback_5(a0, a1, a2) Value3(Func_02006b2c, a0, a1, a2)
#define BattleRuntime_WaitIfModeZero_13(a0) Value1(Func_02006a62, a0)
#define BattleEvent_RunActionAndWait_7(a0, a1) Value2(Func_02006b32, a0, a1)
#define BattleRuntime_WaitIfModeZero_14(a0) Call1(Func_02006a70_a, a0)
#define BattleEffect_SpawnLinkedResourceObject_3(a0, a1, a2) Call3(Func_02006b54, a0, a1, a2)
#define BattleEvent_RunActionAndWait_8(a0, a1) Value2(Func_02006b4c, a0, a1)
#define BattleRuntime_WaitIfModeZero_15(a0) Value1(Func_02006a8a, a0)
#define ObjectMotion_CallThenWaitForAnimationChange_1(a0, a1) Call2(Func_02006b12_a, a0, a1)
#define BattleRuntime_WaitIfModeZero_16(a0) Value1(Func_02006a98, a0)
#define BattleEvent_RunActionAndWait_9(a0, a1) Value2(Func_02006b68, a0, a1)
#define BattleRuntime_WaitIfModeZero_17(a0) Value1(Func_02006aa6, a0)
#define BattleEffect_SpawnLinkedResourceObject_4(a0, a1, a2) Call3(Func_02006b88, a0, a1, a2)
#define BattleEvent_RunActionAndWait_10(a0, a1) Value2(Func_02006b80, a0, a1)
#define BattleRuntime_WaitIfModeZero_18(a0) Call1(Func_02006abe_a, a0)
#define ObjectMotion_SetAngleToward_1(a0, a1, a2) Call3(Func_02006b68_a, a0, a1, a2)
#define BattleEffect_SpawnLinkedResourceObject_5(a0, a1, a2) Call3(Func_02006baa, a0, a1, a2)
#define BattleEvent_RunActionAndWait_11(a0, a1) Call2(Func_02006ba2, a0, a1)
#define ObjectMotion_ArmCallback_6(a0, a1, a2) Value3(Func_02006bb6, a0, a1, a2)
#define BattleRuntime_WaitIfModeZero_19(a0) Call1(Func_02006aec_a, a0)
#define BattleEvent_RunActionAndWait_12(a0, a1) Value2(Func_02006bbc, a0, a1)
#define BattleRuntime_WaitIfModeZero_20(a0) Value1(Func_02006afa, a0)
#define BattleEffect_SpawnLinkedResourceObject_6(a0, a1, a2) Call3(Func_02006bde, a0, a1, a2)
#define BattleEvent_RunActionAndWait_13(a0, a1) Call2(Func_02006bd6, a0, a1)
#define ObjectMotion_SetVariantCallbackAndInvokeObject_4(a0, a1) Value2(Func_02006bae, a0, a1)
#define BattleRuntime_WaitIfModeZero_21(a0) Value1(Func_02006b1c, a0)
#define ObjectMotion_CallThenWaitForAnimationChange_2(a0, a1) Value2(Func_02006ba4, a0, a1)
#define BattleRuntime_WaitIfModeZero_22(a0) Value1(Func_02006b2a, a0)
#define BattleEvent_RunActionAndWait_14(a0, a1) Value2(Func_02006bfa, a0, a1)
#define BattleRuntime_WaitIfModeZero_23(a0) Value1(Func_02006b38, a0)
#define BattleEffect_SpawnLinkedResourceObject_7(a0, a1, a2) Call3(Func_02006c1c, a0, a1, a2)
#define BattleEvent_RunActionAndWait_15(a0, a1) Value2(Func_02006c14, a0, a1)
#define BattleRuntime_WaitIfModeZero_24(a0) Value1(Func_02006b52, a0)
#define BattleEffect_SpawnLinkedResourceObject_8(a0, a1, a2) Call3(Func_02006c34, a0, a1, a2)
#define BattleEffect_SpawnLinkedResourceObject_9(a0, a1, a2) Call3(Func_02006c3e, a0, a1, a2)
#define BattleEffect_SpawnLinkedResourceObject_10(a0, a1, a2) Call3(Func_02006c48, a0, a1, a2)
#define BattleEffect_SpawnLinkedResourceObject_11(a0, a1, a2) Value3(Func_02006c52, a0, a1, a2)
#define BattleRuntime_WaitIfModeZero_25(a0) Call1(Func_02006b80_a, a0)
#define ObjectMotion_SetVariantCallbackAndInvokeObject_5(a0, a1) Value2(Func_02006c20, a0, a1)
#define BattleRuntime_WaitIfModeZero_26(a0) Value1(Func_02006b8e, a0)
#define BattleEvent_RunActionAndWait_16(a0, a1) Value2(Func_02006c5e, a0, a1)
#define BattleRuntime_WaitIfModeZero_27(a0) Value1(Func_02006b9c, a0)
#define BattleEvent_RunActionAndWait_17(a0, a1) Value2(Func_02006c6c, a0, a1)
#define BattleRuntime_WaitIfModeZero_28(a0) Call1(Func_02006baa_a, a0)
#define BattleEffect_SpawnLinkedResourceObject_12(a0, a1, a2) Call3(Func_02006c8e, a0, a1, a2)
#define BattleEvent_RunActionAndWait_18(a0, a1) Value2(Func_02006c86, a0, a1)
#define BattleRuntime_WaitIfModeZero_29(a0) Value1(Func_02006bc4, a0)
#define ObjectMotion_SetVariantCallbackAndInvokeObject_6(a0, a1) Value2(Func_02006c64, a0, a1)
#define BattleRuntime_WaitIfModeZero_30(a0) Value1(Func_02006bd2, a0)
#define BattleEvent_RunActionAndWait_19(a0, a1) Call2(Func_02006ca2, a0, a1)
#define ObjectMotion_SetVariantCallbackAndInvokeObject_7(a0, a1) Value2(Func_02006c7a, a0, a1)
#define BattleRuntime_WaitIfModeZero_31(a0) Value1(Func_02006be8, a0)
#define BattleEvent_RunActionAndWait_20(a0, a1) Value2(Func_02006cb8, a0, a1)
#define BattleRuntime_WaitIfModeZero_32(a0) Value1(Func_02006bf6, a0)
#define BattleEffect_SpawnLinkedResourceObject_13(a0, a1, a2) Call3(Func_02006cda, a0, a1, a2)
#define ObjectMotion_SetSpeedParameters_2(a0, a1) Value2(Func_02006cca, a0, a1)
#define ObjectMotion_SetAngleToward_2(a0, a1, a2) Call3(Func_02006cb4, a0, a1, a2)
#define ObjectMotion_SetAngleToward_3(a0, a1, a2) Call3(Func_02006cbe, a0, a1, a2)
#define UiWork_WaitThenFinalizeCapacity_1(a0, a1) Value2(Func_02006c3e_a, a0, a1)
#define SceneWork_SetStepValue_2(a0) Value1(Func_02006ce8, a0)
#define BattleRuntime_WaitIfModeZero_33(a0) Value1(Func_02006c36, a0)
#define BattleEffect_SpawnLinkedResourceObject_14(a0, a1, a2) Call3(Func_02006d1a, a0, a1, a2)
#define ObjectMotion_SetSpeedParameters_3(a0, a1, a2) Call3(Func_02006c78, a0, a1, a2)
#define ObjectMotion_CommitPositionAndActivate_1(a0, a1, a2) Value3(Func_02006dfc, a0, a1, a2)
#define BattleRuntime_WaitIfModeZero_34(a0) Value1(Func_02006c62, a0)
#define Object_LinkPair_3(a0, a1, a2) Call3(Func_02006d14, a0, a1, a2)
#define BattleEvent_RunActionAndWait_21(a0, a1) Value2(Func_02006d3c, a0, a1)
#define SceneWork_SetStepValue_3(a0) Value1(Func_02006d4c, a0)
#define BattleRuntime_WaitIfModeZero_35(a0) Value1(Func_02006c9a, a0)
#define ObjectMotion_SetSpeedParameters_4(a0, a1, a2) Call3(Func_02006cd0, a0, a1, a2)
#define ObjectMotion_CommitPositionAndActivate_2(a0, a1, a2) Value3(Func_02006e54, a0, a1, a2)
#define BattleRuntime_WaitIfModeZero_36(a0) Value1(Func_02006cba, a0)
#define Object_LinkPair_4(a0, a1, a2) Call3(Func_02006d6c, a0, a1, a2)
#define ObjectMotion_CallThenWaitForAnimationChange_3(a0, a1) Call2(Func_02006d4c_a, a0, a1)
#define BattleRuntime_WaitIfModeZero_37(a0) Value1(Func_02006cd2, a0)
#define BattleEvent_RunActionAndWait_22(a0, a1) Value2(Func_02006da2, a0, a1)
#define SceneWork_SetStepValue_4(a0) Value1(Func_02006d9a, a0)
#define BattleEffect_SpawnLinkedResourceObject_15(a0, a1, a2) Call3(Func_02006dc4, a0, a1, a2)
#define ObjectMotion_SetSpeedParameters_5(a0, a1, a2) Call3(Func_02006d22, a0, a1, a2)
#define ObjectMotion_CommitPositionAndActivate_3(a0, a1, a2) Value3(Func_02006ea6, a0, a1, a2)
#define BattleRuntime_WaitIfModeZero_38(a0) Value1(Func_02006d0c, a0)
#define Object_LinkPair_5(a0, a1, a2) Call3(Func_02006dbe, a0, a1, a2)
#define BattleEvent_RunActionAndWait_23(a0, a1) Call2(Func_02006de6, a0, a1)
#define ObjectMotion_CallThenWaitForAnimationChange_4(a0, a1) Value2(Func_02006da6, a0, a1)
#define BattleRuntime_WaitIfModeZero_39(a0) Value1(Func_02006d2c, a0)
#define ObjectMotion_CallThenWaitForAnimationChange_5(a0, a1) Value2(Func_02006db4, a0, a1)
#define BattleRuntime_WaitIfModeZero_40(a0) Value1(Func_02006d3a, a0)
#define ObjectMotion_SetAngleToward_4(a0, a1, a2) Call3(Func_02006de4, a0, a1, a2)
#define ObjectMotion_SetAngleToward_5(a0, a1, a2) Call3(Func_02006dee, a0, a1, a2)
#define ObjectMotion_SetAngleToward_6(a0, a1, a2) Value3(Func_02006df8, a0, a1, a2)
#define BattleRuntime_WaitIfModeZero_41(a0) Value1(Func_02006d5e, a0)
#define BattleEvent_RunActionAndWait_24(a0, a1) Value2(Func_02006e2e, a0, a1)
#define BattleRuntime_WaitIfModeZero_42(a0) Call1(Func_02006d6c_a, a0)
#define ObjectMotion_SetSpeedParameters_6(a0, a1, a2) Call3(Func_02006da2_a, a0, a1, a2)
#define ObjectMotion_CommitPositionAndActivate_4(a0, a1, a2) Value3(Func_02006f24, a0, a1, a2)
#define BattleRuntime_WaitIfModeZero_43(a0) Value1(Func_02006d8a, a0)
#define ObjectMotion_CallThenWaitForAnimationChange_6(a0, a1) Value2(Func_02006e12, a0, a1)
#define BattleRuntime_WaitIfModeZero_44(a0) Value1(Func_02006d98, a0)
#define ObjectMotion_SetAngleToward_7(a0, a1, a2) Value3(Func_02006e42, a0, a1, a2)
#define BattleRuntime_WaitIfModeZero_45(a0) Value1(Func_02006da8, a0)
#define BattleEvent_RunActionAndWait_25(a0, a1) Value2(Func_02006e78, a0, a1)
#define BattleRuntime_WaitIfModeZero_46(a0) Value1(Func_02006db6, a0)
#define Object_LinkPair_6(a0, a1, a2) Call3(Func_02006e68, a0, a1, a2)
#define ObjectMotion_CallThenWaitForAnimationChange_7(a0, a1) Call2(Func_02006e48, a0, a1)
#define ObjectMotion_CallThenWaitForAnimationChange_8(a0, a1) Call2(Func_02006e50, a0, a1)
#define BattleRuntime_WaitIfModeZero_47(a0) Value1(Func_02006dd6, a0)
#define ObjectMotion_SetSpeedParameters_7(a0, a1, a2) Call3(Func_02006e0c, a0, a1, a2)
#define ObjectMotion_CommitPositionAndActivate_5(a0, a1, a2) Value3(Func_02006f8e, a0, a1, a2)
#define BattleRuntime_WaitIfModeZero_48(a0) Value1(Func_02006df4, a0)
#define ObjectMotion_ArmCallback_7(a0, a1, a2) Call3(Func_02006ed0, a0, a1, a2)
#define ObjectMotion_ArmCallback_8(a0, a1, a2) Call3(Func_02006edc, a0, a1, a2)
#define ObjectMotion_ArmCallback_9(a0, a1, a2) Call3(Func_02006ee8, a0, a1, a2)
#define ObjectMotion_ArmCallback_10(a0, a1, a2) Value3(Func_02006ef6, a0, a1, a2)
#define SceneWork_SetStepValue_5(a0) Value1(Func_02006ee4, a0)
#define BattleRuntime_WaitIfModeZero_49(a0) Value1(Func_02006e32, a0)
#define BattleEffect_SpawnLinkedResourceObject_16(a0, a1, a2) Call3(Func_02006f16, a0, a1, a2)
#define BattleEvent_RunActionAndWait_26(a0, a1) Value2(Func_02006f0e, a0, a1)
#define BattleRuntime_WaitIfModeZero_50(a0) Value1(Func_02006e4c, a0)
#define ObjectMotion_CallThenWaitForAnimationChange_9(a0, a1) Value2(Func_02006ed4, a0, a1)
#define BattleRuntime_WaitIfModeZero_51(a0) Value1(Func_02006e5a, a0)
#define BattleEvent_RunActionAndWait_27(a0, a1) Value2(Func_02006f2a, a0, a1)
#define BattleRuntime_WaitIfModeZero_52(a0) Call1(Func_02006e68_a, a0)
#define BattleEffect_SpawnLinkedResourceObject_17(a0, a1, a2) Call3(Func_02006f4c, a0, a1, a2)
#define BattleEvent_RunActionAndWait_28(a0, a1) Value2(Func_02006f44, a0, a1)
#define BattleRuntime_WaitIfModeZero_53(a0) Value1(Func_02006e82, a0)
#define ObjectMotion_CallThenWaitForAnimationChange_10(a0, a1) Value2(Func_02006f0a, a0, a1)
#define BattleRuntime_WaitIfModeZero_54(a0) Value1(Func_02006e90, a0)
#define BattleEvent_RunActionAndWait_29(a0, a1) Value2(Func_02006f60, a0, a1)
#define BattleRuntime_WaitIfModeZero_55(a0) Value1(Func_02006e9e, a0)
#define ObjectMotion_CallThenWaitForAnimationChange_11(a0, a1) Value2(Func_02006f26, a0, a1)
#define BattleRuntime_WaitIfModeZero_56(a0) Value1(Func_02006eac, a0)
#define BattleEvent_RunActionAndWait_30(a0, a1) Value2(Func_02006f7c, a0, a1)
#define BattleRuntime_WaitIfModeZero_57(a0) Value1(Func_02006eba, a0)
#define Object_LinkPair_7(a0, a1, a2) Call3(Func_02006f6c, a0, a1, a2)
#define ObjectMotion_ArmCallback_11(a0, a1, a2) Call3(Func_02006fa0, a0, a1, a2)
#define ObjectMotion_ArmCallback_12(a0, a1, a2) Value3(Func_02006fac, a0, a1, a2)
#define BattleRuntime_WaitIfModeZero_58(a0) Value1(Func_02006ee2, a0)
#define BattleEvent_RunActionAndWait_31(a0, a1) Value2(Func_02006fb2, a0, a1)
#define BattleRuntime_WaitIfModeZero_59(a0) Value1(Func_02006ef0, a0)
#define BattleEffect_SpawnLinkedResourceObject_18(a0, a1, a2) Call3(Func_02006fd2, a0, a1, a2)
#define BattleEvent_RunActionAndWait_32(a0, a1) Value2(Func_02006fca, a0, a1)
#define BattleRuntime_WaitIfModeZero_60(a0) Value1(Func_02006f08, a0)
#define BattleEvent_RunActionAndWait_33(a0, a1) Value2(Func_02006fd8, a0, a1)
#define BattleRuntime_WaitIfModeZero_61(a0) Call1(Func_02006f16_a, a0)
#define Object_LinkPair_8(a0, a1, a2) Call3(Func_02006fc8, a0, a1, a2)
#define ObjectMotion_ArmCallback_13(a0, a1, a2) Call3(Func_02006ffc, a0, a1, a2)
#define ObjectMotion_ArmCallback_14(a0, a1, a2) Value3(Func_02007008, a0, a1, a2)
#define BattleRuntime_WaitIfModeZero_62(a0) Value1(Func_02006f3e, a0)
#define BattleEvent_RunActionAndWait_34(a0, a1) Value2(Func_0200700e, a0, a1)
#define BattleRuntime_WaitIfModeZero_63(a0) Call1(Func_02006f4c_a, a0)
#define Object_LinkPair_9(a0, a1, a2) Call3(Func_02006ffe, a0, a1, a2)
#define ObjectMotion_ArmCallback_15(a0, a1, a2) Call3(Func_02007032, a0, a1, a2)
#define ObjectMotion_ArmCallback_16(a0, a1, a2) Value3(Func_0200703e, a0, a1, a2)
#define BattleRuntime_WaitIfModeZero_64(a0) Value1(Func_02006f74, a0)
#define BattleEvent_RunActionAndWait_35(a0, a1) Value2(Func_02007044, a0, a1)
#define BattleRuntime_WaitIfModeZero_65(a0) Value1(Func_02006f82, a0)
#define ObjectMotion_SetVariantCallbackAndInvokeObject_8(a0, a1) Value2(Func_02007022, a0, a1)
#define BattleRuntime_WaitIfModeZero_66(a0) Value1(Func_02006f90, a0)
#define BattleEvent_RunActionAndWait_36(a0, a1) Value2(Func_02007060, a0, a1)
#define BattleRuntime_WaitIfModeZero_67(a0) Value1(Func_02006f9e, a0)
#define BattleEvent_RunActionAndWait_37(a0, a1) Value2(Func_0200706e, a0, a1)
#define BattleRuntime_WaitIfModeZero_68(a0) Call1(Func_02006fac_a, a0)
#define ObjectMotion_SetVariantCallbackAndInvokeObject_9(a0, a1) Value2(Func_0200704c, a0, a1)
#define BattleRuntime_WaitIfModeZero_69(a0) Value1(Func_02006fba, a0)
#define BattleEvent_RunActionAndWait_38(a0, a1) Value2(Func_0200708a, a0, a1)
#define BattleRuntime_WaitIfModeZero_70(a0) Call1(Func_02006fc8_a, a0)
#define BattleEffect_SpawnLinkedResourceObject_19(a0, a1, a2) Call3(Func_020070ac, a0, a1, a2)
#define BattleEvent_RunActionAndWait_39(a0, a1) Value2(Func_020070a4, a0, a1)
#define BattleRuntime_WaitIfModeZero_71(a0) Value1(Func_02006fe2, a0)
#define BattleEffect_SpawnLinkedResourceObject_20(a0, a1, a2) Call3(Func_020070c4, a0, a1, a2)
#define BattleEvent_RunActionAndWait_40(a0, a1) Value2(Func_020070bc, a0, a1)
#define BattleRuntime_WaitIfModeZero_72(a0) Value1(Func_02006ffa, a0)
#define BattleEffect_SpawnLinkedResourceObject_21(a0, a1, a2) Call3(Func_020070de, a0, a1, a2)
#define BattleEvent_RunActionAndWait_41(a0, a1) Call2(Func_020070d6, a0, a1)
#define ObjectMotion_SetVariantCallbackAndInvokeObject_10(a0, a1) Value2(Func_020070ae, a0, a1)
#define BattleRuntime_WaitIfModeZero_73(a0) Value1(Func_0200701c, a0)
#define BattleRuntime_WaitIfModeZero_74(a0) Call1(Func_02007022_a, a0)
#define ObjectMotion_CallThenWaitForAnimationChange_12(a0, a1) Value2(Func_020070aa, a0, a1)
#define BattleRuntime_WaitIfModeZero_75(a0) Value1(Func_02007030, a0)
#define BattleEvent_RunActionAndWait_42(a0, a1) Value2(Func_02007100, a0, a1)
#define BattleRuntime_WaitIfModeZero_76(a0) Call1(Func_0200703e_a, a0)
#define ObjectMotion_SetAngleToward_8(a0, a1, a2) Call3(Func_020070e8, a0, a1, a2)
#define ObjectMotion_CallThenWaitForAnimationChange_13(a0, a1) Value2(Func_020070d0, a0, a1)
#define BattleRuntime_WaitIfModeZero_77(a0) Value1(Func_02007056, a0)
#define ObjectMotion_SetAngleToward_9(a0, a1, a2) Call3(Func_02007100_a, a0, a1, a2)
#define ObjectMotion_SetAngleToward_10(a0, a1, a2) Value3(Func_0200710a, a0, a1, a2)
#define BattleRuntime_WaitIfModeZero_78(a0) Value1(Func_02007070, a0)
#define Object_SetModeById_1(a0, a1) Call2(Func_020070f0, a0, a1)
#define ObjectMotion_CallThenWaitForAnimationChange_14(a0, a1) Call2(Func_02007100_b, a0, a1)
#define BattleRuntime_WaitIfModeZero_79(a0) Value1(Func_02007086, a0)
#define Audio_PlayCue_2(a0) Value1(Func_02007266, a0)
#define ObjectMotion_SetSpeedParameters_8(a0, a1, a2) Call3(Func_020070d0_a, a0, a1, a2)
#define ObjectMotion_SetSpeedParameters_9(a0, a1, a2) Call3(Func_020070da, a0, a1, a2)
#define ObjectMotion_SetSpeedParameters_10(a0, a1, a2) Call3(Func_020070e4, a0, a1, a2)
#define Object_SetModeById_2(a0, a1) Value2(Func_0200713c, a0, a1)
#define Scene_GetRecord_1(a0) Value1(Func_020070ea, a0)
#define ObjectMotion_ResetAndSetPosition_1(a0, a1, a2) Value3(Func_02007124, a0, a1, a2)
#define ObjectMotion_CommitCurrentPositionAndActivate_2(a0) Value1(Func_0200714a, a0)
#define ObjectMotion_SetHorizontalPositionWithTerrain_1(a0, a1, a2) Call3(Func_0200715c, a0, a1, a2)
#define Object_SetModeById_3(a0, a1) Value2(Func_0200716c, a0, a1)
#define Scene_GetRecord_2(a0) Value1(Func_0200711a, a0)
#define ObjectMotion_ResetAndSetPosition_2(a0, a1, a2) Value3(Func_02007154, a0, a1, a2)
#define ObjectMotion_CommitCurrentPositionAndActivate_3(a0) Value1(Func_0200717a, a0)
#define ObjectMotion_SetHorizontalPositionWithTerrain_2(a0, a1, a2) Call3(Func_0200718c, a0, a1, a2)
#define Object_SetModeById_4(a0, a1) Value2(Func_0200719c, a0, a1)
#define Scene_GetRecord_3(a0) Value1(Func_0200714a_a, a0)
#define ObjectMotion_ResetAndSetPosition_3(a0, a1, a2) Value3(Func_02007184, a0, a1, a2)
#define ObjectMotion_CommitCurrentPositionAndActivate_4(a0) Value1(Func_020071aa, a0)
#define ObjectMotion_SetHorizontalPositionWithTerrain_3(a0, a1, a2) Call3(Func_020071bc, a0, a1, a2)
#define Audio_PlayCueForPartyMember_1() Call0(Func_02007298)
#define BattleRuntime_ScheduleShoulderButtonModeUpdate_1() Call0(Func_02007164)
#define RECORD_COORD_X_OFFSET 10
#define RECORD_COORD_Y_OFFSET 18
#define FieldScene_SetActor13Value41 Func_020009dc
#define FieldScene_RunBranchingCutsceneSequence Func_02002c44
#define FieldScene_RunFlagGatedActorEightDialogue Func_02003518
#define RuntimeSelectorTable Data_02000240
#define PrimaryRuntimeSelector Value_00000093
#define SecondaryRuntimeSelector Value_00000095
#define TertiaryRuntimeSelector Value_00000097
#define PrimaryOverlayData Data_0200c1b0
#define SecondaryOverlayData Data_0200c270
#define TertiaryOverlayData Data_0200c318
#define DefaultOverlayData Data_0200c198
#define SceneData_GetTablebf70 Func_020009ec
#define SceneData_ReturnZero Func_020009f4
#define SceneData_GetTablec138 Func_020009f8
#define SceneData_SelectDataByRuntimeSelector Func_02000a00
#define SceneData_SelectTableBySceneId Func_02003598
#define SceneState_SetByte1004AndRunWhenIdle Func_02000abc
#define FieldScene_RunIndexedStep0 Func_02000ae8
#define FieldScene_RunIndexedStep1 Func_02000af4
#define FieldScene_RunIndexedStep2 Func_02000b00
#define FieldScene_RunIndexedStep3 Func_02000b0c
#define FieldScene_RunIndexedStep4 Func_02000b18
#define FieldScene_RunIndexedStep5 Func_02000b24
#define FieldScene_RunTwoCallSequence Func_02000d48
#define FieldScene_RunGuardedSixWordStep Func_02000eac
#define FieldScene_RunTwoArmSequenceWithValue217f Func_020034bc
#define SceneEffect_SetupBlendByFlag201 Func_02000b30
#define SceneState_RunFlag200SetupAndPlaceActors16To20 Func_02000c2c
#define SceneState_MarkObjectWhenActorElevenAhead Func_02000d58
#define FieldScene_RunActorElevenCellSetup Func_02000d90
#define SceneActor_SetPositionFromTransformedBase Func_02000f10
#define FIELD(base, type, offset) (*(type *)((u8 *)(base) + (offset)))
#define SceneActor_PlaceFiveActorsInRow Func_02000f6c
#define SceneEffect_UpdateStateMachine Func_02000f94
#define OverlayObject_UpdateThreeStateMotion Func_02003a30

#include "staged_actor.h"
#include "find_clear_actor_position.h"
#include "staged_actor_movement.h"
#include "select_overlay_data_by_runtime_selector.h"

typedef struct {
    s32 unk0;
    s32 unk4;
    s32 unk8;
} Desc;

typedef struct {
    u8 filler0[0x28];
    s16 *unk28;
} Sub;

typedef struct {
    u8 filler0[6];
    u16 unk6;
    s32 unk8;
    s32 unkC;
    s32 unk10;
    u8 filler14[0x10];
    s32 unk24;
    u8 filler28[4];
    s32 unk2C;
    u8 filler30[8];
    s32 unk38;
    u8 filler3C[4];
    s32 unk40;
    u8 filler44[0xC];
    Sub *unk50;
} Ent;

typedef struct {
    s32 a, b, c, d, e, f;
} S6;

typedef struct {
    u8 filler0[8];
    s32 unk8;
    s32 unkC;
    s32 unk10;
} Ent_02000d58;

typedef struct {
    s32 x;
    s32 y;
    s32 z;
} Vec;

extern u8 *Data_03001ebc;
extern u8 Data_02010000[];
extern s32 Data_0200bef4[];
extern u8 Data_0200bf70[];
extern u8 Data_0200c138[];
extern u8 Value_00000093;
extern u8 Value_00000094;
extern u8 Value_00000095;
extern u8 Value_00000096;
extern u8 Value_00000097;
extern u8 Data_0200c688[];
extern u8 Data_0200c724[];
extern u8 Data_0200c76c[];
extern u8 Data_0200c808[];
extern u8 Data_0200c850[];
extern u8 Data_0200c5e0[];
extern u8 Data_02001004;
extern u8 Value_0000217f;
extern u16 *Data_0200bf6c;
extern s8 Data_02001002;
extern u8 Data_02008ee1;

typedef s32(*IwramIntegerSquareRoot)(s32);
struct StagedActor *Func_02003d9c(s32 arg0);
struct StagedActor *Func_02000176(s32 *arg0, struct StagedActor *arg1);
struct StagedActor *Func_020001a2(s32 *arg0, struct StagedActor *arg1);
struct StagedActor *Func_020001ce(s32 *arg0, struct StagedActor *arg1);
s32 Func_02003df8(struct StagedActor *arg0, s32 *arg1);
void Func_02003de0(struct StagedActor *arg0, s32 arg1);
void Func_02003dc0(s32 arg0);
void Func_02004036(s32 arg0);
void Func_02003e0e(struct StagedActor *arg0, s32 arg1, s32 arg2, s32 arg3);
void Func_02003e1e(struct StagedActor *arg0, s32 arg1, s32 arg2, s32 arg3);
void Func_02003e2c(struct StagedActor *arg0);
void Func_02004048(void);
void Func_02003e4c(struct StagedActor *arg0, s32 arg1);
Ent *Func_02000342(Desc *, Ent *);
s32 Func_02003f7a(Ent *, Desc *);
void Func_02004794(s32, s32);
void Func_080770c8();
void Func_080f9010();
void Func_0808a010();
void Func_0808a018();
void Func_0808a020();
s32 Func_0808a070();
u8 *Func_0808a080();
void Func_0808a090();
void Func_0808a0b8();
void Func_0808a0d0();
void Func_0808a0e8();
void Func_0808a0f0();
void Func_0808a100();
void Func_0808a110();
void Func_0808a138();
void Func_0808a148();
void Func_0808a150();
void Func_0808a170();
void Func_0808a178();
void Func_0808a180();
void Func_0808a1b8();
void Func_0808a1e8();
void Func_0808a1f0();
void Func_0808a210();
void Func_0808a460();
void Func_0808a4f0();
void Func_0808a570();
void Func_0808a580();
void Func_020068dc();
void Func_02006ac2();
void Func_02006906();
void Func_02006a42();
void Func_020069c0();
void Func_020069a8();
void Func_02006916();
void Func_020069e6();
void Func_02006924();
void Func_02006a26();
void Func_02006a26_a();
void Func_02006968();
void Func_020069a2();
void Func_02006af6();
void Func_02006b04();
void Func_02006b12();
void Func_02006b12_a();
void Func_020069f0();
void Func_0200698e();
void Func_02006a72();
void Func_020069a0();
void Func_02006a70();
void Func_02006a70_a();
void Func_02006a48();
void Func_020069b6();
void Func_02006a86();
void Func_020069c4();
void Func_02006aa8();
void Func_02006aa0();
void Func_020069de();
void Func_02006a90();
void Func_02006ac4();
void Func_02006ad0();
void Func_02006a06();
void Func_02006ad6();
void Func_02006a14();
void Func_02006afe();
void Func_02006abe();
void Func_02006abe_a();
void Func_02006a2c();
void Func_02006afc();
void Func_02006a3a();
void Func_02006aec();
void Func_02006aec_a();
void Func_02006b20();
void Func_02006b2c();
void Func_02006a62();
void Func_02006b32();
void Func_02006b54();
void Func_02006b4c();
void Func_02006a8a();
void Func_02006a98();
void Func_02006b68();
void Func_02006b68_a();
void Func_02006aa6();
void Func_02006b88();
void Func_02006b80();
void Func_02006b80_a();
void Func_02006baa();
void Func_02006baa_a();
void Func_02006ba2();
void Func_02006bb6();
void Func_02006bbc();
void Func_02006afa();
void Func_02006bde();
void Func_02006bd6();
void Func_02006bae();
void Func_02006b1c();
void Func_02006ba4();
void Func_02006b2a();
void Func_02006bfa();
void Func_02006b38();
void Func_02006c1c();
void Func_02006c14();
void Func_02006b52();
void Func_02006c34();
s32 Func_02006c3e();
s32 Func_02006c3e_a();
void Func_02006c48();
void Func_02006c52();
void Func_02006c20();
void Func_02006b8e();
void Func_02006c5e();
void Func_02006b9c();
void Func_02006c6c();
void Func_02006c8e();
void Func_02006c86();
void Func_02006bc4();
void Func_02006c64();
void Func_02006bd2();
void Func_02006ca2();
void Func_02006c7a();
void Func_02006be8();
void Func_02006cb8();
void Func_02006bf6();
void Func_02006cda();
s32 Func_02006cca();
void Func_02006cb4();
void Func_02006cbe();
void Func_02006ce8();
void Func_02006c36();
void Func_02006d1a();
void Func_02006c78();
void Func_02006dfc();
void Func_02006c62();
void Func_02006d14();
void Func_02006d3c();
void Func_02006d4c();
void Func_02006d4c_a();
void Func_02006c9a();
void Func_02006cd0();
void Func_02006e54();
void Func_02006cba();
void Func_02006d6c();
void Func_02006d6c_a();
void Func_02006cd2();
void Func_02006da2();
void Func_02006da2_a();
void Func_02006d9a();
void Func_02006dc4();
void Func_02006d22();
void Func_02006ea6();
void Func_02006d0c();
void Func_02006dbe();
void Func_02006de6();
void Func_02006da6();
void Func_02006d2c();
void Func_02006db4();
void Func_02006d3a();
void Func_02006de4();
void Func_02006dee();
void Func_02006df8();
void Func_02006d5e();
void Func_02006e2e();
void Func_02006f24();
void Func_02006d8a();
void Func_02006e12();
void Func_02006d98();
void Func_02006e42();
void Func_02006da8();
void Func_02006e78();
void Func_02006db6();
void Func_02006e68();
void Func_02006e68_a();
void Func_02006e48();
void Func_02006e50();
void Func_02006dd6();
void Func_02006e0c();
void Func_02006f8e();
void Func_02006df4();
void Func_02006ed0();
void Func_02006edc();
void Func_02006ee8();
void Func_02006ef6();
void Func_02006ee4();
void Func_02006e32();
void Func_02006f16();
void Func_02006f16_a();
void Func_02006f0e();
void Func_02006e4c();
void Func_02006ed4();
void Func_02006e5a();
void Func_02006f2a();
void Func_02006f4c();
void Func_02006f4c_a();
void Func_02006f44();
void Func_02006e82();
void Func_02006f0a();
void Func_02006e90();
void Func_02006f60();
void Func_02006e9e();
void Func_02006f26();
void Func_02006eac();
void Func_02006f7c();
void Func_02006eba();
void Func_02006f6c();
void Func_02006fa0();
void Func_02006fac();
void Func_02006fac_a();
void Func_02006ee2();
void Func_02006fb2();
void Func_02006ef0();
void Func_02006fd2();
void Func_02006fca();
void Func_02006f08();
void Func_02006fd8();
void Func_02006fc8();
void Func_02006fc8_a();
void Func_02006ffc();
void Func_02007008();
void Func_02006f3e();
void Func_0200700e();
void Func_02006ffe();
void Func_02007032();
void Func_0200703e();
void Func_0200703e_a();
void Func_02006f74();
void Func_02007044();
void Func_02006f82();
void Func_02007022();
void Func_02007022_a();
void Func_02006f90();
void Func_02007060();
void Func_02006f9e();
void Func_0200706e();
void Func_0200704c();
void Func_02006fba();
void Func_0200708a();
void Func_020070ac();
void Func_020070a4();
void Func_02006fe2();
void Func_020070c4();
void Func_020070bc();
void Func_02006ffa();
void Func_020070de();
void Func_020070d6();
void Func_020070ae();
void Func_0200701c();
void Func_020070aa();
void Func_02007030();
void Func_02007100();
void Func_020070e8();
void Func_020070d0();
void Func_020070d0_a();
void Func_02007056();
void Func_02007100_a();
void Func_0200710a();
void Func_02007070();
void Func_020070f0();
void Func_02007100_b();
void Func_02007086();
void Func_02007266();
void Func_020070da();
void Func_020070e4();
void Func_0200713c();
u8 *Func_020070ea();
void Func_02007124();
s32 Func_0200714a();
void Func_0200715c();
void Func_0200716c();
u8 *Func_0200711a();
void Func_02007154();
void Func_0200717a();
void Func_0200718c();
void Func_0200719c();
u8 *Func_0200714a_a();
void Func_02007184();
void Func_020071aa();
void Func_020071bc();
void Func_02007298();
void Func_02007164();
s32 Func_020071a8(s32 value);
s32 Func_020071b2(s32 value);
void Func_020071c4(s32 value);
void Func_020071e8(void);
void Func_0200729e(s32 value);
void Func_020072b6(s32 first, s32 second);
void Func_020071f4(s32 value);
void Func_02007294(s32 first, s32 second);
void Func_02007202(s32 value);
void Func_020072ac(s32 first, s32 second, s32 third);
void Func_02007212(s32 value);
void Func_020072e2(s32 first, s32 second);
void Func_020072a2(s32 first, s32 second);
void Func_02007228(s32 value);
void Func_0200723c(void);
void Func_02001528(void);
void Func_020015aa(s32);
void Func_020015b6(s32);
void Func_020015c2(s32);
void Func_020015ce(s32);
void Func_020015da(s32);
void Func_020015e6(s32);
void Func_02000e10(void);
void Func_02001a48(void);
void Func_02004b62(void);
s32 Func_0200132e(S6 *);
void Func_020014da(S6);
void Func_02004b8e(void);
void Func_02007170(void);
void Func_02007228_a(s32 arg0);
s32 Func_02007238(s32 arg0, s32 arg1);
s32 Func_02007198(s32 arg0, s32 arg1);
void Func_0200718a(s32 arg0);
void Func_02007248(s32 arg0);
void Func_02007260(s32 arg0, s32 arg1);
void Func_020071a0(s32 arg0);
void Func_0200725e(s32 arg0);
void Func_02007276(s32 arg0, s32 arg1);
void Func_020071c2(void);
void Func_0200474c(s32);
void Func_0200490a(s32);
s32 Func_020047ea(s32);
void Func_020015ce_a(void);
s32 Func_020048c0(s32);
void Func_02001770(void);
void Func_020049ba(s32, s32);
void Func_020049c2(s32, s32);
void Func_020049ca(s32, s32);
void Func_020049d2(s32, s32);
void Func_020049da(s32, s32);
Ent *Func_02004a2c(s32);
s32 Func_020049ce(Ent *, Vec *);
u8 *Func_02004a64(s32);
void Func_02004a06(s32, s32, s32, s32, s32, s32);
void Func_02004a66(s32);
u8 *Func_02004bfa(s32);
void Func_02004b68(s32, s32, s32 *);
void Func_02001e8e(s32, s32, u8 *);
s32 Func_02004be0(s32, s32);
u8 *Func_02004cea(s32);
void Func_02001fb2(s32);
void Func_02004ed6(s32);
s32 Func_02007670(void);
void Func_02007684(s32 first, s32 second, s32 *position);
s32 Func_02007898(void *object);
s32 Func_020078b0(void *object);
void Func_020078da(void *object);
void Func_02007904(s32 value);

/*
 * Distance between two three-component 16.16 fixed-point positions.
 *
 * Each argument walks three consecutive 16.16 words in x, y, z order. The
 * per-axis deltas are taken in fixed point, shifted down to integers, squared,
 * and summed; the total is passed to the resident IWRAM integer square root.
 *
 * Expressions are preserved exactly as reconstructed: the walking-pointer form
 * is load-bearing for byte-identity and must not become struct field access.
 */

/*
 * Primary cutscene script -- resource_3bd. A no-argument, void, almost-linear
 * sequence whose one branch rejoins after both arms.
 */

/*
 * Each alias is the per-site call word the overlay image holds -- one word can
 * serve two sites with different targets -- and the macro names the engine
 * function the site reaches, keeping the site's own calling form. Names
 * without a binding elsewhere in the tree are provisional.
 */

/* Signed coordinate halfwords in a placed-actor record, named by offset. */
static __inline__ void Call1(void (*f)(), s32 a0)
{
    f(a0);
}

static __inline__ void Call2(void (*f)(), s32 a0, s32 a1)
{
    f(a0, a1);
}

static __inline__ void Call3(void (*f)(), s32 a0, s32 a1, s32 a2)
{
    f(a0, a1, a2);
}

static __inline__ void Call4(void (*f)(), s32 a0, s32 a1, s32 a2, s32 a3)
{
    f(a0, a1, a2, a3);
}

static __inline__ void Call0(void (*f)())
{
    f();
}

static __inline__ s32 Value0(s32 (*f)())
{
    return f();
}

static __inline__ s32 Value1(s32 (*f)(), s32 a0)
{
    return f(a0);
}

static __inline__ s32 Value2(s32 (*f)(), s32 a0, s32 a1)
{
    return f(a0, a1);
}

static __inline__ s32 Value3(s32 (*f)(), s32 a0, s32 a1, s32 a2)
{
    return f(a0, a1, a2);
}

static __inline__ s32 Value4(s32 (*f)(), s32 a0, s32 a1, s32 a2, s32 a3)
{
    return f(a0, a1, a2, a3);
}

s32 Func_02000030(s32 *first_position, s32 *second_position)
{
    s32 delta_x = (*first_position++ - *second_position++) >> 16;
    s32 delta_y = (*first_position++ - *second_position++) >> 16;
    s32 delta_z = (*first_position - *second_position) >> 16;
    s32 delta_x_squared = delta_x *delta_x;
    s32 delta_y_squared = delta_y *delta_y;
    s32 delta_z_squared = delta_z *delta_z;

    return ((IwramIntegerSquareRoot) 0x030001D8)(delta_x_squared + delta_y_squared + delta_z_squared);
}

s32 *Func_0200006c(s32 *arg0)
{
    s32 **slots = (s32 **)(Data_03001ebc + 0x14);
    u32 i;

    for (i = 8; i <= 65; i++) {
        s32 *p = slots[i];

        if ((arg0[0] >> 20) == (p[2] >> 20)
            && (arg0[1] / 0x10000) == (p[3] / 0x10000)
            && (arg0[2] >> 20) == (p[4] >> 20)) {
            return p;
        }
    }
    return 0;
}

void StagedActor_AdvancePair(void)
{

    s32 destination[3];
    struct StagedActor *lead_actor;
    struct StagedActor *next_actor;
    struct StagedActor *blocking_actor;
    s32 dir;
    u32 step;
    s32 rate;
    s32 zero;

    lead_actor = GetStagedActor(0);
    dir = lead_actor->direction_and_kind >> 12;
    step = StagedActorStepTable[dir];
    destination[0] = lead_actor->x.value + (step & 0xffff0000);
    destination[1] = lead_actor->y;
    step <<= 16;
    destination[2] = lead_actor->z.value + step;
    next_actor = FindNextStagedActor(destination, lead_actor);
    if (next_actor == 0) return;

    step = StagedActorStepTable[dir];
    destination[0] = next_actor->x.value + (step & 0xffff0000);
    destination[1] = next_actor->y;
    step <<= 16;
    destination[2] = next_actor->z.value + step;
    blocking_actor = FindBlockingStagedActor(destination, next_actor);
    if (blocking_actor != 0 && (blocking_actor->collision_flags & 1) != 0) return;

    destination[0] = next_actor->x.value;
    destination[1] = next_actor->y + 0x100000;
    destination[2] = next_actor->z.value;
    blocking_actor = FindElevatedBlockingStagedActor(destination, next_actor);
    if (blocking_actor != 0 && (blocking_actor->collision_flags & 1) != 0) return;

    next_actor->transition_mode = 2;
    step = StagedActorStepTable[dir];
    destination[0] = next_actor->x.value + (step & 0xffff0000);
    destination[1] = next_actor->y;
    step <<= 16;
    destination[2] = next_actor->z.value + step;
    if (CanStartStagedActorMove(next_actor, destination) > 0) return;

    zero = next_actor->transition_busy;
    if (zero != 0) return;

    SetStagedActorMode(lead_actor, 8);
    rate = 0x3333;
    SelectStagedActorSlot(15);
    StartStagedActorEffect(185);
    next_actor->move_rate_x = rate;
    next_actor->move_rate_z = rate;
    StartNextStagedActorMove(next_actor, destination[0], destination[1], destination[2]);
    lead_actor->move_rate_x = rate;
    lead_actor->move_rate_z = rate;
    StartLeadStagedActorMove(lead_actor, destination[0], destination[1], destination[2]);
    FinishStagedActorMove(next_actor);
    FinishStagedActorEffect();
    next_actor->x.value = destination[0];
    next_actor->z.value = destination[2];
    next_actor->unknown_24 = zero;
    next_actor->unknown_2c = zero;
    lead_actor->unknown_38 = 0x80000000;
    lead_actor->unknown_40 = 0x80000000;
    lead_actor->unknown_24 = zero;
    lead_actor->unknown_2c = zero;
    lead_actor->x.value = lead_actor->x.parts.cell << 16;
    lead_actor->z.value = lead_actor->z.parts.cell << 16;
    SetStagedActorTransition(lead_actor, 1);
}

s32 Func_02000244(u32 arg0, s32 arg1, s32 arg2, u32 arg3, u32 arg4, s32 arg5)
{
    u8 *g = (u8 *)Data_03001e70;
    u8 *base;
    u32 i;
    u32 j;

    if (g != 0) {
        if (arg0 <= 2) {
            u32 off = arg0 * 48 + 304;

            base = *(u8 **)(g + off);
        } else {
            base = Data_02010000;
        }
        base += (arg1 + (arg2 << 7)) * 4;
        for (i = 0; i < arg4; i++) {
            u8 *p = base + (i << 9);

            for (j = 0; j < arg3; j++) {
                p[2] = (u8)arg5;
                p += 4;
            }
        }
    }
    return 0;
}

s32 SceneActor_ResetMotionIfBlockedAhead(Ent *a)
{
    extern s32 Data_0200beb4[];

    Desc d;
    u32 idx;
    s32 m;
    Ent *r;

    idx = a->unk6 >> 12;
    m = Data_0200beb4[idx];
    d.unk0 = a->unk8 + (m & 0xffff0000);
    d.unk4 = a->unkC;
    m = m << 16;
    d.unk8 = a->unk10 + m;
    r = Func_02000342(&d, a);
    if (r != 0) {
        u32 i = 0;
        s32 v = *r->unk50->unk28;
        s32 *p = Data_0200bef4;

        do {
            if (v == *p++) {
                goto done;
            }
            i++;
        } while (i <= 5);
        a->unk24 = 0;
        a->unk2C = 0;
        a->unk38 = 0x80000000;
        a->unk40 = 0x80000000;
    }
    m = Data_0200beb4[idx];
    d.unk0 = a->unk8 + (m & 0xffff0000);
    d.unk4 = a->unkC;
    m = m << 16;
    d.unk8 = a->unk10 + m;
    if (Func_02003f7a(a, &d) > 0) {
        a->unk24 = 0;
        a->unk2C = 0;
        a->unk38 = 0x80000000;
        a->unk40 = 0x80000000;
    }
done:
    return 0;
}

s32 SceneActor_FindClearActorPosition(s32 *a)
{
#include "find_clear_actor_position_body.inc"
}

void Func_02000608(StagedActorMovementRequest request)
{
#include "run_staged_actor_movement_and_redraw_body.inc"
}

void FieldScene_SetActor13Value41(void)
{
    Func_02004794(13, 0x41);
}

/* Contiguous unnamed leaf-owner run for resource_3bd. */
u8 *SceneData_GetTablebf70(void)
{
    extern s16 Data_02000240[];

    return Data_0200bf70;
}

s32 SceneData_ReturnZero(void)
{
    extern s16 Data_02000240[];

    return 0;
}

u8 *SceneData_GetTablec138(void)
{
    extern s16 Data_02000240[];

    return Data_0200c138;
}

s32 SceneData_SelectDataByRuntimeSelector(void)
{
    extern s16 Data_02000240[];

#include "select_overlay_data_by_runtime_selector_body.inc"
}

void SceneState_SetByte1004AndRunWhenIdle(s32 val)
{
    u8 *state;
    u8 *d = &Data_02001004;

    state = *(u8 **)0x03001ebc;
    *d = val;
    if (*(s16 *)(state + 0xcb8) == 0) {
        Func_02001528();
    }
}

/* Contiguous unnamed leaf-owner run for resource_3bd. */
void FieldScene_RunIndexedStep0(void)
{
    Func_020015aa(0);
}

void FieldScene_RunIndexedStep1(void)
{
    Func_020015b6(1);
}

void FieldScene_RunIndexedStep2(void)
{
    Func_020015c2(2);
}

void FieldScene_RunIndexedStep3(void)
{
    Func_020015ce(3);
}

void FieldScene_RunIndexedStep4(void)
{
    Func_020015da(4);
}

void FieldScene_RunIndexedStep5(void)
{
    Func_020015e6(5);
}

void SceneEffect_SetupBlendByFlag201(void)
{
    u8 **base = (u8 **)0x03001ebc;
    u8 *state;

    {
        u8 *tmp = *base;
        *(s32 *)(tmp + 0x1c0) = 0x100;
        *(s32 *)(tmp + 0x1c8) = 24;
    }
    Func_0200474c(1);
    Func_0200490a(0x4d);
    state = base[4];
    {
        u16 *slot = (u16 *)(state + 0x52a);
        s32 c = 5;
        *slot = c;
    }
    if (Func_020047ea(0x201) != 0) {
        {
            u16 *slot = (u16 *)(state + 0x534);
            s32 c = 0x1d1d;
            *slot = c;
        }
        {
            u16 *slot = (u16 *)(state + 0x536);
            s32 c = 0x3f;
            *slot = c;
        }
        Func_020015ce_a();
        return;
    } else {
        {
            u16 *slot = (u16 *)(state + 0x534);
            s32 c = 0x3f3f;
            *slot = c;
        }
        {
            u16 *slot = (u16 *)(state + 0x536);
            s32 c = 31;
            *slot = c;
        }
    }
    {
        s32 a = 0x3f42;
        *(u16 *)0x4000050 = a;
    }
    {
        s32 b = 0xc04;
        *(u16 *)0x4000052 = b;
    }
}

void SceneState_RunFlag200SetupAndPlaceActors16To20(void)
{

    u8 *work = *(u8 **)0x03001f30;
    s16 *tbl;

    if (Func_020048c0(0x200) != 0) {
        Func_02001770();
        work[0x34] = 1;
    }
    tbl = Data_02000240;
    if (tbl[0xe0] == (s32)&Value_00000097) {
        Func_020049ba(16, 6);
        Func_020049c2(17, 6);
        Func_020049ca(18, 6);
        Func_020049d2(19, 6);
        Func_020049da(20, 6);
    }
}

void FieldScene_RunTwoCallSequence(void)
{
    Func_02000e10();
    Func_02001a48();
}

/* Contiguous unnamed state-owner run for resource_3bd. */
void SceneState_MarkObjectWhenActorElevenAhead(void)
{
    u8 *obj = *(u8 **)0x03001f30;
    Ent_02000d58 *p = Func_02004a2c(11);
    Vec v;

    v.x = p->unk8;
    v.y = p->unkC;
    v.z = p->unk10;

    if (Func_020049ce(p, &v) > 0) {
        obj[0x35] = 1;
    }
}

void FieldScene_RunActorElevenCellSetup(void)
{
    u8 *obj = *(u8 **)0x03001f30;
    u8 *p = Func_02004a64(11);
    s32 t;

    obj += 0x35;
    t = *obj;
    t = (s8)t;
    if (t == 0) {
        s32 a = 0x49;
        s32 b = 0x11;
        Func_02004a06(0x4c, 0x10, 1, 1, a, b);
        if (p != 0) {
            s32 c = 2;
            p[0x55] = c;
            p[0x23] = t;
        }
        Func_02004a66(0x211);
    }
}

void FieldScene_RunGuardedSixWordStep(void)
{
    S6 s;

    Func_02004b62();
    if (Func_0200132e(&s) != 0) {
        Func_020014da(s);
    }
    Func_02004b8e();
}

void SceneActor_SetPositionFromTransformedBase(s32 a, s32 b, s32 c)
{
    s32 k1 = 0x1f80000;
    s32 k2 = 0x180000;
    s32 k3 = 0x900000;
    u8 *obj = Func_02004bfa(a);
    s32 buf[3];
    s32 *bp = buf;

    bp[0] = k1;
    bp[2] = k2;
    Func_02004b68(b, c, bp);
    *(s32 *)(obj + 8) = bp[0];
    *(s32 *)(obj + 12) = bp[2];
    *(s32 *)(obj + 16) = k3;
}

/* Contiguous unnamed state-owner run for resource_3bd. */
void SceneActor_PlaceFiveActorsInRow(u8 *p)
{
    s32 i = 0;

    do {
        Func_02001e8e(i + 11, 0x180000, p);
        p -= 13107;
        i++;
    } while (i <= 4);
}

void SceneEffect_UpdateStateMachine(void)
{
    u16 *p = Data_0200bf6c;
    s32 flag = 1;
    s32 state = *(s16 *)p;

    if (state == 0) {
        s32 t = p[4] + 16;
        p[4] = t;
        if ((u16)t > 0xbff) {
            p[0] = p[0] + 1;
            p[1] = state;
        }
    } else if (state == 1) {
        if ((s16)p[1] == 30) {
            p[0] = p[0] + 1;
        }
    } else if (state == 2) {
        s32 t = p[4] + 0xfff8;
        p[4] = t;
        if ((u16)t <= 0x2ff) {
            p[0] = p[0] + 1;
        }
    } else if (state == 3) {
        s32 v = Data_02001002;
        s32 r = Func_02004be0(v << 16, 5);
        if ((unsigned int)(((p[3] - r) << 16) + 0xc2ff0000) <= 0x5fe0000) {
            u8 *o;
            s32 nv = r + 0x4000;
            p[3] = nv;
            {
                s32 z = 0;
                s32 k = 0x63;
                p[0] = k;
                p[4] = z;
            }
            o = Func_02004cea(v + 11);
            *(s32 *)(o + 0x6c) = (s32)&Data_02008ee1;
        }
    } else if (state == 0x63) {
        flag = 0;
    }
    if (flag != 0) {
        u16 *q2;
        Data_0200bf6c[3] += Data_0200bf6c[4];
        Func_02001fb2(Data_0200bf6c[3]);
        q2 = Data_0200bf6c;
        {
            s32 t2 = q2[5] + q2[4];
            q2[5] = t2;
            if ((u16)t2 > 0x3000) {
                s32 z2 = 0;
                q2[5] = z2;
                Func_02004ed6(0x87);
            }
        }
    }
    {
        u16 *q = Data_0200bf6c;
        q[1]++;
    }
}

/*
 * Every call site is written out separately and repeated calls must not be
 * folded: the sequence of distinct call words is what reproduces the
 * reference. The three record lookups near the end are null checked before
 * their stored coordinates are forwarded.
 */
void FieldScene_RunBranchingCutsceneSequence(void)
{
    u8 *record;
    s32 line;
    GameFlag_Set_1(0x960);
    Audio_PlayCue_1(24);
    BattleRuntime_Reset_1();
    Value0(Func_02006a42); /* main:0808a460 */
    SceneWork_SetStepValue_1(0x214f);
    ObjectMotion_SetVariantCallbackAndInvokeObject_1(8, 2);
    BattleRuntime_WaitIfModeZero_1(20);
    BattleEvent_RunActionAndWait_1(8, 0);
    BattleRuntime_WaitIfModeZero_2(10);
    ObjectMotion_PlaceWithinCameraBounds_1(0xf80000, -1, 0xb80000, 1);
    ObjectMotion_SetSpeedParameters_1(0, 0xcccc, 0x6666);
    ObjectMotion_SetPositionAndReset_1(0, 248, 192);
    ObjectMotion_ArmCallback_1(0, 0xc000, 0);
    ObjectMotion_RealignToTrackedObjectAndArmCallback_1(1, -16, 16, 0xc000);
    ObjectMotion_RealignToTrackedObjectAndArmCallback_2(3, 0, 16, 0xc000);
    ObjectMotion_RealignToTrackedObjectAndArmCallback_3(2, 16, 16, 0xc000);
    ObjectMotion_CommitCurrentPositionAndActivate_1(1);
    BattleRuntime_WaitIfModeZero_3(20);
    BattleEffect_SpawnLinkedResourceObject_1(2, 0x102, 0);
    BattleRuntime_WaitIfModeZero_4(40);
    BattleEvent_RunActionAndWait_2(2, 0);
    ObjectMotion_SetVariantCallbackAndInvokeObject_2(3, 2);
    BattleRuntime_WaitIfModeZero_5(20);
    BattleEvent_RunActionAndWait_3(3, 0);
    BattleRuntime_WaitIfModeZero_6(10);
    BattleEffect_SpawnLinkedResourceObject_2(8, 0x100, 40);
    BattleEvent_RunActionAndWait_4(8, 0);
    BattleRuntime_WaitIfModeZero_7(10);
    Object_LinkPair_1(1, 2, 50);
    ObjectMotion_ArmCallback_2(1, 0xc000, 0);
    ObjectMotion_ArmCallback_3(2, 0xc000, 0);
    BattleRuntime_WaitIfModeZero_8(30);
    BattleEvent_RunActionAndWait_5(1, 0);
    BattleRuntime_WaitIfModeZero_9(10);
    BattleRuntime_WaitIfModeZero_10(8, 0x102); /* main:0808a1f0 */
    ObjectMotion_SetVariantCallbackAndInvokeObject_3(8, 2);
    BattleRuntime_WaitIfModeZero_11(30);
    BattleEvent_RunActionAndWait_6(8, 0);
    BattleRuntime_WaitIfModeZero_12(20);
    Object_LinkPair_2(3, 2, 50);
    ObjectMotion_ArmCallback_4(3, 0xc000, 0);
    ObjectMotion_ArmCallback_5(2, 0xc000, 0);
    BattleRuntime_WaitIfModeZero_13(30);
    BattleEvent_RunActionAndWait_7(2, 0);
    BattleRuntime_WaitIfModeZero_14(10);
    BattleEffect_SpawnLinkedResourceObject_3(8, 0x100, 40);
    BattleEvent_RunActionAndWait_8(8, 0);
    BattleRuntime_WaitIfModeZero_15(10);
    ObjectMotion_CallThenWaitForAnimationChange_1(3, 4);
    BattleRuntime_WaitIfModeZero_16(20);
    BattleEvent_RunActionAndWait_9(3, 0);
    BattleRuntime_WaitIfModeZero_17(10);
    BattleEffect_SpawnLinkedResourceObject_4(8, 0x105, 40);
    BattleEvent_RunActionAndWait_10(8, 0);
    BattleRuntime_WaitIfModeZero_18(10);
    ObjectMotion_SetAngleToward_1(1, 0, 30);
    BattleEffect_SpawnLinkedResourceObject_5(1, 0x101, 40);
    BattleEvent_RunActionAndWait_11(1, 0);
    ObjectMotion_ArmCallback_6(1, 0xc000, 0);
    BattleRuntime_WaitIfModeZero_19(20);
    BattleEvent_RunActionAndWait_12(1, 0);
    BattleRuntime_WaitIfModeZero_20(10);
    BattleEffect_SpawnLinkedResourceObject_6(8, 0x102, 40);
    BattleEvent_RunActionAndWait_13(8, 0);
    ObjectMotion_SetVariantCallbackAndInvokeObject_4(8, 2);
    BattleRuntime_WaitIfModeZero_21(20);
    ObjectMotion_CallThenWaitForAnimationChange_2(3, 4);
    BattleRuntime_WaitIfModeZero_22(20);
    BattleEvent_RunActionAndWait_14(3, 0);
    BattleRuntime_WaitIfModeZero_23(30);
    BattleEffect_SpawnLinkedResourceObject_7(8, 0x106, 40);
    BattleEvent_RunActionAndWait_15(8, 0);
    BattleRuntime_WaitIfModeZero_24(10);
    BattleEffect_SpawnLinkedResourceObject_8(0, 0x101, 0);
    BattleEffect_SpawnLinkedResourceObject_9(1, 0x101, 0);
    BattleEffect_SpawnLinkedResourceObject_10(3, 0x101, 0);
    BattleEffect_SpawnLinkedResourceObject_11(2, 0x101, 0);
    BattleRuntime_WaitIfModeZero_25(60);
    ObjectMotion_SetVariantCallbackAndInvokeObject_5(2, 2);
    BattleRuntime_WaitIfModeZero_26(20);
    BattleEvent_RunActionAndWait_16(2, 0);
    BattleRuntime_WaitIfModeZero_27(20);
    BattleEvent_RunActionAndWait_17(8, 0);
    BattleRuntime_WaitIfModeZero_28(10);
    BattleEffect_SpawnLinkedResourceObject_12(1, 0x102, 40);
    BattleEvent_RunActionAndWait_18(1, 0);
    BattleRuntime_WaitIfModeZero_29(10);
    ObjectMotion_SetVariantCallbackAndInvokeObject_6(8, 2);
    BattleRuntime_WaitIfModeZero_30(20);
    BattleEvent_RunActionAndWait_19(8, 0);
    ObjectMotion_SetVariantCallbackAndInvokeObject_7(3, 2);
    BattleRuntime_WaitIfModeZero_31(20);
    BattleEvent_RunActionAndWait_20(3, 0);
    BattleRuntime_WaitIfModeZero_32(10);
    BattleEffect_SpawnLinkedResourceObject_13(8, 0x102, 40);
    ObjectMotion_SetSpeedParameters_2(8, 0); /* main:0808a178 */
    ObjectMotion_SetAngleToward_2(1, 0, 0);
    ObjectMotion_SetAngleToward_3(2, 0, 0);
    if (UiWork_WaitThenFinalizeCapacity_1(0, 0) == 0) {
        SceneWork_SetStepValue_2(0x2164);
        BattleRuntime_WaitIfModeZero_33(20);
        BattleEffect_SpawnLinkedResourceObject_14(1, 0x100, 40);
        ObjectMotion_SetSpeedParameters_3(1, 0x20000, 0x10000);
        ObjectMotion_CommitPositionAndActivate_1(1, 0, -16);
        BattleRuntime_WaitIfModeZero_34(10);
        Object_LinkPair_3(1, 0, 30);
        BattleEvent_RunActionAndWait_21(1, 0);
    } else {
        SceneWork_SetStepValue_3(0x2168);
        BattleRuntime_WaitIfModeZero_35(10);
        ObjectMotion_SetSpeedParameters_4(1, 0x10000, 0x8000);
        ObjectMotion_CommitPositionAndActivate_2(1, 0, -16);
        BattleRuntime_WaitIfModeZero_36(10);
        Object_LinkPair_4(1, 0, 30);
        ObjectMotion_CallThenWaitForAnimationChange_3(1, 3);
        BattleRuntime_WaitIfModeZero_37(20);
        BattleEvent_RunActionAndWait_22(1, 0);
    }

    line = 0x2165;
    SceneWork_SetStepValue_4(line);
    BattleEffect_SpawnLinkedResourceObject_15(2, 0x103, 40);
    ObjectMotion_SetSpeedParameters_5(2, 0x20000, 0x10000);
    ObjectMotion_CommitPositionAndActivate_3(2, 0, -16);
    BattleRuntime_WaitIfModeZero_38(10);
    Object_LinkPair_5(2, 0, 30);
    BattleEvent_RunActionAndWait_23(2, 0);
    ObjectMotion_CallThenWaitForAnimationChange_4(3, 3);
    BattleRuntime_WaitIfModeZero_39(10);
    ObjectMotion_CallThenWaitForAnimationChange_5(3, 3);
    BattleRuntime_WaitIfModeZero_40(20);
    ObjectMotion_SetAngleToward_4(0, 3, 0);
    ObjectMotion_SetAngleToward_5(1, 3, 0);
    ObjectMotion_SetAngleToward_6(2, 3, 0);
    BattleRuntime_WaitIfModeZero_41(20);
    BattleEvent_RunActionAndWait_24(3, 0);
    BattleRuntime_WaitIfModeZero_42(20);
    ObjectMotion_SetSpeedParameters_6(1, 0x10000, 0x8000);
    ObjectMotion_CommitPositionAndActivate_4(1, 0, 16);
    BattleRuntime_WaitIfModeZero_43(30);
    ObjectMotion_CallThenWaitForAnimationChange_6(1, 4);
    BattleRuntime_WaitIfModeZero_44(30);
    ObjectMotion_SetAngleToward_7(1, 0, 0);
    BattleRuntime_WaitIfModeZero_45(20);
    BattleEvent_RunActionAndWait_25(1, 0);
    BattleRuntime_WaitIfModeZero_46(30);
    Object_LinkPair_6(3, 2, 30);
    ObjectMotion_CallThenWaitForAnimationChange_7(2, 3);
    ObjectMotion_CallThenWaitForAnimationChange_8(3, 3);
    BattleRuntime_WaitIfModeZero_47(30);
    ObjectMotion_SetSpeedParameters_7(2, 0x10000, 0x8000);
    ObjectMotion_CommitPositionAndActivate_5(2, 0, 16);
    BattleRuntime_WaitIfModeZero_48(10);
    ObjectMotion_ArmCallback_7(0, 0xc000, 0);
    ObjectMotion_ArmCallback_8(1, 0xc000, 0);
    ObjectMotion_ArmCallback_9(3, 0xc000, 0);
    ObjectMotion_ArmCallback_10(2, 0xc000, 0);
    line += 7;
    SceneWork_SetStepValue_5(line);
    BattleRuntime_WaitIfModeZero_49(30);
    BattleEffect_SpawnLinkedResourceObject_16(8, 0x100, 40);
    BattleEvent_RunActionAndWait_26(8, 0);
    BattleRuntime_WaitIfModeZero_50(10);
    ObjectMotion_CallThenWaitForAnimationChange_9(2, 3);
    BattleRuntime_WaitIfModeZero_51(30);
    BattleEvent_RunActionAndWait_27(2, 0);
    BattleRuntime_WaitIfModeZero_52(10);
    BattleEffect_SpawnLinkedResourceObject_17(8, 0x108, 40);
    BattleEvent_RunActionAndWait_28(8, 0);
    BattleRuntime_WaitIfModeZero_53(10);
    ObjectMotion_CallThenWaitForAnimationChange_10(2, 3);
    BattleRuntime_WaitIfModeZero_54(30);
    BattleEvent_RunActionAndWait_29(8, 0);
    BattleRuntime_WaitIfModeZero_55(10);
    ObjectMotion_CallThenWaitForAnimationChange_11(3, 3);
    BattleRuntime_WaitIfModeZero_56(30);
    BattleEvent_RunActionAndWait_30(8, 0);
    BattleRuntime_WaitIfModeZero_57(10);
    Object_LinkPair_7(1, 0, 50);
    ObjectMotion_ArmCallback_11(0, 0xc000, 0);
    ObjectMotion_ArmCallback_12(1, 0xc000, 0);
    BattleRuntime_WaitIfModeZero_58(30);
    BattleEvent_RunActionAndWait_31(8, 0);
    BattleRuntime_WaitIfModeZero_59(10);
    BattleEffect_SpawnLinkedResourceObject_18(3, 0x101, 40);
    BattleEvent_RunActionAndWait_32(3, 0);
    BattleRuntime_WaitIfModeZero_60(30);
    BattleEvent_RunActionAndWait_33(8, 0);
    BattleRuntime_WaitIfModeZero_61(10);
    Object_LinkPair_8(1, 0, 50);
    ObjectMotion_ArmCallback_13(0, 0xc000, 0);
    ObjectMotion_ArmCallback_14(1, 0xc000, 0);
    BattleRuntime_WaitIfModeZero_62(30);
    BattleEvent_RunActionAndWait_34(8, 0);
    BattleRuntime_WaitIfModeZero_63(10);
    Object_LinkPair_9(3, 2, 50);
    ObjectMotion_ArmCallback_15(3, 0xc000, 0);
    ObjectMotion_ArmCallback_16(2, 0xc000, 0);
    BattleRuntime_WaitIfModeZero_64(30);
    BattleEvent_RunActionAndWait_35(8, 0);
    BattleRuntime_WaitIfModeZero_65(10);
    ObjectMotion_SetVariantCallbackAndInvokeObject_8(1, 2);
    BattleRuntime_WaitIfModeZero_66(20);
    BattleEvent_RunActionAndWait_36(1, 0);
    BattleRuntime_WaitIfModeZero_67(30);
    BattleEvent_RunActionAndWait_37(8, 0);
    BattleRuntime_WaitIfModeZero_68(10);
    ObjectMotion_SetVariantCallbackAndInvokeObject_9(2, 2);
    BattleRuntime_WaitIfModeZero_69(20);
    BattleEvent_RunActionAndWait_38(2, 0);
    BattleRuntime_WaitIfModeZero_70(20);
    BattleEffect_SpawnLinkedResourceObject_19(8, 0x108, 50);
    BattleEvent_RunActionAndWait_39(8, 0);
    BattleRuntime_WaitIfModeZero_71(10);
    BattleEffect_SpawnLinkedResourceObject_20(1, 0x101, 40);
    BattleEvent_RunActionAndWait_40(1, 0);
    BattleRuntime_WaitIfModeZero_72(10);
    BattleEffect_SpawnLinkedResourceObject_21(8, 0x102, 40);
    BattleEvent_RunActionAndWait_41(8, 0);
    ObjectMotion_SetVariantCallbackAndInvokeObject_10(8, 2);
    BattleRuntime_WaitIfModeZero_73(20);
    BattleRuntime_WaitIfModeZero_74(10);
    ObjectMotion_CallThenWaitForAnimationChange_12(3, 4);
    BattleRuntime_WaitIfModeZero_75(20);
    BattleEvent_RunActionAndWait_42(3, 0);
    BattleRuntime_WaitIfModeZero_76(10);
    ObjectMotion_SetAngleToward_8(0, 3, 40);
    ObjectMotion_CallThenWaitForAnimationChange_13(0, 3);
    BattleRuntime_WaitIfModeZero_77(30);
    ObjectMotion_SetAngleToward_9(1, 3, 0);
    ObjectMotion_SetAngleToward_10(2, 3, 0);
    BattleRuntime_WaitIfModeZero_78(20);
    Object_SetModeById_1(1, 3);
    ObjectMotion_CallThenWaitForAnimationChange_14(2, 3);
    BattleRuntime_WaitIfModeZero_79(30);
    Audio_PlayCue_2(17);
    ObjectMotion_SetSpeedParameters_8(1, 0x13333, 0x9999);
    ObjectMotion_SetSpeedParameters_9(2, 0x13333, 0x9999);
    ObjectMotion_SetSpeedParameters_10(3, 0x13333, 0x9999);
    Object_SetModeById_2(1, 2);
    /* If the id-1 record lookup succeeds, forward its stored coordinates. */
    record = Scene_GetRecord_1(0);
    if (record != 0) {
        ObjectMotion_ResetAndSetPosition_1(1, *(s16 *)(record + RECORD_COORD_X_OFFSET), *(s16 *)(record + RECORD_COORD_Y_OFFSET));
    }
    ObjectMotion_CommitCurrentPositionAndActivate_2(1);
    ObjectMotion_SetHorizontalPositionWithTerrain_1(1, 0, 0);
    Object_SetModeById_3(2, 2);
    /* If the id-2 record lookup succeeds, forward its stored coordinates. */
    record = Scene_GetRecord_2(0);
    if (record != 0) {
        ObjectMotion_ResetAndSetPosition_2(2, *(s16 *)(record + RECORD_COORD_X_OFFSET), *(s16 *)(record + RECORD_COORD_Y_OFFSET));
    }
    ObjectMotion_CommitCurrentPositionAndActivate_3(2);
    ObjectMotion_SetHorizontalPositionWithTerrain_2(2, 0, 0);
    Object_SetModeById_4(3, 2);
    /* If the id-3 record lookup succeeds, forward its stored coordinates. */
    record = Scene_GetRecord_3(0);
    if (record != 0) {
        ObjectMotion_ResetAndSetPosition_3(3, *(s16 *)(record + RECORD_COORD_X_OFFSET), *(s16 *)(record + RECORD_COORD_Y_OFFSET));
    }
    ObjectMotion_CommitCurrentPositionAndActivate_4(3);
    ObjectMotion_SetHorizontalPositionWithTerrain_3(3, 0, 0);
    Audio_PlayCueForPartyMember_1();
    BattleRuntime_ScheduleShoulderButtonModeUpdate_1();
}

void FieldScene_RunTwoArmSequenceWithValue217f(void)
{
    s32 val;

    Func_02007170();
    val = (s32)&Value_0000217f;
    Func_02007228_a(val);
    Func_02007238(8, 0);
    if (Func_02007198(0, 0) == 0) {
        Func_0200718a(20);
        Func_02007248(val + 1);
        Func_02007260(8, 0);
    } else {
        Func_020071a0(20);
        Func_0200725e(val + 2);
        Func_02007276(8, 0);
    }
    Func_020071c2();
}

void FieldScene_RunFlagGatedActorEightDialogue(void)
{
    if (Func_020071a8(0x960) == 0)
        return;
    if (Func_020071b2(0x962) != 0)
        return;

    Func_020071c4(0x961);
    Func_020071e8();
    Func_0200729e(0x217d);
    Func_020072b6(8, 0);
    Func_020071f4(10);
    Func_02007294(0, 2);
    Func_02007202(30);
    Func_020072ac(0, 8, 0);
    Func_02007212(30);
    Func_020072e2(8, 0);
    Func_020072a2(0, 3);
    Func_02007228(20);
    Func_0200723c();
}

s32 SceneData_SelectTableBySceneId(void)
{
    extern s16 Data_02000240[];

    s16 v = Data_02000240[224];

    if (v == (s32)&Value_00000093) {
        return (s32)Data_0200c688;
    }
    if (v == (s32)&Value_00000094) {
        return (s32)Data_0200c724;
    }
    if (v == (s32)&Value_00000095) {
        return (s32)Data_0200c76c;
    }
    if (v == (s32)&Value_00000096) {
        return (s32)Data_0200c808;
    }
    if (v == (s32)&Value_00000097) {
        return (s32)Data_0200c850;
    }
    return (s32)Data_0200c5e0;
}

void OverlayObject_UpdateThreeStateMotion(void *obj)
{
    s32 position[3];
    s32 x;
    s32 z;
    u8 *p;
    s32 state;

    p = (u8 *)obj + 0x40;
    state = *(s8 *)p;
    if (state == 0) {
        z = FIELD(obj, s32, 0x18);
        x = FIELD(obj, s32, 0x14);
        FIELD(obj, s32, 8) = z;
        position[2] = z;
        FIELD(obj, s32, 4) = x;
        position[0] = x;
        Func_02007684(0x780000, Func_02007670(), position);
        FIELD(obj, s32, 0xC) = position[0];
        FIELD(obj, s32, 0x10) = position[2];
        FIELD(obj, s32, 0x24) = 0x50000;
        FIELD(obj, s32, 0x20) = 0x50000;
        FIELD(obj, u8, 0x42) = state;
        (*p)++;
        if ((*(s32 *)0x03001800 & 3) == 0)
            Func_02007904(0x86);
    } else if (state == 1) {
        if (Func_02007898(obj) == 0) {
            s32 value = *p;
            value--;
            *p = value;
        }
    } else if (state == 2) {
        if (Func_020078b0(obj) == 0)
            Func_020078da(obj);
    }
}
