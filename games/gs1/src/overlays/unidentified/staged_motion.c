#include "types.h"

#define CalculateFixedPointPositionDistance Func_02000030
#define StagedActorStepTable Data_0200e190
#define GetStagedActor Func_020060b8
#define FindNextStagedActor Func_02000176
#define FindBlockingStagedActor Func_020001a2
#define FindElevatedBlockingStagedActor Func_020001ce
#define CanStartStagedActorMove Func_020060e4
#define SetStagedActorMode Func_020060ac
#define SelectStagedActorSlot Func_0200604c
#define StartStagedActorEffect Func_02006312
#define StartNextStagedActorMove Func_020060f2
#define StartLeadStagedActorMove Func_02006102
#define FinishStagedActorMove Func_02006110
#define FinishStagedActorEffect Func_02006334
#define SetStagedActorTransition Func_02006118
#define RefreshStagedActor Func_020066c4_a
#define SceneDialogue_RunActor181Scene Func_02000a44
#define SceneDialogue_RunActorTenFlaggedDialogue Func_02000adc
#define SceneDialogue_RunActor21FlaggedLine Func_02000c60
#define SceneDialogue_RunActor10LineAndFlag81f Func_02000c9c
#define SceneDialogue_RunActorNineteenDialogue Func_02000d2c
#define FieldScene_RunActor181Scene Func_02000a74
#define SceneDialogue_RunActorFourteenTalk Func_02000b28
#define Audio_PlayCue_1(a0) Call1(Func_020070ae, a0)
#define ObjectMotion_ResetAndSetPositionInMode2_1(a0, a1, a2) Call3(Func_02006f94, a0, a1, a2)
#define Audio_PlayCue_1_02000f8c(args...) Func_020070de(args)
#define ObjectMotion_ResetAndSetPositionInMode2_1_02000f8c(a0, a1, a2) Call3(Func_02006fc4, a0, a1, a2)
#define Audio_PlayCue_1_02000fbc(a0) Call1(Func_0200710e, a0)
#define ObjectMotion_ResetAndSetPositionInMode2_1_02000fbc(a0, a1, a2) Call3(Func_02006ff2, a0, a1, a2)
#define BattleRuntime_Reset_1(args...) Func_02008c70(args)
#define ObjectGroup_ConfigureChildValue_1(args...) Func_02008d38(args)
#define ObjectMotion_SetAngleToward_1(args...) Func_02008d32(args)
#define SceneWork_SetStepValue_1(args...) Func_02008d52(args)
#define ObjectMotion_SetVariantCallback_1(args...) Func_02008d32_a(args)
#define BattleRuntime_RunThenWaitIfModeZero_1(args...) Func_02008d7c(args)
#define ObjectMotion_SetSpeedLimitAndAcceleration_1(a0, a1) Call2(Func_02008dc8, a0, a1)
#define ObjectMotion_PlaceWithinCameraBounds_1(a0, a1, a2, a3) Call4(Func_02008de0, a0, a1, a2, a3)
#define ObjectMotion_SetSpeedParameters_1(a0, a1, a2) Call3(Func_02008cf2, a0, a1, a2)
#define ObjectMotion_SetSpeedParameters_2(a0, a1, a2) Call3(Func_02008cfc, a0, a1, a2)
#define ObjectMotion_SetPositionAndReset_1(a0, a1, a2) Call3(Func_02008d42, a0, a1, a2)
#define ObjectMotion_ArmCallback_1(a0, a1, a2) Call3(Func_02008dd6, a0, a1, a2)
#define ObjectMotion_ArmCallback_2(a0, a1, a2) Call3(Func_02008de2, a0, a1, a2)
#define Scene_GetRecord_1(a0) Value1(Func_02008d20, a0)
#define ObjectMotion_SetHorizontalPositionWithTerrain_1(args...) Func_02008d86(args)
#define ObjectMotion_SetPositionAndReset_2(a0, a1, a2) Call3(Func_02008d7c_a, a0, a1, a2)
#define ObjectMotion_ArmCallback_3(a0, a1, a2) Call3(Func_02008e10, a0, a1, a2)
#define BattleEvent_RunActionAndWait_1(a0, a1) Call2(Func_02008e00, a0, a1)
#define ObjectMotion_ArmCallback_4(a0, a1, a2) Call3(Func_02008e24, a0, a1, a2)
#define ObjectMotion_CallThenWaitForAnimationChange_1(args...) Func_02008dcc(args)
#define BattleEvent_RunActionAndWait_2(a0, a1) Call2(Func_02008e1c, a0, a1)
#define Object_SetModeById_1(args...) Func_02008dd4(args)
#define ObjectMotion_CallThenWaitForAnimationChange_2(args...) Func_02008de4(args)
#define BattleRuntime_WaitIfModeZero_1(args...) Func_02008d52_a(args)
#define ObjectMotion_SetVariantCallbackAndInvokeObject_1(args...) Func_02008e0a(args)
#define ObjectMotion_SetSpeedParameters_3(a0, a1) Value2(Func_02008e3a, a0, a1)
#define UiWork_WaitThenFinalizeCapacity_1(a0, a1) Value2(Func_02008d92, a0, a1)
#define ObjectMotion_SetVariantCallback_2(args...) Func_02008e2e(args)
#define BattleRuntime_RunThenWaitIfModeZero_2(a0, a1, a2) Call3(Func_02008e78, a0, a1, a2)
#define BattleEffect_SpawnLinkedResourceObject_1(a0, a1, a2) Call3(Func_02008eaa, a0, a1, a2)
#define SceneWork_SetStepValue_2(args...) Func_02008e70(args)
#define BattleRuntime_RunThenWaitIfModeZero_3(a0, a1, a2) Call3(Func_02008e92, a0, a1, a2)
#define ObjectMotion_SetVariantCallbackAndInvokeObject_2(args...) Func_02008e62(args)
#define BattleRuntime_WaitIfModeZero_2(args...) Func_02008db8(args)
#define BattleRuntime_RunThenWaitIfModeZero_4(a0, a1, a2) Call3(Func_02008eaa_a, a0, a1, a2)
#define ObjectMotion_SetVariantCallbackAndInvokeObject_3(args...) Func_02008e7a(args)
#define ObjectMotion_ArmCallback_5(a0, a1, a2) Call3(Func_02008ece, a0, a1, a2)
#define BattleEvent_RunActionAndWait_3(a0, a1) Call2(Func_02008ebe, a0, a1)
#define ObjectMotion_CallThenWaitForAnimationChange_3(args...) Func_02008e7e(args)
#define BattleRuntime_RunThenWaitIfModeZero_5(a0, a1, a2) Call3(Func_02008ed8, a0, a1, a2)
#define BattleRuntime_RunThenWaitIfModeZero_6(a0, a1, a2) Call3(Func_02008ee2, a0, a1, a2)
#define BattleEffect_SpawnLinkedResourceObject_2(a0, a1, a2) Call3(Func_02008f14, a0, a1, a2)
#define BattleRuntime_RunThenWaitIfModeZero_7(a0, a1, a2) Call3(Func_02008ef6, a0, a1, a2)
#define ObjectMotion_CallThenWaitForAnimationChange_4(args...) Func_02008eae(args)
#define BattleRuntime_RunThenWaitIfModeZero_8(a0, a1, a2) Call3(Func_02008f08, a0, a1, a2)
#define ObjectMotion_CallThenWaitForAnimationChange_5(args...) Func_02008ec0(args)
#define BattleRuntime_WaitIfModeZero_3(args...) Func_02008e2e_a(args)
#define ObjectMotion_ArmCallback_6(a0, a1, a2) Call3(Func_02008f32, a0, a1, a2)
#define BattleRuntime_RunThenWaitIfModeZero_9(a0, a1, a2) Call3(Func_02008f2c, a0, a1, a2)
#define Object_SetModeById_2(args...) Func_02008edc(args)
#define ObjectMotion_CallThenWaitForAnimationChange_6(args...) Func_02008eec(args)
#define BattleRuntime_WaitIfModeZero_4(args...) Func_02008e5a(args)
#define ObjectMotion_CallThenWaitForAnimationChange_7(args...) Func_02008efa(args)
#define Object_SetModeById_3(args...) Func_02008efa_a(args)
#define Scene_GetRecord_2(a0) Value1(Func_02008ea0, a0)
#define ObjectMotion_ResetAndSetPosition_1(args...) Func_02008eda(args)
#define ObjectMotion_CommitCurrentPositionAndActivate_1(args...) Func_02008f08_a(args)
#define ObjectMotion_SetHorizontalPositionWithTerrain_2(args...) Func_02008f1a(args)
#define GameFlag_Set_1(a0) Call1(Func_02008e88, a0)
#define BattleRuntime_ScheduleShoulderButtonModeUpdate_1(args...) Func_02008eac(args)
#define BattleRuntime_Reset_1_02002f14(args...) Func_02008ed4(args)
#define ObjectMotion_PlaceWithinCameraBounds_1_02002f14(a0, a1, a2, a3) Call4(Func_0200900a, a0, a1, a2, a3)
#define ObjectMotion_SetPositionAndReset_1_02002f14(a0, a1, a2) Call3(Func_02008f54, a0, a1, a2)
#define ObjectMotion_ArmCallback_1_02002f14(a0, a1, a2) Call3(Func_02008fe8, a0, a1, a2)
#define Scene_GetRecord_1_02002f14(a0) Value1(Func_02008f26, a0)
#define ObjectMotion_SetHorizontalPositionWithTerrain_1_02002f14(args...) Func_02008f8c(args)
#define ObjectMotion_SetPositionAndReset_2_02002f14(a0, a1, a2) Call3(Func_02008f80, a0, a1, a2)
#define ObjectMotion_ArmCallback_2_02002f14(a0, a1, a2) Call3(Func_02009014, a0, a1, a2)
#define SceneWork_SetStepValue_1_02002f14(a0) Call1(Func_02008ff2, a0)
#define BattleEvent_RunActionAndWait_1_02002f14(args...) Func_0200900a_a(args)
#define ObjectMotion_SetVariantCallbackAndInvokeObject_1_02002f14(args...) Func_02008fe2(args)
#define BattleEffect_SpawnLinkedResourceObject_1_02002f14(a0, a1, a2) Call3(Func_0200904e, a0, a1, a2)
#define ObjectMotion_ArmCallback_3_02002f14(a0, a1, a2) Call3(Func_02009042, a0, a1, a2)
#define ObjectMotion_ArmCallback_4_02002f14(a0, a1, a2) Call3(Func_0200904e_a, a0, a1, a2)
#define ObjectMotion_ArmCallback_5_02002f14(a0, a1, a2) Call3(Func_0200905a, a0, a1, a2)
#define BattleRuntime_RunThenWaitIfModeZero_1_02002f14(args...) Func_02009054(args)
#define Object_SetModeById_1_02002f14(args...) Func_02009004(args)
#define ObjectMotion_CallThenWaitForAnimationChange_1_02002f14(args...) Func_02009014_a(args)
#define ObjectMotion_SetVariantCallbackAndInvokeObject_2_02002f14(args...) Func_02009034(args)
#define ObjectMotion_ArmCallback_6_02002f14(a0, a1, a2) Call3(Func_02009088, a0, a1, a2)
#define BattleRuntime_RunThenWaitIfModeZero_2_02002f14(args...) Func_02009082(args)
#define ObjectMotion_SetVariantCallback_1_02002f14(args...) Func_0200904a(args)
#define BattleEffect_SpawnLinkedResourceObject_2_02002f14(a0, a1, a2) Call3(Func_020090bc, a0, a1, a2)
#define BattleRuntime_RunThenWaitIfModeZero_3_02002f14(args...) Func_0200909e(args)
#define ObjectMotion_CallThenWaitForAnimationChange_2_02002f14(args...) Func_02009056(args)
#define BattleRuntime_RunThenWaitIfModeZero_4_02002f14(args...) Func_020090b0(args)
#define ObjectMotion_ArmCallback_7(a0, a1, a2) Call3(Func_020090cc, a0, a1, a2)
#define ObjectMotion_ArmCallback_8(a0, a1, a2) Call3(Func_020090d8, a0, a1, a2)
#define ObjectMotion_CallThenWaitForAnimationChange_3_02002f14(args...) Func_02009080(args)
#define BattleEvent_RunActionAndWait_2_02002f14(args...) Func_020090d0(args)
#define ObjectMotion_ArmCallback_9(a0, a1, a2) Call3(Func_020090f4, a0, a1, a2)
#define ObjectMotion_ArmCallback_10(a0, a1, a2) Call3(Func_02009100, a0, a1, a2)
#define ObjectMotion_SetVariantCallbackAndInvokeObject_3_02002f14(args...) Func_020090c0(args)
#define BattleRuntime_WaitIfModeZero_1_02002f14(args...) Func_02009016(args)
#define BattleRuntime_RunThenWaitIfModeZero_5_02002f14(args...) Func_02009108(args)
#define ObjectMotion_CallThenWaitForAnimationChange_4_02002f14(args...) Func_020090c0_a(args)
#define BattleRuntime_RunThenWaitIfModeZero_6_02002f14(args...) Func_0200911a(args)
#define BattleEffect_SpawnLinkedResourceObject_3(a0, a1, a2) Call3(Func_0200914e, a0, a1, a2)
#define BattleEffect_SpawnLinkedResourceObject_4(a0, a1, a2) Call3(Func_0200915a, a0, a1, a2)
#define BattleRuntime_RunThenWaitIfModeZero_7_02002f14(args...) Func_0200913c(args)
#define ObjectMotion_SetVariantCallbackAndInvokeObject_4(args...) Func_0200910c(args)
#define BattleRuntime_RunThenWaitIfModeZero_8_02002f14(args...) Func_0200914e_a(args)
#define ObjectMotion_ArmCallback_11(a0, a1, a2) Call3(Func_0200916a, a0, a1, a2)
#define ObjectMotion_SetSpeedParameters_1_02002f14(a0, a1) Value2(Func_02009152, a0, a1)
#define UiWork_WaitThenFinalizeCapacity_1_02002f14(a0, a1) Value2(Func_020090aa, a0, a1)
#define BattleEffect_SpawnLinkedResourceObject_5(a0, a1, a2) Call3(Func_020091a0, a0, a1, a2)
#define BattleRuntime_RunThenWaitIfModeZero_9_02002f14(args...) Func_02009194(args)
#define ObjectMotion_ArmCallback_12(a0, a1, a2) Call3(Func_020091b0, a0, a1, a2)
#define SceneWork_SetStepValue_2_02002f14(args...) Func_0200918e(args)
#define BattleEvent_RunActionAndWait_3_02002f14(args...) Func_020091a6(args)
#define ObjectMotion_CallThenWaitForAnimationChange_5_02002f14(args...) Func_02009166(args)
#define BattleRuntime_RunThenWaitIfModeZero_10(args...) Func_020091c0(args)
#define ObjectMotion_CallThenWaitForAnimationChange_6_02002f14(args...) Func_02009178(args)
#define BattleRuntime_WaitIfModeZero_2_02002f14(args...) Func_020090e6(args)
#define ObjectMotion_ArmCallback_13(a0, a1, a2) Call3(Func_020091ea, a0, a1, a2)
#define ObjectMotion_CallThenWaitForAnimationChange_7_02002f14(args...) Func_02009192(args)
#define ObjectMotion_CallThenWaitForAnimationChange_8(args...) Func_0200919a(args)
#define Object_SetModeById_2_02002f14(args...) Func_0200919a_a(args)
#define Scene_GetRecord_2_02002f14(a0) Value1(Func_02009140, a0)
#define ObjectMotion_ResetAndSetPosition_1_02002f14(args...) Func_0200917a(args)
#define ObjectMotion_CommitCurrentPositionAndActivate_1_02002f14(args...) Func_020091a8(args)
#define ObjectMotion_SetHorizontalPositionWithTerrain_2_02002f14(args...) Func_020091ba(args)
#define GameFlag_Set_1_02002f14(a0) Call1(Func_0200912a, a0)
#define BattleRuntime_ScheduleShoulderButtonModeUpdate_1_02002f14(args...) Func_0200914e_b(args)
#define BattleRuntime_Reset_1_020031b4(args...) Func_02009174(args)
#define Scene_GetRecord_1_020031b4(args...) Func_020091a2(args)
#define Scene_GetRecord_2_020031b4(args...) Func_020091ae(args)
#define Scene_GetRecord_3(args...) Func_020091ba_a(args)
#define Object_SetModeById_1_020031b4(args...) Func_02009228(args)
#define Object_SetModeById_2_020031b4(args...) Func_02009230(args)
#define Object_SetModeById_3_020031b4(args...) Func_02009238(args)
#define ObjectMotion_EnableActionAndSetCallback_1(args...) Func_02009208(args)
#define ObjectMotion_EnableActionAndSetCallback_2(a0, a1) Value2(Func_02009216, a0, a1)
#define ObjectMotion_MarkActiveAndSetActionCallback_1(a0, a1) Value2(Func_02009242, a0, a1)
#define BattleEffect_SpawnLinkedResourceObject_1_020031b4(a0, a1, a2) Call3(Func_0200930e, a0, a1, a2)
#define ObjectMotion_SetVariantCallbackAndInvokeObject_1_020031b4(args...) Func_020092b6(args)
#define ObjectMotion_ArmCallback_1_020031b4(a0, a1, a2) Call3(Func_0200930a, a0, a1, a2)
#define SceneWork_SetStepValue_1_020031b4(a0) Call1(Func_020092e8, a0)
#define BattleRuntime_RunThenWaitIfModeZero_1_020031b4(args...) Func_0200930a_a(args)
#define ObjectMotion_SetAngleToward_1_020031b4(args...) Func_020092e4(args)
#define BattleEvent_RunActionAndWait_1_020031b4(args...) Func_02009314(args)
#define ObjectMotion_ArmCallback_2_020031b4(a0, a1, a2) Call3(Func_02009338, a0, a1, a2)
#define GameFlag_Set_1_020031b4(a0) Call1(Func_02009236, a0)
#define BattleRuntime_ScheduleShoulderButtonModeUpdate_1_020031b4(args...) Func_0200925a(args)
#define ACTOR_A 12
#define ACTOR_B 13
#define ACTOR_C 14
#define ACTOR_D 11
#define SHARED_DATA ((s32)Data_0200e65c)
#define FieldScene_RunFlag807BranchSequence Func_02000ba8
#define FieldScene_RunScene373_02000cd0 Func_02000cd0
#define FieldScene_RunScene373_02000dc0 Func_02000dc0
#define FieldScene_RunScene373_02000e54 Func_02000e54
#define FieldScene_RunScene373_02000e84 Func_02000e84
#define FieldScene_RunPrimarySequence Func_02000f5c
#define FieldScene_RunScene373SequenceA Func_02000f8c
#define FieldScene_RunPrimarySequenceSecond Func_02000fbc
#define FieldScene_RunScene373SequenceC Func_02000fec
#define FieldScene_RunScene373SequenceD Func_020011d8
#define FieldScene_RunScene373SequenceE Func_020012bc
#define FieldScene_RunScene373_02001490 Func_02001490
#define FieldScene_RunSecondaryActorSequence Func_02002cb0
#define FieldScene_RunPrimaryActorSequence Func_02002f14
#define FieldScene_RunCompanionActorSequence Func_020031b4
#define FieldScene_RunLargeStagingSequence Func_020034c8
#define FieldScene_RunStep8C Func_02005594
#define FieldScene_RunStep9 Func_020055d0
#define FieldScene_RunStep17 Func_020055e0
#define FieldScene_RunScriptedStepEE4 Func_02005a24
#define FieldScene_RunScene373SequenceB Func_02005a40
#define SceneState_Send210AndApplyRectAt40x84 Func_02000d70
#define SceneState_Send210AndApplyRect Func_02000d98
#define SceneState_ApplyFlag801Branch Func_02000df0
#define SceneState_SetValue123Mode3 Func_02000e18
#define SceneState_SetValue123Mode4 Func_02000e2c
#define SceneState_ApplyValues123And2 Func_02000e40
#define SceneState_RunFlag204Step Func_02001244
#define SceneState_RunTablePairWhenActor22State1 Func_02001454
#define SceneState_RunTablePairWhenActor22State2 Func_02001554
#define SceneState_SetValue1ThenCall Func_020055b0
#define SceneState_SetValue0ThenCall Func_020055c0
#define SceneState_SetValue24ThenCall Func_020055f0
#define SceneState_SetValue25ThenCall Func_02005600
#define SceneState_ApplyRectAndRunTwo Func_02005928
#define SceneDialogue_RunFlag815GatedStep Func_02000eb4
#define SceneState_SetFlag204AndConfigureRegion49_46 Func_02001280
#define SceneState_RunTablePairByActor22State Func_02001590
#define SceneActor_ResetActorRun Func_0200345c
#define FieldScene_RunStagedActorCutsceneSequence Func_02003fb0
#define Effect_ConfigureSpawnedParticle Func_0200575c
#define Effect_SpawnRisingDustBurst Func_020057fc
#define Effect_UpdateParticlePosition Func_020058f0
#define Effect_PlayStepSound Func_02005a08
#define Effect_AnimateVerticalPositive Func_02005cc4
#define Effect_AnimateVerticalNegative Func_02005d14
#define FieldScene_RunSingleStep Func_020055a4
#define Runtime_ComputeFixedPointDistance Func_02005610
#define SceneActor_RunStep18WhenTargetSet Func_0200572c
#define SceneActor_SetFlagByteBySlotZeroPosition Func_02005a94
#define GetOrbitingSceneObject Func_0200bb38
#define NormalizeOrbitingSceneObject Func_0200bad2
#define IsGameFlagSet Func_0200bb38_a
#define AllocateEffectTransfer Func_0200ba98_a
#define LoadEffectResource Func_0200bb60
#define ConfigurePaletteTransfer Func_0200bac8
#define ReleaseEffectTransfer Func_0200babe
#define UpdateOrbitingSceneObject Value_0200dae1
#define SceneEffect_UpdateOrbitPosition Func_02005ae0
#define InitializeStagedActorSceneOrbitingEffect Func_02005b48
#define OverlayObject_UpdateOnFrameBit1 Func_02005c20
#define SceneEffect_UpdateObjectOnOddFrames Func_02005c5c
#define SceneEffect_UpdateObjectOnOddFramesOnly Func_02005c98
#define MapStagedActor_AdvancePair Func_020000c4
#define MapStagedActor_StopBlockedMotion Func_020002a8
#define MapStagedActor_FindClearPosition Func_02000474
#define MapStagedScene_SelectPrimaryData Func_020009dc
#define MapStagedScene_GetEmptyData Func_020009e4
#define MapStagedScene_SelectSecondaryData Func_020009e8
#define MapStagedScene_SelectTertiaryData Func_020009f0
#define MapStagedScene_SelectQuaternaryData Func_02000aa4

#include "staged_actor.h"
#include "staged_actor_movement.h"

typedef struct StagedMoveTarget { s32 unk0; s32 unk4; s32 unk8; } StagedMoveTarget;

typedef struct SceneSpriteRecord { u8 filler0[0x28]; s16 *unk28; } SceneSpriteRecord;

typedef struct SceneActorRecord {
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
    SceneSpriteRecord *unk50;
} SceneActorRecord;

/*
 * Reset or tear down a consecutive run of scene actors -- resource_373.
 */
struct Resource373Actor {
    u8 unknown_00[8];
    s32 field08;
    s32 field0c;
    s32 field10;
    u8 unknown_14[0x41];
    u8 flag55;
};

struct SourceEntity {
    s32 f00;
    s32 f04;
    s32 f08;
    s32 f0c;
    s32 f10;
};

struct ParticleRecord {
    u8 f00[9];
    u8 f09;
};

struct StagedParticle {
    u8 f00[0x48];
    s32 f48;
    s32 f4c;
    struct ParticleRecord *f50;
    u8 f54;
    u8 f55;
    u8 f56[8];
    u16 f5e;
};

/* Emitter fields are named by offset; the layout is not verified. */
struct Resource373Emitter {
    u8 unknown_00[6];
    u16 field06;
    s32 x;                          /* 0x08 */
    s32 y;                          /* 0x0c */
    s32 z;                          /* 0x10 */
    u8 unknown_14[4];
    s32 field18;
    s32 field1c;
    u8 unknown_20[4];
    s32 field24;
    s32 field28;
    s32 field2c;
    u8 unknown_30[8];
    s32 field38;
    s32 field3c;
    s32 field40;
};

struct Resource373Particle {
    u8 unknown_00[0x28];
    s32 lifetime;                   /* 0x28 */
    u8 unknown_2c[4];
    s32 field30;
    s32 field34;
    u8 unknown_38[0x10];
    s32 field48;
    u8 unknown_4c[9];
    u8 field55;
};

struct StagedVerticalEffect {
    s32 pad0[2];
    s32 f8;
    s32 fc;
    s32 f10;
    s32 pad14;
    s32 f18;
    s32 f1c;
    s32 pad20[17];
    s16 f64;
    s16 pad66;
    s32 *f68;
};

typedef struct OrbitingSceneObjectSprite {
    u8 padding_00[5];
    u8 flags_05_low : 5;
    u8 flags_05_bit_5 : 1;
    u8 flags_05_high : 2;
    u8 padding_06[3];
    u8 flags_09_low : 2;
    u8 flags_09_mode : 2;
    u8 flags_09_high : 4;
    u8 padding_0a[18];
    u8 palette;
    u8 padding_1d[10];
    u8 state;
} OrbitingSceneObjectSprite;

typedef struct OrbitingSceneObject {
    u8 padding_00[8];
    s32 x;
    s32 y;
    u8 padding_10[19];
    u8 flags_23;
    u8 padding_24[12];
    s32 orbit_angle;
    u8 padding_34[4];
    s32 orbit_center_x;
    s32 orbit_center_y;
    u8 padding_40[16];
    OrbitingSceneObjectSprite *sprite;
    u8 padding_54;
    u8 mode;
    u8 state;
    u8 padding_57[5];
    u8 active;
    u8 padding_5d[4];
    u8 visible;
    u8 padding_62[10];
    u32 callback;
} OrbitingSceneObject;


extern u8 Data_02010000[];
extern s32 Data_0200e1d0[];
extern s32 Data_0200e1e8[];
extern s32 Data_0200e708[];
extern s32 Data_0200e870[];
extern u8 Data_02000240[];
extern s32 Data_0200ee48[];
extern s32 Data_0200ecc8[];
extern s32 Data_0200eab8[];
extern s32 Data_0200e8a8[];
extern s32 Data_0200f334[];
extern s32 Data_0200f100[];
extern s32 Data_0200ef38[];
extern char Value_00000806;
extern u8 Value_0200f570;
extern u8 Value_0200f55a;
extern u8 Data_00000f4d[];
extern u8 Data_00001c45[];
extern u8 Data_00001c60[]; /* Buffer passed by address to Func_0200918e. */
extern u8 Data_0200e65c[];
extern u8 Data_00000ee8[];
extern u8 Data_0200e590[];
extern u8 Value_00000ee4;
extern s32 Data_0200f63c[];
extern s32 Data_0200f6cc[];
extern s32 Data_0200f748[];
extern s32 Data_0200f7c4[];
extern s32 Data_0200f55a[];
extern s32 Data_0200e6e0[];
extern u8 Value_0000087a;
extern u8 Value_0200dae1;

struct StagedActor *Func_020060b8(s32 arg0);
struct StagedActor *Func_02000176(s32 *arg0, struct StagedActor *arg1);
struct StagedActor *Func_020001a2(s32 *arg0, struct StagedActor *arg1);
struct StagedActor *Func_020001ce(s32 *arg0, struct StagedActor *arg1);
s32 Func_020060e4(struct StagedActor *arg0, s32 *arg1);
void Func_020060ac(struct StagedActor *arg0, s32 arg1);
void Func_0200604c(s32 arg0);
void Func_02006312(s32 arg0);
void Func_020060f2(struct StagedActor *arg0, s32 arg1, s32 arg2, s32 arg3);
void Func_02006102(struct StagedActor *arg0, s32 arg1, s32 arg2, s32 arg3);
void Func_02006110(struct StagedActor *arg0);
void Func_02006334(void);
void Func_02006118(struct StagedActor *arg0, s32 arg1);
SceneActorRecord *Func_02000342(StagedMoveTarget *, SceneActorRecord *);
s32 Func_02006266(SceneActorRecord *, StagedMoveTarget *);
s32 Func_020007de(s32 *, s32 *, s32 *);
s32 Func_020064a0(u8 *, s32 *);
s32 Func_020069a6(s32);
s32 Func_020069b4(s32);
void Func_02006a04(void);
void Func_02006a8e(s32, s32, s32);
void Func_020069fe(s32);
void Func_02006b7e(s32, s32);
s32 Func_02006a3e(s32, s32);
void Func_02006a32(void);
void Func_02006a9c(void);
s32 Func_02006a82(s32);
void Func_02006b74(s32);
void Func_02006b8c(s32, s32);
void Func_02006b84(s32);
void Func_02006b7e_a(s32, s32, s32);
void Func_02006bb6(s32, s32);
void Func_02006ada(void);
void Func_02006c20(void);
s32 Func_02006c06(s32);
void Func_02006cf8(s32);
void Func_02006d00(s32);
void Func_02006d18(s32, s32);
void Func_02006c4c(void);
void Func_02006c5c(void);
void Func_02006d1e(s32, s32, s32);
void Func_02006d34(s32);
s32 Func_02006d4c(s32, s32);
void Func_02006c62(s32);
void Func_02006c86(void);
void Func_02006cec(void);
s32 Func_02006d9c(s32, s32);
void Func_02006cf2(s32);
s32 Func_02006db4(s32, s32, s32);
void Func_02006dd2(s32);
void Func_02006dfa(s32, s32);
void Func_02006d00_a(s32);
void Func_02006d24(void);
void Func_02006a34(void);
void Func_02006abe(s32, s32, s32);
void Func_02006a2e(s32);
void Func_02006bae(s32, s32);
s32 Func_02006a6e(s32, s32);
void Func_02006a62(void);
s32 Func_02006a46(s32);
s32 Func_02006a54(s32);
void Func_02006ae8(void);
s32 Func_02006ace(s32);
void Func_02006bc0(s32);
void Func_02006bd8(s32, s32);
s32 Func_02006ae8_a(s32);
void Func_02006afa(s32);
void Func_02006be0(s32);
void Func_02006bda(s32, s32, s32);
void Func_02006c12(s32, s32);
void Func_02006bfa(s32);
void Func_02006bf4(s32, s32, s32);
void Func_02006c1c(s32, s32);
void Func_02006b50(void);
s32 Func_02006b4e();
void Func_02006b60();
void Func_02006b68();
void Func_02006c0e();
void Func_02006c46();
void Func_02006c4a();
void Func_02006c4a_a();
void Func_02006c7c();
void Func_02006c7e();
void Func_02006c90();
void Func_02006c98();
void Func_02006c9c();
void Func_02006cba();
void Func_02006cc0();
void Func_02006cd8();
void Func_02006ce2();
void Func_02006cee();
void Func_02006cfa();
void Func_02006b76();
void Func_02006c38();
void Func_02006c4c_a();
void Func_02006c90_a();
void Func_02006ce2_a();
void Func_02006d7e();
void Func_02006d82();
void Func_02006da4();
void Func_02006dae();
void Func_02006cfc();
void Func_02006df8();
void Func_02006ede();
void Func_02006f12();
void Func_02006d90();
void Func_02006e8c();
void Func_02006f72();
void Func_02006fa6();
void Func_02006dc0();
void Func_02006ebc();
void Func_02006fa2();
void Func_02006fd6();
void Func_02006e98();
void Func_02006f94();
void Func_0200707a();
void Func_020070ae();
void Func_02006ec8();
void Func_02006fc4();
void Func_020070aa();
void Func_020070de();
void Func_02006ef8();
void Func_02006ff2();
void Func_020070d8();
void Func_0200710e();
void Func_02006eb4();
void Func_02006fbc();
s32 Func_02006fd6_a();
s32 Func_02006fde();
void Func_02007022();
void Func_02007030();
void Func_02007064();
s32 Func_0200706e();
void Func_0200707e();
void Func_020070a8();
void Func_020070be();
void Func_020070c2();
void Func_020070c8();
void Func_020070d4();
void Func_020070d6();
void Func_020070da();
void Func_020070e8();
void Func_020070fe();
void Func_0200710a();
s32 Func_0200717a();
void Func_02007196();
void Func_020071a2();
void Func_020071d4();
void Func_020071e0();
void Func_020071f2();
void Func_0200724e();
void Func_02007280();
void Func_020072a2();
void Func_020072ae();
void Func_02007284();
s32 Func_020072a6();
void Func_020072c2();
void Func_020072f6();
void Func_02007308();
void Func_0200730c();
void Func_02007328();
void Func_02007334();
void Func_0200733c();
void Func_02007344();
void Func_02007358();
void Func_0200736e();
void Func_0200737e();
void Func_0200737e_a();
void Func_02007380();
void Func_02007396();
void Func_020073a4();
void Func_020073ac();
void Func_020073b8();
void Func_020073c2();
void Func_020073c4();
void Func_020073ca();
void Func_020073da();
void Func_020073e6();
s32 Func_020073f0();
void Func_020073fa();
void Func_020073fa_a();
void Func_02007406();
void Func_0200740c();
void Func_02007422();
void Func_0200742a();
void Func_0200744e();
void Func_0200745e();
void Func_0200746e();
void Func_02007482();
void Func_02007496();
void Func_020074aa();
void Func_02007460();
s32 Func_02007482_a();
void Func_0200748e();
void Func_020074ae();
void Func_020074c8();
void Func_020074ee();
void Func_020074f0();
s32 Func_020074f2();
void Func_020074fa();
void Func_02007508();
s32 Func_0200750a();
void Func_0200751c();
void Func_02007526();
void Func_02007568();
void Func_0200757c();
void Func_02007590();
void Func_0200759c();
void Func_020075a8();
void Func_020075c4();
void Func_02008c70();
void Func_02008cf2();
void Func_02008cfc();
s32 Func_02008d20();
void Func_02008d32();
void Func_02008d32_a();
void Func_02008d38();
void Func_02008d42();
void Func_02008d52();
void Func_02008d52_a();
void Func_02008d7c();
void Func_02008d7c_a();
void Func_02008d86();
s32 Func_02008d92();
void Func_02008db8();
void Func_02008dc8();
void Func_02008dcc();
void Func_02008dd4();
void Func_02008dd6();
void Func_02008de0();
void Func_02008de2();
void Func_02008de4();
void Func_02008e00();
void Func_02008e0a();
void Func_02008e10();
void Func_02008e1c();
void Func_02008e24();
void Func_02008e2e();
void Func_02008e2e_a();
s32 Func_02008e3a();
void Func_02008e5a();
void Func_02008e62();
void Func_02008e70();
void Func_02008e78();
void Func_02008e7a();
void Func_02008e7e();
void Func_02008e88();
void Func_02008e92();
s32 Func_02008ea0();
void Func_02008eaa();
void Func_02008eaa_a();
void Func_02008eac();
void Func_02008eae();
void Func_02008ebe();
void Func_02008ec0();
void Func_02008ece();
void Func_02008ed8();
void Func_02008eda();
void Func_02008edc();
void Func_02008ee2();
void Func_02008eec();
void Func_02008ef6();
void Func_02008efa();
void Func_02008efa_a();
void Func_02008f08();
void Func_02008f08_a();
void Func_02008f14();
void Func_02008f1a();
void Func_02008f2c();
void Func_02008f32();
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
void Func_0200900a_a();
void Func_02009014();
void Func_02009014_a();
void Func_02009016();
void Func_02009034();
void Func_02009042();
void Func_0200904a();
void Func_0200904e();
void Func_0200904e_a();
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
void Func_020090c0_a();
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
void Func_0200914e_a();
void Func_0200914e_b();
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
void Func_0200919a_a();
void Func_020091a0();
void Func_020091a6();
void Func_020091a8();
void Func_020091b0();
void Func_020091ba();
void Func_020091c0();
void Func_020091ea();
void Func_02009086();
void Func_020090a6();
void Func_020090c2();
void Func_02009110();
void Func_0200911c();
void Func_02009128();
void Func_0200915e();
void Func_02009174();
void Func_02009184();
s32 Func_020091a2();
s32 Func_020091ae();
s32 Func_020091ba_a();
void Func_02009208();
s32 Func_02009216();
void Func_02009228();
void Func_02009230();
void Func_02009236();
void Func_02009238();
s32 Func_02009242();
void Func_0200925a();
void Func_020092b6();
void Func_020092e4();
void Func_020092e8();
void Func_0200930a();
void Func_0200930a_a();
void Func_0200930e();
void Func_02009314();
void Func_02009338();
void Func_02008e9a();
void Func_020090a8();
void Func_020092fc();
void Func_0200938e();
void Func_020093a2();
void Func_02009466();
void Func_02009472();
void Func_02009482();
void Func_02009492();
void Func_020094a6();
void Func_020094ac();
s32 Func_020094c0();
s32 Func_020094c8();
void Func_020094d2();
void Func_020094d4();
void Func_020094f6();
void Func_02009518();
void Func_02009524();
void Func_0200952e();
void Func_02009530();
s32 Func_02009548();
void Func_0200954a();
void Func_0200955e();
s32 Func_02009572();
void Func_02009576();
void Func_0200958a();
s32 Func_02009594();
u8 *Func_020095b6();
u8 *Func_020095c2();
void Func_020095e0();
void Func_020095ea();
s32 Func_020095fa();
void Func_02009630();
void Func_0200963c();
void Func_02009674();
void Func_02009678();
void Func_0200967c();
void Func_0200968a();
void Func_020096a2();
void Func_020096ae();
void Func_020096b8();
void Func_020096cc();
void Func_020096da();
void Func_020096de();
void Func_020096fa();
void Func_020096fa_a();
void Func_02009704();
void Func_02009706();
void Func_0200970e();
void Func_02009712();
void Func_02009714();
void Func_0200971a();
void Func_0200971e();
void Func_0200972a();
void Func_0200972c();
void Func_0200974c();
void Func_0200976c();
void Func_0200976c_a();
void Func_02009778();
void Func_02009778_a();
void Func_0200977c();
void Func_02009784();
void Func_02009794();
void Func_02009798();
void Func_020097a6();
void Func_020097b6();
void Func_020097be();
void Func_020097d2();
void Func_020097d6();
void Func_020097e0();
s32 Func_020097ee();
void Func_020097f4();
void Func_02009800();
void Func_02009804();
void Func_02009816();
void Func_02009818();
void Func_0200982a();
void Func_02009840();
void Func_02009842();
void Func_02009844();
void Func_02009848();
void Func_02009848_a();
void Func_0200984c();
void Func_02009854();
void Func_02009860();
void Func_02009862();
void Func_0200986c();
void Func_0200986e();
void Func_02009872();
void Func_02009874();
void Func_02009882();
void Func_02009898();
void Func_0200989e();
void Func_020098a2();
void Func_020098b2();
void Func_020098b6();
void Func_020098d2();
void Func_020098da();
void Func_020098de();
void Func_020098e8();
void Func_020098fe();
void Func_020098fe_a();
void Func_02009904();
void Func_02009904_a();
void Func_02009908();
s32 Func_0200990a();
void Func_0200990c();
void Func_02009912();
void Func_0200993e();
void Func_02009940();
u8 *Func_02009944();
void Func_02009946();
void Func_0200994c();
void Func_02009952();
void Func_02009960();
void Func_02009962();
void Func_02009966();
void Func_0200996c();
void Func_02009970();
void Func_02009992();
void Func_020099a0();
void Func_020099a6();
void Func_020099b2();
void Func_020099b4();
void Func_020099b8();
void Func_020099ba();
void Func_020099ba_a();
void Func_020099bc();
void Func_020099d0();
void Func_020099fa();
void Func_02009a52();
void Func_02009a6a();
u8 *Func_02009a9e();
void Func_02009aac();
void Func_02009abe();
void Func_02009ae4();
void Func_02009afc();
void Func_02009b20();
void Func_02009b3c();
void Func_02009b3e();
void Func_02009b42();
void Func_02009b44();
void Func_02009b5a();
void Func_02009b62();
void Func_02009b66();
void Func_02009b70();
void Func_02009b7e();
void Func_02009b8e();
void Func_02009b8e_a();
void Func_02009b96();
void Func_02009b9e();
void Func_02009ba6();
void Func_02009bb2();
void Func_02009bbe();
void Func_02009bd6();
void Func_02009bd6_a();
void Func_02009bec();
void Func_02009bee();
void Func_02009bf8();
void Func_02009bfc();
void Func_02009c10();
void Func_02009c12();
void Func_02009c14();
void Func_02009c22();
void Func_02009c2e();
void Func_02009c34();
void Func_02009c44();
void Func_02009c50();
void Func_02009c50_a();
void Func_02009c52();
void Func_02009c58();
void Func_02009c60();
void Func_02009c68();
void Func_02009c80();
void Func_02009c82();
void Func_02009c90();
void Func_02009c90_a();
void Func_02009c98();
void Func_02009ca8();
void Func_02009cd6();
void Func_02009cd8();
void Func_02009cd8_a();
void Func_02009ce2();
void Func_02009cea();
void Func_02009cf2();
void Func_02009d0e();
void Func_02009d10();
void Func_02009d1e();
void Func_02009d22();
void Func_02009d2a();
void Func_02009d2c();
void Func_02009d36();
void Func_02009d36_a();
void Func_02009d3a();
void Func_02009d40();
void Func_02009d48();
void Func_02009d4e();
void Func_02009d54();
void Func_02009d5c();
void Func_02009d64();
void Func_02009d7a();
void Func_02009d80();
void Func_02009d80_a();
void Func_02009d80_b();
void Func_02009d90();
void Func_02009d9a();
void Func_02009daa();
void Func_02009db6();
void Func_02009db6_a();
void Func_02009dc6();
void Func_02009dc8();
void Func_02009dc8_a();
void Func_02009dd6();
void Func_02009ddc();
void Func_02009de0();
void Func_02009df8();
void Func_02009e10();
void Func_02009e16();
void Func_02009e22();
void Func_02009e2a();
void Func_02009e3a();
void Func_02009e3e();
void Func_02009e3e_a();
void Func_02009e50();
void Func_02009e60();
void Func_02009e6a();
void Func_02009e72();
void Func_02009e72_a();
void Func_02009e78();
void Func_02009e90();
void Func_02009ea0();
void Func_02009ea4();
void Func_02009ea8();
void Func_02009ea8_a();
void Func_02009eae();
void Func_02009ec8();
void Func_02009ec8_a();
void Func_02009eca();
void Func_02009ecc();
void Func_02009ee8();
void Func_02009ef6();
void Func_02009efe();
void Func_02009f08();
void Func_02009f10();
void Func_02009f1a();
void Func_02009f1e();
void Func_02009f28();
void Func_02009f2c();
void Func_02009f38();
void Func_02009f40();
void Func_02009f42();
void Func_02009f4a();
void Func_02009f4e();
void Func_02009f6a();
void Func_02009f74();
void Func_02009f7a();
void Func_02009f80();
void Func_02009f8a();
void Func_02009fa0();
void Func_02009fba();
void Func_02009fda();
void Func_0200a082();
void Func_0200a088();
void Func_0200a094();
void Func_0200b6c0(s32, s32);
void Func_0200b5ba(s32);
void Func_0200b272(void);
void Func_0200b5ca(s32);
void Func_0200b282(void);
void Func_0200b9e4(void);
void Func_0200b9ac(s32, s32);
void Func_0200b9f8(void);
s32 Func_0200b9e2();
s32 Func_0200b9ee();
s32 Func_0200ba0e();
void Func_0200ba14();
void Func_0200ba2c();
s32 Func_0200ba42();
s32 Func_0200ba60();
void Func_02006d1e_a(s32);
void Func_02006cca(s32, s32, s32, s32, s32, s32);
void Func_02006d4e(s32);
void Func_02006cf2_a(s32, s32, s32, s32, s32, s32);
s32 Func_02006d92(s32);
void Func_02001dea(void);
void Func_02006f52(s32);
void Func_02006f08(s32);
void Func_02006f6a(s32);
void Func_02006f20(s32);
void Func_02006f7e(s32);
void Func_02006f34(s32);
void Func_02006f92(s32);
void Func_02006f48(s32);
void Func_02007206(void);
void Func_0200719a(s32, s32, s32, s32, s32, s32);
s32 Func_0200451a(s32, s32, s32, s32);
void Func_02007216(s32);
void Func_0200723a(void);
u8 *Func_0200743e(s32);
s32 Func_020073fe(s32);
void Func_0200290a(s32, s32);
u8 *Func_0200753e(s32);
s32 Func_020074fe(s32);
void Func_02002a0a(s32, s32);
void Func_0200b59a(s32);
void Func_0200b216(void);
void Func_0200b5aa(s32);
void Func_0200b226(void);
void Func_0200b5da(s32);
void Func_0200b292(void);
void Func_0200b5ea(s32);
void Func_0200b2a2(void);
void Func_0200b87a(s32, s32, s32, s32, s32, s32);
void Func_02005a06(void);
void Func_0200b296(void);
s32 Func_02006e56(s32);
s32 Func_02006e60(s32);
void Func_02006e88(void);
void Func_02006f56(s32);
s32 Func_02006f66(s32, s32);
s32 Func_02006ebe(s32, s32);
void Func_02006f8c(s32, s32, s32);
void Func_02006f8c_a(s32, s32);
void Func_02006ec4(s32);
void Func_02006fac(s32, s32);
void Func_02006ee0(void);
void Func_02007070(s32);
void Func_02006e5a(s32, s32, s32);
void Func_02006f58(s32, s32, s32);
void Func_0200703e(s32);
void Func_02007242(void);
s32 Func_02004612(s32, s32, s32, s32);
void Func_02007246(s32);
void Func_020071ea(s32, s32, s32, s32, s32, s32);
void Func_02007276(void);
u8 *Func_0200757a(s32);
s32 Func_0200753a(s32);
void Func_02002a46(s32, s32);
void Func_02002a54(s32, s32);
struct Resource373Actor *Func_02009458(s32 selector);
void Func_020093d0(struct Resource373Actor *actor, s32 mode);
void Func_020094ec(s32 selector, s32 a, s32 b);
void Func_020078a2();
void Func_02007b8a();
void Func_02007c4a();
void Func_0200882c();
void Func_02008860();
void Func_0200976e();
void Func_02009b40();
void Func_02009e14();
void Func_02009e6e();
void Func_02009f32();
void Func_02009f3e();
void Func_02009f4e_a();
void Func_02009f56();
void Func_02009f5e();
void Func_02009f86();
void Func_02009f9e();
u8 *Func_02009fa8();
void Func_02009fb4();
void Func_0200a00e();
u8 *Func_0200a022();
void Func_0200a02e();
void Func_0200a036();
u8 *Func_0200a044();
void Func_0200a04e();
u8 *Func_0200a062();
void Func_0200a074();
void Func_0200a080();
u8 *Func_0200a084();
void Func_0200a088_a();
void Func_0200a08e();
void Func_0200a096();
s32 Func_0200a0a8();
u8 *Func_0200a0ac();
void Func_0200a0b4();
void Func_0200a0b4_a();
void Func_0200a0c0();
u8 *Func_0200a0cc();
void Func_0200a0d6();
void Func_0200a0f0();
void Func_0200a114();
void Func_0200a116();
void Func_0200a11a();
void Func_0200a11c();
void Func_0200a120();
void Func_0200a132();
void Func_0200a13e();
void Func_0200a148();
void Func_0200a14e();
void Func_0200a156();
void Func_0200a164();
void Func_0200a166();
void Func_0200a17e();
void Func_0200a18a();
void Func_0200a1c2();
void Func_0200a1ca();
void Func_0200a1d0();
void Func_0200a1da();
s32 Func_0200a1ea();
void Func_0200a1f6();
u8 *Func_0200a1fa();
void Func_0200a202();
void Func_0200a208();
void Func_0200a20a();
void Func_0200a216();
void Func_0200a226();
void Func_0200a228();
void Func_0200a232();
void Func_0200a23e();
u8 *Func_0200a240();
void Func_0200a24c();
void Func_0200a256();
void Func_0200a258();
void Func_0200a25c();
void Func_0200a25e();
void Func_0200a262();
void Func_0200a264();
void Func_0200a268();
void Func_0200a274();
void Func_0200a282();
void Func_0200a2a8();
void Func_0200a2ae();
void Func_0200a2b2();
void Func_0200a2c4();
void Func_0200a2d0();
void Func_0200a2e0();
void Func_0200a2f0();
void Func_0200a2f6();
void Func_0200a30e();
void Func_0200a312();
void Func_0200a316();
void Func_0200a32a();
void Func_0200a332();
void Func_0200a348();
void Func_0200a348_a();
void Func_0200a34c();
void Func_0200a358();
void Func_0200a364();
void Func_0200a368();
u8 *Func_0200a382();
s32 Func_0200a384();
void Func_0200a38e();
s32 Func_0200a390();
void Func_0200a3a0();
void Func_0200a3a6();
void Func_0200a3b6();
void Func_0200a3be();
void Func_0200a3c8();
void Func_0200a3e8();
void Func_0200a3ec();
void Func_0200a3ee();
void Func_0200a3f0();
void Func_0200a3f2();
void Func_0200a3f4();
void Func_0200a3f8();
void Func_0200a406();
void Func_0200a410();
void Func_0200a422();
void Func_0200a432();
void Func_0200a438();
void Func_0200a450();
void Func_0200a458();
void Func_0200a46c();
void Func_0200a472();
void Func_0200a488();
void Func_0200a4b8();
void Func_0200a4e2();
void Func_0200a4e6();
void Func_0200a4f2();
void Func_0200a500();
void Func_0200a50c();
void Func_0200a518();
u8 *Func_0200a51c();
void Func_0200a528();
void Func_0200a536();
void Func_0200a540();
void Func_0200a54c();
void Func_0200a586();
void Func_0200a58e();
void Func_0200a5a0();
void Func_0200a5aa();
void Func_0200a5b8();
void Func_0200a5c2();
void Func_0200a5cc();
void Func_0200a5ce();
void Func_0200a5e6();
void Func_0200a5e8();
void Func_0200a5ee();
void Func_0200a608();
void Func_0200a61e();
void Func_0200a62c();
void Func_0200a654();
void Func_0200a666();
void Func_0200a672();
void Func_0200a676();
void Func_0200a67a();
void Func_0200a682();
void Func_0200a692();
s32 Func_0200a6a6();
void Func_0200a6c0();
void Func_0200a6c6();
void Func_0200a6c8();
void Func_0200a6d2();
void Func_0200a6dc();
void Func_0200a6e6();
void Func_0200a6f8();
void Func_0200a706();
void Func_0200a70c();
void Func_0200a710();
void Func_0200a718();
void Func_0200a722();
void Func_0200a722_a();
void Func_0200a73e();
void Func_0200a740();
s32 Func_0200a74e();
s32 Func_0200a75a();
void Func_0200a762();
void Func_0200a76e();
void Func_0200a786();
void Func_0200a794();
void Func_0200a7b6();
void Func_0200a7bc();
void Func_0200a7c8();
void Func_0200a7dc();
void Func_0200a7ea();
void Func_0200a7f0();
void Func_0200a7fc();
void Func_0200a7fe();
s32 Func_0200a802();
void Func_0200a814();
void Func_0200a81a();
void Func_0200a826();
void Func_0200a832();
void Func_0200a838();
void Func_0200a83a();
void Func_0200a854();
void Func_0200a85c();
void Func_0200a87c();
void Func_0200a886();
void Func_0200a886_a();
void Func_0200a88a();
void Func_0200a898();
void Func_0200a89c();
void Func_0200a8a2();
void Func_0200a8b2();
void Func_0200a8b2_a();
void Func_0200a8b8();
void Func_0200a8ba();
void Func_0200a8c6();
void Func_0200a8d4();
void Func_0200a8dc();
void Func_0200a8ea();
void Func_0200a8fa();
void Func_0200a910();
void Func_0200a912();
void Func_0200a914();
void Func_0200a950();
void Func_0200a95c();
void Func_0200a95c_a();
void Func_0200a974();
void Func_0200a974_a();
void Func_0200a98c();
void Func_0200a990();
void Func_0200a992();
void Func_0200a992_a();
void Func_0200a99a();
u8 *Func_0200a9a4();
u8 *Func_0200a9a4_a();
void Func_0200a9b6();
void Func_0200a9b6_a();
void Func_0200a9c0();
void Func_0200a9c2();
u8 *Func_0200a9c8();
void Func_0200a9d0();
void Func_0200a9d6();
void Func_0200a9e4();
void Func_0200a9ea();
void Func_0200a9f0();
void Func_0200a9fc();
void Func_0200a9fc_a();
void Func_0200a9fe();
void Func_0200aa10();
void Func_0200aa26();
void Func_0200aa2a();
void Func_0200aa34();
void Func_0200aa4c();
void Func_0200aa72();
void Func_0200aa7c();
void Func_0200aa96();
void Func_0200aaa6();
void Func_0200aab6();
void Func_0200aab6_a();
void Func_0200aac0();
void Func_0200aac4();
void Func_0200aae8();
void Func_0200aaea();
void Func_0200aaee();
void Func_0200ab04();
void Func_0200ab28();
void Func_0200ab2a();
void Func_0200ab2c();
void Func_0200ab2c_a();
void Func_0200ab38();
void Func_0200ab44();
void Func_0200ab50();
void Func_0200ab52();
void Func_0200ab5e();
void Func_0200ab60();
void Func_0200ab60_a();
void Func_0200ab64();
void Func_0200ab6a();
void Func_0200ab70();
void Func_0200ab76();
void Func_0200ab76_a();
void Func_0200abae();
void Func_0200abb2();
void Func_0200abd0();
void Func_0200abd6();
void Func_0200abe6();
void Func_0200abe6_a();
void Func_0200abf6();
void Func_0200ac04();
void Func_0200ac0a();
void Func_0200ac0e();
void Func_0200ac16();
void Func_0200ac1e();
void Func_0200ac1e_a();
void Func_0200ac26();
void Func_0200ac26_a();
void Func_0200ac30();
void Func_0200ac36();
void Func_0200ac40();
void Func_0200ac54();
void Func_0200ac5e();
void Func_0200ac66();
void Func_0200ac80();
s32 Func_0200aca0();
void Func_0200aca8();
void Func_0200acc0();
void Func_0200acd0();
void Func_0200acd0_a();
void Func_0200acd8();
void Func_0200ace8();
void Func_0200acee();
void Func_0200acf0();
void Func_0200acf4();
void Func_0200acfe();
void Func_0200ad06();
void Func_0200ad2a();
u8 *Func_0200ad36();
u8 *Func_0200ad36_a();
void Func_0200ad3a();
void Func_0200ad3c();
void Func_0200ad42();
s32 Func_0200ad46();
void Func_0200ad56();
void Func_0200ad5e();
void Func_0200ad60();
void Func_0200ad62();
void Func_0200ad62_a();
void Func_0200ad6c();
void Func_0200ad6e();
void Func_0200ad74();
void Func_0200ad7a();
void Func_0200ad7e();
void Func_0200ad82();
void Func_0200ad98();
void Func_0200ada0();
void Func_0200ada4();
void Func_0200adaa();
void Func_0200adb6();
void Func_0200adc2();
void Func_0200adce();
void Func_0200adec();
void Func_0200adf8();
void Func_0200adfa();
void Func_0200ae1c();
s32 Func_0200ae2a();
void Func_0200ae54();
void Func_0200ae58();
void Func_0200ae5e();
void Func_0200ae68();
void Func_0200ae6e();
void Func_0200ae70();
void Func_0200ae92();
void Func_0200ae94();
void Func_0200ae98();
void Func_0200aea0();
void Func_0200aeb8();
void Func_0200aebc();
void Func_0200aeca();
void Func_0200aecc();
void Func_0200aed4();
void Func_0200aeea();
void Func_0200af04();
void Func_0200af08();
void Func_0200af20();
void Func_0200af2c();
void Func_0200af3c();
void Func_0200af3c_a();
void Func_0200af42();
void Func_0200af4a();
void Func_0200af4c();
void Func_0200af54();
void Func_0200af6c();
void Func_0200af74();
void Func_0200af7a();
void Func_0200af90();
void Func_0200af92();
void Func_0200afae();
s32 Func_0200afce();
void Func_0200afda();
void Func_0200afe2();
void Func_0200afee();
void Func_0200aff2();
void Func_0200affe();
void Func_0200b01c();
void Func_0200b022();
void Func_0200b024();
void Func_0200b02e();
void Func_0200b030();
void Func_0200b032();
void Func_0200b03a();
void Func_0200b048();
u8 *Func_0200b04e();
void Func_0200b050();
void Func_0200b050_a();
void Func_0200b05e();
void Func_0200b05e_a();
void Func_0200b062();
void Func_0200b074();
void Func_0200b078();
void Func_0200b07a();
void Func_0200b084();
void Func_0200b08c();
void Func_0200b09a();
void Func_0200b0a2();
s32 Func_0200b0c4();
void Func_0200b0da();
void Func_0200b104();
void Func_0200b114();
void Func_0200b12a();
void Func_0200b12c();
void Func_0200b12e();
void Func_0200b13c();
void Func_0200b142();
void Func_0200b146();
void Func_0200b154();
void Func_0200b156();
void Func_0200b158();
void Func_0200b15e();
void Func_0200b162();
void Func_0200b16e();
void Func_0200b17a();
void Func_0200b18c();
void Func_0200b1a8();
void Func_0200b1bc();
void Func_0200b1c6();
void Func_0200b1d6();
void Func_0200b1e6();
void Func_0200b1f0();
void Func_0200b1f4();
void Func_0200b1f8();
void Func_0200b1fe();
void Func_0200b21e();
void Func_0200b226_a();
void Func_0200b230();
void Func_0200b23a();
void Func_0200b24c();
void Func_0200b24e();
void Func_0200b254();
void Func_0200b270();
void Func_0200b270_a();
void Func_0200b27c();
void Func_0200b286();
void Func_0200b288();
void Func_0200b28a();
void Func_0200b290();
void Func_0200b292_a();
void Func_0200b296_a();
void Func_0200b2aa();
void Func_0200b2ae();
void Func_0200b2b8();
void Func_0200b2c6();
void Func_0200b2c8();
void Func_0200b2ca();
void Func_0200b2cc();
void Func_0200b2e0();
void Func_0200b2ec();
void Func_0200b2f0();
void Func_0200b2f6();
void Func_0200b300();
void Func_0200b302();
void Func_0200b310();
void Func_0200b31a();
s32 Func_0200b31c();
void Func_0200b31e();
void Func_0200b322();
void Func_0200b328();
void Func_0200b32e();
void Func_0200b336();
void Func_0200b340();
void Func_0200b340_a();
void Func_0200b344();
void Func_0200b35a();
void Func_0200b362();
void Func_0200b370();
u8 *Func_0200b37a();
u8 *Func_0200b37a_a();
void Func_0200b388();
void Func_0200b388_a();
u8 *Func_0200b38e();
void Func_0200b39c();
u8 *Func_0200b3a0();
void Func_0200b3a4();
void Func_0200b3b8();
void Func_0200b3b8_a();
void Func_0200b3c6();
void Func_0200b3c8();
void Func_0200b3c8_a();
void Func_0200b3d6();
void Func_0200b3ea();
void Func_0200b410();
void Func_0200b414();
void Func_0200b420();
void Func_0200b42a();
void Func_0200b43a();
void Func_0200b440();
void Func_0200b464();
void Func_0200b468();
void Func_0200b46c();
void Func_0200b47a();
void Func_0200b47c();
void Func_0200b480();
void Func_0200b48a();
void Func_0200b490();
void Func_0200b4a2();
u8 *Func_0200b4b0();
void Func_0200b4c4();
void Func_0200b4cc();
void Func_0200b4de();
void Func_0200b4de_a();
void Func_0200b4e8();
void Func_0200b4f6();
void Func_0200b50e();
void Func_0200b52c();
void Func_0200b54e();
void Func_0200b55e();
void Func_0200b572();
void Func_0200b584();
void Func_0200b5a2();
void Func_0200b5ae();
void Func_0200b5f4();
void Func_0200b610();
void Func_0200d5c1();
void Func_0200d5e1();
void Func_0200d5f1();
void Func_0200d601();
void Func_0200da09();
s32 Func_0200b60e(struct SourceEntity *);
s32 Func_0200b622(void);
s32 Func_0200b628(void);
void Func_0200b656(s32, s32, s32 *);
struct StagedParticle *Func_0200b6a2(s32, s32, s32, s32);
void Func_0200b708(struct StagedParticle *, s32);
void Func_0200b6b8(struct StagedParticle *, s32);
void Func_0200b6c8(struct StagedParticle *, s32);
void Func_0200b92e(s32);
void Func_0200b954();
void Func_0200b6c0_a();
struct Resource373Particle *Func_0200b754();
void Func_0200b7a8();
void Func_0200b760();
s32 Func_0200b70c();
s32 Func_0200b726();
s32 Func_0200b72c();
s32 Func_0200b734();
s32 Func_0200b744();
void Func_0200b19a();
void Func_0200ba08();
void Func_0200b7d4(s32, s32, s32 *);
void Func_0200b838(s32, s32, s32, s32);
void Func_0200bb66(int sound);
void Func_0200bbec(s32);
s32 Func_0200bb9c(s32);
void Func_0200bc3c(s32);
s32 Func_0200bbec_a(s32);
void Func_0200b6d4(void);
s32 Func_0200b718(s32);
void Func_0200ad9e(s32, s32, s32, s32);
s32 *Func_0200ba7e(s32);
s32 Func_0200ba3e(s32);
u8 *Func_0200ba90(s32);
u8 *Func_0200ba98(s32);
s32 Func_0200b99e(s32);
s32 Func_0200b9b4(s32);
s32 Func_0200b9ce(s32);
s32 Func_0200b9c4(void);
s32 Func_0200b9ca(void);
OrbitingSceneObject *Func_0200bb38();
s32 Func_0200bb38_a(s32);
void Func_0200bad2(OrbitingSceneObject *, s32);
u8 *Func_0200ba98_a(s32, s32);
void Func_0200bb60(s32);
s32 Func_0200bac8(u8, s32, u8 *);
void Func_0200babe(s32);
void Func_0200bba8(s32, s32);
void Func_0200bbb2(s32, s32);
void Func_0200b9b8(s32);
s32 Func_0200baf8(s32, s32);
void Func_0200bbf0(s32, s32);
void Func_0200b9f4(s32);
s32 Func_0200bb34(s32, s32);
void Func_0200bc2c(s32, s32);

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

/* Resolved engine calls: each pseudo symbol is the per-site call word the
 * overlay image holds (a word can serve two sites with different targets),
 * and the macro names the engine function the site reaches through the
 * overlay veneer and the main-image veneer island, keeping the site's own
 * calling form. Names without a repository binding are provisional.
 */

/* Actor IDs the scene sets up: three actors share a common data pointer,
 * a fourth actor (11) is driven through its own sequence of position/speed
 * calls. */

/* Shared data used for actors A, B, C. */

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
    extern u8 Data_03001ebc[];

    u8 *work = *(u8 **)Data_03001ebc;

    *(u16 *)(work + 0x1d8) = (u16)(*(u16 *)(work + 0x1d8) + amount);
}

/* Call sites spelled through these wrappers pass their constants straight
 * into the argument registers; a direct call precomputes a costly constant
 * into a pseudo that the compiler then shares with later uses in the block.
 * A value-returning call also sets r0 last of its arguments. */
static __inline__ void Call3_02000dc0(void (*f)(), s32 a0, s32 a1, s32 a2)
{
    extern u8 Data_03001ebc[];

    f(a0, a1, a2);
}

/* Call sites spelled through these wrappers pass their constants straight
 * into the argument registers; a direct call precomputes a costly constant
 * into a pseudo that the compiler then shares with later uses in the block.
 * A value-returning call also sets r0 last of its arguments. */
static __inline__ void Call3_02000e54(void (*f)(), s32 a0, s32 a1, s32 a2)
{
    extern u8 Data_03001ebc[];

    f(a0, a1, a2);
}

/* Call sites spelled through these wrappers pass their constants straight
 * into the argument registers; a direct call precomputes a costly constant
 * into a pseudo that the compiler then shares with later uses in the block.
 * A value-returning call also sets r0 last of its arguments. */
static __inline__ void Call3_02000e84(void (*f)(), s32 a0, s32 a1, s32 a2)
{
    extern u8 Data_03001ebc[];

    f(a0, a1, a2);
}

/* Call sites spelled through these wrappers pass their constants straight
 * into the argument registers, matching the reference codegen. */
static __inline__ s32 Value2(s32 (*f)(), s32 a0, s32 a1)
{
    extern u8 Data_03001ebc[];

    return f(a0, a1);
}

static __inline__ s32 Value3(s32 (*f)(), s32 a0, s32 a1, s32 a2)
{
    extern u8 Data_03001ebc[];

    return f(a0, a1, a2);
}

static __inline__ void Call1_02000f5c(void (*f)(), s32 a0)
{
    extern u8 Data_03001ebc[];

    f(a0);
}

static __inline__ void Call2(void (*f)(), s32 a0, s32 a1)
{
    extern u8 Data_03001ebc[];

    f(a0, a1);
}

/* Call sites spelled through these wrappers pass their constants straight
 * into the argument registers; a direct call precomputes a costly constant
 * into a pseudo that the compiler then shares with later uses in the block.
 * A value-returning call also sets r0 last of its arguments. */
static __inline__ void Call3_02000f8c(void (*f)(), s32 a0, s32 a1, s32 a2)
{
    extern u8 Data_03001ebc[];

    f(a0, a1, a2);
}

/* Runs four fixed steps in order: a single-argument call, a call that takes
 * the address of the byte at Value_0200f55a plus two constants, another
 * three-constant call, and a final single-argument call. */
static __inline__ s32 Value2_02000fbc(s32 (*f)(), s32 a0, s32 a1)
{
    extern u8 Data_03001ebc[];

    return f(a0, a1);
}

static __inline__ void Call1_02000fbc(void (*f)(), s32 a0)
{
    extern u8 Data_03001ebc[];

    f(a0);
}

/* Call sites spelled through these wrappers pass their constants straight
 * into the argument registers; a direct call precomputes a costly constant
 * into a pseudo that the compiler then shares with later uses in the block.
 * A value-returning call also sets r0 last of its arguments. */
static __inline__ s32 Value1_020011d8(s32 (*f)(), s32 a0)
{
    extern u8 Data_03001ebc[];

    return f(a0);
}

static __inline__ s32 Value2_02002cb0(s32 (*f)(), s32 a0, s32 a1)
{
    extern u8 Data_03001ebc[];

    return f(a0, a1);
}

static __inline__ void Call4(void (*f)(), s32 a0, s32 a1, s32 a2, s32 a3)
{
    extern u8 Data_03001ebc[];

    f(a0, a1, a2, a3);
}

/* Newly identified engine calls: each macro below names the site's own
 * engine function from its reconstructed C source, and keeps the site's
 * own calling form. */

/* Call sites spelled through these wrappers pass their constants straight
 * into the argument registers; a direct call precomputes a costly constant
 * into a pseudo that the compiler then shares with later uses in the block.
 * A value-returning call also sets r0 last of its arguments. */
static __inline__ void Call1_02002f14(void (*f)(), s32 a0)
{
    extern u8 Data_03001ebc[];

    f(a0);
}

static __inline__ s32 Value2_02002f14(s32 (*f)(), s32 a0, s32 a1)
{
    extern u8 Data_03001ebc[];

    return f(a0, a1);
}

static __inline__ s32 Value2_020031b4(s32 (*f)(), s32 a0, s32 a1)
{
    extern u8 Data_03001ebc[];

    return f(a0, a1);
}

/* Call sites spelled through these wrappers pass their constants straight
 * into the argument registers; a direct call precomputes a costly constant
 * into a pseudo that the compiler then shares with later uses in the block.
 * A value-returning call also sets r0 last of its arguments. */
static __inline__ s32 Value1_020034c8(s32 (*f)(), s32 a0)
{
    extern u8 Data_03001ebc[];

    return f(a0);
}

static __inline__ s32 Value2_020034c8(s32 (*f)(), s32 a0, s32 a1)
{
    extern u8 Data_03001ebc[];

    return f(a0, a1);
}

static __inline__ void Call6(void (*f)(), s32 a0, s32 a1, s32 a2, s32 a3, s32 a4, s32 a5)
{
    extern u8 Data_03001ebc[];

    f(a0, a1, a2, a3, a4, a5);
}

static __inline__ void Call11(void (*f)(), s32 a0, s32 a1, s32 a2, s32 a3, s32 a4, s32 a5, s32 a6, s32 a7, s32 a8, s32 a9, s32 a10)
{
    extern u8 Data_03001ebc[];

    f(a0, a1, a2, a3, a4, a5, a6, a7, a8, a9, a10);
}

/*
 * Staged actor cutscene tail -- resource_373. The long scene script plus the
 * particle, sound, and vertical-motion effects it drives.
 */

/*
 * Each alias names the loader-relocated call word the image holds, not a
 * runtime address. The declarations are old-style because the call sites vary
 * in arity.
 */

/*
 * Calls spelled through these wrappers pass their constants straight into the
 * argument registers. A direct call instead precomputes a costly constant into
 * a pseudo shared with later uses in the block, and a value-returning call
 * sets r0 last of its arguments.
 */
static __inline__ void Call1_02003fb0(void (*f)(), s32 a0)
{
    f(a0);
}

static __inline__ void Call6_02003fb0(
    void (*f)(), s32 a0, s32 a1, s32 a2, s32 a3, s32 a4, s32 a5)
{
    f(a0, a1, a2, a3, a4, a5);
}

static __inline__ s32 Value2_02003fb0(s32 (*f)(), s32 a0, s32 a1)
{
    return f(a0, a1);
}

s32 Func_02000030(s32 *first_position, s32 *second_position)
{
    typedef s32(*IwramIntegerSquareRoot)(s32);

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

void MapStagedActor_AdvancePair(void)
{


    s32 destination[3];
    struct StagedActor *lead_actor;
    struct StagedActor *next_actor;
    struct StagedActor *blocking_actor;
    s32 direction_index;
    u32 direction_step;
    s32 move_speed;
    s32 transition_state;

    lead_actor = GetStagedActor(0);
    direction_index = lead_actor->direction_and_kind >> 12;
    direction_step = StagedActorStepTable[direction_index];
    destination[0] = lead_actor->x.value + (direction_step & 0xffff0000);
    destination[1] = lead_actor->y;
    direction_step <<= 16;
    destination[2] = lead_actor->z.value + direction_step;
    next_actor = FindNextStagedActor(destination, lead_actor);
    if (next_actor == 0) return;

    direction_step = StagedActorStepTable[direction_index];
    destination[0] = next_actor->x.value + (direction_step & 0xffff0000);
    destination[1] = next_actor->y;
    direction_step <<= 16;
    destination[2] = next_actor->z.value + direction_step;
    blocking_actor = FindBlockingStagedActor(destination, next_actor);
    if (blocking_actor != 0 && (blocking_actor->collision_flags & 1) != 0) return;

    destination[0] = next_actor->x.value;
    destination[1] = next_actor->y + 0x100000;
    destination[2] = next_actor->z.value;
    blocking_actor = FindElevatedBlockingStagedActor(destination, next_actor);
    if (blocking_actor != 0 && (blocking_actor->collision_flags & 1) != 0) return;

    next_actor->transition_mode = 2;
    direction_step = StagedActorStepTable[direction_index];
    destination[0] = next_actor->x.value + (direction_step & 0xffff0000);
    destination[1] = next_actor->y;
    direction_step <<= 16;
    destination[2] = next_actor->z.value + direction_step;
    if (CanStartStagedActorMove(next_actor, destination) > 0) return;

    transition_state = next_actor->transition_busy;
    if (transition_state != 0) return;

    SetStagedActorMode(lead_actor, 8);
    move_speed = 0x3333;
    SelectStagedActorSlot(15);
    StartStagedActorEffect(185);
    next_actor->move_rate_x = move_speed;
    next_actor->move_rate_z = move_speed;
    StartNextStagedActorMove(next_actor, destination[0], destination[1], destination[2]);
    lead_actor->move_rate_x = move_speed;
    lead_actor->move_rate_z = move_speed;
    StartLeadStagedActorMove(lead_actor, destination[0], destination[1], destination[2]);
    FinishStagedActorMove(next_actor);
    FinishStagedActorEffect();
    next_actor->x.value = destination[0];
    next_actor->z.value = destination[2];
    next_actor->unknown_24 = transition_state;
    next_actor->unknown_2c = transition_state;
    lead_actor->unknown_38 = 0x80000000;
    lead_actor->unknown_40 = 0x80000000;
    lead_actor->unknown_24 = transition_state;
    lead_actor->unknown_2c = transition_state;
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

s32 MapStagedActor_StopBlockedMotion(SceneActorRecord *actor)
{
    extern s32 Data_0200e190[];

    StagedMoveTarget target;
    u32 direction_index;
    s32 direction_step;
    SceneActorRecord *blocking_actor;

    direction_index = actor->unk6 >> 12;
    direction_step = Data_0200e190[direction_index];
    target.unk0 = actor->unk8 + (direction_step & 0xffff0000);
    target.unk4 = actor->unkC;
    direction_step = direction_step << 16;
    target.unk8 = actor->unk10 + direction_step;
    blocking_actor = Func_02000342(&target, actor);
    if (blocking_actor != 0) {
        u32 kind_index = 0;
        s32 blocking_kind = *blocking_actor->unk50->unk28;
        s32 *p = Data_0200e1d0;

        do {
            if (blocking_kind == *p++) goto done;
            kind_index++;
        } while (kind_index <= 5);
        actor->unk24 = 0;
        actor->unk2C = 0;
        actor->unk38 = 0x80000000;
        actor->unk40 = 0x80000000;
    }
    direction_step = Data_0200e190[direction_index];
    target.unk0 = actor->unk8 + (direction_step & 0xffff0000);
    target.unk4 = actor->unkC;
    direction_step = direction_step << 16;
    target.unk8 = actor->unk10 + direction_step;
    if (Func_02006266(actor, &target) > 0) {
        actor->unk24 = 0;
        actor->unk2C = 0;
        actor->unk38 = 0x80000000;
        actor->unk40 = 0x80000000;
    }
done:
    return 0;
}

s32 MapStagedActor_FindClearPosition(s32 *request)
{
    extern s32 Data_0200e190[];

    s32 direction_index;
    s32 origin[3];
    u8 *actor;
    s32 step_count;
    s32 tiles_y, tiles_x;
    u8 *active_flag;
    s32 footprint_offset;
    s32 extent_a, extent_b;
    u8 *footprint_table;
    s32 *candidate;
    s32 found;
    request[5] = 0;
    actor = (u8 *)Func_020007de(&direction_index, request + 1, request);
    if (actor == 0)
        return 0;
    active_flag = actor + 0x22;
    *active_flag = 2;
    step_count = 0;
    footprint_table = (u8 *)Data_0200e1e8;
    footprint_offset = request[0] << 4;
    {
        s32 table_offset = footprint_offset + 4;
        extent_a = *(s32 *)(footprint_table + table_offset);
        if (extent_a < 0)
            extent_a = -extent_a;
        table_offset = footprint_offset;
        table_offset += 12;
        extent_b = *(s32 *)(footprint_table + table_offset);
        if (extent_b < 0)
            extent_b = -extent_b;
        tiles_y = (extent_a + extent_b) >> 4;
        extent_a = *(s32 *)(footprint_table + footprint_offset);
        if (extent_a < 0)
            extent_a = -extent_a;
        table_offset = footprint_offset;
        table_offset += 8;
        extent_b = *(s32 *)(footprint_table + table_offset);
        if (extent_b < 0)
            extent_b = -extent_b;
        tiles_x = (extent_a + extent_b) >> 4;
    }
    {
        u8 *actor_bytes;
        s32 actor_y;
        s32 direction_x;
        candidate = origin;
        direction_x = Data_0200e190[direction_index] & 0xffff0000;
        actor_bytes = actor;
        candidate[0] = *(s32 *)(actor_bytes + 8) + direction_x;
        actor_y = *(s32 *)(actor_bytes + 12);
        candidate[1] = actor_y;
        candidate[2] = *(s32 *)(actor_bytes + 16) + (Data_0200e190[direction_index] << 16);
        request[3] = actor_y;
    }
    for (;;) {
        s32 row, column;
        {
            u8 *table = (u8 *)Data_0200e1e8;
            s32 table_offset = request[0] << 4;
            table_offset += 4;
            request[4] = origin[2] + (*(s32 *)(table + table_offset) << 16);
        }
        row = 0;
        while (row < tiles_y) {
            {
                u8 *table = (u8 *)Data_0200e1e8;
                request[2] = origin[0] + (*(s32 *)(table + (request[0] << 4)) << 16);
            }
            column = 0;
            while (column < tiles_x) {
                s32 *probe = request + 2;
                if (Func_020064a0(actor, probe) == 2)
                    goto found;
                probe[0] = probe[0] + 0x100000;
                column++;
            }
            request[4] = request[4] + 0x100000;
            row++;
        }
        step_count++;
        origin[0] = origin[0] + (Data_0200e190[direction_index] & 0xffff0000);
        origin[2] = origin[2] + (Data_0200e190[direction_index] << 16);
    }
found:
    *active_flag = 0;
    found = 0;
    if (step_count != 0) {
        s32 direction = Data_0200e190[direction_index];
        s32 direction_x = direction & 0xffff0000;
        s32 offset_x = direction_x *step_count;
        s32 offset_z = (direction << 16) * step_count;
        u8 *actor_bytes = actor;
        request[2] = *(s32 *)(actor_bytes + 8) + offset_x;
        request[3] = *(s32 *)(actor_bytes + 12);
        request[4] = *(s32 *)(actor_bytes + 16) + offset_z;
        found = 1;
    }
    return found;
}

void Func_02000608(StagedActorMovementRequest request)
{
#include "run_staged_actor_movement_and_redraw_body.inc"
}

s32 MapStagedScene_SelectPrimaryData(void)
{
    return (s32)Data_0200e708;
}

s32 MapStagedScene_GetEmptyData(void)
{
    return 0;
}

s32 MapStagedScene_SelectSecondaryData(void)
{
    return (s32)Data_0200e870;
}

s32 MapStagedScene_SelectTertiaryData(void)
{
    u8 *scene_state = Data_02000240;
    if (*(s16 *)(scene_state + 0x1c2) == 16)
        return (s32)Data_0200ee48;
    if (Func_020069a6(0x87a) != 0)
        return (s32)Data_0200ecc8;
    if (Func_020069b4(0x815) != 0)
        return (s32)Data_0200eab8;
    return (s32)Data_0200e8a8;
}

void SceneDialogue_RunActor181Scene(void)
{
    Func_02006a04();
    Func_02006a8e(26, 0, 0);
    Func_020069fe(0xfd0);
    Func_02006b7e(0xb5, 3);
    Func_02006a3e(0xb5, 0);
    Func_02006a32();
}

void FieldScene_RunActor181Scene(void)
{
    Func_02006a34();
    Func_02006abe(20, 0, 0);
    Func_02006a2e(0xfd0);
    Func_02006bae(0xb5, 3);
    Func_02006a6e(0xb5, 0);
    Func_02006a62();
}

s32 MapStagedScene_SelectQuaternaryData(void)
{
    if (Func_02006a46(0x87a) != 0)
        return (s32)Data_0200f334;
    if (Func_02006a54(0x815) != 0)
        return (s32)Data_0200f100;
    return (s32)Data_0200ef38;
}

void SceneDialogue_RunActorTenFlaggedDialogue(void)
{
    Func_02006a9c();
    if (Func_02006a82(0x815) != 0) {
        Func_02006b74(0x11cc);
        Func_02006b8c(10, 0);
    } else {
        Func_02006b84(0xf81);
        Func_02006b7e_a(10, 0, 4);
        Func_02006bb6(10, 0);
    }
    Func_02006ada();
}

void SceneDialogue_RunActorFourteenTalk(void)
{
    s32 flag = 0x806;
    Func_02006ae8();
    if (Func_02006ace(0x815) != 0) {
        Func_02006bc0(0x11c9);
        Func_02006bd8(14, 0);
    } else if (Func_02006ae8_a(flag) == 0) {
        Func_02006afa(flag);
        Func_02006be0(0xf7c);
        Func_02006bda(14, 0, 4);
        Func_02006c12(14, 0);
    } else {
        Func_02006bfa(0xf7e);
        Func_02006bf4(14, 0, 4);
        Func_02006c1c(14, 0);
    }
    Func_02006b50();
}

void FieldScene_RunFlag807BranchSequence(void)
{
    extern u8 Data_03001ebc[];

    u32 i;
    s32 record;

    Func_02006b68();
    if (Value1(Func_02006b4e, 0x807) == 0) {
        Call1(Func_02006b60, 0x807);
        Call1(Func_02006c46, 0xf63);
        Call3(Func_02006c90, 18, 0x103, 0);
        Func_02006c4a(0, 18, 20);
        Func_02006c7c(18, 0, 6);
        Call3(Func_02006c98, 18, 0x8000, 30);
        Func_02006c4a_a(18, 2, 20);
        Func_02006c9c(18, 0, 6);
        Func_02006c7e(18, 0, 10);
        Call3(Func_02006cd8, 18, 0x103, 0);
        Func_02006cba(18, 0, 10);
        Call3(Func_02006cee, 0, 0x102, 60);
    } else {
        Call3(Func_02006cfa, 18, 0x103, 0);
        Call1(Func_02006cc0, 0xf66);
        Func_02006ce2(18, 0, 20);
    }
    Func_02006c0e();
}

void SceneDialogue_RunActor21FlaggedLine(void)
{
    Func_02006c20();
    if (Func_02006c06(0x202) != 0) {
        Func_02006cf8(0xf68);
    } else {
        Func_02006d00(0xf69);
    }
    Func_02006d18(21, 0);
    Func_02006c4c();
}

void SceneDialogue_RunActor10LineAndFlag81f(void)
{
    Func_02006c5c();
    Func_02006d1e(0, 10, 20);
    Func_02006d34(0x1c8d);
    Func_02006d4c(10, 0);
    Func_02006c62(0x81f);
    Func_02006c86();
}

void FieldScene_RunScene373_02000cd0(void)
{
    extern u8 Data_03001ebc[];

    u32 i;
    s32 record;

    Func_02006c90_a();
    Call3(Func_02006c38, 0x30000, 0x30000, 0x10000);
    Func_02006b76(10);
    Call3(Func_02006c4c_a, -1, -1, 0xe666);
    Call1(Func_02006d82, 0x1c9a);
    Func_02006da4(17, 0, 20);
    Func_02006d7e(17, 0, 20);
    Func_02006dae(17, 0);
    Func_02006ce2_a();
}

void SceneDialogue_RunActorNineteenDialogue(void)
{
    Func_02006cec();
    Func_02006d9c(19, 2);
    Func_02006cf2(20);
    Func_02006db4(19, 0, 20);
    Func_02006dd2(0x1c9d);
    Func_02006dfa(19, 0);
    Func_02006d00_a(0x307);
    Func_02006d24();
}

void SceneState_Send210AndApplyRectAt40x84(void)
{
    s32 m, n;
    Func_02006d1e_a(0x210);
    m = 10;
    n = 84;
    Func_02006cca(40, 84, 7, 4, m, n);
}

void SceneState_Send210AndApplyRect(void)
{
    s32 m, n;
    Func_02006d4e(0x210);
    m = 10;
    n = 84;
    Func_02006cf2_a(40, 89, 7, 4, m, n);
}

void FieldScene_RunScene373_02000dc0(void)
{
    extern u8 Data_03001ebc[];

    u32 i;
    s32 record;

    Func_02006f12(188);
    Call3_02000dc0(Func_02006cfc, 0x200f544, 45, 11);
    Call3_02000dc0(Func_02006df8, 0, 0x101, 0x1a4);
    Func_02006ede(11);
}

void SceneState_ApplyFlag801Branch(void)
{
    if (Func_02006d92(0x801) == 0) {
        Func_02001dea();
    } else {
        Func_02006f52(123);
        Func_02006f08(1);
    }
}

void SceneState_SetValue123Mode3(void)
{
    Func_02006f6a(123);
    Func_02006f20(3);
}

void SceneState_SetValue123Mode4(void)
{
    Func_02006f7e(123);
    Func_02006f34(4);
}

void SceneState_ApplyValues123And2(void)
{
    Func_02006f92(123);
    Func_02006f48(2);
}

void FieldScene_RunScene373_02000e54(void)
{
    extern u8 Data_03001ebc[];

    u32 i;
    s32 record;

    Func_02006fa6(158);
    Call3_02000e54(Func_02006d90, 0x200f55a, 54, 32);
    Call3_02000e54(Func_02006e8c, 0, 0x196, 0x2d7);
    Func_02006f72(5);
}

void FieldScene_RunScene373_02000e84(void)
{
    extern u8 Data_03001ebc[];

    u32 i;
    s32 record;

    Func_02006fd6(158);
    Call3_02000e84(Func_02006dc0, 0x200f570, 45, 39);
    Call3_02000e84(Func_02006ebc, 0, 0x106, 0x325);
    Func_02006fa2(6);
}

void SceneDialogue_RunFlag815GatedStep(void)
{
    if (Func_02006e56(0x815) != 0 && Func_02006e60(0x87a) == 0) {
        Func_02006e88();
        Func_02006f56(0x11b6);
        Func_02006f66(21, 0);
        if (Func_02006ebe(0, 0) == 0) {
            Func_02006f8c(21, 0, 60);
            Func_02006f8c_a(21, 0);
        } else {
            u8 *b = *(u8 **)0x03001ebc;
            u16 *h = (u16 *)(b + 0x1d8);
            *h = *h + 2;
            Func_02006ec4(40);
            Func_02006fac(21, 0);
        }
        Func_02006ee0();
    } else {
        Func_02007070(0x9e);
        Func_02006e5a((s32)Data_0200f55a, 50, 44);
        Func_02006f58(0, 0x154, 0x378);
        Func_0200703e(7);
    }
}

/* Runs four scene primitives in sequence: one single-argument call, one call
 * passing the address of Value_0200f570 plus two small constants, one call
 * with a byte-flag-sized first argument (0) and two larger constants, and a
 * final single-argument call. */
void FieldScene_RunPrimarySequence(void)
{
    extern u8 Data_03001ebc[];

    Audio_PlayCue_1(158);
    Call3(Func_02006e98, (s32)&Value_0200f570, 49, 69); /* main:08009178 */
    ObjectMotion_ResetAndSetPositionInMode2_1(0, 326, 0x466); /* object_id 0, x 326, z 0x466 */
    Call1_02000f5c(Func_0200707a, 8); /* main:0808a248 */
}

/* Runs a short scripted step, then two 3-argument setup calls, then another
 * short scripted step; none of the callees' effects are visible here. */
void FieldScene_RunScene373SequenceA(void)
{
    extern u8 Data_03001ebc[];

    Audio_PlayCue_1_02000f8c(158);
    Call3_02000f8c(Func_02006ec8, 0x200f586, 52, 76); /* main:08009178 */
    ObjectMotion_ResetAndSetPositionInMode2_1_02000f8c(0, 0x176, 0x4d6); /* object_id 0, x 0x176, z 0x4d6 */
    Func_020070aa(9); /* main:0808a248 */
}

void FieldScene_RunPrimarySequenceSecond(void)
{
    extern u8 Data_03001ebc[];

    Audio_PlayCue_1_02000fbc(158);
    Call3(Func_02006ef8, (s32)&Value_0200f55a, 35, 74); /* main:08009178 */
    ObjectMotion_ResetAndSetPositionInMode2_1_02000fbc(0, 102, 0x4b6);
    Call1_02000fbc(Func_020070d8, 10); /* main:0808a248 */
}

void FieldScene_RunScene373SequenceC(void)
{
    extern u8 Data_03001ebc[];

    u32 i;
    s32 rec7;
    s32 rec8;
    s32 record;

    rec8 = Value1(Func_02006fd6_a, 0);
    rec7 = Value1(Func_02006fde, 5);
    Func_02006fbc();
    *(volatile s32 *)(rec7 + 8) = *(volatile s32 *)(rec8 + 8);
    *(volatile s32 *)(rec7 + 12) = *(volatile s32 *)(rec8 + 12);
    *(volatile s32 *)(rec7 + 16) = *(volatile s32 *)(rec8 + 16);
    *(volatile s32 *)(rec7 + 56) = -0x80000000;
    *(volatile s32 *)(rec7 + 60) = -0x80000000;
    *(volatile s32 *)(rec7 + 64) = -0x80000000;
    *(volatile s32 *)(rec7 + 36) = 0;
    *(volatile s32 *)(rec7 + 40) = 0;
    *(volatile s32 *)(rec7 + 44) = 0;
    *(volatile s32 *)(rec7 + 20) = *(volatile s32 *)(rec8 + 12);
    Func_02006eb4(1);
    Call3(Func_02007022, 5, 0x10000, 0x8000);
    Call3(Func_02007064, 5, 110, 0x11b);
    Func_020070be(0, 5, 2);
    Call1(Func_020070d4, 0xf39);
    if (*(volatile s32 *)(rec8 + 8) < *(volatile s32 *)(rec7 + 8)) {
        Call3(Func_020070fe, 0xa005, 0, 2);
    } else {
        Call3(Func_0200710a, 0x8005, 0, 2);
    }
    Func_020070c2(0, 3);
    Func_02007030(2);
    Func_020070c8(5, 2);
    record = Value1(Func_0200706e, 0);
    if (record != 0) {
        Func_020070a8(5, *(s16 *)(record + 10), *(s16 *)(record + 18));
    }
    Func_020070d6(5);
    Func_020070e8(5, 0, 0);
    Call3(Func_020070da, 0, 110, 0x12f);
    Func_0200707e();
}

void FieldScene_RunScene373SequenceD(void)
{
    extern u8 Data_03001ebc[];

    u32 i;
    s32 record;
    s32 base5_f4d;

    if (Value1_020011d8(Func_0200717a, 0x808) == 0) {
        Func_020071a2();
        Call3(Func_020071e0, 0, 0x10000, 0x8000);
        base5_f4d = (s32)Data_00000f4d;
        Func_02007280(base5_f4d);
        Func_020072a2(15, 0, 2);
        Func_020072ae(16, 0, 2);
        Func_02007196((base5_f4d + 2), 1);
        Func_020071d4(6);
        Call3(Func_0200724e, 0, 69, 0x366);
        Func_020071f2();
    }
}

void SceneState_RunFlag204Step(void)
{
    s32 m, n;
    Func_02007206();
    m = 20;
    n = 50;
    Func_0200719a(49, 53, 8, 4, m, n);
    Func_0200451a(0, 10, 11, 1);
    Func_02007216(0x204);
    Func_0200723a();
}

void SceneState_SetFlag204AndConfigureRegion49_46(void)
{
    s32 p5, p6;
    Func_02007242();
    Func_02004612(0, 13, 10, 1);
    Func_02007246(0x204);
    p5 = 20;
    p6 = 50;
    Func_020071ea(49, 46, 8, 4, p5, p6);
    Func_02007276();
}

void FieldScene_RunScene373SequenceE(void)
{
    extern u8 Data_03001ebc[];

    u32 i;
    u8 *rec7;
    s32 record;

    rec7 = Value1(Func_020072a6, 22);
    Func_02007284();
    Call3(Func_020072c2, 0, 0x20000, 0x20000);
    Func_02007334(0, 5, 0);
    Call3(Func_02007308, 0, 215, 0x193);
    rec7[90] |= 1;
    Call3(Func_0200733c, 22, 0xa60000, 0x1770000);
    Call3(Func_020073b8, 22, 0x2000, 20);
    rec7[90] = (rec7[90] ^ 1);
    Call3(Func_0200730c, 22, 0x28000, 0x28000);
    Func_0200737e(22, 4, 0);
    Call3(Func_02007358, 22, 202, 0x18b);
    Func_02007380(0, 1);
    Func_020072f6(10);
    Call3(Func_020073fa, 0, 0xb000, 0);
    Call3(Func_02007406, 22, 0x3000, 24);
    Call3(Func_0200742a, 0, 0x100, 0);
    Func_020073ca(0, 2);
    Func_02007328(20);
    Call3(Func_0200736e, 22, 0x18000, 0x10000);
    Call2(Func_0200737e_a, 0, 0x200f59c);
    Func_02007344(10);
    Call3(Func_0200745e, 22, 0x103, 0);
    Call2(Func_02007396, 22, 0x200f5ec);
    Func_020073a4(0);
    Call3(Func_020073da, 0, 0x100, 0x1da);
    Call3(Func_0200746e, 0, 0xc000, 0);
    Func_020073c4(22);
    Call3(Func_020073fa_a, 22, 0x100, 0x1c8);
    Func_02007422(0, 1);
    Call3(Func_02007496, 22, 0x4000, 20);
    Func_0200744e(22, 2);
    Func_020073ac(20);
    Call1(Func_02007482, 0xfce);
    Func_020074aa(22, 0);
    record = Func_020073f0(22);
    *(volatile s32 *)(record + 108) = 0x200d72d;
    Call2(Func_0200740c, 22, 0x200e248);
    Call1(Func_020073c2, 0x823);
    Func_020073e6();
}

void SceneState_RunTablePairWhenActor22State1(void)
{
    u8 *p = Func_0200743e(22);
    if (Func_020073fe(0x823) != 0) {
        u8 *q = p;
        q += 100;
        if (*(s16 *)q == 1) {
            Func_0200290a((s32)Data_0200f63c, (s32)Data_0200f6cc);
        }
    }
}

void FieldScene_RunScene373_02001490(s32 a0, s32 a1)
{
    extern u8 Data_03001ebc[];

    u32 i;
    s32 p8;
    s32 rec8;
    s32 record;

    p8 = a1;
    rec8 = Value1(Func_02007482_a, 22);
    Func_02007460();
    Func_02007508(22, 2);
    Call3(Func_0200757c, 22, 0x100, 20);
    Func_0200751c(0, 2);
    Call3(Func_02007590, 0, 0x102, 40);
    Func_020074c8(0, a0);
    Func_0200748e(10);
    Call3(Func_020075a8, 22, 0x103, 0);
    Func_020074f0(22, p8);
    Func_020074ee(0);
    Func_020074ae(20);
    Func_02007568(22, 2);
    *(s32 *)(rec8 + 24) = 0x10000;
    *(s32 *)(rec8 + 28) = 0x10000;
    record = Value1(Func_020074f2, 0);
    *(s32 *)(record + 24) = 0x10000;
    *(s32 *)(record + 28) = 0x10000;
    Call1(Func_0200759c, 0xfce);
    Func_020075c4(22, 0);
    record = Func_0200750a(22);
    *(s32 *)(record + 108) = 0x200d72d;
    Call2(Func_02007526, 22, 0x200e248);
    Func_020074fa();
}

void SceneState_RunTablePairWhenActor22State2(void)
{
    u8 *p = Func_0200753e(22);
    if (Func_020074fe(0x823) != 0) {
        u8 *q = p;
        q += 100;
        if (*(s16 *)q == 2) {
            Func_02002a0a((s32)Data_0200f748, (s32)Data_0200f7c4);
        }
    }
}

void SceneState_RunTablePairByActor22State(void)
{
    u8 *rec = Func_0200757a(22);
    if (Func_0200753a(0x823) != 0) {
        u8 *q = rec;
        s32 v;
        q += 100;
        v = *(s16 *)q;
        if (v == 1) {
            Func_02002a46((s32)Data_0200f748, (s32)Data_0200f6cc);
        } else if (v == 2) {
            Func_02002a54((s32)Data_0200f748, (s32)Data_0200f7c4);
        }
    }
}

/* Runs a scripted movement/pose sequence for actors 0, 1 and 8, reading two
 * lookup records along the way (one 32-bit-field record, one 16-bit-field
 * record) to copy their stored values onto actor 1. */
void FieldScene_RunSecondaryActorSequence(void)
{
    extern u8 Data_03001ebc[];

    u32 i;
    s32 record;
    s32 slot_table;

    BattleRuntime_Reset_1();
    ObjectGroup_ConfigureChildValue_1(0, 0);
    ObjectMotion_SetAngleToward_1(8, 0, 20);
    slot_table = (s32)Data_00001c45;
    SceneWork_SetStepValue_1(slot_table);
    ObjectMotion_SetVariantCallback_1(8, 2);
    BattleRuntime_RunThenWaitIfModeZero_1(8, 0, 20);
    ObjectMotion_SetSpeedLimitAndAcceleration_1(0x10000, 0x2000); /* main:0808a208 */
    ObjectMotion_PlaceWithinCameraBounds_1(0x18e0000, -1, 0x2460000, 1);
    ObjectMotion_SetSpeedParameters_1(0, 0xcccc, 0x6666);
    ObjectMotion_SetSpeedParameters_2(1, 0xcccc, 0x6666);
    ObjectMotion_SetPositionAndReset_1(0, 0x1a4, 0x260);
    ObjectMotion_ArmCallback_1(0, 0xa000, 0);
    ObjectMotion_ArmCallback_2(8, 0x3000, 0);
    record = Scene_GetRecord_1(0);
    if (record != 0) {
        /* Copy the record's fields at +8 and +16 onto actor 1. */
        ObjectMotion_SetHorizontalPositionWithTerrain_1(1, *(s32 *)(record + 8), *(s32 *)(record + 16));
    }
    ObjectMotion_SetPositionAndReset_2(1, 0x192, 0x260);
    ObjectMotion_ArmCallback_3(1, 0xd000, 20);
    BattleEvent_RunActionAndWait_1(0x1001, 0);
    ObjectMotion_ArmCallback_4(8, 0x5000, 20);
    ObjectMotion_CallThenWaitForAnimationChange_1(8, 3); /* main:0808a110 */
    BattleEvent_RunActionAndWait_2(0x4008, 0);
    Object_SetModeById_1(0, 3);
    ObjectMotion_CallThenWaitForAnimationChange_2(1, 3); /* main:0808a110 */
    BattleRuntime_WaitIfModeZero_1(20);
    ObjectMotion_SetVariantCallbackAndInvokeObject_1(8, 2); /* main:0808a138 */
    ObjectMotion_SetSpeedParameters_3(0x4008, 0); /* main:0808a178 */
    if (UiWork_WaitThenFinalizeCapacity_1(0, 0) == 1) { /* main:0808a070 */
        bump_step(1);
        ObjectMotion_SetVariantCallback_2(8, 1);
    }
    BattleRuntime_RunThenWaitIfModeZero_2(0x4008, 0, 40); /* main:0808a188 */
    BattleEffect_SpawnLinkedResourceObject_1(8, 0x105, 60);
    /* Pass the slot table's field at +6 for the slot passed above. */
    SceneWork_SetStepValue_2((slot_table + 6)); /* main:0808a170 */
    BattleRuntime_RunThenWaitIfModeZero_3(0x4008, 0, 20); /* main:0808a188 */
    ObjectMotion_SetVariantCallbackAndInvokeObject_2(1, 1); /* main:0808a138 */
    BattleRuntime_WaitIfModeZero_2(40);
    BattleRuntime_RunThenWaitIfModeZero_4(0x1001, 0, 40); /* main:0808a188 */
    ObjectMotion_SetVariantCallbackAndInvokeObject_3(8, 1); /* main:0808a138 */
    ObjectMotion_ArmCallback_5(8, 0xd000, 20);
    BattleEvent_RunActionAndWait_3(0x4008, 0);
    ObjectMotion_CallThenWaitForAnimationChange_3(1, 3); /* main:0808a110 */
    BattleRuntime_RunThenWaitIfModeZero_5(0x1001, 0, 120); /* main:0808a188 */
    BattleRuntime_RunThenWaitIfModeZero_6(0x4008, 0, 20); /* main:0808a188 */
    BattleEffect_SpawnLinkedResourceObject_2(1, 0x105, 40);
    BattleRuntime_RunThenWaitIfModeZero_7(0x1001, 0, 40); /* main:0808a188 */
    ObjectMotion_CallThenWaitForAnimationChange_4(8, 4); /* main:0808a110 */
    BattleRuntime_RunThenWaitIfModeZero_8(0x4008, 0, 20);
    ObjectMotion_CallThenWaitForAnimationChange_5(1, 3); /* main:0808a110 */
    BattleRuntime_WaitIfModeZero_3(40);
    ObjectMotion_ArmCallback_6(8, 0x5000, 20);
    BattleRuntime_RunThenWaitIfModeZero_9(0x4008, 0, 10); /* main:0808a188 */
    Object_SetModeById_2(0, 3);
    ObjectMotion_CallThenWaitForAnimationChange_6(1, 3); /* main:0808a110 */
    BattleRuntime_WaitIfModeZero_4(20);
    ObjectMotion_CallThenWaitForAnimationChange_7(8, 3);
    Object_SetModeById_3(1, 2);
    record = Scene_GetRecord_2(0);
    if (record != 0) {
        /* Copy the record's fields at +10 and +18 onto actor 1. */
        ObjectMotion_ResetAndSetPosition_1(1, *(s16 *)(record + 10), *(s16 *)(record + 18));
    }
    ObjectMotion_CommitCurrentPositionAndActivate_1(1);
    ObjectMotion_SetHorizontalPositionWithTerrain_2(1, 0, 0);
    GameFlag_Set_1(0x303);
    BattleRuntime_ScheduleShoulderButtonModeUpdate_1(); /* main:0808a020 */
}

/* Runs the actor 0 / 1 / 9 setup sequence: position, speed and animation
 * calls in a fixed order. Two steps look up a record for one actor and
 * copy a coordinate pair out of it (offsets +8/+16 as s32, then +10/+18
 * as s16) into the call configuring another actor. */
void FieldScene_RunPrimaryActorSequence(void)
{
    extern u8 Data_03001ebc[];

    u32 i;
    s32 record;

    BattleRuntime_Reset_1_02002f14();
    ObjectMotion_PlaceWithinCameraBounds_1_02002f14(0x1650000, -1, 0x2e20000, 1);
    ObjectMotion_SetPositionAndReset_1_02002f14(0, 0x16f, 0x2e9);
    ObjectMotion_ArmCallback_1_02002f14(0, 0xa000, 0);
    record = Scene_GetRecord_1_02002f14(0);
    if (record != 0) {
        /* Copy the s32 coordinate pair at +8/+16 of the looked-up record. */
        ObjectMotion_SetHorizontalPositionWithTerrain_1_02002f14(1, *(s32 *)(record + 8), *(s32 *)(record + 16));
    }
    ObjectMotion_SetPositionAndReset_2_02002f14(1, 0x15a, 0x2e9);
    ObjectMotion_ArmCallback_2_02002f14(1, 0xd000, 20);
    SceneWork_SetStepValue_1_02002f14(0x1c53);
    BattleEvent_RunActionAndWait_1_02002f14(1, 0);
    ObjectMotion_SetVariantCallbackAndInvokeObject_1_02002f14(9, 2);
    BattleEffect_SpawnLinkedResourceObject_1_02002f14(9, 0x100, 0);
    ObjectMotion_ArmCallback_3_02002f14(9, 0x3000, 10);
    ObjectMotion_ArmCallback_4_02002f14(9, 0x5000, 10);
    ObjectMotion_ArmCallback_5_02002f14(9, 0x3000, 40);
    BattleRuntime_RunThenWaitIfModeZero_1_02002f14(9, 0, 20);
    Object_SetModeById_1_02002f14(0, 3);
    ObjectMotion_CallThenWaitForAnimationChange_1_02002f14(1, 3);
    ObjectMotion_SetVariantCallbackAndInvokeObject_2_02002f14(9, 1);
    ObjectMotion_ArmCallback_6_02002f14(9, 0x5000, 10);
    BattleRuntime_RunThenWaitIfModeZero_2_02002f14(9, 0, 20);
    ObjectMotion_SetVariantCallback_1_02002f14(1, 1);
    BattleEffect_SpawnLinkedResourceObject_2_02002f14(1, 0x103, 40);
    BattleRuntime_RunThenWaitIfModeZero_3_02002f14(1, 0, 10);
    ObjectMotion_CallThenWaitForAnimationChange_2_02002f14(9, 3);
    BattleRuntime_RunThenWaitIfModeZero_4_02002f14(9, 0, 20);
    ObjectMotion_ArmCallback_7(0, 0x8000, 0);
    ObjectMotion_ArmCallback_8(1, 0x1000, 40);
    ObjectMotion_CallThenWaitForAnimationChange_3_02002f14(9, 4);
    BattleEvent_RunActionAndWait_2_02002f14(9, 0);
    ObjectMotion_ArmCallback_9(0, 0xb000, 0);
    ObjectMotion_ArmCallback_10(1, 0xd000, 10);
    ObjectMotion_SetVariantCallbackAndInvokeObject_3_02002f14(1, 2);
    BattleRuntime_WaitIfModeZero_1_02002f14(20);
    BattleRuntime_RunThenWaitIfModeZero_5_02002f14(1, 0, 10);
    ObjectMotion_CallThenWaitForAnimationChange_4_02002f14(9, 3);
    BattleRuntime_RunThenWaitIfModeZero_6_02002f14(9, 0, 20);
    BattleEffect_SpawnLinkedResourceObject_3(0, 0x102, 0);
    BattleEffect_SpawnLinkedResourceObject_4(1, 0x102, 80);
    BattleRuntime_RunThenWaitIfModeZero_7_02002f14(1, 0, 20);
    ObjectMotion_SetVariantCallbackAndInvokeObject_4(9, 2);
    BattleRuntime_RunThenWaitIfModeZero_8_02002f14(9, 0, 20);
    ObjectMotion_ArmCallback_11(1, 0x1000, 20);
    ObjectMotion_SetSpeedParameters_1_02002f14(1, 0); /* main:0808a178 */
    if (UiWork_WaitThenFinalizeCapacity_1_02002f14(0, 0) == 0) {
        BattleEffect_SpawnLinkedResourceObject_5(1, 0x105, 60);
    } else {
        bump_step(1);
    }
    BattleRuntime_RunThenWaitIfModeZero_9_02002f14(1, 0, 20);
    ObjectMotion_ArmCallback_12(1, 0xd000, 10);
    SceneWork_SetStepValue_2_02002f14((s32)Data_00001c60);
    BattleEvent_RunActionAndWait_3_02002f14(1, 0);
    ObjectMotion_CallThenWaitForAnimationChange_5_02002f14(9, 3);
    BattleRuntime_RunThenWaitIfModeZero_10(9, 0, 20);
    ObjectMotion_CallThenWaitForAnimationChange_6_02002f14(1, 3);
    BattleRuntime_WaitIfModeZero_2_02002f14(20);
    ObjectMotion_ArmCallback_13(1, 0x1000, 20);
    ObjectMotion_CallThenWaitForAnimationChange_7_02002f14(1, 3);
    ObjectMotion_CallThenWaitForAnimationChange_8(0, 3);
    Object_SetModeById_2_02002f14(1, 2);
    record = Scene_GetRecord_2_02002f14(0);
    if (record != 0) {
        /* Copy the s16 coordinate pair at +10/+18 of the looked-up record. */
        ObjectMotion_ResetAndSetPosition_1_02002f14(1, *(s16 *)(record + 10), *(s16 *)(record + 18));
    }
    ObjectMotion_CommitCurrentPositionAndActivate_1_02002f14(1);
    ObjectMotion_SetHorizontalPositionWithTerrain_2_02002f14(1, 0, 0);
    GameFlag_Set_1_02002f14(0x304);
    BattleRuntime_ScheduleShoulderButtonModeUpdate_1_02002f14();
}

/* Sets up actors 12, 13, 14 and 20 with shared data and movement/speed
 * parameters, then drives actor 11 through a further sequence of moves. */
void FieldScene_RunCompanionActorSequence(void)
{
    extern u8 Data_03001ebc[];

    u32 i;
    s32 actor_data;
    s32 shared_data;

    BattleRuntime_Reset_1_020031b4();
    actor_data = Scene_GetRecord_1_020031b4(ACTOR_A);
    Func_02009110(actor_data, 0);
    actor_data = Scene_GetRecord_2_020031b4(ACTOR_B);
    Func_0200911c(actor_data, 0);
    actor_data = Scene_GetRecord_3(ACTOR_C);
    Func_02009128(actor_data, 0);
    Object_SetModeById_1_020031b4(ACTOR_A, 0);
    Object_SetModeById_2_020031b4(ACTOR_B, 0);
    Object_SetModeById_3_020031b4(ACTOR_C, 0);
    Func_02009086(20);
    Call3(Func_0200915e, 0x20000, 0x20000, 0x10000);
    shared_data = SHARED_DATA;
    ObjectMotion_EnableActionAndSetCallback_1(ACTOR_A, shared_data);
    Func_020090a6(10);
    ObjectMotion_EnableActionAndSetCallback_2(ACTOR_B, shared_data);
    Call3(Func_02009184, -1, -1, 0xe666);
    Func_020090c2(20);
    ObjectMotion_MarkActiveAndSetActionCallback_1(ACTOR_C, shared_data);
    BattleEffect_SpawnLinkedResourceObject_1_020031b4(ACTOR_D, 0x100, 40);
    ObjectMotion_SetVariantCallbackAndInvokeObject_1_020031b4(ACTOR_D, 2);
    ObjectMotion_ArmCallback_1_020031b4(ACTOR_D, 0xd000, 10);
    SceneWork_SetStepValue_1_020031b4(0x1c90);
    BattleRuntime_RunThenWaitIfModeZero_1_020031b4(ACTOR_D, 0, 40);
    ObjectMotion_SetAngleToward_1_020031b4(ACTOR_D, 0, 20);
    BattleEvent_RunActionAndWait_1_020031b4(ACTOR_D, 0);
    ObjectMotion_ArmCallback_2_020031b4(ACTOR_D, 0x8000, 10);
    GameFlag_Set_1_020031b4(0x305);
    BattleRuntime_ScheduleShoulderButtonModeUpdate_1_020031b4();
}

/*
 * The owner extends through its one literal pool word. The run length is
 * unsigned and the selector advances once per iteration. The Func_ aliases
 * name the call words encoded in the overlay image, not verified runtime
 * addresses.
 */
void SceneActor_ResetActorRun(s32 first, u32 count, s32 mode)
{
    s32 selector = first;
    u32 i;

    if (mode == 0) {
        for (i = 0; i < count; i++) {
            struct Resource373Actor *actor = Func_02009458(selector);

            actor->flag55 = 0;
            Func_020093d0(actor, 0);
            actor->field08 = 0x01860000;   /* 0xc3 << 17. */
            actor->field0c = 0x00a00000;   /* 0xa0 << 16. */
            actor->field10 = 0x034a0000;   /* The literal pool word. */
            selector++;
        }
        return;
    }

    for (i = 0; i < count; i++) {
        Func_020094ec(selector, 0, 0);
        selector++;
    }
}

void FieldScene_RunLargeStagingSequence(void)
{
    extern u8 Data_03001ebc[];

    u32 i;
    s32 rec;
    s32 rec3;
    u8 *rec8;
    u8 *record;
    s32 none;
    s32 none2;
    s32 base7_200e590;
    s32 base5_ee8;
    s32 p1;
    s32 k;
    s32 p2;

    rec3 = Value1_020034c8(Func_020094c0, 0);
    rec8 = Value1_020034c8(Func_020094c8, 14);
    Func_020094a6();
    Call4(Func_020095e0, -1, -1, -1, 0);
    Func_0200938e(1);
    rec = 0;
    {
        u8 *slot = (u8 *)(Func_020095fa() + 85);

        none = 0;
        *slot = rec;
    }
    Func_020093a2(1);
    Call6(Func_02009466, 49, 53, 8, 4, 20, 50);
    Call6(Func_02009472, 2, 102, 84, 41, 2, 1);
    Call6(Func_02009482, 1, 102, 83, 41, 1, 1);
    Call6(Func_02009492, 0, 103, 82, 42, 1, 1);
    rec = Func_02009548(11);
    *(u8 *)(rec + 85) = none;
    k = 0x1840000;
    *(s32 *)(rec + 12) = 0xa00000;
    *(s32 *)(rec + 16) = 0x3480000;
    *(s32 *)(rec + 8) = k;
    Func_020094d4(rec, 0);
    rec = Func_02009572(12);
    *(u8 *)(rec + 85) = none;
    *(s32 *)(rec + 12) = 0xa00000;
    *(s32 *)(rec + 16) = 0x34c0000;
    *(s32 *)(rec + 8) = k;
    Func_020094f6(rec, 0);
    rec = Func_02009594(13);
    *(u8 *)(rec + 85) = none;
    *(s32 *)(rec + 16) = 0x3500000;
    *(s32 *)(rec + 12) = 0xa00000;
    *(s32 *)(rec + 8) = k;
    Func_02009518(rec, 0);
    record = Func_020095b6(11);
    Func_02009524((s32)record, 0);
    record = Func_020095c2(12);
    Func_02009530((s32)record, 0);
    Func_02009630(0, 11);
    base7_200e590 = (s32)Data_0200e590;
    Func_020095ea(0, base7_200e590);
    Func_0200955e();
    base5_ee8 = (s32)Data_00000ee8;
    Func_0200958a(base5_ee8, 0, 0);
    Func_02009576();
    Call4(Func_020096fa, 0x1530000, 0xa00000, 0x4950000, 0);
    Func_0200952e();
    Func_020094ac(1);
    Call2(Func_02009704, 0x547a, 0xa8f);
    Call4(Func_0200971a, 0x1280000, 0xa00000, 0x3990000, 1);
    Call3(Func_0200967c, 5, 0x1990000, 0x46e0000);
    Func_020094d2(1);
    Call3(Func_0200963c, 5, 0xb333, 0x5999);
    Call3(Func_02009678, 5, 0x1a4, 0x42c);
    Func_02009794();
    *(s32 *)(*(u8 *volatile *)Data_03001ebc + 0x1c8) = 60;
    Func_02009778();
    Func_020096ae(5);
    Call3(Func_02009674, 5, 0x10000, 0x8000);
    Call3(Func_020096b8, 5, 0x155, 0x428);
    Call3(Func_0200968a, 5, 0xcccc, 0x6666);
    Call3(Func_020096cc, 5, 0x167, 0x409);
    Call3(Func_020096a2, 8, 0x8000, 0x4000);
    Call3(Func_020096de, 8, 0x13e, 0x3b3);
    Func_0200970e(8, 2);
    Call3(Func_020096fa_a, 5, 0x19c, 0x409);
    Call3(Func_02009706, 5, 0x19c, 0x3fb);
    Call3(Func_02009714, 5, 0x176, 0x3f0);
    Call3(Func_0200971e, 5, 0x15b, 0x3bb);
    Call3(Func_0200972a, 8, 0x13e, 0x3b3);
    Func_02009784(5, 8, 40);
    Func_0200977c(8, 2);
    Func_0200976c(5, 3);
    Func_020096da(10);
    Call3(Func_0200974c, 8, 0x17b, 0x3f9);
    Call2(Func_02009818, 0x8000, 0x1000);
    Call3(Func_0200976c_a, 5, 0x14d, 0x398);
    Call3(Func_02009778_a, 5, 0x12b, 0x39c);
    Func_02009844();
    Func_02009712(10);
    Call3(Func_02009816, 5, 0xf000, 30);
    Func_020097d6(5, 2);
    Func_0200972c(20);
    Call2(Func_02009860, 0x20000, 0x4000);
    Call4(Func_02009874, 0x1830000, 0xa00000, 0x3620000, 1);
    Func_02009882();
    Func_020097f4(10, 2, 20);
    Func_0200982a((base5_ee8 + 1));
    Call3(Func_0200984c, 0x100a, 0, 10);
    *(s32 *)(rec3 + 24) = 0x10000;
    *(s32 *)(rec3 + 28) = 0x10000;
    Func_020097b6(0, 1);
    Func_02009840(10, 0, 40);
    Call2(Func_020098a2, 0, 0x102);
    Func_02009842(0, 2);
    Func_02009798(40);
    Func_02009848(10, 2);
    Func_020097a6(40);
    Call3(Func_02009898, 0x100a, 0, 40);
    Func_02009848_a(0, 11);
    Func_02009800(0, base7_200e590);
    Func_020098e8(5, 1);
    Func_02009904();
    Func_020097d2(40);
    Func_02009872(5, 3);
    Call3(Func_020098de, 5, 0xd000, 10);
    Call3(Func_02009862, 5, 0x138, 0x2f7);
    Call3(Func_0200986e, 5, 0x169, 0x2f8);
    Func_02009804(20);
    Call3(Func_02009908, 5, 0x8000, 40);
    Func_02009912(5, 0, 40);
    Call3(Func_0200990c, 0x6001, 0, 10);
    Call3(Func_02009940, 5, 0x100, 0);
    Func_020098da(5, 4, 40);
    Call3(Func_0200993e, 5, 0xc000, 30);
    Func_020098fe(5, 2);
    Func_02009854(40);
    Func_02009946(5, 0, 30);
    Func_020098fe_a(5, 3);
    Func_0200986c(10);
    Call2(Func_020099a0, 0x40000, 0x8000);
    Call4(Func_020099ba, 0x18c0000, -1, 0x24c0000, 1);
    Call3(Func_02009904_a, 5, 0x1c8, 0x2e3);
    Func_020099d0();
    Func_0200989e(10);
    Call3(Func_020099b8, 1, 0x105, 40);
    Func_02009960(1, 2);
    Func_020098b6(20);
    Func_02008e9a();
    Func_02009952(1, 17);
    Call3(Func_020099b4, 0x2001, 0, 20);
    Func_02009a6a(131);
    for (i = 0; i < 60; i++) {
        Func_0200954a(Func_0200990a(1));
        Func_020097be(1);
    }
    Func_020099fa(1, 1);
    p1 = 0x200d5b1;
    Call2(Func_020097e0, p1, 0xc80);
    p2 = 0x200d5d1;
    Value2_020034c8(Func_020097ee, p2, 0xc80);
    record = Func_02009944(14);
    Func_020098b2((s32)record, 0);
    none2 = 0;
    rec8[85] = none2;
    *(s32 *)((s32)rec8 + 8) = 0x1ac0000;
    *(s32 *)((s32)rec8 + 12) = 0xd00000;
    *(s32 *)((s32)rec8 + 16) = 0x2480000;
    {
        s32 shown = 0x8000;

        *(s32 *)((s32)rec8 + 108) = 0x200d75d;
        *(u16 *)((s32)rec8 + 6) = shown;
    }
    Func_0200994c(4);
    Call3(Func_02009992, 14, 0x20000, 0x20000);
    Call4(Func_020098d2, (s32)rec8, 0x1980000, 0xd00000, 0x2480000);
    Func_02009970(40);
    Call3(Func_020099b2, 9, 0x2666, 0x1333);
    Call3(Func_020099bc, 14, 0x2666, 0x1333);
    Func_020099ba_a(9);
    Call4(Func_020099a6, (s32)rec8, 0x1880000, 0xd00000, 0x2480000);
    Call3(Func_02009aac, 9, 0x17a, 0x248);
    Func_02009a52(20);
    Call3(Func_02009b44, 0x2005, 0, 10);
    *(volatile s32 *)((s32)rec8 + 108) = none2;
    Func_02009b70(1, 2);
    {
        u8 *record = Func_02009a9e(1);
        u8 value = *(volatile u8 *)&record[35];

        record[35] = (u8)(value | 1);
    }
    Func_02009966(p1);
    Func_0200996c(p2);
    Func_02009962(1);
    Func_02009b5a(1, 0);
    Func_02009b62(9, 0);
    Call3(Func_02009b9e, 1, 0x3000, 0);
    Func_02009b3e(1, 1);
    Func_020092fc((s32)rec8);
    Func_020090a8();
    Func_02009abe(10);
    Call3(Func_02009b3c, 5, 0x1a8, 0x270);
    Func_02009b96(1, 5, 60);
    Func_02009b8e(1, 2);
    Func_02009ae4(10);
    Call3(Func_02009bd6, 0x6001, 0, 20);
    Func_02009b8e_a(5, 3);
    Func_02009afc(10);
    Func_02009bee(5, 0, 20);
    Call3(Func_02009c22, 1, 0x102, 40);
    Func_02009bb2(5, 4);
    Func_02009b20(10);
    Func_02009c12(5, 0, 20);
    Call3(Func_02009c44, 1, 0x101, 80);
    Func_02009bec(5, 1);
    Func_02009b42(10);
    Func_02009c34(5, 0, 20);
    Call3(Func_02009c68, 1, 0x102, 80);
    Func_02009bf8(5, 4);
    Func_02009b66(10);
    Func_02009c58(5, 0, 20);
    Func_02009c10(1, 3);
    Func_02009b7e(10);
    Call3(Func_02009bfc, 5, 0x184, 0x25c);
    Call3(Func_02009c90, 5, 0xa000, 20);
    Func_02009c50(5, 2);
    Func_02009ba6(10);
    Call3(Func_02009c98, 0x1005, 0, 20);
    Func_02009c50_a(5, 4);
    Func_02009bbe(10);
    Call3(Func_02009cd8, 1, 0x105, 40);
    Func_02009c80(1, 2);
    Func_02009bd6_a(10);
    Func_02009cd8_a(5, 0, 0);
    Func_02009ce2(1, 0x8000, 40);
    Call11(Func_02009d0e, 1, 1, 2, 25, 2, 5, 10, 14, 4, 14, none2);
    Func_02009c14(40);
    Call2(Func_02009d36, 5, 0x102);
    Call2(Func_02009d40, 1, 0x102);
    Func_02009c2e(80);
    Call3(Func_02009d48, 5, 0x101, 40);
    Call3(Func_02009d2a, 0x1005, 0, 20);
    Call2(Func_02009d64, 1, 0x102);
    Func_02009c52(80);
    Func_02009cf2(1, 4);
    Func_02009c60(10);
    Call3(Func_02009d7a, 5, 0x101, 40);
    Call3(Func_02009d5c, 0x1005, 0, 20);
    Func_02009d2c(1, 2);
    Func_02009c82(40);
    Func_02009d22(1, 4);
    Func_02009c90_a(20);
    Call3(Func_02009daa, 5, 0x101, 80);
    Func_02009d3a(5, 4);
    Func_02009ca8(10);
    Call3(Func_02009d9a, 0x1005, 0, 10);
    Call3(Func_02009db6, 5, 0x1000, 40);
    Func_02009d80(5, 1, 40);
    Func_02009d80_a(1, 2);
    Func_02009cd6(10);
    Call3(Func_02009d54, 5, 0x17c, 0x26c);
    Func_02009cea(10);
    Call3(Func_02009ddc, 0x1005, 0, 10);
    Call3(Func_02009df8, 1, 0x5000, 30);
    Call2(Func_02009e22, 1, 0x102);
    Func_02009d10(80);
    Func_02009dc8(1, 2);
    Func_02009d1e(10);
    Call3(Func_02009e10, 0x6001, 0, 20);
    Func_02009dc8_a(5, 4);
    Func_02009d36_a(10);
    Call3(Func_02009e50, 1, 0x101, 80);
    Func_02009de0(5, 4);
    Func_02009d4e(10);
    Call3(Func_02009d90, 1, 0xcccc, 0x6666);
    Call3(Func_02009dd6, 1, 0x19c, 0x25c);
    Call3(Func_02009e6a, 1, 0x5000, 20);
    Func_02009e2a(5, 1);
    Func_02009d80_b(10);
    Call3(Func_02009e72, 0x1005, 0, 20);
    Call3(Func_02009ea4, 1, 0x101, 80);
    Func_02009e3e(5, 4, 30);
    Call3(Func_02009e90, 0x1005, 0, 20);
    Func_02009e60(1, 3);
    Func_02009db6_a(10);
    Call3(Func_02009ea8, 0x6001, 0, 20);
    Func_02009dc6(30);
    Call3(Func_02009eca, 5, 0xe000, 40);
    Func_02009e72_a(5, 3);
    Call3(Func_02009ecc, 0x1005, 0, 20);
    Call3(Func_02009efe, 1, 0x101, 80);
    Call3(Func_02009f08, 5, 0x103, 40);
    Call3(Func_02009e3a, 5, 0xcccc, 0x6666);
    Call3(Func_02009e78, 5, 0x1ac, 0x274);
    Func_02009e16(20);
    Call3(Func_02009f1a, 1, 0x3000, 0);
    Func_02009ea8_a(5);
    Call2(Func_02009f10, 0x5001, 0);
    Func_02009ec8(5, 1);
    Func_02009e3e_a(60);
    Call3(Func_02009f42, 5, 0xb000, 30);
    Func_02009f28(1, 2);
    Call3(Func_02009ef6, 5, 0x1ac, 0x274);
    Call3(Func_02009f8a, 5, 0xb000, 20);
    Func_02009f4a(1, 3);
    Func_02009ea0(10);
    Func_02009f40(5, 4);
    Func_02009eae(10);
    Call3(Func_02009fa0, 0x2005, 0, 20);
    Call2(Func_02009fda, 1, 0x102);
    Func_02009ec8_a(40);
    Call3(Func_02009fba, 0x5001, 0, 20);
    Func_02009f6a(1, 3);
    Func_02009f7a(5, 3);
    Func_02009ee8(10);
    Call3(Func_02009f2c, 5, 0x8000, 0x4000);
    Call3(Func_02009f38, 1, 0x8000, 0x4000);
    Call3(Func_02009f74, 5, 0x1c2, 0x2ee);
    Call3(Func_02009f80, 1, 0x1c2, 0x2ee);
    Func_02009f1e(60);
    *(s32 *)((*(u8 *volatile *)Data_03001ebc + 0x1c8)) = 60;
    Func_0200a088();
    Func_0200a094();
    Func_0200a082(12);
    Func_02009f4e();
}

void FieldScene_RunStagedActorCutsceneSequence(void)
{
    u8 *scene;
    u8 *rec;
    u32 i;
    s32 turn_back;
    s32 turn_side;
    u8 *turned;
    s32 none;
    s32 flag;
    s32 callback_a;
    s32 callback_b;
    s32 callback_c;
    s32 callback_d;
    s32 callback_e;
    s32 callback_f;

    scene = Func_02009fa8(0);
    Func_02009f86();
    Call4(Func_0200a0c0, -1, -1, -1, 0);
    Func_02009e6e(1);
    Call6_02003fb0(Func_02009f32, 49, 53, 8, 4, 20, 50);
    Func_02009f3e(2, 102, 84, 41, 2, 1);
    Func_02009f4e_a(1, 102, 83, 41, 1, 1);
    Func_02009f5e(0, 103, 82, 42, 1, 1);
    Call3(Func_0200a074, 21, 0x1880000, 0x3800000);
    turned = Func_0200a022(21);
    /*
     * Overwritten at once, but the store must stay: its zero halfword
     * temporary is what the record byte stores below reuse out of a high
     * register.
     */
    *(u16 *)(turned + 6) = 0;
    turn_back = 0xc000;
    *(u16 *)(turned + 6) = turn_back;
    Call3(Func_0200a096, 1, 0x12a0000, 0x2e00000);
    turned = Func_0200a044(1);
    turn_side = 0x4000;
    *(u16 *)(turned + 6) = turn_side;
    Call3(Func_0200a0b4, 5, 0x12a0000, 0x2f80000);
    turned = Func_0200a062(5);
    *(u16 *)(turned + 6) = turn_side;
    Func_0200a0d6(0, 11);
    Func_0200a08e(0, 0x200e590);
    rec = Func_0200a084(23);
    rec[85] = 0;
    *(s32 *)(rec + 8) = 0x1840000;
    *(s32 *)(rec + 12) = 0xa00000;
    *(s32 *)(rec + 16) = 0x3480000;
    Func_0200a00e(rec, 0);
    rec = Func_0200a0ac(24);
    rec[85] = 0;
    *(s32 *)(rec + 8) = 0x1840000;
    *(s32 *)(rec + 12) = 0xa00000;
    *(s32 *)(rec + 16) = 0x34c0000;
    Func_0200a02e(rec, 0);
    rec = Func_0200a0cc(25);
    rec[85] = 0;
    *(s32 *)(rec + 8) = 0x1840000;
    *(s32 *)(rec + 12) = 0xa00000;
    *(s32 *)(rec + 16) = 0x3500000;
    Func_0200a04e(rec, 0);
    Func_0200a1fa()[85] = 0;
    Func_02009f9e(1);
    Call4(Func_0200a202, 0x17f0000, 0xa00000, 0x36d0000, 0);
    Func_0200a036();
    Func_02009fb4(1);
    *(s32 *)(*(u8 **)0x03001ebc + 0x1c8) = 32;
    Func_0200a23e();
    Call3(Func_0200a132, 5, 0x8000, turn_side);
    Call3(Func_0200a13e, 1, 0x8000, turn_side);
    Func_0200a14e(5, 0x200e614);
    Func_0200a156(1, 0x200e5cc);
    Func_0200a11c(40);
    Func_0200a164(0, 1);
    *(s32 *)(scene + 24) = 0x10000;
    *(s32 *)(scene + 28) = 0x10000;
    Call4(Func_0200a232, 0, 0xb000, 40, 0x10000);
    Func_0200a1da(0, 3);
    Func_0200a148(10);
    Call3(Func_0200a18a, 0, 0x4ccc, 0x2666);
    Call3(Func_0200a1d0, 0, 400, 840);
    Func_0200a166(10);
    Func_0200a268(0, turn_back, 30);
    Func_0200a228(0, 1);
    Func_0200a17e(20);
    Call3(Func_0200a282, 0, 0x8000, 40);
    Func_0200976e();
    Func_0200a226(0, 17);
    Func_0200a080((s32)Func_0200da09, 3200);
    for (i = 0; i < 40; i++) {
        Func_02009e14(scene);
        Func_0200a088_a(1);
    }
    Func_0200a2c4(0, 1);
    /* Callback symbols are pooled loads that stay after the preceding call. */
    callback_a = (s32)Func_0200d5c1;
    Value2_02003fb0(Func_0200a0a8, callback_a, 3200);
    callback_b = (s32)Func_0200d5e1;
    Call2(Func_0200a0b4_a, callback_b, 3200);
    Call3(Func_0200a216, 23, 0x3333, 0x1999);
    Call3(Func_0200a24c, 23, 390, 832);
    Call3(Func_0200a2f0, 0, 0xc000, 0);
    Call3(Func_0200a264, 23, 400, 826);
    Func_0200a20a(20);
    {
        u8 *record = Func_0200a240(0);
        u8 value = *(volatile u8 *)&record[35]; /* Keeps the byte in its own register. */

        record[35] = (u8)(value | 1);
    }
    Func_0200a2b2(0, 1);
    Func_0200a114((s32)Func_0200da09);
    Func_0200a11a(callback_a);
    Func_0200a120(callback_b);
    Func_0200a116(1);
    Func_0200a30e(0, 0);
    Func_0200a316(23, 0);
    Func_0200a2e0(23, 0, 0);
    Func_0200a25e(20);
    Func_0200a2f6(0, 11);
    Func_0200a2ae(0, 0x200e590);
    Func_0200a274(120);
    Call6_02003fb0(Func_0200a208, 7, 102, 84, 41, 2, 1);
    Func_0200a2d0(0, 1);
    *(s32 *)(scene + 24) = 0x10000;
    *(s32 *)(scene + 28) = 0x10000;
    Func_0200a332(0, 1);
    Func_0200a2a8(40);
    Func_0200a348(0, 3);
    Call3(Func_0200a32a, 0, 377, 843);
    Call3(Func_0200a3be, 0, 0xc000, 40);
    Func_0200a3c8(0, 0, 20);
    Func_0200a368(0, 17);
    Call2(Func_0200a1c2, (s32)Func_0200da09, 3200);
    for (i = 0; i < 40; i++) {
        Func_02009f56(scene);
        Func_0200a1ca(1);
    }
    Func_0200a406(0, 1);
    callback_c = (s32)Func_0200d5c1;
    Value2_02003fb0(Func_0200a1ea, callback_c, 3200);
    callback_d = (s32)Func_0200d5f1;
    Call2(Func_0200a1f6, callback_d, 3200);
    Call3(Func_0200a358, 24, 0x3333, 0x1999);
    Call3(Func_0200a38e, 24, 390, 832);
    Call3(Func_0200a432, 0, 0xc000, 0);
    Call3(Func_0200a3a6, 24, 377, 828);
    Func_0200a34c(20);
    {
        u8 *record = Func_0200a382(0);
        u8 value = *(volatile u8 *)&record[35];

        record[35] = (u8)(value | 1);
    }
    Func_0200a3f4(0, 1);
    Func_0200a256((s32)Func_0200da09);
    Func_0200a25c(callback_c);
    Func_0200a262(callback_d);
    Func_0200a258(1);
    Func_0200a450(0, 0);
    Func_0200a458(24, 0);
    Func_0200a422(24, 0, 0);
    Func_0200a3a0(20);
    Func_0200a438(0, 11);
    Func_0200a3f0(0, 0x200e590);
    Func_0200a3b6(120);
    Call6_02003fb0(Func_0200a348_a, 6, 102, 83, 41, 1, 1);
    Func_0200a410(0, 1);
    *(s32 *)(scene + 24) = 0x10000;
    *(s32 *)(scene + 28) = 0x10000;
    Func_0200a472(0, 1);
    Func_0200a3e8(40);
    Func_0200a488(0, 3);
    Call3(Func_0200a46c, 0, 360, 855);
    Call3(Func_0200a500, 21, 0xb000, 10);
    Call3(Func_0200a50c, 0, 0xc000, 30);
    Call3(Func_0200a518, 0, 0xd000, 20);
    Func_0200a4b8(0, 17);
    Call2(Func_0200a312, (s32)Func_0200da09, 3200);
    for (i = 0; i < 40; i++) {
        Func_0200a0f0(scene);
        Func_0200a364(1);
    }
    Func_0200a5a0(0, 1);
    callback_e = (s32)Func_0200d5c1;
    Value2_02003fb0(Func_0200a384, callback_e, 3200);
    callback_f = (s32)Func_0200d601;
    Value2_02003fb0(Func_0200a390, callback_f, 3200);
    Call3(Func_0200a4f2, 25, 0x3333, 0x1999);
    Call3(Func_0200a528, 25, 390, 832);
    Call3(Func_0200a5cc, 0, 0xc000, 0);
    Call3(Func_0200a540, 25, 360, 837);
    Func_0200a4e6(20);
    {
        u8 *record = Func_0200a51c(0);
        u8 value = *(volatile u8 *)&record[35];

        record[35] = (u8)(value | 1);
    }
    Func_0200a58e(0, 1);
    Func_0200a3ec((s32)Func_0200da09);
    Func_0200a3f2(callback_e);
    Func_0200a3f8(callback_f);
    Func_0200a3ee(1);
    Func_0200a5e6(0, 0);
    Func_0200a5ee(25, 0);
    Func_0200a5b8(25, 0, 0);
    Func_0200a536(20);
    Func_0200a5ce(0, 11);
    Func_0200a586(0, 0x200e590);
    Func_0200a54c(120);
    Func_02009b40();
    Call6_02003fb0(Func_0200a4e2, 5, 103, 82, 42, 1, 1);
    Func_0200a5aa(0, 1);
    *(s32 *)(scene + 24) = 0x10000;
    *(s32 *)(scene + 28) = 0x10000;
    Func_0200a61e(21, 2, 20);
    Call1_02003fb0(Func_0200a654, 0xf03);
    Func_0200a676(21, 0, 10);
    Call3(Func_0200a692, 0, 0x1000, 10);
    Func_020078a2(21, 5, 6, 0);
    Call3(Func_0200a5e8, 21, 0x4ccc, 0x2666);
    Call3(Func_0200a62c, 21, 397, 832);
    Func_0200a5c2(20);
    Call3(Func_0200a6c6, 21, 0x4000, 60);
    Call3(Func_0200a6d2, 21, 0xc000, 60);
    Func_0200a67a(21, 3);
    Func_0200a682(21, 3);
    Func_0200a6dc(21, 0, 10);
    Call3(Func_0200a672, 21, 372, 832);
    Func_0200a608(20);
    Call3(Func_0200a70c, 21, 0x4000, 40);
    Call3(Func_0200a718, 21, 0x8000, 40);
    Func_0200a6c0(21, 3);
    Func_0200a6c8(21, 3);
    Func_0200a722(21, 0, 20);
    Call3(Func_0200a73e, 21, 0x5000, 20);
    Func_0200a6e6(21, 3);
    Func_0200a740(21, 0, 10);
    Func_0200a710(0, 2);
    Func_0200a666(20);
    Func_0200a706(21, 4);
    Value2_02003fb0(Func_0200a74e, 21, 0);
    if (Func_0200a6a6(0, 0) == 0) {
        Func_0200a722_a(21, 3);
        bump_step(1);
    } else {
        Func_0200a76e(21, 4);
    }
    Func_0200a7c8(21, 0, 20);
    Call1_02003fb0(Func_0200a7b6, 0xf0a);
    Call3(Func_0200a762, 21, 386, 841);
    Func_0200a6f8(10);
    Call3(Func_0200a7fc, 21, 0xd000, 60);
    Func_0200a7bc(21, 2);
    Func_0200a7fe(21, 0, 20);
    Call3(Func_0200a81a, 21, 0x5000, 30);
    Value2_02003fb0(Func_0200a802, 21, 0);
    if (Func_0200a75a(0, 0) == 1) {
        bump_step(1);
    }
    Func_0200a838(21, 0, 20);
    Call3(Func_0200a854, 21, 0xd000, 60);
    Func_0200a814(21, 2);
    Call1_02003fb0(Func_0200a83a, 0xf0e);
    Func_0200a85c(21, 0, 20);
    Call3(Func_0200a7f0, 21, 386, 825);
    Func_0200a786(10);
    Func_0200a826(21, 4);
    Func_0200a794(60);
    Func_0200a886(21, 0, 60);
    Call3(Func_0200a8a2, 21, 0x5000, 10);
    Func_0200a89c(21, 0, 10);
    Call3(Func_0200a832, 21, 372, 832);
    Call3(Func_0200a8c6, 21, 0x5000, 10);
    Func_0200a886_a(0, 2);
    Func_0200a7dc(20);
    Func_0200a87c(21, 3);
    Func_0200a7ea(20);
    Func_0200a8dc(21, 0, 20);
    Call3(Func_0200a910, 0, 258, 60);
    Func_0200a8b8(21, 2);
    Func_0200a8fa(21, 0, 20);
    Func_0200a8b2(0, 3);
    Func_0200a8ba(21, 3);
    Func_0200a914(21, 0, 10);
    Call2(Func_0200a95c, 0x6666, 0xccc);
    Call4(Func_0200a974, 0x1790000, 0xa00000, 0x35c0000, 1);
    Call3(Func_0200a88a, 5, 0x10000, 0x8000);
    Call3(Func_0200a898, 1, 0x10000, 0x8000);
    Call3(Func_0200a8d4, 1, 369, 904);
    Call3(Func_0200a8ea, 5, 392, 904);
    Func_0200a912(1, 1);
    Func_02007b8a(5, 10, 11, 0);
    Call3(Func_0200a992, 5, 0xa000, 0);
    Func_0200a98c(5, 0, 10);
    Func_0200a95c_a(21, 2);
    Func_0200a8b2_a(10);
    Call3(Func_0200a9b6, 21, 0x3000, 0);
    Call3(Func_0200a9c2, 0, 0x1000, 20);
    Func_0200a974_a(5, 4, 0);
    Call3(Func_0200a950, 5, 392, 843);
    Call3(Func_0200a9e4, 5, 0x9000, 0);
    Call3(Func_0200a9f0, 21, 0x3000, 0);
    Call3(Func_0200a9fc, 0, 0xd000, 20);
    Call2((void (*)())Func_0200a9a4, 21, 3);
    Func_0200a9fe(21, 0, 10);
    Func_0200a9b6_a(5, 3);
    Func_0200aa10(5, 0, 10);
    Func_0200a9c0(21, 3);
    Func_0200a9d0(0, 3);
    Func_0200aa2a(21, 0, 20);
    Func_02007c4a(1, 10, 11, 0);
    Call3(Func_0200a990, 5, 0x4ccc, 0x2666);
    Call3(Func_0200a99a, 1, 0x4ccc, 0x2666);
    Call3(Func_0200a9d6, 1, 392, 843);
    Func_0200a9a4_a(5)[90] &= 0xfe;
    Call3(Func_0200a9fc_a, 5, 408, 843);
    Func_0200a992_a(1);
    {
        u8 *record = Func_0200a9c8(5);
        u8 value = *(volatile u8 *)&record[90];

        record[90] = (u8)(value | 1);
    }
    Call3(Func_0200aaa6, 5, 0x8000, 0);
    Func_0200aa34(1);
    Func_0200aa4c(1, 1);
    Call3(Func_0200aac0, 1, 0x8000, 30);
    Func_0200aa72(21, 4, 30);
    Func_0200aac4(21, 0, 20);
    Func_0200aa7c(1, 3);
    Func_0200a9ea(20);
    Call3(Func_0200aaee, 21, 0x5000, 20);
    Func_0200aae8(21, 0, 10);
    Call3(Func_0200ab04, 0, 0xd000, 0);
    Func_0200aab6(0, 2, 30);
    Call2(Func_0200ab38, 0, 258);
    Func_0200aa26(60);
    Call3(Func_0200ab2a, 21, 0x3000, 40);
    Func_0200aaea(1, 2);
    Func_0200ab2c(1, 0, 20);
    Call3(Func_0200ab5e, 21, 257, 80);
    Call3(Func_0200ab52, 21, 0x5000, 30);
    Call3(Func_0200ab76, 0, 258, 60);
    Call3(Func_0200ab6a, 21, 0x3000, 0);
    Call3(Func_0200ab76_a, 0, 0xd000, 20);
    Func_0200ab70(21, 0, 10);
    Func_0200ab28(1, 3);
    Func_0200aa96(80);
    Func_0200ab60(5, 1, 30);
    Func_0200ab50(1, 2);
    Func_0200ab60_a(5, 2);
    Func_0200aab6_a(10);
    Call3(Func_0200abd0, 21, 261, 60);
    Func_0200abb2(21, 0, 10);
    Call3(Func_0200ac0a, 5, 0x8000, 0);
    Call3(Func_0200ac16, 1, 0x8000, 10);
    Func_0200abd6(5, 2);
    Func_0200ab2c_a(20);
    Func_0200ac1e(5, 0, 10);
    Func_0200abe6(21, 2);
    Func_0200ab44(40);
    Func_0200ac36(21, 0, 10);
    Func_0200abe6_a(1, 3);
    Func_0200abf6(5, 3);
    Func_0200ab64(20);
    Func_0200ac04(21, 4);
    Func_0200ac5e(21, 0, 10);
    Func_0200ac0e(1, 3);
    Func_0200ac1e_a(5, 3);
    Func_0200ac26(21, 3);
    Func_0200ac80(21, 0, 10);
    Func_0200ac30(1, 3);
    Func_0200ac40(5, 3);
    Func_0200abae(20);
    Func_0200ac66(21, 2);
    Func_0200aca8(21, 0, 20);
    Call2(Func_0200acf0, 0x9999, 0x1333);
    Call4(Func_0200ad06, 0x1750000, 0xa00000, 0x3450000, 1);
    Call3(Func_0200ac54, 21, 364, 816);
    Call3(Func_0200ace8, 0, 0xd000, 0);
    Call3(Func_0200acf4, 21, 0x3000, 10);
    Func_0200acee(21, 0, 40);
    Func_0200acd0(5, 1, 30);
    Func_0200acc0(1, 2);
    Func_0200acd0_a(5, 2);
    Func_0200ac26_a(20);
    Call3(Func_0200ad2a, 1, 0x8000, 0);
    Call3((void (*)())Func_0200ad36, 5, 0x8000, 20);
    Call3(Func_0200ad42, 21, 0x5000, 20);
    Func_0200ad3c(21, 0, 10);
    Call3(Func_0200ad6e, 0, 261, 60);
    Func_0200acfe(21, 4);
    Value2_02003fb0(Func_0200ad46, 21, 0);
    none = 0; /* One zero shared by the placement call and the byte store. */
    if (Func_0200aca0(0, 0) == 1) {
        bump_step(1);
    }
    Func_0200ad7e(21, 0, 20);
    Call1_02003fb0(Func_0200ad6c, 0xf27);
    Call3(Func_0200adb6, 21, 259, 0);
    Func_0200ad5e(21, 3);
    Func_0200ada0(21, 0, 10);
    Func_0200ad62(21, 4, 0);
    Func_0200ad7a(21, 3);
    Func_0200ad62_a(21, 7);
    Func_0200acd8(5);
    Call11(Func_0200adfa, 21, 14, 2, 24, 2, 1, 10, 14, 4, 14, none);
    Func_0200ae98(161);
    rec = Func_0200ad36_a(21);
    {
        u8 value = *(volatile u8 *)&rec[90];

        *(u8 *)(*(s32 *)(rec + 80) + 38) = none;
        rec[90] = (u8)(value & 0xfe);
    }
    Call3(Func_0200ad60, 21, 0x30000, 0x18000);
    Call3(Func_0200ada4, 21, 364, 815);
    Func_0200ad3a(4);
    for (i = 0; i != 4; i++) {
        *(s32 *)(rec + 16) += 0x18000;
        *(s32 *)(rec + 28) += -0x1999;
        Func_0200ad56(1);
    }
    Func_0200adec(21, 0, 0);
    Call3(Func_0200adaa, 1, 0x30000, 0x18000);
    Func_0200ae1c(1, 6, 0);
    Call3(Func_0200adf8, 1, 374, 827);
    Func_0200ae70(5, 0);
    Call3(Func_0200ae94, 1, 0xb000, 0);
    Call3(Func_0200aeb8, 5, 256, 0);
    Func_0200ae58(5, 2);
    Call3(Func_0200aecc, 1, 256, 10);
    Func_0200ae54(1, 13);
    Func_0200ae6e(1, 2, 5);
    Func_0200af6c(143);
    Call3(Func_0200ad82, 0, 0x40000, 0x10000);
    Call6_02003fb0(Func_0200ad74, 1, 102, 83, 41, 1, 1);
    Func_0200ad98(Func_0200ae2a(1), 0);
    Call3(Func_0200af04, 0, 0xd000, 10);
    Func_0200aebc(1, 3);
    Call3(Func_0200adc2, -1, -1, 0xe666);
    Func_0200adce();
    Call3(Func_0200af42, 1, 258, 80);
    Func_0200aeca(21, 8);
    *(s32 *)(rec + 28) = 0x8000;
    Call3(Func_0200aed4, 21, 0x16c0000, 0x32b0000);
    for (i = 0; i != 5; i++) {
        *(s32 *)(rec + 28) += 0x1999;
        Func_0200ae5e(1);
    }
    none = 0; /* Refreshed after the loops for the closing scene store. */
    Func_0200ae68(60);
    Func_0200af20(1, 2);
    Call3(Func_0200af74, 1, 0x5000, 30);
    Func_0200af2c(1, 2);
    Func_0200af3c(5, 2);
    Func_0200ae92(60);
    Func_0200af4a(21, 2);
    Func_0200aea0(20);
    Func_0200af92(21, 0, 20);
    Call2(Func_0200afda, 0x4ccc, 0x999);
    Call4(Func_0200aff2, 0x1740000, 0xa00000, 0x35b0000, 1);
    Call3(Func_0200af08, 21, 0x30000, 0x18000);
    Func_0200af7a(21, 6, 0);
    Call3(Func_0200af54, 21, 359, 835);
    Func_0200aeea(20);
    Call3(Func_0200afee, 21, 0x4000, 20);
    Func_0200afae(21, 2);
    rec[35] &= 0xfe;
    Func_0200affe(21, 0, 80);
    Call3(Func_0200b030, 21, 257, 80);
    Func_0200b022(21, 0, 60);
    Func_0200afe2(21, 3);
    Func_0200b024(21, 0, 10);
    Call2(Func_0200b05e, 21, 258);
    Func_0200af4c(80);
    Call3(Func_0200b050, 1, 0x5000, 40);
    Call3(Func_0200b074, 1, 258, 80);
    Func_0200b01c(1, 2);
    Func_0200b05e_a(1, 0, 20);
    Call3(Func_0200b07a, 1, 0x8000, 20);
    Func_0200b03a(1, 3);
    Func_0200af90(10);
    Func_0200b048(1, 3);
    Func_0200af3c_a(Func_0200afce(1), 1);
    Func_0200b0a2(1, 6, 0);
    Func_0200b09a(1, 1);
    Call3(Func_0200b050_a, 1, 0x40000, 0x20000);
    rec = Func_0200b04e(1);
    rec[90] &= 0xfe;
    Call3(Func_0200b08c, 1, 403, 827);
    Call2(Func_0200b156, 5, 258);
    Call3(Func_0200b142, 5, 0xc000, 20);
    Func_0200b13c(5, 0, 1);
    Func_0200b0da(1);
    Call3(Func_0200b15e, 1, 0x5000, 20);
    Func_0200b158(1, 0, 20);
    Call3(Func_0200b18c, 1, 256, 0);
    Func_0200b114(1, 13);
    Func_0200b12e(1, 2, 5);
    Func_0200b032(Func_0200b0c4(1), 0);
    Call6_02003fb0(Func_0200b02e, 2, 102, 84, 41, 2, 1);
    Func_0200b24c(143);
    Call3(Func_0200b062, 0, 0x40000, 0x10000);
    Func_0200b17a(1, 3);
    Call3(Func_0200b078, -1, -1, 0xe666);
    Func_0200b084();
    Call3(Func_0200b1f8, 1, 258, 30);
    Call3(Func_0200b12a, 5, 0x4ccc, 0x2666);
    Call3(Func_0200b16e, 5, 408, 855);
    Func_0200b104(60);
    Func_0200b1bc(1, 2);
    Call3(Func_0200b226_a, 21, 261, 60);
    Func_0200b1c6(5, 3);
    Func_0200b1d6(0, 3);
    Func_0200b12c(80);
    Call3(Func_0200b230, 1, 0x4000, 30);
    Func_0200b1f0(1, 3);
    Func_0200b146(10);
    Func_0200b1e6(5, 4);
    Func_0200b154(80);
    Func_0200b1f4(21, 3);
    Func_0200b162(10);
    Func_0200b254(21, 0, 10);
    Call3(Func_0200b270, 5, 0xb000, 0);
    Call3(Func_0200b27c, 0, 0xc000, 0);
    Func_0200b286(21, 0, 60);
    Call3(Func_0200b292_a, 21, 0x4000, 60);
    Func_0200b23a(21, 4);
    Func_0200b1a8(60);
    Func_0200b2aa(21, 0, 80);
    Call3(Func_0200b2cc, 21, 261, 80);
    Func_0200b2ae(21, 0, 60);
    Call3(Func_0200b2ca, 0, 0xc000, 0);
    Call3(Func_0200b2ec, 0, 257, 0);
    Call3(Func_0200b2f6, 5, 257, 0);
    Call3(Func_0200b300, 1, 257, 60);
    Func_0200b290(21, 4);
    Func_0200b1fe(30);
    Func_0200b2f0(21, 0, 60);
    Func_0200b2b8(1, 2);
    Func_0200b2c8(5, 2);
    Func_0200b21e(20);
    Call3(Func_0200b322, 5, 0x8000, 60);
    Call3(Func_0200b32e, 21, 0x4000, 30);
    Func_0200b328(21, 0, 30);
    Func_0200b2e0(5, 3);
    Func_0200b24e(10);
    Func_0200b340(5, 0, 20);
    Func_0200b35a(21, 0, 30);
    Func_0200b302(21, 4);
    Func_0200b270_a(20);
    Func_0200b362(21, 0, 20);
    Func_0200b31a(5, 3);
    Func_0200b288(20);
    Func_0200b340_a(1, 3);
    Func_0200b296_a(10);
    Func_0200b388(1, 0, 20);
    Call3(Func_0200b3a4, 0, 0xd000, 0);
    Call3(Func_0200b3c8, 21, 256, 0);
    Func_0200b370(21, 3);
    Func_0200b2c6(30);
    Func_0200b3b8(21, 0, 60);
    Func_0200b388_a(1, 3);
    Call3(Func_0200b31e, 1, 0x10000, 0x8000);
    Func_0200b28a(Func_0200b31c(1), 0);
    Func_0200b39c(1, 4, 0);
    Call3((void (*)())Func_0200b37a, 1, 398, 828);
    Func_0200b310(60);
    Call3(Func_0200b414, 21, 0x4000, 0);
    Call3(Func_0200b420, 0, 0xc000, 60);
    Func_0200b3c8_a(0, 3);
    Func_0200b336(60);
    Func_0200b3d6(21, 3);
    Func_0200b344(60);
    rec = Func_0200b37a_a(1);
    flag = 1; /* One shared mark bit for the three record flags. */
    rec[90] |= flag;
    rec = Func_0200b38e(5);
    rec[90] |= flag;
    rec = Func_0200b3a0(0);
    Call3(Func_0200b3b8_a, 1, 0x10000, 0x8000);
    Call3(Func_0200b3c6, 5, 0x10000, 0x8000);
    Func_0200b490(0, 0, 0);
    Func_0200b410(5, *(s16 *)(rec + 10) + 16, *(s16 *)(rec + 18));
    Func_0200b42a(1, *(s16 *)(rec + 10) + 16, *(s16 *)(rec + 18) - 16);
    Func_0200b440(1);
    Call3(Func_0200b4c4, 1, 0x5000, 30);
    Func_0200b464(1, 3);
    Func_0200b46c(5, 3);
    Func_0200b47c(0, 3);
    Func_0200b3ea(40);
    Func_0200b468(5, *(s16 *)(rec + 10), *(s16 *)(rec + 18));
    Func_0200b48a(5, 0, 0);
    Func_0200b480(1, *(s16 *)(rec + 10), *(s16 *)(rec + 18));
    Func_0200b4a2(1, 0, 0);
    Func_0200b43a(1, 5);
    Call4(Func_0200b584, 0x1790000, 0xa00000, 0x3770000, 1);
    Func_0200882c(0, 13, 10, 0);
    Call3(Func_0200b4de, 0, 376, 912);
    Call3(Func_0200b572, 0, 0xc000, 0);
    {
        u8 *record = Func_0200b4b0(21);
        u8 value = *(volatile u8 *)&record[90];

        record[90] = (u8)(value | flag);
    }
    Call4(Func_02008860, 21, 6, 5, 0);
    Call3(Func_0200b50e, 21, 373, 887);
    Call3(Func_0200b5a2, 21, 0x4000, 0);
    Call3(Func_0200b5ae, 0, 0xc000, 40);
    Func_0200b54e(21, 3);
    Func_0200b55e(0, 3);
    Func_0200b4cc(20);
    Func_0200b5f4(0, 1);
    Func_0200b610();
    Func_0200b4de_a(100);
    Call6_02003fb0(Func_0200b47a, 49, 46, 8, 4, 20, 50);
    Call1_02003fb0(Func_0200b4e8, 514);
    Call1_02003fb0(Func_0200b4f6, 303);
    scene[85] = 3;
    *(s32 *)(scene + 12) = 0xa00000;
    *(s32 *)(scene + 60) = 0x80000000;
    *(s32 *)(scene + 40) = none;
    Func_0200b52c();
}

void FieldScene_RunStep8C(void)
{
    extern u8 Data_03001ebc[];

    Func_0200b6c0(0x8c, 0);
}

void FieldScene_RunSingleStep(void)
{
    Func_0200b6d4();
}

void SceneState_SetValue1ThenCall(void)
{
    Func_0200b59a(1);
    Func_0200b216();
}

void SceneState_SetValue0ThenCall(void)
{
    Func_0200b5aa(0);
    Func_0200b226();
}

void FieldScene_RunStep9(void)
{
    extern u8 Data_03001ebc[];

    Func_0200b5ba(9);
    Func_0200b272();
}

void FieldScene_RunStep17(void)
{
    extern u8 Data_03001ebc[];

    Func_0200b5ca(0x17);
    Func_0200b282();
}

void SceneState_SetValue24ThenCall(void)
{
    Func_0200b5da(0x18);
    Func_0200b292();
}

void SceneState_SetValue25ThenCall(void)
{
    Func_0200b5ea(0x19);
    Func_0200b2a2();
}

s32 Runtime_ComputeFixedPointDistance(s32 *first_position, s32 *second_position)
{
    typedef s32(*FixedPointSqrt)(s32);

    s32 delta_x = (*first_position++ - *second_position++) >> 16;
    s32 delta_y = (*first_position++ - *second_position++) >> 16;
    s32 delta_z = (*first_position - *second_position) >> 16;
    s32 delta_x_squared = delta_x *delta_x;
    s32 delta_y_squared = delta_y *delta_y;
    s32 delta_z_squared = delta_z *delta_z;

    return ((FixedPointSqrt) 0x030001D8)(delta_x_squared + delta_y_squared + delta_z_squared);
}

s32 SceneActor_RunStep18WhenTargetSet(s32 *p)
{
    s32 t = Func_0200b718(0);
    if (p[14] == (s32)0x80000000 && p[16] == (s32)0x80000000)
        return 0;
    Func_0200ad9e((s32)p, t, 18, 0);
    return 0;
}

void Effect_ConfigureSpawnedParticle(struct SourceEntity *source)
{
    s32 spawn_position[3];
    s32 particle_index;
    struct StagedParticle *particle;
    spawn_position[0] = source->f08;
    spawn_position[1] = source->f0c - (Func_0200b60e(source) << 4) + (s32)0xfff80000;
    spawn_position[2] = source->f10;
    particle_index = Func_0200b622();
    Func_0200b656(((particle_index << 1) + particle_index) << 4, Func_0200b628(), spawn_position);
    particle = Func_0200b6a2(0x11d, spawn_position[0], spawn_position[1], spawn_position[2]);
    if (particle != 0) {
        particle->f55 = 2;
        particle->f48 = 0x1999;
        particle->f5e = 12;
        Func_0200b708(particle, 0);
        Func_0200b6b8(particle, 0);
        Func_0200b6c8(particle, (s32)Data_0200e6e0);
        {
            struct ParticleRecord *record = particle->f50;
            s32 record_flags = ~12;
            record_flags &= record->f09;
            record_flags |= 4;
            record->f09 = record_flags;
        }
    }
    Func_0200b92e(0x8a);
}

/*
 * Play a rising-dust burst from a source entity. The owner extends through its
 * four literal pool words. Both loops count down inclusively, so they run 31
 * lift steps and 8 particles. The address handed to Func_0200b760 is passed
 * through as a plain pointer; whether it is animation data or a callback is
 * not established.
 */
void Effect_SpawnRisingDustBurst(struct Resource373Emitter *emitter)
{
    s32 frame_countdown;

    Func_0200b954(154);

    for (frame_countdown = 30; frame_countdown >= 0; frame_countdown--) {
        emitter->y += 0x10000;              /* 0x80 << 9. */
        emitter->field06 = (u16)(emitter->field06 + 0x2000);  /* 0x80 << 6. */
        emitter->field18 += -2048;          /* The pool word 0xfffff800. */
        emitter->field1c += -2048;
        Func_0200b6c0_a(1);
    }

    for (frame_countdown = 7; frame_countdown >= 0; frame_countdown--) {
        struct Resource373Particle *particle =
            Func_0200b754(0x11d, emitter->x, emitter->y, emitter->z);

        if (particle != 0) {
            s32 vertical_speed;

            Func_0200b7a8(particle, 0);
            Func_0200b760(particle, (const void *)0x0200e6e4);

            vertical_speed = Func_0200b70c() + 0x10000;
            particle->field34 = 0x10000;
            particle->field30 = vertical_speed;
            particle->field55 = 2;
            particle->field48 = 0x0a3d;

            particle->lifetime = Func_0200b726() - Func_0200b72c();

            Func_0200b19a(
                particle,
                ((Func_0200b734() * 3) << 3) + 0x80000,
                Func_0200b744());
        }
    }

    Func_0200ba08(131);

    emitter->x = 0;
    emitter->y = 0;
    emitter->z = 0;
    emitter->field38 = (s32)0x80000000;
    emitter->field3c = (s32)0x80000000;
    emitter->field40 = (s32)0x80000000;
    emitter->field24 = 0;
    emitter->field28 = 0;
    emitter->field2c = 0;
}

void Effect_UpdateParticlePosition(s32 *particle, s32 delta_x, s32 delta_z)
{
    s32 position[3];
    if (particle != 0) {
        position[0] = particle[2];
        position[1] = particle[3];
        position[2] = particle[4];
        Func_0200b7d4(delta_x, delta_z, position);
        Func_0200b838((s32)particle, position[0], position[1], position[2]);
    }
}

void SceneState_ApplyRectAndRunTwo(void)
{
    s32 e = 22;
    s32 f = 36;
    Func_0200b87a(17, 0, 3, 1, e, f);
    Func_02005a06();
    Func_0200b296();
}

void Effect_PlayStepSound(void)
{
    if ((*(u32 *)0x03001e40 & 15) == 0)
        Func_0200bb66(0x83);
}

void FieldScene_RunScriptedStepEE4(void)
{
    extern u8 Data_03001ebc[];

    Func_0200b9e4();
    Func_0200b9ac((s32)&Value_00000ee4, 1);
    Func_0200b9f8();
}

void FieldScene_RunScene373SequenceB(void)
{
    extern u8 Data_03001ebc[];

    u32 i;
    s32 rec7;
    s32 record;

    if (Value1(Func_0200b9e2, 0x241) != 0) {
        rec7 = Value1(Func_0200b9ee, 0x106);
        if (rec7 != 0) {
            goto L_02005a8a;
        }
        *(u8 *)(Func_0200ba42(22) + 91) = rec7;
        Call1(Func_0200ba14, 0x241);
    } else {
        if (Value1(Func_0200ba0e, 0x106) != 0) {
            *(u8 *)(Func_0200ba60(22) + 91) = 1;
            Call1(Func_0200ba2c, 0x241);
        }
    }
    L_02005a8a:;
}

void SceneActor_SetFlagByteBySlotZeroPosition(void)
{
    s32 *g = Func_0200ba7e(0);
    u8 *q;
    if (Func_0200ba3e((s32)&Value_0000087a) != 0)
        q = Func_0200ba90(21);
    else
        q = Func_0200ba98(20);
    if (q != 0) {
        if (g[3] > 0xc80000)
            q[0x23] = 3;
        else
            q[0x23] = 1;
    }
}

s32 SceneEffect_UpdateOrbitPosition(s32 *p)
{
    s16 *q = (s16 *)p[20];
    s32 a, b;
    s32 d = Func_0200b99e(p[12]) * 2;
    if (d > 0)
        d = -d;
    p[2] = p[14] + Func_0200b9b4(p[12]) * 2;
    p[3] = p[15] + d;
    q[15] = Func_0200b9ce(p[12] + 0x8000) / 8;
    a = Func_0200b9c4();
    b = Func_0200b9ca();
    p[12] = p[12] + ((((u32)a << 9) >> 16) + (((u32)b << 9) >> 16)) + 0x400;
    return 0;
}

void InitializeStagedActorSceneOrbitingEffect(void)
{
    OrbitingSceneObject *actor;
    OrbitingSceneObjectSprite *sprite;
    u8 *transfer;
    s32 zero;

    actor = GetOrbitingSceneObject();
    sprite = actor->sprite;
    sprite->flags_09_mode = 1;
    sprite->flags_05_bit_5 = 0;
    sprite->flags_09_high = 0;

    zero = 0;
    sprite->state = zero;
    NormalizeOrbitingSceneObject(actor, zero);
    actor->active = zero;
    actor->mode = zero;

    if (IsGameFlagSet(0x109) == 0)
        actor->y += 0x200000;

    actor->flags_23 &= 0xfe;
    actor->visible = 1;

    transfer = AllocateEffectTransfer(17, 0x608);
    LoadEffectResource(181);
    transfer += 0x400;
    ConfigurePaletteTransfer(sprite->palette, 128, transfer);
    ReleaseEffectTransfer(17);

    actor->orbit_center_x = actor->x;
    actor->orbit_angle = zero;
    actor->orbit_center_y = actor->y;
    actor->active = 1;
    actor->callback = (u32)&UpdateOrbitingSceneObject;
    actor->state = zero;
}

void OverlayObject_UpdateOnFrameBit1(s32 p)
{
    extern s32 Data_03001e40;

    if ((Data_03001e40 & 2) != 0)
        Func_0200bba8(p, 7);
    else
        Func_0200bbb2(p, 0);
    if ((Data_03001e40 & 0xf) == 0)
        Func_0200b9b8(p);
}

void SceneEffect_UpdateObjectOnOddFrames(s32 p)
{
    extern volatile u32 Data_03001e40;

    if ((Data_03001e40 & 1) != 0)
        Func_0200bbf0(p, Func_0200baf8(Data_03001e40 >> 1, 6));
    if ((Data_03001e40 & 0xf) == 0)
        Func_0200b9f4(p);
}

void SceneEffect_UpdateObjectOnOddFramesOnly(s32 p)
{
    extern volatile u32 Data_03001e40;

    if ((Data_03001e40 & 1) != 0)
        Func_0200bc2c(p, Func_0200bb34(Data_03001e40 >> 1, 6));
}

void Effect_AnimateVerticalPositive(struct StagedVerticalEffect *effect)
{
    s32 *anchor = effect->f68;
    s32 frame = ++effect->f64;
    if (frame > 31) {
        Func_0200bbec((s32)effect);
    } else {
        s32 amplitude = Func_0200bb9c(frame << 10);
        s32 offset;
        effect->f18 = amplitude;
        effect->f1c = amplitude;
        effect->f8 = anchor[2];
        effect->fc += 0x10000;
        offset = 0x10000 - amplitude;
        effect->f10 = anchor[4] + offset * 5 + 0x80000;
    }
}

void Effect_AnimateVerticalNegative(struct StagedVerticalEffect *effect)
{
    s32 *anchor = effect->f68;
    s32 frame = ++effect->f64;
    if (frame > 31) {
        Func_0200bc3c((s32)effect);
    } else {
        s32 amplitude = Func_0200bbec_a(frame << 10);
        s32 offset;
        effect->f18 = amplitude;
        effect->f1c = -amplitude;
        effect->f8 = anchor[2];
        effect->fc += 0x10000;
        offset = 0x10000 - amplitude;
        effect->f10 = anchor[4] - offset * 5 + 0x100000;
    }
}
