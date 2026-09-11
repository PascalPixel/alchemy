#include "types.h"

#define SceneData_GetTableD8f8 Func_02000030
#define SceneData_GetTableDa60 Func_02000038
#define SceneData_GetTableDa80 Func_02000040
#define SceneData_SelectTableBySceneRange Func_02000048
#define ActorPresentation_SetSceneCellByAngle Func_02000080
#define FieldScene_RunObjectTwentySixPositionCheck Func_020000fc
#define FieldScene_RunActorNineteenAngleDialogue Func_0200013c
#define FieldScene_RunActorTwentyAngleDialogue Func_0200019c
#define FieldScene_RunActorTwentyThreeAngleDialogue Func_020001fc
#define SceneState_RunGuardedActorStep Func_02000378
#define SceneDialogue_PromptAndCountSkip Func_020003bc
#define FieldScene_RunActorEighteenConditionalScene Func_0200025c
#define SceneDialogue_ShowLine12BB Func_020002a4
#define SceneDialogue_RunActorElevenDialogue Func_02000400
#define SceneDialogue_RunActor9FlaggedLine Func_020004bc
#define SceneDialogue_RunActorTwelveFlaggedDialogue Func_020004f4
#define SceneDialogue_ShowLine128E Func_0200059c
#define SceneDialogue_RunActor10Line Func_020007cc
#define SceneDialogue_RunActor11Line Func_020007ec
#define SceneDialogue_RunActor14Line Func_0200080c
#define SceneDialogue_RunActorNineFlaggedDialogue Func_02000a14
#define SceneDialogue_RunActorElevenFlaggedDialogue Func_02000a4c
#define SceneDialogue_ShowLine124EOr135E Func_02000a84
#define SceneDialogue_RunActor16FlaggedLine Func_02000abc
#define SceneDialogue_RunActorEighteenBranchedDialogue Func_02000af4
#define SceneState_SetFlags92bAnd94b Func_020002c0
#define SceneState_SetFlags929And949 Func_020002e8
#define SceneState_SetValue123Mode11 Func_02001990
#define SceneActor_UpdateAnimationOnStateMatch Func_020026e4
#define SceneState_SetFlagByActorPosition Func_0200271c
#define SceneState_SetWord1c0To209AndRun Func_020045cc
#define SceneState_SetValue2ThenFinish Func_02004684
#define SceneData_SelectTableE1fcBySceneRangeOrFlag855 Func_02000310
#define SceneData_FindEntryAtPosition Func_02004704
#define ActorPresentation_RunActorModeOneThenZeroWithStep Func_02000354
#define SceneActor_SetModeZeroAndValue Func_020045f4
#define SceneActor_SetPairZeroAndValue Func_02004624
#define BattleRuntime_Reset_1(args...) Func_02005890(args)
#define ObjectMotion_SetSpeedParameters_1(a0, a1, a2) Call3(Func_020058da, a0, a1, a2)
#define ObjectMotion_SetSpeedParameters_2(a0, a1, a2) Call3(Func_020058e4_b, a0, a1, a2)
#define ObjectMotion_SetSpeedParameters_3(a0, a1, a2) Call3(Func_020058ee, a0, a1, a2)
#define Audio_PlayCue_1(args...) Func_02005a94(args)
#define ObjectMotion_SetPositionAndReset_1(a0, a1, a2) Call3(Func_02005932, a0, a1, a2)
#define ObjectMotion_ArmCallback_1(a0, a1, a2) Call3(Func_020059b6, a0, a1, a2)
#define ObjectMotion_SetHorizontalPositionWithTerrain_1(a0, a1, a2) Call3(Func_02005964, a0, a1, a2)
#define ObjectMotion_SetPositionAndReset_2(a0, a1, a2) Call3(Func_0200595a, a0, a1, a2)
#define ObjectMotion_ArmCallback_2(a0, a1, a2) Call3(Func_020059de, a0, a1, a2)
#define GameFlag_IsSet_1(a0) Value1(Func_020058ce, a0)
#define GameFlag_Set_1(a0) Call1(Func_020058e4_a, a0)
#define BattleRuntime_WaitIfModeZero_1(args...) Func_02005912(args)
#define SceneWork_SetStepValue_1(a0) Call1(Func_020059f4, a0)
#define Audio_PlayCue_2(args...) Func_02005b0a(args)
#define BattleRuntime_WaitIfModeZero_2(args...) Func_02005928(args)
#define ObjectMotion_SetVariantCallback_1(args...) Func_020059fa(args)
#define ObjectMotion_SetVariantCallbackAndInvokeObject_1(args...) Func_02005a0a(args)
#define BattleRuntime_WaitIfModeZero_3(args...) Func_02005950(args)
#define BattleRuntime_WaitIfModeZero_4(args...) Func_0200595e(args)
#define ObjectMotion_ArmCallback_3(a0, a1, a2) Call3(Func_02005a70, a0, a1, a2)
#define ObjectMotion_ArmCallback_4(a0, a1, a2) Call3(Func_02005a7c, a0, a1, a2)
#define BattleRuntime_WaitIfModeZero_5(args...) Func_02005992(args)
#define BattleRuntime_WaitIfModeZero_6(args...) Func_020059a0(args)
#define ObjectMotion_SetVariantCallbackAndInvokeObject_2(args...) Func_02005a6c(args)
#define Object_SetModeById_1(args...) Func_02005a54(args)
#define ObjectMotion_SetVariantCallbackAndInvokeObject_3(args...) Func_02005aa0(args)
#define BattleRuntime_WaitIfModeZero_7(args...) Func_020059e6(args)
#define BattleRuntime_WaitIfModeZero_8(args...) Func_020059f8(args)
#define ObjectMotion_SetPositionAndReset_3(a0, a1, a2) Call3(Func_02005a7e, a0, a1, a2)
#define BattleRuntime_WaitIfModeZero_9(args...) Func_02005a0c(args)
#define ObjectMotion_ArmCallback_5(a0, a1, a2) Call3(Func_02005b12, a0, a1, a2)
#define ObjectMotion_ArmCallback_6(a0, a1, a2) Call3(Func_02005b1e, a0, a1, a2)
#define ObjectMotion_SetVariantCallbackAndInvokeObject_4(args...) Func_02005af6(args)
#define BattleRuntime_WaitIfModeZero_10(args...) Func_02005a3c(args)
#define ObjectMotion_ArmCallback_7(a0, a1, a2) Call3(Func_02005b42, a0, a1, a2)
#define BattleRuntime_WaitIfModeZero_11(args...) Func_02005a58(args)
#define BattleEvent_RunActionAndWait_1(args...) Func_02005b48(args)
#define ObjectMotion_SetSpeedParameters_4(args...) Func_02005b48_a(args)
#define Audio_PlayCue_3(args...) Func_02005c58(args)
#define SceneWork_SetStepValue_2(a0) Call1(Func_02005b4e, a0)
#define ObjectMotion_SetSpeedParameters_5(args...) Func_02005b5e(args)
#define UiWork_WaitThenFinalizeCapacity_1(args...) Func_02005abe(args)
#define GameFlag_Set_2(a0) Call1(Func_02005a74, a0)
#define Object_SetModeById_2(args...) Func_02005b34(args)
#define Scene_GetRecord_1(args...) Func_02005ae2(args)
#define ObjectMotion_ResetAndSetPosition_1(args...) Func_02005b14(args)
#define ObjectMotion_CommitCurrentPositionAndActivate_1(args...) Func_02005b42_a(args)
#define ObjectMotion_SetHorizontalPositionWithTerrain_2(args...) Func_02005b54(args)
#define BattleEvent_RunActionAndWait_2(args...) Func_02005bb6(args)
#define Object_SetModeById_3(args...) Func_02005b6e(args)
#define Scene_GetRecord_2(args...) Func_02005b1c(args)
#define ObjectMotion_ResetAndSetPosition_2(args...) Func_02005b4e_a(args)
#define ObjectMotion_CommitCurrentPositionAndActivate_2(args...) Func_02005b7c(args)
#define ObjectMotion_SetHorizontalPositionWithTerrain_3(args...) Func_02005b8e(args)
#define Audio_PlayCueForPartyMember_1(args...) Func_02005cca(args)
#define BattleRuntime_ScheduleShoulderButtonModeUpdate_1(args...) Func_02005b16(args)
#define ObjectMotion_SetSpeedParameters_1_020019a4(a0, a1, a2) Call3(Func_02006736, a0, a1, a2)
#define ObjectMotion_ResetAndSetPositionInMode2_1(a0, a1, a2) Call3(Func_0200676c, a0, a1, a2)
#define Audio_PlayCue_1_020019a4(a0) Value1(Func_020068f8, a0)
#define SCENE_WORD_1C8 (*(u32 *)(*(u8 **)0x03001ebc + 456))
#define ObjectMotion_SetSpeedParameters_1_020019e4(a0, a1, a2) Call3(Func_02006776, a0, a1, a2)
#define ObjectMotion_SetPositionAndReset_1_020019e4(a0, a1, a2) Call3(Func_020067b4, a0, a1, a2)
#define GameFlag_IsSet_1_020019e4(args...) Func_0200671a(args)
#define BattleRuntime_Reset_1_020019e4() Call0(Func_02006752)
#define SceneWork_SetStepValue_1_020019e4(a0) Value1(Func_02006828, a0)
#define BattleEvent_RunActionAndWait_1_020019e4(a0, a1) Call2(Func_02006840, a0, a1)
#define BattleRuntime_ScheduleShoulderButtonModeUpdate_1_020019e4() Call0(Func_0200676c_a)
#define Audio_PlayCue_1_020019e4(a0) Value1(Func_02006958, a0)
#define SCENE_WORK_FIELD_456 (*(u32 *)(*(u8 **)0x03001ebc + 456))
#define Scene_GetRecord_1_02001a4c(args...) Func_020067d0(args)
#define Scene_GetRecord_2_02001a4c(args...) Func_020067d8(args)
#define BattleRuntime_Reset_1_02001a4c() Call0(Func_020067a6)
#define ObjectMotion_SetSpeedParameters_1_02001a4c(a0, a1, a2) Call3(Func_020067f0, a0, a1, a2)
#define ObjectMotion_SetSpeedParameters_2_02001a4c(a0, a1, a2) Call3(Func_020067fa, a0, a1, a2)
#define ObjectMotion_SetSpeedParameters_3_02001a4c(a0, a1, a2) Call3(Func_02006804, a0, a1, a2)
#define ObjectMotion_SetPositionAndReset_1_02001a4c(a0, a1, a2) Call3(Func_02006840_a, a0, a1, a2)
#define ObjectMotion_SetPositionAndReset_2_02001a4c(a0, a1, a2) Call3(Func_0200684c, a0, a1, a2)
#define BattleRuntime_WaitIfModeZero_1_02001a4c(a0) Call1(Func_020067da, a0)
#define BattleEffect_SpawnLinkedResourceObject_1(a0, a1, a2) Call3(Func_020068e6, a0, a1, a2)
#define BattleEffect_SpawnLinkedResourceObject_2(a0, a1, a2) Call3(Func_020068f2, a0, a1, a2)
#define BattleRuntime_WaitIfModeZero_2_02001a4c(a0) Call1(Func_020067f8, a0)
#define ObjectMotion_SetVariantCallbackAndInvokeObject_1_02001a4c(a0, a1) Call2(Func_020068ca, a0, a1)
#define BattleRuntime_WaitIfModeZero_3_02001a4c(a0) Call1(Func_02006810, a0)
#define SceneWork_SetStepValue_1_02001a4c(a0) Value1(Func_020068ee, a0)
#define BattleRuntime_WaitIfModeZero_4_02001a4c(a0, a1) Call2(Func_02006930, a0, a1)
#define BattleRuntime_WaitIfModeZero_5_02001a4c(a0) Call1(Func_0200682e, a0)
#define ObjectMotion_SetVariantCallbackAndInvokeObject_2_02001a4c(a0, a1) Call2(Func_020068fe, a0, a1)
#define ObjectMotion_SetSpeedParameters_4_02001a4c(a0, a1, a2) Call3(Func_0200689c, a0, a1, a2)
#define ObjectMotion_SetSpeedParameters_5_02001a4c(a0, a1, a2) Call3(Func_020068aa, a0, a1, a2)
#define ObjectMotion_EnableActionAndSetCallback_1(a0, a1) Value2(Func_020068ba_a, a0, a1)
#define ObjectMotion_EnableActionAndSetCallback_2(a0, a1) Value2(Func_020068c2, a0, a1)
#define Object_LookupAndStep_1(a0) Call1(Func_020068d0, a0)
#define GameFlag_Set_1_02001a4c(a0) Value1(Func_02006872, a0)
#define GameFlag_Set_2_02001a4c(a0) Call1(Func_0200687a, a0)
#define BattleRuntime_ScheduleShoulderButtonModeUpdate_1_02001a4c() Value0(Func_020068c4)
#define RATIO_HI 52428
#define RATIO_LO 26214
#define FieldScene_RunScene383_02000428 Func_02000428
#define FieldScene_RunFlag856DialogueBranch Func_0200052c
#define FieldScene_RunScene383SequenceC Func_0200082c
#define FieldScene_RunScene383_0200091c Func_0200091c
#define FieldScene_RunSetupSequence Func_02000b48
#define FieldScene_RunOpeningSequenceHead Func_020019a4
#define FieldScene_RunOpeningSequenceSecond Func_020019e4
#define FieldScene_RunOpeningSequenceThird Func_02001a4c
#define SceneState_BranchOnSlotZeroFacingAndFlag855 Func_02000454
#define SceneActor_StepActor24AnimationByFacing Func_020005bc
#define SceneActor_FaceActors24And25TowardActorZero Func_02002ba0
#define Scene_GetRecord_1_02001ba0(a0) Value1(Func_02006924, a0)
#define Scene_GetRecord_2_02001ba0(a0) Value1(Func_0200692c, a0)
#define BattleRuntime_Reset_1_02001ba0(args...) Func_020068fa(args)
#define GameFlag_Clear_1(a0) Call1(Func_020068ea, a0)
#define ObjectMotion_EnableActionAndSetCallback_1_02001ba0(args...) Func_02006962(args)
#define ObjectMotion_EnableActionAndSetCallback_2_02001ba0(args...) Func_0200696c(args)
#define ObjectMotion_EnableActionAndSetCallback_3(args...) Func_02006980(args)
#define ObjectMotion_EnableActionAndSetCallback_4(args...) Func_0200698a(args)
#define ObjectMotion_SetSpeedParameters_1_02001ba0(a0, a1, a2) Call3(Func_0200698c, a0, a1, a2)
#define ObjectMotion_SetSpeedParameters_2_02001ba0(a0, a1, a2) Call3(Func_02006996, a0, a1, a2)
#define ObjectMotion_SetSpeedParameters_3_02001ba0(a0, a1, a2) Call3(Func_020069a0, a0, a1, a2)
#define ObjectMotion_SetPositionAndReset_1_02001ba0(a0, a1, a2) Call3(Func_020069dc, a0, a1, a2)
#define ObjectMotion_SetHorizontalPositionWithTerrain_1_02001ba0(a0, a1, a2) Call3(Func_02006a02, a0, a1, a2)
#define ObjectMotion_SetHorizontalPositionWithTerrain_2_02001ba0(a0, a1, a2) Call3(Func_02006a10, a0, a1, a2)
#define ObjectMotion_ResetAndSetPositionInMode2_1_02001ba0(a0, a1, a2) Call3(Func_020069fe, a0, a1, a2)
#define ObjectMotion_SetPositionAndReset_2_02001ba0(a0, a1, a2) Call3(Func_02006a12, a0, a1, a2)
#define ObjectMotion_CommitCurrentPositionAndActivate_1_02001ba0(args...) Func_02006a28(args)
#define ObjectMotion_ArmCallback_1_02001ba0(a0, a1, a2) Call3(Func_02006a9c, a0, a1, a2)
#define ObjectMotion_SetAngleToward_1(args...) Func_02006a7e(args)
#define ObjectMotion_SetVariantCallbackAndInvokeObject_1_02001ba0(args...) Func_02006a88(args)
#define SceneWork_SetStepValue_1_02001ba0(args...) Func_02006aa8(args)
#define BattleEvent_RunActionAndWait_1_02001ba0(args...) Func_02006ac0(args)
#define ObjectMotion_SetAngleToward_2(args...) Func_02006aaa(args)
#define Object_SetModeById_1_02001ba0(args...) Func_02006a8c(args)
#define ObjectMotion_SetSpeedParameters_4_02001ba0(a0, a1) Value2(Func_02006af0, a0, a1)
#define UiWork_WaitThenFinalizeCapacity_1_02001ba0(a0, a1) Value2(Func_02006a50, a0, a1)
#define SceneWork_SetStepValue_2_02001ba0(args...) Func_02006b12(args)
#define ObjectMotion_SetAngleToward_3(args...) Func_02006b0c(args)
#define BattleEffect_SpawnLinkedResourceObject_1_02001ba0(a0, a1, a2) Call3(Func_02006b5a, a0, a1, a2)
#define BattleRuntime_WaitIfModeZero_1_02001ba0(args...) Func_02006a60(args)
#define ObjectMotion_SetAngleToward_4(args...) Func_02006b3c(args)
#define BattleEffect_SpawnLinkedResourceObject_2_02001ba0(a0, a1, a2) Call3(Func_02006b9c, a0, a1, a2)
#define BattleEffect_SpawnLinkedResourceObject_3(a0, a1, a2) Call3(Func_02006ba8, a0, a1, a2)
#define BattleRuntime_WaitIfModeZero_2_02001ba0(args...) Func_02006aae(args)
#define SceneWork_SetStepValue_3(a0) Call1(Func_02006b9e, a0)
#define BattleEvent_RunActionAndWait_2_02001ba0(args...) Func_02006bb6(args)
#define ObjectMotion_ArmCallback_2_02001ba0(a0, a1, a2) Call3(Func_02006bca, a0, a1, a2)
#define BattleRuntime_WaitIfModeZero_3_02001ba0(args...) Func_02006ae0(args)
#define Object_SetModeById_2_02001ba0(args...) Func_02006b80(args)
#define Object_SetModeById_3_02001ba0(args...) Func_02006b88(args)
#define ObjectMotion_ResetAndSetPositionInMode2_2(a0, a1, a2) Call3(Func_02006b76, a0, a1, a2)
#define ObjectMotion_SetPositionAndReset_3_02001ba0(a0, a1, a2) Call3(Func_02006b8a, a0, a1, a2)
#define ObjectMotion_SetHorizontalPositionWithTerrain_3_02001ba0(args...) Func_02006bac(args)
#define ObjectMotion_SetHorizontalPositionWithTerrain_4(args...) Func_02006bb6_a(args)
#define ObjectMotion_SetHorizontalPositionWithTerrain_5(a0, a1, a2) Call3(Func_02006bc4, a0, a1, a2)
#define ObjectMotion_SetHorizontalPositionWithTerrain_6(a0, a1, a2) Call3(Func_02006bd2, a0, a1, a2)
#define ObjectMotion_ArmCallback_3_02001ba0(args...) Func_02006c3c(args)
#define ObjectMotion_ArmCallback_4_02001ba0(a0, a1, a2) Call3(Func_02006c48, a0, a1, a2)
#define BattleRuntime_ScheduleShoulderButtonModeUpdate_1_02001ba0(args...) Func_02006b80_a(args)
#define BattleRuntime_WaitIfModeZero_1_02001e80(args...) Func_02006bc4_a(args)
#define ObjectMotion_SetVariantCallbackAndInvokeObject_1_02001e80(args...) Func_02006c8c(args)
#define BattleRuntime_WaitIfModeZero_2_02001e80(args...) Func_02006bd2_a(args)
#define SceneWork_SetStepValue_1_02001e80(args...) Func_02006cb0(args)
#define ObjectMotion_ArmCallback_1_02001e80(args...) Func_02006cda(args)
#define BattleRuntime_WaitIfModeZero_3_02001e80(a0, a1) Call2(Func_02006cfc, a0, a1)
#define ObjectMotion_SetVariantCallbackAndInvokeObject_2_02001ba0(args...) Func_02006cbc(args)
#define ObjectMotion_CallThenWaitForAnimationChange_1(args...) Func_02006cac(args)
#define BattleRuntime_WaitIfModeZero_4_02001ba0(args...) Func_02006c12(args)
#define ObjectMotion_SetSpeedParameters_1_02001e80(a0, a1, a2) Call3(Func_02006c70, a0, a1, a2)
#define ObjectMotion_SetSpeedParameters_2_02001e80(a0, a1, a2) Call3(Func_02006c7e, a0, a1, a2)
#define ObjectMotion_EnableActionAndSetCallback_1_02001e80(a0, a1) Value2(Func_02006c8e, a0, a1)
#define ObjectMotion_EnableActionAndSetCallback_2_02001e80(a0, a1) Call2(Func_02006c96, a0, a1)
#define Object_LookupAndStep_1_02001ba0(args...) Func_02006ca4(args)
#define Scene_GetRecord_1_02001e80(args...) Func_02006c92(args)
#define Scene_GetRecord_2_02001e80(a0) Value1(Func_02006c9e, a0)
#define ACTOR_SHOWN_OFFSET 100
#define Scene_GetRecord_1_02002fd4(a0) Value1(Func_02007d56, a0)
#define BattleRuntime_Reset_1_02002fd4(args...) Func_02007d24(args)
#define ObjectMotion_SetHorizontalPositionWithTerrain_1_02002fd4(a0, a1, a2) Call3(Func_02007dba, a0, a1, a2)
#define ObjectMotion_SetHorizontalPositionWithTerrain_2_02002fd4(a0, a1, a2) Call3(Func_02007dc8, a0, a1, a2)
#define ObjectMotion_SetHorizontalPositionWithTerrain_3_02002fd4(a0, a1, a2) Call3(Func_02007dd6, a0, a1, a2)
#define Scene_GetRecord_2_02002fd4(args...) Func_02007d8c(args)
#define Scene_GetRecord_3(args...) Func_02007d98(args)
#define Scene_GetRecord_4(args...) Func_02007da4(args)
#define Object_SetModeById_1_02002fd4(args...) Func_02007e0a(args)
#define Object_SetModeById_2_02002fd4(args...) Func_02007e12(args)
#define Object_SetModeById_3_02002fd4(args...) Func_02007e1a(args)
#define Scene_GetRecord_5(args...) Func_02007dc8_a(args)
#define ObjectMotion_EnableActionAndSetCallback_1_02002fd4(args...) Func_02007df4(args)
#define ObjectMotion_SetHorizontalPositionWithTerrain_4_02002fd4(a0, a1, a2) Call3(Func_02007e42, a0, a1, a2)
#define ObjectMotion_SetHorizontalPositionWithTerrain_5_02002fd4(a0, a1, a2) Call3(Func_02007e50, a0, a1, a2)
#define ObjectMotion_SetHorizontalPositionWithTerrain_6_02002fd4(a0, a1, a2) Call3(Func_02007e5e, a0, a1, a2)
#define ObjectMotion_ArmCallback_1_02002fd4(a0, a1, a2) Call3(Func_02007eca, a0, a1, a2)
#define ObjectMotion_ArmCallback_2_02002fd4(a0, a1, a2) Call3(Func_02007ed6, a0, a1, a2)
#define ObjectMotion_ArmCallback_3_02002fd4(a0, a1, a2) Call3(Func_02007ee2, a0, a1, a2)
#define ObjectMotion_SetSpeedParameters_1_02002fd4(a0, a1, a2) Call3(Func_02007e44, a0, a1, a2)
#define ObjectMotion_ArmCallback_4_02002fd4(a0, a1, a2) Call3(Func_02007ef8, a0, a1, a2)
#define ObjectMotion_SetPositionAndReset_1_02002fd4(args...) Func_02007f2e(args)
#define Object_CommitPositionThenWaitIfModeZero_1(args...) Func_02007f42(args)
#define ObjectMotion_EnableActionAndSetCallback_2_02002fd4(args...) Func_02007e82(args)
#define BattleRuntime_WaitIfModeZero_1_02002fd4(args...) Func_02007e38(args)
#define ObjectMotion_EnableActionAndSetCallback_3_02002fd4(args...) Func_02007e90(args)
#define BattleRuntime_WaitIfModeZero_2_02002fd4(args...) Func_02007e46(args)
#define SceneWork_SetStepValue_1_02002fd4(a0) Call1(Func_02007f24, a0)
#define BattleEffect_SpawnLinkedResourceObject_1_02002fd4(a0, a1, a2) Call3(Func_02007f60, a0, a1, a2)
#define BattleRuntime_WaitIfModeZero_3_02002fd4(args...) Func_02007e66(args)
#define ObjectMotion_SetPositionAndReset_2_02002fd4(a0, a1, a2) Call3(Func_02007ef4, a0, a1, a2)
#define ObjectMotion_SetSpeedParameters_2_02002fd4(a0, a1, a2) Call3(Func_02007ece, a0, a1, a2)
#define ObjectMotion_SetPositionAndReset_3_02002fd4(a0, a1, a2) Call3(Func_02007f0c, a0, a1, a2)
#define ObjectMotion_ArmCallback_5_02001ba0(args...) Func_02007f8e(args)
#define ObjectMotion_SetPositionAndReset_4(a0, a1, a2) Call3(Func_02007f24_a, a0, a1, a2)
#define ObjectMotion_ArmCallback_6_02001ba0(a0, a1, a2) Call3(Func_02007fa8, a0, a1, a2)
#define ObjectMotion_SetPositionAndReset_5(a0, a1, a2) Call3(Func_02007f3e, a0, a1, a2)
#define ObjectMotion_ArmCallback_7_02001ba0(a0, a1, a2) Call3(Func_02007fc2, a0, a1, a2)
#define BattleRuntime_WaitIfModeZero_4_02002fd4(args...) Func_02007ed8(args)
#define ObjectMotion_SetPositionAndReset_6(a0, a1, a2) Call3(Func_02007f70, a0, a1, a2)
#define BattleRuntime_WaitIfModeZero_5_02001ba0(args...) Func_02007efe(args)
#define ObjectMotion_SetAngleToward_1_02002fd4(args...) Func_02007fd0(args)
#define ObjectMotion_SetAngleToward_2_02002fd4(args...) Func_02007fda(args)
#define ObjectMotion_SetAngleToward_3_02002fd4(args...) Func_02007fe4(args)
#define Object_SetModeById_4(args...) Func_02007fce(args)
#define Object_SetModeById_5(args...) Func_02007fd6(args)
#define ObjectMotion_SetPositionAndReset_7(a0, a1, a2) Call3(Func_02007fce_a, a0, a1, a2)
#define BattleRuntime_WaitIfModeZero_6_02001ba0(args...) Func_02007f5c(args)
#define ObjectMotion_SetVariantCallbackAndInvokeObject_1_02002fd4(args...) Func_02008024(args)
#define ObjectMotion_EnableActionAndSetCallback_4_02002fd4(args...) Func_02007fc4(args)
#define ObjectMotion_SetVariantCallbackAndInvokeObject_2_02002fd4(args...) Func_0200803c(args)
#define BattleRuntime_WaitIfModeZero_7_02001ba0(args...) Func_02007f82(args)
#define ObjectMotion_SetAngleToward_4_02002fd4(args...) Func_02008054(args)
#define ObjectMotion_SetAngleToward_5(args...) Func_0200805e(args)
#define ObjectMotion_SetVariantCallbackAndInvokeObject_3_02001ba0(args...) Func_02008070(args)
#define ObjectMotion_EnableActionAndSetCallback_5(args...) Func_02008010(args)
#define BattleEffect_SpawnLinkedResourceObject_2_02002fd4(a0, a1, a2) Call3(Func_020080ca, a0, a1, a2)
#define BattleRuntime_WaitIfModeZero_8_02001ba0(args...) Func_02007fd0_a(args)
#define BattleEffect_SpawnLinkedResourceObject_3_02002fd4(a0, a1, a2) Call3(Func_02008126, a0, a1, a2)
#define BattleRuntime_WaitIfModeZero_9_02001ba0(args...) Func_0200802c(args)
#define BattleEffect_SpawnLinkedResourceObject_4(a0, a1, a2) Call3(Func_0200813e, a0, a1, a2)
#define BattleRuntime_WaitIfModeZero_10_02001ba0(args...) Func_02008044(args)
#define Object_SetModeById_6(args...) Func_020080ee(args)
#define ObjectMotion_SetAngleToward_6(args...) Func_0200813e_a(args)
#define ObjectMotion_SetVariantCallback_1_02001ba0(args...) Func_02008140(args)
#define ObjectMotion_SetVariantCallbackAndInvokeObject_4_02001ba0(args...) Func_02008150(args)
#define BattleRuntime_WaitIfModeZero_11_02001ba0(args...) Func_02008096(args)
#define BattleRuntime_WaitIfModeZero_12(a0, a1) Call2(Func_020081a8, a0, a1)
#define BattleRuntime_WaitIfModeZero_13(args...) Func_020080a6(args)
#define ObjectMotion_SetSpeedParameters_3_02002fd4(a0, a1) Value2(Func_02008198, a0, a1)
#define UiWork_WaitThenFinalizeCapacity_1_02002fd4(a0, a1) Value2(Func_020080f8, a0, a1)
#define BattleRuntime_WaitIfModeZero_14(args...) Func_020080ca_a(args)
#define ObjectMotion_SetAngleToward_7(args...) Func_0200819c(args)
#define ObjectMotion_SetVariantCallback_2(args...) Func_020081a2(args)
#define ObjectMotion_SetVariantCallbackAndInvokeObject_5(args...) Func_020081b2(args)
#define BattleEffect_SpawnLinkedResourceObject_5(a0, a1, a2) Call3(Func_02008238, a0, a1, a2)
#define BattleRuntime_WaitIfModeZero_15(args...) Func_0200813e_b(args)
#define ObjectMotion_SetAngleToward_8(args...) Func_02008218(args)
#define ObjectMotion_SetVariantCallback_3(args...) Func_0200821e(args)
#define ObjectMotion_SetVariantCallbackAndInvokeObject_6(args...) Func_0200822e(args)
#define BattleRuntime_WaitIfModeZero_16(args...) Func_02008174(args)
#define BattleEffect_SpawnLinkedResourceObject_6(a0, a1, a2) Call3(Func_02008280, a0, a1, a2)
#define BattleRuntime_WaitIfModeZero_17(args...) Func_02008186(args)
#define SceneWork_SetStepValue_2_02002fd4(a0) Call1(Func_0200826e, a0)
#define Object_SetModeById_7(args...) Func_0200823e(args)
#define ObjectMotion_SetPositionAndReset_8(a0, a1, a2) Call3(Func_02008236, a0, a1, a2)
#define ObjectMotion_ArmCallback_8(a0, a1, a2) Call3(Func_020082ba, a0, a1, a2)
#define BattleRuntime_WaitIfModeZero_18(args...) Func_020081d0(args)
#define ObjectMotion_SetAngleToward_9(args...) Func_020082a2(args)
#define ObjectMotion_SetAngleToward_10(args...) Func_020082ac(args)
#define ObjectMotion_SetVariantCallbackAndInvokeObject_7(args...) Func_020082b6(args)
#define BattleRuntime_WaitIfModeZero_19(args...) Func_020081fc(args)
#define ObjectMotion_SetSpeedParameters_4_02002fd4(a0, a1, a2) Call3(Func_02008256, a0, a1, a2)
#define ObjectMotion_SetSpeedParameters_5_02001ba0(a0, a1, a2) Call3(Func_02008260, a0, a1, a2)
#define ObjectMotion_SetSpeedParameters_6(a0, a1, a2) Call3(Func_0200826a, a0, a1, a2)
#define ObjectMotion_SetHorizontalPositionWithTerrain_7(a0, a1, a2) Call3(Func_020082c0, a0, a1, a2)
#define ObjectMotion_SetPositionAndReset_9(a0, a1, a2) Call3(Func_020082b6_a, a0, a1, a2)
#define ObjectMotion_SetHorizontalPositionWithTerrain_8(a0, a1, a2) Call3(Func_020082e6, a0, a1, a2)
#define ObjectMotion_SetPositionAndReset_10(a0, a1, a2) Call3(Func_020082dc, a0, a1, a2)
#define ObjectMotion_SetHorizontalPositionWithTerrain_9(a0, a1, a2) Call3(Func_02008302, a0, a1, a2)
#define ObjectMotion_ResetAndSetPositionInMode2_1_02002fd4(a0, a1, a2) Call3(Func_020082f0, a0, a1, a2)
#define ObjectMotion_SetPositionAndReset_11(a0, a1, a2) Call3(Func_02008306, a0, a1, a2)
#define ObjectMotion_CommitCurrentPositionAndActivate_1_02002fd4(args...) Func_0200831c(args)
#define ObjectMotion_SetAngleToward_11(args...) Func_02008366(args)
#define ObjectMotion_SetAngleToward_12(args...) Func_0200837a(args)
#define ObjectMotion_SetAngleToward_13(args...) Func_02008384(args)
#define ObjectMotion_SetVariantCallbackAndInvokeObject_8(args...) Func_020083ba(args)
#define BattleRuntime_WaitIfModeZero_20(args...) Func_02008300(args)
#define Object_SetModeById_8(args...) Func_020083d8(args)
#define Object_SetModeById_9(args...) Func_020083f4(args)
#define ObjectMotion_ResetAndSetPositionInMode2_2_02002fd4(a0, a1, a2) Call3(Func_020083e4, a0, a1, a2)
#define ObjectMotion_SetPositionAndReset_12(a0, a1, a2) Call3(Func_020083fa, a0, a1, a2)
#define ObjectMotion_SetAngleToward_14(args...) Func_02008454(args)
#define ObjectMotion_CommitCurrentPositionAndActivate_2_02001ba0(args...) Func_0200841a(args)
#define ObjectMotion_SetVariantCallbackAndInvokeObject_9(args...) Func_02008464(args)
#define BattleRuntime_WaitIfModeZero_21(args...) Func_020083aa(args)
#define Object_SetModeById_10(args...) Func_02008476(args)
#define Object_SetModeById_11(args...) Func_0200847e(args)
#define ObjectMotion_SetSpeedParameters_7(args...) Func_02008580(args)
#define BattleRuntime_WaitIfModeZero_22(args...) Func_02008416(args)
#define BattleRuntime_ScheduleShoulderButtonModeUpdate_1_02002fd4(args...) Func_0200842a(args)
#define SCENE_PHASE (*(s32 *)(*(u8 *volatile *)Data_03001ebc + 0x1c0))
#define FieldScene_RunScene383SequenceB Func_02001ba0
#define FieldScene_RunSteps107And250 Func_02001e64
#define FieldScene_ConfigurePairedActors Func_02001e80
#define FieldScene_RunLateSequence Func_02002bf0
#define RunEventScript01 Func_02002fd4
#define RunDialoguePromptScene Func_020036f8
#define FieldScene_RunScene383_02004b2c Func_02004b2c
#define OverlayObject_GetObjectTwoByte118 Func_02002f84
#define OverlayObject_RunObjectTwoWhenFlagged Func_02002f98
#define OverlayObject_ConfigureObject22WithResource17 Func_02004698
#define FieldScene_RunSplitTripleSteps Func_0200460c
#define SceneEffect_ApplyThreeValuesAndFinish Func_0200463c
#define SceneEffect_ApplyPairWithValue141 Func_02004658
#define SceneActor_CheckTileFreeOfKinds Func_02004840
#define SceneActor_ApplyScaledBytePairPosition Func_020048a4

struct Object {
    u8 filler00[8];
    s32 x;
};

struct SceneRecord {
    u8 pad[100];
    u16 value100;
};

typedef struct {
    u8 filler0[6];
    u16 unk6;
} T;

/*
 * resource_383 owner at 0x02002ba0, 80 bytes.
 * Points two records at a third: each gets the angle from its own offset to the
 * reference record, stored as a halfword at +6.
 */
struct Rec_383 {
    u8 pad00[6];
    u16 f6;                     /* +6  */
    s32 f8;                     /* +8  */
    u8 pad0c[4];
    s32 f16;                    /* +16 */
};

extern s16 Data_02000240[];
extern u8 Data_0200dcc8[];
extern u8 Data_0200dab8[];
extern u8 Value_00001280;
extern u8 Data_0200e1fc[];
extern u8 Data_0200e250[];
extern u8 Data_0200de30[];
extern u8 Data_0200cf2c[];
extern u8 Data_00001360[];
extern u8 Value_00000854;
extern u8 Value_000012c3;
extern s32 Data_0200e4a8[];
extern s32 Data_0200e4c0[];
extern u8 Data_00001299[];
extern u8 Data_000012a0[];
extern u8 Data_0200d17c[];
extern u8 Data_00000000[];
extern u8 Data_000012fc[];
extern u8 Data_00001324[];
extern u8 Data_0200d354[];
extern u8 Data_0200d4c8[];
extern u8 Data_0200e4f8[];

void Func_02004dbc(u8 *);
u8 *Func_02004e04(s32);
u8 *Func_02004e14(s32);
u8 *Func_02004e3e(s32);
u8 *Func_02004e4e(s32);
void Func_02004f32(void);
void Func_02004d9e(s32, s32, s32, s32, s32, s32);
void Func_02004f6c(void);
void Func_02004dd8(s32, s32, s32, s32, s32, s32);
void Func_02004e46(void);
void Func_02004e46_b(s32);
struct Object *Func_02004e84(s32);
void Func_02004e08(s32, s32, s32, s32, s32, s32);
void Func_02004e7a(void);
u8 *Func_02004ebe(s32);
void Func_02004e8c(void);
void Func_02005066(s32, s32);
s32 Func_02004e76(s32);
void Func_02004f80(s32);
void Func_02004f88(s32);
void Func_02004fa0(s32, s32);
void Func_02004ecc(void);
u8 *Func_02004f1e(s32);
void Func_02004eec(void);
void Func_020050c6(s32, s32);
s32 Func_02004ed6(s32);
void Func_02004fe0(s32);
void Func_02004fe8(s32);
void Func_02005000(s32, s32);
void Func_02004f2c(void);
u8 *Func_02004f7e(s32);
void Func_02004f4c(void);
void Func_0200512e(s32, s32);
s32 Func_02004f36(s32);
void Func_02005040(s32);
void Func_02005048(s32);
void Func_02005060(s32, s32);
void Func_02004f8c(void);
s32 Func_020050fe(void);
void Func_020050d6(void);
void Func_0200516e(s32, s32);
void Func_020050dc(s32);
void Func_020051cc(s32, s32);
void Func_020050f8(void);
void Func_020049ea(s32, s32, s32);
s32 Func_020051ea(s32, s32);
s32 Func_0200514a(s32, s32);
void Func_02005216(s32, s32);
void Func_02004fa4(void);
s32 Func_02004f98(void);
s32 Func_0200504c(s32, s32);
void Func_02004fb2(s32);
void Func_02005090(s32);
void Func_020050a8(s32, s32);
void Func_0200514a_a(s32, s32);
void Func_02005002(s32, s32);
void Func_02004fe6(void);
void Func_0200513c(void);
void Func_020050c2(s32);
void Func_020050da(s32, s32);
void Func_02005148(void);
void Func_0200521e(s32);
void Func_020051e6(s32, s32);
void Func_020007d4(s32);
void Func_02005168(void);
void Func_02005204(void);
s32 Func_020051da(s32);
void Func_020052e4(s32);
void Func_020052ec(s32);
void Func_02000832(s32);
void Func_0200522e(void);
void Func_0200523c(void);
s32 Func_02005212(s32);
void Func_0200531c(s32);
void Func_02005324(s32);
void Func_0200086a(s32);
void Func_02005266(void);
void Func_020052e4_a(void);
void Func_020053ba(s32);
void Func_02000900(s32);
void Func_020052fc(void);
void Func_02005514(void);
void Func_020055ea(s32);
void Func_02000b98(s32);
void Func_0200552c(void);
void Func_02005534(void);
void Func_0200560a(s32);
void Func_02000b50(s32);
void Func_0200554c(void);
void Func_02005554(void);
void Func_0200562a(s32);
void Func_02000bd8(s32);
void Func_0200556c(void);
void Func_0200575c(void);
s32 Func_02005732(s32);
void Func_0200583c(s32);
void Func_02005844(s32);
void Func_02000dae(s32);
void Func_02005786(void);
void Func_02005794(void);
s32 Func_0200576a(s32);
void Func_02005874(s32);
void Func_0200587c(s32);
void Func_02000de6(s32);
void Func_020057be(void);
void Func_020057cc(void);
s32 Func_020057a2(s32);
void Func_020058ac(s32);
void Func_020058b4(s32);
void Func_02000e1e(s32);
void Func_020057f6(void);
void Func_02005804(void);
s32 Func_020057da(s32);
void Func_020058e4(s32);
void Func_020058ec(s32);
void Func_02000e56(s32);
void Func_0200582e(void);
void Func_0200583c_a(void);
s32 Func_02005812(s32);
void Func_0200591c(s32);
s32 Func_02005824(s32);
void Func_0200592e(s32);
void Func_02005936(s32);
void Func_02000ea0(s32);
void Func_02005878(void);
void Func_02005008(void);
void Func_02004fc0(s32, s32);
void Func_02004fc8(s32, s32);
void Func_02005024(void);
void Func_02005030(void);
void Func_02004fe8_a(s32, s32);
void Func_02004ff0(s32, s32);
void Func_0200504c_a(void);
void Func_020068ba(s32);
void Func_02006808(s32);
u8 *Func_02007472(s32 actor);
void Func_02007496(s32 actor, const u8 *desc);
u8 *Func_0200749e(s32 actor);
u8 *Func_020074a6(s32 actor);
void Func_02007462(s32 flag);
void Func_02007474(s32 flag);
void Func_0200947a(void);
void Func_0200948e(void);
void Func_02009324(s32);
void Func_02009556(s32);
void Func_02009572(void);
void Func_0200957e(void);
s32 Func_02005042(s32);
void Func_02005130(s32, s32);
void Func_0200498a(s32, s32, s32);
void Func_02005192(s32, s32);
void Func_02009420(s32, s32);
void Func_0200933e(s32);
void Func_02009438(s32, s32, s32);
void Func_0200936e(s32);
void Func_020007f4();
void Func_02005170();
void Func_02005194();
void Func_02005246();
void Func_02005270();
void Func_02005274(void);
s32 Func_0200524a(s32);
s32 Func_02005254(s32);
void Func_0200535e(s32);
void Func_020008a4(s32);
void Func_02005292(s32);
void Func_02004b9c(s32, s32, s32);
void Func_02005282(s32);
void Func_02005382(s32);
void Func_0200538a(s32);
void Func_020008d0(s32);
void Func_020052cc(void);
void Func_02004e3e_a();
void Func_02004e50();
void Func_02004e90();
void Func_02004ea8();
void Func_02004ea8_a();
void Func_02004eca();
s32 Func_0200554a();
void Func_02005574();
void Func_020055a8();
void Func_020055b6();
s32 Func_020055d6();
void Func_020055dc();
void Func_02005604();
void Func_02005618();
void Func_02005634();
void Func_02005650();
void Func_02005656();
s32 Func_02005658();
void Func_02005670();
void Func_02005692();
void Func_020056c0();
void Func_020056e2();
void Func_020056f2();
void Func_02005724();
void Func_02004f4e();
s32 Func_02005644();
void Func_02005664();
void Func_020056a0();
void Func_020056ae();
s32 Func_020056b8();
void Func_020056bc();
s32 Func_020056ce();
void Func_020056d2();
void Func_020056d8();
void Func_02005710();
void Func_0200571e();
void Func_02005722();
void Func_02005746();
void Func_0200574e();
void Func_0200575e();
void Func_0200575e_a();
void Func_0200576c();
void Func_0200576e();
void Func_02005776();
void Func_02005790();
void Func_020057b0();
void Func_020057b8();
void Func_020057c8();
void Func_0200580e();
void Func_02005822();
void Func_0200586a();
void Func_02001b36();
void Func_020051f2();
void Func_02005222();
void Func_02005228();
void Func_02005232();
void Func_0200525c();
void Func_0200525e();
void Func_0200526c();
void Func_02005274_a();
void Func_02005290();
void Func_020052aa();
void Func_020052b6();
void Func_020052c6();
void Func_020052d0();
void Func_020052ec_a();
void Func_020052fe();
void Func_02005346();
void Func_02005890();
s32 Func_020058ce();
void Func_020058da();
void Func_020058e4_b();
void Func_020058e4_a();
void Func_020058ee();
void Func_02005912();
void Func_02005928();
void Func_02005932();
void Func_02005950();
void Func_0200595a();
void Func_0200595e();
void Func_02005964();
void Func_02005992();
void Func_020059a0();
void Func_020059b6();
void Func_020059de();
void Func_020059e6();
void Func_020059f4();
void Func_020059f8();
void Func_020059fa();
void Func_02005a0a();
void Func_02005a0c();
void Func_02005a3c();
void Func_02005a54();
void Func_02005a58();
void Func_02005a6c();
void Func_02005a70();
void Func_02005a74();
void Func_02005a7c();
void Func_02005a7e();
void Func_02005a94();
void Func_02005aa0();
s32 Func_02005abe();
void Func_02005af6();
void Func_02005b0a();
void Func_02005b12();
void Func_02005b14();
void Func_02005b16();
void Func_02005b1e();
void Func_02005b34();
void Func_02005b42();
void Func_02005b42_a();
void Func_02005b48();
void Func_02005b48_a();
void Func_02005b4e();
void Func_02005b4e_a();
void Func_02005b54();
void Func_02005b5e();
void Func_02005b6e();
void Func_02005b7c();
void Func_02005b8e();
void Func_02005bb6();
void Func_02005c58();
void Func_02005cca();
u8 *Func_02005ae2_a();
u8 *Func_02005b1c();
void Func_02006736();
void Func_0200676c();
void Func_02006846();
void Func_020068f8();
s32 Func_0200671a();
void Func_02006752();
void Func_0200676c_a();
void Func_02006776();
void Func_020067b4();
void Func_02006828();
void Func_02006840();
void Func_020068a6();
void Func_02006958();
void Func_020060d2();
void Func_020060d6();
void Func_020060ee();
void Func_020060fe();
void Func_020067a6();
void Func_020067da();
void Func_020067dc();
void Func_020067f0();
void Func_020067f8();
void Func_020067fa();
void Func_02006804();
void Func_02006810();
void Func_0200682e();
void Func_02006834();
void Func_02006840_a();
void Func_0200684c();
void Func_02006872();
void Func_0200687a();
void Func_0200689c();
void Func_020068aa();
void Func_020068ba_a();
void Func_020068c2();
void Func_020068c4();
void Func_020068ca();
void Func_020068d0();
void Func_020068e6();
void Func_020068ee();
void Func_020068f2();
void Func_020068fe();
void Func_02006930();
void *Func_020067d0();
u8 *Func_020051d6(s32);
void Func_020051a4(void);
void Func_0200537e(s32, s32);
s32 Func_0200518e(s32);
void Func_02005298(s32);
void Func_02000846(s32);
void Func_020052a6(s32);
void Func_020052be(s32, s32);
void Func_020051ea_a(void);
T *Func_0200533e(s32);
void Func_0200530c(void);
void Func_020053cc(s32, s32);
void Func_020053ea(s32);
void Func_02005402(s32, s32);
void Func_02005370(s32, s32, s32);
void Func_020053a2(s32, s32);
void Func_020053be(s32, s32);
void Func_020053de(s32, s32);
void Func_02005418(s32);
void Func_020053d4(void);
struct Rec_383 *Func_02007926();
struct Rec_383 *Func_0200792e();
struct Rec_383 *Func_02007936();
s32 Func_02007858();
s32 Func_0200786c();
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
void Func_02006b80_a();
void Func_02006b88();
void Func_02006b8a();
void Func_02006b9c();
void Func_02006b9e();
void Func_02006ba8();
void Func_02006bac();
void Func_02006bb6();
void Func_02006bb6_a();
void Func_02006bc4();
void Func_02006bca();
void Func_02006bd2();
void Func_02006c3c();
void Func_02006c48();
void Func_02006b86(s32);
void Func_02006b8e(s32);
void Func_020069a2(void);
void Func_02006498();
void Func_020064bc();
void Func_020064d2();
void Func_02006bc4_a();
void Func_02006bd2_a();
void Func_02006c12();
void Func_02006c20();
void Func_02006c70();
void Func_02006c7e();
void Func_02006c8c();
s32 Func_02006c8e();
u8 *Func_02006c92();
void Func_02006c96();
s32 Func_02006c9e();
void Func_02006ca4();
void Func_02006cac();
void Func_02006cb0();
void Func_02006cbc();
void Func_02006cda();
void Func_02006cfc();
void Func_020072d6();
void Func_0200731c();
void Func_02007324();
void Func_02007356();
void Func_02007378();
void Func_0200738a();
void Func_0200739a();
void Func_020073b8();
void Func_02007938();
void Func_020079b0();
void Func_020079bc();
void Func_020079c8();
void Func_020079ca();
void Func_020079ce();
void Func_020079d0();
void Func_020079da();
void Func_020079dc();
void Func_020079ea();
s32 Func_02007a1a();
s32 Func_02007a26();
s32 Func_02007a30();
s32 Func_02007a30_a();
s32 Func_02007a3a();
void Func_02007a3e();
s32 Func_02007a46();
void Func_02007a4c();
void Func_02007a4c_a();
s32 Func_02007a52();
void Func_02007a56();
void Func_02007a5c();
void Func_02007a62();
void Func_02007a64();
void Func_02007a6c();
void Func_02007a6e();
void Func_02007aae();
void Func_02007ae6();
void Func_02007aea();
void Func_02007af8();
void Func_02007b14();
void Func_02007b16();
void Func_02007b26();
void Func_02007b34();
void Func_02007b4a();
void Func_02007b5e();
void Func_02007b88();
void Func_02007b98();
void Func_020076ba();
void Func_0200770c();
void Func_02007726();
void Func_020077a2();
void Func_020077e2();
void Func_020077e6();
void Func_020077f6();
void Func_02007824();
void Func_02007848();
void Func_02007860();
void Func_02007870();
void Func_02007870_a();
void Func_02007890();
void Func_020078b2();
void Func_020078b6();
void Func_020078bc();
void Func_020078d2();
void Func_020078d8();
void Func_020078ec();
void Func_020078f6();
void Func_02007912();
void Func_02007936_a();
void Func_02007950();
void Func_02007960();
void Func_02007980();
void Func_02007984();
void Func_020079ae();
void Func_020079b8();
void Func_020079fe();
void Func_02007a2a();
void Func_02007a2a_a();
void Func_02007a56_a();
void Func_02007a90();
void Func_02007aa6();
void Func_02007ab0();
void Func_02007abc();
void Func_02007abe();
void Func_02007b18();
void Func_02007b78();
void Func_02007b90();
void Func_02007b96();
void Func_02007b98_a();
void Func_02007ba0();
void Func_02007bc0();
void Func_02007bd0();
void Func_02007bda();
void Func_02007bf0();
void Func_02007c00();
void Func_02007c04();
void Func_02007c12();
void Func_02007c26();
void Func_02007c3c();
void Func_02007c4a();
void Func_02007c66();
void Func_02007c6a();
void Func_02007c6c();
void Func_02007c7c();
void Func_02007c8e();
void Func_02007cb6();
void Func_02007cbc();
void Func_02007cf0();
void Func_02007d02();
void Func_02007d0e();
void Func_02007d1a();
void Func_02007d24();
s32 Func_02007d56();
s32 Func_02007d8c();
s32 Func_02007d98();
s32 Func_02007da4();
void Func_02007dae();
void Func_02007dba();
s32 Func_02007dc8();
s32 Func_02007dc8_a();
void Func_02007dd6();
void Func_02007df4();
void Func_02007e0a();
void Func_02007e12();
void Func_02007e1a();
void Func_02007e38();
void Func_02007e42();
void Func_02007e44();
void Func_02007e46();
void Func_02007e50();
void Func_02007e5e();
void Func_02007e66();
void Func_02007e82();
void Func_02007e90();
void Func_02007eca();
void Func_02007ece();
void Func_02007ed6();
void Func_02007ed8();
void Func_02007ee2();
void Func_02007ef4();
void Func_02007ef8();
void Func_02007efe();
void Func_02007f0c();
void Func_02007f24();
void Func_02007f24_a();
void Func_02007f2e();
void Func_02007f3e();
void Func_02007f42();
void Func_02007f5c();
void Func_02007f60();
void Func_02007f70();
void Func_02007f82();
void Func_02007f8e();
void Func_02007fa8();
void Func_02007fc2();
void Func_02007fc4();
void Func_02007fce();
void Func_02007fce_a();
void Func_02007fd0();
void Func_02007fd0_a();
void Func_02007fd6();
void Func_02007fda();
void Func_02007fe4();
void Func_02008010();
void Func_02008024();
void Func_0200802c();
void Func_0200803c();
void Func_02008044();
void Func_02008054();
void Func_0200805e();
void Func_02008070();
void Func_02008096();
void Func_020080a6();
void Func_020080ca();
void Func_020080ca_a();
void Func_020080ee();
s32 Func_020080f8();
s32 Func_020080f8_a();
void Func_02008126();
void Func_0200813e();
void Func_0200813e_a();
void Func_0200813e_b();
void Func_02008140();
void Func_02008150();
void Func_02008174();
void Func_02008186();
s32 Func_02008198();
void Func_0200819c();
void Func_020081a2();
void Func_020081a8();
void Func_020081b2();
void Func_020081d0();
void Func_020081fc();
void Func_02008218();
void Func_0200821e();
void Func_0200822c();
void Func_0200822e();
void Func_02008236();
void Func_02008238();
void Func_0200823e();
void Func_02008256();
void Func_02008260();
void Func_0200826a();
void Func_0200826e();
void Func_02008280();
void Func_020082a2();
void Func_020082ac();
void Func_020082b6();
void Func_020082b6_a();
void Func_020082ba();
void Func_020082c0();
void Func_020082dc();
void Func_020082e6();
void Func_020082f0();
void Func_02008300();
void Func_02008302();
void Func_02008306();
void Func_0200831c();
void Func_02008366();
void Func_0200837a();
void Func_02008384();
void Func_020083aa();
void Func_020083ba();
void Func_020083d8();
void Func_020083e4();
void Func_020083f4();
void Func_020083fa();
void Func_02008416();
void Func_0200841a();
void Func_0200842a();
void Func_02008454();
void Func_02008464();
void Func_02008476();
void Func_0200847e();
void Func_02008574();
void Func_02008580();
s32 Func_0200740e();
void Func_02007438();
void Func_02007ea2();
void Func_02007ec2();
void Func_02007f02();
void Func_02007f1c();
void Func_02007fbe();
void Func_02007fc4_a();
void Func_02007fdc();
void Func_02007ffe();
void Func_02008010_a();
void Func_02008016();
void Func_02008020();
void Func_02008026();
void Func_02008044_a();
void Func_02008070_a();
void Func_02008084();
void Func_020080e4();
void Func_020080ee_a();
void Func_02008128();
void Func_020081b8();
void Func_020081de();
void Func_020081e4();
void Func_020081f2();
void Func_0200820a();
void Func_0200821e_a();
void Func_020082b6_b();
void Func_020082f0_a();
void Func_02008300_a();
void Func_02008326();
void Func_0200833a();
void Func_0200834a();
void Func_02008378();
void Func_02008382();
void Func_020083aa_a();
void Func_020083b0();
void Func_020083bc();
void Func_020083d6();
void Func_020083ea();
void Func_020083fa_a();
void Func_02008418();
void Func_02008422();
void Func_02008428();
void Func_02008430();
void Func_0200843a();
void Func_02008442();
void Func_02008454_a();
void Func_0200847c();
u8 *Func_02008492();
void Func_02008494();
void Func_0200849c();
void Func_020084a8();
void Func_020084b4();
void Func_020084b6();
void Func_020084bc();
void Func_020084d0();
void Func_020084dc();
void Func_020084e0();
void Func_020084e8();
void Func_020084ea();
void Func_020084f0();
void Func_02008516();
void Func_0200851e();
u8 *Func_02008526();
void Func_0200852a();
void Func_0200852c();
void Func_0200852c_a();
u8 *Func_02008532();
void Func_0200853a();
u8 *Func_0200853e();
void Func_0200854a();
void Func_02008552();
void Func_0200855a();
void Func_02008562();
void Func_02008592();
void Func_02008594();
void Func_02008596();
void Func_0200859e();
void Func_020085a2();
void Func_020085a2_a();
void Func_020085a8();
void Func_020085b0();
void Func_020085c2();
void Func_020085cc();
void Func_020085d2();
void Func_020085d2_a();
void Func_020085e6();
void Func_020085ea();
void Func_020085f8();
void Func_020085fc();
void Func_02008602();
void Func_0200860e();
void Func_0200860e_a();
void Func_02008610();
void Func_02008614();
void Func_02008618();
void Func_0200861e();
void Func_02008620();
void Func_02008622();
void Func_0200862c();
void Func_02008632();
void Func_02008648();
void Func_0200864e();
void Func_02008660();
void Func_0200866a();
void Func_02008670();
void Func_02008674();
void Func_0200867e();
void Func_02008684();
void Func_0200868c();
void Func_0200869a();
void Func_0200869a_a();
void Func_020086aa();
void Func_020086b0();
void Func_020086b2();
void Func_020086b8();
void Func_020086ba();
void Func_020086ba_a();
void Func_020086c8();
void Func_020086d6();
void Func_020086dc();
void Func_020086e4();
void Func_020086e4_a();
void Func_020086fe();
void Func_02008702();
void Func_0200870c();
void Func_02008728();
void Func_02008728_a();
void Func_02008734();
void Func_02008738();
void Func_0200873a();
void Func_0200873c();
void Func_02008744();
void Func_0200874a();
void Func_02008750();
void Func_02008762();
void Func_0200876e();
void Func_0200879a();
void Func_0200879c();
void Func_020087a4();
void Func_020087a6();
void Func_020087a8();
void Func_020087d4();
void Func_020087e4();
void Func_020087ea();
s32 Func_020087f2();
void Func_020087f4();
void Func_020087f6();
void Func_020087f6_a();
void Func_020087f8();
void Func_020087f8_a();
void Func_020087fe();
void Func_020087fe_a();
void Func_0200880a();
void Func_0200880a_a();
void Func_0200880c();
void Func_0200880c_a();
void Func_02008814();
void Func_02008814_a();
void Func_02008830();
void Func_0200883a();
void Func_0200884a();
void Func_0200885a();
void Func_0200885c();
void Func_02008866();
void Func_0200886e();
void Func_02008870();
void Func_02008870_a();
void Func_02008884();
void Func_0200888c();
void Func_0200889c();
void Func_020088bc();
void Func_020088be();
void Func_020088c4();
void Func_020088cc();
void Func_020088ce();
void Func_020088dc();
void Func_020088ec();
void Func_020088f4();
void Func_020088fc();
void Func_020088fe();
void Func_0200890e();
void Func_02008910();
void Func_0200891c();
void Func_02008922();
void Func_02008922_a();
void Func_02008930();
void Func_02008936();
void Func_0200893e();
void Func_0200894a();
void Func_02008950();
void Func_0200896c();
void Func_0200896e();
void Func_02008978();
void Func_0200897e();
void Func_0200897e_a();
void Func_02008992();
s32 Func_020089a6();
void Func_020089b8();
void Func_020089be();
void Func_020089d4();
void Func_020089f6();
void Func_020089f8();
void Func_020089f8_a();
void Func_02008a12();
void Func_02008a12_a();
void Func_02008a18();
void Func_02008a1c();
void Func_02008a26();
void Func_02008a30();
void Func_02008a34();
void Func_02008a4a();
void Func_02008a5c();
void Func_02008a64();
void Func_02008a66();
void Func_02008a66_a();
void Func_02008a68();
void Func_02008a68_a();
void Func_02008a6c();
void Func_02008a70();
void Func_02008a76();
void Func_02008a9e();
void Func_02008aa2();
void Func_02008abe();
void Func_02008ac6();
void Func_02008ad6();
void Func_02008ad6_a();
void Func_02008ae2();
void Func_02008aee();
void Func_02008af0();
void Func_02008afc();
void Func_02008b16();
void Func_02008b30();
void Func_02008b34();
void Func_02008b40();
void Func_02008b4e();
void Func_02008b54();
void Func_02008b56();
void Func_02008b82();
void Func_02008b98();
void Func_02008ba6();
void Func_02008bac();
void Func_02008bb2();
void Func_02008bc0();
void Func_02008bcc();
void Func_02008bea();
void Func_02008c14();
void Func_02008c1e();
void Func_02008c20();
void Func_02008c28();
void Func_02008c34();
void Func_02008c36();
void Func_02008c44();
void Func_02008c56();
void Func_02008c6c();
void Func_02008c6c_a();
void Func_02008c74();
void Func_02008c7c();
void Func_02008c96();
void Func_02008c98();
void Func_02008cc4();
void Func_02008cc8();
void Func_02008cde();
void Func_02008cea();
void Func_02008cf4();
void Func_02008d26();
void Func_02008d26_a();
void Func_02008d3c();
void Func_02008d44();
void Func_02008d4e();
void Func_02008d58();
void Func_02008d6a();
void Func_02008d70();
void Func_02008d72();
void Func_02008d7e();
void Func_02008d82();
void Func_02008d88();
void Func_02008d92();
void Func_02008dd0();
void Func_02008dd8();
void Func_02008de4();
void Func_02008de6();
void Func_02008dee();
void Func_02008df6();
void Func_02008e04();
void Func_02008e10();
void Func_02008e14();
void Func_02008e1c();
s32 Func_02008e26();
void Func_02008e2e();
void Func_02008e3a();
void Func_02008e3e();
void Func_02008e4c();
void Func_02008e9c();
void Func_02008ea4();
void Func_02008ea6();
void Func_02008ea8();
void Func_02008eae();
void Func_02008ec0();
void Func_02008ec4();
void Func_02008ee6();
void Func_02008ee6_a();
void Func_02008eec();
void Func_02008efa();
void Func_02008f06();
void Func_02008f18();
void Func_02008f1e();
void Func_02008f22();
void Func_02008f22_a();
void Func_02008f28();
void Func_02008f36();
void Func_02008f4c();
void Func_02008f92();
void Func_02008f96();
void Func_02008fa0();
void Func_02008fa2();
void Func_02008fbe();
void Func_02008fc2();
void Func_02008fe6();
void Func_02008ff0();
void Func_02008ff2();
void Func_02008ff4();
void Func_02009006();
void Func_02009030();
void Func_0200903e();
s32 Func_0200905e();
void Func_02009082();
void Func_0200908c();
void Func_02009098();
void Func_020090b0();
void Func_020090bc();
void Func_020090ca();
void Func_020090e0();
void Func_020090ec();
void Func_020090ee();
void Func_020090f8();
s32 Func_020090fe();
s32 Func_020090fe_a();
void Func_02009108();
void Func_02009120();
void Func_02009126();
void Func_02009142();
void Func_02009158();
void Func_02009162();
void Func_0200917a();
void Func_02009184();
void Func_0200919c();
void Func_020091a6();
void Func_020091b6();
void Func_020091b6_a();
void Func_020091b8();
void Func_020091d8();
void Func_020091d8_a();
void Func_020091fa();
void Func_0200920a();
void Func_02009210();
void Func_0200921a();
void Func_02009224();
void Func_02009232();
void Func_0200923c();
void Func_0200923c_a();
void Func_02009250();
void Func_0200925c();
void Func_02009274();
s32 Func_02009278();
void Func_02009280();
void Func_02009284();
void Func_02009296();
s32 Func_0200929c();
void Func_020092a8();
void Func_020092c0();
void Func_020092ce();
void Func_020092ce_a();
void Func_020092d8();
void Func_020092ee();
void Func_020092f6();
void Func_020092f6_a();
void Func_020092fc();
void Func_0200930e();
void Func_0200931e();
void Func_02009328();
void Func_02009332();
void Func_0200933c();
void Func_02009346();
void Func_02009350();
void Func_0200935a();
void Func_02009364();
void Func_02006a5c();
void Func_02009874();
void Func_020098a8();
void Func_020098d0();
void Func_020098da();
void Func_020098e4();
void Func_02009920();
void Func_02009940();
void Func_02009946();
void Func_0200994c();
void Func_02009954();
void Func_0200995e();
void Func_02009960();
void Func_02009976();
void Func_02009982();
void Func_0200998a();
void Func_02009990();
void Func_02009994();
void Func_02009996();
void Func_0200999c();
void Func_020099aa();
void Func_020099ac();
void Func_020099ea();
void Func_020099f6();
void Func_02009a54();
void Func_02009b68();
u8 *Func_02007c8e_a(s32);
void Func_02007eb8(void);
u8 *Func_02007ca6(s32);
void Func_02007ce6(s32, s32, s32, s32);
void Func_02007ee4(s32);
void Func_02007cd2(s32);
s32 Func_02007cd8(s32);
u8 *Func_02009356(s32);
u8 *Func_02009370(s32, s32);
void Func_020093e0(s32);
void Func_02009398(s32, s32, u8 *);
void Func_02009396(s32);
void Func_02009418(s32, s32, s32);
void Func_02009356_a(s32);
void Func_02009418_a(s32, s32, s32);
void Func_0200942e(s32);
void Func_0200938c(s32);
void Func_02009538(s32, s32);
void Func_02009548(s32, s32);
void Func_02009564(void);
void Func_02009542(s32);
void Func_020092e8(s32);
s32 Func_02009062(s32, s32, s32);
s32 Func_02009030_a(s32, s32, s32);
s32 Func_0200903e_a(s32, s32, s32);
s32 Func_0200904c(s32, s32, s32);
void Func_0200958c(s32, s32, s32, s32);

/*
 * Update one actor's animation descriptor when its current state matches the
 * expected state. The helper is called by the 17-entry scene transition table
 * at 0x02002564.
 *
 * The owner starts with push {r5,r6,r7,lr} at 0x020026e4, returns through
 * pop {r5,r6,r7}/pop {r0}/bx r0 at 0x02002716-0x0200271a, and is immediately
 * followed by the callback owner at 0x0200271c. It has no trailing pool, so
 * the complete span is 56 bytes.
 */

/* Resolved engine calls: each pseudo symbol is the per-site call word the
 * overlay image holds (a word can serve two sites with different targets),
 * and the macro names the engine function the site reaches through the
 * overlay veneer and the main-image veneer island, keeping the site's own
 * calling form. Names without a repository binding are provisional.
 */

/* Word at +456 of the shared scene work record. */

/* Field at +456 of the shared scene work record, addressed through the
 * loader-fixed pointer at 0x03001ebc. */

/* Pair of ratio-like arguments shared by three setup calls below (each
 * applied to a different index: 0, 1, 2). */

/* Loader-relocated overlay calls: each symbol names the pre-relocation call
 * word the image holds. */

/* Call sites spelled through these wrappers pass their constants straight
 * into the argument registers; a direct call precomputes a costly constant
 * into a pseudo that the compiler then shares with later uses in the block.
 * A value-returning call also sets r0 last of its arguments. */
static __inline__ void Call1(void (*f)(), s32 a0)
{
    extern u8 Data_03001ebc[];

    f(a0);
}

static __inline__ void Call3(void (*f)(), s32 a0, s32 a1, s32 a2)
{
    extern u8 Data_03001ebc[];

    f(a0, a1, a2);
}

/* The scene step counter at 0x1d8 of the shared scene work record. */
static __inline__ void bump_step(s32 amount)
{
    extern u8 Data_03001ebc[];

    u8 *work = *(u8 **)Data_03001ebc;

    *(u16 *)(work + 0x1d8) = (u16)(*(u16 *)(work + 0x1d8) + amount);
}

/* Call sites spelled through these wrappers pass their constants straight
 * into the argument registers; a direct call precomputes a costly constant
 * into a pseudo that the compiler then shares with later uses in the block.
 * A value-returning call also sets r0 last of its arguments. */
static __inline__ s32 Value0(s32 (*f)())
{
    extern u8 Data_03001ebc[];

    return f();
}

static __inline__ void Call1_0200082c(void (*f)(), s32 a0)
{
    extern u8 Data_03001ebc[];

    f(a0);
}

static __inline__ s32 Value1(s32 (*f)(), s32 a0)
{
    extern u8 Data_03001ebc[];

    return f(a0);
}

static __inline__ s32 Value2(s32 (*f)(), s32 a0, s32 a1)
{
    extern u8 Data_03001ebc[];

    return f(a0, a1);
}

static __inline__ void Call1_0200091c(void (*f)(), s32 a0)
{
    extern u8 Data_03001ebc[];

    f(a0);
}

static __inline__ s32 Value3(s32 (*f)(), s32 a0, s32 a1, s32 a2)
{
    extern u8 Data_03001ebc[];

    return f(a0, a1, a2);
}

static __inline__ void Call1_020019a4(void (*f)(), s32 a0)
{
    extern u8 Data_03001ebc[];

    f(a0);
}

static __inline__ void Call0(void (*f)())
{
    extern u8 Data_03001ebc[];

    f();
}

static __inline__ void Call1_020019e4(void (*f)(), s32 a0)
{
    extern u8 Data_03001ebc[];

    f(a0);
}

static __inline__ void Call2(void (*f)(), s32 a0, s32 a1)
{
    extern u8 Data_03001ebc[];

    f(a0, a1);
}

static __inline__ s32 Value0_02001a4c(s32 (*f)())
{
    extern u8 Data_03001ebc[];

    void *Func_02005ae2();
    void *Func_020067d8();

    return f();
}

static __inline__ void Call1_02001a4c(void (*f)(), s32 a0)
{
    extern u8 Data_03001ebc[];

    void *Func_02005ae2();
    void *Func_020067d8();

    f(a0);
}

static __inline__ void Call6(void (*f)(), s32 a0, s32 a1, s32 a2, s32 a3, s32 a4, s32 a5)
{
    extern u8 Data_03001ebc[];

    void *Func_02005ae2();
    void *Func_020067d8();

    f(a0, a1, a2, a3, a4, a5);
}

/* Resolved engine calls: each pseudo symbol is the per-site call word the
 * overlay image holds (a word can serve two sites with different targets),
 * and the macro names the engine function the site reaches through the
 * overlay veneer and the main-image veneer island, keeping the site's own
 * calling form. Names without a repository binding are provisional.
 */

/* The "shown" half word at +100 of an actor record. */

/* Phase/status word at 0x1c0 of the shared scene work record. */

/* Loader-relocated overlay calls: each symbol names the pre-relocation call
 * word the image holds. */

/* Call sites spelled through these wrappers pass their constants straight
 * into the argument registers; a direct call precomputes a costly constant
 * into a pseudo that the compiler then shares with later uses in the block.
 * A value-returning call also sets r0 last of its arguments. */
static __inline__ void Call1_02001ba0(void (*f)(), s32 a0)
{
    extern u8 Data_03001ebc[];

    f(a0);
}

static __inline__ void Call6_02001ba0(void (*f)(), s32 a0, s32 a1, s32 a2, s32 a3, s32 a4, s32 a5)
{
    extern u8 Data_03001ebc[];

    f(a0, a1, a2, a3, a4, a5);
}

/* Call sites spelled through these wrappers pass their constants straight
 * into the argument registers; a direct call precomputes a costly constant
 * into a pseudo that the compiler then shares with later uses in the block.
 * A value-returning call also sets r0 last of its arguments. */
static __inline__ s32 Value1_02001e80(s32 (*f)(), s32 a0)
{
    extern u8 Data_03001ebc[];

    return f(a0);
}

static __inline__ void Call1_020036f8(void (*f)(), s32 a0)
{
    extern u8 Data_03001ebc[];

    f(a0);
}

/* The scene step counter at 0x1d8 of the shared scene work record. */
static __inline__ void bump_step_020036f8(s32 off, s32 amount)
{
    extern u8 Data_03001ebc[];

    u8 *work = *(u8 **)Data_03001ebc;
    u16 *slot = (u16 *)((s32)work + off);
    s32 next = *slot + amount;

    *slot = next;
}

static __inline__ void Call4(void (*f)(), s32 a0, s32 a1, s32 a2, s32 a3)
{
    extern u8 Data_03001ebc[];

    f(a0, a1, a2, a3);
}

/*
 * Head export stub at 0x02000030. The eight-byte owner includes its one pool
 * word at 0x02000034, which the load reads and which sits past the return.
 * The word is an address returned without being dereferenced, so this is a
 * getter for an in-image table; it names bytes only while this overlay is
 * resident at 0x02000000.
 */
u8 *SceneData_GetTableD8f8(void)
{
    extern u8 *Data_03001ebc;

    return (u8 *)0x0200d8f8;
}

/*
 * Head export stub at 0x02000038. The eight-byte owner includes its one pool
 * word at 0x0200003c, which the load reads and which sits past the return.
 * The word is an address returned without being dereferenced, so this is a
 * getter for an in-image table; it names bytes only while this overlay is
 * resident at 0x02000000.
 */
u8 *SceneData_GetTableDa60(void)
{
    extern u8 *Data_03001ebc;

    return (u8 *)0x0200da60;
}

/*
 * Head export stub at 0x02000040. The eight-byte owner includes its one pool
 * word at 0x02000044, which the load reads and which sits past the return.
 * The word is an address returned without being dereferenced, so this is a
 * getter for an in-image table; it names bytes only while this overlay is
 * resident at 0x02000000.
 */
u8 *SceneData_GetTableDa80(void)
{
    extern u8 *Data_03001ebc;

    return (u8 *)0x0200da80;
}

u8 *SceneData_SelectTableBySceneRange(void)
{
    extern u8 *Data_03001ebc;

    s16 *tbl = Data_02000240;
    u8 *ret;
    s32 v = tbl[225];
    s32 low = 15;

    if (v <= 17) {
        if (v >= low) {
            ret = Data_0200dcc8;
        } else {
            ret = Data_0200dab8;
        }
    } else {
        ret = Data_0200dab8;
    }
    Func_02004dbc(ret);
    return ret;
}

void ActorPresentation_SetSceneCellByAngle(void)
{
    extern u8 *Data_03001ebc;

    s32 x;
    s32 z;

    if (*(u16 *)(Func_02004e04(0) + 6) >= 0xa000
        && *(u16 *)(Func_02004e14(0) + 6) <= 0xe000) {
        Func_02004f32();
        x = 42;
        z = 85;
        Func_02004d9e(41, 85, 1, 1, x, z);
    } else if (*(u16 *)(Func_02004e3e(0) + 6) >= 0x2000
               && *(u16 *)(Func_02004e4e(0) + 6) <= 0x6000) {
        Func_02004f6c();
        x = 42;
        z = 85;
        Func_02004dd8(43, 85, 1, 1, x, z);
    }
}

void FieldScene_RunObjectTwentySixPositionCheck(void)
{
    extern u8 *Data_03001ebc;

    struct Object *obj;
    s32 x;
    s32 z;

    Func_02004e46();
    obj = Func_02004e84(26);
    if ((obj->x >> 20) == 42) {
        x = 41;
        z = 24;
        Func_02004e08(101, 24, 3, 4, x, z);
        Func_02004e46_b(0x859);
    }
    Func_02004e7a();
}

void FieldScene_RunActorNineteenAngleDialogue(void)
{
    extern u8 *Data_03001ebc;

    s32 v = *(u16 *)(Func_02004ebe(0) + 6);

    Func_02004e8c();
    if (v >= 0xa001 && v <= 0xdfff) {
        Func_02005066(4, 19);
    } else {
        if (Func_02004e76(0x855) == 0) {
            Func_02004f80((s32)&Value_00001280);
        } else {
            Func_02004f88(0x1370);
        }
        Func_02004fa0(19, 0);
    }
    Func_02004ecc();
}

void FieldScene_RunActorTwentyAngleDialogue(void)
{
    extern u8 *Data_03001ebc;

    s32 v = *(u16 *)(Func_02004f1e(0) + 6);

    Func_02004eec();
    if (v >= 0xa001 && v <= 0xdfff) {
        Func_020050c6(5, 20);
    } else {
        if (Func_02004ed6(0x855) == 0) {
            Func_02004fe0(0x1282);
        } else {
            Func_02004fe8(0x1372);
        }
        Func_02005000(20, 0);
    }
    Func_02004f2c();
}

void FieldScene_RunActorTwentyThreeAngleDialogue(void)
{
    extern u8 *Data_03001ebc;

    s32 v = *(u16 *)(Func_02004f7e(0) + 6);

    Func_02004f4c();
    if (v >= 0xa001 && v <= 0xdfff) {
        Func_0200512e(1, 23);
    } else {
        if (Func_02004f36(0x855) == 0) {
            Func_02005040(0x128d);
        } else {
            Func_02005048(0x137b);
        }
        Func_02005060(23, 0);
    }
    Func_02004f8c();
}

void FieldScene_RunActorEighteenConditionalScene(void)
{
    Func_02004fa4();
    if (Func_02004f98() == 0) {
        Func_0200504c(18, 4);
        Func_02004fb2(20);
        Func_02005090(0x1384);
        Func_020050a8(18, 0);
    } else {
        Func_0200514a_a(231, 3);
        Func_02005002(231, 0);
    }
    Func_02004fe6();
}

void SceneDialogue_ShowLine12BB(void)
{
    Func_0200513c();
    Func_020050c2(0x12bb);
    Func_020050da(1, 0);
}

void SceneState_SetFlags92bAnd94b(void)
{
    Func_02005008();
    Func_02004fc0(0x92b, 1);
    Func_02004fc8(0x94b, 1);
    Func_02005024();
}

void SceneState_SetFlags929And949(void)
{
    Func_02005030();
    Func_02004fe8_a(0x929, 1);
    Func_02004ff0(0x949, 1);
    Func_0200504c_a();
}

u8 *SceneData_SelectTableE1fcBySceneRangeOrFlag855(void)
{
    s16 *tbl = Data_02000240;
    s32 v = tbl[225];
    s32 lo = 15;

    if (v <= 17) {
        if (v >= lo) {
            return Data_0200e1fc;
        }
    }
    if (Func_02005042(0x855) != 0) {
        return Data_0200e250;
    }
    return Data_0200de30;
}

void ActorPresentation_RunActorModeOneThenZeroWithStep(s32 x)
{
    Func_02005130(x, 1);
    Func_0200498a(x, 0, 2);
    Func_02005192(x, 0);
}

void SceneState_RunGuardedActorStep(s32 x)
{
    extern u8 *Data_03001ebc;

    u8 *flag = (u8 *)Func_020050fe() + 91;
    s32 zero = 0;

    *flag = 1;
    Func_020050d6();
    Func_0200516e(x, 1);
    Func_020050dc(2);
    Func_020051cc(x, 0);
    Func_020050f8();
    *flag = zero;
}

void SceneDialogue_PromptAndCountSkip(s32 x)
{
    extern u8 *Data_03001ebc;

    Func_020049ea(x, 0, 2);
    Func_020051ea(x, 0);
    if (Func_0200514a(0, 0) != 0) {
        *(u16 *)(Data_03001ebc + 472) += 1;
    }
    Func_02005216(x, 0);
}

void SceneDialogue_RunActorElevenDialogue(void)
{
    Func_02005148();
    Func_0200521e(0x1247);
    Func_020051e6(11, 1);
    Func_020007d4(11);
    Func_02005168();
}

void FieldScene_RunScene383_02000428(void)
{
    extern u8 Data_03001ebc[];

    u32 i;
    s32 record;

    Func_02005170();
    Call1(Func_02005246, 0x1253);
    Func_020007f4(15);
    Call3(Func_02005270, 15, 0x8000, 0);
    Func_02005194();
}

void SceneState_BranchOnSlotZeroFacingAndFlag855(void)
{
    s32 value = *(u16 *)(Func_020051d6(0) + 6);

    Func_020051a4();
    if (value >= 0xa001 && value <= 0xdfff) {
        Func_0200537e(6, 21);
    } else if (Func_0200518e(0x855) == 0) {
        Func_02005298(0x1284);
        Func_02000846(21);
    } else {
        Func_020052a6(0x1374);
        Func_020052be(21, 0);
    }
    Func_020051ea_a();
}

void SceneDialogue_RunActor9FlaggedLine(void)
{
    Func_02005204();
    if (Func_020051da(0x855) == 0) {
        Func_020052e4(0x1243);
    } else {
        Func_020052ec(0x1353);
    }
    Func_02000832(9);
    Func_0200522e();
}

void SceneDialogue_RunActorTwelveFlaggedDialogue(void)
{
    Func_0200523c();
    if (Func_02005212(0x855) != 0) {
        Func_0200531c(0x135c);
    } else {
        Func_02005324(0x124c);
    }
    Func_0200086a(12);
    Func_02005266();
}

void FieldScene_RunFlag856DialogueBranch(void)
{
    extern u8 Data_03001ebc[];

    s32 g;
    g = 0x851;
    Func_02005274();
    if (Func_0200524a(0x856) != 0) {
        if (Func_02005254(g) == 0) {
            Func_0200535e(0x1276);
            Func_020008a4(16);
            Func_02005292(10);
            Func_02004b9c(16, 3, 20);
            Func_02005282(g);
        } else {
            Func_02005382(0x1278);
        }
    } else {
        Func_0200538a(0x1250);
    }
    Func_020008d0(16);
    Func_020052cc();
}

void SceneDialogue_ShowLine128E(void)
{
    Func_020052e4_a();
    Func_020053ba(0x128e);
    Func_02000900(18);
    Func_020052fc();
}

void SceneActor_StepActor24AnimationByFacing(void)
{
    T *p;
    s16 *q;
    s32 v;
    s32 n;

    p = Func_0200533e(24);
    Func_0200530c();
    Func_020053cc(24, 2);
    Func_020053ea(0x12ac);
    Func_02005402(24, 0);
    Func_02005370(24, 0x40000, 0x20000);
    if ((u32)((p->unk6 & 0xf000) - 0x5000) <= 0x6000) {
        q = (s16 *)((u8 *)p + 100);
        v = *q;
        if (v <= 2) {
            Func_020053a2(24, Data_0200e4a8[v]);
            *(u16 *)q = *(u16 *)q + 1;
            goto clamp;
        }
    } else {
        q = (s16 *)((u8 *)p + 100);
        v = *q;
        if (v > 2) {
            Func_020053be(24, Data_0200e4a8[v]);
            *(u16 *)q = *(u16 *)q + 1;
            goto clamp;
        }
    }
    Func_020053de(24, Data_0200e4c0[v]);
    n = *(u16 *)q - 1;
    *(u16 *)q = n;
clamp:
    if (*q > 5) {
        n = 0;
        *(u16 *)q = n;
    }
    if (*q < 0) {
        n = 5;
        *(u16 *)q = n;
    }
    Func_02005418(24);
    Func_020053d4();
}

void SceneDialogue_RunActor10Line(void)
{
    Func_02005514();
    Func_020055ea(0x1356);
    Func_02000b98(10);
    Func_0200552c();
}

void SceneDialogue_RunActor11Line(void)
{
    Func_02005534();
    Func_0200560a(0x1359);
    Func_02000b50(11);
    Func_0200554c();
}

void SceneDialogue_RunActor14Line(void)
{
    Func_02005554();
    Func_0200562a(0x1368);
    Func_02000bd8(14);
    Func_0200556c();
}

void FieldScene_RunScene383SequenceC(void)
{
    extern u8 Data_03001ebc[];

    u32 i;
    s32 record;

    Func_02005574();
    if (Value1(Func_0200554a, 0x857) == 0) {
        Func_02005656((s32)Data_00001360);
        Func_02004e3e_a(16, 20);
        Func_02004e90(16, 3, 20);
        Func_02004e50(16, 30);
        Func_02005692(16, 0, 0);
        Func_020055a8(30);
        Func_02005670(16, 2);
        Func_020055b6(30);
        Func_02004ea8(0, 16, 20);
        Func_02004eca(16, 3, 20);
        bump_step(1);
        if (Value0(Func_020055d6) == 0) {
            Func_020056c0(((s32)Data_00001360 + 3));
            Func_02004ea8_a(16, 20);
            Func_02005604();
            goto L_02000906;
        }
        Call1_0200082c(Func_020055dc, 0x857);
        Func_02005634(189, 0);
    }
    Call1_0200082c(Func_020056e2, 0x1364);
    Func_020056f2(16, 0);
    Func_02005618(20);
    if (Value2(Func_02005658, 0, 0) != 0) {
        bump_step(1);
    }
    Func_02005724(16, 0);
    Func_02005650();
    L_02000906:;
}

void FieldScene_RunScene383_0200091c(void)
{
    extern u8 Data_03001ebc[];

    u32 i;
    s32 record;

    Func_02005664();
    Func_02004f4e(18, 0, 2);
    if (Value1(Func_02005644, 0x85b) == 0) {
        Call1_0200091c(Func_0200574e, 0x137c);
        Func_0200575e(18, 0);
    } else {
        Call1_0200091c(Func_0200575e_a, 0x1385);
        Func_0200576e(18, 0);
    }
    if (Value2(Func_020056ce, 0, 0) == 0) {
        Func_020056a0(20);
        Func_02005790(18, 0);
        Func_020056ae(20);
        Func_02005776(18, 2);
        Func_020056bc(20);
        if (Value0(Func_020056b8) == 0) {
            Func_0200576c(18, 4);
            Func_020056d2(20);
            Call1_0200091c(Func_020057b0, 0x1384);
            Func_020057c8(18, 0);
            goto L_020009ec;
        }
        Func_0200586a(231, 3);
        Func_02005722(231, 0);
        Call1_0200091c(Func_020056d8, 0x85b);
    } else {
        bump_step(1);
        Func_02005710(20);
        Func_020057b8(18, 3);
        Func_0200571e(20);
        Func_0200580e(18, 0);
    }
    L_020009ec:;
    Call3(Func_02005822, 18, 0x4000, 0);
    Func_02005746();
}

void SceneDialogue_RunActorNineFlaggedDialogue(void)
{
    Func_0200575c();
    if (Func_02005732(0x855) == 0) {
        Func_0200583c(0x1245);
    } else {
        Func_02005844(0x1355);
    }
    Func_02000dae(9);
    Func_02005786();
}

void SceneDialogue_RunActorElevenFlaggedDialogue(void)
{
    Func_02005794();
    if (Func_0200576a(0x855) == 0) {
        Func_02005874(0x124b);
    } else {
        Func_0200587c(0x135b);
    }
    Func_02000de6(11);
    Func_020057be();
}

void SceneDialogue_ShowLine124EOr135E(void)
{
    Func_020057cc();
    if (Func_020057a2(0x855) == 0) {
        Func_020058ac(0x124e);
    } else {
        Func_020058b4(0x135e);
    }
    Func_02000e1e(12);
    Func_020057f6();
}

void SceneDialogue_RunActor16FlaggedLine(void)
{
    Func_02005804();
    if (Func_020057da(0x855) == 0) {
        Func_020058e4(0x127c);
    } else {
        Func_020058ec(0x136c);
    }
    Func_02000e56(16);
    Func_0200582e();
}

void SceneDialogue_RunActorEighteenBranchedDialogue(void)
{
    Func_0200583c_a();
    if (Func_02005812(0x855) == 0) {
        Func_0200591c(0x1294);
    } else if (Func_02005824(0x85b) == 0) {
        Func_0200592e(0x1382);
    } else {
        Func_02005936(0x1cf4);
    }
    Func_02000ea0(18);
    Func_02005878();
}

/* Configures actors 0, 1, 2 (position, movement, and animation timing), then
 * branches on whether actor 0 is already set up: one path sets up actors 0-2
 * with poses and movement, the other only advances actor 2's animation. Both
 * paths converge to check actor 0's record and, depending on that check,
 * configure either actor 2 or actor 1 from it before the scene finishes. */
void FieldScene_RunSetupSequence(void)
{
    extern u8 Data_03001ebc[];

    u8 *Func_02005ae2();
    u8 *Func_020067d8();

    u8 *record;

    BattleRuntime_Reset_1();
    ObjectMotion_SetSpeedParameters_1(0, 0xcccc, 0x6666);
    ObjectMotion_SetSpeedParameters_2(1, 0xcccc, 0x6666);
    ObjectMotion_SetSpeedParameters_3(2, 0xcccc, 0x6666);
    Audio_PlayCue_1(19);
    ObjectMotion_SetPositionAndReset_1(0, 0x180, 0x198);
    ObjectMotion_ArmCallback_1(0, 0xc000, 0);
    ObjectMotion_SetHorizontalPositionWithTerrain_1(1, 0x1800000, 0x1980000);
    ObjectMotion_SetPositionAndReset_2(1, 0x170, 0x198);
    ObjectMotion_ArmCallback_2(1, 0xd000, 20);
    if (GameFlag_IsSet_1(0x850) != 0) {
    } else {
        GameFlag_Set_1(0x850);
        Func_02005228(2, 0);
        BattleRuntime_WaitIfModeZero_1(40);
        Func_0200525e();
        SceneWork_SetStepValue_1(0x1256);
        Audio_PlayCue_2(60);
        BattleRuntime_WaitIfModeZero_2(30);
        Func_02005232(2, 3, 30);
        Func_020051f2(2, 30);
        ObjectMotion_SetVariantCallback_1(0, 1);
        ObjectMotion_SetVariantCallbackAndInvokeObject_1(1, 1);
        BattleRuntime_WaitIfModeZero_3(20);
        Func_02005274_a(2, 0);
        BattleRuntime_WaitIfModeZero_4(40);
        Func_020052aa();
        Func_02005222(2, 30);
        Func_0200525c(0, 1, 50);
        ObjectMotion_ArmCallback_3(0, 0xc000, 0);
        ObjectMotion_ArmCallback_4(1, 0xd000, 0);
        BattleRuntime_WaitIfModeZero_5(20);
        Func_020052b6(2, 0);
        BattleRuntime_WaitIfModeZero_6(40);
        Func_020052ec_a();
        ObjectMotion_SetVariantCallbackAndInvokeObject_2(2, 1);
        Func_0200526c(2, 50);
        Object_SetModeById_1(0, 3);
        Func_020052c6(1, 3, 20);
        Func_020052d0(2, 3, 20);
        Func_02005290(2, 40);
        ObjectMotion_SetVariantCallbackAndInvokeObject_3(2, 1);
        BattleRuntime_WaitIfModeZero_7(30);
        Call3((void (*)())Func_02005ae2_a, 2, 0xc000, 0);
        BattleRuntime_WaitIfModeZero_8(30);
        ObjectMotion_SetPositionAndReset_3(2, 0x178, 0x178);
        BattleRuntime_WaitIfModeZero_9(40);
        Func_020052fe(0, 1, 50);
        ObjectMotion_ArmCallback_5(0, 0xc000, 0);
        ObjectMotion_ArmCallback_6(1, 0xd000, 0);
        ObjectMotion_SetVariantCallbackAndInvokeObject_4(2, 1);
        BattleRuntime_WaitIfModeZero_10(50);
        Func_02005346(2, 3, 30);
        ObjectMotion_ArmCallback_7(2, 0x4000, 0);
        BattleRuntime_WaitIfModeZero_11(10);
        BattleEvent_RunActionAndWait_1(2, 0);
        ObjectMotion_SetSpeedParameters_4(2, 0);
        goto L_join_setup_paths;
    }
    Audio_PlayCue_3(60);
    SceneWork_SetStepValue_2(0x125d);
    ObjectMotion_SetSpeedParameters_5(2, 0);
    L_join_setup_paths:;
    if (UiWork_WaitThenFinalizeCapacity_1(0, 0) == 0) {
        Func_02001b36();
        GameFlag_Set_2(0x856);
        Object_SetModeById_2(2, 2);
        record = Scene_GetRecord_1(0);
        if (record != 0) {
            /* Read the two s16 fields at +10 and +18 of the record. */
            ObjectMotion_ResetAndSetPosition_1(2, *(s16 *)(record + 10), *(s16 *)(record + 18));
        }
        ObjectMotion_CommitCurrentPositionAndActivate_1(2);
        ObjectMotion_SetHorizontalPositionWithTerrain_2(2, 0, 0);
    } else {
        BattleEvent_RunActionAndWait_2(2, 0);
    }
    Object_SetModeById_3(1, 2);
    record = Scene_GetRecord_2(0);
    if (record != 0) {
        /* Read the two s16 fields at +10 and +18 of the record. */
        ObjectMotion_ResetAndSetPosition_2(1, *(s16 *)(record + 10), *(s16 *)(record + 18));
    }
    ObjectMotion_CommitCurrentPositionAndActivate_2(1);
    ObjectMotion_SetHorizontalPositionWithTerrain_3(1, 0, 0);
    Audio_PlayCueForPartyMember_1();
    BattleRuntime_ScheduleShoulderButtonModeUpdate_1();
}

void SceneState_SetValue123Mode11(void)
{
    Func_020068ba(123);
    Func_02006808(11);
}

/* Sets up the opening sequence: two calls with fixed argument pairs, a
 * write to the scene work record, and two more calls with fixed args. */
void FieldScene_RunOpeningSequenceHead(void)
{
    extern u8 Data_03001ebc[];

    ObjectMotion_SetSpeedParameters_1_020019a4(0, 32768, 16384); /* object_id 0, speed_limit 32768, acceleration 16384 */
    ObjectMotion_ResetAndSetPositionInMode2_1(0, 728, 408); /* object_id 0, x 728, z 408 */
    SCENE_WORD_1C8 = 16;
    Audio_PlayCue_1_020019a4(123);
    Call1_020019a4(Func_02006846, 15); /* main:0808a248 */
}

/* Runs the second part of an opening sequence: two parameterised calls, then
 * (only if a flag read via Value_00000854 is clear) a short block of setup
 * calls, then unconditionally sets a scene work field and two more calls. */
void FieldScene_RunOpeningSequenceSecond(void)
{
    extern u8 Data_03001ebc[];

    ObjectMotion_SetSpeedParameters_1_020019e4(0, 32768, 16384); /* object 0, speed_limit 32768, acceleration 16384 */
    ObjectMotion_SetPositionAndReset_1_020019e4(0, 744, 408); /* object 0, x 744, z 408 */
    if (GameFlag_IsSet_1_020019e4((s32)&Value_00000854) == 0) {
        BattleRuntime_Reset_1_020019e4();
        SceneWork_SetStepValue_1_020019e4((s32)&Value_000012c3);
        BattleEvent_RunActionAndWait_1_020019e4(8, 0); /* action 8 */
        BattleRuntime_ScheduleShoulderButtonModeUpdate_1_020019e4();
    }
    SCENE_WORK_FIELD_456 = 16;
    Audio_PlayCue_1_020019e4(123);
    Call1_020019e4(Func_020068a6, 14); /* main:0808a248 */
}

/* Third scene step: sets up actors 24 and 25 (fetching each one's record),
 * runs a shared series of configuration calls touching actors 0-2, 10, 14,
 * 20, 24 and 25, then marks the two fetched records with a byte flag. */
void FieldScene_RunOpeningSequenceThird(void)
{
    extern u8 Data_03001ebc[];

    void *Func_02005ae2();
    void *Func_020067d8();

    void *actor24;
    void *actor25;

    actor24 = Scene_GetRecord_1_02001a4c(24);
    actor25 = Scene_GetRecord_2_02001a4c(25);
    BattleRuntime_Reset_1_02001a4c();
    ObjectMotion_SetSpeedParameters_1_02001a4c(0, RATIO_HI, RATIO_LO);
    ObjectMotion_SetSpeedParameters_2_02001a4c(1, RATIO_HI, RATIO_LO);
    ObjectMotion_SetSpeedParameters_3_02001a4c(2, RATIO_HI, RATIO_LO);
    ObjectMotion_SetPositionAndReset_1_02001a4c(0, 232, 696);
    ObjectMotion_SetPositionAndReset_2_02001a4c(0, 200, 696);
    BattleRuntime_WaitIfModeZero_1_02001a4c(10);
    BattleEffect_SpawnLinkedResourceObject_1(25, 256, 0);
    BattleEffect_SpawnLinkedResourceObject_2(24, 256, 0);
    BattleRuntime_WaitIfModeZero_2_02001a4c(60);
    Call3(Func_020060d2, 25, 0, 10);
    ObjectMotion_SetVariantCallbackAndInvokeObject_1_02001a4c(24, 2);
    BattleRuntime_WaitIfModeZero_3_02001a4c(20);
    SceneWork_SetStepValue_1_02001a4c(4758);
    Call2(Func_020060d6, 24, 20);
    BattleRuntime_WaitIfModeZero_4_02001a4c(25, 258); /* main:0808a1f0 */
    BattleRuntime_WaitIfModeZero_5_02001a4c(60);
    Call2(Func_020060ee, 25, 20);
    ObjectMotion_SetVariantCallbackAndInvokeObject_2_02001a4c(24, 1);
    Call2(Func_020060fe, 24, 30);
    ObjectMotion_SetSpeedParameters_4_02001a4c(24, 262144, 131072);
    ObjectMotion_SetSpeedParameters_5_02001a4c(25, 229376, 114688);
    ObjectMotion_EnableActionAndSetCallback_1(25, 33609776);
    ObjectMotion_EnableActionAndSetCallback_2(24, 33609056);
    Object_LookupAndStep_1(24);
    Call6(Func_02006834, 14, 45, 3, 1, 14, 44); /* main:080091c0 */
    GameFlag_Set_1_02001a4c(2130);
    GameFlag_Set_2_02001a4c(768);
    Call2(Func_020067dc, 33598369, 3200); /* main:080000d0 */
    /* Byte flag at +100 of each record: 1 for actor 24's, 3 for actor 25's. */
    ((struct SceneRecord *)actor24)->value100 = 1;
    ((struct SceneRecord *)actor25)->value100 = 3;
    BattleRuntime_ScheduleShoulderButtonModeUpdate_1_02001a4c();
}

/* Sets up actors 24 and 25 (pose picked from a per-actor threshold field at
 * +100), positions and moves actors 0-2 and 24-25, and advances the shared
 * scene step counter partway through. */
void FieldScene_RunScene383SequenceB(void)
{
    extern u8 Data_03001ebc[];

    s32 actor24;
    s32 actor25;
    s32 record_00001299;

    actor24 = Scene_GetRecord_1_02001ba0(24);
    actor25 = Scene_GetRecord_2_02001ba0(25);
    BattleRuntime_Reset_1_02001ba0();
    Call1_02001ba0(Func_02006838, 0x200aba1);
    GameFlag_Clear_1(0x300);
    if (*(s16 *)(actor24 + 100) <= 3) {
        ObjectMotion_EnableActionAndSetCallback_1_02001ba0(24, 0x200d678);
    } else {
        ObjectMotion_EnableActionAndSetCallback_2_02001ba0(24, 0x200d650);
    }
    if (*(s16 *)(actor25 + 100) <= 2) {
        ObjectMotion_EnableActionAndSetCallback_3(25, 0x200d768);
    } else {
        ObjectMotion_EnableActionAndSetCallback_4(25, 0x200d650);
    }
    ObjectMotion_SetSpeedParameters_1_02001ba0(0, 0xcccc, 0x6666);
    ObjectMotion_SetSpeedParameters_2_02001ba0(1, 0xcccc, 0x6666);
    ObjectMotion_SetSpeedParameters_3_02001ba0(2, 0xcccc, 0x6666);
    ObjectMotion_SetPositionAndReset_1_02001ba0(0, 248, 0x2d8);
    ObjectMotion_SetHorizontalPositionWithTerrain_1_02001ba0(2, 0xf80000, 0x2d80000);
    ObjectMotion_SetHorizontalPositionWithTerrain_2_02001ba0(1, 0xf80000, 0x2d80000);
    ObjectMotion_ResetAndSetPositionInMode2_1_02001ba0(2, 0x108, 0x2e8);
    ObjectMotion_SetPositionAndReset_2_02001ba0(1, 232, 0x2e8);
    ObjectMotion_CommitCurrentPositionAndActivate_1_02001ba0(2);
    ObjectMotion_ArmCallback_1_02001ba0(0, 0x4000, 0);
    ObjectMotion_SetAngleToward_1(1, 0, 0);
    Func_02006290(2, 0, 30);
    ObjectMotion_SetVariantCallbackAndInvokeObject_1_02001ba0(2, 1);
    record_00001299 = (s32)Data_00001299;
    SceneWork_SetStepValue_1_02001ba0(record_00001299);
    BattleEvent_RunActionAndWait_1_02001ba0(2, 0);
    ObjectMotion_SetAngleToward_2(0, 2, 0);
    Func_020062bc(1, 2, 20);
    Object_SetModeById_1_02001ba0(0, 3);
    Func_020062fe(1, 3, 20);
    Func_020062f0(0, 1, 10);
    ObjectMotion_SetSpeedParameters_4_02001ba0(1, 0);
    if (UiWork_WaitThenFinalizeCapacity_1_02001ba0(0, 0) != 0) {
        bump_step(1);
    }
    Func_020062ec(1, 30);
    SceneWork_SetStepValue_2_02001ba0((record_00001299 + 4));
    ObjectMotion_SetAngleToward_3(0, 2, 0);
    Func_0200631e(1, 2, 50);
    BattleEffect_SpawnLinkedResourceObject_1_02001ba0(2, 0x100, 0);
    BattleRuntime_WaitIfModeZero_1_02001ba0(60);
    Func_02006352(0, 1, 50);
    ObjectMotion_SetAngleToward_4(0, 2, 0);
    Func_0200634e(1, 2, 30);
    Func_02006388(2, 3, 10);
    Func_02006348(2, 20);
    BattleEffect_SpawnLinkedResourceObject_2_02001ba0(0, 0x102, 0);
    BattleEffect_SpawnLinkedResourceObject_3(1, 0x102, 0);
    BattleRuntime_WaitIfModeZero_2_02001ba0(60);
    Func_020063b8(2, 3, 20);
    Func_02006378(2, 30);
    SceneWork_SetStepValue_3(0x129f);
    BattleEvent_RunActionAndWait_2_02001ba0(1, 0);
    ObjectMotion_ArmCallback_2_02001ba0(0, 0x4000, 0);
    BattleRuntime_WaitIfModeZero_3_02001ba0(40);
    Object_SetModeById_2_02001ba0(0, 3);
    Object_SetModeById_3_02001ba0(1, 3);
    Func_020063fa(2, 3, 50);
    ObjectMotion_ResetAndSetPositionInMode2_2(2, 248, 0x2d8);
    ObjectMotion_SetPositionAndReset_3_02001ba0(1, 248, 0x2d8);
    ObjectMotion_SetHorizontalPositionWithTerrain_3_02001ba0(1, 0, 0);
    ObjectMotion_SetHorizontalPositionWithTerrain_4(2, 0, 0);
    ObjectMotion_SetHorizontalPositionWithTerrain_5(24, 0x680000, 0x2b80000);
    ObjectMotion_SetHorizontalPositionWithTerrain_6(25, 0x780000, 0x2b80000);
    ObjectMotion_ArmCallback_3_02001ba0(24, 0, 0);
    ObjectMotion_ArmCallback_4_02001ba0(25, 0x8000, 0);
    Call6_02001ba0(Func_02006b14, 14, 50, 3, 1, 14, 44);
    BattleRuntime_ScheduleShoulderButtonModeUpdate_1_02001ba0();
}

void FieldScene_RunSteps107And250(void)
{
    extern u8 Data_03001ebc[];

    Func_02006b86(0x107);
    Func_02006b8e(0x250);
    Func_020069a2();
}

/* Configures actor records 24 and 25 (their +100 "shown" half words end up
 * set to 1 and 3 respectively) and finishes with a six-argument call that
 * repeats actor id 14. */
void FieldScene_ConfigurePairedActors(void)
{
    extern u8 Data_03001ebc[];

    u32 i;
    u8 *record;

    BattleRuntime_WaitIfModeZero_1_02001e80(30);
    ObjectMotion_SetVariantCallbackAndInvokeObject_1_02001e80(24, 1);
    BattleRuntime_WaitIfModeZero_2_02001e80(20);
    SceneWork_SetStepValue_1_02001e80((s32)Data_000012a0);
    Func_02006498(24, 20);
    ObjectMotion_ArmCallback_1_02001e80(25, 0, 20);
    BattleRuntime_WaitIfModeZero_3_02001e80(25, 0x102);
    ObjectMotion_SetVariantCallbackAndInvokeObject_2_02001ba0(25, 2);
    Func_020064bc(25, 20);
    ObjectMotion_CallThenWaitForAnimationChange_1(24, 4);
    BattleRuntime_WaitIfModeZero_4_02001ba0(20);
    Func_020064d2(24, 20);
    ObjectMotion_SetSpeedParameters_1_02001e80(24, 0x40000, 0x20000);
    ObjectMotion_SetSpeedParameters_2_02001e80(25, 0x38000, 0x1c000);
    ObjectMotion_EnableActionAndSetCallback_1_02001e80(25, 0x200d830);
    ObjectMotion_EnableActionAndSetCallback_2_02001e80(24, 0x200d560);
    Object_LookupAndStep_1_02001ba0(24);
    {
        u8 *record = Scene_GetRecord_1_02001e80(24);
        s32 shown = 1;

        *(u16 *)(record + ACTOR_SHOWN_OFFSET) = shown;
    }
    {
        u8 *record = Scene_GetRecord_2_02001e80(25);
        s32 shown = 3;

        *(u16 *)(record + ACTOR_SHOWN_OFFSET) = shown;
    }
    Call6_02001ba0(Func_02006c20, 14, 48, 4, 1, 14, 44);
}

void SceneActor_UpdateAnimationOnStateMatch(s32 actor, s32 expected, s32 next, const u8 *desc)
{
    u8 *rec = Func_02007472(actor);

    if (*(s16 *)(rec + 100) == expected) {
        Func_02007496(actor, desc);
        *(u16 *)(rec + 100) = (u16)next;
    }
}

void SceneState_SetFlagByActorPosition(void)
{
    u8 *p0 = Func_0200749e(0);
    s32 rx = *(s32 *)(p0 + 8);
    s32 x;
    s32 z;

    u8 *p1 = Func_020074a6(0);
    x = rx >> 20;
    z = *(s32 *)(p1 + 16);
    x = x - 34;
    z = z >> 20;

    if ((u32)x <= 1 && z > 40 && z <= 42) {
        Func_02007462(148 << 2);
    } else {
        Func_02007474(148 << 2);
    }
}

void SceneActor_FaceActors24And25TowardActorZero(void)
{
    struct Rec_383 *origin = Func_02007926(0);
    struct Rec_383 *first = Func_0200792e(24);
    struct Rec_383 *second = Func_02007936(25);

    first->f6 = Func_02007858(origin->f16 - first->f16, origin->f8 - first->f8);
    second->f6 = Func_0200786c(origin->f16 - second->f16, origin->f8 - second->f8);
}

void FieldScene_RunLateSequence(void)
{
    extern u8 Data_03001ebc[];

    u32 i;
    s32 record;
    s32 v5;

    Func_02007938();
    Call3(Func_020079ce, 10, 0x3180000, 0x1a00000);
    Call3(Func_020079dc, 11, 0x3200000, 0x1900000);
    Call3(Func_020079ea, 12, 0x3080000, 0x1a00000);
    Call3(Func_02007a56, 10, 0x3000, 0);
    Call3(Func_02007a62, 11, 0x3000, 0);
    Call3(Func_02007a6e, 12, 0x3000, 0);
    Call3(Func_020079d0, 11, 0xcccc, 0x6666);
    Call3(Func_020079da, 12, 0xcccc, 0x6666);
    Call3(Func_02007a30, 0, 0x3100000, 0x1c00000);
    Call3(Func_02007a3e, 1, 0x3280000, 0x1b00000);
    Call3(Func_02007a4c, 2, 0x3080000, 0x1b00000);
    Func_02007a5c(0, 19);
    Func_02007a64(1, 19);
    Func_02007a6c(2, 19);
    v5 = 2;
    *(u8 *)(Func_02007a1a(0) + 35) = v5;
    *(u8 *)(Func_02007a26(1) + 35) = v5;
    *(u8 *)(Func_02007a30_a(2) + 35) = v5;
    record = Func_02007a3a(0);
    Func_020079b0(record, 0);
    record = Func_02007a46(2);
    Func_020079bc(record, 0);
    record = Func_02007a52(1);
    Func_020079c8(record, 0);
    Call3(Func_02007b14, 8, 0xb000, 0);
    *(s32 *)((*(u8 *volatile *)Data_03001ebc + 0x1c0)) = 0x209;
    Func_02007b4a(0, 0);
    Func_02007b5e();
    Func_020079ca();
    Func_020072d6();
    Func_02007a4c_a(60);
    Func_02007356(10, 3, 20);
    Call1_02001ba0(Func_02007b34, 0x12dd);
    Func_0200731c(10, 30);
    Func_02007324(8, 30);
    Call3(Func_02007aea, 11, 0x328, 0x1c8);
    Call3(Func_02007af8, 12, 0x318, 0x1c8);
    Func_02007b16(12);
    Func_02007b88(12, 0, 0);
    Func_02007b26(11);
    Func_02007b98(11, 0, 0);
    Func_02007aae(30);
    Func_020073b8(11, 3, 20);
    Func_02007378(11, 20);
    Func_0200739a(12, 0, 30);
    Func_0200738a(12, 60);
    Func_02007ae6();
}

s32 OverlayObject_GetObjectTwoByte118(void)
{
    return Func_02007c8e_a(2)[0x118];
}

s32 OverlayObject_RunObjectTwoWhenFlagged(void)
{
    u8 *o;

    Func_02007eb8();
    o = Func_02007ca6(2);
    o += 0xf8;
    if (*(s32 *)o & 1) {
        Func_02007ce6(2, 0, 0, 0);
        Func_02007ee4(126);
        Func_02007cd2(0);
        return Func_02007cd8(2);
    }
}

/* Sets up actors 10, 11, 12 (position, pose, flags) and actors 0-2 and 8-9 and
 * 13-14 in several later waves, moving and animating them through a long
 * scripted sequence, branches once on an actor-12 state check, then advances
 * the shared scene phase twice before returning. */
void RunEventScript01(void)
{
    extern u8 Data_03001ebc[];

    u32 i;
    s32 record;
    u8 *work;
    s32 base5_200d17c;
    u8 *actor12_record;

    record = Scene_GetRecord_1_02002fd4(12);
    actor12_record = *(volatile s32 *)(record + 80);
    BattleRuntime_Reset_1_02002fd4();
    ObjectMotion_SetHorizontalPositionWithTerrain_1_02002fd4(10, 0x3180000, 0x1a00000);
    ObjectMotion_SetHorizontalPositionWithTerrain_2_02002fd4(11, 0x3200000, 0x1900000);
    ObjectMotion_SetHorizontalPositionWithTerrain_3_02002fd4(12, 0x3080000, 0x1880000);
    record = Scene_GetRecord_2_02002fd4(10);
    Func_02007d02(record, 0);
    record = Scene_GetRecord_3(11);
    Func_02007d0e(record, 0);
    record = Scene_GetRecord_4(12);
    Func_02007d1a(record, 0);
    Object_SetModeById_1_02002fd4(10, 9);
    Object_SetModeById_2_02002fd4(11, 9);
    Object_SetModeById_3_02002fd4(12, 9);
    /* Clear the low bit of the flag byte at +35. */
    *(u8 *)(Scene_GetRecord_5(12) + 35) &= 254;
    /* Set flag bits 0x0c of the byte at +9. */
    actor12_record[9] |= 12;
    base5_200d17c = (s32)Data_0200d17c;
    ObjectMotion_EnableActionAndSetCallback_1_02002fd4(10, base5_200d17c);
    ObjectMotion_SetHorizontalPositionWithTerrain_4_02002fd4(0, 0x3180000, 0x1b80000);
    ObjectMotion_SetHorizontalPositionWithTerrain_5_02002fd4(1, 0x3280000, 0x1b00000);
    ObjectMotion_SetHorizontalPositionWithTerrain_6_02002fd4(2, 0x3080000, 0x1b80000);
    ObjectMotion_ArmCallback_1_02002fd4(0, 0xc000, 0);
    ObjectMotion_ArmCallback_2_02002fd4(1, 0xb000, 0);
    ObjectMotion_ArmCallback_3_02002fd4(2, 0xb000, 0);
    ObjectMotion_SetSpeedParameters_1_02002fd4(8, 0xcccc, 0x6666);
    ObjectMotion_ArmCallback_4_02002fd4(8, 0xb000, 0);
    SCENE_PHASE = 0x209;
    ObjectMotion_SetPositionAndReset_1_02002fd4(0, 0);
    Object_CommitPositionThenWaitIfModeZero_1();
    Func_02007dae();
    Func_020076ba();
    ObjectMotion_EnableActionAndSetCallback_2_02002fd4(11, base5_200d17c);
    BattleRuntime_WaitIfModeZero_1_02002fd4(30);
    ObjectMotion_EnableActionAndSetCallback_3_02002fd4(12, base5_200d17c);
    BattleRuntime_WaitIfModeZero_2_02002fd4(30);
    SceneWork_SetStepValue_1_02002fd4(0x12e4);
    Func_0200770c(10, 20);
    BattleEffect_SpawnLinkedResourceObject_1_02002fd4(8, 0x102, 0);
    BattleRuntime_WaitIfModeZero_3_02002fd4(60);
    Func_02007726(8, 20);
    ObjectMotion_SetPositionAndReset_2_02002fd4(8, 0x328, 0x1c8);
    ObjectMotion_SetSpeedParameters_2_02002fd4(1, 0xcccc, 0x6666);
    ObjectMotion_SetPositionAndReset_3_02002fd4(1, 0x318, 0x1b0);
    ObjectMotion_ArmCallback_5_02001ba0(1, 0, 0);
    ObjectMotion_SetPositionAndReset_4(8, 0x328, 0x198);
    ObjectMotion_ArmCallback_6_02001ba0(8, 0x8000, 0);
    ObjectMotion_SetPositionAndReset_5(1, 0x328, 0x1b0);
    ObjectMotion_ArmCallback_7_02001ba0(1, 0xc000, 0);
    BattleRuntime_WaitIfModeZero_4_02002fd4(20);
    Func_020077e2(8, 3, 20);
    Func_020077a2(8, 20);
    ObjectMotion_SetPositionAndReset_6(8, 0x300, 0x198);
    BattleRuntime_WaitIfModeZero_5_02001ba0(20);
    ObjectMotion_SetAngleToward_1_02002fd4(8, 0, 0);
    ObjectMotion_SetAngleToward_2_02002fd4(0, 8, 0);
    ObjectMotion_SetAngleToward_3_02002fd4(1, 8, 0);
    Func_020077f6(2, 8, 40);
    Func_020077e6(8, 30);
    Object_SetModeById_4(0, 3);
    Object_SetModeById_5(1, 3);
    Func_02007848(2, 3, 20);
    ObjectMotion_SetPositionAndReset_7(8, 0x2e8, 0x198);
    BattleRuntime_WaitIfModeZero_6_02001ba0(50);
    ObjectMotion_SetVariantCallbackAndInvokeObject_1_02002fd4(11, 2);
    Func_02007824(11, 20);
    ObjectMotion_EnableActionAndSetCallback_4_02002fd4(11, base5_200d17c);
    ObjectMotion_SetVariantCallbackAndInvokeObject_2_02002fd4(1, 1);
    BattleRuntime_WaitIfModeZero_7_02001ba0(20);
    ObjectMotion_SetAngleToward_4_02002fd4(0, 11, 0);
    ObjectMotion_SetAngleToward_5(1, 11, 0);
    Func_02007870(2, 11, 20);
    Func_02007860(1, 20);
    ObjectMotion_SetVariantCallbackAndInvokeObject_3_02001ba0(12, 2);
    Func_02007870_a(12, 30);
    ObjectMotion_EnableActionAndSetCallback_5(12, base5_200d17c);
    BattleEffect_SpawnLinkedResourceObject_2_02002fd4(1, 0x103, 0);
    BattleRuntime_WaitIfModeZero_8_02001ba0(60);
    Func_02007890(1, 30);
    Func_020078b2(2, 0, 30);
    Func_020078bc(0, 2, 30);
    Func_020078f6(2, 3, 20);
    Func_020078b6(2, 20);
    Func_020078d8(1, 2, 30);
    Func_02007912(0, 3, 30);
    Func_020078d2(2, 20);
    BattleEffect_SpawnLinkedResourceObject_3_02002fd4(1, 0x100, 0);
    BattleRuntime_WaitIfModeZero_9_02001ba0(60);
    Func_020078ec(1, 30);
    BattleEffect_SpawnLinkedResourceObject_4(2, 0x101, 0);
    BattleRuntime_WaitIfModeZero_10_02001ba0(60);
    Func_02007936_a(0, 1, 20);
    Object_SetModeById_6(0, 3);
    Func_02007960(1, 3, 20);
    Func_02007984(2, 0);
    Func_0200822c();
    ObjectMotion_SetAngleToward_6(0, 2, 0);
    Func_02007950(1, 2, 10);
    ObjectMotion_SetVariantCallback_1_02001ba0(0, 1);
    ObjectMotion_SetVariantCallbackAndInvokeObject_4_02001ba0(1, 1);
    BattleRuntime_WaitIfModeZero_11_02001ba0(20);
    BattleRuntime_WaitIfModeZero_12(1, 0x102);
    BattleRuntime_WaitIfModeZero_13(60);
    Func_02007980(0, 1, 10);
    ObjectMotion_SetSpeedParameters_3_02002fd4(1, 0);
    if (UiWork_WaitThenFinalizeCapacity_1_02002fd4(0, 0) == 0) {
        BattleRuntime_WaitIfModeZero_14(20);
        ObjectMotion_SetAngleToward_7(0, 2, 0);
        Func_020079ae(1, 2, 20);
        Func_02007a2a();
        ObjectMotion_SetVariantCallback_2(0, 1);
        ObjectMotion_SetVariantCallbackAndInvokeObject_5(1, 1);
        ((void (*)())Func_020080f8_a)(20);
        Func_020079b8(1, 20);
    } else {
        bump_step(1);
        BattleEffect_SpawnLinkedResourceObject_5(1, 0x102, 0);
        BattleRuntime_WaitIfModeZero_15(60);
        Func_020079fe(1, 20);
        ObjectMotion_SetAngleToward_8(0, 2, 0);
        Func_02007a2a_a(1, 2, 20);
        Func_02007aa6();
        ObjectMotion_SetVariantCallback_3(0, 1);
        ObjectMotion_SetVariantCallbackAndInvokeObject_6(1, 1);
        BattleRuntime_WaitIfModeZero_16(20);
    }
    BattleEffect_SpawnLinkedResourceObject_6(2, 0x102, 0);
    BattleRuntime_WaitIfModeZero_17(60);
    Func_02007a90(2, 4, 20);
    SceneWork_SetStepValue_2_02002fd4(0x12f2);
    Func_02007a56_a(2, 20);
    Object_SetModeById_7(0, 3);
    Func_02007ab0(1, 3, 40);
    ObjectMotion_SetPositionAndReset_8(8, 0x328, 0x198);
    ObjectMotion_ArmCallback_8(8, 0x8000, 0);
    BattleRuntime_WaitIfModeZero_18(30);
    ObjectMotion_SetAngleToward_9(0, 8, 0);
    ObjectMotion_SetAngleToward_10(1, 8, 0);
    Func_02007abe(2, 8, 20);
    ObjectMotion_SetVariantCallbackAndInvokeObject_7(8, 1);
    BattleRuntime_WaitIfModeZero_19(20);
    Func_02007abc(8, 20);
    ObjectMotion_SetSpeedParameters_4_02002fd4(9, 0xcccc, 0x6666);
    ObjectMotion_SetSpeedParameters_5_02001ba0(13, 0xcccc, 0x6666);
    ObjectMotion_SetSpeedParameters_6(14, 0xcccc, 0x6666);
    ObjectMotion_SetHorizontalPositionWithTerrain_7(9, 0x2e80000, 0x1980000);
    ObjectMotion_SetPositionAndReset_9(9, 0x300, 0x198);
    Func_02007b18(9, 10, 30);
    ObjectMotion_SetHorizontalPositionWithTerrain_8(13, 0x2e80000, 0x1980000);
    ObjectMotion_SetPositionAndReset_10(13, 0x300, 0x198);
    ObjectMotion_SetHorizontalPositionWithTerrain_9(14, 0x2e80000, 0x1980000);
    ObjectMotion_ResetAndSetPositionInMode2_1_02002fd4(14, 0x310, 0x190);
    ObjectMotion_SetPositionAndReset_11(13, 0x308, 0x1a8);
    ObjectMotion_CommitCurrentPositionAndActivate_1_02002fd4(14);
    ObjectMotion_SetAngleToward_11(13, 10, 0);
    Func_02007b78(14, 10, 20);
    ObjectMotion_SetAngleToward_12(0, 9, 0);
    ObjectMotion_SetAngleToward_13(1, 9, 0);
    Func_02007b96(2, 9, 20);
    Func_02007bd0(9, 4, 20);
    Func_02007b90(9, 20);
    Func_02007b98_a(11, 20);
    Func_02007ba0(12, 30);
    Func_02007bda(9, 13, 20);
    ObjectMotion_SetVariantCallbackAndInvokeObject_8(13, 1);
    BattleRuntime_WaitIfModeZero_20(20);
    Func_02007bc0(13, 20);
    Func_02007c12(9, 3, 30);
    Func_02007c04(9, 14, 20);
    Func_02007c26(9, 3, 30);
    Func_02007c00(9, 10, 20);
    Func_02007bf0(9, 20);
    Object_SetModeById_8(13, 3);
    Func_02007c4a(14, 3, 20);
    Func_02007c3c(13, 14, 20);
    Object_SetModeById_9(13, 3);
    Func_02007c66(14, 3, 20);
    ObjectMotion_ResetAndSetPositionInMode2_2_02002fd4(14, 0x318, 0x188);
    ObjectMotion_SetPositionAndReset_12(13, 0x310, 0x190);
    ObjectMotion_SetAngleToward_14(13, 12, 0);
    ObjectMotion_CommitCurrentPositionAndActivate_2_02001ba0(14);
    Func_02007c6c(14, 11, 20);
    ObjectMotion_SetVariantCallbackAndInvokeObject_9(13, 1);
    BattleRuntime_WaitIfModeZero_21(20);
    Func_02007c6a(13, 20);
    Func_02007cbc(14, 4, 20);
    Func_02007c7c(14, 30);
    Func_02007cb6(13, 0, 20);
    Func_02007c8e(13, 20);
    Object_SetModeById_10(0, 3);
    Object_SetModeById_11(1, 3);
    Func_02007cf0(2, 3, 50);
    /* Write the field at +0x1c8, then the phase/status word at +0x1c0, of
     * the shared scene work record. */
    work = *(u8 *volatile *)Data_03001ebc;
    *(volatile s32 *)((work + 0x1c8)) = 30;
    *(volatile s32 *)((work + 0x1c0)) = 0x201;
    Func_02008574();
    ObjectMotion_SetSpeedParameters_7();
    BattleRuntime_WaitIfModeZero_22(60);
    BattleRuntime_ScheduleShoulderButtonModeUpdate_1_02002fd4();
}

void RunDialoguePromptScene(void)
{
    extern u8 Data_03001ebc[];

    u32 i;
    s32 off1c8;
    s32 off1d8;
    u8 *rec8;
    u8 *record;
    u8 *work;
    s32 base6_12fc;
    s32 base5_200d354;
    s32 base5_200d4c8;
    s32 base5_1324;
    u8 *p7;

    p7 = *(volatile s32 *)Data_03001ebc;
    Call1_020036f8(Func_02008428, 0x855);
    Func_02008454_a();
    {
        u8 *record = Func_02008492(12);
        u8 value = *(volatile u8 *)&record[35];

        record[35] = (u8)(value | 1);
    }
    Call3(Func_020084d0, 15, 0x368, 0x1a9);
    Call3(Func_020084dc, 16, 0x368, 0x199);
    Call3(Func_020084e8, 17, 0x368, 0x179);
    Call3(Func_0200851e, 11, 0x3080000, 0x1880000);
    Call3(Func_0200852c, 10, 0x3180000, 0x1880000);
    Call3(Func_0200853a, 12, 0x3280000, 0x1880000);
    Func_0200854a(10, 5);
    Func_02008552(11, 5);
    Func_0200855a(12, 5);
    Func_02008594(11, 0, 0);
    Func_0200859e(10, 0, 0);
    Func_020085a8(12, 0, 0);
    record = Func_02008526(10);
    Func_0200849c((s32)record, 1);
    record = Func_02008532(11);
    Func_020084a8((s32)record, 1);
    record = Func_0200853e(12);
    Func_020084b4((s32)record, 1);
    Call3(Func_020085a2, 13, 0x3000000, 0x1980000);
    Call3(Func_020085b0, 14, 0x3000000, 0x1a80000);
    Call3(Func_02008596, 9, 0x310, 0x1a8);
    Call3(Func_020085cc, 8, 0x3280000, 0x1980000);
    Func_0200860e(13, 9, 0);
    Func_02008618(8, 9, 0);
    Func_02008622(14, 10, 0);
    Func_0200862c(9, 10, 0);
    Call3(Func_02008602, 0, 0x3180000, 0x1b80000);
    Call3(Func_02008610, 1, 0x3280000, 0x1b80000);
    Call3(Func_0200861e, 2, 0x3080000, 0x1b80000);
    Func_02008660(0, 10, 0);
    Func_0200866a(1, 10, 0);
    Func_02008674(2, 10, 0);
    work = *(u8 *volatile *)Data_03001ebc;
    off1c8 = 0x1c8;
    *(volatile s32 *)((s32)work + off1c8) = 30;
    *(volatile s32 *)(((s32)work + 0x1c0)) = 0x201;
    Func_02008728();
    Func_0200873c();
    Func_020085d2(40);
    Func_0200869a(10, 2);
    base6_12fc = (s32)Data_000012fc;
    Func_020086ba(base6_12fc);
    Func_02007ea2(10, 20);
    Func_020086b2(9, 1);
    Func_020085f8(20);
    Func_02007f02(9, 4, 20);
    Func_02007ec2(9, 20);
    Func_020086aa(13, 3);
    Func_02007f1c(8, 3, 20);
    Func_020086dc(11, 2);
    Func_020086e4(12, 2);
    Func_02008632(60);
    Call3(Func_02008684, 13, 0xcccc, 0x6666);
    Call3(Func_020086b8, 13, 0x2ea, 0x198);
    Call3(Func_02008744, 9, 0xb000, 0);
    Call3(Func_02008750, 14, 0xb000, 0);
    base5_200d354 = (s32)Data_0200d354;
    Func_020086ba_a(11, base5_200d354);
    Func_02008670(20);
    Func_020086c8(10, base5_200d354);
    Func_0200867e(15);
    Func_020086d6(12, base5_200d354);
    Func_0200868c(35);
    Call2(Func_020086e4_a, 8, 0x200d2fc);
    Func_0200869a_a(20);
    Func_02008728_a(13);
    Func_0200873a(13, 0, 0);
    Func_020086b0(40);
    Call3(Func_02008702, 9, 0xcccc, 0x6666);
    Call3(Func_0200870c, 14, 0xcccc, 0x6666);
    Call3(Func_0200874a, 9, 0x310, 0x198);
    Func_020087a4(9, 0, 0);
    Call3(Func_02008762, 14, 0x300, 0x198);
    Func_02007fc4_a(14, 0, 20);
    Func_02007ffe(9, 3, 20);
    Func_02007fbe(9, 20);
    Func_02008010_a(14, 3, 20);
    Call3(Func_0200880c, 14, 0x2000, 10);
    Func_02007fdc(14, 20);
    Func_02008016(0, 1, 50);
    Func_02008020(0, 2, 50);
    Func_0200880a(0, 9, 0);
    Func_02008814(1, 9, 0);
    Func_02008026(2, 9, 20);
    Func_020087f6(0, 3);
    Func_020087fe(1, 3);
    Func_02008070_a(2, 3, 50);
    Func_0200884a(9, 14, 0);
    Func_02008084(9, 3, 20);
    Func_02008044_a(9, 20);
    Call2(Func_020087e4, 14, 0x200d3ac);
    Func_0200879a(50);
    Value2(Func_020087f2, 9, 0x200d444);
    Call3(Func_020087f4, 1, 0xcccc, 0x6666);
    Call3(Func_020087fe_a, 2, 0xcccc, 0x6666);
    Call3(Func_0200883a, 1, 0x318, off1c8);
    Call3(Func_020088be, 1, 0xd000, 0);
    Func_020087d4(30);
    Call3(Func_0200885a, 2, 0x318, 0x198);
    Func_020088dc(2, 0, 0);
    Call3(Func_02008870, 1, 0x328, off1c8);
    Call3(Func_020088f4, 1, 0xd000, 0);
    Func_0200880a_a(100);
    Func_020080e4(14, 9, 60);
    Func_020080ee_a(9, 14, 40);
    Func_02008128(9, 3, 40);
    Func_02008922(9, 0, 0);
    Func_0200886e(20);
    Func_02008936(9, 2);
    Func_02008a64(124);
    Func_0200891c(15, 4);
    Call3(Func_02008922_a, 18, 0x3680000, 0x1a80000);
    Func_02008992(18, 1);
    Call3(Func_020088ec, 18, 0xcccc, 0x6666);
    Call3(Func_02008930, 18, 0, -8);
    Func_0200893e(18);
    Func_0200897e(18, 2);
    Func_020088c4(60);
    Func_02008884((base6_12fc + 5), 1);
    Func_0200896c(15, 2);
    Func_0200896e(18, 0, 0);
    {
        u8 *work0 = *(u8 **)Data_03001ebc;
        u16 *slot0;
        s32 next0;

        off1d8 = 0x1d8;
        slot0 = (u16 *)((s32)work0 + off1d8);
        next0 = *slot0 + 1;
        *slot0 = next0;
    }
    Func_020089b8(14, 1);
    Func_020081b8(14, 20);
    Func_020081f2(0, 1, 40);
    Func_020081e4(9, 14, 20);
    Func_0200821e_a(9, 3, 20);
    Func_020081de(9, 30);
    Func_020089f8(0, 14, 0);
    Func_0200820a(1, 14, 40);
    Func_02008a34(14, 0, 0);
    Func_0200894a(40);
    Func_02008a12(14, 2);
    Func_02008b40(124);
    Func_020089f8_a(16, 4);
    {
        u8 *rec;
        s32 t = 0;
        u16 zero_sym = (u16)(t + t);

        rec = (u8 *)Func_020089a6(19);
        rec[85] = zero_sym;
    }
    Func_02008a6c(19, 1);
    Call3(Func_02008a12_a, 19, 0x3680000, 0x1980000);
    Call3(Func_020089d4, 19, 0xcccc, 0x6666);
    Call3(Func_02008a18, 19, 0, -8);
    Func_02008a26(19);
    Func_02008a76(19, 2);
    Func_020089be(60);
    Func_0200897e_a((base6_12fc + 8), 1);
    Func_02008a66(16, 2);
    Func_02008a68(19, 0, 0);
    bump_step_020036f8(off1d8, 1);
    Call3(Func_02008af0, 9, 0x102, 0);
    Func_020089f6(60);
    Func_020082b6_b(9, 20);
    Func_02008abe(0, 2);
    Func_02008ac6(1, 2);
    Func_02008ad6(2, 2);
    Func_02008a1c(20);
    Func_02008326(14, 3, 50);
    Func_02008300_a(9, 0, 20);
    Func_020082f0_a(9, 30);
    Call3(Func_02008b34, 14, 0xd000, 0);
    Func_02008a4a(30);
    Call3(Func_02008b56, 14, 0x100, 0);
    Func_02008a5c(60);
    Call3(Func_02008ae2, 14, 0x358, 0x178);
    Func_02008a70(20);
    Func_0200834a(14, 9, 20);
    Func_0200833a(14, 20);
    Func_02008b54(9, 14, 0);
    Call3(Func_02008b98, 2, 0x100, 0);
    Func_02008a9e(60);
    Func_02008378(2, 14, 30);
    Func_02008382(9, 2, 20);
    Func_020083bc(9, 3, 20);
    Func_02008bac(9, 0);
    Call3(Func_02008bc0, 14, 0x5000, 0);
    Func_02008ad6_a(30);
    Func_020083b0(2, 9, 20);
    Func_020083ea(2, 3, 20);
    Func_020083aa_a(9, 20);
    Func_02008bcc(0, 2, 0);
    Func_020083d6(1, 2, 20);
    Func_02008ba6(0, 3);
    Func_02008418(1, 3, 40);
    Func_02008422(2, 3, 30);
    Func_02008bea(9, 1);
    Func_02008b30(20);
    Func_0200843a(9, 4, 20);
    Func_020083fa_a(9, 20);
    Func_02008c14(0, 9, 0);
    Func_02008c1e(1, 9, 0);
    Func_02008430(2, 9, 20);
    Func_02008c20(0, 1);
    Func_02008c28(1, 1);
    Call3(Func_02008c7c, 2, 0x102, 0);
    Func_02008b82(60);
    Func_02008442(2, 40);
    Func_02008494(9, 3, 20);
    Func_02008c34(0, 4);
    Func_02008c44(1, 4);
    Func_02008c6c(2, 3);
    Func_02008bb2(30);
    Func_020084bc(9, 4, 20);
    Func_0200847c(9, 20);
    Func_020084b6(0, 1, 20);
    Func_02008c96(2, 2);
    Func_020084e0(2, 4, 30);
    Call3(Func_02008c36, 2, 0x18000, 0xc000);
    Call3(Func_02008c74, 2, 0x320, 0x198);
    Func_02008cc4(2, 2);
    Func_02008cf4(2, 0);
    Func_02008cde(0, 9, 0);
    Func_020084f0(1, 9, 30);
    Func_0200852a(9, 4, 20);
    Func_020084ea(9, 20);
    Call2(Func_02008d44, 0, 0x102);
    Call2(Func_02008d4e, 1, 0x102);
    Call2(Func_02008d58, 2, 0x102);
    Func_02008c56(60);
    Func_02008516(2, 20);
    Func_02008d26(9, 1);
    Func_02008c6c_a(20);
    Func_0200852c_a(9, 20);
    Call3(Func_02008d7e, 0, 0x101, 0);
    Call3(Func_02008d88, 1, 0x101, 0);
    Call3(Func_02008d92, 2, 0x101, 0);
    Func_02008c98(60);
    Func_020085a2_a(9, 4, 20);
    Func_02008562(9, 20);
    Func_02008d6a(0, 1);
    Func_02008d72(1, 1);
    Func_02008d82(2, 1);
    Func_02008cc8(20);
    Func_020085d2_a(9, 3, 20);
    Func_02008592(9, 40);
    Call3(Func_02008de4, 2, 0x105, 0);
    Func_02008cea(60);
    Call3(Func_02008d70, 9, 0x348, 0x1a8);
    Func_020085ea(9, 0, 20);
    Func_020085c2(9, 20);
    Func_02008614(0, 3, 20);
    Call3(Func_02008e10, 9, 0x5000, 0);
    Func_02008d26_a(20);
    Func_020085e6(9, 20);
    Func_02008df6(1, 1);
    Func_02008d3c(20);
    Func_020085fc(1, 20);
    Func_0200864e(9, 3, 20);
    Func_0200860e_a(9, 30);
    Func_02008648(9, 14, 20);
    Func_02008620(9, 20);
    Call3(Func_02008dee, 14, 0x358, 0x198);
    base5_200d4c8 = (s32)Data_0200d4c8;
    Func_02008dd0(9, base5_200d4c8);
    Func_02008dd8(14, base5_200d4c8);
    Call3(Func_02008e04, 1, 0x318, off1c8);
    Call3(Func_02008de6, 2, 0xcccc, 0x6666);
    Call3(Func_02008e1c, 2, 0x308, 0x1b0);
    Func_02008e3a(1);
    Call3(Func_02008eae, 1, 0xd000, 0);
    Func_02008e4c(2);
    Call3(Func_02008ec0, 2, 0xd000, 0);
    Func_02008e2e(9);
    Func_02008ea8(0, 9, 0);
    rec8 = Value1(Func_02008e26, 14);
    {
        u8 *target = rec8 + 91;
        s32 shown = 1;

        *target = shown;
    }
    *(s32 *)((s32)rec8 + 56) = -0x80000000;
    *(s32 *)((s32)rec8 + 60) = -0x80000000;
    *(s32 *)((s32)rec8 + 64) = -0x80000000;
    Call3(Func_02008f06, 9, 0x100, 0);
    Func_02008ea6(14, 1);
    Func_02008e14(50);
    Func_02008ee6(9, 0, 0);
    Call3(Func_02008ea4, 1, 0x328, 0x1b8);
    Call3(Func_02008f28, 1, 0xb000, 0);
    Func_02008e3e(20);
    Func_020086fe(9, 20);
    Func_02008738(0, 1, 50);
    Func_02008f22(0, 9, 0);
    Func_02008734(1, 9, 20);
    Func_0200876e(9, 3, 20);
    Call3(Func_02008eec, 9, 0x2e8, 0x198);
    Call3(Func_02008efa, 14, 0x2e8, 0x198);
    Func_02008f18(9);
    Func_02008f1e(14);
    Func_02008e9c(30);
    Call3(Func_02008f22_a, 0, 0x318, 0x198);
    Func_0200879c(0, 1, 30);
    Call3(Func_02008fbe, 2, 0x105, 0);
    Func_02008ec4(50);
    Func_02008f96(0, 2, 0);
    Func_020087a8(1, 2, 20);
    Func_02008fa0(1, 1);
    Func_02008ee6_a(20);
    Func_020087a6(1, 40);
    Func_020087f8(2, 3, 30);
    Func_020087ea(0, 1, 40);
    Func_02008fa2(0, 4);
    Func_02008814_a(1, 4, 30);
    Func_02008fe6(0, 2, 0);
    Func_020087f8_a(1, 2, 20);
    Func_02008ff0(1, 1);
    Func_02008f36(20);
    Func_020087f6_a(1, 40);
    Func_02009006(2, 2);
    Func_02008f4c(20);
    Func_0200880c_a(2, 20);
    Func_02008ff4(0, 3);
    Func_02008866(1, 3, 20);
    Func_02008870_a(2, 4, 20);
    Func_02008830(2, 20);
    Call3(Func_02009082, 0, 0x101, 0);
    Call3(Func_0200908c, 1, 0x101, 0);
    Func_02008f92(60);
    Func_0200889c(2, 3, 20);
    Func_0200885c(2, 20);
    Call3(Func_020090b0, 0, 0x100, 0);
    Call3(Func_020090bc, 1, 0x100, 0);
    Func_02008fc2(60);
    Func_020088cc(2, 3, 20);
    Func_0200888c(2, 20);
    Call3(Func_020090e0, 0, 0x102, 0);
    Call3(Func_020090ec, 1, 0x102, 0);
    Func_02008ff2(60);
    Func_020088fc(2, 4, 20);
    Func_020088bc(2, 20);
    Func_0200890e(1, 3, 20);
    Func_020088ce(1, 30);
    Value2(Func_020090fe, 2, 0);
    if (Value2(Func_0200905e, 0, 0) != 0) {
        Func_02009030(20);
        Func_020090f8(1, 1);
        Func_0200903e(20);
        Func_020088fe(1, 20);
        Func_02008950(2, 4, 20);
        Func_02008910(2, 20);
    }
    Func_02008978(2, 3, 30);
    Call3(Func_020090fe_a, 2, 0x320, 0x1c8);
    Func_02009158(0, 2, 0);
    Func_02009162(1, 2, 0);
    Call3(Func_02009120, 2, 0x358, 0x1c8);
    Func_0200917a(0, 2, 0);
    Func_02009184(1, 2, 0);
    Call3(Func_02009142, 2, 0x358, 0x178);
    Func_0200919c(0, 2, 0);
    Func_020091a6(1, 2, 0);
    Func_020091d8(2, 0, 0);
    Func_020090ee(20);
    Func_020091b6(2, 2);
    rec8 = Value3(Func_02009278, 2, 17, 65);
    Func_02009108(60);
    base5_1324 = (s32)Data_00001324;
    Func_020090ca(base5_1324, 1);
    Func_02009098((s32)rec8);
    Func_020091b8(17, 2);
    Func_02009126(20);
    Func_02008a30(2, 3, 20);
    Call3(Func_020091b6_a, 2, 0x358, 0x1c8);
    Func_02009210(0, 2, 0);
    Func_0200921a(1, 2, 0);
    Call3(Func_020091d8_a, 2, 0x320, 0x1c8);
    Func_02009232(0, 2, 0);
    Func_0200923c(1, 2, 0);
    Call3(Func_020091fa, 2, 0x308, 0x1a8);
    Func_0200925c(0, 2, 0);
    Func_02008a66_a(1, 2, 30);
    Func_02008aa2(2, 3, 20);
    Func_02009280((base5_1324 + 1));
    Func_02008a68_a(2, 20);
    Func_02009250(0, 3);
    {
        u16 *slot = (u16 *)((s32)p7 + 0x1d8);
        s32 saved = *(s16 *)slot;

        if (Value0(Func_0200740e)!= 0) {
            Call1_020036f8(Func_020092a8, 0x132a);
            Func_020092c0(2, 0);
            Func_02007438();
        }
        Func_0200920a(2);
        *slot = saved;
    }
    Func_02008aee(1, 3, 50);
    Call3(Func_02009274, 2, 0x308, 0x198);
    Func_020092ce(0, 2, 0);
    Func_020092d8(1, 2, 0);
    Call3(Func_02009296, 2, 0x2e8, 0x198);
    Func_02009224(40);
    Func_02008b16(0, 1, 20);
    Func_020092f6(1, 1);
    Func_0200923c_a(20);
    Func_02008afc(1, 20);
    Func_02008b4e(0, 3, 20);
    Func_020092ee(1, 2);
    record = Value1(Func_0200929c, 0);
    if ((s32)record != 0) {
        Func_020092ce_a(1, *(s16 *)((s32)record + 10), *(s16 *)((s32)record + 18));
    }
    Func_020092fc(1);
    Func_0200930e(1, 0, 0);
    Func_02009284(30);
    Func_0200931e(8, 0, 0);
    Func_02009328(9, 0, 0);
    Func_02009332(13, 0, 0);
    Func_0200933c(14, 0, 0);
    Func_02009346(10, 0, 0);
    Func_02009350(11, 0, 0);
    Func_0200935a(12, 0, 0);
    Func_02009364(2, 0, 0);
    *(s32 *)((*(u8 *volatile *)Data_03001ebc + 0x1c0)) = 0x209;
    Func_020092f6_a();
}

void SceneState_SetWord1c0To209AndRun(void)
{
    u8 *state;

    state = *(u8 **)0x03001ebc;
    *(s32 *)(state + 0x1c0) = 0x209;
    Func_0200947a();
    Func_0200948e();
    Func_02009324(1);
}

void SceneActor_SetModeZeroAndValue(s32 a, s32 b)
{
    Func_02009420(a, 0);
    Func_0200933e(b);
}

void FieldScene_RunSplitTripleSteps(s32 a, s32 b, s32 c)
{
    Func_02009418(a, b, 0);
    Func_02009356_a(c);
}

void SceneActor_SetPairZeroAndValue(s32 a, s32 b, s32 c)
{
    Func_02009438(a, b, 0);
    Func_0200936e(c);
}

void SceneEffect_ApplyThreeValuesAndFinish(s32 a, s32 b, s32 c)
{
    Func_02009418_a(a, b, c);
    Func_0200942e(a);
    Func_0200938c(c);
}

void SceneEffect_ApplyPairWithValue141(s32 a, s32 b)
{
    Func_02009538(141, 1);
    Func_02009548(a, b);
    Func_02009564();
    Func_02009542(1);
    Func_020092e8(1);
}

void SceneState_SetValue2ThenFinish(void)
{
    Func_02009556(2);
    Func_02009572();
    Func_0200957e();
}

void OverlayObject_ConfigureObject22WithResource17(s32 a)
{
    u8 *o;
    u8 *q;
    u8 *p;
    u8 *v;
    s32 z;
    s32 m;

    z = 0;
    o = Func_02009356(22);
    if (o != 0) {
        q = *(u8 **)(o + 0x50);
        p = q + 38;
        *p = z;
        p += 1;
        *p = z;
        m = 33;
        m = -m;
        q[5] &= m;
        q[9] &= 15;
        o[0x55] = z;
        o[0x5c] = 1;
        v = Func_02009370(17, 0x608);
        Func_020093e0(a);
        v += 0x400;
        Func_02009398(q[28], 0x80, v);
        Func_02009396(17);
    }
}

u8 *SceneData_FindEntryAtPosition(s32 *o)
{
    s32 x = (o[0] + (s32)0xFFC00000) >> 19;
    s32 y = (o[2] + (s32)0xFD900000) >> 19;
    u8 *e = Data_0200cf2c;
    u8 *ret = 0;
    u32 i;

    for (i = 0; i <= 36; i++, e += 16) {
        s32 a = e[0];

        if (a == x || a + 1 == x) {
            s32 b = e[1];

            if (b == y || b + 1 == y) {
                ret = e;
                break;
            }
        }
    }
    return ret;
}

s32 SceneActor_CheckTileFreeOfKinds(u8 *p)
{
    s32 x;
    s32 y;

    if (p == 0) {
        return 1;
    }
    x = (p[0] << 19) + 0x480000;
    y = (p[1] << 19) + 0x2780000;
    if (Func_02009062(x, y, 0) != 0 || Func_02009030_a(x, y, 2) != 0
        || Func_0200903e_a(x, y, 24) != 0 || Func_0200904c(x, y, 25) != 0) {
        return -1;
    }
    return 0;
}

void SceneActor_ApplyScaledBytePairPosition(s32 a, u8 *p)
{
    Func_0200958c(a, (p[0] << 19) + 0x480000, 0, (p[1] << 19) + 0x2780000);
}

void FieldScene_RunScene383_02004b2c(void)
{
    extern u8 Data_03001ebc[];

    Func_02009874();
    Call4(Func_02009996, 0xa80000, -1, 0x2900000, 1);
    Call3(Func_020098d0, 0, 0xcccc, 0x6666);
    Call3(Func_020098da, 1, 0xcccc, 0x6666);
    Call3(Func_020098e4, 2, 0xcccc, 0x6666);
    Call3(Func_02009920, 0, 248, 0x2b8);
    Call3(Func_02009946, 1, 0xf80000, 0x2b80000);
    Call3(Func_02009954, 2, 0xf80000, 0x2b80000);
    Call3(Func_02009940, 0, 200, 0x2b8);
    Call3(Func_0200994c, 1, 248, 0x2c8);
    Call3(Func_02009960, 2, 232, 0x2b8);
    Func_02009976(1);
    Call3(Func_020099ea, 1, 0xc000, 0);
    Call3(Func_020099f6, 2, 0x8000, 0);
    Func_02009994(0);
    Func_020099ac(1, 12);
    Func_02006a5c();
    Call4(Func_02009a54, 0x300000, 0x2400000, 0x1200000, 0x2e00000);
    Call3(Func_02009982, 1, 0x10000, 0x8000);
    Call3(Func_02009990, 2, 0xc000, 0x6000);
    Call3(Func_0200999c, 24, 0x10000, 0x13333);
    Call3(Func_020099aa, 25, 0x18000, 0x18000);
    *(volatile u16 *)Data_0200e4f8 = 0;
    Call2(Func_020098a8, 0x200c8c9, 0xc94);
    Func_0200995e(0x1ff);
    Func_0200998a();
    Func_02009b68(9);
}
