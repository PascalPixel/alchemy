#include "TYPES.H"
#include "FIELD_EVENT.H"
#include "ITEM_IDS.H"

#define GameFlag_IsSet_1(a0) Value1(Func_02007dc6, a0)
#define GameFlag_Set_1(a0) Value1(Engine_GameFlagSet, a0)
#define BattleRuntime_Reset_1(args...) Engine_EventBegin(args)
#define GameFlag_IsSet_2(a0) Value1(Func_02007de0, a0)
#define ObjectMotion_SetSpeedParameters_1(a0, a1, a2) Call3(Engine_ActorSetSpeed, a0, a1, a2)
#define ObjectMotion_SetPositionAndReset_1(a0, a1, a2) Call3(Func_02007e78, a0, a1, a2)
#define ObjectMotion_SetAngleToward_1(args...) Engine_ActorFaceActor(args)
#define BattleEffect_SpawnLinkedResourceObject_1(a0, a1, a2) Call3(Engine_ActorShowEmote, a0, a1, a2)
#define ObjectMotion_ArmCallback_1(a0, a1, a2) Call3(Func_02007f1e, a0, a1, a2)
#define ObjectMotion_Launch_1(args...) Func_02007ed8(args)
#define Scene_GetRecord_1(args...) Func_02007e76(args)
#define BattleRuntime_WaitIfModeZero_1(args...) Engine_EventWait(args)
#define ObjectMotion_SetSpeedParameters_2(a0, a1, a2) Call3(Engine_ActorSetSpeed, a0, a1, a2)
#define ObjectMotion_ResetAndSetPositionInMode2_1(a0, a1, a2) Value3(Engine_ActorWalkTo, a0, a1, a2)
#define ObjectMotion_CommitCurrentPositionAndActivate_1(args...) Engine_ActorWaitForMove(args)
#define BattleRuntime_WaitIfModeZero_2(args...) Engine_EventWait(args)
#define ObjectMotion_ArmCallback_2(a0, a1, a2) Call3(Func_02007f68, a0, a1, a2)
#define ObjectMotion_ArmCallback_3(a0, a1, a2) Call3(Func_02007f74, a0, a1, a2)
#define ObjectMotion_ArmCallback_4(a0, a1, a2) Call3(Func_02007f80, a0, a1, a2)
#define ObjectMotion_ArmCallback_5(a0, a1, a2) Call3(Func_02007f8c, a0, a1, a2)
#define ObjectMotion_SetHorizontalPositionWithTerrain_1(a0, a1, a2) Call3(Engine_ActorSetPosition, a0, a1, a2)
#define ObjectMotion_SetSpeedLimitAndAcceleration_1(a0, a1) Value2(Engine_CameraSetSpeed, a0, a1)
#define SceneWork_SetStepValue_1(args...) Engine_EventSetMessage(args)
#define BattleEvent_RunActionAndWait_1(args...) Engine_EventShowMessage(args)
#define ObjectMotion_ResetAndSetPositionInMode2_2(a0, a1, a2) Value3(Engine_ActorWalkTo, a0, a1, a2)
#define ObjectMotion_CommitCurrentPositionAndActivate_2(args...) Engine_ActorWaitForMove(args)
#define ObjectMotion_ArmCallback_6(a0, a1, a2) Value3(Func_02007fd6, a0, a1, a2)
#define BattleRuntime_WaitIfModeZero_3(args...) Engine_EventWait(args)
#define ObjectMotion_OffsetPositionAndResetMotion_1(a0, a1, a2) Value3(Engine_ActorSetDestinationOffset, a0, a1, a2)
#define ObjectMotion_CommitCurrentPositionAndActivate_3(args...) Engine_ActorWaitForMove(args)
#define BattleRuntime_WaitIfModeZero_4(args...) Engine_EventWait(args)
#define Audio_PlayCue_1(args...) Engine_AudioPlayCue(args)
#define UiText_DrawMessage_1(args...) Engine_MessageShowCentered(args)
#define ObjectMotion_OffsetPositionAndResetMotion_2(a0, a1, a2) Value3(Engine_ActorSetDestinationOffset, a0, a1, a2)
#define BattleRuntime_WaitIfModeZero_5(args...) Engine_EventWait(args)
#define ObjectMotion_SetVariantCallback_1(args...) Engine_ActorStartRepeatedMotion(args)
#define ObjectMotion_SetVariantCallback_2(args...) Engine_ActorStartRepeatedMotion(args)
#define ObjectMotion_SetVariantCallback_3(args...) Engine_ActorStartRepeatedMotion(args)
#define ObjectMotion_SetVariantCallback_4(args...) Engine_ActorStartRepeatedMotion(args)
#define BattleRuntime_WaitIfModeZero_6(args...) Engine_EventWait(args)
#define ObjectMotion_SetAngleToward_2(args...) Engine_ActorFaceActor(args)
#define SceneWork_SetStepValue_2(args...) Engine_EventSetMessage(args)
#define BattleEvent_RunActionAndWait_2(args...) Engine_EventShowMessage(args)
#define BattleRuntime_WaitIfModeZero_7(args...) Engine_EventWait(args)
#define SharedWorkData_SetFirstAndSecondFields_1(args...) Func_020080e6_a(args)
#define BattleEffect_ComputeWeightedResultAndDispatch_1(args...) Func_020080e6_b(args)
#define Object_SetModeById_1(args...) Engine_ActorSetAnimation(args)
#define Audio_PlayCue_2(args...) Engine_AudioPlayCue(args)
#define BattleRuntime_WaitIfModeZero_8(args...) Engine_EventWait(args)
#define SceneWork_SetStepValue_3(args...) Engine_EventSetMessage(args)
#define BattleEvent_RunActionAndWait_3(args...) Engine_EventShowMessage(args)
#define Object_LinkPair_1(args...) Engine_ActorFaceEachOther(args)
#define BattleRuntime_WaitIfModeZero_9(args...) Engine_EventWait(args)
#define ObjectMotion_ArmCallback_7(a0, a1, a2) Call3(Func_020080f6, a0, a1, a2)
#define ObjectMotion_Launch_2(args...) Func_020080b0(args)
#define Scene_GetRecord_2(args...) Func_0200804e(args)
#define SceneWork_SetStepValue_4(args...) Engine_EventSetMessage(args)
#define BattleEvent_RunActionAndWait_4(args...) Engine_EventShowMessage(args)
#define ObjectMotion_SetSpeedParameters_3(a0, a1, a2) Call3(Engine_ActorSetSpeed, a0, a1, a2)
#define ObjectMotion_SetPositionAndReset_2(a0, a1, a2) Call3(Func_020080b4_b, a0, a1, a2)
#define ObjectMotion_SetAngleToward_3(args...) Engine_ActorFaceActor(args)
#define ObjectMotion_SetSpeedParameters_4(a0, a1, a2) Call3(Engine_ActorSetSpeed, a0, a1, a2)
#define ObjectMotion_ResetAndSetPositionInMode2_3(a0, a1, a2) Value3(Engine_ActorWalkTo, a0, a1, a2)
#define ObjectMotion_CommitCurrentPositionAndActivate_4(args...) Engine_ActorWaitForMove(args)
#define BattleRuntime_WaitIfModeZero_10(args...) Engine_EventWait(args)
#define SceneWork_SetStepValue_5(args...) Engine_EventSetMessage(args)
#define BattleEvent_RunActionAndWait_5(args...) Engine_EventShowMessage(args)
#define BattleEffect_SpawnLinkedResourceObject_2(a0, a1, a2) Value3(Engine_ActorShowEmote, a0, a1, a2)
#define BattleRuntime_WaitIfModeZero_11(args...) Engine_EventWait(args)
#define Audio_PlayCue_3(args...) Engine_AudioPlayCue(args)
#define SceneWork_SetStepValue_6(args...) Engine_EventSetMessage(args)
#define BattleEvent_RunActionAndWait_6(args...) Engine_EventShowMessage(args)
#define BattleRuntime_WaitIfModeZero_12(args...) Engine_EventWait(args)
#define ObjectMotion_CallThenWaitForAnimationChange_1(args...) Func_0200814e(args)
#define BattleRuntime_WaitIfModeZero_13(args...) Engine_EventWait(args)
#define Object_SetModeById_2(args...) Engine_ActorSetAnimation(args)
#define ObjectMotion_SetVariantCallback_5(args...) Engine_ActorStartRepeatedMotion(args)
#define BattleRuntime_WaitIfModeZero_14(args...) Engine_EventWait(args)
#define ObjectMotion_SetSpeedParameters_5(a0, a1, a2) Call3(Engine_ActorSetSpeed, a0, a1, a2)
#define ObjectMotion_ResetAndSetPositionInMode2_4(a0, a1, a2) Value3(Engine_ActorWalkTo, a0, a1, a2)
#define ObjectMotion_CommitCurrentPositionAndActivate_5(args...) Engine_ActorWaitForMove(args)
#define Object_SetModeById_3(args...) Engine_ActorSetAnimation(args)
#define BattleRuntime_WaitIfModeZero_15(args...) Engine_EventWait(args)
#define ObjectMotion_ArmCallback_8(a0, a1, a2) Value3(Func_020081f8, a0, a1, a2)
#define BattleRuntime_WaitIfModeZero_16(args...) Engine_EventWait(args)
#define ObjectMotion_ArmCallback_9(a0, a1, a2) Value3(Func_0200820a, a0, a1, a2)
#define BattleRuntime_WaitIfModeZero_17(args...) Engine_EventWait(args)
#define ObjectMotion_SetAngleToward_4(args...) Engine_ActorFaceActor(args)
#define BattleRuntime_WaitIfModeZero_18(args...) Engine_EventWait(args)
#define SceneWork_SetStepValue_7(args...) Engine_EventSetMessage(args)
#define BattleEvent_RunActionAndWait_7(args...) Engine_EventShowMessage(args)
#define BattleRuntime_WaitIfModeZero_19(args...) Engine_EventWait(args)
#define ObjectMotion_CallThenWaitForAnimationChange_2(args...) Func_020081e4(args)
#define BattleRuntime_WaitIfModeZero_20(args...) Engine_EventWait(args)
#define BattleEffect_SpawnLinkedResourceObject_3(a0, a1, a2) Value3(Engine_ActorShowEmote, a0, a1, a2)
#define BattleRuntime_WaitIfModeZero_21(args...) Engine_EventWait(args)
#define SceneWork_SetStepValue_8(args...) Engine_EventSetMessage(args)
#define BattleEvent_RunActionAndWait_8(args...) Engine_EventShowMessage(args)
#define BattleRuntime_WaitIfModeZero_22(args...) Engine_EventWait(args)
#define ObjectMotion_CallThenWaitForAnimationChange_3(args...) Func_02008218(args)
#define BattleRuntime_WaitIfModeZero_23(args...) Engine_EventWait(args)
#define ObjectMotion_CallThenWaitForAnimationChange_4(args...) Func_02008226_a(args)
#define BattleRuntime_WaitIfModeZero_24(args...) Engine_EventWait(args)
#define ObjectMotion_ResetAndSetPositionInMode2_5(a0, a1, a2) Value3(Engine_ActorWalkTo, a0, a1, a2)
#define ObjectMotion_CommitCurrentPositionAndActivate_6(args...) Engine_ActorWaitForMove(args)
#define Object_LinkPair_2(args...) Engine_ActorFaceEachOther(args)
#define BattleRuntime_WaitIfModeZero_25(args...) Engine_EventWait(args)
#define SceneWork_SetStepValue_9(args...) Engine_EventSetMessage(args)
#define BattleEvent_RunActionAndWait_9(args...) Engine_EventShowMessage(args)
#define BattleRuntime_WaitIfModeZero_26(args...) Engine_EventWait(args)
#define Object_SetModeById_4(args...) Engine_ActorSetAnimation(args)
#define BattleRuntime_WaitIfModeZero_27(args...) Engine_EventWait(args)
#define SceneWork_SetStepValue_10(args...) Engine_EventSetMessage(args)
#define BattleEvent_RunActionAndWait_10(args...) Engine_EventShowMessage(args)
#define ObjectMotion_SetHorizontalPositionWithTerrain_2(a0, a1, a2) Value3(Engine_ActorSetPosition, a0, a1, a2)
#define Audio_PlayCue_4(args...) Engine_AudioPlayCue(args)
#define SceneWork_SetStepValue_11(args...) Engine_EventSetMessage(args)
#define BattleEvent_RunActionAndWait_11(args...) Engine_EventShowMessage(args)
#define ObjectMotion_SetAngleToward_5(args...) Engine_ActorFaceActor(args)
#define ObjectMotion_SetAngleToward_6(args...) Engine_ActorFaceActor(args)
#define ObjectMotion_SetAngleToward_7(args...) Engine_ActorFaceActor(args)
#define BattleRuntime_WaitIfModeZero_28(args...) Engine_EventWait(args)
#define ObjectMotion_ArmCallback_10(a0, a1, a2) Call3(Func_0200832a, a0, a1, a2)
#define ObjectMotion_SetAngleToward_8(args...) Engine_ActorFaceActor(args)
#define BattleRuntime_WaitIfModeZero_29(args...) Engine_EventWait(args)
#define Audio_PlayCue_5(args...) Engine_AudioPlayCue(args)
#define ObjectMotion_SetSpeedLimitAndAcceleration_2(a0, a1) Call2(Engine_CameraSetSpeed, a0, a1)
#define ObjectMotion_PlaceLookedUpObjectWithinCameraBounds_1(args...) Engine_CameraMoveToActor(args)
#define Object_CommitPositionThenWaitIfModeZero_1(args...) Engine_CameraWaitForMove(args)
#define ObjectMotion_SetSpeedParameters_6(a0, a1, a2) Call3(Engine_ActorSetSpeed, a0, a1, a2)
#define ObjectMotion_ResetAndSetPositionInMode2_6(a0, a1, a2) Call3(Engine_ActorWalkTo, a0, a1, a2)
#define ObjectMotion_SetPositionAndReset_3(args...) Engine_CameraFollowActor(args)
#define ObjectMotion_CommitCurrentPositionAndActivate_7(args...) Engine_ActorWaitForMove(args)
#define ObjectMotion_SetPositionAndReset_4(args...) Engine_CameraFollowActor(args)
#define BattleEffect_SpawnLinkedResourceObject_4(a0, a1, a2) Call3(Engine_ActorShowEmote, a0, a1, a2)
#define BattleEffect_SpawnLinkedResourceObject_5(a0, a1, a2) Call3(Engine_ActorShowEmote, a0, a1, a2)
#define BattleEffect_SpawnLinkedResourceObject_6(a0, a1, a2) Call3(Engine_ActorShowEmote, a0, a1, a2)
#define BattleEffect_SpawnLinkedResourceObject_7(a0, a1, a2) Call3(Engine_ActorShowEmote, a0, a1, a2)
#define BattleEffect_SpawnLinkedResourceObject_8(a0, a1, a2) Value3(Engine_ActorShowEmote, a0, a1, a2)
#define BattleRuntime_WaitIfModeZero_30(args...) Engine_EventWait(args)
#define ObjectMotion_SetAngleToward_9(args...) Engine_ActorFaceActor(args)
#define ObjectMotion_SetVariantCallback_6(args...) Engine_ActorStartRepeatedMotion(args)
#define BattleRuntime_WaitIfModeZero_31(args...) Engine_EventWait(args)
#define SceneWork_SetStepValue_12(args...) Engine_EventSetMessage(args)
#define BattleEvent_RunActionAndWait_12(args...) Engine_EventShowMessage(args)
#define ObjectMotion_SetAngleToward_10(args...) Engine_ActorFaceActor(args)
#define SceneWork_SetStepValue_13(args...) Engine_EventSetMessage(args)
#define BattleEvent_RunActionAndWait_13(args...) Engine_EventShowMessage(args)
#define BattleRuntime_WaitIfModeZero_32(args...) Engine_EventWait(args)
#define ObjectMotion_SetAngleToward_11(args...) Engine_ActorFaceActor(args)
#define BattleRuntime_WaitIfModeZero_33(args...) Engine_EventWait(args)
#define SceneWork_SetStepValue_14(args...) Engine_EventSetMessage(args)
#define BattleEvent_RunActionAndWait_14(args...) Engine_EventShowMessage(args)
#define Object_LinkPair_3(args...) Engine_ActorFaceEachOther(args)
#define Object_LinkPair_4(args...) Engine_ActorFaceEachOther(args)
#define BattleRuntime_WaitIfModeZero_34(args...) Engine_EventWait(args)
#define ObjectMotion_SetAngleToward_12(args...) Engine_ActorFaceActor(args)
#define ObjectMotion_SetAngleToward_13(args...) Engine_ActorFaceActor(args)
#define ObjectMotion_SetAngleToward_14(args...) Engine_ActorFaceActor(args)
#define ObjectMotion_SetAngleToward_15(args...) Engine_ActorFaceActor(args)
#define ObjectMotion_SetVariantCallback_7(args...) Engine_ActorStartRepeatedMotion(args)
#define BattleRuntime_WaitIfModeZero_35(args...) Engine_EventWait(args)
#define SceneWork_SetStepValue_15(args...) Engine_EventSetMessage(args)
#define BattleEvent_RunActionAndWait_15(args...) Engine_EventShowMessage(args)
#define BattleEffect_SpawnLinkedResourceObject_9(a0, a1, a2) Value3(Engine_ActorShowEmote, a0, a1, a2)
#define BattleRuntime_WaitIfModeZero_36(args...) Engine_EventWait(args)
#define Object_SetModeById_5(args...) Engine_ActorSetAnimation(args)
#define SceneWork_SetStepValue_16(args...) Engine_EventSetMessage(args)
#define BattleEvent_RunActionAndWait_16(args...) Engine_EventShowMessage(args)
#define ObjectMotion_ResetAndSetPositionInMode2_7(a0, a1, a2) Value3(Engine_ActorWalkTo, a0, a1, a2)
#define ObjectMotion_CommitCurrentPositionAndActivate_8(args...) Engine_ActorWaitForMove(args)
#define ObjectMotion_ArmCallback_11(a0, a1, a2) Value3(Func_020084f4_a, a0, a1, a2)
#define SceneWork_SetStepValue_17(args...) Engine_EventSetMessage(args)
#define BattleEvent_RunActionAndWait_17(args...) Engine_EventShowMessage(args)
#define ObjectMotion_ResetAndSetPositionInMode2_8(a0, a1, a2) Value3(Engine_ActorWalkTo, a0, a1, a2)
#define ObjectMotion_CommitCurrentPositionAndActivate_9(args...) Engine_ActorWaitForMove(args)
#define ObjectMotion_ArmCallback_12(a0, a1, a2) Value3(Func_02008522_a, a0, a1, a2)
#define BattleRuntime_WaitIfModeZero_37(args...) Engine_EventWait(args)
#define SceneWork_SetStepValue_18(args...) Engine_EventSetMessage(args)
#define BattleEvent_RunActionAndWait_18(args...) Engine_EventShowMessage(args)
#define BattleEffect_SpawnLinkedResourceObject_10(a0, a1, a2) Value3(Engine_ActorShowEmote, a0, a1, a2)
#define BattleRuntime_WaitIfModeZero_38(args...) Engine_EventWait(args)
#define SceneWork_SetStepValue_19(args...) Engine_EventSetMessage(args)
#define BattleEvent_RunActionAndWait_19(args...) Engine_EventShowMessage(args)
#define ObjectMotion_ResetAndSetPositionInMode2_9(a0, a1, a2) Value3(Engine_ActorWalkTo, a0, a1, a2)
#define ObjectMotion_CommitCurrentPositionAndActivate_10(args...) Engine_ActorWaitForMove(args)
#define ObjectMotion_SetAngleToward_16(args...) Engine_ActorFaceActor(args)
#define ObjectMotion_CallThenWaitForAnimationChange_5(args...) Func_02008524(args)
#define BattleRuntime_WaitIfModeZero_39(args...) Engine_EventWait(args)
#define SceneWork_SetStepValue_20(args...) Engine_EventSetMessage(args)
#define BattleEvent_RunActionAndWait_20(args...) Engine_EventShowMessage(args)
#define Object_SetModeById_6(args...) Engine_ActorSetAnimation(args)
#define SceneWork_SetStepValue_21(args...) Engine_EventSetMessage(args)
#define BattleEvent_RunActionAndWait_21(args...) Engine_EventShowMessage(args)
#define BattleEffect_SpawnLinkedResourceObject_11(a0, a1, a2) Call3(Engine_ActorShowEmote, a0, a1, a2)
#define BattleEffect_SpawnLinkedResourceObject_12(a0, a1, a2) Call3(Engine_ActorShowEmote, a0, a1, a2)
#define BattleEffect_SpawnLinkedResourceObject_13(a0, a1, a2) Call3(Engine_ActorShowEmote, a0, a1, a2)
#define BattleEffect_SpawnLinkedResourceObject_14(a0, a1, a2) Call3(Engine_ActorShowEmote, a0, a1, a2)
#define BattleEffect_SpawnLinkedResourceObject_15(a0, a1, a2) Value3(Engine_ActorShowEmote, a0, a1, a2)
#define BattleRuntime_WaitIfModeZero_40(args...) Engine_EventWait(args)
#define SceneWork_SetStepValue_22(args...) Engine_EventSetMessage(args)
#define BattleEvent_RunActionAndWait_22(args...) Engine_EventShowMessage(args)
#define BattleRuntime_WaitIfModeZero_41(args...) Engine_EventWait(args)
#define BattleEffect_SpawnLinkedResourceObject_16(a0, a1, a2) Value3(Engine_ActorShowEmote, a0, a1, a2)
#define BattleRuntime_WaitIfModeZero_42(args...) Engine_EventWait(args)
#define SceneWork_SetStepValue_23(args...) Engine_EventSetMessage(args)
#define BattleEvent_RunActionAndWait_23(args...) Engine_EventShowMessage(args)
#define ObjectMotion_SetVariantCallback_8(args...) Engine_ActorStartRepeatedMotion(args)
#define BattleRuntime_WaitIfModeZero_43(args...) Engine_EventWait(args)
#define SceneWork_SetStepValue_24(args...) Engine_EventSetMessage(args)
#define BattleEvent_RunActionAndWait_24(args...) Engine_EventShowMessage(args)
#define ObjectMotion_ResetAndSetPositionInMode2_10(a0, a1, a2) Value3(Engine_ActorWalkTo, a0, a1, a2)
#define ObjectMotion_CommitCurrentPositionAndActivate_11(args...) Engine_ActorWaitForMove(args)
#define ObjectMotion_ArmCallback_13(a0, a1, a2) Value3(Func_02008660, a0, a1, a2)
#define BattleRuntime_WaitIfModeZero_44(args...) Engine_EventWait(args)
#define ObjectMotion_OffsetPositionAndResetMotion_3(a0, a1, a2) Value3(Engine_ActorSetDestinationOffset, a0, a1, a2)
#define ObjectMotion_CommitCurrentPositionAndActivate_12(args...) Engine_ActorWaitForMove(args)
#define BattleRuntime_WaitIfModeZero_45(args...) Engine_EventWait(args)
#define Audio_PlayCue_6(args...) Engine_AudioPlayCue(args)
#define UiText_DrawMessage_2(args...) Engine_MessageShowCentered(args)
#define ObjectMotion_OffsetPositionAndResetMotion_4(a0, a1, a2) Call3(Engine_ActorSetDestinationOffset, a0, a1, a2)
#define ObjectMotion_SetAngleToward_17(args...) Engine_ActorFaceActor(args)
#define Audio_PlayCue_7(args...) Engine_AudioPlayCue(args)
#define SceneWork_SetStepValue_25(args...) Engine_EventSetMessage(args)
#define BattleEvent_RunActionAndWait_25(args...) Engine_EventShowMessage(args)
#define BattleRuntime_WaitIfModeZero_46(args...) Engine_EventWait(args)
#define ObjectMotion_SetAngleToward_18(args...) Engine_ActorFaceActor(args)
#define ObjectMotion_SetAngleToward_19(args...) Engine_ActorFaceActor(args)
#define ObjectMotion_SetAngleToward_20(args...) Engine_ActorFaceActor(args)
#define ObjectMotion_SetAngleToward_21(args...) Engine_ActorFaceActor(args)
#define ObjectMotion_SetAngleToward_22(args...) Engine_ActorFaceActor(args)
#define GameFlag_Set_2(a0) Value1(Engine_GameFlagSet, a0)
#define BattleRuntime_WaitIfModeZero_47(args...) Engine_EventWait(args)
#define SharedWorkData_SetFirstAndSecondFields_2(args...) Func_02008788_a(args)
#define BattleEffect_ComputeWeightedResultAndDispatch_2(args...) Func_02008788_b(args)
#define BattleRuntime_ScheduleShoulderButtonModeUpdate_1(args...) Engine_EventEnd(args)
#define STATE_TABLE_022B Data_02000240[0x22B]
#define SHARED_RECORD_FIELD_448 (*(u32 *)(*(u8 **)0x03001ebc + 448))
#define Object_SetModeById_1_02001e94(args...) Engine_ActorSetAnimation(args)
#define Object_SetModeById_2_02001e94(args...) Engine_ActorSetAnimation(args)
#define Object_SetModeById_3_02001e94(args...) Engine_ActorSetAnimation(args)
#define Object_SetModeById_4_02001e94(args...) Engine_ActorSetAnimation(args)
#define Audio_PlayCue_1_02001e94(args...) Engine_AudioPlayCue(args)
#define BattleEffect_SpawnLinkedResourceObject_1_02001e94(a0, a1, a2) Call3(Engine_ActorShowEmote, a0, a1, a2)
#define BattleRuntime_WaitIfModeZero_1_02001e94(args...) Engine_EventWait(args)
#define ObjectMotion_SetAngleToward_1_02001e94(args...) Engine_ActorFaceActor(args)
#define SceneWork_SetStepValue_1_02001e94(args...) Engine_EventSetMessage(args)
#define BattleEvent_RunActionAndWait_1_02001e94(args...) Engine_EventShowMessage(args)
#define BattleEffect_SpawnLinkedResourceObject_2_02001e94(a0, a1, a2) Call3(Engine_ActorShowEmote, a0, a1, a2)
#define BattleEffect_SpawnLinkedResourceObject_3_02001e94(a0, a1, a2) Call3(Engine_ActorShowEmote, a0, a1, a2)
#define ObjectMotion_SetAngleToward_2_02001e94(args...) Engine_ActorFaceActor(args)
#define ObjectMotion_SetAngleToward_3_02001e94(args...) Engine_ActorFaceActor(args)
#define ObjectMotion_SetAngleToward_4_02001e94(args...) Engine_ActorFaceActor(args)
#define BattleRuntime_WaitIfModeZero_2_02001e94(args...) Engine_EventWait(args)
#define ObjectMotion_ArmCallback_1_02001e94(a0, a1, a2) Call3(Func_02007636, a0, a1, a2)
#define ObjectMotion_ArmCallback_2_02001e94(a0, a1, a2) Call3(Func_02007642, a0, a1, a2)
#define SceneWork_SetStepValue_2_02001e94(args...) Engine_EventSetMessage(args)
#define BattleEvent_RunActionAndWait_2_02001e94(args...) Engine_EventShowMessage(args)
#define ObjectMotion_CallThenWaitForAnimationChange_1_02001e94(args...) Func_02007600(args)
#define SceneWork_SetStepValue_3_02001e94(args...) Engine_EventSetMessage(args)
#define BattleEvent_RunActionAndWait_3_02001e94(args...) Engine_EventShowMessage(args)
#define SceneWork_SetStepValue_4_02001e94(args...) Engine_EventSetMessage(args)
#define BattleEvent_RunActionAndWait_4_02001e94(args...) Engine_EventShowMessage(args)
#define ObjectMotion_SetVariantCallbackAndInvokeObject_1(args...) Func_0200763c(args)
#define SceneWork_SetStepValue_5_02001e94(args...) Engine_EventSetMessage(args)
#define BattleEvent_RunActionAndWait_5_02001e94(args...) Engine_EventShowMessage(args)
#define ObjectMotion_CallThenWaitForAnimationChange_2_02001e94(args...) Func_0200763a(args)
#define SceneWork_SetStepValue_6_02001e94(args...) Engine_EventSetMessage(args)
#define BattleEvent_RunActionAndWait_6_02001e94(args...) Engine_EventShowMessage(args)
#define ObjectMotion_CallThenWaitForAnimationChange_3_02001e94(args...) Func_02007650(args)
#define BattleRuntime_WaitIfModeZero_3_02001e94(args...) Engine_EventWait(args)
#define ObjectMotion_SetAngleToward_5_02001e94(args...) Engine_ActorFaceActor(args)
#define ObjectMotion_SetAngleToward_6_02001e94(args...) Engine_ActorFaceActor(args)
#define BattleRuntime_WaitIfModeZero_4_02001e94(args...) Engine_EventWait(args)
#define ObjectMotion_ResetAndSetPositionInMode2_1_02001e94(a0, a1, a2) Call3(Engine_ActorWalkTo, a0, a1, a2)
#define ObjectMotion_CommitCurrentPositionAndActivate_1_02001e94(args...) Engine_ActorWaitForMove(args)
#define ObjectMotion_SetAngleToward_7_02001e94(args...) Engine_ActorFaceActor(args)
#define ObjectMotion_CallThenWaitForAnimationChange_4_02001e94(args...) Func_02007694(args)
#define BattleRuntime_WaitIfModeZero_5_02001e94(args...) Engine_EventWait(args)
#define SceneWork_SetStepValue_7_02001e94(args...) Engine_EventSetMessage(args)
#define BattleEvent_RunActionAndWait_7_02001e94(args...) Engine_EventShowMessage(args)
#define SharedWorkData_SetFirstAndSecondFields_1_02001e94(args...) Func_02007778(args)
#define BattleEffect_ComputeWeightedResultAndDispatch_1_02001e94(args...) Func_02007782(args)
#define ObjectMotion_SetHorizontalPositionWithTerrain_1_02001e94(args...) Engine_ActorSetPosition(args)
#define ObjectMotion_SetHorizontalPositionWithTerrain_2_02001e94(args...) Engine_ActorSetPosition(args)
#define ObjectMotion_SetHorizontalPositionWithTerrain_3(args...) Engine_ActorSetPosition(args)
#define BattleRuntime_ScheduleShoulderButtonModeUpdate_1_02001e94(args...) Engine_EventEnd(args)
#define GameFlag_Set_1_02001e94(a0) Call1(Engine_GameFlagSet, a0)
#define BattleRuntime_Reset_1_0200206c() Call0(Engine_EventBegin)
#define ObjectMotion_SetHorizontalPositionWithTerrain_1_0200206c(a0, a1, a2) Call3(Engine_ActorSetPosition, a0, a1, a2)
#define ObjectMotion_SetHorizontalPositionWithTerrain_2_0200206c(a0, a1, a2) Call3(Engine_ActorSetPosition, a0, a1, a2)
#define ObjectMotion_SetHorizontalPositionWithTerrain_3_0200206c(a0, a1, a2) Call3(Engine_ActorSetPosition, a0, a1, a2)
#define Object_SetModeById_1_0200206c(a0, a1) Call2(Engine_ActorSetAnimation, a0, a1)
#define Object_SetModeById_2_0200206c(a0, a1) Call2(Engine_ActorSetAnimation, a0, a1)
#define Object_SetModeById_3_0200206c(a0, a1) Call2(Engine_ActorSetAnimation, a0, a1)
#define ObjectMotion_SetAngleToward_1_0200206c(a0, a1, a2) Call3(Engine_ActorFaceActor, a0, a1, a2)
#define BattleRuntime_ScheduleShoulderButtonModeUpdate_1_0200206c() Call0(Engine_EventEnd)
#define BattleRuntime_WaitIfModeZero_1_0200206c() Call0(Engine_EventOpenScreen)
#define GameFlag_Set_1_02003054(a0) Call1(Engine_GameFlagSet, a0)
#define GameFlag_Set_2_02003054(a0) Call1(Engine_GameFlagSet, a0)
#define BattleRuntime_Reset_1_02003054(args...) Engine_EventBegin(args)
#define Scene_GetRecord_1_02003054(args...) Func_020086d6(args)
#define ObjectMotion_SetHorizontalPositionWithTerrain_1_02003054(a0, a1, a2) Call3(Engine_ActorSetPosition, a0, a1, a2)
#define ObjectMotion_ArmCallback_1_02003054(a0, a1, a2) Call3(Func_020087ae, a0, a1, a2)
#define ObjectMotion_SetHorizontalPositionWithTerrain_2_02003054(a0, a1, a2) Call3(Engine_ActorSetPosition, a0, a1, a2)
#define ObjectMotion_ArmCallback_2_02003054(a0, a1, a2) Call3(Func_020087c8, a0, a1, a2)
#define ObjectMotion_SetHorizontalPositionWithTerrain_3_02003054(a0, a1, a2) Call3(Engine_ActorSetPosition, a0, a1, a2)
#define ObjectMotion_ArmCallback_3_02003054(a0, a1, a2) Call3(Func_020087e2, a0, a1, a2)
#define ObjectMotion_SetHorizontalPositionWithTerrain_4(a0, a1, a2) Call3(Engine_ActorSetPosition, a0, a1, a2)
#define ObjectMotion_ArmCallback_4_02003054(a0, a1, a2) Call3(Func_020087fc, a0, a1, a2)
#define ObjectMotion_SetHorizontalPositionWithTerrain_5(a0, a1, a2) Call3(Engine_ActorSetPosition, a0, a1, a2)
#define ObjectMotion_ArmCallback_5_02003054(a0, a1, a2) Call3(Func_02008816, a0, a1, a2)
#define ObjectMotion_SetHorizontalPositionWithTerrain_6(a0, a1, a2) Call3(Engine_ActorSetPosition, a0, a1, a2)
#define ObjectMotion_ArmCallback_6_02003054(a0, a1, a2) Call3(Func_02008830, a0, a1, a2)
#define ObjectMotion_SetHorizontalPositionWithTerrain_7(a0, a1, a2) Call3(Engine_ActorSetPosition, a0, a1, a2)
#define ObjectMotion_SetActionVariant_1(args...) Engine_ActorSetSpritePriority(args)
#define Object_SetModeById_1_02003054(args...) Engine_ActorSetAnimation(args)
#define ObjectMotion_SetHorizontalPositionWithTerrain_8(a0, a1, a2) Call3(Engine_ActorSetPosition, a0, a1, a2)
#define ObjectMotion_ArmCallback_7_02003054(a0, a1, a2) Call3(Func_02008868, a0, a1, a2)
#define Object_SetModeById_2_02003054(args...) Engine_ActorSetAnimation(args)
#define BattleRuntime_WaitIfModeZero_1_02003054(args...) Engine_EventOpenScreen(args)
#define BattleRuntime_WaitIfModeZero_2_02003054(args...) Engine_EventWait(args)
#define SceneWork_SetStepValue_1_02003054(args...) Engine_EventSetMessage(args)
#define BattleEvent_RunActionAndWait_1_02003054(args...) Engine_EventShowMessage(args)
#define BattleEffect_SpawnLinkedResourceObject_1_02003054(a0, a1, a2) Call3(Engine_ActorShowEmote, a0, a1, a2)
#define SceneWork_SetStepValue_2_02003054(args...) Engine_EventSetMessage(args)
#define BattleEvent_RunActionAndWait_2_02003054(args...) Engine_EventShowMessage(args)
#define BattleEffect_SpawnLinkedResourceObject_2_02003054(a0, a1, a2) Call3(Engine_ActorShowEmote, a0, a1, a2)
#define BattleEffect_SpawnLinkedResourceObject_3_02003054(a0, a1, a2) Call3(Engine_ActorShowEmote, a0, a1, a2)
#define BattleEffect_SpawnLinkedResourceObject_4_02003054(a0, a1, a2) Call3(Engine_ActorShowEmote, a0, a1, a2)
#define BattleRuntime_WaitIfModeZero_3_02003054(args...) Engine_EventWait(args)
#define ObjectMotion_SetAngleToward_1_02003054(args...) Engine_ActorFaceActor(args)
#define BattleRuntime_WaitIfModeZero_4_02003054(args...) Engine_EventWait(args)
#define SceneWork_SetStepValue_3_02003054(args...) Engine_EventSetMessage(args)
#define BattleEvent_RunActionAndWait_3_02003054(args...) Engine_EventShowMessage(args)
#define ObjectMotion_SetAngleToward_2_02003054(args...) Engine_ActorFaceActor(args)
#define ObjectMotion_SetAngleToward_3_02003054(args...) Engine_ActorFaceActor(args)
#define ObjectMotion_SetAngleToward_4_02003054(args...) Engine_ActorFaceActor(args)
#define ObjectMotion_SetAngleToward_5_02003054(args...) Engine_ActorFaceActor(args)
#define BattleRuntime_WaitIfModeZero_5_02003054(args...) Engine_EventWait(args)
#define ObjectMotion_ResetAndSetPositionInMode2_1_02003054(a0, a1, a2) Call3(Engine_ActorWalkTo, a0, a1, a2)
#define Object_SetModeById_3_02003054(args...) Engine_ActorSetAnimation(args)
#define BattleRuntime_WaitIfModeZero_6_02003054(args...) Engine_EventWait(args)
#define SceneWork_SetStepValue_4_02003054(args...) Engine_EventSetMessage(args)
#define BattleEvent_RunActionAndWait_4_02003054(args...) Engine_EventShowMessage(args)
#define BattleRuntime_WaitIfModeZero_7_02003054(args...) Engine_EventWait(args)
#define ObjectMotion_SetVariantCallback_1_02003054(args...) Engine_ActorStartRepeatedMotion(args)
#define SceneWork_SetStepValue_5_02003054(args...) Engine_EventSetMessage(args)
#define BattleEvent_RunActionAndWait_5_02003054(args...) Engine_EventShowMessage(args)
#define ObjectMotion_SetAngleToward_6_02003054(args...) Engine_ActorFaceActor(args)
#define ObjectMotion_SetAngleToward_7_02003054(args...) Engine_ActorFaceActor(args)
#define ObjectMotion_SetAngleToward_8_02003054(args...) Engine_ActorFaceActor(args)
#define ObjectMotion_SetAngleToward_9_02003054(args...) Engine_ActorFaceActor(args)
#define ObjectMotion_SetVariantCallback_2_02003054(args...) Engine_ActorStartRepeatedMotion(args)
#define SceneWork_SetStepValue_6_02003054(args...) Engine_EventSetMessage(args)
#define BattleEvent_RunActionAndWait_6_02003054(args...) Engine_EventShowMessage(args)
#define BattleEffect_SpawnLinkedResourceObject_5_02003054(a0, a1, a2) Call3(Engine_ActorShowEmote, a0, a1, a2)
#define Object_SetModeById_4_02003054(args...) Engine_ActorSetAnimation(args)
#define BattleRuntime_WaitIfModeZero_8_02003054(args...) Engine_EventWait(args)
#define Object_SetModeById_5_02003054(args...) Engine_ActorSetAnimation(args)
#define SceneWork_SetStepValue_7_02003054(args...) Engine_EventSetMessage(args)
#define BattleEvent_RunActionAndWait_7_02003054(args...) Engine_EventShowMessage(args)
#define BattleEffect_SpawnLinkedResourceObject_6_02003054(a0, a1, a2) Call3(Engine_ActorShowEmote, a0, a1, a2)
#define ObjectMotion_SetVariantCallback_3_02003054(args...) Engine_ActorStartRepeatedMotion(args)
#define BattleRuntime_WaitIfModeZero_9_02003054(args...) Engine_EventWait(args)
#define SceneWork_SetStepValue_8_02003054(args...) Engine_EventSetMessage(args)
#define BattleEvent_RunActionAndWait_8_02003054(args...) Engine_EventShowMessage(args)
#define ObjectMotion_SetAngleToward_10_02003054(args...) Engine_ActorFaceActor(args)
#define BattleRuntime_WaitIfModeZero_10_02003054(args...) Engine_EventWait(args)
#define SceneWork_SetStepValue_9_02003054(args...) Engine_EventSetMessage(args)
#define BattleEvent_RunActionAndWait_9_02003054(args...) Engine_EventShowMessage(args)
#define ObjectMotion_SetVariantCallback_4_02003054(args...) Engine_ActorStartRepeatedMotion(args)
#define BattleRuntime_WaitIfModeZero_11_02003054(args...) Engine_EventWait(args)
#define SceneWork_SetStepValue_10_02003054(args...) Engine_EventSetMessage(args)
#define BattleEvent_RunActionAndWait_10_02003054(args...) Engine_EventShowMessage(args)
#define ObjectMotion_SetAngleToward_11_02003054(args...) Engine_ActorFaceActor(args)
#define SceneWork_SetStepValue_11_02003054(args...) Engine_EventSetMessage(args)
#define ObjectMotion_SetSpeedParameters_1_02003054(a0, a1) Value2(Func_02008a2a, a0, a1)
#define UiWork_WaitThenFinalizeCapacity_1(args...) Engine_EventChooseYesNo(args)
#define Object_SetModeById_6_02003054(args...) Engine_ActorSetAnimation(args)
#define Object_SetModeById_7(args...) Engine_ActorSetAnimation(args)
#define Object_SetModeById_8(args...) Engine_ActorSetAnimation(args)
#define ObjectMotion_SetAngleToward_12_02003054(args...) Engine_ActorFaceActor(args)
#define BattleRuntime_WaitIfModeZero_12_02003054(args...) Engine_EventWait(args)
#define SceneWork_SetStepValue_12_02003054(args...) Engine_EventSetMessage(args)
#define BattleEvent_RunActionAndWait_11_02003054(args...) Engine_EventShowMessage(args)
#define ObjectMotion_SetAngleToward_13_02003054(args...) Engine_ActorFaceActor(args)
#define ObjectMotion_SetAngleToward_14_02003054(args...) Engine_ActorFaceActor(args)
#define ObjectMotion_SetAngleToward_15_02003054(args...) Engine_ActorFaceActor(args)
#define ObjectMotion_SetAngleToward_16_02003054(args...) Engine_ActorFaceActor(args)
#define BattleRuntime_WaitIfModeZero_13_02003054(args...) Engine_EventWait(args)
#define ObjectMotion_SetAngleToward_17_02003054(args...) Engine_ActorFaceActor(args)
#define BattleRuntime_WaitIfModeZero_14_02003054(args...) Engine_EventWait(args)
#define ObjectMotion_SetAngleToward_18_02003054(args...) Engine_ActorFaceActor(args)
#define BattleRuntime_WaitIfModeZero_15_02003054(args...) Engine_EventWait(args)
#define ObjectMotion_CallThenWaitForAnimationChange_1_02003054(args...) Func_02008a9c(args)
#define BattleRuntime_WaitIfModeZero_16_02003054(args...) Engine_EventWait(args)
#define ObjectMotion_SetAngleToward_19_02003054(args...) Engine_ActorFaceActor(args)
#define SceneWork_SetStepValue_13_02003054(args...) Engine_EventSetMessage(args)
#define BattleEvent_RunActionAndWait_12_02003054(args...) Engine_EventShowMessage(args)
#define BattleRuntime_WaitIfModeZero_17_02003054(args...) Engine_EventWait(args)
#define ObjectMotion_SetAngleToward_20_02003054(args...) Engine_ActorFaceActor(args)
#define ObjectMotion_SetAngleToward_21_02003054(args...) Engine_ActorFaceActor(args)
#define ObjectMotion_SetAngleToward_22_02003054(args...) Engine_ActorFaceActor(args)
#define ObjectMotion_SetAngleToward_23(args...) Engine_ActorFaceActor(args)
#define Object_SetModeById_9(args...) Engine_ActorSetAnimation(args)
#define Object_SetModeById_10(args...) Engine_ActorSetAnimation(args)
#define Object_SetModeById_11(args...) Engine_ActorSetAnimation(args)
#define Object_SetModeById_12(args...) Engine_ActorSetAnimation(args)
#define BattleRuntime_WaitIfModeZero_18_02003054(args...) Engine_EventWait(args)
#define ObjectMotion_SetAngleToward_24(args...) Engine_ActorFaceActor(args)
#define ObjectMotion_SetAngleToward_25(args...) Engine_ActorFaceActor(args)
#define ObjectMotion_SetAngleToward_26(args...) Engine_ActorFaceActor(args)
#define ObjectMotion_SetAngleToward_27(args...) Engine_ActorFaceActor(args)
#define ObjectMotion_SetAngleToward_28(args...) Engine_ActorFaceActor(args)
#define ObjectMotion_SetSpeedParameters_2_02003054(a0, a1, a2) Call3(Engine_ActorSetSpeed, a0, a1, a2)
#define ObjectMotion_SetSpeedParameters_3_02003054(a0, a1, a2) Call3(Engine_ActorSetSpeed, a0, a1, a2)
#define ObjectMotion_SetSpeedParameters_4_02003054(a0, a1, a2) Call3(Engine_ActorSetSpeed, a0, a1, a2)
#define ObjectMotion_SetSpeedParameters_5_02003054(a0, a1, a2) Call3(Engine_ActorSetSpeed, a0, a1, a2)
#define ObjectMotion_ResetAndSetPositionInMode2_2_02003054(a0, a1, a2) Call3(Engine_ActorWalkTo, a0, a1, a2)
#define ObjectMotion_CommitCurrentPositionAndActivate_1_02003054(args...) Engine_ActorWaitForMove(args)
#define ObjectMotion_ResetAndSetPositionInMode2_3_02003054(a0, a1, a2) Call3(Engine_ActorWalkTo, a0, a1, a2)
#define ObjectMotion_CommitCurrentPositionAndActivate_2_02003054(args...) Engine_ActorWaitForMove(args)
#define ObjectMotion_ResetAndSetPositionInMode2_4_02003054(a0, a1, a2) Call3(Engine_ActorWalkTo, a0, a1, a2)
#define ObjectMotion_CommitCurrentPositionAndActivate_3_02003054(args...) Engine_ActorWaitForMove(args)
#define ObjectMotion_ArmCallback_8_02003054(a0, a1, a2) Call3(Func_02008c14, a0, a1, a2)
#define ObjectMotion_ResetAndSetPositionInMode2_5_02003054(a0, a1, a2) Call3(Engine_ActorWalkTo, a0, a1, a2)
#define ObjectMotion_CommitCurrentPositionAndActivate_4_02003054(args...) Engine_ActorWaitForMove(args)
#define ObjectMotion_ArmCallback_9_02003054(args...) Func_02008c30(args)
#define ObjectMotion_ResetAndSetPositionInMode2_6_02003054(a0, a1, a2) Call3(Engine_ActorWalkTo, a0, a1, a2)
#define ObjectMotion_CommitCurrentPositionAndActivate_5_02003054(args...) Engine_ActorWaitForMove(args)
#define ObjectMotion_ResetAndSetPositionInMode2_7_02003054(a0, a1, a2) Call3(Engine_ActorWalkTo, a0, a1, a2)
#define ObjectMotion_CommitCurrentPositionAndActivate_6_02003054(args...) Engine_ActorWaitForMove(args)
#define ObjectMotion_ArmCallback_10_02003054(a0, a1, a2) Call3(Func_02008c60, a0, a1, a2)
#define ObjectMotion_ResetAndSetPositionInMode2_8_02003054(a0, a1, a2) Call3(Engine_ActorWalkTo, a0, a1, a2)
#define ObjectMotion_CommitCurrentPositionAndActivate_7_02003054(args...) Engine_ActorWaitForMove(args)
#define ObjectMotion_ArmCallback_11_02003054(a0, a1, a2) Call3(Func_02008c7e_a, a0, a1, a2)
#define Object_LinkPair_1_02003054(args...) Engine_ActorFaceEachOther(args)
#define Object_LinkPair_2_02003054(args...) Engine_ActorFaceEachOther(args)
#define BattleRuntime_WaitIfModeZero_19_02003054(args...) Engine_EventWait(args)
#define ObjectMotion_CallThenWaitForAnimationChange_2_02003054(args...) Func_02008c48_a(args)
#define Object_SetModeById_13(args...) Engine_ActorSetAnimation(args)
#define Object_SetModeById_14(args...) Engine_ActorSetAnimation(args)
#define Object_SetModeById_15(args...) Engine_ActorSetAnimation(args)
#define BattleRuntime_WaitIfModeZero_20_02003054(args...) Engine_EventWait(args)
#define Audio_PlayCue_1_02003054(args...) Engine_AudioPlayCue(args)
#define Object_SetModeById_16(args...) Engine_ActorSetAnimation(args)
#define ObjectMotion_SetSpeedParameters_6_02003054(a0, a1, a2) Call3(Engine_ActorSetSpeed, a0, a1, a2)
#define ObjectMotion_OffsetPositionAndResetMotion_1_02003054(a0, a1, a2) Call3(Engine_ActorSetDestinationOffset, a0, a1, a2)
#define ObjectMotion_SetSpeedParameters_7(a0, a1, a2) Call3(Engine_ActorSetSpeed, a0, a1, a2)
#define ObjectMotion_OffsetPositionAndResetMotion_2_02003054(a0, a1, a2) Call3(Engine_ActorSetDestinationOffset, a0, a1, a2)
#define ObjectMotion_SetSpeedParameters_8(a0, a1, a2) Call3(Engine_ActorSetSpeed, a0, a1, a2)
#define ObjectMotion_OffsetPositionAndResetMotion_3_02003054(a0, a1, a2) Call3(Engine_ActorSetDestinationOffset, a0, a1, a2)
#define ObjectMotion_SetSpeedParameters_9(a0, a1, a2) Call3(Engine_ActorSetSpeed, a0, a1, a2)
#define ObjectMotion_OffsetPositionAndResetMotion_4_02003054(a0, a1, a2) Call3(Engine_ActorSetDestinationOffset, a0, a1, a2)
#define ObjectMotion_SetSpeedParameters_10(a0, a1, a2) Call3(Engine_ActorSetSpeed, a0, a1, a2)
#define ObjectMotion_SetSpeedParameters_11(a0, a1, a2) Call3(Engine_ActorSetSpeed, a0, a1, a2)
#define ObjectMotion_OffsetPositionAndResetMotion_5(a0, a1, a2) Call3(Engine_ActorSetDestinationOffset, a0, a1, a2)
#define ObjectMotion_OffsetPositionAndResetMotion_6(a0, a1, a2) Call3(Engine_ActorSetDestinationOffset, a0, a1, a2)
#define ObjectMotion_CommitCurrentPositionAndActivate_8_02003054(args...) Engine_ActorWaitForMove(args)
#define Audio_PlayCue_2_02003054(a0) Call1(Engine_AudioPlayCue, a0)
#define BattleRuntime_WaitIfModeZero_21_02003054(args...) Engine_EventWait(args)
#define ObjectMotion_ArmCallback_12_02003054(a0, a1, a2) Call3(Func_02008d70, a0, a1, a2)
#define ObjectMotion_ArmCallback_13_02003054(a0, a1, a2) Call3(Func_02008d7c, a0, a1, a2)
#define ObjectMotion_ArmCallback_14(a0, a1, a2) Call3(Func_02008d88, a0, a1, a2)
#define ObjectMotion_ArmCallback_15(a0, a1, a2) Call3(Func_02008d94_a, a0, a1, a2)
#define BattleRuntime_WaitIfModeZero_22_02003054(args...) Engine_EventWait(args)
#define Object_SetModeById_17(args...) Engine_ActorSetAnimation(args)
#define Object_SetModeById_18(args...) Engine_ActorSetAnimation(args)
#define Object_SetModeById_19(args...) Engine_ActorSetAnimation(args)
#define Object_SetModeById_20(args...) Engine_ActorSetAnimation(args)
#define BattleRuntime_WaitIfModeZero_23_02003054(args...) Engine_EventWait(args)
#define ObjectMotion_SetSpeedParameters_12(a0, a1, a2) Call3(Engine_ActorSetSpeed, a0, a1, a2)
#define ObjectMotion_SetSpeedParameters_13(a0, a1, a2) Call3(Engine_ActorSetSpeed, a0, a1, a2)
#define ObjectMotion_SetSpeedParameters_14(a0, a1, a2) Call3(Engine_ActorSetSpeed, a0, a1, a2)
#define ObjectMotion_SetSpeedParameters_15(a0, a1, a2) Call3(Engine_ActorSetSpeed, a0, a1, a2)
#define ObjectMotion_ResetAndSetPositionInMode2_9_02003054(a0, a1, a2) Call3(Engine_ActorWalkTo, a0, a1, a2)
#define ObjectMotion_CommitCurrentPositionAndActivate_9_02003054(args...) Engine_ActorWaitForMove(args)
#define Object_SetModeById_21(args...) Engine_ActorSetAnimation(args)
#define ObjectMotion_ArmCallback_16(a0, a1, a2) Call3(Func_02008e0e, a0, a1, a2)
#define ObjectMotion_ResetAndSetPositionInMode2_10_02003054(a0, a1, a2) Call3(Engine_ActorWalkTo, a0, a1, a2)
#define ObjectMotion_CommitCurrentPositionAndActivate_10_02003054(args...) Engine_ActorWaitForMove(args)
#define ObjectMotion_ResetAndSetPositionInMode2_11(a0, a1, a2) Call3(Engine_ActorWalkTo, a0, a1, a2)
#define ObjectMotion_CommitCurrentPositionAndActivate_11_02003054(args...) Engine_ActorWaitForMove(args)
#define ObjectMotion_ResetAndSetPositionInMode2_12(a0, a1, a2) Call3(Engine_ActorWalkTo, a0, a1, a2)
#define ObjectMotion_CommitCurrentPositionAndActivate_12_02003054(args...) Engine_ActorWaitForMove(args)
#define Object_SetModeById_22(args...) Engine_ActorSetAnimation(args)
#define ObjectMotion_ArmCallback_17(a0, a1, a2) Call3(Func_02008e5a, a0, a1, a2)
#define ObjectMotion_ResetAndSetPositionInMode2_13(a0, a1, a2) Call3(Engine_ActorWalkTo, a0, a1, a2)
#define ObjectMotion_CommitCurrentPositionAndActivate_13(args...) Engine_ActorWaitForMove(args)
#define ObjectMotion_ResetAndSetPositionInMode2_14(a0, a1, a2) Call3(Engine_ActorWalkTo, a0, a1, a2)
#define ObjectMotion_ResetAndSetPositionInMode2_15(a0, a1, a2) Call3(Engine_ActorWalkTo, a0, a1, a2)
#define ObjectMotion_CommitCurrentPositionAndActivate_14(args...) Engine_ActorWaitForMove(args)
#define Object_SetModeById_23(args...) Engine_ActorSetAnimation(args)
#define ObjectMotion_ArmCallback_18(a0, a1, a2) Call3(Func_02008e9e, a0, a1, a2)
#define ObjectMotion_CommitCurrentPositionAndActivate_15(args...) Engine_ActorWaitForMove(args)
#define Object_SetModeById_24(args...) Engine_ActorSetAnimation(args)
#define ObjectMotion_ArmCallback_19(a0, a1, a2) Call3(Func_02008eb8, a0, a1, a2)
#define BattleRuntime_WaitIfModeZero_24_02003054(args...) Engine_EventWait(args)
#define BattleRuntime_WaitIfModeZero_25_02003054(args...) Engine_EventWait(args)
#define SceneWork_SetStepValue_14_02003054(args...) Engine_EventSetMessage(args)
#define BattleEvent_RunActionAndWait_13_02003054(args...) Engine_EventShowMessage(args)
#define Object_SetModeById_25(args...) Engine_ActorSetAnimation(args)
#define BattleRuntime_WaitIfModeZero_26_02003054(args...) Engine_EventWait(args)
#define ObjectMotion_SetVariantCallback_5_02003054(args...) Engine_ActorStartRepeatedMotion(args)
#define BattleRuntime_WaitIfModeZero_27_02003054(args...) Engine_EventWait(args)
#define Object_SetModeById_26(args...) Engine_ActorSetAnimation(args)
#define SceneWork_SetStepValue_15_02003054(args...) Engine_EventSetMessage(args)
#define BattleEvent_RunActionAndWait_14_02003054(args...) Engine_EventShowMessage(args)
#define BattleRuntime_WaitIfModeZero_28_02003054(args...) Engine_EventWait(args)
#define Object_LinkPair_3_02003054(args...) Engine_ActorFaceEachOther(args)
#define BattleRuntime_WaitIfModeZero_29_02003054(args...) Engine_EventWait(args)
#define BattleEffect_SpawnLinkedResourceObject_7_02003054(a0, a1, a2) Call3(Engine_ActorShowEmote, a0, a1, a2)
#define ObjectMotion_SetAngleToward_29(args...) Engine_ActorFaceActor(args)
#define ObjectMotion_SetAngleToward_30(args...) Engine_ActorFaceActor(args)
#define SceneWork_SetStepValue_16_02003054(args...) Engine_EventSetMessage(args)
#define BattleEvent_RunActionAndWait_15_02003054(args...) Engine_EventShowMessage(args)
#define BattleEffect_SpawnLinkedResourceObject_8_02003054(a0, a1, a2) Call3(Engine_ActorShowEmote, a0, a1, a2)
#define ObjectMotion_SetVariantCallback_6_02003054(args...) Engine_ActorStartRepeatedMotion(args)
#define BattleRuntime_WaitIfModeZero_30_02003054(args...) Engine_EventWait(args)
#define Object_SetModeById_27(args...) Engine_ActorSetAnimation(args)
#define BattleRuntime_WaitIfModeZero_31_02003054(args...) Engine_EventWait(args)
#define SceneWork_SetStepValue_17_02003054(args...) Engine_EventSetMessage(args)
#define BattleEvent_RunActionAndWait_16_02003054(args...) Engine_EventShowMessage(args)
#define Object_SetModeById_28(args...) Engine_ActorSetAnimation(args)
#define BattleEffect_SpawnLinkedResourceObject_9_02003054(a0, a1, a2) Call3(Engine_ActorShowEmote, a0, a1, a2)
#define SceneWork_SetStepValue_18_02003054(args...) Engine_EventSetMessage(args)
#define BattleEvent_RunActionAndWait_17_02003054(args...) Engine_EventShowMessage(args)
#define ObjectMotion_CallThenWaitForAnimationChange_3_02003054(args...) Func_02008f50(args)
#define BattleEffect_SpawnLinkedResourceObject_10_02003054(a0, a1, a2) Call3(Engine_ActorShowEmote, a0, a1, a2)
#define ObjectMotion_ResetAndSetPositionInMode2_16(a0, a1, a2) Call3(Engine_ActorWalkTo, a0, a1, a2)
#define ObjectMotion_CommitCurrentPositionAndActivate_16(args...) Engine_ActorWaitForMove(args)
#define ObjectMotion_SetAngleToward_31(args...) Engine_ActorFaceActor(args)
#define SceneWork_SetStepValue_19_02003054(args...) Engine_EventSetMessage(args)
#define BattleEvent_RunActionAndWait_18_02003054(args...) Engine_EventShowMessage(args)
#define BattleRuntime_WaitIfModeZero_32_02003054(args...) Engine_EventWait(args)
#define BattleEffect_SpawnLinkedResourceObject_11_02003054(a0, a1, a2) Call3(Engine_ActorShowEmote, a0, a1, a2)
#define BattleEffect_SpawnLinkedResourceObject_12_02003054(a0, a1, a2) Call3(Engine_ActorShowEmote, a0, a1, a2)
#define BattleEffect_SpawnLinkedResourceObject_13_02003054(a0, a1, a2) Call3(Engine_ActorShowEmote, a0, a1, a2)
#define BattleEffect_SpawnLinkedResourceObject_14_02003054(a0, a1, a2) Call3(Engine_ActorShowEmote, a0, a1, a2)
#define BattleRuntime_WaitIfModeZero_33_02003054(args...) Engine_EventWait(args)
#define ObjectMotion_SetAngleToward_32(args...) Engine_ActorFaceActor(args)
#define BattleEffect_SpawnLinkedResourceObject_15_02003054(a0, a1, a2) Call3(Engine_ActorShowEmote, a0, a1, a2)
#define ObjectMotion_SetAngleToward_33(args...) Engine_ActorFaceActor(args)
#define Object_SetModeById_29(args...) Engine_ActorSetAnimation(args)
#define SceneWork_SetStepValue_20_02003054(args...) Engine_EventSetMessage(args)
#define BattleEvent_RunActionAndWait_19_02003054(args...) Engine_EventShowMessage(args)
#define BattleEffect_SpawnLinkedResourceObject_16_02003054(a0, a1, a2) Call3(Engine_ActorShowEmote, a0, a1, a2)
#define BattleRuntime_WaitIfModeZero_34_02003054(args...) Engine_EventWait(args)
#define SceneWork_SetStepValue_21_02003054(args...) Engine_EventSetMessage(args)
#define BattleEvent_RunActionAndWait_20_02003054(args...) Engine_EventShowMessage(args)
#define ObjectMotion_ResetAndSetPositionInMode2_17(a0, a1, a2) Call3(Engine_ActorWalkTo, a0, a1, a2)
#define ObjectMotion_CommitCurrentPositionAndActivate_17(args...) Engine_ActorWaitForMove(args)
#define Object_SetModeById_30(args...) Engine_ActorSetAnimation(args)
#define ObjectMotion_SetAngleToward_34(args...) Engine_ActorFaceActor(args)
#define SceneWork_SetStepValue_22_02003054(args...) Engine_EventSetMessage(args)
#define BattleEvent_RunActionAndWait_21_02003054(args...) Engine_EventShowMessage(args)
#define ObjectMotion_SetVariantCallback_7_02003054(args...) Engine_ActorStartRepeatedMotion(args)
#define BattleRuntime_WaitIfModeZero_35_02003054(args...) Engine_EventWait(args)
#define ObjectMotion_SetAngleToward_35(args...) Engine_ActorFaceActor(args)
#define BattleRuntime_WaitIfModeZero_36_02003054(args...) Engine_EventWait(args)
#define SceneWork_SetStepValue_23_02003054(args...) Engine_EventSetMessage(args)
#define BattleEvent_RunActionAndWait_22_02003054(args...) Engine_EventShowMessage(args)
#define Object_SetModeById_31(args...) Engine_ActorSetAnimation(args)
#define BattleRuntime_WaitIfModeZero_37_02003054(args...) Engine_EventWait(args)
#define SceneWork_SetStepValue_24_02003054(args...) Engine_EventSetMessage(args)
#define BattleEvent_RunActionAndWait_23_02003054(args...) Engine_EventShowMessage(args)
#define ObjectMotion_SetVariantCallback_8_02003054(args...) Engine_ActorStartRepeatedMotion(args)
#define BattleRuntime_WaitIfModeZero_38_02003054(args...) Engine_EventWait(args)
#define SceneWork_SetStepValue_25_02003054(args...) Engine_EventSetMessage(args)
#define BattleEvent_RunActionAndWait_24_02003054(args...) Engine_EventShowMessage(args)
#define ObjectMotion_SetAngleToward_36(args...) Engine_ActorFaceActor(args)
#define BattleRuntime_WaitIfModeZero_39_02003054(args...) Engine_EventWait(args)
#define SceneWork_SetStepValue_26(args...) Engine_EventSetMessage(args)
#define BattleEvent_RunActionAndWait_25_02003054(args...) Engine_EventShowMessage(args)
#define BattleRuntime_WaitIfModeZero_40_02003054(args...) Engine_EventWait(args)
#define BattleEffect_SpawnLinkedResourceObject_17(a0, a1, a2) Call3(Engine_ActorShowEmote, a0, a1, a2)
#define ObjectMotion_SetAngleToward_37(args...) Engine_ActorFaceActor(args)
#define SceneWork_SetStepValue_27(args...) Engine_EventSetMessage(args)
#define BattleEvent_RunActionAndWait_26(args...) Engine_EventShowMessage(args)
#define ObjectMotion_SetAngleToward_38(args...) Engine_ActorFaceActor(args)
#define SceneWork_SetStepValue_28(args...) Engine_EventSetMessage(args)
#define BattleEvent_RunActionAndWait_27(args...) Engine_EventShowMessage(args)
#define ObjectMotion_SetAngleToward_39(args...) Engine_ActorFaceActor(args)
#define ObjectMotion_SetAngleToward_40(args...) Engine_ActorFaceActor(args)
#define ObjectMotion_SetAngleToward_41(args...) Engine_ActorFaceActor(args)
#define ObjectMotion_SetAngleToward_42(args...) Engine_ActorFaceActor(args)
#define BattleRuntime_WaitIfModeZero_41_02003054(args...) Engine_EventWait(args)
#define BattleEffect_SpawnLinkedResourceObject_18(a0, a1, a2) Call3(Engine_ActorShowEmote, a0, a1, a2)
#define ObjectMotion_SetVariantCallback_9(args...) Engine_ActorStartRepeatedMotion(args)
#define BattleRuntime_WaitIfModeZero_42_02003054(args...) Engine_EventWait(args)
#define ObjectMotion_SetVariantCallback_10(args...) Engine_ActorStartRepeatedMotion(args)
#define BattleRuntime_WaitIfModeZero_43_02003054(args...) Engine_EventWait(args)
#define SceneWork_SetStepValue_29(args...) Engine_EventSetMessage(args)
#define BattleEvent_RunActionAndWait_28(args...) Engine_EventShowMessage(args)
#define BattleEffect_SpawnLinkedResourceObject_19(a0, a1, a2) Call3(Engine_ActorShowEmote, a0, a1, a2)
#define ObjectMotion_ResetAndSetPositionInMode2_18(a0, a1, a2) Call3(Engine_ActorWalkTo, a0, a1, a2)
#define ObjectMotion_CommitCurrentPositionAndActivate_18(args...) Engine_ActorWaitForMove(args)
#define Object_SetModeById_32(args...) Engine_ActorSetAnimation(args)
#define SceneWork_SetStepValue_30(args...) Engine_EventSetMessage(args)
#define BattleEvent_RunActionAndWait_29(args...) Engine_EventShowMessage(args)
#define ObjectMotion_SetAngleToward_43(args...) Engine_ActorFaceActor(args)
#define BattleRuntime_WaitIfModeZero_44_02003054(args...) Engine_EventWait(args)
#define ObjectMotion_SetVariantCallback_11(args...) Engine_ActorStartRepeatedMotion(args)
#define SceneWork_SetStepValue_31(args...) Engine_EventSetMessage(args)
#define BattleEvent_RunActionAndWait_30(args...) Engine_EventShowMessage(args)
#define ObjectMotion_SetAngleToward_44(args...) Engine_ActorFaceActor(args)
#define BattleRuntime_WaitIfModeZero_45_02003054(args...) Engine_EventWait(args)
#define ObjectMotion_SetAngleToward_45(args...) Engine_ActorFaceActor(args)
#define BattleRuntime_WaitIfModeZero_46_02003054(args...) Engine_EventWait(args)
#define ObjectMotion_SetAngleToward_46(args...) Engine_ActorFaceActor(args)
#define BattleRuntime_WaitIfModeZero_47_02003054(args...) Engine_EventWait(args)
#define ObjectMotion_SetAngleToward_47(args...) Engine_ActorFaceActor(args)
#define ObjectMotion_SetAngleToward_48(args...) Engine_ActorFaceActor(args)
#define ObjectMotion_SetAngleToward_49(args...) Engine_ActorFaceActor(args)
#define ObjectMotion_SetAngleToward_50(args...) Engine_ActorFaceActor(args)
#define BattleRuntime_WaitIfModeZero_48(args...) Engine_EventWait(args)
#define ObjectMotion_CallThenWaitForAnimationChange_4_02003054(args...) Func_02009258(args)
#define BattleRuntime_WaitIfModeZero_49(args...) Engine_EventWait(args)
#define SceneWork_SetStepValue_32(args...) Engine_EventSetMessage(args)
#define BattleEvent_RunActionAndWait_31(args...) Engine_EventShowMessage(args)
#define ObjectMotion_SetVariantCallback_12(args...) Engine_ActorStartRepeatedMotion(args)
#define BattleRuntime_WaitIfModeZero_50(args...) Engine_EventWait(args)
#define SceneWork_SetStepValue_33(args...) Engine_EventSetMessage(args)
#define BattleEvent_RunActionAndWait_32(args...) Engine_EventShowMessage(args)
#define BattleEffect_SpawnLinkedResourceObject_20(a0, a1, a2) Call3(Engine_ActorShowEmote, a0, a1, a2)
#define SceneWork_SetStepValue_34(args...) Engine_EventSetMessage(args)
#define BattleEvent_RunActionAndWait_33(args...) Engine_EventShowMessage(args)
#define ObjectMotion_SetAngleToward_51(args...) Engine_ActorFaceActor(args)
#define BattleRuntime_WaitIfModeZero_51(args...) Engine_EventWait(args)
#define SceneWork_SetStepValue_35(args...) Engine_EventSetMessage(args)
#define ObjectMotion_SetSpeedParameters_16(a0, a1) Value2(Func_02009308, a0, a1)
#define ObjectMotion_SetAngleToward_52(args...) Engine_ActorFaceActor(args)
#define ObjectMotion_SetAngleToward_53(args...) Engine_ActorFaceActor(args)
#define ObjectMotion_SetAngleToward_54(args...) Engine_ActorFaceActor(args)
#define ObjectMotion_SetAngleToward_55(args...) Engine_ActorFaceActor(args)
#define ObjectMotion_SetHorizontalPositionWithTerrain_9(a0, a1, a2) Call3(Engine_ActorSetPosition, a0, a1, a2)
#define ObjectMotion_SetSpeedParameters_17(a0, a1, a2) Call3(Engine_ActorSetSpeed, a0, a1, a2)
#define UiWork_WaitThenFinalizeCapacity_2(args...) Engine_EventChooseYesNo(args)
#define Audio_PlayCue_3_02003054(args...) Engine_AudioPlayCue(args)
#define SceneWork_SetStepValue_36(args...) Engine_EventSetMessage(args)
#define BattleEvent_RunActionAndWait_34(args...) Engine_EventShowMessage(args)
#define Audio_PlayCue_4_02003054(args...) Engine_AudioPlayCue(args)
#define SceneWork_SetStepValue_37(args...) Engine_EventSetMessage(args)
#define BattleEvent_RunActionAndWait_35(args...) Engine_EventShowMessage(args)
#define ObjectMotion_SetAngleToward_56(args...) Engine_ActorFaceActor(args)
#define ObjectMotion_SetAngleToward_57(args...) Engine_ActorFaceActor(args)
#define ObjectMotion_SetAngleToward_58(args...) Engine_ActorFaceActor(args)
#define ObjectMotion_SetAngleToward_59(args...) Engine_ActorFaceActor(args)
#define ObjectMotion_SetAngleToward_60(args...) Engine_ActorFaceActor(args)
#define ObjectMotion_SetAngleToward_61(args...) Engine_ActorFaceActor(args)
#define ObjectMotion_PlaceWithinCameraBounds_1(a0, a1, a2, a3) Call4(Engine_CameraMoveTo, a0, a1, a2, a3)
#define ObjectMotion_SetVariantCallback_13(args...) Engine_ActorStartRepeatedMotion(args)
#define BattleRuntime_WaitIfModeZero_52(args...) Engine_EventWait(args)
#define Audio_PlayCue_5_02003054(args...) Engine_AudioPlayCue(args)
#define SceneWork_SetStepValue_38(args...) Engine_EventSetMessage(args)
#define BattleEvent_RunActionAndWait_36(args...) Engine_EventShowMessage(args)
#define ObjectMotion_ResetAndSetPositionInMode2_19(a0, a1, a2) Call3(Engine_ActorWalkTo, a0, a1, a2)
#define ObjectMotion_CommitCurrentPositionAndActivate_19(args...) Engine_ActorWaitForMove(args)
#define ObjectMotion_ResetAndSetPositionInMode2_20(a0, a1, a2) Call3(Engine_ActorWalkTo, a0, a1, a2)
#define ObjectMotion_PlaceWithinCameraBounds_2(a0, a1, a2, a3) Call4(Engine_CameraMoveTo, a0, a1, a2, a3)
#define SceneWork_SetStepValue_39(args...) Engine_EventSetMessage(args)
#define BattleEvent_RunActionAndWait_37(args...) Engine_EventShowMessage(args)
#define ObjectMotion_CommitCurrentPositionAndActivate_20(args...) Engine_ActorWaitForMove(args)
#define ObjectMotion_ArmCallback_20(a0, a1, a2) Call3(Func_0200946a_a, a0, a1, a2)
#define BattleEffect_SpawnLinkedResourceObject_21(a0, a1, a2) Call3(Engine_ActorShowEmote, a0, a1, a2)
#define Object_SetModeById_33(args...) Engine_ActorSetAnimation(args)
#define BattleRuntime_WaitIfModeZero_53(args...) Engine_EventWait(args)
#define SceneWork_SetStepValue_40(args...) Engine_EventSetMessage(args)
#define BattleEvent_RunActionAndWait_38(args...) Engine_EventShowMessage(args)
#define ObjectMotion_SetVariantCallbackAndInvokeObject_1_02003054(args...) Func_0200945a(args)
#define ObjectMotion_CallThenWaitForAnimationChange_5_02003054(args...) Func_0200944a(args)
#define SceneWork_SetStepValue_41(args...) Engine_EventSetMessage(args)
#define BattleEvent_RunActionAndWait_39(args...) Engine_EventShowMessage(args)
#define BattleEffect_SpawnLinkedResourceObject_22(a0, a1, a2) Call3(Engine_ActorShowEmote, a0, a1, a2)
#define Object_SetModeById_34(args...) Engine_ActorSetAnimation(args)
#define BattleRuntime_WaitIfModeZero_54(args...) Engine_EventWait(args)
#define ObjectMotion_ArmCallback_21(a0, a1, a2) Call3(Func_020094d6, a0, a1, a2)
#define BattleRuntime_WaitIfModeZero_55(args...) Engine_EventWait(args)
#define SceneWork_SetStepValue_42(args...) Engine_EventSetMessage(args)
#define BattleEvent_RunActionAndWait_40(args...) Engine_EventShowMessage(args)
#define ObjectMotion_SetVariantCallback_14(args...) Engine_ActorStartRepeatedMotion(args)
#define SceneWork_SetStepValue_43(args...) Engine_EventSetMessage(args)
#define BattleEvent_RunActionAndWait_41(args...) Engine_EventShowMessage(args)
#define BattleRuntime_WaitIfModeZero_56(args...) Engine_EventWait(args)
#define ObjectMotion_CallThenWaitForAnimationChange_6(args...) Func_020094b6(args)
#define ObjectMotion_SetAngleToward_62(args...) Engine_ActorFaceActor(args)
#define BattleRuntime_WaitIfModeZero_57(args...) Engine_EventWait(args)
#define SceneWork_SetStepValue_44(args...) Engine_EventSetMessage(args)
#define BattleEvent_RunActionAndWait_42(args...) Engine_EventShowMessage(args)
#define ObjectMotion_SetVariantCallback_15(args...) Engine_ActorStartRepeatedMotion(args)
#define BattleEffect_SpawnLinkedResourceObject_23(a0, a1, a2) Call3(Engine_ActorShowEmote, a0, a1, a2)
#define SceneWork_SetStepValue_45(args...) Engine_EventSetMessage(args)
#define BattleEvent_RunActionAndWait_43(args...) Engine_EventShowMessage(args)
#define BattleEffect_SpawnLinkedResourceObject_24(a0, a1, a2) Call3(Engine_ActorShowEmote, a0, a1, a2)
#define SceneWork_SetStepValue_46(args...) Engine_EventSetMessage(args)
#define BattleEvent_RunActionAndWait_44(args...) Engine_EventShowMessage(args)
#define ObjectMotion_CallThenWaitForAnimationChange_7(args...) Func_02009518(args)
#define SceneWork_SetStepValue_47(args...) Engine_EventSetMessage(args)
#define BattleEvent_RunActionAndWait_45(args...) Engine_EventShowMessage(args)
#define BattleRuntime_WaitIfModeZero_58(args...) Engine_EventWait(args)
#define BattleRuntime_WaitIfModeZero_59(a0, a1) Call2(Engine_ActorSetAttachedEffect, a0, a1)
#define BattleRuntime_WaitIfModeZero_60(args...) Engine_EventWait(args)
#define BattleRuntime_WaitIfModeZero_61(args...) Engine_ActorSetAttachedEffect(args)
#define ObjectMotion_ResetAndSetPositionInMode2_21(a0, a1, a2) Call3(Engine_ActorWalkTo, a0, a1, a2)
#define ObjectMotion_CommitCurrentPositionAndActivate_21(args...) Engine_ActorWaitForMove(args)
#define ObjectMotion_SetAngleToward_63(args...) Engine_ActorFaceActor(args)
#define BattleRuntime_WaitIfModeZero_62(args...) Engine_EventWait(args)
#define ObjectMotion_SetAngleToward_64(args...) Engine_ActorFaceActor(args)
#define SceneWork_SetStepValue_48(args...) Engine_EventSetMessage(args)
#define BattleEvent_RunActionAndWait_46(args...) Engine_EventShowMessage(args)
#define ObjectMotion_SetVariantCallback_16(args...) Engine_ActorStartRepeatedMotion(args)
#define SceneWork_SetStepValue_49(args...) Engine_EventSetMessage(args)
#define BattleEvent_RunActionAndWait_47(args...) Engine_EventShowMessage(args)
#define BattleRuntime_WaitIfModeZero_63(args...) Engine_EventWait(args)
#define ObjectMotion_CallThenWaitForAnimationChange_8(args...) Func_020095a6_b(args)
#define BattleRuntime_WaitIfModeZero_64(args...) Engine_EventWait(args)
#define SceneWork_SetStepValue_50(args...) Engine_EventSetMessage(args)
#define BattleEvent_RunActionAndWait_48(args...) Engine_EventShowMessage(args)
#define Object_SetModeById_35(args...) Engine_ActorSetAnimation(args)
#define BattleEffect_SpawnLinkedResourceObject_25(a0, a1, a2) Call3(Engine_ActorShowEmote, a0, a1, a2)
#define SceneWork_SetStepValue_51(args...) Engine_EventSetMessage(args)
#define BattleEvent_RunActionAndWait_49(args...) Engine_EventShowMessage(args)
#define BattleEffect_SpawnLinkedResourceObject_26(a0, a1, a2) Call3(Engine_ActorShowEmote, a0, a1, a2)
#define SceneWork_SetStepValue_52(args...) Engine_EventSetMessage(args)
#define BattleEvent_RunActionAndWait_50(args...) Engine_EventShowMessage(args)
#define Object_SetModeById_36(args...) Engine_ActorSetAnimation(args)
#define BattleRuntime_WaitIfModeZero_65(args...) Engine_EventWait(args)
#define Object_SetModeById_37(args...) Engine_ActorSetAnimation(args)
#define BattleRuntime_WaitIfModeZero_66(args...) Engine_EventWait(args)
#define Object_LinkPair_4_02003054(args...) Engine_ActorFaceEachOther(args)
#define Object_SetModeById_38(args...) Engine_ActorSetAnimation(args)
#define Object_SetModeById_39(args...) Engine_ActorSetAnimation(args)
#define Object_SetModeById_40(args...) Engine_ActorSetAnimation(args)
#define Object_SetModeById_41(args...) Engine_ActorSetAnimation(args)
#define BattleRuntime_WaitIfModeZero_67(args...) Engine_EventWait(args)
#define BattleEffect_SpawnLinkedResourceObject_27(a0, a1, a2) Call3(Engine_ActorShowEmote, a0, a1, a2)
#define SceneWork_SetStepValue_53(args...) Engine_EventSetMessage(args)
#define BattleEvent_RunActionAndWait_51(args...) Engine_EventShowMessage(args)
#define BattleEffect_SpawnLinkedResourceObject_28(a0, a1, a2) Call3(Engine_ActorShowEmote, a0, a1, a2)
#define SceneWork_SetStepValue_54(args...) Engine_EventSetMessage(args)
#define BattleEvent_RunActionAndWait_52(args...) Engine_EventShowMessage(args)
#define Object_LinkPair_5(args...) Engine_ActorFaceEachOther(args)
#define Object_LinkPair_6(args...) Engine_ActorFaceEachOther(args)
#define BattleRuntime_WaitIfModeZero_68(args...) Engine_EventWait(args)
#define ObjectMotion_SetAngleToward_65(args...) Engine_ActorFaceActor(args)
#define BattleRuntime_WaitIfModeZero_69(args...) Engine_EventWait(args)
#define ObjectMotion_SetAngleToward_66(args...) Engine_ActorFaceActor(args)
#define ObjectMotion_SetAngleToward_67(args...) Engine_ActorFaceActor(args)
#define ObjectMotion_SetAngleToward_68(args...) Engine_ActorFaceActor(args)
#define ObjectMotion_SetAngleToward_69(args...) Engine_ActorFaceActor(args)
#define BattleRuntime_WaitIfModeZero_70(args...) Engine_EventWait(args)
#define SceneWork_SetStepValue_55(args...) Engine_EventSetMessage(args)
#define BattleEvent_RunActionAndWait_53(args...) Engine_EventShowMessage(args)
#define BattleEffect_SpawnLinkedResourceObject_29(a0, a1, a2) Call3(Engine_ActorShowEmote, a0, a1, a2)
#define BattleEffect_SpawnLinkedResourceObject_30(a0, a1, a2) Call3(Engine_ActorShowEmote, a0, a1, a2)
#define BattleEffect_SpawnLinkedResourceObject_31(a0, a1, a2) Call3(Engine_ActorShowEmote, a0, a1, a2)
#define BattleEffect_SpawnLinkedResourceObject_32(a0, a1, a2) Call3(Engine_ActorShowEmote, a0, a1, a2)
#define BattleRuntime_WaitIfModeZero_71(args...) Engine_EventWait(args)
#define SceneWork_SetStepValue_56(args...) Engine_EventSetMessage(args)
#define BattleEvent_RunActionAndWait_54(args...) Engine_EventShowMessage(args)
#define ObjectMotion_SetVariantCallback_17(args...) Engine_ActorStartRepeatedMotion(args)
#define BattleRuntime_WaitIfModeZero_72(args...) Engine_EventWait(args)
#define SceneWork_SetStepValue_57(args...) Engine_EventSetMessage(args)
#define BattleEvent_RunActionAndWait_55(args...) Engine_EventShowMessage(args)
#define ObjectMotion_SetAngleToward_70(args...) Engine_ActorFaceActor(args)
#define BattleRuntime_WaitIfModeZero_73(args...) Engine_EventWait(args)
#define SceneWork_SetStepValue_58(args...) Engine_EventSetMessage(args)
#define BattleEvent_RunActionAndWait_56(args...) Engine_EventShowMessage(args)
#define BattleEffect_SpawnLinkedResourceObject_33(a0, a1, a2) Call3(Engine_ActorShowEmote, a0, a1, a2)
#define BattleEffect_SpawnLinkedResourceObject_34(a0, a1, a2) Call3(Engine_ActorShowEmote, a0, a1, a2)
#define BattleEffect_SpawnLinkedResourceObject_35(a0, a1, a2) Call3(Engine_ActorShowEmote, a0, a1, a2)
#define BattleEffect_SpawnLinkedResourceObject_36(a0, a1, a2) Call3(Engine_ActorShowEmote, a0, a1, a2)
#define BattleRuntime_WaitIfModeZero_74(args...) Engine_EventWait(args)
#define Object_SetModeById_42(args...) Engine_ActorSetAnimation(args)
#define BattleRuntime_WaitIfModeZero_75(args...) Engine_EventWait(args)
#define SceneWork_SetStepValue_59(args...) Engine_EventSetMessage(args)
#define BattleEvent_RunActionAndWait_57(args...) Engine_EventShowMessage(args)
#define BattleEffect_SpawnLinkedResourceObject_37(a0, a1, a2) Call3(Engine_ActorShowEmote, a0, a1, a2)
#define SceneWork_SetStepValue_60(args...) Engine_EventSetMessage(args)
#define BattleEvent_RunActionAndWait_58(args...) Engine_EventShowMessage(args)
#define Object_SetModeById_43(args...) Engine_ActorSetAnimation(args)
#define Object_SetModeById_44(args...) Engine_ActorSetAnimation(args)
#define Object_SetModeById_45(args...) Engine_ActorSetAnimation(args)
#define Object_SetModeById_46(args...) Engine_ActorSetAnimation(args)
#define Object_SetModeById_47(args...) Engine_ActorSetAnimation(args)
#define BattleRuntime_WaitIfModeZero_76(args...) Engine_EventWait(args)
#define Object_SetModeById_48(args...) Engine_ActorSetAnimation(args)
#define BattleRuntime_WaitIfModeZero_77(args...) Engine_EventWait(args)
#define SceneWork_SetStepValue_61(args...) Engine_EventSetMessage(args)
#define BattleEvent_RunActionAndWait_59(args...) Engine_EventShowMessage(args)
#define BattleEffect_SpawnLinkedResourceObject_38(a0, a1, a2) Call3(Engine_ActorShowEmote, a0, a1, a2)
#define SceneWork_SetStepValue_62(args...) Engine_EventSetMessage(args)
#define BattleEvent_RunActionAndWait_60(args...) Engine_EventShowMessage(args)
#define ObjectMotion_SetAngleToward_71(args...) Engine_ActorFaceActor(args)
#define BattleRuntime_WaitIfModeZero_78(args...) Engine_EventWait(args)
#define ObjectMotion_SetVariantCallback_18(args...) Engine_ActorStartRepeatedMotion(args)
#define BattleRuntime_WaitIfModeZero_79(args...) Engine_EventWait(args)
#define SceneWork_SetStepValue_63(args...) Engine_EventSetMessage(args)
#define BattleEvent_RunActionAndWait_61(args...) Engine_EventShowMessage(args)
#define Object_SetModeById_49(args...) Engine_ActorSetAnimation(args)
#define BattleRuntime_WaitIfModeZero_80(args...) Engine_EventWait(args)
#define SceneWork_SetStepValue_64(args...) Engine_EventSetMessage(args)
#define BattleEvent_RunActionAndWait_62(args...) Engine_EventShowMessage(args)
#define Object_SetModeById_50(args...) Engine_ActorSetAnimation(args)
#define SceneWork_SetStepValue_65(args...) Engine_EventSetMessage(args)
#define BattleEvent_RunActionAndWait_63(args...) Engine_EventShowMessage(args)
#define ObjectMotion_SetAngleToward_72(args...) Engine_ActorFaceActor(args)
#define ObjectMotion_CallThenWaitForAnimationChange_9(args...) Func_020098b6_b(args)
#define BattleRuntime_WaitIfModeZero_81(args...) Engine_EventWait(args)
#define SceneWork_SetStepValue_66(args...) Engine_EventSetMessage(args)
#define BattleEvent_RunActionAndWait_64(args...) Engine_EventShowMessage(args)
#define BattleEffect_SpawnLinkedResourceObject_39(a0, a1, a2) Call3(Engine_ActorShowEmote, a0, a1, a2)
#define SceneWork_SetStepValue_67(args...) Engine_EventSetMessage(args)
#define BattleEvent_RunActionAndWait_65(args...) Engine_EventShowMessage(args)
#define ObjectMotion_ArmCallback_22(a0, a1, a2) Call3(Func_0200994a, a0, a1, a2)
#define BattleRuntime_WaitIfModeZero_82(args...) Engine_EventWait(args)
#define SceneWork_SetStepValue_68(args...) Engine_EventSetMessage(args)
#define BattleEvent_RunActionAndWait_66(args...) Engine_EventShowMessage(args)
#define ObjectMotion_SetVariantCallback_19(args...) Engine_ActorStartRepeatedMotion(args)
#define BattleRuntime_WaitIfModeZero_83(args...) Engine_EventWait(args)
#define SceneWork_SetStepValue_69(args...) Engine_EventSetMessage(args)
#define BattleEvent_RunActionAndWait_67(args...) Engine_EventShowMessage(args)
#define ObjectMotion_SetAngleToward_73(args...) Engine_ActorFaceActor(args)
#define BattleRuntime_WaitIfModeZero_84(args...) Engine_EventWait(args)
#define ObjectMotion_CallThenWaitForAnimationChange_10(args...) Func_0200993e_b(args)
#define BattleRuntime_WaitIfModeZero_85(args...) Engine_EventWait(args)
#define SceneWork_SetStepValue_70(args...) Engine_EventSetMessage(args)
#define BattleEvent_RunActionAndWait_68(args...) Engine_EventShowMessage(args)
#define BattleEffect_SpawnLinkedResourceObject_40(a0, a1, a2) Call3(Engine_ActorShowEmote, a0, a1, a2)
#define SceneWork_SetStepValue_71(args...) Engine_EventSetMessage(args)
#define BattleEvent_RunActionAndWait_69(args...) Engine_EventShowMessage(args)
#define ObjectMotion_SetAngleToward_74(args...) Engine_ActorFaceActor(args)
#define BattleRuntime_WaitIfModeZero_86(args...) Engine_EventWait(args)
#define SceneWork_SetStepValue_72(args...) Engine_EventSetMessage(args)
#define BattleEvent_RunActionAndWait_70(args...) Engine_EventShowMessage(args)
#define BattleEffect_SpawnLinkedResourceObject_41(a0, a1, a2) Call3(Engine_ActorShowEmote, a0, a1, a2)
#define SceneWork_SetStepValue_73(args...) Engine_EventSetMessage(args)
#define BattleEvent_RunActionAndWait_71(args...) Engine_EventShowMessage(args)
#define ObjectMotion_CallThenWaitForAnimationChange_11(args...) Func_020099b4(args)
#define SceneWork_SetStepValue_74(args...) Engine_EventSetMessage(args)
#define BattleEvent_RunActionAndWait_72(args...) Engine_EventShowMessage(args)
#define ObjectMotion_SetAngleToward_75(args...) Engine_ActorFaceActor(args)
#define BattleRuntime_WaitIfModeZero_87(args...) Engine_EventWait(args)
#define Object_SetModeById_51(args...) Engine_ActorSetAnimation(args)
#define BattleRuntime_WaitIfModeZero_88(args...) Engine_EventWait(args)
#define SceneWork_SetStepValue_75(args...) Engine_EventSetMessage(args)
#define ObjectMotion_SetSpeedParameters_18(a0, a1) Value2(Func_02009a32, a0, a1)
#define UiWork_WaitThenFinalizeCapacity_3(args...) Engine_EventChooseYesNo(args)
#define SceneWork_SetStepValue_76(args...) Engine_EventSetMessage(args)
#define BattleEvent_RunActionAndWait_73(args...) Engine_EventShowMessage(args)
#define BattleRuntime_WaitIfModeZero_89(args...) Engine_EventWait(args)
#define Object_SetModeById_52(args...) Engine_ActorSetAnimation(args)
#define Object_SetModeById_53(args...) Engine_ActorSetAnimation(args)
#define Object_SetModeById_54(args...) Engine_ActorSetAnimation(args)
#define BattleRuntime_WaitIfModeZero_90(args...) Engine_EventWait(args)
#define Object_LinkPair_7(args...) Engine_ActorFaceEachOther(args)
#define Object_SetModeById_55(args...) Engine_ActorSetAnimation(args)
#define Object_SetModeById_56(args...) Engine_ActorSetAnimation(args)
#define BattleRuntime_WaitIfModeZero_91(args...) Engine_EventWait(args)
#define Object_SetModeById_57(args...) Engine_ActorSetAnimation(args)
#define Scene_GetRecord_2_02003054(args...) Func_02009a00(args)
#define ObjectMotion_ResetAndSetPosition_1(args...) Engine_ActorSetDestination(args)
#define ObjectMotion_CommitCurrentPositionAndActivate_22(args...) Func_02009a60_a(args)
#define ObjectMotion_SetHorizontalPositionWithTerrain_10(args...) Engine_ActorSetPosition(args)
#define Object_SetModeById_58(args...) Engine_ActorSetAnimation(args)
#define Scene_GetRecord_3(args...) Func_02009a30(args)
#define ObjectMotion_ResetAndSetPosition_2(args...) Engine_ActorSetDestination(args)
#define ObjectMotion_CommitCurrentPositionAndActivate_23(args...) Engine_ActorWaitForMove(args)
#define ObjectMotion_SetHorizontalPositionWithTerrain_11(args...) Engine_ActorSetPosition(args)
#define Object_SetModeById_59(args...) Engine_ActorSetAnimation(args)
#define Scene_GetRecord_4(args...) Func_02009a60_b(args)
#define ObjectMotion_ResetAndSetPosition_3(args...) Engine_ActorSetDestination(args)
#define ObjectMotion_CommitCurrentPositionAndActivate_24(args...) Engine_ActorWaitForMove(args)
#define ObjectMotion_SetHorizontalPositionWithTerrain_12(args...) Engine_ActorSetPosition(args)
#define BattleRuntime_WaitIfModeZero_92(args...) Func_02009a60_c(args)
#define ObjectMotion_SetSpeedParameters_19(a0, a1, a2) Call3(Engine_ActorSetSpeed, a0, a1, a2)
#define ObjectMotion_ResetAndSetPositionInMode2_22(a0, a1, a2) Call3(Engine_ActorWalkTo, a0, a1, a2)
#define ObjectMotion_CommitCurrentPositionAndActivate_25(args...) Engine_ActorWaitForMove(args)
#define Object_SetModeById_60(args...) Engine_ActorSetAnimation(args)
#define ObjectMotion_ArmCallback_23(a0, a1, a2) Call3(Func_02009b70_a, a0, a1, a2)
#define ObjectMotion_SetAngleToward_76(args...) Engine_ActorFaceActor(args)
#define Object_SetModeById_61(args...) Engine_ActorSetAnimation(args)
#define BattleRuntime_WaitIfModeZero_93(args...) Engine_EventWait(args)
#define ObjectMotion_SetAngleToward_77(args...) Engine_ActorFaceActor(args)
#define BattleRuntime_WaitIfModeZero_94(args...) Engine_EventWait(args)
#define SceneWork_SetStepValue_77(args...) Engine_EventSetMessage(args)
#define BattleEvent_RunActionAndWait_74(args...) Engine_EventShowMessage(args)
#define ObjectMotion_SetVariantCallbackAndInvokeObject_2(args...) Func_02009b70_b(args)
#define SceneWork_SetStepValue_78(args...) Engine_EventSetMessage(args)
#define BattleEvent_RunActionAndWait_75(args...) Engine_EventShowMessage(args)
#define Object_SetModeById_62(args...) Engine_ActorSetAnimation(args)
#define BattleRuntime_WaitIfModeZero_95(args...) Engine_EventWait(args)
#define ObjectMotion_SetSpeedParameters_20(a0, a1, a2) Call3(Engine_ActorSetSpeed, a0, a1, a2)
#define ObjectMotion_SetSpeedParameters_21(a0, a1, a2) Call3(Engine_ActorSetSpeed, a0, a1, a2)
#define ObjectMotion_SetPositionAndReset_1_02003054(args...) Engine_CameraFollowActor(args)
#define ObjectMotion_ResetAndSetPositionInMode2_23(a0, a1, a2) Call3(Engine_ActorWalkTo, a0, a1, a2)
#define BattleRuntime_WaitIfModeZero_96(args...) Engine_EventWait(args)
#define ObjectMotion_SetActionVariant_2(args...) Engine_ActorSetSpritePriority(args)
#define ObjectMotion_SetActionVariant_3(args...) Engine_ActorSetSpritePriority(args)
#define ObjectMotion_ResetAndSetPositionInMode2_24(a0, a1, a2) Call3(Engine_ActorWalkTo, a0, a1, a2)
#define ObjectMotion_CommitCurrentPositionAndActivate_26(args...) Engine_ActorWaitForMove(args)
#define ObjectMotion_SetAngleToward_78(args...) Engine_ActorFaceActor(args)
#define ObjectMotion_CommitCurrentPositionAndActivate_27(args...) Engine_ActorWaitForMove(args)
#define ObjectMotion_SetAngleToward_79(args...) Engine_ActorFaceActor(args)
#define ObjectMotion_ArmCallback_24(a0, a1, a2) Call3(Func_02009c4c_a, a0, a1, a2)
#define Object_SetModeById_63(args...) Engine_ActorSetAnimation(args)
#define BattleRuntime_WaitIfModeZero_97(args...) Engine_EventWait(args)
#define Object_SetModeById_64(args...) Engine_ActorSetAnimation(args)
#define BattleRuntime_WaitIfModeZero_98(args...) Engine_EventWait(args)
#define Object_SetModeById_65(args...) Engine_ActorSetAnimation(args)
#define BattleRuntime_WaitIfModeZero_99(args...) Engine_EventWait(args)
#define ObjectMotion_ResetAndSetPositionInMode2_25(a0, a1, a2) Call3(Engine_ActorWalkTo, a0, a1, a2)
#define ObjectMotion_ResetAndSetPositionInMode2_26(a0, a1, a2) Call3(Engine_ActorWalkTo, a0, a1, a2)
#define ObjectMotion_CommitCurrentPositionAndActivate_28(args...) Engine_ActorWaitForMove(args)
#define ObjectMotion_ResetAndSetPositionInMode2_27(a0, a1, a2) Call3(Engine_ActorWalkTo, a0, a1, a2)
#define ObjectMotion_CommitCurrentPositionAndActivate_29(args...) Engine_ActorWaitForMove(args)
#define ObjectMotion_ResetAndSetPositionInMode2_28(a0, a1, a2) Call3(Engine_ActorWalkTo, a0, a1, a2)
#define ObjectMotion_CommitCurrentPositionAndActivate_30(args...) Engine_ActorWaitForMove(args)
#define ObjectMotion_ResetAndSetPositionInMode2_29(a0, a1, a2) Call3(Engine_ActorWalkTo, a0, a1, a2)
#define ObjectMotion_CommitCurrentPositionAndActivate_31(args...) Engine_ActorWaitForMove(args)
#define ObjectMotion_ResetAndSetPositionInMode2_30(a0, a1, a2) Call3(Engine_ActorWalkTo, a0, a1, a2)
#define ObjectMotion_CommitCurrentPositionAndActivate_32(args...) Engine_ActorWaitForMove(args)
#define ObjectMotion_ResetAndSetPositionInMode2_31(a0, a1, a2) Call3(Engine_ActorWalkTo, a0, a1, a2)
#define BattleRuntime_WaitIfModeZero_100(args...) Engine_EventWait(args)
#define Audio_PlayCue_6_02003054(args...) Engine_AudioPlayCue(args)
#define BattleRuntime_WaitIfModeZero_101(args...) Engine_EventWait(args)
#define BattleRuntime_WaitIfModeZero_102(args...) Engine_EventWait(args)
#define BattleRuntime_ScheduleShoulderButtonModeUpdate_1_02003054(args...) Engine_EventEnd(args)
#define SCENE_PHASE_02003054 (*(s32 *)(*(u8 **)0x03001ebc + 0x1c0))
#define PRIMARY_ID 24
#define DERIVED_ID 25
#define GameFlag_IsSet_1_020049a0(a0) Value1(Func_02009fb6, a0)
#define GameFlag_IsSet_2_020049a0(a0) Value1(Func_02009fc2, a0)
#define GameFlag_IsSet_3(a0) Value1(Func_02009fd0_a, a0)
#define SceneWork_SetStepValue_1_020049a0(a0) Call1(Engine_EventSetMessage, a0)
#define BattleEvent_RunActionAndWait_1_020049a0(a0, a1) Call2(Engine_EventShowMessage, a0, a1)
#define ObjectMotion_SetVariantCallbackAndInvokeObject_1_020049a0(a0, a1) Value2(Func_0200a0a6, a0, a1)
#define BattleRuntime_WaitIfModeZero_1_020049a0(a0) Value1(Engine_EventWait, a0)
#define ObjectMotion_SetSpeedParameters_1_020049a0(a0, a1, a2) Call3(Engine_ActorSetSpeed, a0, a1, a2)
#define ObjectMotion_OffsetPositionAndResetMotion_1_020049a0(a0, a1, a2) Value3(Engine_ActorSetDestinationOffset, a0, a1, a2)
#define ObjectMotion_CommitCurrentPositionAndActivate_1_020049a0(a0) Value1(Engine_ActorWaitForMove, a0)
#define Object_SetModeById_1_020049a0(a0, a1) Value2(Engine_ActorSetAnimation, a0, a1)
#define BattleRuntime_WaitIfModeZero_2_020049a0(a0) Value1(Engine_EventWait, a0)
#define ObjectMotion_SetSpeedParameters_2_020049a0(a0, a1, a2) Call3(Engine_ActorSetSpeed, a0, a1, a2)
#define ObjectMotion_OffsetPositionAndResetMotion_2_020049a0(a0, a1, a2) Call3(Engine_ActorSetDestinationOffset, a0, a1, a2)
#define ObjectMotion_SetAngleToward_1_020049a0(a0, a1, a2) Value3(Engine_ActorFaceActor, a0, a1, a2)
#define ObjectMotion_CommitCurrentPositionAndActivate_2_020049a0(a0) Value1(Engine_ActorWaitForMove, a0)
#define SceneWork_SetStepValue_2_020049a0(a0) Value1(Engine_EventSetMessage, a0)
#define BattleEvent_RunActionAndWait_2_020049a0(a0, a1) Call2(Engine_EventShowMessage, a0, a1)
#define ObjectMotion_SetVariantCallbackAndInvokeObject_2_020049a0(a0, a1) Call2(Func_0200a112, a0, a1)
#define ObjectMotion_SetAngleToward_2_020049a0(a0, a1, a2) Value3(Engine_ActorFaceActor, a0, a1, a2)
#define SceneWork_SetStepValue_3_020049a0(a0) Value1(Engine_EventSetMessage, a0)
#define BattleEvent_RunActionAndWait_3_020049a0(a0, a1) Value2(Engine_EventShowMessage, a0, a1)
#define BattleRuntime_WaitIfModeZero_3_020049a0(a0) Value1(Engine_EventWait, a0)
#define Object_SetModeById_2_020049a0(a0, a1) Value2(Engine_ActorSetAnimation, a0, a1)
#define BattleRuntime_WaitIfModeZero_4_020049a0(a0) Value1(Engine_EventWait, a0)
#define ObjectMotion_SetSpeedParameters_3_020049a0(a0, a1, a2) Call3(Engine_ActorSetSpeed, a0, a1, a2)
#define ObjectMotion_ResetAndSetPositionInMode2_1_020049a0(a0, a1, a2) Value3(Engine_ActorWalkTo, a0, a1, a2)
#define ObjectMotion_CommitCurrentPositionAndActivate_3_020049a0(a0) Value1(Engine_ActorWaitForMove, a0)
#define ObjectMotion_ArmCallback_1_020049a0(a0, a1, a2) Value3(Func_0200a1a6, a0, a1, a2)
#define SceneWork_SetStepValue_4_020049a0(a0) Value1(Engine_EventSetMessage, a0)
#define BattleEvent_RunActionAndWait_4_020049a0(a0, a1) Call2(Engine_EventShowMessage, a0, a1)
#define Object_SetModeById_3_020049a0(a0, a1) Value2(Engine_ActorSetAnimation, a0, a1)
#define BattleRuntime_WaitIfModeZero_5_020049a0(a0) Value1(Engine_EventWait, a0)
#define ObjectMotion_OffsetPositionAndResetMotion_3_020049a0(a0, a1, a2) Value3(Engine_ActorSetDestinationOffset, a0, a1, a2)
#define ObjectMotion_CommitCurrentPositionAndActivate_4_020049a0(a0) Value1(Engine_ActorWaitForMove, a0)
#define Object_SetModeById_4_020049a0(a0, a1) Value2(Engine_ActorSetAnimation, a0, a1)
#define SceneWork_SetStepValue_5_020049a0(a0) Value1(Engine_EventSetMessage, a0)
#define BattleEvent_RunActionAndWait_5_020049a0(a0, a1) Call2(Engine_EventShowMessage, a0, a1)
#define ObjectMotion_ResetAndSetPositionInMode2_2_020049a0(a0, a1, a2) Value3(Engine_ActorWalkTo, a0, a1, a2)
#define ObjectMotion_CommitCurrentPositionAndActivate_5_020049a0(a0) Value1(Engine_ActorWaitForMove, a0)
#define Object_LinkPair_1_020049a0(a0, a1, a2) Value3(Engine_ActorFaceEachOther, a0, a1, a2)
#define BattleRuntime_WaitIfModeZero_6_020049a0(a0) Value1(Engine_EventWait, a0)
#define Object_SetModeById_5_020049a0(a0, a1) Call2(Engine_ActorSetAnimation, a0, a1)
#define BattleRuntime_WaitIfModeZero_7_020049a0(a0) Value1(Engine_EventWait, a0)
#define GameFlag_Set_1_020049a0(a0) Value1(Engine_GameFlagSet, a0)
#define SceneWork_SetStepValue_6_020049a0(a0) Call1(Engine_EventSetMessage, a0)
#define BattleEvent_RunActionAndWait_6_020049a0(a0, a1) Call2(Engine_EventShowMessage, a0, a1)
#define SceneWork_SetStepValue_7_020049a0(a0) Call1(Engine_EventSetMessage, a0)
#define BattleEvent_RunActionAndWait_7_020049a0(a0, a1) Call2(Engine_EventShowMessage, a0, a1)

#include "STAGED_ACTOR.H"
#include "OBJECT_RUNTIME.H"


typedef struct SceneActor {
    u8 pad0[8];
    s32 x;          /* 0x08 */
    s32 y;          /* 0x0c */
    s32 z;          /* 0x10 */
} SceneActor;

typedef struct DirectionalSceneActor {
    u8 pad0[6];
    u16 dir;        /* 0x06 */
    s32 x;          /* 0x08 */
    s32 y;          /* 0x0c */
    s32 z;          /* 0x10 */
} DirectionalSceneActor;

extern s32 Value_00000f2e;
extern u8 Value_000000a0;
extern u8 Value_000000a1;
extern u8 Value_000000a2;
extern u8 Data_0200e2a4[];
extern u8 Data_0200e4b4[];
extern u8 Data_0200e754[];
extern u8 Data_0200e814[];
extern u8 Value_000000a3;
extern u8 Data_0200e910[];
extern u8 Data_0200e97c[];
extern u8 Data_0200e8a4[];
extern u8 Value_0000006a;
extern u8 Data_0200e9d0[];
extern u8 Data_0200ee08[];
extern u8 Data_0200ec28[];
extern u8 Data_0200eac0[];
extern u8 Data_0200ee98[];
extern u8 Data_0200e9b8[];
extern u8 Data_0200eff4[];
extern u8 Data_0200f258[];
extern u8 Data_0200f528[];
extern u8 Data_0200f63c[];
extern s32 Value_00000953;
extern s32 Value_00000f2a;
extern s32 Value_00000f2b;
extern s32 Value_00000f2c;
extern s32 Value_00000f2d;
extern u8 Value_00002440;
extern u8 Value_00002459;
extern u8 Value_0000256c;
extern s32 Data_0200f714[];
extern s32 Data_0200f754[];
extern s32 Data_0200f764[];
extern u8 Data_00002411[];
extern u32 Data_03001e40;
extern u8 Value_00002092;
extern u8 Value_0000240d;
extern u8 Value_0000241e;
extern u8 Value_00002428;
extern u8 Value_0000247d;
extern u8 Value_00002464;
extern u8 Value_0000247e;
extern u8 Data_0000242e[];
extern u8 Data_00002430[];
extern u8 Data_00002424[];
extern u8 Data_000000a1[];
extern u8 Data_00002438[];
extern u8 Data_0000240d[];
extern u8 Data_00002481[];
extern u8 Data_0000248e[];
extern u8 Data_000024a6[];
extern u8 Data_0000244f[];
extern u8 Data_00002455[];
extern u8 Data_02009151[], Data_0200938d[], Data_02009a45[];
extern u8 Data_02009719[], Data_0200975d[], Data_020097bd[];
extern u8 Data_02009529[], Data_020099e9[], Data_0200969d[];
void Func_020067d0(s32, s32, s32);
void Func_0200386a(s32);
void Func_020038ca(s32);
void Func_0200391e(s32);
void Func_02003996(s32);
void Func_020038e2(s32);
s32 Func_02009f3e(s32);
s32 Func_02009f7a(s32);
void Func_020017d6();
s32 Func_020065a2(s32);
s32 Func_02002008(s32, s32);
s32 Func_02002024(s32, s32);
s32 Func_02002040(s32, s32);
s32 Func_0200205c(s32, s32);
void Func_02002fe6(s32);
void Func_0200317a(s32);
s32 Func_0200a322(s32);
void Func_02009936(void);
s32 Func_0200ab38(s32);
void Func_020061c2(s32);
s32 Func_0200ab48(s32);
void Func_020061d2(s32);
s32 Func_0200ab58(s32);
void Func_020061e2(s32);
s32 Func_0200ab68(s32);
void Func_020061f2(s32);
s32 Func_0200ab7a(s32);
void Func_02006204(s32);
void Func_02002e3c(s32);
void Func_0200a30a(s32, s32, s32);
void Func_0200a254(s32, s32);
void Func_0200a274(s32, s32);
void Func_0200a294(s32, s32);
s32 Func_0200a36e(s32);
s32 Func_020067d2(s32, s32, s32);
void Func_02006636(s32);
SceneActor *Func_0200675e(s32);
SceneActor *Func_0200697a(s32);
SceneActor *Func_02006982(s32);
SceneActor *Func_02006b22(s32);
SceneActor *Func_02006cb6(s32);
SceneActor *Func_02006e70(void);
SceneActor *Func_02006e78(s32);
SceneActor *Func_02006ed4(void);
SceneActor *Func_02006edc(s32);
s32 Func_0200326a(void);
s32 Func_02003114(s32);
s32 Func_02003186(s32);
SceneActor *Func_02006f6c(void);
SceneActor *Func_02006f74(s32);
SceneActor *Func_02006fd4(void);
SceneActor *Func_020070da(void);
SceneActor *Func_0200703e(s32);
s32 Func_02007006(s32);
void Func_02002960(void);
SceneActor *Func_0200709a(s32);
s32 Func_02007064(s32);
void Func_02002920(void);
u8 *Func_020061ca(s32);
void Func_0200616e(u8 *, s32);
void Func_02000c68(void);
void Func_02001728();
void Func_02006240();
s32 Func_02006244();
s32 Func_0200629c();
void Func_020019a4();
s32 Func_020062f2();
s32 Func_02006316();
void Func_02001b44();
s32 Func_020063de();
void Func_02001e34();
s32 Func_02006542();
s32 Func_02006492();
void Func_02001cd4();
void Func_02009da6();
void Func_02009db8();
void Func_02009dca();
void Func_02009e22();
void Func_02009e34_a();
void Func_02009e46();
void Func_02009e76();
struct ObjectRuntime *Func_020067ac();
struct ObjectRuntime *Func_020067b4();
s32 Func_020067a6();
s32 Func_020067de();
double Func_02006ce6(s32);
double Func_02006c7a(double, double);
s32 Func_02006d72(double);
s32 Func_02002314();
s32 Func_02002b3e();
s32 Func_02002b72();
struct ObjectRuntime *Func_020069e6();
void Func_02006afa();
s32 Func_020069dc();
s32 Func_02006a0e();
s32 Func_02002756();
s32 Func_02002dbc();
s32 Func_02002d5a();
s32 Func_02002d58();
struct ObjectRuntime *Func_02006b82();
s32 Func_02006b74();
s32 Func_02006ba6();
s32 Func_02002a96();
s32 Func_02002eec();
s32 Func_02002eea();
s32 Func_02002d10();
s32 Func_02002fae();
s32 Func_02006cb8();
s32 Func_02006cf4_b();
void Func_02003232(s32);
s32 Func_02003006(s32);
void Func_02006cd4(s32);
s32 Func_02003054_scene_scripts();
void Func_02006d28();
void Func_02006d2e();
s32 Func_02006d76();
s32 Func_020030b4();
void Func_02006d88();
void Func_02006d8e();
s32 Func_02006dd6();
void Func_02007126(s32, s32);
void Func_0200713c_b();
void Func_020071ac(s32, s32);
void Func_020071d8(s32, s32);
void Func_020071ee_b();
void Func_0200731c();
void Func_02007338();
void Func_02007386_a(s32, s32);
void Func_02007386_b(s32, s32);
void Func_0200742a(s32, s32);
void Func_02007458_a(s32, s32);
void Func_02007458_b(s32, s32);
void Func_020077aa(s32, s32);
void Func_02004690();
s32 Func_020079b4();
void Func_02004d2a();
void Func_02004f02();
void Func_02004f66();
void Func_02004ff4();
void Func_02005054();
void Func_02005596();
void Func_02005690();
void Func_02005708();
void Func_020057ec();
s32 Func_02007dc6();
s32 Func_02007de0();
void Func_02007e14();
s32 Func_02007e76();
void Func_02007e78();
void Func_02007ed8();
void Func_02007f1e();
void Func_02007f68();
void Func_02007f74();
void Func_02007f80();
void Func_02007f8c();
void Func_02007fd6();
void Func_02007fec();
s32 Func_0200804e();
void Func_020080b0();
void Func_020080b4_b();
void Func_020080e6_a();
void Func_020080e6_b();
void Func_020080f6();
void Func_0200814e();
void Func_020081e4();
void Func_020081f8();
void Func_0200820a();
void Func_02008218();
void Func_02008226_a();
void Func_0200832a();
void Func_020084f4_a();
void Func_02008522_a();
void Func_02008524();
void Func_02008660();
void Func_02008788_a();
void Func_02008788_b();
s32 Func_0200a212();
s32 Func_0200a23a();
void Func_0200a2ac();
void Func_0200a2f0();
s32 Func_0200a3a2();
void Func_0200a3bc_b();
void Func_0200a3d2();
void Func_020073f0_a();
void Func_020073f0_b();
void Func_020039c8(s32);
void Func_02007564(s32, s32);
void Func_02003a30(s32);
void Func_020075cc(s32, s32);
void Func_02003cd0(s32);
void Func_0200786c_a(s32, s32);
void Func_020045d8();
s32 Func_02007930();
void Func_020047c2(void);
void Func_02007600();
void Func_02007636();
void Func_0200763a();
void Func_0200763c();
void Func_02007642();
void Func_02007650();
void Func_02007694();
void Func_02007778();
void Func_02007782();
void Func_0200783a();
s32 Func_02007b82();
s32 Func_02007b96();
s32 Func_02007baa();
void Func_02007c7a();
s32 Func_02007da0();
void Func_02007e80();
void Func_02007e92();
void Func_02007ea4();
void Func_02007eb0();
void Func_02003d0c();
void Func_02008674_b();
u8 *Func_020086d6();
void Func_020087ae();
void Func_020087c8();
void Func_020087e2();
void Func_020087fc();
void Func_02008816();
void Func_02008830();
void Func_02008868();
s32 Func_02008a2a();
void Func_02008a9c();
void Func_02008c14();
void Func_02008c30();
void Func_02008c48_a();
void Func_02008c60();
void Func_02008c7e_a();
void Func_02008d70();
void Func_02008d7c();
void Func_02008d88();
void Func_02008d94_a();
void Func_02008e0e();
void Func_02008e5a();
void Func_02008e9e();
void Func_02008eb8();
void Func_02008f50();
void Func_02009258();
s32 Func_02009308();
void Func_0200944a();
void Func_0200945a();
void Func_0200946a_a();
void Func_020094b6();
void Func_020094d6();
void Func_02009518();
void Func_020095a6_b();
void Func_020098b6_b();
void Func_0200993e_b();
void Func_0200994a();
void Func_020099b4();
u8 *Func_02009a00();
u8 *Func_02009a30();
s32 Func_02009a32();
void Func_02009b70_a();
void Func_02009b70_b();
void Func_02009c4c_a();
u8 *Func_02009a60_a();
u8 *Func_02009a60_b();
u8 *Func_02009a60_c();
s32 Func_02009dae();
s32 Func_02009dcc();
s32 Func_02009ebe();
s32 Func_02009f6e();
void Func_02009f88_a();
void Func_02009f98_a();
void Func_02009f98_b();
void Func_02009f98_c();
s32 Func_02009fb8();
s32 Func_02009fce();
void Func_02009fe6();
s32 Func_02009fb6();
s32 Func_02009fc2();
s32 Func_02009fd0_a();
void Func_0200a0a6();
void Func_0200a112();
void Func_0200a1a6();
void Func_0200a812();
s32 Func_0200a94c();
void Func_02005fbc();
s32 Func_0200a968();
s32 Func_0200a982();
void Func_02007658();
s32 Func_0200a99a();
s32 Func_0200a9b6();
void Func_0200761a();
s32 Func_0200a9c4();
void Func_0200765c();
struct ObjectRuntime *Func_0200aa16();
struct ObjectRuntime *Func_0200aa2a();
struct ObjectRuntime *Village_GetActor();
struct ObjectRuntime *Func_0200aa52();
void Func_0200a9f6();
struct ObjectRuntime *Func_0200aa6c();
struct ObjectRuntime *Func_0200aa84();
void Func_0200aa22();
void Village_FinishActorRestore();
s32 Func_0200aa86(s32);
void Func_02006360(s32);
s32 Func_0200aa96(s32);
void Func_02006370(s32);
s32 Func_0200aaa8(s32);
void Func_02006382(s32);
s32 Func_0200aac8(s32);
void Func_020062f2_a(s32);
s32 Func_0200aad8(s32);
void Func_02006302(s32);
s32 Func_0200aaf2(s32);
void Func_02006240_a(s32);
s32 Func_0200ab02(s32);
void Func_02006250(s32);
s32 Func_0200ab12(s32);
void Func_02006260(s32);
void Func_0200a598();
s32 Func_0200a716();
s32 Func_0200a72a();
s32 Func_0200a73e();
s32 Func_0200a752();
void Func_0200a7f8();
void Func_0200a816();
struct ObjectRuntime *Func_0200a874();
void Func_0200a812_setup();
s32 Func_0200a842();
void Func_0200a854();
void Func_0200a7f6();
s32 Func_0200a89c();
void Func_02007300();
void Func_02007314();
void Func_0200a844();
void Func_0200a8c4();
struct ObjectRuntime *Func_0200a922();
struct ObjectRuntime *Func_0200a92a();
void Func_0200a8c8();
void Func_0200a3d6();
s32 Func_0200a572();
s32 Func_0200a57c();
s32 Func_0200a586();
s32 Func_0200a5b2();
s32 Func_0200a5bc();
void Func_0200a650();
s32 Func_0200a602();
void Func_0200a682();

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

/* No scene data is available for this slot. */

/* External Func aliases name loader-relocated call words, not runtime addresses. */

/* Loader-relocated overlay calls: each symbol names the pre-relocation call
 * word the image holds. */

/* Resolved engine calls: each pseudo symbol is the per-site call word the
 * overlay image holds (a word can serve two sites with different targets),
 * and the macro names the engine function the site reaches through the
 * overlay veneer and the main-image veneer island, keeping the site's own
 * calling form. Names without a repository binding are provisional.
 */

/* Audited retained supplemental scene body.
 * The complete production span preserves 49 calls, 0 loop(s), and 0 explicit memory operation(s).
 * Approved GCC changes scheduling, allocation, control-flow lowering, and
 * literal placement, so the reviewed assembly remains authoritative. */

/* Two id arguments recur across most of the calls below: 24 is passed as the
 * first argument throughout the main sequence, and 25 is introduced only
 * after ObjectMotion_SetAngleToward_2_020049a0(25, 24, 0), whose second argument is 24 -- so 25 is
 * derived from 24 partway through the sequence. */

static __inline__ void PlaceActor(s32 actor, s32 x, s32 y)
{
    extern s16 Data_02000240[];
    extern u8 *Data_03001ebc;

    Engine_ActorSetPosition(actor, x, y);
}

static __inline__ void Call1(void (*f)(), s32 a0)
{
    extern s16 Data_02000240[];
    extern u32 Data_0200dfa4;
    extern u8 Data_03001e70[];
    extern u8 Data_03001ebc[];

    f(a0);
}

static __inline__ void Call3(void (*f)(), s32 a0, s32 a1, s32 a2)
{
    extern s16 Data_02000240[];
    extern u32 Data_0200dfa4;
    extern u8 Data_03001e70[];
    extern u8 Data_03001ebc[];

    f(a0, a1, a2);
}

static __inline__ s32 Value1(s32 (*f)(), s32 a0)
{
    extern s16 Data_02000240[];
    extern u32 Data_0200dfa4;
    extern u8 Data_03001e70[];
    extern u8 Data_03001ebc[];

    return f(a0);
}

static __inline__ void Call6(void (*f)(), s32 a0, s32 a1, s32 a2, s32 a3, s32 a4, s32 a5)
{
    extern s16 Data_02000240[];
    extern u32 Data_0200dfa4;
    extern u8 Data_03001e70[];
    extern u8 Data_03001ebc[];

    f(a0, a1, a2, a3, a4, a5);
}

static __inline__ void SetSceneValue(s16 *field, s32 value)
{
    extern s16 Data_02000240[];
    extern u32 Data_0200dfa4;
    extern u8 Data_03001e70[];
    extern u8 Data_03001ebc[];

    *field = value;
}

/*
 * Actor script -- resource_3bf.  It walks a run of consecutive resource
 * identifiers starting at 0x241e, re-supplying the actor handle to every
 * service that takes one, and returns nothing.
 */

/*
 * Symbol names use the offsets as printed while absolute data addresses keep
 * their raw pool values; the module's own pool constants sit 0x8000 above
 * the printed offsets.  Func_02007376 is reached with two different argument
 * shapes -- once as (handle, 0) and once as (identifier) -- so these services
 * keep old-style declarations rather than an invented common prototype.
 */

/* Loader-relocated overlay calls: each symbol names the pre-relocation call
 * word the image holds. */

/* Resolved engine calls: each pseudo symbol is the per-site call word the
 * overlay image holds (a word can serve two sites with different targets),
 * and the macro names the engine function the site reaches through the
 * overlay veneer and the main-image veneer island, keeping the site's own
 * calling form. Names without a repository binding are provisional.
 */

/* Byte 0x22B of Data_02000240, a flag/state table indexed by other code. */

/* Word at +448 of the record pointed to by the global at 0x03001ebc. */
static __inline__ s32 Value0(s32 (*f)())
{
    extern s16 Data_02000240[];
    extern u8 *Data_03001ebc;

    return f();
}

static __inline__ void Call1_0200169c(void (*f)(), s32 a0)
{
    extern s16 Data_02000240[];
    extern u8 *Data_03001ebc;

    f(a0);
}

static __inline__ s32 Value1_0200169c(s32 (*f)(), s32 a0)
{
    extern s16 Data_02000240[];
    extern u8 *Data_03001ebc;

    return f(a0);
}

/*
 * Keeping this call in its own helper preserves the interleaving of the
 * handle move with the two-instruction constant.
 */
static __inline__ void Call_02007388(s32 handle)
{
    Engine_ActorShowEmote(handle, 258, 60);
}

/* Call sites spelled through these wrappers pass their constants straight
 * into the argument registers; a direct call precomputes a costly constant
 * into a pseudo that the compiler then shares with later uses in the block.
 * A value-returning call also sets r0 last of its arguments. */
static __inline__ void Call1_0200238c(void (*f)(), s32 a0)
{
    extern u8 Data_03001ebc[];

    f(a0);
}

static __inline__ void Call2(void (*f)(), s32 a0, s32 a1)
{
    extern u8 Data_03001ebc[];

    f(a0, a1);
}

static __inline__ void Call6_0200169c(void (*f)(), s32 a0, s32 a1, s32 a2, s32 a3, s32 a4, s32 a5)
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

static __inline__ void Call3_0200169c(void (*f)(), s32 a0, s32 a1, s32 a2)
{
    extern u8 Data_02000240[];

    f(a0, a1, a2);
}

static __inline__ s32 Value2(s32 (*f)(), s32 a0, s32 a1)
{
    extern u8 Data_02000240[];

    return f(a0, a1);
}

static __inline__ s32 Value3(s32 (*f)(), s32 a0, s32 a1, s32 a2)
{
    extern u8 Data_02000240[];

    return f(a0, a1, a2);
}

/* Call sites spelled through these wrappers pass their constants straight
 * into the argument registers; a direct call precomputes a costly constant
 * into a pseudo that the compiler then shares with later uses in the block.
 * A value-returning call also sets r0 last of its arguments. */
static __inline__ void Call1_02004bfc(void (*f)(), s32 a0)
{
    extern u8 Data_03001ebc[];

    f(a0);
}

/* Call sites spelled through these wrappers pass their constants straight
 * into the argument registers; a direct call precomputes a costly constant
 * into a pseudo that the compiler then shares with later uses in the block.
 * A value-returning call also sets r0 last of its arguments. */
static __inline__ void Call3_02001cf0(void (*f)(), s32 a0, s32 a1, s32 a2)
{
    extern u8 Data_03001ebc[];

    f(a0, a1, a2);
}

/* The scene step counter at 0x1d8 of the shared scene work record. */
static __inline__ void bump_step_02001cf0(s32 amount)
{
    extern u8 Data_03001ebc[];

    u8 *work = *(u8 **)Data_03001ebc;

    *(u16 *)(work + 0x1d8) = (u16)(*(u16 *)(work + 0x1d8) + amount);
}

/* Loader-relocated overlay calls: each symbol names the pre-relocation call
 * word the image holds. */

/* Call sites spelled through these wrappers pass their constants straight
 * into the argument registers; a direct call precomputes a costly constant
 * into a pseudo that the compiler then shares with later uses in the block.
 * A value-returning call also sets r0 last of its arguments. */
static __inline__ void Call1_02001dc4(void (*f)(), s32 a0)
{
    extern u8 Data_02000240[];

    extern u8 Data_03001ebc[];

    f(a0);
}

static __inline__ s32 Value1_02001dc4(s32 (*f)(), s32 a0)
{
    extern u8 Data_02000240[];

    extern u8 Data_03001ebc[];

    return f(a0);
}

static __inline__ void Call6_02001dc4(void (*f)(), s32 a0, s32 a1, s32 a2, s32 a3, s32 a4, s32 a5)
{
    extern u8 Data_02000240[];

    extern u8 Data_03001ebc[];

    f(a0, a1, a2, a3, a4, a5);
}

/* Call sites spelled through these wrappers pass their constants straight
 * into the argument registers; a direct call precomputes a costly constant
 * into a pseudo that the compiler then shares with later uses in the block.
 * A value-returning call also sets r0 last of its arguments. */
static __inline__ void Call1_02001e94(void (*f)(), s32 a0)
{
    extern u8 Data_02000240[];
    extern u8 Data_03001ebc[];

    f(a0);
}

/* The scene step counter at 0x1d8 of the shared scene work record. */
static __inline__ void bump_step_02001e94(s32 amount)
{
    extern u8 Data_02000240[];
    extern u8 Data_03001ebc[];

    u8 *work = *(u8 **)Data_03001ebc;

    *(u16 *)(work + 0x1d8) = (u16)(*(u16 *)(work + 0x1d8) + amount);
}

static __inline__ void Call0(void (*f)())
{
    f();
}

static __inline__ void Call2_0200206c(void (*f)(), s32 a0, s32 a1)
{
    f(a0, a1);
}

/* Call sites spelled through these wrappers pass their constants straight
 * into the argument registers; a direct call precomputes a costly constant
 * into a pseudo that the compiler then shares with later uses in the block.
 * A value-returning call also sets r0 last of its arguments. */
static __inline__ void Call1_020021c4(void (*f)(), s32 a0)
{
    extern u8 Data_03001ebc[];

    f(a0);
}

/* The scene step counter at 0x1d8 of the shared scene work record. */
static __inline__ void bump_step_020021c4(s32 amount)
{
    extern u8 Data_03001ebc[];

    u8 *work = *(u8 **)Data_03001ebc;

    *(u16 *)(work + 0x1d8) = (u16)(*(u16 *)(work + 0x1d8) + amount);
}

/* Call sites spelled through these wrappers pass their constants straight
 * into the argument registers; a direct call precomputes a costly constant
 * into a pseudo that the compiler then shares with later uses in the block.
 * A value-returning call also sets r0 last of its arguments. */
static __inline__ s32 Value1_0200252c(s32 (*f)(), s32 a0)
{
    extern u8 Data_03001ebc[];

    return f(a0);
}

/* The scene step counter at 0x1d8 of the shared scene work record. */
static __inline__ void bump_step_0200252c(s32 amount)
{
    extern u8 Data_03001ebc[];

    u8 *work = *(u8 **)Data_03001ebc;

    *(u16 *)(work + 0x1d8) = (u16)(*(u16 *)(work + 0x1d8) + amount);
}

/* Call sites spelled through these wrappers pass their constants straight
 * into the argument registers; a direct call precomputes a costly constant
 * into a pseudo that the compiler then shares with later uses in the block.
 * A value-returning call also sets r0 last of its arguments. */
static __inline__ void Call1_020025f8(void (*f)(), s32 a0)
{
    extern u8 Data_03001ebc[];

    f(a0);
}

static __inline__ s32 Value3_020025f8(s32 (*f)(), s32 a0, s32 a1, s32 a2)
{
    extern u8 Data_03001ebc[];

    return f(a0, a1, a2);
}

/* The scene step counter at 0x1d8 of the shared scene work record. */
static __inline__ void bump_step_020025f8(s32 amount)
{
    extern u8 Data_03001ebc[];

    u8 *work = *(u8 **)Data_03001ebc;

    *(u16 *)(work + 0x1d8) = (u16)(*(u16 *)(work + 0x1d8) + amount);
}

/* Call sites spelled through these wrappers pass their constants straight
 * into the argument registers; a direct call precomputes a costly constant
 * into a pseudo that the compiler then shares with later uses in the block.
 * A value-returning call also sets r0 last of its arguments. */
static __inline__ void Call1_0200269c(void (*f)(), s32 a0)
{
    extern u8 Data_03001ebc[];

    f(a0);
}

/* The scene step counter at 0x1d8 of the shared scene work record. */
static __inline__ void bump_step_0200269c(s32 amount)
{
    extern u8 Data_03001ebc[];

    u8 *work = *(u8 **)Data_03001ebc;

    *(u16 *)(work + 0x1d8) = (u16)(*(u16 *)(work + 0x1d8) + amount);
}

/* Call sites spelled through these wrappers pass their constants straight
 * into the argument registers; a direct call precomputes a costly constant
 * into a pseudo that the compiler then shares with later uses in the block.
 * A value-returning call also sets r0 last of its arguments. */
static __inline__ void Call3_02002718(void (*f)(), s32 a0, s32 a1, s32 a2)
{
    extern u8 Data_03001ebc[];

    f(a0, a1, a2);
}

/* The scene step counter at 0x1d8 of the shared scene work record. */
static __inline__ void bump_step_02002718(s32 amount)
{
    extern u8 Data_03001ebc[];

    u8 *work = *(u8 **)Data_03001ebc;

    *(u16 *)(work + 0x1d8) = (u16)(*(u16 *)(work + 0x1d8) + amount);
}

/* Call sites spelled through these wrappers pass their constants straight
 * into the argument registers; a direct call precomputes a costly constant
 * into a pseudo that the compiler then shares with later uses in the block.
 * A value-returning call also sets r0 last of its arguments. */
static __inline__ void Call1_02003054(void (*f)(), s32 a0)
{
    f(a0);
}

static __inline__ void Call2_02003054(void (*f)(), s32 a0, s32 a1)
{
    f(a0, a1);
}

static __inline__ s32 Value2_02003054(s32 (*f)(), s32 a0, s32 a1)
{
    return f(a0, a1);
}

static __inline__ void Call4(void (*f)(), s32 a0, s32 a1, s32 a2, s32 a3)
{
    f(a0, a1, a2, a3);
}

/* The scene step counter at 0x1d8 of the shared scene work record. */
static __inline__ void bump_step_02003054(s32 amount)
{
    u8 *work = *(u8 **)0x03001ebc;

    *(u16 *)(work + 0x1d8) = (u16)(*(u16 *)(work + 0x1d8) + amount);
}

/* Call sites spelled through these wrappers pass their constants straight
 * into the argument registers; a direct call precomputes a costly constant
 * into a pseudo that the compiler then shares with later uses in the block.
 * A value-returning call also sets r0 last of its arguments. */
static __inline__ void Call1_02004794(void (*f)(), s32 a0)
{
    extern u8 Data_03001ebc[];

    f(a0);
}

static __inline__ s32 Value2_02004794(s32 (*f)(), s32 a0, s32 a1)
{
    extern u8 Data_03001ebc[];

    return f(a0, a1);
}

/* The scene step counter at 0x1d8 of the shared scene work record. */
static __inline__ void bump_step_02004794(s32 amount)
{
    extern u8 Data_03001ebc[];

    u8 *work = *(u8 **)Data_03001ebc;

    *(u16 *)(work + 0x1d8) = (u16)(*(u16 *)(work + 0x1d8) + amount);
}

/* Loader-relocated ROM calls: each site names the pre-relocation call word the image holds. */
static __inline__ void Call0_020049a0(void (*f)())
{
    f();
}

static __inline__ void Call2_020049a0(void (*f)(), s32 a0, s32 a1)
{
    f(a0, a1);
}

static __inline__ s32 Value0_020049a0(s32 (*f)())
{
    return f();
}

static __inline__ s32 Value2_020049a0(s32 (*f)(), s32 a0, s32 a1)
{
    return f(a0, a1);
}

static __inline__ s32 Value3_020049a0(s32 (*f)(), s32 a0, s32 a1, s32 a2)
{
    return f(a0, a1, a2);
}

static __inline__ void Call1_02004f60(void (*f)(), s32 value)
{
    extern s16 Data_02000240[];
    extern u8 Data_03001ebc[];

    f(value);
}

/* Scheduler_AddOrUpdateCallback returns an index even when it is ignored. */
static __inline__ s32 Value2_02004f60(s32 (*f)(), s32 a0, s32 a1)
{
    extern s16 Data_02000240[];
    extern u8 Data_03001ebc[];

    return f(a0, a1);
}

#if defined(TBS_EDITION_JA)
#define Value_00002421 Value_000025aa
#elif defined(TBS_EDITION_DE) || defined(TBS_EDITION_ES) || defined(TBS_EDITION_FR) || defined(TBS_EDITION_IT)
#define Value_00002421 Value_00002403
#endif

extern u8 Value_00002421;


s32 SelectPrimarySceneData(void)
{
    extern s16 Data_02000240[];
    extern u8 *Data_03001ebc;

    s16 scene_variant = Data_02000240[224];

    if (scene_variant == (s32)&Value_000000a0) {
        return (s32)Data_0200e2a4;
    }
    if (scene_variant == (s32)&Value_000000a1) {
        return (s32)Data_0200e4b4;
    }
    if (scene_variant == (s32)&Value_000000a2) {
        return (s32)Data_0200e754;
    }
    return (s32)Data_0200e814;
}

s32 GetEmptySceneData(void) { return 0; }

s32 SelectSecondarySceneData(void)
{
    extern s16 Data_02000240[];
    extern u8 *Data_03001ebc;

    s16 scene_variant = Data_02000240[224];

    if (scene_variant == (s32)&Value_000000a1) {
        return (s32)Data_0200e910;
    }
    if (scene_variant == (s32)&Value_000000a2 || scene_variant == (s32)&Value_000000a3) {
        return (s32)Data_0200e97c;
    }
    return (s32)Data_0200e8a4;
}

s32 SelectTertiarySceneData(void)
{
    extern s16 Data_02000240[];
    extern u8 *Data_03001ebc;

    s16 scene_variant = Data_02000240[224];

    if (scene_variant == (s32)&Value_0000006a) {
        return (s32)Data_0200e9d0;
    }
    if (scene_variant == (s32)&Value_000000a2) {
        return (s32)Data_0200ee08;
    }
    if (scene_variant == (s32)&Value_000000a1) {
        return (s32)Data_0200ec28;
    }
    if (scene_variant == (s32)&Value_000000a0) {
        return (s32)Data_0200eac0;
    }
    if (scene_variant == (s32)&Value_000000a3) {
        return (s32)Data_0200ee98;
    }
    return (s32)Data_0200e9b8;
}

s32 SelectQuaternarySceneData(void)
{
    extern s16 Data_02000240[];
    extern u8 *Data_03001ebc;

    s16 scene_variant = Data_02000240[224];

    if (scene_variant == (s32)&Value_000000a0) {
        return (s32)Data_0200eff4;
    }
    if (scene_variant == (s32)&Value_000000a1) {
        return (s32)Data_0200f258;
    }
    if (scene_variant == (s32)&Value_000000a2) {
        return (s32)Data_0200f528;
    }
    return (s32)Data_0200f63c;
}

void ConfigureSceneActor12(void)
{
    extern s16 Data_02000240[];
    extern u32 Data_0200dfa4;
    extern u8 Data_03001e70[];
    extern u8 Data_03001ebc[];

    s32 actor_slot = 15;
    u8 *actor;

    Engine_MapCopyCellAttributes(15, 20, 1, 1, actor_slot, 22);
    Engine_MapCopyCellAttributes(17, 23, 1, 3, actor_slot, 23);
    actor = Func_020061ca(12);
    if (actor != 0) {
        Func_0200616e(actor, 0);
        actor[0x55] = 0;
        actor[0x23] = 2;
    }
}

void RunSceneObjectSetup(void)
{
    extern s16 Data_02000240[];
    extern u32 Data_0200dfa4;
    extern u8 Data_03001e70[];
    extern u8 Data_03001ebc[];

    Func_02000c68();
}

void FieldScene_StartActorTwelveTransition(void)
{
    extern s16 Data_02000240[];
    extern u32 Data_0200dfa4;
    extern u8 Data_03001e70[];
    extern u8 Data_03001ebc[];

    Call3(Engine_ActorSetSpeed, 12, 0x10000, 0x8000);
    Call3(Engine_ActorSetDestination, 12, 248, 0x178);
    Engine_ActorWaitForMove(12);
    Engine_AudioPlayCue(215);
    Engine_EventWait(60);
    Func_02001728();
    Call1(Engine_GameFlagSet, 0x943);
}

void FieldScene_UpdateActorTwelveTransition(void)
{
    struct FieldActor *actor;

    actor = (struct FieldActor *)Value1(Func_02006244, 12);
    if ((actor->z.fixed >> 20) > 22) {
        Call3(Engine_WorkSetValuesIfNonNegative, 0x40000, 0x40000, 0x10000);
        Call3(Engine_WorkSetValuesIfNonNegative, -1, -1, 0xe666);
        Engine_AudioPlayCue(144);
        Call6(Engine_MapCopyCellAttributes, 15, 20, 1, 1, 15, 22);
        Call6(Engine_MapCopyCellAttributes, 17, 23, 1, 3, 15, 23);
        actor = (struct FieldActor *)Value1(Func_0200629c, 12);
        if (actor != NULL) {
            Func_02006240(actor, 0);
            actor->priority_flags = ACTOR_PRIORITY_UNDERFOOT;
        }
        Call1(Engine_GameFlagSet, 0x943);
    }
}

void PlaceActorTwelveAndFinishScene(void)
{
    extern s16 Data_02000240[];
    extern u8 *Data_03001ebc;

    PlaceActor(12, 0x00f80000, 0x01780000);
    Func_020017d6();
}

void PlaceSceneObjectPairFromTableA(s32 table_index)
{
    extern s16 Data_02000240[];
    extern u32 Data_0200dfa4;
    extern u8 Data_03001e70[];
    extern u8 Data_03001ebc[];

    s32 position_x = Data_0200f714[table_index * 2];
    s32 position_z = Data_0200f714[table_index * 2 + 1];

    Engine_MapCopyCells(0, 0x4d, 1, 3, position_x, position_z);
    Engine_MapCopyCells(1, 0x4d, 1, 1, position_x + 1, position_z);
    Engine_MapCopyCells(position_x, position_z - 0x30, 1, 1, position_x, position_z - 0x2e);
}

void FieldScene_UpdateObjectPairA(void)
{
    extern struct EventWork *Data_03001ebc;

    struct EventWork *work;
    s32 trigger;
    s32 index;

    work = Data_03001ebc;
    if (Func_020062f2(234) != -1) {
        trigger = work->touched_trigger;
        index = trigger - 40;
        if (Value1(Func_02006316, 0x941) == 0 || index != 4) {
            Func_020019a4(index);
            Engine_AudioPlayCue(157);
            Call3(Engine_WorkSetValuesIfNonNegative, 0x30000, 0x30000, 0x10000);
            Call3(Engine_WorkSetValuesIfNonNegative, -1, -1, 0xe666);
            Engine_GameFlagSet(trigger + 0x328);
        }
    }
}

void FieldScene_UpdateObjectPairB(void)
{
    extern struct EventWork *Data_03001ebc;

    struct EventWork *work;
    s32 trigger;

    work = Data_03001ebc;
    if (Func_020063de(234) != -1) {
        trigger = work->touched_trigger;
        Func_02001b44(trigger - 40);
        Engine_AudioPlayCue(157);
        Call3(Engine_WorkSetValuesIfNonNegative, 0x30000, 0x30000, 0x10000);
        Call3(Engine_WorkSetValuesIfNonNegative, -1, -1, 0xe666);
        Engine_GameFlagSet(trigger + 0x32d);
    }
}

void PlaceSceneObjectPairFromTableB(s32 table_index)
{
    extern s16 Data_02000240[];
    extern u32 Data_0200dfa4;
    extern u8 Data_03001e70[];
    extern u8 Data_03001ebc[];

    s32 position_x = Data_0200f754[table_index * 2];
    s32 position_z = Data_0200f754[table_index * 2 + 1];

    Engine_MapCopyCells(0x37, 0x79, 1, 3, position_x, position_z);
    Engine_MapCopyCells(0x38, 0x79, 1, 1, position_x + 1, position_z);
    Engine_MapCopyCells(position_x, position_z - 0x3f, 1, 1, position_x, position_z - 0x3e);
}

void FieldScene_UpdateTableBObjectPair(void)
{
    extern struct EventWork *Data_03001ebc;

    struct EventWork *work;
    s32 trigger;

    work = Data_03001ebc;
    if (Func_02006492(234) != -1) {
        trigger = work->touched_trigger;
        Func_02001cd4(trigger - 40);
        Engine_AudioPlayCue(157);
        Call3(Engine_WorkSetValuesIfNonNegative, 0x30000, 0x30000, 0x10000);
        Call3(Engine_WorkSetValuesIfNonNegative, -1, -1, 0xe666);
        Engine_GameFlagSet(trigger + 0x330);
    }
}

void PlaceSceneObjectPairFromTableC(s32 table_index)
{
    extern s16 Data_02000240[];
    extern u32 Data_0200dfa4;
    extern u8 Data_03001e70[];
    extern u8 Data_03001ebc[];

    s32 position_x = Data_0200f764[table_index * 2];
    s32 position_z = Data_0200f764[table_index * 2 + 1];

    Engine_MapCopyCells(1, 0x50, 1, 3, position_x, position_z);
    Engine_MapCopyCells(2, 0x50, 1, 1, position_x + 1, position_z);
    Engine_MapCopyCellAttributes(position_x, position_z - 0x3f, 1, 1, position_x, position_z - 0x3e);
}

void FieldScene_UpdateObjectPairC(void)
{
    extern struct EventWork *Data_03001ebc;

    struct EventWork *work;
    s32 trigger;

    work = Data_03001ebc;
    if (Func_02006542(234) != -1) {
        trigger = work->touched_trigger;
        Func_02001e34(trigger - 40);
        Engine_AudioPlayCue(157);
        Call3(Engine_WorkSetValuesIfNonNegative, 0x30000, 0x30000, 0x10000);
        Call3(Engine_WorkSetValuesIfNonNegative, -1, -1, 0xe666);
        Engine_GameFlagSet(trigger + 0x332);
    }
}

void CellDoor_Touch(void)
{
    extern s16 Data_02000240[];
    extern u8 *Data_03001ebc;

    if (Func_020065a2(ITEM_CELL_KEY) == -1) {
        Engine_MessageShowCentered((s32)&Value_00000953, 1);
    }
}

void LockedDoor_Touch(void)
{
    extern s16 Data_02000240[];
    extern u8 *Data_03001ebc;

    Engine_MessageShowCentered((s32)&Value_00000953, 1);
}

void Actor8_Interact(void)
{
    extern s16 Data_02000240[];
    extern u8 *Data_03001ebc;

    if (Func_02002008(8, 8) != 0) {
        Engine_GameFlagSet((s32)&Value_00000f2a);
    }
}

void Actor9_Interact(void)
{
    extern s16 Data_02000240[];
    extern u8 *Data_03001ebc;

    if (Func_02002024(9, 7) != 0) {
        Engine_GameFlagSet((s32)&Value_00000f2b);
    }
}

void Actor10_Interact(void)
{
    extern s16 Data_02000240[];
    extern u8 *Data_03001ebc;

    if (Func_02002040(10, 6) != 0) {
        Engine_GameFlagSet((s32)&Value_00000f2c);
    }
}

void Actor11_Interact(void)
{
    extern s16 Data_02000240[];
    extern u8 *Data_03001ebc;

    if (Func_0200205c(11, 5) != 0) {
        Engine_GameFlagSet((s32)&Value_00000f2d);
    }
}

s32 TryStartActorInteraction(s32 actor_id, s32 interaction_id)
{
    extern s16 Data_02000240[];
    extern u8 *Data_03001ebc;

    s32 started = 0;
    s32 interaction;

    Engine_EventBegin();
    interaction = Func_020067d2(0, actor_id, interaction_id);
    if (Engine_PartyGiveItem(interaction_id, 0) != -1) {
        Engine_ActorSetAnimation(actor_id, 2);
        started = 1;
    } else {
        Engine_AudioPlayCue(0x7d);
        Engine_ActorSetAnimation(actor_id, 5);
    }
    Func_02006636(interaction);
    Engine_EventEnd();
    return started;
}

void NoOpSceneCallbackA(void)
{
    extern s16 Data_02000240[];
    extern u8 *Data_03001ebc;

}

void NoOpSceneCallbackB(void)
{
    extern s16 Data_02000240[];
    extern u8 *Data_03001ebc;

}

void NoOpSceneCallbackC(void)
{
    extern s16 Data_02000240[];
    extern u8 *Data_03001ebc;

}

void NoOpSceneCallbackD(void)
{
    extern s16 Data_02000240[];
    extern u8 *Data_03001ebc;

}

void CellKey_PickUp(void)
{
    extern s32 Data_0200dfa4;

    Engine_ActorWalkTo(0, 0x108, 0x318);
    Engine_ActorWaitForMove(0);
    Func_020067d0(0, 0x4000, 0);
    Engine_EventWait(10);
    Engine_ActorSetAnimation(0, 1);
    Engine_ItemShowFound(ITEM_CELL_KEY, 3);
    Engine_ActorSetAnimation(0, 1);
    Engine_PartyGiveItem(ITEM_CELL_KEY, 0);
    Engine_GameFlagSet((s32)&Value_00000f2e);
    Engine_ActorSetPosition(8, 0, 0);
}

s32 IsPlayerInAccidentTriggerArea(void)
{
    extern s16 Data_02000240[];
    extern u8 *Data_03001ebc;

    SceneActor *player = Func_0200675e(0);
    s32 z = player->z;
    s32 x;
    s32 zz, xx;

    if (z < 0) {
        z += 0xfffff;
    }
    x = player->x;
    zz = z >> 20;
    if (x < 0) {
        x += 0xfffff;
    }
    xx = x >> 20;
    if ((u32)(zz - 5) <= 2 && xx <= 10) {
        return 1;
    }
    if ((u32)(xx - 8) <= 1 && zz > 22) {
        return 1;
    }
    return 0;
}

void FieldScene_UpdateActorPairInteraction(void)
{
    extern s16 Data_02000240[];
    extern u32 Data_0200dfa4;
    extern u8 Data_03001e70[];
    extern u8 Data_03001ebc[];

    struct ObjectRuntime *actor = Func_020067ac(9);
    struct ObjectRuntime *other = Func_020067b4(10);
    s32 *work = (s32 *)(*(u8 **)Data_03001e70 + 0x164);
    s16 *scene = *(s16 **)(Data_03001e70 + 0x4c);

    if (Data_03001e40 & 1) {
        work[6] = 1;
        work[7] = 1;
    } else {
        work[6] = -1;
        work[7] = -1;
    }
    if (Func_020067a6(0x106) || scene[191] != 0 || scene[192] != 0) {
        actor->movement_state = 1;
        other->movement_state = 1;
    } else if (!Value1(Func_020067de, 0x214)) {
        actor->movement_state = 0;
        other->movement_state = 0;
        if (!Value1(Func_020067de, 0x214) && actor->movement_state == 0) {
            work[8] = Func_02006d72(Func_02006c7a(8912896.0, Func_02006ce6(actor->x)));
        }
        if (!Func_02002314()) {
            if (Data_02000240[294] != 0) {
                if (Func_02002b3e(9) && Data_02000240[294] != 0) {
                    SetSceneValue(&scene[191], (s32)&Value_00002092);
                    return;
                }
                if (Func_02002b3e(10) && Data_02000240[294] != 0) {
                    SetSceneValue(&scene[191], (s32)&Value_00002092);
                    return;
                }
            }
            if (Data_02000240[294] == 0) {
                if (Func_02002b72(9)) {
                    Engine_GameFlagSet(0x215);
                    Engine_GameFlagSet(0x214);
                }
                if (Func_02002b72(10)) {
                    Engine_GameFlagSet(0x215);
                    Engine_GameFlagSet(0x214);
                }
            }
            if (Value1(Func_020067de, 0x214)) {
                SetSceneValue(&scene[193], 91);
            }
        }
    }
}

void ConfigureSceneActor9(void)
{
    extern s16 Data_02000240[];
    extern u8 *Data_03001ebc;

    Engine_EventBegin();
    Engine_ActorStop(9);
    Engine_ActorSetDestinationOffset(9, 0, 0);
    Engine_ActorSetAnimation(9, 0);
    Engine_ActorFaceActor(9, 0, 0);
    Engine_ActorShowEmote(9, 256, 0);
    Func_02002e3c(10);
    Engine_EventEnd();
}

s32 AreSceneActorsInPassingLane(void)
{
    extern s16 Data_02000240[];
    extern u8 *Data_03001ebc;

    SceneActor *player = Func_0200697a(0);
    SceneActor *passing_actor = Func_02006982(17);
    s32 ox = player->x;
    s32 pz;
    s32 px;
    s32 oxx, pzz, pxx;

    if (ox < 0) {
        ox += 0xfffff;
    }
    oxx = ox >> 20;
    pz = passing_actor->z;
    if (pz < 0) {
        pz += 0xfffff;
    }
    px = passing_actor->x;
    pzz = pz >> 20;
    if (px < 0) {
        px += 0xfffff;
    }
    pxx = px >> 20;
    if (oxx == 52 && pxx == 57 && pzz > 34 && pzz <= 40) {
        return 1;
    }
    if (oxx == 57 && pxx == 52 && pzz > 34 && pzz <= 40) {
        return 1;
    }
    return 0;
}

void FieldScene_UpdateActorSeventeenInteraction(void)
{
    extern s16 Data_02000240[];
    extern u32 Data_0200dfa4;
    extern u8 Data_03001e70[];
    extern u8 Data_03001ebc[];

    struct ObjectRuntime *actor = Func_020069e6(17);
    s32 *work = (s32 *)(*(u8 **)Data_03001e70 + 0x164);
    s16 *scene = *(s16 **)(Data_03001e70 + 0x4c);

    Func_02006afa(actor);
    if (Data_03001e40 & 1) {
        work[6] = 1;
        work[7] = 1;
    } else {
        work[6] = -1;
        work[7] = -1;
    }
    if (Func_020069dc(0x106) || scene[191] != 0 || scene[192] != 0) {
        actor->movement_state = 1;
    } else if (!Value1(Func_02006a0e, 0x214)) {
        actor->movement_state = 0;
        if (!Value1(Func_02006a0e, 0x214) && actor->movement_state == 0) {
            work[8] = 0x3400000 - actor->x;
            work[9] = 0x2400000 - actor->z;
        }
        if (!Func_02002756()) {
            Func_02002dbc(17);
            if (Func_02002d5a(17) && Data_02000240[294] != 0) {
                SetSceneValue(&scene[191], (s32)&Value_00002092);
                return;
            }
            if (Data_02000240[294] == 0) {
                if (Func_02002d58(17)) {
                    Engine_GameFlagSet(0x215);
                    Engine_GameFlagSet(0x214);
                }
            }
            if (Value1(Func_02006a0e, 0x214)) {
                SetSceneValue(&scene[193], 92);
            }
        }
    }
}

void ActivateSceneActor17(void)
{
    extern s16 Data_02000240[];
    extern u8 *Data_03001ebc;

    Func_02002fe6(17);
    Engine_EventEnd();
}

s32 IsPlayerInSecondaryTriggerArea(void)
{
    extern s16 Data_02000240[];
    extern u8 *Data_03001ebc;

    SceneActor *player = Func_02006b22(0);
    s32 zz = player->z / 0x100000;
    s32 xx = player->x / 0x100000;

    if ((u32)(xx - 41) <= 3 && zz > 25 && zz <= 28) {
        return 1;
    }
    if (xx == 41 && zz > 37 && zz <= 41) {
        return 1;
    }
    if ((u32)(xx - 54) <= 2 && zz > 30 && zz <= 40) {
        return 1;
    }
    return 0;
}

void FieldScene_UpdateActorEighteenInteraction(void)
{
    extern s16 Data_02000240[];
    extern u32 Data_0200dfa4;
    extern u8 Data_03001e70[];
    extern u8 Data_03001ebc[];

    struct ObjectRuntime *actor = Func_02006b82(18);
    s32 *work = (s32 *)(*(u8 **)Data_03001e70 + 0x164);
    s16 *scene = *(s16 **)(Data_03001e70 + 0x4c);

    if (Data_03001e40 & 1) {
        work[6] = 1;
        work[7] = 1;
    } else {
        work[6] = -1;
        work[7] = -1;
    }
    if (Func_02006b74(0x106) || scene[191] != 0 || scene[192] != 0) {
        actor->movement_state = 1;
    } else if (!Value1(Func_02006ba6, 0x214)) {
        actor->movement_state = 0;
        if (!Value1(Func_02006ba6, 0x214) && actor->movement_state == 0) {
            work[8] = 0x2f00000 - actor->x;
            work[9] = 0x1f00000 - actor->z;
        }
        if (!Func_02002a96()) {
            if (Func_02002eec(18) && Data_02000240[294] != 0) {
                SetSceneValue(&scene[191], (s32)&Value_00002092);
                return;
            }
            if (Data_02000240[294] == 0) {
                if (Func_02002eea(18)) {
                    Engine_GameFlagSet(0x215);
                    Engine_GameFlagSet(0x214);
                }
            }
            if (Value1(Func_02006ba6, 0x214)) {
                SetSceneValue(&scene[193], 93);
            }
        }
    }
}

void ActivateSceneActor18(void)
{
    extern s16 Data_02000240[];
    extern u8 *Data_03001ebc;

    Func_0200317a(18);
    Engine_EventEnd();
}

s32 IsPlayerOutsideSceneRectangle(void)
{
    extern s16 Data_02000240[];
    extern u8 *Data_03001ebc;

    SceneActor *player = Func_02006cb6(0);
    s32 zz = player->z / 0x100000;
    s32 xx = player->x / 0x100000;

    if (xx > 45 && zz > 14 && xx <= 64 && zz <= 16) {
        return 0;
    }
    return 1;
}

void FieldScene_RunScene3bfSequenceA(void)
{
    extern s16 Data_02000240[];
    extern struct EventWork *Data_03001ebc;

    struct EventWork *work;

    work = Data_03001ebc;
    if (Value1_0200169c(Func_02006cb8, 0x214) == 0) {
        if (Value0(Func_02002d10) == 0) {
            if (Data_02000240[294] == 0) {
                if (Value1_0200169c(Func_02002fae, 17) != 0) {
                    Call1_0200169c(Engine_GameFlagSet, 0x215);
                    Call1_0200169c(Engine_GameFlagSet, 0x214);
                }
            }
            if (Value1_0200169c(Func_02006cf4_b, 0x214) != 0) {
                work->raised_trigger = 94;
            }
        }
    }
}

void RunActor17SceneStep(void)
{
    extern s16 Data_02000240[];
    extern u8 *Data_03001ebc;

    Func_02003232(17);
    Engine_EventEnd();
}

void TriggerSceneStage95FromActor12(void)
{
    extern s16 Data_02000240[];
    extern u8 *Data_03001ebc;

    u8 *scene_state = Data_03001ebc;

    if (Func_02003006(12) != 0 && Data_02000240[294] == 0) {
        s16 *scene_stage;
        s32 next_stage;

        Func_02006cd4(0x02009719);
        scene_stage = (s16 *)(scene_state + 386);
        next_stage = 95;
        *scene_stage = next_stage;
    }
}

void FieldScene_RunScene3bfSequenceB(void)
{
    extern s16 Data_02000240[];
    extern struct EventWork *Data_03001ebc;

    struct EventWork *work;

    work = Data_03001ebc;
    if (Value1_0200169c(Func_02006d76, 0x225) == 0) {
        if (Value1_0200169c(Func_02003054_scene_scripts, 13) != 0) {
            if (Data_02000240[294] == 0) {
                Call1_0200169c(Engine_GameFlagSet, 0x225);
                Call1_0200169c(Func_02006d28, 0x200975d);
                Call1_0200169c(Func_02006d2e, 0x20097bd);
                work->raised_trigger = 96;
            }
        }
    }
}

void FieldScene_RunScene3bfSequenceC(void)
{
    extern s16 Data_02000240[];
    extern struct EventWork *Data_03001ebc;

    struct EventWork *work;

    work = Data_03001ebc;
    if (Value1_0200169c(Func_02006dd6, 0x225) == 0) {
        if (Value1_0200169c(Func_020030b4, 21) != 0) {
            if (Data_02000240[294] == 0) {
                Call1_0200169c(Engine_GameFlagSet, 0x225);
                Call1_0200169c(Func_02006d88, 0x20097bd);
                Call1_0200169c(Func_02006d8e, 0x200975d);
                work->raised_trigger = 96;
            }
        }
    }
}

s32 IsSceneActorVerticallyNearPlayer(void)
{
    extern s16 Data_02000240[];
    extern u8 *Data_03001ebc;

    SceneActor *scene_actor = Func_02006e70();
    SceneActor *player = Func_02006e78(0);
    s32 actor_z = scene_actor->z / 0x100000;
    s32 actor_x = scene_actor->x / 0x100000;
    s32 player_z = player->z / 0x100000;
    s32 player_x = player->x / 0x100000;
    s32 z_distance = actor_z - player_z;

    if (z_distance >= -6 && z_distance <= 6 && actor_x - 1 < player_x && actor_x + 1 > player_x) {
        return 1;
    }
    return 0;
}

s32 IsSceneActorHorizontallyNearPlayer(void)
{
    extern s16 Data_02000240[];
    extern u8 *Data_03001ebc;

    SceneActor *scene_actor = Func_02006ed4();
    SceneActor *player = Func_02006edc(0);
    s32 actor_z = scene_actor->z / 0x100000;
    s32 actor_x = scene_actor->x / 0x100000;
    s32 player_z = player->z / 0x100000;
    s32 player_x = player->x / 0x100000;
    s32 x_distance = actor_x - player_x;

    if (x_distance < -6 || x_distance > 6) {
        return 0;
    }
    if (actor_z - 2 < player_z && actor_z + 2 > player_z) {
        return 1;
    }
    return 0;
}

s32 IsActorInteractionAvailable(s32 actor_id)
{
    extern s16 Data_02000240[];
    extern u8 *Data_03001ebc;

    if (Func_0200326a() == 0) {
        return 0;
    }
    if (Func_02003114(actor_id)!= 0) {
        return 1;
    }
    {
        s32 result = Func_02003186(actor_id);

        /* branchless "result != 0" */
        return (u32)(result | -result) >> 31;
    }
}

s32 IsSceneActorWithinFourSteps(void)
{
    extern s16 Data_02000240[];
    extern u8 *Data_03001ebc;

    SceneActor *scene_actor = Func_02006f6c();
    SceneActor *player = Func_02006f74(0);
    s32 actor_z = scene_actor->z / 0x100000;
    s32 actor_x = scene_actor->x / 0x100000;
    s32 player_z = player->z / 0x100000;
    s32 player_x = player->x / 0x100000;
    s32 x_distance = actor_x - player_x;
    s32 z_distance;

    actor_z += 1;
    if (x_distance < 0) {
        x_distance = -x_distance;
    }
    z_distance = actor_z - player_z;
    if (z_distance < 0) {
        z_distance = -z_distance;
    }
    if (x_distance + z_distance <= 4) {
        return 1;
    }
    return 0;
}

s32 IsSceneActorWithinTriggerBox(void)
{
    extern s16 Data_02000240[];
    extern u8 *Data_03001ebc;

    SceneActor *scene_actor = Func_02006fd4();
    SceneActor *player = Func_020070da();
    s32 actor_x = scene_actor->x / 0x100000;
    s32 actor_z = scene_actor->z / 0x100000;
    s32 player_x = player->x / 0x100000;
    s32 player_z = player->z / 0x100000;
    s32 x_distance = actor_x - player_x;
    s32 z_distance;

    if (x_distance < 0) {
        x_distance = -x_distance;
    }
    z_distance = actor_z - player_z;
    if (z_distance < 0) {
        z_distance = -z_distance;
    }
    if (x_distance > 7 || z_distance > 5) {
        return 0;
    }
    return 1;
}

void TriggerScene41AtVillagePath(void)
{
    extern s16 Data_02000240[];
    extern u8 *Data_03001ebc;

    SceneActor *player = Func_0200703e(0);

    if (Func_02007006(859) == 0) {
        s32 player_x = player->x / 0x100000;
        s32 player_z = player->z / 0x100000;

        if (player_x == 43 && player_z > 28 && player_z <= 31) {
            s16 *q = (s16 *)(Data_03001ebc + 364);
            s32 v = 41;

            *q = v;
            Func_02002960();
        }
    }
}

void TriggerScene40AtVillagePath(void)
{
    extern s16 Data_02000240[];
    extern u8 *Data_03001ebc;

    DirectionalSceneActor *player = Func_0200709a(0);

    if (Func_02007064(856) == 0) {
        s32 player_x = player->x / 0x100000;
        s32 player_z = player->z / 0x100000;

        if (player_x == 16 && player_z > 55 && player_z <= 58
            && (player->dir == 0xc000 || player->dir == 0x4000)) {
            s16 *q = (s16 *)(Data_03001ebc + 364);
            s32 v = 40;

            *q = v;
            Func_02002920();
        }
    }
}

void RunActor9ScriptedSequence(void)
{
    Engine_EventBegin();
    Engine_ActorSetDestinationOffset(9, 0, 0);
    Func_02007126(9, 1);
    Engine_ActorStop(9);
    Engine_ActorSetAnimation(9, 0);
    Func_0200713c_b(0, 1);
    {
        u8 *t = &Value_0000240d;

        Engine_EventSetMessage((s32)t);
        Engine_EventShowMessage(9, 0);
        Engine_ActorShowEmote(0, 258, 60);
        Engine_EventSetMessage((s32)(t + 1));
    }
    Engine_EventShowMessage(9, 0);
    Engine_EventRequestExit(60);
    Engine_EventCloseScreen();
    Engine_EventEnd();
}

void RunActorScriptedSequenceA(s32 actor_id)
{
    Engine_EventBegin();
    Engine_EventBegin();
    Engine_ActorShowEmote(actor_id, 256, 1);
    Engine_ActorSetDestinationOffset(actor_id, 0, 0);
    Func_020071ac(actor_id, 1);
    Engine_ActorSetAnimation(actor_id, 0);
    Engine_ActorFaceActor(actor_id, 0, 0);
    Engine_ActorSetAnimation(0, 1);
    Engine_ActorSetDestinationOffset(actor_id, 0, 0);
    Func_020071d8(actor_id, 1);
    Engine_ActorStop(actor_id);
    Engine_ActorSetAnimation(actor_id, 0);
    Func_020071ee_b(0, 1);
    {
        u8 *t = &Value_0000240d;

        Engine_EventSetMessage((s32)t);
        Engine_EventShowMessage(actor_id, 0);
        Engine_ActorFaceActor(0, actor_id, 0);
        Engine_ActorShowEmote(0, 258, 60);
        Engine_EventSetMessage((s32)(t + 1));
    }
    Engine_EventShowMessage(actor_id, 0);
    Engine_EventCloseScreen();
    Engine_EventWait(60);
    Engine_EventRequestExit(60);
    Engine_EventEnd();
}

void TurnActorToSceneDirection(s32 actor_id)
{
    extern s32 Data_0200dfa4;

    Engine_ActorFaceActor(actor_id, 0, 0);
    Engine_ActorFaceActor(0, actor_id, 0);
    switch (Data_0200dfa4 & 3) {
    case 0:
        Func_0200386a(actor_id);
        break;
    case 1:
        Func_020038ca(actor_id);
        break;
    case 2:
        Func_0200391e(actor_id);
        break;
    case 3:
        Func_02003996(actor_id);
        break;
    default:
        Func_020038e2(actor_id);
        break;
    }
}

/*
 * The resource run is taken as the address of Value_0000241e rather than as
 * an integer constant, which preserves its pointer identity and materialises
 * it after the first call.
 */
void RunActorScriptedSequenceB(s32 handle)
{
    u8 *id;

    Func_0200731c(handle, 1);
    id = &Value_0000241e;
    Engine_EventSetMessage((s32)id);
    Engine_EventShowMessage(handle, 0);
    Call_02007388(handle);
    Engine_EventSetMessage((s32)(id + 1));
    Engine_EventShowMessage(handle, 0);
    id += 2;
    Func_02007338(handle, 4);
    Engine_EventSetMessage((s32)id);
    Engine_EventShowMessage(handle, 0);
}

void RunActorScriptedSequenceC(s32 actor_id)
{
    u8 *t = &Value_00002421;

    Engine_EventSetMessage((s32)t);
    Engine_EventShowMessage(actor_id, 0);
    Func_02007386_a(actor_id, 1);
    Engine_EventSetMessage((s32)(t + 1));
    Engine_EventShowMessage(actor_id, 0);
    Func_02007386_b(actor_id, 4);
    Engine_EventSetMessage((s32)(t + 2));
    Engine_EventShowMessage(actor_id, 0);
}

void FieldScene_RunScene3bf_02001cf0(s32 a0)
{
    extern u8 Data_03001ebc[];

    u32 i;
    s32 record;
    s32 base6_2424;

    base6_2424 = (s32)Data_00002424;
    Engine_EventSetMessage(base6_2424);
    Engine_EventShowMessage(a0, 0);
    Engine_EventWait(120);
    Call3_02001cf0(Engine_ActorShowEmote, a0, 0x101, 60);
    Engine_EventSetMessage((base6_2424 + 1));
    Engine_EventShowMessage(a0, 0);
    Func_020073f0_a(a0, 1);
    Engine_EventSetMessage((base6_2424 + 2));
    Engine_EventShowMessage(a0, 0);
    Func_020073f0_b(a0, 4);
    Engine_EventSetMessage((base6_2424 + 3));
    Engine_EventShowMessage(a0, 0);
}

void RunActorScriptedSequenceD(s32 actor_id)
{
    u8 *t = &Value_00002428;

    Engine_EventSetMessage((s32)t);
    Engine_EventShowMessage(actor_id, 0);
    Func_0200742a(actor_id, 4);
    Engine_EventSetMessage((s32)(t + 1));
    Engine_EventShowMessage(actor_id, 0);
    Func_02007458_a(actor_id, 1);
    Engine_EventSetMessage((s32)(t + 2));
    Engine_EventShowMessage(actor_id, 0);
    Func_02007458_b(actor_id, 3);
    Engine_EventSetMessage((s32)(t + 3));
    Engine_EventShowMessage(actor_id, 0);
}

void InspectOrdinaryObject(void)
{
    extern u8 Data_02000240[];

    extern u8 *Data_03001ebc;

    Engine_ActorSetAnimation(0, 1);
    Engine_AudioPlayCue(113);
    Engine_ActorShowEmote(15, 256, 60);
    Func_020039c8(15);
    *(s32 *)(Data_03001ebc + 448) = 512;
    Data_02000240[0x22b] = 3;
    Func_02007564(98, 2);
    Engine_ActorSetPosition(15, 0, 0);
    Engine_EventEnd();
    Engine_GameFlagSet(2380);
}

void InspectEmptyChest(void)
{
    extern u8 Data_02000240[];

    extern u8 *Data_03001ebc;

    Engine_ActorSetAnimation(0, 1);
    Engine_AudioPlayCue(113);
    Engine_ActorShowEmote(11, 256, 60);
    Func_02003a30(11);
    *(s32 *)(Data_03001ebc + 448) = 512;
    Data_02000240[0x22b] = 3;
    Func_020075cc(98, 2);
    Engine_ActorSetPosition(11, 0, 0);
    Engine_EventEnd();
    Engine_GameFlagSet(2377);
}

/* Runs a scripted beat on the objects indexed 12, 13 and 14, stepping
 * through the entries at Data_00002438 as it goes, then sets the scene
 * phase word and a status byte at +0x22b of the record at Data_02000240
 * before handing off to the next step. */
void FieldScene_RunSupplementalSequenceOne(void)
{
    extern u8 Data_02000240[];
    extern struct EventWork *Data_03001ebc;

    s32 sequence_2438;

    Object_SetModeById_1_02001e94(0, 1);
    Object_SetModeById_2_02001e94(12, 1);
    Object_SetModeById_3_02001e94(13, 1);
    Object_SetModeById_4_02001e94(14, 1);
    Audio_PlayCue_1_02001e94(113);
    BattleEffect_SpawnLinkedResourceObject_1_02001e94(12, 0x100, 0);
    BattleRuntime_WaitIfModeZero_1_02001e94(30);
    ObjectMotion_SetAngleToward_1_02001e94(12, 0, 0);
    sequence_2438 = (s32)Data_00002438;
    SceneWork_SetStepValue_1_02001e94(sequence_2438);
    BattleEvent_RunActionAndWait_1_02001e94(12, 0);
    BattleEffect_SpawnLinkedResourceObject_2_02001e94(13, 0x100, 0);
    BattleEffect_SpawnLinkedResourceObject_3_02001e94(14, 0x100, 0);
    ObjectMotion_SetAngleToward_2_02001e94(13, 0, 0);
    ObjectMotion_SetAngleToward_3_02001e94(14, 0, 0);
    ObjectMotion_SetAngleToward_4_02001e94(0, 13, 0);
    BattleRuntime_WaitIfModeZero_2_02001e94(65);
    ObjectMotion_ArmCallback_1_02001e94(13, 0x5000, 0);
    ObjectMotion_ArmCallback_2_02001e94(14, 0xd000, 0);
    SceneWork_SetStepValue_2_02001e94((sequence_2438 + 1));
    BattleEvent_RunActionAndWait_2_02001e94(13, 0);
    ObjectMotion_CallThenWaitForAnimationChange_1_02001e94(14, 3);
    SceneWork_SetStepValue_3_02001e94((sequence_2438 + 2));
    BattleEvent_RunActionAndWait_3_02001e94(14, 0);
    SceneWork_SetStepValue_4_02001e94((sequence_2438 + 3));
    BattleEvent_RunActionAndWait_4_02001e94(12, 0);
    ObjectMotion_SetVariantCallbackAndInvokeObject_1(13, 1);
    SceneWork_SetStepValue_5_02001e94((sequence_2438 + 4));
    BattleEvent_RunActionAndWait_5_02001e94(13, 0);
    ObjectMotion_CallThenWaitForAnimationChange_2_02001e94(14, 3);
    SceneWork_SetStepValue_6_02001e94((sequence_2438 + 5));
    BattleEvent_RunActionAndWait_6_02001e94(14, 0);
    ObjectMotion_CallThenWaitForAnimationChange_3_02001e94(14, 3);
    BattleRuntime_WaitIfModeZero_3_02001e94(60);
    ObjectMotion_SetAngleToward_5_02001e94(13, 0, 0);
    ObjectMotion_SetAngleToward_6_02001e94(14, 0, 0);
    BattleRuntime_WaitIfModeZero_4_02001e94(70);
    ObjectMotion_ResetAndSetPositionInMode2_1_02001e94(12, 0x2a0, 88); /* object_id 12, x 0x2a0, z 88 */
    ObjectMotion_CommitCurrentPositionAndActivate_1_02001e94(12);
#if !defined(TBS_EDITION_JA)
    /* The localized sequence adds this turn after actor 12 is placed. */
    ObjectMotion_SetAngleToward_7_02001e94(12, 0, 0);
#endif
    ObjectMotion_CallThenWaitForAnimationChange_4_02001e94(12, 3);
    BattleRuntime_WaitIfModeZero_5_02001e94(30);
    SceneWork_SetStepValue_7_02001e94((sequence_2438 + 6));
    BattleEvent_RunActionAndWait_7_02001e94(12, 0);
    Data_03001ebc->start_transition = SCENE_TRANSITION(TRANSITION_WINDOW, 0);
    SharedWorkData_SetFirstAndSecondFields_1_02001e94((s32)Data_000000a1, 31);
    Data_02000240[0x22b] = 3;
    BattleEffect_ComputeWeightedResultAndDispatch_1_02001e94(98, 3);
    ObjectMotion_SetHorizontalPositionWithTerrain_1_02001e94(12, 0, 0);
    ObjectMotion_SetHorizontalPositionWithTerrain_2_02001e94(13, 0, 0);
    ObjectMotion_SetHorizontalPositionWithTerrain_3(14, 0, 0);
    BattleRuntime_ScheduleShoulderButtonModeUpdate_1_02001e94();
    GameFlag_Set_1_02001e94(0x94a); /* main:080770c8 */
}

void FieldScene_RunSequenceTail(void)
{
    BattleRuntime_Reset_1_0200206c();
    ObjectMotion_SetHorizontalPositionWithTerrain_1_0200206c(12, 45088768, 5767168); /* object_id 12, x, z */
    ObjectMotion_SetHorizontalPositionWithTerrain_2_0200206c(13, 46137344, 5767168); /* object_id 13, x, z */
    ObjectMotion_SetHorizontalPositionWithTerrain_3_0200206c(14, 47185920, 6291456); /* object_id 14, x, z */
    Object_SetModeById_1_0200206c(12, 5); /* object_id 12, action 5 */
    Object_SetModeById_2_0200206c(13, 5); /* object_id 13, action 5 */
    Object_SetModeById_3_0200206c(14, 5); /* object_id 14, action 5 */
    ObjectMotion_SetAngleToward_1_0200206c(0, 13, 0);
    BattleRuntime_ScheduleShoulderButtonModeUpdate_1_0200206c();
    BattleRuntime_WaitIfModeZero_1_0200206c(); /* main:0808a360 */
}

void InspectEmptySceneObject(void)
{
    extern u8 Data_02000240[];

    extern u8 *Data_03001ebc;

    Engine_ActorSetAnimation(0, 1);
    Engine_AudioPlayCue(113);
    Engine_ActorShowEmote(16, 256, 60);
    Func_02003cd0(16);
    *(s32 *)(Data_03001ebc + 448) = 512;
    Data_02000240[0x22b] = 3;
    Func_0200786c_a(98, 2);
    Engine_ActorSetPosition(16, 0, 0);
    Engine_EventEnd();
    Engine_GameFlagSet(2379);
}

void RunActor12InteractionSequence(void)
{
    Engine_EventBegin();
    Engine_ActorSetDestinationOffset(0, 0, 0);
    Func_020077aa(0, 1);
    Engine_ActorSetAnimation(0, 1);
    Engine_ActorFaceActor(12, 0, 0);
    Engine_AudioPlayCue(113);
    Engine_ActorShowEmote(12, 256, 60);
    {
        u8 *t = &Value_0000240d;

        Engine_EventSetMessage((s32)t);
        Engine_EventShowMessage(12, 0);
        Engine_ActorShowEmote(0, 258, 50);
        Engine_EventSetMessage((s32)(t + 1));
    }
    Engine_EventShowMessage(12, 0);
    Engine_EventCloseScreen();
    Engine_EventWait(60);
    Engine_EventRequestExit(60);
    Engine_EventEnd();
    Engine_GameFlagSet(548);
}

void FieldScene_RunScene3bf_020021c4(void)
{
    extern u8 Data_03001ebc[];

    u32 i;
    s32 record;
    s32 base5_240d;

    Engine_EventBegin();
    Engine_ActorSetDestinationOffset(0, 0, 0);
    Func_0200783a(0, 1);
    Engine_ActorSetAnimation(0, 1);
    Engine_AudioPlayCue(113);
    Call3(Engine_ActorShowEmote, 21, 0x100, 0);
    Call3(Engine_ActorShowEmote, 13, 0x100, 60);
    Engine_ActorFaceActor(21, 0, 0);
    Engine_ActorFaceActor(13, 0, 0);
    base5_240d = (s32)Data_0000240d;
    Engine_EventSetMessage(base5_240d);
    Engine_EventShowMessage(13, 0);
    Call3(Engine_ActorShowEmote, 0, 0x102, 30);
    Engine_EventSetMessage((base5_240d + 1));
    Engine_EventShowMessage(13, 0);
    Engine_EventCloseScreen();
    Engine_EventWait(60);
    Engine_EventRequestExit(60);
    Engine_EventEnd();
    Call1_020021c4(Engine_GameFlagSet, 0x225);
}

void ConfigureInteractionRegionA(void)
{
    extern s16 Data_02000240[];
    extern u8 *Data_03001ebc;

    Engine_MapCopyCells(2, 82, 1, 2, 21, 81);
    Engine_MapCopyCellAttributes(21, 32, 1, 1, 21, 34);
}

void ConfigureInteractionRegionB(void)
{
    extern s16 Data_02000240[];
    extern u8 *Data_03001ebc;

    Engine_MapCopyCells(2, 84, 1, 2, 6, 55);
    Engine_MapCopyCellAttributes(5, 9, 1, 1, 6, 10);
}

void ConfigureInteractionRegionC(void)
{
    extern s16 Data_02000240[];
    extern u8 *Data_03001ebc;

    Engine_MapCopyCells(2, 86, 1, 2, 27, 62);
    Engine_MapCopyCellAttributes(26, 16, 1, 1, 27, 17);
}

void InspectVillageWell(void)
{
    extern u8 *Data_03001ebc;

    if (*(s16 *)(Data_03001ebc + 0xcb8) != 0) {
        if (Value1_02001dc4(Func_02007930, 0x947) == 0) {
            Call2(Engine_MessageShowCentered, 0x1528, 1);
            Engine_AudioPlayCue(188);
            Engine_EventWait(1);
            Call6_02001dc4(Engine_MapCopyCells, 6, 77, 1, 2, 17, 82);
            Engine_EventWait(5);
            Call6_02001dc4(Engine_MapCopyCells, 7, 77, 1, 2, 17, 82);
            Engine_EventWait(1);
            Func_020045d8();
            Call1_02001dc4(Engine_GameFlagSet, 0x947);
        }
    }
}

void RunSecondaryMapInteraction(void)
{
    extern u8 *Data_03001ebc;

    if (*(s16 *)(Data_03001ebc + 0xcb8) != 0) {
        if (Value1_0200169c(Func_020079b4, 0x948) == 0) {
            Call2(Engine_MessageShowCentered, 0x1528, 1);
            Engine_AudioPlayCue(188);
            Engine_EventWait(1);
            Call6_0200169c(Engine_MapCopyCells, 6, 77, 1, 2, 3, 55);
            Engine_EventWait(5);
            Call6_0200169c(Engine_MapCopyCells, 7, 77, 1, 2, 3, 55);
            Engine_EventWait(1);
            Func_02004690();
            Call1_0200238c(Engine_GameFlagSet, 0x948);
        }
    }
}

void ConfigurePrimaryInteractionRegions(void)
{
    extern s16 Data_02000240[];
    extern u8 *Data_03001ebc;

    Engine_MapCopyCells(5, 77, 1, 2, 17, 82);
    Engine_MapCopyCells(5, 77, 1, 2, 3, 55);
    Engine_MapCopyCellAttributes(15, 33, 1, 1, 17, 35);
    Engine_MapCopyCellAttributes(3, 8, 1, 1, 3, 10);
}

void ConfigureSecondaryInteractionRegions(void)
{
    extern s16 Data_02000240[];
    extern u8 *Data_03001ebc;

    Engine_MapCopyCells(8, 77, 1, 2, 17, 82);
    Engine_MapCopyCells(8, 77, 1, 2, 3, 55);
    Engine_MapCopyCellAttributes(18, 35, 1, 1, 17, 35);
    Engine_MapCopyCellAttributes(2, 10, 1, 1, 3, 10);
}

void InspectWardrobe(void)
{
    extern u8 Data_02000240[];

    Engine_GameFlagSet(2372);
    Engine_GameFlagClear(535);
    Engine_ActorSetPosition(8, 0, 0);
}

void InspectFirewood(void)
{
    extern u8 Data_02000240[];

    Engine_GameFlagSet(2373);
    Func_020047c2();
    Engine_ActorSetPosition(9, 0, 0);
}

void InspectBooks(void)
{
    extern u8 Data_02000240[];

    Engine_GameFlagSet(2374);
    Engine_GameFlagClear(536);
    Engine_ActorSetPosition(10, 0, 0);
}

void NoOpInteractionCallback(void)
{
    extern s16 Data_02000240[];
    extern u8 *Data_03001ebc;

}

void FieldScene_RunScene3bf_0200252c(void)
{
    struct FieldActor *actor;

    actor = (struct FieldActor *)Value1_0200252c(Func_02007b82, 0);
    if (actor != NULL) {
        Engine_ActorSetPosition(2, actor->x.fixed, actor->z.fixed);
    }
    actor = (struct FieldActor *)Value1_0200252c(Func_02007b96, 0);
    if (actor != NULL) {
        Engine_ActorSetPosition(3, actor->x.fixed, actor->z.fixed);
    }
    actor = (struct FieldActor *)Value1_0200252c(Func_02007baa, 0);
    if (actor != NULL) {
        Engine_ActorSetPosition(1, actor->x.fixed, actor->z.fixed);
    }
    Func_02007c7a(0, 0, 0);
    Call3(Engine_ActorSetSpeed, 2, 0xb333, 0x5999);
    Call3(Engine_ActorWalkTo, 2, 0x1c8, 192);
    Call3(Engine_ActorSetSpeed, 3, 0xb333, 0x5999);
    Call3(Engine_ActorWalkTo, 3, 0x1b8, 184);
    Call3(Engine_ActorSetSpeed, 1, 0xb333, 0x5999);
    Call3(Engine_ActorWalkTo, 1, 0x1c0, 240);
    Engine_ActorWaitForMove(2);
    Engine_ActorFaceActor(2, 12, 0);
    Engine_ActorWaitForMove(1);
    Engine_ActorWaitForMove(3);
    Engine_ActorFaceActor(1, 12, 0);
    Engine_ActorFaceActor(3, 12, 0);
    Engine_EventWait(15);
}

void FieldScene_RunScene3bf_020025f8(void)
{
    extern u8 Data_03001ebc[];

    u32 i;
    s32 record;

    Call3(Engine_WorkSetValuesIfNonNegative, 0x40000, 0x40000, 0x10000);
    Engine_AudioPlayCue(141);
    Engine_EventWait(80);
    Call1_020025f8(Engine_AudioPlayCue, 0x120);
    Engine_EventWait(5);
    Engine_AudioPlayCue(145);
    Call6(Engine_MapCopyCells, 16, 75, 7, 4, 26, 55);
    Call3(Engine_WorkSetValuesIfNonNegative, -1, -1, 0xe666);
    Call3(Engine_ActorShowEmote, 0, 0x100, 0);
    Call3(Engine_ActorShowEmote, 1, 0x100, 0);
    Call3(Engine_ActorShowEmote, 2, 0x100, 0);
    Call3(Engine_ActorShowEmote, 3, 0x100, 0);
    Value3_020025f8(Func_02007da0, 12, 0x100, 0);
    Engine_EventWait(60);
}

void FieldScene_RunScene3bf_0200269c(void)
{
    extern u8 Data_03001ebc[];

    u32 i;
    s32 record;

    Engine_CameraMoveToActor(11, 1);
    Engine_CameraWaitForMove();
    Engine_EventWait(60);
    Call1_0200269c(Engine_EventSetMessage, 0x247c);
    Engine_EventShowMessage(13, 0);
    Call3(Engine_ActorSetSpeed, 11, 0x10000, 0x8000);
    Call3(Engine_ActorSetSpeed, 15, 0x10000, 0x8000);
    Call3(Engine_ActorWalkTo, 11, 0x1d8, 180);
    Call3(Engine_ActorWalkTo, 15, 0x1d8, 180);
    Engine_CameraFollowActor(11, 1);
    Engine_ActorWaitForMove(11);
    Engine_ActorSetAnimation(11, 4);
    Engine_EventWait(30);
}

void FieldScene_RunScene3bf_02002718(void)
{
    extern u8 Data_03001ebc[];

    u32 i;
    s32 record;

    Call3_02002718(Engine_ActorSetSpeed, 2, 0x10000, 0x8000);
    Call3_02002718(Engine_ActorWalkTo, 2, 0x1f8, 216);
    Call3_02002718(Engine_ActorSetSpeed, 3, 0x10000, 0x8000);
    Call3_02002718(Engine_ActorWalkTo, 3, 0x1b8, 232);
    Call3_02002718(Engine_ActorSetSpeed, 1, 0x10000, 0x8000);
    Call3_02002718(Engine_ActorWalkTo, 1, 0x1e0, 224);
    Engine_ActorWaitForMove(1);
    Call3_02002718(Func_02007e80, 1, 0xc000, 0);
    Engine_ActorWaitForMove(2);
    Call3_02002718(Func_02007e92, 2, 0xc000, 0);
    Engine_ActorWaitForMove(3);
    Call3_02002718(Func_02007ea4, 3, 0xc000, 0);
    Call3_02002718(Func_02007eb0, 0, 0xc000, 0);
}

/* Runs one of two long fixed sequences (chosen by a status check), each
 * placing actors, panning/animating them, and running dialogue lines built
 * from a base text pointer plus a running line offset. */
void PlayStoryScene(void)
{
    extern u8 Data_02000240[];

    s32 text_line;

    if (GameFlag_IsSet_1(769) != 0) {
        Func_020057ec();
    }
    GameFlag_Set_1(624);
    BattleRuntime_Reset_1();
    if (GameFlag_IsSet_2(2370) != 0) {
        ObjectMotion_SetSpeedParameters_1(0, 32768, 16384);
        ObjectMotion_SetPositionAndReset_1(0, 456, 216);
        ObjectMotion_SetAngleToward_1(0, 12, 0);
        Func_02004d2a();
        BattleEffect_SpawnLinkedResourceObject_1(12, 256, 60);
        ObjectMotion_ArmCallback_1(12, 32768, 0);
        ObjectMotion_Launch_1(12, 4, 0);
        Func_02007e14(Scene_GetRecord_1(12), 1);
        BattleRuntime_WaitIfModeZero_1(30);
        ObjectMotion_SetSpeedParameters_2(2, 45875, 22937);
        ObjectMotion_ResetAndSetPositionInMode2_1(2, 464, 192);
        ObjectMotion_CommitCurrentPositionAndActivate_1(2);
        BattleRuntime_WaitIfModeZero_2(30);
        ObjectMotion_ArmCallback_2(0, 16384, 0);
        ObjectMotion_ArmCallback_3(2, 16384, 0);
        ObjectMotion_ArmCallback_4(1, 16384, 0);
        ObjectMotion_ArmCallback_5(3, 16384, 0);
        ObjectMotion_SetHorizontalPositionWithTerrain_1(13, 29884416, 20971520);
        ObjectMotion_SetSpeedLimitAndAcceleration_1(131072, 16384);
        text_line = (s32)&Value_0000247d;
        SceneWork_SetStepValue_1(text_line);
        BattleEvent_RunActionAndWait_1(13, 0);
        ObjectMotion_ResetAndSetPositionInMode2_2(13, 458, 272);
        ObjectMotion_CommitCurrentPositionAndActivate_2(13);
        ObjectMotion_ArmCallback_6(13, 20480, 0);
        BattleRuntime_WaitIfModeZero_3(40);
        ObjectMotion_OffsetPositionAndResetMotion_1(13, -8, 8);
        ObjectMotion_CommitCurrentPositionAndActivate_3(13);
        BattleRuntime_WaitIfModeZero_4(60);
        Audio_PlayCue_1(155);
        UiText_DrawMessage_1(text_line + 1, 1);
        ObjectMotion_OffsetPositionAndResetMotion_2(13, 8, -8);
        Func_02004f02();
        BattleRuntime_WaitIfModeZero_5(120);
        ObjectMotion_SetVariantCallback_1(0, 2);
        ObjectMotion_SetVariantCallback_2(2, 2);
        ObjectMotion_SetVariantCallback_3(1, 2);
        ObjectMotion_SetVariantCallback_4(3, 2);
        BattleRuntime_WaitIfModeZero_6(20);
        text_line += 2;
        Func_02005054();
        ObjectMotion_SetAngleToward_2(13, 0, 0);
        SceneWork_SetStepValue_2(text_line);
        BattleEvent_RunActionAndWait_2(13, 0);
        Func_02004ff4();
        SHARED_RECORD_FIELD_448 = 512;
        BattleRuntime_WaitIfModeZero_7(1);
        do {
            STATE_TABLE_022B = 3;
        } while (0);
        SharedWorkData_SetFirstAndSecondFields_1((s32)&Value_000000a3, 4);
        BattleEffect_ComputeWeightedResultAndDispatch_1(98, 4);
    } else {
        Object_SetModeById_1(0, 1);
        Audio_PlayCue_2(17);
        BattleRuntime_WaitIfModeZero_8(30);
        text_line = (s32)&Value_00002464;
        SceneWork_SetStepValue_3(text_line);
        BattleEvent_RunActionAndWait_3(12, 0);
        Object_LinkPair_1(0, 12, 0);
        BattleRuntime_WaitIfModeZero_9(140);
        ObjectMotion_ArmCallback_7(12, 32768, 0);
        ObjectMotion_Launch_2(12, 4, 0);
        Func_02007fec(Scene_GetRecord_2(12), 1);
        SceneWork_SetStepValue_4(text_line + 1);
        BattleEvent_RunActionAndWait_4(12, 0);
        ObjectMotion_SetSpeedParameters_3(0, 32768, 16384);
        ObjectMotion_SetPositionAndReset_2(0, 456, 216);
        ObjectMotion_SetAngleToward_3(0, 12, 0);
        Func_02004f66();
        ObjectMotion_SetSpeedParameters_4(2, 45875, 22937);
        ObjectMotion_ResetAndSetPositionInMode2_3(2, 464, 192);
        ObjectMotion_CommitCurrentPositionAndActivate_4(2);
        BattleRuntime_WaitIfModeZero_10(30);
        SceneWork_SetStepValue_5(text_line + 2);
        BattleEvent_RunActionAndWait_5(2, 0);
        BattleEffect_SpawnLinkedResourceObject_2(12, 256, 0);
        BattleRuntime_WaitIfModeZero_11(110);
        Audio_PlayCue_3(60);
        SceneWork_SetStepValue_6(text_line + 3);
        BattleEvent_RunActionAndWait_6(12, 0);
        BattleRuntime_WaitIfModeZero_12(30);
        ObjectMotion_CallThenWaitForAnimationChange_1(2, 3);
        BattleRuntime_WaitIfModeZero_13(20);
        Object_SetModeById_2(2, 1);
        ObjectMotion_SetVariantCallback_5(12, 1);
        BattleRuntime_WaitIfModeZero_14(20);
        ObjectMotion_SetSpeedParameters_5(12, 26214, 13107);
        ObjectMotion_ResetAndSetPositionInMode2_4(12, 520, 208);
        ObjectMotion_CommitCurrentPositionAndActivate_5(12);
        Object_SetModeById_3(12, 1);
        BattleRuntime_WaitIfModeZero_15(20);
        ObjectMotion_ArmCallback_8(12, 45056, 0);
        BattleRuntime_WaitIfModeZero_16(30);
        ObjectMotion_ArmCallback_9(12, 20480, 0);
        BattleRuntime_WaitIfModeZero_17(30);
        ObjectMotion_SetAngleToward_4(12, 2, 0);
        BattleRuntime_WaitIfModeZero_18(20);
        SceneWork_SetStepValue_7(text_line + 4);
        BattleEvent_RunActionAndWait_7(12, 0);
        BattleRuntime_WaitIfModeZero_19(40);
        ObjectMotion_CallThenWaitForAnimationChange_2(2, 3);
        BattleRuntime_WaitIfModeZero_20(20);
        BattleEffect_SpawnLinkedResourceObject_3(12, 264, 0);
        BattleRuntime_WaitIfModeZero_21(120);
        SceneWork_SetStepValue_8(text_line + 5);
        BattleEvent_RunActionAndWait_8(12, 0);
        BattleRuntime_WaitIfModeZero_22(25);
        ObjectMotion_CallThenWaitForAnimationChange_3(2, 3);
        BattleRuntime_WaitIfModeZero_23(30);
        ObjectMotion_CallThenWaitForAnimationChange_4(12, 3);
        BattleRuntime_WaitIfModeZero_24(40);
        ObjectMotion_ResetAndSetPositionInMode2_5(2, 480, 200);
        ObjectMotion_CommitCurrentPositionAndActivate_6(2);
        Object_LinkPair_2(2, 12, 0);
        BattleRuntime_WaitIfModeZero_25(60);
        SceneWork_SetStepValue_9(text_line + 6);
        BattleEvent_RunActionAndWait_9(2, 0);
        BattleRuntime_WaitIfModeZero_26(20);
        Object_SetModeById_4(12, 4);
        BattleRuntime_WaitIfModeZero_27(80);
        SceneWork_SetStepValue_10(text_line + 7);
        BattleEvent_RunActionAndWait_10(12, 0);
        ObjectMotion_SetHorizontalPositionWithTerrain_2(13, 29884416, 20971520);
        Audio_PlayCue_4(19);
        SceneWork_SetStepValue_11(text_line + 8);
        BattleEvent_RunActionAndWait_11(13, 0);
        ObjectMotion_SetAngleToward_5(0, 13, 0);
        ObjectMotion_SetAngleToward_6(2, 13, 0);
        ObjectMotion_SetAngleToward_7(1, 13, 0);
        BattleRuntime_WaitIfModeZero_28(5);
        ObjectMotion_ArmCallback_10(3, 16384, 0);
        ObjectMotion_SetAngleToward_8(12, 13, 0);
        BattleRuntime_WaitIfModeZero_29(30);
        Audio_PlayCue_5(61);
        ObjectMotion_SetSpeedLimitAndAcceleration_2(131072, 16384);
        ObjectMotion_PlaceLookedUpObjectWithinCameraBounds_1(13, 1);
        Object_CommitPositionThenWaitIfModeZero_1();
        ObjectMotion_SetSpeedParameters_6(13, 52428, 26214);
        ObjectMotion_ResetAndSetPositionInMode2_6(13, 456, 304);
        ObjectMotion_SetPositionAndReset_3(13, 1);
        ObjectMotion_CommitCurrentPositionAndActivate_7(13);
        ObjectMotion_SetPositionAndReset_4(1, 1);
        BattleEffect_SpawnLinkedResourceObject_4(0, 258, 0);
        BattleEffect_SpawnLinkedResourceObject_5(2, 258, 0);
        BattleEffect_SpawnLinkedResourceObject_6(1, 258, 0);
        BattleEffect_SpawnLinkedResourceObject_7(3, 258, 0);
        BattleEffect_SpawnLinkedResourceObject_8(12, 258, 0);
        BattleRuntime_WaitIfModeZero_30(60);
        ObjectMotion_SetAngleToward_9(12, 13, 0);
        ObjectMotion_SetVariantCallback_6(12, 2);
        BattleRuntime_WaitIfModeZero_31(60);
        SceneWork_SetStepValue_12(text_line + 9);
        BattleEvent_RunActionAndWait_12(12, 0);
        ObjectMotion_SetAngleToward_10(13, 12, 0);
        SceneWork_SetStepValue_13(text_line + 10);
        BattleEvent_RunActionAndWait_13(13, 0);
        BattleRuntime_WaitIfModeZero_32(60);
        ObjectMotion_SetAngleToward_11(13, 2, 0);
        BattleRuntime_WaitIfModeZero_33(30);
        SceneWork_SetStepValue_14(text_line + 11);
        BattleEvent_RunActionAndWait_14(13, 0);
        Object_LinkPair_3(3, 2, 0);
        Object_LinkPair_4(0, 1, 0);
        BattleRuntime_WaitIfModeZero_34(60);
        ObjectMotion_SetAngleToward_12(0, 13, 0);
        ObjectMotion_SetAngleToward_13(2, 13, 0);
        ObjectMotion_SetAngleToward_14(1, 13, 0);
        ObjectMotion_SetAngleToward_15(3, 13, 0);
        ObjectMotion_SetVariantCallback_7(13, 1);
        BattleRuntime_WaitIfModeZero_35(60);
        SceneWork_SetStepValue_15(text_line + 12);
        BattleEvent_RunActionAndWait_15(13, 0);
        BattleEffect_SpawnLinkedResourceObject_9(1, 259, 0);
        BattleRuntime_WaitIfModeZero_36(60);
        Object_SetModeById_5(13, 4);
        SceneWork_SetStepValue_16(text_line + 13);
        BattleEvent_RunActionAndWait_16(13, 0);
        ObjectMotion_ResetAndSetPositionInMode2_7(1, 456, 248);
        ObjectMotion_CommitCurrentPositionAndActivate_8(1);
        ObjectMotion_ArmCallback_11(1, 16384, 0);
        SceneWork_SetStepValue_17(text_line + 14);
        BattleEvent_RunActionAndWait_17(1, 0);
        ObjectMotion_ResetAndSetPositionInMode2_8(2, 472, 216);
        ObjectMotion_CommitCurrentPositionAndActivate_9(2);
        ObjectMotion_ArmCallback_12(2, 16384, 0);
        BattleRuntime_WaitIfModeZero_37(10);
        SceneWork_SetStepValue_18(text_line + 15);
        BattleEvent_RunActionAndWait_18(2, 0);
        BattleEffect_SpawnLinkedResourceObject_10(12, 261, 0);
        BattleRuntime_WaitIfModeZero_38(60);
        SceneWork_SetStepValue_19(text_line + 16);
        BattleEvent_RunActionAndWait_19(12, 0);
        ObjectMotion_ResetAndSetPositionInMode2_9(3, 440, 216);
        ObjectMotion_CommitCurrentPositionAndActivate_10(3);
        ObjectMotion_SetAngleToward_16(3, 13, 0);
        ObjectMotion_CallThenWaitForAnimationChange_5(3, 3);
        BattleRuntime_WaitIfModeZero_39(10);
        SceneWork_SetStepValue_20(text_line + 17);
        BattleEvent_RunActionAndWait_20(3, 0);
        Object_SetModeById_6(13, 4);
        SceneWork_SetStepValue_21(text_line + 18);
        BattleEvent_RunActionAndWait_21(13, 0);
        BattleEffect_SpawnLinkedResourceObject_11(0, 258, 0);
        BattleEffect_SpawnLinkedResourceObject_12(1, 258, 0);
        BattleEffect_SpawnLinkedResourceObject_13(3, 258, 0);
        BattleEffect_SpawnLinkedResourceObject_14(2, 258, 0);
        BattleEffect_SpawnLinkedResourceObject_15(13, 264, 0);
        BattleRuntime_WaitIfModeZero_40(60);
        SceneWork_SetStepValue_22(text_line + 19);
        BattleEvent_RunActionAndWait_22(13, 0);
        BattleRuntime_WaitIfModeZero_41(20);
        BattleEffect_SpawnLinkedResourceObject_16(1, 259, 0);
        BattleRuntime_WaitIfModeZero_42(60);
        SceneWork_SetStepValue_23(text_line + 20);
        BattleEvent_RunActionAndWait_23(1, 0);
        ObjectMotion_SetVariantCallback_8(13, 1);
        BattleRuntime_WaitIfModeZero_43(60);
        SceneWork_SetStepValue_24(text_line + 21);
        BattleEvent_RunActionAndWait_24(13, 0);
        ObjectMotion_ResetAndSetPositionInMode2_10(13, 456, 280);
        ObjectMotion_CommitCurrentPositionAndActivate_11(13);
        ObjectMotion_ArmCallback_13(13, 20480, 0);
        BattleRuntime_WaitIfModeZero_44(80);
        ObjectMotion_OffsetPositionAndResetMotion_3(13, -8, 8);
        ObjectMotion_CommitCurrentPositionAndActivate_12(13);
        BattleRuntime_WaitIfModeZero_45(60);
        Audio_PlayCue_6(155);
        UiText_DrawMessage_2((s32)&Value_0000247e, 1);
        ObjectMotion_OffsetPositionAndResetMotion_4(13, 8, -8);
        ObjectMotion_SetAngleToward_17(13, 11, 0);
        Func_02005596();
        Audio_PlayCue_7(52);
        SceneWork_SetStepValue_25(text_line + 23);
        BattleEvent_RunActionAndWait_25(13, 0);
        BattleRuntime_WaitIfModeZero_46(60);
        ObjectMotion_SetAngleToward_18(0, 11, 0);
        ObjectMotion_SetAngleToward_19(1, 11, 0);
        ObjectMotion_SetAngleToward_20(2, 11, 0);
        ObjectMotion_SetAngleToward_21(3, 11, 0);
        ObjectMotion_SetAngleToward_22(12, 11, 0);
        Func_02005708();
        Func_02005690();
        GameFlag_Set_2(2370);
        SHARED_RECORD_FIELD_448 = 512;
        BattleRuntime_WaitIfModeZero_47(1);
        do {
            STATE_TABLE_022B = 3;
        } while (0);
        SharedWorkData_SetFirstAndSecondFields_2((s32)&Value_000000a3, 4);
        BattleEffect_ComputeWeightedResultAndDispatch_2(98, 4);
    }
    BattleRuntime_ScheduleShoulderButtonModeUpdate_1();
}

/* Long fixed sequence of setup, positioning, and per-actor animation calls
 * against actor slots 0-3, 11-15, driven by three script line tables, with
 * two two-way branches on the outcome of a query call. Ends by writing the
 * scene phase word and issuing a final batch of calls. */
void FieldScene_RunMainScriptSequence(void)
{
    u32 i;
    u8 *record;
    s32 script_a;
    s32 script_b;
    s32 script_c;

    GameFlag_Set_1_02003054(0x301);
    GameFlag_Set_2_02003054(0x941);
    Call6(Engine_MapCopyCells, 16, 75, 7, 4, 26, 55);
    Func_02003d0c(4);
    BattleRuntime_Reset_1_02003054();
    record = Scene_GetRecord_1_02003054(12);
    Func_02008674_b(record, 1);
    ObjectMotion_SetHorizontalPositionWithTerrain_1_02003054(0, 0x1c80000, 0xb80000);
    ObjectMotion_ArmCallback_1_02003054(0, 0x4000, 0);
    ObjectMotion_SetHorizontalPositionWithTerrain_2_02003054(1, 0x1b80000, 0xc00000);
    ObjectMotion_ArmCallback_2_02003054(1, 0x4000, 0);
    ObjectMotion_SetHorizontalPositionWithTerrain_3_02003054(3, 0x1e80000, 0xb80000);
    ObjectMotion_ArmCallback_3_02003054(3, 0x4000, 0);
    ObjectMotion_SetHorizontalPositionWithTerrain_4(2, 0x1d80000, 0xb80000);
    ObjectMotion_ArmCallback_4_02003054(2, 0x4000, 0);
    ObjectMotion_SetHorizontalPositionWithTerrain_5(12, 0x2080000, 0xe00000);
    ObjectMotion_ArmCallback_5_02003054(12, 0xb000, 0);
    ObjectMotion_SetHorizontalPositionWithTerrain_6(11, 0x1c00000, 0xed0000);
    ObjectMotion_ArmCallback_6_02003054(11, 0x8000, 0);
    ObjectMotion_SetHorizontalPositionWithTerrain_7(15, 0x1c00000, 0xee0000);
    ObjectMotion_SetActionVariant_1(15, 3);
    Object_SetModeById_1_02003054(11, 3);
    ObjectMotion_SetHorizontalPositionWithTerrain_8(13, 0x1ca0000, 0xf30000);
    ObjectMotion_ArmCallback_7_02003054(13, 0x4000, 0);
    Object_SetModeById_2_02003054(13, 5);
    BattleRuntime_WaitIfModeZero_1_02003054();
    BattleRuntime_WaitIfModeZero_2_02003054(120);
    /* Script line bases are overlay data symbols: an integer base would be
     * constant-propagated into every offset instead of staying in r5. */
    script_a = (s32)Data_00002481;
    SceneWork_SetStepValue_1_02003054(script_a);
    BattleEvent_RunActionAndWait_1_02003054(13, 0);
    BattleEffect_SpawnLinkedResourceObject_1_02003054(1, 0x101, 60);
    SceneWork_SetStepValue_2_02003054((script_a + 1));
    BattleEvent_RunActionAndWait_2_02003054(1, 0);
    BattleEffect_SpawnLinkedResourceObject_2_02003054(0, 0x101, 1);
    BattleEffect_SpawnLinkedResourceObject_3_02003054(2, 0x101, 1);
    BattleEffect_SpawnLinkedResourceObject_4_02003054(3, 0x101, 1);
    BattleRuntime_WaitIfModeZero_3_02003054(60);
    ObjectMotion_SetAngleToward_1_02003054(12, 13, 0);
    BattleRuntime_WaitIfModeZero_4_02003054(60);
    SceneWork_SetStepValue_3_02003054((script_a + 2));
    BattleEvent_RunActionAndWait_3_02003054(12, 0);
    ObjectMotion_SetAngleToward_2_02003054(0, 12, 0);
    ObjectMotion_SetAngleToward_3_02003054(1, 12, 0);
    ObjectMotion_SetAngleToward_4_02003054(2, 12, 0);
    ObjectMotion_SetAngleToward_5_02003054(3, 12, 0);
    BattleRuntime_WaitIfModeZero_5_02003054(60);
    ObjectMotion_ResetAndSetPositionInMode2_1_02003054(12, 0x200, 232);
    Object_SetModeById_3_02003054(12, 4);
    BattleRuntime_WaitIfModeZero_6_02003054(60);
    SceneWork_SetStepValue_4_02003054((script_a + 3));
    BattleEvent_RunActionAndWait_4_02003054(12, 0);
    BattleRuntime_WaitIfModeZero_7_02003054(15);
    ObjectMotion_SetVariantCallback_1_02003054(13, 2);
    SceneWork_SetStepValue_5_02003054((script_a + 4));
    BattleEvent_RunActionAndWait_5_02003054(13, 0);
    ObjectMotion_SetAngleToward_6_02003054(0, 13, 0);
    ObjectMotion_SetAngleToward_7_02003054(2, 13, 0);
    ObjectMotion_SetAngleToward_8_02003054(3, 13, 0);
    ObjectMotion_SetAngleToward_9_02003054(1, 13, 0);
    ObjectMotion_SetVariantCallback_2_02003054(2, 1);
    SceneWork_SetStepValue_6_02003054((script_a + 5));
    BattleEvent_RunActionAndWait_6_02003054(2, 0);
    BattleEffect_SpawnLinkedResourceObject_5_02003054(3, 0x105, 0);
    Object_SetModeById_4_02003054(3, 4);
    BattleRuntime_WaitIfModeZero_8_02003054(80);
    Object_SetModeById_5_02003054(3, 1);
    SceneWork_SetStepValue_7_02003054((script_a + 6));
    BattleEvent_RunActionAndWait_7_02003054(3, 0);
    BattleEffect_SpawnLinkedResourceObject_6_02003054(12, 0x102, 65);
    ObjectMotion_SetVariantCallback_3_02003054(12, 2);
    BattleRuntime_WaitIfModeZero_9_02003054(100);
    SceneWork_SetStepValue_8_02003054((script_a + 7));
    BattleEvent_RunActionAndWait_8_02003054(13, 0);
    ObjectMotion_SetAngleToward_10_02003054(1, 0, 0);
    BattleRuntime_WaitIfModeZero_10_02003054(10);
    SceneWork_SetStepValue_9_02003054((script_a + 8));
    BattleEvent_RunActionAndWait_9_02003054(1, 0);
    ObjectMotion_SetVariantCallback_4_02003054(2, 1);
    BattleRuntime_WaitIfModeZero_11_02003054(30);
    SceneWork_SetStepValue_10_02003054((script_a + 9));
    BattleEvent_RunActionAndWait_10_02003054(2, 0);
    ObjectMotion_SetAngleToward_11_02003054(3, 0, 0);
    SceneWork_SetStepValue_11_02003054((script_a + 10));
    ObjectMotion_SetSpeedParameters_1_02003054(3, 0);
    if (UiWork_WaitThenFinalizeCapacity_1(0, 0) == 0) {
        Object_SetModeById_6_02003054(2, 3);
        Object_SetModeById_7(1, 3);
        Object_SetModeById_8(3, 3);
        ObjectMotion_SetAngleToward_12_02003054(12, 13, 0);
        BattleRuntime_WaitIfModeZero_12_02003054(60);
    } else {
        SceneWork_SetStepValue_12_02003054((script_a + 11));
        BattleEvent_RunActionAndWait_11_02003054(3, 0);
        ObjectMotion_SetAngleToward_13_02003054(0, 12, 0);
        ObjectMotion_SetAngleToward_14_02003054(1, 12, 0);
        ObjectMotion_SetAngleToward_15_02003054(2, 12, 0);
        ObjectMotion_SetAngleToward_16_02003054(3, 12, 0);
        BattleRuntime_WaitIfModeZero_13_02003054(20);
        ObjectMotion_SetAngleToward_17_02003054(12, 0, 0);
        BattleRuntime_WaitIfModeZero_14_02003054(60);
        ObjectMotion_SetAngleToward_18_02003054(12, 13, 0);
        BattleRuntime_WaitIfModeZero_15_02003054(80);
        ObjectMotion_CallThenWaitForAnimationChange_1_02003054(12, 3);
        BattleRuntime_WaitIfModeZero_16_02003054(30);
        ObjectMotion_SetAngleToward_19_02003054(12, 0, 0);
        SceneWork_SetStepValue_13_02003054((script_a + 12));
        BattleEvent_RunActionAndWait_12_02003054(12, 0);
        BattleRuntime_WaitIfModeZero_17_02003054(60);
        ObjectMotion_SetAngleToward_20_02003054(2, 12, 0);
        ObjectMotion_SetAngleToward_21_02003054(1, 12, 0);
        ObjectMotion_SetAngleToward_22_02003054(3, 12, 0);
        ObjectMotion_SetAngleToward_23(0, 12, 0);
        Object_SetModeById_9(2, 3);
        Object_SetModeById_10(1, 3);
        Object_SetModeById_11(3, 3);
        Object_SetModeById_12(0, 3);
        BattleRuntime_WaitIfModeZero_18_02003054(80);
        ObjectMotion_SetAngleToward_24(12, 13, 0);
        ObjectMotion_SetAngleToward_25(2, 13, 0);
        ObjectMotion_SetAngleToward_26(1, 13, 0);
        ObjectMotion_SetAngleToward_27(3, 13, 0);
        ObjectMotion_SetAngleToward_28(0, 13, 0);
    }
    ObjectMotion_SetSpeedParameters_2_02003054(0, 0x8000, 0x4000);
    ObjectMotion_SetSpeedParameters_3_02003054(1, 0x8000, 0x4000);
    ObjectMotion_SetSpeedParameters_4_02003054(3, 0x8000, 0x4000);
    ObjectMotion_SetSpeedParameters_5_02003054(2, 0x8000, 0x4000);
    ObjectMotion_ResetAndSetPositionInMode2_2_02003054(1, 0x1a0, 216);
    ObjectMotion_CommitCurrentPositionAndActivate_1_02003054(1);
    ObjectMotion_ResetAndSetPositionInMode2_3_02003054(1, 0x1a0, 248);
    ObjectMotion_CommitCurrentPositionAndActivate_2_02003054(1);
    ObjectMotion_ResetAndSetPositionInMode2_4_02003054(1, 0x1b8, 248);
    ObjectMotion_CommitCurrentPositionAndActivate_3_02003054(1);
    ObjectMotion_ArmCallback_8_02003054(1, 0xc000, 0);
    ObjectMotion_ResetAndSetPositionInMode2_5_02003054(0, 0x1b8, 216);
    ObjectMotion_CommitCurrentPositionAndActivate_4_02003054(0);
    ObjectMotion_ArmCallback_9_02003054(0, 0, 0);
    ObjectMotion_ResetAndSetPositionInMode2_6_02003054(3, 0x1e8, 248);
    ObjectMotion_CommitCurrentPositionAndActivate_5_02003054(3);
    ObjectMotion_ResetAndSetPositionInMode2_7_02003054(3, 0x1c8, 248);
    ObjectMotion_CommitCurrentPositionAndActivate_6_02003054(3);
    ObjectMotion_ArmCallback_10_02003054(3, 0xc000, 0);
    ObjectMotion_ResetAndSetPositionInMode2_8_02003054(2, 0x1c8, 216);
    ObjectMotion_CommitCurrentPositionAndActivate_7_02003054(2);
    ObjectMotion_ArmCallback_11_02003054(2, 0x8000, 0);
    Object_LinkPair_1_02003054(1, 0, 0);
    Object_LinkPair_2_02003054(3, 2, 0);
    BattleRuntime_WaitIfModeZero_19_02003054(60);
    ObjectMotion_CallThenWaitForAnimationChange_2_02003054(0, 3);
    Object_SetModeById_13(2, 3);
    Object_SetModeById_14(1, 3);
    Object_SetModeById_15(3, 3);
    BattleRuntime_WaitIfModeZero_20_02003054(100);
    Audio_PlayCue_1_02003054(226);
    Object_SetModeById_16(13, 7);
    ObjectMotion_SetSpeedParameters_6_02003054(0, 0x1999, 0xccc);
    ObjectMotion_OffsetPositionAndResetMotion_1_02003054(0, -24, 0);
    ObjectMotion_SetSpeedParameters_7(1, 0x1999, 0xccc);
    ObjectMotion_OffsetPositionAndResetMotion_2_02003054(1, -24, 0);
    ObjectMotion_SetSpeedParameters_8(3, 0x1999, 0xccc);
    ObjectMotion_OffsetPositionAndResetMotion_3_02003054(3, -24, 0);
    ObjectMotion_SetSpeedParameters_9(2, 0x1999, 0xccc);
    ObjectMotion_OffsetPositionAndResetMotion_4_02003054(2, -24, 0);
    ObjectMotion_SetSpeedParameters_10(11, 0x1999, 0xccc);
    ObjectMotion_SetSpeedParameters_11(15, 0x1999, 0xccc);
    ObjectMotion_OffsetPositionAndResetMotion_5(11, -24, 0);
    ObjectMotion_OffsetPositionAndResetMotion_6(15, -24, 0);
    ObjectMotion_CommitCurrentPositionAndActivate_8_02003054(0);
    Audio_PlayCue_2_02003054(0x120);
    BattleRuntime_WaitIfModeZero_21_02003054(60);
    ObjectMotion_ArmCallback_12_02003054(0, 0x4000, 0);
    ObjectMotion_ArmCallback_13_02003054(1, 0xc000, 0);
    ObjectMotion_ArmCallback_14(2, 0x4000, 0);
    ObjectMotion_ArmCallback_15(3, 0xc000, 0);
    BattleRuntime_WaitIfModeZero_22_02003054(60);
    Object_SetModeById_17(0, 3);
    Object_SetModeById_18(1, 3);
    Object_SetModeById_19(2, 3);
    Object_SetModeById_20(3, 3);
    BattleRuntime_WaitIfModeZero_23_02003054(100);
    ObjectMotion_SetSpeedParameters_12(0, 0xb333, 0x5999);
    ObjectMotion_SetSpeedParameters_13(1, 0xb333, 0x5999);
    ObjectMotion_SetSpeedParameters_14(2, 0xb333, 0x5999);
    ObjectMotion_SetSpeedParameters_15(3, 0xb333, 0x5999);
    ObjectMotion_ResetAndSetPositionInMode2_9_02003054(0, 0x1c8, 184);
    ObjectMotion_CommitCurrentPositionAndActivate_9_02003054(0);
    Object_SetModeById_21(0, 1);
    ObjectMotion_ArmCallback_16(0, 0x4000, 0);
    ObjectMotion_ResetAndSetPositionInMode2_10_02003054(1, 0x1d0, 0x100);
    ObjectMotion_CommitCurrentPositionAndActivate_10_02003054(1);
    ObjectMotion_ResetAndSetPositionInMode2_11(1, 0x1e0, 248);
    ObjectMotion_CommitCurrentPositionAndActivate_11_02003054(1);
    ObjectMotion_ResetAndSetPositionInMode2_12(1, 0x1b8, 192);
    ObjectMotion_CommitCurrentPositionAndActivate_12_02003054(1);
    Object_SetModeById_22(1, 1);
    ObjectMotion_ArmCallback_17(1, 0x4000, 0);
    ObjectMotion_ResetAndSetPositionInMode2_13(3, 0x1e8, 248);
    ObjectMotion_CommitCurrentPositionAndActivate_13(3);
    ObjectMotion_ResetAndSetPositionInMode2_14(3, 0x1e8, 184);
    ObjectMotion_ResetAndSetPositionInMode2_15(2, 0x1d8, 184);
    ObjectMotion_CommitCurrentPositionAndActivate_14(2);
    Object_SetModeById_23(2, 1);
    ObjectMotion_ArmCallback_18(2, 0x4000, 0);
    ObjectMotion_CommitCurrentPositionAndActivate_15(3);
    Object_SetModeById_24(3, 1);
    ObjectMotion_ArmCallback_19(3, 0x4000, 0);
    BattleRuntime_WaitIfModeZero_24_02003054(30);
    BattleRuntime_WaitIfModeZero_25_02003054(60);
    script_b = (s32)Data_0000248e;
    SceneWork_SetStepValue_14_02003054(script_b);
    BattleEvent_RunActionAndWait_13_02003054(1, 0);
    Object_SetModeById_25(13, 6);
    BattleRuntime_WaitIfModeZero_26_02003054(120);
    ObjectMotion_SetVariantCallback_5_02003054(13, 2);
    BattleRuntime_WaitIfModeZero_27_02003054(60);
    Object_SetModeById_26(13, 7);
    SceneWork_SetStepValue_15_02003054((script_b + 1));
    BattleEvent_RunActionAndWait_14_02003054(13, 0);
    BattleRuntime_WaitIfModeZero_28_02003054(20);
    Object_LinkPair_3_02003054(3, 2, 0);
    BattleRuntime_WaitIfModeZero_29_02003054(10);
    BattleEffect_SpawnLinkedResourceObject_7_02003054(3, 0x101, 80);
    ObjectMotion_SetAngleToward_29(3, 13, 0);
    ObjectMotion_SetAngleToward_30(2, 13, 0);
    SceneWork_SetStepValue_16_02003054((script_b + 2));
    BattleEvent_RunActionAndWait_15_02003054(3, 0);
    BattleEffect_SpawnLinkedResourceObject_8_02003054(13, 0x102, 70);
    ObjectMotion_SetVariantCallback_6_02003054(13, 2);
    BattleRuntime_WaitIfModeZero_30_02003054(60);
    Object_SetModeById_27(13, 5);
    BattleRuntime_WaitIfModeZero_31_02003054(70);
    SceneWork_SetStepValue_17_02003054((script_b + 3));
    BattleEvent_RunActionAndWait_16_02003054(13, 0);
    Object_SetModeById_28(13, 7);
    BattleEffect_SpawnLinkedResourceObject_9_02003054(2, 0x108, 40);
    SceneWork_SetStepValue_18_02003054((script_b + 4));
    BattleEvent_RunActionAndWait_17_02003054(2, 0);
    ObjectMotion_CallThenWaitForAnimationChange_3_02003054(12, 3);
    BattleEffect_SpawnLinkedResourceObject_10_02003054(1, 0x103, 60);
    ObjectMotion_ResetAndSetPositionInMode2_16(1, 0x1b8, 208);
    ObjectMotion_CommitCurrentPositionAndActivate_16(1);
    ObjectMotion_SetAngleToward_31(1, 0, 0);
    SceneWork_SetStepValue_19_02003054((script_b + 5));
    BattleEvent_RunActionAndWait_18_02003054(1, 0);
    BattleRuntime_WaitIfModeZero_32_02003054(20);
    BattleEffect_SpawnLinkedResourceObject_11_02003054(0, 0x101, 0);
    BattleEffect_SpawnLinkedResourceObject_12_02003054(2, 0x101, 0);
    BattleEffect_SpawnLinkedResourceObject_13_02003054(3, 0x101, 0);
    BattleEffect_SpawnLinkedResourceObject_14_02003054(12, 0x101, 0);
    BattleRuntime_WaitIfModeZero_33_02003054(70);
    ObjectMotion_SetAngleToward_32(1, 2, 0);
    BattleEffect_SpawnLinkedResourceObject_15_02003054(1, 0x102, 75);
    ObjectMotion_SetAngleToward_33(1, 0, 0);
    Object_SetModeById_29(1, 4);
    SceneWork_SetStepValue_20_02003054((script_b + 6));
    BattleEvent_RunActionAndWait_19_02003054(1, 0);
    BattleEffect_SpawnLinkedResourceObject_16_02003054(12, 0x101, 0);
    BattleRuntime_WaitIfModeZero_34_02003054(60);
    SceneWork_SetStepValue_21_02003054((script_b + 7));
    BattleEvent_RunActionAndWait_20_02003054(12, 0);
    ObjectMotion_ResetAndSetPositionInMode2_17(1, 0x1c0, 208);
    ObjectMotion_CommitCurrentPositionAndActivate_17(1);
    Object_SetModeById_30(1, 1);
    ObjectMotion_SetAngleToward_34(1, 12, 0);
    SceneWork_SetStepValue_22_02003054((script_b + 8));
    BattleEvent_RunActionAndWait_21_02003054(1, 0);
    ObjectMotion_SetVariantCallback_7_02003054(12, 1);
    BattleRuntime_WaitIfModeZero_35_02003054(60);
    ObjectMotion_SetAngleToward_35(1, 12, 0);
    BattleRuntime_WaitIfModeZero_36_02003054(60);
    SceneWork_SetStepValue_23_02003054((script_b + 9));
    BattleEvent_RunActionAndWait_22_02003054(12, 0);
    Object_SetModeById_31(1, 4);
    BattleRuntime_WaitIfModeZero_37_02003054(60);
    SceneWork_SetStepValue_24_02003054((script_b + 10));
    BattleEvent_RunActionAndWait_23_02003054(1, 0);
    ObjectMotion_SetVariantCallback_8_02003054(2, 1);
    BattleRuntime_WaitIfModeZero_38_02003054(30);
    SceneWork_SetStepValue_25_02003054((script_b + 11));
    BattleEvent_RunActionAndWait_24_02003054(2, 0);
    ObjectMotion_SetAngleToward_36(1, 3, 0);
    BattleRuntime_WaitIfModeZero_39_02003054(20);
    SceneWork_SetStepValue_26((script_b + 12));
    BattleEvent_RunActionAndWait_25_02003054(1, 0);
    BattleRuntime_WaitIfModeZero_40_02003054(30);
    BattleEffect_SpawnLinkedResourceObject_17(3, 0x100, 80);
    ObjectMotion_SetAngleToward_37(3, 1, 0);
    SceneWork_SetStepValue_27((script_b + 13));
    BattleEvent_RunActionAndWait_26(3, 0);
    ObjectMotion_SetAngleToward_38(3, 13, 0);
    SceneWork_SetStepValue_28((script_b + 14));
    BattleEvent_RunActionAndWait_27(3, 0);
    ObjectMotion_SetAngleToward_39(0, 13, 0);
    ObjectMotion_SetAngleToward_40(2, 13, 0);
    ObjectMotion_SetAngleToward_41(1, 13, 0);
    ObjectMotion_SetAngleToward_42(3, 13, 0);
    BattleRuntime_WaitIfModeZero_41_02003054(120);
    BattleEffect_SpawnLinkedResourceObject_18(13, 0x102, 30);
    ObjectMotion_SetVariantCallback_9(13, 1);
    BattleRuntime_WaitIfModeZero_42_02003054(120);
    ObjectMotion_SetVariantCallback_10(12, 1);
    BattleRuntime_WaitIfModeZero_43_02003054(60);
    SceneWork_SetStepValue_29((script_b + 15));
    BattleEvent_RunActionAndWait_28(12, 0);
    BattleEffect_SpawnLinkedResourceObject_19(1, 0x107, 110);
    ObjectMotion_ResetAndSetPositionInMode2_18(1, 0x1c8, 212);
    ObjectMotion_CommitCurrentPositionAndActivate_18(1);
    Object_SetModeById_32(1, 1);
    SceneWork_SetStepValue_30((script_b + 16));
    BattleEvent_RunActionAndWait_29(1, 0);
    ObjectMotion_SetAngleToward_43(2, 1, 0);
    BattleRuntime_WaitIfModeZero_44_02003054(60);
    ObjectMotion_SetVariantCallback_11(2, 1);
    SceneWork_SetStepValue_31((script_b + 17));
    BattleEvent_RunActionAndWait_30(2, 0);
    ObjectMotion_SetAngleToward_44(1, 13, 0);
    BattleRuntime_WaitIfModeZero_45_02003054(80);
    ObjectMotion_SetAngleToward_45(1, 2, 0);
    BattleRuntime_WaitIfModeZero_46_02003054(60);
    ObjectMotion_SetAngleToward_46(1, 13, 0);
    BattleRuntime_WaitIfModeZero_47_02003054(30);
    ObjectMotion_SetAngleToward_47(0, 13, 0);
    ObjectMotion_SetAngleToward_48(3, 13, 0);
    ObjectMotion_SetAngleToward_49(13, 13, 0);
    ObjectMotion_SetAngleToward_50(12, 13, 0);
    BattleRuntime_WaitIfModeZero_48(80);
    ObjectMotion_CallThenWaitForAnimationChange_4_02003054(1, 3);
    BattleRuntime_WaitIfModeZero_49(30);
    SceneWork_SetStepValue_32((script_b + 18));
    BattleEvent_RunActionAndWait_31(1, 0);
    ObjectMotion_SetVariantCallback_12(13, 2);
    BattleRuntime_WaitIfModeZero_50(70);
    SceneWork_SetStepValue_33((script_b + 19));
    BattleEvent_RunActionAndWait_32(13, 0);
    BattleEffect_SpawnLinkedResourceObject_20(3, 0x100, 60);
    SceneWork_SetStepValue_34((script_b + 20));
    BattleEvent_RunActionAndWait_33(3, 0);
    ObjectMotion_SetAngleToward_51(2, 0, 0);
    BattleRuntime_WaitIfModeZero_51(80);
    SceneWork_SetStepValue_35((script_b + 21));
    ObjectMotion_SetSpeedParameters_16(2, 0);
    ObjectMotion_SetAngleToward_52(1, 0, 0);
    ObjectMotion_SetAngleToward_53(3, 0, 0);
    ObjectMotion_SetAngleToward_54(2, 0, 0);
    ObjectMotion_SetAngleToward_55(12, 0, 0);
    ObjectMotion_SetHorizontalPositionWithTerrain_9(14, 0x1c80000, 0x1300000);
    ObjectMotion_SetSpeedParameters_17(14, 0x8000, 0x4000);
    if (UiWork_WaitThenFinalizeCapacity_2(0, 0) == 0) {
        Audio_PlayCue_3_02003054(19);
        SceneWork_SetStepValue_36((script_b + 22));
        BattleEvent_RunActionAndWait_34(14, 0);
    } else {
        Audio_PlayCue_4_02003054(19);
        SceneWork_SetStepValue_37((script_b + 23));
        BattleEvent_RunActionAndWait_35(14, 0);
    }
    ObjectMotion_SetAngleToward_56(0, 14, 0);
    ObjectMotion_SetAngleToward_57(1, 14, 0);
    ObjectMotion_SetAngleToward_58(3, 14, 0);
    ObjectMotion_SetAngleToward_59(2, 14, 0);
    ObjectMotion_SetAngleToward_60(12, 14, 0);
    ObjectMotion_SetAngleToward_61(13, 14, 0);
    ObjectMotion_PlaceWithinCameraBounds_1(0x1c80000, -1, 0xf00000, 1);
    ObjectMotion_SetVariantCallback_13(13, 1);
    BattleRuntime_WaitIfModeZero_52(60);
    Audio_PlayCue_5_02003054(8);
    script_c = (s32)Data_000024a6;
    SceneWork_SetStepValue_38(script_c);
    BattleEvent_RunActionAndWait_36(13, 0);
    ObjectMotion_ResetAndSetPositionInMode2_19(14, 0x1c8, 0x118);
    ObjectMotion_CommitCurrentPositionAndActivate_19(14);
    ObjectMotion_ResetAndSetPositionInMode2_20(14, 0x1b8, 0x100);
    ObjectMotion_PlaceWithinCameraBounds_2(0x1c80000, -1, 0xe00000, 1);
    SceneWork_SetStepValue_39((script_c + 1));
    BattleEvent_RunActionAndWait_37(14, 0);
    ObjectMotion_CommitCurrentPositionAndActivate_20(14);
    ObjectMotion_ArmCallback_20(14, 0xd000, 0);
    BattleEffect_SpawnLinkedResourceObject_21(13, 0x102, 80);
    Object_SetModeById_33(14, 4);
    BattleRuntime_WaitIfModeZero_53(89);
    SceneWork_SetStepValue_40((script_c + 2));
    BattleEvent_RunActionAndWait_38(14, 0);
    ObjectMotion_SetVariantCallbackAndInvokeObject_1_02003054(13, 2);
    ObjectMotion_CallThenWaitForAnimationChange_5_02003054(14, 3);
    SceneWork_SetStepValue_41((script_c + 3));
    BattleEvent_RunActionAndWait_39(14, 0);
    BattleEffect_SpawnLinkedResourceObject_22(13, 0x100, 80);
    Object_SetModeById_34(14, 4);
    BattleRuntime_WaitIfModeZero_54(80);
    ObjectMotion_ArmCallback_21(14, 0x3000, 0);
    BattleRuntime_WaitIfModeZero_55(20);
    SceneWork_SetStepValue_42((script_c + 4));
    BattleEvent_RunActionAndWait_40(14, 0);
    ObjectMotion_SetVariantCallback_14(13, 1);
    SceneWork_SetStepValue_43((script_c + 5));
    BattleEvent_RunActionAndWait_41(13, 0);
    BattleRuntime_WaitIfModeZero_56(30);
    ObjectMotion_CallThenWaitForAnimationChange_6(14, 3);
    ObjectMotion_SetAngleToward_62(14, 13, 0);
    BattleRuntime_WaitIfModeZero_57(20);
    SceneWork_SetStepValue_44((script_c + 6));
    BattleEvent_RunActionAndWait_42(14, 0);
    ObjectMotion_SetVariantCallback_15(13, 1);
    BattleEffect_SpawnLinkedResourceObject_23(13, 0x102, 80);
    SceneWork_SetStepValue_45((script_c + 7));
    BattleEvent_RunActionAndWait_43(13, 0);
    BattleEffect_SpawnLinkedResourceObject_24(14, 0x103, 60);
    SceneWork_SetStepValue_46((script_c + 8));
    BattleEvent_RunActionAndWait_44(14, 0);
    ObjectMotion_CallThenWaitForAnimationChange_7(14, 3);
    SceneWork_SetStepValue_47((script_c + 9));
    BattleEvent_RunActionAndWait_45(14, 0);
    BattleRuntime_WaitIfModeZero_58(20);
    BattleRuntime_WaitIfModeZero_59(13, 0x101);
    BattleRuntime_WaitIfModeZero_60(80);
    BattleRuntime_WaitIfModeZero_61(13, 0);
    ObjectMotion_ResetAndSetPositionInMode2_21(14, 0x1f0, 240);
    ObjectMotion_CommitCurrentPositionAndActivate_21(14);
    ObjectMotion_SetAngleToward_63(14, 12, 0);
    BattleRuntime_WaitIfModeZero_62(20);
    ObjectMotion_SetAngleToward_64(12, 14, 0);
    SceneWork_SetStepValue_48((script_c + 10));
    BattleEvent_RunActionAndWait_46(14, 0);
    ObjectMotion_SetVariantCallback_16(12, 1);
    SceneWork_SetStepValue_49((script_c + 11));
    BattleEvent_RunActionAndWait_47(12, 0);
    BattleRuntime_WaitIfModeZero_63(40);
    ObjectMotion_CallThenWaitForAnimationChange_8(14, 3);
    BattleRuntime_WaitIfModeZero_64(20);
    SceneWork_SetStepValue_50((script_c + 12));
    BattleEvent_RunActionAndWait_48(14, 0);
    Object_SetModeById_35(14, 3);
    BattleEffect_SpawnLinkedResourceObject_25(12, 0x102, 60);
    SceneWork_SetStepValue_51((script_c + 13));
    BattleEvent_RunActionAndWait_49(12, 0);
    BattleEffect_SpawnLinkedResourceObject_26(14, 0x100, 70);
    SceneWork_SetStepValue_52((script_c + 14));
    BattleEvent_RunActionAndWait_50(14, 0);
    Object_SetModeById_36(12, 3);
    BattleRuntime_WaitIfModeZero_65(140);
    Object_SetModeById_37(14, 3);
    BattleRuntime_WaitIfModeZero_66(120);
    Object_LinkPair_4_02003054(0, 14, 0);
    Object_SetModeById_38(0, 3);
    Object_SetModeById_39(2, 3);
    Object_SetModeById_40(1, 3);
    Object_SetModeById_41(3, 3);
    BattleRuntime_WaitIfModeZero_67(120);
    BattleEffect_SpawnLinkedResourceObject_27(14, 0x108, 180);
    SceneWork_SetStepValue_53((script_c + 15));
    BattleEvent_RunActionAndWait_51(14, 0);
    BattleEffect_SpawnLinkedResourceObject_28(12, 0x101, 80);
    SceneWork_SetStepValue_54((script_c + 16));
    BattleEvent_RunActionAndWait_52(12, 0);
    Object_LinkPair_5(0, 1, 0);
    Object_LinkPair_6(3, 2, 0);
    BattleRuntime_WaitIfModeZero_68(80);
    ObjectMotion_SetAngleToward_65(14, 0, 0);
    BattleRuntime_WaitIfModeZero_69(60);
    ObjectMotion_SetAngleToward_66(0, 14, 0);
    ObjectMotion_SetAngleToward_67(1, 14, 0);
    ObjectMotion_SetAngleToward_68(2, 14, 0);
    ObjectMotion_SetAngleToward_69(3, 14, 0);
    BattleRuntime_WaitIfModeZero_70(60);
    SceneWork_SetStepValue_55((script_c + 17));
    BattleEvent_RunActionAndWait_53(14, 0);
    BattleEffect_SpawnLinkedResourceObject_29(0, 0x101, 0);
    BattleEffect_SpawnLinkedResourceObject_30(1, 0x101, 0);
    BattleEffect_SpawnLinkedResourceObject_31(2, 0x101, 0);
    BattleEffect_SpawnLinkedResourceObject_32(3, 0x101, 0);
    BattleRuntime_WaitIfModeZero_71(100);
    SceneWork_SetStepValue_56((script_c + 18));
    BattleEvent_RunActionAndWait_54(1, 0);
    ObjectMotion_SetVariantCallback_17(2, 1);
    BattleRuntime_WaitIfModeZero_72(60);
    SceneWork_SetStepValue_57((script_c + 19));
    BattleEvent_RunActionAndWait_55(2, 0);
    ObjectMotion_SetAngleToward_70(14, 2, 0);
    BattleRuntime_WaitIfModeZero_73(20);
    SceneWork_SetStepValue_58((script_c + 20));
    BattleEvent_RunActionAndWait_56(14, 0);
    BattleEffect_SpawnLinkedResourceObject_33(0, 0x101, 0);
    BattleEffect_SpawnLinkedResourceObject_34(1, 0x101, 0);
    BattleEffect_SpawnLinkedResourceObject_35(2, 0x101, 0);
    BattleEffect_SpawnLinkedResourceObject_36(3, 0x101, 0);
    BattleRuntime_WaitIfModeZero_74(120);
    Object_SetModeById_42(14, 4);
    BattleRuntime_WaitIfModeZero_75(120);
    SceneWork_SetStepValue_59((script_c + 21));
    BattleEvent_RunActionAndWait_57(14, 0);
    BattleEffect_SpawnLinkedResourceObject_37(14, 0x102, 90);
    SceneWork_SetStepValue_60((script_c + 22));
    BattleEvent_RunActionAndWait_58(14, 0);
    Object_SetModeById_43(0, 3);
    Object_SetModeById_44(2, 3);
    Object_SetModeById_45(3, 3);
    Object_SetModeById_46(1, 3);
    Object_SetModeById_47(12, 3);
    BattleRuntime_WaitIfModeZero_76(80);
    Object_SetModeById_48(14, 4);
    BattleRuntime_WaitIfModeZero_77(120);
    SceneWork_SetStepValue_61((script_c + 23));
    BattleEvent_RunActionAndWait_59(14, 0);
    BattleEffect_SpawnLinkedResourceObject_38(12, 0x100, 60);
    SceneWork_SetStepValue_62((script_c + 24));
    BattleEvent_RunActionAndWait_60(12, 0);
    ObjectMotion_SetAngleToward_71(14, 12, 0);
    BattleRuntime_WaitIfModeZero_78(20);
    ObjectMotion_SetVariantCallback_18(14, 1);
    BattleRuntime_WaitIfModeZero_79(50);
    SceneWork_SetStepValue_63((script_c + 25));
    BattleEvent_RunActionAndWait_61(14, 0);
    Object_SetModeById_49(1, 3);
    BattleRuntime_WaitIfModeZero_80(70);
    SceneWork_SetStepValue_64((script_c + 26));
    BattleEvent_RunActionAndWait_62(1, 0);
    Object_SetModeById_50(3, 3);
    SceneWork_SetStepValue_65((script_c + 27));
    BattleEvent_RunActionAndWait_63(3, 0);
    ObjectMotion_SetAngleToward_72(14, 0, 0);
    ObjectMotion_CallThenWaitForAnimationChange_9(14, 3);
    BattleRuntime_WaitIfModeZero_81(20);
    SceneWork_SetStepValue_66((script_c + 28));
    BattleEvent_RunActionAndWait_64(14, 0);
    BattleEffect_SpawnLinkedResourceObject_39(2, 0x101, 90);
    SceneWork_SetStepValue_67((script_c + 29));
    BattleEvent_RunActionAndWait_65(2, 0);
    ObjectMotion_ArmCallback_22(14, 0x5000, 0);
    BattleRuntime_WaitIfModeZero_82(20);
    SceneWork_SetStepValue_68((script_c + 30));
    BattleEvent_RunActionAndWait_66(14, 0);
    ObjectMotion_SetVariantCallback_19(12, 1);
    BattleRuntime_WaitIfModeZero_83(60);
    SceneWork_SetStepValue_69((script_c + 31));
    BattleEvent_RunActionAndWait_67(12, 0);
    ObjectMotion_SetAngleToward_73(14, 12, 0);
    BattleRuntime_WaitIfModeZero_84(20);
    ObjectMotion_CallThenWaitForAnimationChange_10(14, 3);
    BattleRuntime_WaitIfModeZero_85(20);
    SceneWork_SetStepValue_70((script_c + 32));
    BattleEvent_RunActionAndWait_68(14, 0);
    BattleEffect_SpawnLinkedResourceObject_40(1, 0x100, 70);
    SceneWork_SetStepValue_71((script_c + 33));
    BattleEvent_RunActionAndWait_69(1, 0);
    ObjectMotion_SetAngleToward_74(14, 0, 0);
    BattleRuntime_WaitIfModeZero_86(30);
    SceneWork_SetStepValue_72((script_c + 34));
    BattleEvent_RunActionAndWait_70(14, 0);
    BattleEffect_SpawnLinkedResourceObject_41(3, 0x100, 80);
    SceneWork_SetStepValue_73((script_c + 35));
    BattleEvent_RunActionAndWait_71(3, 0);
    ObjectMotion_CallThenWaitForAnimationChange_11(14, 3);
    SceneWork_SetStepValue_74((script_c + 36));
    BattleEvent_RunActionAndWait_72(14, 0);
    ObjectMotion_SetAngleToward_75(2, 0, 0);
    BattleRuntime_WaitIfModeZero_87(30);
    Object_SetModeById_51(2, 3);
    BattleRuntime_WaitIfModeZero_88(100);
    SceneWork_SetStepValue_75((script_c + 37));
    ObjectMotion_SetSpeedParameters_18(2, 0);
    if (UiWork_WaitThenFinalizeCapacity_3(0, 0) != 0) {
        SceneWork_SetStepValue_76((script_c + 38));
        BattleEvent_RunActionAndWait_73(14, 0);
    }
    BattleRuntime_WaitIfModeZero_89(30);
    Object_SetModeById_52(1, 3);
    Object_SetModeById_53(2, 3);
    Object_SetModeById_54(3, 3);
    BattleRuntime_WaitIfModeZero_90(100);
    Object_LinkPair_7(0, 12, 0);
    Object_SetModeById_55(0, 3);
    Object_SetModeById_56(12, 3);
    BattleRuntime_WaitIfModeZero_91(100);
    Object_SetModeById_57(1, 2);
    record = Scene_GetRecord_2_02003054(0);
    if (record != 0) {
        ObjectMotion_ResetAndSetPosition_1(1, *(s16 *)(record + 10), *(s16 *)(record + 18));
    }
    ObjectMotion_CommitCurrentPositionAndActivate_22(1);
    ObjectMotion_SetHorizontalPositionWithTerrain_10(1, 0, 0);
    Object_SetModeById_58(2, 2);
    record = Scene_GetRecord_3(0);
    if (record != 0) {
        ObjectMotion_ResetAndSetPosition_2(2, *(s16 *)(record + 10), *(s16 *)(record + 18));
    }
    ObjectMotion_CommitCurrentPositionAndActivate_23(2);
    ObjectMotion_SetHorizontalPositionWithTerrain_11(2, 0, 0);
    Object_SetModeById_59(3, 2);
    record = Scene_GetRecord_4(0);
    if (record != 0) {
        ObjectMotion_ResetAndSetPosition_3(3, *(s16 *)(record + 10), *(s16 *)(record + 18));
    }
    ObjectMotion_CommitCurrentPositionAndActivate_24(3);
    ObjectMotion_SetHorizontalPositionWithTerrain_12(3, 0, 0);
    BattleRuntime_WaitIfModeZero_92(30);
    ObjectMotion_SetSpeedParameters_19(12, 0x6666, 0x3333);
    ObjectMotion_ResetAndSetPositionInMode2_22(12, 0x1d8, 184);
    ObjectMotion_CommitCurrentPositionAndActivate_25(12);
    Object_SetModeById_60(12, 1);
    ObjectMotion_ArmCallback_23(12, 0x3000, 0);
    ObjectMotion_SetAngleToward_76(14, 0, 0);
    Object_SetModeById_61(14, 3);
    BattleRuntime_WaitIfModeZero_93(20);
    ObjectMotion_SetAngleToward_77(14, 13, 0);
    BattleRuntime_WaitIfModeZero_94(20);
    SceneWork_SetStepValue_77((script_c + 39));
    BattleEvent_RunActionAndWait_74(14, 0);
    ObjectMotion_SetVariantCallbackAndInvokeObject_2(13, 2);
    SceneWork_SetStepValue_78((script_c + 40));
    BattleEvent_RunActionAndWait_75(13, 0);
    Object_SetModeById_62(0, 3);
    BattleRuntime_WaitIfModeZero_95(30);
    ObjectMotion_SetSpeedParameters_20(0, 0x10000, 0x8000);
    ObjectMotion_SetSpeedParameters_21(12, 0x10000, 0x8000);
    ObjectMotion_SetPositionAndReset_1_02003054(0, 1);
    ObjectMotion_ResetAndSetPositionInMode2_23(0, 0x1e0, 248);
    BattleRuntime_WaitIfModeZero_96(40);
    ObjectMotion_SetActionVariant_2(12, 0);
    ObjectMotion_SetActionVariant_3(0, 0);
    ObjectMotion_ResetAndSetPositionInMode2_24(12, 0x1e0, 216);
    ObjectMotion_CommitCurrentPositionAndActivate_26(0);
    ObjectMotion_SetAngleToward_78(0, 14, 0);
    ObjectMotion_CommitCurrentPositionAndActivate_27(12);
    ObjectMotion_SetAngleToward_79(12, 14, 0);
    ObjectMotion_ArmCallback_24(12, 0x3000, 0);
    Object_SetModeById_63(0, 3);
    BattleRuntime_WaitIfModeZero_97(5);
    Object_SetModeById_64(12, 3);
    BattleRuntime_WaitIfModeZero_98(100);
    Object_SetModeById_65(14, 3);
    BattleRuntime_WaitIfModeZero_99(100);
    ObjectMotion_ResetAndSetPositionInMode2_25(12, 0x1e0, 248);
    ObjectMotion_ResetAndSetPositionInMode2_26(0, 0x1c8, 248);
    ObjectMotion_CommitCurrentPositionAndActivate_28(12);
    ObjectMotion_ResetAndSetPositionInMode2_27(12, 0x1c8, 248);
    ObjectMotion_CommitCurrentPositionAndActivate_29(0);
    ObjectMotion_ResetAndSetPositionInMode2_28(0, 0x1c8, 0x168);
    ObjectMotion_CommitCurrentPositionAndActivate_30(12);
    ObjectMotion_ResetAndSetPositionInMode2_29(12, 0x1c8, 0x168);
    ObjectMotion_CommitCurrentPositionAndActivate_31(0);
    ObjectMotion_ResetAndSetPositionInMode2_30(0, 0x160, 0x168);
    ObjectMotion_CommitCurrentPositionAndActivate_32(12);
    ObjectMotion_ResetAndSetPositionInMode2_31(12, 0x160, 0x168);
    BattleRuntime_WaitIfModeZero_100(20);
    Audio_PlayCue_6_02003054(17);
    SCENE_PHASE_02003054 = 0x203;
    Engine_EventCloseScreen();
    BattleRuntime_WaitIfModeZero_101(1);
    BattleRuntime_WaitIfModeZero_102(210);
    Engine_EventRequestExit(4);
    BattleRuntime_ScheduleShoulderButtonModeUpdate_1_02003054();
}

void FieldScene_SelectActorTwentyOneMessage(void)
{
    extern s16 Data_02000240[];
    extern u32 Data_0200dfa4;
    extern u8 Data_03001e70[];
    extern u8 Data_03001ebc[];

    switch (Data_0200dfa4) {
    case 0:
        Engine_EventSetMessage(0x2414);
        Engine_EventShowMessage(21, 0);
        break;
    case 1:
        Engine_EventSetMessage(0x2415);
        Engine_EventShowMessage(21, 0);
        break;
    case 2:
        Engine_EventSetMessage(0x2416);
        Engine_EventShowMessage(21, 0);
        break;
    case 3:
        Engine_EventSetMessage(0x2417);
        Engine_EventShowMessage(21, 0);
        break;
    case 4:
        Engine_EventSetMessage(0x2418);
        Engine_EventShowMessage(21, 0);
        break;
    case 6:
        Engine_EventSetMessage(0x241a);
        Engine_EventShowMessage(21, 0);
        break;
    case 7:
        Engine_EventSetMessage(0x241b);
        Engine_EventShowMessage(21, 0);
        break;
    case 5:
        Call3(Func_02009da6, 21, 0xd000, 0);
        Engine_EventWait(50);
        Call3(Func_02009db8, 21, 0xb000, 0);
        Engine_EventWait(50);
        Call3(Func_02009dca, 21, 0x5000, 0);
        Engine_EventWait(50);
        Engine_EventSetMessage(0x2419);
        Engine_EventShowMessage(21, 0);
        break;
    }
}

void FieldScene_RunActorTwentyOneSequence(void)
{
    extern s16 Data_02000240[];
    extern u32 Data_0200dfa4;
    extern u8 Data_03001e70[];
    extern u8 Data_03001ebc[];

    s32 base5_2411;

    Call3(Engine_ActorShowEmote, 21, 0x101, 30);
    Call3(Func_02009e22, 21, 0xd000, 0);
    Engine_EventWait(50);
    Call3(Func_02009e34_a, 21, 0xb000, 0);
    Engine_EventWait(50);
    Call3(Func_02009e46, 21, 0x5000, 0);
    Engine_EventWait(50);
    base5_2411 = (s32)Data_00002411;
    Engine_EventSetMessage(base5_2411);
    Engine_EventShowMessage(21, 0);
    Engine_ActorSetAnimation(21, 4);
    Engine_EventWait(60);
    Call3(Func_02009e76, 21, 0xb000, 0);
    Engine_EventWait(40);
    Engine_EventSetMessage((base5_2411 + 1));
    Engine_EventShowMessage(21, 0);
}

void FieldScene_RunScene3bf_02004794(void)
{
    extern u8 Data_03001ebc[];

    u32 i;
    s32 record;
    s32 base5_244f;
    s32 base5_2455;

    Engine_EventBegin();
    if (Value1(Func_02009dae, 0x941) != 0) {
        Call1_02004794(Engine_EventSetMessage, 0x2566);
        Engine_EventShowMessage(18, 0);
        Engine_EventEnd();
    } else {
        if (Value1(Func_02009dcc, 0x313) != 0) {
            Call1_02004794(Engine_EventSetMessage, 0x2457);
            Value2_02004794(Func_02009ebe, 25, 0);
            Engine_EventEnd();
        } else {
            Call3(Engine_ActorShowEmote, 25, 0x102, 30);
            Engine_ActorFaceActor(25, 0, 0);
            base5_244f = (s32)Data_0000244f;
            Engine_EventSetMessage(base5_244f);
            Engine_EventShowMessage(25, 0);
            Engine_ActorFaceActor(25, 24, 0);
            Engine_CameraMoveToActor(24, 1);
            Engine_CameraWaitForMove();
            Engine_EventWait(60);
            Engine_CameraMoveToActor(0, 1);
            Engine_EventWait(20);
            Call3(Engine_ActorShowEmote, 25, 0x105, 60);
            Engine_EventSetMessage((base5_244f + 1));
            Engine_EventShowMessage(25, 0);
            Call3(Engine_ActorShowEmote, 25, 0x107, 60);
            Engine_EventSetMessage((base5_244f + 2));
            Engine_EventShowMessage(25, 0);
            Engine_EventWait(70);
            Call3(Engine_ActorShowEmote, 25, 0x100, 60);
            Engine_ActorFaceActor(25, 0, 0);
            Engine_EventSetMessage((base5_244f + 3));
            Value2_02004794(Func_02009f6e, 25, 0);
            if (Value2_02004794(Engine_EventChooseYesNo, 0, 0) == 0) {
                Engine_EventSetMessage((base5_244f + 4));
                Func_02009f88_a(25, 0);
            } else {
                Engine_EventSetMessage((base5_244f + 5));
                Func_02009f98_a(25, 0);
            }
            Engine_EventWait(60);
            Call3(Engine_ActorShowEmote, 25, 0x105, 60);
            base5_2455 = (s32)Data_00002455;
            Engine_EventSetMessage(base5_2455);
            Value2_02004794(Func_02009fb8, 25, 0);
            Func_02009f98_b(25, 1);
            Engine_EventSetMessage((base5_2455 + 1));
            Value2_02004794(Func_02009fce, 25, 0);
            Func_02009f98_c(25, 3);
            Engine_EventSetMessage((base5_2455 + 2));
            Value2_02004794(Func_02009fe6, 25, 0);
            Call1_02004794(Engine_GameFlagSet, 0x313);
            Engine_EventEnd();
        }
    }
}

void SelectActor25SceneVariant(void)
{
    extern s16 Data_02000240[];
    extern u8 *Data_03001ebc;

    if (Func_02009f3e(0x941)) {
        Engine_EventSetMessage(0x2568);
        Engine_EventShowMessage(25, 0);
    } else {
        Engine_EventSetMessage(0x2458);
        Engine_EventShowMessage(25, 0);
    }
}

void SelectActor24SceneVariant(void)
{
    extern s16 Data_02000240[];
    extern u8 *Data_03001ebc;

    if (Func_02009f7a(0x941)) {
        Engine_EventSetMessage(0x2569);
        Engine_EventShowMessage(24, 0);
    } else {
        Engine_EventSetMessage(0x244e);
        Engine_EventShowMessage(24, 0);
    }
}

/* Runs a gated sequence of parameterized calls on PRIMARY_ID (24) and, once
 * derived partway through, DERIVED_ID (25); a sequence id counting up from
 * 9569 is threaded through four of the calls one apart. Each of the two
 * outer gating checks has its own short fallback branch on PRIMARY_ID. */
void FieldScene_RunSupplementalSequenceTwo(void)
{
    s32 sequence_id;

    if (GameFlag_IsSet_1_020049a0(2369) != 0) {
        if (GameFlag_IsSet_2_020049a0(2382) == 0 && GameFlag_IsSet_3(788) == 0) {
        sequence_id = 9569;
        SceneWork_SetStepValue_1_020049a0(sequence_id);
        BattleEvent_RunActionAndWait_1_020049a0(PRIMARY_ID, 0);
        ObjectMotion_SetVariantCallbackAndInvokeObject_1_020049a0(PRIMARY_ID, 1);
        BattleRuntime_WaitIfModeZero_1_020049a0(30);
        ObjectMotion_SetSpeedParameters_1_020049a0(PRIMARY_ID, 6553, 3276);
        ObjectMotion_OffsetPositionAndResetMotion_1_020049a0(PRIMARY_ID, -4, 0);
        ObjectMotion_CommitCurrentPositionAndActivate_1_020049a0(PRIMARY_ID);
        Object_SetModeById_1_020049a0(PRIMARY_ID, 3);
        BattleRuntime_WaitIfModeZero_2_020049a0(60);
        ObjectMotion_SetSpeedParameters_2_020049a0(PRIMARY_ID, 13107, 6553);
        ObjectMotion_OffsetPositionAndResetMotion_2_020049a0(PRIMARY_ID, -6, 0);
        ObjectMotion_SetAngleToward_1_020049a0(PRIMARY_ID, 0, 0);
        ObjectMotion_CommitCurrentPositionAndActivate_2_020049a0(PRIMARY_ID);
        SceneWork_SetStepValue_2_020049a0(sequence_id + 1);
        BattleEvent_RunActionAndWait_2_020049a0(PRIMARY_ID, 0);
        ObjectMotion_SetVariantCallbackAndInvokeObject_2_020049a0(PRIMARY_ID, 1);
        ObjectMotion_SetAngleToward_2_020049a0(DERIVED_ID, PRIMARY_ID, 0);
        SceneWork_SetStepValue_3_020049a0(sequence_id + 2);
        BattleEvent_RunActionAndWait_3_020049a0(PRIMARY_ID, 0);
        BattleRuntime_WaitIfModeZero_3_020049a0(70);
        Object_SetModeById_2_020049a0(DERIVED_ID, 3);
        BattleRuntime_WaitIfModeZero_4_020049a0(60);
        ObjectMotion_SetSpeedParameters_3_020049a0(DERIVED_ID, 26214, 13107);
        ObjectMotion_ResetAndSetPositionInMode2_1_020049a0(DERIVED_ID, 880, 112);
        ObjectMotion_CommitCurrentPositionAndActivate_3_020049a0(DERIVED_ID);
        ObjectMotion_ArmCallback_1_020049a0(DERIVED_ID, 53248, 0);
        SceneWork_SetStepValue_4_020049a0(sequence_id + 3);
        BattleEvent_RunActionAndWait_4_020049a0(PRIMARY_ID, 0);
        Object_SetModeById_3_020049a0(PRIMARY_ID, 3);
        BattleRuntime_WaitIfModeZero_5_020049a0(70);
        ObjectMotion_OffsetPositionAndResetMotion_3_020049a0(PRIMARY_ID, 8, 0);
        ObjectMotion_CommitCurrentPositionAndActivate_4_020049a0(PRIMARY_ID);
        Object_SetModeById_4_020049a0(PRIMARY_ID, 5);
        SceneWork_SetStepValue_5_020049a0(sequence_id + 4);
        BattleEvent_RunActionAndWait_5_020049a0(PRIMARY_ID, 0);
        ObjectMotion_ResetAndSetPositionInMode2_2_020049a0(0, 896, 120);
        ObjectMotion_CommitCurrentPositionAndActivate_5_020049a0(0);
        Object_LinkPair_1_020049a0(0, DERIVED_ID, 0);
        BattleRuntime_WaitIfModeZero_6_020049a0(60);
        Object_SetModeById_5_020049a0(DERIVED_ID, 3);
        BattleRuntime_WaitIfModeZero_7_020049a0(30);
        GameFlag_Set_1_020049a0(788);
        } else {
            SceneWork_SetStepValue_6_020049a0(9575);
            BattleEvent_RunActionAndWait_6_020049a0(PRIMARY_ID, 0);
        }
    } else {
        SceneWork_SetStepValue_7_020049a0(0x244d);
        BattleEvent_RunActionAndWait_7_020049a0(PRIMARY_ID, 0);
    }
}

void ConfigureSceneActor26(void)
{
    extern s16 Data_02000240[];
    extern u8 *Data_03001ebc;

    Func_0200a30a(26, 1, 5);
    Engine_GameFlagSet(0x94e);
}

void ConfigureSceneActor14(void)
{
    extern s16 Data_02000240[];
    extern u8 *Data_03001ebc;

    Func_0200a254(14, 2);
    Engine_EventSetMessage(0x2441);
    Engine_EventShowMessage(14, 0);
}

void ConfigureSceneActor13(void)
{
    extern s16 Data_02000240[];
    extern u8 *Data_03001ebc;

    Func_0200a274(13, 2);
    Engine_EventSetMessage((s32)&Value_00002440);
    Engine_EventShowMessage(13, 0);
}

void ConfigureSceneActor12Variant(void)
{
    extern s16 Data_02000240[];
    extern u8 *Data_03001ebc;

    Func_0200a294(12, 2);
    Engine_EventSetMessage(0x243f);
    Engine_EventShowMessage(12, 0);
}

void ConfigureSceneActor18(void)
{
    extern s16 Data_02000240[];
    extern u8 *Data_03001ebc;

    Engine_EventSetMessage((s32)&Value_00002459);
    Engine_EventAskYesNo(18, 0);
}

void RunActor20SceneSequence(void)
{
    extern u8 Data_03001ebc[];

    u32 i;
    s32 record;
    s32 base5_242e;
    s32 base5_2430;

    if (Value1_0200169c(Func_0200a212, 0x226) != 0) {
        Call1_02004bfc(Engine_EventSetMessage, 0x2434);
        Engine_EventShowMessage(20, 0);
    } else {
        Engine_EventBegin();
        Engine_ActorFaceActor(20, 0, 0);
        if (Value1_0200169c(Func_0200a23a, 0x227) == 0) {
            Func_0200a2f0(20, 4, 0);
            Engine_ActorStop(20);
            Func_0200a2ac(20);
            Engine_EventWait(20);
            base5_242e = (s32)Data_0000242e;
            Engine_EventSetMessage(base5_242e);
            Engine_EventShowMessage(20, 0);
            Call3_0200169c(Engine_ActorShowEmote, 20, 0x102, 30);
            Engine_EventSetMessage((base5_242e + 1));
            Engine_EventShowMessage(20, 0);
            Engine_EventWait(30);
            Engine_ActorSetAnimation(20, 4);
            Engine_EventWait(30);
        }
        base5_2430 = (s32)Data_00002430;
        Engine_EventSetMessage(base5_2430);
        Engine_EventShowMessage(20, 0);
        Call3_0200169c(Engine_ActorShowEmote, 20, 0x101, 40);
        Engine_EventSetMessage((base5_2430 + 1));
        Value2(Func_0200a3a2, 20, 0);
        if (Value2(Engine_EventChooseYesNo, 0, 0) == 0) {
            Engine_EventSetMessage((base5_2430 + 2));
            Func_0200a3bc_b(20, 0);
            Call1_02004bfc(Engine_GameFlagSet, 0x226);
        } else {
            Engine_EventSetMessage((base5_2430 + 3));
            Func_0200a3d2(20, 0);
        }
        Call1_02004bfc(Engine_GameFlagSet, 0x227);
        Engine_EventEnd();
    }
}

void FinishActor20SceneSequence(void)
{
    extern s16 Data_02000240[];
    extern u8 *Data_03001ebc;

    if (Func_0200a322(0x226)) {
        Engine_EventSetMessage(0x2435);
        Engine_EventShowMessage(20, 0);
    } else {
        s16 *q = (s16 *)(Data_03001ebc + 382);

        *q = 0;
        Engine_PsynergyCancel();
        Func_02009936();
    }
}

void NoOpActorCallback(void)
{
    extern s16 Data_02000240[];
    extern u8 *Data_03001ebc;

}

void ConfigureActor13Interaction(void)
{
    extern s16 Data_02000240[];
    extern u8 *Data_03001ebc;

    u8 *interaction_resources = &Value_0000256c;

    Engine_EventSetMessage((s32)interaction_resources);
    Engine_EventShowMessage(0x800d, 0);
    if (Func_0200a36e(234) != -1) {
        Engine_MessageShowCentered((s32)(interaction_resources + 2), 1);
    }
}

void ConfigureActor13SceneResource(void)
{
    extern s16 Data_02000240[];
    extern u8 *Data_03001ebc;

    Engine_EventSetMessage(0x256d);
    Engine_EventShowMessage(13, 0);
}

#include "OBJECT_RUNTIME.H"

struct DispatcherEventRuntime {
    u8 unknown_000[0x1c0];
    s32 value_1c0;
};


union DispatcherEventWork {
    struct {
        u8 unknown_000[0x1c0];
        u16 first;
        u16 second;
    } pair;
    u32 words[0x1c4 / 4];
};
extern union DispatcherEventWork Dispatch_Data_02000240;
extern u32 Dispatch_Data_0200dfa4;
extern struct DispatcherEventRuntime *Dispatch_Data_03001ebc;
extern u8 Dispatch_SceneEvent_InstallTasks[];
extern u8 Dispatch_SceneEvent_SetupActors[];
extern u8 Dispatch_SceneEvent_RestoreActors[];
extern u8 Dispatch_SceneEvent_UpdateActors[];
extern void Dispatch_Func_0200d5f8(u32);
extern void Dispatch_Func_0200cf60(void);
extern void Dispatch_Func_0200d0e4(void);
extern void Dispatch_Func_0200d324(void);
extern struct ObjectRuntime *Dispatch_Func_0200d650(s32);
extern void Dispatch_Func_0200d5e8(struct ObjectRuntime *, s32);
extern void Dispatch_Func_0200d708(s32, s32, s32);
extern void Dispatch_Func_0200d6a8(s32, s32);
extern void Dispatch_Func_0200d520(void);
extern void Dispatch_Func_0200b054(void);
extern s32 Dispatch_Func_0200d610(s32);
extern void Dispatch_Func_0200d6a0(s32, s32, s32);

static __inline__ void Scene_Call1(void (*func)(u32), u32 value)
{
    func(value);
}

static __inline__ void Scene_Call3(
    void (*func)(s32, s32, s32), s32 id, s32 x, s32 z)
{
    func(id, x, z);
}

s32 FieldScene_DispatchActorUpdate(void)
{
    struct ObjectRuntime *actor;

    Dispatch_Data_0200dfa4 = (u32)Engine_RandomNext() * 7 >> 16;
    if ((s16)Dispatch_Data_02000240.pair.first == (s32)Dispatch_SceneEvent_InstallTasks) {
        Scene_Call1(Dispatch_Func_0200d5f8, 0xe00);
        Dispatch_Func_0200cf60();
    }
    if ((s16)Dispatch_Data_02000240.pair.first == (s32)Dispatch_SceneEvent_SetupActors)
        Dispatch_Func_0200d0e4();
    if ((s16)Dispatch_Data_02000240.pair.first == (s32)Dispatch_SceneEvent_RestoreActors)
        Dispatch_Func_0200d324();
    if ((s16)Dispatch_Data_02000240.pair.first == (s32)Dispatch_SceneEvent_UpdateActors) {
        Dispatch_Data_03001ebc->value_1c0 = 0x204;
        Dispatch_Func_0200d5e8(Dispatch_Func_0200d650(12), 0);
        Dispatch_Func_0200d708(12, 0, 0);
        Dispatch_Func_0200d6a8(12, 0);
        Dispatch_Func_0200d520();
        actor = Dispatch_Func_0200d650(8);
        if (actor != 0)
            Dispatch_Func_0200d5e8(actor, 0);
        actor->unknown_23 = 2;
        actor = Dispatch_Func_0200d650(9);
        if (actor != 0)
            Dispatch_Func_0200d5e8(actor, 0);
        actor->unknown_23 = 2;
        actor = Dispatch_Func_0200d650(10);
        if (actor != 0)
            Dispatch_Func_0200d5e8(actor, 0);
        actor->unknown_23 = 2;
        Scene_Call1(Dispatch_Func_0200d5f8, 0xe00);
        if ((s16)Dispatch_Data_02000240.pair.second == 4) {
            Scene_Call1(Dispatch_Func_0200d5f8, 0xc00);
            Dispatch_Func_0200b054();
        }
        if ((s16)Dispatch_Data_02000240.pair.second == 3) {
            Scene_Call1(Dispatch_Func_0200d5f8, 0xc00);
            if (Dispatch_Func_0200d610(0x941) != 0) {
                Dispatch_Func_0200d6a0(12, 0, 0);
                Scene_Call3(Dispatch_Func_0200d6a0, 16, 0x1b00000, 0x1580000);
                Scene_Call3(Dispatch_Func_0200d708, 16, 0x5000, 0);
                Scene_Call3(Dispatch_Func_0200d6a0, 13, 0x1c80000, 0x1200000);
                Scene_Call3(Dispatch_Func_0200d708, 13, 0x5000, 0);
                Scene_Call3(Dispatch_Func_0200d6a0, 17, 0x1c80000, 0x1400000);
                Dispatch_Func_0200d5e8(Dispatch_Func_0200d650(17), 0);
            }
        }
        actor = Dispatch_Func_0200d650(15);
        if (actor != 0)
            Dispatch_Func_0200d5e8(actor, 0);
        actor->unknown_23 = 2;
        *(s32 *)&actor->unknown_18[0] = 0xcccc;
    }
    return 0;
}

void FieldScene_InstallSceneTasks(void)
{
    extern s16 Data_02000240[];
    extern u8 Data_03001ebc[];

    Func_0200a3d6();
    switch (Data_02000240[225]) {
    case 2:
    case 3:
    case 4:
    case 5:
    case 6:
    case 7:
        *(s32 *)(*(u8 **)Data_03001ebc + 0x1c0) = 0x200;
        Value2_02004f60(Func_0200a572, (s32)Data_02009719, 3200);
        Value2_02004f60(Func_0200a57c, (s32)Data_0200975d, 3200);
        Value2_02004f60(Func_0200a586, (s32)Data_020097bd, 3200);
        Func_0200a650(0xe00);
        break;
    case 12:
    case 19:
        *(s32 *)(*(u8 **)Data_03001ebc + 0x1c0) = 0x209;
        Func_0200a650(0xc00);
        break;
    case 16:
    case 17:
    case 18:
        *(s32 *)(*(u8 **)Data_03001ebc + 0x1c0) = 0x200;
        Value2_02004f60(Func_0200a5b2, (s32)Data_02009529, 3200);
        Value2_02004f60(Func_0200a5bc, (s32)Data_020099e9, 3200);
        Engine_TaskWait(1);
        Engine_MapRedraw();
        Engine_TaskWait(1);
        Call6(Engine_MapCopyCells, 101, 9, 10, 8, 110, 9);
        Func_0200a650(0xe00);
        break;
    case 13:
    case 14:
    case 15:
        *(s32 *)(*(u8 **)Data_03001ebc + 0x1c0) = 0x200;
        Value2_02004f60(Func_0200a602, (s32)Data_0200969d, 3200);
        break;
    default:
        *(s32 *)(*(u8 **)Data_03001ebc + 0x1c0) = 0x200;
        Func_0200a682(0xe00);
        break;
    }
    Engine_ActorSetChildValue(18, 1);
    Engine_ActorSetChildValue(17, 1);
    Engine_ActorSetChildValue(21, 1);
    Engine_ActorSetChildValue(12, 1);
    Engine_ActorSetChildValue(13, 1);
    Engine_TaskWait(1);
}

void FieldScene_SetupActorsForScene(void)
{
    extern s16 Data_02000240[];
    extern u8 Data_03001ebc[];

    struct ObjectRuntime *actor;

    Func_0200a598();
    Engine_ActorSetChildValue(9, 1);
    Engine_ActorSetChildValue(10, 1);
    Engine_ActorSetChildValue(17, 1);
    if (Func_0200a716(0x94c)) {
        Engine_ActorSetPosition(15, 0, 0);
    }
    if (Func_0200a72a(0x949)) {
        Engine_ActorSetPosition(11, 0, 0);
    }
    if (Func_0200a73e(0x94b)) {
        Engine_ActorSetPosition(16, 0, 0);
    }
    if (Func_0200a752(0xf2e)) {
        Engine_ActorSetPosition(8, 0, 0);
    }
    switch (Data_02000240[225]) {
    case 1:
    case 2:
    case 3:
        *(s32 *)(*(u8 **)Data_03001ebc + 0x1c0) = 0x200;
        Func_0200a7f8(0xe00);
        Func_0200a7f6(Data_02009151, 3200);
        Engine_TaskWait(1);
        Engine_MapRedraw();
        Engine_TaskWait(1);
        break;
    case 10:
    case 13:
    case 20:
    case 23:
    case 24:
        *(s32 *)(*(u8 **)Data_03001ebc + 0x1c0) = 0x209;
        Func_0200a816(0xc00);
        Func_0200a812_setup(Func_0200a874(24), 0);
        if (Func_0200a842(0x314)) {
            Call3(Engine_ActorSetPosition, 25, 0x3680000, 0x780000);
        }
        break;
    case 21:
    case 22:
        *(s32 *)(*(u8 **)Data_03001ebc + 0x1c0) = 0x200;
        Func_0200a854(0xe00);
        Func_0200a7f6(Data_0200938d, 3200);
        Engine_TaskWait(1);
        Engine_MapRedraw();
        Engine_TaskWait(1);
        break;
    case 11:
    case 12:
        *(s32 *)(*(u8 **)Data_03001ebc + 0x1c0) = 0x200;
        if (Func_0200a89c(0x94a)) {
            Func_02007300();
        }
        break;
    case 31:
        *(s32 *)(*(u8 **)Data_03001ebc + 0x1c0) = 0x200;
        Func_02007314();
        break;
    case 14:
    case 15:
    case 16:
        Func_0200a844(Data_02009a45, 3200);
        break;
    default:
        *(s32 *)(*(u8 **)Data_03001ebc + 0x1c0) = 0x200;
        Func_0200a8c4(0xe00);
        break;
    }
    actor = Func_0200a922(8);
    Func_0200a8c8(Func_0200a92a(8), 0);
    Engine_ActorSetSpritePriority(8, 1);
    *(s32 *)&actor->unknown_18[0] = 0xc000;
    *(s32 *)&actor->unknown_18[4] = 0xc000;
}

void FieldScene_RestoreActorsFromFlags(void)
{
    extern s16 Data_02000240[];
    extern u8 Data_03001ebc[];

    struct ObjectRuntime *actor;

    *(s32 *)(*(u8 **)Data_03001ebc + 0x1c0) = 0x200;
    Func_0200a812();
    if (Func_0200a94c(0x943)) {
        Func_02005fbc();
    }
    Call1_02004f60(Engine_GameFlagSet, 0x217);
    Call1_02004f60(Engine_GameFlagSet, 0x218);
    if (Func_0200a968(0x944)) {
        Engine_ActorSetPosition(8, 0, 0);
        Call1_02004f60(Engine_GameFlagClear, 0x217);
    }
    if (Func_0200a982(0x945)) {
        Engine_ActorSetPosition(9, 0, 0);
        Func_02007658();
    }
    if (Func_0200a99a(0x946)) {
        Engine_ActorSetPosition(10, 0, 0);
        Call1_02004f60(Engine_GameFlagClear, 0x218);
    }
    if (Func_0200a9b6(0x947)) {
        Func_0200761a();
    }
    if (Func_0200a9c4(0x948)) {
        Func_0200765c();
    }
    Engine_EventBegin();
    actor = Func_0200aa16(8);
    if (actor != 0) {
        actor->unknown_23 = 2;
    }
    actor = Func_0200aa2a(9);
    if (actor != 0) {
        actor->unknown_23 = 2;
    }
    actor = Village_GetActor(10);
    if (actor != 0) {
        actor->unknown_23 = 2;
    }
    actor = Func_0200aa52(11);
    if (actor != 0) {
        Func_0200a9f6(actor, 0);
    }
    actor->unknown_23 = 2;
    actor = Func_0200aa6c(12);
    if (actor != 0) {
        actor->unknown_56[3] |= 0x10;
    }
    Func_0200aa22(Func_0200aa84(11), 0);
    Engine_EventEnd();
    Village_FinishActorRestore(0xe00);
}

void FieldScene_ActivateThreeActorGroup(void)
{
    extern s16 Data_02000240[];
    extern u8 Data_03001ebc[];

    if (Func_0200aa86(0x35a)) {
        Func_02006360(0);
    }
    if (Func_0200aa96(0x35b)) {
        Func_02006370(1);
    }
    if (Func_0200aaa8(0x35c)) {
        Func_02006382(2);
    }
}

void FieldScene_ActivateTwoActorGroup(void)
{
    extern s16 Data_02000240[];
    extern u8 Data_03001ebc[];

    if (Func_0200aac8(0x358)) {
        Func_020062f2_a(0);
    }
    if (Func_0200aad8(0x359)) {
        Func_02006302(1);
    }
}

void FieldScene_ActivateAlternateActorGroup(void)
{
    extern s16 Data_02000240[];
    extern u8 Data_03001ebc[];

    if (Func_0200aaf2(0x355)) {
        Func_02006240_a(0);
    }
    if (Func_0200ab02(0x356)) {
        Func_02006250(1);
    }
    if (Func_0200ab12(0x357)) {
        Func_02006260(2);
    }
}

void ActivateFiveActorGroupFromFlags(void)
{
    extern s16 Data_02000240[];
    extern u8 *Data_03001ebc;

    if (Func_0200ab38(0x350)) {
        Func_020061c2(0);
    }
    if (Func_0200ab48(0x351)) {
        Func_020061d2(1);
    }
    if (Func_0200ab58(0x352)) {
        Func_020061e2(2);
    }
    if (Func_0200ab68(0x353)) {
        Func_020061f2(3);
    }
    if (Func_0200ab7a(0x354)) {
        Func_02006204(4);
    }
}
