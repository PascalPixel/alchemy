#include "types.h"

#define SceneData_SelectOverlayDataBySelector Func_02000030
#define SceneData_GetTable9F14 Func_02000074
#define FieldScene_RunScene37fSequenceA Func_02000154
#define SceneDialogue_RunFlag81aMessageBranch Func_02000200
#define FieldScene_RunOpeningAuxiliarySequence Func_02000258
#define FieldScene_RunFlag821Dialogue Func_020003bc
#define GATE_ID 2081
#define GameFlag_IsSet_1(a0) Value1(Func_02001fb4, a0)
#define GameFlag_IsSet_2(a0) Value1(Func_02001fc0, a0)
#define BattleRuntime_Reset_1() Call0(Func_02001fea)
#define Audio_PlayCue_1(a0) Value1(Func_02002114, a0)
#define BattleRuntime_WaitIfModeZero_1(a0) Value1(Func_02002008, a0)
#define UiText_DrawMessage_1(a0, a1) Value2(Func_02001ff2, a0, a1)
#define BattleRuntime_WaitIfModeZero_2(a0) Call1(Func_02002018, a0)
#define Audio_PlayCue_2(a0) Value1(Func_02002146, a0)
#define BattleRuntime_WaitIfModeZero_3(a0) Value1(Func_0200205e, a0)
#define BattleEffect_SpawnLinkedResourceObject_1(a0, a1, a2) Call3(Func_02002132, a0, a1, a2)
#define BattleRuntime_WaitIfModeZero_4(a0) Value1(Func_02002080, a0)
#define ObjectMotion_ArmCallback_1(a0, a1, a2) Call3(Func_02002144, a0, a1, a2)
#define ObjectMotion_ArmCallback_2(a0, a1, a2) Call3(Func_02002150, a0, a1, a2)
#define ObjectMotion_ArmCallback_3(a0, a1, a2) Call3(Func_0200215a, a0, a1, a2)
#define ObjectMotion_ArmCallback_4(a0, a1, a2) Call3(Func_02002166, a0, a1, a2)
#define ObjectMotion_Launch_1(a0, a1, a2) Call3(Func_02002128, a0, a1, a2)
#define ObjectMotion_Launch_2(a0, a1, a2) Call3(Func_02002132_a, a0, a1, a2)
#define BattleRuntime_WaitIfModeZero_5(a0) Value1(Func_020020d8, a0)
#define UiText_DrawMessage_2(a0, a1) Value2(Func_020020c0, a0, a1)
#define GameFlag_Set_1(a0) Value1(Func_020020d6, a0)
#define GameFlag_Set_2(a0) Call1(Func_020020dc, a0)
#define BattleRuntime_ScheduleShoulderButtonModeUpdate_1() Call0(Func_02002100)
#define BattleRuntime_Reset_1_late() Value0(Func_02002b3c)
#define BattleRuntime_WaitIfModeZero_1_late() Value0(Func_02002c38)
#define ObjectMotion_SetSpeedParameters_1() Value0(Func_02002c4c)
#define Scene_GetRecord_1(a0) Value1(Func_02002b6a, a0)
#define ObjectMotion_SetHorizontalPositionWithTerrain_1(a0, a1, a2) Value3(Func_02002ba8, a0, a1, a2)
#define Scene_GetRecord_2(a0) Value1(Func_02002b7e, a0)
#define ObjectMotion_SetHorizontalPositionWithTerrain_2(a0, a1, a2) Value3(Func_02002bbc, a0, a1, a2)
#define Scene_GetRecord_3(a0) Value1(Func_02002b92, a0)
#define ObjectMotion_SetHorizontalPositionWithTerrain_3(a0, a1, a2) Value3(Func_02002bd0, a0, a1, a2)
#define ObjectMotion_SetSpeedParameters_2(a0, a1, a2) Call3(Func_02002bb2, a0, a1, a2)
#define ObjectMotion_SetSpeedParameters_3(a0, a1, a2) Call3(Func_02002bbc_a, a0, a1, a2)
#define ObjectMotion_SetSpeedParameters_4(a0, a1, a2) Call3(Func_02002bc6, a0, a1, a2)
#define Object_SetModeById_1(a0, a1) Call2(Func_02002bfe, a0, a1)
#define Object_SetModeById_2(a0, a1) Call2(Func_02002c06, a0, a1)
#define Object_SetModeById_3(a0, a1) Call2(Func_02002c0e, a0, a1)
#define ObjectMotion_OffsetPositionAndResetMotion_1(a0, a1, a2) Call3(Func_02002c02, a0, a1, a2)
#define ObjectMotion_OffsetPositionAndResetMotion_2(a0, a1, a2) Call3(Func_02002c0c, a0, a1, a2)
#define ObjectMotion_OffsetPositionAndResetMotion_3(a0, a1, a2) Value3(Func_02002c18, a0, a1, a2)
#define ObjectMotion_CommitCurrentPositionAndActivate_1(a0) Value1(Func_02002c26, a0)
#define Object_SetModeById_4(a0, a1) Call2(Func_02002c3e, a0, a1)
#define Object_SetModeById_5(a0, a1) Call2(Func_02002c46, a0, a1)
#define Object_SetModeById_6(a0, a1) Call2(Func_02002c4e, a0, a1)
#define Object_SetModeById_7(a0, a1) Call2(Func_02002c56, a0, a1)
#define ObjectMotion_ArmCallback_1_late(a0, a1, a2) Call3(Func_02002cba, a0, a1, a2)
#define ObjectMotion_ArmCallback_2_late(a0, a1, a2) Call3(Func_02002cc6, a0, a1, a2)
#define ObjectMotion_ArmCallback_3_late(a0, a1, a2) Call3(Func_02002cd2, a0, a1, a2)
#define ObjectMotion_ArmCallback_4_late(a0, a1, a2) Call3(Func_02002cde, a0, a1, a2)
#define ObjectMotion_ArmCallback_5(a0, a1, a2) Call3(Func_02002ce8, a0, a1, a2)
#define ObjectMotion_ArmCallback_6(a0, a1, a2) Call3(Func_02002cf4, a0, a1, a2)
#define ObjectMotion_ArmCallback_7(a0, a1, a2) Call3(Func_02002d00, a0, a1, a2)
#define ObjectMotion_ArmCallback_8(a0, a1, a2) Call3(Func_02002d0c, a0, a1, a2)
#define ObjectMotion_ArmCallback_9(a0, a1, a2) Call3(Func_02002d18, a0, a1, a2)
#define ObjectMotion_ArmCallback_10(a0, a1, a2) Call3(Func_02002d22, a0, a1, a2)
#define ObjectMotion_ArmCallback_11(a0, a1, a2) Call3(Func_02002d2e, a0, a1, a2)
#define ObjectMotion_ArmCallback_12(a0, a1, a2) Call3(Func_02002d3a, a0, a1, a2)
#define ObjectMotion_ArmCallback_13(a0, a1, a2) Call3(Func_02002d46, a0, a1, a2)
#define ObjectMotion_ArmCallback_14(a0, a1, a2) Call3(Func_02002d52, a0, a1, a2)
#define ObjectMotion_ArmCallback_15(a0, a1, a2) Call3(Func_02002d5e, a0, a1, a2)
#define ObjectMotion_SetVariantCallbackAndInvokeObject_1(a0, a1) Value2(Func_02002d2e_a, a0, a1)
#define BattleRuntime_WaitIfModeZero_2_late(a0) Value1(Func_02002cb4, a0)
#define Object_SetModeById_8(a0, a1) Call2(Func_02002d1c, a0, a1)
#define ObjectMotion_OffsetPositionAndResetMotion_4(a0, a1, a2) Value3(Func_02002d10, a0, a1, a2)
#define ObjectMotion_CommitCurrentPositionAndActivate_2(a0) Value1(Func_02002d1e, a0)
#define Object_SetModeById_9(a0, a1) Value2(Func_02002d36, a0, a1)
#define BattleRuntime_WaitIfModeZero_3_late(a0) Value1(Func_02002cdc, a0)
#define ObjectMotion_ArmCallback_16(a0, a1, a2) Call3(Func_02002da0, a0, a1, a2)
#define ObjectMotion_ArmCallback_17(a0, a1, a2) Call3(Func_02002daa, a0, a1, a2)
#define ObjectMotion_ArmCallback_18(a0, a1, a2) Call3(Func_02002db6, a0, a1, a2)
#define ObjectMotion_SetVariantCallbackAndInvokeObject_2(a0, a1) Value2(Func_02002d86, a0, a1)
#define BattleRuntime_WaitIfModeZero_4_late(a0) Value1(Func_02002d0c_a, a0)
#define Object_SetModeById_10(a0, a1) Call2(Func_02002d74, a0, a1)
#define ObjectMotion_OffsetPositionAndResetMotion_5(a0, a1, a2) Value3(Func_02002d68, a0, a1, a2)
#define ObjectMotion_CommitCurrentPositionAndActivate_3(a0) Value1(Func_02002d76, a0)
#define Object_SetModeById_11(a0, a1) Call2(Func_02002d8e, a0, a1)
#define ObjectMotion_SetSpeedLimitAndAcceleration_1(a0, a1) Call2(Func_02002e12, a0, a1)
#define ObjectMotion_PlaceWithinCameraBounds_1(a0, a1, a2, a3) Call4(Func_02002e2a, a0, a1, a2, a3)
#define Object_CommitPositionThenWaitIfModeZero_1() Value0(Func_02002e36)
#define BattleRuntime_WaitIfModeZero_5_late(a0) Value1(Func_02002d54, a0)
#define ObjectMotion_SetSpeedLimitAndAcceleration_2(a0, a1) Call2(Func_02002e34, a0, a1)
#define ObjectMotion_PlaceWithinCameraBounds_2(a0, a1, a2, a3) Call4(Func_02002e4c, a0, a1, a2, a3)
#define Object_CommitPositionThenWaitIfModeZero_2() Call0(Func_02002e58)
#define ObjectMotion_PlaceWithinCameraBounds_3(a0, a1, a2, a3) Call4(Func_02002e60, a0, a1, a2, a3)
#define Object_CommitPositionThenWaitIfModeZero_3() Call0(Func_02002e6c)
#define Object_SetModeById_12(a0, a1) Call2(Func_02002dec, a0, a1)
#define ObjectMotion_PlaceWithinCameraBounds_4(a0, a1, a2, a3) Value4(Func_02002e7e, a0, a1, a2, a3)
#define Object_CommitPositionThenWaitIfModeZero_4() Value0(Func_02002e8a)
#define BattleRuntime_WaitIfModeZero_6(a0) Value1(Func_02002da8, a0)
#define ObjectMotion_SetSpeedLimitAndAcceleration_3(a0, a1) Call2(Func_02002e88, a0, a1)
#define ObjectMotion_PlaceWithinCameraBounds_5(a0, a1, a2, a3) Call4(Func_02002ea0, a0, a1, a2, a3)
#define Object_CommitPositionThenWaitIfModeZero_5() Call0(Func_02002eac)
#define ObjectMotion_CallThenWaitForAnimationChange_1(a0, a1) Value2(Func_02002e34_a, a0, a1)
#define BattleRuntime_WaitIfModeZero_7(a0) Value1(Func_02002dd2, a0)
#define ObjectMotion_ArmCallback_19(a0, a1, a2) Call3(Func_02002e96, a0, a1, a2)
#define ObjectMotion_ArmCallback_20(a0, a1, a2) Call3(Func_02002ea2, a0, a1, a2)
#define BattleEffect_SpawnLinkedResourceObject_1_late(a0, a1, a2) Call3(Func_02002ebc, a0, a1, a2)
#define SceneWork_SetStepValue_1(a0) Call1(Func_02002e8a_a, a0)
#define BattleRuntime_RunThenWaitIfModeZero_1(a0, a1, a2) Call3(Func_02002eac_a, a0, a1, a2)
#define BattleEffect_SpawnLinkedResourceObject_2(a0, a1, a2) Call3(Func_02002ed8, a0, a1, a2)
#define ObjectMotion_SetVariantCallback_1(a0, a1) Call2(Func_02002e90, a0, a1)
#define BattleRuntime_RunThenWaitIfModeZero_2(a0, a1, a2) Call3(Func_02002eca, a0, a1, a2)
#define ObjectMotion_SetVariantCallback_2(a0, a1) Call2(Func_02002ea2_a, a0, a1)
#define ObjectMotion_SetVariantCallback_3(a0, a1) Call2(Func_02002eaa, a0, a1)
#define ObjectMotion_SetVariantCallback_4(a0, a1) Call2(Func_02002eb2, a0, a1)
#define BattleRuntime_WaitIfModeZero_8(a0, a1) Call2(Func_02002f14, a0, a1)
#define BattleRuntime_WaitIfModeZero_9(a0, a1) Call2(Func_02002f1e, a0, a1)
#define BattleRuntime_WaitIfModeZero_10(a0, a1) Value2(Func_02002f28, a0, a1)
#define BattleRuntime_WaitIfModeZero_11(a0) Value1(Func_02002e5e, a0)
#define ObjectMotion_SetVariantCallbackAndInvokeObject_3(a0, a1) Value2(Func_02002ee6, a0, a1)
#define BattleRuntime_WaitIfModeZero_12(a0) Value1(Func_02002e6c_a, a0)
#define BattleEvent_RunActionAndWait_1(a0, a1) Call2(Func_02002f14_a, a0, a1)
#define ObjectMotion_CallThenWaitForAnimationChange_2(a0, a1) Call2(Func_02002ee4, a0, a1)
#define BattleRuntime_RunThenWaitIfModeZero_3(a0, a1, a2) Call3(Func_02002f2e, a0, a1, a2)
#define Object_LinkPair_1(a0, a1, a2) Value3(Func_02002f18, a0, a1, a2)
#define BattleRuntime_WaitIfModeZero_13(a0) Value1(Func_02002e96_a, a0)
#define ObjectMotion_SetVariantCallback_5(a0, a1) Call2(Func_02002f16, a0, a1)
#define ObjectMotion_SetVariantCallbackAndInvokeObject_4(a0, a1) Value2(Func_02002f26, a0, a1)
#define BattleRuntime_WaitIfModeZero_14(a0) Value1(Func_02002eac_b, a0)
#define ObjectMotion_ArmCallback_21(a0, a1, a2) Call3(Func_02002f70, a0, a1, a2)
#define ObjectMotion_ArmCallback_22(a0, a1, a2) Call3(Func_02002f7c, a0, a1, a2)
#define ObjectMotion_SetVariantCallbackAndInvokeObject_5(a0, a1) Call2(Func_02002f4c, a0, a1)
#define BattleRuntime_RunThenWaitIfModeZero_4(a0, a1, a2) Call3(Func_02002f7e, a0, a1, a2)
#define ObjectMotion_CallThenWaitForAnimationChange_3(a0, a1) Call2(Func_02002f46, a0, a1)
#define BattleRuntime_RunThenWaitIfModeZero_5(a0, a1, a2) Call3(Func_02002f90, a0, a1, a2)
#define ObjectMotion_ArmCallback_23(a0, a1, a2) Call3(Func_02002fac, a0, a1, a2)
#define BattleRuntime_RunThenWaitIfModeZero_6(a0, a1, a2) Call3(Func_02002fa6, a0, a1, a2)
#define ObjectMotion_ArmCallback_24(a0, a1, a2) Call3(Func_02002fc2, a0, a1, a2)
#define ObjectMotion_SetVariantCallbackAndInvokeObject_6(a0, a1) Value2(Func_02002f92, a0, a1)
#define BattleRuntime_WaitIfModeZero_15(a0) Value1(Func_02002f18_a, a0)
#define BattleRuntime_WaitIfModeZero_16(a0, a1) Call2(Func_02002ff2, a0, a1)
#define ObjectMotion_ArmCallback_25(a0, a1, a2) Call3(Func_02002fe6, a0, a1, a2)
#define ObjectMotion_ArmCallback_26(a0, a1, a2) Call3(Func_02002ff0, a0, a1, a2)
#define ObjectMotion_ArmCallback_27(a0, a1, a2) Call3(Func_02002ffc, a0, a1, a2)
#define ObjectMotion_SetVariantCallbackAndInvokeObject_7(a0, a1) Value2(Func_02002fcc, a0, a1)
#define BattleRuntime_WaitIfModeZero_17(a0) Value1(Func_02002f52, a0)
#define BattleRuntime_RunThenWaitIfModeZero_7(a0, a1, a2) Call3(Func_02003004, a0, a1, a2)
#define ObjectMotion_ArmCallback_28(a0, a1, a2) Call3(Func_02003020, a0, a1, a2)
#define ObjectMotion_Launch_1_late(a0, a1, a2) Call3(Func_02002fe2, a0, a1, a2)
#define BattleRuntime_RunThenWaitIfModeZero_8(a0, a1, a2) Call3(Func_02003054, a0, a1, a2)
#define ObjectMotion_ArmCallback_29(a0, a1, a2) Call3(Func_0200306e, a0, a1, a2)
#define ObjectMotion_SetSpeedParameters_5(a0, a1) Value2(Func_02003056, a0, a1)
#define UiWork_WaitThenFinalizeCapacity_1(a0, a1) Value2(Func_02002fe6_a, a0, a1)
#define SceneWork_SetStepValue_2(a0) Value1(Func_02003060, a0)
#define ObjectMotion_SetVariantCallbackAndInvokeObject_8(a0, a1) Call2(Func_02003058, a0, a1)
#define BattleRuntime_RunThenWaitIfModeZero_9(a0, a1, a2) Value3(Func_0200308a, a0, a1, a2)
#define SceneWork_SetStepValue_3(a0) Value1(Func_0200307a, a0)
#define ObjectMotion_ArmCallback_30(a0, a1, a2) Call3(Func_020030ae, a0, a1, a2)
#define BattleRuntime_RunThenWaitIfModeZero_10(a0, a1, a2) Call3(Func_020030a8, a0, a1, a2)
#define ObjectMotion_ArmCallback_31(a0, a1, a2) Call3(Func_020030c4, a0, a1, a2)
#define ObjectMotion_ArmCallback_32(a0, a1, a2) Call3(Func_020030d0, a0, a1, a2)
#define ObjectMotion_ArmCallback_33(a0, a1, a2) Call3(Func_020030dc, a0, a1, a2)
#define BattleEffect_SpawnLinkedResourceObject_3(a0, a1, a2) Call3(Func_020030f8, a0, a1, a2)
#define ObjectMotion_SetVariantCallbackAndInvokeObject_9(a0, a1) Value2(Func_020030b8, a0, a1)
#define BattleRuntime_WaitIfModeZero_18(a0) Value1(Func_0200303e, a0)
#define ObjectMotion_CallThenWaitForAnimationChange_4(a0, a1) Value2(Func_020030ae_a, a0, a1)
#define BattleRuntime_WaitIfModeZero_19(a0) Value1(Func_0200304c, a0)
#define ObjectMotion_ArmCallback_34(a0, a1, a2) Call3(Func_0200310e, a0, a1, a2)
#define ObjectMotion_ArmCallback_35(a0, a1, a2) Call3(Func_0200311a, a0, a1, a2)
#define ObjectMotion_SetVariantCallbackAndInvokeObject_10(a0, a1) Value2(Func_020030ea, a0, a1)
#define BattleRuntime_WaitIfModeZero_20(a0) Value1(Func_02003070, a0)
#define BattleRuntime_RunThenWaitIfModeZero_11(a0, a1, a2) Value3(Func_02003122, a0, a1, a2)
#define ObjectMotion_SetSpeedParameters_6(a0, a1, a2) Call3(Func_020030b4, a0, a1, a2)
#define Object_SetModeById_13(a0, a1) Call2(Func_020030ec, a0, a1)
#define ObjectMotion_OffsetPositionAndResetMotion_6(a0, a1, a2) Value3(Func_020030de, a0, a1, a2)
#define ObjectMotion_CommitCurrentPositionAndActivate_4(a0) Value1(Func_020030ec_a, a0)
#define Object_SetModeById_14(a0, a1) Value2(Func_02003104, a0, a1)
#define BattleRuntime_WaitIfModeZero_21(a0) Value1(Func_020030aa, a0)
#define ObjectMotion_ArmCallback_36(a0, a1, a2) Call3(Func_0200316e, a0, a1, a2)
#define ObjectMotion_ArmCallback_37(a0, a1, a2) Value3(Func_0200317a, a0, a1, a2)
#define ObjectMotion_CommitCurrentPositionAndActivate_5(a0) Value1(Func_02003118, a0)
#define Object_SetModeById_15(a0, a1) Value2(Func_02003130, a0, a1)
#define BattleRuntime_WaitIfModeZero_22(a0) Value1(Func_020030d6, a0)
#define Object_SetModeById_16(a0, a1) Call2(Func_0200313e, a0, a1)
#define Object_SetModeById_17(a0, a1) Call2(Func_02003146, a0, a1)
#define ObjectMotion_CallThenWaitForAnimationChange_5(a0, a1) Value2(Func_02003156, a0, a1)
#define BattleRuntime_WaitIfModeZero_23(a0) Value1(Func_020030f4, a0)
#define Object_SetModeById_18(a0, a1) Value2(Func_0200315c, a0, a1)
#define Scene_GetRecord_4(a0) Value1(Func_0200312a, a0)
#define ObjectMotion_ResetAndSetPosition_1(a0, a1, a2) Value3(Func_0200314c, a0, a1, a2)
#define Object_SetModeById_19(a0, a1) Value2(Func_0200317c, a0, a1)
#define Scene_GetRecord_5(a0) Value1(Func_0200314a, a0)
#define ObjectMotion_ResetAndSetPosition_2(a0, a1, a2) Value3(Func_0200316c, a0, a1, a2)
#define Object_SetModeById_20(a0, a1) Value2(Func_0200319c, a0, a1)
#define Scene_GetRecord_6(a0) Value1(Func_0200316a, a0)
#define ObjectMotion_ResetAndSetPosition_3(a0, a1, a2) Value3(Func_0200318c, a0, a1, a2)
#define ObjectMotion_CommitCurrentPositionAndActivate_6(a0) Value1(Func_020031aa, a0)
#define ObjectMotion_SetHorizontalPositionWithTerrain_4(a0, a1, a2) Call3(Func_020031bc, a0, a1, a2)
#define ObjectMotion_SetHorizontalPositionWithTerrain_5(a0, a1, a2) Call3(Func_020031c6, a0, a1, a2)
#define ObjectMotion_SetHorizontalPositionWithTerrain_6(a0, a1, a2) Call3(Func_020031d0, a0, a1, a2)
#define Object_SetModeById_21(a0, a1) Call2(Func_020031e0, a0, a1)
#define Object_SetModeById_22(a0, a1) Call2(Func_020031e8, a0, a1)
#define Object_SetModeById_23(a0, a1) Value2(Func_020031f0, a0, a1)
#define GameFlag_Set_1_late(a0) Value1(Func_02003186, a0)
#define GameFlag_Clear_1(a0) Value1(Func_02003194, a0)
#define BattleRuntime_ScheduleShoulderButtonModeUpdate_1_late() Value0(Func_020031b0)
#define RECORD_A32(rec) (*(s32 *)((rec) + 8))
#define RECORD_B32(rec) (*(s32 *)((rec) + 16))
#define RECORD_A16(rec) (*(s16 *)((rec) + 10))
#define RECORD_B16(rec) (*(s16 *)((rec) + 18))
#define Scene_RunTransitionCue Func_0200161c
#define Scene_RunActorFormation Func_020017c0
#define Scene_UpdateCueTimer Func_02001ac8
#define FieldScene_RunScriptedStep953 Func_02001aac
#define FieldScene_RunSupplementalSequenceTwo Func_02000420
#define Scene_UpdateOuterActor9Flags Func_0200056c
#define Scene_UpdateOuterActor10Flags Func_020005ac
#define Scene_UpdateFormationActor9Flags Func_020005ec
#define Scene_UpdateFormationActor10Flags Func_02000634
#define Scene_UpdateFormationActor11Flags Func_0200067c
#define Scene_UpdateFormationActor12Flags Func_020006c4
#define Scene_UpdateFormationActor13Flags Func_0200070c
#define Scene_UpdateFormationActor14Flags Func_02000754
#define SceneActor_FindSlotByTilePosition Func_0200079c
#define FieldScene_DispatchByScenarioId Func_020008f4
#define FieldScene_RunScene37f_0200092c Func_0200092c
#define FieldScene_RunScene37f_02000d1c Func_02000d1c
#define FieldScene_RunFourEntitySequence Func_02000f8c
#define SceneData_SelectFormationTable Func_020000ec

extern u8 Value_00000013;
extern u8 Value_00000010;
extern u8 Data_02009d04[];
extern u8 Data_02009d64[];
extern u8 Data_02009cd4[];
extern u8 Data_00000010[];
extern u8 Data_00000013[];
extern u8 Data_02009fc0[];
extern u8 Data_02009fd8[];
extern u8 Data_0200a050[];
extern u8 Data_0200a1b8[];
extern u8 Data_0200a2a8[];
extern s16 Data_02000240_t[][1];
extern void Func_02001db0(void);  /* Func_0808a018 veneer. */
extern s32 Func_02001d96();       /* GameFlag_IsSet veneer, first site. */
extern void Func_02001d9a();      /* UiText_DrawMessage veneer, first site. */
extern void Func_02001da4();      /* UiText_DrawMessage veneer, second site. */
extern s32 Func_02001db2();       /* GameFlag_IsSet veneer, second site. */
extern void Func_02001df0(void);  /* Func_0808a020 veneer. */
extern u8 Value_00001032;
extern u8 Data_0200a69c[];
extern u8 Value_00000fe0;
extern u8 Data_0200a2e4[];
extern u8 Data_0200a524[];
extern u8 Data_0200a41c[];
extern u8 Data_0200a32c[];
extern u8 Data_0200a2d8[];

void Func_02001c70();
void Func_02001c9e();
void Func_02001cb4();
void Func_02001cca();
void Func_02001cd0();
void Func_02001ce6();
void Func_02001cfc();
void Func_02001d06();
void Func_02001d80();
void Func_02001d8c();
void Func_02001da2();
void Func_02001da6();
void Func_02001dc2();
void Func_02001dce();
void Func_02001e12();
void Func_02001e2c();
void Func_02001e7e();
void Func_02001e8c();
void Func_02001e98();
void Func_02001dde();
s32 Func_02001df0_a();
void Func_02001df2();
s32 Func_02001dfc();
void Func_02001e26();
void Func_02001e28();
void Func_02001e30();
void Func_02001e34();
void Func_02001e40();
void Func_02001e44();
void Func_02001e48();
void Func_02001e56();
void Func_02001e7c();
void Func_02001e9e();
void Func_02001eaa();
void Func_02001ecc();
void Func_02001ef4();
void Func_02001f0e();
void Func_02001f22();
void Func_02001f24();
void Func_02001f2a();
void Func_02001f3a();
void Func_02001f4e();
void Func_02001f50();
void Func_02001f74();
void Func_02001f7e();
void Func_02001f7e_a();
void Func_02001f84();
void Func_02001f90();
void Func_02001f9c();
void Func_02001fa6();
void Func_02001fb2();
void Func_02001f6c();
s32 Func_02001f52();
void Func_02001f56();
s32 Func_02001f66();
void Func_02001f6e();
void Func_02001f82();
void Func_02001fb6();
void Func_02001fa2();
s32 Func_02001fb4();
s32 Func_02001fc0();
void Func_02001fe8();
void Func_02001fe8_a();
void Func_02001fea();
void Func_02001ff2();
void Func_02002004();
void Func_02002008();
void Func_02002018();
void Func_02002030();
void Func_02002052();
void Func_0200205e();
void Func_02002080();
void Func_020020a8();
void Func_020020c0();
void Func_020020d6();
void Func_020020d8();
void Func_020020dc();
void Func_020020fe();
void Func_02002100();
void Func_02002114();
void Func_02002128();
void Func_02002132();
void Func_02002132_a();
void Func_02002144();
void Func_02002146();
void Func_02002150();
void Func_0200215a();
void Func_02002166();
s32 *Func_0200213e(s32);
void Func_0200211c(s32);
void Func_02002122(s32);
void Func_02002124(s32);
void Func_02002130(s32);
s32 *Func_0200217e(s32);
void Func_0200215e(s32);
void Func_02002164(s32);
void Func_02002168(s32);
void Func_02002174(s32);
s32 *Func_020021be(s32);
void Func_0200219e(s32);
void Func_020021a4(s32);
void Func_020021a6(s32);
void Func_020021b4(s32);
void Func_02001de6(s32);
s32 *Func_02002206(s32);
void Func_020021e4(s32);
void Func_020021ea(s32);
void Func_020021ec(s32);
void Func_020021f8(s32);
void Func_02001e2a(s32);
s32 *Func_0200224e(s32);
void Func_0200222e(s32);
void Func_02002234(s32);
void Func_02002236(s32);
void Func_02002244(s32);
void Func_02001e76(s32);
s32 *Func_02002296(s32);
void Func_02002274(s32);
void Func_0200227a(s32);
void Func_0200227c(s32);
void Func_02002288(s32);
void Func_02001eba(s32);
s32 *Func_020022de(s32);
void Func_020022be(s32);
void Func_020022c4(s32);
void Func_020022c6(s32);
void Func_020022d4(s32);
void Func_02001f06(s32);
s32 *Func_02002326(s32);
void Func_02002304(s32);
void Func_0200230a(s32);
void Func_0200230c(s32);
void Func_02002318(s32);
void Func_02001f4a(s32);
void Func_02001236(void);
void Func_0200133a(void);
void Func_0200248a();
void Func_020024ca();
s32 Func_020024d6();
void Func_020024de();
s32 Func_020024f0();
s32 Func_020024f0_a();
void Func_02002502();
void Func_02002514();
void Func_02002540();
s32 Func_02002542();
void Func_02002552();
void Func_02002564();
void Func_020025c4();
void Func_020028cc();
void Func_020028e4();
s32 Func_0200293e();
s32 Func_0200293e_a();
s32 Func_02002952();
s32 Func_02002966();
void Func_0200297c();
void Func_02002986();
void Func_02002990();
void Func_02002990_a();
void Func_0200299a();
void Func_020029a4();
void Func_020029d2();
void Func_020029d6();
void Func_020029d6_a();
void Func_020029d6_b();
void Func_020029d6_c();
void Func_020029da();
void Func_020029e0();
void Func_020029e2();
void Func_020029e6();
void Func_020029ec();
void Func_020029f0();
void Func_020029f4();
void Func_020029fa();
void Func_02002a12();
void Func_02002a1a();
void Func_02002a28();
void Func_02002a36();
void Func_02002a40();
void Func_02002a6e();
s32 Func_02002a7e();
s32 Func_02002a7e_a();
void Func_02002a8a();
void Func_02002a92();
s32 Func_02002a9e();
void Func_02002aa0();
void Func_02002ab0();
void Func_02002ab8();
void Func_02002ab8_a();
s32 Func_02002abe();
void Func_02002ac0();
void Func_02002ac4();
void Func_02002ad0();
void Func_02002ad0_a();
void Func_02002ae0();
void Func_02002ae0_a();
void Func_02002ae0_b();
void Func_02002af0();
void Func_02002afa();
void Func_02002afe();
void Func_02002b10();
void Func_02002b16();
void Func_02002b18();
void Func_02002b1a();
void Func_02002b28();
void Func_02002b2a();
void Func_02002b3a();
void Func_02002b42();
void Func_02002b4a();
void Func_0808a018();
void Func_0808a020();
int Func_0808a070();
s32 Func_02002b3c();
s32 Func_02002b6a();
s32 Func_02002b7e();
s32 Func_02002b92();
s32 Func_02002ba8();
s32 Func_02002bb2();
s32 Func_02002bbc();
s32 Func_02002bbc_a();
s32 Func_02002bc6();
s32 Func_02002bd0();
s32 Func_02002bfe();
s32 Func_02002c02();
s32 Func_02002c06();
s32 Func_02002c0c();
s32 Func_02002c0e();
s32 Func_02002c18();
s32 Func_02002c26();
s32 Func_02002c38();
s32 Func_02002c3e();
s32 Func_02002c46();
s32 Func_02002c4c();
s32 Func_02002c4e();
s32 Func_02002c56();
s32 Func_02002cb4();
s32 Func_02002cba();
s32 Func_02002cc6();
s32 Func_02002cd2();
s32 Func_02002cdc();
s32 Func_02002cde();
s32 Func_02002ce8();
s32 Func_02002cf4();
s32 Func_02002d00();
s32 Func_02002d0c();
s32 Func_02002d0c_a();
s32 Func_02002d10();
s32 Func_02002d18();
s32 Func_02002d1c();
s32 Func_02002d1e();
s32 Func_02002d22();
s32 Func_02002d2e();
s32 Func_02002d2e_a();
s32 Func_02002d36();
s32 Func_02002d3a();
s32 Func_02002d46();
s32 Func_02002d52();
s32 Func_02002d54();
s32 Func_02002d5e();
s32 Func_02002d68();
s32 Func_02002d74();
s32 Func_02002d76();
s32 Func_02002d86();
s32 Func_02002d8e();
s32 Func_02002da0();
s32 Func_02002da8();
s32 Func_02002daa();
s32 Func_02002db6();
s32 Func_02002dd2();
s32 Func_02002dec();
s32 Func_02002e12();
s32 Func_02002e2a();
s32 Func_02002e34();
s32 Func_02002e34_a();
s32 Func_02002e36();
s32 Func_02002e4c();
s32 Func_02002e58();
s32 Func_02002e5e();
s32 Func_02002e60();
s32 Func_02002e6c();
s32 Func_02002e6c_a();
s32 Func_02002e7e();
s32 Func_02002e88();
s32 Func_02002e8a();
s32 Func_02002e8a_a();
s32 Func_02002e90();
s32 Func_02002e96();
s32 Func_02002e96_a();
s32 Func_02002ea0();
s32 Func_02002ea2();
s32 Func_02002ea2_a();
s32 Func_02002eaa();
s32 Func_02002eac();
s32 Func_02002eac_a();
s32 Func_02002eac_b();
s32 Func_02002eb2();
s32 Func_02002ebc();
s32 Func_02002eca();
s32 Func_02002ed8();
s32 Func_02002ee4();
s32 Func_02002ee6();
s32 Func_02002f14();
s32 Func_02002f14_a();
s32 Func_02002f16();
s32 Func_02002f18();
s32 Func_02002f18_a();
s32 Func_02002f1e();
s32 Func_02002f26();
s32 Func_02002f28();
s32 Func_02002f2e();
s32 Func_02002f46();
s32 Func_02002f4c();
s32 Func_02002f52();
s32 Func_02002f70();
s32 Func_02002f7c();
s32 Func_02002f7e();
s32 Func_02002f90();
s32 Func_02002f92();
s32 Func_02002fa6();
s32 Func_02002fac();
s32 Func_02002fc2();
s32 Func_02002fcc();
s32 Func_02002fe2();
s32 Func_02002fe6();
s32 Func_02002fe6_a();
s32 Func_02002ff0();
s32 Func_02002ff2();
s32 Func_02002ffc();
s32 Func_02003004();
s32 Func_02003020();
s32 Func_0200303e();
s32 Func_0200304c();
s32 Func_02003054();
s32 Func_02003056();
s32 Func_02003058();
s32 Func_02003060();
s32 Func_0200306e();
s32 Func_02003070();
s32 Func_0200307a();
s32 Func_0200308a();
s32 Func_020030a8();
s32 Func_020030aa();
s32 Func_020030ae();
s32 Func_020030ae_a();
s32 Func_020030b4();
s32 Func_020030b8();
s32 Func_020030c4();
s32 Func_020030d0();
s32 Func_020030d6();
s32 Func_020030dc();
s32 Func_020030de();
s32 Func_020030ea();
s32 Func_020030ec();
s32 Func_020030ec_a();
s32 Func_020030f4();
s32 Func_020030f8();
s32 Func_02003104();
s32 Func_0200310e();
s32 Func_02003118();
s32 Func_0200311a();
s32 Func_02003122();
s32 Func_0200312a();
s32 Func_02003130();
s32 Func_0200313e();
s32 Func_02003146();
s32 Func_0200314a();
s32 Func_0200314c();
s32 Func_02003156();
s32 Func_0200315c();
s32 Func_0200316a();
s32 Func_0200316c();
s32 Func_0200316e();
s32 Func_0200317a();
s32 Func_0200317c();
s32 Func_02003186();
s32 Func_0200318c();
s32 Func_02003194();
s32 Func_0200319c();
s32 Func_020031aa();
s32 Func_020031b0();
s32 Func_020031bc();
s32 Func_020031c6();
s32 Func_020031d0();
s32 Func_020031e0();
s32 Func_020031e8();
s32 Func_020031f0();
void Func_020031cc();
void Func_020031e0_a();
s32 Func_0200320e();
void Func_0200322a();
void Func_02003232();
void Func_0200324c();
void Func_0200325e();
void Func_02003266();
void Func_0200326a();
void Func_0200326c();
void Func_02003284();
void Func_0200328a();
void Func_020032ac();
void Func_020032d6();
void Func_020032ea();
void Func_020032ee();
void Func_020032fa();
void Func_0200331e();
void Func_02003322();
s32 Func_0200332e();
void Func_02003334();
void Func_0200333c();
void Func_02003342();
void Func_02003346();
void Func_02003348();
void Func_02003350();
void Func_02003354();
void Func_02003360();
void Func_02003360_a();
void Func_02003362();
void Func_0200336a();
void Func_0200336c();
void Func_0200336e();
void Func_02003376();
void Func_02003376_a();
void Func_02003380();
void Func_02003382();
void Func_0200338e();
void Func_020033a0();
void Func_020033a2();
void Func_020033a6();
s32 Func_0200333c_a();
void Func_0200334e();
void Func_02003360_b();
void Func_02003372();
void Func_02003384();
void Func_02003396();
void Func_020033b0();
s32 Func_020033c4();
void Func_020033e0();
s32 Func_020033f4();
void Func_02003410();
s32 Func_02003420();
void Func_02003442();
s32 Func_02003452();
void Func_0200345a();
void Func_02003472();
s32 Func_02003482();
void Func_0200348a();
void Func_020034a6();
s32 Func_020034b6();
void Func_020034ba();
void Func_020034d6();
s32 Func_020034e6();
void Func_020034ec();
void Func_02003508();
s32 Func_02003518();
void Func_0200351c();
void Func_02003538();
s32 Func_02003548();
void Func_02003550();
void Func_0200356c();
s32 Func_0200357c();
void Func_02003580();
void Func_0200359c();
s32 Func_020035ac();
void Func_020035b2();
void Func_020035ce();
s32 Func_020035de();
void Func_020035e2();
void Func_02003616();
void Func_02003646();
void Func_02003678();
void Func_0200365c(void);
void Func_0200363c(s32, s32);
void Func_02003670(void);
s32 Func_02003620();
void Func_02003662();
void Func_0200368a();
void Func_020037ca();

/*
 * Return the table at 0x02009f14.  The eight-byte owner includes its one
 * pool word.
 */

/* Runs a linear setup sequence once two prior gate checks both pass;
 * either gate failing skips the whole sequence. Every step below is an
 * opaque call into other overlay code, so only the gate values and the
 * shared byte pair are named from what this function itself shows. */

/* Gate id checked before the sequence, and passed again to the call that
 * closes it out. */

/* Resolved engine calls: each pseudo symbol is the per-site call word the
 * overlay image holds (a word can serve two sites with different targets),
 * and the macro names the engine function the site reaches through the
 * overlay veneer and the main-image veneer island, keeping the site's own
 * calling form. Names without a repository binding are provisional.
 */

/* Contiguous unnamed state-owner run for resource_37f. */

/* Loader-relocated overlay calls: each symbol names the pre-relocation call
 * word the image holds. */

/* Call sites spelled through these wrappers pass their constants straight
 * into the argument registers; a direct call precomputes a costly constant
 * into a pseudo that the compiler then shares with later uses in the block.
 * A value-returning call also sets r0 last of its arguments. */

/* The scene step counter at 0x1d8 of the shared scene work record. */

/*
 * resource_37f owner at 0x02000f8c, 1,680 bytes: full scene sequence
 * for entities 0, 1, 5 and 8.
 *
 * Complete owner: saved-link prologue at 0x02000f8c through the sole
 * interworking return at 0x02001600, alignment and six-word trailing pool
 * through 0x0200161b.  A branch at 0x020013d0 skips an eleven-word interior
 * constant pool and resumes at 0x02001400.  All 174 BL sites resolve to 30
 * import veneers and are represented below in machine order.
 */

/* Loader-relocated ROM calls: each site names the pre-relocation call word the image holds. */

/* Two-field record read used when copying a found record's s32 pair into a
 * setter call alongside an entity id. */

/* Two-field record read used when copying a found record's s16 pair into a
 * setter call alongside an entity id. */

/*
 * Loader-relocated overlay calls: each symbol names the pre-relocation call
 * word the image holds, not a runtime address.
 */

/*
 * Resource 37f owner at 0x020003bc: 76 bytes of code plus a five-word literal
 * pool at 0x0200040c-0x0200041f.  The interworking return pops the saved
 * return address into r0, so the owner returns void.  All seven calls go
 * through import veneers, and GameFlag_IsSet is used only as a predicate.  r5
 * holds the workspace pointer, loaded before the message call and read after
 * it; that is why the owner saves r5.
 */

/*
 * These wrappers pass their constants straight into the argument registers.
 * A direct call precomputes a costly constant into a pseudo that is then
 * shared with later uses in the block.  A value-returning call also sets r0
 * last of its arguments.
 */
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

/*
 * These wrappers pass their constants straight into the argument registers.
 * A direct call precomputes a costly constant into a pseudo that is then
 * shared with later uses in the block.  A value-returning call also sets r0
 * last of its arguments.
 */
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

static __inline__ void Call2(void (*f)(), s32 a0, s32 a1)
{
    extern u8 Data_03001ebc[];

    f(a0, a1);
}

static __inline__ void Call3_02000258(void (*f)(), s32 a0, s32 a1, s32 a2)
{
    extern u8 Data_03001ebc[];

    f(a0, a1, a2);
}

/* Loader-relocated ROM calls: each site names the pre-relocation call word the image holds. */
static __inline__ void Call0(void (*f)())
{
    f();
}

static __inline__ s32 Value0(s32 (*f)())
{
    return f();
}

static __inline__ s32 Value2(s32 (*f)(), s32 a0, s32 a1)
{
    return f(a0, a1);
}

static __inline__ s32 Value3(s32 (*f)(), s32 a0, s32 a1, s32 a2)
{
    return f(a0, a1, a2);
}

static __inline__ s32 Value6(s32 (*f)(), s32 a0, s32 a1, s32 a2, s32 a3, s32 a4, s32 a5)
{
    return f(a0, a1, a2, a3, a4, a5);
}

/* Call sites spelled through these wrappers pass their constants straight
 * into the argument registers; a direct call precomputes a costly constant
 * into a pseudo that the compiler then shares with later uses in the block.
 * A value-returning call also sets r0 last of its arguments. */
static __inline__ void Call4(void (*f)(), s32 a0, s32 a1, s32 a2, s32 a3)
{
    f(a0, a1, a2, a3);
}

/* Additional resolved engine calls: named directly from each target's own
 * reconstructed C source (see the trailing "provisional" comment on each). */
static __inline__ s32 Value4(s32 (*f)(), s32 a0, s32 a1, s32 a2, s32 a3)
{
    return f(a0, a1, a2, a3);
}

static __inline__ void Scene_Call(void (*func)(s32), s32 arg)
{
    func(arg);
}

/* Call sites spelled through these wrappers pass their constants straight
 * into the argument registers; a direct call precomputes a costly constant
 * into a pseudo that the compiler then shares with later uses in the block.
 * A value-returning call also sets r0 last of its arguments. */
static __inline__ void Call1_0200161c(void (*f)(), s32 a0)
{
    f(a0);
}

static __inline__ s32 Value1_0200161c(s32 (*f)(), s32 a0)
{
    return f(a0);
}

static __inline__ void Call2_0200161c(void (*f)(), s32 a0, s32 a1)
{
    f(a0, a1);
}

static __inline__ void Call3_0200161c(void (*f)(), s32 a0, s32 a1, s32 a2)
{
    f(a0, a1, a2);
}

static __inline__ void Call4_0200161c(void (*f)(), s32 a0, s32 a1, s32 a2, s32 a3)
{
    f(a0, a1, a2, a3);
}

/* Call sites spelled through these wrappers pass their constants straight
 * into the argument registers; a direct call precomputes a costly constant
 * into a pseudo that the compiler then shares with later uses in the block.
 * A value-returning call also sets r0 last of its arguments. */
static __inline__ s32 Value1_020017c0(s32 (*f)(), s32 a0)
{
    return f(a0);
}

static __inline__ void Call6_0200161c(void (*f)(), s32 a0, s32 a1, s32 a2, s32 a3, s32 a4, s32 a5)
{
    f(a0, a1, a2, a3, a4, a5);
}

/* Call sites spelled through these wrappers pass their constants straight
 * into the argument registers; a direct call precomputes a costly constant
 * into a pseudo that the compiler then shares with later uses in the block.
 * A value-returning call also sets r0 last of its arguments. */
static __inline__ s32 Value0_0200161c(s32 (*f)())
{
    return f();
}

/* Runs the full scene sequence for entities 0, 1, 5 and 8: per-entity setup,
 * a shared movement/pose pass, a branch on one entity's saved state, and a
 * closing pass over the same four entities. */
s32 SceneData_SelectOverlayDataBySelector(void)
{
    extern s16 Data_02000240[];

    s16 v = Data_02000240[224];

    if (v == (s32)&Value_00000013) {
        return (s32)Data_02009d04;
    }
    if (v == (s32)&Value_00000010) {
        return (s32)Data_02009d64;
    }
    return (s32)Data_02009cd4;
}

s32 Func_02000070(void)
{
    return 0;
}

u8 *SceneData_GetTable9F14(void)
{
    return (u8 *)0x02009f14;
}

s32 Func_0200007c(void)
{
    extern u8 Data_02000240[];
    extern u8 Data_03001ebc[];

    s32 base5_2009fd8;
    s32 lo = 11;

    if (Data_02000240_t[224][0] == (s32)Data_00000010) {
        if (Data_02000240_t[225][0] >= lo) {
            if (Data_02000240_t[225][0] > 13) {
                if (Data_02000240_t[225][0] > 16) {
                    goto L_020000ae;
                }
                return (s32)Data_0200a1b8;
            }
            return (s32)Data_0200a050;
        }
        L_020000ae:;
        base5_2009fd8 = (s32)Data_02009fd8;
        Func_02001c70(base5_2009fd8);
        return base5_2009fd8;
    } else {
        if (Data_02000240_t[224][0] == (s32)Data_00000013) {
            return (s32)Data_0200a2a8;
        }
    }
    return (s32)Data_02009fc0;
}

/* Call sites spelled through these wrappers pass their constants straight
 * into the argument registers; a direct call precomputes a costly constant
 * into a pseudo that the compiler then shares with later uses in the block.
 * A value-returning call also sets r0 last of its arguments. */
s32 SceneData_SelectFormationTable(void)
{
    s32 lo = 11;

    if (Data_02000240_t[224][0] == (s32)Data_00000013) {
        return (s32)Data_0200a2e4;
    } else {
        if (Data_02000240_t[224][0] == (s32)Data_00000010) {
            if (Data_02000240_t[225][0] >= lo) {
                if (Data_02000240_t[225][0] > 13) {
                    if (Data_02000240_t[225][0] > 16) {
                        goto L_02000128;
                    }
                    return (s32)Data_0200a524;
                }
                return (s32)Data_0200a41c;
            }
            L_02000128:;
            return (s32)Data_0200a32c;
        } else {
        }
    }
    L_0200012e:;
    return (s32)Data_0200a2d8;
}

void FieldScene_RunScene37fSequenceA(void)
{
    extern u8 Data_03001ebc[];

    u32 i;
    s32 record;
    s32 v5;
    s32 v6;

    Func_02001d06();
    v5 = 3;
    v6 = 2;
    Func_02001e2c(181);
    Call6(Func_02001cd0, 16, 28, 21, 3, v5, v6);
    Func_02001c9e(10);
    Call6(Func_02001ce6, 16, 30, 21, 3, v5, v6);
    Func_02001cb4(10);
    Call6(Func_02001cfc, 16, 32, 21, 3, v5, v6);
    Func_02001cca(10);
    Func_02001e12(0, 2);
    Call3(Func_02001d8c, 0, 0x9999, 0x4ccc);
    Func_02001da6(0, 120, 98);
    Func_02001dce(0, 2);
    Call3(Func_02001dc2, 0, 0, -8);
    Func_02001d80(10);
    Func_02001e8c();
    Func_02001e98();
    Func_02001e7e(2);
    Func_02001da2();
}

void SceneDialogue_RunFlag81aMessageBranch(void)
{
    extern u8 *Data_03001ebc;

    Func_02001db0();

    if (Func_02001d96(0x81a) != 0) {
        Func_02001d9a(0x1034, 1);
    } else {
        Func_02001da4(0x1031, 1);
        if (Func_02001db2(0xf01) != 0) {
            u16 *p = (u16 *)(Data_03001ebc + 370);
            u16 val = 1;
            *p = val;
        }
    }

    Func_02001df0();
}

void FieldScene_RunOpeningAuxiliarySequence(void)
{
    extern u8 Data_03001ebc[];

    s32 id;
    s32 v5;
    s32 v6;

    if (Value1(Func_02001df0_a, 0xf01) == 0) {
    } else {
        if (Value1(Func_02001dfc, 0x81a) != 0) {
        } else {
            Func_02001e26();
            Func_02001f3a();
            v5 = 1;
            Func_02001f50(182);
            Call6(Func_02001df2, 0, 70, 30, 42, v5, v5);
            Func_02001dde();
            Func_02001e44(40);
            id = 0x1032;
            Call2(Func_02001e30, id, 1);
            Func_02001e56(20);
            v6 = 3;
            Func_02001f84(183);
            Call6(Func_02001e28, 0, 29, 3, 1, v6, 2);
            Call6(Func_02001e40, 0, 29, 3, 2, v6, v5);
            Call6(Func_02001e48, 1, 109, 4, 81, v5, v5);
            Func_02001e34();
            Call3_02000258(Func_02001e7c, 0x10000, 0x10000, 0x10000);
            Func_02001eaa(20);
            Call3_02000258(Func_02001f7e, 0, 0x100, 0);
            Call3_02000258(Func_02001e9e, 0x20000, 0x20000, 0x10000);
            Func_02001ecc(20);
            Call3_02000258(Func_02001f90, 0, 0x4000, 40);
            Call3_02000258(Func_02001f9c, 0, 0x8000, 20);
            Func_02001fa6(0, 0, 20);
            Call3_02000258(Func_02001fb2, 0, 0x4000, 10);
            Func_02001f74(0, 4, 20);
            Func_02001f7e_a(0, 6, 40);
            Call3_02000258(Func_02001ef4, -1, -1, 0xe666);
            Func_02001f22(40);
            Call2(Func_02001f0e, id + 1, 1);
            Call1(Func_02001f24, 0x143);
            Call1(Func_02001f2a, 0x81a);
            Func_02001f4e();
        }
    }
}

void FieldScene_RunFlag821Dialogue(void)
{
    extern u8 *Data_03001ebc;

    u8 *work;

    Func_02001f6c();

    if (Func_02001f52(0x821) != 0) {
        Func_02001f56(0x1034, 1);
    } else if (Func_02001f66(0xf02) != 0) {
        work = Data_03001ebc;
        Func_02001f6e(0x1031, 1);
        {
            /*
             * The halfword store goes through a pointer local and then an
             * s32 value local, in that order.  Storing the literal straight
             * into the halfword builds the constant in HImode and fetches it
             * from the literal pool, which costs a pool word; splitting the
             * address out first also fixes which register holds it.
             */
            u16 *frame = (u16 *)(work + 370);
            s32 one = 1;
            *frame = (u16)one;
        }
    } else {
        Func_02001f82(0x1031, 1);
    }

    Func_02001fb6();
}

void FieldScene_RunSupplementalSequenceTwo(void)
{
    void Func_02001fb6_early();

    s32 byte_pair_addr;

    if (GameFlag_IsSet_1(3842) == 0) {
    } else {
        if (GameFlag_IsSet_2(GATE_ID)!= 0) {
        } else {
            BattleRuntime_Reset_1();
            Value0(Func_020020fe);
            Audio_PlayCue_1(182);
            Value6(Func_02001fb6_early, 0, 71, 100, 71, 1, 1);
            Value0(Func_02001fa2);
            BattleRuntime_WaitIfModeZero_1(40);
            /* Address of the byte pair starting at Value_00001032. */
            byte_pair_addr = (s32)&Value_00001032;
            UiText_DrawMessage_1(byte_pair_addr, 1);
            BattleRuntime_WaitIfModeZero_2(20);
            Audio_PlayCue_2(183);
            Func_02001fe8(122, 20, 120, 30, 1, 2);
            Call6(Func_02002004, 122, 20, 1, 2, 120, 30);
            Call0(Func_02001fe8_a);
            Value3(Func_02002030, 65536, 65536, 65536);
            BattleRuntime_WaitIfModeZero_3(20);
            BattleEffect_SpawnLinkedResourceObject_1(0, 256, 0);
            Value3(Func_02002052, 131072, 131072, 65536);
            BattleRuntime_WaitIfModeZero_4(20);
            ObjectMotion_ArmCallback_1(0, 16384, 40);
            ObjectMotion_ArmCallback_2(0, 32768, 20);
            ObjectMotion_ArmCallback_3(0, 0, 20);
            ObjectMotion_ArmCallback_4(0, 16384, 10);
            ObjectMotion_Launch_1(0, 4, 20);
            ObjectMotion_Launch_2(0, 6, 40);
            Value3(Func_020020a8, -1, -1, 58982);
            BattleRuntime_WaitIfModeZero_5(40);
            UiText_DrawMessage_2(byte_pair_addr + 1, 1);
            GameFlag_Set_1(0x143);
            GameFlag_Set_2(GATE_ID);
            BattleRuntime_ScheduleShoulderButtonModeUpdate_1();
        }
    }
}

void Scene_UpdateOuterActor9Flags(void)
{
    s32 *work = Func_0200213e(9);
    s32 pos;

    if (work == 0) {
        return;
    }
    pos = work[2] >> 20;
    Scene_Call(Func_0200211c, 0x302);
    Scene_Call(Func_02002122, 0x303);
    if (pos == 93) {
        Scene_Call(Func_02002124, 0x303);
    } else if (pos == 95) {
        Scene_Call(Func_02002130, 0x302);
    }
}

void Scene_UpdateOuterActor10Flags(void)
{
    s32 *work = Func_0200217e(10);
    s32 pos;

    if (work == 0) {
        return;
    }
    pos = work[2] >> 20;
    Scene_Call(Func_0200215e, 0x300);
    Scene_Call(Func_02002164, 0x301);
    if (pos == 115) {
        Scene_Call(Func_02002168, 0x300);
    } else if (pos == 113) {
        Scene_Call(Func_02002174, 0x301);
    }
}

void Scene_UpdateFormationActor9Flags(void)
{
    s32 *work = Func_020021be(9);
    s32 pos;

    if (work == 0) {
        return;
    }
    pos = work[2] >> 20;
    Scene_Call(Func_0200219e, 0x310);
    Scene_Call(Func_020021a4, 0x311);
    if (pos == 99) {
        Scene_Call(Func_020021a6, 0x311);
    } else if (pos == 101) {
        Scene_Call(Func_020021b4, 0x310);
    }
    Scene_Call(Func_02001de6, 0);
}

void Scene_UpdateFormationActor10Flags(void)
{
    s32 *work = Func_02002206(10);
    s32 pos;

    if (work == 0) {
        return;
    }
    pos = work[2] >> 20;
    Scene_Call(Func_020021e4, 0x312);
    Scene_Call(Func_020021ea, 0x313);
    if (pos == 103) {
        Scene_Call(Func_020021ec, 0x313);
    } else if (pos == 105) {
        Scene_Call(Func_020021f8, 0x312);
    }
    Scene_Call(Func_02001e2a, 0);
}

void Scene_UpdateFormationActor11Flags(void)
{
    s32 *work = Func_0200224e(11);
    s32 pos;

    if (work == 0) {
        return;
    }
    pos = work[2] >> 20;
    Scene_Call(Func_0200222e, 0x314);
    Scene_Call(Func_02002234, 0x315);
    if (pos == 107) {
        Scene_Call(Func_02002236, 0x315);
    } else if (pos == 109) {
        Scene_Call(Func_02002244, 0x314);
    }
    Scene_Call(Func_02001e76, 0);
}

void Scene_UpdateFormationActor12Flags(void)
{
    s32 *work = Func_02002296(12);
    s32 pos;

    if (work == 0) {
        return;
    }
    pos = work[2] >> 20;
    Scene_Call(Func_02002274, 0x316);
    Scene_Call(Func_0200227a, 0x317);
    if (pos == 111) {
        Scene_Call(Func_0200227c, 0x317);
    } else if (pos == 113) {
        Scene_Call(Func_02002288, 0x316);
    }
    Scene_Call(Func_02001eba, 0);
}

void Scene_UpdateFormationActor13Flags(void)
{
    s32 *work = Func_020022de(13);
    s32 pos;

    if (work == 0) {
        return;
    }
    pos = work[2] >> 20;
    Scene_Call(Func_020022be, 0x318);
    Scene_Call(Func_020022c4, 0x319);
    if (pos == 115) {
        Scene_Call(Func_020022c6, 0x319);
    } else if (pos == 117) {
        Scene_Call(Func_020022d4, 0x318);
    }
    Scene_Call(Func_02001f06, 0);
}

void Scene_UpdateFormationActor14Flags(void)
{
    s32 *work = Func_02002326(14);
    s32 pos;

    if (work == 0) return;
    pos = work[2] >> 20;
    Scene_Call(Func_02002304, 0x31a);
    Scene_Call(Func_0200230a, 0x31b);
    if (pos == 119) {
        Scene_Call(Func_0200230c, 0x31b);
    } else if (pos == 121) {
        Scene_Call(Func_02002318, 0x31a);
    }
    Scene_Call(Func_02001f4a, 0);
}

s32 *SceneActor_FindSlotByTilePosition(s32 x, s32 z)
{
    extern u8 *Data_03001ebc;

    s32 **slots = (s32 **)(Data_03001ebc + 0x14);
    u32 i;

    for (i = 8; i <= 65; i++) {
        s32 *p = slots[i];

        if (x == (p[2] >> 20) && z == (p[4] >> 20)) {
            return p;
        }
    }
    return 0;
}

s32 FieldScene_DispatchByScenarioId(void)
{
    extern s16 Data_02000240[];

    s32 scenario = Data_02000240[224];

    if (scenario == (s32)&Value_00000013) {
        Func_02001236();
    } else if (scenario == (s32)&Value_00000010) {
        Func_0200133a();
    }
    return 0;
}

void FieldScene_RunScene37f_0200092c(void)
{
    extern u8 Data_03001ebc[];

    Call1(Func_020024ca, 0x144);
    *(s32 *)((*(u8 *volatile *)Data_03001ebc + 0x1c0)) = 0x100;
    if (Value1(Func_020024d6, 0x814) != 0) {
        s32 zero = 0;
        *(volatile s32 *)Data_0200a69c = zero;
        Call2(Func_0200248a, 0x2009ac9, 0xc80);
    }
    if (Value1(Func_020024f0, 0x879) != 0) {
        Call6(Func_020024de, 5, 6, 1, 1, 6, 6);
        Call6(Func_020024f0_a, 5, 6, 1, 1, 7, 6);
        Call6(Func_02002502, 5, 6, 1, 1, 8, 6);
        Call6(Func_02002514, 0, 1, 3, 1, 6, 5);
    }
    if (Value1(Func_02002542, 0x815) != 0) {
        Call3(Func_020025c4, 8, 0x780000, 0xe80000);
        Call6(Func_02002540, 2, 10, 1, 1, 6, 14);
        Call6(Func_02002552, 2, 10, 1, 1, 7, 14);
        Call6(Func_02002564, 2, 10, 1, 1, 8, 14);
    }
}

void FieldScene_RunScene37f_02000d1c(void)
{
    extern u8 Data_03001ebc[];

    u32 i;
    s32 record;

    Func_020028cc();
    *(s32 *)((*(u8 *volatile *)Data_03001ebc + 0x1c0)) = 0x100;
    Func_020029d6();
    ((void (*)())Func_0200293e)(0, 0);
    Func_020028e4(4);
    Call4(Func_020029d6_a, -1, -1, -1, 0);
    Call2(Func_020029d6_b, 0x9999, 0x1333);
    Call4(Func_020029f0, 0x4c80000, -1, 0x880000, 1);
    record = Value1(Func_0200293e_a, 0);
    if (record != 0) {
        Func_0200297c(8, *(volatile s32 *)(record + 8), *(volatile s32 *)(record + 16));
    }
    record = Value1(Func_02002952, 0);
    if (record != 0) {
        Func_02002990(5, *(volatile s32 *)(record + 8), *(volatile s32 *)(record + 16));
    }
    record = Value1(Func_02002966, 0);
    if (record != 0) {
        Func_020029a4(1, *(volatile s32 *)(record + 8), *(volatile s32 *)(record + 16));
    }
    Call3(Func_02002986, 8, 0x9999, 0x4ccc);
    Call3(Func_02002990_a, 5, 0x9999, 0x4ccc);
    Call3(Func_0200299a, 1, 0x9999, 0x4ccc);
    Func_020029d2(1, 2);
    Func_020029da(5, 2);
    Func_020029e2(8, 2);
    Call3(Func_020029d6_c, 1, -16, 0);
    Func_020029e0(5, 16, 0);
    Call3(Func_020029ec, 8, 0, -32);
    Func_020029fa(1);
    Func_02002a12(1, 0);
    Func_02002a1a(5, 0);
    Call3(Func_02002a7e, 1, 0xc000, 0);
    Call3(Func_02002a8a, 5, 0xc000, 0);
    Func_02002a28(8);
    Func_02002a40(8, 1);
    Func_020029e6(40);
    Func_02002a6e(8, 2);
    Func_020029f4(20);
    Call3(Func_02002ab8, 8, 0x3000, 40);
    Call3(Func_02002ac4, 8, 0x5000, 40);
    Call3(Func_02002ad0, 8, 0x3000, 20);
    Func_02002a92(8, 4, 20);
    Call1(Func_02002ab8_a, 0xfd3);
    Call2(Func_02002ae0, 0x4008, 0);
    Func_02002a36(20);
    Call4(Func_02002b28, 0x4c80000, -1, 0x940000, 1);
    Func_02002ab0(1, 2);
    record = Value1(Func_02002a7e_a, 0);
    if (record != 0) {
        Func_02002aa0(1, *(s16 *)(record + 10), *(s16 *)(record + 18));
    }
    Func_02002ad0_a(5, 2);
    record = Value1(Func_02002a9e, 0);
    if (record != 0) {
        Func_02002ac0(5, *(s16 *)(record + 10), *(s16 *)(record + 18));
    }
    Func_02002af0(8, 2);
    record = Value1(Func_02002abe, 0);
    if (record != 0) {
        Func_02002ae0_a(8, *(s16 *)(record + 10), *(s16 *)(record + 18));
    }
    Func_02002afe(1);
    Func_02002b10(1, 0, 0);
    Func_02002b1a(5, 0, 0);
    Func_02002b18(8);
    Func_02002b2a(8, 0, 0);
    Func_02002b3a(1, 1);
    Func_02002b42(5, 1);
    Func_02002b4a(8, 1);
    Call1(Func_02002ae0_b, 0x802);
    *(s32 *)((*(u8 *volatile *)Data_03001ebc + 0x1c0)) = 0x204;
    Call1(Func_02002afa, 0x12f);
    Func_02002b16();
}

void FieldScene_RunFourEntitySequence(void)
{
    u8 *record;

    BattleRuntime_Reset_1_late();
    BattleRuntime_WaitIfModeZero_1_late(); /* main:0808a360 */
    ObjectMotion_SetSpeedParameters_1(); /* main:0808a370 */

    record = Scene_GetRecord_1(0);
    if (record != 0)
        ObjectMotion_SetHorizontalPositionWithTerrain_1(8, RECORD_A32(record), RECORD_B32(record));
    record = Scene_GetRecord_2(0);
    if (record != 0)
        ObjectMotion_SetHorizontalPositionWithTerrain_2(5, RECORD_A32(record), RECORD_B32(record));
    record = Scene_GetRecord_3(0);
    if (record != 0)
        ObjectMotion_SetHorizontalPositionWithTerrain_3(1, RECORD_A32(record), RECORD_B32(record));

    ObjectMotion_SetSpeedParameters_2(8, 0x9999, 0x4ccc);
    ObjectMotion_SetSpeedParameters_3(5, 0x9999, 0x4ccc);
    ObjectMotion_SetSpeedParameters_4(1, 0x9999, 0x4ccc);
    Object_SetModeById_1(1, 2);
    Object_SetModeById_2(5, 2);
    Object_SetModeById_3(8, 2);
    ObjectMotion_OffsetPositionAndResetMotion_1(1, -16, 0);
    ObjectMotion_OffsetPositionAndResetMotion_2(5, 16, 0);
    ObjectMotion_OffsetPositionAndResetMotion_3(8, 0, -16);
    ObjectMotion_CommitCurrentPositionAndActivate_1(8);
    Object_SetModeById_4(8, 1);
    Object_SetModeById_5(0, 0);
    Object_SetModeById_6(1, 0);
    Object_SetModeById_7(5, 0);
    ObjectMotion_ArmCallback_1_late(1, 0xe000, 0);
    ObjectMotion_ArmCallback_2_late(5, 0xa000, 0);
    ObjectMotion_ArmCallback_3_late(8, 0xc000, 30);
    ObjectMotion_ArmCallback_4_late(1, 0x8000, 0);
    ObjectMotion_ArmCallback_5(5, 0, 0);
    ObjectMotion_ArmCallback_6(0, 0x4000, 0);
    ObjectMotion_ArmCallback_7(8, 0x8000, 30);
    ObjectMotion_ArmCallback_8(1, 0x4000, 0);
    ObjectMotion_ArmCallback_9(5, 0x8000, 0);
    ObjectMotion_ArmCallback_10(0, 0, 0);
    ObjectMotion_ArmCallback_11(8, 0x4000, 30);
    ObjectMotion_ArmCallback_12(1, 0xe000, 0);
    ObjectMotion_ArmCallback_13(5, 0xa000, 0);
    ObjectMotion_ArmCallback_14(0, 0xc000, 0);
    ObjectMotion_ArmCallback_15(8, 0xc000, 40);
    ObjectMotion_SetVariantCallbackAndInvokeObject_1(8, 2);
    BattleRuntime_WaitIfModeZero_2_late(10);
    Object_SetModeById_8(8, 2);
    ObjectMotion_OffsetPositionAndResetMotion_4(8, 0, -16);
    ObjectMotion_CommitCurrentPositionAndActivate_2(8);
    Object_SetModeById_9(8, 1);
    BattleRuntime_WaitIfModeZero_3_late(6);
    ObjectMotion_ArmCallback_16(8, 0x8000, 20);
    ObjectMotion_ArmCallback_17(8, 0, 20);
    ObjectMotion_ArmCallback_18(8, 0xc000, 40);
    ObjectMotion_SetVariantCallbackAndInvokeObject_2(8, 2);
    BattleRuntime_WaitIfModeZero_4_late(20);
    Object_SetModeById_10(8, 2);
    ObjectMotion_OffsetPositionAndResetMotion_5(8, 0, -32);
    ObjectMotion_CommitCurrentPositionAndActivate_3(8);
    Object_SetModeById_11(8, 1);

    ObjectMotion_SetSpeedLimitAndAcceleration_1(0x20000, 0x4000);
    ObjectMotion_PlaceWithinCameraBounds_1(0x06310000, -1, 0x00960000, 1);
    Object_CommitPositionThenWaitIfModeZero_1();
    BattleRuntime_WaitIfModeZero_5_late(10);
    ObjectMotion_SetSpeedLimitAndAcceleration_2(0x13333, 0x2666);
    ObjectMotion_PlaceWithinCameraBounds_2(0x06550000, -1, 0x00640000, 1);
    Object_CommitPositionThenWaitIfModeZero_2();
    ObjectMotion_PlaceWithinCameraBounds_3(0x06b60000, -1, 0x00640000, 1);
    Object_CommitPositionThenWaitIfModeZero_3();
    Object_SetModeById_12(8, 1);
    ObjectMotion_PlaceWithinCameraBounds_4(0x06d80000, -1, 0x00960000, 1);
    Object_CommitPositionThenWaitIfModeZero_4();
    BattleRuntime_WaitIfModeZero_6(40);
    ObjectMotion_SetSpeedLimitAndAcceleration_3(0x26666, 0x4ccc);
    ObjectMotion_PlaceWithinCameraBounds_5(0x06840000, -1, 0x01000000, 1);
    Object_CommitPositionThenWaitIfModeZero_5();
    ObjectMotion_CallThenWaitForAnimationChange_1(8, 3);
    BattleRuntime_WaitIfModeZero_7(10);

    ObjectMotion_ArmCallback_19(1, 0xe000, 0);
    ObjectMotion_ArmCallback_20(5, 0xa000, 10);
    BattleEffect_SpawnLinkedResourceObject_1_late(1, 0x101, 20);
    SceneWork_SetStepValue_1(0xfd6);
    BattleRuntime_RunThenWaitIfModeZero_1(1, 0, 10);
    BattleEffect_SpawnLinkedResourceObject_2(8, 0x102, 60);
    ObjectMotion_SetVariantCallback_1(8, 2);
    BattleRuntime_RunThenWaitIfModeZero_2(8, 0, 10);
    ObjectMotion_SetVariantCallback_2(0, 2);
    ObjectMotion_SetVariantCallback_3(1, 2);
    ObjectMotion_SetVariantCallback_4(5, 2);
    BattleRuntime_WaitIfModeZero_8(0, 0x102);
    BattleRuntime_WaitIfModeZero_9(1, 0x102); /* main:0808a1f0 */
    BattleRuntime_WaitIfModeZero_10(5, 0x102); /* main:0808a1f0 */
    BattleRuntime_WaitIfModeZero_11(40);
    ObjectMotion_SetVariantCallbackAndInvokeObject_3(8, 2);
    BattleRuntime_WaitIfModeZero_12(20);
    BattleEvent_RunActionAndWait_1(8, 0);
    ObjectMotion_CallThenWaitForAnimationChange_2(8, 4);
    BattleRuntime_RunThenWaitIfModeZero_3(8, 0, 10);
    Object_LinkPair_1(0, 5, 0);
    BattleRuntime_WaitIfModeZero_13(40);
    ObjectMotion_SetVariantCallback_5(0, 1);
    ObjectMotion_SetVariantCallbackAndInvokeObject_4(5, 1);
    BattleRuntime_WaitIfModeZero_14(10);
    ObjectMotion_ArmCallback_21(0, 0xc000, 0);
    ObjectMotion_ArmCallback_22(5, 0xa000, 20);
    ObjectMotion_SetVariantCallbackAndInvokeObject_5(5, 2);
    BattleRuntime_RunThenWaitIfModeZero_4(5, 0, 10);
    ObjectMotion_CallThenWaitForAnimationChange_3(8, 4);
    BattleRuntime_RunThenWaitIfModeZero_5(8, 0, 10);
    ObjectMotion_ArmCallback_23(8, 0x3000, 10);
    BattleRuntime_RunThenWaitIfModeZero_6(8, 0, 40);
    ObjectMotion_ArmCallback_24(8, 0xc000, 20);
    ObjectMotion_SetVariantCallbackAndInvokeObject_6(8, 1);
    BattleRuntime_WaitIfModeZero_15(10);
    BattleRuntime_WaitIfModeZero_16(8, 0x102); /* main:0808a1f0 */
    ObjectMotion_ArmCallback_25(8, 0x8000, 20);
    ObjectMotion_ArmCallback_26(8, 0, 20);
    ObjectMotion_ArmCallback_27(8, 0xc000, 60);
    ObjectMotion_SetVariantCallbackAndInvokeObject_7(8, 2);
    BattleRuntime_WaitIfModeZero_17(10);
    BattleRuntime_RunThenWaitIfModeZero_7(8, 0, 10);
    ObjectMotion_ArmCallback_28(8, 0x4000, 20);
    ObjectMotion_Launch_1_late(8, 2, 20);

    BattleRuntime_RunThenWaitIfModeZero_8(8, 0, 40);
    ObjectMotion_ArmCallback_29(1, 0, 20);
    ObjectMotion_SetSpeedParameters_5(1, 0); /* main:0808a178 */
    if (UiWork_WaitThenFinalizeCapacity_1(0, 0) == 0) {
        /* Passes the address of Value_00000fe0 in place of a record pointer. */
        SceneWork_SetStepValue_2((s32)&Value_00000fe0);
        ObjectMotion_SetVariantCallbackAndInvokeObject_8(1, 1);
        BattleRuntime_RunThenWaitIfModeZero_9(1, 0, 10);
    } else {
        SceneWork_SetStepValue_3(0xfe1);
        ObjectMotion_ArmCallback_30(5, 0x8000, 20);
        BattleRuntime_RunThenWaitIfModeZero_10(5, 0, 10);
        ObjectMotion_ArmCallback_31(0, 0xc000, 0);
        ObjectMotion_ArmCallback_32(1, 0xe000, 0);
        ObjectMotion_ArmCallback_33(5, 0xa000, 60);
        BattleEffect_SpawnLinkedResourceObject_3(0, 0x102, 40);
        ObjectMotion_SetVariantCallbackAndInvokeObject_9(1, 1);
        BattleRuntime_WaitIfModeZero_18(10);
        ObjectMotion_CallThenWaitForAnimationChange_4(1, 3);
        BattleRuntime_WaitIfModeZero_19(10);
        ObjectMotion_ArmCallback_34(1, 0, 0);
        ObjectMotion_ArmCallback_35(5, 0x8000, 30);
        ObjectMotion_SetVariantCallbackAndInvokeObject_10(1, 1);
        BattleRuntime_WaitIfModeZero_20(10);
        BattleRuntime_RunThenWaitIfModeZero_11(1, 0, 10);
    }

    ObjectMotion_SetSpeedParameters_6(8, 0x9999, 0x4ccc);
    Object_SetModeById_13(8, 2);
    ObjectMotion_OffsetPositionAndResetMotion_6(8, 0, 48);
    ObjectMotion_CommitCurrentPositionAndActivate_4(8);
    Object_SetModeById_14(8, 1);
    BattleRuntime_WaitIfModeZero_21(6);
    ObjectMotion_ArmCallback_36(1, 0xe000, 0);
    ObjectMotion_ArmCallback_37(5, 0xa000, 0);
    ObjectMotion_CommitCurrentPositionAndActivate_5(8);
    Object_SetModeById_15(8, 1);
    BattleRuntime_WaitIfModeZero_22(20);
    Object_SetModeById_16(1, 3);
    Object_SetModeById_17(5, 3);
    ObjectMotion_CallThenWaitForAnimationChange_5(0, 3);
    BattleRuntime_WaitIfModeZero_23(6);
    Object_SetModeById_18(1, 2);

    record = Scene_GetRecord_4(0);
    if (record != 0)
        ObjectMotion_ResetAndSetPosition_1(1, RECORD_A16(record), RECORD_B16(record));
    Object_SetModeById_19(5, 2);
    record = Scene_GetRecord_5(0);
    if (record != 0)
        ObjectMotion_ResetAndSetPosition_2(5, RECORD_A16(record), RECORD_B16(record));
    Object_SetModeById_20(8, 2);
    record = Scene_GetRecord_6(0);
    if (record != 0)
        ObjectMotion_ResetAndSetPosition_3(8, RECORD_A16(record), RECORD_B16(record));

    ObjectMotion_CommitCurrentPositionAndActivate_6(8);
    ObjectMotion_SetHorizontalPositionWithTerrain_4(1, 0, 0);
    ObjectMotion_SetHorizontalPositionWithTerrain_5(5, 0, 0);
    ObjectMotion_SetHorizontalPositionWithTerrain_6(8, 0, 0);
    Object_SetModeById_21(8, 1);
    Object_SetModeById_22(1, 1);
    Object_SetModeById_23(5, 1);
    GameFlag_Set_1_late(0x804);
    GameFlag_Clear_1(0x12f);
    BattleRuntime_ScheduleShoulderButtonModeUpdate_1_late();
}

void Scene_RunTransitionCue(void)
{
    u32 i;
    s32 record;

    Func_020031cc();
    *(s32 *)((*(s32 *)0x03001ebc + 0x1c0)) = 0x204;
    Func_020032d6();
    Func_020032ea();
    Func_020031e0_a(20);
    record = Value1_0200161c(Func_0200320e, 0);
    if (record != 0) {
        Func_0200324c(8, *(s32 *)(record + 8), *(s32 *)(record + 16));
    }
    Call3_0200161c(Func_02003232, 8, 0x10000, 0x8000);
    Func_0200326a(8, 2);
    Call3_0200161c(Func_0200325e, 8, 24, -10);
    Func_0200326c(8);
    Func_02003284(8, 1);
    Func_0200322a(6);
    Call3_0200161c(Func_020032ee, 8, 0xb000, 0);
    Call3_0200161c(Func_020032fa, 0, 0xc000, 40);
    Call2_0200161c(Func_02003322, 0x26666, 0x4ccc);
    Call4_0200161c(Func_0200333c, 0x6880000, -1, 0x20c0000, 1);
    Func_02003348();
    Func_02003266(20);
    Call2_0200161c(Func_02003346, 0x19999, 0x3333);
    Call4_0200161c(Func_02003360, 0x7580000, -1, 0x20c0000, 1);
    Func_0200336c();
    Func_0200328a(20);
    Call2_0200161c(Func_0200336a, 0x33333, 0x6666);
    Call4_0200161c(Func_02003382, 0x6e90000, -1, 0x2240000, 1);
    Func_0200338e();
    Func_020032ac(20);
    Func_02003334(8, 2);
    Func_02003376(8, 0, 30);
    Call1_0200161c(Func_02003354, 0x103a);
    Call3_0200161c(Func_02003376_a, 0x4008, 0, 10);
    Call3_0200161c(Func_020033a2, 8, 0x100, 40);
    Func_02003362(8, 1);
    Call3_0200161c(Func_020033a6, 8, 0x5000, 20);
    Call3_0200161c(Func_020033a0, 0x4008, 0, 10);
    Func_02003360_a(8, 2);
    record = Value1_0200161c(Func_0200332e, 0);
    if (record != 0) {
        Func_02003350(8, *(s16 *)(record + 10), *(s16 *)(record + 18));
    }
    Func_0200336e(8);
    Func_02003380(8, 0, 0);
    Call1_0200161c(Func_0200331e, 0x825);
    Func_02003342();
}

void Scene_RunActorFormation(s32 a0)
{
    u32 i;
    s32 record;

    Value6(Func_0200333c_a, 122, 20, 1, 1, 100, 32);
    Call6_0200161c(Func_0200334e, 122, 20, 1, 1, 104, 32);
    Call6_0200161c(Func_02003360_b, 122, 20, 1, 1, 108, 32);
    Call6_0200161c(Func_02003372, 122, 20, 1, 1, 112, 32);
    Call6_0200161c(Func_02003384, 122, 20, 1, 1, 116, 32);
    Call6_0200161c(Func_02003396, 122, 20, 1, 1, 120, 32);
    if (Value1_020017c0(Func_020033c4, 0x311) != 0) {
        Call6_0200161c(Func_020033b0, 121, 20, 1, 1, 100, 32);
        if (a0 == 0) {
            goto L_02001890;
        }
        Call3_0200161c(Func_0200345a, 9, 0x6380000, 0x2080000);
    } else {
        if (Value1_020017c0(Func_020033f4, 0x310) != 0) {
            Call6_0200161c(Func_020033e0, 121, 20, 1, 1, 100, 32);
            if (a0 != 0) {
                Call3_0200161c(Func_0200348a, 9, 0x6580000, 0x2080000);
            }
        }
    }
    L_02001890:;
    if (Value1_020017c0(Func_02003420, 0x313) != 0) {
        Call6_0200161c(Func_02003410, 121, 20, 1, 1, 104, 32);
        if (a0 == 0) {
            goto L_020018f2;
        }
        Call3_0200161c(Func_020034ba, 10, 0x6780000, 0x2080000);
    } else {
        if (Value1_020017c0(Func_02003452, 0x312) != 0) {
            Call6_0200161c(Func_02003442, 121, 20, 1, 1, 104, 32);
            if (a0 != 0) {
                Call3_0200161c(Func_020034ec, 10, 0x6980000, 0x2080000);
            }
        }
    }
    L_020018f2:;
    if (Value1_020017c0(Func_02003482, 0x315) != 0) {
        Call6_0200161c(Func_02003472, 121, 20, 1, 1, 108, 32);
        if (a0 == 0) {
            goto L_02001956;
        }
        Call3_0200161c(Func_0200351c, 11, 0x6b80000, 0x2080000);
    } else {
        if (Value1_020017c0(Func_020034b6, 0x314) != 0) {
            Call6_0200161c(Func_020034a6, 121, 20, 1, 1, 108, 32);
            if (a0 != 0) {
                Call3_0200161c(Func_02003550, 11, 0x6d80000, 0x2080000);
            }
        }
    }
    L_02001956:;
    if (Value1_020017c0(Func_020034e6, 0x317) != 0) {
        Call6_0200161c(Func_020034d6, 121, 20, 1, 1, 112, 32);
        if (a0 == 0) {
            goto L_020019b8;
        }
        Call3_0200161c(Func_02003580, 12, 0x6f80000, 0x2080000);
    } else {
        if (Value1_020017c0(Func_02003518, 0x316) != 0) {
            Call6_0200161c(Func_02003508, 121, 20, 1, 1, 112, 32);
            if (a0 != 0) {
                Call3_0200161c(Func_020035b2, 12, 0x7180000, 0x2080000);
            }
        }
    }
    L_020019b8:;
    if (Value1_020017c0(Func_02003548, 0x319) != 0) {
        Call6_0200161c(Func_02003538, 121, 20, 1, 1, 116, 32);
        if (a0 == 0) {
            goto L_02001a1c;
        }
        Call3_0200161c(Func_020035e2, 13, 0x7380000, 0x2080000);
    } else {
        if (Value1_020017c0(Func_0200357c, 0x318) != 0) {
            Call6_0200161c(Func_0200356c, 121, 20, 1, 1, 116, 32);
            if (a0 != 0) {
                Call3_0200161c(Func_02003616, 13, 0x7580000, 0x2080000);
            }
        }
    }
    L_02001a1c:;
    if (Value1_020017c0(Func_020035ac, 0x31b) != 0) {
        Call6_0200161c(Func_0200359c, 121, 20, 1, 1, 120, 32);
        if (a0 == 0) {
            goto L_02001a7e;
        }
        Call3_0200161c(Func_02003646, 14, 0x7780000, 0x2080000);
    } else {
        if (Value1_020017c0(Func_020035de, 0x31a) != 0) {
            Call6_0200161c(Func_020035ce, 121, 20, 1, 1, 120, 32);
            if (a0 != 0) {
                Call3_0200161c(Func_02003678, 14, 0x7980000, 0x2080000);
            }
        }
    }
    L_02001a7e:;
}

void FieldScene_RunScriptedStep953(void)
{
    Func_0200365c();
    Func_0200363c(0x953, 1);
    Func_02003670();
}

void Scene_UpdateCueTimer(s32 a0, s32 a1, s32 a2)
{
    u32 i;
    s32 record;
    s32 value;
    s32 base5_200a69c;
    s32 v3;

    base5_200a69c = 0x200a69c;
    if (*(s32 *)base5_200a69c != 0) {
        v3 = (*(s32 *)base5_200a69c - 1);
        *(s32 *)base5_200a69c = (*(s32 *)base5_200a69c - 1);
        if (v3 != 40) {
            goto L_02001b14;
        }
        Call3_0200161c(Func_02003662, -1, -1, 0xe666);
    } else {
        value = Value0_0200161c(Func_02003620);
        if (((u32)(((value << 4) - value) << 3) >> 16) == 0) {
            Func_020037ca(138);
            Call3_0200161c(Func_0200368a, 0x10000, 0x20000, 0x10000);
            *(s32 *)base5_200a69c = 80;
        }
    }
    L_02001b14:;
}
