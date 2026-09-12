#include "types.h"

#define CalculateFixedPointPositionDistance Func_02000030
#define StagedActorStepTable Data_0200ded8
#define GetStagedActor Func_02005724
#define FindNextStagedActor Func_02000176
#define FindBlockingStagedActor Func_020001a2
#define FindElevatedBlockingStagedActor Func_020001ce
#define CanStartStagedActorMove Func_02005780
#define SetStagedActorMode Func_02005760
#define SelectStagedActorSlot Func_02005748
#define StartStagedActorEffect Func_0200596e
#define StartNextStagedActorMove Func_02005796
#define StartLeadStagedActorMove Func_020057a6
#define FinishStagedActorMove Func_020057b4
#define FinishStagedActorEffect Func_02005990
#define SetStagedActorTransition Func_020057cc
#define AdvanceStagedActorPair Func_020000c4
#define FindClearActorPosition Func_02000474
#define StartDoraHouseAccidentScene Func_020010a8
#define TurnActorToSceneDirection Func_02001be4
#define SelectPrimarySceneData Func_020009dc
#define GetEmptySceneData Func_02000a30
#define SelectSecondarySceneData Func_02000a34
#define SelectTertiarySceneData Func_02000a80
#define SelectQuaternarySceneData Func_02000af8
#define SelectActor25SceneVariant Func_02004928
#define SelectActor24SceneVariant Func_02004964
#define PlaceActorTwelveAndFinishScene Func_02000c78
#define ShowLockedDoorMessageOnce Func_02000f94
#define ShowLockedDoorMessage Func_02000fb4
#define ShowGeraldDontMessage Func_02000fc8
#define ShowThatWasExcitingMessage Func_02000fe4
#define ShowSorryIScaredYouMessage Func_02001000
#define ShowWellMessage Func_0200101c
#define NoOpSceneCallbackA Func_02001098
#define NoOpSceneCallbackB Func_0200109c
#define NoOpSceneCallbackC Func_020010a0
#define NoOpSceneCallbackD Func_020010a4
#define NoOpInteractionCallback Func_02002528
#define NoOpActorCallback Func_02004d4c
#define StopBlockedActorMotion Func_020002a8
#define ActivateSceneActor17 Func_020014bc
#define ActivateSceneActor18 Func_02001650
#define FinishActor20SceneSequence Func_02004d0c
#define ActivateFiveActorGroupFromFlags Func_02005520
#define ConfigureSceneActor9 Func_020012e0
#define ConfigureInteractionRegionA Func_0200226c
#define ConfigureInteractionRegionB Func_020022a0
#define ConfigureInteractionRegionC Func_020022d4
#define ConfigurePrimaryInteractionRegions Func_02002410
#define ConfigureSecondaryInteractionRegions Func_02002468
#define ConfigureSceneActor26 Func_02004b68
#define ConfigureSceneActor14 Func_02004b84
#define ConfigureSceneActor13 Func_02004ba4
#define ConfigureSceneActor12Variant Func_02004bc4
#define ConfigureSceneActor18 Func_02004be4
#define ConfigureActor13Interaction Func_02004d50
#define ConfigureActor13SceneResource Func_02004d88
#define TryStartActorInteraction Func_02001038
#define IsPlayerInAccidentTriggerArea Func_02001108
#define AreSceneActorsInPassingLane Func_02001324
#define IsPlayerInSecondaryTriggerArea Func_020014cc
#define IsPlayerOutsideSceneRectangle Func_02001660
#define IsSceneActorVerticallyNearPlayer Func_0200181c
#define IsSceneActorHorizontallyNearPlayer Func_02001880
#define IsActorInteractionAvailable Func_020018e4
#define IsSceneActorWithinFourSteps Func_02001918
#define IsSceneActorWithinTriggerBox Func_02001980
#define TriggerScene41AtVillagePath Func_020019e8
#define TriggerScene40AtVillagePath Func_02001a44
#define GameFlag_IsSet_1(a0) Value1(Func_02007dc6, a0)
#define GameFlag_Set_1(a0) Value1(Func_02007dde, a0)
#define BattleRuntime_Reset_1(args...) Func_02007dfa(args)
#define GameFlag_IsSet_2(a0) Value1(Func_02007de0, a0)
#define ObjectMotion_SetSpeedParameters_1(a0, a1, a2) Call3(Func_02007e3c, a0, a1, a2)
#define ObjectMotion_SetPositionAndReset_1(a0, a1, a2) Call3(Func_02007e78, a0, a1, a2)
#define ObjectMotion_SetAngleToward_1(args...) Func_02007eca(args)
#define BattleEffect_SpawnLinkedResourceObject_1(a0, a1, a2) Call3(Func_02007f22, a0, a1, a2)
#define ObjectMotion_ArmCallback_1(a0, a1, a2) Call3(Func_02007f1e, a0, a1, a2)
#define ObjectMotion_Launch_1(args...) Func_02007ed8(args)
#define Scene_GetRecord_1(args...) Func_02007e76(args)
#define BattleRuntime_WaitIfModeZero_1(args...) Func_02007e5a(args)
#define ObjectMotion_SetSpeedParameters_2(a0, a1, a2) Call3(Func_02007e94, a0, a1, a2)
#define ObjectMotion_ResetAndSetPositionInMode2_1(a0, a1, a2) Value3(Func_02007ec8, a0, a1, a2)
#define ObjectMotion_CommitCurrentPositionAndActivate_1(args...) Func_02007ee6(args)
#define BattleRuntime_WaitIfModeZero_2(args...) Func_02007e7c(args)
#define ObjectMotion_ArmCallback_2(a0, a1, a2) Call3(Func_02007f68, a0, a1, a2)
#define ObjectMotion_ArmCallback_3(a0, a1, a2) Call3(Func_02007f74, a0, a1, a2)
#define ObjectMotion_ArmCallback_4(a0, a1, a2) Call3(Func_02007f80, a0, a1, a2)
#define ObjectMotion_ArmCallback_5(a0, a1, a2) Call3(Func_02007f8c, a0, a1, a2)
#define ObjectMotion_SetHorizontalPositionWithTerrain_1(a0, a1, a2) Call3(Func_02007f32, a0, a1, a2)
#define ObjectMotion_SetSpeedLimitAndAcceleration_1(a0, a1) Value2(Func_02007fce, a0, a1)
#define SceneWork_SetStepValue_1(args...) Func_02007f8e(args)
#define BattleEvent_RunActionAndWait_1(args...) Func_02007fa6(args)
#define ObjectMotion_ResetAndSetPositionInMode2_2(a0, a1, a2) Value3(Func_02007f3c, a0, a1, a2)
#define ObjectMotion_CommitCurrentPositionAndActivate_2(args...) Func_02007f5a(args)
#define ObjectMotion_ArmCallback_6(a0, a1, a2) Value3(Func_02007fd6, a0, a1, a2)
#define BattleRuntime_WaitIfModeZero_3(args...) Func_02007efc(args)
#define ObjectMotion_OffsetPositionAndResetMotion_1(a0, a1, a2) Value3(Func_02007f70, a0, a1, a2)
#define ObjectMotion_CommitCurrentPositionAndActivate_3(args...) Func_02007f7e(args)
#define BattleRuntime_WaitIfModeZero_4(args...) Func_02007f14(args)
#define Audio_PlayCue_1(args...) Func_0200809a(args)
#define UiText_DrawMessage_1(args...) Func_02007efa(args)
#define ObjectMotion_OffsetPositionAndResetMotion_2(a0, a1, a2) Value3(Func_02007f96, a0, a1, a2)
#define BattleRuntime_WaitIfModeZero_5(args...) Func_02007f38(args)
#define ObjectMotion_SetVariantCallback_1(args...) Func_02007fd8(args)
#define ObjectMotion_SetVariantCallback_2(args...) Func_02007fe0(args)
#define ObjectMotion_SetVariantCallback_3(args...) Func_02007fe8(args)
#define ObjectMotion_SetVariantCallback_4(args...) Func_02007ff0(args)
#define BattleRuntime_WaitIfModeZero_6(args...) Func_02007f5e(args)
#define ObjectMotion_SetAngleToward_2(args...) Func_02008016(args)
#define SceneWork_SetStepValue_2(args...) Func_02008034(args)
#define BattleEvent_RunActionAndWait_2(args...) Func_0200804c(args)
#define BattleRuntime_WaitIfModeZero_7(args...) Func_02007f94(args)
#define SharedWorkData_SetFirstAndSecondFields_1(args...) Func_020080e6_a(args)
#define BattleEffect_ComputeWeightedResultAndDispatch_1(args...) Func_020080e6_b(args)
#define Object_SetModeById_1(args...) Func_0200805e(args)
#define Audio_PlayCue_2(args...) Func_02008164(args)
#define BattleRuntime_WaitIfModeZero_8(args...) Func_02007fea(args)
#define SceneWork_SetStepValue_3(args...) Func_020080b2(args)
#define BattleEvent_RunActionAndWait_3(args...) Func_020080ca(args)
#define Object_LinkPair_1(args...) Func_020080b4_a(args)
#define BattleRuntime_WaitIfModeZero_9(args...) Func_0200800a(args)
#define ObjectMotion_ArmCallback_7(a0, a1, a2) Call3(Func_020080f6, a0, a1, a2)
#define ObjectMotion_Launch_2(args...) Func_020080b0(args)
#define Scene_GetRecord_2(args...) Func_0200804e(args)
#define SceneWork_SetStepValue_4(args...) Func_020080f2(args)
#define BattleEvent_RunActionAndWait_4(args...) Func_0200810a(args)
#define ObjectMotion_SetSpeedParameters_3(a0, a1, a2) Call3(Func_02008078, a0, a1, a2)
#define ObjectMotion_SetPositionAndReset_2(a0, a1, a2) Call3(Func_020080b4_b, a0, a1, a2)
#define ObjectMotion_SetAngleToward_3(args...) Func_02008106(args)
#define ObjectMotion_SetSpeedParameters_4(a0, a1, a2) Call3(Func_0200809c, a0, a1, a2)
#define ObjectMotion_ResetAndSetPositionInMode2_3(a0, a1, a2) Value3(Func_020080d0, a0, a1, a2)
#define ObjectMotion_CommitCurrentPositionAndActivate_4(args...) Func_020080ee(args)
#define BattleRuntime_WaitIfModeZero_10(args...) Func_02008084(args)
#define SceneWork_SetStepValue_5(args...) Func_0200814a(args)
#define BattleEvent_RunActionAndWait_5(args...) Func_02008162_a(args)
#define BattleEffect_SpawnLinkedResourceObject_2(a0, a1, a2) Value3(Func_0200818e, a0, a1, a2)
#define BattleRuntime_WaitIfModeZero_11(args...) Func_020080a4(args)
#define Audio_PlayCue_3(args...) Func_0200822a(args)
#define SceneWork_SetStepValue_6(args...) Func_02008170(args)
#define BattleEvent_RunActionAndWait_6(args...) Func_02008188_a(args)
#define BattleRuntime_WaitIfModeZero_12(args...) Func_020080be(args)
#define ObjectMotion_CallThenWaitForAnimationChange_1(args...) Func_0200814e(args)
#define BattleRuntime_WaitIfModeZero_13(args...) Func_020080cc(args)
#define Object_SetModeById_2(args...) Func_02008154_a(args)
#define ObjectMotion_SetVariantCallback_5(args...) Func_02008174_a(args)
#define BattleRuntime_WaitIfModeZero_14(args...) Func_020080e2(args)
#define ObjectMotion_SetSpeedParameters_5(a0, a1, a2) Call3(Func_0200811c, a0, a1, a2)
#define ObjectMotion_ResetAndSetPositionInMode2_4(a0, a1, a2) Value3(Func_02008150, a0, a1, a2)
#define ObjectMotion_CommitCurrentPositionAndActivate_5(args...) Func_0200816e(args)
#define Object_SetModeById_3(args...) Func_02008186(args)
#define BattleRuntime_WaitIfModeZero_15(args...) Func_0200810c(args)
#define ObjectMotion_ArmCallback_8(a0, a1, a2) Value3(Func_020081f8, a0, a1, a2)
#define BattleRuntime_WaitIfModeZero_16(args...) Func_0200811e(args)
#define ObjectMotion_ArmCallback_9(a0, a1, a2) Value3(Func_0200820a, a0, a1, a2)
#define BattleRuntime_WaitIfModeZero_17(args...) Func_02008130(args)
#define ObjectMotion_SetAngleToward_4(args...) Func_020081e2(args)
#define BattleRuntime_WaitIfModeZero_18(args...) Func_02008140(args)
#define SceneWork_SetStepValue_7(args...) Func_02008206(args)
#define BattleEvent_RunActionAndWait_7(args...) Func_0200821e(args)
#define BattleRuntime_WaitIfModeZero_19(args...) Func_02008154_b(args)
#define ObjectMotion_CallThenWaitForAnimationChange_2(args...) Func_020081e4(args)
#define BattleRuntime_WaitIfModeZero_20(args...) Func_02008162_b(args)
#define BattleEffect_SpawnLinkedResourceObject_3(a0, a1, a2) Value3(Func_0200825e, a0, a1, a2)
#define BattleRuntime_WaitIfModeZero_21(args...) Func_02008174_b(args)
#define SceneWork_SetStepValue_8(args...) Func_0200823a(args)
#define BattleEvent_RunActionAndWait_8(args...) Func_02008252(args)
#define BattleRuntime_WaitIfModeZero_22(args...) Func_02008188_b(args)
#define ObjectMotion_CallThenWaitForAnimationChange_3(args...) Func_02008218(args)
#define BattleRuntime_WaitIfModeZero_23(args...) Func_02008196(args)
#define ObjectMotion_CallThenWaitForAnimationChange_4(args...) Func_02008226_a(args)
#define BattleRuntime_WaitIfModeZero_24(args...) Func_020081a4(args)
#define ObjectMotion_ResetAndSetPositionInMode2_5(a0, a1, a2) Value3(Func_02008208, a0, a1, a2)
#define ObjectMotion_CommitCurrentPositionAndActivate_6(args...) Func_02008226_b(args)
#define Object_LinkPair_2(args...) Func_02008270(args)
#define BattleRuntime_WaitIfModeZero_25(args...) Func_020081c6(args)
#define SceneWork_SetStepValue_9(args...) Func_0200828c(args)
#define BattleEvent_RunActionAndWait_9(args...) Func_020082a4(args)
#define BattleRuntime_WaitIfModeZero_26(args...) Func_020081da(args)
#define Object_SetModeById_4(args...) Func_02008262(args)
#define BattleRuntime_WaitIfModeZero_27(args...) Func_020081e8(args)
#define SceneWork_SetStepValue_10(args...) Func_020082ae(args)
#define BattleEvent_RunActionAndWait_10(args...) Func_020082c6(args)
#define ObjectMotion_SetHorizontalPositionWithTerrain_2(a0, a1, a2) Value3(Func_0200827c, a0, a1, a2)
#define Audio_PlayCue_4(args...) Func_0200838a(args)
#define SceneWork_SetStepValue_11(args...) Func_020082d2(args)
#define BattleEvent_RunActionAndWait_11(args...) Func_020082ea(args)
#define ObjectMotion_SetAngleToward_5(args...) Func_020082cc(args)
#define ObjectMotion_SetAngleToward_6(args...) Func_020082d6(args)
#define ObjectMotion_SetAngleToward_7(args...) Func_020082e0(args)
#define BattleRuntime_WaitIfModeZero_28(args...) Func_0200823e(args)
#define ObjectMotion_ArmCallback_10(a0, a1, a2) Call3(Func_0200832a, a0, a1, a2)
#define ObjectMotion_SetAngleToward_8(args...) Func_020082fc(args)
#define BattleRuntime_WaitIfModeZero_29(args...) Func_0200825a(args)
#define Audio_PlayCue_5(args...) Func_020083e0_a(args)
#define ObjectMotion_SetSpeedLimitAndAcceleration_2(a0, a1) Call2(Func_02008374, a0, a1)
#define ObjectMotion_PlaceLookedUpObjectWithinCameraBounds_1(args...) Func_02008394(args)
#define Object_CommitPositionThenWaitIfModeZero_1(args...) Func_02008390(args)
#define ObjectMotion_SetSpeedParameters_6(a0, a1, a2) Call3(Func_020082b2, a0, a1, a2)
#define ObjectMotion_ResetAndSetPositionInMode2_6(a0, a1, a2) Call3(Func_020082e8_a, a0, a1, a2)
#define ObjectMotion_SetPositionAndReset_3(args...) Func_02008398(args)
#define ObjectMotion_CommitCurrentPositionAndActivate_7(args...) Func_0200830e(args)
#define ObjectMotion_SetPositionAndReset_4(args...) Func_020083a6(args)
#define BattleEffect_SpawnLinkedResourceObject_4(a0, a1, a2) Call3(Func_020083a2, a0, a1, a2)
#define BattleEffect_SpawnLinkedResourceObject_5(a0, a1, a2) Call3(Func_020083ae, a0, a1, a2)
#define BattleEffect_SpawnLinkedResourceObject_6(a0, a1, a2) Call3(Func_020083ba, a0, a1, a2)
#define BattleEffect_SpawnLinkedResourceObject_7(a0, a1, a2) Call3(Func_020083c6, a0, a1, a2)
#define BattleEffect_SpawnLinkedResourceObject_8(a0, a1, a2) Value3(Func_020083d2, a0, a1, a2)
#define BattleRuntime_WaitIfModeZero_30(args...) Func_020082e8_b(args)
#define ObjectMotion_SetAngleToward_9(args...) Func_0200839a(args)
#define ObjectMotion_SetVariantCallback_6(args...) Func_02008392(args)
#define BattleRuntime_WaitIfModeZero_31(args...) Func_02008300(args)
#define SceneWork_SetStepValue_12(args...) Func_020083c8(args)
#define BattleEvent_RunActionAndWait_12(args...) Func_020083e0_b(args)
#define ObjectMotion_SetAngleToward_10(args...) Func_020083c2(args)
#define SceneWork_SetStepValue_13(args...) Func_020083e2_a(args)
#define BattleEvent_RunActionAndWait_13(args...) Func_020083fa(args)
#define BattleRuntime_WaitIfModeZero_32(args...) Func_02008330(args)
#define ObjectMotion_SetAngleToward_11(args...) Func_020083e2_b(args)
#define BattleRuntime_WaitIfModeZero_33(args...) Func_02008340(args)
#define SceneWork_SetStepValue_14(args...) Func_02008408(args)
#define BattleEvent_RunActionAndWait_14(args...) Func_02008420(args)
#define Object_LinkPair_3(args...) Func_0200840a(args)
#define Object_LinkPair_4(args...) Func_02008414(args)
#define BattleRuntime_WaitIfModeZero_34(args...) Func_0200836a(args)
#define ObjectMotion_SetAngleToward_12(args...) Func_0200841c(args)
#define ObjectMotion_SetAngleToward_13(args...) Func_02008426(args)
#define ObjectMotion_SetAngleToward_14(args...) Func_02008430(args)
#define ObjectMotion_SetAngleToward_15(args...) Func_0200843a(args)
#define ObjectMotion_SetVariantCallback_7(args...) Func_02008432(args)
#define BattleRuntime_WaitIfModeZero_35(args...) Func_020083a0(args)
#define SceneWork_SetStepValue_15(args...) Func_02008468_a(args)
#define BattleEvent_RunActionAndWait_15(args...) Func_02008480(args)
#define BattleEffect_SpawnLinkedResourceObject_9(a0, a1, a2) Value3(Func_020084aa, a0, a1, a2)
#define BattleRuntime_WaitIfModeZero_36(args...) Func_020083c0(args)
#define Object_SetModeById_5(args...) Func_02008448_a(args)
#define SceneWork_SetStepValue_16(args...) Func_02008490(args)
#define BattleEvent_RunActionAndWait_16(args...) Func_020084a8(args)
#define ObjectMotion_ResetAndSetPositionInMode2_7(a0, a1, a2) Value3(Func_0200843c, a0, a1, a2)
#define ObjectMotion_CommitCurrentPositionAndActivate_8(args...) Func_02008478(args)
#define ObjectMotion_ArmCallback_11(a0, a1, a2) Value3(Func_020084f4_a, a0, a1, a2)
#define SceneWork_SetStepValue_17(args...) Func_020084dc_a(args)
#define BattleEvent_RunActionAndWait_17(args...) Func_020084f4_b(args)
#define ObjectMotion_ResetAndSetPositionInMode2_8(a0, a1, a2) Value3(Func_02008488, a0, a1, a2)
#define ObjectMotion_CommitCurrentPositionAndActivate_9(args...) Func_020084a6(args)
#define ObjectMotion_ArmCallback_12(a0, a1, a2) Value3(Func_02008522_a, a0, a1, a2)
#define BattleRuntime_WaitIfModeZero_37(args...) Func_02008448_b(args)
#define SceneWork_SetStepValue_18(args...) Func_02008510(args)
#define BattleEvent_RunActionAndWait_18(args...) Func_02008528(args)
#define BattleEffect_SpawnLinkedResourceObject_10(a0, a1, a2) Value3(Func_02008552, a0, a1, a2)
#define BattleRuntime_WaitIfModeZero_38(args...) Func_02008468_b(args)
#define SceneWork_SetStepValue_19(args...) Func_02008530(args)
#define BattleEvent_RunActionAndWait_19(args...) Func_02008548(args)
#define ObjectMotion_ResetAndSetPositionInMode2_9(a0, a1, a2) Value3(Func_020084dc_b, a0, a1, a2)
#define ObjectMotion_CommitCurrentPositionAndActivate_10(args...) Func_020084fa(args)
#define ObjectMotion_SetAngleToward_16(args...) Func_0200853c(args)
#define ObjectMotion_CallThenWaitForAnimationChange_5(args...) Func_02008524(args)
#define BattleRuntime_WaitIfModeZero_39(args...) Func_020084a2(args)
#define SceneWork_SetStepValue_20(args...) Func_0200856a(args)
#define BattleEvent_RunActionAndWait_20(args...) Func_02008582_a(args)
#define Object_SetModeById_6(args...) Func_0200853a(args)
#define SceneWork_SetStepValue_21(args...) Func_02008582_b(args)
#define BattleEvent_RunActionAndWait_21(args...) Func_0200859a(args)
#define BattleEffect_SpawnLinkedResourceObject_11(a0, a1, a2) Call3(Func_020085c6_a, a0, a1, a2)
#define BattleEffect_SpawnLinkedResourceObject_12(a0, a1, a2) Call3(Func_020085d2, a0, a1, a2)
#define BattleEffect_SpawnLinkedResourceObject_13(a0, a1, a2) Call3(Func_020085de, a0, a1, a2)
#define BattleEffect_SpawnLinkedResourceObject_14(a0, a1, a2) Call3(Func_020085ea, a0, a1, a2)
#define BattleEffect_SpawnLinkedResourceObject_15(a0, a1, a2) Value3(Func_020085f6, a0, a1, a2)
#define BattleRuntime_WaitIfModeZero_40(args...) Func_0200850c(args)
#define SceneWork_SetStepValue_22(args...) Func_020085d4(args)
#define BattleEvent_RunActionAndWait_22(args...) Func_020085ec(args)
#define BattleRuntime_WaitIfModeZero_41(args...) Func_02008522_b(args)
#define BattleEffect_SpawnLinkedResourceObject_16(a0, a1, a2) Value3(Func_0200861c, a0, a1, a2)
#define BattleRuntime_WaitIfModeZero_42(args...) Func_02008532(args)
#define SceneWork_SetStepValue_23(args...) Func_020085fa_a(args)
#define BattleEvent_RunActionAndWait_23(args...) Func_02008612_a(args)
#define ObjectMotion_SetVariantCallback_8(args...) Func_020085e2_a(args)
#define BattleRuntime_WaitIfModeZero_43(args...) Func_02008550(args)
#define SceneWork_SetStepValue_24(args...) Func_02008618(args)
#define BattleEvent_RunActionAndWait_24(args...) Func_02008630(args)
#define ObjectMotion_ResetAndSetPositionInMode2_10(a0, a1, a2) Value3(Func_020085c6_b, a0, a1, a2)
#define ObjectMotion_CommitCurrentPositionAndActivate_11(args...) Func_020085e4(args)
#define ObjectMotion_ArmCallback_13(a0, a1, a2) Value3(Func_02008660, a0, a1, a2)
#define BattleRuntime_WaitIfModeZero_44(args...) Func_02008586(args)
#define ObjectMotion_OffsetPositionAndResetMotion_3(a0, a1, a2) Value3(Func_020085fa_b, a0, a1, a2)
#define ObjectMotion_CommitCurrentPositionAndActivate_12(args...) Func_02008608(args)
#define BattleRuntime_WaitIfModeZero_45(args...) Func_0200859e(args)
#define Audio_PlayCue_6(args...) Func_02008724(args)
#define UiText_DrawMessage_2(args...) Func_02008584(args)
#define ObjectMotion_OffsetPositionAndResetMotion_4(a0, a1, a2) Call3(Func_02008620, a0, a1, a2)
#define ObjectMotion_SetAngleToward_17(args...) Func_0200866a(args)
#define Audio_PlayCue_7(args...) Func_0200874c(args)
#define SceneWork_SetStepValue_25(args...) Func_02008694_a(args)
#define BattleEvent_RunActionAndWait_25(args...) Func_020086ac(args)
#define BattleRuntime_WaitIfModeZero_46(args...) Func_020085e2_b(args)
#define ObjectMotion_SetAngleToward_18(args...) Func_02008694_b(args)
#define ObjectMotion_SetAngleToward_19(args...) Func_0200869e(args)
#define ObjectMotion_SetAngleToward_20(args...) Func_020086a8(args)
#define ObjectMotion_SetAngleToward_21(args...) Func_020086b2(args)
#define ObjectMotion_SetAngleToward_22(args...) Func_020086bc(args)
#define GameFlag_Set_2(a0) Value1(Func_02008612_b, a0)
#define BattleRuntime_WaitIfModeZero_47(args...) Func_02008636(args)
#define SharedWorkData_SetFirstAndSecondFields_2(args...) Func_02008788_a(args)
#define BattleEffect_ComputeWeightedResultAndDispatch_2(args...) Func_02008788_b(args)
#define BattleRuntime_ScheduleShoulderButtonModeUpdate_1(args...) Func_02008664(args)
#define STATE_TABLE_022B Data_02000240[0x22B]
#define SHARED_RECORD_FIELD_448 (*(u32 *)(*(u8 **)0x03001ebc + 448))
#define FieldScene_RunScene3bfSequenceA Func_0200169c
#define RunActor17SceneStep Func_02001708
#define TriggerSceneStage95FromActor12 Func_02001718
#define FieldScene_RunScene3bfSequenceB Func_0200175c
#define FieldScene_RunScene3bfSequenceC Func_020017bc
#define RunActor9ScriptedSequence Func_02001ab0
#define RunActorScriptedSequenceA Func_02001b24
#define RunActorScriptedSequenceB Func_02001c4c
#define RunActorScriptedSequenceC Func_02001ca4
#define RunActorScriptedSequenceD Func_02001d60
#define RunActor12InteractionSequence Func_02002134
#define RunSecondaryMapInteraction Func_0200238c
#define PlayStoryScene Func_020027b0
#define RunActor20SceneSequence Func_02004bfc
#define InspectOrdinaryObject Func_02001dc4
#define InspectEmptyChest Func_02001e2c
#define InspectEmptySceneObject Func_020020cc
#define InspectVillageWell Func_02002308
#define InspectWardrobe Func_020024c0
#define InspectFirewood Func_020024e4
#define InspectBooks Func_02002504
#define Object_SetModeById_1_02001e94(args...) Func_02007544(args)
#define Object_SetModeById_2_02001e94(args...) Func_0200754c(args)
#define Object_SetModeById_3_02001e94(args...) Func_02007554(args)
#define Object_SetModeById_4_02001e94(args...) Func_0200755c(args)
#define Audio_PlayCue_1_02001e94(args...) Func_02007662_a(args)
#define BattleEffect_SpawnLinkedResourceObject_1_02001e94(a0, a1, a2) Call3(Func_020075de_a, a0, a1, a2)
#define BattleRuntime_WaitIfModeZero_1_02001e94(args...) Func_020074f4(args)
#define ObjectMotion_SetAngleToward_1_02001e94(args...) Func_020075a6(args)
#define SceneWork_SetStepValue_1_02001e94(args...) Func_020075c6(args)
#define BattleEvent_RunActionAndWait_1_02001e94(args...) Func_020075de_b(args)
#define BattleEffect_SpawnLinkedResourceObject_2_02001e94(a0, a1, a2) Call3(Func_0200760a, a0, a1, a2)
#define BattleEffect_SpawnLinkedResourceObject_3_02001e94(a0, a1, a2) Call3(Func_02007616, a0, a1, a2)
#define ObjectMotion_SetAngleToward_2_02001e94(args...) Func_020075d8(args)
#define ObjectMotion_SetAngleToward_3_02001e94(args...) Func_020075e2_a(args)
#define ObjectMotion_SetAngleToward_4_02001e94(args...) Func_020075ec(args)
#define BattleRuntime_WaitIfModeZero_2_02001e94(args...) Func_0200754a(args)
#define ObjectMotion_ArmCallback_1_02001e94(a0, a1, a2) Call3(Func_02007636, a0, a1, a2)
#define ObjectMotion_ArmCallback_2_02001e94(a0, a1, a2) Call3(Func_02007642, a0, a1, a2)
#define SceneWork_SetStepValue_2_02001e94(args...) Func_02007628(args)
#define BattleEvent_RunActionAndWait_2_02001e94(args...) Func_02007640(args)
#define ObjectMotion_CallThenWaitForAnimationChange_1_02001e94(args...) Func_02007600(args)
#define SceneWork_SetStepValue_3_02001e94(args...) Func_0200763e(args)
#define BattleEvent_RunActionAndWait_3_02001e94(args...) Func_02007656(args)
#define SceneWork_SetStepValue_4_02001e94(args...) Func_0200764c_a(args)
#define BattleEvent_RunActionAndWait_4_02001e94(args...) Func_02007664(args)
#define ObjectMotion_SetVariantCallbackAndInvokeObject_1(args...) Func_0200763c(args)
#define SceneWork_SetStepValue_5_02001e94(args...) Func_02007662_b(args)
#define BattleEvent_RunActionAndWait_5_02001e94(args...) Func_0200767a(args)
#define ObjectMotion_CallThenWaitForAnimationChange_2_02001e94(args...) Func_0200763a(args)
#define SceneWork_SetStepValue_6_02001e94(args...) Func_02007678(args)
#define BattleEvent_RunActionAndWait_6_02001e94(args...) Func_02007690(args)
#define ObjectMotion_CallThenWaitForAnimationChange_3_02001e94(args...) Func_02007650(args)
#define BattleRuntime_WaitIfModeZero_3_02001e94(args...) Func_020075ce(args)
#define ObjectMotion_SetAngleToward_5_02001e94(args...) Func_02007680(args)
#define ObjectMotion_SetAngleToward_6_02001e94(args...) Func_0200768a(args)
#define BattleRuntime_WaitIfModeZero_4_02001e94(args...) Func_020075e8(args)
#define ObjectMotion_ResetAndSetPositionInMode2_1_02001e94(a0, a1, a2) Call3(Func_0200764c_b, a0, a1, a2)
#define ObjectMotion_CommitCurrentPositionAndActivate_1_02001e94(args...) Func_0200766a(args)
#define ObjectMotion_SetAngleToward_7_02001e94(args...) Func_020076ac(args)
#define ObjectMotion_CallThenWaitForAnimationChange_4_02001e94(args...) Func_02007694(args)
#define BattleRuntime_WaitIfModeZero_5_02001e94(args...) Func_02007614(args)
#define SceneWork_SetStepValue_7_02001e94(args...) Func_020076da(args)
#define BattleEvent_RunActionAndWait_7_02001e94(args...) Func_020076f2(args)
#define SharedWorkData_SetFirstAndSecondFields_1_02001e94(args...) Func_02007778(args)
#define BattleEffect_ComputeWeightedResultAndDispatch_1_02001e94(args...) Func_02007782(args)
#define ObjectMotion_SetHorizontalPositionWithTerrain_1_02001e94(args...) Func_020076cc(args)
#define ObjectMotion_SetHorizontalPositionWithTerrain_2_02001e94(args...) Func_020076d6(args)
#define ObjectMotion_SetHorizontalPositionWithTerrain_3(args...) Func_020076e0(args)
#define BattleRuntime_ScheduleShoulderButtonModeUpdate_1_02001e94(args...) Func_0200767c(args)
#define GameFlag_Set_1_02001e94(a0) Call1(Func_02007662_c, a0)
#define SCENE_PHASE (*(s32 *)(*(u8 *volatile *)Data_03001ebc + 0x1c0))
#define BattleRuntime_Reset_1_0200206c() Call0(Func_020076a0)
#define ObjectMotion_SetHorizontalPositionWithTerrain_1_0200206c(a0, a1, a2) Call3(Func_0200771e, a0, a1, a2)
#define ObjectMotion_SetHorizontalPositionWithTerrain_2_0200206c(a0, a1, a2) Call3(Func_0200772c, a0, a1, a2)
#define ObjectMotion_SetHorizontalPositionWithTerrain_3_0200206c(a0, a1, a2) Call3(Func_0200773a, a0, a1, a2)
#define Object_SetModeById_1_0200206c(a0, a1) Call2(Func_0200774a, a0, a1)
#define Object_SetModeById_2_0200206c(a0, a1) Call2(Func_02007752_a, a0, a1)
#define Object_SetModeById_3_0200206c(a0, a1) Call2(Func_0200775a, a0, a1)
#define ObjectMotion_SetAngleToward_1_0200206c(a0, a1, a2) Call3(Func_0200778c, a0, a1, a2)
#define BattleRuntime_ScheduleShoulderButtonModeUpdate_1_0200206c() Call0(Func_020076f8)
#define BattleRuntime_WaitIfModeZero_1_0200206c() Call0(Func_02007834)
#define GameFlag_Set_1_02003054(a0) Call1(Func_02008674_a, a0)
#define GameFlag_Set_2_02003054(a0) Call1(Func_0200867a, a0)
#define BattleRuntime_Reset_1_02003054(args...) Func_020086b0(args)
#define Scene_GetRecord_1_02003054(args...) Func_020086d6(args)
#define ObjectMotion_SetHorizontalPositionWithTerrain_1_02003054(a0, a1, a2) Call3(Func_0200873a, a0, a1, a2)
#define ObjectMotion_ArmCallback_1_02003054(a0, a1, a2) Call3(Func_020087ae, a0, a1, a2)
#define ObjectMotion_SetHorizontalPositionWithTerrain_2_02003054(a0, a1, a2) Call3(Func_02008754, a0, a1, a2)
#define ObjectMotion_ArmCallback_2_02003054(a0, a1, a2) Call3(Func_020087c8, a0, a1, a2)
#define ObjectMotion_SetHorizontalPositionWithTerrain_3_02003054(a0, a1, a2) Call3(Func_0200876e, a0, a1, a2)
#define ObjectMotion_ArmCallback_3_02003054(a0, a1, a2) Call3(Func_020087e2, a0, a1, a2)
#define ObjectMotion_SetHorizontalPositionWithTerrain_4(a0, a1, a2) Call3(Func_02008788, a0, a1, a2)
#define ObjectMotion_ArmCallback_4_02003054(a0, a1, a2) Call3(Func_020087fc, a0, a1, a2)
#define ObjectMotion_SetHorizontalPositionWithTerrain_5(a0, a1, a2) Call3(Func_020087a2, a0, a1, a2)
#define ObjectMotion_ArmCallback_5_02003054(a0, a1, a2) Call3(Func_02008816, a0, a1, a2)
#define ObjectMotion_SetHorizontalPositionWithTerrain_6(a0, a1, a2) Call3(Func_020087bc, a0, a1, a2)
#define ObjectMotion_ArmCallback_6_02003054(a0, a1, a2) Call3(Func_02008830, a0, a1, a2)
#define ObjectMotion_SetHorizontalPositionWithTerrain_7(a0, a1, a2) Call3(Func_020087d6, a0, a1, a2)
#define ObjectMotion_SetActionVariant_1(args...) Func_0200884e(args)
#define Object_SetModeById_1_02003054(args...) Func_020087ee(args)
#define ObjectMotion_SetHorizontalPositionWithTerrain_8(a0, a1, a2) Call3(Func_020087f4, a0, a1, a2)
#define ObjectMotion_ArmCallback_7_02003054(a0, a1, a2) Call3(Func_02008868, a0, a1, a2)
#define Object_SetModeById_2_02003054(args...) Func_02008810(args)
#define BattleRuntime_WaitIfModeZero_1_02003054(args...) Func_020088dc(args)
#define BattleRuntime_WaitIfModeZero_2_02003054(args...) Func_0200879a(args)
#define SceneWork_SetStepValue_1_02003054(args...) Func_02008862(args)
#define BattleEvent_RunActionAndWait_1_02003054(args...) Func_0200887a_a(args)
#define BattleEffect_SpawnLinkedResourceObject_1_02003054(a0, a1, a2) Call3(Func_020088a4, a0, a1, a2)
#define SceneWork_SetStepValue_2_02003054(args...) Func_0200887a_b(args)
#define BattleEvent_RunActionAndWait_2_02003054(args...) Func_02008892(args)
#define BattleEffect_SpawnLinkedResourceObject_2_02003054(a0, a1, a2) Call3(Func_020088bc_a, a0, a1, a2)
#define BattleEffect_SpawnLinkedResourceObject_3_02003054(a0, a1, a2) Call3(Func_020088c6_a, a0, a1, a2)
#define BattleEffect_SpawnLinkedResourceObject_4_02003054(a0, a1, a2) Call3(Func_020088d0, a0, a1, a2)
#define BattleRuntime_WaitIfModeZero_3_02003054(args...) Func_020087e6(args)
#define ObjectMotion_SetAngleToward_1_02003054(args...) Func_02008898(args)
#define BattleRuntime_WaitIfModeZero_4_02003054(args...) Func_020087f6(args)
#define SceneWork_SetStepValue_3_02003054(args...) Func_020088bc_b(args)
#define BattleEvent_RunActionAndWait_3_02003054(args...) Func_020088d4_a(args)
#define ObjectMotion_SetAngleToward_2_02003054(args...) Func_020088b6(args)
#define ObjectMotion_SetAngleToward_3_02003054(args...) Func_020088c0(args)
#define ObjectMotion_SetAngleToward_4_02003054(args...) Func_020088ca(args)
#define ObjectMotion_SetAngleToward_5_02003054(args...) Func_020088d4_b(args)
#define BattleRuntime_WaitIfModeZero_5_02003054(args...) Func_02008832(args)
#define ObjectMotion_ResetAndSetPositionInMode2_1_02003054(a0, a1, a2) Call3(Func_02008896, a0, a1, a2)
#define Object_SetModeById_3_02003054(args...) Func_020088c6_b(args)
#define BattleRuntime_WaitIfModeZero_6_02003054(args...) Func_0200884c(args)
#define SceneWork_SetStepValue_4_02003054(args...) Func_02008912(args)
#define BattleEvent_RunActionAndWait_4_02003054(args...) Func_0200892a(args)
#define BattleRuntime_WaitIfModeZero_7_02003054(args...) Func_02008860(args)
#define ObjectMotion_SetVariantCallback_1_02003054(args...) Func_02008900(args)
#define SceneWork_SetStepValue_5_02003054(args...) Func_0200892e(args)
#define BattleEvent_RunActionAndWait_5_02003054(args...) Func_02008946_a(args)
#define ObjectMotion_SetAngleToward_6_02003054(args...) Func_02008928(args)
#define ObjectMotion_SetAngleToward_7_02003054(args...) Func_02008932(args)
#define ObjectMotion_SetAngleToward_8_02003054(args...) Func_0200893c(args)
#define ObjectMotion_SetAngleToward_9_02003054(args...) Func_02008946_b(args)
#define ObjectMotion_SetVariantCallback_2_02003054(args...) Func_0200893e(args)
#define SceneWork_SetStepValue_6_02003054(args...) Func_0200896c(args)
#define BattleEvent_RunActionAndWait_6_02003054(args...) Func_02008984(args)
#define BattleEffect_SpawnLinkedResourceObject_5_02003054(a0, a1, a2) Call3(Func_020089ae, a0, a1, a2)
#define Object_SetModeById_4_02003054(args...) Func_02008946_c(args)
#define BattleRuntime_WaitIfModeZero_8_02003054(args...) Func_020088cc(args)
#define Object_SetModeById_5_02003054(args...) Func_02008954(args)
#define SceneWork_SetStepValue_7_02003054(args...) Func_0200899a(args)
#define BattleEvent_RunActionAndWait_7_02003054(args...) Func_020089b2(args)
#define BattleEffect_SpawnLinkedResourceObject_6_02003054(a0, a1, a2) Call3(Func_020089de, a0, a1, a2)
#define ObjectMotion_SetVariantCallback_3_02003054(args...) Func_0200898e(args)
#define BattleRuntime_WaitIfModeZero_9_02003054(args...) Func_020088fc(args)
#define SceneWork_SetStepValue_8_02003054(args...) Func_020089c2(args)
#define BattleEvent_RunActionAndWait_8_02003054(args...) Func_020089da(args)
#define ObjectMotion_SetAngleToward_10_02003054(args...) Func_020089bc(args)
#define BattleRuntime_WaitIfModeZero_10_02003054(args...) Func_0200891a(args)
#define SceneWork_SetStepValue_9_02003054(args...) Func_020089e2(args)
#define BattleEvent_RunActionAndWait_9_02003054(args...) Func_020089fa_a(args)
#define ObjectMotion_SetVariantCallback_4_02003054(args...) Func_020089ca(args)
#define BattleRuntime_WaitIfModeZero_11_02003054(args...) Func_02008938(args)
#define SceneWork_SetStepValue_10_02003054(args...) Func_02008a00(args)
#define BattleEvent_RunActionAndWait_10_02003054(args...) Func_02008a18(args)
#define ObjectMotion_SetAngleToward_11_02003054(args...) Func_020089fa_b(args)
#define SceneWork_SetStepValue_11_02003054(args...) Func_02008a1a_a(args)
#define ObjectMotion_SetSpeedParameters_1_02003054(a0, a1) Value2(Func_02008a2a, a0, a1)
#define UiWork_WaitThenFinalizeCapacity_1(args...) Func_0200898a(args)
#define Object_SetModeById_6_02003054(args...) Func_020089f6(args)
#define Object_SetModeById_7(args...) Func_020089fe(args)
#define Object_SetModeById_8(args...) Func_02008a06(args)
#define ObjectMotion_SetAngleToward_12_02003054(args...) Func_02008a38(args)
#define BattleRuntime_WaitIfModeZero_12_02003054(args...) Func_02008996(args)
#define SceneWork_SetStepValue_12_02003054(args...) Func_02008a76(args)
#define BattleEvent_RunActionAndWait_11_02003054(args...) Func_02008a8e_a(args)
#define ObjectMotion_SetAngleToward_13_02003054(args...) Func_02008a70(args)
#define ObjectMotion_SetAngleToward_14_02003054(args...) Func_02008a7a(args)
#define ObjectMotion_SetAngleToward_15_02003054(args...) Func_02008a84(args)
#define ObjectMotion_SetAngleToward_16_02003054(args...) Func_02008a8e_b(args)
#define BattleRuntime_WaitIfModeZero_13_02003054(args...) Func_020089ec(args)
#define ObjectMotion_SetAngleToward_17_02003054(args...) Func_02008a9e(args)
#define BattleRuntime_WaitIfModeZero_14_02003054(args...) Func_020089fc(args)
#define ObjectMotion_SetAngleToward_18_02003054(args...) Func_02008aae(args)
#define BattleRuntime_WaitIfModeZero_15_02003054(args...) Func_02008a0c(args)
#define ObjectMotion_CallThenWaitForAnimationChange_1_02003054(args...) Func_02008a9c(args)
#define BattleRuntime_WaitIfModeZero_16_02003054(args...) Func_02008a1a_b(args)
#define ObjectMotion_SetAngleToward_19_02003054(args...) Func_02008acc(args)
#define SceneWork_SetStepValue_13_02003054(args...) Func_02008aec_a(args)
#define BattleEvent_RunActionAndWait_12_02003054(args...) Func_02008b04(args)
#define BattleRuntime_WaitIfModeZero_17_02003054(args...) Func_02008a3a(args)
#define ObjectMotion_SetAngleToward_20_02003054(args...) Func_02008aec_b(args)
#define ObjectMotion_SetAngleToward_21_02003054(args...) Func_02008af6(args)
#define ObjectMotion_SetAngleToward_22_02003054(args...) Func_02008b00(args)
#define ObjectMotion_SetAngleToward_23(args...) Func_02008b0a(args)
#define Object_SetModeById_9(args...) Func_02008aea(args)
#define Object_SetModeById_10(args...) Func_02008af2(args)
#define Object_SetModeById_11(args...) Func_02008afa(args)
#define Object_SetModeById_12(args...) Func_02008b02(args)
#define BattleRuntime_WaitIfModeZero_18_02003054(args...) Func_02008a88(args)
#define ObjectMotion_SetAngleToward_24(args...) Func_02008b3a(args)
#define ObjectMotion_SetAngleToward_25(args...) Func_02008b44(args)
#define ObjectMotion_SetAngleToward_26(args...) Func_02008b4e(args)
#define ObjectMotion_SetAngleToward_27(args...) Func_02008b58(args)
#define ObjectMotion_SetAngleToward_28(args...) Func_02008b62(args)
#define ObjectMotion_SetSpeedParameters_2_02003054(a0, a1, a2) Call3(Func_02008af8, a0, a1, a2)
#define ObjectMotion_SetSpeedParameters_3_02003054(a0, a1, a2) Call3(Func_02008b06, a0, a1, a2)
#define ObjectMotion_SetSpeedParameters_4_02003054(a0, a1, a2) Call3(Func_02008b14, a0, a1, a2)
#define ObjectMotion_SetSpeedParameters_5_02003054(a0, a1, a2) Call3(Func_02008b22, a0, a1, a2)
#define ObjectMotion_ResetAndSetPositionInMode2_2_02003054(a0, a1, a2) Call3(Func_02008b56, a0, a1, a2)
#define ObjectMotion_CommitCurrentPositionAndActivate_1_02003054(args...) Func_02008b74(args)
#define ObjectMotion_ResetAndSetPositionInMode2_3_02003054(a0, a1, a2) Call3(Func_02008b68, a0, a1, a2)
#define ObjectMotion_CommitCurrentPositionAndActivate_2_02003054(args...) Func_02008b86(args)
#define ObjectMotion_ResetAndSetPositionInMode2_4_02003054(a0, a1, a2) Call3(Func_02008b7a, a0, a1, a2)
#define ObjectMotion_CommitCurrentPositionAndActivate_3_02003054(args...) Func_02008b98_a(args)
#define ObjectMotion_ArmCallback_8_02003054(a0, a1, a2) Call3(Func_02008c14, a0, a1, a2)
#define ObjectMotion_ResetAndSetPositionInMode2_5_02003054(a0, a1, a2) Call3(Func_02008b98_b, a0, a1, a2)
#define ObjectMotion_CommitCurrentPositionAndActivate_4_02003054(args...) Func_02008bb6(args)
#define ObjectMotion_ArmCallback_9_02003054(args...) Func_02008c30(args)
#define ObjectMotion_ResetAndSetPositionInMode2_6_02003054(a0, a1, a2) Call3(Func_02008bb4, a0, a1, a2)
#define ObjectMotion_CommitCurrentPositionAndActivate_5_02003054(args...) Func_02008bd2(args)
#define ObjectMotion_ResetAndSetPositionInMode2_7_02003054(a0, a1, a2) Call3(Func_02008bc6, a0, a1, a2)
#define ObjectMotion_CommitCurrentPositionAndActivate_6_02003054(args...) Func_02008be4_a(args)
#define ObjectMotion_ArmCallback_10_02003054(a0, a1, a2) Call3(Func_02008c60, a0, a1, a2)
#define ObjectMotion_ResetAndSetPositionInMode2_8_02003054(a0, a1, a2) Call3(Func_02008be4_b, a0, a1, a2)
#define ObjectMotion_CommitCurrentPositionAndActivate_7_02003054(args...) Func_02008c02(args)
#define ObjectMotion_ArmCallback_11_02003054(a0, a1, a2) Call3(Func_02008c7e_a, a0, a1, a2)
#define Object_LinkPair_1_02003054(args...) Func_02008c58_a(args)
#define Object_LinkPair_2_02003054(args...) Func_02008c62(args)
#define BattleRuntime_WaitIfModeZero_19_02003054(args...) Func_02008bb8(args)
#define ObjectMotion_CallThenWaitForAnimationChange_2_02003054(args...) Func_02008c48_a(args)
#define Object_SetModeById_13(args...) Func_02008c48_b(args)
#define Object_SetModeById_14(args...) Func_02008c50(args)
#define Object_SetModeById_15(args...) Func_02008c58_b(args)
#define BattleRuntime_WaitIfModeZero_20_02003054(args...) Func_02008bde(args)
#define Audio_PlayCue_1_02003054(args...) Func_02008d64(args)
#define Object_SetModeById_16(args...) Func_02008c6c(args)
#define ObjectMotion_SetSpeedParameters_6_02003054(a0, a1, a2) Call3(Func_02008c26, a0, a1, a2)
#define ObjectMotion_OffsetPositionAndResetMotion_1_02003054(a0, a1, a2) Call3(Func_02008c6a, a0, a1, a2)
#define ObjectMotion_SetSpeedParameters_7(a0, a1, a2) Call3(Func_02008c3c, a0, a1, a2)
#define ObjectMotion_OffsetPositionAndResetMotion_2_02003054(a0, a1, a2) Call3(Func_02008c80, a0, a1, a2)
#define ObjectMotion_SetSpeedParameters_8(a0, a1, a2) Call3(Func_02008c52, a0, a1, a2)
#define ObjectMotion_OffsetPositionAndResetMotion_3_02003054(a0, a1, a2) Call3(Func_02008c96, a0, a1, a2)
#define ObjectMotion_SetSpeedParameters_9(a0, a1, a2) Call3(Func_02008c68, a0, a1, a2)
#define ObjectMotion_OffsetPositionAndResetMotion_4_02003054(a0, a1, a2) Call3(Func_02008cac, a0, a1, a2)
#define ObjectMotion_SetSpeedParameters_10(a0, a1, a2) Call3(Func_02008c7e_b, a0, a1, a2)
#define ObjectMotion_SetSpeedParameters_11(a0, a1, a2) Call3(Func_02008c88, a0, a1, a2)
#define ObjectMotion_OffsetPositionAndResetMotion_5(a0, a1, a2) Call3(Func_02008ccc, a0, a1, a2)
#define ObjectMotion_OffsetPositionAndResetMotion_6(a0, a1, a2) Call3(Func_02008cd8, a0, a1, a2)
#define ObjectMotion_CommitCurrentPositionAndActivate_8_02003054(args...) Func_02008ce6(args)
#define Audio_PlayCue_2_02003054(a0) Call1(Func_02008dfe, a0)
#define BattleRuntime_WaitIfModeZero_21_02003054(args...) Func_02008c84(args)
#define ObjectMotion_ArmCallback_12_02003054(a0, a1, a2) Call3(Func_02008d70, a0, a1, a2)
#define ObjectMotion_ArmCallback_13_02003054(a0, a1, a2) Call3(Func_02008d7c, a0, a1, a2)
#define ObjectMotion_ArmCallback_14(a0, a1, a2) Call3(Func_02008d88, a0, a1, a2)
#define ObjectMotion_ArmCallback_15(a0, a1, a2) Call3(Func_02008d94_a, a0, a1, a2)
#define BattleRuntime_WaitIfModeZero_22_02003054(args...) Func_02008cba(args)
#define Object_SetModeById_17(args...) Func_02008d42(args)
#define Object_SetModeById_18(args...) Func_02008d4a(args)
#define Object_SetModeById_19(args...) Func_02008d52(args)
#define Object_SetModeById_20(args...) Func_02008d5a(args)
#define BattleRuntime_WaitIfModeZero_23_02003054(args...) Func_02008ce0(args)
#define ObjectMotion_SetSpeedParameters_12(a0, a1, a2) Call3(Func_02008d1a, a0, a1, a2)
#define ObjectMotion_SetSpeedParameters_13(a0, a1, a2) Call3(Func_02008d24, a0, a1, a2)
#define ObjectMotion_SetSpeedParameters_14(a0, a1, a2) Call3(Func_02008d2e, a0, a1, a2)
#define ObjectMotion_SetSpeedParameters_15(a0, a1, a2) Call3(Func_02008d38, a0, a1, a2)
#define ObjectMotion_ResetAndSetPositionInMode2_9_02003054(a0, a1, a2) Call3(Func_02008d6c, a0, a1, a2)
#define ObjectMotion_CommitCurrentPositionAndActivate_9_02003054(args...) Func_02008d8a(args)
#define Object_SetModeById_21(args...) Func_02008da2(args)
#define ObjectMotion_ArmCallback_16(a0, a1, a2) Call3(Func_02008e0e, a0, a1, a2)
#define ObjectMotion_ResetAndSetPositionInMode2_10_02003054(a0, a1, a2) Call3(Func_02008d94_b, a0, a1, a2)
#define ObjectMotion_CommitCurrentPositionAndActivate_10_02003054(args...) Func_02008db2(args)
#define ObjectMotion_ResetAndSetPositionInMode2_11(a0, a1, a2) Call3(Func_02008da6, a0, a1, a2)
#define ObjectMotion_CommitCurrentPositionAndActivate_11_02003054(args...) Func_02008dc4(args)
#define ObjectMotion_ResetAndSetPositionInMode2_12(a0, a1, a2) Call3(Func_02008db8, a0, a1, a2)
#define ObjectMotion_CommitCurrentPositionAndActivate_12_02003054(args...) Func_02008dd6(args)
#define Object_SetModeById_22(args...) Func_02008dee(args)
#define ObjectMotion_ArmCallback_17(a0, a1, a2) Call3(Func_02008e5a, a0, a1, a2)
#define ObjectMotion_ResetAndSetPositionInMode2_13(a0, a1, a2) Call3(Func_02008dde_a, a0, a1, a2)
#define ObjectMotion_CommitCurrentPositionAndActivate_13(args...) Func_02008dfc_a(args)
#define ObjectMotion_ResetAndSetPositionInMode2_14(a0, a1, a2) Call3(Func_02008df0, a0, a1, a2)
#define ObjectMotion_ResetAndSetPositionInMode2_15(a0, a1, a2) Call3(Func_02008dfc_b, a0, a1, a2)
#define ObjectMotion_CommitCurrentPositionAndActivate_14(args...) Func_02008e1a(args)
#define Object_SetModeById_23(args...) Func_02008e32(args)
#define ObjectMotion_ArmCallback_18(a0, a1, a2) Call3(Func_02008e9e, a0, a1, a2)
#define ObjectMotion_CommitCurrentPositionAndActivate_15(args...) Func_02008e34(args)
#define Object_SetModeById_24(args...) Func_02008e4c(args)
#define ObjectMotion_ArmCallback_19(a0, a1, a2) Call3(Func_02008eb8, a0, a1, a2)
#define BattleRuntime_WaitIfModeZero_24_02003054(args...) Func_02008dde_b(args)
#define BattleRuntime_WaitIfModeZero_25_02003054(args...) Func_02008de4(args)
#define SceneWork_SetStepValue_14_02003054(args...) Func_02008eac(args)
#define BattleEvent_RunActionAndWait_13_02003054(args...) Func_02008ec4(args)
#define Object_SetModeById_25(args...) Func_02008e7c(args)
#define BattleRuntime_WaitIfModeZero_26_02003054(args...) Func_02008e02(args)
#define ObjectMotion_SetVariantCallback_5_02003054(args...) Func_02008ea2(args)
#define BattleRuntime_WaitIfModeZero_27_02003054(args...) Func_02008e10(args)
#define Object_SetModeById_26(args...) Func_02008e98(args)
#define SceneWork_SetStepValue_15_02003054(args...) Func_02008ede(args)
#define BattleEvent_RunActionAndWait_14_02003054(args...) Func_02008ef6(args)
#define BattleRuntime_WaitIfModeZero_28_02003054(args...) Func_02008e2c(args)
#define Object_LinkPair_3_02003054(args...) Func_02008ee6(args)
#define BattleRuntime_WaitIfModeZero_29_02003054(args...) Func_02008e3c(args)
#define BattleEffect_SpawnLinkedResourceObject_7_02003054(a0, a1, a2) Call3(Func_02008f36_a, a0, a1, a2)
#define ObjectMotion_SetAngleToward_29(args...) Func_02008ef8(args)
#define ObjectMotion_SetAngleToward_30(args...) Func_02008f02_a(args)
#define SceneWork_SetStepValue_16_02003054(args...) Func_02008f20(args)
#define BattleEvent_RunActionAndWait_15_02003054(args...) Func_02008f38(args)
#define BattleEffect_SpawnLinkedResourceObject_8_02003054(a0, a1, a2) Call3(Func_02008f64, a0, a1, a2)
#define ObjectMotion_SetVariantCallback_6_02003054(args...) Func_02008f14(args)
#define BattleRuntime_WaitIfModeZero_30_02003054(args...) Func_02008e82(args)
#define Object_SetModeById_27(args...) Func_02008f0a(args)
#define BattleRuntime_WaitIfModeZero_31_02003054(args...) Func_02008e90(args)
#define SceneWork_SetStepValue_17_02003054(args...) Func_02008f56(args)
#define BattleEvent_RunActionAndWait_16_02003054(args...) Func_02008f6e(args)
#define Object_SetModeById_28(args...) Func_02008f26(args)
#define BattleEffect_SpawnLinkedResourceObject_9_02003054(a0, a1, a2) Call3(Func_02008fa2, a0, a1, a2)
#define SceneWork_SetStepValue_18_02003054(args...) Func_02008f78(args)
#define BattleEvent_RunActionAndWait_17_02003054(args...) Func_02008f90(args)
#define ObjectMotion_CallThenWaitForAnimationChange_3_02003054(args...) Func_02008f50(args)
#define BattleEffect_SpawnLinkedResourceObject_10_02003054(a0, a1, a2) Call3(Func_02008fc2, a0, a1, a2)
#define ObjectMotion_ResetAndSetPositionInMode2_16(a0, a1, a2) Call3(Func_02008f36_b, a0, a1, a2)
#define ObjectMotion_CommitCurrentPositionAndActivate_16(args...) Func_02008f54(args)
#define ObjectMotion_SetAngleToward_31(args...) Func_02008f96(args)
#define SceneWork_SetStepValue_19_02003054(args...) Func_02008fb4(args)
#define BattleEvent_RunActionAndWait_18_02003054(args...) Func_02008fcc(args)
#define BattleRuntime_WaitIfModeZero_32_02003054(args...) Func_02008f02_b(args)
#define BattleEffect_SpawnLinkedResourceObject_11_02003054(a0, a1, a2) Call3(Func_02008ffc, a0, a1, a2)
#define BattleEffect_SpawnLinkedResourceObject_12_02003054(a0, a1, a2) Call3(Func_02009006_a, a0, a1, a2)
#define BattleEffect_SpawnLinkedResourceObject_13_02003054(a0, a1, a2) Call3(Func_02009010, a0, a1, a2)
#define BattleEffect_SpawnLinkedResourceObject_14_02003054(a0, a1, a2) Call3(Func_0200901a, a0, a1, a2)
#define BattleRuntime_WaitIfModeZero_33_02003054(args...) Func_02008f30(args)
#define ObjectMotion_SetAngleToward_32(args...) Func_02008fe2(args)
#define BattleEffect_SpawnLinkedResourceObject_15_02003054(a0, a1, a2) Call3(Func_02009036_a, a0, a1, a2)
#define ObjectMotion_SetAngleToward_33(args...) Func_02008ff8(args)
#define Object_SetModeById_29(args...) Func_02008fd8(args)
#define SceneWork_SetStepValue_20_02003054(args...) Func_0200901e_a(args)
#define BattleEvent_RunActionAndWait_19_02003054(args...) Func_02009036_b(args)
#define BattleEffect_SpawnLinkedResourceObject_16_02003054(a0, a1, a2) Call3(Func_02009060, a0, a1, a2)
#define BattleRuntime_WaitIfModeZero_34_02003054(args...) Func_02008f76(args)
#define SceneWork_SetStepValue_21_02003054(args...) Func_0200903c(args)
#define BattleEvent_RunActionAndWait_20_02003054(args...) Func_02009054(args)
#define ObjectMotion_ResetAndSetPositionInMode2_17(a0, a1, a2) Call3(Func_02008fe8, a0, a1, a2)
#define ObjectMotion_CommitCurrentPositionAndActivate_17(args...) Func_02009006_b(args)
#define Object_SetModeById_30(args...) Func_0200901e_b(args)
#define ObjectMotion_SetAngleToward_34(args...) Func_02009050(args)
#define SceneWork_SetStepValue_22_02003054(args...) Func_02009070(args)
#define BattleEvent_RunActionAndWait_21_02003054(args...) Func_02009088(args)
#define ObjectMotion_SetVariantCallback_7_02003054(args...) Func_02009058(args)
#define BattleRuntime_WaitIfModeZero_35_02003054(args...) Func_02008fc6(args)
#define ObjectMotion_SetAngleToward_35(args...) Func_02009078(args)
#define BattleRuntime_WaitIfModeZero_36_02003054(args...) Func_02008fd6(args)
#define SceneWork_SetStepValue_23_02003054(args...) Func_0200909e(args)
#define BattleEvent_RunActionAndWait_22_02003054(args...) Func_020090b6(args)
#define Object_SetModeById_31(args...) Func_0200908e(args)
#define BattleRuntime_WaitIfModeZero_37_02003054(args...) Func_02009014(args)
#define SceneWork_SetStepValue_24_02003054(args...) Func_020090dc(args)
#define BattleEvent_RunActionAndWait_23_02003054(args...) Func_020090f4_a(args)
#define ObjectMotion_SetVariantCallback_8_02003054(args...) Func_020090c4(args)
#define BattleRuntime_WaitIfModeZero_38_02003054(args...) Func_02009032(args)
#define SceneWork_SetStepValue_25_02003054(args...) Func_020090fa(args)
#define BattleEvent_RunActionAndWait_24_02003054(args...) Func_02009112(args)
#define ObjectMotion_SetAngleToward_36(args...) Func_020090f4_b(args)
#define BattleRuntime_WaitIfModeZero_39_02003054(args...) Func_02009052(args)
#define SceneWork_SetStepValue_26(args...) Func_0200911a(args)
#define BattleEvent_RunActionAndWait_25_02003054(args...) Func_02009132(args)
#define BattleRuntime_WaitIfModeZero_40_02003054(args...) Func_02009068(args)
#define BattleEffect_SpawnLinkedResourceObject_17(a0, a1, a2) Call3(Func_02009164_a, a0, a1, a2)
#define ObjectMotion_SetAngleToward_37(args...) Func_02009126(args)
#define SceneWork_SetStepValue_27(args...) Func_02009146(args)
#define BattleEvent_RunActionAndWait_26(args...) Func_0200915e(args)
#define ObjectMotion_SetAngleToward_38(args...) Func_02009140(args)
#define SceneWork_SetStepValue_28(args...) Func_02009160(args)
#define BattleEvent_RunActionAndWait_27(args...) Func_02009178_a(args)
#define ObjectMotion_SetAngleToward_39(args...) Func_0200915a(args)
#define ObjectMotion_SetAngleToward_40(args...) Func_02009164_b(args)
#define ObjectMotion_SetAngleToward_41(args...) Func_0200916e(args)
#define ObjectMotion_SetAngleToward_42(args...) Func_02009178_b(args)
#define BattleRuntime_WaitIfModeZero_41_02003054(args...) Func_020090d6(args)
#define BattleEffect_SpawnLinkedResourceObject_18(a0, a1, a2) Call3(Func_020091d2, a0, a1, a2)
#define ObjectMotion_SetVariantCallback_9(args...) Func_02009182(args)
#define BattleRuntime_WaitIfModeZero_42_02003054(args...) Func_020090f0(args)
#define ObjectMotion_SetVariantCallback_10(args...) Func_02009190(args)
#define BattleRuntime_WaitIfModeZero_43_02003054(args...) Func_020090fe(args)
#define SceneWork_SetStepValue_29(args...) Func_020091c6(args)
#define BattleEvent_RunActionAndWait_28(args...) Func_020091de(args)
#define BattleEffect_SpawnLinkedResourceObject_19(a0, a1, a2) Call3(Func_02009208, a0, a1, a2)
#define ObjectMotion_ResetAndSetPositionInMode2_18(a0, a1, a2) Call3(Func_0200917c, a0, a1, a2)
#define ObjectMotion_CommitCurrentPositionAndActivate_18(args...) Func_0200919a_a(args)
#define Object_SetModeById_32(args...) Func_020091b2(args)
#define SceneWork_SetStepValue_30(args...) Func_020091fa(args)
#define BattleEvent_RunActionAndWait_29(args...) Func_02009212(args)
#define ObjectMotion_SetAngleToward_43(args...) Func_020091f4_a(args)
#define BattleRuntime_WaitIfModeZero_44_02003054(args...) Func_02009152(args)
#define ObjectMotion_SetVariantCallback_11(args...) Func_020091f2(args)
#define SceneWork_SetStepValue_31(args...) Func_02009222(args)
#define BattleEvent_RunActionAndWait_30(args...) Func_0200923a(args)
#define ObjectMotion_SetAngleToward_44(args...) Func_0200921c(args)
#define BattleRuntime_WaitIfModeZero_45_02003054(args...) Func_0200917a(args)
#define ObjectMotion_SetAngleToward_45(args...) Func_0200922c(args)
#define BattleRuntime_WaitIfModeZero_46_02003054(args...) Func_0200918a(args)
#define ObjectMotion_SetAngleToward_46(args...) Func_0200923c(args)
#define BattleRuntime_WaitIfModeZero_47_02003054(args...) Func_0200919a_b(args)
#define ObjectMotion_SetAngleToward_47(args...) Func_0200924c(args)
#define ObjectMotion_SetAngleToward_48(args...) Func_02009256(args)
#define ObjectMotion_SetAngleToward_49(args...) Func_02009260(args)
#define ObjectMotion_SetAngleToward_50(args...) Func_0200926a(args)
#define BattleRuntime_WaitIfModeZero_48(args...) Func_020091c8(args)
#define ObjectMotion_CallThenWaitForAnimationChange_4_02003054(args...) Func_02009258(args)
#define BattleRuntime_WaitIfModeZero_49(args...) Func_020091d6(args)
#define SceneWork_SetStepValue_32(args...) Func_0200929e(args)
#define BattleEvent_RunActionAndWait_31(args...) Func_020092b6(args)
#define ObjectMotion_SetVariantCallback_12(args...) Func_02009286(args)
#define BattleRuntime_WaitIfModeZero_50(args...) Func_020091f4_b(args)
#define SceneWork_SetStepValue_33(args...) Func_020092bc(args)
#define BattleEvent_RunActionAndWait_32(args...) Func_020092d4(args)
#define BattleEffect_SpawnLinkedResourceObject_20(a0, a1, a2) Call3(Func_02009300, a0, a1, a2)
#define SceneWork_SetStepValue_34(args...) Func_020092d8(args)
#define BattleEvent_RunActionAndWait_33(args...) Func_020092f0(args)
#define ObjectMotion_SetAngleToward_51(args...) Func_020092d2(args)
#define BattleRuntime_WaitIfModeZero_51(args...) Func_02009230(args)
#define SceneWork_SetStepValue_35(args...) Func_020092f8(args)
#define ObjectMotion_SetSpeedParameters_16(a0, a1) Value2(Func_02009308, a0, a1)
#define ObjectMotion_SetAngleToward_52(args...) Func_020092f2(args)
#define ObjectMotion_SetAngleToward_53(args...) Func_020092fc(args)
#define ObjectMotion_SetAngleToward_54(args...) Func_02009306(args)
#define ObjectMotion_SetAngleToward_55(args...) Func_02009310(args)
#define ObjectMotion_SetHorizontalPositionWithTerrain_9(a0, a1, a2) Call3(Func_020092ee, a0, a1, a2)
#define ObjectMotion_SetSpeedParameters_17(a0, a1, a2) Call3(Func_020092b4, a0, a1, a2)
#define UiWork_WaitThenFinalizeCapacity_2(args...) Func_020092ac(args)
#define Audio_PlayCue_3_02003054(args...) Func_02009416(args)
#define SceneWork_SetStepValue_36(args...) Func_0200935e(args)
#define BattleEvent_RunActionAndWait_34(args...) Func_02009376_a(args)
#define Audio_PlayCue_4_02003054(args...) Func_02009434(args)
#define SceneWork_SetStepValue_37(args...) Func_0200937c(args)
#define BattleEvent_RunActionAndWait_35(args...) Func_02009394_a(args)
#define ObjectMotion_SetAngleToward_56(args...) Func_02009376_b(args)
#define ObjectMotion_SetAngleToward_57(args...) Func_02009380(args)
#define ObjectMotion_SetAngleToward_58(args...) Func_0200938a(args)
#define ObjectMotion_SetAngleToward_59(args...) Func_02009394_b(args)
#define ObjectMotion_SetAngleToward_60(args...) Func_0200939e(args)
#define ObjectMotion_SetAngleToward_61(args...) Func_020093a8(args)
#define ObjectMotion_PlaceWithinCameraBounds_1(a0, a1, a2, a3) Call4(Func_02009422, a0, a1, a2, a3)
#define ObjectMotion_SetVariantCallback_13(args...) Func_020093b2(args)
#define BattleRuntime_WaitIfModeZero_52(args...) Func_02009320(args)
#define Audio_PlayCue_5_02003054(args...) Func_020094a6_a(args)
#define SceneWork_SetStepValue_38(args...) Func_020093ee_a(args)
#define BattleEvent_RunActionAndWait_36(args...) Func_02009406(args)
#define ObjectMotion_ResetAndSetPositionInMode2_19(a0, a1, a2) Call3(Func_0200939c, a0, a1, a2)
#define ObjectMotion_CommitCurrentPositionAndActivate_19(args...) Func_020093ba(args)
#define ObjectMotion_ResetAndSetPositionInMode2_20(a0, a1, a2) Call3(Func_020093b0, a0, a1, a2)
#define ObjectMotion_PlaceWithinCameraBounds_2(a0, a1, a2, a3) Call4(Func_0200947a, a0, a1, a2, a3)
#define SceneWork_SetStepValue_39(args...) Func_02009430(args)
#define BattleEvent_RunActionAndWait_37(args...) Func_02009448(args)
#define ObjectMotion_CommitCurrentPositionAndActivate_20(args...) Func_020093ee_b(args)
#define ObjectMotion_ArmCallback_20(a0, a1, a2) Call3(Func_0200946a_a, a0, a1, a2)
#define BattleEffect_SpawnLinkedResourceObject_21(a0, a1, a2) Call3(Func_02009486, a0, a1, a2)
#define Object_SetModeById_33(args...) Func_0200941e(args)
#define BattleRuntime_WaitIfModeZero_53(args...) Func_020093a4(args)
#define SceneWork_SetStepValue_40(args...) Func_0200946a_b(args)
#define BattleEvent_RunActionAndWait_38(args...) Func_02009482(args)
#define ObjectMotion_SetVariantCallbackAndInvokeObject_1_02003054(args...) Func_0200945a(args)
#define ObjectMotion_CallThenWaitForAnimationChange_5_02003054(args...) Func_0200944a(args)
#define SceneWork_SetStepValue_41(args...) Func_02009488(args)
#define BattleEvent_RunActionAndWait_39(args...) Func_020094a0(args)
#define BattleEffect_SpawnLinkedResourceObject_22(a0, a1, a2) Call3(Func_020094cc, a0, a1, a2)
#define Object_SetModeById_34(args...) Func_02009464(args)
#define BattleRuntime_WaitIfModeZero_54(args...) Func_020093ea(args)
#define ObjectMotion_ArmCallback_21(a0, a1, a2) Call3(Func_020094d6, a0, a1, a2)
#define BattleRuntime_WaitIfModeZero_55(args...) Func_020093fc(args)
#define SceneWork_SetStepValue_42(args...) Func_020094c2(args)
#define BattleEvent_RunActionAndWait_40(args...) Func_020094da(args)
#define ObjectMotion_SetVariantCallback_14(args...) Func_020094aa(args)
#define SceneWork_SetStepValue_43(args...) Func_020094d8(args)
#define BattleEvent_RunActionAndWait_41(args...) Func_020094f0(args)
#define BattleRuntime_WaitIfModeZero_56(args...) Func_02009426(args)
#define ObjectMotion_CallThenWaitForAnimationChange_6(args...) Func_020094b6(args)
#define ObjectMotion_SetAngleToward_62(args...) Func_020094e0(args)
#define BattleRuntime_WaitIfModeZero_57(args...) Func_0200943e(args)
#define SceneWork_SetStepValue_44(args...) Func_02009504(args)
#define BattleEvent_RunActionAndWait_42(args...) Func_0200951c(args)
#define ObjectMotion_SetVariantCallback_15(args...) Func_020094ec(args)
#define BattleEffect_SpawnLinkedResourceObject_23(a0, a1, a2) Call3(Func_02009550, a0, a1, a2)
#define SceneWork_SetStepValue_45(args...) Func_02009526(args)
#define BattleEvent_RunActionAndWait_43(args...) Func_0200953e_a(args)
#define BattleEffect_SpawnLinkedResourceObject_24(a0, a1, a2) Call3(Func_02009568, a0, a1, a2)
#define SceneWork_SetStepValue_46(args...) Func_02009540(args)
#define BattleEvent_RunActionAndWait_44(args...) Func_02009558_a(args)
#define ObjectMotion_CallThenWaitForAnimationChange_7(args...) Func_02009518(args)
#define SceneWork_SetStepValue_47(args...) Func_02009558_b(args)
#define BattleEvent_RunActionAndWait_45(args...) Func_02009570(args)
#define BattleRuntime_WaitIfModeZero_58(args...) Func_020094a6_b(args)
#define BattleRuntime_WaitIfModeZero_59(a0, a1) Call2(Func_020095a6_a, a0, a1)
#define BattleRuntime_WaitIfModeZero_60(args...) Func_020094b4(args)
#define BattleRuntime_WaitIfModeZero_61(args...) Func_020095b4(args)
#define ObjectMotion_ResetAndSetPositionInMode2_21(a0, a1, a2) Call3(Func_02009520, a0, a1, a2)
#define ObjectMotion_CommitCurrentPositionAndActivate_21(args...) Func_0200953e_b(args)
#define ObjectMotion_SetAngleToward_63(args...) Func_02009580(args)
#define BattleRuntime_WaitIfModeZero_62(args...) Func_020094de(args)
#define ObjectMotion_SetAngleToward_64(args...) Func_02009590_a(args)
#define SceneWork_SetStepValue_48(args...) Func_020095b0(args)
#define BattleEvent_RunActionAndWait_46(args...) Func_020095c8_a(args)
#define ObjectMotion_SetVariantCallback_16(args...) Func_02009598(args)
#define SceneWork_SetStepValue_49(args...) Func_020095c8_b(args)
#define BattleEvent_RunActionAndWait_47(args...) Func_020095e0(args)
#define BattleRuntime_WaitIfModeZero_63(args...) Func_02009516(args)
#define ObjectMotion_CallThenWaitForAnimationChange_8(args...) Func_020095a6_b(args)
#define BattleRuntime_WaitIfModeZero_64(args...) Func_02009524(args)
#define SceneWork_SetStepValue_50(args...) Func_020095ec(args)
#define BattleEvent_RunActionAndWait_48(args...) Func_02009604(args)
#define Object_SetModeById_35(args...) Func_020095bc(args)
#define BattleEffect_SpawnLinkedResourceObject_25(a0, a1, a2) Call3(Func_02009638, a0, a1, a2)
#define SceneWork_SetStepValue_51(args...) Func_02009610_a(args)
#define BattleEvent_RunActionAndWait_49(args...) Func_02009628(args)
#define BattleEffect_SpawnLinkedResourceObject_26(a0, a1, a2) Call3(Func_02009654, a0, a1, a2)
#define SceneWork_SetStepValue_52(args...) Func_0200962c(args)
#define BattleEvent_RunActionAndWait_50(args...) Func_02009644(args)
#define Object_SetModeById_36(args...) Func_020095fc(args)
#define BattleRuntime_WaitIfModeZero_65(args...) Func_02009582(args)
#define Object_SetModeById_37(args...) Func_0200960a(args)
#define BattleRuntime_WaitIfModeZero_66(args...) Func_02009590_b(args)
#define Object_LinkPair_4_02003054(args...) Func_0200964a(args)
#define Object_SetModeById_38(args...) Func_02009622(args)
#define Object_SetModeById_39(args...) Func_0200962a(args)
#define Object_SetModeById_40(args...) Func_02009632(args)
#define Object_SetModeById_41(args...) Func_0200963a(args)
#define BattleRuntime_WaitIfModeZero_67(args...) Func_020095c0(args)
#define BattleEffect_SpawnLinkedResourceObject_27(a0, a1, a2) Call3(Func_020096bc, a0, a1, a2)
#define SceneWork_SetStepValue_53(args...) Func_02009694(args)
#define BattleEvent_RunActionAndWait_51(args...) Func_020096ac(args)
#define BattleEffect_SpawnLinkedResourceObject_28(a0, a1, a2) Call3(Func_020096d6, a0, a1, a2)
#define SceneWork_SetStepValue_54(args...) Func_020096ae(args)
#define BattleEvent_RunActionAndWait_52(args...) Func_020096c6(args)
#define Object_LinkPair_5(args...) Func_020096b0(args)
#define Object_LinkPair_6(args...) Func_020096ba(args)
#define BattleRuntime_WaitIfModeZero_68(args...) Func_02009610_b(args)
#define ObjectMotion_SetAngleToward_65(args...) Func_020096c2(args)
#define BattleRuntime_WaitIfModeZero_69(args...) Func_02009620(args)
#define ObjectMotion_SetAngleToward_66(args...) Func_020096d2(args)
#define ObjectMotion_SetAngleToward_67(args...) Func_020096dc(args)
#define ObjectMotion_SetAngleToward_68(args...) Func_020096e6(args)
#define ObjectMotion_SetAngleToward_69(args...) Func_020096f0(args)
#define BattleRuntime_WaitIfModeZero_70(args...) Func_0200964e(args)
#define SceneWork_SetStepValue_55(args...) Func_02009716_a(args)
#define BattleEvent_RunActionAndWait_53(args...) Func_0200972e(args)
#define BattleEffect_SpawnLinkedResourceObject_29(a0, a1, a2) Call3(Func_02009758, a0, a1, a2)
#define BattleEffect_SpawnLinkedResourceObject_30(a0, a1, a2) Call3(Func_02009762, a0, a1, a2)
#define BattleEffect_SpawnLinkedResourceObject_31(a0, a1, a2) Call3(Func_0200976c_a, a0, a1, a2)
#define BattleEffect_SpawnLinkedResourceObject_32(a0, a1, a2) Call3(Func_02009776, a0, a1, a2)
#define BattleRuntime_WaitIfModeZero_71(args...) Func_0200968c(args)
#define SceneWork_SetStepValue_56(args...) Func_02009754(args)
#define BattleEvent_RunActionAndWait_54(args...) Func_0200976c_b(args)
#define ObjectMotion_SetVariantCallback_17(args...) Func_0200973c(args)
#define BattleRuntime_WaitIfModeZero_72(args...) Func_020096aa(args)
#define SceneWork_SetStepValue_57(args...) Func_02009772(args)
#define BattleEvent_RunActionAndWait_55(args...) Func_0200978a(args)
#define ObjectMotion_SetAngleToward_70(args...) Func_0200976c_c(args)
#define BattleRuntime_WaitIfModeZero_73(args...) Func_020096ca(args)
#define SceneWork_SetStepValue_58(args...) Func_02009792(args)
#define BattleEvent_RunActionAndWait_56(args...) Func_020097aa(args)
#define BattleEffect_SpawnLinkedResourceObject_33(a0, a1, a2) Call3(Func_020097d4, a0, a1, a2)
#define BattleEffect_SpawnLinkedResourceObject_34(a0, a1, a2) Call3(Func_020097de_a, a0, a1, a2)
#define BattleEffect_SpawnLinkedResourceObject_35(a0, a1, a2) Call3(Func_020097e8_a, a0, a1, a2)
#define BattleEffect_SpawnLinkedResourceObject_36(a0, a1, a2) Call3(Func_020097f2, a0, a1, a2)
#define BattleRuntime_WaitIfModeZero_74(args...) Func_02009708(args)
#define Object_SetModeById_42(args...) Func_02009790(args)
#define BattleRuntime_WaitIfModeZero_75(args...) Func_02009716_b(args)
#define SceneWork_SetStepValue_59(args...) Func_020097de_b(args)
#define BattleEvent_RunActionAndWait_57(args...) Func_02009804(args)
#define BattleEffect_SpawnLinkedResourceObject_37(a0, a1, a2) Call3(Func_02009830, a0, a1, a2)
#define SceneWork_SetStepValue_60(args...) Func_02009808(args)
#define BattleEvent_RunActionAndWait_58(args...) Func_02009820(args)
#define Object_SetModeById_43(args...) Func_020097d8(args)
#define Object_SetModeById_44(args...) Func_020097e0(args)
#define Object_SetModeById_45(args...) Func_020097e8_b(args)
#define Object_SetModeById_46(args...) Func_020097f0(args)
#define Object_SetModeById_47(args...) Func_020097f8(args)
#define BattleRuntime_WaitIfModeZero_76(args...) Func_0200977e(args)
#define Object_SetModeById_48(args...) Func_02009806(args)
#define BattleRuntime_WaitIfModeZero_77(args...) Func_0200978c(args)
#define SceneWork_SetStepValue_61(args...) Func_02009854(args)
#define BattleEvent_RunActionAndWait_59(args...) Func_0200986c(args)
#define BattleEffect_SpawnLinkedResourceObject_38(a0, a1, a2) Call3(Func_02009898, a0, a1, a2)
#define SceneWork_SetStepValue_62(args...) Func_02009870_a(args)
#define BattleEvent_RunActionAndWait_60(args...) Func_02009888(args)
#define ObjectMotion_SetAngleToward_71(args...) Func_0200986a(args)
#define BattleRuntime_WaitIfModeZero_78(args...) Func_020097c8(args)
#define ObjectMotion_SetVariantCallback_18(args...) Func_02009868(args)
#define BattleRuntime_WaitIfModeZero_79(args...) Func_020097d6(args)
#define SceneWork_SetStepValue_63(args...) Func_0200989e(args)
#define BattleEvent_RunActionAndWait_61(args...) Func_020098b6_a(args)
#define Object_SetModeById_49(args...) Func_0200986e(args)
#define BattleRuntime_WaitIfModeZero_80(args...) Func_020097f4(args)
#define SceneWork_SetStepValue_64(args...) Func_020098bc_a(args)
#define BattleEvent_RunActionAndWait_62(args...) Func_020098d4_a(args)
#define Object_SetModeById_50(args...) Func_0200988c(args)
#define SceneWork_SetStepValue_65(args...) Func_020098d4_b(args)
#define BattleEvent_RunActionAndWait_63(args...) Func_020098ec(args)
#define ObjectMotion_SetAngleToward_72(args...) Func_020098ce(args)
#define ObjectMotion_CallThenWaitForAnimationChange_9(args...) Func_020098b6_b(args)
#define BattleRuntime_WaitIfModeZero_81(args...) Func_02009834(args)
#define SceneWork_SetStepValue_66(args...) Func_020098fc(args)
#define BattleEvent_RunActionAndWait_64(args...) Func_02009914(args)
#define BattleEffect_SpawnLinkedResourceObject_39(a0, a1, a2) Call3(Func_0200993e_a, a0, a1, a2)
#define SceneWork_SetStepValue_67(args...) Func_02009916(args)
#define BattleEvent_RunActionAndWait_65(args...) Func_0200992e(args)
#define ObjectMotion_ArmCallback_22(a0, a1, a2) Call3(Func_0200994a, a0, a1, a2)
#define BattleRuntime_WaitIfModeZero_82(args...) Func_02009870_b(args)
#define SceneWork_SetStepValue_68(args...) Func_02009938(args)
#define BattleEvent_RunActionAndWait_66(args...) Func_02009950_a(args)
#define ObjectMotion_SetVariantCallback_19(args...) Func_02009920(args)
#define BattleRuntime_WaitIfModeZero_83(args...) Func_0200988e(args)
#define SceneWork_SetStepValue_69(args...) Func_02009956(args)
#define BattleEvent_RunActionAndWait_67(args...) Func_0200996e(args)
#define ObjectMotion_SetAngleToward_73(args...) Func_02009950_b(args)
#define BattleRuntime_WaitIfModeZero_84(args...) Func_020098ae(args)
#define ObjectMotion_CallThenWaitForAnimationChange_10(args...) Func_0200993e_b(args)
#define BattleRuntime_WaitIfModeZero_85(args...) Func_020098bc_b(args)
#define SceneWork_SetStepValue_70(args...) Func_02009984(args)
#define BattleEvent_RunActionAndWait_68(args...) Func_0200999c(args)
#define BattleEffect_SpawnLinkedResourceObject_40(a0, a1, a2) Call3(Func_020099c8, a0, a1, a2)
#define SceneWork_SetStepValue_71(args...) Func_020099a0(args)
#define BattleEvent_RunActionAndWait_69(args...) Func_020099b8(args)
#define ObjectMotion_SetAngleToward_74(args...) Func_0200999a(args)
#define BattleRuntime_WaitIfModeZero_86(args...) Func_020098f8(args)
#define SceneWork_SetStepValue_72(args...) Func_020099c0(args)
#define BattleEvent_RunActionAndWait_70(args...) Func_020099d8(args)
#define BattleEffect_SpawnLinkedResourceObject_41(a0, a1, a2) Call3(Func_02009a04, a0, a1, a2)
#define SceneWork_SetStepValue_73(args...) Func_020099dc(args)
#define BattleEvent_RunActionAndWait_71(args...) Func_020099f4_a(args)
#define ObjectMotion_CallThenWaitForAnimationChange_11(args...) Func_020099b4(args)
#define SceneWork_SetStepValue_74(args...) Func_020099f4_b(args)
#define BattleEvent_RunActionAndWait_72(args...) Func_02009a0c(args)
#define ObjectMotion_SetAngleToward_75(args...) Func_020099ee(args)
#define BattleRuntime_WaitIfModeZero_87(args...) Func_0200994c(args)
#define Object_SetModeById_51(args...) Func_020099d4(args)
#define BattleRuntime_WaitIfModeZero_88(args...) Func_0200995a(args)
#define SceneWork_SetStepValue_75(args...) Func_02009a22(args)
#define ObjectMotion_SetSpeedParameters_18(a0, a1) Value2(Func_02009a32, a0, a1)
#define UiWork_WaitThenFinalizeCapacity_3(args...) Func_02009992(args)
#define SceneWork_SetStepValue_76(args...) Func_02009a3e(args)
#define BattleEvent_RunActionAndWait_73(args...) Func_02009a56(args)
#define BattleRuntime_WaitIfModeZero_89(args...) Func_0200998c(args)
#define Object_SetModeById_52(args...) Func_02009a14(args)
#define Object_SetModeById_53(args...) Func_02009a1c(args)
#define Object_SetModeById_54(args...) Func_02009a24(args)
#define BattleRuntime_WaitIfModeZero_90(args...) Func_020099aa(args)
#define Object_LinkPair_7(args...) Func_02009a64(args)
#define Object_SetModeById_55(args...) Func_02009a3c(args)
#define Object_SetModeById_56(args...) Func_02009a44(args)
#define BattleRuntime_WaitIfModeZero_91(args...) Func_020099ca(args)
#define Object_SetModeById_57(args...) Func_02009a52(args)
#define Scene_GetRecord_2_02003054(args...) Func_02009a00(args)
#define ObjectMotion_ResetAndSetPosition_1(args...) Func_02009a3a(args)
#define ObjectMotion_CommitCurrentPositionAndActivate_22(args...) Func_02009a60_a(args)
#define ObjectMotion_SetHorizontalPositionWithTerrain_10(args...) Func_02009a72(args)
#define Object_SetModeById_58(args...) Func_02009a82(args)
#define Scene_GetRecord_3(args...) Func_02009a30(args)
#define ObjectMotion_ResetAndSetPosition_2(args...) Func_02009a6a(args)
#define ObjectMotion_CommitCurrentPositionAndActivate_23(args...) Func_02009a90(args)
#define ObjectMotion_SetHorizontalPositionWithTerrain_11(args...) Func_02009aa2(args)
#define Object_SetModeById_59(args...) Func_02009ab2(args)
#define Scene_GetRecord_4(args...) Func_02009a60_b(args)
#define ObjectMotion_ResetAndSetPosition_3(args...) Func_02009a9a_a(args)
#define ObjectMotion_CommitCurrentPositionAndActivate_24(args...) Func_02009ac0(args)
#define ObjectMotion_SetHorizontalPositionWithTerrain_12(args...) Func_02009ad2(args)
#define BattleRuntime_WaitIfModeZero_92(args...) Func_02009a60_c(args)
#define ObjectMotion_SetSpeedParameters_19(a0, a1, a2) Call3(Func_02009a9a_b, a0, a1, a2)
#define ObjectMotion_ResetAndSetPositionInMode2_22(a0, a1, a2) Call3(Func_02009ace, a0, a1, a2)
#define ObjectMotion_CommitCurrentPositionAndActivate_25(args...) Func_02009aec(args)
#define Object_SetModeById_60(args...) Func_02009b04(args)
#define ObjectMotion_ArmCallback_23(a0, a1, a2) Call3(Func_02009b70_a, a0, a1, a2)
#define ObjectMotion_SetAngleToward_76(args...) Func_02009b42(args)
#define Object_SetModeById_61(args...) Func_02009b22(args)
#define BattleRuntime_WaitIfModeZero_93(args...) Func_02009aa8(args)
#define ObjectMotion_SetAngleToward_77(args...) Func_02009b5a(args)
#define BattleRuntime_WaitIfModeZero_94(args...) Func_02009ab8(args)
#define SceneWork_SetStepValue_77(args...) Func_02009b80(args)
#define BattleEvent_RunActionAndWait_74(args...) Func_02009b98_a(args)
#define ObjectMotion_SetVariantCallbackAndInvokeObject_2(args...) Func_02009b70_b(args)
#define SceneWork_SetStepValue_78(args...) Func_02009b98_b(args)
#define BattleEvent_RunActionAndWait_75(args...) Func_02009bb0(args)
#define Object_SetModeById_62(args...) Func_02009b68(args)
#define BattleRuntime_WaitIfModeZero_95(args...) Func_02009aee(args)
#define ObjectMotion_SetSpeedParameters_20(a0, a1, a2) Call3(Func_02009b2c, a0, a1, a2)
#define ObjectMotion_SetSpeedParameters_21(a0, a1, a2) Call3(Func_02009b3a, a0, a1, a2)
#define ObjectMotion_SetPositionAndReset_1_02003054(args...) Func_02009c12(args)
#define ObjectMotion_ResetAndSetPositionInMode2_23(a0, a1, a2) Call3(Func_02009b76, a0, a1, a2)
#define BattleRuntime_WaitIfModeZero_96(args...) Func_02009b24(args)
#define ObjectMotion_SetActionVariant_2(args...) Func_02009c14(args)
#define ObjectMotion_SetActionVariant_3(args...) Func_02009c1c(args)
#define ObjectMotion_ResetAndSetPositionInMode2_24(a0, a1, a2) Call3(Func_02009b98_c, a0, a1, a2)
#define ObjectMotion_CommitCurrentPositionAndActivate_26(args...) Func_02009bb6(args)
#define ObjectMotion_SetAngleToward_78(args...) Func_02009bf8(args)
#define ObjectMotion_CommitCurrentPositionAndActivate_27(args...) Func_02009bc6(args)
#define ObjectMotion_SetAngleToward_79(args...) Func_02009c08(args)
#define ObjectMotion_ArmCallback_24(a0, a1, a2) Call3(Func_02009c4c_a, a0, a1, a2)
#define Object_SetModeById_63(args...) Func_02009bf4(args)
#define BattleRuntime_WaitIfModeZero_97(args...) Func_02009b7a(args)
#define Object_SetModeById_64(args...) Func_02009c02(args)
#define BattleRuntime_WaitIfModeZero_98(args...) Func_02009b88(args)
#define Object_SetModeById_65(args...) Func_02009c10(args)
#define BattleRuntime_WaitIfModeZero_99(args...) Func_02009b96(args)
#define ObjectMotion_ResetAndSetPositionInMode2_25(a0, a1, a2) Call3(Func_02009bfa, a0, a1, a2)
#define ObjectMotion_ResetAndSetPositionInMode2_26(a0, a1, a2) Call3(Func_02009c06, a0, a1, a2)
#define ObjectMotion_CommitCurrentPositionAndActivate_28(args...) Func_02009c24(args)
#define ObjectMotion_ResetAndSetPositionInMode2_27(a0, a1, a2) Call3(Func_02009c18, a0, a1, a2)
#define ObjectMotion_CommitCurrentPositionAndActivate_29(args...) Func_02009c36(args)
#define ObjectMotion_ResetAndSetPositionInMode2_28(a0, a1, a2) Call3(Func_02009c2c, a0, a1, a2)
#define ObjectMotion_CommitCurrentPositionAndActivate_30(args...) Func_02009c4a(args)
#define ObjectMotion_ResetAndSetPositionInMode2_29(a0, a1, a2) Call3(Func_02009c40, a0, a1, a2)
#define ObjectMotion_CommitCurrentPositionAndActivate_31(args...) Func_02009c5e(args)
#define ObjectMotion_ResetAndSetPositionInMode2_30(a0, a1, a2) Call3(Func_02009c54, a0, a1, a2)
#define ObjectMotion_CommitCurrentPositionAndActivate_32(args...) Func_02009c72(args)
#define ObjectMotion_ResetAndSetPositionInMode2_31(a0, a1, a2) Call3(Func_02009c68, a0, a1, a2)
#define BattleRuntime_WaitIfModeZero_100(args...) Func_02009c16(args)
#define Audio_PlayCue_6_02003054(args...) Func_02009d9c(args)
#define BattleRuntime_WaitIfModeZero_101(args...) Func_02009c46(args)
#define BattleRuntime_WaitIfModeZero_102(args...) Func_02009c4c_b(args)
#define BattleRuntime_ScheduleShoulderButtonModeUpdate_1_02003054(args...) Func_02009c66(args)
#define SCENE_PHASE_02003054 (*(s32 *)(*(u8 **)0x03001ebc + 0x1c0))
#define PRIMARY_ID 24
#define DERIVED_ID 25
#define GameFlag_IsSet_1_020049a0(a0) Value1(Func_02009fb6, a0)
#define GameFlag_IsSet_2_020049a0(a0) Value1(Func_02009fc2, a0)
#define GameFlag_IsSet_3(a0) Value1(Func_02009fd0_a, a0)
#define SceneWork_SetStepValue_1_020049a0(a0) Call1(Func_0200a0b6, a0)
#define BattleEvent_RunActionAndWait_1_020049a0(a0, a1) Call2(Func_0200a0ce, a0, a1)
#define ObjectMotion_SetVariantCallbackAndInvokeObject_1_020049a0(a0, a1) Value2(Func_0200a0a6, a0, a1)
#define BattleRuntime_WaitIfModeZero_1_020049a0(a0) Value1(Func_0200a00c, a0)
#define ObjectMotion_SetSpeedParameters_1_020049a0(a0, a1, a2) Call3(Func_0200a046, a0, a1, a2)
#define ObjectMotion_OffsetPositionAndResetMotion_1_020049a0(a0, a1, a2) Value3(Func_0200a08a, a0, a1, a2)
#define ObjectMotion_CommitCurrentPositionAndActivate_1_020049a0(a0) Value1(Func_0200a098, a0)
#define Object_SetModeById_1_020049a0(a0, a1) Value2(Func_0200a0b0, a0, a1)
#define BattleRuntime_WaitIfModeZero_2_020049a0(a0) Value1(Func_0200a036, a0)
#define ObjectMotion_SetSpeedParameters_2_020049a0(a0, a1, a2) Call3(Func_0200a070, a0, a1, a2)
#define ObjectMotion_OffsetPositionAndResetMotion_2_020049a0(a0, a1, a2) Call3(Func_0200a0b4, a0, a1, a2)
#define ObjectMotion_SetAngleToward_1_020049a0(a0, a1, a2) Value3(Func_0200a0fe, a0, a1, a2)
#define ObjectMotion_CommitCurrentPositionAndActivate_2_020049a0(a0) Value1(Func_0200a0cc, a0)
#define SceneWork_SetStepValue_2_020049a0(a0) Value1(Func_0200a122, a0)
#define BattleEvent_RunActionAndWait_2_020049a0(a0, a1) Call2(Func_0200a13a_a, a0, a1)
#define ObjectMotion_SetVariantCallbackAndInvokeObject_2_020049a0(a0, a1) Call2(Func_0200a112, a0, a1)
#define ObjectMotion_SetAngleToward_2_020049a0(a0, a1, a2) Value3(Func_0200a124, a0, a1, a2)
#define SceneWork_SetStepValue_3_020049a0(a0) Value1(Func_0200a142, a0)
#define BattleEvent_RunActionAndWait_3_020049a0(a0, a1) Value2(Func_0200a15a, a0, a1)
#define BattleRuntime_WaitIfModeZero_3_020049a0(a0) Value1(Func_0200a090, a0)
#define Object_SetModeById_2_020049a0(a0, a1) Value2(Func_0200a118, a0, a1)
#define BattleRuntime_WaitIfModeZero_4_020049a0(a0) Value1(Func_0200a09e, a0)
#define ObjectMotion_SetSpeedParameters_3_020049a0(a0, a1, a2) Call3(Func_0200a0d8, a0, a1, a2)
#define ObjectMotion_ResetAndSetPositionInMode2_1_020049a0(a0, a1, a2) Value3(Func_0200a10c, a0, a1, a2)
#define ObjectMotion_CommitCurrentPositionAndActivate_3_020049a0(a0) Value1(Func_0200a12a, a0)
#define ObjectMotion_ArmCallback_1_020049a0(a0, a1, a2) Value3(Func_0200a1a6, a0, a1, a2)
#define SceneWork_SetStepValue_4_020049a0(a0) Value1(Func_0200a18c_a, a0)
#define BattleEvent_RunActionAndWait_4_020049a0(a0, a1) Call2(Func_0200a1a4, a0, a1)
#define Object_SetModeById_3_020049a0(a0, a1) Value2(Func_0200a15c, a0, a1)
#define BattleRuntime_WaitIfModeZero_5_020049a0(a0) Value1(Func_0200a0e2, a0)
#define ObjectMotion_OffsetPositionAndResetMotion_3_020049a0(a0, a1, a2) Value3(Func_0200a154, a0, a1, a2)
#define ObjectMotion_CommitCurrentPositionAndActivate_4_020049a0(a0) Value1(Func_0200a162, a0)
#define Object_SetModeById_4_020049a0(a0, a1) Value2(Func_0200a17c, a0, a1)
#define SceneWork_SetStepValue_5_020049a0(a0) Value1(Func_0200a1c2, a0)
#define BattleEvent_RunActionAndWait_5_020049a0(a0, a1) Call2(Func_0200a1da, a0, a1)
#define ObjectMotion_ResetAndSetPositionInMode2_2_020049a0(a0, a1, a2) Value3(Func_0200a16e, a0, a1, a2)
#define ObjectMotion_CommitCurrentPositionAndActivate_5_020049a0(a0) Value1(Func_0200a18c_b, a0)
#define Object_LinkPair_1_020049a0(a0, a1, a2) Value3(Func_0200a1d6, a0, a1, a2)
#define BattleRuntime_WaitIfModeZero_6_020049a0(a0) Value1(Func_0200a12c, a0)
#define Object_SetModeById_5_020049a0(a0, a1) Call2(Func_0200a1b4, a0, a1)
#define BattleRuntime_WaitIfModeZero_7_020049a0(a0) Value1(Func_0200a13a_b, a0)
#define GameFlag_Set_1_020049a0(a0) Value1(Func_0200a132, a0)
#define SceneWork_SetStepValue_6_020049a0(a0) Call1(Func_0200a20a, a0)
#define BattleEvent_RunActionAndWait_6_020049a0(a0, a1) Call2(Func_0200a222, a0, a1)
#define SceneWork_SetStepValue_7_020049a0(a0) Call1(Func_0200a21a, a0)
#define BattleEvent_RunActionAndWait_7_020049a0(a0, a1) Call2(Func_0200a232, a0, a1)
#define ConfigureSceneActor12 Func_02000b4c
#define RunSceneObjectSetup Func_02000ba0
#define FieldScene_StartActorTwelveTransition Func_02000bac
#define FieldScene_UpdateActorTwelveTransition Func_02000bec
#define PlaceSceneObjectPairFromTableA Func_02000c90
#define FieldScene_UpdateObjectPairA Func_02000ce0
#define FieldScene_UpdateObjectPairB Func_02000dcc
#define PlaceSceneObjectPairFromTableB Func_02000e30
#define FieldScene_UpdateTableBObjectPair Func_02000e80
#define PlaceSceneObjectPairFromTableC Func_02000ee0
#define FieldScene_UpdateObjectPairC Func_02000f30
#define FieldScene_UpdateActorPairInteraction Func_02001150
#define FieldScene_UpdateActorSeventeenInteraction Func_0200138c
#define FieldScene_UpdateActorEighteenInteraction Func_02001528
#define FieldScene_RunScene3bf_02001cf0 Func_02001cf0
#define FieldScene_RunSupplementalSequenceOne Func_02001e94
#define FieldScene_RunSequenceTail Func_0200206c
#define FieldScene_RunScene3bf_020021c4 Func_020021c4
#define FieldScene_RunScene3bf_0200252c Func_0200252c
#define FieldScene_RunScene3bf_020025f8 Func_020025f8
#define FieldScene_RunScene3bf_0200269c Func_0200269c
#define FieldScene_RunScene3bf_02002718 Func_02002718
#define FieldScene_RunMainScriptSequence Func_02003054
#define FieldScene_SelectActorTwentyOneMessage Func_02004638
#define FieldScene_RunActorTwentyOneSequence Func_02004704
#define FieldScene_RunScene3bf_02004794 Func_02004794
#define FieldScene_RunSupplementalSequenceTwo Func_020049a0
#define FieldScene_InstallSceneTasks Func_02004f60
#define FieldScene_SetupActorsForScene Func_020050e4
#define FieldScene_RestoreActorsFromFlags Func_02005324
#define FieldScene_ActivateThreeActorGroup Func_02005470
#define FieldScene_ActivateTwoActorGroup Func_020054b0
#define FieldScene_ActivateAlternateActorGroup Func_020054dc

#include "staged_actor.h"
#include "object_runtime.h"

typedef struct Obj {
    u8 pad0[8];
    s32 x;          /* 0x08 */
    s32 y;          /* 0x0c */
    s32 z;          /* 0x10 */
} Obj;

typedef struct Req {
    s32 kind;       /* 0x00 */
    s32 f4;         /* 0x04 */
    s32 x;          /* 0x08 */
    s32 y;          /* 0x0c */
    s32 z;          /* 0x10 */
    s32 f14;        /* 0x14 */
} Req;

typedef struct MovingActor {
    u8 pad0[6];
    u16 kind;       /* 0x06 */
    s32 x;          /* 0x08 */
    s32 y;          /* 0x0c */
    s32 z;          /* 0x10 */
    u8 pad14[0x10];
    s32 vx;         /* 0x24 */
    u8 pad28[4];
    s32 vy;         /* 0x2c */
    u8 pad30[8];
    s32 a;          /* 0x38 */
    u8 pad3c[4];
    s32 b;          /* 0x40 */
} MovingActor;

typedef struct BlockingActor {
    u8 pad0[0x50];
    s32 *info;      /* 0x50 */
} BlockingActor;

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

extern s32 Data_0200df30[];
extern s32 Value_00000f2e;
extern u8 Data_02010000[];
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
extern s32 Data_0200ded8[];
extern s32 Data_0200df18[];
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

typedef s32(*IwramIntegerSquareRoot)(s32);
struct StagedActor *Func_02005724(s32 arg0);
struct StagedActor *Func_02000176(s32 *arg0, struct StagedActor *arg1);
struct StagedActor *Func_020001a2(s32 *arg0, struct StagedActor *arg1);
struct StagedActor *Func_020001ce(s32 *arg0, struct StagedActor *arg1);
s32 Func_02005780(struct StagedActor *arg0, s32 *arg1);
void Func_02005760(struct StagedActor *arg0, s32 arg1);
void Func_02005748(s32 arg0);
void Func_0200596e(s32 arg0);
void Func_02005796(struct StagedActor *arg0, s32 arg1, s32 arg2, s32 arg3);
void Func_020057a6(struct StagedActor *arg0, s32 arg1, s32 arg2, s32 arg3);
void Func_020057b4(struct StagedActor *arg0);
void Func_02005990(void);
void Func_020057cc(struct StagedActor *arg0, s32 arg1);
Obj *Func_020007de(s32 *, s32 *, Req *);
s32 Func_02005b3c(Obj *, s32 *);
void Func_02006736(s32, s32, s32);
void Func_02006754(s32);
void Func_020067d0(s32, s32, s32);
void Func_020066f6(s32);
void Func_0200677e(s32, s32);
void Func_02006866(s32, s32);
void Func_0200678e(s32, s32);
void Func_0200672e(s32, s32);
void Func_0200670c(s32);
void Func_0200679e(s32, s32, s32);
void Func_020072be(s32, s32, s32);
void Func_020072c8(s32, s32, s32);
void Func_0200386a(s32);
void Func_020038ca(s32);
void Func_0200391e(s32);
void Func_02003996(s32);
void Func_020038e2(s32);
s32 Func_02009f3e(s32);
void Func_0200a020(s32);
void Func_0200a038(s32, s32);
void Func_0200a030(s32);
void Func_0200a048(s32, s32);
s32 Func_02009f7a(s32);
void Func_0200a05c(s32);
void Func_0200a074(s32, s32);
void Func_0200a06c(s32);
void Func_0200a084(s32, s32);
void Func_02006326(s32, s32, s32);
void Func_020017d6();
s32 Func_020065a2(s32);
void Func_020065aa(s32, s32);
void Func_020065bc(s32, s32);
s32 Func_02002008(s32, s32);
void Func_020065f2(s32);
s32 Func_02002024(s32, s32);
void Func_0200660e(s32);
s32 Func_02002040(s32, s32);
void Func_0200662a(s32);
s32 Func_0200205c(s32, s32);
void Func_02006646(s32);
BlockingActor *Func_02000342(s32 *position, MovingActor *actor);
s32 Func_02005902(MovingActor *actor, s32 *position);
void Func_02002fe6(s32);
void Func_02006afe(void);
void Func_0200317a(s32);
void Func_02006c92(void);
s32 Func_0200a322(s32);
void Func_0200a404(s32);
void Func_0200a41c(s32, s32);
void Func_0200a4c6(void);
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
void Func_02006914(void);
void Func_0200695a(s32);
void Func_02006984(s32, s32, s32);
void Func_020069a4(s32, s32);
void Func_020069d6(s32, s32, s32);
void Func_02006a2a(s32, s32, s32);
void Func_02002e3c(s32);
void Func_02006954(void);
void Func_02007852(s32, s32, s32, s32, s32, s32);
void Func_0200786c(s32, s32, s32, s32, s32, s32);
void Func_02007886(s32, s32, s32, s32, s32, s32);
void Func_020078a0(s32, s32, s32, s32, s32, s32);
void Func_020078ba(s32, s32, s32, s32, s32, s32);
void Func_020078d4(s32, s32, s32, s32, s32, s32);
void Func_020079f6(s32, s32, s32, s32, s32, s32);
void Func_02007a0a(s32, s32, s32, s32, s32, s32);
void Func_02007a24(s32, s32, s32, s32, s32, s32);
void Func_02007a36(s32, s32, s32, s32, s32, s32);
void Func_02007a4e(s32, s32, s32, s32, s32, s32);
void Func_02007a62(s32, s32, s32, s32, s32, s32);
void Func_02007a7c(s32, s32, s32, s32, s32, s32);
void Func_02007a8e(s32, s32, s32, s32, s32, s32);
void Func_0200a30a(s32, s32, s32);
void Func_0200a190(s32);
void Func_0200a254(s32, s32);
void Func_0200a27a(s32);
void Func_0200a292(s32, s32);
void Func_0200a274(s32, s32);
void Func_0200a29a(s32);
void Func_0200a2b2(s32, s32);
void Func_0200a294(s32, s32);
void Func_0200a2ba(s32);
void Func_0200a2d2(s32, s32);
void Func_0200a2d2_a(s32);
s32 Func_0200a2f2(s32, s32);
void Func_0200a440(s32);
void Func_0200a458(s32, s32);
s32 Func_0200a36e(s32);
void Func_0200a376(s32, s32);
void Func_0200a476(s32);
void Func_0200a48e(s32, s32);
void Func_02006678(void);
s32 Func_020067d2(s32, s32, s32);
s32 Func_0200669c(s32, s32);
void Func_02006714(s32, s32);
void Func_02006820(s32);
void Func_02006728(s32, s32);
void Func_02006636(s32);
void Func_020066c2(void);
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
void Func_0200613a(s32, s32, s32, s32, s32, s32);
void Func_0200614c(s32, s32, s32, s32, s32, s32);
u8 *Func_020061ca(s32);
void Func_0200616e(u8 *, s32);
void Func_02000c68(void);
void Func_02001728();
void Func_020061fa();
void Func_02006200();
void Func_02006212();
void Func_0200623e();
void Func_02006264();
void Func_0200637a();
void Func_020061fc();
void Func_0200620a();
void Func_0200620c();
void Func_0200621e();
void Func_02006240();
s32 Func_02006244();
void Func_0200627e();
s32 Func_0200629c();
void Func_020063c8();
void Func_0200627c(s32, s32, s32, s32, s32, s32);
void Func_0200628e(s32, s32, s32, s32, s32, s32);
void Func_020062a2(s32, s32, s32, s32, s32, s32);
void Func_020019a4();
s32 Func_020062f2();
s32 Func_02006316();
void Func_0200631a();
void Func_02006324();
void Func_02006356();
void Func_020064c2();
void Func_02001b44();
s32 Func_020063de();
void Func_020063f6();
void Func_02006400();
void Func_02006432();
void Func_0200659e();
void Func_0200641c(s32, s32, s32, s32, s32, s32);
void Func_0200642e(s32, s32, s32, s32, s32, s32);
void Func_02006442(s32, s32, s32, s32, s32, s32);
void Func_020064cc(s32, s32, s32, s32, s32, s32);
void Func_020064de(s32, s32, s32, s32, s32, s32);
void Func_020064fa(s32, s32, s32, s32, s32, s32);
void Func_02001e34();
s32 Func_02006542();
void Func_0200655a();
void Func_02006564();
void Func_02006596();
void Func_02006702();
s32 Func_02006492();
void Func_02001cd4();
void Func_02006652();
void Func_020064aa();
void Func_020064b4();
void Func_020064e6();
void Func_02009d70();
void Func_02009d88();
void Func_02009da6();
void Func_02009db8();
void Func_02009dca();
void Func_02009ccc();
void Func_02009cde();
void Func_02009cf0();
void Func_02009db6();
void Func_02009dce();
void Func_02009d48();
void Func_02009d5a();
void Func_02009d6c();
void Func_02009d8a();
void Func_02009d9e();
void Func_02009e04();
void Func_02009e22();
void Func_02009e26();
void Func_02009e34_a();
void Func_02009e34_b();
void Func_02009e46();
void Func_02009e4c();
void Func_02009e64();
void Func_02009e76();
void Func_02009e7c();
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
void Func_020068a0();
struct ObjectRuntime *Func_020069e6();
void Func_02006afa();
s32 Func_020069dc();
s32 Func_02006a0e();
s32 Func_02002756();
s32 Func_02002dbc();
s32 Func_02002d5a();
s32 Func_02002d58();
void Func_02006a96();
struct ObjectRuntime *Func_02006b82();
s32 Func_02006b74();
s32 Func_02006ba6();
s32 Func_02002a96();
s32 Func_02002eec();
s32 Func_02002eea();
void Func_02006c28();
s32 Func_02002d10();
s32 Func_02002fae();
s32 Func_02006cb8();
void Func_02006cec();
s32 Func_02006cf4_a();
s32 Func_02006cf4_b();
void Func_02003232(s32);
void Func_02006d4a(void);
s32 Func_02003006(s32);
void Func_02006cd4(s32);
s32 Func_02003054_scene_scripts();
void Func_02006d28();
void Func_02006d2e();
s32 Func_02006d76();
void Func_02006da2();
s32 Func_020030b4();
void Func_02006d88();
void Func_02006d8e();
s32 Func_02006dd6();
void Func_02006e02();
void Func_020070e4(void);
void Func_0200714e_a();
void Func_0200714e_b();
void Func_02007126(s32, s32);
void Func_0200713c_a();
void Func_0200713c_b();
void Func_0200717c(s32, s32);
void Func_020071cc(s32);
void Func_020071e4(s32, s32);
void Func_02007212(s32, s32, s32);
void Func_020071e8(s32);
void Func_02007200(s32, s32);
void Func_02007266(s32);
void Func_0200728a(void);
void Func_0200715a(void);
void Func_0200715e(void);
void Func_02007252(s32, s32, s32);
void Func_020071d4(s32, s32, s32);
void Func_020071ac(s32, s32);
void Func_020071fc(s32, s32);
void Func_0200722e_a();
void Func_0200722e_b();
void Func_0200720e(s32, s32);
void Func_02007200_a(s32, s32, s32);
void Func_020071d8(s32, s32);
void Func_020071ee_a();
void Func_020071ee_b();
void Func_0200727e(s32);
void Func_02007296(s32, s32);
void Func_02007278(s32, s32, s32);
void Func_020072ce(s32, s32, s32);
void Func_020072a4(s32);
void Func_020072bc(s32, s32);
void Func_02007340(void);
void Func_020071f6(s32);
void Func_0200732c(s32);
void Func_02007210(void);
void Func_0200731c();
void Func_02007338();
void Func_02007344();
void Func_0200735c();
void Func_0200735e();
void Func_02007376_a();
void Func_02007376_b();
void Func_02007388();
void Func_0200738e();
void Func_02007396(s32);
void Func_020073ae(s32, s32);
void Func_02007386_a(s32, s32);
void Func_02007386_b(s32, s32);
void Func_020073ac(s32);
void Func_020073c4_a();
void Func_020073c4_b();
void Func_020073dc(s32, s32);
void Func_02007452(s32);
void Func_0200746a(s32, s32);
void Func_0200742a(s32, s32);
void Func_02007468(s32);
void Func_02007480(s32, s32);
void Func_02007458_a(s32, s32);
void Func_02007458_b(s32, s32);
void Func_0200747e(s32);
void Func_02007496_a();
void Func_02007496_b();
void Func_020074ae(s32, s32);
void Func_02007768(void);
void Func_020077d2(s32, s32, s32);
void Func_020077aa(s32, s32);
void Func_020077fa(s32, s32);
void Func_0200782c(s32, s32, s32);
void Func_0200790a(s32);
void Func_02007886_a(s32, s32, s32);
void Func_0200785e(s32);
void Func_02007876(s32, s32);
void Func_020078a4(s32, s32, s32);
void Func_0200787a(s32);
void Func_02007892(s32, s32);
void Func_02007916(void);
void Func_020077cc(s32);
void Func_02007902(s32);
void Func_020077e6(void);
void Func_020077ce(s32);
void Func_02004690();
void Func_020079a0();
void Func_020079b0();
s32 Func_020079b4();
void Func_020079b6();
void Func_020079e4();
void Func_020079fe();
void Func_02007a0e();
void Func_02007a14();
void Func_02007b5e();
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
void Func_02007dde();
s32 Func_02007de0();
void Func_02007dfa();
void Func_02007e14();
void Func_02007e3c();
void Func_02007e5a();
s32 Func_02007e76();
void Func_02007e78();
void Func_02007e7c();
void Func_02007e94();
void Func_02007ec8();
void Func_02007eca();
void Func_02007ed8();
void Func_02007ee6();
void Func_02007efa();
void Func_02007efc();
void Func_02007f14();
void Func_02007f1e();
void Func_02007f22();
void Func_02007f32();
void Func_02007f38();
void Func_02007f3c();
void Func_02007f5a();
void Func_02007f5e();
void Func_02007f68();
void Func_02007f70();
void Func_02007f74();
void Func_02007f7e();
void Func_02007f80();
void Func_02007f8c();
void Func_02007f8e();
void Func_02007f94();
void Func_02007f96();
void Func_02007fa6();
void Func_02007fce();
void Func_02007fd6();
void Func_02007fd8();
void Func_02007fe0();
void Func_02007fe8();
void Func_02007fea();
void Func_02007fec();
void Func_02007ff0();
void Func_0200800a();
void Func_02008016();
void Func_02008034();
void Func_0200804c();
s32 Func_0200804e();
void Func_0200805e();
void Func_02008078();
void Func_02008084();
void Func_0200809a();
void Func_0200809c();
void Func_020080a4();
void Func_020080b0();
void Func_020080b2();
void Func_020080b4_a();
void Func_020080b4_b();
void Func_020080be();
void Func_020080ca();
void Func_020080cc();
void Func_020080d0();
void Func_020080e2();
void Func_020080e6_a();
void Func_020080e6_b();
void Func_020080ee();
void Func_020080f2();
void Func_020080f6();
void Func_02008106();
void Func_0200810a();
void Func_0200810c();
void Func_0200811c();
void Func_0200811e();
void Func_02008130();
void Func_02008140();
void Func_0200814a();
void Func_0200814e();
void Func_02008150();
void Func_02008154_a();
void Func_02008154_b();
void Func_02008162_a();
void Func_02008162_b();
void Func_02008164();
void Func_0200816e();
void Func_02008170();
void Func_02008174_a();
void Func_02008174_b();
void Func_02008186();
void Func_02008188_a();
void Func_02008188_b();
void Func_0200818e();
void Func_02008196();
void Func_020081a4();
void Func_020081c6();
void Func_020081da();
void Func_020081e2();
void Func_020081e4();
void Func_020081e8();
void Func_020081f8();
void Func_02008206();
void Func_02008208();
void Func_0200820a();
void Func_02008218();
void Func_0200821e();
void Func_02008226_a();
void Func_02008226_b();
void Func_0200822a();
void Func_0200823a();
void Func_0200823e();
void Func_02008252();
void Func_0200825a();
void Func_0200825e();
void Func_02008262();
void Func_02008270();
void Func_0200827c();
void Func_0200828c();
void Func_020082a4();
void Func_020082ae();
void Func_020082b2();
void Func_020082c6();
void Func_020082cc();
void Func_020082d2();
void Func_020082d6();
void Func_020082e0();
void Func_020082e8_a();
void Func_020082e8_b();
void Func_020082ea();
void Func_020082fc();
void Func_02008300();
void Func_0200830e();
void Func_0200832a();
void Func_02008330();
void Func_02008340();
void Func_0200836a();
void Func_02008374();
void Func_0200838a();
void Func_02008390();
void Func_02008392();
void Func_02008394();
void Func_02008398();
void Func_0200839a();
void Func_020083a0();
void Func_020083a2();
void Func_020083a6();
void Func_020083ae();
void Func_020083ba();
void Func_020083c0();
void Func_020083c2();
void Func_020083c6();
void Func_020083c8();
void Func_020083d2();
void Func_020083e0_a();
void Func_020083e0_b();
void Func_020083e2_a();
void Func_020083e2_b();
void Func_020083fa();
void Func_02008408();
void Func_0200840a();
void Func_02008414();
void Func_0200841c();
void Func_02008420();
void Func_02008426();
void Func_02008430();
void Func_02008432();
void Func_0200843a();
void Func_0200843c();
void Func_02008448_a();
void Func_02008448_b();
void Func_02008468_a();
void Func_02008468_b();
void Func_02008478();
void Func_02008480();
void Func_02008488();
void Func_02008490();
void Func_020084a2();
void Func_020084a6();
void Func_020084a8();
void Func_020084aa();
void Func_020084dc_a();
void Func_020084dc_b();
void Func_020084f4_a();
void Func_020084f4_b();
void Func_020084fa();
void Func_0200850c();
void Func_02008510();
void Func_02008522_a();
void Func_02008522_b();
void Func_02008524();
void Func_02008528();
void Func_02008530();
void Func_02008532();
void Func_0200853a();
void Func_0200853c();
void Func_02008548();
void Func_02008550();
void Func_02008552();
void Func_0200856a();
void Func_02008582_a();
void Func_02008582_b();
void Func_02008584();
void Func_02008586();
void Func_0200859a();
void Func_0200859e();
void Func_020085c6_a();
void Func_020085c6_b();
void Func_020085d2();
void Func_020085d4();
void Func_020085de();
void Func_020085e2_a();
void Func_020085e2_b();
void Func_020085e4();
void Func_020085ea();
void Func_020085ec();
void Func_020085f6();
void Func_020085fa_a();
void Func_020085fa_b();
void Func_02008608();
void Func_02008612_a();
void Func_02008612_b();
void Func_02008618();
void Func_0200861c();
void Func_02008620();
void Func_02008630();
void Func_02008636();
void Func_02008660();
void Func_02008664();
void Func_0200866a();
void Func_02008694_a();
void Func_02008694_b();
void Func_0200869e();
void Func_020086a8();
void Func_020086ac();
void Func_020086b2();
void Func_020086bc();
void Func_02008724();
void Func_0200874c();
void Func_02008788_a();
void Func_02008788_b();
s32 Func_0200a212();
s32 Func_0200a23a();
void Func_0200a24a();
void Func_0200a272();
void Func_0200a2a4();
void Func_0200a2ac();
void Func_0200a2ae();
void Func_0200a2b2_a();
void Func_0200a2ea();
void Func_0200a2f0();
void Func_0200a2f4_a();
void Func_0200a2f4_b();
void Func_0200a300();
s32 Func_0200a302();
void Func_0200a30c();
void Func_0200a324();
void Func_0200a32c();
void Func_0200a33a();
void Func_0200a352();
void Func_0200a356();
void Func_0200a36e_a();
void Func_0200a37a();
void Func_0200a380();
void Func_0200a392_a();
void Func_0200a392_b();
s32 Func_0200a3a2();
void Func_0200a3ac();
void Func_0200a3bc_a();
void Func_0200a3bc_b();
void Func_0200a3c2();
void Func_0200a3d2();
void Func_02007330();
void Func_020073e2();
void Func_020073f0_a();
void Func_020073f0_b();
void Func_020073fa();
void Func_02007400();
void Func_02007416();
void Func_02007418();
void Func_0200742a_a();
void Func_0200742e_a();
void Func_0200742e_b();
void Func_02007446();
void Func_02007474(s32, s32);
void Func_0200757a(s32);
void Func_020074f6(s32, s32, s32);
void Func_020039c8(s32);
void Func_02007564(s32, s32);
void Func_020074ae_a(s32, s32, s32);
void Func_0200744a(void);
void Func_02007430(s32);
void Func_020074dc(s32, s32);
void Func_020075e2(s32);
void Func_0200755e(s32, s32, s32);
void Func_02003a30(s32);
void Func_020075cc(s32, s32);
void Func_02007516(s32, s32, s32);
void Func_020074b2(void);
void Func_02007498(s32);
void Func_0200777c(s32, s32);
void Func_02007882(s32);
void Func_020077fe(s32, s32, s32);
void Func_02003cd0(s32);
void Func_0200786c_a(s32, s32);
void Func_020077b6(s32, s32, s32);
void Func_02007752(void);
void Func_02007738(s32);
void Func_020045d8();
void Func_0200791c();
void Func_0200792c();
s32 Func_02007930();
void Func_02007932();
void Func_02007960();
void Func_0200797a();
void Func_0200798a();
void Func_02007990();
void Func_02007ada();
void Func_02007ade(s32);
void Func_02007aec(s32);
void Func_02007b76(s32, s32, s32);
void Func_02007b02(s32);
void Func_020047c2(void);
void Func_02007b98(s32, s32, s32);
void Func_02007b22(s32);
void Func_02007b32(s32);
void Func_02007bbc(s32, s32, s32);
void Func_020074f4();
void Func_02007544();
void Func_0200754a();
void Func_0200754c();
void Func_02007554();
void Func_0200755c();
void Func_020075a6();
void Func_020075c6();
void Func_020075ce();
void Func_020075d8();
void Func_020075de_a();
void Func_020075de_b();
void Func_020075e2_a();
void Func_020075e8();
void Func_020075ec();
void Func_02007600();
void Func_0200760a();
void Func_02007614();
void Func_02007616();
void Func_02007628();
void Func_02007636();
void Func_0200763a();
void Func_0200763c();
void Func_0200763e();
void Func_02007640();
void Func_02007642();
void Func_0200764c_a();
void Func_0200764c_b();
void Func_02007650();
void Func_02007656();
void Func_02007662_a();
void Func_02007662_b();
void Func_02007662_c();
void Func_02007664();
void Func_0200766a();
void Func_02007678();
void Func_0200767a();
void Func_0200767c();
void Func_02007680();
void Func_0200768a();
void Func_02007690();
void Func_02007694();
void Func_020076ac();
void Func_020076cc();
void Func_020076d6();
void Func_020076da();
void Func_020076e0();
void Func_020076f2();
void Func_02007778();
void Func_02007782();
void Func_020076a0();
void Func_020076f8();
void Func_0200771e();
void Func_0200772c();
void Func_0200773a();
void Func_0200774a();
void Func_02007752_a();
void Func_0200775a();
void Func_0200778c();
void Func_02007834();
void Func_020077f8();
void Func_0200783a();
void Func_02007862();
void Func_02007872_a();
void Func_02007872_b();
void Func_0200788a();
void Func_0200788c();
void Func_020078da();
void Func_020078e4();
void Func_02007904();
void Func_0200790c();
void Func_02007918();
void Func_0200791c_a();
void Func_02007920();
void Func_02007938();
void Func_0200794a();
void Func_02007990_a();
void Func_020079a8();
void Func_020079bc();
s32 Func_02007b82();
s32 Func_02007b96();
s32 Func_02007baa();
void Func_02007bd4();
void Func_02007be0();
void Func_02007bea();
void Func_02007bf4();
void Func_02007c00();
void Func_02007c08_a();
void Func_02007c08_b();
void Func_02007c12();
void Func_02007c1e();
void Func_02007c34();
void Func_02007c52();
void Func_02007c62();
void Func_02007c68();
void Func_02007c7a();
void Func_02007c94();
void Func_02007caa();
void Func_02007cb4();
void Func_02007bfa();
void Func_02007c0e();
void Func_02007c3c();
void Func_02007c3e();
void Func_02007c4c();
void Func_02007cb6();
void Func_02007d70();
void Func_02007d7c();
void Func_02007d88();
void Func_02007d94();
s32 Func_02007da0();
void Func_02007db8();
void Func_02007dc6_a();
void Func_02007dd2();
void Func_02007cd6();
void Func_02007d22();
void Func_02007d30();
void Func_02007d34();
void Func_02007d64();
void Func_02007d70_a();
void Func_02007d96();
void Func_02007d9c();
void Func_02007dae();
void Func_02007db4();
void Func_02007de8();
void Func_02007dec();
void Func_02007e20();
void Func_02007d7e();
void Func_02007d98();
void Func_02007db2_a();
void Func_02007db2_b();
void Func_02007dcc();
void Func_02007de6();
void Func_02007e04();
void Func_02007e16();
void Func_02007e28();
void Func_02007e80();
void Func_02007e92();
void Func_02007ea4();
void Func_02007eb0();
void Func_02003d0c();
void Func_02008646();
void Func_02008674_a();
void Func_02008674_b();
void Func_0200867a();
void Func_020086b0();
u8 *Func_020086d6();
void Func_0200873a();
void Func_02008754();
void Func_0200876e();
void Func_02008788();
void Func_0200879a();
void Func_020087a2();
void Func_020087ae();
void Func_020087bc();
void Func_020087c8();
void Func_020087d6();
void Func_020087e2();
void Func_020087e6();
void Func_020087ee();
void Func_020087f4();
void Func_020087f6();
void Func_020087fc();
void Func_02008810();
void Func_02008816();
void Func_02008830();
void Func_02008832();
void Func_0200884c();
void Func_0200884e();
void Func_02008860();
void Func_02008862();
void Func_02008868();
void Func_0200887a_a();
void Func_0200887a_b();
void Func_02008892();
void Func_02008896();
void Func_02008898();
void Func_020088a4();
void Func_020088b6();
void Func_020088bc_a();
void Func_020088bc_b();
void Func_020088c0();
void Func_020088c6_a();
void Func_020088c6_b();
void Func_020088ca();
void Func_020088cc();
void Func_020088d0();
void Func_020088d4_a();
void Func_020088d4_b();
void Func_020088dc();
void Func_020088fc();
void Func_02008900();
void Func_02008912();
void Func_0200891a();
void Func_02008928();
void Func_0200892a();
void Func_0200892e();
void Func_02008932();
void Func_02008938();
void Func_0200893c();
void Func_0200893e();
void Func_02008946_a();
void Func_02008946_b();
void Func_02008946_c();
void Func_02008954();
void Func_0200896c();
void Func_02008984();
s32 Func_0200898a();
void Func_0200898e();
void Func_02008996();
void Func_0200899a();
void Func_020089ae();
void Func_020089b2();
void Func_020089bc();
void Func_020089c2();
void Func_020089ca();
void Func_020089da();
void Func_020089de();
void Func_020089e2();
void Func_020089ec();
void Func_020089f6();
void Func_020089fa_a();
void Func_020089fa_b();
void Func_020089fc();
void Func_020089fe();
void Func_02008a00();
void Func_02008a06();
void Func_02008a0c();
void Func_02008a18();
void Func_02008a1a_a();
void Func_02008a1a_b();
s32 Func_02008a2a();
void Func_02008a38();
void Func_02008a3a();
void Func_02008a70();
void Func_02008a76();
void Func_02008a7a();
void Func_02008a84();
void Func_02008a88();
void Func_02008a8e_a();
void Func_02008a8e_b();
void Func_02008a9c();
void Func_02008a9e();
void Func_02008aae();
void Func_02008acc();
void Func_02008aea();
void Func_02008aec_a();
void Func_02008aec_b();
void Func_02008af2();
void Func_02008af6();
void Func_02008af8();
void Func_02008afa();
void Func_02008b00();
void Func_02008b02();
void Func_02008b04();
void Func_02008b06();
void Func_02008b0a();
void Func_02008b14();
void Func_02008b22();
void Func_02008b3a();
void Func_02008b44();
void Func_02008b4e();
void Func_02008b56();
void Func_02008b58();
void Func_02008b62();
void Func_02008b68();
void Func_02008b74();
void Func_02008b7a();
void Func_02008b86();
void Func_02008b98_a();
void Func_02008b98_b();
void Func_02008bb4();
void Func_02008bb6();
void Func_02008bb8();
void Func_02008bc6();
void Func_02008bd2();
void Func_02008bde();
void Func_02008be4_a();
void Func_02008be4_b();
void Func_02008c02();
void Func_02008c14();
void Func_02008c26();
void Func_02008c30();
void Func_02008c3c();
void Func_02008c48_a();
void Func_02008c48_b();
void Func_02008c50();
void Func_02008c52();
void Func_02008c58_a();
void Func_02008c58_b();
void Func_02008c60();
void Func_02008c62();
void Func_02008c68();
void Func_02008c6a();
void Func_02008c6c();
void Func_02008c7e_a();
void Func_02008c7e_b();
void Func_02008c80();
void Func_02008c84();
void Func_02008c88();
void Func_02008c96();
void Func_02008cac();
void Func_02008cba();
void Func_02008ccc();
void Func_02008cd8();
void Func_02008ce0();
void Func_02008ce6();
void Func_02008d1a();
void Func_02008d24();
void Func_02008d2e();
void Func_02008d38();
void Func_02008d42();
void Func_02008d4a();
void Func_02008d52();
void Func_02008d5a();
void Func_02008d64();
void Func_02008d6c();
void Func_02008d70();
void Func_02008d7c();
void Func_02008d88();
void Func_02008d8a();
void Func_02008d94_a();
void Func_02008d94_b();
void Func_02008da2();
void Func_02008da6();
void Func_02008db2();
void Func_02008db8();
void Func_02008dc4();
void Func_02008dd6();
void Func_02008dde_a();
void Func_02008dde_b();
void Func_02008de4();
void Func_02008dee();
void Func_02008df0();
void Func_02008dfc_a();
void Func_02008dfc_b();
void Func_02008dfe();
void Func_02008e02();
void Func_02008e0e();
void Func_02008e10();
void Func_02008e1a();
void Func_02008e2c();
void Func_02008e32();
void Func_02008e34();
void Func_02008e3c();
void Func_02008e4c();
void Func_02008e5a();
void Func_02008e7c();
void Func_02008e82();
void Func_02008e90();
void Func_02008e98();
void Func_02008e9e();
void Func_02008ea2();
void Func_02008eac();
void Func_02008eb8();
void Func_02008ec4();
void Func_02008ede();
void Func_02008ee6();
void Func_02008ef6();
void Func_02008ef8();
void Func_02008f02_a();
void Func_02008f02_b();
void Func_02008f0a();
void Func_02008f14();
void Func_02008f20();
void Func_02008f26();
void Func_02008f30();
void Func_02008f36_a();
void Func_02008f36_b();
void Func_02008f38();
void Func_02008f50();
void Func_02008f54();
void Func_02008f56();
void Func_02008f64();
void Func_02008f6e();
void Func_02008f76();
void Func_02008f78();
void Func_02008f90();
void Func_02008f96();
void Func_02008fa2();
void Func_02008fb4();
void Func_02008fc2();
void Func_02008fc6();
void Func_02008fcc();
void Func_02008fd6();
void Func_02008fd8();
void Func_02008fe2();
void Func_02008fe8();
void Func_02008ff8();
void Func_02008ffc();
void Func_02009006_a();
void Func_02009006_b();
void Func_02009010();
void Func_02009014();
void Func_0200901a();
void Func_0200901e_a();
void Func_0200901e_b();
void Func_02009032();
void Func_02009036_a();
void Func_02009036_b();
void Func_0200903c();
void Func_02009050();
void Func_02009052();
void Func_02009054();
void Func_02009058();
void Func_02009060();
void Func_02009068();
void Func_02009070();
void Func_02009078();
void Func_02009088();
void Func_0200908e();
void Func_0200909e();
void Func_020090b6();
void Func_020090c4();
void Func_020090d6();
void Func_020090dc();
void Func_020090f0();
void Func_020090f4_a();
void Func_020090f4_b();
void Func_020090fa();
void Func_020090fe();
void Func_02009112();
void Func_0200911a();
void Func_02009126();
void Func_02009132();
void Func_02009140();
void Func_02009146();
void Func_02009152();
void Func_0200915a();
void Func_0200915e();
void Func_02009160();
void Func_02009164_a();
void Func_02009164_b();
void Func_0200916e();
void Func_02009178_a();
void Func_02009178_b();
void Func_0200917a();
void Func_0200917c();
void Func_02009182();
void Func_0200918a();
void Func_02009190();
void Func_0200919a_a();
void Func_0200919a_b();
void Func_020091b2();
void Func_020091c6();
void Func_020091c8();
void Func_020091d2();
void Func_020091d6();
void Func_020091de();
void Func_020091f2();
void Func_020091f4_a();
void Func_020091f4_b();
void Func_020091fa();
void Func_02009208();
void Func_02009212();
void Func_0200921c();
void Func_02009222();
void Func_0200922c();
void Func_02009230();
void Func_0200923a();
void Func_0200923c();
void Func_0200924c();
void Func_02009256();
void Func_02009258();
void Func_02009260();
void Func_0200926a();
void Func_02009286();
void Func_0200929e();
s32 Func_020092ac();
void Func_020092b4();
void Func_020092b6();
void Func_020092bc();
void Func_020092d2();
void Func_020092d4();
void Func_020092d8();
void Func_020092ee();
void Func_020092f0();
void Func_020092f2();
void Func_020092f8();
void Func_020092fc();
void Func_02009300();
void Func_02009306();
s32 Func_02009308();
void Func_02009310();
void Func_02009320();
void Func_0200935e();
void Func_02009376_a();
void Func_02009376_b();
void Func_0200937c();
void Func_02009380();
void Func_0200938a();
void Func_02009394_a();
void Func_02009394_b();
void Func_0200939c();
void Func_0200939e();
void Func_020093a4();
void Func_020093a8();
void Func_020093b0();
void Func_020093b2();
void Func_020093ba();
void Func_020093ea();
void Func_020093ee_a();
void Func_020093ee_b();
void Func_020093fc();
void Func_02009406();
void Func_02009416();
void Func_0200941e();
void Func_02009422();
void Func_02009426();
void Func_02009430();
void Func_02009434();
void Func_0200943e();
void Func_02009448();
void Func_0200944a();
void Func_0200945a();
void Func_02009464();
void Func_0200946a_a();
void Func_0200946a_b();
void Func_0200947a();
void Func_02009482();
void Func_02009486();
void Func_02009488();
void Func_020094a0();
void Func_020094a6_a();
void Func_020094a6_b();
void Func_020094aa();
void Func_020094b4();
void Func_020094b6();
void Func_020094c2();
void Func_020094cc();
void Func_020094d6();
void Func_020094d8();
void Func_020094da();
void Func_020094de();
void Func_020094e0();
void Func_020094ec();
void Func_020094f0();
void Func_02009504();
void Func_02009516();
void Func_02009518();
void Func_0200951c();
void Func_02009520();
void Func_02009524();
void Func_02009526();
void Func_0200953e_a();
void Func_0200953e_b();
void Func_02009540();
void Func_02009550();
void Func_02009558_a();
void Func_02009558_b();
void Func_02009568();
void Func_02009570();
void Func_02009580();
void Func_02009582();
void Func_02009590_a();
void Func_02009590_b();
void Func_02009598();
void Func_020095a6_a();
void Func_020095a6_b();
void Func_020095b0();
void Func_020095b4();
void Func_020095bc();
void Func_020095c0();
void Func_020095c8_a();
void Func_020095c8_b();
void Func_020095e0();
void Func_020095ec();
void Func_020095fc();
void Func_02009604();
void Func_0200960a();
void Func_02009610_a();
void Func_02009610_b();
void Func_02009620();
void Func_02009622();
void Func_02009628();
void Func_0200962a();
void Func_0200962c();
void Func_02009632();
void Func_02009638();
void Func_0200963a();
void Func_02009644();
void Func_0200964a();
void Func_0200964e();
void Func_02009654();
void Func_0200968c();
void Func_02009694();
void Func_020096aa();
void Func_020096ac();
void Func_020096ae();
void Func_020096b0();
void Func_020096ba();
void Func_020096bc();
void Func_020096c2();
void Func_020096c6();
void Func_020096ca();
void Func_020096d2();
void Func_020096d6();
void Func_020096dc();
void Func_020096e6();
void Func_020096f0();
void Func_02009708();
void Func_02009716_a();
void Func_02009716_b();
void Func_0200972e();
void Func_0200973c();
void Func_02009754();
void Func_02009758();
void Func_02009762();
void Func_0200976c_a();
void Func_0200976c_b();
void Func_0200976c_c();
void Func_02009772();
void Func_02009776();
void Func_0200977e();
void Func_0200978a();
void Func_0200978c();
void Func_02009790();
void Func_02009792();
void Func_020097aa();
void Func_020097c8();
void Func_020097d4();
void Func_020097d6();
void Func_020097d8();
void Func_020097de_a();
void Func_020097de_b();
void Func_020097e0();
void Func_020097e8_a();
void Func_020097e8_b();
void Func_020097f0();
void Func_020097f2();
void Func_020097f4();
void Func_020097f8();
void Func_02009804();
void Func_02009806();
void Func_02009808();
void Func_02009820();
void Func_02009830();
void Func_02009834();
void Func_02009854();
void Func_02009868();
void Func_0200986a();
void Func_0200986c();
void Func_0200986e();
void Func_02009870_a();
void Func_02009870_b();
void Func_02009888();
void Func_0200988c();
void Func_0200988e();
void Func_02009898();
void Func_0200989e();
void Func_020098ae();
void Func_020098b6_a();
void Func_020098b6_b();
void Func_020098bc_a();
void Func_020098bc_b();
void Func_020098ce();
void Func_020098d4_a();
void Func_020098d4_b();
void Func_020098ec();
void Func_020098f8();
void Func_020098fc();
void Func_02009914();
void Func_02009916();
void Func_02009920();
void Func_0200992e();
void Func_02009938();
void Func_0200993e_a();
void Func_0200993e_b();
void Func_0200994a();
void Func_0200994c();
void Func_02009950_a();
void Func_02009950_b();
void Func_02009956();
void Func_0200995a();
void Func_0200996e();
void Func_02009984();
void Func_0200998c();
s32 Func_02009992();
void Func_0200999a();
void Func_0200999c();
void Func_020099a0();
void Func_020099aa();
void Func_020099b4();
void Func_020099b8();
void Func_020099c0();
void Func_020099c8();
void Func_020099ca();
void Func_020099d4();
void Func_020099d8();
void Func_020099dc();
void Func_020099ee();
void Func_020099f4_a();
void Func_020099f4_b();
u8 *Func_02009a00();
void Func_02009a04();
void Func_02009a0c();
void Func_02009a14();
void Func_02009a1c();
void Func_02009a22();
void Func_02009a24();
u8 *Func_02009a30();
s32 Func_02009a32();
void Func_02009a3a();
void Func_02009a3c();
void Func_02009a3e();
void Func_02009a44();
void Func_02009a52();
void Func_02009a56();
void Func_02009a64();
void Func_02009a6a();
void Func_02009a72();
void Func_02009a82();
void Func_02009a90();
void Func_02009a9a_a();
void Func_02009a9a_b();
void Func_02009aa2();
void Func_02009aa8();
void Func_02009ab2();
void Func_02009ab8();
void Func_02009ac0();
void Func_02009ace();
void Func_02009ad2();
void Func_02009aec();
void Func_02009aee();
void Func_02009b04();
void Func_02009b22();
void Func_02009b24();
void Func_02009b2c();
void Func_02009b3a();
void Func_02009b42();
void Func_02009b5a();
void Func_02009b68();
void Func_02009b70_a();
void Func_02009b70_b();
void Func_02009b76();
void Func_02009b7a();
void Func_02009b80();
void Func_02009b88();
void Func_02009b96();
void Func_02009b98_a();
void Func_02009b98_b();
void Func_02009b98_c();
void Func_02009bb0();
void Func_02009bb6();
void Func_02009bc6();
void Func_02009bf4();
void Func_02009bf8();
void Func_02009bfa();
void Func_02009c02();
void Func_02009c06();
void Func_02009c08();
void Func_02009c10();
void Func_02009c12();
void Func_02009c14();
void Func_02009c16();
void Func_02009c18();
void Func_02009c1c();
void Func_02009c24();
void Func_02009c2c();
void Func_02009c36();
void Func_02009c40();
void Func_02009c46();
void Func_02009c4a();
void Func_02009c4c_a();
void Func_02009c4c_b();
void Func_02009c54();
void Func_02009c5e();
void Func_02009c66();
void Func_02009c68();
void Func_02009c72();
void Func_02009d7e();
void Func_02009d82();
void Func_02009d9c();
u8 *Func_02009a60_a();
u8 *Func_02009a60_b();
u8 *Func_02009a60_c();
s32 Func_02009dae();
void Func_02009dc8();
s32 Func_02009dcc();
void Func_02009dec();
void Func_02009e0a();
void Func_02009e3e();
void Func_02009e4c_a();
void Func_02009e82();
void Func_02009e90();
void Func_02009ea8();
void Func_02009eae();
void Func_02009eba();
s32 Func_02009ebe();
s32 Func_02009ece();
void Func_02009ed4();
void Func_02009ed6();
void Func_02009eda();
void Func_02009ef2();
void Func_02009ef8();
void Func_02009f14();
void Func_02009f1c();
void Func_02009f34_a();
void Func_02009f34_b();
void Func_02009f38();
void Func_02009f40();
void Func_02009f46();
void Func_02009f4c();
void Func_02009f50();
void Func_02009f54();
void Func_02009f5e_a();
void Func_02009f5e_b();
void Func_02009f66();
s32 Func_02009f6e();
void Func_02009f78();
void Func_02009f7e();
void Func_02009f88_a();
void Func_02009f88_b();
void Func_02009f98_a();
void Func_02009f98_b();
void Func_02009f98_c();
void Func_02009fa8();
s32 Func_02009fb8();
void Func_02009fbe();
s32 Func_02009fce();
void Func_02009fd0();
void Func_02009fd6();
void Func_02009fe6();
s32 Func_02009fb6();
s32 Func_02009fc2();
s32 Func_02009fd0_a();
void Func_0200a00c();
void Func_0200a036();
void Func_0200a046();
void Func_0200a070();
void Func_0200a08a();
void Func_0200a090();
void Func_0200a098();
void Func_0200a09e();
void Func_0200a0a6();
void Func_0200a0b0();
void Func_0200a0b4();
void Func_0200a0b6();
void Func_0200a0cc();
void Func_0200a0ce();
void Func_0200a0d8();
void Func_0200a0e2();
void Func_0200a0fe();
void Func_0200a10c();
void Func_0200a112();
void Func_0200a118();
void Func_0200a122();
void Func_0200a124();
void Func_0200a12a();
void Func_0200a12c();
void Func_0200a132();
void Func_0200a13a_a();
void Func_0200a13a_b();
void Func_0200a142();
void Func_0200a154();
void Func_0200a15a();
void Func_0200a15c();
void Func_0200a162();
void Func_0200a16e();
void Func_0200a17c();
void Func_0200a18c_a();
void Func_0200a18c_b();
void Func_0200a1a4();
void Func_0200a1a6();
void Func_0200a1b4();
void Func_0200a1c2();
void Func_0200a1d6();
void Func_0200a1da();
void Func_0200a20a();
void Func_0200a21a();
void Func_0200a222();
void Func_0200a232();
void Func_0200a812();
s32 Func_0200a94c();
void Func_02005fbc();
void Func_0200a962();
void Func_0200a96a();
s32 Func_0200a968();
void Func_0200aa06();
void Func_0200a98c();
s32 Func_0200a982();
void Func_0200aa20();
void Func_02007658();
s32 Func_0200a99a();
void Func_0200aa38();
void Func_0200a9c0();
s32 Func_0200a9b6();
void Func_0200761a();
s32 Func_0200a9c4();
void Func_0200765c();
void Func_0200a9f0();
struct ObjectRuntime *Func_0200aa16();
struct ObjectRuntime *Func_0200aa2a();
struct ObjectRuntime *Village_GetActor();
struct ObjectRuntime *Func_0200aa52();
void Func_0200a9f6();
struct ObjectRuntime *Func_0200aa6c();
struct ObjectRuntime *Func_0200aa84();
void Func_0200aa22();
void Func_0200aa76();
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
void Func_0200a7d0();
void Func_0200a7d8();
void Func_0200a7e0();
s32 Func_0200a716();
s32 Func_0200a72a();
s32 Func_0200a73e();
s32 Func_0200a752();
void Func_0200a7b4();
void Func_0200a7c8();
void Func_0200a7dc();
void Func_0200a7f0();
void Func_0200a7f8();
void Func_0200a816();
struct ObjectRuntime *Func_0200a874();
void Func_0200a812_setup();
s32 Func_0200a842();
void Func_0200a8e4();
void Func_0200a854();
void Func_0200a7f6();
void Func_0200a7f4();
void Func_0200a828();
void Func_0200a7fe();
s32 Func_0200a89c();
void Func_02007300();
void Func_02007314();
void Func_0200a844();
void Func_0200a8c4();
struct ObjectRuntime *Func_0200a922();
struct ObjectRuntime *Func_0200a92a();
void Func_0200a8c8();
void Func_0200a9f8();
void Func_0200a3d6();
s32 Func_0200a572();
s32 Func_0200a57c();
s32 Func_0200a586();
s32 Func_0200a5b2();
s32 Func_0200a5bc();
void Func_0200a5ba();
void Func_0200a5ee();
void Func_0200a5c4();
void Func_0200a620();
void Func_0200a650();
s32 Func_0200a602();
void Func_0200a682();
void Func_0200a772();
void Func_0200a77a();
void Func_0200a782();
void Func_0200a78a();
void Func_0200a792();
void Func_0200a640();

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

/* Phase/status word at 0x1c0 of the shared scene work record. */

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

    Func_02006326(actor, x, y);
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
    Func_02007388(handle, 258, 60);
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

#if defined(GS1_EDITION_JA)
#define Value_00002421 Value_000025aa
#elif defined(GS1_EDITION_DE) || defined(GS1_EDITION_ES) || defined(GS1_EDITION_FR) || defined(GS1_EDITION_IT)
#define Value_00002421 Value_00002403
#endif

extern u8 Value_00002421;

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
    extern u8 *Data_03001ebc;

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

void AdvanceStagedActorPair(void)
{
    extern s32 Data_0200dfa4;

    s32 destination[3];
    struct StagedActor *lead_actor;
    struct StagedActor *next_actor;
    struct StagedActor *blocking_actor;
    s32 facing_index;
    u32 step;
    s32 move_rate;
    s32 zero;

    lead_actor = GetStagedActor(0);
    facing_index = lead_actor->direction_and_kind >> 12;
    step = StagedActorStepTable[facing_index];
    destination[0] = lead_actor->x.value + (step & 0xffff0000);
    destination[1] = lead_actor->y;
    step <<= 16;
    destination[2] = lead_actor->z.value + step;
    next_actor = FindNextStagedActor(destination, lead_actor);
    if (next_actor == 0) return;

    step = StagedActorStepTable[facing_index];
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
    step = StagedActorStepTable[facing_index];
    destination[0] = next_actor->x.value + (step & 0xffff0000);
    destination[1] = next_actor->y;
    step <<= 16;
    destination[2] = next_actor->z.value + step;
    if (CanStartStagedActorMove(next_actor, destination) > 0) return;

    zero = next_actor->transition_busy;
    if (zero != 0) return;

    SetStagedActorMode(lead_actor, 8);
    move_rate = 0x3333;
    SelectStagedActorSlot(15);
    StartStagedActorEffect(185);
    next_actor->move_rate_x = move_rate;
    next_actor->move_rate_z = move_rate;
    StartNextStagedActorMove(next_actor, destination[0], destination[1], destination[2]);
    lead_actor->move_rate_x = move_rate;
    lead_actor->move_rate_z = move_rate;
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
    extern u8 *Data_03001e70;

    u8 *g = Data_03001e70;
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

s32 StopBlockedActorMotion(MovingActor *actor)
{
    extern s16 Data_02000240[];
    extern u8 *Data_03001ebc;

    s32 destination[3];
    u32 direction_index = actor->kind >> 12;
    s32 step = Data_0200ded8[direction_index];
    BlockingActor *blocking_actor;

    destination[0] = actor->x + (step & 0xffff0000);
    destination[1] = actor->y;
    step <<= 16;
    destination[2] = actor->z + step;
    blocking_actor = Func_02000342(destination, actor);
    if (blocking_actor != 0) {
        u32 kind_index = 0;
        s32 blocking_kind = ((s16 *)*(s32 *)((u8 *)blocking_actor->info + 0x28))[kind_index];

        {
            s32 *p = Data_0200df18;

            do {
                if (blocking_kind == *p++) {
                    goto done;
                }
                kind_index++;
            } while (kind_index <= 5);
        }
        actor->vx = 0;
        actor->vy = 0;
        actor->a = 0x80000000;
        actor->b = 0x80000000;
    }
    step = Data_0200ded8[direction_index];
    destination[0] = actor->x + (step & 0xffff0000);
    destination[1] = actor->y;
    step <<= 16;
    destination[2] = actor->z + step;
    if (Func_02005902(actor, destination) > 0) {
        actor->vx = 0;
        actor->vy = 0;
        actor->a = 0x80000000;
        actor->b = 0x80000000;
    }
done:
    return 0;
}

s32 FindClearActorPosition(Req *request)
{
    extern s32 Data_0200dfa4;

    extern s32 Data_0200ded8[];

    s32 out;
    s32 vec[3];
    Obj *r;
    s32 n;
    s32 nx, ny;
    u8 *flag;
    s32 i, j;
    s32 w1, w2;
    s32 off;

    request->f14 = 0;
    r = Func_020007de(&out, &request->f4, request);
    if (r == 0) {
        return 0;
    }
    flag = (u8 *)r + 0x22;
    *flag = 2;
    {
        s32 k = request->kind;

        n = 0;
        off = k * 4;
        w1 = Data_0200df30[off + 1];
        if (w1 < 0) {
            w1 = -w1;
        }
        w2 = Data_0200df30[off + 3];
        if (w2 < 0) {
            w2 = -w2;
        }
        ny = (w1 + w2) >> 4;
        w1 = Data_0200df30[off];
        if (w1 < 0) {
            w1 = -w1;
        }
        w2 = Data_0200df30[off + 2];
        if (w2 < 0) {
            w2 = -w2;
        }
        nx = (w1 + w2) >> 4;
    }
    vec[0] = r->x + (Data_0200ded8[out] & 0xffff0000);
    {
        s32 ry = r->y;

        vec[1] = ry;
        vec[2] = r->z + (Data_0200ded8[out] << 16);
        request->y = ry;
    }
    for (;;) {
        request->z = vec[2] + (Data_0200df30[request->kind * 4 + 1] << 16);
        for (j = 0; j < ny; j++) {
            request->x = vec[0] + (Data_0200df30[request->kind * 4] << 16);
            for (i = 0; i < nx; i++) {
                if (Func_02005b3c(r, &request->x) == 2) {
                    goto found;
                }
                request->x += 0x100000;
            }
            request->z += 0x100000;
        }
        n++;
        vec[0] += Data_0200ded8[out] & 0xffff0000;
        vec[2] += Data_0200ded8[out] << 16;
    }
found:
    *flag = 0;
    if (n == 0) {
        return 0;
    }
    request->x = r->x + (Data_0200ded8[out] & 0xffff0000) * n;
    request->y = r->y;
    request->z = r->z + (Data_0200ded8[out] << 16) * n;
    return 1;
}

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

    Func_0200613a(15, 20, 1, 1, actor_slot, 22);
    Func_0200614c(17, 23, 1, 3, actor_slot, 23);
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

    Call3(Func_02006212, 12, 0x10000, 0x8000);
    Call3(Func_0200623e, 12, 248, 0x178);
    Func_02006264(12);
    Func_0200637a(215);
    Func_02006200(60);
    Func_02001728();
    Call1(Func_020061fa, 0x943);
}

void FieldScene_UpdateActorTwelveTransition(void)
{
    extern s16 Data_02000240[];
    extern u32 Data_0200dfa4;
    extern u8 Data_03001e70[];
    extern u8 Data_03001ebc[];

    u8 *rec7;
    s32 record;

    record = Value1(Func_02006244, 12);
    if ((*(volatile s32 *)(record + 16) >> 20) > 22) {
        Call3(Func_020061fc, 0x40000, 0x40000, 0x10000);
        Call3(Func_0200620a, -1, -1, 0xe666);
        Func_020063c8(144);
        Call6(Func_0200620c, 15, 20, 1, 1, 15, 22);
        Call6(Func_0200621e, 17, 23, 1, 3, 15, 23);
        rec7 = Value1(Func_0200629c, 12);
        if ((s32)rec7 != 0) {
            Func_02006240((s32)rec7, 0);
            rec7[35] = 2;
        }
        Call1(Func_0200627e, 0x943);
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

    Func_0200627c(0, 0x4d, 1, 3, position_x, position_z);
    Func_0200628e(1, 0x4d, 1, 1, position_x + 1, position_z);
    Func_020062a2(position_x, position_z - 0x30, 1, 1, position_x, position_z - 0x2e);
}

void FieldScene_UpdateObjectPairA(void)
{
    extern s16 Data_02000240[];
    extern u32 Data_0200dfa4;
    extern u8 Data_03001e70[];
    extern u8 Data_03001ebc[];

    s32 v6;
    s32 raw;
    s32 d;
    u8 *p5;

    p5 = *(volatile s32 *)Data_03001ebc;
    v6 = 1;
    if (Func_020062f2(234) != -v6) {
        raw = *(s16 *)(((s32)p5 + 0x16c));
        d = raw - 40;
        if (Value1(Func_02006316, 0x941) != 0) {
            if (d == 4) {
                goto done;
            }
        }
        Func_020019a4(d);
        Func_020064c2(157);
        Call3(Func_0200631a, 0x30000, 0x30000, 0x10000);
        Call3(Func_02006324, -v6, -v6, 0xe666);
        Func_02006356(raw + 0x328);
    }
done:;
}

void FieldScene_UpdateObjectPairB(void)
{
    extern s16 Data_02000240[];
    extern u32 Data_0200dfa4;
    extern u8 Data_03001e70[];
    extern u8 Data_03001ebc[];

    u8 *p5;
    s16 field;

    p5 = *(volatile s32 *)Data_03001ebc;
    if (Func_020063de(234) != -1) {
        field = *(s16 *)((s32)p5 + 0x16c);
        Func_02001b44(field - 40);
        Func_0200659e(157);
        Call3(Func_020063f6, 0x30000, 0x30000, 0x10000);
        Call3(Func_02006400, -1, -1, 0xe666);
        Func_02006432(field + 0x32d);
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

    Func_0200641c(0x37, 0x79, 1, 3, position_x, position_z);
    Func_0200642e(0x38, 0x79, 1, 1, position_x + 1, position_z);
    Func_02006442(position_x, position_z - 0x3f, 1, 1, position_x, position_z - 0x3e);
}

void FieldScene_UpdateTableBObjectPair(void)
{
    extern s16 Data_02000240[];
    extern u32 Data_0200dfa4;
    extern u8 Data_03001e70[];
    extern u8 Data_03001ebc[];

    u8 *work;
    s16 field;

    work = *(volatile s32 *)Data_03001ebc;
    if (Func_02006492(234) != -1) {
        field = *(s16 *)((s32)work + 0x16c);
        Func_02001cd4(field - 40);
        Func_02006652(157);
        Call3(Func_020064aa, 0x30000, 0x30000, 0x10000);
        Call3(Func_020064b4, -1, -1, 0xe666);
        Func_020064e6(field + 0x330);
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

    Func_020064cc(1, 0x50, 1, 3, position_x, position_z);
    Func_020064de(2, 0x50, 1, 1, position_x + 1, position_z);
    Func_020064fa(position_x, position_z - 0x3f, 1, 1, position_x, position_z - 0x3e);
}

void FieldScene_UpdateObjectPairC(void)
{
    extern s16 Data_02000240[];
    extern u32 Data_0200dfa4;
    extern u8 Data_03001e70[];
    extern u8 Data_03001ebc[];

    u8 *p5;
    s16 field;

    p5 = *(volatile s32 *)Data_03001ebc;
    if (Func_02006542(234) != -1) {
        field = *(s16 *)((s32)p5 + 0x16c);
        Func_02001e34(field - 40);
        Func_02006702(157);
        Call3(Func_0200655a, 0x30000, 0x30000, 0x10000);
        Call3(Func_02006564, -1, -1, 0xe666);
        Func_02006596(field + 0x332);
    }
}

void ShowLockedDoorMessageOnce(void)
{
    extern s16 Data_02000240[];
    extern u8 *Data_03001ebc;

    if (Func_020065a2(0xea) == -1) {
        Func_020065aa((s32)&Value_00000953, 1);
    }
}

void ShowLockedDoorMessage(void)
{
    extern s16 Data_02000240[];
    extern u8 *Data_03001ebc;

    Func_020065bc((s32)&Value_00000953, 1);
}

void ShowGeraldDontMessage(void)
{
    extern s16 Data_02000240[];
    extern u8 *Data_03001ebc;

    if (Func_02002008(8, 8) != 0) {
        Func_020065f2((s32)&Value_00000f2a);
    }
}

void ShowThatWasExcitingMessage(void)
{
    extern s16 Data_02000240[];
    extern u8 *Data_03001ebc;

    if (Func_02002024(9, 7) != 0) {
        Func_0200660e((s32)&Value_00000f2b);
    }
}

void ShowSorryIScaredYouMessage(void)
{
    extern s16 Data_02000240[];
    extern u8 *Data_03001ebc;

    if (Func_02002040(10, 6) != 0) {
        Func_0200662a((s32)&Value_00000f2c);
    }
}

void ShowWellMessage(void)
{
    extern s16 Data_02000240[];
    extern u8 *Data_03001ebc;

    if (Func_0200205c(11, 5) != 0) {
        Func_02006646((s32)&Value_00000f2d);
    }
}

s32 TryStartActorInteraction(s32 actor_id, s32 interaction_id)
{
    extern s16 Data_02000240[];
    extern u8 *Data_03001ebc;

    s32 started = 0;
    s32 interaction;

    Func_02006678();
    interaction = Func_020067d2(0, actor_id, interaction_id);
    if (Func_0200669c(interaction_id, 0) != -1) {
        Func_02006714(actor_id, 2);
        started = 1;
    } else {
        Func_02006820(0x7d);
        Func_02006728(actor_id, 5);
    }
    Func_02006636(interaction);
    Func_020066c2();
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

void StartDoraHouseAccidentScene(void)
{
    extern s32 Data_0200dfa4;

    Func_02006736(0, 0x108, 0x318);
    Func_02006754(0);
    Func_020067d0(0, 0x4000, 0);
    Func_020066f6(10);
    Func_0200677e(0, 1);
    Func_02006866(0xea, 3);
    Func_0200678e(0, 1);
    Func_0200672e(0xea, 0);
    Func_0200670c((s32)&Value_00000f2e);
    Func_0200679e(8, 0, 0);
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
                    Func_020068a0(0x215);
                    Func_020068a0(0x214);
                }
                if (Func_02002b72(10)) {
                    Func_020068a0(0x215);
                    Func_020068a0(0x214);
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

    Func_02006914();
    Func_0200695a(9);
    Func_02006984(9, 0, 0);
    Func_020069a4(9, 0);
    Func_020069d6(9, 0, 0);
    Func_02006a2a(9, 256, 0);
    Func_02002e3c(10);
    Func_02006954();
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
                    Func_02006a96(0x215);
                    Func_02006a96(0x214);
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
    Func_02006afe();
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
                    Func_02006c28(0x215);
                    Func_02006c28(0x214);
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
    Func_02006c92();
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
    extern u8 *Data_03001ebc;

    u8 *p5;

    p5 = *(volatile s32 *)&Data_03001ebc;
    if (Value1_0200169c(Func_02006cb8, 0x214) == 0) {
        if (Value0(Func_02002d10) == 0) {
            if (Data_02000240[294] == 0) {
                if (Value1_0200169c(Func_02002fae, 17) != 0) {
                    Call1_0200169c(Func_02006cec, 0x215);
                    Call1_0200169c(Func_02006cf4_a, 0x214);
                }
            }
            if (Value1_0200169c(Func_02006cf4_b, 0x214) != 0) {
                {
                    volatile u16 *target = (volatile u16 *)(((s32)p5 + 0x182));
                    s32 shown = 94;

                    *target = shown;
                }
            }
        }
    }
}

void RunActor17SceneStep(void)
{
    extern s16 Data_02000240[];
    extern u8 *Data_03001ebc;

    Func_02003232(17);
    Func_02006d4a();
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
    extern u8 *Data_03001ebc;

    u8 *p5;

    p5 = *(volatile s32 *)&Data_03001ebc;
    if (Value1_0200169c(Func_02006d76, 0x225) == 0) {
        if (Value1_0200169c(Func_02003054_scene_scripts, 13) != 0) {
            if (Data_02000240[294] == 0) {
                Call1_0200169c(Func_02006da2, 0x225);
                Call1_0200169c(Func_02006d28, 0x200975d);
                Call1_0200169c(Func_02006d2e, 0x20097bd);
                {
                    volatile u16 *target = (volatile u16 *)(((s32)p5 + 0x182));
                    s32 shown = 96;

                    *target = shown;
                }
            }
        }
    }
}

void FieldScene_RunScene3bfSequenceC(void)
{
    extern s16 Data_02000240[];
    extern u8 *Data_03001ebc;

    u8 *p5;

    p5 = *(volatile s32 *)&Data_03001ebc;
    if (Value1_0200169c(Func_02006dd6, 0x225) == 0) {
        if (Value1_0200169c(Func_020030b4, 21) != 0) {
            if (Data_02000240[294] == 0) {
                Call1_0200169c(Func_02006e02, 0x225);
                Call1_0200169c(Func_02006d88, 0x20097bd);
                Call1_0200169c(Func_02006d8e, 0x200975d);
                {
                    volatile u16 *target = (volatile u16 *)(((s32)p5 + 0x182));
                    s32 shown = 96;

                    *target = shown;
                }
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
    Func_020070e4();
    Func_0200714e_a(9, 0, 0);
    Func_02007126(9, 1);
    Func_0200713c_a(9);
    Func_0200717c(9, 0);
    Func_0200713c_b(0, 1);
    {
        u8 *t = &Value_0000240d;

        Func_020071cc((s32)t);
        Func_020071e4(9, 0);
        Func_02007212(0, 258, 60);
        Func_020071e8((s32)(t + 1));
    }
    Func_02007200(9, 0);
    Func_02007266(60);
    Func_0200728a();
    Func_0200714e_b();
}

void RunActorScriptedSequenceA(s32 actor_id)
{
    Func_0200715a();
    Func_0200715e();
    Func_02007252(actor_id, 256, 1);
    Func_020071d4(actor_id, 0, 0);
    Func_020071ac(actor_id, 1);
    Func_020071fc(actor_id, 0);
    Func_0200722e_a(actor_id, 0, 0);
    Func_0200720e(0, 1);
    Func_02007200_a(actor_id, 0, 0);
    Func_020071d8(actor_id, 1);
    Func_020071ee_a(actor_id);
    Func_0200722e_b(actor_id, 0);
    Func_020071ee_b(0, 1);
    {
        u8 *t = &Value_0000240d;

        Func_0200727e((s32)t);
        Func_02007296(actor_id, 0);
        Func_02007278(0, actor_id, 0);
        Func_020072ce(0, 258, 60);
        Func_020072a4((s32)(t + 1));
    }
    Func_020072bc(actor_id, 0);
    Func_02007340();
    Func_020071f6(60);
    Func_0200732c(60);
    Func_02007210();
}

void TurnActorToSceneDirection(s32 actor_id)
{
    extern s32 Data_0200dfa4;

    Func_020072be(actor_id, 0, 0);
    Func_020072c8(0, actor_id, 0);
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
    Func_02007344((s32)id);
    Func_0200735c(handle, 0);
    Call_02007388(handle);
    Func_0200735e((s32)(id + 1));
    Func_02007376_a(handle, 0);
    id += 2;
    Func_02007338(handle, 4);
    Func_02007376_b((s32)id);
    Func_0200738e(handle, 0);
}

void RunActorScriptedSequenceC(s32 actor_id)
{
    u8 *t = &Value_00002421;

    Func_02007396((s32)t);
    Func_020073ae(actor_id, 0);
    Func_02007386_a(actor_id, 1);
    Func_020073ac((s32)(t + 1));
    Func_020073c4_a(actor_id, 0);
    Func_02007386_b(actor_id, 4);
    Func_020073c4_b((s32)(t + 2));
    Func_020073dc(actor_id, 0);
}

void FieldScene_RunScene3bf_02001cf0(s32 a0)
{
    extern u8 Data_03001ebc[];

    u32 i;
    s32 record;
    s32 base6_2424;

    base6_2424 = (s32)Data_00002424;
    Func_020073e2(base6_2424);
    Func_020073fa(a0, 0);
    Func_02007330(120);
    Call3_02001cf0(Func_0200742a_a, a0, 0x101, 60);
    Func_02007400((base6_2424 + 1));
    Func_02007418(a0, 0);
    Func_020073f0_a(a0, 1);
    Func_02007416((base6_2424 + 2));
    Func_0200742e_a(a0, 0);
    Func_020073f0_b(a0, 4);
    Func_0200742e_b((base6_2424 + 3));
    Func_02007446(a0, 0);
}

void RunActorScriptedSequenceD(s32 actor_id)
{
    u8 *t = &Value_00002428;

    Func_02007452((s32)t);
    Func_0200746a(actor_id, 0);
    Func_0200742a(actor_id, 4);
    Func_02007468((s32)(t + 1));
    Func_02007480(actor_id, 0);
    Func_02007458_a(actor_id, 1);
    Func_0200747e((s32)(t + 2));
    Func_02007496_a(actor_id, 0);
    Func_02007458_b(actor_id, 3);
    Func_02007496_b((s32)(t + 3));
    Func_020074ae(actor_id, 0);
}

void InspectOrdinaryObject(void)
{
    extern u8 Data_02000240[];

    extern u8 *Data_03001ebc;

    Func_02007474(0, 1);
    Func_0200757a(113);
    Func_020074f6(15, 256, 60);
    Func_020039c8(15);
    *(s32 *)(Data_03001ebc + 448) = 512;
    Data_02000240[0x22b] = 3;
    Func_02007564(98, 2);
    Func_020074ae_a(15, 0, 0);
    Func_0200744a();
    Func_02007430(2380);
}

void InspectEmptyChest(void)
{
    extern u8 Data_02000240[];

    extern u8 *Data_03001ebc;

    Func_020074dc(0, 1);
    Func_020075e2(113);
    Func_0200755e(11, 256, 60);
    Func_02003a30(11);
    *(s32 *)(Data_03001ebc + 448) = 512;
    Data_02000240[0x22b] = 3;
    Func_020075cc(98, 2);
    Func_02007516(11, 0, 0);
    Func_020074b2();
    Func_02007498(2377);
}

/* Runs a scripted beat on the objects indexed 12, 13 and 14, stepping
 * through the entries at Data_00002438 as it goes, then sets the scene
 * phase word and a status byte at +0x22b of the record at Data_02000240
 * before handing off to the next step. */
void FieldScene_RunSupplementalSequenceOne(void)
{
    extern u8 Data_02000240[];
    extern u8 Data_03001ebc[];

    u32 i;
    s32 record;
    s32 sequence_2438;
    s32 status_record_2000240;

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
    ObjectMotion_SetAngleToward_7_02001e94(12, 0, 0);
    ObjectMotion_CallThenWaitForAnimationChange_4_02001e94(12, 3);
    BattleRuntime_WaitIfModeZero_5_02001e94(30);
    SceneWork_SetStepValue_7_02001e94((sequence_2438 + 6));
    BattleEvent_RunActionAndWait_7_02001e94(12, 0);
    SCENE_PHASE = 0x200;
    SharedWorkData_SetFirstAndSecondFields_1_02001e94((s32)Data_000000a1, 31);
    status_record_2000240 = (s32)Data_02000240;
    /* Status byte at +0x22b of the record. */
    *(u8 *)((status_record_2000240 + 0x22b)) = 3;
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

    Func_0200777c(0, 1);
    Func_02007882(113);
    Func_020077fe(16, 256, 60);
    Func_02003cd0(16);
    *(s32 *)(Data_03001ebc + 448) = 512;
    Data_02000240[0x22b] = 3;
    Func_0200786c_a(98, 2);
    Func_020077b6(16, 0, 0);
    Func_02007752();
    Func_02007738(2379);
}

void RunActor12InteractionSequence(void)
{
    Func_02007768();
    Func_020077d2(0, 0, 0);
    Func_020077aa(0, 1);
    Func_020077fa(0, 1);
    Func_0200782c(12, 0, 0);
    Func_0200790a(113);
    Func_02007886_a(12, 256, 60);
    {
        u8 *t = &Value_0000240d;

        Func_0200785e((s32)t);
        Func_02007876(12, 0);
        Func_020078a4(0, 258, 50);
        Func_0200787a((s32)(t + 1));
    }
    Func_02007892(12, 0);
    Func_02007916();
    Func_020077cc(60);
    Func_02007902(60);
    Func_020077e6();
    Func_020077ce(548);
}

void FieldScene_RunScene3bf_020021c4(void)
{
    extern u8 Data_03001ebc[];

    u32 i;
    s32 record;
    s32 base5_240d;

    Func_020077f8();
    Func_02007862(0, 0, 0);
    Func_0200783a(0, 1);
    Func_0200788a(0, 1);
    Func_02007990_a(113);
    Call3(Func_0200790c, 21, 0x100, 0);
    Call3(Func_02007918, 13, 0x100, 60);
    Func_020078da(21, 0, 0);
    Func_020078e4(13, 0, 0);
    base5_240d = (s32)Data_0000240d;
    Func_02007904(base5_240d);
    Func_0200791c_a(13, 0);
    Call3(Func_0200794a, 0, 0x102, 30);
    Func_02007920((base5_240d + 1));
    Func_02007938(13, 0);
    Func_020079bc();
    Func_02007872_a(60);
    Func_020079a8(60);
    Func_0200788c();
    Call1_020021c4(Func_02007872_b, 0x225);
}

void ConfigureInteractionRegionA(void)
{
    extern s16 Data_02000240[];
    extern u8 *Data_03001ebc;

    Func_02007852(2, 82, 1, 2, 21, 81);
    Func_0200786c(21, 32, 1, 1, 21, 34);
}

void ConfigureInteractionRegionB(void)
{
    extern s16 Data_02000240[];
    extern u8 *Data_03001ebc;

    Func_02007886(2, 84, 1, 2, 6, 55);
    Func_020078a0(5, 9, 1, 1, 6, 10);
}

void ConfigureInteractionRegionC(void)
{
    extern s16 Data_02000240[];
    extern u8 *Data_03001ebc;

    Func_020078ba(2, 86, 1, 2, 27, 62);
    Func_020078d4(26, 16, 1, 1, 27, 17);
}

void InspectVillageWell(void)
{
    extern u8 Data_02000240[];

    extern u8 Data_03001ebc[];

    u32 i;
    s32 record;
    s32 v5;
    s32 v6;

    if (*(s16 *)((*(u8 *volatile *)Data_03001ebc + 0xcb8)) != 0) {
        if (Value1_02001dc4(Func_02007930, 0x947) == 0) {
            Call2(Func_0200792c, 0x1528, 1);
            Func_02007ada(188);
            v5 = 17;
            v6 = 82;
            Func_02007960(1);
            Call6_02001dc4(Func_0200791c, 6, 77, 1, 2, v5, v6);
            Func_0200797a(5);
            Call6_02001dc4(Func_02007932, 7, 77, 1, 2, v5, v6);
            Func_02007990(1);
            Func_020045d8();
            Call1_02001dc4(Func_0200798a, 0x947);
        }
    }
}

void RunSecondaryMapInteraction(void)
{
    extern u8 Data_03001ebc[];

    u32 i;
    s32 record;
    s32 v5;
    s32 v6;

    if (*(s16 *)((*(u8 *volatile *)Data_03001ebc + 0xcb8)) != 0) {
        if (Value1_0200169c(Func_020079b4, 0x948) == 0) {
            Call2(Func_020079b0, 0x1528, 1);
            Func_02007b5e(188);
            v5 = 3;
            v6 = 55;
            Func_020079e4(1);
            Call6_0200169c(Func_020079a0, 6, 77, 1, 2, v5, v6);
            Func_020079fe(5);
            Call6_0200169c(Func_020079b6, 7, 77, 1, 2, v5, v6);
            Func_02007a14(1);
            Func_02004690();
            Call1_0200238c(Func_02007a0e, 0x948);
        }
    }
}

void ConfigurePrimaryInteractionRegions(void)
{
    extern s16 Data_02000240[];
    extern u8 *Data_03001ebc;

    Func_020079f6(5, 77, 1, 2, 17, 82);
    Func_02007a0a(5, 77, 1, 2, 3, 55);
    Func_02007a24(15, 33, 1, 1, 17, 35);
    Func_02007a36(3, 8, 1, 1, 3, 10);
}

void ConfigureSecondaryInteractionRegions(void)
{
    extern s16 Data_02000240[];
    extern u8 *Data_03001ebc;

    Func_02007a4e(8, 77, 1, 2, 17, 82);
    Func_02007a62(8, 77, 1, 2, 3, 55);
    Func_02007a7c(18, 35, 1, 1, 17, 35);
    Func_02007a8e(2, 10, 1, 1, 3, 10);
}

void InspectWardrobe(void)
{
    extern u8 Data_02000240[];

    Func_02007ade(2372);
    Func_02007aec(535);
    Func_02007b76(8, 0, 0);
}

void InspectFirewood(void)
{
    extern u8 Data_02000240[];

    Func_02007b02(2373);
    Func_020047c2();
    Func_02007b98(9, 0, 0);
}

void InspectBooks(void)
{
    extern u8 Data_02000240[];

    Func_02007b22(2374);
    Func_02007b32(536);
    Func_02007bbc(10, 0, 0);
}

void NoOpInteractionCallback(void)
{
    extern s16 Data_02000240[];
    extern u8 *Data_03001ebc;

}

void FieldScene_RunScene3bf_0200252c(void)
{
    extern u8 Data_03001ebc[];

    u32 i;
    s32 record;

    record = Value1_0200252c(Func_02007b82, 0);
    if (record != 0) {
        Func_02007be0(2, *(volatile s32 *)(record + 8), *(volatile s32 *)(record + 16));
    }
    record = Value1_0200252c(Func_02007b96, 0);
    if (record != 0) {
        Func_02007bf4(3, *(volatile s32 *)(record + 8), *(volatile s32 *)(record + 16));
    }
    record = Value1_0200252c(Func_02007baa, 0);
    if (record != 0) {
        Func_02007c08_a(1, *(volatile s32 *)(record + 8), *(volatile s32 *)(record + 16));
    }
    Func_02007c7a(0, 0, 0);
    Call3(Func_02007bd4, 2, 0xb333, 0x5999);
    Call3(Func_02007c08_b, 2, 0x1c8, 192);
    Call3(Func_02007bea, 3, 0xb333, 0x5999);
    Call3(Func_02007c1e, 3, 0x1b8, 184);
    Call3(Func_02007c00, 1, 0xb333, 0x5999);
    Call3(Func_02007c34, 1, 0x1c0, 240);
    Func_02007c52(2);
    Func_02007c94(2, 12, 0);
    Func_02007c62(1);
    Func_02007c68(3);
    Func_02007caa(1, 12, 0);
    Func_02007cb4(3, 12, 0);
    Func_02007c12(15);
}

void FieldScene_RunScene3bf_020025f8(void)
{
    extern u8 Data_03001ebc[];

    u32 i;
    s32 record;

    Call3(Func_02007bfa, 0x40000, 0x40000, 0x10000);
    Func_02007db8(141);
    Func_02007c3e(80);
    Call1_020025f8(Func_02007dc6_a, 0x120);
    Func_02007c4c(5);
    Func_02007dd2(145);
    Call6(Func_02007c0e, 16, 75, 7, 4, 26, 55);
    Call3(Func_02007c3c, -1, -1, 0xe666);
    Call3(Func_02007d70, 0, 0x100, 0);
    Call3(Func_02007d7c, 1, 0x100, 0);
    Call3(Func_02007d88, 2, 0x100, 0);
    Call3(Func_02007d94, 3, 0x100, 0);
    Value3_020025f8(Func_02007da0, 12, 0x100, 0);
    Func_02007cb6(60);
}

void FieldScene_RunScene3bf_0200269c(void)
{
    extern u8 Data_03001ebc[];

    u32 i;
    s32 record;

    Func_02007dec(11, 1);
    Func_02007de8();
    Func_02007cd6(60);
    Call1_0200269c(Func_02007d9c, 0x247c);
    Func_02007db4(13, 0);
    Call3(Func_02007d22, 11, 0x10000, 0x8000);
    Call3(Func_02007d30, 15, 0x10000, 0x8000);
    Call3(Func_02007d64, 11, 0x1d8, 180);
    Call3(Func_02007d70_a, 15, 0x1d8, 180);
    Func_02007e20(11, 1);
    Func_02007d96(11);
    Func_02007dae(11, 4);
    Func_02007d34(30);
}

void FieldScene_RunScene3bf_02002718(void)
{
    extern u8 Data_03001ebc[];

    u32 i;
    s32 record;

    Call3_02002718(Func_02007d7e, 2, 0x10000, 0x8000);
    Call3_02002718(Func_02007db2_a, 2, 0x1f8, 216);
    Call3_02002718(Func_02007d98, 3, 0x10000, 0x8000);
    Call3_02002718(Func_02007dcc, 3, 0x1b8, 232);
    Call3_02002718(Func_02007db2_b, 1, 0x10000, 0x8000);
    Call3_02002718(Func_02007de6, 1, 0x1e0, 224);
    Func_02007e04(1);
    Call3_02002718(Func_02007e80, 1, 0xc000, 0);
    Func_02007e16(2);
    Call3_02002718(Func_02007e92, 2, 0xc000, 0);
    Func_02007e28(3);
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
    Call6(Func_02008646, 16, 75, 7, 4, 26, 55);
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
    Func_02009d7e();
    BattleRuntime_WaitIfModeZero_101(1);
    BattleRuntime_WaitIfModeZero_102(210);
    Func_02009d82(4);
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
        Func_02009d70(0x2414);
        Func_02009d88(21, 0);
        break;
    case 1:
        Func_02009d70(0x2415);
        Func_02009d88(21, 0);
        break;
    case 2:
        Func_02009d70(0x2416);
        Func_02009d88(21, 0);
        break;
    case 3:
        Func_02009d70(0x2417);
        Func_02009d88(21, 0);
        break;
    case 4:
        Func_02009d70(0x2418);
        Func_02009d88(21, 0);
        break;
    case 6:
        Func_02009d70(0x241a);
        Func_02009d88(21, 0);
        break;
    case 7:
        Func_02009d70(0x241b);
        Func_02009d88(21, 0);
        break;
    case 5:
        Call3(Func_02009da6, 21, 0xd000, 0);
        Func_02009ccc(50);
        Call3(Func_02009db8, 21, 0xb000, 0);
        Func_02009cde(50);
        Call3(Func_02009dca, 21, 0x5000, 0);
        Func_02009cf0(50);
        Func_02009db6(0x2419);
        Func_02009dce(21, 0);
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

    Call3(Func_02009e26, 21, 0x101, 30);
    Call3(Func_02009e22, 21, 0xd000, 0);
    Func_02009d48(50);
    Call3(Func_02009e34_a, 21, 0xb000, 0);
    Func_02009d5a(50);
    Call3(Func_02009e46, 21, 0x5000, 0);
    Func_02009d6c(50);
    base5_2411 = (s32)Data_00002411;
    Func_02009e34_b(base5_2411);
    Func_02009e4c(21, 0);
    Func_02009e04(21, 4);
    Func_02009d8a(60);
    Call3(Func_02009e76, 21, 0xb000, 0);
    Func_02009d9e(40);
    Func_02009e64((base5_2411 + 1));
    Func_02009e7c(21, 0);
}

void FieldScene_RunScene3bf_02004794(void)
{
    extern u8 Data_03001ebc[];

    u32 i;
    s32 record;
    s32 base5_244f;
    s32 base5_2455;

    Func_02009dc8();
    if (Value1(Func_02009dae, 0x941) != 0) {
        Call1_02004794(Func_02009e90, 0x2566);
        Func_02009ea8(18, 0);
        Func_02009dec();
    } else {
        if (Value1(Func_02009dcc, 0x313) != 0) {
            Call1_02004794(Func_02009eae, 0x2457);
            Value2_02004794(Func_02009ebe, 25, 0);
            Func_02009e0a();
        } else {
            Call3(Func_02009ef8, 25, 0x102, 30);
            Func_02009eba(25, 0, 0);
            base5_244f = (s32)Data_0000244f;
            Func_02009eda(base5_244f);
            Func_02009ef2(25, 0);
            Func_02009ed4(25, 24, 0);
            Func_02009f54(24, 1);
            Func_02009f50();
            Func_02009e3e(60);
            Func_02009f66(0, 1);
            Func_02009e4c_a(20);
            Call3(Func_02009f46, 25, 0x105, 60);
            Func_02009f1c((base5_244f + 1));
            Func_02009f34_a(25, 0);
            Call3(Func_02009f5e_a, 25, 0x107, 60);
            Func_02009f34_b((base5_244f + 2));
            Func_02009f4c(25, 0);
            Func_02009e82(70);
            Call3(Func_02009f7e, 25, 0x100, 60);
            Func_02009f40(25, 0, 0);
            Func_02009f5e_b((base5_244f + 3));
            Value2_02004794(Func_02009f6e, 25, 0);
            if (Value2_02004794(Func_02009ece, 0, 0) == 0) {
                Func_02009f78((base5_244f + 4));
                Func_02009f88_a(25, 0);
            } else {
                Func_02009f88_b((base5_244f + 5));
                Func_02009f98_a(25, 0);
            }
            Func_02009ed6(60);
            Call3(Func_02009fd0, 25, 0x105, 60);
            base5_2455 = (s32)Data_00002455;
            Func_02009fa8(base5_2455);
            Value2_02004794(Func_02009fb8, 25, 0);
            Func_02009f98_b(25, 1);
            Func_02009fbe((base5_2455 + 1));
            Value2_02004794(Func_02009fce, 25, 0);
            Func_02009f98_c(25, 3);
            Func_02009fd6((base5_2455 + 2));
            Value2_02004794(Func_02009fe6, 25, 0);
            Call1_02004794(Func_02009f14, 0x313);
            Func_02009f38();
        }
    }
}

void SelectActor25SceneVariant(void)
{
    extern s16 Data_02000240[];
    extern u8 *Data_03001ebc;

    if (Func_02009f3e(0x941)) {
        Func_0200a020(0x2568);
        Func_0200a038(25, 0);
    } else {
        Func_0200a030(0x2458);
        Func_0200a048(25, 0);
    }
}

void SelectActor24SceneVariant(void)
{
    extern s16 Data_02000240[];
    extern u8 *Data_03001ebc;

    if (Func_02009f7a(0x941)) {
        Func_0200a05c(0x2569);
        Func_0200a074(24, 0);
    } else {
        Func_0200a06c(0x244e);
        Func_0200a084(24, 0);
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
    Func_0200a190(0x94e);
}

void ConfigureSceneActor14(void)
{
    extern s16 Data_02000240[];
    extern u8 *Data_03001ebc;

    Func_0200a254(14, 2);
    Func_0200a27a(0x2441);
    Func_0200a292(14, 0);
}

void ConfigureSceneActor13(void)
{
    extern s16 Data_02000240[];
    extern u8 *Data_03001ebc;

    Func_0200a274(13, 2);
    Func_0200a29a((s32)&Value_00002440);
    Func_0200a2b2(13, 0);
}

void ConfigureSceneActor12Variant(void)
{
    extern s16 Data_02000240[];
    extern u8 *Data_03001ebc;

    Func_0200a294(12, 2);
    Func_0200a2ba(0x243f);
    Func_0200a2d2(12, 0);
}

void ConfigureSceneActor18(void)
{
    extern s16 Data_02000240[];
    extern u8 *Data_03001ebc;

    Func_0200a2d2_a((s32)&Value_00002459);
    Func_0200a2f2(18, 0);
}

void RunActor20SceneSequence(void)
{
    extern u8 Data_03001ebc[];

    u32 i;
    s32 record;
    s32 base5_242e;
    s32 base5_2430;

    if (Value1_0200169c(Func_0200a212, 0x226) != 0) {
        Call1_02004bfc(Func_0200a2f4_a, 0x2434);
        Func_0200a30c(20, 0);
    } else {
        Func_0200a24a();
        Func_0200a2f4_b(20, 0, 0);
        if (Value1_0200169c(Func_0200a23a, 0x227) == 0) {
            Func_0200a2f0(20, 4, 0);
            Func_0200a2ae(20);
            Func_0200a2ac(20);
            Func_0200a272(20);
            base5_242e = (s32)Data_0000242e;
            Func_0200a33a(base5_242e);
            Func_0200a352(20, 0);
            Call3_0200169c(Func_0200a380, 20, 0x102, 30);
            Func_0200a356((base5_242e + 1));
            Func_0200a36e_a(20, 0);
            Func_0200a2a4(30);
            Func_0200a32c(20, 4);
            Func_0200a2b2_a(30);
        }
        base5_2430 = (s32)Data_00002430;
        Func_0200a37a(base5_2430);
        Func_0200a392_a(20, 0);
        Call3_0200169c(Func_0200a3bc_a, 20, 0x101, 40);
        Func_0200a392_b((base5_2430 + 1));
        Value2(Func_0200a3a2, 20, 0);
        if (Value2(Func_0200a302, 0, 0) == 0) {
            Func_0200a3ac((base5_2430 + 2));
            Func_0200a3bc_b(20, 0);
            Call1_02004bfc(Func_0200a2ea, 0x226);
        } else {
            Func_0200a3c2((base5_2430 + 3));
            Func_0200a3d2(20, 0);
        }
        Call1_02004bfc(Func_0200a300, 0x227);
        Func_0200a324();
    }
}

void FinishActor20SceneSequence(void)
{
    extern s16 Data_02000240[];
    extern u8 *Data_03001ebc;

    if (Func_0200a322(0x226)) {
        Func_0200a404(0x2435);
        Func_0200a41c(20, 0);
    } else {
        s16 *q = (s16 *)(Data_03001ebc + 382);

        *q = 0;
        Func_0200a4c6();
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

    Func_0200a440((s32)interaction_resources);
    Func_0200a458(0x800d, 0);
    if (Func_0200a36e(234) != -1) {
        Func_0200a376((s32)(interaction_resources + 2), 1);
    }
}

void ConfigureActor13SceneResource(void)
{
    extern s16 Data_02000240[];
    extern u8 *Data_03001ebc;

    Func_0200a476(0x256d);
    Func_0200a48e(13, 0);
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
        Func_0200a5ba(1);
        Func_0200a5ee();
        Func_0200a5c4(1);
        Call6(Func_0200a620, 101, 9, 10, 8, 110, 9);
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
    Func_0200a772(18, 1);
    Func_0200a77a(17, 1);
    Func_0200a782(21, 1);
    Func_0200a78a(12, 1);
    Func_0200a792(13, 1);
    Func_0200a640(1);
}

void FieldScene_SetupActorsForScene(void)
{
    extern s16 Data_02000240[];
    extern u8 Data_03001ebc[];

    struct ObjectRuntime *actor;

    Func_0200a598();
    Func_0200a7d0(9, 1);
    Func_0200a7d8(10, 1);
    Func_0200a7e0(17, 1);
    if (Func_0200a716(0x94c)) {
        Func_0200a7b4(15, 0, 0);
    }
    if (Func_0200a72a(0x949)) {
        Func_0200a7c8(11, 0, 0);
    }
    if (Func_0200a73e(0x94b)) {
        Func_0200a7dc(16, 0, 0);
    }
    if (Func_0200a752(0xf2e)) {
        Func_0200a7f0(8, 0, 0);
    }
    switch (Data_02000240[225]) {
    case 1:
    case 2:
    case 3:
        *(s32 *)(*(u8 **)Data_03001ebc + 0x1c0) = 0x200;
        Func_0200a7f8(0xe00);
        Func_0200a7f6(Data_02009151, 3200);
        Func_0200a7f4(1);
        Func_0200a828();
        Func_0200a7fe(1);
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
            Call3(Func_0200a8e4, 25, 0x3680000, 0x780000);
        }
        break;
    case 21:
    case 22:
        *(s32 *)(*(u8 **)Data_03001ebc + 0x1c0) = 0x200;
        Func_0200a854(0xe00);
        Func_0200a7f6(Data_0200938d, 3200);
        Func_0200a7f4(1);
        Func_0200a828();
        Func_0200a7fe(1);
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
    Func_0200a9f8(8, 1);
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
    Call1_02004f60(Func_0200a962, 0x217);
    Call1_02004f60(Func_0200a96a, 0x218);
    if (Func_0200a968(0x944)) {
        Func_0200aa06(8, 0, 0);
        Call1_02004f60(Func_0200a98c, 0x217);
    }
    if (Func_0200a982(0x945)) {
        Func_0200aa20(9, 0, 0);
        Func_02007658();
    }
    if (Func_0200a99a(0x946)) {
        Func_0200aa38(10, 0, 0);
        Call1_02004f60(Func_0200a9c0, 0x218);
    }
    if (Func_0200a9b6(0x947)) {
        Func_0200761a();
    }
    if (Func_0200a9c4(0x948)) {
        Func_0200765c();
    }
    Func_0200a9f0();
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
    Func_0200aa76();
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
