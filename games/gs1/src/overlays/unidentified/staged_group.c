#include "types.h"

void Func_02003c74();
void Func_02003d0c();
void Func_02003ce4();
void Func_02003c2c();
void Func_02003d04();
void Func_02003cfc();
void Func_02003c5c();

#define NULL ((void *)0)
#define FIELD_AT_OFFSET(base, type, offset)     (*(type)((u8 *)(base) + (offset)))
#define SceneState_ApplyValues12And2And3 Func_02000030
#define SceneState_ClearWord24WhenFlag200 Func_0200158c
#define SceneState_ClearRecordZeroBit0At35 Func_020015f8
#define SceneState_CheckFlags941And940 Func_020017e8
#define SceneState_ApplyFlagGatedActorEightSetup Func_02001858
#define SceneState_SetWork448AndRunFlag915Step Func_020018a4
#define SceneState_SetValues27Through34 Func_02001e6c
#define SampleEffectAction Func_02003b7e
#define SampleEffectOffset Func_02003bac
#define SampleEffectTimer Func_02003bba
#define RunEffectAction3 Func_02003bcc
#define RunEffectAction4 Func_02003bd6
#define SceneEffect_UpdateRandomAction Func_02000040
#define SceneEffect_SetSlotVariantAndDescriptor Func_02001ea4
#define OverlayObject_AdvanceXWhenCounterExpires Func_020000ac
#define SceneData_SelectTableByWord224 Func_020000ec
#define SceneData_ReturnZero Func_02000174
#define SceneData_GetTablec6b8 Func_02000178
#define SceneData_SelectTableByWord224B Func_02000180
#define SceneData_SelectTableBySelector224 Func_02000240
#define FieldScene_RunStepWithValue29df Func_020001e4
#define FieldScene_RunStepWithValueFd6 Func_0200020c
#define FieldScene_RunSlotZeroFacingSequence Func_020002e8
#define RunSceneArrivalSetup Func_02000504
#define FieldScene_DrawTilesAndRaiseActor11 Func_02003970
#define SceneDialogue_RunActorNineteenDialogue Func_020002c8
#define SceneDialogue_RunActorTenDialogue Func_02000374
#define SceneDialogue_RunActor181Line916 Func_02001610
#define SceneState_BranchOnSlotZeroFacing Func_02000394
#define FieldScene_RunActorTwentyFourAngleDialogue Func_02000420
#define SceneDialogue_RunActorTwentyThreeByLeaderHeading Func_020003d8
#define SceneActor_SetActorZeroByte35Bit0 Func_020015e0
#define FieldScene_RunStagedGroupSequence Func_02000590
#define Func_02003c74(a0, a1, a2) Call3(Func_02003c74, a0, a1, a2)
#define Func_02003c2c(a0, a1, a2) Call3(Func_02003c2c, a0, a1, a2)
#define Func_02003ce4(a0, a1, a2) Call3(Func_02003ce4, a0, a1, a2)
#define Func_02003cfc(a0, a1, a2) Call3(Func_02003cfc, a0, a1, a2)
#define Func_02003d04(a0, a1) Call2(Func_02003d04, a0, a1)
#define Func_02003d0c(a0, a1) Call2(Func_02003d0c, a0, a1)
#define Func_02003c5c(a0, a1, a2) Call3(Func_02003c5c, a0, a1, a2)
#define SceneState_LinkRecordZeroWhenFlag200Clear Func_020015b4
#define BattleRuntime_Reset_1(args...) Func_02005c06(args)
#define ObjectMotion_PlaceWithinCameraBounds_1(a0, a1, a2, a3) Call4(Func_02005d38, a0, a1, a2, a3)
#define ObjectMotion_SetHorizontalPositionWithTerrain_1(a0, a1, a2) Call3(Func_02005cac, a0, a1, a2)
#define ObjectMotion_SetSpeedLimitAndAcceleration_1(a0, a1) Call2(Func_02005d52, a0, a1)
#define ObjectMotion_PlaceWithinCameraBounds_2(a0, a1, a2, a3) Call4(Func_02005d6c, a0, a1, a2, a3)
#define BattleRuntime_WaitIfModeZero_1(args...) Func_02005d98(args)
#define ObjectMotion_SetSpeedParameters_1(a0, a1, a2) Call3(Func_02005ca2, a0, a1, a2)
#define ObjectMotion_SetPositionAndReset_1(a0, a1, a2) Call3(Func_02005cde, a0, a1, a2)
#define ObjectMotion_SetSpeedParameters_2(a0, a1, a2) Call3(Func_02005cbc, a0, a1, a2)
#define ObjectMotion_SetSpeedParameters_3(a0, a1, a2) Call3(Func_02005cca, a0, a1, a2)
#define Scene_GetRecord_1(args...) Func_02005cc0(args)
#define Scene_GetRecord_2(args...) Func_02005ccc(args)
#define Scene_GetRecord_3(args...) Func_02005cde_a(args)
#define Scene_GetRecord_4(args...) Func_02005cf0(args)
#define ObjectMotion_ResetAndSetPositionInMode2_1(args...) Func_02005d3a(args)
#define ObjectMotion_SetPositionAndReset_2(args...) Func_02005d4c(args)
#define Object_SetModeById_1(args...) Func_02005d74(args)
#define BattleRuntime_WaitIfModeZero_2(args...) Func_02005cea(args)
#define Scene_GetRecord_5(args...) Func_02005d20(args)
#define Scene_GetRecord_6(args...) Func_02005d30(args)
#define BattleRuntime_WaitIfModeZero_3(args...) Func_02005d0e(args)
#define ObjectMotion_CallThenWaitForAnimationChange_1(args...) Func_02005dae(args)
#define SceneWork_SetStepValue_1(a0) Call1(Func_02005dec, a0)
#define BattleRuntime_RunThenWaitIfModeZero_1(args...) Func_02005e0e(args)
#define ObjectMotion_ArmCallback_1(args...) Func_02005e28(args)
#define ObjectMotion_CallThenWaitForAnimationChange_2(args...) Func_02005dd0(args)
#define BattleRuntime_RunThenWaitIfModeZero_2(args...) Func_02005e2a(args)
#define BattleRuntime_WaitIfModeZero_4(a0, a1) Call2(Func_02005e64, a0, a1)
#define BattleRuntime_WaitIfModeZero_5(args...) Func_02005d52_a(args)
#define Scene_GetRecord_7(a0) Value1(Func_02005d88, a0)
#define ObjectMotion_SetHorizontalPositionWithTerrain_2(args...) Func_02005dee(args)
#define ObjectMotion_SetSpeedParameters_4(a0, a1, a2) Call3(Func_02005db0, a0, a1, a2)
#define ObjectMotion_SetPositionAndReset_3(a0, a1, a2) Call3(Func_02005dec_a, a0, a1, a2)
#define ObjectMotion_ArmCallback_2(a0, a1, a2) Call3(Func_02005e80, a0, a1, a2)
#define ObjectMotion_ArmCallback_3(a0, a1, a2) Call3(Func_02005e8c, a0, a1, a2)
#define BattleRuntime_RunThenWaitIfModeZero_3(a0, a1, a2) Call3(Func_02005e86, a0, a1, a2)
#define ObjectMotion_ArmCallback_4(a0, a1, a2) Call3(Func_02005ea2, a0, a1, a2)
#define ObjectMotion_SetPositionAndReset_4(args...) Func_02005e24(args)
#define BattleRuntime_WaitIfModeZero_6(args...) Func_02005dba(args)
#define ObjectMotion_CallThenWaitForAnimationChange_3(args...) Func_02005e5a(args)
#define BattleRuntime_WaitIfModeZero_7(args...) Func_02005dc8(args)
#define BattleRuntime_RunThenWaitIfModeZero_4(args...) Func_02005eba(args)
#define BattleEffect_SpawnLinkedResourceObject_1(a0, a1, a2) Call3(Func_02005eec, a0, a1, a2)
#define BattleEffect_SpawnLinkedResourceObject_2(a0, a1, a2) Call3(Func_02005ef6, a0, a1, a2)
#define ObjectMotion_ArmCallback_5(args...) Func_02005ee8(args)
#define ObjectMotion_ArmCallback_6(a0, a1, a2) Call3(Func_02005ef4, a0, a1, a2)
#define ObjectMotion_ArmCallback_7(args...) Func_02005efe(args)
#define ObjectMotion_ArmCallback_8(args...) Func_02005f08(args)
#define BattleEffect_SpawnLinkedResourceObject_3(a0, a1, a2) Call3(Func_02005f2c, a0, a1, a2)
#define ObjectMotion_SetVariantCallbackAndInvokeObject_1(args...) Func_02005ed4(args)
#define BattleRuntime_RunThenWaitIfModeZero_5(args...) Func_02005f16(args)
#define ObjectMotion_SetVariantCallbackAndInvokeObject_2(args...) Func_02005ee6(args)
#define BattleRuntime_RunThenWaitIfModeZero_6(args...) Func_02005f28(args)
#define BattleEffect_SpawnLinkedResourceObject_4(a0, a1, a2) Call3(Func_02005f5a, a0, a1, a2)
#define ObjectMotion_SetVariantCallback_1(args...) Func_02005efa(args)
#define BattleRuntime_RunThenWaitIfModeZero_7(args...) Func_02005f44(args)
#define ObjectMotion_SetVariantCallbackAndInvokeObject_3(args...) Func_02005f14(args)
#define BattleRuntime_RunThenWaitIfModeZero_8(args...) Func_02005f56(args)
#define ObjectMotion_SetVariantCallbackAndInvokeObject_4(args...) Func_02005f26(args)
#define BattleRuntime_WaitIfModeZero_8(args...) Func_02005e7c(args)
#define ObjectMotion_ArmCallback_9(a0, a1, a2) Call3(Func_02005f80, a0, a1, a2)
#define BattleEventRuntime_ProcessAction_1(a0, a1) Call2(Func_02005f80_a, a0, a1)
#define BattleRuntime_WaitIfModeZero_9(args...) Func_02005e96(args)
#define ObjectMotion_SetVariantCallbackAndInvokeObject_5(args...) Func_02005f4e(args)
#define ObjectMotion_CallThenWaitForAnimationChange_4(args...) Func_02005f3e(args)
#define BattleRuntime_RunThenWaitIfModeZero_9(args...) Func_02005f98(args)
#define BattleEffect_SpawnLinkedResourceObject_5(a0, a1, a2) Call3(Func_02005fca, a0, a1, a2)
#define ObjectMotion_ArmCallback_10(a0, a1, a2) Call3(Func_02005fbe, a0, a1, a2)
#define ObjectMotion_SetVariantCallbackAndInvokeObject_6(args...) Func_02005f7e(args)
#define ObjectMotion_CallThenWaitForAnimationChange_5(args...) Func_02005f6e(args)
#define ObjectMotion_ArmCallback_11(a0, a1, a2) Call3(Func_02005fda, a0, a1, a2)
#define BattleRuntime_RunThenWaitIfModeZero_10(args...) Func_02005fd4(args)
#define ObjectMotion_ArmCallback_12(a0, a1, a2) Call3(Func_02005ff0, a0, a1, a2)
#define ObjectMotion_ArmCallback_13(a0, a1, a2) Call3(Func_02005ffc, a0, a1, a2)
#define BattleRuntime_RunThenWaitIfModeZero_11(args...) Func_02005ff6(args)
#define ObjectMotion_ArmCallback_14(args...) Func_02006010(args)
#define ObjectMotion_ArmCallback_15(a0, a1, a2) Call3(Func_0200601c, a0, a1, a2)
#define ObjectMotion_ArmCallback_16(args...) Func_02006026(args)
#define ObjectMotion_ArmCallback_17(args...) Func_02006030(args)
#define ObjectMotion_SetVariantCallbackAndInvokeObject_7(args...) Func_02005ff0_a(args)
#define ObjectMotion_SetSpeedParameters_5(a0, a1) Value2(Func_02006020, a0, a1)
#define ObjectMotion_ArmCallback_18(a0, a1, a2) Call3(Func_0200604c, a0, a1, a2)
#define UiWork_WaitThenFinalizeCapacity_1(a0, a1) Value2(Func_02005f84, a0, a1)
#define ObjectMotion_SetVariantCallbackAndInvokeObject_8(args...) Func_02006018(args)
#define BattleRuntime_RunThenWaitIfModeZero_12(a0, a1, a2) Call3(Func_0200605a, a0, a1, a2)
#define ObjectMotion_CallThenWaitForAnimationChange_6(args...) Func_0200605a_a(args)
#define BattleRuntime_RunThenWaitIfModeZero_13(a0, a1, a2) Call3(Func_020060b4, a0, a1, a2)
#define ObjectMotion_SetVariantCallbackAndInvokeObject_9(args...) Func_02006084(args)
#define BattleRuntime_RunThenWaitIfModeZero_14(args...) Func_020060c6(args)
#define ObjectMotion_ArmCallback_19(a0, a1, a2) Call3(Func_020060e2, a0, a1, a2)
#define ObjectMotion_CallThenWaitForAnimationChange_7(args...) Func_0200608a(args)
#define BattleRuntime_RunThenWaitIfModeZero_15(args...) Func_020060e4(args)
#define BattleRuntime_WaitIfModeZero_10(a0, a1) Call2(Func_0200611e, a0, a1)
#define BattleRuntime_WaitIfModeZero_11(args...) Func_0200600c(args)
#define BattleRuntime_RunThenWaitIfModeZero_16(args...) Func_020060fe(args)
#define ObjectMotion_ArmCallback_20(a0, a1, a2) Call3(Func_0200611a, a0, a1, a2)
#define ObjectMotion_CallThenWaitForAnimationChange_8(args...) Func_020060c2(args)
#define BattleRuntime_RunThenWaitIfModeZero_17(args...) Func_0200611c(args)
#define BattleRuntime_WaitIfModeZero_12(a0, a1) Call2(Func_02006156, a0, a1)
#define BattleRuntime_WaitIfModeZero_13(a0, a1) Call2(Func_02006160, a0, a1)
#define BattleRuntime_WaitIfModeZero_14(args...) Func_0200604e(args)
#define ObjectMotion_SetVariantCallbackAndInvokeObject_10(args...) Func_02006106(args)
#define BattleRuntime_RunThenWaitIfModeZero_18(args...) Func_02006148(args)
#define ObjectMotion_SetVariantCallbackAndInvokeObject_11(args...) Func_02006118(args)
#define ObjectMotion_ArmCallback_21(a0, a1, a2) Call3(Func_0200616c, a0, a1, a2)
#define BattleRuntime_RunThenWaitIfModeZero_19(args...) Func_02006166(args)
#define ObjectMotion_ArmCallback_22(args...) Func_02006180(args)
#define ObjectMotion_CallThenWaitForAnimationChange_9(args...) Func_02006128(args)
#define ObjectMotion_ArmCallback_23(a0, a1, a2) Call3(Func_02006194, a0, a1, a2)
#define BattleEvent_RunActionAndWait_1(args...) Func_02006184(args)
#define ObjectMotion_ArmCallback_24(a0, a1, a2) Call3(Func_020061a8, a0, a1, a2)
#define ObjectMotion_CallThenWaitForAnimationChange_10(args...) Func_02006150(args)
#define ObjectMotion_ArmCallback_25(a0, a1, a2) Call3(Func_020061bc, a0, a1, a2)
#define BattleEventRuntime_ProcessAction_2(a0, a1) Call2(Func_020061bc_a, a0, a1)
#define BattleRuntime_WaitIfModeZero_15(args...) Func_020060d2(args)
#define ObjectMotion_SetVariantCallbackAndInvokeObject_12(args...) Func_0200618a(args)
#define BattleRuntime_RunThenWaitIfModeZero_20(a0, a1, a2) Call3(Func_020061cc, a0, a1, a2)
#define ObjectMotion_CallThenWaitForAnimationChange_11(args...) Func_02006184_a(args)
#define ObjectMotion_CallThenWaitForAnimationChange_12(args...) Func_0200618c(args)
#define ObjectMotion_ArmCallback_26(a0, a1, a2) Call3(Func_020061f8, a0, a1, a2)
#define ObjectMotion_CallThenWaitForAnimationChange_13(args...) Func_020061a0(args)
#define ObjectMotion_SetSpeedParameters_6(args...) Func_02006152_a(args)
#define ObjectMotion_SetSpeedParameters_7(args...) Func_0200615c_a(args)
#define Scene_GetRecord_8(args...) Func_02006168(args)
#define Scene_GetRecord_9(args...) Func_0200617a(args)
#define ObjectMotion_ResetAndSetPositionInMode2_2(args...) Func_020061c4(args)
#define ObjectMotion_SetPositionAndReset_5(args...) Func_020061d6(args)
#define Object_SetModeById_2(args...) Func_020061fe(args)
#define BattleRuntime_WaitIfModeZero_16(args...) Func_02006174(args)
#define Scene_GetRecord_10(args...) Func_020061aa(args)
#define Scene_GetRecord_11(args...) Func_020061ba(args)
#define ObjectMotion_SetPositionAndReset_6(args...) Func_0200620c(args)
#define ObjectMotion_ArmCallback_27(a0, a1, a2) Call3(Func_020062a0, a0, a1, a2)
#define Audio_PlayCue_1(args...) Func_0200633e(args)
#define ObjectMotion_SetPositionAndReset_7(args...) Func_02006256(args)
#define ObjectMotion_SetHorizontalPositionWithTerrain_3(args...) Func_02006278(args)
#define ObjectMotion_SetSpeedParameters_8(a0, a1, a2) Call3(Func_0200623a, a0, a1, a2)
#define ObjectMotion_SetPositionAndReset_8(args...) Func_02006274(args)
#define ObjectMotion_SetHorizontalPositionWithTerrain_4(args...) Func_02006296(args)
#define Object_SetModeById_3(args...) Func_020062a6(args)
#define ObjectMotion_CallThenWaitForAnimationChange_14(args...) Func_020062b6(args)
#define ObjectMotion_ResetAndSetPositionInMode2_3(args...) Func_02006290(args)
#define ObjectMotion_SetPositionAndReset_9(args...) Func_020062a2(args)
#define ObjectMotion_ResetAndSetPositionInMode2_4(args...) Func_020062a4(args)
#define ObjectMotion_SetPositionAndReset_10(args...) Func_020062b6_a(args)
#define ObjectMotion_SetHorizontalPositionWithTerrain_5(args...) Func_020062d8(args)
#define ObjectMotion_SetHorizontalPositionWithTerrain_6(args...) Func_020062e2(args)
#define ObjectMotion_SetSpeedParameters_9(args...) Func_020063ca(args)
#define BattleRuntime_ScheduleShoulderButtonModeUpdate_1(args...) Func_02006294(args)
#define SCENE_FIELD_1C8 (*(s32 *)(*(u8 **)Data_03001ebc + 0x1c8))
#define SCENE_FIELD_1C0 (*(s32 *)(*(u8 **)Data_03001ebc + 0x1c0))
#define FieldScene_DispatchBySceneId Func_0200164c
#define FieldScene_RunMiddleSequence Func_02001930
#define FieldScene_RunTwoActorCutsceneSequence Func_02002008
#define FieldScene_RunLateSequence Func_020031b8
#define FieldScene_RunSupplementalSequenceOne Func_02003768
#define FieldScene_RunSecondaryGroupSequence Func_020026c0
#define SceneActor_FindAtTileXZ Func_02003184
#define SceneEffect_UpdateMotionWithDamping Func_020036f8
#define GetOrbitingSceneObject Func_02007664
#define NormalizeOrbitingSceneObject Func_0200762e
#define IsGameFlagSet Func_02007664_a
#define AllocateEffectTransfer Func_02007604
#define LoadEffectResource Func_02007694
#define ConfigurePaletteTransfer Func_0200762c
#define ReleaseEffectTransfer Func_0200762a
#define UpdateOrbitingSceneObject Value_0200b9d5
#define SceneEffect_UpdateLobeOrbitEffect26 Func_0200390c
#define SceneEffect_UpdateOrbitingEffect Func_020039d4
#define InitializeOrbitingRenderEffect Func_02003a3c

#include "resource_3a8_effect.h"

struct Obj {
    u8 filler00[6];
    u16 f06;
};

struct Obj_02000040 {
    u8 filler00[100];
    u16 f64;
    u16 f66;
};

struct Object {
    u8 filler00[6];
    u16 x;
    u8 filler08[94];
    s16 cnt;
};

typedef struct Effect {
    unsigned char pad00[0xC];
    s32 y;
    unsigned char pad10[0x13];
    s8 state23;
} Effect;

struct SceneWork {
    u8 unknown_000[0x1c0];
    s32 request;
    u8 unknown_1c4[4];
    s32 setup;
    u8 unknown_1cc[12];
    u16 step;
};

struct Obj_020036f8 {
    u8 filler00[8];
    s32 f08;
    s32 f0c;
    u8 filler10[8];
    s32 f18;
    s32 f1c;
    s32 f20;
    s32 f24;
    s32 f28;
    u8 filler2c[56];
    s16 f64;
};

typedef struct RenderData {
    unsigned char pad00[0x1E];
    s16 rotation;
} RenderData;

typedef struct Effect_0200390c {
    unsigned char pad00[8];
    s32 x;
    s32 y;
    unsigned char pad10[0x20];
    s32 angle;
    unsigned char pad34[4];
    s32 base_x;
    s32 base_y;
    unsigned char pad40[0x10];
    RenderData *render;
} Effect_0200390c;

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

extern s16 Data_02000240[];
extern u8 Data_0200beac[];
extern u8 Value_00000063;
extern u8 Value_00000066;
extern u8 Value_00000099;
extern u8 Value_0000009a;
extern u8 Value_0000009b;
extern u8 Value_0000009c;
extern u8 Data_0200c2c8[];
extern u8 Data_0200c448[];
extern u8 Data_0200c4a8[];
extern u8 Data_0200c520[];
extern u8 Data_0200c580[];
extern u8 Data_0200c628[];
extern u8 Data_0200c298[];
extern u8 Data_0200c6b8[];
extern u8 Data_0200c768[];
extern u8 Data_0200ca20[];
extern u8 Data_0200ca80[];
extern u8 Data_0200cb58[];
extern u8 Data_0200c750[];
extern u8 Data_0200cbf4[];
extern u8 Data_0200ce88[];
extern u8 Data_0200cedc[];
extern u8 Data_0200cf24[];
extern u8 Data_0200cf54[];
extern u8 Data_0200cf9c[];
extern u8 Data_0200cbe8[];
extern u8 Data_0200b90d[];
extern u8 Value_00001a9e[];
extern u8 Value_00001aa2[];
extern const u32 SceneAction_GroupMotion[];
extern const u32 SceneAction_GroupOffsetMotion[];
extern s16 Data_02000240_t[][1];
extern volatile s32 Data_0200bd84[];
extern const u8 Data_0200c21c[];
extern const u8 Data_0200c230[];
extern u8 Value_0200b9d5;

void Func_02003d96(s32, s32, s32);
s32 Func_02005168();
void Func_0200518c();
s32 Func_0200521a(s32);
s32 Func_020053c2(s32);
void Func_020053d4(s32);
void Func_020053da(s32);
void Func_020053e0(s32);
s32 Func_020053e6(s32);
s32 Func_020053e6_a(s32);
void Func_020053f8(s32);
s32 Func_02005406(s32);
void Func_02003842(void);
s32 Func_02005432(s32);
void Func_020052a4(s32);
s32 Func_02005442(s32);
struct Obj *Func_02005494(s32);
void Func_02003f50(void);
s32 Func_0200548e(s32);
void Func_0200546e(s32, s32, s32, s32, s32, s32);
void Func_0200548a(s32, s32, s32, s32, s32, s32);
void Func_02005494_a(s32, s32, s32, s32, s32, s32);
void Func_02005478(void);
void Func_02005416(s32);
void Func_0200567c(s32);
void Func_02003d16(s32);
void Func_02003d1c(s32);
void Func_02003d22(s32);
void Func_02003d28(s32);
void Func_02003d2e(s32);
void Func_02003d34(s32);
void Func_02003d3a(s32);
void Func_02003d40(s32);
s32 Func_02003b7e();
s32 Func_02003bac();
s32 Func_02003bba();
u8 Func_02003bcc(void *, s32);
u8 Func_02003bd6(void *, s32);
struct Obj *Func_02005ac6(s32);
u32 Func_020059e2(void);
void Func_02005a2e(struct Obj *, s32);
s32 Func_02003bec(void);
s32 Func_02003bfa(void);
void Func_02003ddc(void);
void Func_02003dac(s32, s32);
void Func_02003db4(s32, s32);
void Func_02003df8(void);
void Func_02003e04(void);
s32 Func_02003e8e(s32, s32, s32);
void Func_02003dfc(s32);
void Func_02003f6c(s32, s32);
s32 Func_02003e3c(s32, s32);
void Func_02003e30(void);
struct Obj *Func_02003f0a(s32);
void Func_02003ef6(void);
s32 Func_02003fb0(s32, s32, s32);
void Func_02003efe(s32);
void Func_02003fd4(s32);
s32 Func_02003fe4(s32, s32);
s32 Func_02003f3c(s32, s32);
void Func_02003fb8(s32, s32);
void Func_02004008(s32, s32);
void Func_02003fda(s32, s32);
void Func_0200402a(s32, s32);
void Func_02003f5e(void);
void Func_020040fe();
void Func_0200428c();
void Func_020040be();
void Func_0200403c();
void Func_020040d4();
void Func_02004052();
void Func_02004178();
u8 *Func_0200416e();
void Func_020041dc();
void Func_020041d8();
void Func_0200415e();
void Func_0200429c();
void Func_020042b0();
void Func_020042bc();
void Func_02004180();
void Func_02007532(s32, s32, s32, s32, s32, s32);
void Func_02007544(s32, s32, s32, s32, s32, s32);
void Func_02007560(Effect *, s32);
Effect *Func_02007594(s32);
void Func_0200759e(s32);
void Func_02003ec0(void);
void Func_02003f8e(s32);
s32 Func_02003fb6(s32, s32);
void Func_02003eda(void);
void Func_02003f6c_a(void);
void Func_0200403a(s32);
s32 Func_02004062(s32, s32);
void Func_02003f86(void);
void Func_0200513a(s32);
void Func_02005130(s32);
s32 Func_0200529a(s32, s32, s32);
void Func_02005208(s32);
void Func_02005378(s32, s32);
s32 Func_02005248(s32, s32);
struct Obj *Func_02003fb6_a(s32);
s32 Func_02004126(s32, s32);
void Func_02003fac(void);
void Func_0200407a(s32);
void Func_02004092(s32, s32);
void Func_02003fc6(void);
struct Obj *Func_02004042(s32);
s32 Func_020041b2(s32, s32);
void Func_02004038(void);
void Func_02004106(s32);
void Func_0200411e(s32, s32);
void Func_02004052_a(void);
struct Obj *Func_02003ffa(s32);
s32 Func_02004168(s32, s32);
void Func_02003fee(void);
void Func_020040bc(s32);
s32 Func_020040e4(s32, s32);
void Func_02004008_a(void);
s32 Func_02005202(s32);
void Func_02003bf4();
s32 Func_02003bd4();
void Func_02003c04();
void Func_02003d7c();
void Func_02003cec();
void Func_02003bc4();
void Func_02003bec_a();
void Func_02003c9c();
void Func_02003c34();
u8 *Func_02003c1c();
void Func_02003d14();
void Func_02003c3c();
void Func_02003b14();
void Func_02003cbc();
void Func_02003cd4();
void Func_02003c7c();
void Func_02003c84();
void Func_02003c8c();
void Func_02003c94();
void Func_02003c4c();
void Func_02001ed8();
void Func_02003c44();
void Func_02003b24();
void Func_02003cf4();
void Func_02003c24();
void Func_02003d54();
void Func_02003bdc();
void Func_02003bfc();
void Func_02003d24();
void Func_02003d34_a();
void Func_02003d3c();
s32 Func_02005190();
void Func_020051b0();
s32 Func_020051e6();
void Func_0200522e(s32);
void Func_02002d24(void);
void Func_02002e5c(void);
void Func_02002ed8(void);
void Func_02002f30(void);
void Func_02002fc8(void);
s32 Func_02005520();
s32 Func_02005530();
void Func_02005532();
void Func_0200554e();
s32 Func_02005568();
void Func_02005572();
void Func_02005576();
s32 Func_02005580();
s32 Func_02005582();
void Func_02005588();
void Func_020055b4();
void Func_020055c0();
void Func_020055d2();
void Func_020055e8();
s32 Func_020055f0();
void Func_020055fa();
u8 *Func_02005606();
void Func_0200560a();
u8 *Func_02005616();
void Func_0200561a();
u8 *Func_02005622();
void Func_02005626();
void Func_0200563a();
u8 *Func_0200564e();
void Func_02005654();
s32 Func_0200565c();
void Func_0200565e();
void Func_02005672();
void Func_02005682();
void Func_02005692();
void Func_02005692_a();
void Func_020056a0();
void Func_020056a2();
void Func_020056b2();
void Func_020056c2();
void Func_020056d2();
void Func_020056e2();
void Func_020056f2();
void Func_02005702();
void Func_02005710();
void Func_02005712();
void Func_02005722();
void Func_02005732();
void Func_02005742();
void Func_02005752();
void Func_02005762();
void Func_02005772();
void Func_02005782();
void Func_02005792();
void Func_020057a2();
void Func_020057b2();
void Func_020057c2();
void Func_020057d2();
void Func_020057e6();
void Func_020057f8();
void Func_02005814();
void Func_02005826();
void Func_0200588a();
s32 Func_0200589a();
void Func_0200589e();
void Func_020058ae();
void Func_020058be();
void Func_020058ce();
void Func_020058de();
void Func_020058ee();
void Func_020058fe();
void Func_0200590e();
void Func_0200591e();
void Func_0200592e();
void Func_0200593e();
void Func_0200594e();
void Func_0200594e_a();
void Func_0200595e();
void Func_0200596e();
void Func_0200597e();
void Func_0200598e();
void Func_0200599e();
void Func_020059b2();
void Func_020059c4();
void Func_020059e0();
void Func_02005a24();
void Func_02005bbc();
void Func_02005b3e();
void Func_02005b52();
void Func_02005bd0();
void Func_02005c06();
void Func_02005ca2();
void Func_02005cac();
void Func_02005cbc();
u8 *Func_02005cc0();
void Func_02005cca();
u8 *Func_02005ccc();
s32 Func_02005cde();
s32 Func_02005cde_a();
void Func_02005cea();
s32 Func_02005cf0();
void Func_02005d0e();
u8 *Func_02005d20();
u8 *Func_02005d30();
void Func_02005d38();
void Func_02005d3a();
void Func_02005d4c();
void Func_02005d52();
void Func_02005d52_a();
void Func_02005d6c();
void Func_02005d74();
s32 Func_02005d88();
void Func_02005d98();
void Func_02005dae();
void Func_02005db0();
void Func_02005dba();
void Func_02005dc8();
void Func_02005dd0();
void Func_02005dec();
void Func_02005dec_a();
void Func_02005dee();
void Func_02005e0e();
void Func_02005e24();
void Func_02005e28();
void Func_02005e2a();
void Func_02005e5a();
void Func_02005e64();
void Func_02005e7c();
void Func_02005e80();
void Func_02005e86();
void Func_02005e8c();
void Func_02005e96();
void Func_02005ea2();
void Func_02005eba();
void Func_02005ed4();
void Func_02005ee6();
void Func_02005ee8();
void Func_02005eec();
void Func_02005ef4();
void Func_02005ef6();
void Func_02005efa();
void Func_02005efe();
void Func_02005f08();
void Func_02005f14();
void Func_02005f16();
void Func_02005f26();
void Func_02005f28();
void Func_02005f2c();
void Func_02005f3e();
void Func_02005f44();
void Func_02005f4e();
void Func_02005f56();
void Func_02005f5a();
void Func_02005f6e();
void Func_02005f7e();
void Func_02005f80();
void Func_02005f80_a();
s32 Func_02005f84();
void Func_02005f98();
void Func_02005fbe();
void Func_02005fca();
void Func_02005fd4();
void Func_02005fda();
void Func_02005ff0();
void Func_02005ff0_a();
void Func_02005ff6();
void Func_02005ffc();
void Func_0200600c();
void Func_02006010();
void Func_02006018();
void Func_0200601c();
s32 Func_02006020();
void Func_02006026();
void Func_02006030();
void Func_0200604c();
void Func_0200604e();
void Func_0200605a();
void Func_0200605a_a();
void Func_02006084();
void Func_0200608a();
void Func_020060b4();
void Func_020060c2();
void Func_020060c6();
void Func_020060d2();
void Func_020060e2();
void Func_020060e4();
void Func_020060ee();
void Func_020060fe();
void Func_02006104();
void Func_02006106();
void Func_02006118();
void Func_0200611a();
void Func_0200611c();
void Func_0200611e();
void Func_02006128();
void Func_02006148();
void Func_02006150();
void Func_02006156();
void Func_02006160();
void Func_02006166();
s32 Func_02006168();
void Func_0200616c();
void Func_02006170();
void Func_02006174();
s32 Func_0200617a();
void Func_02006180();
void Func_02006184();
void Func_02006184_a();
void Func_02006186();
void Func_0200618a();
void Func_0200618c();
void Func_02006194();
void Func_020061a0();
void Func_020061a8();
u8 *Scene_GetRecord_12();
u8 *Scene_GetRecord_13();
void Func_020061bc();
void Func_020061bc_a();
void Func_020061c4();
void Func_020061cc();
void Func_020061d6();
void Func_020061f8();
void Func_020061fe();
void Func_0200620c();
void Func_0200623a();
void Func_02006256();
void Func_02006274();
void Func_02006278();
void Func_02006290();
void Func_02006294();
void Func_02006296();
void Func_020062a0();
void Func_020062a2();
void Func_020062a4();
void Func_020062a6();
void Func_020062b6();
void Func_020062b6_a();
void Func_020062d8();
void Func_020062e2();
void Func_0200633e();
void Func_020063b8();
void Func_020063be();
void Func_020063ca();
u8 *Func_02006152();
u8 *Func_0200615c();
u8 *Func_02006152_a();
u8 *Func_0200615c_a();
u8 *Func_020061aa();
u8 *Func_020061ba();
void Func_02004aba();
void Func_02004c98();
s32 Func_02006376();
void Func_0200652a();
void Func_02006d4e();
void Func_02006d8e();
s32 Func_02006dd2();
void Func_02006dd4();
void Func_02006dde();
s32 Func_02006de4();
void Func_02006de6();
void Func_02006df4();
void Func_02006e94();
s32 Func_02006e96();
void Func_02006ea2();
void Func_02006ecc();
s32 Func_02006ece();
void Func_02006ed4();
s32 Func_02006ed6();
void Func_02006ef2();
s32 Func_02006ef6();
void Func_02006ef8();
void Func_02006f02();
void Func_02006f12();
void Func_02006f22();
void Func_02006f32();
void Func_02006f42();
void Func_02006f52();
void Func_02006f62();
void Func_02006f72();
void Func_02006f82();
s32 Func_02006f88();
void Func_02006f92();
void Func_02006fa2();
void Func_02006fb8();
void Func_02006fbc();
void Func_02006fc0();
void Func_02006fc8();
void Func_02006fd8();
void Func_02006fe8();
void Func_02006ff8();
void Func_02007008();
void Func_02007018();
void Func_02007028();
void Func_02007038();
void Func_02007048();
void Func_02007058();
void Func_02007072();
void Func_02007084();
void Func_020070a0();
void Func_020070ae();
void Func_020070bc();
void Func_020070ea();
void Func_020070ee();
s32 Func_020070f6();
void Func_02007118();
void Func_02007128();
void Func_0200712c();
void Func_0200713a();
void Func_02007140();
void Func_02007150();
void Func_02007160();
void Func_02007170();
void Func_0200717e();
void Func_02007180();
void Func_02007190();
void Func_020071a0();
void Func_020071b0();
void Func_020071c6();
void Func_020071d6();
void Func_020071e6();
void Func_020071f6();
void Func_02007206();
void Func_02007216();
void Func_02007226();
void Func_02007236();
void Func_02007250();
void Func_02007262();
void Func_0200727e();
void Func_0200728c();
void Func_020072cc();
void Func_020072e4();
void Func_02006f22_a();
void Func_02006fd8_a();
void Func_02006ff8_a();
void Func_02007140_a();
void Func_02007206_a();
void Func_02007056();
void Func_0200709e();
s32 Func_02007386();
s32 Func_0200738a();
s32 Func_02007392();
s32 Func_0200739e();
s32 Func_020073a4();
s32 Func_020073b0();
void Func_020073c4();
s32 Func_020073cc();
s32 Func_020073ea();
s32 Func_020073ec();
s32 Func_020073f8();
s32 Func_020073fc();
void Func_0200740c();
void Func_0200744c();
void Func_0200745c();
void Func_0200746e();
void Func_02007540();
void Func_0200755e();
void Func_02007566();
void Func_02007588();
void Func_020075a6();
void Func_02003bb4();
void Func_02003b7c();
void Func_02003d2c();
void Func_02003bec_b();
u32 Func_02003b2c();
void Func_02003c54();
s32 Func_02003cc4();
s32 Func_02003c14();
void Func_02003b9c();
void Func_02003ba4();
u8 *Func_02003d1c_a();
void Func_02003d34_b();
void Func_0200727e_a(void);
s32 Func_02007450(s32);
s32 Func_02007466(s32);
s32 Func_02007478(void);
s32 Func_0200747e(void);
s32 Func_02007480(s32);
Effect *Func_0200752e(s32);
s32 Func_02007512(s32);
s32 Func_02007528(s32);
s32 Func_02007540_a(void);
s32 Func_02007542(s32);
s32 Func_02007546(void);
OrbitingSceneObject *Func_02007664();
OrbitingSceneObject *Func_02007664_a();
void Func_0200762e(OrbitingSceneObject *, s32);
u8 *Func_02007604(s32, s32);
void Func_02007694(s32);
s32 Func_0200762c(u8, s32, u8 *);
void Func_0200762a(s32);

/* Two early long branches share the scene-skip tail. Four polling loops
 * wait on signed actor fields; calls bind at loader runtime addresses. */

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

/* The scene step counter at 0x1d8 of the shared scene work record. */
static __inline__ void bump_step(s32 amount)
{
    extern u8 Data_03001ebc[];

    u8 *work = *(u8 **)Data_03001ebc;

    *(u16 *)(work + 0x1d8) = (u16)(*(u16 *)(work + 0x1d8) + amount);
}

/* Contiguous unnamed leaf-owner run for resource_3a8. */

/* resource_3a8 owner at 0x02000504, 138 bytes. Scene arrival sequence. */
static __inline__ void SetOffset(s32 actor, s32 axis, s32 offset)
{
    extern u8 *Data_03001ebc;

    Func_020041d8(actor, axis, offset);
}

/* Verified scene siblings use these call forms for independently evaluated
 * large constants in repeated actor operations. */
static __inline__ void Call2(void (*f)(), s32 a0, s32 a1)
{
    void Func_02001e6c();
    void Func_02001ea4();

    f(a0, a1);
}

static __inline__ void Call3(void (*f)(), s32 a0, s32 a1, s32 a2)
{
    void Func_02001e6c();
    void Func_02001ea4();

    f(a0, a1, a2);
}

/* Loader-relocated overlay calls: each symbol names the pre-relocation call
 * word the image holds. */

/* Call sites spelled through these wrappers pass their constants straight
 * into the argument registers; a direct call precomputes a costly constant
 * into a pseudo that the compiler then shares with later uses in the block.
 * A value-returning call also sets r0 last of its arguments. */
static __inline__ void Call1_020015b4(void (*f)(), s32 a0)
{
    extern u8 Data_03001ebc[];

    f(a0);
}

static __inline__ s32 Value1_020015b4(s32 (*f)(), s32 a0)
{
    extern u8 Data_03001ebc[];

    return f(a0);
}

/* The scene step counter at 0x1d8 of the shared scene work record. */
static __inline__ void bump_step_020015b4(s32 amount)
{
    extern u8 Data_03001ebc[];

    u8 *work = *(u8 **)Data_03001ebc;

    *(u16 *)(work + 0x1d8) = (u16)(*(u16 *)(work + 0x1d8) + amount);
}

/* Loader-relocated overlay calls: each symbol names the pre-relocation call
 * word the image holds. */

/* Resolved engine calls: each pseudo symbol is the per-site call word the
 * overlay image holds (a word can serve two sites with different targets),
 * and the macro names the engine function the site reaches through the
 * overlay veneer and the main-image veneer island, keeping the site's own
 * calling form. Names without a repository binding are provisional.
 */

/* Field at 0x1c8 of the shared scene work record. */

/* Field at 0x1c0 of the shared scene work record. */

/* Call sites spelled through these wrappers pass their constants straight
 * into the argument registers; a direct call precomputes a costly constant
 * into a pseudo that the compiler then shares with later uses in the block.
 * A value-returning call also sets r0 last of its arguments. */
static __inline__ void Call1_0200164c(void (*f)(), s32 a0)
{
    extern u8 Data_03001ebc[];


    f(a0);
}

static __inline__ s32 Value1_0200164c(s32 (*f)(), s32 a0)
{
    extern u8 Data_03001ebc[];


    return f(a0);
}

static __inline__ void Call2_0200164c(void (*f)(), s32 a0, s32 a1)
{
    extern u8 Data_03001ebc[];


    f(a0, a1);
}

static __inline__ void Call3_0200164c(void (*f)(), s32 a0, s32 a1, s32 a2)
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
static __inline__ void bump_step_0200164c(s32 amount)
{
    extern u8 Data_03001ebc[];


    u8 *work = *(u8 **)Data_03001ebc;

    *(u16 *)(work + 0x1d8) = (u16)(*(u16 *)(work + 0x1d8) + amount);
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
static __inline__ s32 Value0(s32 (*f)())
{
    extern u8 Data_03001ebc[];

    return f();
}

static __inline__ void Call1_02003768(void (*f)(), s32 a0)
{
    extern u8 Data_03001ebc[];

    f(a0);
}

static __inline__ s32 Value4(s32 (*f)(), s32 a0, s32 a1, s32 a2, s32 a3)
{
    extern u8 Data_03001ebc[];

    return f(a0, a1, a2, a3);
}

static __inline__ void Call1_02003864(void (*f)(), s32 a0)
{
    extern u8 Data_03001ebc[];

    f(a0);
}

static __inline__ void Call1_020026c0(void (*f)(), s32 a0)
{
    extern struct SceneWork *Data_03001ebc;

    f(a0);
}

static __inline__ void Call2_020026c0(void (*f)(), s32 a0, s32 a1)
{
    extern struct SceneWork *Data_03001ebc;

    f(a0, a1);
}

static __inline__ void Call6_020026c0(void (*f)(), s32 a0, s32 a1, s32 a2, s32 a3, s32 a4, s32 a5)
{
    extern struct SceneWork *Data_03001ebc;

    f(a0, a1, a2, a3, a4, a5);
}

void SceneState_ApplyValues12And2And3(void)
{
    Func_02003d96(0xC, 2, 3);
}

s32 SceneEffect_UpdateRandomAction(struct Resource3a8Effect *effect)
{
    u32 action;
    u32 next_timer;

    if (effect->action_timer == 0) {
        action = (u32)(SampleEffectAction() * 8) >> 0x10;
        switch (action) {
        case 0:
            RunEffectAction3(effect, 3);
            break;
        case 1:
            RunEffectAction4(effect, 4);
            break;
        case 3:
        case 4:
            effect->unknown_06 += (u32)(SampleEffectOffset() << 0xF) >> 0x10;
            break;
        }
        next_timer = (u32)(SampleEffectTimer() * 0x50) >> 0x10;
        effect->action_timer = (s16)next_timer;
        if (next_timer != 0) {
            goto decrement_timer;
        }
    } else {
decrement_timer:
        --effect->action_timer;
    }
    return 1;
}

s32 OverlayObject_AdvanceXWhenCounterExpires(struct Object *obj)
{
    s32 cnt = *(u16 *)&obj->cnt;
    s32 loaded = obj->cnt;
    s32 amount;

    if (loaded == 0) {
        obj->x += (u32)(Func_02003bec() << 15) >> 16;
        amount = (u32)(Func_02003bfa() * 80) >> 16;
        obj->cnt = amount;
        if (amount == 0) {
            goto done;
        }
        cnt = amount;
    }
    obj->cnt = cnt - 1;
done:
    return 1;
}

/* Contiguous unnamed leaf-owner run for resource_3a8. */
s32 SceneData_SelectTableByWord224(void)
{
    s16 v = Data_02000240[224];

    if (v == (s32)&Value_00000063) {
        return (s32)Data_0200c2c8;
    }
    if (v == (s32)&Value_00000066) {
        return (s32)Data_0200c448;
    }
    if (v == (s32)&Value_00000099) {
        return (s32)Data_0200c4a8;
    }
    if (v == (s32)&Value_0000009a) {
        return (s32)Data_0200c520;
    }
    if (v == (s32)&Value_0000009b) {
        return (s32)Data_0200c580;
    }
    if (v == (s32)&Value_0000009c) {
        return (s32)Data_0200c628;
    }
    return (s32)Data_0200c298;
}

s32 SceneData_ReturnZero(void)
{
    return 0;
}

u8 *SceneData_GetTablec6b8(void)
{
    return Data_0200c6b8;
}

s32 SceneData_SelectTableByWord224B(void)
{
    s16 v = Data_02000240[224];

    if (v == (s32)&Value_00000063) {
        return (s32)Data_0200c768;
    }
    if (v == (s32)&Value_00000066) {
        return (s32)Data_0200ca20;
    }
    if (v == (s32)&Value_00000099) {
        return (s32)Data_0200ca80;
    }
    if (v == (s32)&Value_0000009c) {
        return (s32)Data_0200cb58;
    }
    return (s32)Data_0200c750;
}

void FieldScene_RunStepWithValue29df(void)
{
    extern u8 *Data_03001ebc;

    Func_02003ddc();
    Func_02003dac(0x947, 1);
    Func_02003db4(0x29df, 1);
    Func_02003df8();
}

void FieldScene_RunStepWithValueFd6(void)
{
    extern u8 *Data_03001ebc;

    Func_02003e04();
    Func_02003e8e(12, 0, 0);
    Func_02003dfc(0xfd6);
    Func_02003f6c(181, 3);
    Func_02003e3c(181, 0);
    Func_02003e30();
}

s32 SceneData_SelectTableBySelector224(void)
{
    s16 v = Data_02000240[224];

    if (v == (s32)&Value_00000063) {
        return (s32)Data_0200cbf4;
    }
    if (v == (s32)&Value_00000066) {
        return (s32)Data_0200ce88;
    }
    if (v == (s32)&Value_00000099) {
        return (s32)Data_0200cedc;
    }
    if (v == (s32)&Value_0000009a) {
        return (s32)Data_0200cf24;
    }
    if (v == (s32)&Value_0000009b) {
        return (s32)Data_0200cf54;
    }
    if (v == (s32)&Value_0000009c) {
        return (s32)Data_0200cf9c;
    }
    return (s32)Data_0200cbe8;
}

void SceneDialogue_RunActorNineteenDialogue(void)
{
    Func_02003ec0();
    Func_02003f8e(0x1a7c);
    Func_02003fb6(19, 0);
    Func_02003eda();
}

void FieldScene_RunSlotZeroFacingSequence(void)
{
    extern u8 *Data_03001ebc;

    struct Obj *o;
    u32 v;
    u16 *q;

    o = Func_02003f0a(0);
    v = (o->f06 + 0xfffff000) << 16;
    if (v > 0x60000000) {
        Func_02003ef6();
        Func_02003fb0(0, 8, 0);
        Func_02003efe(10);
        Func_02003fd4(0x2584);
        Func_02003fe4(8, 0);
        if (Func_02003f3c(0, 0) == 0) {
            Func_02003fb8(8, 4);
            Func_02004008(8, 0);
        } else {
            q = (u16 *)(Data_03001ebc + 472);
            *q = *q + 1;
            Func_02003fda(8, 3);
            Func_0200402a(8, 0);
        }
        Func_02003f5e();
    }
}

void SceneDialogue_RunActorTenDialogue(void)
{
    Func_02003f6c_a();
    Func_0200403a(0x25b3);
    Func_02004062(10, 0);
    Func_02003f86();
}

void SceneState_BranchOnSlotZeroFacing(void)
{
    struct Obj *o;
    u32 v;

    o = Func_02003fb6_a(0);
    v = (o->f06 - 0x2000) << 16;
    if (v > 0x80000000) {
        Func_02004126(22, 22);
    } else {
        Func_02003fac();
        Func_0200407a(0x1acf);
        Func_02004092(22, 0);
        Func_02003fc6();
    }
}

void SceneDialogue_RunActorTwentyThreeByLeaderHeading(void)
{
    struct Obj *o;
    u32 v;

    o = Func_02003ffa(0);
    v = (o->f06 - 0x6001) << 16;
    if (v <= 0x7ffe0000) {
        Func_02004168(23, 23);
    } else {
        Func_02003fee();
        Func_020040bc(0x1ad1);
        Func_020040e4(23, 0);
        Func_02004008_a();
    }
}

void FieldScene_RunActorTwentyFourAngleDialogue(void)
{
    struct Obj *o;
    u32 v;

    o = Func_02004042(0);
    v = (o->f06 - 0x2000) << 16;
    if (v > 0xC0000000) {
        Func_020041b2(24, 24);
    } else {
        Func_02004038();
        Func_02004106(0x1ad5);
        Func_0200411e(24, 0);
        Func_02004052_a();
    }
}

void RunSceneArrivalSetup(void)
{
    extern u8 *Data_03001ebc;

    s32 two = 2;

    Func_020040fe();
    Func_0200428c(188);
    Func_020040be(36, 23, 43, 12, two, two);
    Func_0200403c(5);
    Func_020040d4(39, 23, 43, 12, two, two);
    Func_02004052(5);
    Func_02004178(0, 0x8000, 0x4000);
    Func_0200416e(0)[85] = 0;
    Func_020041dc(0, 2);
    SetOffset(0, 0, -8);
    Func_0200415e(10);
    Func_0200429c(2);
    Func_020042b0();
    Func_020042bc();
    Func_02004180();
}

void FieldScene_RunStagedGroupSequence(void)
{
    void Func_02001e6c();
    void Func_02001ea4();

    s32 messageId;
    const u32 *actionDescriptor;
    s32 idleState;
    s32 actionEnabled;
    u8 *actor20PairedWait;
    u8 *actor21PairedWait;
    u8 *closingWaitRecord;
    u8 *actor14FinalFacing;
    u8 *actor20FinalFacing;
    u8 *actor21FinalFacing;
    u8 *playerFinalFacing;
    u8 *actor21OpeningWait;
    u8 *actor20BeforeFirstMove;
    u8 *actor20AfterFirstMove;
    u8 *actor20BeforeSecondMove;
    u8 *actor20AfterSecondMove;
    u8 *sceneWorkspace;

    sceneWorkspace = *(u8 **)0x03001ebc;
    Func_02003bf4();
    if (Func_02003bd4(2320) == 0) {
        goto skip_scene;
    }
    if (Func_02003bd4(2321) != 0) {
        goto skip_scene;
    }
    Func_02003c04(33605960);
    Func_02003c74(20, 16515072, 17825792);
    Func_02003c74(27, 18612224, 17301504);
    Func_02003c74(28, 18612224, 18350080);
    Func_02003c74(29, 19660800, 17301504);
    Func_02003c74(30, 19660800, 18350080);
    Func_02003c74(32, 20709376, 17301504);
    Func_02003c74(31, 20709376, 18350080);
    Func_02003c74(33, 21757952, 17301504);
    Func_02003c74(34, 21757952, 18350080);
    Func_02003c74(21, 23855104, 17825792);
    Func_02003d7c(17);
    Func_02003cec(20);
    Func_02003bc4(6801, 1, 0);
    Func_02003d7c(9);
    Func_02003bec_a(10);
    Func_02003c9c(0, 2);
    if (*(s16 *)(sceneWorkspace + 364) == 9) {
        Func_02003d0c(157286, 19660);
        Func_02003ce4(0, 57344, 20);
    } else {
        Func_02003d0c(78643, 9830);
        Func_02003ce4(0, 0, 20);
    }
    Func_02003c2c(20, 72089, 36044);
    Func_02003c2c(27, 65536, 32768);
    Func_02003c2c(28, 65536, 32768);
    Func_02003c2c(29, 58982, 29491);
    Func_02003c2c(30, 58982, 29491);
    Func_02003c2c(32, 52428, 26214);
    Func_02003c2c(31, 52428, 26214);
    Func_02003c2c(33, 45875, 22937);
    Func_02003c2c(34, 45875, 22937);
    Func_02003c2c(21, 39321, 19660);
    actionDescriptor = SceneAction_GroupMotion;
    Func_02003c34(20, actionDescriptor);
    Func_02003c34(27, actionDescriptor);
    Func_02003c34(28, actionDescriptor);
    Func_02003c34(29, actionDescriptor);
    Func_02003c34(30, actionDescriptor);
    Func_02003c34(32, actionDescriptor);
    Func_02003c34(31, actionDescriptor);
    Func_02003c34(33, actionDescriptor);
    Func_02003c34(34, actionDescriptor);
    actor21OpeningWait = Func_02003c1c(21);
    {
        s32 value = 0;
        *(u16 *)(actor21OpeningWait + 100) = value;
    }
    Func_02003c34(21, actionDescriptor);
    Func_02003d14(12189696, -1, 17825792, 1);
    Func_02003c3c(20);
    Func_02003ce4(20, 0, 0);
    do {
        Func_02003b14(1);
    } while (*(s16 *)((u8 *)actor21OpeningWait + 100) == 0);
    Func_02003bec_a(40);
    Func_02003c9c(27, 2);
    Func_02003ce4(27, 20480, 20);
    Func_02003cbc(6802);
    Func_02003cd4(27, 0, 10);
    Func_02003c9c(28, 2);
    Func_02003ce4(28, 45056, 10);
    Func_02003c7c(28, 3);
    Func_02003cd4(28, 0, 10);
    Func_02003d04(32, 258);
    Func_02003bec_a(40);
    Func_02003cd4(32, 0, 10);
    Func_02003cfc(31, 256, 40);
    Func_02003ce4(31, 45056, 10);
    Func_02003cd4(31, 0, 10);
    Func_02003ce4(31, 32768, 10);
    Func_02003c9c(31, 2);
    Func_02003c7c(31, 4);
    Func_02003cd4(31, 0, 10);
    Func_02003ce4(31, 45056, 0);
    Func_02003ce4(32, 20480, 20);
    Func_02003c7c(31, 3);
    Func_02003c84(32, 3);
    Func_02003c9c(20, 2);
    Func_02003d04(20, 258);
    Func_02003bec_a(40);
    Func_02003cd4(20, 0, 10);
    Func_02003c9c(20, 2);
    actor20BeforeFirstMove = Func_02003c1c(20);
    {
        s32 flags = actor20BeforeFirstMove[90] & 0xfe;
        idleState = 0;
        actor20BeforeFirstMove[90] = flags;
    }
    Func_02003c5c(20, 172, 264);
    Func_02003bec_a(1);
    actor20AfterFirstMove = Func_02003c1c(20);
    actionEnabled = 1;
    {
        s32 flags = actor20AfterFirstMove[90];
        flags |= actionEnabled;
        actor20AfterFirstMove[90] = flags;
    }
    Func_02003ce4(27, 32768, 0);
    Func_02003ce4(28, 32768, 0);
    Func_02003ce4(32, 32768, 0);
    Func_02003ce4(31, 32768, 20);
    Func_02003c84(20, 3);
    Func_02003bec_a(20);
    actor20BeforeSecondMove = Func_02003c1c(20);
    *(u8 *)((u8 *)(actor20BeforeSecondMove) + 90) &= 0xfe;
    Func_02003c5c(20, 172, 272);
    Func_02003bec_a(1);
    actor20AfterSecondMove = Func_02003c1c(20);
    actionEnabled |= actor20AfterSecondMove[90];
    actor20AfterSecondMove[90] = actionEnabled;
    Func_02003c5c(20, 180, 272);
    Func_02003ce4(20, 0, 0);
    Func_02003cd4(20, 0, 10);
    Func_02003cfc(34, 261, 0);
    Func_02003c9c(34, 1);
    Func_02003c84(34, 3);
    Func_02003cd4(34, 0, 10);
    Func_02003c9c(33, 1);
    Func_02003cd4(33, 0, 10);
    Func_02003c7c(33, 4);
    Func_02003cd4(33, 0, 10);
    Func_02003c9c(21, 2);
    Func_02003cfc(21, 258, 0);
    Func_02003cd4(21, 0, 10);
    Func_02003c8c(20, 2, 20);
    Func_02003c8c(20, 4, 40);
    Func_02003c9c(20, 2);
    Func_02003cd4(20, 0, 10);
    Func_02003c2c(21, 104857, 52428);
    Func_02003c5c(21, 265, 282);
    Func_02003c5c(21, 251, 284);
    Func_02003c5c(21, 246, 296);
    Func_02003ce4(21, 49152, 0);
    Func_02001e6c();
    Func_02003bec_a(40);
    Func_02003c2c(21, 104857, 52428);
    Func_02003c5c(21, 228, 296);
    Func_02003ce4(21, 49152, 40);
    Func_02003c5c(21, 212, 296);
    Func_02003ce4(21, 49152, 40);
    Func_02003c5c(21, 192, 296);
    Func_02003ce4(21, 49152, 40);
    Func_02003c94(21, 2);
    Func_02003cfc(21, 256, 60);
    Func_02003ce4(20, 12288, 0);
    Func_02003c5c(21, 184, 286);
    Func_02003ce4(21, 45056, 10);
    Func_02003d04(21, 258);
    Func_02003bec_a(40);
    Func_02003c84(21, 4);
    Func_02003cfc(20, 257, 40);
    Func_02003c84(20, 3);
    Func_02003c9c(20, 2);
    Func_02003ce4(20, 0, 0);
    Func_02003ce4(21, 0, 60);
    Func_02003ce4(20, 12288, 0);
    Func_02003ce4(21, 45056, 10);
    Func_02003c84(20, 3);
    Func_02003c84(21, 3);
    Func_02003ce4(21, 0, 0);
    Func_02003c2c(20, 104857, 52428);
    Func_02003c4c(20, 33603504);
    Func_02003c5c(20, 228, 296);
    Func_02003ce4(20, 49152, 40);
    Func_02003c5c(20, 212, 296);
    Func_02003ce4(20, 49152, 40);
    Func_02003c5c(20, 192, 296);
    Func_02003ce4(20, 49152, 40);
    Func_02003ce4(20, 45056, 0);
    Func_02003ce4(21, 12288, 10);
    Func_02003d04(20, 258);
    Func_02003bec_a(60);
    Func_02003c84(20, 4);
    messageId = (s32)Value_00001a9e;
    Func_02003cbc(messageId);
    Func_02003cd4(20, 0, 40);
    Func_02001ed8();
    Func_02003c5c(20, 178, 272);
    Func_02003ce4(20, 0, 0);
    Func_02003bec_a(240);
    Func_02003c44(27);
    Func_02003b14(1);
    Func_02003ce4(27, 32768, 10);
    Func_02003cfc(27, 257, 60);
    Func_02003cd4(27, 0, 10);
    Func_02001ea4(27);
    Func_02003bec_a(80);
    Func_02003c44(28);
    Func_02003b14(1);
    Func_02003ce4(28, 53248, 20);
    Func_02003c9c(28, 2);
    Func_02003cd4(28, 0, 10);
    Func_02001ea4(28);
    Func_02003bec_a(160);
    Func_02003c44(32);
    Func_02003b14(1);
    Func_02003ce4(32, 20480, 10);
    Func_02003cfc(32, 257, 60);
    Func_02003cd4(32, 0, 10);
    Func_02001ea4(32);
    Func_02003bec_a(80);
    Func_02003c44(30);
    Func_02003b14(1);
    Func_02003ce4(30, 45056, 10);
    Func_02003c9c(30, 1);
    Func_02003cbc(messageId + 6);
    Func_02003cd4(30, 0, 10);
    Func_02003b24(33595157);
    Func_02003c44(20);
    Func_02003c44(21);
    Func_02003b14(1);
    actor20PairedWait = Func_02003c1c(20);
    *(u16 *)(actor20PairedWait + 100) = idleState;
    actor21PairedWait = Func_02003c1c(21);
    *(u16 *)(actor21PairedWait + 100) = idleState;
    Func_02003c2c(20, 52428, 26214);
    Func_02003c2c(21, 52428, 26214);
    Func_02003c34(20, 33603636);
    Func_02003c34(21, 33603788);
    Func_02003c44(29);
    Func_02003b14(1);
    Func_02003ce4(29, 20480, 10);
    messageId += 5;
    Func_02003c9c(29, 2);
    Func_02003cbc(messageId);
    Func_02003cd4(29, 0, 20);
    Func_02001ea4(29);
    Func_02001ea4(30);
    do {
        Func_02003b14(1);
    } while (*(s16 *)((u8 *)Func_02003c1c(20) + 100) == 0 ||
             *(s16 *)((u8 *)Func_02003c1c(21) + 100) != 1);
    Func_02003c34(20, 33603940);
    Func_02003c34(21, 33604012);
    Func_02003c44(31);
    Func_02003b14(1);
    Func_02003ce4(31, 0x5000, 10);
    Func_02003c9c(31, 1);
    Func_02003c84(31, 4);
    messageId = (s32)Value_00001aa2;
    Func_02003cbc(messageId);
    Func_02003cd4(31, 0, 10);
    Func_02001ea4(31);
    Func_02003c44(34);
    Func_02003c44(33);
    Func_02003b14(1);
    Func_02003cfc(34, 261, 40);
    Func_02003cfc(33, 261, 60);
    Func_02003ce4(34, 45056, 10);
    Func_02003ce4(33, 20480, 10);
    messageId += 3;
    Func_02003c84(34, 4);
    Func_02003cbc(messageId);
    Func_02003cd4(34, 0, 10);
    Func_02003c9c(33, 1);
    Func_02003c7c(33, 4);
    Func_02003cd4(33, 0, 10);
    Func_02003cfc(34, 258, 60);
    Func_02003cfc(20, 259, 0);
    Func_02003c9c(20, 2);
    Func_02003cbc(6834);
    Func_02003cd4(20, 0, 10);
    Func_02003c44(27);
    Func_02003c44(28);
    Func_02003c44(29);
    Func_02003c44(30);
    Func_02003c44(32);
    Func_02003c44(31);
    Func_02003c44(33);
    Func_02003c44(34);
    Func_02003c44(20);
    Func_02003c44(21);
    Func_02003b14(1);
    Func_02003c8c(27, 2, 0);
    Func_02003c8c(28, 2, 0);
    Func_02003c8c(29, 2, 0);
    Func_02003c8c(30, 2, 0);
    Func_02003c8c(32, 2, 0);
    Func_02003c8c(31, 2, 0);
    Func_02003c8c(33, 2, 0);
    Func_02003c8c(34, 2, 0);
    Func_02003c8c(21, 2, 40);
    Func_02003ce4(27, 32768, 0);
    Func_02003ce4(28, 32768, 0);
    Func_02003ce4(29, 32768, 0);
    Func_02003ce4(30, 32768, 0);
    Func_02003ce4(32, 32768, 0);
    Func_02003ce4(31, 32768, 0);
    Func_02003ce4(33, 32768, 0);
    Func_02003ce4(34, 32768, 40);
    Func_02003c8c(21, 4, 40);
    Func_02003cd4(21, 0, 10);
    Func_02003c9c(20, 1);
    Func_02003cd4(20, 0, 10);
    Func_02003c84(21, 3);
    Func_02003cd4(21, 0, 10);
    Func_02003c84(20, 3);
    Func_02003cd4(20, 0, 10);
    Func_02003cfc(27, 258, 40);
    Func_02003c94(27, 1);
    Func_02003cd4(27, 0, 10);
    Func_02003cfc(28, 258, 40);
    Func_02003cd4(28, 0, 10);
    Func_02003c84(21, 4);
    Func_02003bec_a(40);
    Func_02003c84(21, 3);
    Func_02003cd4(21, 0, 20);
    Func_02003c84(20, 3);
    Func_02003cd4(20, 0, 10);
    Func_02003ce4(27, 20480, 0);
    Func_02003ce4(28, 45056, 4);
    Func_02003ce4(29, 20480, 0);
    Func_02003ce4(30, 45056, 4);
    Func_02003ce4(32, 20480, 0);
    Func_02003ce4(31, 45056, 4);
    Func_02003ce4(33, 20480, 0);
    Func_02003ce4(34, 45056, 4);
    Func_02003c7c(27, 3);
    Func_02003c84(28, 3);
    Func_02003c7c(29, 3);
    Func_02003c84(30, 3);
    Func_02003c7c(32, 3);
    Func_02003c84(31, 3);
    Func_02003c7c(33, 3);
    Func_02003c84(34, 3);
    Func_02003c8c(20, 2, 40);
    Func_02003cd4(20, 0, 10);
    Func_02003ce4(27, 32768, 0);
    Func_02003ce4(28, 32768, 4);
    Func_02003ce4(29, 32768, 0);
    Func_02003ce4(30, 32768, 4);
    Func_02003ce4(32, 32768, 0);
    Func_02003ce4(31, 32768, 4);
    Func_02003ce4(33, 32768, 0);
    Func_02003ce4(34, 32768, 4);
    Func_02003c2c(20, 72089, 36044);
    Func_02003c2c(27, 68812, 34406);
    Func_02003c2c(28, 68812, 34406);
    Func_02003c2c(29, 65536, 32768);
    Func_02003c2c(30, 65536, 32768);
    Func_02003c2c(32, 0xf333, 0x7999);
    Func_02003c2c(31, 0xf333, 0x7999);
    Func_02003c2c(33, 58982, 29491);
    Func_02003c2c(34, 58982, 29491);
    Func_02003c2c(21, 55705, 27852);
    Func_02003cf4(27, 1);
    Func_02003cf4(28, 1);
    Func_02003cf4(29, 1);
    Func_02003cf4(30, 1);
    Func_02003cf4(32, 1);
    Func_02003cf4(31, 1);
    Func_02003cf4(33, 1);
    Func_02003cf4(34, 1);
    Func_02003cf4(20, 1);
    Func_02003cf4(21, 1);
    Func_02003c44(27);
    Func_02003c44(28);
    Func_02003c44(29);
    Func_02003c44(30);
    Func_02003c44(32);
    Func_02003c44(31);
    Func_02003c44(33);
    Func_02003c44(34);
    Func_02003c44(20);
    Func_02003c44(21);
    Func_02003b14(1);
    actionDescriptor = SceneAction_GroupOffsetMotion;
    Func_02003c34(20, actionDescriptor);
    Func_02003c34(27, actionDescriptor);
    Func_02003c34(28, actionDescriptor);
    Func_02003c34(29, actionDescriptor);
    Func_02003c34(30, actionDescriptor);
    Func_02003c34(32, actionDescriptor);
    Func_02003c34(31, actionDescriptor);
    Func_02003c34(33, actionDescriptor);
    Func_02003c34(34, actionDescriptor);
    closingWaitRecord = Func_02003c1c(21);
    {
        u16 *state = (u16 *)(closingWaitRecord + 100);
        s32 value = 0;
        *state = value;
    }
    Func_02003c34(21, actionDescriptor);
    do {
        Func_02003b14(1);
    } while (*(s16 *)((u8 *)Func_02003c1c(21) + 100) != 1);
    Func_02003bec_a(80);
    Func_02003c74(14, 22282240, 17956864);
    Func_02003b14(1);
    Func_02003c2c(14, 65536, 32768);
    Func_02003c5c(14, 224, 274);
    Func_02003ce4(14, 0, 40);
    Func_02003ce4(14, 32768, 40);
    Func_02003ce4(14, 49152, 40);
    Func_02003ce4(14, 20480, 40);
    Func_02003cfc(14, 257, 60);
    Func_02003cd4(14, 0, 10);
    Func_02003ce4(14, 0, 40);
    Func_02003ce4(14, 49152, 40);
    Func_02003ce4(14, 32768, 40);
    Func_02003d04(14, 258);
    Func_02003c8c(14, 4, 40);
    Func_02003cd4(14, 0, 20);
    Func_02003c9c(14, 2);
    Func_02003cd4(14, 0, 10);
    Func_02003c8c(14, 4, 40);
    Func_02003c2c(14, 78643, 39321);
    closingWaitRecord = Func_02003c1c(14);
    {
        u16 *state = (u16 *)(closingWaitRecord + 100);
        s32 value = 0;
        *state = value;
    }
    Func_02003c34(14, SceneAction_GroupOffsetMotion);
    do {
        Func_02003b14(1);
    } while (*(s16 *)((u8 *)Func_02003c1c(14) + 100) != 1);
    Func_02003c74(14, 23527424, 20578304);
    actor14FinalFacing = Func_02003c1c(14);
    {
        s32 value = 53248;
        *(u16 *)(actor14FinalFacing + 6) = value;
    }
    Func_02003c74(20, 29818880, 28442624);
    actor20FinalFacing = Func_02003c1c(20);
    {
        s32 value = 53248;
        *(u16 *)(actor20FinalFacing + 6) = value;
    }
    Func_02003c74(21, 30408704, 27262976);
    actor21FinalFacing = Func_02003c1c(21);
    {
        s32 value = 20480;
        *(u16 *)(actor21FinalFacing + 6) = value;
    }
    Func_02003c24(27);
    Func_02003c24(28);
    Func_02003c24(29);
    Func_02003c24(30);
    Func_02003c24(31);
    Func_02003c24(32);
    Func_02003c24(33);
    Func_02003c24(34);
    Func_02003d7c(17);
    if (*(s16 *)(sceneWorkspace + 364) == 9) {
        Func_02003c5c(0, 224, 458);
        playerFinalFacing = Func_02003c1c(0);
        {
            s32 value = 49152;
            *(u16 *)(playerFinalFacing + 6) = value;
        }
    } else {
        Func_02003c5c(0, 40, 248);
        playerFinalFacing = Func_02003c1c(0);
        {
            s32 value = 16384;
            *(u16 *)(playerFinalFacing + 6) = value;
        }
    }
    Func_02003d54();
    Func_02003bdc(0x911);
    goto finish;
skip_scene:
    Func_02003d7c(123);
    Func_02003d24(*(s16 *)(sceneWorkspace + 364));
    Func_02003d34_a();
    Func_02003d3c();
finish:
    Func_02003bfc();
}

void SceneState_ClearWord24WhenFlag200(void)
{
    extern u8 Data_03001ebc[];

    u32 i;
    s32 record;

    if (Value1(Func_02005168, 0x200) != 0) {
        *(volatile s32 *)(*(volatile s32 *)0x03001ee0 + 24) = 0;
        Call1(Func_0200518c, 0x200);
    }
}

void SceneState_LinkRecordZeroWhenFlag200Clear(void)
{
    extern u8 Data_03001ebc[];

    u32 i;
    s32 rec;
    u8 *work;

    if (Value1_020015b4(Func_02005190, 0x200) == 0) {
        work = *(volatile s32 *)0x03001ee0;
        rec = Value1_020015b4(Func_020051e6, 0);
        *(volatile s32 *)((s32)work + 24) = rec;
        Call1_020015b4(Func_020051b0, 0x200);
    }
}

void SceneActor_SetActorZeroByte35Bit0(void)
{
    u8 *p;
    s32 v;

    p = (u8 *)(Func_02005202(0) + 35);
    v = 1;
    v |= *p;
    *p = v;
}

void SceneState_ClearRecordZeroBit0At35(void)
{
    u8 *p;
    s32 v;

    p = (u8 *)(Func_0200521a(0) + 35);
    v = 254;
    v &= *p;
    *p = v;
}

void SceneDialogue_RunActor181Line916(void)
{
    Func_0200513a((s32)Data_0200b90d);
    Func_02005130(1);
    Func_0200529a(26, 0, 0);
    Func_02005208(0x916);
    Func_02005378(181, 3);
    Func_02005248(181, 0);
}

s32 FieldScene_DispatchBySceneId(void)
{
    extern u8 Data_03001ebc[];

    extern s16 Data_02000240[];

    s32 v;

    Func_0200522e(0x87a);
    v = Data_02000240[224];
    if (v == (s32)&Value_00000063) {
        Func_02002d24();
    } else if (v == (s32)&Value_00000066) {
        Func_02002e5c();
    } else if (v == (s32)&Value_00000099) {
        Func_02002ed8();
    } else if (v == (s32)&Value_0000009b) {
        Func_02002f30();
    } else if (v == (s32)&Value_0000009c) {
        Func_02002fc8();
    }
    return 0;
}

void SceneState_CheckFlags941And940(void)
{
    if (Func_020053c2(0x941) != 0) {
        Func_020053d4(0x321);
        Func_020053da(0x913);
        Func_020053e0(0x912);
        Func_020053e6(0x915);
    }
    if (Func_020053e6_a(0x940) != 0) {
        Func_020053f8(0x321);
    }
    if (Data_02000240[225] != 0) {
        if (Func_02005406(0x912) == 0) {
            Func_02003842();
        }
    }
}

void SceneState_ApplyFlagGatedActorEightSetup(void)
{
    struct Obj *o;

    if (Func_02005432(0xfd6) == 0) {
        Func_020052a4(12);
    }
    if (Func_02005442(0x915) != 0) {
        o = Func_02005494(8);
        o->f06 = 0;
    }
    if (Data_02000240[225] == 10) {
        Func_02003f50();
    }
}

void SceneState_SetWork448AndRunFlag915Step(void)
{
    extern u8 *Data_03001ebc;

    *(s32 *)(Data_03001ebc + 448) = 516;
    if (Func_0200548e(0x915) != 0) {
        s32 k = 2;
        Func_0200546e(58, 5, 58, 8, k, 3);
        { s32 a = 8, b = 10; Func_0200548a(8, 11, 2, 1, a, b); }
        Func_02005494_a(8, 12, 8, 11, k, 1);
        Func_02005478();
        Func_02005416(1);
    }
    if (Data_02000240[225] <= 3) {
        Func_0200567c(170);
    }
}

void FieldScene_RunMiddleSequence(void)
{
    extern u8 Data_03001ebc[];


    u32 i;
    s32 rec7;
    u8 *record;
    s32 r0;
    s32 v5;
    u8 *p5;
    u8 *volatile *base = (u8 *volatile *)Data_03001ebc;

    *(s32 *)(base[0] + 0x1c0) = 0x204;
    Func_02005692(0);
    rec7 = Value1_0200164c(Func_02005520, 0x109);
    if (rec7 != 0) {
        p5 = base[9];
        r0 = Value1_0200164c(Func_02005530, 0x200);
        if (r0 != 0) {
            r0 = Value1_0200164c(Func_02005582, 0);
        }
        *(volatile s32 *)(p5 + 24) = r0;
    } else {
        Call1_0200164c(Func_0200554e, 0x200);
        if (Data_02000240_t[225][0] == 4) {
            *(volatile s32 *)(base[9] + 24) = rec7;
            Call1_0200164c(Func_02005572, 0x200);
        }
    }
    if (Value1_0200164c(Func_02005568, 0x302) != 0) {
        Call3_0200164c(Func_0200561a, 11, 0x960000, 0x2d80000);
        if (Value1_0200164c(Func_02005580, 0x201) != 0) {
            Func_020055d2(11);
            v5 = 9;
            Func_0200563a(11, 5);
            Call6(Func_02005576, 0, 0, 1, 1, v5, 14);
            Call6(Func_02005588, 0, 0, 1, 1, v5, 45);
            {
                u8 *record = Func_02005606(11);
                u8 value = *(volatile u8 *)&record[35];

                record[35] = (u8)(value | 2);
            }
        }
    }
    record = Func_02005616(8);
    Func_020055b4((s32)record, 0);
    record = Func_02005622(9);
    Func_020055c0((s32)record, 0);
    Call2_0200164c(Func_02005532, 0x200b769, 0xc80);
    if (Value1_0200164c(Func_020055f0, 0x915) != 0) {
        Call3_0200164c(Func_020056a0, 10, 0x1aa0000, 0x2da0000);
        record = Func_0200564e(10);
        {
            s32 shown = 0x5000;

            *(volatile u16 *)((s32)record + 6) = shown;
        }
        Call6(Func_020055e8, 88, 48, 88, 45, 2, 3);
        Call6(Func_020055fa, 24, 49, 24, 48, 2, 1);
        Call6(Func_0200560a, 25, 42, 25, 47, 1, 1);
        Call6(Func_02005626, 22, 50, 2, 1, 24, 49);
    }
    if (Value1_0200164c(Func_0200565c, 0x302) == 0) {
    } else {
        Call3_0200164c(Func_02005710, 8, 0xe80000, 0x2dc0000);
        Call6(Func_02005654, 7, 44, 1, 1, 0, 1);
        Call6(Func_0200565e, 74, 58, 78, 41, 1, 5);
        Call6(Func_02005672, 16, 109, 13, 109, 3, 2);
        Call6(Func_02005682, 67, 64, 71, 44, 1, 2);
        Call6(Func_02005692_a, 67, 64, 72, 44, 1, 2);
        Call6(Func_020056a2, 67, 68, 73, 43, 1, 2);
        Call6(Func_020056b2, 67, 68, 74, 43, 1, 2);
        Call6(Func_020056c2, 67, 64, 75, 44, 1, 2);
        Call6(Func_020056d2, 67, 66, 76, 44, 1, 2);
        Call6(Func_020056e2, 67, 64, 77, 44, 1, 2);
        Call6(Func_020056f2, 67, 64, 78, 44, 1, 2);
        Call6(Func_02005702, 67, 64, 79, 44, 1, 2);
        Call6(Func_02005712, 67, 66, 80, 44, 1, 2);
        Call6(Func_02005722, 2, 0, 9, 42, 2, 2);
        Call6(Func_02005732, 68, 64, 71, 44, 1, 2);
        Call6(Func_02005742, 68, 64, 72, 44, 1, 2);
        Call6(Func_02005752, 68, 68, 73, 43, 1, 2);
        Call6(Func_02005762, 68, 68, 74, 43, 1, 2);
        Call6(Func_02005772, 68, 64, 75, 44, 1, 2);
        Call6(Func_02005782, 68, 66, 76, 44, 1, 2);
        Call6(Func_02005792, 68, 64, 77, 44, 1, 2);
        Call6(Func_020057a2, 68, 64, 78, 44, 1, 2);
        Call6(Func_020057b2, 68, 64, 79, 44, 1, 2);
        Call6(Func_020057c2, 68, 66, 80, 44, 1, 2);
        Call6(Func_020057d2, 4, 0, 9, 42, 2, 2);
        Call6(Func_020057e6, 7, 11, 7, 42, 10, 8);
        Call6(Func_020057f8, 71, 12, 71, 43, 10, 13);
        Call6(Func_02005814, 6, 13, 12, 12, 6, 44);
        Call6(Func_02005826, 0, 1, 1, 1, 7, 44);
        goto L_02001cc2;
    }
    switch (Data_02000240_t[225][0]) {
    case 1:
    case 2:
        Func_02005a24(170);
        break;
    }
    L_02001cc2:;
    if (Value1_0200164c(Func_0200589a, 0x303) == 0) {
    } else {
        Call3_0200164c(Func_0200594e, 9, 0x2b80000, 0x2dc0000);
        Call6(Func_0200588a, 74, 58, 107, 41, 1, 5);
        Call6(Func_0200589e, 45, 109, 42, 109, 3, 2);
        Call6(Func_020058ae, 67, 64, 102, 44, 1, 2);
        Call6(Func_020058be, 67, 64, 103, 44, 1, 2);
        Call6(Func_020058ce, 67, 64, 104, 44, 1, 2);
        Call6(Func_020058de, 67, 66, 105, 44, 1, 2);
        Call6(Func_020058ee, 67, 64, 106, 44, 1, 2);
        Call6(Func_020058fe, 67, 64, 107, 44, 1, 2);
        Call6(Func_0200590e, 67, 64, 108, 44, 1, 2);
        Call6(Func_0200591e, 67, 66, 109, 44, 1, 2);
        Call6(Func_0200592e, 68, 64, 102, 44, 1, 2);
        Call6(Func_0200593e, 68, 64, 103, 44, 1, 2);
        Call6(Func_0200594e_a, 68, 64, 104, 44, 1, 2);
        Call6(Func_0200595e, 68, 66, 105, 44, 1, 2);
        Call6(Func_0200596e, 68, 64, 106, 44, 1, 2);
        Call6(Func_0200597e, 68, 64, 107, 44, 1, 2);
        Call6(Func_0200598e, 68, 64, 108, 44, 1, 2);
        Call6(Func_0200599e, 68, 66, 109, 44, 1, 2);
        Call6(Func_020059b2, 38, 14, 38, 44, 8, 4);
        Call6(Func_020059c4, 102, 14, 102, 44, 8, 12);
        Call6(Func_020059e0, 37, 13, 10, 12, 37, 43);
        goto L_02001e5a;
    }
    switch (Data_02000240_t[225][0]) {
    case 3:
    case 4:
        Func_02005bbc(170);
        break;
    }
    L_02001e5a:;
}

void SceneState_SetValues27Through34(void)
{
    Func_02003d16(27);
    Func_02003d1c(28);
    Func_02003d22(29);
    Func_02003d28(30);
    Func_02003d2e(32);
    Func_02003d34(31);
    Func_02003d3a(33);
    Func_02003d40(34);
}

void SceneEffect_SetSlotVariantAndDescriptor(s32 a)
{
    struct Obj_02000040 *p;
    u32 t;

    p = Func_02005ac6(a);
    p->f64 = a;
    t = Func_020059e2();
    p->f66 = (t * 5) >> 12;
    Func_02005a2e(p, (s32)Data_0200beac);
}

/* Runs a scripted sequence for two actors (8 and 9): sets up their sprite
 * records, moves and animates them in lockstep through a series of timed
 * steps, then hands off to a third actor (2) and a couple of standalone
 * calls (5) before advancing the shared scene step counter and phase word. */
void FieldScene_RunTwoActorCutsceneSequence(void)
{
    extern u8 Data_03001ebc[];

    u32 i;
    u8 *record;

    BattleRuntime_Reset_1();
    ObjectMotion_PlaceWithinCameraBounds_1(-1, -1, -1, 0);
    Func_02005b3e(1);
    ObjectMotion_SetHorizontalPositionWithTerrain_1(0, 0xc00000, 0x1560000);
    Func_02005b52(1);
    ObjectMotion_SetSpeedLimitAndAcceleration_1(0x3333, 0x666);
    ObjectMotion_PlaceWithinCameraBounds_2(0xc00000, -1, 0xfc0000, 1);
    SCENE_FIELD_1C8 = 40;
    BattleRuntime_WaitIfModeZero_1();
    ObjectMotion_SetSpeedParameters_1(0, 0xcccc, 0x6666);
    ObjectMotion_SetPositionAndReset_1(0, 192, 0x116);
    ObjectMotion_SetSpeedParameters_2(8, 0x10000, 0x8000);
    ObjectMotion_SetSpeedParameters_3(9, 0x10000, 0x8000);
    record = Scene_GetRecord_1(8);
    {
        /* Field at +6 of the record: a visibility/state word. */
        s32 shown = 0x3000;

        *(u16 *)(record + 6) = shown;
    }
    record = Scene_GetRecord_2(9);
    {
        /* Field at +6 of the record: a visibility/state word. */
        s32 shown = 0x5000;

        *(u16 *)(record + 6) = shown;
    }
    Func_02005bd0(1);
    /* Clear bit 0 of the flag byte at +90. */
    *(u8 *)(Scene_GetRecord_3(8) + 90) &= 254;
    *(u8 *)(Scene_GetRecord_4(9) + 90) &= 254;
    ObjectMotion_ResetAndSetPositionInMode2_1(8, 184, 232);
    ObjectMotion_SetPositionAndReset_2(9, 198, 232);
    Object_SetModeById_1(8, 1);
    BattleRuntime_WaitIfModeZero_2(20);
    {
        /* Set bit 0 of the flag byte at +90. */
        u8 *record = Scene_GetRecord_5(8);
        u8 flags = *(volatile u8 *)&record[90];

        record[90] = (u8)(flags | 1);
    }
    {
        u8 *record = Scene_GetRecord_6(9);
        u8 flags = *(volatile u8 *)&record[90];

        record[90] = (u8)(flags | 1);
    }
    BattleRuntime_WaitIfModeZero_3(20);
    ObjectMotion_CallThenWaitForAnimationChange_1(8, 4);
    SceneWork_SetStepValue_1(0x1b05);
    BattleRuntime_RunThenWaitIfModeZero_1(8, 0, 10);
    ObjectMotion_ArmCallback_1(9, 0x5000, 10);
    ObjectMotion_CallThenWaitForAnimationChange_2(9, 3);
    BattleRuntime_RunThenWaitIfModeZero_2(9, 0, 10);
    BattleRuntime_WaitIfModeZero_4(0, 0x102);
    BattleRuntime_WaitIfModeZero_5(60);
    record = Scene_GetRecord_7(0);
    if (record != 0) {
        ObjectMotion_SetHorizontalPositionWithTerrain_2(2, *(s32 *)(record + 8), *(s32 *)(record + 16));
    }
    ObjectMotion_SetSpeedParameters_4(2, 0xcccc, 0x6666);
    ObjectMotion_SetPositionAndReset_3(2, 212, 0x10c);
    ObjectMotion_ArmCallback_2(0, 0xe000, 0);
    ObjectMotion_ArmCallback_3(2, 0x6000, 20);
    BattleRuntime_RunThenWaitIfModeZero_3(0x4002, 0, 20);
    ObjectMotion_ArmCallback_4(0, 0xc000, 0);
    ObjectMotion_SetPositionAndReset_4(2, 202, 254);
    BattleRuntime_WaitIfModeZero_6(20);
    ObjectMotion_CallThenWaitForAnimationChange_3(2, 3);
    BattleRuntime_WaitIfModeZero_7(10);
    BattleRuntime_RunThenWaitIfModeZero_4(2, 0, 10);
    BattleEffect_SpawnLinkedResourceObject_1(8, 0x101, 0);
    BattleEffect_SpawnLinkedResourceObject_2(9, 0x101, 40);
    ObjectMotion_ArmCallback_5(8, 0, 0);
    ObjectMotion_ArmCallback_6(9, 0x8000, 40);
    ObjectMotion_ArmCallback_7(8, 0x3000, 0);
    ObjectMotion_ArmCallback_8(9, 0x3000, 20);
    BattleEffect_SpawnLinkedResourceObject_3(2, 0x102, 60);
    ObjectMotion_SetVariantCallbackAndInvokeObject_1(2, 1);
    BattleRuntime_RunThenWaitIfModeZero_5(2, 0, 10);
    ObjectMotion_SetVariantCallbackAndInvokeObject_2(8, 2);
    BattleRuntime_RunThenWaitIfModeZero_6(8, 0, 10);
    BattleEffect_SpawnLinkedResourceObject_4(8, 0x105, 60);
    ObjectMotion_SetVariantCallback_1(8, 2);
    BattleRuntime_RunThenWaitIfModeZero_7(8, 0, 10);
    ObjectMotion_SetVariantCallbackAndInvokeObject_3(9, 2);
    BattleRuntime_RunThenWaitIfModeZero_8(9, 0, 20);
    ObjectMotion_SetVariantCallbackAndInvokeObject_4(2, 1);
    BattleRuntime_WaitIfModeZero_8(10);
    ObjectMotion_ArmCallback_9(2, 0x6000, 10);
    BattleEventRuntime_ProcessAction_1(0x4002, 0);
    BattleRuntime_WaitIfModeZero_9(10);
    ObjectMotion_SetVariantCallbackAndInvokeObject_5(8, 1);
    ObjectMotion_CallThenWaitForAnimationChange_4(8, 4);
    BattleRuntime_RunThenWaitIfModeZero_9(8, 0, 10);
    BattleEffect_SpawnLinkedResourceObject_5(2, 0x101, 40);
    ObjectMotion_ArmCallback_10(2, 0xa000, 20);
    ObjectMotion_SetVariantCallbackAndInvokeObject_6(9, 1);
    ObjectMotion_CallThenWaitForAnimationChange_5(9, 3);
    ObjectMotion_ArmCallback_11(2, 0xc000, 0);
    BattleRuntime_RunThenWaitIfModeZero_10(9, 0, 10);
    ObjectMotion_ArmCallback_12(2, 0x6000, 60);
    ObjectMotion_ArmCallback_13(2, 0xc000, 10);
    BattleRuntime_RunThenWaitIfModeZero_11(2, 0, 10);
    ObjectMotion_ArmCallback_14(8, 0, 0);
    ObjectMotion_ArmCallback_15(9, 0x8000, 40);
    ObjectMotion_ArmCallback_16(8, 0x3000, 0);
    ObjectMotion_ArmCallback_17(9, 0x3000, 10);
    ObjectMotion_SetVariantCallbackAndInvokeObject_7(8, 1);
    ObjectMotion_SetSpeedParameters_5(8, 0);
    ObjectMotion_ArmCallback_18(2, 0x6000, 0);
    if (UiWork_WaitThenFinalizeCapacity_1(0, 0) == 0) {
        ObjectMotion_SetVariantCallbackAndInvokeObject_8(2, 2);
        BattleRuntime_RunThenWaitIfModeZero_12(0x4002, 0, 10);
        bump_step_0200164c(1);
    } else {
        bump_step_0200164c(1);
        ObjectMotion_CallThenWaitForAnimationChange_6(2, 4);
        BattleRuntime_RunThenWaitIfModeZero_13(0x4002, 0, 10);
    }
    ObjectMotion_SetVariantCallbackAndInvokeObject_9(9, 2);
    BattleRuntime_RunThenWaitIfModeZero_14(9, 0, 10);
    ObjectMotion_ArmCallback_19(2, 0xc000, 10);
    ObjectMotion_CallThenWaitForAnimationChange_7(2, 4);
    BattleRuntime_RunThenWaitIfModeZero_15(2, 0, 10);
    BattleRuntime_WaitIfModeZero_10(8, 0x102);
    BattleRuntime_WaitIfModeZero_11(60);
    BattleRuntime_RunThenWaitIfModeZero_16(8, 0, 10);
    ObjectMotion_ArmCallback_20(2, 0xa000, 10);
    ObjectMotion_CallThenWaitForAnimationChange_8(2, 3);
    BattleRuntime_RunThenWaitIfModeZero_17(2, 0, 10);
    BattleRuntime_WaitIfModeZero_12(8, 0x102);
    BattleRuntime_WaitIfModeZero_13(9, 0x102);
    BattleRuntime_WaitIfModeZero_14(60);
    ObjectMotion_SetVariantCallbackAndInvokeObject_10(8, 2);
    BattleRuntime_RunThenWaitIfModeZero_18(8, 0, 10);
    ObjectMotion_SetVariantCallbackAndInvokeObject_11(9, 2);
    ObjectMotion_ArmCallback_21(9, 0x8000, 10);
    BattleRuntime_RunThenWaitIfModeZero_19(9, 0, 10);
    ObjectMotion_ArmCallback_22(8, 0, 10);
    ObjectMotion_CallThenWaitForAnimationChange_9(8, 3);
    ObjectMotion_ArmCallback_23(8, 0x3000, 10);
    BattleEvent_RunActionAndWait_1(8, 0);
    ObjectMotion_ArmCallback_24(9, 0x3000, 10);
    ObjectMotion_CallThenWaitForAnimationChange_10(2, 3);
    ObjectMotion_ArmCallback_25(2, 0x6000, 10);
    BattleEventRuntime_ProcessAction_2(0x4002, 0);
    BattleRuntime_WaitIfModeZero_15(10);
    ObjectMotion_SetVariantCallbackAndInvokeObject_12(2, 1);
    BattleRuntime_RunThenWaitIfModeZero_20(0x4002, 0, 10);
    ObjectMotion_CallThenWaitForAnimationChange_11(0, 3);
    ObjectMotion_CallThenWaitForAnimationChange_12(2, 3);
    ObjectMotion_ArmCallback_26(2, 0xc000, 10);
    ObjectMotion_CallThenWaitForAnimationChange_13(2, 3);
    Call3_0200164c((void (*)())Func_02006152, 8, 0xcccc, 0x6666);
    Call3_0200164c((void (*)())Func_0200615c, 9, 0xcccc, 0x6666);
    record = ObjectMotion_SetSpeedParameters_6(8);
    {
        /* Field at +6 of the record: a visibility/state word. */
        s32 shown = 0;

        *(u16 *)(record + 6) = shown;
    }
    record = ObjectMotion_SetSpeedParameters_7(9);
    {
        s32 shown = 0x8000;

        *(u16 *)(record + 6) = shown;
    }
    /* Clear bit 0 of the flag byte at +90. */
    *(u8 *)(Scene_GetRecord_8(8) + 90) &= 254;
    *(u8 *)(Scene_GetRecord_9(9) + 90) &= 254;
    ObjectMotion_ResetAndSetPositionInMode2_2(8, 168, 232);
    ObjectMotion_SetPositionAndReset_5(9, 212, 232);
    Object_SetModeById_2(8, 1);
    BattleRuntime_WaitIfModeZero_16(20);
    {
        /* Set bit 0 of the flag byte at +90. */
        u8 *record = Scene_GetRecord_10(8);
        u8 flags = *(volatile u8 *)&record[90];

        record[90] = (u8)(flags | 1);
    }
    {
        u8 *record = Scene_GetRecord_11(9);
        u8 flags = *(volatile u8 *)&record[90];

        record[90] = (u8)(flags | 1);
    }
    ObjectMotion_SetPositionAndReset_6(2, 192, 232);
    ObjectMotion_ArmCallback_27(2, 0xc000, 0);
    Audio_PlayCue_1(188);
    Call6(Func_02006170, 36, 23, 43, 12, 2, 2);
    Func_020060ee(5);
    Call6(Func_02006186, 39, 23, 43, 12, 2, 2);
    Func_02006104(5);
    ObjectMotion_SetPositionAndReset_7(2, 192, 222);
    ObjectMotion_SetHorizontalPositionWithTerrain_3(2, 0, 0);
    ObjectMotion_SetSpeedParameters_8(0, 0x13333, 0x9999);
    ObjectMotion_SetPositionAndReset_8(0, 192, 222);
    ObjectMotion_SetHorizontalPositionWithTerrain_4(0, 0, 0);
    Object_SetModeById_3(8, 3);
    ObjectMotion_CallThenWaitForAnimationChange_14(9, 3);
    ObjectMotion_ResetAndSetPositionInMode2_3(8, 184, 232);
    ObjectMotion_SetPositionAndReset_9(9, 198, 232);
    ObjectMotion_ResetAndSetPositionInMode2_4(8, 188, 212);
    ObjectMotion_SetPositionAndReset_10(9, 194, 212);
    ObjectMotion_SetHorizontalPositionWithTerrain_5(8, 0, 0);
    ObjectMotion_SetHorizontalPositionWithTerrain_6(9, 0, 0);
    SCENE_FIELD_1C8 = 24;
    SCENE_FIELD_1C0 = 0x201;
    Func_020063be();
    ObjectMotion_SetSpeedParameters_9();
    Func_020063b8(5);
    BattleRuntime_ScheduleShoulderButtonModeUpdate_1();
}

void FieldScene_RunSecondaryGroupSequence(void)
{
    extern struct SceneWork *Data_03001ebc;

    u8 *fieldActor;
    u8 *object;
    struct SceneWork *work;
    u32 random;
    u32 motionPhase;
    const u8 *motionActions;
    s32 scale;
    const u8 *exitActions;

    Func_02003bf4();
    Call4(Func_02003d14, -1, -1, -1, 0);
    Func_02003b14(1);
    Func_02003c74(0, 0, 0);
    Call3(Func_02003c74, 9, 0x1b80000, 0x20a0000);
    object = Func_02003c1c(9);
    Func_02003bb4(object, 0);
    Call4(Func_02003d14, 0x1b80000, -1, 0x20a0000, 0);
    Func_02003b7c();
    Func_02003b14(1);
    Func_02003d2c();
    Call2_020026c0(Func_02003d0c, 0x19999, 0x3333);
    Call4(Func_02003d14, 0x1b80000, -1, 0x1900000, 1);
    Func_02003d7c(141);
    Call3(Func_02003c2c, 9, 0x19999, 0xcccc);
    Call3(Func_02003c5c, 9, 0x1b8, 0x190);
    Call2_020026c0(Func_02003d0c, 0xc000, 0x1800);
    Call4(Func_02003d14, 0x1b80000, -1, 0x12c0000, 1);
    Call3(Func_02003c2c, 9, 0xcccc, 0x6666);
    Call3(Func_02003c5c, 9, 0x1b8, 0x12c);
    Func_02003c7c(9, 0);
    Call1_020026c0(Func_02003d7c, 0x121);
    Func_02003bec_b(40);
    Call3(Func_02003c2c, 11, 0xcccc, 0x6666);
    Call3(Func_02003c74, 11, 0x1b70000, 0x1320000);
    Func_02003c8c(11, 4, 0);
    Call3(Func_02003c5c, 11, 0x1b7, 0x138);
    Call3(Func_02003c5c, 11, 0x1a0, 0x138);
    Call3(Func_02003c5c, 11, 0x190, 0x100);
    Call3(Func_02003ce4, 11, 0x3000, 40);
    Call4(Func_02003d14, 0x19a0000, -1, 0x1180000, 1);
    Call3(Func_02003c2c, 10, 0x9999, 0x4ccc);
    Call3(Func_02003c74, 10, 0x1b70000, 0x1320000);
    Func_02003c8c(10, 4, 0);
    Call3(Func_02003c5c, 10, 0x1b7, 0x138);
    Call3(Func_02003c5c, 10, 0x1a0, 0x138);
    Call3(Func_02003c5c, 10, 0x184, 0x10e);
    Call3(Func_02003ce4, 10, 0xd000, 10);
    Call3(Func_02003ce4, 11, 0x5000, 10);
    Call3(Func_02003c2c, 0, 0xcccc, 0x6666);
    Call3(Func_02003c74, 0, 0x1b70000, 0x1320000);
    Func_02003c8c(0, 4, 0);
    Call3(Func_02003c5c, 0, 0x1b7, 0x138);
    Call3(Func_02003c5c, 0, 0x1a0, 0x138);
    Call3(Func_02003c5c, 0, 0x184, 0x12c);
    Call3(Func_02003ce4, 0, 0xc000, 40);
    fieldActor = Func_02003c1c(0);
    random = Func_02003b2c();
    motionPhase = random * 5;
    motionActions = Data_0200c21c;
    fieldActor += 102;
    *(u16 *)fieldActor = motionPhase >> 12;
    Func_02003c34(0, motionActions);
    Func_02003c8c(11, 2, 20);
    Func_02003c84(11, 3);
    Call1_020026c0(Func_02003cbc, 0x256f);
    Func_02003cd4(11, 0, 10);
    Func_02003c84(10, 3);
    object = Func_02003c1c(0);
    if (object != 0) {
        Func_02003c74(1, *(s32 *)(object + 8), *(s32 *)(object + 16));
    }
    object = Func_02003c1c(0);
    if (object != 0) {
        Func_02003c74(2, *(s32 *)(object + 8), *(s32 *)(object + 16));
    }
    object = Func_02003c1c(0);
    if (object != 0) {
        Func_02003c74(3, *(s32 *)(object + 8), *(s32 *)(object + 16));
    }
    Call3(Func_02003c2c, 1, 0xcccc, 0x6666);
    Call3(Func_02003c2c, 2, 0xcccc, 0x6666);
    Call3(Func_02003c2c, 3, 0xcccc, 0x6666);
    Call3(Func_02003c54, 1, 0x17a, 0x136);
    Call3(Func_02003c54, 2, 0x190, 0x120);
    Call3(Func_02003c5c, 3, 0x19a, 0x134);
    Func_02003c7c(1, 1);
    Func_02003c7c(2, 1);
    Call3(Func_02003ce4, 1, 0xc000, 0);
    Call3(Func_02003ce4, 2, 0xc000, 0);
    Call3(Func_02003ce4, 3, 0xc000, 40);
    fieldActor = Func_02003c1c(1);
    random = Func_02003b2c();
    fieldActor += 102;
    *(u16 *)fieldActor = ((random * 5) >> 12);
    fieldActor = Func_02003c1c(2);
    random = Func_02003b2c();
    fieldActor += 102;
    *(u16 *)fieldActor = ((random * 5) >> 12);
    fieldActor = Func_02003c1c(3);
    random = Func_02003b2c();
    fieldActor += 102;
    *(u16 *)fieldActor = ((random * 5) >> 12);
    Func_02003c34(1, motionActions);
    Func_02003c34(2, motionActions);
    Func_02003c34(3, motionActions);
    Call3(Func_02003cfc, 2, 0x101, 60);
    Call3(Func_02003cd4, 0x2002, 0, 10);
    Call3(Func_02003cfc, 10, 0x100, 40);
    Call3(Func_02003ce4, 10, 0x3000, 20);
    Func_02003cd4(10, 0, 10);
    Func_02003c44(0);
    Func_02003c44(1);
    Func_02003c44(2);
    Func_02003c44(3);
    Func_02003b14(1);
    Call3(Func_02003ce4, 2, 0xc000, 10);
    Func_02003c84(2, 3);
    Call3(Func_02003ce4, 0, 0xc000, 0);
    Call3(Func_02003ce4, 1, 0xc000, 0);
    Call3(Func_02003ce4, 3, 0xc000, 20);
    Func_02003c84(10, 3);
    Func_02003cd4(10, 0, 10);
    Call3(Func_02003cfc, 3, 0x101, 60);
    Call3(Func_02003cd4, 0x2003, 0, 10);
    Func_02003c84(10, 3);
    Func_02003cd4(10, 0, 10);
    Call3(Func_02003cfc, 1, 0x101, 60);
    Func_02003cd4(1, 0, 10);
    Call3(Func_02003ce4, 10, 0x5000, 0);
    Func_02003c9c(11, 2);
    Func_02003cd4(11, 0, 10);
    Call3(Func_02003ce4, 0, 0x6000, 0);
    Call3(Func_02003ce4, 1, 0xe000, 0);
    Call3(Func_02003ce4, 2, 0x2000, 0);
    Call3(Func_02003ce4, 3, 0xa000, 20);
    Func_02003c7c(0, 3);
    Func_02003c7c(1, 3);
    Func_02003c7c(2, 3);
    Func_02003c84(3, 3);
    Func_02003bec_b(10);
    Call3(Func_02003ce4, 0, 0xc000, 0);
    Call3(Func_02003ce4, 1, 0xc000, 0);
    Call3(Func_02003ce4, 2, 0xc000, 0);
    Call3(Func_02003ce4, 3, 0xc000, 10);
    Call3(Func_02003ce4, 10, 0xd000, 10);
    Func_02003c84(10, 3);
    Func_02003cd4(10, 0, 10);
    Func_02003c84(11, 3);
    Call2_020026c0(Func_02003d04, 2, 0x102);
    Func_02003bec_b(40);
    Func_02003c94(2, 2);
    Call3(Func_02003cd4, 0x2002, 0, 10);
    Call3(Func_02003ce4, 11, 0x3000, 10);
    Func_02003c7c(11, 4);
    Func_02003cd4(11, 0, 10);
    Call3(Func_02003cfc, 3, 0x101, 40);
    Call3(Func_02003cd4, 0x2003, 0, 10);
    Call3(Func_02003ce4, 10, 0x3000, 10);
    Call3(Func_02003cfc, 10, 0x108, 20);
    Func_02003cd4(10, 0, 10);
    Func_02003c9c(1, 2);
    Func_02003cd4(1, 0, 10);
    Call3(Func_02003ce4, 11, 0x5000, 0);
    Call3(Func_02003ce4, 10, 0x5000, 10);
    Func_02003c7c(11, 4);
    Func_02003cd4(11, 0, 10);
    Call3(Func_02003ce4, 0, 0x6000, 0);
    Call3(Func_02003ce4, 1, 0xe000, 40);
    Call3(Func_02003ce4, 0, 0xc000, 0);
    Call3(Func_02003ce4, 1, 0xc000, 10);
    Func_02003c84(11, 4);
    Func_02003cd4(11, 0, 10);
    Func_02003c84(10, 4);
    Func_02003cd4(10, 0, 10);
    Call3(Func_02003cfc, 0, 0x105, 0);
    Call3(Func_02003cfc, 1, 0x105, 0);
    Call3(Func_02003cfc, 2, 0x105, 0);
    Call3(Func_02003cfc, 3, 0x105, 60);
    Call3(Func_02003ce4, 10, 0x3000, 10);
    Func_02003c7c(10, 3);
    Value2(Func_02003cc4, 10, 0);
    Call3(Func_02003ce4, 1, 0xe000, 0);
    Call3(Func_02003ce4, 2, 0x6000, 0);
    Call3(Func_02003ce4, 3, 0xa000, 0);
    if (Value2(Func_02003c14, 0, 0) == 0) {
        Func_02003bec_b(20);
        Data_03001ebc->step += 3;
    } else {
        Func_02003bec_b(20);
        Func_02003c9c(11, 2);
        Func_02003cd4(11, 0, 40);
        Func_02003cd4(11, 0, 10);
        Call3(Func_02003cfc, 3, 0x106, 40);
        Call3(Func_02003ce4, 3, 0xc000, 20);
        Call3(Func_02003cd4, 0x2003, 0, 10);
    }
    Call3(Func_02003ce4, 1, 0xc000, 0);
    Call3(Func_02003ce4, 2, 0xc000, 0);
    Call3(Func_02003ce4, 3, 0xc000, 10);
    Func_02003c7c(1, 3);
    Func_02003c7c(2, 3);
    Func_02003c84(3, 3);
    Func_02003bec_b(20);
    Call3(Func_02003ce4, 8, 0x3000, 0);
    Func_02003c84(10, 3);
    Call2_020026c0(Func_02003d0c, 0x10000, 0x2000);
    Call4(Func_02003d14, 0x1180000, -1, 0xc80000, 1);
    Call3(Func_02003c5c, 10, 0x14d, 222);
    Call3(Func_02003c5c, 10, 0x11c, 198);
    Call3(Func_02003ce4, 10, 0x8000, 10);
    Func_02003c84(10, 3);
    Func_02003ce4(8, 0, 10);
    Func_02003c84(8, 3);
    Call3(Func_02003c2c, 8, 0xcccc, 0x6666);
    Call3(Func_02003c5c, 8, 0x10c, 198);
    Call3(Func_02003ce4, 8, 0xc000, 10);
    Func_02003c9c(8, 2);
    Func_02003d7c(125);
    Call6_020026c0(Func_02003b9c, 71, 60, 76, 11, 2, 1);
    Call6_020026c0(Func_02003ba4, 71, 60, 2, 1, 16, 11);
    Func_02003bec_b(20);
    Func_02003c5c(8, 246, 198);
    Call3(Func_02003ce4, 8, 0, 20);
    *(u8 *)(Func_02003d1c_a() + 85) = 0;
    Call2_020026c0(Func_02003d0c, 0x9999, 0x1333);
    Call4(Func_02003d14, 0xf80000, -1, 0xaa0000, 1);
    Call3(Func_02003c5c, 10, 0x10e, 198);
    Call3(Func_02003c5c, 10, 0x10e, 174);
    Func_02003c5c(10, 224, 170);
    Func_02003c5c(10, 210, 158);
    Func_02003c5c(10, 246, 148);
    Func_02003c5c(10, 246, 142);
    Func_02003c74(10, 0, 0);
    Data_03001ebc->request = 514;
    Func_02003d34_b();
    Func_02003d3c();
    Call3(Func_02003c74, 9, 0x1b80000, 0x1540000);
    object = Func_02003c1c(9);
    scale = 0x4000;
    *(u16 *)(object + 6) = scale;
    Call4(Func_02003d14, 0x17c0000, -1, 0x1180000, 0);
    Func_02003b7c();
    Func_02003b14(10);
    Func_02003d2c();
    Func_02003d3c();
    Call1_020026c0(Func_02003bec_b, 40);
    Func_02003c84(11, 3);
    Func_02003bec_b(20);
    Func_02003c7c(0, 3);
    Func_02003c7c(1, 3);
    Func_02003c7c(2, 3);
    ((void (*)())Func_02003c84)(3, 3);
    Call3(Func_02003c5c, 11, 0x1a4, 0x11a);
    Call3(Func_02003c5c, 11, 0x1a4, 0x138);
    Call3(Func_02003c5c, 11, 0x1b7, 0x138);
    Call3(Func_02003c5c, 11, 0x1b7, 0x132);
    Func_02003c74(11, 0, 0);
    exitActions = Data_0200c230;
    Func_02003c34(1, exitActions);
    Call2_020026c0(Func_02003c34, 2, (s32)exitActions);
    Call2_020026c0(Func_02003c4c, 3, (s32)exitActions);
    Call4(Func_02003d14, 0x19a0000, -1, 0x12c0000, 1);
    Call3(Func_02003c5c, 0, 0x1a0, 0x138);
    Call3(Func_02003c5c, 0, 0x1b7, 0x138);
    Call3(Func_02003c5c, 0, 0x1b7, 0x132);
    Func_02003c74(0, 0, 0);
    Func_02003d7c(141);
    Call3(Func_02003c2c, 9, 0xcccc, 0x6666);
    Call2_020026c0(Func_02003d0c, 0x10000, 0x2000);
    Call4(Func_02003d14, 0x1b80000, -1, 0x1a40000, 1);
    Call3(Func_02003c5c, 9, 0x1b8, 0x1a4);
    Call2_020026c0(Func_02003d0c, 0x20000, scale);
    Call4(Func_02003d14, 0x1b80000, -1, 0x2580000, 1);
    Call3(Func_02003c2c, 9, 0x19999, 0xcccc);
    Call3(Func_02003c5c, 9, 0x1b8, 0x1f4);
    Call3(Func_02003c54, 9, 0x1b8, 0x258);
    Call1_020026c0(Func_02003d7c, 0x121);
    work = Data_03001ebc;
    work->setup = 24;
    work->request = 0x100;
    Func_02003d34_b();
    Func_02003d3c();
    Func_02003d24(10);
    Func_02003bfc();
}

s32 *SceneActor_FindAtTileXZ(s32 x, s32 z)
{
    extern u8 *Data_03001ebc;

    s32 **tbl = (s32 **)(Data_03001ebc + 0x14);
    u32 i;

    for (i = 8; i <= 65; i++) {
        s32 *p = tbl[i];

        if (x == (p[2] >> 20) && z == (p[4] >> 20)) {
            return p;
        }
    }
    return 0;
}

void FieldScene_RunLateSequence(void)
{
    extern u8 Data_03001ebc[];

    s32 tmp[3];
    s32 record;
    s32 rec;
    s32 idx;
    s32 w;
    s32 w2;
    s32 a;
    s32 *dst;
    s32 zero;
    s32 k;

    record = Value1_0200164c(Func_02006de4, 0);
    idx = (s32)((u32)*(u16 *)(record + 6) >> 12);
    a = *(s16 *)(record + 10);
    w = Data_0200bd84[idx];
    rec = Value2(Func_02006376,
                 (a + (w >> 16)) >> 4,
                 (*(s16 *)(record + 18) + (s32)(s16)w) >> 4);
    if (rec != 0) {
        zero = 0;
        *(u8 *)(rec + 34) = 2;
        dst = tmp;
        w2 = Data_0200bd84[idx];
        dst[0] = *(volatile s32 *)(rec + 8) + (w2 & -0x10000);
        dst[1] = *(volatile s32 *)(rec + 12);
        dst[2] = *(volatile s32 *)(rec + 16) + (w2 << 16);
        if (Value2(Func_02006dd2, rec, (s32)dst) <= 0) {
            Func_02006d8e(record, 8);
            k = 0x3333;
            Func_02006d4e(15);
            Func_02006fbc(185);
            *(volatile s32 *)(rec + 48) = k;
            *(volatile s32 *)(rec + 52) = k;
            Func_02006dd4(rec, dst[0], dst[1], dst[2]);
            *(volatile s32 *)(record + 48) = k;
            *(volatile s32 *)(record + 52) = k;
            Func_02006de6(record, dst[0], dst[1], dst[2]);
            Func_02006df4(rec);
            Func_02006fd8();
            *(volatile s32 *)(rec + 8) = dst[0];
            *(volatile s32 *)(rec + 16) = dst[2];
            *(s32 *)(rec + 36) = zero;
            *(s32 *)(rec + 44) = zero;
            Func_02006dde(record, 1);
            Func_0200652a();
        }
    }
}

void Func_020032a4(void)
{
    extern u8 Data_03001ebc[];

    s32 rec8;
    s32 rec4;
    s32 rec7;
    s32 rec2;
    u32 i;
    s32 v5;
    s32 v6;
    s32 v7;

    rec8 = Value1_0200164c(Func_02006ece, 8);
    rec4 = Value1_0200164c(Func_02006ed6, 9);
    rec7 = Value1_0200164c(Func_02006e96, 0x302);
    if (rec7 != 0) {
    } else if ((*(volatile s32 *)(rec8 + 8) >> 19) > 29) {
    } else {
        rec2 = Value1_0200164c(Func_02006ef6, 11);
        Func_02006ed4();
        Func_02006e94(7, 44, 1, 1, rec7, 1);
        i = 67;
        v7 = 1;
        v6 = 5;
        do {
            Func_02006ea2(i, 58, 78, 41, v7, v6);
            Func_02006ef8(4);
            if (i == 70) {
                Func_02006ef2(0x302);
            }
            i++;
        } while (i <= 74);
        v5 = 2;
        Func_02006ecc(16, 109, 13, 109, 3, v5);
        Func_02006f22(40);
        *(s32 *)(rec2 + 24) = 0x1999;
        *(s32 *)(rec2 + 28) = 0x1999;
        Call3_0200164c(Func_02006fc0, 11, 0x960000, 0x2d80000);
        Value2(Func_02006f88, 11, 0x200c268);
        v6 = 1;
        Func_02006f02(67, 64, 71, 44, v6, v5);
        Func_02006f12(67, 64, 72, 44, v6, v5);
        Func_02006f22_a(67, 68, 73, 43, v6, v5);
        Func_02006f32(67, 68, 74, 43, v6, v5);
        Func_02006f42(67, 64, 75, 44, v6, v5);
        Func_02006f52(67, 66, 76, 44, v6, v5);
        Func_02006f62(67, 64, 77, 44, v6, v5);
        Func_02006f72(67, 64, 78, 44, v6, v5);
        Func_02006f82(67, 64, 79, 44, v6, v5);
        Func_02006f92(67, 66, 80, 44, v6, v5);
        Func_02006fa2(2, 0, 9, 42, v5, v5);
        Func_02006ff8(40);
        Func_02006fb8(68, 64, 71, 44, v6, v5);
        Func_02006fc8(68, 64, 72, 44, v6, v5);
        Func_02006fd8_a(68, 68, 73, 43, v6, v5);
        Func_02006fe8(68, 68, 74, 43, v6, v5);
        Func_02006ff8_a(68, 64, 75, 44, v6, v5);
        Func_02007008(68, 66, 76, 44, v6, v5);
        Func_02007018(68, 64, 77, 44, v6, v5);
        Func_02007028(68, 64, 78, 44, v6, v5);
        Func_02007038(68, 64, 79, 44, v6, v5);
        Func_02007048(68, 66, 80, 44, v6, v5);
        Func_02007058(4, 0, 9, 42, v5, v5);
        Func_020070ae(40);
        v5 = 10;
        Func_02007072(7, 11, 7, 42, v5, 8);
        Func_02007084(71, 12, 71, 43, v5, 13);
        v5 = 44;
        Func_020070a0(6, 13, 12, 12, 6, v5);
        Func_020070ee(40);
        Func_02004aba();
        Func_020070bc(0, 1, 1, 1, 7, v5);
        Func_02007118();
    }
    if (Value1_0200164c(Func_020070f6, 0x303) != 0) {
    } else if ((*(volatile s32 *)(rec4 + 8) >> 19) > 87) {
    } else {
        Func_0200712c();
        i = 67;
        v7 = 1;
        v6 = 5;
        do {
            Func_020070ea(i, 58, 107, 41, v7, v6);
            Func_02007140(4);
            if (i == 70) {
                Func_0200713a(0x303);
            }
            i++;
        } while (i <= 74);
        v6 = 2;
        Func_02007128(45, 109, 42, 109, 3, v6);
        Func_0200717e(40);
        v5 = 1;
        Func_02007140_a(67, 64, 102, 44, v5, v6);
        Func_02007150(67, 64, 103, 44, v5, v6);
        Func_02007160(67, 64, 104, 44, v5, v6);
        Func_02007170(67, 66, 105, 44, v5, v6);
        Func_02007180(67, 64, 106, 44, v5, v6);
        Func_02007190(67, 64, 107, 44, v5, v6);
        Func_020071a0(67, 64, 108, 44, v5, v6);
        Func_020071b0(67, 66, 109, 44, v5, v6);
        Func_02007206(40);
        Func_020071c6(68, 64, 102, 44, v5, v6);
        Func_020071d6(68, 64, 103, 44, v5, v6);
        Func_020071e6(68, 64, 104, 44, v5, v6);
        Func_020071f6(68, 66, 105, 44, v5, v6);
        Func_02007206_a(68, 64, 106, 44, v5, v6);
        Func_02007216(68, 64, 107, 44, v5, v6);
        Func_02007226(68, 64, 108, 44, v5, v6);
        Func_02007236(68, 66, 109, 44, v5, v6);
        Func_0200728c(40);
        v5 = 8;
        Func_02007250(38, 14, 38, 44, v5, 4);
        Func_02007262(102, 14, 102, 44, v5, 12);
        Call6(Func_0200727e, 37, 13, 10, 12, 37, 43);
        Func_020072cc(40);
        Func_02004c98();
        Func_020072e4();
    }
}

void SceneEffect_UpdateMotionWithDamping(struct Obj_020036f8 *p)
{
    s16 *h;
    s32 v;
    s32 a;
    s32 b;

    h = &p->f64;
    v = *h;
    if (v == 0) {
        Func_0200727e_a();
    } else if (v == 1) {
        p->f24 = 0;
        p->f28 = 0;
        p->f08 = 0;
        p->f0c = 0;
    } else {
        p->f18 += 0x800;
        p->f1c += 0x800;
    }
    p->f08 += p->f24;
    p->f0c += p->f28;
    a = p->f24;
    b = p->f28;
    p->f24 = a - a / 256;
    p->f28 = b - b / 16;
    { s32 t = *(u16 *)h; t -= 1; *(u16 *)h = t; }
}

void FieldScene_RunSupplementalSequenceOne(s32 a0)
{
    extern u8 Data_03001ebc[];

    s32 rec7;
    s32 rec8;
    s32 record;
    s32 mask;
    s32 c12;
    s32 old;
    u8 *rp;
    u8 *p4;

    rec7 = Value1_0200164c(Func_0200738a, 8);
    rec8 = Func_02007392(9);
    if ((u32)(*(s16 *)(rec7 + 10) + -0x17d) <= 12) {
        if (*(s16 *)(rec7 + 18) <= 0x309) {
            goto L_020037ae;
        }
        record = Value1_0200164c(Func_020073b0, 0);
        p4 = (u8 *)*(s32 *)(rec7 + 80);
        rp = (u8 *)*(s32 *)(record + 80);
        c12 = 12 & rp[9];
        old = p4[9];
        mask = -13;
        mask &= old;
        p4[9] = (mask | c12);
    } else {
        L_020037ae:;
        if (Value1_0200164c(Func_02007386, 0x302) == 0) {
            if (*(s16 *)(rec7 + 10) <= 245) {
                if ((*(volatile s32 *)0x03001e40 & 1) == 0) {
                    if (Value1_0200164c(Func_020073a4, 0x202) == 0) {
                        Call1_02003768(Func_02007540, -1);
                        Func_0200755e(230);
                        Call1_02003768(Func_020073c4, 0x202);
                    }
                    Func_02007056(*(volatile s32 *)(rec7 + 8), *(volatile s32 *)(rec7 + 12), *(volatile s32 *)(rec7 + 16));
                }
            }
        }
    }
    if (Value1_0200164c(Func_020073cc, 0x303) == 0) {
        if (*(s16 *)(rec8 + 10) <= 0x2c5) {
            if ((*(volatile s32 *)0x03001e40 & 1) == 0) {
                if (Value1_0200164c(Func_020073ec, 0x203) == 0) {
                    Call1_02003768(Func_02007588, -1);
                    Func_020075a6(230);
                    Call1_02003768(Func_0200740c, 0x203);
                }
                Func_0200709e(*(volatile s32 *)(rec8 + 8), *(volatile s32 *)(rec8 + 12), *(volatile s32 *)(rec8 + 16));
            }
        }
    }
}

void Func_02003864(s32 a0, s32 a1, s32 a2)
{
    extern u8 Data_03001ebc[];

    s32 p8;
    u8 *rec7;
    s32 value;
    s32 mask;
    u8 *link;

    p8 = a2;
    value = Value0(Func_0200739e);
    rec7 = (u8 *)Value4(Func_020073fc, 222, (a0 + -0x80000), (((((u32)(value << 3) >> 16) << 16) + a1) + 0x100000), p8);
    if ((s32)rec7 != 0) {
        rec7[85] = (mask = 0);
        link = (u8 *)*(volatile s32 *)((s32)rec7 + 80);
        mask -= 13;
        link[9] = ((link[9] & mask) | 8);
        Func_02007566((s32)rec7, 9);
        Func_0200746e((s32)rec7, 0);
        value = Func_020073ea();
        *(volatile s32 *)((s32)rec7 + 36) = ((((u32)(value << 1) >> 16) - 1) << 16);
        value = Value0(Func_020073f8);
        *(volatile s32 *)((s32)rec7 + 40) = ((((u32)(((value << 1) + value) << 1) >> 16) - 3) << 16);
        {
            volatile u16 *target = (volatile u16 *)((s32)rec7 + 100);
            s32 shown = 20;

            *target = shown;
            *((u8 *)target - 3) = 1;
        }
        Func_0200744c((s32)rec7, 1);
        Func_0200745c((s32)rec7, 0x200d120);
    }
}

void SceneEffect_UpdateLobeOrbitEffect26(void)
{
    Effect_0200390c *effect = Func_0200752e(26);
    RenderData *render = effect->render;
    s32 offset = Func_02007450(effect->angle) * 2;
    s32 first;

    if (offset > 0) {
        offset = -offset;
    }
    effect->x = effect->base_x + Func_02007466(effect->angle) * 2;
    effect->y = effect->base_y + offset;
    render->rotation = Func_02007480(effect->angle + 0x8000) >> 3;
    first = Func_02007478();
    effect->angle +=
        ((u32)(first << 9) >> 16)
        + ((u32)(Func_0200747e() << 9) >> 16)
        + 0x400;
}

void FieldScene_DrawTilesAndRaiseActor11(void)
{
    extern u8 *Data_03001ebc;

    Effect *effect = Func_02007594(11);

    Func_02007532(0, 0, 1, 1, 9, 14);
    Func_02007544(0, 0, 1, 1, 9, 45);
    if (effect != 0) {
        Func_02007560(effect, 0);
        effect->y -= 0x200000;
        effect->state23 = 2;
    }
    Func_0200759e(0x201);
}

s32 SceneEffect_UpdateOrbitingEffect(Effect_0200390c *effect)
{
    RenderData *render = effect->render;
    s32 ofs = Func_02007512(effect->angle) * 2;
    s32 first;

    if (ofs > 0) {
        ofs = -ofs;
    }
    effect->x = effect->base_x + Func_02007528(effect->angle) * 2;
    effect->y = effect->base_y + ofs;
    render->rotation = Func_02007542(effect->angle + 0x8000) / 8;
    first = Func_02007540_a();
    effect->angle +=
        ((u32)(first << 9) >> 16)
        + ((u32)(Func_02007546() << 9) >> 16)
        + 0x400;
    return 0;
}

void InitializeOrbitingRenderEffect(void)
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
