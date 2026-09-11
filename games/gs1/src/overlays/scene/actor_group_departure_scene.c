#include "types.h"

#define OverlayObject_SetField6OnCountdown Func_02000030
#define OverlayObject_SetChildByte5AndMark Func_0200209c
#define OverlayObject_UpdateRandomSlotByFrame Func_02004304
#define OverlayObject_ApplyIwramWord1e40 Func_0200434c
#define OverlayObject_UpdateArcFromParent Func_020043c8
#define OverlayObject_CopyRecordField1ToSlots22And8 Func_020045b8
#define UnsignedRemainder Func_020046fa
#define NextRandomValue Func_02004714
#define UpdateFixedPointCountdown Func_02000064
#define SceneData_GetTableD0E4 Func_020000e8
#define SceneData_ReturnZero Func_020000f0
#define SceneData_GetTableD27c Func_020000f4
#define SceneData_GetTableD2B8 Func_020000fc
#define SceneData_GetTableD558 Func_02000104
#define SceneState_SetFlag210AndConfigureRegion40_84 Func_0200010c
#define FieldScene_SetupDescriptorD774 Func_02000198
#define FieldScene_SetupDescriptorD78a Func_02000240
#define FieldScene_SetupDescriptorD78aIfFlag205Clear Func_020002e4
#define FieldScene_SetupWithDescriptorD7A0 Func_02000328
#define FieldScene_SetupDescriptorD7b6 Func_02000360
#define FieldScene_RunFlagGatedActorSetup Func_02000434
#define FieldScene_DrawFiveTileBlocks Func_02001498
#define FieldScene_RunActor22SceneWhenFlag836Only Func_02001684
#define SceneState_SetFlag210AndConfigureRegion40_89 Func_02000134
#define SceneState_SetWork1c0AndRunObject Func_0200015c
#define SceneState_SetValue123Mode1 Func_020001d0
#define SceneState_ApplyValues123And3 Func_020001e4
#define SceneState_SetValue123Mode4 Func_020001f8
#define SceneState_ApplyFourRects Func_020012f0
#define SceneState_SetWords1c0And1c8AndRun Func_02004234
#define SceneState_SetWorkWordsAndFlag87f Func_02004278
#define SceneState_SetValue140Mode0 Func_02004550
#define SceneState_SetValue19ThenCall Func_020045a8
#define SceneState_SetValueEe4 Func_020045fc
#define FieldScene_RunStep7BAndCheckFlags841And842 Func_0200020c
#define FieldScene_RunScene372_02000278 Func_02000278
#define FieldScene_RunScene372_02000398 Func_02000398
#define FieldScene_RunScene372_020003cc Func_020003cc
#define FieldScene_RunScene372_02000400 Func_02000400
#define FieldScene_RunScene372_02000a10 Func_02000a10
#define FieldScene_RunScene372_02000ec4 Func_02000ec4
#define FieldScene_RunOpeningAuxiliarySequence Func_02001154
#define FieldScene_RunScene372SequenceA Func_0200150c
#define FieldScene_RunScene372SequenceB Func_02001600
#define FieldScene_RunScene372SequenceC Func_020016cc
#define ActorPresentation_SetEightSceneCells Func_020010a4
#define SceneActor_RunActor22PlacementSequence Func_02001a64
#define ActorPresentation_SetFourActorsModeByBit Func_020020fc
#define ActorPresentation_SelectActorTwentySevenState Func_02003e18
#define SceneActor_SetModeByFrameBit1 Func_020042bc
#define BattleRuntime_Reset_1(args...) Func_020086b0(args)
#define ObjectMotion_PlaceWithinCameraBounds_1(a0, a1, a2, a3) Call4(Func_020087ea, a0, a1, a2, a3)
#define ObjectMotion_EnableActionAndResetMotion_1(args...) Func_02008720(args)
#define ObjectMotion_SetPositionAndReset_1(a0, a1, a2) Call3(Func_0200875c, a0, a1, a2)
#define ObjectMotion_SetHorizontalPositionWithTerrain_1(args...) Func_02008776(args)
#define ObjectMotion_ArmCallback_1(a0, a1, a2) Call3(Func_020087f2, a0, a1, a2)
#define Scene_GetRecord_1(args...) Func_02008728(args)
#define ObjectMotion_SetHorizontalPositionWithTerrain_2(a0, a1, a2) Call3(Func_020087a0, a0, a1, a2)
#define SceneWork_SetStepValue_1(a0) Call1(Func_020087f4, a0)
#define BattleEvent_RunActionAndWait_1(a0, a1) Call2(Func_0200880c, a0, a1)
#define ObjectMotion_SetHorizontalPositionWithTerrain_3(a0, a1, a2) Call3(Func_020087c0, a0, a1, a2)
#define ObjectMotion_SetSpeedLimitAndAcceleration_1(a0, a1) Call2(Func_02008862, a0, a1)
#define ObjectMotion_PlaceWithinCameraBounds_2(a0, a1, a2, a3) Call4(Func_02008878, a0, a1, a2, a3)
#define Object_CommitPositionThenWaitIfModeZero_1(args...) Func_02008884(args)
#define BattleRuntime_WaitIfModeZero_1(args...) Func_02008752(args)
#define ObjectMotion_CallThenWaitForAnimationChange_1(args...) Func_02008802(args)
#define BattleRuntime_RunThenWaitIfModeZero_1(a0, a1, a2) Call3(Func_0200885c, a0, a1, a2)
#define ObjectMotion_ArmCallback_2(a0, a1, a2) Call3(Func_02008878_a, a0, a1, a2)
#define ObjectMotion_SetVariantCallbackAndInvokeObject_1(args...) Func_02008838(args)
#define BattleRuntime_RunThenWaitIfModeZero_2(a0, a1, a2) Call3(Func_0200887a, a0, a1, a2)
#define ObjectMotion_ArmCallback_3(a0, a1, a2) Call3(Func_02008896, a0, a1, a2)
#define ObjectMotion_CallThenWaitForAnimationChange_2(args...) Func_0200883e(args)
#define ObjectMotion_SetSpeedParameters_1(a0, a1, a2) Call3(Func_020087ec, a0, a1, a2)
#define ObjectMotion_SetPositionAndReset_2(a0, a1, a2) Call3(Func_02008836, a0, a1, a2)
#define ObjectMotion_SetPositionAndReset_3(a0, a1, a2) Call3(Func_02008842, a0, a1, a2)
#define GameFlag_Set_1(a0) Call1(Func_020087b0, a0)
#define ACTOR_ID 22
#define FieldScene_RunScene372SequenceE Func_02001828
#define FieldScene_RunScene372SequenceD Func_0200198c
#define FieldScene_RunReturnScript Func_02001b18
#define FieldScene_RunFlagGatedActorSequence Func_02002180
#define FieldScene_RunScene372_02003c48 Func_02003c48
#define FieldScene_RunScene372_02003e48 Func_02003e48
#define FieldScene_ConfigureActorTwentyTwoScene Func_02003fb0
#define FieldScene_BuildPlacementGrid Func_020040f0
#define FieldScene_RunFourPairedSteps Func_0200456c
#define FieldScene_RunScriptedStep1120 Func_02004618
#define ObjectMotion_ArmCallback_1_020028a4(a0, a1, a2) Call3(Func_020070ac, a0, a1, a2)
#define ObjectMotion_ArmCallback_2_020028a4(a0, a1, a2) Call3(Func_020070b8, a0, a1, a2)
#define ObjectMotion_ArmCallback_3_020028a4(a0, a1, a2) Call3(Func_020070c4, a0, a1, a2)
#define ObjectMotion_ArmCallback_4(a0, a1, a2) Call3(Func_020070d0, a0, a1, a2)
#define ObjectMotion_ArmCallback_5(a0, a1, a2) Call3(Func_020070dc, a0, a1, a2)
#define Object_SetModeById_1(args...) Func_0200707c(args)
#define Object_SetModeById_2(args...) Func_02007084(args)
#define Object_SetModeById_3(args...) Func_0200708c(args)
#define Object_SetModeById_4(args...) Func_02007094(args)
#define ObjectMotion_CallThenWaitForAnimationChange_1_020028a4(args...) Func_020070a4(args)
#define BattleRuntime_WaitIfModeZero_1_020028a4(args...) Func_02007002(args)
#define ObjectMotion_SetSpeedLimitAndAcceleration_1_020028a4(a0, a1) Call2(Func_02007136, a0, a1)
#define ObjectMotion_PlaceWithinCameraBounds_1_020028a4(a0, a1, a2, a3) Call4(Func_0200714e, a0, a1, a2, a3)
#define ObjectMotion_SetSpeedParameters_1_020028a4(a0, a1, a2) Call3(Func_02007070, a0, a1, a2)
#define ObjectMotion_SetSpeedParameters_2(a0, a1, a2) Call3(Func_0200707a, a0, a1, a2)
#define ObjectMotion_EnableActionAndSetCallback_1(a0, a1) Value2(Func_0200708a, a0, a1)
#define ObjectMotion_MarkActiveAndSetActionCallback_1(a0, a1) Call2(Func_020070aa, a0, a1)
#define Audio_PlayCue_1(args...) Func_02007208(args)
#define BattleRuntime_WaitIfModeZero_2(args...) Func_02007058(args)
#define ObjectMotion_SetPositionAndReset_1_020028a4(a0, a1, a2) Call3(Func_020070ea, a0, a1, a2)
#define ObjectMotion_SetHorizontalPositionWithTerrain_1_020028a4(args...) Func_02007104(args)
#define ObjectMotion_SetPositionAndReset_2_020028a4(a0, a1, a2) Call3(Func_020070fe, a0, a1, a2)
#define BattleEffect_PlayQueuedSound_1(args...) Func_02007232(args)
#define ObjectMotion_ArmCallback_6(a0, a1, a2) Call3(Func_0200718e, a0, a1, a2)
#define ObjectMotion_ArmCallback_7(a0, a1, a2) Call3(Func_0200719a, a0, a1, a2)
#define ObjectMotion_ArmCallback_8(a0, a1, a2) Call3(Func_020071a6, a0, a1, a2)
#define Object_SetModeById_5(args...) Func_02007146(args)
#define Object_SetModeById_6(args...) Func_0200714e_a(args)
#define Object_SetModeById_7(args...) Func_02007156(args)
#define Scene_GetRecord_1_020028a4(a0) Value1(Func_020070f4, a0)
#define Scene_GetRecord_2(a0) Value1(Func_0200710c, a0)
#define Scene_GetRecord_3(a0) Value1(Func_02007124, a0)
#define ObjectMotion_EnableActionAndSetCallback_2(args...) Func_02007158(args)
#define ObjectMotion_EnableActionAndSetCallback_3(a0, a1) Value2(Func_02007160, a0, a1)
#define ObjectMotion_EnableActionAndSetCallback_4(args...) Func_02007168(args)
#define Object_LookupAndStep_1(args...) Func_02007176(args)
#define BattleRuntime_WaitIfModeZero_3(args...) ((void (*)())Func_02007124_a)(args)
#define Audio_PlayCue_2(args...) Func_020072ea(args)
#define BattleRuntime_WaitIfModeZero_4(args...) Func_0200713a(args)
#define BattleEffect_PlayQueuedSound_2(args...) Func_020072f6(args)
#define ObjectMotion_PlaceWithinCameraBounds_2_020028a4(a0, a1, a2, a3) Call4(Func_0200727e, a0, a1, a2, a3)
#define Audio_PlayCue_3(args...) Func_02007314(args)
#define BattleRuntime_WaitIfModeZero_5(args...) Func_02007164(args)
#define BattleEffect_PlayQueuedSound_3(args...) Func_02007320(args)
#define ObjectMotion_EnableActionAndSetCallback_5(a0, a1) Call2(Func_020071c0, a0, a1)
#define BattleRuntime_WaitIfModeZero_6(args...) Func_02007176_a(args)
#define ObjectMotion_EnableActionAndSetCallback_6(a0, a1) Call2(Func_020071ce, a0, a1)
#define BattleRuntime_WaitIfModeZero_7(args...) Func_02007184(args)
#define Audio_PlayCue_4(args...) Func_0200734a(args)
#define Object_LookupAndStep_2(args...) Func_020071f2(args)
#define BattleEffect_PlayQueuedSound_4(args...) Func_02007356(args)
#define BattleRuntime_WaitIfModeZero_8(args...) Func_020071a4(args)
#define SceneWork_SetStepValue_1_020028a4(args...) Func_0200728c(args)
#define BattleRuntime_RunThenWaitIfModeZero_1_020028a4(args...) Func_020072ae(args)
#define ObjectMotion_CallThenWaitForAnimationChange_2_020028a4(args...) Func_02007266(args)
#define BattleRuntime_RunThenWaitIfModeZero_2_020028a4(a0, a1, a2) Call3(Func_020072c0, a0, a1, a2)
#define Object_SetModeById_8(args...) Func_02007270(args)
#define ObjectMotion_CallThenWaitForAnimationChange_3(args...) Func_02007280(args)
#define BattleRuntime_WaitIfModeZero_9(args...) Func_020071de(args)
#define ObjectMotion_EnableActionAndSetCallback_7(a0, a1) Value2(Func_02007236, a0, a1)
#define ObjectMotion_EnableActionAndSetCallback_8(a0, a1) Call2(Func_0200723e, a0, a1)
#define BattleRuntime_WaitIfModeZero_10(args...) Func_020071f4(args)
#define ObjectMotion_SetSpeedLimitAndAcceleration_2(a0, a1) Call2(Func_02007328, a0, a1)
#define ObjectMotion_PlaceWithinCameraBounds_3(a0, a1, a2, a3) Call4(Func_02007340, a0, a1, a2, a3)
#define Object_LookupAndStep_3(args...) Func_0200726e(args)
#define ObjectMotion_ArmCallback_9(args...) Func_02007328_a(args)
#define BattleEffect_SpawnLinkedResourceObject_1(a0, a1, a2) Call3(Func_02007344, a0, a1, a2)
#define BattleRuntime_RunThenWaitIfModeZero_3(args...) Func_0200732e(args)
#define ObjectMotion_ArmCallback_10(a0, a1, a2) Call3(Func_0200734a_a, a0, a1, a2)
#define ObjectMotion_ArmCallback_11(a0, a1, a2) Call3(Func_02007356_a, a0, a1, a2)
#define ObjectMotion_SetPositionAndReset_3_020028a4(a0, a1, a2) Call3(Func_020072e0, a0, a1, a2)
#define ObjectMotion_SetVariantCallbackAndInvokeObject_1_020028a4(args...) Func_02007320_a(args)
#define ObjectMotion_SetSpeedParameters_3(a0, a1) Value2(Func_02007350, a0, a1)
#define ObjectMotion_ArmCallback_12(args...) Func_0200737a(args)
#define UiWork_WaitThenFinalizeCapacity_1(a0, a1) Value2(Func_020072aa, a0, a1)
#define ObjectMotion_CallThenWaitForAnimationChange_4(args...) Func_0200732e_a(args)
#define SceneWork_SetStepValue_2(args...) Func_0200736c(args)
#define ObjectMotion_SetVariantCallbackAndInvokeObject_2(args...) Func_02007356_b(args)
#define SceneWork_SetStepValue_3(args...) Func_0200737c(args)
#define BattleEvent_RunActionAndWait_1_020028a4(a0, a1) Call2(Func_02007394, a0, a1)
#define ObjectMotion_ArmCallback_13(a0, a1, a2) Call3(Func_020073b8, a0, a1, a2)
#define BattleEffect_SpawnLinkedResourceObject_2(a0, a1, a2) Call3(Func_020073d4, a0, a1, a2)
#define SceneWork_SetStepValue_4(args...) Func_020073a4(args)
#define ObjectMotion_SetSpeedParameters_4(a0, a1) Value2(Func_020073b4, a0, a1)
#define UiWork_WaitThenFinalizeCapacity_2(a0, a1) Value2(Func_02007304, a0, a1)
#define ObjectMotion_CallThenWaitForAnimationChange_5(args...) Func_02007388(args)
#define SceneWork_SetStepValue_5(args...) Func_020073c6(args)
#define BattleRuntime_RunThenWaitIfModeZero_4(a0, a1, a2) Call3(Func_020073e8, a0, a1, a2)
#define ObjectMotion_ArmCallback_14(a0, a1, a2) Call3(Func_02007404, a0, a1, a2)
#define Object_SetModeById_9(args...) Func_020073a4_a(args)
#define Object_SetModeById_10(args...) Func_020073ac(args)
#define ObjectMotion_CallThenWaitForAnimationChange_6(args...) Func_020073bc(args)
#define BattleRuntime_WaitIfModeZero_11(args...) Func_0200731a(args)
#define BattleEffect_SpawnLinkedResourceObject_3(a0, a1, a2) Call3(Func_0200743e, a0, a1, a2)
#define BattleEffect_SpawnLinkedResourceObject_4(a0, a1, a2) Call3(Func_02007448, a0, a1, a2)
#define Object_SetModeById_11(args...) Func_020073d8(args)
#define SceneWork_SetStepValue_6(args...) Func_0200741e(args)
#define BattleEvent_RunActionAndWait_2(a0, a1) Call2(Func_02007436, a0, a1)
#define ObjectMotion_EnableActionAndSetCallback_9(a0, a1) Call2(Func_0200739e, a0, a1)
#define BattleRuntime_WaitIfModeZero_12(args...) Func_02007354(args)
#define Object_LinkPair_1(args...) Func_0200742e(args)
#define BattleRuntime_WaitIfModeZero_13(args...) Func_02007364(args)
#define Object_SetModeById_12(args...) Func_0200740c(args)
#define ObjectMotion_CallThenWaitForAnimationChange_7(args...) Func_0200741c(args)
#define BattleRuntime_WaitIfModeZero_14(args...) Func_0200737a_a(args)
#define Object_SetModeById_13(args...) Func_02007422(args)
#define Scene_GetRecord_4(a0) Value1(Func_020073c0, a0)
#define ObjectMotion_ResetAndSetPosition_1(args...) Func_0200740a(args)
#define ObjectMotion_CommitCurrentPositionAndActivate_1(args...) Func_02007430(args)
#define ObjectMotion_SetHorizontalPositionWithTerrain_2_020028a4(args...) Func_02007442(args)
#define RunEventScript01 Func_020028a4
#define SceneDialogue_RunActorTenFlag30dDialogue Func_02002d28
#define SceneEffect_UpdateArcOverAnchor Func_02004378
#define FieldScene_RunSingleStep Func_02004560

struct Object {
    u8 filler00[6];
    u16 x;
    u8 filler08[92];
    s16 counter;
};

typedef struct Obj {
    u8 pad00[8];
    s32 f08;
    s32 f0c;
    s32 f10;
    u8 pad14[4];
    s32 f18;
    s32 f1c;
    u8 pad20[0x44];
    u16 f64;
    u8 pad66[2];
    struct Obj *f68;
} Obj;

typedef struct Ent {
    u8 pad00[9];
    u8 b01:2;
    u8 f:2;
    u8 b45:4;
} Ent;

typedef struct Rec {
    u8 pad00[0x50];
    Ent *f50;
} Rec;

struct FixedPointCountdown {
    u8 pad_00[0x18];
    s32 fixed_point_18;
    s32 fixed_point_1c;
    u8 pad_20[0x44];
    s16 countdown;
};

typedef struct {
    u8 filler0[8];
    s32 unk8;
    u8 fillerC[4];
    s32 unk10;
} Thing1;

typedef struct {
    u8 filler0[10];
    s16 unkA;
    u8 fillerC[6];
    s16 unk12;
} Thing2;

struct Actor {
    u8 reserved_00[80];
    u8 *presentation;
};

extern u8 Data_0200c8bc[];
extern u8 Data_0200d0e4[];
extern u8 Data_0200d27c[];
extern u8 Data_0200d2b8[];
extern u8 Data_0200d558[];
extern u8 Data_0200d774[];
extern u8 Value_00000101;
extern u8 Data_0200d78a[];
extern u8 Value_000002d7;
extern u8 Value_00000205;
extern u8 Value_00000109;
extern u8 Value_00000206;
extern u8 Value_00000831;
extern u8 Value_00000832;
extern u8 Value_00000833;
extern u8 Value_00000837;
extern u8 Value_00000838;
extern u8 Value_00000841;
extern u8 Value_00000842;
extern u8 Value_0000083a;
extern u8 Value_0000087b;
extern u8 Value_00000834;
extern u8 Data_0200c9f4[];
extern u8 Data_0200cec8[];
extern u8 Data_0200c5b9[];
extern u8 Data_02000240[];
extern u8 Data_03001ebc[];
extern u8 Data_00000e5c[];
extern u8 Data_00000e67[];
extern u8 Data_00000ed0[];
extern u32 Data_03001e40;
extern u8 Data_00000e74[];
extern u8 Data_0200d7f8[];
extern u8 Value_00000e8c;
extern u8 Value_0000201a;
extern u8 Value_00001120;
extern u8 Data_00000e9b[];
extern u8 Data_00000ea1[];
extern u8 Value_0000030d;

s32 Func_02004698(void);
s32 Func_0200469e(void);
s32 Func_02004684(s32, s32);
s32 Func_02008950(s32, s32);
void Func_020089f0(s32, s32);
s32 Func_02008966(s32, s32);
void Func_02008758(s32);
s32 Func_02008998(s32, s32);
void Func_02008a38(s32, s32);
void Func_02008a60(Obj *);
s32 Func_02008a48(s32);
Rec *Func_02008cea(s32);
Rec *Func_02008cf2(s32);
Rec *Func_02008d0e(s32);
u32 Func_020046fa(u32 value, u32 divisor);
u32 Func_02004714(void);
void Func_020047fa(s32);
void Func_020047d6(s32, s32, s32, s32, s32, s32);
void Func_02004a52(s32);
void Func_02004834(u8 *, s32, s32);
void Func_02004928(s32, s32, s32);
void Func_020048ae(s32);
void Func_0200031c(s32);
void Func_02004afa(s32);
void Func_020048dc(u8 *, s32, s32);
void Func_020049d0(s32, s32, s32);
void Func_02004956(s32);
void Func_020003c4(s32);
s32 Func_020049c6(s32);
void Func_02004ba8(s32);
void Func_0200498a(u8 *, s32, s32);
void Func_02004a80(s32, s32, s32);
void Func_02004a06(s32);
void Func_02000474(s32);
void Func_02004be2(s32);
void Func_020049c4(void *, s32, s32);
s32 Func_02004ab8(s32, s32, s32);
void Func_02004a3e(s32);
void Func_020004ac(s32);
void Func_02004c1a(s32);
void Func_020049fc(void *, s32, s32);
s32 Func_02004af0(s32, s32, s32);
void Func_02004a76(s32);
void Func_020004e4(s32);
void Func_02004ce4(s32);
void Func_02004bd6(s32, s32, s32);
s32 Func_02004b2c(s32);
void Func_02004b46(s32);
void Func_02004b4c(s32);
s32 Func_02004b44(s32);
void Func_02004c06(s32, s32, s32);
void Func_02001342(void);
s32 Func_02004b60(s32);
s32 Func_02004c22(s32);
void Func_02004c22_b(s32, s32, s32);
void Func_0200153e(void);
s32 Func_02004b7c(s32);
void Func_02004c3c(s32, s32, s32);
void Func_020017a4(void);
s32 Func_02004b96(s32);
u8 *Func_02004c56(s32);
void Func_02004c56_b(s32, s32, s32);
void Func_02001966(void);
u8 *Func_02004c00(s32);
u8 *Func_02004c10(s32);
u8 *Func_02004c1e(s32);
u8 *Func_02004c2c(s32);
u8 *Func_02004c3a(s32);
u8 *Func_02004c48(s32);
u8 *Func_02004c64(s32);
u8 *Func_02004c64_a(s32);
void Func_02004ce0(s32, s32, s32);
u8 *Func_02004c86(s32);
s32 Func_02004c44(s32);
void Func_02004d04(s32, s32, s32);
void Func_02004cc6(s32, u8 *);
void Func_02004682(void);
void Func_02004d2a(s32, s32, s32);
u8 *Func_02004cd0(s32);
s32 Func_02004c04(void);
s32 Func_02004bea(s32, s32);
void Func_02004d14(s32, u8 *);
void Func_02004d68(s32, s32, s32);
u8 *Func_02004d0e(s32);
s32 Func_02004c3e(void);
s32 Func_02004c24(s32, s32);
void Func_02004d4a(s32, u8 *);
void Func_02004d9e(s32, s32, s32);
u8 *Func_02004d44(s32);
s32 Func_02004c74(void);
s32 Func_02004c5a(s32, s32);
void Func_02004d80(s32, u8 *);
void Func_02004dd6(s32, s32, s32);
u8 *Func_02004d7c(s32);
s32 Func_02004cac(void);
s32 Func_02004c92(s32, s32);
void Func_02004db8(s32, u8 *);
void Func_02004e10(s32, s32);
u8 *Func_02004dae(s32);
u8 *Func_02004dc0(s32);
u8 *Func_02004dc0_a(s32);
void Func_02004cea(u8 *, s32);
u8 *Func_02004e3c(s32);
void Func_02004e3c_b(s32, s32, s32);
void Func_02004e46(s32, s32, s32);
void Func_02004e50(s32, s32, s32);
void Func_02004e5a(s32, s32, s32);
s32 Func_02004db0(s32);
void Func_02004f70(s32, s32, s32);
void Func_02004e82(s32, s32, s32);
void Func_02004efe(s32, s32, s32);
void Func_02004e9e(s32, s32);
s32 Func_02004d6a(void);
s32 Func_02004d50(s32, s32);
void Func_02004e70(s32, u8 *);
void Func_02004ec4(s32, s32, s32);
void Func_02004f40(s32, s32, s32);
void Func_02004ee0(s32, s32);
u8 *Func_02004e7e(s32);
s32 Func_02004dac(void);
s32 Func_02004d92(s32, s32);
void Func_02004eb0(s32, u8 *);
void Func_02004f04(s32, s32, s32);
void Func_02004f80(s32, s32, s32);
void Func_02004f20(s32, s32);
u8 *Func_02004ebe(s32);
s32 Func_02004dec(void);
s32 Func_02004dd2(s32, s32);
void Func_02004ef0(s32, u8 *);
s32 Func_02004f44(s32);
void Func_02004f44_b(s32, s32, s32);
void Func_02004fc0(s32, s32, s32);
s32 Func_02004ef6(s32);
void Func_02004e84(s32, s32);
void Func_02004f66(s32, s32, s32);
void Func_02004fd4(s32, s32, s32);
void Func_02004fde(s32, s32, s32);
void Func_020050bc(void);
void Func_020050c8(void);
s32 Func_02004f58(s32);
void Func_02000990(void);
s32 Func_02004f6e(s32);
void Func_02004f6e_b(s32, s32, s32, s32, s32, s32);
s32 Func_02004f4a(s32);
void Func_02004f4a_b(s32, s32, s32, s32, s32, s32);
void Func_02004f5c(s32, s32, s32, s32, s32, s32);
void Func_02004f82(s32, s32, s32, s32, s32, s32);
void Func_02004f94(s32, s32, s32, s32, s32, s32);
u8 *Func_0200501a(s32);
void Func_02004fbc(u8 *, s32);
s32 Func_02004ff0(s32);
void Func_0200133c(void);
void Func_020050ca(s32, s32);
s32 Func_02005018(s32);
void Func_02005040(void);
void Func_02005160(s32, s32);
void Func_020050e4(s32, s32, s32);
void Func_020050e4_a(s32, s32, s32);
void Func_020050f0(s32, s32, s32);
void Func_020050f8(s32, s32, s32);
void Func_02005118(s32, s32);
void Func_02005120(s32, s32);
void Func_02005094(void);
void Func_02005092(void);
void Func_0200512e(s32, s32, s32);
void Func_0200512a(s32, s32, s32);
void Func_0200514a(s32, s32);
void Func_020050be(void);
void Func_02005258(void);
void Func_02005264(void);
void Func_02005248(void);
void Func_02005b60(s32, s32, s32, s32, s32, s32);
void Func_02005b72(s32, s32, s32, s32, s32, s32);
void Func_02005b84(s32, s32, s32, s32, s32, s32);
void Func_02005b98(s32, s32, s32, s32, s32, s32);
void Func_02005baa(s32, s32, s32, s32, s32, s32);
s32 Func_02005d66(s32);
s32 Func_02005d70(s32);
void Func_02005d98(void);
void Func_02005e58(s32, s32);
void Func_02005d9e(s32);
void Func_02005e84(s32);
void Func_02002df0(void);
void Func_02005dbc(void);
void Func_0200482a(s32);
void Func_020047fe(s32, s32, s32, s32, s32, s32);
s32 Func_02004840(s32);
void Func_020049c8(void);
void Func_020049c4_a(u8 *);
void Func_02004a8a(s32);
void Func_02000338(s32);
void Func_02004a9e(s32);
void Func_0200034c(s32);
void Func_02004ab2(s32);
void Func_020059b2(s32, s32, s32, s32, s32, s32);
void Func_020059c4(s32, s32, s32, s32, s32, s32);
void Func_020059d6(s32, s32, s32, s32, s32, s32);
void Func_020059e8(s32, s32, s32, s32, s32, s32);
void Func_02008934(void);
void Func_02008938(s32);
s32 Func_02008aa0(s32, s32);
void Func_02008948(s32);
void Func_0200896c(void);
void Func_02008978(void);
void Func_0200897c(s32);
s32 Func_02008ae4(s32, s32);
void Func_0200898c(s32);
void Func_020089b0(void);
void Func_02008dec(s32, s32);
void Func_02008cda(s32);
void Func_020088fe(void);
void Func_02008cfc(void);
void Func_02008cdc(s32, s32);
void Func_02008d10(void);
void Func_02004ac6(s32);
s32 Func_020048f4(s32);
s32 Func_020048fe(s32);
void Func_020041da(void);
void Func_0200038c(s32);
void Func_02000426();
void Func_020017b6();
void Func_0200491e();
s32 Func_0200495a();
s32 Func_02004974();
s32 Func_0200497e();
void Func_02004994();
void Func_020049b8();
void Func_02004a32();
void Func_02004b3c();
void Func_0200051a();
void Func_02004a34();
void Func_02004aac();
void Func_02004b26();
void Func_02004c52();
void Func_0200054e();
void Func_02004a68();
void Func_02004ae0();
void Func_02004b5a();
void Func_02004c86_a();
void Func_02000582();
void Func_02004a9c();
void Func_02004b14();
void Func_02004b8e();
void Func_02004cba();
void Func_02005062();
void Func_02005110();
void Func_0200511a();
void Func_020051dc();
void Func_020051de();
void Func_020051e6();
void Func_020051e8();
void Func_020051f0();
void Func_0200523e();
void Func_02005248_a();
void Func_0200524c();
void Func_0200524e();
void Func_02005250();
void Func_0200525e();
void Func_02005264_a();
void Func_02005268();
void Func_02005268_a();
void Func_02005270();
void Func_02005274();
void Func_0200527c();
void Func_02005284();
void Func_02005288();
void Func_020052a8();
void Func_020052aa();
void Func_020052b4();
void Func_020052c0();
void Func_020052d0();
void Func_020052ea();
void Func_020052ee();
void Func_020052fa();
void Func_02005304();
void Func_02005306();
void Func_0200531c();
void Func_02005320();
void Func_02005324();
void Func_02005328();
void Func_02005338();
void Func_0200533e();
void Func_02005344();
void Func_0200536a();
void Func_0200536c();
void Func_02005372();
void Func_0200537c();
void Func_0200538c();
void Func_02005392();
void Func_02005394();
void Func_02005396();
s32 Func_0200539c();
void Func_020053ae();
void Func_020053b6();
void Func_020053be();
void Func_020053c6();
void Func_020053c8();
void Func_020053ce();
void Func_020053d2();
void Func_020053e0();
void Func_020053e2();
void Func_020053e6();
void Func_020053e6_a();
void Func_020053e6_b();
void Func_020053f6();
void Func_020053f6_a();
void Func_0200540a();
void Func_0200540c();
void Func_02005410();
void Func_02005416();
s32 Func_0200541c();
void Func_02005420();
void Func_02005428();
void Func_0200542e();
void Func_02005438();
void Func_0200543c();
void Func_02005440();
void Func_0200544c();
void Func_02005454();
void Func_0200545a();
void Func_0200545e();
void Func_02005464();
void Func_02005474();
void Func_0200547c();
void Func_02005480();
void Func_02005484();
void Func_0200558c();
void Func_0200559e();
void Func_020055b0();
void Func_020055c2();
void Func_020055d4();
void Func_02002538();
s32 Func_02002ce8();
s32 Func_02002cfc();
void Func_020057ea();
s32 Func_0200583c();
s32 Func_0200584c();
void Func_0200585e();
void Func_02005866();
s32 Func_020058a6();
u8 *Func_020058ae();
void Func_020058d2();
void Func_020058e0();
u8 *Func_020058ee();
void Func_020058f4();
void Func_0200590a();
s32 Func_02005930();
void Func_02005932();
s32 Func_0200593a();
s32 Func_02005946();
u8 *Func_02005960();
void Func_02005962();
void Func_02005976();
void Func_0200598e();
u8 *Func_020059a0();
void Func_020059b2_a();
void Func_020059c0();
void Func_02005a0a();
void Func_02005a1e();
void Func_02005a32();
void Func_02005a5c();
void Func_02005a68();
void Func_02005aba();
void Func_02005acc();
void Func_02005c0c();
void Func_02005c72();
void Func_02005c7e();
void Func_02005c98();
void Func_02005c9c();
void Func_02005ca8();
void Func_02005cb4();
void Func_02005cbc();
void Func_02005cd4();
void Func_02005ce0();
void Func_02005cf0();
void Func_02005cf4();
void Func_02005d0e();
void Func_02005d2e();
void Func_02005d38();
void Func_02005d3a();
void Func_02005d6e();
void Func_02005d6e_a();
void Func_02005d86();
void Func_02005d94();
void Func_02005d9e_a();
s32 Func_02005da4();
void Func_02005db6();
void Func_02005e24();
s32 Func_02005ce2();
s32 Func_02005cec();
void Func_02005d14();
void Func_02005d48();
void Func_02005d4a();
void Func_02005d6c();
void Func_02005dba();
void Func_02005df2();
void Func_02005e04();
void Func_02005e04_a();
void Func_02005e14();
void Func_02005e3e();
void Func_02005e42();
void Func_02002e5e();
s32 Func_02005dae();
void Func_02005dd6();
void Func_02005dde();
s32 Func_02005dec();
void Func_02005e0c();
void Func_02005e14_a();
void Func_02005e2a();
void Func_02005ea0();
void Func_02005ec4();
void Func_02005edc();
void Func_02005ef2();
void Func_02005f00();
void Func_0200576e(s32, s32, s32, s32, s32, s32);
void Func_02005780(s32, s32, s32, s32, s32, s32);
void Func_02005794(s32, s32, s32, s32, s32, s32);
void Func_020057a6(s32, s32, s32, s32, s32, s32);
void Func_020057ba(s32, s32, s32, s32, s32, s32);
void Func_020057cc(s32, s32, s32, s32, s32, s32);
void Func_020057dc(s32, s32, s32, s32, s32, s32);
void Func_020057f0(s32, s32, s32, s32, s32, s32);
Thing1 *Func_0200619a(s32);
void Func_02006208(s32, s32, s32);
void Func_020061c6(s32, s32, s32);
void Func_02006210(s32, s32, s32);
void Func_02006262(s32, s32, s32);
void Func_02006198(s32);
void Func_020062c2(s32, s32);
void Func_020061a8(s32);
void Func_0200628e(s32);
void Func_020062a6(s32, s32);
void Func_0200627e(s32, s32);
void Func_020062b6(s32, s32);
void Func_02006276(s32, s32);
void Func_02006276_a(s32, s32);
Thing2 *Func_02006214(s32);
void Func_0200625e(s32, s32, s32);
void Func_02006284(s32);
void Func_02006296(s32, s32, s32);
s32 Func_02006840(s32);
void Func_020041b6(s32, s32);
s32 Func_0200684c(s32);
void Func_020041c2(s32, s32);
s32 Func_02006858(s32);
void Func_020041ce(s32, s32);
s32 Func_02006864(s32);
void Func_020041da_a(s32, s32);
s32 Func_02006872(s32);
void Func_020041e8(s32, s32);
s32 Func_0200687e(s32);
void Func_020041f4(s32, s32);
s32 Func_0200688a(s32);
void Func_02004200(s32, s32);
s32 Func_02006896(s32);
void Func_0200420c(s32, s32);
struct Actor *Func_0200854a(s32);
void Func_0200899c(s32, s32);
void Func_020089a6(s32, s32);
s32 Func_0200891e(s32, s32);
void Func_02008710(s32);
s32 Func_02005f0a();
void Func_02005f34();
void Func_02005f9e();
void Func_02005fc8();
void Func_02005fce();
void Func_02005fd6();
s32 Func_02005fec();
s32 Func_02005ff6();
s32 Func_02005ff6_a();
void Func_0200601e();
void Func_02006036();
void Func_0200604a();
void Func_02006056();
s32 Func_0200605a();
void Func_0200605c();
void Func_02006066();
void Func_02006068();
void Func_0200606a();
void Func_0200606c();
void Func_0200606e();
void Func_02006082();
void Func_0200608e();
void Func_0200609e();
void Func_020060a0();
void Func_020060a0_a();
void Func_020060a4();
void Func_020060a8();
void Func_020060a8_a();
void Func_020060bc();
void Func_020060c6();
void Func_020060ca();
void Func_020060d0();
void Func_020060dc();
void Func_020060e8();
void Func_020060f4();
void Func_020060f8();
void Func_0200608c();
void Func_020060c0();
s32 Func_020060c2();
void Func_020060de();
void Func_020060ec();
void Func_020060ee();
void Func_02006130();
void Func_02006138();
s32 Func_02006146();
void Func_02006150();
void Func_0200618a();
void Func_02006190();
void Func_020061a0();
void Func_020061a6();
void Func_020061a6_a();
void Func_020061a8_a();
void Func_020061b6();
void Func_020061be();
void Func_020061c4();
void Func_020061c8();
void Func_020061f0();
void Func_02006200();
void Func_02005f86();
void Func_02005f90();
void Func_02005f9a();
void Func_02005fa4();
void Func_0200608e_a();
void Func_02006180();
s32 Func_02006206();
void Func_02006216();
void Func_0200622c();
void Func_02006230();
void Func_02006246();
void Func_02006254();
void Func_02006270();
void Func_020062a6_a();
s32 Func_020062ae();
void Func_020062c2_a();
void Func_020062da();
void Func_020062e8();
void Func_020062ea();
s32 Func_020062f8();
void Func_02006306();
void Func_0200630e();
void Func_02006318();
void Func_0200631c();
void Func_02006320();
void Func_0200632c();
s32 Func_02006350();
void Func_0200635e();
s32 Func_0200637e();
void Func_020063c8();
void Func_020063d0();
void Func_020063d8();
void Func_020063da();
void Func_020063e0();
void Func_020063e6();
s32 Func_020063f2();
s32 Func_020063fc();
void Func_020063fe();
s32 Func_02006406();
s32 Func_02006410();
void Func_02006414();
void Func_02006420();
void Func_0200642a();
void Func_02006430();
void Func_0200643a();
void Func_0200643a_a();
void Func_02006450();
void Func_02006462();
void Func_0200646a();
s32 Func_0200646c();
void Func_02006470();
void Func_02006472();
void Func_02006472_a();
void Func_0200647a();
void Func_0200648e();
void Func_02006492();
void Func_0200649a();
void Func_020064a4();
void Func_020064a8();
void Func_020064c0();
void Func_020064c6();
void Func_020064c6_a();
void Func_020064cc();
void Func_020064d2();
void Func_020064de();
void Func_020064de_a();
void Func_020064ea();
void Func_02006502();
void Func_02006506();
void Func_02006508();
void Func_0200650a();
void Func_02006514();
void Func_02006538();
void Func_0200653a();
void Func_02006540();
void Func_0200655c();
void Func_02006568();
void Func_02006570();
void Func_02006572();
void Func_02006576();
void Func_020065a6();
void Func_020065ac();
void Func_020065dc();
void Func_02006610();
void Func_0200662e();
void Func_02006632();
void Func_02006638();
void Func_02006638_a();
void Func_0200664a();
void Func_02006656();
void Func_0200665c();
void Func_02006666();
void Func_02006666_a();
void Func_0200666e();
void Func_02006670();
void Func_02006686();
void Func_0200668c();
void Func_0200669a();
void Func_0200669e();
void Func_020066a2();
void Func_020066a6();
void Func_020066be();
void Func_020066c4();
void Func_020066d4();
void Func_020066dc();
void Func_020066e8();
void Func_020066ec();
void Func_020066f2();
void Func_020066f8();
void Func_020066fe();
void Func_0200670a();
void Func_0200670a_a();
void Func_02006712();
void Func_02006712_a();
void Func_02006712_b();
void Func_02006718();
s32 Func_02006758();
void Func_0200675c();
void Func_02006768();
void Func_02006772();
void Func_02006780();
void Func_02006786();
void Func_02006788();
void Func_0200678e();
void Func_02006794();
void Func_0200679a();
void Func_020067a0();
void Func_020067a2();
void Func_020067a4();
void Func_020067a6();
void Func_020067b4();
void Func_020067ba();
void Func_020067ba_a();
void Func_020067c2();
void Func_020067c8();
void Func_020067d8();
void Func_020067da();
void Func_020067dc();
void Func_020067e8();
void Func_020067ee();
void Func_02006840_a();
void Func_0200686c();
void Func_0200c56d();
s32 Func_02006862(s32);
void Func_0200688c(void);
void Func_02006928(s32, s32, s32);
void Func_020069a4(s32, s32, s32);
void Func_02006944(s32, s32);
u8 *Func_020068e2(s32);
s32 Func_02006810(void);
s32 Func_020067f6(s32, s32);
void Func_02006916(s32, u8 *);
void Func_0200696a(s32, s32, s32);
void Func_020069e6(s32, s32, s32);
void Func_02006982(s32, s32, s32);
void Func_020069fe(s32, s32, s32);
void Func_0200699e(s32, s32);
u8 *Func_0200693c(s32);
s32 Func_0200686a(void);
s32 Func_02006850(s32, s32);
void Func_0200696e(s32, u8 *);
void Func_020069c2(s32, s32, s32);
void Func_02006a3e(s32, s32, s32);
void Func_020069de(s32, s32);
u8 *Func_0200697c(s32);
s32 Func_020068aa(void);
s32 Func_02006890(s32, s32);
void Func_020069ae(s32, u8 *);
void Func_02006a02(s32, s32, s32);
u8 *Func_02006a02_b(s32);
void Func_02006a7e(s32, s32, s32);
void Func_02006a1a(s32, s32, s32);
void Func_02006a96(s32, s32, s32);
s32 Func_020069cc(s32);
void Func_0200695a(s32, s32);
void Func_020068e8(s32);
void Func_02006a86(s32);
void Func_02006a9e(s32, s32);
void Func_02006ad2(s32, s32, s32);
void Func_02006a4c(s32, s32, s32);
void Func_02006a70(s32, s32, s32);
void Func_02006a6a(s32, s32, s32);
void Func_02006abc(s32, s32, s32);
void Func_020069f2(s32);
void Func_02006b06(s32, s32, s32);
void Func_02006b06_b(s32, s32);
void Func_02006b12(s32, s32, s32);
u8 *Func_02006b12_b(s32);
void Func_02006b3e(s32, s32);
void Func_02006b58(s32, s32, s32, s32);
void Func_02006b64(void);
void Func_02006a32(s32);
void Func_02006afa(s32, s32);
void Func_02006b3c(s32, s32, s32);
void Func_02006b0c(s32, s32);
void Func_02006b5e(s32, s32, s32);
void Func_02006b60(s32, s32, s32);
void Func_02006b7c(s32, s32, s32);
void Func_02006ba8(s32, s32);
void Func_02006bc0(s32, s32, s32, s32);
void Func_02006bcc(void);
void Func_02006a9a(s32);
void Func_02006c60(s32);
void Func_02006b5a(s32, s32, s32);
void Func_02006b5a_a(s32, s32, s32);
void Func_02006b52(s32, s32);
void Func_02006ab8(s32);
void Func_02006ac8(s32);
void Func_02006c84(void);
void Func_02006c84_a(void);
void Func_02006b74(s32, s32);
void Func_02006b8a(s32, s32);
u8 *Func_02006b28(s32);
void Func_02006b9c(s32, s32);
u8 *Func_02006b3a(s32);
void Func_02006bc6(s32, s32);
void Func_02006bce(s32, s32);
void Func_02006bd6(s32, s32);
void Func_02006bde(s32, s32);
void Func_02006bee(s32, s32);
void Func_02006c5e(s32, s32);
void Func_02006c78(s32, s32, s32, s32);
void Func_02006c76(s32, s32);
void Func_02006c80(s32, s32);
void Func_02006b66(s32);
void Func_02006c2e(s32, s32);
void Func_02006c2e_a(s32, s32);
void Func_02006c2e_b(s32);
void Func_02006c6e(s32, s32);
void Func_02006c38(s32, s32, s32);
void Func_02006c02(s32, s32, s32);
void Func_02006c02_b(s32);
void Func_02006c4c(s32, s32, s32);
void Func_02006c16(s32, s32, s32);
u8 *Func_02006c16_b(s32);
void Func_02006bac(s32);
void Func_02006cec(s32, s32, s32, s32);
void Func_02006cf8(void);
void Func_02006c64(s32, s32, s32);
void Func_02006b1a(s32);
void Func_02006d98(s32);
void Func_02006bea(s32);
void Func_02006c92(s32, s32);
void Func_02006bf8(s32);
void Func_02006db4(void);
void Func_02006d32(s32, s32);
void Func_02006d4c(s32, s32, s32, s32);
void Func_02006d4c_b(s32, s32, s32);
void Func_02006d58(void);
void Func_02006ce8(s32, s32);
void Func_02006d50(s32, s32, s32);
void Func_02006d12(s32, s32, s32);
void Func_02006c48(s32);
void Func_02006d10(s32, s32);
void Func_02006d10_b(s32, s32, s32);
void Func_02006d48(s32, s32);
u8 *Func_02006c96(s32);
u8 *Func_02006cee(s32);
void Func_02006d1a(s32, s32, s32);
void Func_02006d5e(s32, s32, s32);
u8 *Func_02006d24(s32);
u8 *Func_02006d34(s32);
void Func_02006e18(s32, s32, s32);
void Func_02006e24(s32, s32, s32);
void Func_02006dc4(s32, s32);
void Func_02006e26(s32, s32, s32);
void Func_02006e42(s32, s32, s32);
void Func_02006e4c(s32, s32, s32);
void Func_02006dec(s32, s32);
void Func_02006e4e(s32, s32, s32);
void Func_02006e78(s32, s32, s32);
void Func_02006e82(s32, s32, s32);
void Func_02006e8e(s32, s32, s32);
void Func_02006e8a(s32, s32, s32);
void Func_02006e96(s32, s32, s32);
void Func_02006ea0(s32, s32, s32);
void Func_02006eac(s32, s32, s32);
void Func_02006ea6(s32, s32, s32);
void Func_02006ea6_b(s32, s32);
void Func_02006ec2(s32, s32, s32);
void Func_02006ece(s32, s32, s32);
void Func_02006eda(s32, s32, s32);
void Func_02006ee6(s32, s32, s32);
void Func_02006ee8(s32, s32, s32);
void Func_02006e98(s32, s32);
void Func_02006efa(s32, s32, s32);
void Func_02006f24(s32, s32, s32);
void Func_02006f2e(s32, s32, s32);
void Func_02006f38(s32, s32, s32);
void Func_02006f42(s32, s32, s32);
void Func_02006f3c(s32, s32, s32);
void Func_02006f0e(s32, s32, s32);
void Func_02006e44(s32);
void Func_02006f56(s32, s32, s32);
void Func_02006f60(s32, s32, s32);
void Func_02006f6c(s32, s32, s32);
void Func_02006f78(s32, s32, s32);
void Func_02006f18(s32, s32);
void Func_02006f28(s32, s32);
void Func_02006f5a(s32, s32, s32);
void Func_02006e90(s32);
void Func_02006f58(s32, s32);
void Func_02006f9a(s32, s32, s32);
void Func_02006f52(s32, s32);
void Func_02006fbe(s32, s32, s32);
void Func_02006fbe_b(s32, s32);
void Func_02006f76(s32, s32);
void Func_02006fc0(s32, s32, s32);
void Func_02006fda(s32, s32, s32);
void Func_02006fe4(s32, s32, s32);
void Func_02006fe4_b(s32, s32);
void Func_02006fa4(s32, s32);
void Func_02006ff8(s32, s32, s32);
void Func_02007004(s32, s32, s32);
void Func_02006fac(s32, s32);
void Func_02007006(s32, s32, s32);
void Func_02006f1c(s32);
void Func_02006fd4(s32, s32);
void Func_0200702e(s32, s32, s32);
void Func_0200704a(s32, s32, s32);
void Func_02006ff2(s32, s32);
void Func_02006f50(s32);
void Func_02007018(s32, s32);
void Func_0200705a(s32, s32, s32);
void Func_02005116(void);
void Func_02006f5c(s32);
void Func_02006f80(void);
void Func_0200828a();
void Func_020082d4();
void Func_020082e8();
void Func_02008300();
void Func_02008306();
void Func_0200830c();
void Func_02008312();
void Func_02008318();
void Func_0200831e();
void Func_0200833a();
void Func_02008356();
void Func_0200835c();
void Func_02008362();
void Func_02008368();
void Func_0200836e();
void Func_02008374();
void Func_020083a0();
void Func_020084f6();
void Func_02008500();
void Func_0200850e();
void Func_02008514();
void Func_02008522();
s32 Func_0200853e();
void Func_02008558();
void Func_02008566();
void Func_02008574();
s32 Func_0200857a();
s32 Func_02008582();
void Func_0200859c();
void Func_020085c0();
void Func_020085f6();
s32 Func_02008602();
void Func_0200861e();
void Func_02008622();
void Func_02008624();
void Func_0200862c();
void Func_0200862e();
void Func_0200862e_a();
void Func_0200864c();
void Func_02008656();
void Func_0200865a();
void Func_0200865c();
void Func_0200866a();
void Func_02008678();
void Func_02008682();
void Func_02008686();
void Func_02008690();
void Func_0200869a();
void Func_0200869c();
void Func_020086ae();
void Func_020086c8();
s32 Func_020086ca();
void Func_020086dc();
void Func_020086dc_a();
void Func_020086de();
void Func_020086e6();
void Func_020086e6_a();
void Func_020086fe();
void Func_02008706();
void Func_02008722();
void Func_02008724();
void Func_02008762();
void Func_0200861e_a();
void Func_02008656_a();
void Func_02008676();
void Func_020086b0();
void Func_02008720();
u8 *Func_02008728();
void Func_02008752();
void Func_0200875c();
void Func_02008776();
void Func_020087a0();
void Func_020087b0();
void Func_020087c0();
void Func_020087ea();
void Func_020087ec();
void Func_020087f2();
void Func_020087f4();
void Func_02008802();
void Func_0200880c();
void Func_02008836();
void Func_02008838();
void Func_0200883e();
void Func_02008842();
void Func_0200885c();
void Func_02008862();
void Func_02008878();
void Func_02008878_a();
void Func_0200887a();
void Func_02008884();
void Func_02008896();
void Func_020087a8();
void Func_020087be();
void Func_020087d0();
void Func_020087e4();
void Func_020087f6();
void Func_0200880a();
void Func_0200881c();
void Func_0200882c();
void Func_02008858();
void Func_0200886c();
void Func_02008880();
void Func_02008892();
void Func_020088a4();
void Func_020088a6();
void Func_020088b6();
void Func_020088ca();
void Func_02008c9e(s32);
void Func_0200887a_a(void);
void Func_02008ca8(s32);
void Func_02008884_a(void);
void Func_02008cb2(s32);
void Func_0200888e(void);
void Func_02008cc4(s32);
void Func_020088a0(void);
void Func_02008d18(void);
void Func_02008cf8(s32, s32);
void Func_02008d2c(void);
void Func_02006fea();
void Func_02007002();
s32 Func_02007008();
s32 Func_02007020();
s32 Func_02007022();
s32 Func_02007038();
s32 Func_0200703a();
s32 Func_02007052();
void Func_02007058();
void Func_02007070();
void Func_0200707a();
void Func_0200707c();
void Func_02007084();
s32 Func_0200708a();
void Func_0200708c();
void Func_02007094();
void Func_020070a4();
void Func_020070aa();
void Func_020070ac();
void Func_020070b8();
void Func_020070c4();
void Func_020070cc();
void Func_020070d0();
void Func_020070dc();
void Func_020070ea();
s32 Func_020070f4();
void Func_020070f6();
void Func_020070fe();
void Func_02007104();
s32 Func_0200710c();
s32 Func_02007124();
s32 Func_02007124_a();
void Func_0200712c();
void Func_02007136();
void Func_0200713a();
void Func_02007146();
void Func_0200714e();
void Func_0200714e_a();
void Func_02007156();
void Func_02007158();
s32 Func_02007160();
void Func_02007164();
void Func_02007168();
void Func_02007176();
void Func_02007176_a();
void Func_02007184();
void Func_0200718e();
void Func_0200719a();
void Func_020071a4();
void Func_020071a6();
void Func_020071c0();
void Func_020071ce();
void Func_020071de();
void Func_020071f2();
void Func_020071f4();
void Func_02007208();
void Func_02007232();
s32 Func_02007236();
void Func_0200723e();
void Func_02007266();
void Func_0200726e();
void Func_02007270();
void Func_0200727e();
void Func_02007280();
void Func_0200728c();
s32 Func_020072aa();
void Func_020072ae();
void Func_020072c0();
void Func_020072e0();
void Func_020072ea();
void Func_020072f6();
s32 Func_02007304();
void Func_02007314();
void Func_0200731a();
void Func_02007320();
void Func_02007320_a();
void Func_02007328();
void Func_02007328_a();
void Func_0200732e();
void Func_0200732e_a();
void Func_02007340();
void Func_02007344();
void Func_0200734a();
void Func_0200734a_a();
s32 Func_02007350();
void Func_02007354();
void Func_02007356();
void Func_02007356_a();
void Func_02007356_b();
void Func_02007364();
void Func_0200736c();
void Func_0200737a();
void Func_0200737a_a();
void Func_0200737c();
void Func_02007388();
void Func_02007394();
void Func_0200739e();
void Func_020073a4();
void Func_020073a4_a();
void Func_020073ac();
s32 Func_020073b4();
void Func_020073b8();
void Func_020073bc();
s32 Func_020073c0();
void Func_020073c6();
void Func_020073d4();
void Func_020073d8();
void Func_020073e8();
void Func_02007404();
void Func_0200740a();
void Func_0200740c();
void Func_0200741c();
void Func_0200741e();
void Func_02007422();
void Func_0200742e();
void Func_02007430();
void Func_02007436();
void Func_0200743e();
void Func_02007442();
void Func_02007448();
void Func_02007428(void);
void Func_020074c8(s32, s32);
void Func_0200742e_a(s32);
void Func_02007508(s32, s32, s32);
void Func_02007508_b(s32, u8 *);
s32 Func_02007426(s32);
void Func_02007528(s32);
void Func_0200754a(s32, s32, s32);
void Func_0200753a(s32);
void Func_0200751a(s32, s32);
void Func_02007564(s32, s32, s32);
void Func_0200752c(s32, s32);
void Func_02007576(s32, s32, s32);
void Func_02007592(s32, s32, s32);
void Func_02007532(s32, s32);
void Func_02007498(s32);
u8 *Func_020074d6(s32);
s32 Func_02007404_a(void);
s32 Func_020073ea(s32, s32);
void Func_020074be(s32);
void Func_020074b4(s32);
void Func_020074d8(void);
void Func_02008a10(Obj *);
s32 Func_020089f8(s32);
void Func_02008e00(void);

/* Resolved engine calls: each pseudo symbol is the per-site call word the
 * overlay image holds (a word can serve two sites with different targets),
 * and the macro names the engine function the site reaches through the
 * overlay veneer and the main-image veneer island, keeping the site's own
 * calling form. Names without a repository binding are provisional.
 */

/* Loader-relocated overlay calls: each symbol names the pre-relocation call
 * word the image holds. */

/* Loader-relocated overlay calls: each symbol names the pre-relocation call
 * word the image holds. */

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

static __inline__ void Call3(void (*f)(), s32 a0, s32 a1, s32 a2)
{
    f(a0, a1, a2);
}

/* The scene step counter at 0x1d8 of the shared scene work record. */
static __inline__ void bump_step(s32 amount)
{
    u8 *work = *(u8 **)Data_03001ebc;

    *(u16 *)(work + 0x1d8) = (u16)(*(u16 *)(work + 0x1d8) + amount);
}

/* Call sites spelled through these wrappers pass their constants straight
 * into the argument registers; a direct call precomputes a costly constant
 * into a pseudo that the compiler then shares with later uses in the block.
 * A value-returning call also sets r0 last of its arguments. */
static __inline__ void Call3_02000398(void (*f)(), s32 a0, s32 a1, s32 a2)
{
    f(a0, a1, a2);
}

/* Call sites spelled through these wrappers pass their constants straight
 * into the argument registers; a direct call precomputes a costly constant
 * into a pseudo that the compiler then shares with later uses in the block.
 * A value-returning call also sets r0 last of its arguments. */
static __inline__ void Call3_020003cc(void (*f)(), s32 a0, s32 a1, s32 a2)
{
    f(a0, a1, a2);
}

/* Call sites spelled through these wrappers pass their constants straight
 * into the argument registers; a direct call precomputes a costly constant
 * into a pseudo that the compiler then shares with later uses in the block.
 * A value-returning call also sets r0 last of its arguments. */
static __inline__ void Call3_02000400(void (*f)(), s32 a0, s32 a1, s32 a2)
{
    f(a0, a1, a2);
}

static __inline__ void Call2(void (*f)(), s32 a0, s32 a1)
{
    f(a0, a1);
}

static __inline__ s32 Value2(s32 (*f)(), s32 a0, s32 a1)
{
    return f(a0, a1);
}

static __inline__ void Call4(void (*f)(), s32 a0, s32 a1, s32 a2, s32 a3)
{
    f(a0, a1, a2, a3);
}

/* Call sites spelled through these wrappers pass their constants straight
 * into the argument registers; a direct call precomputes a costly constant
 * into a pseudo that the compiler then shares with later uses in the block.
 * A value-returning call also sets r0 last of its arguments. */
static __inline__ void Call6(void (*f)(), s32 a0, s32 a1, s32 a2, s32 a3, s32 a4, s32 a5)
{
    f(a0, a1, a2, a3, a4, a5);
}

/* Loader-relocated overlay calls: each symbol names the pre-relocation call
 * word the image holds. */

/* Resolved engine calls: each pseudo symbol is the per-site call word the
 * overlay image holds (a word can serve two sites with different targets),
 * and the macro names the engine function the site reaches through the
 * overlay veneer and the main-image veneer island, keeping the site's own
 * calling form. Names without a repository binding are provisional.
 */

/* Configures actor 22 (position, pose, and movement/sprite flags) for the
 * scene. */

/* Call sites spelled through these wrappers pass their constants straight
 * into the argument registers; a direct call precomputes a costly constant
 * into a pseudo that the compiler then shares with later uses in the block.
 * A value-returning call also sets r0 last of its arguments. */
static __inline__ void Call1_02001828(void (*f)(), s32 a0)
{
    extern u8 Data_0200d7fc[];

    f(a0);
}

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
static __inline__ void Call6_02001828(void (*f)(), s32 a0, s32 a1, s32 a2, s32 a3, s32 a4, s32 a5)
{
    extern u8 Data_0200d7fc[];

    f(a0, a1, a2, a3, a4, a5);
}

/* Call sites spelled through these wrappers pass their constants straight
 * into the argument registers; a direct call precomputes a costly constant
 * into a pseudo that the compiler then shares with later uses in the block.
 * A value-returning call also sets r0 last of its arguments. */
static __inline__ s32 Value1_020028a4(s32 (*f)(), s32 a0)
{
    return f(a0);
}

s32 OverlayObject_SetField6OnCountdown(struct Object *object)
{
    s32 loaded = object->counter;
    s32 counter = (s16)loaded;

    if (loaded == 0) {
        object->x = Func_02004698();
        counter = Func_02004684(Func_0200469e(), 20) + 20;
        object->counter = counter;
    }
    object->counter = counter - 1;
    return 1;
}

s32 UpdateFixedPointCountdown(struct FixedPointCountdown *state)
{
    switch (state->countdown) {
    case 6:
        state->fixed_point_18 += (s32) 0xFFFFC000;
        state->fixed_point_1c += 0x2000;
        break;
    case 4:
        state->fixed_point_18 += 0x2000;
        state->fixed_point_1c += -0x1000;
        break;
    case 2:
        state->fixed_point_18 += 0x1000;
        state->fixed_point_1c += (s32) 0xFFFFF800;
        break;
    case 0:
        state->fixed_point_18 = 0x10000;
        state->fixed_point_1c = 0x10000;
        state->countdown = (s16)(UnsignedRemainder(NextRandomValue(), 90) + 60);
        break;
    }
    state->countdown--;
    return 1;
}

u8 *SceneData_GetTableD0E4(void)
{
    return Data_0200d0e4;
}

s32 SceneData_ReturnZero(void)
{
    return 0;
}

u8 *SceneData_GetTableD27c(void)
{
    return Data_0200d27c;
}

u8 *SceneData_GetTableD2B8(void)
{
    return Data_0200d2b8;
}

u8 *SceneData_GetTableD558(void)
{
    return Data_0200d558;
}

void SceneState_SetFlag210AndConfigureRegion40_84(void)
{
    s32 a;
    s32 b;

    Func_020047fa(0x210);
    a = 10;
    b = 84;
    Func_020047d6(40, 84, 7, 4, a, b);
}

void SceneState_SetFlag210AndConfigureRegion40_89(void)
{
    void Func_02000360_r(s32);

    s32 a;
    s32 b;

    Func_0200482a(0x210);
    a = 10;
    b = 84;
    Func_020047fe(40, 89, 7, 4, a, b);
}

void SceneState_SetWork1c0AndRunObject(u8 *o)
{
    void Func_02000360_r(s32);

    u8 *state;

    if (Func_02004840((s32)&Value_00000834) != 0) {
        Func_020049c8();
    }
    state = *(u8 **)0x03001ebc;
    *(s32 *)(state + 0x1C0) = 0x100;
    *(s32 *)(state + 0x1C8) = 16;
    Func_020049c4_a(o);
}

void FieldScene_SetupDescriptorD774(void)
{
    Func_02004a52(0x9E);
    Func_02004834(Data_0200d774, 45, 11);
    Func_02004928(0, (s32)&Value_00000101, 0x1A4);
    Func_020048ae(3);
    Func_0200031c(11);
}

void SceneState_SetValue123Mode1(void)
{
    void Func_02000360_r(s32);

    Func_02004a8a(0x7B);
    Func_02000338(1);
}

void SceneState_ApplyValues123And3(void)
{
    void Func_02000360_r(s32);

    Func_02004a9e(0x7B);
    Func_0200034c(3);
}

void SceneState_SetValue123Mode4(void)
{
    void Func_02000360_r(s32);

    Func_02004ab2(0x7B);
    Func_02000360_r(4);
}

void FieldScene_RunStep7BAndCheckFlags841And842(void)
{
    Func_02004ac6(0x7B);
    if (Func_020048f4((s32)&Value_00000841) != 0
        && Func_020048fe((s32)&Value_00000842) == 0) {
        Func_020041da();
    }
    Func_0200038c(2);
}

void FieldScene_SetupDescriptorD78a(void)
{
    Func_02004afa(0x9E);
    Func_020048dc(Data_0200d78a, 54, 32);
    Func_020049d0(0, 0x196, (s32)&Value_000002d7);
    Func_02004956(3);
    Func_020003c4(5);
}

void FieldScene_RunScene372_02000278(void)
{
    u32 i;
    s32 record;

    if (Value1(Func_0200495a, 0x206) == 0) {
        Func_02004b3c(158);
        Call3(Func_0200491e, 0x200d7a0, 45, 39);
    }
    if (Value1(Func_02004974, 0x835) == 0) {
        record = Value1(Func_0200497e, 0x831);
        if (record != 0) {
            goto L_020002b4;
        }
        Func_020017b6();
        Call1(Func_02004994, 0x206);
    } else {
        L_020002b4:;
        Call3(Func_02004a32, 0, 0x106, 0x325);
        Func_020049b8(3);
        Func_02000426(6);
    }
}

void FieldScene_SetupDescriptorD78aIfFlag205Clear(void)
{
    if (Func_020049c6((s32)&Value_00000205) == 0) {
        Func_02004ba8(0x9E);
        Func_0200498a(Data_0200d78a, 50, 44);
    }
    Func_02004a80(0, 0x154, 0x378);
    Func_02004a06(3);
    Func_02000474(7);
}

void FieldScene_SetupWithDescriptorD7A0(void)
{
    Func_02004be2(158);
    Func_020049c4((void *)0x0200d7a0, 49, 69);
    Func_02004ab8(0, 0x146, 0x466);
    Func_02004a3e(3);
    Func_020004ac(8);
}

void FieldScene_SetupDescriptorD7b6(void)
{
    Func_02004c1a(158);
    Func_020049fc((void *)0x0200d7b6, 52, 76);
    Func_02004af0(0, 0x176, 0x4d6);
    Func_02004a76(3);
    Func_020004e4(9);
}

void FieldScene_RunScene372_02000398(void)
{
    u32 i;
    s32 record;

    Func_02004c52(158);
    Call3_02000398(Func_02004a34, 0x200d78a, 35, 74);
    Call3_02000398(Func_02004b26, 0, 102, 0x4b6);
    Func_02004aac(3);
    Func_0200051a(10);
}

void FieldScene_RunScene372_020003cc(void)
{
    u32 i;
    s32 record;

    Func_02004c86_a(158);
    Call3_020003cc(Func_02004a68, 0x200d78a, 35, 73);
    Call3_020003cc(Func_02004b5a, 0, 102, 0x4b6);
    Func_02004ae0(3);
    Func_0200054e(12);
}

void FieldScene_RunScene372_02000400(void)
{
    u32 i;
    s32 record;

    Func_02004cba(158);
    Call3_02000400(Func_02004a9c, 0x200d7a0, 38, 72);
    Call3_02000400(Func_02004b8e, 0, 146, 0x49e);
    Func_02004b14(3);
    Func_02000582(13);
}

s32 FieldScene_RunFlagGatedActorSetup(void)
{
    s32 m;
    s32 t;
    s32 m2;
    s32 h;
    s32 k;
    s32 w1 = 0x14E0000;
    s32 w2 = 0x3A40000;
    s32 w3 = 0xE00000;
    s32 w4 = 0x3680000;
    s32 w5 = 0x400000;
    s32 w6 = 0x1B00000;
    s32 w7 = 0x720000;
    s32 w8 = 0xC00000;
    s32 w9 = 0x2000;
    s32 w10 = 0xE30000;
    s32 w11 = 0x4000;
    s32 w12 = 0xF70000;
    s32 w13 = 0x4000;
    s32 w14 = 0xF30000;
    s32 w15 = 0x1900000;
    s32 w16 = 0x1A80000;
    s32 w17 = 0x190;
    s32 w18 = 0x1A8;
    s32 w19 = 0x1A80000;
    s32 w20 = 0x1A8;
    s32 b1 = 0x4BE0000;
    s32 b2 = 0x4BE0000;
    s32 b3 = 0x4BE0000;
    s32 w21 = 0xA50000;
    s32 w22 = 0xA50000;
    s32 p1 = 0x2BF0000;
    s32 p2 = 0x47B0000;
    s32 p3 = 0x14D0000;
    s32 p4 = 0x4FD0000;
    s32 p5 = 0x2630000;
    s32 p6 = 0x2730000;
    s32 p7 = 0x2730000;
    s32 c1 = 0x26B;
    s32 c2 = 0x101;
    s32 c3 = 0x26B;

    Func_02004ce4(0xAA);
    Func_02004bd6(23, 0, 0);
    if (Func_02004b2c((s32)&Value_00000109) != 0) {
        Func_02004b46((s32)&Value_00000205);
        Func_02004b4c((s32)&Value_00000206);
    }
    if (Func_02004b44(0x830) != 0) {
        Func_02004c06(11, w1, w2);
        Func_02001342();
    }
    if (Func_02004b60((s32)&Value_00000831) != 0) {
        Func_02004c22_b(12, w3, w4);
        Func_0200153e();
    }
    if (Func_02004b7c((s32)&Value_00000832) != 0) {
        Func_02004c3c(13, w5, p1);
        Func_020017a4();
    }
    if (Func_02004b96((s32)&Value_00000833) != 0) {
        Func_02004c56_b(14, w6, p2);
        Func_02001966();
    }
    {
        u8 *q;
        q = Func_02004c00(11);
        q += 0x59;
        m = 4;
        *q = *q | m;
        q = Func_02004c10(12);
        q += 0x59;
        *q = *q | m;
        q = Func_02004c1e(13);
        q += 0x59;
        *q = *q | m;
        q = Func_02004c2c(14);
        q += 0x59;
        *q = *q | m;
        q = Func_02004c3a(15);
        q += 0x59;
        *q = *q | m;
        q = Func_02004c48(16);
        q += 0x59;
        *q = *q | m;
        q = Func_02004c56(17);
        q += 0x59;
        *q = *q | m;
        q = Func_02004c64(18);
        q += 0x59;
        { u8 tmp = m | *q; *q = tmp; }
    }
    if (Func_02004c22((s32)&Value_00000837) != 0) {
        Func_02004ce0(22, 0, 0);
    }
    {
        u8 *q;
        q = Func_02004c86(19);
        *(s32 *)(q + 0x18) = 0x20000;
        *(s32 *)(q + 0x1C) = 0x20000;
    }
    if (Func_02004c44((s32)&Value_00000838) != 0) {
        Func_02004d04(19, w7, p3);
    } else {
        Func_02004cc6(19, Data_0200c9f4);
    }
    if (Func_02004c64_a((s32)&Value_00000841) != 0) {
        s32 h2;
        u8 *tbl;
        Func_02004682();
        Func_02004d2a(9, w21, 0x4CD0000);
        {
            u8 *o;
            s32 v;
            o = Func_02004cd0(9);
            h2 = 0xE000;
            *(u16 *)(o + 6) = h2;
            v = Func_02004bea(Func_02004c04(), 0x5A) + 60;
            *(u16 *)(o + 0x64) = v;
            tbl = Data_0200cec8;
            o += 0x66;
            t = 1;
            *(u16 *)o = t;
            Func_02004d14(9, tbl);
        }
        Func_02004d68(26, w22, 0x4E60000);
        {
            u8 *o;
            s32 v;
            o = Func_02004d0e(26);
            *(u16 *)(o + 6) = h2;
            v = Func_02004c24(Func_02004c3e(), 0x5A) + 60;
            *(u16 *)(o + 0x64) = v;
            o += 0x66;
            t = 2;
            *(u16 *)o = t;
            Func_02004d4a(26, tbl);
        }
        Func_02004d9e(22, 0x980000, 0x5050000);
        {
            u8 *o;
            s32 v;
            o = Func_02004d44(22);
            *(u16 *)(o + 6) = h2;
            v = Func_02004c5a(Func_02004c74(), 0x5A) + 60;
            *(u16 *)(o + 0x64) = v;
            o += 0x66;
            t = 3;
            *(u16 *)o = t;
            Func_02004d80(22, tbl);
        }
        Func_02004dd6(8, 0xB80000, 0x5180000);
        {
            u8 *o;
            s32 v;
            o = Func_02004d7c(8);
            *(u16 *)(o + 6) = h2;
            v = Func_02004c92(Func_02004cac(), 0x5A) + 60;
            *(u16 *)(o + 0x64) = v;
            o += 0x66;
            t = 4;
            *(u16 *)o = t;
            Func_02004db8(8, tbl);
        }
        Func_02004e10(8, 6);
        {
            u8 *r;
            r = Func_02004dae(22);
            r += 0x23;
            m2 = 0xFE;
            *r = *r & m2;
            r = Func_02004dc0(8);
            r += 0x23;
            *r = m2 & *r;
        }
        Func_02004cea(Data_0200c5b9, 0xC80);
        Func_02004e3c_b(24, 0, 0);
        Func_02004e46(25, 0, 0);
        Func_02004e50(23, 0, 0);
        Func_02004e5a(19, 0, 0);
        if (Func_02004db0((s32)&Value_00000842) != 0) {
            Func_02004f70(22, 0, 0);
        }
    } else if (Func_02004dc0_a((s32)&Value_0000083a) != 0) {
        u8 *tbl;
        Func_02004e82(10, w8, b1);
        Func_02004efe(10, w9, 0);
        Func_02004e9e(10, 5);
        {
            u8 *o;
            s32 v;
            o = Func_02004e3c(10);
            v = Func_02004d50(Func_02004d6a(), 0x5A) + 60;
            *(u16 *)(o + 0x64) = v;
        }
        tbl = Data_0200cec8;
        Func_02004e70(10, tbl);
        Func_02004ec4(24, w10, b2);
        Func_02004f40(24, w11, 0);
        Func_02004ee0(24, 6);
        {
            u8 *o;
            s32 v;
            o = Func_02004e7e(24);
            v = Func_02004d92(Func_02004dac(), 0x5A) + 60;
            *(u16 *)(o + 0x64) = v;
        }
        Func_02004eb0(24, tbl);
        Func_02004f04(25, w12, b3);
        Func_02004f80(25, w13, 0);
        Func_02004f20(25, 6);
        {
            u8 *o;
            s32 v;
            o = Func_02004ebe(25);
            v = Func_02004dd2(Func_02004dec(), 0x5A) + 60;
            *(u16 *)(o + 0x64) = v;
        }
        Func_02004ef0(25, tbl);
        Func_02004f44_b(23, w14, p4);
        Func_02004fc0(23, 0xC000, 0);
        Func_02004e84(Func_02004ef6(23), 0);
        Func_02004f66(17, 0, 0);
        Func_02004f70(18, 0, 0);
    } else {
        Func_02004fd4(17, 0, 0);
        Func_02004fde(18, 0, 0);
    }
    {
        s16 *table = (s16 *)Data_02000240;
        if (table[225] != 15 || Func_02004f44((s32)&Value_0000087b) != 0) {
            Func_020050bc();
            Func_020050c8();
        }
    }
    if (Func_02004f58(0x210) != 0) {
        Func_02000990();
    }
    Func_02004f6e((s32)&Value_00000834);
    k = 46;
    Func_02004f4a_b(29, 24, 1, 2, 26, k);
    Func_02004f5c(29, 25, 1, 1, 27, k);
    Func_02004f6e_b(29, 25, 1, 1, 28, k);
    k = 20;
    Func_02004f82(19, 0x5A, 1, 1, k, 0x58);
    Func_02004f94(19, 0x5A, 1, 1, k, 0x59);
    {
        u8 *o;
        u8 *q;
        o = Func_0200501a(21);
        q = o + 0x55;
        *q = 0;
        *(s32 *)(o + 0xC) = 0xC00000;
        q += 4;
        *q = 8;
        Func_02004fbc(o, 0);
    }
    Func_02004f4a(1);
    if (Func_02004ff0((s32)&Value_0000087b) == 0) {
        s16 *table = (s16 *)Data_02000240;
        if (table[225] == 15) {
            Func_0200133c();
            return 0;
        }
    }
    Func_020050ca(23, 7);
    if (Func_02005018((s32)&Value_00000837) == 0) {
        Func_02005040();
        Func_02005160(22, c2);
        Func_020050e4(22, w15, p5);
        Func_020050f0(21, w16, p6);
        Func_020050e4_a(22, w17, c1);
        Func_020050f8(21, w18, 0x26B);
        Func_02005118(21, 2);
        Func_02005120(22, 5);
        Func_02005094();
    } else {
        Func_02005092();
        Func_0200512e(21, w19, p7);
        Func_0200512a(21, w20, c3);
        Func_0200514a(21, 3);
        Func_020050be();
    }
    {
        u8 *state = *(u8 **)0x03001EBC;
        *(s32 *)(state + 0x1C0) = 0x100;
        *(s32 *)(state + 0x1C8) = 24;
    }
    Func_02005258();
    Func_02005264();
    Func_02005248();
    return 0;
}

void FieldScene_RunScene372_02000a10(void)
{
    u32 i;
    s32 record;
    u8 *work;
    s32 base5_e5c;

    Func_02005110();
    Func_02005264_a();
    Func_02005270();
    Func_02005284();
    Func_02005062(60);
    Call2(Func_0200524e, 0x4000, 0x800);
    Call4(Func_02005268, 0x13c0000, 0xa00000, 0x3700000, 1);
    Call3(Func_020051de, 10, 0x1260000, 0x3640000);
    Func_020051e8(0, 0, 0);
    work = *(u8 *volatile *)Data_03001ebc;
    *(volatile s32 *)(((s32)work + 0x1c0)) = 0x100;
    *(volatile s32 *)(((s32)work + 0x1c8)) = 16;
    Func_020052fa();
    Func_02005306();
    Func_020052ea();
    Func_02005338(158);
    Call3(Func_0200511a, 0x200d78a, 50, 44);
    Call2(Func_020052aa, 22, 0x101);
    Call3(Func_020051dc, 9, 0xcccc, 0x6666);
    Call3(Func_020051e6, 0, 0xcccc, 0x6666);
    Call3(Func_020051f0, 10, 0xcccc, 0x6666);
    Call3(Func_0200524c, 9, 0x1560000, 0x37a0000);
    Call3(Func_02005248_a, 9, 0x156, 0x389);
    Func_0200537c();
    Call3(Func_02005250, 9, 0x128, 0x389);
    Call3(Func_02005274, 0, 0x1560000, 0x37a0000);
    Call3(Func_02005268_a, 0, 0x156, 0x37a);
    Call3(Func_0200527c, 0, 0x156, 0x389);
    Call3(Func_02005288, 0, 0x13e, 0x389);
    Func_020052a8(9, 1);
    Func_020052d0(9, 1);
    Call3(Func_02005324, 9, 0xc000, 60);
    base5_e5c = (s32)Data_00000e5c;
    Func_02005304(base5_e5c);
    Func_0200531c(9, 0);
    Call3(Func_020052c0, 10, 0x126, 0x346);
    Func_0200523e(40);
    Func_020052ee(10, 4);
    Func_0200533e(10, 0);
    Func_02005328(0, 9, 0);
    Func_0200525e(40);
    Call3(Func_02005372, 10, 0x4000, 0);
    Func_0200536c(10, 0, 20);
    Call3(Func_02005396, 9, 0x101, 20);
    Call3(Func_02005392, 9, 0xc000, 10);
    Func_0200538c(9, 0, 10);
    Func_02005344(10, 4);
    Func_02005394(10, 0);
    Call2(Func_020053ce, 9, 0x102);
    Func_020052b4(30);
    Func_020053c6(9, 0, 50);
    Call3(Func_020053d2, 9, 0xc000, 10);
    Call3(Func_02005320, 9, 0x18000, 0xc000);
    Call3(Func_0200536a, 9, 0x121, 0x373);
    Call3(Func_020053f6, 9, 0xe000, 0);
    Func_020053e6(9, 0);
    Func_020053be(10, 2);
    Func_020053f6_a(10, 0);
    Func_020053b6(9, 4);
    Func_02005410(9, 0, 10);
    Call3(Func_0200542e, 9, 0x2000, 10);
    Func_0200540c((base5_e5c + 8));
    Value2(Func_0200541c, 9, 0);
    Call3(Func_020053c8, 0, 0x12e, 0x389);
    Call3(Func_02005454, 0, 0xc000, 0);
    while (Value2(Func_0200539c, 0, 0) == 1) {
        Func_02005416(9, 1);
        Call1(Func_0200543c, 0xe65);
        Func_0200544c(9, 0);
    }
    Func_02005420(9, 3);
    Call1(Func_0200545e, 0xe66);
    Func_02005480(9, 0, 10);
    Func_02005438(0, 3);
    Call3(Func_020053e6_a, 10, 0x18000, 0xc000);
    Call3(Func_02005428, 10, 0x129, 0x2ee);
    Func_020053ae(10);
    Call3(Func_02005440, 9, 0x129, 0x2ee);
    Func_0200545a(9, 0, 0);
    Func_02005464(10, 0, 0);
    Func_02005474(10, 1);
    Func_0200547c(21, 2);
    Func_02005484(22, 5);
    Call1(Func_020053e2, 0x12f);
    Call1(Func_020053e0, 0x87b);
    Call1(Func_020053e6_b, 0x205);
    Func_0200540a();
}

void FieldScene_RunScene372_02000ec4(void)
{
    u32 i;
    s32 record;

    Call6(Func_0200558c, 29, 64, 1, 1, 21, 57);
    Call6(Func_0200559e, 29, 64, 1, 1, 21, 58);
    Call6(Func_020055b0, 29, 64, 1, 1, 22, 58);
    Call6(Func_020055c2, 29, 64, 1, 1, 20, 58);
    Call6(Func_020055d4, 28, 20, 1, 1, 20, 57);
}

void ActorPresentation_SetEightSceneCells(void)
{
    extern s32 Data_0200d7fc;

    s32 a = 15;
    s32 d = 0x35;
    s32 e;
    s32 b;
    s32 c;
    s32 f;

    Func_0200576e(29, 23, 1, 1, a, d);
    b = 14;
    Func_02005780(29, 23, 1, 1, b, d);
    c = 13;
    Func_02005794(29, 23, 1, 1, c, d);
    Func_020057a6(26, 20, 2, 1, b, 0x34);
    e = 0x36;
    Func_020057ba(25, 21, 1, 1, c, e);
    Func_020057cc(25, 21, 1, 1, a, e);
    Func_020057dc(14, 0x35, 1, 1, b, e);
    f = 0x37;
    Func_020057f0(13, 0x37, 1, 1, a, f);
}

void FieldScene_RunOpeningAuxiliarySequence(void)
{
    u32 i;
    s32 p10;
    s32 p10b;
    s32 rec8;
    u8 *record;
    s32 r8;
    s32 v5;
    u8 *p5;
    u8 *later;

    if (Value1(Func_0200583c, 0x312) != 0) {
        v5 = r8;
    } else {
        Func_02005866();
        v5 = r8;
        if (Value1(Func_0200584c, 0x832) == 0) {
            rec8 = Value1(Func_020058a6, 13);
            record = Func_020058ae(0);
            p5 = (u8 *)(s32)*(volatile u8 *)(*(volatile s32 *)((s32)record + 80) + 9);
            record += 35;
            p10 = *record;
            Call3(Func_0200585e, 0x40000, 0x40000, 0x10000);
            Func_02005a5c(141);
            Func_020057ea(40);
            Func_02005a68(145);
            Func_020059c0(0, 3);
            {
                u8 *record = Func_020058ee(0);
                u8 value = *(volatile u8 *)&record[35];

                record[35] = (u8)(value | 2);
            }
            Call3(Func_02005962, 13, 0, 0x2bf0000);
            *(volatile s32 *)(rec8 + 48) = 0x18000;
            *(volatile s32 *)(rec8 + 52) = 0x18000;
            {
                s32 shifted = *(volatile s32 *)(rec8 + 12) + 0x500000;

                *(volatile s32 *)(rec8 + 12) = shifted;
                *(volatile s32 *)(rec8 + 60) = shifted;
            }
            *(volatile s32 *)(rec8 + 68) = 0x8000;
            Call3(Func_02005976, 13, 64, 0x2bf);
            Func_020058f4(40);
            Call1(Func_02005aba, 0x121);
            Call3(Func_020058d2, -1, -1, 0xe666);
            Func_020058e0();
            Func_02005acc();
            Call1(Func_0200590a, 0x832);
            Func_02005a32(0, ((u32)(s32)((s32)p5 << 28) >> 30));
            {
                u8 *record = Func_02005960(0);
                u8 value = *(volatile u8 *)&record[35];

                record[35] = (u8)(value | 1);
            }
            *record = p10;
        }
        Func_02002538();
        Call1(Func_02005932, 0x312);
        if (Value1(Func_02005930, 0x837) != 0) {
            if (Value1(Func_0200593a, 0x841) == 0) {
                if (Value1(Func_02005946, 0x30c) == 0) {
                    later = Func_020059a0(0);
                    if (*(volatile s32 *)((s32)later + 16) <= 0x2b4ffff) {
                        Value2(Func_02002ce8, 62, 0x29d);
                        Call3(Func_02005a0a, 0, 27, 0x273);
                    } else {
                        Value2(Func_02002cfc, 75, 0x2cb);
                        Call3(Func_02005a1e, 0, 67, 0x2f5);
                    }
                    Call1(Func_0200598e, 0x30c);
                }
            }
        }
        Func_020059b2_a();
    }
    p10b = v5;
}

void SceneState_ApplyFourRects(void)
{
    void Func_02000360_r(s32);

    s32 a = 0x2a;
    s32 b;

    Func_020059b2(29, 22, 1, 1, 3, a);
    b = 2;
    Func_020059c4(29, 21, 1, 1, b, a);
    Func_020059d6(29, 21, 1, 1, 4, a);
    Func_020059e8(23, 20, 3, 1, b, 0x2b);
}

void FieldScene_DrawFiveTileBlocks(void)
{
    s32 a = 26;
    s32 h = 0x47;
    s32 b;
    s32 a2;

    Func_02005b60(29, 20, 1, 1, a, h);
    b = 0x46;
    Func_02005b72(29, 20, 1, 1, a, b);
    a2 = 27;
    Func_02005b84(29, 20, 1, 1, a2, b);
    Func_02005b98(28, 21, 1, 1, 28, h);
    Func_02005baa(28, 22, 1, 1, a2, 0x48);
}

void FieldScene_RunScene372SequenceA(void)
{
    u32 i;
    s32 record;
    s32 base5_e67;

    Func_02005c0c();
    Call3(Func_02005c98, 0, 0x106, 0x32a);
    Call3(Func_02005cb4, 20, 0x1060000, 0x3250000);
    Call3(Func_02005ca8, 20, 0x106, 0x339);
    Call3(Func_02005c7e, 0, 0x20000, 0x10000);
    Func_02005cf0(0, 2, 0);
    Call3(Func_02005cd4, 0, 0x11a, 0x357);
    Func_02005cf4(20, 1);
    Func_02005d0e(0, 4, 0);
    Func_02005d38(0, 20, 0);
    Func_02005e24();
    Func_02005c72(30);
    Func_02005d3a(0, 2);
    Call3(Func_02005d9e_a, 20, 0x100, 20);
    base5_e67 = (s32)Data_00000e67;
    Func_02005d6e(base5_e67);
    Func_02005d86(20, 0);
    Func_02005c9c(20);
    Value2(Func_02005da4, 20, 0);
    Func_02005d6e_a(20, 2);
    Func_02005d94((base5_e67 + 4));
    Func_02005db6(20, 0, 20);
    Call2(Func_02005d2e, 20, 0x200c8c0);
    Call1(Func_02005cbc, 0x835);
    Func_02005ce0();
}

void FieldScene_RunScene372SequenceB(void)
{
    u32 i;
    s32 record;

    if (Value1(Func_02005ce2, 0x836) == 0) {
        if (Value1(Func_02005cec, 0x837) == 0) {
            Func_02005d14();
            Call1(Func_02005df2, 0xe6c);
            Func_02005e14(22, 0, 20);
            Call3(Func_02005e3e, 0, 0x101, 40);
            Call3(Func_02005dba, 0, 0x17e, 0x26b);
            Func_02005e04(0, 22, 0);
            Func_02005e04_a(0, 2);
            Func_02005d4a(30);
            Func_02005e42(22, 0);
            Call1(Func_02005d48, 0x836);
            Func_02005d6c();
        }
    }
}

void FieldScene_RunActor22SceneWhenFlag836Only(void)
{
    if (Func_02005d66(0x837) == 0 && Func_02005d70(0x836) != 0) {
        Func_02005d98();
        Func_02005e58(22, 2);
        Func_02005d9e(20);
        Func_02005e84(0xe71);
        Func_02002df0();
        Func_02005dbc();
    }
}

void FieldScene_RunScene372SequenceC(void)
{
    u32 i;
    s32 record;

    if (Value1(Func_02005dae, 0x841) != 0) {
        Func_02005dd6();
        Func_02005ea0(22, 0, 0);
        Func_02005dde(20);
        Func_02005ec4((s32)Data_00000ed0);
        Func_02005edc(22, 0);
        Call3(Func_02005f00, 22, 0xe000, 10);
        Func_02005e0c();
    } else {
        if (Value1(Func_02005dec, 0x837) == 0) {
            Func_02005e14_a();
            Call1(Func_02005ef2, 0xe6e);
            Func_02002e5e();
            Func_02005e2a();
        }
    }
}

void FieldScene_RunScene372SequenceE(void)
{
    extern u8 Data_0200d7fc[];

    u32 i;
    s32 record;
    s32 base5_e74;
    s32 v6;

    if (Value1(Func_02005f0a, 0x837) != 0) {
    } else {
        Func_02005f34();
        Call2(Func_02006056, 22, 0x100);
        base5_e74 = (s32)Data_00000e74;
        Func_0200601e(base5_e74);
        Func_02006036(22, 0);
        Call3(Func_0200606a, 0, 0x100, 20);
        Call3(Func_02006066, 0, 0x4000, 0);
        Call2(Func_0200608e, 0x6666, 0xccc);
        Call4(Func_020060a8, 0x1000000, -1, 0x24c0000, 1);
        Call3(Func_02005fce, 22, 0x20000, 0x10000);
        Value2(Func_02005ff6, 22, 0x200c934);
        Func_02006068(0, 22, 0);
        Func_02005f9e(30);
        Value2(Func_02005ff6_a, 22, 0x200c984);
        Func_0200609e(22, 0);
        v6 = 128;
        record = Func_02005fec(22);
        *(volatile s32 *)(record + 28) = (v6 << 9);
        Func_02006082(22, 1);
        Func_02005fc8(20);
        Func_020060d0(22, 0);
        Func_02005fd6(40);
        Func_020060a0(22, 1);
        Func_020060c6((base5_e74 + 5));
        Func_020060e8(22, 0, 20);
        Func_020060a0_a(0, 3);
        Func_020060a8_a(22, 3);
        Func_020060f8(22, 0);
        Call3(Func_0200605c, 22, (v6 << 9), 0x8000);
        Func_020060bc(22, 2);
        record = Value1(Func_0200605a, 0);
        if (record != 0) {
            Func_020060a4(22, *(s16 *)(record + 10), *(s16 *)(record + 18));
        }
        Func_020060ca(22);
        Func_020060dc(22, 0, 0);
        Func_0200606c(1, 1);
        Func_020060f4(21, 3);
        Call1_02001828(Func_0200604a, 0x837);
        Func_0200606e();
    }
}

void FieldScene_RunScene372SequenceD(void)
{
    extern u8 Data_0200d7fc[];

    u32 i;
    s32 record;

    Func_0200608c();
    record = Value1(Func_020060c2, 0);
    if (record != 0) {
        Func_02006130(22, *(volatile s32 *)(record + 8), *(volatile s32 *)(record + 16));
    }
    Call3(Func_020060ee, 22, 0x10000, 0x8000);
    Call3(Func_02006138, 22, 0x119, 0x1fb);
    Func_0200618a(22, 0, 0);
    Func_020060c0(30);
    Call1_02001828(Func_020061a6, 0xe7b);
    Func_020061be(22, 0);
    Func_020061a0(0, 22, 0);
    Func_020060de(10);
    Func_020061a6_a(0, 1);
    Func_020060ec(20);
    Call3(Func_02006200, 22, 0x4000, 0);
    Func_020061f0(22, 0);
    Func_020061a8_a(22, 2);
    record = Value1(Func_02006146, 0);
    if (record != 0) {
        Func_02006190(22, *(s16 *)(record + 10), *(s16 *)(record + 18));
    }
    Func_020061b6(22);
    Func_020061c8(22, 0, 0);
    Call3(Func_020061c4, 0, 0x100, 0x205);
    Func_02006150();
}

void SceneActor_RunActor22PlacementSequence(s32 x, s32 y)
{
    extern s32 Data_0200d7fc;

    Thing1 *a;
    s32 w = 0x10000;
    s32 h = 0x8000;
    Thing2 *b;

    a = Func_0200619a(0);
    if (a != 0) {
        Func_02006208(22, a->unk8, a->unk10);
    }
    Func_020061c6(22, w, h);
    Func_02006210(22, x, y);
    Func_02006262(0, 22, 0);
    Func_02006198(20);
    Func_020062c2(0, 0x102);
    Func_020061a8(40);
    Func_0200628e(0xe7d);
    Func_020062a6(22, 0);
    Func_0200627e(22, 2);
    Func_020062b6(22, 0);
    Func_02006276(0, 3);
    Func_02006276_a(22, 2);
    b = Func_02006214(0);
    if (b != 0) {
        Func_0200625e(22, b->unkA, b->unk12);
    }
    Func_02006284(22);
    Func_02006296(22, 0, 0);
}

void FieldScene_RunReturnScript(void)
{
    extern u8 Data_0200d7fc[];

    u32 i;
    s32 p10;
    s32 p11;
    s32 p9;
    s32 record;
    s32 r9;
    s32 r10;
    s32 r8;
    s32 base5_200d7fc;
    s32 base7_200c56d;
    s32 base_200c5a9;
    s32 shifted;
    s32 v5;
    s32 v6;
    s32 v7;

    if (Value1(Func_02006206, 0x838) != 0) {
        v5 = r9;
        v6 = r10;
        v7 = r8;
    } else {
        Func_02006230();
        Call1_02001828(Func_02006246, 0x200d4b0);
        Func_0200608e_a();
        Func_02006180(1);
        Func_020063fe(141);
        Call3(Func_02006216, 0x20000, 0x20000, 0x10000);
        Func_02006254(30);
        Call3(Func_0200622c, 0x30000, 0x30000, 0x10000);
        Func_0200642a(145);
        Func_02006270(30);
        record = Value1(Func_020062ae, 0);
        if (record != 0) {
            Func_0200631c(22, *(volatile s32 *)(record + 8), *(volatile s32 *)(record + 16));
        }
        Call3(Func_020062da, 0, 0x20000, 0x10000);
        Call3(Func_020062e8, 22, 0x20000, 0x10000);
        Value2(Func_020062f8, 0, 0x200ca00);
        Call2(Func_02006318, 22, 0x200ca3c);
        Func_0200630e(0);
        Call3(Func_020063da, 0, 0x100, 0);
        Call3(Func_020063e6, 22, 0x100, 30);
        Call3(Func_020062a6_a, 0x40000, 0x40000, 0x10000);
        Func_020064a4(145);
        Func_020062ea(40);
        Call3(Func_020062c2_a, 0x50000, 0x50000, 0x10000);
        Func_020064c0(145);
        Func_02006306(20);
        Call2(Func_02006430, 0, 0x102);
        Call2(Func_0200643a, 22, 0x102);
        Func_02006320(40);
        Func_020063c8(32, 5);
        Func_020063d0(33, 5);
        Func_020063d8(30, 8);
        Func_020063e0(29, 8);
        record = Func_0200637e(30);
        *(volatile s32 *)(record + 24) = -0x10000;
        Func_02006462(32, 2);
        Func_0200646a(33, 2);
        Func_02006472(30, 3);
        Func_0200647a(29, 3);
        Call1_02001828(Func_02006450, 0xe7f);
        Func_02006472_a(28, 0, 20);
        Call3(Func_0200648e, 0, 0xc000, 0);
        Call3(Func_0200649a, 22, 0xc000, 20);
        Call2(Func_020064c6, 0x40000, 0x8000);
        Call4(Func_020064de, 0x700000, -1, 0x14b0000, 1);
        Func_020064ea();
        for (i = 0; i < 40; i++) {
            Func_02005f86(Func_020063f2(32));
            Func_02005f90(Func_020063fc(33));
            Func_02005f9a(Func_02006406(30));
            Func_02005fa4(Func_02006410(29));
            Func_0200632c(1);
        }
        base5_200d7fc = (s32)Data_0200d7fc;
        base7_200c56d = (s32)Func_0200c56d;
        v5 = base5_200d7fc;
        v6 = 0;
        v7 = base7_200c56d;
        *(volatile s32 *)Data_0200d7f8 = 0;
        *(volatile s32 *)base5_200d7fc = 0;
        Value2(Func_02006350, base7_200c56d, 0xc80);
        base_200c5a9 = 0x200c5a9;
        Call2(Func_0200635e, base_200c5a9, 0xc80);
        Func_02006414(40);
        *(volatile s32 *)base5_200d7fc = 1;
        Func_02006420(30);
        Call3(Func_020064c6_a, 19, 0x720000, 0x1220000);
        record = Func_0200646c(19);
        shifted = *(s32 *)(record + 12) + 0x400000;
        *(s32 *)(record + 12) = shifted;
        *(s32 *)(record + 60) = shifted;
        Call3(Func_02006492, 19, 0xcccc, 0x6666);
        Func_02006610(145);
        Call3(Func_020064d2, 19, 114, 0x14d);
        Func_02006502(19, 2);
        Call3(Func_0200643a_a, 0x20000, 0x20000, 0x10000);
        Func_02006638(145);
        *(volatile s32 *)base5_200d7fc = 0;
        Call3(Func_020064cc, 19, 0x6666, 0x3333);
        Call3(Func_02006508, 19, 114, 0x12c);
        Func_02006538(19, 2);
        Call3(Func_02006470, 0x50000, 0x50000, 0x10000);
        Func_0200666e(145);
        *(volatile s32 *)base5_200d7fc = 2;
        Call3(Func_02006506, 19, 0xcccc, 0x6666);
        Call3(Func_02006540, 19, 114, 0x14d);
        Func_02006570(19, 2);
        Call3(Func_020064a8, 0x20000, 0x20000, 0x10000);
        Func_020066a6(145);
        *(volatile s32 *)base5_200d7fc = 0;
        Call3(Func_0200653a, 19, 0x6666, 0x3333);
        Call3(Func_02006576, 19, 114, 0x12c);
        Func_020065a6(19, 2);
        Call3(Func_020064de_a, 0x40000, 0x40000, 0x10000);
        Func_020066dc(145);
        *(volatile s32 *)base5_200d7fc = 2;
        Call3(Func_02006572, 19, 0xcccc, 0x6666);
        Call3(Func_020065ac, 19, 114, 0x14d);
        Func_020065dc(19, 2);
        Call3(Func_02006514, 0x20000, 0x20000, 0x10000);
        Func_02006712(145);
        *(volatile s32 *)base5_200d7fc = 1;
        Func_0200655c(20);
        Call2(Func_02006686, 32, 0x102);
        Func_0200662e(32, 2);
        Func_02006666(31, 0);
        Call3(Func_0200669a, 33, 0x100, 0);
        Func_0200664a(33, 2);
        Func_0200668c(28, 0, 40);
        Call2(Func_020066be, 30, 0x102);
        Func_02006666_a(30, 2);
        Func_0200669e(30, 0);
        *(volatile s32 *)Data_0200d7f8 = 1;
        Func_0200665c(29, 1);
        Func_0200650a(1);
        Func_020066a2(29, 0);
        Call3(Func_020066ec, 29, 0x105, 20);
        Call3(Func_020066e8, 29, 0x8000, 40);
        Func_020066f2(29, 0, 20);
        Call3(Func_020066fe, 29, 0x8000, 20);
        Call3(Func_0200670a, 29, 0x4000, 40);
        Call3(Func_02006768, 29, 0x100, 0);
        Func_02006718(29, 2);
        Func_02006712_a(29, 4, 40);
        Func_0200670a_a(29, 9);
        Func_02006670(10);
        Func_02006772(29, 0, 20);
        Call1_02001828(Func_02006840_a, 0x121);
        Call3(Func_02006656, -1, -1, 0xe666);
        Call2(Func_020067c2, 0x60000, 0xc000);
        Call4(Func_020067dc, 0x540000, -1, 0x2340000, 1);
        Func_020067e8();
        Func_0200686c();
        Func_02006786(22, 0, 0);
        Func_020066c4(20);
        Call2(Func_020067ee, 22, 0x102);
        Func_020066d4(30);
        Func_02006632(base7_200c56d);
        Func_02006638_a(base_200c5a9);
        Func_020067d8(22, 0);
        Func_020067ba(0, 22, 0);
        Func_020066f8(20);
        Func_02006568();
        Func_020067a4(0, 3);
        Func_020067b4(22, 3);
        Func_02006712_b(20);
        Func_020067ba_a(22, 2);
        record = Value1(Func_02006758, 0);
        if (record != 0) {
            Func_020067a2(22, *(s16 *)(record + 10), *(s16 *)(record + 18));
        }
        Func_020067c8(22);
        Func_020067da(22, 0, 0);
        Func_02006788(31);
        Func_0200678e(28);
        Func_02006794(30);
        Func_0200679a(29);
        Func_020067a0(32);
        Func_020067a6(33);
        Call1_02001828(Func_0200675c, 0x838);
        Func_02006780();
    }
    p9 = v5;
    p10 = v6;
    p11 = v7;
}

void OverlayObject_SetChildByte5AndMark(u8 *o, s32 n)
{
    if ((*(u8 *)(o + 0x54) & 15) == 1) {
        u8 *c = *(u8 **)(o + 0x50);
        s32 idx = n - 1;
        u8 cnt;
        if (n == 0) {
            idx = Data_0200c8bc[(*(u32 *)0x03001E40 >> 1) & (*(u8 *)(o + 0x54) & 15)];
        }
        cnt = *(u8 *)(c + 0x27);
        if (cnt != 0) {
            u8 **p = (u8 **)(c + 0x28);
            s32 k = cnt;
            do {
                u8 *e = *p++;
                if (e != 0 && *(s32 *)(e + 16) != 0) {
                    *(u8 *)(e + 5) = idx;
                }
                k--;
            } while (k != 0);
        }
        *(u8 *)(c + 0x25) = 1;
    }
}

void ActorPresentation_SetFourActorsModeByBit(void)
{
    extern s32 Data_0200d7fc;

    if (((*(u32 *)0x03001E40 >> Data_0200d7fc) & 3) != 0) {
        Func_020041b6(Func_02006840(32), 1);
        Func_020041c2(Func_0200684c(33), 1);
        Func_020041ce(Func_02006858(30), 1);
        Func_020041da_a(Func_02006864(29), 1);
    } else {
        Func_020041e8(Func_02006872(32), 8);
        Func_020041f4(Func_0200687e(33), 8);
        Func_02004200(Func_0200688a(30), 8);
        Func_0200420c(Func_02006896(29), 8);
    }
}

void FieldScene_RunFlagGatedActorSequence(void)
{
    extern u8 Data_0200d7fc[];

    s32 kc0_1 = 0xC00000;
    s32 k4be_1 = 0x4BE0000;
    s32 k2000_1 = 0x2000;
    s32 kc0_2 = 0xC00000;
    s32 k4a5_1 = 0x4A50000;
    s32 k2000_2 = 0x2000;
    s32 ke3_1 = 0xE30000;
    s32 k4be_2 = 0x4BE0000;
    s32 k4000_1 = 0x4000;
    s32 kfa_1 = 0xFA0000;
    s32 k4be_3 = 0x4BE0000;
    s32 k4000_2 = 0x4000;
    s32 ke3_2 = 0xE30000;
    s32 k4a5_2 = 0x4A50000;
    s32 k2000_3 = 0x3000;
    s32 kf3_1 = 0xF30000;
    s32 k4fd_1 = 0x4FD0000;
    s32 kc000_1 = 0xC000;
    s32 k100_1 = 0x100;
    s32 k446_1 = 0x446;
    s32 k446_2 = 0x446;
    s32 k4000_3 = 0x4000;
    s32 k4000_4 = 0x4000;
    s32 k40000_1 = 0x40000;
    s32 k8000_1 = 0x8000;
    s32 kd8_1 = 0xD80000;
    s32 ng1 = -1;
    s32 k4d0_1 = 0x4D00000;
    s32 k3000_1 = 0x3000;
    s32 k30000_1 = 0x30000;
    s32 k6000_1 = 0x6000;
    s32 ke8_1 = 0xE80000;
    s32 ng2 = -1;
    s32 k4e5_1 = 0x4E50000;
    s32 k9999_1 = 0x9999;
    s32 k1333_1 = 0x1333;
    s32 kd8_2 = 0xD80000;
    s32 ng3 = -1;
    s32 k4d0_2 = 0x4D00000;
    s32 k102_1 = 0x102;
    s32 k102_2 = 0x102;
    s32 k4b5_1 = 0x4B5;
    s32 k4b1_1 = 0x4B1;
    s32 ke8_2 = 0xE80000;
    s32 ng4 = -1;
    s32 k4e5_2 = 0x4E50000;
    s32 kf3_2 = 0xF30000;
    s32 k4fd_2 = 0x4FD0000;
    s32 k20000_1 = 0x20000;
    s32 k19999_1 = 0x19999;
    s32 k3333_1 = 0x3333;
    s32 kd8_3 = 0xD80000;
    s32 ng5 = -1;
    s32 k4d0_3 = 0x4D00000;
    s32 k105_1 = 0x105;
    s32 k800a_1 = 0x800A;
    s32 k9999_2 = 0x9999;
    s32 k4ccc_1 = 0x4CCC;
    s32 k9999_3 = 0x9999;
    s32 k4ccc_2 = 0x4CCC;
    s32 k4ba_1 = 0x4BA;
    s32 k4a5_3 = 0x4A5;
    s32 k6000_2 = 0x6000;
    s32 k8000_2 = 0x8000;
    s32 k8018_1 = 0x8018;
    s32 kc000_2 = 0xC000;
    s32 k800a_2 = 0x800A;
    s32 k105_2 = 0x105;
    s32 k105_3 = 0x105;
    s32 k106_1 = 0x106;
    s32 k8000_3 = 0x8000;
    s32 kc000_3 = 0xC000;
    s32 k4000_5 = 0x4000;
    s32 kc000_4 = 0xC000;
    s32 k9000_1 = 0x9000;
    s32 ka000_1 = 0xA000;
    s32 k8000_4 = 0x8000;
    s32 k800a_3 = 0x800A;
    s32 k105_4 = 0x105;
    s32 k105_5 = 0x105;
    s32 k105_6 = 0x105;
    s32 k105_7 = 0x105;
    s32 k8000_5 = 0x8000;
    s32 k8000_6 = 0x8000;
    s32 k800a_4 = 0x800A;
    s32 kd000_1 = 0xD000;
    s32 k2000_4 = 0x2000;
    s32 ka000_2 = 0xA000;
    s32 k8000_7 = 0x8000;
    s32 v83a_2 = 0x83A;
    u8 *tbl;
    s32 w16;
    s32 m;
    s32 one;

    if (Func_02006862((s32)&Value_0000083a) != 0) {
        return;
    }
    Func_0200688c();
    Func_02006928(10, kc0_1, k4be_1);
    Func_020069a4(10, k2000_1, 0);
    Func_02006944(10, 5);
    {
        u8 *o;
        s32 v;
        o = Func_020068e2(10);
        v = Func_020067f6(Func_02006810(), 0x5A) + 60;
        tbl = Data_0200cec8;
        *(u16 *)(o + 0x64) = v;
        Func_02006916(10, tbl);
    }
    Func_0200696a(9, kc0_2, k4a5_1);
    Func_020069e6(9, k2000_2, 0);
    Func_02006982(24, ke3_1, k4be_2);
    Func_020069fe(24, k4000_1, 0);
    Func_0200699e(24, 6);
    {
        u8 *o;
        s32 v;
        o = Func_0200693c(24);
        v = Func_02006850(Func_0200686a(), 0x5A) + 60;
        *(u16 *)(o + 0x64) = v;
        Func_0200696e(24, tbl);
    }
    Func_020069c2(25, kfa_1, k4be_3);
    Func_02006a3e(25, k4000_2, 0);
    Func_020069de(25, 6);
    {
        u8 *o;
        s32 v;
        o = Func_0200697c(25);
        v = Func_02006890(Func_020068aa(), 0x5A) + 60;
        *(u16 *)(o + 0x64) = v;
        Func_020069ae(25, tbl);
    }
    Func_02006a02(26, ke3_2, k4a5_2);
    Func_02006a7e(26, k2000_3, 0);
    Func_02006a1a(23, kf3_1, k4fd_1);
    Func_02006a96(23, kc000_1, 0);
    Func_0200695a(Func_020069cc(23), 0);
    Func_020068e8(3);
    Func_02006a86((s32)&Value_00000e8c);
    Func_02006a9e((s32)&Value_0000201a, 0);
    Func_02006ad2(0, k100_1, 20);
    Func_02006a4c(0, 150, k446_1);
    {
        u8 *p;
        p = Func_02006a02_b(0);
        if (p != 0) {
            Func_02006a70(22, *(s32 *)(p + 8), *(s32 *)(p + 16));
        }
    }
    Func_02006a6a(22, 132, k446_2);
    Func_02006abc(0, 22, 0);
    Func_020069f2(40);
    Func_02006b06(0, k4000_3, 0);
    Func_02006b12(22, k4000_4, 20);
    Func_02006b3e(k40000_1, k8000_1);
    Func_02006b58(kd8_1, ng1, k4d0_1, 1);
    Func_02006b64();
    Func_02006a32(40);
    Func_02006afa(10, 2);
    Func_02006b3c(10, 0, 10);
    Func_02006b0c(23, 3);
    Func_02006b5e(9, 0, 10);
    Func_02006b06_b(9, 3);
    Func_02006b60(9, 0, 10);
    Func_02006b7c(9, k3000_1, 10);
    Func_02006ba8(k30000_1, k6000_1);
    Func_02006bc0(ke8_1, ng2, k4e5_1, 1);
    Func_02006bcc();
    Func_02006a9a(20);
    Func_02006c60(134);
    Func_02006b5a(23, 4, 0);
    Func_02006b52(23, 6);
    Func_02006ab8(10);
    Func_02006b5a_a(23, 0, 0);
    Func_02006ac8(60);
    Func_02006c84();
    Func_02006b74(10, 1);
    {
        u8 *o;
        o = Func_02006b12_b(10);
        w16 = 0x10000;
        *(s32 *)(o + 0x18) = w16;
        *(s32 *)(o + 0x1C) = w16;
    }
    Func_02006b8a(24, 1);
    {
        u8 *o;
        o = Func_02006b28(24);
        *(s32 *)(o + 0x18) = w16;
        *(s32 *)(o + 0x1C) = w16;
    }
    Func_02006b9c(25, 1);
    {
        u8 *o;
        o = Func_02006b3a(25);
        *(s32 *)(o + 0x18) = w16;
        *(s32 *)(o + 0x1C) = w16;
    }
    Func_02006bc6(10, 2);
    Func_02006bce(9, 2);
    Func_02006bd6(24, 2);
    Func_02006bde(25, 2);
    Func_02006bee(26, 2);
    Func_02006c5e(k9999_1, k1333_1);
    Func_02006c78(kd8_2, ng3, k4d0_2, 1);
    Func_02006c84_a();
    Func_02006c76(26, k102_1);
    Func_02006c80(9, k102_2);
    Func_02006b66(60);
    Func_02006c2e(26, 2);
    Func_02006c2e_a(26, 3);
    Func_02006c6e(26, 0);
    Func_02006c38(25, 2, 0);
    Func_02006c02(25, 234, k4b5_1);
    Func_02006c4c(26, 2, 0);
    Func_02006c16(26, 227, k4b1_1);
    Func_02006bac(90);
    Func_02006cec(ke8_2, ng4, k4e5_2, 1);
    Func_02006cf8();
    Func_02006c64(23, kf3_2, k4fd_2);
    Func_02006b1a(1);
    Func_02006d98(106);
    {
        u8 *o;
        o = Func_02006c16_b(23);
        *(s32 *)(o + 0x28) = k20000_1;
    }
    Func_02006bea(6);
    Func_02006c92(23, 7);
    Func_02006bf8(20);
    Func_02006db4();
    Func_02006c02_b(20);
    Func_02006d32(k19999_1, k3333_1);
    Func_02006d4c(kd8_3, ng5, k4d0_3, 1);
    Func_02006d58();
    Func_02006ce8(24, 2);
    Func_02006c2e_b(20);
    Func_02006d50(24, k105_1, 40);
    Func_02006d12(24, 10, 0);
    Func_02006c48(10);
    Func_02006d10(10, 2);
    Func_02006d48(k800a_1, 0);
    {
        u8 *o;
        o = Func_02006c96(25);
        o += 0x5A;
        m = 0xFE;
        {
            u8 lv = m & *o;
            *o = lv;
        }
    }
    {
        u8 *o;
        o = Func_02006cee(26);
        o += 0x5A;
        *o = *o & m;
    }
    Func_02006d10_b(25, k9999_2, k4ccc_1);
    Func_02006d1a(26, k9999_3, k4ccc_2);
    Func_02006d4c_b(25, 247, k4ba_1);
    Func_02006d5e(26, 227, k4a5_3);
    {
        u8 *o;
        o = Func_02006d24(25);
        o += 0x5A;
        one = 1;
        {
            u8 lv = *o | one;
            *o = lv;
        }
    }
    {
        u8 *o;
        o = Func_02006d34(26);
        o += 0x5A;
        {
            u8 lv = one | *o;
            *o = lv;
        }
    }
    Func_02006e18(26, k6000_2, 0);
    Func_02006e24(25, k8000_2, 10);
    Func_02006dc4(24, 4);
    Func_02006e26(k8018_1, 0, 10);
    Func_02006e42(10, kc000_2, 20);
    Func_02006e4c(10, 0, 10);
    Func_02006dec(10, 4);
    Func_02006e4e(k800a_2, 0, 10);
    Func_02006e78(24, k105_2, 0);
    Func_02006e82(10, k105_3, 60);
    Func_02006e8e(9, k106_1, 20);
    Func_02006e8a(9, k8000_3, 40);
    Func_02006e96(9, kc000_3, 20);
    Func_02006ea0(9, 0, 30);
    Func_02006eac(9, k4000_5, 10);
    Func_02006ea6(9, 0, 10);
    Func_02006ec2(10, kc000_4, 0);
    Func_02006ece(25, k9000_1, 0);
    Func_02006eda(24, ka000_1, 0);
    Func_02006ee6(26, k8000_4, 10);
    Func_02006ea6_b(10, 1);
    Func_02006ee8(k800a_3, 0, 10);
    Func_02006e98(9, 4);
    Func_02006efa(9, 0, 10);
    Func_02006f24(10, k105_4, 0);
    Func_02006f2e(24, k105_5, 0);
    Func_02006f38(25, k105_6, 0);
    Func_02006f42(26, k105_7, 40);
    Func_02006f3c(9, 0, 10);
    Func_02006f0e(24, 25, 0);
    Func_02006e44(20);
    Func_02006f56(9, 0, 0);
    Func_02006f60(10, 0, 10);
    Func_02006f6c(24, k8000_5, 0);
    Func_02006f78(25, k8000_6, 10);
    Func_02006f18(24, 3);
    Func_02006f28(25, 3);
    Func_02006f5a(10, 9, 0);
    Func_02006e90(20);
    Func_02006f58(10, 1);
    Func_02006f9a(k800a_4, 0, 10);
    Func_02006f52(9, 3);
    Func_02006fbe(24, kd000_1, 10);
    Func_02006f76(24, 1);
    Func_02006fc0(24, 0, 10);
    Func_02006fda(10, 0, 0);
    Func_02006fe4(9, 0, 0);
    Func_02006fa4(26, 1);
    Func_02006ff8(26, k2000_4, 20);
    Func_02007004(25, ka000_2, 20);
    Func_02006fac(25, 3);
    Func_02007006(25, 0, 10);
    Func_02006fbe_b(26, 3);
    Func_02006f1c(20);
    Func_02006fe4_b(9, 2);
    Func_02006fd4(9, 3);
    Func_0200702e(9, 0, 10);
    Func_0200704a(26, k8000_7, 10);
    Func_02006ff2(26, 3);
    Func_02006f50(20);
    Func_02007018(9, 1);
    Func_0200705a(9, 0, 10);
    Func_02005116();
    Func_02006f5c(v83a_2);
    Func_02006f80();
}

/* Sets up records 26, 24, 25, 9 and 10 (position/speed, facing, movement),
 * runs several timed particle/object sequences against constant tables, then
 * takes two branches whose outcome picks entries out of the Data_00000e9b
 * and Data_00000ea1 byte tables to drive further record 9/22 setup calls. */
void RunEventScript01(void)
{
    u32 i;
    s32 entry;
    s32 record;
    s32 base5_200cec8;
    s32 base5_e9b;
    s32 base5_ea1;

    ObjectMotion_ArmCallback_1_020028a4(26, 0x3000, 0);
    ObjectMotion_ArmCallback_2_020028a4(24, 0xd000, 0);
    ObjectMotion_ArmCallback_3_020028a4(25, 0xb000, 0);
    ObjectMotion_ArmCallback_4(9, 0x3000, 0);
    ObjectMotion_ArmCallback_5(10, 0xd000, 20);
    Object_SetModeById_1(26, 3);
    Object_SetModeById_2(24, 3);
    Object_SetModeById_3(25, 3);
    Object_SetModeById_4(9, 3);
    ObjectMotion_CallThenWaitForAnimationChange_1_020028a4(25, 3);
    BattleRuntime_WaitIfModeZero_1_020028a4(20);
    ObjectMotion_SetSpeedLimitAndAcceleration_1_020028a4(0x10000, 0x2000);
    ObjectMotion_PlaceWithinCameraBounds_1_020028a4(0x860000, -1, 0x4ab0000, 1);
    ObjectMotion_SetSpeedParameters_1_020028a4(26, 0x19999, 0xcccc);
    ObjectMotion_SetSpeedParameters_2(9, 0x19999, 0xcccc);
    ObjectMotion_EnableActionAndSetCallback_1(26, 0x200cab4);
    ObjectMotion_MarkActiveAndSetActionCallback_1(9, 0x200ca78);
    Audio_PlayCue_1(158);
    Call3(Func_02006fea, 0x200d7a0, 38, 72);
    BattleRuntime_WaitIfModeZero_2(10);
    ObjectMotion_SetPositionAndReset_1_020028a4(9, 149, 0x497);
    ObjectMotion_SetHorizontalPositionWithTerrain_1_020028a4(9, 0, 0);
    ObjectMotion_SetPositionAndReset_2_020028a4(25, 250, 0x4be);
    BattleEffect_PlayQueuedSound_1();
    ObjectMotion_ArmCallback_6(10, 0x3000, 0);
    ObjectMotion_ArmCallback_7(24, 0x3000, 0);
    ObjectMotion_ArmCallback_8(25, 0x3000, 0);
    Object_SetModeById_5(10, 5);
    Object_SetModeById_6(24, 6);
    Object_SetModeById_7(25, 6);
    /* For records 10, 24 and 25: fetch the record's entry pointer, fetch a
     * value from that record's own state, and store a derived value into
     * the entry's field at offset 100. */
    entry = Scene_GetRecord_1_020028a4(10);
    record = Func_02007022();
    *(u16 *)(entry + 100) = (Func_02007008(record, 90) + 60);
    entry = Scene_GetRecord_2(24);
    record = Func_0200703a();
    *(u16 *)(entry + 100) = (Func_02007020(record, 90) + 60);
    entry = Scene_GetRecord_3(25);
    record = Func_02007052();
    *(u16 *)(entry + 100) = (Func_02007038(record, 90) + 60);
    base5_200cec8 = (s32)Data_0200cec8;
    ObjectMotion_EnableActionAndSetCallback_2(10, base5_200cec8);
    ObjectMotion_EnableActionAndSetCallback_3(24, base5_200cec8);
    ObjectMotion_EnableActionAndSetCallback_4(25, base5_200cec8);
    Object_LookupAndStep_1(26);
    BattleRuntime_WaitIfModeZero_3(10);
    Audio_PlayCue_2(159);
    Call3(Func_020070cc, 0x200d7e2, 38, 72);
    BattleRuntime_WaitIfModeZero_4(30);
    BattleEffect_PlayQueuedSound_2();
    ObjectMotion_PlaceWithinCameraBounds_2_020028a4(0x700000, -1, 0x4c90000, 1);
    Audio_PlayCue_3(158);
    Call3(Func_020070f6, 0x200d78a, 35, 73);
    BattleRuntime_WaitIfModeZero_5(20);
    BattleEffect_PlayQueuedSound_3();
    ObjectMotion_EnableActionAndSetCallback_5(9, 0x200cb28);
    BattleRuntime_WaitIfModeZero_6(20);
    ObjectMotion_EnableActionAndSetCallback_6(26, 0x200cb9c);
    BattleRuntime_WaitIfModeZero_7(40);
    Audio_PlayCue_4(159);
    Call3(Func_0200712c, 0x200d7cc, 35, 73);
    Object_LookupAndStep_2(26);
    BattleEffect_PlayQueuedSound_4();
    BattleRuntime_WaitIfModeZero_8(40);
    base5_e9b = (s32)Data_00000e9b;
    SceneWork_SetStepValue_1_020028a4(base5_e9b);
    BattleRuntime_RunThenWaitIfModeZero_1_020028a4(9, 0, 20);
    ObjectMotion_CallThenWaitForAnimationChange_2_020028a4(26, 3);
    BattleRuntime_RunThenWaitIfModeZero_2_020028a4(0x201a, 0, 40);
    Object_SetModeById_8(9, 3);
    ObjectMotion_CallThenWaitForAnimationChange_3(26, 3);
    BattleRuntime_WaitIfModeZero_9(30);
    ObjectMotion_EnableActionAndSetCallback_7(9, 0x200cc0c);
    ObjectMotion_EnableActionAndSetCallback_8(26, 0x200cc5c);
    BattleRuntime_WaitIfModeZero_10(40);
    ObjectMotion_SetSpeedLimitAndAcceleration_2(0x20000, 0x4000);
    ObjectMotion_PlaceWithinCameraBounds_3(0x690000, -1, 0x43e0000, 1);
    Object_LookupAndStep_3(9);
    ObjectMotion_ArmCallback_9(9, 0, 0);
    BattleEffect_SpawnLinkedResourceObject_1(9, 0x100, 40);
    BattleRuntime_RunThenWaitIfModeZero_3(9, 0, 10);
    ObjectMotion_ArmCallback_10(0, 0x8000, 0);
    ObjectMotion_ArmCallback_11(22, 0x8000, 10);
    ObjectMotion_SetPositionAndReset_3_020028a4(9, 105, 0x43e);
    ObjectMotion_SetVariantCallbackAndInvokeObject_1_020028a4(9, 2);
    ObjectMotion_SetSpeedParameters_3(0x8009, 0);
    ObjectMotion_ArmCallback_12(22, 0, 0);
    /* Branch on a condition; pass byte 4 or byte 5 of the Data_00000e9b
     * table to the corresponding follow-up call. */
    if (UiWork_WaitThenFinalizeCapacity_1(0, 0) == 0) {
        ObjectMotion_CallThenWaitForAnimationChange_4(9, 3);
        SceneWork_SetStepValue_2((base5_e9b + 4));
    } else {
        ObjectMotion_SetVariantCallbackAndInvokeObject_2(9, 2);
        SceneWork_SetStepValue_3((base5_e9b + 5));
    }
    BattleEvent_RunActionAndWait_1_020028a4(0x8009, 0);
    ObjectMotion_ArmCallback_13(22, 0x8000, 40);
    BattleEffect_SpawnLinkedResourceObject_2(9, 0x100, 30);
    base5_ea1 = (s32)Data_00000ea1;
    SceneWork_SetStepValue_4(base5_ea1);
    ObjectMotion_SetSpeedParameters_4(0x8009, 0);
    /* Branch on a condition; each side reads a different byte of the
     * Data_00000ea1 table and runs its own follow-up sequence. */
    if (UiWork_WaitThenFinalizeCapacity_2(0, 0) == 0) {
        ObjectMotion_CallThenWaitForAnimationChange_5(9, 3);
        SceneWork_SetStepValue_5((base5_ea1 + 1));
        BattleRuntime_RunThenWaitIfModeZero_4(0x8009, 0, 30);
        ObjectMotion_ArmCallback_14(22, 0x8000, 20);
        Object_SetModeById_9(0, 3);
        Object_SetModeById_10(22, 3);
        ObjectMotion_CallThenWaitForAnimationChange_6(9, 3);
        BattleRuntime_WaitIfModeZero_11(40);
    } else {
        BattleEffect_SpawnLinkedResourceObject_3(9, 0x105, 90);
        BattleEffect_SpawnLinkedResourceObject_4(9, 0x103, 40);
        Object_SetModeById_11(9, 4);
        SceneWork_SetStepValue_6((base5_ea1 + 2));
        BattleEvent_RunActionAndWait_2(0x8009, 0);
    }
    ObjectMotion_EnableActionAndSetCallback_9(9, 0x200cca8);
    BattleRuntime_WaitIfModeZero_12(90);
    Object_LinkPair_1(0, 22, 0);
    BattleRuntime_WaitIfModeZero_13(40);
    Object_SetModeById_12(0, 3);
    ObjectMotion_CallThenWaitForAnimationChange_7(22, 3);
    BattleRuntime_WaitIfModeZero_14(20);
    Object_SetModeById_13(22, 2);
    /* If a record pointer is returned, pass its s16 fields at offsets 10
     * and 18 through to the follow-up call. */
    record = Scene_GetRecord_4(0);
    if (record != 0) {
        ObjectMotion_ResetAndSetPosition_1(22, *(s16 *)(record + 10), *(s16 *)(record + 18));
    }
    ObjectMotion_CommitCurrentPositionAndActivate_1(22);
    ObjectMotion_SetHorizontalPositionWithTerrain_2_020028a4(22, 0, 0);
}

void SceneDialogue_RunActorTenFlag30dDialogue(void)
{
    s32 v2000 = 0x2000;
    s32 msg1 = 0xEA5;
    s32 msg0 = 0xEA4;
    u8 *tbl;

    Func_02007428();
    Func_020074c8(10, 1);
    Func_0200742e_a(10);
    Func_02007508(10, 0, 20);
    if (Func_02007426((s32)&Value_0000030d) != 0) {
        Func_02007528(msg1);
        Func_0200754a(10, 0, 10);
    } else {
        Func_0200753a(msg0);
        Func_0200751a(10, 1);
        Func_02007564(10, 0, 10);
        Func_0200752c(10, 2);
        Func_02007576(10, 0, 10);
    }
    Func_02007592(10, v2000, 20);
    Func_02007532(10, 5);
    Func_02007498(10);
    {
        u8 *rec;
        s32 v;
        rec = Func_020074d6(10);
        v = Func_020073ea(Func_02007404_a(), 0x5A) + 60;
        tbl = Data_0200cec8;
        *(u16 *)(rec + 0x64) = v;
        Func_02007508_b(10, tbl);
    }
    Func_020074be(20);
    Func_020074b4((s32)&Value_0000030d);
    Func_020074d8();
}

void FieldScene_RunScene372_02003c48(void)
{
    extern u8 Data_0200d7fc[];

    u32 i;
    s32 record;

    Func_0200828a(20);
    Call1_02001828(Func_0200833a, 0x166);
    Func_02008300(0);
    Func_02008306(1);
    Func_0200830c(2);
    Func_02008312(3);
    Func_02008318(4);
    Func_0200831e(5);
    Call2(Func_020084f6, 0x10003, 1);
    Call2(Func_02008500, 0x10000, 2);
    Func_0200850e(1);
    Func_020082d4(120);
    Func_02008514(0, 0);
    Func_02008522(60);
    Func_020082e8(60);
    Call1_02001828(Func_020083a0, 0x166);
    Func_02008356(0);
    Func_0200835c(1);
    Func_02008362(2);
    Func_02008368(3);
    Func_0200836e(4);
    Func_02008374(5);
}

void ActorPresentation_SelectActorTwentySevenState(void)
{
    extern s32 Data_0200d7fc;

    struct Actor *actor = Func_0200854a(27);
    u32 flags = Data_03001e40;
    u8 *presentation = actor->presentation;

    if (flags & 1) {
        u8 *state = presentation + 35;
        *state = 2;
    } else {
        u8 *state = presentation + 35;
        *state = 64;
    }
}

void FieldScene_RunScene372_02003e48(void)
{
    extern u8 Data_0200d7fc[];

    u32 i;
    s32 rec7;
    s32 rec8;
    s32 record;

    rec8 = Value1(Func_0200857a, 0);
    rec7 = Value1(Func_02008582, 8);
    Func_02008558();
    if (Value1(Func_0200853e, 0x305) != 0) {
        Func_020085c0(8);
        Func_02008566(10);
        Func_0200862e(8, 2);
        Func_02008574(40);
        if (*(s16 *)(rec8 + 6) >= 0) {
            Func_02008624(8, 7);
        } else {
            Func_0200862e_a(8, 8);
        }
        Func_02008656(8, 2);
        Func_0200859c(20);
        Call1_02001828(Func_02008682, 0xed2);
        Func_0200869a(8, 0);
        Value2(Func_02008602, 8, 0x200cec8);
        Func_0200865a(8, 6);
    } else {
        Func_02008622(8);
        *(s32 *)(rec7 + 24) = 0x10000;
        *(s32 *)(rec7 + 28) = 0x10000;
        Call3(Func_020086de, 8, 0x1000, 0);
        if (*(s16 *)(rec8 + 6) >= 0) {
            Func_02008686(8, 7);
        } else {
            Func_02008690(8, 8);
        }
        Func_020085f6(20);
        Call1_02001828(Func_020086dc, 0xed1);
        Func_020086fe(8, 0, 20);
        Func_020086ae(8, 1);
        Func_020086c8(8, 4, 0);
        Func_0200861e(80);
        Func_020086e6(8, 2);
        Func_0200862c(40);
        if (*(s16 *)(rec8 + 6) >= 0) {
            Func_020086dc_a(8, 7);
        } else {
            Func_020086e6_a(8, 8);
        }
        Func_0200864c(2);
        Func_02008706(8, 2, 0);
        Func_0200865c(60);
        Func_02008724(8, 2);
        Func_0200866a(20);
        Func_02008762(8, 0);
        Value2(Func_020086ca, 8, 0x200cec8);
        Func_02008722(8, 6);
        Call1_02001828(Func_02008678, 0x305);
    }
    Func_0200869c();
}

void FieldScene_ConfigureActorTwentyTwoScene(void)
{
    extern u8 Data_0200d7fc[];

    u32 i;
    u8 *record;

    BattleRuntime_Reset_1();
    ObjectMotion_PlaceWithinCameraBounds_1(-1, -1, -1, 0);
    ObjectMotion_EnableActionAndResetMotion_1(ACTOR_ID);
    Call1_02001828(Func_0200861e_a, 0x200c5b9);
    ObjectMotion_SetPositionAndReset_1(0, 0x1e0, 0x570);
    ObjectMotion_SetHorizontalPositionWithTerrain_1(0, 0, 0);
    ObjectMotion_ArmCallback_1(ACTOR_ID, 0x3000, 20);
    {
        /* Set bit 0 of the flag byte at +35. */
        u8 *record = Scene_GetRecord_1(ACTOR_ID);
        u8 flags = *(volatile u8 *)&record[35];

        record[35] = (u8)(flags | 1);
    }
    ObjectMotion_SetHorizontalPositionWithTerrain_2(ACTOR_ID, 0xf90000, 0x4d80000);
    Func_02008656_a(1);
    SceneWork_SetStepValue_1(0xed3);
    BattleEvent_RunActionAndWait_1(0x1016, 0);
    ObjectMotion_SetHorizontalPositionWithTerrain_3(ACTOR_ID, 0xac0000, 0x4fe0000);
    Func_02008676(1);
    ObjectMotion_SetSpeedLimitAndAcceleration_1(0x40000, 0x8000);
    ObjectMotion_PlaceWithinCameraBounds_2(0xa20000, 0, 0x5050000, 1);
    Object_CommitPositionThenWaitIfModeZero_1();
    BattleRuntime_WaitIfModeZero_1(40);
    ObjectMotion_CallThenWaitForAnimationChange_1(ACTOR_ID, 4);
    BattleRuntime_RunThenWaitIfModeZero_1(0x1016, 0, 10);
    ObjectMotion_ArmCallback_2(ACTOR_ID, 0xc000, 20);
    ObjectMotion_SetVariantCallbackAndInvokeObject_1(ACTOR_ID, 2);
    BattleRuntime_RunThenWaitIfModeZero_2(0x1016, 0, 10);
    ObjectMotion_ArmCallback_3(ACTOR_ID, 0x1000, 20);
    ObjectMotion_CallThenWaitForAnimationChange_2(ACTOR_ID, 3);
    ObjectMotion_SetSpeedParameters_1(ACTOR_ID, 0x20000, 0x10000);
    ObjectMotion_SetPositionAndReset_2(ACTOR_ID, 165, 0x514);
    ObjectMotion_SetPositionAndReset_3(ACTOR_ID, 195, 0x598);
    GameFlag_Set_1(0x842);
}

/* Each of the 15 placement calls below takes the same 6-argument shape:
 * two coordinate-like values, two more coordinate-like values, and a
 * trailing pair of small counts. The final call takes no arguments. */
void FieldScene_BuildPlacementGrid(void)
{
    extern u8 Data_0200d7fc[];

    u32 i;
    u8 *record;

    Call6_02001828(Func_020087a8, 16, 96, 11, 73, 6, 3); /* main:08009180 */
    Call6_02001828(Func_020087be, 16, 96, 34, 68, 14, 10); /* main:08009180 */
    Call6_02001828(Func_020087d0, 16, 96, 64, 68, 7, 7); /* main:08009180 */
    Call6_02001828(Func_020087e4, 9, 95, 11, 73, 6, 3); /* main:08009180 */
    Call6_02001828(Func_020087f6, 40, 94, 34, 68, 14, 10); /* main:08009180 */
    Call6_02001828(Func_0200880a, 54, 94, 64, 68, 8, 7); /* main:08009180 */
    Call6_02001828(Func_0200881c, 72, 75, 72, 76, 1, 1); /* main:08009180 */
    Call6_02001828(Func_0200882c, 72, 75, 74, 76, 1, 1); /* main:08009180 */
    Call6_02001828(Func_02008858, 7, 75, 1, 1, 6, 75); /* main:080091c0 */
    Call6_02001828(Func_0200886c, 8, 70, 3, 1, 8, 71); /* main:080091c0 */
    Call6_02001828(Func_02008880, 8, 70, 2, 1, 9, 72); /* main:080091c0 */
    Call6_02001828(Func_02008892, 8, 70, 2, 1, 9, 73); /* main:080091c0 */
    Call6_02001828(Func_020088a4, 11, 66, 1, 1, 8, 73); /* main:080091c0 */
    Call6_02001828(Func_020088b6, 12, 66, 1, 4, 11, 73); /* main:080091c0 */
    Call6_02001828(Func_020088ca, 25, 0, 1, 1, 6, 74); /* main:080091c0 */
    /* No-argument call that closes out the sequence started above. */
    Func_020088a6(); /* main:08009128 */
}

void SceneState_SetWords1c0And1c8AndRun(void)
{
    void Func_02000360_r(s32);

    u8 *state;

    Func_02008934();
    state = *(u8 **)0x03001ebc;
    *(s32 *)(state + 0x1C0) = 0x200;
    *(s32 *)(state + 0x1C8) = 64;
    Func_02008938(0x87c);
    Func_02008aa0(12, 2);
    Func_02008948(0x900);
    Func_0200896c();
}

void SceneState_SetWorkWordsAndFlag87f(void)
{
    void Func_02000360_r(s32);

    u8 *state;

    Func_02008978();
    state = *(u8 **)0x03001ebc;
    *(s32 *)(state + 0x1C0) = 0x200;
    *(s32 *)(state + 0x1C8) = 64;
    Func_0200897c(0x87f);
    Func_02008ae4(12, 3);
    Func_0200898c(0x900);
    Func_020089b0();
}

void SceneActor_SetModeByFrameBit1(s32 o)
{
    extern s32 Data_0200d7fc;

    s32 v;

    if ((*(volatile s32 *)0x03001e40 & 2) != 0) {
        Func_0200899c(o, 7);
    } else {
        Func_020089a6(o, 0);
    }
    {
        volatile s32 *q = (volatile s32 *)0x03001e40;
        v = (*(s32 *)0x0200d7fc << 3) + 16;
        if (Func_0200891e(*q, v) == 0) {
            Func_02008710(o);
        }
    }
}

void OverlayObject_UpdateRandomSlotByFrame(s32 obj)
{
    volatile s32 *fc = (volatile s32 *)0x03001e40;
    s32 t;
    s32 n;

    if ((*fc & 1) != 0) {
        t = (s32)((u32)*fc >> 1);
        Func_020089f0(obj, Func_02008950(t, 6));
    }
    n = (*(s32 *)0x0200d7fc << 3) + 16;
    if (Func_02008966(*fc, n) == 0) {
        Func_02008758(obj);
    }
}

void OverlayObject_ApplyIwramWord1e40(s32 o)
{
    volatile s32 *p = (s32 *)0x03001e40;
    s32 t;

    if ((*p & 1) != 0) {
        t = (s32)((u32)*p >> 1);
        Func_02008a38(o, Func_02008998(t, 6));
    }
}

void SceneEffect_UpdateArcOverAnchor(Obj *o)
{
    Obj *b;
    s32 t;
    s32 d;
    s32 k;

    b = o->f68;
    o->f64++;
    t = (s16)o->f64;
    t = (s16)o->f64;
    if (t > 31) {
        Func_02008a10(o);
    } else {
        d = Func_020089f8(t << 10);
        o->f18 = d;
        o->f1c = d;
        o->f08 = b->f08;
        k = 0x10000;
        o->f0c += k;
        k -= d;
        o->f10 = b->f10 + ((k << 2) + k) + 0x80000;
    }
}

void OverlayObject_UpdateArcFromParent(Obj *o)
{
    Obj *b;
    s32 t;
    s32 d;
    s32 k;

    b = o->f68;
    o->f64++;
    t = (s16)o->f64;
    t = (s16)o->f64;
    if (t > 31) {
        Func_02008a60(o);
    } else {
        d = Func_02008a48(t << 10);
        o->f18 = d;
        o->f1c = -d;
        o->f08 = b->f08;
        k = 0x10000;
        o->f0c += k;
        k -= d;
        o->f10 = b->f10 - ((k << 2) + k) + 0x100000;
    }
}

void SceneState_SetValue140Mode0(void)
{
    void Func_02000360_r(s32);

    Func_02008dec(140, 0);
}

void FieldScene_RunSingleStep(void)
{
    Func_02008e00();
}

void FieldScene_RunFourPairedSteps(void)
{
    extern u8 Data_0200d7fc[];

    Func_02008c9e(32);
    Func_0200887a_a();
    Func_02008ca8(33);
    Func_02008884_a();
    Func_02008cb2(30);
    Func_0200888e();
    if (*(s32 *)0x0200d7f8 == 0) {
        Func_02008cc4(29);
        Func_020088a0();
    }
}

void SceneState_SetValue19ThenCall(void)
{
    void Func_02000360_r(s32);

    Func_02008cda(19);
    Func_020088fe();
}

void OverlayObject_CopyRecordField1ToSlots22And8(void)
{
    Ent *src;
    Ent *dst;
    Ent *dst2;

    src = Func_02008cea(0)->f50;
    dst = Func_02008cf2(22)->f50;
    dst->f = src->f;
    dst2 = Func_02008d0e(8)->f50;
    dst2->f = src->f;
}

void SceneState_SetValueEe4(void)
{
    void Func_02000360_r(s32);

    Func_02008cfc();
    Func_02008cdc(0xee4, 1);
    Func_02008d10();
}

void FieldScene_RunScriptedStep1120(void)
{
    extern u8 Data_0200d7fc[];

    Func_02008d18();
    Func_02008cf8((s32)&Value_00001120, 1);
    Func_02008d2c();
}
