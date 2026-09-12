#include "types.h"

#define BuildMotionCountdown Func_020042e6
#define GetMotionCountdownInput Func_020042f8
#define SceneEffect_UpdateCountdownMotion Func_02000030
#define SceneState_ApplyArgMode1AndReturnZero Func_020000b4
#define SceneState_ApplyFiveRectsAtColumn78 Func_02003950
#define SceneActor_OscillateHeightBetweenLimits Func_02000514
#define SceneActor_SetWord28RandomlyOneIn40 Func_0200089c
#define SceneActor_SetFacingFromSample Func_02000570
#define OverlayObject_DecayFields24And28 Func_020008c0
#define OverlayObject_ActivateSlotWithMode3 Func_02003380
#define SceneData_GetTablec994 Func_02000938
#define SceneData_GetTablecb44 Func_02000940
#define SceneData_GetTablecb64 Func_02000948
#define SceneData_SelectTableByFlags Func_02000950
#define SceneData_SelectTableByThreeFlags Func_020009fc
#define SceneState_FindFirstSetFlagOfGroup Func_02003150
#define SceneData_SelectTableEntryByFlagGroup Func_02003464
#define GameFlag_IsSet_1(a0) Value1(Func_02004f7e, a0)
#define BattleRuntime_Reset_1(args...) Func_02004fa8(args)
#define ObjectMotion_SetAngleToward_1(args...) Func_02005066(args)
#define ObjectMotion_SetSpeedLimitAndAcceleration_1(a0, a1) Call2(Func_020050de, a0, a1)
#define ObjectMotion_PlaceWithinCameraBounds_1(a0, a1, a2, a3) Call4(Func_020050f8, a0, a1, a2, a3)
#define Object_CommitPositionThenWaitIfModeZero_1(args...) Func_02005104(args)
#define BattleRuntime_WaitIfModeZero_1(args...) Func_02004fd4(args)
#define ObjectMotion_SetVariantCallbackAndInvokeObject_1(args...) Func_0200508c(args)
#define SceneWork_SetStepValue_1(a0) Call1(Func_020050aa, a0)
#define BattleEffect_SpawnLinkedResourceObject_1(a0, a1, a2) Call3(Func_020050fc, a0, a1, a2)
#define ObjectMotion_SetVariantCallback_1(args...) Func_020050a4(args)
#define ObjectMotion_SetVariantCallbackAndInvokeObject_2(args...) Func_020050ba(args)
#define ObjectMotion_ArmCallback_1(a0, a1, a2) Call3(Func_02005106, a0, a1, a2)
#define ObjectMotion_SetVariantCallbackAndInvokeObject_3(args...) Func_020050d4_a(args)
#define Object_SetModeById_1(args...) Func_020050e0(args)
#define BattleEffect_SpawnLinkedResourceObject_2(a0, a1, a2) Call3(Func_02005168, a0, a1, a2)
#define ObjectMotion_ArmCallback_2(a0, a1, a2) Call3(Func_0200515c, a0, a1, a2)
#define ObjectMotion_CallThenWaitForAnimationChange_1(args...) Func_0200511a(args)
#define BattleEffect_SpawnLinkedResourceObject_3(a0, a1, a2) Call3(Func_0200519c, a0, a1, a2)
#define ObjectMotion_SetVariantCallback_2(args...) Func_02005144(args)
#define ObjectMotion_CallThenWaitForAnimationChange_2(args...) Func_02005142(args)
#define Object_SetModeById_2(args...) Func_02005156(args)
#define ObjectMotion_CallThenWaitForAnimationChange_3(args...) Func_02005166(args)
#define BattleRuntime_WaitIfModeZero_2(args...) Func_020050d4_b(args)
#define BattleEffect_SpawnLinkedResourceObject_4(a0, a1, a2) Call3(Func_020051e8, a0, a1, a2)
#define SceneWork_SetStepValue_2(args...) Func_020051bc_a(args)
#define ObjectMotion_SetVariantCallback_3(args...) Func_020051a4(args)
#define BattleEffect_SpawnLinkedResourceObject_5(a0, a1, a2) Call3(Func_02005214, a0, a1, a2)
#define ObjectMotion_SetVariantCallback_4(args...) Func_020051bc_b(args)
#define BattleEffect_SpawnLinkedResourceObject_6(a0, a1, a2) Call3(Func_0200522e, a0, a1, a2)
#define BattleRuntime_RunThenWaitIfModeZero_1(args...) Func_02005210(args)
#define BattleEffect_SpawnLinkedResourceObject_7(a0, a1, a2) Call3(Func_02005244, a0, a1, a2)
#define ObjectMotion_CallThenWaitForAnimationChange_4(args...) Func_020051ec(args)
#define BattleRuntime_RunThenWaitIfModeZero_2(args...) Func_0200523e(args)
#define BattleEffect_SpawnLinkedResourceObject_8(a0, a1, a2) Call3(Func_02005272_a, a0, a1, a2)
#define ObjectMotion_SetVariantCallback_5(args...) Func_0200521a(args)
#define Object_SetModeById_3(args...) Func_02005218(args)
#define ObjectMotion_ArmCallback_3(args...) Func_02005288(args)
#define ObjectMotion_ArmCallback_4(args...) Func_02005292(args)
#define ObjectMotion_ArmCallback_5(args...) Func_0200529c(args)
#define ObjectMotion_ArmCallback_6(args...) Func_020052a6(args)
#define ObjectMotion_SetVariantCallbackAndInvokeObject_4(args...) Func_0200526e(args)
#define BattleRuntime_WaitIfModeZero_3(args...) Func_020051c4(args)
#define BattleRuntime_WaitIfModeZero_4(a0, a1) Call2(Func_020052e4, a0, a1)
#define BattleRuntime_WaitIfModeZero_5(a0, a1) Call2(Func_020052ee, a0, a1)
#define BattleRuntime_WaitIfModeZero_6(args...) Func_020051e4(args)
#define ObjectMotion_CallThenWaitForAnimationChange_5(args...) Func_02005284(args)
#define ObjectMotion_SetSpeedLimitAndAcceleration_2(a0, a1) Call2(Func_02005322, a0, a1)
#define ObjectMotion_PlaceWithinCameraBounds_2(a0, a1, a2, a3) Call4(Func_0200533c, a0, a1, a2, a3)
#define ObjectMotion_SetSpeedParameters_1(a0, a1, a2) Call3(Func_0200524e, a0, a1, a2)
#define ObjectMotion_EnableActionAndSetCallback_1(a0, a1) Value2(Func_0200525e, a0, a1)
#define ObjectMotion_SetSpeedParameters_2(a0, a1, a2) Call3(Func_02005260, a0, a1, a2)
#define ObjectMotion_EnableActionAndSetCallback_2(a0, a1) Value2(Func_02005270, a0, a1)
#define ObjectMotion_SetSpeedParameters_3(a0, a1, a2) Call3(Func_02005272_b, a0, a1, a2)
#define ObjectMotion_SetPositionAndReset_1(a0, a1, a2) Call3(Func_020052be, a0, a1, a2)
#define ObjectMotion_SetVariantCallback_6(args...) Func_020052f6_a(args)
#define BattleEffect_SpawnLinkedResourceObject_9(a0, a1, a2) Call3(Func_02005362, a0, a1, a2)
#define BattleRuntime_RunThenWaitIfModeZero_3(args...) Func_0200534c_a(args)
#define ObjectMotion_CallThenWaitForAnimationChange_6(args...) Func_0200530c(args)
#define ObjectMotion_Launch_1(args...) Func_0200531e(args)
#define ObjectMotion_ArmCallback_7(args...) Func_02005378(args)
#define ObjectMotion_SetSpeedLimitAndAcceleration_3(a0, a1) Call2(Func_020053bc, a0, a1)
#define ObjectMotion_PlaceWithinCameraBounds_3(a0, a1, a2, a3) Call4(Func_020053d4, a0, a1, a2, a3)
#define Scene_GetRecord_1(args...) Func_020052d2(args)
#define ObjectMotion_SetSpeedParameters_4(a0, a1, a2) Call3(Func_020052f6_b, a0, a1, a2)
#define ObjectMotion_SetPositionAndReset_2(a0, a1, a2) Call3(Func_02005340, a0, a1, a2)
#define ObjectMotion_SetPositionAndReset_3(a0, a1, a2) Call3(Func_0200534c_b, a0, a1, a2)
#define ObjectMotion_SetPositionAndReset_4(a0, a1, a2) Call3(Func_02005358, a0, a1, a2)
#define ObjectMotion_SetVariantCallbackAndInvokeObject_5(args...) Func_020053a0(args)
#define ObjectMotion_SetPositionAndReset_5(a0, a1, a2) Call3(Func_02005376, a0, a1, a2)
#define ObjectMotion_SetHorizontalPositionWithTerrain_1(args...) Func_02005390(args)
#define Scene_GetRecord_2(args...) Func_02005336(args)
#define Scene_GetRecord_3(a0) Value1(Func_02005342, a0)
#define GameFlag_Set_1(a0) Call1(Func_02005314, a0)
#define BattleRuntime_ScheduleShoulderButtonModeUpdate_1(args...) Func_02005338(args)
#define FieldScene_RunOpeningAuxiliarySequence Func_02000a48
#define FieldScene_RunScene3af_02000bb8 Func_02000bb8
#define FieldScene_RunScene3af_02000bf0 Func_02000bf0
#define FieldScene_RunActorAndEffectPresentationSetup Func_02000ca0
#define FieldScene_RunScene3af_020010a0 Func_020010a0
#define FieldScene_RunScene3af_020011c8 Func_020011c8
#define FieldScene_RunScene3af_020012f0 Func_020012f0
#define FieldScene_RunScene3af_0200185c Func_0200185c
#define FieldScene_RunScene3af_02001920 Func_02001920
#define SceneDialogue_RunActor21Line Func_02000b98
#define SceneState_ResetWorkValuesAndStart Func_020013d4
#define SceneState_ConfigureEntries8Through19 Func_020034bc
#define SceneState_InitActorSlots8To19 Func_02003710
#define SceneActor_PlaceActors20To27 Func_020017a0
#define BattleRuntime_Reset_1_020029d4(args...) Func_02006cc4(args)
#define Object_NotifyLastActiveOfEvent_1(a0) Call1(Func_02006cda, a0)
#define ObjectMotion_SetHorizontalPositionWithTerrain_1_020029d4(a0, a1, a2) Call3(Func_02006d5c, a0, a1, a2)
#define ObjectMotion_SetHorizontalPositionWithTerrain_2(a0, a1, a2) Call3(Func_02006d68, a0, a1, a2)
#define ObjectMotion_SetHorizontalPositionWithTerrain_3(a0, a1, a2) Call3(Func_02006d74, a0, a1, a2)
#define Scene_GetRecord_1_020029d4(args...) Func_02006d1a(args)
#define ObjectMotion_EnableActionAndSetCallback_1_020029d4(a0, a1) Call2(Func_02006d3e, a0, a1)
#define Scene_GetRecord_2_020029d4(args...) Func_02006d2c(args)
#define ObjectMotion_SetSpeedParameters_1_020029d4(a0, a1, a2) Call3(Func_02006d50, a0, a1, a2)
#define ObjectMotion_EnableActionAndSetCallback_2_020029d4(a0, a1) Call2(Func_02006d60, a0, a1)
#define BattleRuntime_WaitIfModeZero_1_020029d4(args...) Func_02006e8a(args)
#define ObjectMotion_SetSpeedParameters_2_020029d4(args...) Func_02006e9e(args)
#define BattleRuntime_WaitIfModeZero_2_020029d4(args...) Func_02006d3c(args)
#define ObjectMotion_SetSpeedParameters_3_020029d4(a0, a1, a2) Call3(Func_02006d7e, a0, a1, a2)
#define ObjectMotion_SetPositionAndReset_1_020029d4(a0, a1, a2) Call3(Func_02006dca, a0, a1, a2)
#define ObjectMotion_SetVariantCallbackAndInvokeObject_1_020029d4(args...) Func_02006e1c(args)
#define SceneWork_SetStepValue_1_020029d4(a0) Call1(Func_02006e3a, a0)
#define ObjectMotion_CallThenWaitForAnimationChange_1_020029d4(args...) Func_02006e18(args)
#define BattleRuntime_WaitIfModeZero_3_020029d4(args...) Func_02006d86(args)
#define ObjectMotion_ArmCallback_1_020029d4(a0, a1, a2) Call3(Func_02006e82, a0, a1, a2)
#define BattleEffect_SpawnLinkedResourceObject_1_020029d4(a0, a1, a2) Call3(Func_02006ea4, a0, a1, a2)
#define BattleRuntime_RunThenWaitIfModeZero_1_020029d4(args...) Func_02006e86(args)
#define ObjectMotion_CallThenWaitForAnimationChange_2_020029d4(args...) Func_02006e54_a(args)
#define ObjectMotion_CallThenWaitForAnimationChange_3_020029d4(args...) Func_02006e5c(args)
#define ObjectMotion_SetPositionAndReset_2_020029d4(a0, a1, a2) Call3(Func_02006e48, a0, a1, a2)
#define ObjectMotion_SetPositionAndReset_3_020029d4(a0, a1, a2) Call3(Func_02006e54_b, a0, a1, a2)
#define BattleRuntime_WaitIfModeZero_4_020029d4(args...) Func_02006df0(args)
#define ObjectMotion_SetPositionAndReset_4_020029d4(a0, a1, a2) Call3(Func_02006e74, a0, a1, a2)
#define ObjectMotion_SetHorizontalPositionWithTerrain_4(args...) Func_02006e8e(args)
#define GameFlag_Set_1_020029d4(a0) Call1(Func_02006e02, a0)
#define GameFlag_Clear_1(a0) Call1(Func_02006e10, a0)
#define BattleRuntime_ScheduleShoulderButtonModeUpdate_1_020029d4(args...) Func_02006e2c(args)
#define SCENE_PHASE (*(s32 *)(*(u8 *volatile *)Data_03001ebc + 0x1c0))
#define BattleRuntime_Reset_1_02002b7c(args...) Func_02006e6c(args)
#define ObjectGroup_ConfigureChildValue_1(args...) Func_02006f2c(args)
#define Scene_GetRecord_1_02002b7c(args...) Func_02006e9a(args)
#define Object_NotifyLastActiveOfEvent_1_02002b7c(a0) Call1(Func_02006e96, a0)
#define ObjectMotion_SetHorizontalPositionWithTerrain_1_02002b7c(a0, a1, a2) Call3(Func_02006f1a, a0, a1, a2)
#define Scene_GetRecord_2_02002b7c(args...) Func_02006ec0(args)
#define ObjectMotion_SetHorizontalPositionWithTerrain_2_02002b7c(a0, a1, a2) Call3(Func_02006f34, a0, a1, a2)
#define Scene_GetRecord_3_02002b7c(args...) Func_02006eda_a(args)
#define ObjectMotion_SetActionVariant_1(a0, a1) Value2(Func_02006fc0, a0, a1)
#define ObjectMotion_SetHorizontalPositionWithTerrain_3_02002b7c(a0, a1, a2) Call3(Func_02006f56, a0, a1, a2)
#define Scene_GetRecord_4(args...) Func_02006efc(args)
#define BattleRuntime_WaitIfModeZero_1_02002b7c(args...) Func_02007040(args)
#define ObjectMotion_SetSpeedParameters_1_02002b7c(args...) Func_02007054(args)
#define BattleRuntime_WaitIfModeZero_2_02002b7c(args...) Func_02006ef2(args)
#define ObjectMotion_Launch_1_02002b7c(args...) Func_02006f9c(args)
#define ObjectMotion_Launch_2(args...) Func_02006fa6(args)
#define SceneWork_SetStepValue_1_02002b7c(a0) Call1(Func_02006fd4, a0)
#define ObjectMotion_CallThenWaitForAnimationChange_1_02002b7c(args...) Func_02006fb2(args)
#define ObjectMotion_SetSpeedParameters_2_02002b7c(a0, a1, a2) Call3(Func_02006f60, a0, a1, a2)
#define ObjectMotion_SetPositionAndReset_1_02002b7c(a0, a1, a2) Call3(Func_02006faa, a0, a1, a2)
#define ObjectMotion_ArmCallback_1_02002b7c(a0, a1, a2) Value3(Func_0200702c, a0, a1, a2)
#define ObjectMotion_SetVariantCallbackAndInvokeObject_1_02002b7c(args...) Func_02006ff4(args)
#define SceneEffect_InitSlotsEightToNineteen Func_02003284
#define SceneEffect_AdvanceSlotByValueBand Func_02003558
#define SceneEffect_SelectSlotValueAndPosition Func_020035ec
#define SceneState_FillTableDb70ByChecks Func_020033b8
#define DialogueLayout_ConfigureTwoRegions Func_020039b8
#define FieldScene_RunStepThen10 Func_020039ec
#define FieldScene_CallPairWith10 Func_02003a00
#define BattleRuntime_Reset_1_02003a0c(args...) Func_02007cfc(args)
#define Object_NotifyLastActiveOfEvent_1_02003a0c(a0) Call1(Func_02007d20, a0)
#define ObjectMotion_SetHorizontalPositionWithTerrain_1_02003a0c(a0, a1, a2) Call3(Func_02007da2, a0, a1, a2)
#define Scene_GetRecord_1_02003a0c(args...) Func_02007d48(args)
#define ObjectMotion_SetHorizontalPositionWithTerrain_2_02003a0c(a0, a1, a2) Call3(Func_02007dbc, a0, a1, a2)
#define Scene_GetRecord_2_02003a0c(args...) Func_02007d62(args)
#define ObjectMotion_SetHorizontalPositionWithTerrain_3_02003a0c(a0, a1, a2) Call3(Func_02007dd4, a0, a1, a2)
#define Scene_GetRecord_3_02003a0c(args...) Func_02007d7a(args)
#define ObjectMotion_SetHorizontalPositionWithTerrain_4_02003a0c(args...) Func_02007de6(args)
#define ObjectGroup_ConfigureChildValue_1_02003a0c(args...) Func_02007e26_a(args)
#define Scene_GetRecord_4_02003a0c(args...) Func_02007d94(args)
#define BattleRuntime_WaitIfModeZero_1_02003a0c(args...) Func_02007ee2(args)
#define ObjectMotion_SetSpeedParameters_1_02003a0c(args...) Func_02007ef6(args)
#define ObjectMotion_SetSpeedParameters_2_02003a0c(a0, a1, a2) Call3(Func_02007dd0, a0, a1, a2)
#define ObjectMotion_SetPositionAndReset_1_02003a0c(a0, a1, a2) Call3(Func_02007e1c, a0, a1, a2)
#define ObjectMotion_ArmCallback_1_02003a0c(args...) Func_02007e9e(args)
#define SceneWork_SetStepValue_1_02003a0c(a0) Call1(Func_02007e7c, a0)
#define ObjectMotion_SetVariantCallbackAndInvokeObject_1_02003a0c(args...) Func_02007e74_a(args)
#define Object_SetModeById_1_02003a0c(args...) Func_02007e5c(args)
#define ObjectMotion_ArmCallback_2_02003a0c(a0, a1, a2) Call3(Func_02007ece, a0, a1, a2)
#define ObjectMotion_ArmCallback_3_02003a0c(a0, a1, a2) Call3(Func_02007eda, a0, a1, a2)
#define BattleEffect_SpawnLinkedResourceObject_1_02003a0c(a0, a1, a2) Call3(Func_02007efc, a0, a1, a2)
#define BattleEffect_SpawnLinkedResourceObject_2_02003a0c(a0, a1, a2) Call3(Func_02007f06, a0, a1, a2)
#define Object_SetModeById_2_02003a0c(args...) Func_02007e96(args)
#define BattleRuntime_WaitIfModeZero_2_02003a0c(a0, a1) Call2(Func_02007f26, a0, a1)
#define BattleRuntime_WaitIfModeZero_3_02003a0c(a0, a1) Call2(Func_02007f30, a0, a1)
#define BattleRuntime_WaitIfModeZero_4_02003a0c(args...) Func_02007e26_b(args)
#define BattleEffect_SpawnLinkedResourceObject_3_02003a0c(a0, a1, a2) Call3(Func_02007f3a, a0, a1, a2)
#define ObjectMotion_SetSpeedParameters_3_02003a0c(a0, a1, a2) Call3(Func_02007e74_b, a0, a1, a2)
#define ObjectMotion_SetPositionAndReset_2_02003a0c(a0, a1, a2) Call3(Func_02007ec0, a0, a1, a2)
#define ObjectMotion_ArmCallback_4_02003a0c(args...) Func_02007f42(args)
#define ObjectMotion_SetSpeedParameters_4_02003a0c(a0, a1, a2) Call3(Func_02007e94, a0, a1, a2)
#define ObjectMotion_SetPositionAndReset_3_02003a0c(a0, a1, a2) Call3(Func_02007ede, a0, a1, a2)
#define ObjectMotion_ArmCallback_5_02003a0c(args...) Func_02007f60(args)
#define ObjectMotion_SetSpeedParameters_5(a0, a1, a2) Call3(Func_02007eb6, a0, a1, a2)
#define ObjectMotion_SetPositionAndReset_4_02003a0c(a0, a1, a2) Call3(Func_02007f02, a0, a1, a2)
#define ObjectMotion_SetVariantCallbackAndInvokeObject_2_02003a0c(args...) Func_02007f4a(args)
#define ObjectMotion_CallThenWaitForAnimationChange_1_02003a0c(args...) Func_02007f40(args)
#define ObjectMotion_ArmCallback_6_02003a0c(a0, a1, a2) Call3(Func_02007fb4, a0, a1, a2)
#define Object_SetModeById_3_02003a0c(args...) Func_02007f5c(args)
#define ObjectMotion_SetPositionAndReset_5_02003a0c(a0, a1, a2) Call3(Func_02007f56, a0, a1, a2)
#define ObjectMotion_ArmCallback_7_02003a0c(args...) Func_02007fd8(args)
#define ObjectMotion_SetPositionAndReset_6(a0, a1, a2) Call3(Func_02007f6c, a0, a1, a2)
#define ObjectMotion_SetPositionAndReset_7(a0, a1, a2) Call3(Func_02007f78, a0, a1, a2)
#define ObjectMotion_ResetAndSetPositionInMode2_1(a0, a1, a2) Call3(Func_02007f7c, a0, a1, a2)
#define BattleRuntime_WaitIfModeZero_5_02003a0c(args...) Func_02007f12(args)
#define ObjectMotion_SetSpeedParameters_6(args...) Func_02008082(args)
#define BattleRuntime_Reset_1_02003c88(args...) Func_02007f78_a(args)
#define ObjectMotion_SetSpeedParameters_1_02003c88(a0, a1, a2) Call3(Func_02007fb6, a0, a1, a2)
#define ObjectMotion_SetPositionAndReset_1_02003c88(a0, a1, a2) Call3(Func_02008000, a0, a1, a2)
#define ObjectMotion_ArmCallback_1_02003c88(a0, a1, a2) Call3(Func_02008084, a0, a1, a2)
#define Scene_GetRecord_1_02003c88(a0) Value1(Func_02007fc2, a0)
#define ObjectMotion_SetHorizontalPositionWithTerrain_1_02003c88(args...) Func_02008030(args)
#define Scene_GetRecord_2_02003c88(a0) Value1(Func_02007fd6, a0)
#define ObjectMotion_SetHorizontalPositionWithTerrain_2_02003c88(args...) Func_02008044(args)
#define Scene_GetRecord_3_02003c88(a0) Value1(Func_02007fea, a0)
#define ObjectMotion_SetHorizontalPositionWithTerrain_3_02003c88(args...) Func_02008058(args)
#define ObjectMotion_SetSpeedParameters_2_02003c88(a0, a1, a2) Call3(Func_02008012, a0, a1, a2)
#define ObjectMotion_SetSpeedParameters_3_02003c88(a0, a1, a2) Call3(Func_02008020, a0, a1, a2)
#define ObjectMotion_SetSpeedParameters_4_02003c88(a0, a1, a2) Call3(Func_0200802a_a, a0, a1, a2)
#define ObjectMotion_ResetAndSetPositionInMode2_1_02003c88(a0, a1, a2) Call3(Func_0200806e, a0, a1, a2)
#define ObjectMotion_ResetAndSetPositionInMode2_2(a0, a1, a2) Call3(Func_02008078, a0, a1, a2)
#define ObjectMotion_SetPositionAndReset_2_02003c88(a0, a1, a2) Call3(Func_0200808c, a0, a1, a2)
#define Object_SetModeById_1_02003c88(args...) Func_020080ac(args)
#define Object_SetModeById_2_02003c88(args...) Func_020080b4(args)
#define BattleRuntime_WaitIfModeZero_1_02003c88(args...) Func_0200802a_b(args)
#define ObjectMotion_ArmCallback_2_02003c88(a0, a1, a2) Call3(Func_02008126, a0, a1, a2)
#define ObjectMotion_ArmCallback_3_02003c88(a0, a1, a2) Call3(Func_02008132, a0, a1, a2)
#define SceneWork_SetStepValue_1_02003c88(a0) Call1(Func_02008122, a0)
#define BattleRuntime_RunThenWaitIfModeZero_1_02003c88(args...) Func_02008154(args)
#define BattleEffect_SpawnLinkedResourceObject_1_02003c88(a0, a1, a2) Call3(Func_02008188_a, a0, a1, a2)
#define ObjectMotion_SetVariantCallbackAndInvokeObject_1_02003c88(args...) Func_02008138(args)
#define ObjectMotion_SetSpeedParameters_5_02003c88(a0, a1) Value2(Func_02008160_a, a0, a1)
#define UiWork_WaitThenFinalizeCapacity_1(a0, a1) Value2(Func_020080b8, a0, a1)
#define ObjectMotion_CallThenWaitForAnimationChange_1_02003c88(args...) Func_0200813c(args)
#define Object_SetModeById_3_02003c88(args...) Func_0200814c(args)
#define ObjectMotion_SetVariantCallbackAndInvokeObject_2_02003c88(args...) Func_02008184(args)
#define ObjectMotion_SetSpeedParameters_6_02003c88(args...) Func_020081ac(args)
#define UiWork_WaitThenFinalizeCapacity_2(a0, a1) Value2(Func_02008104, a0, a1)
#define ObjectMotion_SetVariantCallbackAndInvokeObject_3_02003c88(args...) Func_020081a0(args)
#define SceneWork_SetStepValue_2_02003c88(a0) Call1(Func_020081be, a0)
#define ObjectMotion_SetSpeedParameters_7(args...) Func_020081ce(args)
#define BattleRuntime_WaitIfModeZero_2_02003c88(args...) Func_02008106(args)
#define ObjectMotion_CallThenWaitForAnimationChange_2_02003c88(args...) Func_020081a6(args)
#define SceneWork_SetStepValue_3(a0) Call1(Func_020081dc, a0)
#define ObjectMotion_SetSpeedParameters_8(a0, a1, a2) Call3(Func_02008160_b, a0, a1, a2)
#define ObjectMotion_SetSpeedParameters_9(a0, a1, a2) Call3(Func_0200816e, a0, a1, a2)
#define Scene_GetRecord_4_02003c88(args...) Func_02008164(args)
#define ObjectMotion_SetPositionAndReset_3_02003c88(a0, a1, a2) Call3(Func_020081ca, a0, a1, a2)
#define BattleRuntime_WaitIfModeZero_3_02003c88(args...) Func_02008158(args)
#define Scene_GetRecord_5(args...) Func_02008186(args)
#define Scene_GetRecord_6(a0) Value1(Func_02008196, a0)
#define ObjectMotion_SetPositionAndReset_4_02003c88(a0, a1, a2) Call3(Func_020081fa, a0, a1, a2)
#define BattleRuntime_WaitIfModeZero_4_02003c88(args...) Func_02008188_b(args)
#define Scene_GetRecord_7(args...) Func_020081b6(args)
#define ObjectMotion_ArmCallback_4_02003c88(a0, a1, a2) Call3(Func_02008292, a0, a1, a2)
#define ObjectMotion_ResetAndSetPositionInMode2_3(a0, a1, a2) Call3(Func_0200822c, a0, a1, a2)
#define ObjectMotion_ResetAndSetPositionInMode2_4(a0, a1, a2) Call3(Func_02008236, a0, a1, a2)
#define ObjectMotion_SetPositionAndReset_5_02003c88(a0, a1, a2) Call3(Func_02008248, a0, a1, a2)
#define ObjectTable_DestroyById_1(args...) Func_02008206(args)
#define ObjectTable_DestroyById_2(args...) Func_0200820c(args)
#define ObjectTable_DestroyById_3(args...) Func_02008212(args)
#define GameFlag_Set_1_02003c88(a0) Call1(Func_020081d8, a0)
#define BattleRuntime_ScheduleShoulderButtonModeUpdate_1_02003c88(args...) Func_020081fc(args)
#define ACTOR_FLAGS_OFFSET 90
#define BattleRuntime_Reset_1_02003f30(args...) Func_02008224(args)
#define Object_NotifyLastActiveOfEvent_1_02003f30(a0) Call1(Func_0200823a, a0)
#define BattleRuntime_WaitIfModeZero_1_02003f30(args...) Func_02008384(args)
#define ObjectMotion_SetSpeedParameters_1_02003f30(a0, a1, a2) Call3(Func_02008272, a0, a1, a2)
#define ObjectMotion_SetPositionAndReset_1_02003f30(a0, a1, a2) Call3(Func_020082be, a0, a1, a2)
#define BattleEffect_SpawnLinkedResourceObject_1_02003f30(a0, a1, a2) Call3(Func_0200835c, a0, a1, a2)
#define ObjectMotion_SetVariantCallbackAndInvokeObject_1_02003f30(args...) Func_0200830e(args)
#define SceneWork_SetStepValue_1_02003f30(a0) Call1(Func_02008334, a0)
#define ObjectMotion_SetSpeedParameters_2_02003f30(a0, a1) Value2(Func_02008344, a0, a1)
#define ObjectMotion_ArmCallback_1_02003f30(a0, a1, a2) Call3(Func_02008370, a0, a1, a2)
#define UiWork_WaitThenFinalizeCapacity_1_02003f30(a0, a1) Value2(Func_020082a8, a0, a1)
#define BattleRuntime_ScheduleShoulderButtonModeUpdate_1_02003f30(args...) Func_020082a6(args)
#define BattleEventRuntime_ProcessAction_1(a0, a1) Value2(Func_0200839a, a0, a1)
#define ObjectMotion_SetHorizontalPositionWithTerrain_1_02003f30(a0, a1, a2) Call3(Func_0200834c, a0, a1, a2)
#define ObjectMotion_SetSpeedParameters_3_02003f30(a0, a1, a2) Call3(Func_02008306, a0, a1, a2)
#define ObjectMotion_SetPositionAndReset_2_02003f30(a0, a1, a2) Call3(Func_02008352, a0, a1, a2)
#define ObjectMotion_SetPositionAndReset_3_02003f30(a0, a1, a2) Call3(Func_0200835e, a0, a1, a2)
#define ObjectMotion_ArmCallback_2_02003f30(a0, a1, a2) Call3(Func_020083e2, a0, a1, a2)
#define ObjectMotion_ArmCallback_3_02003f30(a0, a1, a2) Call3(Func_020083ee, a0, a1, a2)
#define ObjectMotion_ArmCallback_4_02003f30(a0, a1, a2) Call3(Func_020083fa, a0, a1, a2)
#define ObjectMotion_Launch_1_02003f30(args...) Func_020083bc(args)
#define Object_SetModeById_1_02003f30(args...) Func_020083b4(args)
#define BattleEvent_RunActionAndWait_1(args...) Func_02008404(args)
#define ObjectMotion_SetHorizontalPositionWithTerrain_2_02003f30(a0, a1, a2) Call3(Func_020083c0, a0, a1, a2)
#define ObjectMotion_SetSpeedParameters_4_02003f30(a0, a1, a2) Call3(Func_0200837e, a0, a1, a2)
#define ObjectMotion_SetPositionAndReset_4_02003f30(a0, a1, a2) Call3(Func_020083ca, a0, a1, a2)
#define ObjectMotion_ArmCallback_5_02003f30(a0, a1, a2) Call3(Func_02008450, a0, a1, a2)
#define ObjectMotion_ArmCallback_6_02003f30(a0, a1, a2) Call3(Func_02008462_a, a0, a1, a2)
#define ObjectMotion_ArmCallback_7_02003f30(a0, a1, a2) Call3(Func_0200846e, a0, a1, a2)
#define BattleEffect_SpawnLinkedResourceObject_2_02003f30(a0, a1, a2) Call3(Func_02008490, a0, a1, a2)
#define ObjectMotion_SetVariantCallbackAndInvokeObject_2_02003f30(args...) Func_02008440(args)
#define ObjectMotion_SetVariantCallback_1_02003f30(args...) Func_02008446_a(args)
#define ObjectMotion_ArmCallback_8(a0, a1, a2) Value3(Func_0200849e, a0, a1, a2)
#define Object_SetModeById_2_02003f30(args...) Func_02008446_b(args)
#define ObjectMotion_Launch_2_02003f30(args...) Func_02008466(args)
#define Object_SetModeById_3_02003f30(args...) Func_0200845e(args)
#define ObjectMotion_SetPositionAndReset_5_02003f30(a0, a1, a2) Call3(Func_02008458, a0, a1, a2)
#define ObjectMotion_ArmCallback_9(a0, a1, a2) Call3(Func_020084dc, a0, a1, a2)
#define BattleEffect_SpawnLinkedResourceObject_3_02003f30(a0, a1, a2) Call3(Func_02008506, a0, a1, a2)
#define ObjectMotion_SetVariantCallback_2_02003f30(args...) Func_020084ae(args)
#define ObjectMotion_CallThenWaitForAnimationChange_1_02003f30(args...) Func_020084ac(args)
#define ObjectMotion_SetVariantCallbackAndInvokeObject_3_02003f30(args...) Func_020084d4(args)
#define ObjectMotion_SetSpeedParameters_5_02003f30(a0, a1, a2) Call3(Func_0200846c, a0, a1, a2)
#define ObjectMotion_EnableActionAndSetCallback_1_02003f30(args...) Func_0200847e(args)
#define ObjectMotion_SetSpeedParameters_6_02003f30(a0, a1, a2) Call3(Func_02008480, a0, a1, a2)
#define ObjectMotion_SetPositionAndReset_6_02003f30(a0, a1, a2) Call3(Func_020084cc, a0, a1, a2)
#define ObjectMotion_EnableActionAndSetCallback_2_02003f30(args...) Func_0200849c(args)
#define BattleRuntime_WaitIfModeZero_2_02003f30(args...) Func_02008462_b(args)
#define ObjectMotion_EnableActionAndSetCallback_3(args...) Func_020084aa(args)
#define BattleRuntime_WaitIfModeZero_3_02003f30(args...) Func_02008470(args)
#define ObjectMotion_CallThenWaitForAnimationChange_2_02003f30(args...) Func_0200852a(args)
#define ObjectMotion_CallThenWaitForAnimationChange_3_02003f30(args...) Func_02008532(args)
#define ObjectMotion_SetSpeedParameters_7_02003f30(args...) Func_02008618(args)
#define SCENE_WORK (*(u8 **)0x03001ebc)
#define SCENE_PHASE_02003f30 (*(s32 *)(SCENE_WORK + 0x1c0))
#define FieldScene_RunScene3af_02001a98 Func_02001a98
#define FieldScene_RunScene3af_02001b58 Func_02001b58
#define FieldScene_RunScene3af_02001c14 Func_02001c14
#define FieldScene_RunActorTwentyDialogueSequence Func_02001d0c
#define FieldScene_ConfigureLeadActors Func_020029d4
#define FieldScene_ConfigureThreeActors Func_02002b7c
#define FieldScene_ConfigureFourActorPresentation Func_02003a0c
#define FieldScene_RunThreeActorEncounter Func_02003c88
#define FieldScene_RunEncounterClosingSequence Func_02003f30
#define FieldScene_RunScene3af_02004218 Func_02004218

#include "resource_3af_motion.h"

union Slot {
    s32 w;
    s16 h[2];
};

extern u8 Data_0200c994[];
extern u8 Data_0200cb44[];
extern u8 Data_0200cb64[];
extern u8 Value_0000093e;
extern u8 Value_00000927;
extern u8 Value_00000928;
extern u8 Value_00000911;
extern u8 Value_00000925;
extern u8 Value_00000922;
extern u8 Data_0200d508[];
extern u8 Data_0200cef0[];
extern u8 Data_0200d028[];
extern u8 Data_0200ccf8[];
extern u8 Data_0200cba8[];
extern u8 Data_0200d9d0[];
extern u8 Data_0200da54[];
extern u8 Data_0200d958[];
extern u8 Data_0200d778[];
extern s32 Data_0200db08[];
extern u8 Data_00001d40[];
extern unsigned char Value_00001f00;
extern u8 *Data_03001e70;
extern u32 Data_0200db58;
extern u32 Data_0200db38;
extern u16 Data_0200db30[];
extern u16 Data_0200db40[];
extern s32 Data_0200db90[];
extern u8 Data_0200d160[];
extern u8 Data_00002014[];
extern u8 Data_0200c918[];

s32 Func_020042e6(s32, s16);
s32 Func_020042f8(void);
void Func_0200435e(s32, s32);
void Func_02007bfa(s32, s32, s32, s32, s32, s32);
void Func_02007c0a(s32, s32, s32, s32, s32, s32);
void Func_02007c1c(s32, s32, s32, s32, s32, s32);
void Func_02007c2c(s32, s32, s32, s32, s32, s32);
void Func_02007c48(s32, s32, s32, s32, s32, s32);
s32 Func_02004792(void);
s32 Func_020047b0(void);
s32 Func_02004b0e(void);
s32 Func_020047e2(void);
u8 *Func_02007692(s32);
void Func_02007778(s32, s32);
void Func_02007640(u8 *, s32);
s32 Func_02004c22(s32);
s32 Func_02004c30(s32);
s32 Func_02004c3e(s32);
s32 Func_02004c4e(s32);
s32 Func_02004c58(s32);
s32 Func_02004c76(s32);
s32 Func_02004cce(s32);
s32 Func_02004cde(s32);
s32 Func_02004cec(s32);
s32 Func_0200744e(s32);
s32 Func_02007764(s32);
s32 Func_02004cfe();
s32 Func_02004d1e();
s32 Func_02004d38_a();
s32 Func_02004d38_b();
s32 Func_02004d98();
s32 Func_02004da6();
void Func_02004dc0();
s32 Func_02004dc6();
void Func_02004dca();
s32 Func_02004de0();
void Func_02004e08();
void Func_02004e0c();
void Func_02004e20();
void Func_02004e2a();
s32 Func_02004e40();
void Func_02004e42();
void Func_02004e46();
void Func_02004e68();
void Func_02004e6c();
void Func_02004e72();
void Func_02004e84();
void Func_02004e9e();
void Func_02004eb0();
void Func_02004eb4();
void Func_02004ec8();
void Func_02004ed2();
void Func_02004eea();
void Func_02004eee();
void Func_02004f14();
void Func_02004f2c();
void Func_02004f46();
s32 Func_02004d38();
s32 Func_02004de0_a();
s32 Func_02004de0_b();
void Func_02004e72_a();
s32 Func_02004e8c();
void Func_02004eb8();
void Func_02005026();
void Func_02004eaa();
s32 Func_02004ec4();
void Func_02004ef0();
void Func_0200505e();
void Func_02001c00();
void Func_020046f0();
void Func_0200470a();
void Func_02004724();
void Func_02004740();
void Func_0200474e();
void Func_0200475e();
void Func_02004764();
void Func_0200477a();
void Func_02004790();
void Func_02004796();
void Func_020047aa();
void Func_020047b8();
void Func_020047da();
void Func_0200480a_a();
void Func_0200480a_b();
void Func_02004822();
void Func_0200484a();
s32 Func_02004868();
void Func_02004880();
void Func_02004896();
void Func_0200489c();
void Func_020048d2();
void Func_020048fa();
void Func_0200497e();
void Func_02004a04();
s32 Func_02004f7e();
void Func_02004fa8();
void Func_02004fd4();
void Func_02005066();
void Func_0200508c();
void Func_020050a4();
void Func_020050aa();
void Func_020050ba();
void Func_020050d4_a();
void Func_020050d4_b();
void Func_020050de();
void Func_020050e0();
void Func_020050f8();
void Func_020050fc();
void Func_02005104();
void Func_02005106();
void Func_02005114();
void Func_0200511a();
void Func_02005142();
void Func_02005144();
void Func_02005156();
void Func_0200515c();
void Func_02005166();
void Func_02005168();
void Func_0200519c();
void Func_020051a4();
void Func_020051bc_a();
void Func_020051bc_b();
void Func_020051c4();
void Func_020051e4();
void Func_020051e8();
void Func_020051ec();
void Func_02005210();
void Func_02005214();
void Func_02005218();
void Func_0200521a();
void Func_0200522e();
void Func_0200523e();
void Func_02005244();
void Func_0200524e();
s32 Func_0200525e();
void Func_02005260();
void Func_0200526e();
s32 Func_02005270();
void Func_02005272_a();
void Func_02005272_b();
void Func_02005284();
void Func_02005288();
void Func_02005292();
void Func_0200529c();
void Func_020052a6();
void Func_020052be();
u8 *Func_020052d2();
void Func_020052e4();
void Func_020052ee();
void Func_020052f6_a();
void Func_020052f6_b();
void Func_0200530c();
void Func_02005314();
void Func_0200531e();
void Func_02005322();
u8 *Func_02005336();
void Func_02005338();
void Func_0200533c();
void Func_02005340();
s32 Func_02005342();
void Func_0200534c_a();
void Func_0200534c_b();
void Func_02005358();
void Func_02005362();
void Func_02005376();
void Func_02005378();
void Func_02005390();
void Func_020053a0();
void Func_020053bc();
void Func_020053d4();
void Func_020023b0();
void Func_02004afe();
s32 Func_02005372();
s32 Func_0200537c();
void Func_020053a4();
void Func_020053d2();
s32 Func_020053dc();
void Func_020053e6();
void Func_020053e8();
s32 Func_02005400();
void Func_02005420();
s32 Func_0200542e();
void Func_02005434();
void Func_02005438();
void Func_02005444();
u8 *Func_0200544e();
void Func_02005476();
void Func_0200548a();
void Func_02005492();
void Func_02005498();
void Func_020054c0();
void Func_020054cc();
void Func_020054ea();
void Func_02005510();
void Func_0200555c();
void Func_0200557e();
void Func_020024d8();
void Func_02004c26();
s32 Func_0200549a();
s32 Func_020054a4();
void Func_020054cc_a();
void Func_020054fa();
s32 Func_02005504();
void Func_0200550e();
void Func_02005510_a();
s32 Func_02005528();
void Func_02005548();
s32 Func_02005556();
void Func_0200555c_a();
void Func_02005560();
void Func_0200556c();
u8 *Func_02005576();
void Func_0200559e();
void Func_020055b2();
void Func_020055ba();
void Func_020055c0();
void Func_020055e8();
void Func_020055f4();
void Func_02005612();
void Func_02005638();
void Func_02005684();
void Func_020056a6();
void Func_02001f0a();
void Func_020055c2();
void Func_020055fa();
void Func_02005618();
void Func_02005668();
void Func_02005684_a();
void Func_020056ae();
void Func_020056b2();
void Func_020056e4();
void Func_020056fc_a();
void Func_020056fc_b();
void Func_02005706();
void Func_0200571a();
void Func_02005720();
void Func_02005722();
void Func_0200572c();
void Func_02005740();
void Func_02005762();
void Func_02005768();
s32 Func_02005796();
void Func_02005ac8();
void Func_02005b02();
void Func_02005b4c();
void Func_02005b62();
u8 *Func_02005ba0();
s32 Func_02005bac();
s32 Func_02005bae();
u8 *Func_02005bce();
void Func_02005bd0();
void Func_02005be0();
void Func_02005be2_a();
void Func_02005be2_b();
void Func_02005bee();
void Func_02005bf0();
void Func_02005bfa();
void Func_02005c00();
void Func_02005b8c();
void Func_02005bac_a();
void Func_02005c10();
void Func_02005c26();
s32 Func_02005c58();
u8 *Func_02005c64();
u8 *Func_02005c76();
void Func_02005c88();
void Func_02005c8c();
void Func_02005c9a();
void Func_02005ca6();
void Func_02005caa();
void Func_02005cb2();
void Func_02005cbe();
void Func_02004e88(void);
void Func_02004ea2(void);
void Func_02004f4e(s32 no);
s32 Func_02004f76(s32 no, s32 val);
void Func_020056c2(s32);
s32 Func_02005668_a(void);
s32 Func_02005674(void);
void Func_020056a0(void);
void Func_0200567e(s32);
void Func_0200286a(void);
void Func_02006a1c(s32, s32);
void Func_02006a2a(s32, s32);
void Func_02006a38(s32, s32);
void Func_02006a4a(s32, s32);
void Func_02006af2(s32, s32, s32);
void Func_02006afc(s32, s32, s32);
void Func_02006b06(s32, s32, s32);
void Func_02006b10(s32, s32, s32);
void Func_02006b1a(s32, s32, s32);
void Func_02006b24(s32, s32, s32);
void Func_02006b2e(s32, s32, s32);
void Func_02006b38(s32, s32, s32);
s32 *Func_02007aac_a();
s32 *Func_02007aac_b();
void Func_02007ab6(s32, s32, s32);
void Func_02007ac0(s32, s32, s32);
void Func_02007aca(s32, s32, s32);
void Func_02006ae4(s32);
void Func_02006aea(s32);
void Func_02006af0(s32);
s32 *Func_02007a90(s32);
s32 *Func_02007aa0(s32);
s32 *Func_02007ab8(s32);
void Func_02006b38_a(s32);
void Func_02006b3e(s32);
void Func_02006b44(s32);
void Func_02006b4a(s32);
s32 *Func_02007adc(s32);
s32 *Func_02007ae6(s32);
s32 *Func_02007aee(s32);
s32 *Func_02007af6(s32);
s32 *Func_02007b08(s32);
s32 *Func_02007b14(s32);
s32 *Func_02007b20(s32);
s32 *Func_02007b2c_a();
s32 *Func_02007b2c_b();
s32 *Func_02007b38(s32);
void Func_02007ba6(s32, s32, s32);
void Func_02007a9c(s32);
void Func_02007c44(s32, s32);
void Func_02007c4c(s32, s32);
void Func_02007c54(s32, s32);
void Func_02007c5c(s32, s32);
void Func_02007c64(s32, s32);
void Func_02007c6c(s32, s32);
void Func_02007c74(s32, s32);
u8 *Func_02007b8a(s32);
u8 *Func_02007b96(s32);
u8 *Func_02007ba0(s32);
u8 *Func_02007baa(s32);
u8 *Func_02007bb4(s32);
u8 *Func_02007bbe(s32);
u8 *Func_02007bc8(s32);
u8 *Func_02007bd2(s32);
void Func_02007c4a(s32, s32, s32);
void Func_02007b40(s32);
void Func_02006eda(s32, s32, s32);
void Func_02006ee4(s32, s32, s32);
void Func_02006eee(s32, s32, s32);
void Func_02006ef8(s32, s32, s32);
void Func_02006f02(s32, s32, s32);
void Func_02006f0c(s32, s32, s32);
void Func_02006f16(s32, s32, s32);
void Func_02006f20(s32, s32, s32);
void Func_02005b18(s32, s32, s32);
u8 *Func_02005abe(s32);
void Func_02005b32(s32, s32, s32);
u8 *Func_02005ad8(s32);
void Func_02005bbc(s32, s32);
void Func_02005b50(s32, s32, s32);
u8 *Func_02005af6(s32);
void Func_02005bdc(s32, s32);
void Func_02005b70(s32, s32, s32);
u8 *Func_02005b16(s32);
void Func_02005b88(s32, s32, s32);
u8 *Func_02005b2e(s32);
void Func_02005b9a(s32, s32, s32);
void Func_02005ba4(s32, s32, s32);
void Func_02005bae_a(s32, s32, s32);
void Func_02003760();
void Func_02005d10();
void Func_02005d88();
void Func_02005da0();
void Func_02005dc2();
s32 Func_02005e16();
void Func_02005e2a();
s32 Func_02005e2e();
void Func_02005e34();
void Func_02005e3e();
void Func_02005e48();
void Func_02005e52();
void Func_02005e5c();
void Func_02005e66();
void Func_02005e70();
void Func_02005e88();
void Func_02005ec2();
void Func_02005f4e();
void Func_02003818();
void Func_02005dc4();
void Func_02005dec();
void Func_02005dfe();
void Func_02005e10();
void Func_02005e20();
void Func_02005e2e_a();
void Func_02005e48_a();
void Func_02005e58();
void Func_02005e5e();
s32 Func_02005e90();
void Func_02005edc();
void Func_02005ee2_a();
void Func_02005ee2_b();
s32 Func_02005ee4();
s32 Func_02005efc();
void Func_02005f22();
void Func_02005f2a();
void Func_02005f34();
void Func_02005f3e();
void Func_02005f56();
void Func_02005f9c();
void Func_020027fa();
void Func_020056ac();
void Func_02005f22_a();
void Func_02005f76();
void Func_02005f86();
void Func_02005fbc();
void Func_02005fc2();
void Func_02005fce();
void Func_02005fda();
void Func_02005fe4();
void Func_02006018();
void Func_0200603a();
void Func_0200604a();
void Func_02006068();
void Func_02006070_a();
void Func_02006070_b();
void Func_02006084();
void Func_02006092();
void Func_0200610a();
void Func_02006120();
void Func_0200612c();
void Func_02005ffc(void);
void Func_02006012(s32);
void Func_02005f78(s32);
void Func_020060c8(s32, s32);
s32 Func_02006036(s32);
void Func_02005fd4(s32, s32);
void Func_0200617e(void);
void Func_02006192(void);
void Func_02006030(s32);
void Func_020060e8(s32, s32);
void Func_02006106(s32);
void Func_02006128(s32, s32, s32);
void Func_0200576e(s32, s32);
void Func_020060fc(s32, s32, s32);
void Func_0200610c(s32, s32);
void Func_0200614e(s32, s32, s32);
void Func_020061d2(void);
void Func_020061de(void);
void Func_020061bc(s32);
void Func_020036be();
void Func_02006476();
s32 Func_02006480_a();
s32 Func_02006480_b();
void Func_020064bc();
void Func_020064ca();
void Func_02006502();
void Func_02006c40();
void Func_02006cc4();
void Func_02006cda();
void Func_02006d3c();
void Func_02006d3e();
void Func_02006d50();
void Func_02006d5c();
void Func_02006d60();
void Func_02006d68();
void Func_02006d74();
void Func_02006d7e();
void Func_02006d86();
void Func_02006dca();
void Func_02006df0();
void Func_02006e02();
void Func_02006e10();
void Func_02006e18();
void Func_02006e1c();
void Func_02006e2c();
void Func_02006e3a();
void Func_02006e48();
void Func_02006e54_a();
void Func_02006e54_b();
void Func_02006e5c();
void Func_02006e74();
void Func_02006e82();
void Func_02006e86();
void Func_02006e8a();
void Func_02006e8e();
void Func_02006e9e();
void Func_02006ea4();
u8 *Func_02006d1a();
u8 *Func_02006d2c();
void Func_0200661a();
void Func_02006652();
void Func_02006dfc();
void Func_02006e38();
void Func_02006e6c();
void Func_02006e96();
s32 Func_02006e9a();
s32 Func_02006ec0();
s32 Func_02006eda_a();
void Func_02006ef2();
s32 Func_02006efc();
void Func_02006f1a();
void Func_02006f2c();
void Func_02006f34();
void Func_02006f56();
void Func_02006f60();
void Func_02006f9c();
void Func_02006fa6();
void Func_02006faa();
void Func_02006fb2();
s32 Func_02006fc0();
void Func_02006fd4();
void Func_02006ff4();
s32 Func_0200702c();
void Func_02007040();
void Func_02007054();
void Func_02007090();
u8 *Func_02007596(s32);
u8 *Func_020075a2(s32);
u8 *Func_020075ac(s32);
u8 *Func_020075b6(s32);
void Func_02006634(s32);
void Func_0200663a(s32);
void Func_02006640(s32);
void Func_02006646(s32);
void Func_0200664c(s32);
void Func_02006652_a(s32);
void Func_02006658(s32);
void Func_0200665e(s32);
s32 *Func_020075f0(s32);
s32 *Func_020075fc(s32);
s32 *Func_02007606(s32);
s32 *Func_02007610(s32);
void Func_0200668e(s32);
void Func_02006694(s32);
void Func_0200669a(s32);
void Func_020066a0(s32);
s32 *Func_02007632(s32);
s32 *Func_0200763c(s32);
s32 *Func_02007644(s32);
s32 *Func_0200764c(s32);
s32 *Func_02007654(s32);
s32 *Func_0200765e(s32);
s32 *Func_02007668(s32);
s32 *Func_02007672(s32);
void Func_02006726(void);
void Func_02007902(s32, s32);
void Func_02007922(s32, s32);
void Func_02007936(s32, s32);
s32 Func_0200790e_a(s32);
s32 Func_0200790e_b(s32);
void Func_02007a40(s32, s32);
void Func_02007a54(s32, s32);
void Func_02007a68(s32, s32);
void Func_02007a7c(s32, s32);
s32 Func_0200791c(s32);
s32 Func_02007940(s32);
s32 Func_02007948(s32);
s32 Func_02006514(s32);
s32 Func_02006832(s32);
s32 Func_02006532(s32);
s32 Func_02006850(s32);
s32 Func_0200654e(s32);
s32 Func_0200686c(s32);
s32 Func_02006572(s32);
s32 Func_02006890(s32);
s32 Func_02006588(s32);
s32 Func_020068a6(s32);
void Func_02007c62(s32, s32, s32, s32, s32, s32);
void Func_02007c7c(s32, s32, s32, s32, s32, s32);
void Func_02007dae(s32, s32);
void Func_02007cdc(s32);
void Func_02007dda(s32, s32, s32);
void Func_020074c2();
void Func_020074da();
void Func_02007514();
void Func_020075a8();
s32 Func_020075ae();
void Func_020075c0();
void Func_020075ce();
void Func_020075da();
void Func_02007c86();
void Func_02007cf0();
void Func_02007cfc();
void Func_02007d20();
void Func_02007d32();
s32 Func_02007d7a();
s32 Func_02007d94();
void Func_02007da2();
void Func_02007dbc();
void Func_02007dd0();
void Func_02007dd4();
void Func_02007de6();
void Func_02007e1c();
void Func_02007e26_a();
void Func_02007e26_b();
void Func_02007e5c();
void Func_02007e74_a();
void Func_02007e74_b();
void Func_02007e7c();
void Func_02007e94();
void Func_02007e96();
void Func_02007e9e();
void Func_02007eb6();
void Func_02007ec0();
void Func_02007ece();
void Func_02007eda();
void Func_02007ede();
void Func_02007ee2();
void Func_02007ef6();
void Func_02007efc();
void Func_02007f02();
void Func_02007f06();
void Func_02007f12();
void Func_02007f26();
void Func_02007f30();
void Func_02007f3a();
void Func_02007f40();
void Func_02007f42();
void Func_02007f4a();
void Func_02007f56();
void Func_02007f5c();
void Func_02007f60();
void Func_02007f6c();
void Func_02007f78();
void Func_02007f7c();
void Func_02007fb4();
void Func_02007fd8();
void Func_02008060();
void Func_02008076();
void Func_02008082();
s32 Func_02007768_a();
void Func_02007768_b();
void Func_02007770();
s32 Func_02007786();
void Func_020077b2();
void Func_020077ca();
s32 Func_020077d0();
s32 Func_020077e8();
void Func_02007822();
void Func_020078ba();
void Func_020078c8();
void Func_02007f78_a();
void Func_02007fb6();
s32 Func_02007fc2();
s32 Func_02007fd6();
s32 Func_02007fea();
void Func_02008000();
void Func_02008012();
void Func_02008020();
void Func_0200802a_a();
void Func_0200802a_b();
void Func_02008030();
void Func_02008044();
void Func_02008058();
void Func_0200806e();
void Func_02008078();
void Func_02008084();
void Func_0200808c();
void Func_020080ac();
void Func_020080b4();
s32 Func_020080b8();
s32 Func_02008104();
void Func_02008106();
void Func_02008122();
void Func_02008126();
void Func_02008132();
void Func_02008138();
void Func_0200813c();
void Func_0200814c();
void Func_02008154();
void Func_02008158();
s32 Func_02008160_a();
s32 Func_02008160_b();
s32 Func_02008164();
void Func_0200816e();
void Func_02008184();
u8 *Func_02008186();
void Func_02008188_a();
void Func_02008188_b();
s32 Func_02008196();
void Func_020081a0();
void Func_020081a6();
void Func_020081ac();
u8 *Func_020081b6();
void Func_020081be();
void Func_020081ca();
void Func_020081ce();
void Func_020081d8();
void Func_020081dc();
void Func_020081fa();
void Func_020081fc();
void Func_02008206();
void Func_0200820c();
void Func_02008212();
void Func_0200822c();
void Func_02008236();
void Func_02008248();
void Func_02008292();
void Func_02004b8a();
void Func_02007982();
void Func_0200799a();
s32 Func_02007a2e();
void Func_02007a6e();
void Func_02007a9e();
void Func_02007aac();
void Func_02007ac4();
void Func_02007adc_a();
void Func_02007afa();
void Func_02007b14_a();
void Func_02007b32();
void Func_02007b38_a();
void Func_02007b88();
void Func_02007b96_a();
s32 Func_02007ba6_a();
void Func_020081a0_a();
void Func_02008224();
void Func_0200823a();
void Func_02008272();
void Func_020082a6();
s32 Func_020082a8();
void Func_020082be();
void Func_02008306();
void Func_0200830e();
void Func_02008334();
s32 Func_02008344();
void Func_0200834c();
void Func_02008352();
void Func_0200835c();
void Func_0200835e();
void Func_02008370();
void Func_0200837e();
void Func_02008384();
s32 Func_0200839a();
void Func_020083b4();
void Func_020083bc();
void Func_020083c0();
void Func_020083ca();
void Func_020083e2();
void Func_020083ee();
void Func_020083fa();
void Func_02008404();
void Func_02008440();
void Func_02008446_a();
void Func_02008446_b();
void Func_02008450();
void Func_02008458();
void Func_0200845e();
void Func_02008462_a();
void Func_02008462_b();
void Func_02008466();
void Func_0200846c();
void Func_0200846e();
void Func_02008470();
void Func_0200847e();
void Func_02008480();
void Func_02008490();
void Func_0200849c();
s32 Func_0200849e();
void Func_020084aa();
void Func_020084ac();
void Func_020084ae();
void Func_020084cc();
void Func_020084d4();
void Func_020084dc();
void Func_02008506();
void Func_0200852a();
void Func_02008532();
void Func_020085f6();
void Func_0200860c();
void Func_02008618();
void Func_020084aa_a();
void Func_020084b2();
s32 Func_0200854e();
void Func_020085a8();
void Func_0200863e();

/* Loader-relocated overlay calls: each symbol names the pre-relocation call
 * word the image holds. */

/* Resolved engine calls: each pseudo symbol is the per-site call word the
 * overlay image holds (a word can serve two sites with different targets),
 * and the macro names the engine function the site reaches through the
 * overlay veneer and the main-image veneer island, keeping the site's own
 * calling form. Names without a repository binding are provisional.
 */

/* Phase/status word at 0x1c0 of the shared scene work record. */

/* The step value differs in the localized scene data. */

/* Offset of a flag byte on an actor record, cleared and set below. */

/* Pointer, held at fixed address 0x03001ebc, to the shared scene work
 * record. The phase/status word lives at offset 0x1c0 of that record. */

/* Loader-relocated overlay calls: each symbol names the pre-relocation call
 * word the image holds. */

/* Resolved engine calls: each pseudo symbol is the per-site call word the
 * overlay image holds (a word can serve two sites with different targets),
 * and the macro names the engine function the site reaches through the
 * overlay veneer and the main-image veneer island, keeping the site's own
 * calling form. Names without a repository binding are provisional.
 */

/* Call sites spelled through these wrappers pass their constants straight
 * into the argument registers; a direct call precomputes a costly constant
 * into a pseudo that the compiler then shares with later uses in the block.
 * A value-returning call also sets r0 last of its arguments. */
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

static __inline__ void Call3(void (*f)(), s32 a0, s32 a1, s32 a2)
{
    extern u8 Data_03001ebc[];

    f(a0, a1, a2);
}

/* The scene step counter at 0x1d8 of the shared scene work record. */
static __inline__ void bump_step(s32 amount)
{
    u8 *work = *(u8 **)0x03001ebc;

    *(u16 *)(work + 0x1d8) = (u16)(*(u16 *)(work + 0x1d8) + amount);
}

static __inline__ void Call6(void (*f)(), s32 a0, s32 a1, s32 a2, s32 a3, s32 a4, s32 a5)
{
    extern u8 Data_03001ebc[];

    f(a0, a1, a2, a3, a4, a5);
}

static __inline__ void Call2(void (*f)(), s32 a0, s32 a1)
{
    extern u8 Data_03001ebc[];

    f(a0, a1);
}

static __inline__ s32 Value2(s32 (*f)(), s32 a0, s32 a1)
{
    extern u8 Data_03001ebc[];

    return f(a0, a1);
}

static __inline__ void Call4(void (*f)(), s32 a0, s32 a1, s32 a2, s32 a3)
{
    extern u8 Data_03001ebc[];

    f(a0, a1, a2, a3);
}

/* Call sites spelled through these wrappers pass their constants straight
 * into the argument registers; a direct call precomputes a costly constant
 * into a pseudo that the compiler then shares with later uses in the block.
 * A value-returning call also sets r0 last of its arguments. */
static __inline__ s32 Value1_020010a0(s32 (*f)(), s32 a0)
{
    extern u8 Data_03001ebc[];

    return f(a0);
}

/* Call sites spelled through these wrappers pass their constants straight
 * into the argument registers; a direct call precomputes a costly constant
 * into a pseudo that the compiler then shares with later uses in the block.
 * A value-returning call also sets r0 last of its arguments. */
static __inline__ s32 Value1_020011c8(s32 (*f)(), s32 a0)
{
    extern u8 Data_03001ebc[];

    return f(a0);
}

static __inline__ s32 Value3(s32 (*f)(), s32 a0, s32 a1, s32 a2)
{
    extern u8 Data_03001ebc[];

    return f(a0, a1, a2);
}

/* Call sites spelled through these wrappers pass their constants straight
 * into the argument registers; a direct call precomputes a costly constant
 * into a pseudo that the compiler then shares with later uses in the block.
 * A value-returning call also sets r0 last of its arguments. */
static __inline__ void Call3_02001a98(void (*f)(), s32 a0, s32 a1, s32 a2)
{
    extern u8 Data_03001ebc[];

    f(a0, a1, a2);
}

/* Call sites spelled through these wrappers pass their constants straight
 * into the argument registers; a direct call precomputes a costly constant
 * into a pseudo that the compiler then shares with later uses in the block.
 * A value-returning call also sets r0 last of its arguments. */
static __inline__ void Call1_02001b58(void (*f)(), s32 a0)
{
    extern u8 Data_03001ebc[];

    f(a0);
}

/* Call sites spelled through these wrappers pass their constants straight
 * into the argument registers; a direct call precomputes a costly constant
 * into a pseudo that the compiler then shares with later uses in the block.
 * A value-returning call also sets r0 last of its arguments. */
static __inline__ void Call1_02001c14(void (*f)(), s32 a0)
{
    extern u8 Data_03001ebc[];

    f(a0);
}

/* Call sites spelled through these wrappers pass their constants straight
 * into the argument registers; a direct call precomputes a costly constant
 * into a pseudo that the compiler then shares with later uses in the block.
 * A value-returning call also sets r0 last of its arguments. */
static __inline__ void Call1_020029d4(void (*f)(), s32 a0)
{
    extern u8 Data_03001ebc[];
    u8 *Func_02007d48();
    u8 *Func_02007d62();

    f(a0);
}

/* Call sites spelled through these wrappers pass their constants straight
 * into the argument registers; a direct call precomputes a costly constant
 * into a pseudo that the compiler then shares with later uses in the block.
 * A value-returning call also sets r0 last of its arguments. */
static __inline__ void Call1_02002b7c(void (*f)(), s32 a0)
{
    extern u8 Data_03001ebc[];

    f(a0);
}

/* Call sites spelled through these wrappers pass their constants straight
 * into the argument registers; a direct call precomputes a costly constant
 * into a pseudo that the compiler then shares with later uses in the block.
 * A value-returning call also sets r0 last of its arguments. */
static __inline__ void Call1_02003a0c(void (*f)(), s32 a0)
{
    extern u8 Data_0200db50[];
    extern u8 Data_0200db60[];
    extern u8 Data_03001ebc[];
    s32 Func_02007d48();
    s32 Func_02007d62();

    f(a0);
}

/* Call sites spelled through these wrappers pass their constants straight
 * into the argument registers; a direct call precomputes a costly constant
 * into a pseudo that the compiler then shares with later uses in the block.
 * A value-returning call also sets r0 last of its arguments. */
static __inline__ void Call1_02003c88(void (*f)(), s32 a0)
{
    f(a0);
}

/* Call sites spelled through these wrappers pass their constants straight
 * into the argument registers; a direct call precomputes a costly constant
 * into a pseudo that the compiler then shares with later uses in the block.
 * A value-returning call also sets r0 last of its arguments. */
static __inline__ void Call1_02003f30(void (*f)(), s32 a0)
{
    f(a0);
}

/* Call sites spelled through these wrappers pass their constants straight
 * into the argument registers; a direct call precomputes a costly constant
 * into a pseudo that the compiler then shares with later uses in the block.
 * A value-returning call also sets r0 last of its arguments. */
static __inline__ void Call3_02004218(void (*f)(), s32 a0, s32 a1, s32 a2)
{
    extern u8 Data_03001ebc[];

    f(a0, a1, a2);
}

#if defined(GS1_EDITION_DE) || defined(GS1_EDITION_ES) || defined(GS1_EDITION_FR) || defined(GS1_EDITION_IT)
#define Data_0200db40 Data_0200db80
#define Data_0200db70 Data_0200dbb0
#define Data_0200db90 Data_0200dbd0
#endif

extern void Func_02006ab6(s32);

#if defined(GS1_EDITION_JA)
#define SCENE_STEP_VALUE 0x20ac
#elif defined(GS1_EDITION_DE) || defined(GS1_EDITION_ES) || defined(GS1_EDITION_FR) || defined(GS1_EDITION_IT)
#define SCENE_STEP_VALUE 0x1f05
#else
#define SCENE_STEP_VALUE 0x1f23
#endif

#if defined(GS1_EDITION_DE) || defined(GS1_EDITION_ES) || defined(GS1_EDITION_FR) || defined(GS1_EDITION_IT)
#define Data_0200db50 Data_0200db90
#define Data_0200db60 Data_0200dba0
#define Data_0200d160 Data_0200d1a4
#endif
#if defined(GS1_EDITION_DE)
#define Data_03001ebc Data_03001ecc
#endif

/* Loader-relocated overlay calls: each symbol names the pre-relocation call
 * word the image holds. */
extern u8 Data_00006014[];

s32 SceneEffect_UpdateCountdownMotion(struct Resource3afMotion *motion)
{
    switch (motion->countdown) {
    case 6:
        motion->component_a += (s32) 0xFFFFC000;
        motion->component_b += 0x2000;
        break;
    case 4:
        motion->component_a += 0x2000;
        /* The loader relocates the stored pool word to -0x1000. */
        motion->component_b -= 0x1000;
        break;
    case 2:
        motion->component_a += 0x1000;
        motion->component_b += (s32) 0xFFFFF800;
        break;
    case 0:
        motion->component_a = 0x10000;
        motion->component_b = 0x10000;
        motion->countdown =
            (s16)(BuildMotionCountdown(GetMotionCountdownInput(), 90) + 60);
        break;
    }
    motion->countdown--;
    return 1;
}

s32 SceneState_ApplyArgMode1AndReturnZero(s32 a)
{
    Func_0200435e(a, 1);
    return 0;
}

s32 SceneActor_OscillateHeightBetweenLimits(u8 *obj)
{
    s16 *flag = (s16 *)(obj + 0x66);
    s32 val;
    s32 tmp;

    if (*flag != 0) {
        val = *(s32 *)(obj + 0xc) - (((u32)(Func_02004792() << 15)) >> 16) - 0x8000;
        *(s32 *)(obj + 0xc) = val;
        if (val >= 0x40000)
            goto done;
        tmp = 0;
    } else {
        val = *(s32 *)(obj + 0xc) + (((u32)(Func_020047b0() << 15)) >> 16) + 0x8000;
        *(s32 *)(obj + 0xc) = val;
        if (val <= 0xC0000)
            goto done;
        tmp = 1;
    }
    *flag = tmp;
done:
    return 1;
}

s32 SceneActor_SetFacingFromSample(u8 *a)
{
    u32 v = ((u32)(Func_020047e2() << 5)) >> 16;

    if (v == 6) {
        s32 t = 0xD0;
        *(u16 *)(a + 6) = t << 8;
    } else if (v == 9) {
        s32 t = 0xB0;
        *(u16 *)(a + 6) = t << 8;
    }
    return 1;
}

s32 SceneActor_SetWord28RandomlyOneIn40(u8 *obj)
{
    if ((((u32)(Func_02004b0e() * 40)) >> 16) == 0)
        *(s32 *)(obj + 0x28) = 0x40000;
    return 1;
}

void OverlayObject_DecayFields24And28(u8 *o)
{
    if (*(s32 *)(o + 24) > 0x10000) {
        *(s32 *)(o + 24) += 0xFFFFF800;
        *(s32 *)(o + 28) += 0xFFFFF800;
    }
}

/* Contiguous unnamed leaf-owner run for resource_3af. */
u8 *SceneData_GetTablec994(void)
{
    return Data_0200c994;
}

u8 *SceneData_GetTablecb44(void)
{
    return Data_0200cb44;
}

u8 *SceneData_GetTablecb64(void)
{
    return Data_0200cb64;
}

u8 *SceneData_SelectTableByFlags(void)
{
    s32 v;

    if (Func_02004c22((s32)&Value_0000093e))
        return Data_0200d508;
    if (Func_02004c30((s32)&Value_00000927))
        return Data_0200cef0;
    v = Func_02004c3e((s32)&Value_00000928);
    if (v != 0)
        return Data_0200d028;
    if (Func_02004c4e((s32)&Value_00000911)) {
        if (Func_02004c58((s32)&Value_00000925)) {
            Data_0200ccf8[0x14E] = v;
            Data_0200ccf8[0x1AE] = 2;
            Data_0200ccf8[0x1C6] = 2;
        } else if (Func_02004c76((s32)&Value_00000922)) {
            Data_0200ccf8[0x1AE] = 1;
            Data_0200ccf8[0x1C6] = 1;
        }
        return Data_0200ccf8;
    }
    return Data_0200cba8;
}

u8 *SceneData_SelectTableByThreeFlags(void)
{
    if (Func_02004cce((s32)&Value_0000093e))
        return Data_0200d9d0;
    if (Func_02004cde(0x8A0))
        return Data_0200da54;
    if (Func_02004cec((s32)&Value_00000928))
        return Data_0200d958;
    return Data_0200d778;
}

void FieldScene_RunOpeningAuxiliarySequence(void)
{
    extern u8 Data_03001ebc[];

    s32 rec7;
    s32 record;
    s32 shown;

    ((void (*)())Func_02004d38_a)();
    if (Value1(Func_02004d1e, 0x925) != 0) {
        Call1(Func_02004e08, 0x1e08);
        Func_02004e20(21, 0);
    } else {
        if (Value1(Func_02004d38_b, 0x922) != 0) {
            Func_02004e0c(21, 2);
            Call1(Func_02004e2a, 0x1d6f);
            Func_02004e42(21, 0);
            rec7 = Value1(Func_02004d98, 21);
            record = Func_02004cfe();
            shown = ((u32)(90 * record) >> 16) + 60;
            *(volatile u16 *)(rec7 + 100) = shown;
            Func_02004dca(21, 0x200c4d8);
        } else {
            Call3(Func_02004e9e, 21, 0x103, 0);
            Func_02004e46(21, 3);
            Call1(Func_02004e6c, 0x1d36);
            Func_02004e84(21, 0);
        }
    }
    Func_02004dc0();
}

void Func_02000af0(void)
{
    extern u8 Data_03001ebc[];

    s32 rec7;
    s32 record;
    s32 shown;

    ((void (*)())Func_02004de0_a)();
    if (Value1(Func_02004dc6, 0x925) != 0) {
        Call1(Func_02004eb0, 0x1e09);
        Func_02004ec8(24, 0);
    } else {
        if (Value1(Func_02004de0_b, 0x922) != 0) {
            Func_02004eb4(24, 2);
            Call1(Func_02004ed2, 0x1d70);
            Func_02004eea(24, 0);
            rec7 = Value1(Func_02004e40, 24);
            record = Func_02004da6();
            shown = ((u32)(90 * record) >> 16) + 60;
            *(volatile u16 *)(rec7 + 100) = shown;
            Func_02004e72(24, 0x200c4d8);
        } else {
            Call3(Func_02004f46, 24, 0x103, 0);
            Func_02004eee(24, 3);
            Call1(Func_02004f14, 0x1d37);
            Func_02004f2c(24, 0);
        }
    }
    Func_02004e68();
}

/*
 * One-line dialogue callback in overlay resource_3af.  It is installed into
 * the script-record table rather than called.
 */

/*
 * Open the scripted-scene bracket, speak the line, hand actor 21 back, close
 * the bracket.  The 32-byte owner ends with its one pool word, 0x1f00, the
 * dialogue line id: it is passed as the address of Value_00001f00 so the id
 * stays a pool word and is not folded into an immediate.  The actor and line
 * ids are transcribed only, and what the installing record represents is not
 * established.
 */
void SceneDialogue_RunActor21Line(void)
{
    Func_02004e88();
    Func_02004f4e((s32)&Value_00001f00);
    Func_02004f76(21, 0);
    Func_02004ea2();
}

void FieldScene_RunScene3af_02000bb8(void)
{
    extern u8 Data_03001ebc[];

    u32 i;
    s32 record;

    if (Value1(Func_02004e8c, 0x271) == 0) {
        Func_02005026(158);
        Call6(Func_02004e72_a, 30, 94, 13, 94, 1, 3);
        Call1(Func_02004eb8, 0x271);
    }
}

void FieldScene_RunScene3af_02000bf0(void)
{
    extern u8 Data_03001ebc[];

    u32 i;
    s32 record;

    if (Value1(Func_02004ec4, 0x272) == 0) {
        Func_0200505e(158);
        Call6(Func_02004eaa, 30, 108, 13, 108, 1, 2);
        Call1(Func_02004ef0, 0x272);
    }
}

/* Gated on scene condition 0x911; when set, configures actors 20, 22 and
 * 23 (position, pose, movement and sprite flags) and their attached
 * effects, then advances the shared scene phase. */
void FieldScene_RunActorAndEffectPresentationSetup(void)
{
    extern u8 Data_03001ebc[];

    u32 i;
    u8 *record;

    if (GameFlag_IsSet_1(0x911) == 0) {
    } else {
        BattleRuntime_Reset_1();
        Func_02005114();
        ObjectMotion_SetAngleToward_1(0, 20, 10);
        ObjectMotion_SetSpeedLimitAndAcceleration_1(0x19999, 0x3333);
        ObjectMotion_PlaceWithinCameraBounds_1(0xbe0000, -1, 0x2c40000, 1);
        Object_CommitPositionThenWaitIfModeZero_1();
        BattleRuntime_WaitIfModeZero_1(40);
        ObjectMotion_SetVariantCallbackAndInvokeObject_1(22, 1);
        SceneWork_SetStepValue_1(0x1d26);
        Func_020046f0(0x4016);
        BattleEffect_SpawnLinkedResourceObject_1(20, 0x102, 60);
        ObjectMotion_SetVariantCallback_1(20, 2);
        Func_0200470a(20);
        ObjectMotion_SetVariantCallbackAndInvokeObject_2(22, 1);
        ObjectMotion_ArmCallback_1(22, 0x5000, 0);
        Func_02004724(0x4016);
        ObjectMotion_SetVariantCallbackAndInvokeObject_3(20, 1);
        Func_0200474e(20, 0xb000);
        Func_02004740(20);
        Func_02004764(23, 0x3000);
        Object_SetModeById_1(23, 3);
        Func_0200475e(0x4017);
        BattleEffect_SpawnLinkedResourceObject_2(22, 0x101, 40);
        ObjectMotion_ArmCallback_2(22, 0x8000, 20);
        Func_0200477a(0x4016);
        Func_02004796(23, 0);
        ObjectMotion_CallThenWaitForAnimationChange_1(23, 4);
        Func_02004790(0x4017);
        BattleEffect_SpawnLinkedResourceObject_3(20, 0x100, 40);
        ObjectMotion_SetVariantCallback_2(20, 2);
        Func_020047aa(20);
        ObjectMotion_CallThenWaitForAnimationChange_2(22, 3);
        Func_020047b8(0x4016);
        Func_020047da(20, 0xd000);
        Object_SetModeById_2(23, 3);
        ObjectMotion_CallThenWaitForAnimationChange_3(20, 3);
        BattleRuntime_WaitIfModeZero_2(60);
        BattleEffect_SpawnLinkedResourceObject_4(22, 0x106, 40);
        Func_0200480a_a(22, 0x5000);
        SceneWork_SetStepValue_2((s32)Data_00001d40);
        ObjectMotion_SetVariantCallback_3(22, 1);
        Func_0200480a_b(0x4016);
        BattleEffect_SpawnLinkedResourceObject_5(20, 0x101, 40);
        ObjectMotion_SetVariantCallback_4(20, 2);
        Func_02004822(20);
        BattleEffect_SpawnLinkedResourceObject_6(22, 0x108, 20);
        BattleRuntime_RunThenWaitIfModeZero_1(0x4016, 0, 20);
        BattleEffect_SpawnLinkedResourceObject_7(23, 0x102, 60);
        Func_0200484a(0x4017);
        Value2(Func_02004868, 22, 0x8000);
        ObjectMotion_CallThenWaitForAnimationChange_4(22, 3);
        BattleRuntime_RunThenWaitIfModeZero_2(0x4016, 0, 20);
        BattleEffect_SpawnLinkedResourceObject_8(20, 0x102, 40);
        ObjectMotion_SetVariantCallback_5(20, 2);
        Func_02004880(20);
        Func_0200489c(22, 0x5000);
        Object_SetModeById_3(22, 4);
        Func_02004896(22);
        ObjectMotion_ArmCallback_3(20, 0xb000, 0);
        ObjectMotion_ArmCallback_4(23, 0x3000, 40);
        ObjectMotion_ArmCallback_5(23, 0, 0);
        ObjectMotion_ArmCallback_6(20, 0xd000, 20);
        ObjectMotion_SetVariantCallbackAndInvokeObject_4(22, 2);
        BattleRuntime_WaitIfModeZero_3(20);
        Func_020048d2(0x4016);
        BattleRuntime_WaitIfModeZero_4(23, 0x102);
        BattleRuntime_WaitIfModeZero_5(20, 0x102);
        BattleRuntime_WaitIfModeZero_6(40);
        ObjectMotion_CallThenWaitForAnimationChange_5(22, 3);
        Func_020048fa(0x4016);
        ObjectMotion_SetSpeedLimitAndAcceleration_2(0xcccc, 0x1999);
        ObjectMotion_PlaceWithinCameraBounds_2(0xb60000, -1, 0x2f80000, 1);
        ObjectMotion_SetSpeedParameters_1(23, 0xcccc, 0x6666);
        ObjectMotion_EnableActionAndSetCallback_1(23, 0x200c464);
        ObjectMotion_SetSpeedParameters_2(22, 0xcccc, 0x6666);
        ObjectMotion_EnableActionAndSetCallback_2(22, 0x200c49c);
        ObjectMotion_SetSpeedParameters_3(20, 0xcccc, 0x6666);
        ObjectMotion_SetPositionAndReset_1(20, 182, 0x2f8);
        ObjectMotion_SetVariantCallback_6(20, 2);
        BattleEffect_SpawnLinkedResourceObject_9(20, 0x100, 60);
        Func_0200497e(20, 0xd000);
        BattleRuntime_RunThenWaitIfModeZero_3(20, 0, 20);
        ObjectMotion_CallThenWaitForAnimationChange_6(20, 3);
        ObjectMotion_Launch_1(20, 4, 0);
        ObjectMotion_ArmCallback_7(20, 0x3000, 40);
        ObjectMotion_SetSpeedLimitAndAcceleration_3(0x10000, 0x2000);
        ObjectMotion_PlaceWithinCameraBounds_3(0xd80000, -1, 0x3160000, 1);
        {
            /* Set the low bit of the flag byte at +35 of actor 20's record. */
            u8 *record = Scene_GetRecord_1(20);
            u8 flags = *(volatile u8 *)&record[35];

            record[35] = (u8)(flags | 1);
        }
        ObjectMotion_SetSpeedParameters_4(20, 0x13333, 0x9999);
        ObjectMotion_SetPositionAndReset_2(20, 182, 0x30e);
        ObjectMotion_SetPositionAndReset_3(20, 192, 0x328);
        ObjectMotion_SetPositionAndReset_4(20, 216, 0x328);
        Func_02004a04(20, 0xd000);
        ObjectMotion_SetVariantCallbackAndInvokeObject_5(20, 2);
        Func_02001c00();
        ObjectMotion_SetPositionAndReset_5(20, 216, 0x31e);
        ObjectMotion_SetHorizontalPositionWithTerrain_1(20, 0, 0);
        /* Set the fixed-point word at +24 of actor 20's record to 1.0. */
        record = Scene_GetRecord_2(20);
        *(s32 *)(record + 24) = 0x10000;
        /* Set the fixed-point word at +28 of actor 20's record to 1.0. */
        record = Scene_GetRecord_3(20);
        *(s32 *)(record + 28) = 0x10000;
        GameFlag_Set_1(0x920);
        BattleRuntime_ScheduleShoulderButtonModeUpdate_1();
    }
}

void FieldScene_RunScene3af_020010a0(void)
{
    extern u8 Data_03001ebc[];

    u32 i;
    u8 *record;

    if (Value1_020010a0(Func_02005372, 0x911) != 0) {
        if (Value1_020010a0(Func_0200537c, 0x922) == 0) {
            Func_020053a4();
            Func_02005510();
            Func_020023b0();
            Call3(Func_020053e6, 20, 0x6666, 0x3333);
            *(u8 *)(Func_020053dc(20) + 90) &= 254;
            Call3(Func_02005444, 20, 232, 0x330);
            Func_020053d2(1);
            *(u8 *)(Func_02005400(20) + 90) |= 1;
            Func_020053e8(20);
            Func_02005498(20, 2);
            Func_02004afe(20);
            Call3(Func_02005438, 20, 0x13333, 0x9999);
            *(u8 *)(Func_0200542e(20) + 90) &= 254;
            Call3(Func_02005492, 20, 244, 0x324);
            Func_02005420(1);
            {
                u8 *record = Func_0200544e(20);
                u8 value = *(volatile u8 *)&record[90];

                record[90] = (u8)(value | 1);
            }
            Func_02005434(20);
            Call3(Func_02005476, 20, 0x33333, 0x19999);
            Call3(Func_020054c0, 20, 248, 0x30a);
            Call3(Func_020054cc, 20, 248, 0x2bc);
            Call3(Func_020054ea, 20, 0xf60000, 0x2000000);
            Func_0200555c(20, 0, 0);
            Call3(Func_0200557e, 0, 0x101, 60);
            Func_0200548a();
        }
    }
}

void FieldScene_RunScene3af_020011c8(void)
{
    extern u8 Data_03001ebc[];

    u32 i;
    u8 *record;

    if (Value1_020011c8(Func_0200549a, 0x911) != 0) {
        if (Value1_020011c8(Func_020054a4, 0x922) == 0) {
            Func_020054cc_a();
            Func_02005638();
            Func_020024d8();
            Call3(Func_0200550e, 20, 0x6666, 0x3333);
            *(u8 *)(Func_02005504(20) + 90) &= 254;
            Call3(Func_0200556c, 20, 202, 0x330);
            Func_020054fa(1);
            *(u8 *)(Func_02005528(20) + 90) |= 1;
            Func_02005510_a(20);
            Func_020055c0(20, 2);
            Func_02004c26(20);
            Call3(Func_02005560, 20, 0x13333, 0x9999);
            *(u8 *)(Func_02005556(20) + 90) &= 254;
            Call3(Func_020055ba, 20, 192, 0x324);
            Func_02005548(1);
            {
                u8 *record = Func_02005576(20);
                u8 value = *(volatile u8 *)&record[90];

                record[90] = (u8)(value | 1);
            }
            Func_0200555c_a(20);
            Call3(Func_0200559e, 20, 0x33333, 0x19999);
            Call3(Func_020055e8, 20, 180, 0x30a);
            Call3(Func_020055f4, 20, 180, 0x2bc);
            Call3(Func_02005612, 20, 0xf60000, 0x2000000);
            Func_02005684(20, 0, 0);
            Call3(Func_020056a6, 0, 0x101, 60);
            Func_020055b2();
        }
    }
}

void FieldScene_RunScene3af_020012f0(void)
{
    extern u8 Data_03001ebc[];

    u32 i;
    s32 record;

    Call2(Func_02005706, 0x19999, 0x3333);
    Call4(Func_02005720, 0xd80000, -1, 0x3380000, 1);
    Func_0200572c();
    Func_020055fa(20);
    Func_02001f0a();
    Call6(Func_020055c2, 30, 108, 13, 108, 1, 2);
    Func_02005618(10);
    Call3(Func_020056ae, 20, 0xd80000, 0x3200000);
    Call3(Func_02005668, 20, 0x13333, 0x9999);
    Call3(Func_020056b2, 20, 216, 0x32e);
    Func_020056fc_a(0, 20, 10);
    Func_020056e4(20, 4);
    Func_020056fc_b(20, 2);
    Call3(Func_02005768, 20, 0x100, 20);
    Func_02005722(20, 0, 20);
    Func_0200571a(20, 2);
    Call1(Func_02005740, 0x1d8d);
    Func_02005762(20, 0, 20);
    Value3(Func_02005796, 20, 0x102, 0);
    Call1(Func_02005684_a, 0x923);
}

s32 SceneState_ResetWorkValuesAndStart(void)
{
    extern u32 Data_0200db50[];
    extern u32 Data_0200db60;
    extern s32 Data_0200db70[];

    u8 **base = &Data_03001e70;
    u8 *p = base[0] + 0x104;

    Func_020056c2(0x11C);
    *(s32 *)(base[19] + 0x1C0) = 0x209;
    *(s32 *)(p + 0x1C) = 0;
    Data_0200db58 = (u16)Func_02005668_a();
    Data_0200db38 = (u16)Func_02005674();
    Data_0200db50[0] = 0;
    Data_0200db50[1] = 0;
    Data_0200db60 = 0;
    Func_020056a0();
    Func_0200567e(1);
    Func_0200286a();
    return 0;
}

void SceneActor_PlaceActors20To27(void)
{
    s32 m = 0xA0;

    m <<= 7;
    Func_02005b18(21, 0x1060000, 0x2C20000);
    *(u16 *)(Func_02005abe(21) + 6) = m;
    Func_02005b32(24, 0xA40000, 0x2880000);
    {
        s32 z = 0;
        *(u16 *)(Func_02005ad8(24) + 6) = z;
    }
    Func_02005bbc(24, 1);
    Func_02005b50(25, 0xC60000, 0x2990000);
    {
        s32 x = 0x80;
        *(u16 *)(Func_02005af6(25) + 6) = x << 8;
    }
    Func_02005bdc(25, 1);
    Func_02005b70(26, 0xBC0000, 0x2A60000);
    {
        s32 x = 0xB0;
        *(u16 *)(Func_02005b16(26) + 6) = x << 8;
    }
    Func_02005b88(27, 0xBA0000, 0x27B0000);
    *(u16 *)(Func_02005b2e(27) + 6) = m;
    Func_02005b9a(22, 0, 0);
    Func_02005ba4(23, 0, 0);
    Func_02005bae_a(20, 0, 0);
}

void FieldScene_RunScene3af_0200185c(void)
{
    extern u8 Data_03001ebc[];

    u32 i;
    u8 *record;

    Func_02005b4c();
    Call1(Func_02005b62, 0x200d160);
    Func_02005ac8(1);
    Func_02005be2_a(20, 0, 0);
    Call3(Func_02005bee, 23, 0xee0000, 0x2720000);
    Call3(Func_02005bfa, 22, 0xcc0000, 0x2090000);
    record = Func_02005ba0(22);
    *(volatile s32 *)((s32)record + 12) = 0x100000;
    *(u8 *)(Func_02005bac(22) + 89) |= 128;
    Call3(Func_02005bd0, 22, 0x9999, 0x4ccc);
    Call2(Func_02005be0, 22, 0x200c58c);
    {
        u8 *record = Func_02005bce(21);
        u8 value = *(volatile u8 *)&record[89];

        record[89] = (u8)(value | 128);
    }
    Call3(Func_02005bf0, 21, 0xcccc, 0x6666);
    Call2(Func_02005c00, 21, 0x200c628);
    if (Value1(Func_02005bae, 0x109) != 0) {
        Func_02005b02();
    }
    Func_02005be2_b();
}

void FieldScene_RunScene3af_02001920(void)
{
    extern u8 Data_03001ebc[];

    u32 i;
    u8 *record;

    Func_02005c10();
    Call1(Func_02005c26, 0x200d160);
    Func_02005b8c(1);
    Func_02005ca6(20, 0, 0);
    Call3(Func_02005cb2, 23, 0xee0000, 0x2720000);
    Call3(Func_02005cbe, 22, 0x10c0000, 0x2a60000);
    record = Func_02005c64(22);
    {
        s32 shown = 0;

        *(volatile u16 *)((s32)record + 6) = shown;
    }
    Call2(Func_02005c88, 22, 0x200c980);
    {
        u8 *record = Func_02005c76(21);
        u8 value = *(volatile u8 *)&record[89];

        record[89] = (u8)(value | 128);
    }
    Call3(Func_02005c9a, 21, 0xcccc, 0x6666);
    Call2(Func_02005caa, 21, 0x200c628);
    if (Value1(Func_02005c58, 0x109) != 0) {
        Func_02005bac_a();
    }
    Func_02005c8c();
}

void FieldScene_RunScene3af_02001a98(void)
{
    extern u8 Data_03001ebc[];

    u32 i;
    s32 record;

    Func_02005d88();
    Call4(Func_02005ec2, -1, -1, -1, 0);
    Func_02005d10(1);
    Func_02005e2a(20, 0, 0);
    Func_02005e34(22, 0, 0);
    Func_02005e3e(24, 0, 0);
    Func_02005e48(25, 0, 0);
    Func_02005e52(26, 0, 0);
    Func_02005e5c(27, 0, 0);
    Func_02005e66(0, 0, 0);
    Func_02005e70(23, 0, 0);
    record = Func_02005e16(23);
    {
        s32 shown = 0x3000;

        *(volatile u16 *)(record + 6) = shown;
    }
    Call3_02001a98(Func_02005e88, 21, 0xe80000, 0x28a0000);
    record = Func_02005e2e(21);
    {
        s32 shown = 0xb000;

        *(volatile u16 *)(record + 6) = shown;
    }
    Call4(Func_02005f4e, 0xe80000, -1, 0x27c0000, 0);
    Func_02005dc2();
    Func_02005da0(1);
    Func_02003760(23, 21);
}

void FieldScene_RunScene3af_02001b58(void)
{
    extern u8 Data_03001ebc[];

    u32 i;
    s32 record;

    Func_02005e48_a();
    Call1_02001b58(Func_02005e5e, 0x200d160);
    Func_02005dc4(1);
    Call3(Func_02005ee2_a, 0, 0xe80000, 0x27c0000);
    Func_02005f22(0, 15);
    record = Func_02005e90(0);
    Func_02005e2e_a(record, 0);
    Func_02005dec(1);
    Func_02005f9c(0, 0);
    Func_02005e20();
    Func_02005dfe(1);
    Func_02005edc(22);
    Func_02005ee2_b(21);
    Func_02005e10(1);
    Func_02005f2a(22, 0, 0);
    Func_02005f34(21, 0, 0);
    Func_02005f3e(20, 0, 0);
    record = Func_02005ee4(20);
    {
        s32 shown = 0x3000;

        *(volatile u16 *)(record + 6) = shown;
    }
    Call3(Func_02005f56, 23, 0xe80000, 0x28a0000);
    record = Func_02005efc(23);
    {
        s32 shown = 0xb000;

        *(volatile u16 *)(record + 6) = shown;
    }
    Func_02005e58(1);
    Func_02003818(20, 23);
}

void FieldScene_RunScene3af_02001c14(s32 a0, s32 a1)
{
    extern u8 Data_03001ebc[];

    u32 i;
    s32 p10;
    s32 record;

    *(s32 *)(*(u8 *volatile *)Data_03001ebc + 0x1c0) = 0x100;
    Func_02006070_a();
    Func_02006084();
    Func_02005f22_a(20);
    Func_020027fa();
    Call3(Func_02005fbc, a0, 0xd80000, 0x24c0000);
    Call3(Func_02005f76, a0, 0xcccc, 0x6666);
    Call3(Func_02005fc2, a0, 216, 0x258);
    Call3(Func_02005fce, a0, 218, 0x25c);
    Call3(Func_02005fda, a0, 234, 0x25c);
    Call3(Func_02005fe4, a0, 236, 0x26a);
    Call3(Func_02006068, a0, 0x5000, 20);
    Func_02006018(a0, 3);
    Func_02005f86(20);
    Call2(Func_020056ac, a1, 0x5000);
    Func_0200603a(a1, 4, 40);
    Func_0200604a(a1, 2);
    Call1_02001c14(Func_02006070_b, 0x1e39);
    Func_02006092(a1, 0, 20);
    *(s32 *)(*(u8 *volatile *)Data_03001ebc + 0x1c0) = 0x202;
    Func_02006120();
    Func_0200612c();
    Func_0200610a(10);
    p10 = a0;
}

void FieldScene_RunActorTwentyDialogueSequence(void)
{
    extern s32 *Data_03001ebc;

    Func_02005ffc();
    Func_02006012((s32)Data_0200d160);
    Func_02005f78(1);
    Func_020060c8(0, 15);
    Func_02005fd4(Func_02006036(0), 0);
    Data_03001ebc[0x70] = 0x202;
    Func_0200617e();
    Func_02006192();
    Func_02006030(20);
    Func_020060e8(20, 1);
    Func_02006106(0x1e41);
    Func_02006128(20, 0, 10);
    Func_0200576e(22, 0x5000);
    Func_020060fc(22, 4, 20);
    Func_0200610c(22, 2);
    Func_0200614e(0x6016, 0, 20);
    Func_020061d2();
    Func_020061de();
    Func_020061bc(11);
}

/* Configures actors 20, 21, 22 and 23 (position, pose, and movement/sprite
 * flags) and advances the shared scene phase before the scene runs. */
void FieldScene_ConfigureLeadActors(void)
{
    extern u8 Data_03001ebc[];
    u8 *Func_02007d48();
    u8 *Func_02007d62();

    u8 *record;

    BattleRuntime_Reset_1_020029d4();
    Object_NotifyLastActiveOfEvent_1(0x200d160);
    Func_02006c40(1);
    ObjectMotion_SetHorizontalPositionWithTerrain_1_020029d4(20, 0xb60000, 0x26a0000);
    ObjectMotion_SetHorizontalPositionWithTerrain_2(23, 0xee0000, 0x2720000);
    ObjectMotion_SetHorizontalPositionWithTerrain_3(22, 0x10c0000, 0x2a60000);
    record = Scene_GetRecord_1_020029d4(22);
    {
        /* Clear the visibility/active flag at +6. */
        s32 shown = 0;

        *(volatile u16 *)(record + 6) = shown;
    }
    ObjectMotion_EnableActionAndSetCallback_1_020029d4(22, 0x200c980);
    {
        /* Set the high bit of the flag byte at +89. */
        u8 *record = Scene_GetRecord_2_020029d4(21);
        u8 flags = *(volatile u8 *)&record[89];

        record[89] = (u8)(flags | 128);
    }
    ObjectMotion_SetSpeedParameters_1_020029d4(21, 0xcccc, 0x6666);
    ObjectMotion_EnableActionAndSetCallback_2_020029d4(21, 0x200c628);
    SCENE_PHASE = 0x100;
    BattleRuntime_WaitIfModeZero_1_020029d4();
    ObjectMotion_SetSpeedParameters_2_020029d4();
    BattleRuntime_WaitIfModeZero_2_020029d4(20);
    ObjectMotion_SetSpeedParameters_3_020029d4(20, 0x19999, 0xcccc);
    ObjectMotion_SetPositionAndReset_1_020029d4(20, 182, 0x224);
    Func_02006476(20, 0);
    Value2(Func_02006480_a, 0, 0x8000);
    ObjectMotion_SetVariantCallbackAndInvokeObject_1_020029d4(20, 1);
    SceneWork_SetStepValue_1_020029d4(0x1ee1);
    ((void (*)())Func_02006480_b)(20);
    ObjectMotion_CallThenWaitForAnimationChange_1_020029d4(0, 3);
    BattleRuntime_WaitIfModeZero_3_020029d4(40);
    ObjectMotion_ArmCallback_1_020029d4(20, 0x5000, 20);
    BattleEffect_SpawnLinkedResourceObject_1_020029d4(20, 0x105, 60);
    BattleRuntime_RunThenWaitIfModeZero_1_020029d4(20, 0, 40);
    Func_020064ca(20, 0);
    Func_020064bc(20);
    ObjectMotion_CallThenWaitForAnimationChange_2_020029d4(0, 3);
    ObjectMotion_CallThenWaitForAnimationChange_3_020029d4(20, 3);
    ObjectMotion_SetPositionAndReset_2_020029d4(20, 182, 0x258);
    ObjectMotion_SetPositionAndReset_3_020029d4(20, 216, 0x258);
    Call2(Func_02006502, 20, 0xc000);
    Func_020036be();
    BattleRuntime_WaitIfModeZero_4_020029d4(10);
    ObjectMotion_SetPositionAndReset_4_020029d4(20, 216, 0x244);
    ObjectMotion_SetHorizontalPositionWithTerrain_4(20, 0, 0);
    SCENE_PHASE = 0x209;
    GameFlag_Set_1_020029d4(0x92b);
    GameFlag_Clear_1(0x302);
    BattleRuntime_ScheduleShoulderButtonModeUpdate_1_020029d4();
}

/* Configures actors 20, 21 and 22 (position and movement/sprite flags) and
 * advances the shared scene phase before the scene runs. */
void FieldScene_ConfigureThreeActors(void)
{
    extern u8 Data_03001ebc[];

    u32 i;
    s32 record;

    BattleRuntime_Reset_1_02002b7c();
    ObjectGroup_ConfigureChildValue_1(0, 15);
    record = Scene_GetRecord_1_02002b7c(0);
    Func_02006e38(record, 0);
    Object_NotifyLastActiveOfEvent_1_02002b7c(0x200d160);
    Func_02006dfc(1);
    ObjectMotion_SetHorizontalPositionWithTerrain_1_02002b7c(20, 0xc40000, 0x1f60000);
    record = Scene_GetRecord_2_02002b7c(20);
    {
        /* Set the visibility/active flag at +6. */
        s32 shown = 0xa000;

        *(volatile u16 *)(record + 6) = shown;
    }
    ObjectMotion_SetHorizontalPositionWithTerrain_2_02002b7c(22, 0xb80000, 0x20c0000);
    record = Scene_GetRecord_3_02002b7c(22);
    {
        /* Set the visibility/active flag at +6. */
        s32 shown = 0xb000;

        *(volatile u16 *)(record + 6) = shown;
    }
    ObjectMotion_SetActionVariant_1(21, 1);
    ObjectMotion_SetHorizontalPositionWithTerrain_3_02002b7c(21, 0xb80000, 0x2780000);
    record = Scene_GetRecord_4(21);
    {
        /* Set the visibility/active flag at +6. */
        s32 shown = 0xb000;

        *(volatile u16 *)(record + 6) = shown;
    }
    SCENE_PHASE = 0x202;
    BattleRuntime_WaitIfModeZero_1_02002b7c();
    ObjectMotion_SetSpeedParameters_1_02002b7c();
    BattleRuntime_WaitIfModeZero_2_02002b7c(20);
    ObjectMotion_Launch_1_02002b7c(22, 4, 10);
    ObjectMotion_Launch_2(22, 6, 20);
    SceneWork_SetStepValue_1_02002b7c(0x1ee5);
    Func_0200661a(22);
    ObjectMotion_CallThenWaitForAnimationChange_1_02002b7c(20, 3);
    ObjectMotion_SetSpeedParameters_2_02002b7c(21, 0x30000, 0x18000);
    ObjectMotion_SetPositionAndReset_1_02002b7c(21, 180, 0x222);
    ObjectMotion_ArmCallback_1_02002b7c(21, 0xb000, 40);
    ObjectMotion_SetVariantCallbackAndInvokeObject_1_02002b7c(21, 1);
    Func_02006652(21);
    Func_02007090(15);
}

s32 SceneState_FindFirstSetFlagOfGroup(u32 sel)
{
    s32 v = 0;
    s32 id = 23;
    u32 i;

    switch (sel) {
    case 0:
        v = 0x92C;
        break;
    case 1:
        v = 0x935;
        break;
    case 2:
        v = 0x917;
        break;
    case 3:
        v = 0x990;
        break;
    }
    for (i = 0; i < 9; i++) {
        if (Func_0200744e(v)!= 0) return id;
        v++;
        id++;
    }
    return 0;
}

void SceneEffect_InitSlotsEightToNineteen(void)
{
    extern u32 Data_0200db70[];

    {
        s32 z = 0;
        *(Func_02007596(8) + 89) = z;
        *(Func_020075a2(9) + 89) = z;
        *(Func_020075ac(10) + 89) = z;
        *(Func_020075b6(11) + 89) = z;
    }
    Func_02006634(8);
    Func_0200663a(9);
    Func_02006640(10);
    Func_02006646(11);
    Func_0200664c(12);
    Func_02006652_a(13);
    Func_02006658(14);
    Func_0200665e(15);
    {
        Data_0200db90[0] = Func_020075f0(12)[4];
        Data_0200db90[1] = Func_020075fc(13)[4];
        Data_0200db90[2] = Func_02007606(14)[4];
        Data_0200db90[3] = Func_02007610(15)[4];
        Func_0200668e(16);
        Func_02006694(17);
        Func_0200669a(18);
        Func_020066a0(19);
        {
            s32 v = 0xFFFF0000;

            Func_02007632(16)[6] = v;
            Func_0200763c(17)[6] = v;
            Func_02007644(18)[6] = v;
            Func_0200764c(19)[6] = v;
        }
        Data_0200db90[4] = Func_02007654(16)[4];
        Data_0200db90[5] = Func_0200765e(17)[4];
        Data_0200db90[6] = Func_02007668(18)[4];
        Data_0200db90[7] = Func_02007672(19)[4];
    }
    Func_02006726();
}

void OverlayObject_ActivateSlotWithMode3(s32 a)
{
    u8 *p = Func_02007692(a);

    if (p != 0) {
        Func_02007778(a, 3);
        Func_02007640(p, 0);
        p[89] = 0;
        {
            s32 c;
            c = 2 | p[35];
            p[35] = c;
        }
    }
}

void SceneState_FillTableDb70ByChecks(void)
{
    extern s32 Data_0200db70[];

    {
        s32 *p = Data_0200db70;
        u32 i = 0;
        s32 t = 3;

        for (i = 0; i < 4; i++) {
            if (Func_02006514(i)!= 0) p[i] = Func_02006832(i);
            else p[i] = t;
        }
    }
    if (Func_02006532(0) != 0) Data_0200db70[0] = Func_02006850(0);
    else Data_0200db70[0] = 3;
    if (Func_0200654e(2) != 0) Data_0200db70[1] = Func_0200686c(2);
    else Data_0200db70[1] = 3;
    {
        s32 *q = Data_0200db70;
        s32 t = 3;

        q[2] = t;
        q[3] = t;
        if (Func_02006572(1) != 0) q[4] = Func_02006890(1);
        else q[4] = t;
    }
    if (Func_02006588(3) != 0) Data_0200db70[5] = Func_020068a6(3);
    else Data_0200db70[5] = 3;
    Data_0200db70[6] = 3;
    Data_0200db70[7] = 3;
}

s32 SceneData_SelectTableEntryByFlagGroup(u32 sel)
{
    s32 base = 0;
    u32 i;

    switch (sel) {
    case 0:
        base = 0x92C;
        break;
    case 1:
        base = 0x935;
        break;
    case 2:
        base = 0x917;
        break;
    case 3:
        base = 0x990;
        break;
    }
    for (i = 0; i < 9; i++) {
        if (Func_02007764(base + i) != 0) return Data_0200db08[i];
    }
    return 0;
}

void SceneState_ConfigureEntries8Through19(void)
{
    extern u32 Data_0200db50[];
    extern u32 Data_0200db60;
    extern s32 Data_0200db70[];

    u16 *p;

    Func_02006a1c(8, 0);
    p = Data_0200db30;
    {
        s32 z = 0;

        p[0] = z;
    }
    Func_02006a2a(9, 1);
    p[1] += 0x80;
    Func_02006a38(10, 2);
    p[2] += 0x100;
    Func_02006a4a(11, 3);
    p[3] += 0x200;
    Func_02006af2(12, 0, 0);
    Func_02006afc(13, 1, 0);
    Func_02006b06(14, 2, 0);
    Func_02006b10(15, 3, 0);
    Func_02006b1a(16, 4, 1);
    Func_02006b24(17, 5, 1);
    Func_02006b2e(18, 6, 1);
    Func_02006b38(19, 7, 1);
}

void SceneEffect_AdvanceSlotByValueBand(s32 a, s32 i)
{
    extern u32 Data_0200db70[];

    u16 v = Data_0200db40[i];

    if (v >= 0x6801 && v <= 0x6FFF) {
        Data_0200db30[i] += 0x70;
        Func_02007902(a, 3);
    } else if (v >= 0xE801 && v <= 0xEFFF) {
        Data_0200db30[i] += 0xE0;
        Func_02007902(a, 3);
    } else if (v >= 0x7001 && v <= 0xEFFF) {
        Data_0200db30[i] += 0x1C0;
        Func_02007922(a, 2);
    } else {
        Data_0200db30[i] += 0x300;
        Func_02007936(a, 1);
    }
}

void SceneEffect_SelectSlotValueAndPosition(s32 a, s32 b, s32 c)
{
    extern u32 Data_0200db70[];

    s32 *p = (s32 *)Func_0200790e_a(a);
    union Slot *q = (union Slot *)p[20];
    s32 t;

    if ((c & 2) == 0) {
        switch (Data_0200db70[b]) {
        case 1:
            Data_0200db40[b] = Data_0200db30[0];
            Func_02007a40(a, 8);
            break;
        case 2:
            Data_0200db40[b] = Data_0200db30[1];
            Func_02007a54(a, 9);
            break;
        case 3:
            Data_0200db40[b] = Data_0200db30[2];
            Func_02007a68(a, 10);
            break;
        case 4:
            Data_0200db40[b] = Data_0200db30[3];
            Func_02007a7c(a, 11);
            break;
        }
    }
    if ((c & 1) != 0) {
        t = Func_0200790e_b(Data_0200db40[b]);
        q[7].h[1] = Func_0200791c(Data_0200db40[b] + 0x8000) >> 5;
        p[4] = Data_0200db90[b] - (t << 2) - (t << 1);
    } else {
        t = Func_02007940(Data_0200db40[b] + 0x8000);
        q[7].h[1] = Func_02007948(Data_0200db40[b]) >> 5;
        p[4] = Data_0200db90[b] + (t << 2) + (t << 1);
    }
}

void SceneState_InitActorSlots8To19(void)
{
    extern u32 Data_0200db50[];
    extern u32 Data_0200db60;
    extern s32 Data_0200db70[];

    {
        u16 *p = Data_0200db40;
        u32 i;

        for (i = 0; i < 8; i++) p[i] = 0xC000;
    }
    Func_02006ab6(8);
    ((void (*)())Func_02007aac_a)(9, 0, 0);
    Func_02007ab6(10, 0, 0);
    Func_02007ac0(11, 0, 0);
    Func_02007aca(12, 0, 0);
    Func_02006ae4(13);
    Func_02006aea(14);
    Func_02006af0(15);
    {
        s32 *t = Data_0200db70;
        s32 z = 0;
        t[0] = z;
        t[1] = z;
        t[2] = z;
        t[3] = z;
        Data_0200db90[0] = Func_02007a90(8)[4];
        Data_0200db90[1] = Func_02007aa0(13)[4];
        Data_0200db90[2] = Func_02007aac_b(14)[4];
        Data_0200db90[3] = Func_02007ab8(15)[4];
        Func_02006b38_a(16);
        Func_02006b3e(17);
        Func_02006b44(18);
        Func_02006b4a(19);
        {
            s32 v = 0xFFFF0000;

            Func_02007adc(16)[6] = v;
            Func_02007ae6(17)[6] = v;
            Func_02007aee(18)[6] = v;
            Func_02007af6(19)[6] = v;
        }
        t[4] = z;
        t[5] = z;
        t[6] = z;
        t[7] = z;
        Data_0200db90[4] = Func_02007b08(16)[4];
        Data_0200db90[5] = Func_02007b14(17)[4];
        Data_0200db90[6] = Func_02007b20(18)[4];
        Data_0200db90[7] = Func_02007b2c_a(19)[4];
    }
    {
        s32 *w = Func_02007b38(0);

        if (w != 0) Func_02007ba6(8, w[2], w[4]);
    }
    Func_02007a9c(1);
    Func_02007c44(13, 8);
    Func_02007c4c(14, 8);
    Func_02007c54(15, 8);
    Func_02007c5c(16, 8);
    Func_02007c64(17, 8);
    Func_02007c6c(18, 8);
    Func_02007c74(19, 8);
    {
        s32 o = 1;

        Func_02007b8a(8)[92] = o;
        Func_02007b96(13)[92] = o;
        Func_02007ba0(14)[92] = o;
        Func_02007baa(15)[92] = o;
        Func_02007bb4(16)[92] = o;
        Func_02007bbe(17)[92] = o;
        Func_02007bc8(18)[92] = o;
        Func_02007bd2(19)[92] = o;
    }
    Func_02007b2c_b(1);
    Func_02007c4a(8, 0x840000, 0x2780000);
    Func_02007b40(1);
    Func_02006eda(8, 0, 2);
    Func_02006ee4(13, 1, 2);
    Func_02006eee(14, 2, 2);
    Func_02006ef8(15, 3, 2);
    Func_02006f02(16, 4, 3);
    Func_02006f0c(17, 5, 3);
    Func_02006f16(18, 6, 3);
    Func_02006f20(19, 7, 3);
}

void SceneState_ApplyFiveRectsAtColumn78(void)
{
    Func_02007bfa(78, 39, 78, 40, 5, 1);
    Func_02007c0a(78, 39, 78, 41, 5, 1);
    Func_02007c1c(78, 39, 79, 42, 4, 1);
    Func_02007c2c(78, 39, 82, 43, 1, 1);
    {
        s32 x = 17;
        s32 y = 40;

        Func_02007c48(17, 38, 5, 2, x, y);
    }
}

void DialogueLayout_ConfigureTwoRegions(void)
{
    Func_02007c62(66, 61, 64, 40, 5, 4);
    Func_02007c7c(0, 0, 5, 4, 5, 39);
}

void FieldScene_RunStepThen10(s32 a)
{
    Func_02007dae(a, 0);
    Func_02007cdc(10);
}

void FieldScene_CallPairWith10(s32 a, s32 b)
{
    Func_02007dda(a, b, 10);
}

void FieldScene_ConfigureFourActorPresentation(void)
{
    extern u8 Data_0200db50[];
    extern u8 Data_0200db60[];
    extern u8 Data_03001ebc[];
    s32 Func_02007d48();
    s32 Func_02007d62();

    u32 i;
    s32 record;
    s32 base6_6014;

    BattleRuntime_Reset_1_02003a0c();
    *(volatile s32 *)Data_0200db50 = 0x40000;
    *(volatile s32 *)Data_0200db60 = -0x8000;
    Object_NotifyLastActiveOfEvent_1_02003a0c((s32)Data_0200d160);
    Func_02007c86(1);
    ObjectMotion_SetHorizontalPositionWithTerrain_1_02003a0c(21, 0xb60000, 0x26a0000);
    record = Scene_GetRecord_1_02003a0c(21);
    {
        s32 shown = 0xc000;

        *(volatile u16 *)(record + 6) = shown;
    }
    ObjectMotion_SetHorizontalPositionWithTerrain_2_02003a0c(20, 0xda0000, 0x2040000);
    record = Scene_GetRecord_2_02003a0c(20);
    {
        s32 shown = 0xb000;

        *(volatile u16 *)(record + 6) = shown;
    }
    ObjectMotion_SetHorizontalPositionWithTerrain_3_02003a0c(22, 0xcc0000, 0x20e0000);
    record = Scene_GetRecord_3_02003a0c(22);
    {
        s32 shown = 0xb000;

        *(volatile u16 *)(record + 6) = shown;
    }
    ObjectMotion_SetHorizontalPositionWithTerrain_4_02003a0c(23, 0, 0);
    ObjectGroup_ConfigureChildValue_1_02003a0c(0, 15);
    record = Scene_GetRecord_4_02003a0c(0);
    Func_02007d32(record, 0);
    Func_02007cf0(1);
    *(s32 *)((*(u8 *volatile *)Data_03001ebc + 0x1c0)) = 0x202;
    BattleRuntime_WaitIfModeZero_1_02003a0c();
    ObjectMotion_SetSpeedParameters_1_02003a0c();
    ObjectMotion_SetSpeedParameters_2_02003a0c(21, 0xcccc, 0x6666);
    ObjectMotion_SetPositionAndReset_1_02003a0c(21, 182, 0x214);
    ObjectMotion_ArmCallback_1_02003a0c(21, 0xb000, 40);
    SceneWork_SetStepValue_1_02003a0c(SCENE_STEP_VALUE);
    Func_020074c2(21);
    base6_6014 = (s32)Data_00006014;
    ObjectMotion_SetVariantCallbackAndInvokeObject_1_02003a0c(20, 2);
    Object_SetModeById_1_02003a0c(20, 4);
    Func_020074da(base6_6014);
    ObjectMotion_ArmCallback_2_02003a0c(21, 0xd000, 0);
    ObjectMotion_ArmCallback_3_02003a0c(22, 0xd000, 0);
    BattleEffect_SpawnLinkedResourceObject_1_02003a0c(21, 0x101, 0);
    BattleEffect_SpawnLinkedResourceObject_2_02003a0c(22, 0x101, 60);
    Object_SetModeById_2_02003a0c(20, 3);
    Func_02007514(base6_6014);
    BattleRuntime_WaitIfModeZero_2_02003a0c(21, 0x102);
    BattleRuntime_WaitIfModeZero_3_02003a0c(22, 0x102);
    BattleRuntime_WaitIfModeZero_4_02003a0c(80);
    BattleEffect_SpawnLinkedResourceObject_3_02003a0c(21, 0x100, 20);
    ObjectMotion_SetSpeedParameters_3_02003a0c(21, 0x19999, 0xcccc);
    ObjectMotion_SetPositionAndReset_2_02003a0c(21, 194, 0x1f4);
    ObjectMotion_ArmCallback_4_02003a0c(21, 0xb000, 20);
    ObjectMotion_SetSpeedParameters_4_02003a0c(22, 0xcccc, 0x6666);
    ObjectMotion_SetPositionAndReset_3_02003a0c(22, 192, 0x206);
    ObjectMotion_ArmCallback_5_02003a0c(22, 0xb000, 0);
    ObjectMotion_SetSpeedParameters_5(20, 0x10000, 0x8000);
    ObjectMotion_SetPositionAndReset_4_02003a0c(20, 210, 0x1fc);
    Value2(Func_020075ae, 20, 0xb000);
    ObjectMotion_SetVariantCallbackAndInvokeObject_2_02003a0c(21, 1);
    Call1_02003a0c(Func_020075a8, 0x5015);
    ObjectMotion_CallThenWaitForAnimationChange_1_02003a0c(20, 3);
    Call2(Func_020075ce, 22, 0xd000);
    Call1_02003a0c(Func_020075c0, 0x9016);
    ObjectMotion_ArmCallback_6_02003a0c(20, 0x8000, 20);
    Object_SetModeById_3_02003a0c(20, 4);
    Call1_02003a0c(Func_020075da, 0xa014);
    ObjectMotion_SetPositionAndReset_5_02003a0c(20, 204, 0x218);
    ObjectMotion_ArmCallback_7_02003a0c(22, 0xb000, 0);
    ObjectMotion_SetPositionAndReset_6(20, 182, 0x224);
    ObjectMotion_SetPositionAndReset_7(20, 182, 0x250);
    ObjectMotion_ResetAndSetPositionInMode2_1(20, 182, 0x298);
    BattleRuntime_WaitIfModeZero_5_02003a0c(40);
    Func_02008076();
    ObjectMotion_SetSpeedParameters_6();
    Func_02008060(16);
}

/* Sets up actors 1, 2, and 3 from three source records, runs their
 * animations and a wait loop gated on actor 0, then clears a flag byte
 * at +90 on actors 21 and 22 before finishing the scene. */
void FieldScene_RunThreeActorEncounter(void)
{
    u8 *record;

    BattleRuntime_Reset_1_02003c88();
    ObjectMotion_SetSpeedParameters_1_02003c88(0, 0x10000, 0x8000);
    ObjectMotion_SetPositionAndReset_1_02003c88(0, 180, 0x28e);
    ObjectMotion_ArmCallback_1_02003c88(0, 0x8000, 0);
    /* For each of actors 1, 2, and 3: fetch a source record, and if one
     * exists, copy its fields at +8 and +16 into the actor. */
    record = Scene_GetRecord_1_02003c88(0);
    if (record != 0) {
        ObjectMotion_SetHorizontalPositionWithTerrain_1_02003c88(1, *(s32 *)(record + 8), *(s32 *)(record + 16));
    }
    record = Scene_GetRecord_2_02003c88(0);
    if (record != 0) {
        ObjectMotion_SetHorizontalPositionWithTerrain_2_02003c88(2, *(s32 *)(record + 8), *(s32 *)(record + 16));
    }
    record = Scene_GetRecord_3_02003c88(0);
    if (record != 0) {
        ObjectMotion_SetHorizontalPositionWithTerrain_3_02003c88(3, *(s32 *)(record + 8), *(s32 *)(record + 16));
    }
    ObjectMotion_SetSpeedParameters_2_02003c88(1, 0x13333, 0x9999);
    ObjectMotion_SetSpeedParameters_3_02003c88(2, 0x10000, 0x8000);
    ObjectMotion_SetSpeedParameters_4_02003c88(3, 0x13333, 0x9999);
    ObjectMotion_ResetAndSetPositionInMode2_1_02003c88(1, 194, 0x280);
    ObjectMotion_ResetAndSetPositionInMode2_2(2, 198, 0x28e);
    ObjectMotion_SetPositionAndReset_2_02003c88(3, 194, 0x2a0);
    Object_SetModeById_1_02003c88(1, 1);
    Object_SetModeById_2_02003c88(2, 1);
    BattleRuntime_WaitIfModeZero_1_02003c88(10);
    ObjectMotion_ArmCallback_2_02003c88(1, 0x8000, 0);
    ObjectMotion_ArmCallback_3_02003c88(2, 0x8000, 0);
    Value2(Func_02007768_a, 3, 0x8000);
    Func_02007770(22, 0);
    SceneWork_SetStepValue_1_02003c88(0x1f55);
    Call1_02003c88(Func_02007768_b, 22);
    Value2(Func_02007786, 21, 0xd000);
    BattleRuntime_RunThenWaitIfModeZero_1_02003c88(21, 0, 40);
    BattleEffect_SpawnLinkedResourceObject_1_02003c88(22, 0x100, 20);
    ObjectMotion_SetVariantCallbackAndInvokeObject_1_02003c88(22, 1);
    ObjectMotion_SetSpeedParameters_5_02003c88(22, 0);
    /* Gated on a condition read from actor 0: configure actors 2, 1, and 3,
     * then spin, re-checking actor 0, while a condition on actor 2 holds. */
    if (UiWork_WaitThenFinalizeCapacity_1(0, 0) == 1) {
        ObjectMotion_CallThenWaitForAnimationChange_1_02003c88(2, 4);
        Func_020077b2(2);
        Value2(Func_020077d0, 3, 0xa000);
        Object_SetModeById_3_02003c88(3, 3);
        Func_020077ca(3);
        Value2(Func_020077e8, 1, 0x6000);
        ObjectMotion_SetVariantCallbackAndInvokeObject_2_02003c88(1, 1);
        ObjectMotion_SetSpeedParameters_6_02003c88(1, 0);
        L_02003dfa:;
        if (UiWork_WaitThenFinalizeCapacity_2(0, 0) == 1) {
            ObjectMotion_SetVariantCallbackAndInvokeObject_3_02003c88(2, 1);
            SceneWork_SetStepValue_2_02003c88(0x1f53);
            ObjectMotion_SetSpeedParameters_7(2, 0);
            goto L_02003dfa;
        }
    }
    BattleRuntime_WaitIfModeZero_2_02003c88(20);
    ObjectMotion_CallThenWaitForAnimationChange_2_02003c88(22, 3);
    SceneWork_SetStepValue_3(0x1f5b);
    Func_02007822(22);
    ObjectMotion_SetSpeedParameters_8(22, 0x10000, 0x8000);
    ObjectMotion_SetSpeedParameters_9(21, 0x10000, 0x8000);
    /* Clear the low bit of the flag byte on actor 22. */
    *(u8 *)(Scene_GetRecord_4_02003c88(22) + ACTOR_FLAGS_OFFSET) &= 254;
    ObjectMotion_SetPositionAndReset_3_02003c88(22, 162, 0x27a);
    BattleRuntime_WaitIfModeZero_3_02003c88(1);
    {
        /* Set the low bit of the flag byte on actor 22. */
        u8 *record = Scene_GetRecord_5(22);
        u8 value = *(volatile u8 *)&record[ACTOR_FLAGS_OFFSET];

        record[ACTOR_FLAGS_OFFSET] = (u8)(value | 1);
    }
    /* Clear the low bit of the flag byte on actor 21. */
    *(u8 *)(Scene_GetRecord_6(21) + ACTOR_FLAGS_OFFSET) &= 254;
    ObjectMotion_SetPositionAndReset_4_02003c88(21, 162, 0x2a4);
    BattleRuntime_WaitIfModeZero_4_02003c88(1);
    {
        /* Set the low bit of the flag byte on actor 21. */
        u8 *record = Scene_GetRecord_7(21);
        u8 value = *(volatile u8 *)&record[ACTOR_FLAGS_OFFSET];

        record[ACTOR_FLAGS_OFFSET] = (u8)(value | 1);
    }
    ObjectMotion_ArmCallback_4_02003c88(22, 0x3000, 0);
    Call2(Func_020078c8, 21, 0xd000);
    Func_020078ba(22);
    /* Finish actors 1, 2, and 3 with the same target values used earlier. */
    ObjectMotion_ResetAndSetPositionInMode2_3(1, 180, 0x28e);
    ObjectMotion_ResetAndSetPositionInMode2_4(2, 180, 0x28e);
    ObjectMotion_SetPositionAndReset_5_02003c88(3, 180, 0x28e);
    ObjectTable_DestroyById_1(1);
    ObjectTable_DestroyById_2(2);
    ObjectTable_DestroyById_3(3);
    GameFlag_Set_1_02003c88(0x903);
    BattleRuntime_ScheduleShoulderButtonModeUpdate_1_02003c88();
}

/* Sets up background/camera parameters and either takes the short branch
 * (branch value 1 from Func_020082a8) or runs a longer sequence that moves
 * and reconfigures actors 20, 21, 22 and 26 before advancing the scene
 * phase to 0x201 and finishing the scene. */
void FieldScene_RunEncounterClosingSequence(void)
{
    u32 i;
    s32 record;
    s32 data_00002014_addr;
    s32 data_0200c918_addr;

    BattleRuntime_Reset_1_02003f30();
    Object_NotifyLastActiveOfEvent_1_02003f30(0x200d1d8);
    Func_020081a0_a(1);
    BattleRuntime_WaitIfModeZero_1_02003f30();
    ObjectMotion_SetSpeedParameters_1_02003f30(0, 0x10000, 0x8000);
    ObjectMotion_SetPositionAndReset_1_02003f30(0, 148, 0x290);
    BattleEffect_SpawnLinkedResourceObject_1_02003f30(22, 0x100, 0);
    ObjectMotion_SetVariantCallbackAndInvokeObject_1_02003f30(22, 1);
    Func_02007982(22, 0x5000);
    SceneWork_SetStepValue_1_02003f30(0x1f69);
    ObjectMotion_SetSpeedParameters_2_02003f30(0x2016, 0);
    ObjectMotion_ArmCallback_1_02003f30(0, 0xe000, 0);
    if (UiWork_WaitThenFinalizeCapacity_1_02003f30(0, 0) == 1) {
        Call1_02003f30(Func_0200799a, 0x2016);
        BattleRuntime_ScheduleShoulderButtonModeUpdate_1_02003f30();
    } else {
        bump_step(1);
        BattleEventRuntime_ProcessAction_1(0x2016, 0);
        Func_02004b8a();
        ObjectMotion_SetHorizontalPositionWithTerrain_1_02003f30(26, 0xd80000, 0x24c0000);
        ObjectMotion_SetSpeedParameters_3_02003f30(26, 0x13333, 0x9999);
        ObjectMotion_SetPositionAndReset_2_02003f30(26, 216, 0x254);
        ObjectMotion_SetPositionAndReset_3_02003f30(26, 188, 0x268);
        ObjectMotion_ArmCallback_2_02003f30(0, 0xe000, 0);
        ObjectMotion_ArmCallback_3_02003f30(21, 0xd000, 0);
        ObjectMotion_ArmCallback_4_02003f30(22, 0xd000, 0);
        Value2(Func_02007a2e, 26, 0x5000);
        ObjectMotion_Launch_1_02003f30(26, 2, 0);
        Object_SetModeById_1_02003f30(26, 4);
        BattleEvent_RunActionAndWait_1(26, 0);
        ObjectMotion_SetHorizontalPositionWithTerrain_2_02003f30(20, 0xb40000, 0x3090000);
        ObjectMotion_SetSpeedParameters_4_02003f30(20, 0x10000, 0x8000);
        ObjectMotion_SetPositionAndReset_4_02003f30(20, 180, 0x298);
        data_00002014_addr = (s32)Data_00002014;
        ObjectMotion_ArmCallback_5_02003f30(20, 0xd000, 0);
        Func_02007a6e(data_00002014_addr);
        ObjectMotion_ArmCallback_6_02003f30(0, 0x2000, 0);
        ObjectMotion_ArmCallback_7_02003f30(22, 0x3000, 0);
        BattleEffect_SpawnLinkedResourceObject_2_02003f30(26, 0x101, 60);
        ObjectMotion_SetVariantCallbackAndInvokeObject_2_02003f30(20, 1);
        Func_02007a9e(data_00002014_addr);
        ObjectMotion_SetVariantCallback_1_02003f30(21, 2);
        Func_02007aac(21);
        ObjectMotion_ArmCallback_8(20, 0x5000, 20);
        Object_SetModeById_2_02003f30(20, 3);
        Call1_02003f30(Func_02007ac4, 0x6014);
        ObjectMotion_Launch_2_02003f30(26, 2, 20);
        Object_SetModeById_3_02003f30(26, 4);
        Func_02007adc_a(26);
        ObjectMotion_SetPositionAndReset_5_02003f30(20, 182, 0x280);
        ObjectMotion_ArmCallback_9(20, 0xd000, 0);
        Call1_02003f30(Func_02007afa, 0x8014);
        BattleEffect_SpawnLinkedResourceObject_3_02003f30(26, 0x100, 20);
        ObjectMotion_SetVariantCallback_2_02003f30(26, 2);
        Func_02007b14_a(26);
        ObjectMotion_CallThenWaitForAnimationChange_1_02003f30(20, 3);
        Func_02007b38_a(22, 0);
        ObjectMotion_SetVariantCallbackAndInvokeObject_3_02003f30(22, 1);
        Func_02007b32(22);
        ObjectMotion_SetSpeedParameters_5_02003f30(22, 0x19999, 0xcccc);
        data_0200c918_addr = (s32)Data_0200c918;
        ObjectMotion_EnableActionAndSetCallback_1_02003f30(22, data_0200c918_addr);
        ObjectMotion_SetSpeedParameters_6_02003f30(21, 0x19999, 0xcccc);
        ObjectMotion_SetPositionAndReset_6_02003f30(21, 168, 0x278);
        ObjectMotion_EnableActionAndSetCallback_2_02003f30(21, data_0200c918_addr);
        BattleRuntime_WaitIfModeZero_2_02003f30(80);
        ObjectMotion_EnableActionAndSetCallback_3(26, data_0200c918_addr);
        BattleRuntime_WaitIfModeZero_3_02003f30(40);
        Call2(Func_02007b96_a, 20, 0x8000);
        Func_02007b88(data_00002014_addr);
        Value2(Func_02007ba6_a, 0, 0xe000);
        ObjectMotion_CallThenWaitForAnimationChange_2_02003f30(0, 3);
        ObjectMotion_CallThenWaitForAnimationChange_3_02003f30(20, 3);
        SCENE_PHASE_02003f30 = 0x201;
        Func_0200860c();
        ObjectMotion_SetSpeedParameters_7_02003f30();
        Func_020085f6(17);
    }
}

void FieldScene_RunScene3af_02004218(void)
{
    extern u8 Data_03001ebc[];

    u32 i;
    s32 record;

    Call4(Func_0200863e, 0xe80000, -1, 0x2a40000, 0);
    Func_020084b2();
    Call3_02004218(Func_020085a8, 0, 0xe80000, 0x2a40000);
    record = Func_0200854e(0);
    {
        s32 shown = 0x4000;

        *(volatile u16 *)(record + 6) = shown;
    }
    Func_020084aa_a(1);
}
