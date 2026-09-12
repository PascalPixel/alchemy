#include "types.h"

#define SceneData_SelectTableBySceneId Func_02000030
#define SceneData_GetTableB2bc Func_02000074
#define SceneData_SelectDataBySelectorAndSubstate Func_02000238
#define SceneDialogue_RunActor10MessageByFlag962 Func_020002a0
#define SceneDialogue_RunActor13MessageByFlag962 Func_020002e4
#define SceneDialogue_ShowLine2118WithActor15Steps Func_0200050c
#define BattleRuntime_Reset_1(args...) Func_02003dbc(args)
#define GameFlag_IsSet_1(a0) Value1(Func_02003d8a, a0)
#define Object_LinkPair_1(args...) Func_02003e62(args)
#define SceneWork_SetStepValue_1(a0) Call1(Func_02003e78, a0)
#define ObjectMotion_ArmCallback_1(a0, a1, a2) Call3(Func_02003eb2_a, a0, a1, a2)
#define ObjectMotion_SetVariantCallback_1(args...) Func_02003e74(args)
#define SceneWork_SetStepValue_2(a0) Call1(Func_02003e9a, a0)
#define BattleEvent_RunActionAndWait_1(args...) Func_02003eb2_b(args)
#define RuntimeBlock_GetOffset1e0Pointer_1(args...) Func_02003f06(args)
#define RuntimeBlock_GetOffset1e0Pointer_2(args...) Func_02003f06(args)
#define ObjectMotion_SetSpeedLimitAndAcceleration_1(a0, a1) Call2(Func_02003f00, a0, a1)
#define ObjectMotion_PlaceWithinCameraBounds_1(a0, a1, a2, a3) Call4(Func_02003f1a, a0, a1, a2, a3)
#define Object_CommitPositionThenWaitIfModeZero_1(args...) Func_02003f26(args)
#define ObjectMotion_SetSpeedParameters_1(args...) Func_02003f84(args)
#define GameFlag_IsSet_2(a0) Value1(Func_02003e1a, a0)
#define BattleRuntime_ScheduleShoulderButtonModeUpdate_1(args...) Func_02003e70(args)
#define SCENE_PHASE (*(s32 *)(*(u8 **)0x03001ebc + 0x1c0))
#define SCENE_FIELD_1C8 (*(s32 *)(*(u8 **)0x03001ebc + 0x1c8))
#define RECORD_17 17
#define BattleRuntime_Reset_1_02001688(args...) Func_02004280(args)
#define ObjectMotion_PlaceWithinCameraBounds_1_02001688(a0, a1, a2, a3) Call4(Func_02004382_a, a0, a1, a2, a3)
#define Audio_PlayCue_1(args...) Func_020043d8(args)
#define Object_SetModeById_1(args...) Func_02004308(args)
#define Object_SetModeById_2(args...) Func_02004318(args)
#define Object_SetModeById_3(args...) Func_02004320(args)
#define Object_SetModeById_4(args...) Func_02004328(args)
#define Object_SetModeById_5(args...) Func_02004330(args)
#define Object_SetModeById_6(args...) Func_02004338(args)
#define Object_SetModeById_7(args...) Func_02004340(args)
#define ObjectMotion_SetHorizontalPositionWithTerrain_1(args...) Func_02004342_a(args)
#define Object_SetModeById_8(args...) Func_02004352(args)
#define Object_SetModeById_9(args...) Func_0200435a(args)
#define Scene_GetRecord_1(args...) Func_02004310_a(args)
#define Scene_GetRecord_2(args...) Func_0200431c(args)
#define Scene_GetRecord_3(a0) Value1(Func_02004326, a0)
#define ObjectMotion_SetHorizontalPositionWithTerrain_2(args...) Func_02004380(args)
#define BattleRuntime_WaitIfModeZero_1(args...) Func_02004468(args)
#define ObjectMotion_SetSpeedParameters_1_02001688(args...) Func_0200447c(args)
#define BattleRuntime_WaitIfModeZero_2(args...) Func_02004342_b(args)
#define ObjectMotion_SetVariantCallbackAndInvokeObject_1(args...) Func_020043da(args)
#define BattleRuntime_WaitIfModeZero_3(args...) Func_02004350(args)
#define ObjectMotion_SetSpeedParameters_2(a0, a1, a2) Call3(Func_02004382_b, a0, a1, a2)
#define ObjectMotion_SetPositionAndReset_1(a0, a1, a2) Call3(Func_020043c6, a0, a1, a2)
#define BattleRuntime_WaitIfModeZero_4(args...) Func_0200436c(args)
#define Object_SetModeById_10(args...) Func_020043e4(args)
#define BattleRuntime_WaitIfModeZero_5(args...) Func_0200437a(args)
#define Object_SetModeById_11(args...) Func_020043f2(args)
#define BattleRuntime_WaitIfModeZero_6(args...) Func_02004388(args)
#define Object_SetModeById_12(args...) Func_02004400(args)
#define BattleRuntime_WaitIfModeZero_7(args...) Func_02004396(args)
#define ObjectMotion_SetPositionAndReset_2(a0, a1, a2) Call3(Func_02004402, a0, a1, a2)
#define ObjectMotion_SetPositionAndReset_3(a0, a1, a2) Call3(Func_0200440e_a, a0, a1, a2)
#define ObjectMotion_ArmCallback_1_02001688(a0, a1, a2) Call3(Func_0200448a, a0, a1, a2)
#define ObjectMotion_SetPositionAndReset_4(a0, a1, a2) Call3(Func_02004426, a0, a1, a2)
#define Object_SetModeById_13(args...) Func_0200443e(args)
#define BattleRuntime_WaitIfModeZero_8(args...) Func_020043d4(args)
#define ObjectMotion_SetVariantCallbackAndInvokeObject_2(args...) Func_0200446c(args)
#define BattleRuntime_WaitIfModeZero_9(args...) Func_020043e2(args)
#define ObjectMotion_SetVariantCallbackAndInvokeObject_3(args...) Func_0200447a(args)
#define BattleRuntime_WaitIfModeZero_10(args...) Func_020043f0(args)
#define ObjectMotion_EnableActionAndSetCallback_1(a0, a1) Call2(Func_02004428, a0, a1)
#define BattleRuntime_WaitIfModeZero_11(args...) Func_020043fe(args)
#define BattleRuntime_WaitIfModeZero_12(a0, a1) Call2(Func_020044e8, a0, a1)
#define BattleRuntime_WaitIfModeZero_13(args...) Func_0200440e_b(args)
#define ObjectMotion_ArmCallback_2(a0, a1, a2) Call3(Func_020044ea, a0, a1, a2)
#define ObjectMotion_ArmCallback_3(a0, a1, a2) Call3(Func_020044f6, a0, a1, a2)
#define ObjectMotion_ArmCallback_4(a0, a1, a2) Call3(Func_02004502, a0, a1, a2)
#define BattleRuntime_WaitIfModeZero_14(a0, a1) Call2(Func_0200451c_a, a0, a1)
#define BattleRuntime_WaitIfModeZero_15(a0, a1) Call2(Func_02004526, a0, a1)
#define BattleRuntime_WaitIfModeZero_16(a0, a1) Call2(Func_02004530, a0, a1)
#define BattleRuntime_WaitIfModeZero_17(a0, a1) Call2(Func_0200453a, a0, a1)
#define BattleRuntime_WaitIfModeZero_18(args...) Func_02004460(args)
#define ObjectMotion_ArmCallback_5(a0, a1, a2) Call3(Func_0200453c, a0, a1, a2)
#define ObjectMotion_ArmCallback_6(a0, a1, a2) Call3(Func_02004548, a0, a1, a2)
#define BattleRuntime_WaitIfModeZero_19(a0, a1) Call2(Func_0200456a_a, a0, a1)
#define Scene_GetRecord_4(a0) Value1(Func_020044b0, a0)
#define Object_SetModeById_14(args...) Func_02004536(args)
#define Object_SetModeById_15(args...) Func_0200453e(args)
#define BattleRuntime_WaitIfModeZero_20(args...) Func_020044d4(args)
#define BattleRuntime_WaitIfModeZero_21(a0, a1) Call2(Func_020045be, a0, a1)
#define ObjectMotion_EnableActionAndResetMotion_1(args...) Func_0200451c_b(args)
#define Object_SetModeById_16(args...) Func_0200455c(args)
#define Scene_GetRecord_5(args...) Func_02004512(args)
#define BattleRuntime_WaitIfModeZero_22(args...) Func_0200450a(args)
#define Audio_PlayCue_2(args...) Func_02004658(args)
#define BattleRuntime_WaitIfModeZero_23(args...) Func_02004520(args)
#define Audio_PlayCue_3(a0) Call1(Func_0200466e, a0)
#define ObjectMotion_SetSpeedParameters_3(a0, a1, a2) Call3(Func_0200456a_b, a0, a1, a2)
#define ObjectMotion_SetPositionAndReset_5(a0, a1, a2) Call3(Func_020045ae, a0, a1, a2)
#define Audio_PlayCue_4(args...) Func_0200469c(args)
#define ObjectMotion_ArmCallback_7(a0, a1, a2) Call3(Func_02004630, a0, a1, a2)
#define Object_SetModeById_17(args...) Func_020045d8(args)
#define BattleRuntime_WaitIfModeZero_24(args...) Func_0200456e(args)
#define Object_SetModeById_18(args...) Func_020045e6(args)
#define BattleRuntime_WaitIfModeZero_25(args...) Func_0200457c(args)
#define Object_SetModeById_19(args...) Func_020045f4(args)
#define BattleRuntime_WaitIfModeZero_26(args...) Func_0200458a(args)
#define Object_SetModeById_20(args...) Func_02004602(args)
#define BattleRuntime_WaitIfModeZero_27(args...) Func_02004598(args)
#define ObjectMotion_SetSpeedParameters_4(args...) Func_020046f6(args)
#define GameFlag_Set_1(a0) Call1(Func_02004594, a0)
#define BattleRuntime_ScheduleShoulderButtonModeUpdate_1_02001688(args...) Func_020045d6(args)
#define FieldScene_RunScene3b9_02000334 Func_02000334
#define FieldScene_RunScene3b9_0200039c Func_0200039c
#define FieldScene_RunScene3b9_02000468 Func_02000468
#define FieldScene_RunScene3b9_020004c8 Func_020004c8
#define FieldScene_RunScene3b9_0200055c Func_0200055c
#define FieldScene_RunActorSeventeenDialogueSteps Func_020005f0
#define FieldScene_RunScene3b9_02000648 Func_02000648
#define FieldScene_RunConditionalSceneSetup Func_020011c4
#define FieldScene_RunMiddleSequence Func_02001688
#define FieldScene_RunScene3b9_020023e0 Func_020023e0
#define FieldScene_RunScene3b9_020024d8 Func_020024d8
#define FieldScene_RunScene3b9_020025f0 Func_020025f0
#define FieldScene_RunScene3b9_02002668 Func_02002668
#define FieldScene_RunScene3b9_02002820 Func_02002820
#define FieldScene_RunScene3b9_02002904 Func_02002904
#define FieldScene_RunScene3b9_02002964 Func_02002964
#define FieldScene_RunScene3b9_020006bc Func_020006bc
#define SceneState_ApplyFlags565And564 Func_020011ac
#define SceneState_SetRuntimeWord448To513 Func_0200160c
#define SceneState_DispatchByStatus448 Func_02001a14
#define SceneState_ForwardMaskedHalfwordWith10 Func_02001c5c
#define FieldScene_BuildActorPresentationGroup Func_02001298
#define FieldScene_DispatchBySelector Func_02001a4c
#define SceneState_ApplyRectsByFlags2384And962 Func_02001c6c
#define FieldScene_BuildActorPresentationSequence Func_02001cd4
#define SCENE_WORK (*(u8 **)0x03001ebc)
#define SCENE_PHASE_02001cd4 (*(s32 *)(SCENE_WORK + 0x1c0))
#define SCENE_FIELD_1C8_02001cd4 (*(s32 *)(SCENE_WORK + 0x1c8))
#define SCENE_STEP (*(u16 *)(SCENE_WORK + 0x1d8))
#define BattleRuntime_Reset Func_020048d4
#define BattleRuntime_WaitIfModeZero(a0) Call1(Func_02004966, a0)
#define UiWork_WaitThenFinalizeCapacity(a0, a1) Value2(Func_02004d3c, a0, a1)
#define ObjectMotion_SetHorizontalPositionWithTerrain(a0, a1, a2) Call3(Func_02004942, a0, a1, a2)
#define ObjectMotion_SetSpeedParameters(a0, a1, a2) Call3(Func_02004f2e, a0, a1, a2)
#define ObjectMotion_EnableActionAndSetCallback_1_02001cd4 Func_02004efc
#define ObjectMotion_EnableActionAndSetCallback_2 Func_02004f04
#define ObjectMotion_MarkActiveAndSetActionCallback Func_02004f1c
#define ObjectMotion_ResetAndSetPosition(a0, a1, a2) Call3(Func_02004f86, a0, a1, a2)
#define ObjectMotion_SetPositionAndCommit(a0, a1, a2) Call3(Func_02004f74, a0, a1, a2)
#define ObjectMotion_ResetAndSetPositionInMode2(a0, a1, a2) Call3(Func_02004fd0_a, a0, a1, a2)
#define ObjectMotion_SetPositionAndReset(a0, a1, a2) Call3(Func_02004fca, a0, a1, a2)
#define Object_SetModeById(a0, a1) Call2(Func_02004b5e, a0, a1)
#define ObjectMotion_CallThenWaitForAnimationChange(a0, a1) Call2(Func_02004a62, a0, a1)
#define ObjectMotion_SetVariantCallback(a0, a1) Call2(Func_02004ace, a0, a1)
#define ObjectMotion_SetVariantCallbackAndInvokeObject(a0, a1) Call2(Func_020049fe, a0, a1)
#define SceneWork_SetStepValue(a0) Call1(Func_02004a7c, a0)
#define ObjectMotion_ArmCallback(a0, a1, a2) Call3(Func_02004ae0, a0, a1, a2)
#define BattleEffect_SpawnLinkedResourceObject(a0, a1, a2) Call3(Func_02004a98, a0, a1, a2)
#define ObjectMotion_SetSpeedLimitAndAcceleration(a0, a1) Call2(Func_0200503c, a0, a1)
#define ObjectMotion_PlaceWithinCameraBounds(a0, a1, a2, a3) Call4(Func_02004a06, a0, a1, a2, a3)
#define ActorPresentation_SetActorsTwelveToEighteen Func_02002b1c
#define FieldScene_CallPairWith10 Func_02001c48

extern u8 Value_0000008c;
extern u8 Value_0000008e;
extern u8 Data_0200b094[];
extern u8 Data_0200b274[];
extern u8 Data_0200b034[];
extern u8 Data_0200b2bc[];   /* image offset 0x32bc */
extern u8 Value_0000008d;
extern u8 Data_0200be70[];
extern u8 Data_0200c110[];
extern u8 Data_0200be94[];
extern u8 Data_0200bf60[];
extern u8 Data_0200be64[];
extern u8 Data_0200adac[];
extern u8 Value_00002125[];

void Func_02002e98(void);
s32 Func_02002e66(s32 flag);
void Func_02002f48(s32 msg);
void Func_02002f60(s32 id, s32 arg1);
void Func_02002f58(s32 msg);
void Func_02002f80(s32 id, s32 arg1);
void Func_02002ecc(void);
void Func_02002edc(void);
s32 Func_02002eaa(s32 flag);
void Func_02002fc2(s32 id, s32 arg1, s32 arg2);
void Func_02002f98(s32 msg);
void Func_02002fb0(s32 id, s32 arg1);
void Func_02002fa8(s32 msg);
void Func_02002fc0(s32 id, s32 arg1);
void Func_02002f1c(void);
void Func_02003104();
void Func_020031aa_a();
void Func_020031aa_b();
void Func_02002164();
void Func_02002174();
void Func_020031a0();
void Func_02003198();
void Func_0200218a();
void Func_020021a8();
void Func_0200314c();
void Func_02001f9e();
void Func_02001fd2();
s32 Func_02002efa();
void Func_02002f2c();
void Func_02002f52();
void Func_02002f86();
void Func_02002fc6();
void Func_02002fe4_a();
void Func_02002fe4_b();
void Func_02003012();
s32 Func_02003022();
void Func_0200302a();
s32 Func_02002f62();
s32 Func_02002f6e();
void Func_02002f94();
s32 Func_02002fd8();
void Func_02002ff0();
s32 Func_02003008_a();
s32 Func_02003008_b();
void Func_02003046();
void Func_02003050();
void Func_02003058();
s32 Func_02003068();
s32 Func_02003098();
void Func_020030b8();
void Func_020030c8();
void Func_020030d2();
void Func_020030da();
void Func_020030fa();
void Func_020020d8();
void Func_02003068_a();
s32 Func_0200307a();
void Func_0200309e();
void Func_020030b8_a();
void Func_020030d4();
void Func_02003108();
void Func_02003114();
void Func_0200311e();
void Func_02003144();
void Func_02002132();
void Func_020030c0();
void Func_020030fe();
void Func_0200315a();
void Func_02003178();
void Func_020031a2_a();
void Func_020031a2_b();
void Func_020031ba();
void Func_020021e8();
void Func_020021ee();
void Func_02002200();
void Func_02002210();
void Func_0200222e();
s32 Func_0200313a();
void Func_02003154();
void Func_02003174();
void Func_0200319c();
void Func_020031d8();
void Func_020031e4();
void Func_02003202();
void Func_0200320e();
void Func_02003216();
void Func_02003224();
void Func_0200322e();
void Func_02003286();
void Func_020031e8();
void Func_02003282();
void Func_02003298();
void Func_02002252();
void Func_0200327e();
void Func_02003286_a();
void Func_02002268();
void Func_020032a4();
void Func_02002276();
void Func_02002294();
void Func_02003238();
void Func_020022aa();
void Func_020022de();
void Func_020022ec();
s32 Func_0200230a();
void Func_02003240();
void Func_020032ae();
void Func_020032da();
void Func_020032f0();
void Func_0200330a();
void Func_02003314();
void Func_0200332a();
void Func_02003336();
void Func_02003342();
void Func_02002e32();
s32 Func_02003d8a();
void Func_02003da8();
void Func_02003dbc();
s32 Func_02003e1a();
void Func_02003e62();
void Func_02003e70();
void Func_02003e74();
void Func_02003e78();
void Func_02003e9a();
void Func_02003eb2_a();
void Func_02003eb2_b();
void Func_02003f00();
s32 Func_02003f06();
void Func_02003f1a();
void Func_02003f26();
void Func_02003f64();
void Func_02003f6c();
void Func_02003f78();
void Func_02003f84();
s32 Func_020034f2();
void Func_02004280();
void Func_020042ae_a();
void Func_020042ae_b();
void Func_020042be();
void Func_02004308();
s32 Func_02004310_a();
s32 Func_02004310_b();
void Func_02004318();
s32 Func_0200431c();
void Func_02004320();
s32 Func_02004326();
void Func_02004328();
void Func_02004330();
void Func_02004338();
void Func_02004340();
void Func_02004342_a();
void Func_02004342_b();
void Func_02004350();
void Func_02004352();
void Func_0200435a();
void Func_0200436c();
void Func_0200437a();
void Func_02004380();
void Func_02004382_a();
void Func_02004382_b();
void Func_02004388();
void Func_02004396();
void Func_020043c6();
void Func_020043d4();
void Func_020043d8();
void Func_020043da();
void Func_020043e2();
void Func_020043e4();
void Func_020043f0();
void Func_020043f2();
void Func_020043fe();
void Func_02004400();
void Func_02004402();
void Func_0200440e_a();
void Func_0200440e_b();
void Func_02004426();
void Func_02004428();
void Func_0200443e();
void Func_0200444a();
void Func_0200445a();
void Func_02004460();
void Func_02004468();
void Func_0200446c();
void Func_0200447a();
void Func_0200447c();
void Func_0200448a();
s32 Func_020044b0();
void Func_020044d4();
void Func_020044da();
void Func_020044e8();
void Func_020044ea();
void Func_020044f4();
void Func_020044f6();
void Func_02004500();
void Func_02004502();
void Func_0200450a();
s32 Func_02004512();
void Func_0200451c_a();
void Func_0200451c_b();
void Func_02004520();
void Func_02004526();
void Func_02004530();
void Func_02004536();
void Func_0200453a();
void Func_0200453c();
void Func_0200453e();
void Func_02004548();
void Func_0200455c();
void Func_0200456a_a();
void Func_0200456a_b();
void Func_0200456e();
void Func_0200457c();
void Func_0200458a();
void Func_02004594();
void Func_02004598();
void Func_020045ae();
void Func_020045be();
void Func_020045d6();
void Func_020045d8();
void Func_020045e6();
void Func_020045f4();
void Func_02004602();
void Func_02004630();
void Func_02004658();
void Func_0200466e();
void Func_0200469c();
void Func_020046d2();
void Func_020046ea();
void Func_020046f6();
void Func_02004fa0();
s32 Func_02004fa2();
void Func_02004fb4();
void Func_02004fd0();
void Func_02004fd2();
void Func_02004ff4();
void Func_0200503a();
s32 Func_02005042();
void Func_02005044();
void Func_02005062();
s32 Func_0200506c();
void Func_02005090();
void Func_020050a2();
void Func_020050a6();
void Func_020050aa();
void Func_020050ac();
void Func_020050c2();
void Func_020050c8();
void Func_020050d6();
void Func_020050f0();
void Func_0200515a();
void Func_020051b6();
void Func_020051be();
void Func_020051ca();
void Func_020051d6();
void Func_020041c0();
void Func_02005092();
void Func_020050d8();
void Func_020050de();
s32 Func_020050ea();
void Func_02005124();
void Func_02005176_a();
void Func_02005176_b();
void Func_020051b4();
void Func_020051ce();
void Func_020051de();
void Func_020051e2();
void Func_020051ec_a();
void Func_020051ec_b();
void Func_020051f8();
void Func_02005204_a();
void Func_02005204_b();
void Func_02005206();
void Func_02005210();
void Func_02005218();
void Func_0200521c();
void Func_02005228();
void Func_02005260();
void Func_0200526c();
void Func_02005278();
void Func_02005282();
void Func_0200528e();
void Func_0200529a();
void Func_020051e8();
s32 Func_020051fe();
void Func_02005212();
void Func_02005254();
void Func_02005262();
void Func_02005266();
void Func_02005268();
void Func_0200531e();
void Func_02005348();
void Func_02005350();
void Func_0200535c();
void Func_02005368();
void Func_02004308_a();
void Func_02004316();
void Func_02004324();
void Func_0200433a();
void Func_020043b8();
void Func_02005260_a();
void Func_02005290();
void Func_020052ce();
void Func_020052dc();
void Func_020052ea();
void Func_02005310();
void Func_02005328();
void Func_0200532c();
void Func_0200533a();
void Func_02005340();
void Func_02005344();
void Func_02005348_a();
void Func_02005348_b();
void Func_0200534e();
void Func_02005352();
void Func_0200535a();
void Func_02005360();
s32 Func_02005362();
void Func_0200537a();
void Func_0200538e();
void Func_0200539c();
void Func_020053a6();
void Func_020053b6();
void Func_020053ba();
void Func_020053c6();
void Func_020053ca();
void Func_020053d0();
void Func_020053d6();
void Func_020053d8();
void Func_020053e0();
void Func_020053e2();
void Func_020053e8();
void Func_020053ec();
void Func_0200542c();
void Func_02005432();
void Func_0200549e();
void Func_020054b8();
void Func_020054fe();
void Func_0200551c();
void Func_02005528();
void Func_020053e0_a();
s32 Func_020053e2_a();
void Func_020053f4();
void Func_02005410();
void Func_02005412();
void Func_02005434();
void Func_0200547a();
s32 Func_02005482();
void Func_02005484();
void Func_020054a2();
void Func_020054d2();
void Func_020054e4();
void Func_020054e8();
void Func_020054ec();
void Func_020054ee();
void Func_02005504();
void Func_0200550a();
void Func_02005518();
void Func_02005530();
void Func_0200559c();
void Func_020055ee();
void Func_0200560c();
void Func_02005618();
void Func_020054fc();
void Func_02005526();
void Func_02005568();
void Func_02005576();
void Func_0200557a();
void Func_0200557c();
void Func_02005632();
void Func_02005652();
void Func_02005670();
void Func_0200567c();
void Func_02004604();
void Func_02004612();
void Func_02004620();
void Func_02004636();
void Func_020046b4();
void Func_0200555c();
void Func_0200558c();
void Func_020055ca();
void Func_020055d8();
void Func_020055e6();
void Func_0200560c_a();
void Func_02005624();
void Func_02005628();
void Func_02005636();
void Func_0200563c();
void Func_02005640();
void Func_02005644_a();
void Func_02005644_b();
void Func_0200564a();
void Func_0200564e();
void Func_02005656();
void Func_0200565c();
s32 Func_0200565e();
void Func_02005676();
void Func_0200568a();
void Func_02005698();
void Func_020056a2();
void Func_020056b2();
void Func_020056b6();
void Func_020056c2();
void Func_020056c6();
void Func_020056cc();
void Func_020056d2();
void Func_020056d4();
void Func_020056dc();
void Func_020056de();
void Func_020056e4();
void Func_020056e8();
void Func_02005728();
void Func_0200572e();
void Func_0200579a();
void Func_020057b4();
void Func_020057fa();
void Func_02005818();
void Func_02005824();
void Func_020032b4();
s32 Func_0200334e();
void Func_02003364();
s32 Func_02003374();
s32 Func_020032e4();
void Func_020033a0();
void Func_020032fc();
void Func_02003d7e_a();
void Func_02003d7e_b();
void Func_02004204();
void Func_0200433e();
void Func_02004352_a();
void Func_02004218();
void Func_02003292();
void Func_020042d0();
s32 Func_020041fe();
void Func_020032a2();
void Func_020032c0();
void Func_0200422e();
void Func_0200426a();
void Func_02003476(void);
void Func_020036a2(void);
void Func_02004922(s32 arg0, u16 arg1, s32 arg2);
void Func_02003e9a_a();
void Func_02003f9c();
void Func_02003ff2_a();
void Func_02003ff2_b();
void Func_02003f22();
void Func_02003f2a();
void Func_02003f32();
void Func_02003f3a();
void Func_02003f42();
void Func_02003f4a();
void Func_02003f52();
void Func_02003f5a();
void Func_02003f62();
void Func_02003f6a();
void Func_02003f72();
s32 Func_02003f28();
void Func_02003ec6_a();
void Func_02003ec6_b();
s32 Func_02003f34();
s32 Func_02003f3e();
void Func_02003f98_a();
void Func_02003f98_b();
void Func_02003ed6();
void Func_02004086();
void Func_0200409a();
void Func_02003f60();
void Func_02003ff8();
void Func_02003f6e();
void Func_02003fa0();
void Func_02003fe4();
void Func_02003f8a();
void Func_02004002();
void Func_02004010();
void Func_02003fa6();
void Func_0200401e();
void Func_02003fb4();
void Func_02004020();
void Func_0200402c_a();
void Func_0200402c_b();
void Func_020040a8();
void Func_02004044();
void Func_0200405c();
void Func_0200408a();
void Func_02004000();
void Func_02004098();
void Func_0200400e();
void Func_02004046();
void Func_0200401c();
void Func_02004106();
void Func_02004108();
void Func_02004114_a();
void Func_02004114_b();
void Func_0200411e();
void Func_0200412a();
void Func_02004144();
void Func_0200414e();
void Func_02004158();
void Func_02004162();
void Func_02004088();
void Func_02004164();
void Func_02004170();
void Func_02004180();
s32 Func_02003128();
void Func_020040f6();
s32 Func_020040e4();
void Func_020040d8();
void Func_02004150();
void Func_020040e6();
void Func_0200415e();
void Func_02004166();
void Func_020040fc();
void Func_020041d6();
void Func_0200417e();
void Func_0200418c();
void Func_02004194();
void Func_0200419c();
void Func_020041b4();
void Func_0200413a();
void Func_020041c2();
void Func_02004148();
void Func_0200417c();
void Func_020041c0_a();
void Func_020041ca();
void Func_020041e2();
void Func_020041ea();
void Func_020041e4();
void Func_020041f0();
void Func_02004202();
void Func_020042e6();
void Func_020042f2();
void Func_020042d4();
void Func_020041d8();
void Func_020045de(s32 arg0);
void Func_020047da(s32 arg0, s32 arg1);
void Func_020047e2(s32 arg0, s32 arg1);
void Func_020047ec(s32 arg0, s32 arg1);
void Func_020047ec_a(s32 arg0);
void Func_020047f4(s32 arg0, s32 arg1);
s32 Func_0200475a(s32 arg0);
void Func_020031b2(void);
void Func_02002e44(void);
void Func_0200323a(void);
void Func_0200388c(void);
void Func_020047a0(void);
void Func_02003fa2(void);
void Func_020041b8(void);
void Func_02004796(s32 arg0);
void Func_020046f2(void);
s32 Func_02004798(s32 arg0);
void Func_020040bc(void);
void Func_020047c0(s32 arg0);
void Func_020047c6(s32 arg0);
void Func_020047cc(s32 arg0);
void Func_020047c2(s32 arg0);
void Func_0200426a_a(void);
void Func_02004428_a(void);
void Func_02004512_a(void);
void Func_020047ea(s32 arg0);
void Func_020047f0(s32 arg0);
void Func_020047f6(s32 arg0);
void Func_02004590(void);
void Func_020048f2(s32 no, s32 arg1);
void Func_02004840(s32 no);
s32 Func_02004832();
void Func_02004820();
void Func_0200483c();
void Func_02004936();
s32 Func_0200486c();
void Func_02004864();
void Func_020048d4();
void Func_02004966();
s32 Func_02004d3c();
void Func_02004942();
void Func_02004f2e();
void Func_02004efc();
void Func_02004f04();
void Func_02004f1c();
void Func_02004f86();
void Func_02004f74();
void Func_02004fd0_a();
void Func_02004fca();
void Func_02004b5e();
void Func_02004a62();
void Func_02004ace();
void Func_020049fe();
void Func_02004a7c();
void Func_02004ae0();
void Func_02004a98();
void Func_0200503c();
void Func_02004a06();
void Func_02003a36();
void Func_02003a00();
void Func_0200489c();
s32 Func_02004da2();
void Func_020050b2();
void Func_02004ac0();
void Data_0200ad0c();
void Func_02004a4c();
void Func_02004a76();
void Func_020050d0();
void Func_02004a8a();
void Func_020057b0();
void Func_020057b8();
void Func_020057c0_a();
void Func_020057c0_b();
void Func_020057c8();
void Func_020057c8_a();
void Func_020057c8_b();
void Func_020057d0();
void Func_020057d8();
void Func_020057d8_a();
void Func_020057e0();
void Func_020057e0_a();
void Func_020057e8();

/* resource_3b9:0x02001298, 884 bytes: the middle presentation script for
 * actor 16, a sibling of FieldScene_RunMiddleSequence (0x02001688) with the
 * same opening, scene-phase writes and closing, without the stepping loop. */

/*
 * resource_3b9 owner at 0x02001c48, 12 bytes plus one alignment halfword: a
 * two-call trampoline forwarding the argument to the first callee with a
 * fixed second argument, then making a second call with a fixed constant.
 * The argument is never freshly loaded before the first call, so it is a
 * pass-through parameter.  Neither callee is identified beyond call shape.
 */

/* FieldScene_BuildActorPresentationSequence - resource_3b9:02001cd4, 1804 bytes.
 *
 * One straight-line field-scene presentation script for actor records 0-3 and
 * 8-11. It resets the battle runtime, places the three lead actors, raises the
 * scene phase word, then walks a long ordered list of spawn, motion, callback
 * and wait calls. Two thirds of the way through it asks the UI work helper
 * whether the player already finished the pending step: the answer selects a
 * short path (one paired call) or a long one (an animation-change wait, a
 * masked-halfword forward and the same paired call), and both paths bump the
 * scene step counter exactly once. Near the end it arms the action callbacks
 * of actors 1, 2 and 3 with one shared 0x0200adac word, then walks actors 11
 * and 0 to their closing positions. It ends by lowering the scene phase word
 * back to 256, storing 40 at field 0x1c8 and running the three closing
 * helpers.
 *
 * Uncertain: the argument roles of the unnamed veneers below, whether the
 * 0x0200adac word passed to the three callback helpers is a function pointer
 * or a work record, and whether the second scene-step bump is a separate
 * source statement or a shared tail the compiler duplicated.
 *
 * Residual: candidate=1804 reference=1804 differing_halfwords=11,
 * topology=equal, wrong_instructions=0, class=scheduling-floor. The candidate
 * emits exactly the reference's instructions; two short runs sit in a
 * different order.
 *
 *   - at owner offset 0x42c the two constant arguments of the Func_02004da2
 *     call are set r0 before r1 where the reference sets r1 before r0;
 *   - the pc-relative load of 0x0200adac that the three action-callback calls
 *     share lands at owner offset 0x5f8, before the third
 *     ObjectMotion_SetSpeedParameters call; the reference issues it at 0x604,
 *     after that call and before its first use, which also
 *     swaps the r0/r1 setup of the first of those three calls: the reference
 *     schedule fills the load-use gap with `movs r0, #1`, ours has no gap to
 *     fill.
 *
 * Neither survived respelling. Wrapping or prototyping the Func_02004da2 site
 * leaves it unchanged. Spelling the first action-callback site through the
 * constant-passing wrapper below, or through a shared local, moves the load
 * but does not place it after the call: the wrapper form measures 8 differing
 * halfwords, and is not used here because it buys that number by emitting a
 * second `ldr r1, [pc]` the reference does not contain and dropping an
 * `adds r1, r5, #0` it does (wrong_instructions=2, class=missing-extra-code).
 */

/* Shared scene work record, reached through the pointer at 0x03001ebc. The
 * three field offsets are the ones the adopted sibling
 * games/gs1/src/overlays/scene_primary_script/run_scene_3b9_conditional_scene_setup.c
 * already spells. */

/* Overlay call bindings.
 *
 * The loader rewrites every BL-shaped halfword pair, so the word an overlay
 * image stores for a call site depends on the site as well as on the callee.
 * The legacy Func_<address> alias a disassembler prints is therefore per site,
 * and the 180 call sites in this owner reach only 31 distinct runtime targets.
 * Each declaration below is one runtime target, spelled with an alias that the
 * reference binds to that target and to no other; its comment gives the
 * overlay veneer the call enters and the main-image entry that veneer reaches.
 *
 * The alias a site produces folds in that site's own position: it is
 * RESOURCE_BASE + (owner offset + site + 4) + the stored displacement, while
 * the runtime target the loader reaches is RUNTIME_BASE + that displacement
 * + 2. Two sites therefore share one alias whenever they are as far apart as
 * their two targets. That is the case for the veneer at 0x0200ad0c: it is
 * reached only from 0x02001d28 and 0x02001d64, and each of those sits exactly
 * eight bytes before a site that calls the neighbouring veneer at 0x0200ad04,
 * so both aliases the reference offers for it - Func_02004a36 and
 * Func_02004a72 - name two targets and neither can bind. Every other target
 * in this owner has at least one alias of its own, and is declared under it
 * exactly once.
 *
 * The one unbindable target is spelled Data_0200ad0c: a name carrying the
 * runtime address directly, which is the only spelling that reaches an
 * overlay-space address without going through the per-site alias table
 * (a Func_02xxxxxx name is always resolved as a legacy alias). The emitted
 * call word matches the reference at both sites. If this owner is ever given
 * a translation-unit entry, that entry should declare the runtime symbol
 * instead and the name can go back to the ordinary Func_ form.
 *
 * Names come from the established spelling of the same main-image entry in
 * already adopted overlay sources. Unnamed veneers keep their alias.
 */

/* 0x0200abf4 -> 0x0808a018 */

/* 0x0200abec -> 0x0808a010 */

/* 0x0200ac04 -> 0x0808a070 */

/* 0x0200ac54 -> 0x0808a0f0 */

/* 0x0200ac14 -> 0x0808a090 */

/* 0x0200ac1c -> 0x0808a098. Two sites reach this veneer, at 0x020022de and
 * 0x020022e6; each keeps its own unambiguous alias. */

/* 0x0200ac2c -> 0x0808a0b0 */

/* 0x0200ac34 -> 0x0808a0b8 */

/* 0x0200ac3c -> 0x0808a0c0 */

/* 0x0200ac44 -> 0x0808a0c8 */

/* 0x0200ac4c -> 0x0808a0d0 */

/* 0x0200ac5c -> 0x0808a100 */

/* 0x0200ac6c -> 0x0808a110 */

/* 0x0200ac74 -> 0x0808a130 */

/* 0x0200ac7c -> 0x0808a138 */

/* 0x0200ac94 -> 0x0808a170 */

/* 0x0200acbc -> 0x0808a1b8 */

/* 0x0200acc4 -> 0x0808a1e8 */

/* 0x0200acdc -> 0x0808a208 */

/* 0x0200ace4 -> 0x0808a210 */

/* 0x02009c48, the owner registered as FieldScene_CallPairWith10. Its readable
 * name is bound to Func_02001c48, which is not one of this owner's call words,
 * so the call keeps its alias. Takes one record id. */

/* 0x02009c5c, the owner registered as SceneState_ForwardMaskedHalfwordWith10;
 * same reason. Takes a record id and the masked halfword. */

/* Unnamed veneers: no adopted source spells these main-image entries, so the
 * alias stands and the comment records the entry and the observed arity. */

/* 0x0200ab8c -> 0x080000c0, (arg0) */

/* 0x0200ac9c -> 0x0808a178, (id, arg1) */

/* 0x0200acfc -> 0x0808a248, (arg0) */

/* 0x0200ad04 -> 0x0808a330, (arg0, arg1) */

/* 0x0200ad0c -> 0x0808a338, (arg0, arg1). Named by runtime address rather
 * than by a legacy alias; see the binding note above. */

/* 0x0200ad14 -> 0x0808a348, (arg0) */

/* 0x0200ad1c -> 0x0808a360, () */

/* 0x0200ad24 -> 0x0808a368, () */

/* 0x0200ad2c -> 0x0808a370, () */

/* Integration note. This owner has no translation-unit entry, so it scores
 * standalone with `alchemy score <this file> --owner resource_3b9:02001cd4`.
 * The only symbol that cannot come from the reference alias table is
 * Data_0200ad0c; if an entry is ever added, declare
 * {"address": "0x0200ad0c", "kind": "thumb"} for it and the call can be
 * spelled with an ordinary Func_ name again. Nothing else in this file
 * depends on that entry existing.
 */

/* Loader-relocated overlay calls: each symbol names the pre-relocation call
 * word the image holds. */

/* Resolved engine calls: each pseudo symbol is the per-site call word the
 * overlay image holds (a word can serve two sites with different targets),
 * and the macro names the engine function the site reaches through the
 * overlay veneer and the main-image veneer island, keeping the site's own
 * calling form. Names without a repository binding are provisional.
 */

/* Phase/status word at 0x1c0 of the shared scene work record. */

/* A second word at 0x1c8 of the shared scene work record, set only on the
 * flag-clear path below. */

/* Record id passed to every Func_02003exx call below. */

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

static __inline__ s32 Value2(s32 (*f)(), s32 a0, s32 a1)
{
    extern u8 Data_03001ebc[];

    return f(a0, a1);
}

/* The scene step counter at 0x1d8 of the shared scene work record. */
static __inline__ void bump_step(s32 amount)
{
    extern u8 Data_03001ebc[];

    u8 *work = *(u8 **)Data_03001ebc;

    *(u16 *)(work + 0x1d8) = (u16)(*(u16 *)(work + 0x1d8) + amount);
}

static __inline__ void Call3(void (*f)(), s32 a0, s32 a1, s32 a2)
{
    extern u8 Data_03001ebc[];

    f(a0, a1, a2);
}

static __inline__ void Call2(void (*f)(), s32 a0, s32 a1)
{
    extern u8 Data_03001ebc[];

    f(a0, a1);
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
static __inline__ s32 Value1_020025f0(s32 (*f)(), s32 a0)
{
    extern u8 Data_03001ebc[];

    return f(a0);
}

/* Call sites spelled through these wrappers pass their constants straight
 * into the argument registers; a direct call precomputes a costly constant
 * into a pseudo that the compiler then shares with later uses in the block.
 * A value-returning call also sets r0 last of its arguments. */
static __inline__ void Call3_02002904(void (*f)(), s32 a0, s32 a1, s32 a2)
{
    extern u8 Data_03001ebc[];

    f(a0, a1, a2);
}

static __inline__ __attribute__((always_inline)) void bump_step_020006bc(s32 amount)
{
    extern u8 Data_03001ebc[];

    u8 *scene = *(u8 **)Data_03001ebc;

    *(u16 *)(scene + 0x1d8) = (u16)(*(u16 *)(scene + 0x1d8) + amount);
}

/* A call site spelled through one of these wrappers passes its constants
 * straight into the argument registers; a direct call lets the compiler
 * precompute a costly constant into a pseudo and share it with later uses in
 * the block. Most sites here want the first shape; the three action-callback
 * sites near the end want the second, because the reference reaches all three
 * of them through one shared pointer register. The adopted sibling
 * run_scene_3b9_conditional_scene_setup.c uses the same device for the same
 * reason. */
static __inline__ void Call1_02001cd4(void (*f)(), s32 a0)
{
    f(a0);
}

#if defined(GS1_EDITION_JA)
#define SCENE_STEP_VALUE 0x22c1
#elif defined(GS1_EDITION_DE) || defined(GS1_EDITION_ES) || defined(GS1_EDITION_FR) || defined(GS1_EDITION_IT)
#define SCENE_STEP_VALUE 0x211a
#else
#define SCENE_STEP_VALUE 0x2138
#endif

/* Loader-relocated overlay calls: each symbol names the pre-relocation call
 * word the image holds. */
extern u8 Data_0200adac[];

/*
 * resource_3b9 owner at 0x02000074: a leaf that loads its literal pool word
 * and returns it. The eight-byte owner includes that one pool word at
 * 0x02000078, holding the address 0x0200b2bc -- image offset 0x32bc --
 * which is returned without being dereferenced.
 */
s32 SceneData_SelectTableBySceneId(void)
{
    extern s16 Data_02000240[];

    s16 v = Data_02000240[224];

    if (v == (s32)&Value_0000008c) {
        return (s32)Data_0200b094;
    }
    if (v == (s32)&Value_0000008e) {
        return (s32)Data_0200b274;
    }
    return (s32)Data_0200b034;
}

s32 Func_02000070(void)
{
    return 0;
}

u8 *SceneData_GetTableB2bc(void)
{
    extern s16 Data_02000240[];

    return Data_0200b2bc;
}

s32 SceneData_SelectDataBySelectorAndSubstate(void)
{
    extern s16 Data_02000240[];

    s16 v = Data_02000240[224];

    if (v == (s32)&Value_0000008d) {
        return (s32)Data_0200be70;
    }
    if (v == (s32)&Value_0000008c) {
        if (Data_02000240[225] == 12) {
            return (s32)Data_0200c110;
        }
        return (s32)Data_0200be94;
    }
    if (v == (s32)&Value_0000008e) {
        return (s32)Data_0200bf60;
    }
    return (s32)Data_0200be64;
}

/* Contiguous unnamed leaf-owner run for resource_3b9. */
void SceneDialogue_RunActor10MessageByFlag962(void)
{
    Func_02002e98();
    if (Func_02002e66(0x962)) {
        Func_02002f48(0x2251);
        Func_02002f60(10, 0);
    } else {
        Func_02002f58(0x2057);
        Func_02002f80(10, 0);
    }
    Func_02002ecc();
}

void SceneDialogue_RunActor13MessageByFlag962(void)
{
    Func_02002edc();
    if (Func_02002eaa(0x962)) {
        Func_02002fc2(13, 258, 40);
        Func_02002f98(0x2254);
        Func_02002fb0(13, 0);
    } else {
        Func_02002fa8(0x205c);
        Func_02002fc0(13, 0);
    }
    Func_02002f1c();
}

void FieldScene_RunScene3b9_02000334(void)
{
    extern u8 Data_03001ebc[];

    u32 i;
    s32 record;

    Func_02002f2c();
    if (Value1(Func_02002efa, 0x962) != 0) {
        Func_02002fc6(14, 2);
        Call1(Func_02002fe4_a, 0x2256);
        Func_02001f9e(14);
        Func_02002fe4_b(14, 0, 0);
        Func_02002f52(20);
        Value2(Func_02003022, 14, 0);
        Func_02001fd2(14, 0);
    } else {
        Call1(Func_02003012, 0x205d);
        Func_0200302a(14, 0);
    }
    Func_02002f86();
}

void FieldScene_RunScene3b9_0200039c(void)
{
    extern u8 Data_03001ebc[];

    u32 i;
    s32 record;

    Func_02002f94();
    if (Value1(Func_02002f62, 0x962) != 0) {
        if (Value1(Func_02002f6e, 0x3c0) != 0) {
            Call1(Func_02003050, 0x225e);
        } else {
            Call1(Func_02003058, 0x225a);
            Value2(Func_02003068, 16, 0);
            if (Value2(Func_02002fd8, 0, 0) == 0) {
                bump_step(1);
                Call3(Func_020030b8, 16, 0x100, 40);
                Value2(Func_02003098, 16, 0);
                if (Value2(Func_02003008_a, 0, 0) == 0) {
                    bump_step(1);
                }
                ((void (*)())Func_02003008_b)(40);
                Func_020030c8(16, 0);
                Call1(Func_02002ff0, 0x3c0);
                goto L_02000448;
            }
        }
        Func_020030da(16, 0);
    } else {
        Call1(Func_020030d2, 0x205e);
        Func_020030fa(16, 0);
    }
    L_02000448:;
    Func_02003046();
}

void FieldScene_RunScene3b9_02000468(void)
{
    extern u8 Data_03001ebc[];

    u32 i;
    s32 rec7;
    s32 record;

    rec7 = Value1(Func_0200307a, 13);
    Func_02003068_a();
    Func_0200309e(13);
    Func_02003108(13, 0, 20);
    Call1(Func_0200311e, 0x2114);
    Func_020020d8(13);
    Func_02003114(13, 1);
    Func_02003144(13, 0);
    {
        u16 *target = (u16 *)(rec7 + 100);
        s32 shown = 0x2d0;

        *target = shown;
    }
    {
        u16 *target = (u16 *)(rec7 + 102);
        s32 shown = 112;

        *target = shown;
    }
    Func_020030d4(13, 2);
    Func_020030b8_a();
}

void FieldScene_RunScene3b9_020004c8(void)
{
    extern u8 Data_03001ebc[];

    u32 i;
    s32 record;

    Func_020030c0();
    Call2(Func_020031a2_a, 14, 0x102);
    Func_0200315a(14, 2);
    Call1(Func_02003178, 0x2116);
    Func_02002132(14);
    Call3(Func_020031ba, 14, 0x102, 40);
    Func_020031a2_b(14, 0);
    Func_020030fe();
}

void SceneDialogue_ShowLine2118WithActor15Steps(void)
{
    Func_02003104();
    Func_020031aa_a(0x2118);
    Func_02002164(15);
    Func_020031aa_b(15, 0, 20);
    Func_02002174(15);
    Func_020031a0(15, 3);
    Func_02003198(15, 0);
    Func_0200218a(15);
    Func_020021a8(15, 20480);
    Func_0200314c();
}

void FieldScene_RunScene3b9_0200055c(void)
{
    extern u8 Data_03001ebc[];

    u32 i;
    s32 record;

    Func_02003154();
    Func_020031e4(16, 2);
    Call1(Func_02003202, 0x211b);
    Func_02003224(16, 0, 20);
    if (Value1(Func_0200313a, 0x3c1) != 0) {
        Func_02003174(20);
    } else {
        Func_020021ee(17, 0);
        Func_02003216(17, 1);
        Func_020021e8(17);
        Func_0200322e(17, 0, 20);
        Func_0200320e(17, 4);
        Func_02002200(17);
        Call3(Func_02003286, 17, 0x105, 40);
        Func_02002210(17);
        Call2(Func_0200222e, 17, 0x5000);
        Call1(Func_0200319c, 0x3c1);
    }
    Func_020031d8();
}

void FieldScene_RunActorSeventeenDialogueSteps(void)
{
    extern u8 Data_03001ebc[];

    Func_020031e8();
    Func_02003282(17, 0, 20);
    Func_02003298(0x211f);
    Func_02002252(17);
    Func_0200327e(0, 3);
    Func_02003286_a(17, 3);
    Func_02002268(17);
    Func_020032a4(17, 1);
    Func_02002276(17);
    Func_02002294(17, 20480);
    Func_02003238();
}

void FieldScene_RunScene3b9_02000648(void)
{
    extern u8 Data_03001ebc[];

    u32 i;
    s32 record;

    Func_02003240();
    Func_020032da(18, 0, 20);
    Call1(Func_020032f0, 0x2122);
    Func_020022aa(18);
    Call3(Func_0200332a, 18, 0xd000, 20);
    Call3(Func_02003336, 18, 0xb000, 20);
    Call3(Func_02003342, 18, 0x8000, 40);
    Func_02003314(18, 0, 20);
    Func_020022de(18);
    Func_0200330a(18, 3);
    Func_020022ec(18);
    Value2(Func_0200230a, 18, 0x5000);
    Func_020032ae();
}

void FieldScene_RunScene3b9_020006bc(void)
{
    extern u8 Data_03001ebc[];

    Func_020032b4();
    Func_0200334e(8, 0, 20);
    Func_02003364((s32)&Value_00002125);
    Func_02003374(8, 0);
    if (Func_020032e4(0, 0) != 0)
        bump_step_020006bc(1);
    Func_020033a0(8, 0);
    Func_020032fc();
}

/*
 * Story-flag hand-off in overlay resource_3b9: clear one flag, set its
 * immediate neighbour.  It is installed into a script record rather than
 * called, and each Func_ name spells a relocated call word.
 */

/*
 * resource_3b9 owner at 0x02001c5c, 12 bytes with no literal pool: a
 * single-call trampoline forwarding `arg0' unchanged, `arg1' masked to its
 * low 16 bits, and a fixed third argument.  `arg0' is never freshly loaded
 * before the call, so it is a pass-through parameter.  The callee is not
 * identified beyond call shape.
 */

/*
 * The two flag ids are adjacent but spelled differently, and the spellings
 * are load-bearing.  565 is 0x235 and comes from the owner's single pool
 * word; 564 is built as movs #141 / lsls #2, so it must stay a plain decimal
 * value rather than another pool constant.  The 24-byte owner covers that
 * pool word and the alignment halfword after it.  What the pair gates is not
 * established.
 */
void SceneState_ApplyFlags565And564(void)
{
    extern u8 Data_02000240[];
    extern u8 *Data_03001ebc;

    Func_02003d7e_a(0x235);
    Func_02003d7e_b(564);
}

/* Reads flag record 0x8a4; when set, runs one short setup on record 17.
 * When clear, runs a longer setup on record 17 plus scene phase/field
 * updates, then checks flag record 0x8a3 to pick a final call. Either path
 * ends with Func_02003e70(). */
void FieldScene_RunConditionalSceneSetup(void)
{
    extern u8 Data_03001ebc[];

    u32 i;
    s32 flag_8a4;
    s32 record;

    BattleRuntime_Reset_1();
    flag_8a4 = GameFlag_IsSet_1(0x8a4);
    if (flag_8a4 != 0) {
        Object_LinkPair_1(RECORD_17, 0, 40);
        SceneWork_SetStepValue_1(0x206f);
        Func_02002e32(RECORD_17);
        ObjectMotion_ArmCallback_1(RECORD_17, 0x3000, 20);
    } else {
        ObjectMotion_SetVariantCallback_1(RECORD_17, 2);
        SceneWork_SetStepValue_2(0x206d);
        BattleEvent_RunActionAndWait_1(RECORD_17, 0);
        /* Byte at +85 of the record returned by RuntimeBlock_GetOffset1e0Pointer_1(); written
         * with the (already known zero) flag value here. */
        *(u8 *)(RuntimeBlock_GetOffset1e0Pointer_2() + 85) = flag_8a4;
        Func_02003da8(1);
        ObjectMotion_SetSpeedLimitAndAcceleration_1(0x66666, 0xcccc);
        ObjectMotion_PlaceWithinCameraBounds_1(0x21c0000, -1, 0xd00000, 1);
        Object_CommitPositionThenWaitIfModeZero_1();
        SCENE_PHASE = 0x200;
        SCENE_FIELD_1C8 = 32;
        Func_02003f78();
        ObjectMotion_SetSpeedParameters_1();
        if (GameFlag_IsSet_2(0x8a3) != 0) {
            Func_02003f64(70);
        } else {
            Func_02003f6c(7);
        }
    }
    BattleRuntime_ScheduleShoulderButtonModeUpdate_1();
}

void FieldScene_BuildActorPresentationGroup(void)
{
    extern u8 Data_03001ebc[];

    s32 rec2;
    u8 *work;
    s32 shift;
    s32 phase;
    s32 field;

    Func_02003e9a_a();
    Call4(Func_02003f9c, -1, -1, -1, 0);
    Func_02003ff2_a(247);
    Func_02003f22(8, 2);
    Func_02003f2a(9, 2);
    Func_02003f32(10, 2);
    Func_02003f3a(11, 2);
    Func_02003f42(12, 2);
    Func_02003f4a(13, 2);
    Func_02003f52(14, 0);
    Func_02003f5a(15, 0);
    Func_02003f62(16, 0);
    Func_02003f6a(17, 0);
    Func_02003f72(18, 0);
    rec2 = Func_02003f28(21);
    Func_02003ec6_a(rec2, 0);
    rec2 = Func_02003f34(19);
    *(volatile s32 *)(rec2 + 24) = -0x10000;
    rec2 = Value1(Func_02003f3e, 20);
    *(volatile s32 *)(rec2 + 24) = -0x10000;
    Func_02003ec6_b(1);
    Func_02003f98_a(0, 0, 0);
    Func_02003ed6(1);
    work = *(u8 *volatile *)Data_03001ebc;
    phase = 0x1c0;
    *(volatile s32 *)(work + phase) = 0x200;
    field = 0x1c8;
    *(volatile s32 *)(work + field) = 32;
    Func_02004086();
    Func_0200409a();
    Func_02003f60(40);
    Func_02003ff8(16, 1);
    Func_02003f6e(20);
    Call3(Func_02003fa0, 16, 0xcccc, 0x6666);
    Func_02003fe4(16, 164, 0x388);
    Func_02003f8a(20);
    Func_02004002(16, 9);
    Func_02003f98_b(40);
    Func_02004010(16, 10);
    Func_02003fa6(60);
    Func_0200401e(16, 1);
    Func_02003fb4(20);
    Call3(Func_02004020, 16, 164, 0x398);
    Call3(Func_0200402c_a, 16, 185, 0x398);
    Call3(Func_020040a8, 16, 0xc000, 20);
    Call3(Func_02004044, 16, 185, 0x394);
    Func_0200405c(16, 11);
    Func_02003ff2_b(40);
    Func_0200408a(16, 1);
    Func_02004000(60);
    Func_02004098(16, 3);
    Func_0200400e(40);
    Func_02004046(16, 0x200af88);
    Func_0200401c(80);
    Call2(Func_02004106, 16, 0x102);
    Func_0200402c_b(60);
    Call3(Func_02004108, 14, 0xd000, 0);
    Call3(Func_02004114_a, 15, 0x5000, 0);
    Func_0200411e(17, 0, 0);
    Call3(Func_0200412a, 18, 0x8000, 20);
    Call2(Func_02004144, 14, 0x102);
    Call2(Func_0200414e, 15, 0x102);
    Call2(Func_02004158, 17, 0x102);
    Call2(Func_02004162, 18, 0x102);
    Func_02004088(60);
    Call3(Func_02004164, 14, 0x3000, 0);
    Call3(Func_02004170, 15, 0x3000, 0);
    Call3(Func_02004180, 17, 0x3000, 0);
    shift = 0x3000;
    Value2(Func_02003128, 18, shift);
    Func_020040f6(16);
    rec2 = Func_020040e4(16);
    {
        s32 shown = 0xd000;
        *(volatile u16 *)(rec2 + 6) = shown;
    }
    *(volatile s32 *)(rec2 + 24) = 0x10000;
    *(volatile s32 *)(rec2 + 28) = 0x10000;
    Func_020040d8(20);
    Func_02004150(16, 0);
    Func_020040e6(40);
    Func_0200415e(19, 5);
    Func_02004166(20, 5);
    Func_020040fc(60);
    Func_020041d6(16, shift, 20);
    Func_0200417e(16, 8);
    Func_02004114_b(20);
    Func_0200418c(14, 4);
    Func_02004194(15, 4);
    Func_0200419c(17, 4);
    Func_020041b4(18, 4);
    Func_0200413a(40);
    Func_020041c2(16, 4);
    Func_02004148(10);
    Call3(Func_0200417c, 16, 0x20000, 0x10000);
    Call3(Func_020041c0_a, 16, 162, 0x394);
    Func_020041ca(16, 162, 0x37a);
    Func_020041e2(19, 1);
    Func_020041ea(20, 1);
    Call3(Func_020041e4, 16, 184, 0x35f);
    Call3(Func_020041f0, 16, 184, 0x31c);
    Func_02004202(16, 0, 0);
    work = *(u8 *volatile *)Data_03001ebc;
    *(volatile s32 *)(work + phase) = 0x201;
    *(volatile s32 *)(work + field) = 16;
    Func_020042e6();
    Func_020042f2();
    work = *(u8 *volatile *)Data_03001ebc;
    *(volatile s32 *)(work + phase) = 0x100;
    Func_020042d4(69);
    Func_020041d8();
}

void SceneState_SetRuntimeWord448To513(void)
{
    extern u8 Data_02000240[];
    extern u8 *Data_03001ebc;

    Func_02004204();
    *(s32 *)((u8 *)Data_03001ebc + 448) = 513;

    Func_0200433e();
    Func_02004352_a();

    Func_02004218(20);
    Func_02003292(17, 160 << 7);
    Func_020042d0(0x206e);

    if (Func_020041fe(0x8a4) != 0) {
        *(u16 *)((u8 *)Data_03001ebc + 472) =
            (u16)(*(u16 *)((u8 *)Data_03001ebc + 472) + 1);
    }

    Func_020032a2(17);
    Func_020032c0(17, 192 << 6);
    Func_0200422e(0x8a3);

    Func_0200426a();
}

/* Sets up actors 8-20 (position, pose, or movement/sprite flags), advances
 * two actor records' +24 fields, advances the shared scene phase, then runs
 * a long chain of per-actor moves, pose changes, and waits. */
void FieldScene_RunMiddleSequence(void)
{
    extern u8 Data_03001ebc[];

    u32 counter;
    u8 *rec;
    s32 rec2;
    u8 *work;

    BattleRuntime_Reset_1_02001688();
    ObjectMotion_PlaceWithinCameraBounds_1_02001688(-1, -1, -1, 0);
    Audio_PlayCue_1(247);
    Object_SetModeById_1(8, 2);
    Call2((void (*)())Func_02004310_b, 9, 2);
    Object_SetModeById_2(10, 2);
    Object_SetModeById_3(11, 2);
    Object_SetModeById_4(12, 2);
    Object_SetModeById_5(13, 2);
    Object_SetModeById_6(14, 0);
    Object_SetModeById_7(15, 0);
    ObjectMotion_SetHorizontalPositionWithTerrain_1(16, 0, 0);
    Object_SetModeById_8(17, 0);
    Object_SetModeById_9(18, 0);
    rec2 = Scene_GetRecord_1(21);
    Func_020042ae_a(rec2, 0);
    rec2 = Scene_GetRecord_2(19);
    *(volatile s32 *)(rec2 + 24) = -0x10000;
    rec2 = Scene_GetRecord_3(20);
    *(volatile s32 *)(rec2 + 24) = -0x10000;
    Func_020042ae_b(1);
    ObjectMotion_SetHorizontalPositionWithTerrain_2(0, 0, 0);
    Func_020042be(1);
    /* Advance the scene phase/status word at +0x1c0 and its companion word
     * at +0x1c8 of the shared scene work record. */
    work = *(u8 *volatile *)Data_03001ebc;
    *(volatile s32 *)((work + 0x1c0)) = 0x200;
    *(volatile s32 *)((work + 0x1c8)) = 32;
    BattleRuntime_WaitIfModeZero_1();
    ObjectMotion_SetSpeedParameters_1_02001688();
    BattleRuntime_WaitIfModeZero_2(40);
    ObjectMotion_SetVariantCallbackAndInvokeObject_1(17, 1);
    BattleRuntime_WaitIfModeZero_3(20);
    ObjectMotion_SetSpeedParameters_2(17, 0xcccc, 0x6666);
    ObjectMotion_SetPositionAndReset_1(17, 164, 0x388);
    BattleRuntime_WaitIfModeZero_4(20);
    Object_SetModeById_10(17, 9);
    BattleRuntime_WaitIfModeZero_5(40);
    Object_SetModeById_11(17, 10);
    BattleRuntime_WaitIfModeZero_6(60);
    Object_SetModeById_12(17, 1);
    BattleRuntime_WaitIfModeZero_7(20);
    ObjectMotion_SetPositionAndReset_2(17, 164, 0x398);
    ObjectMotion_SetPositionAndReset_3(17, 185, 0x398);
    ObjectMotion_ArmCallback_1_02001688(17, 0xc000, 20);
    ObjectMotion_SetPositionAndReset_4(17, 185, 0x394);
    Object_SetModeById_13(17, 11);
    BattleRuntime_WaitIfModeZero_8(40);
    ObjectMotion_SetVariantCallbackAndInvokeObject_2(17, 1);
    BattleRuntime_WaitIfModeZero_9(60);
    ObjectMotion_SetVariantCallbackAndInvokeObject_3(17, 3);
    BattleRuntime_WaitIfModeZero_10(40);
    ObjectMotion_EnableActionAndSetCallback_1(17, 0x200af88);
    BattleRuntime_WaitIfModeZero_11(80);
    BattleRuntime_WaitIfModeZero_12(17, 0x102);
    BattleRuntime_WaitIfModeZero_13(60);
    ObjectMotion_ArmCallback_2(14, 0xd000, 0);
    ObjectMotion_ArmCallback_3(15, 0x5000, 0);
    ObjectMotion_ArmCallback_4(18, 0x8000, 20);
    BattleRuntime_WaitIfModeZero_14(14, 0x102);
    BattleRuntime_WaitIfModeZero_15(15, 0x102);
    BattleRuntime_WaitIfModeZero_16(17, 0x102);
    BattleRuntime_WaitIfModeZero_17(18, 0x102);
    BattleRuntime_WaitIfModeZero_18(60);
    ObjectMotion_ArmCallback_5(14, 0x3000, 0);
    ObjectMotion_ArmCallback_6(15, 0x3000, 0);
    Value2(Func_020034f2, 18, 0x3000);
    BattleRuntime_WaitIfModeZero_19(17, 0x101);
    /* Clear the flag byte at +85, then step the +12 field back and forth
     * 20 times, waiting between each step. */
    rec = Scene_GetRecord_4(21);
    rec[85] = 0;
    for (counter = 0; counter < 20; counter++) {
        *(volatile s32 *)(rec + 12) += 0x9999;
        Func_0200444a(4);
        *(volatile s32 *)(rec + 12) += -0x4ccc;
        Func_0200445a(4);
    }
    Object_SetModeById_14(19, 6);
    Object_SetModeById_15(20, 6);
    BattleRuntime_WaitIfModeZero_20(60);
    BattleRuntime_WaitIfModeZero_21(17, 0x100);
    ObjectMotion_EnableActionAndResetMotion_1(17);
    Object_SetModeById_16(17, 1);
    rec2 = Scene_GetRecord_5(17);
    {
        /* Set the u16 field at +6. */
        s32 shown = 0xd000;

        *(volatile u16 *)(rec2 + 6) = shown;
    }
    rec[85] = 3;
    *(volatile s32 *)(rec + 24) = 0x10000;
    *(volatile s32 *)(rec + 28) = 0x10000;
    BattleRuntime_WaitIfModeZero_22(10);
    Audio_PlayCue_2(107);
    Func_020044da(0x10000, 0x10000, 0x10000);
    BattleRuntime_WaitIfModeZero_23(10);
    Audio_PlayCue_3(0x121);
    Call3(Func_020044f4, -1, -1, 0xe666);
    Func_02004500();
    ObjectMotion_SetSpeedParameters_3(17, 0x19999, 0xcccc);
    ObjectMotion_SetPositionAndReset_5(17, 208, 0x3a0);
    Audio_PlayCue_4(92);
    ObjectMotion_ArmCallback_7(17, 0x3000, 20);
    Object_SetModeById_17(17, 9);
    BattleRuntime_WaitIfModeZero_24(20);
    Object_SetModeById_18(17, 10);
    BattleRuntime_WaitIfModeZero_25(40);
    Object_SetModeById_19(17, 9);
    BattleRuntime_WaitIfModeZero_26(20);
    Object_SetModeById_20(17, 10);
    BattleRuntime_WaitIfModeZero_27(80);
    /* Advance the scene phase/status word at +0x1c0 and its companion word
     * at +0x1c8 of the shared scene work record. */
    work = *(u8 *volatile *)Data_03001ebc;
    *(volatile s32 *)((work + 0x1c0)) = 0x201;
    *(volatile s32 *)((work + 0x1c8)) = 16;
    Func_020046ea();
    ObjectMotion_SetSpeedParameters_4();
    GameFlag_Set_1(0x8a4);
    Func_020046d2(69);
    BattleRuntime_ScheduleShoulderButtonModeUpdate_1_02001688();
}

s32 SceneState_DispatchByStatus448(void)
{
    extern u8 Data_02000240[];
    extern u8 *Data_03001ebc;

    s32 off = 448;
    s16 status = *(s16 *)(Data_02000240 + off);

    if (status == (s32)&Value_0000008c) {
        Func_02003476();
    } else if (status == (s32)&Value_0000008e) {
        Func_020036a2();
    }
    return 0;
}

/*
 * The message/event dispatcher for resource_3b9: read the selector from the
 * shared table, subtract 5, and dispatch through a 66-entry jump table. Only
 * twelve selectors act; the rest fall through. The 508-byte owner includes the
 * alignment halfword and the five-word literal pool after the return.
 */

/* A Func_ name here is a loader-relocated call word, not a runtime address.
 * Func_020047ec and Func_020047ec_a share an encoded word but reach different
 * veneers, so the two declarations must stay distinct; the suffix marks the
 * second call word, not a second entry point. */

/*
 * The case order is load-bearing and it is not the selector order: the arms
 * are laid out in the order the reference places their bodies, and the jump
 * table stores their absolute addresses. Each callee name is keyed to the
 * address of the instruction that calls it, so an arm moved out of place
 * emits the wrong call word.
 */
void FieldScene_DispatchBySelector(void)
{
    extern u8 Data_02000240[];

    s32 no;

    Func_020045de(1);
    {
        s32 off = 450;

        no = *(s16 *)(Data_02000240 + off);
    }
    switch (no) {
    case 5:
        Func_020047da(8, 2);
        Func_020047e2(9, 2);
        break;
    case 69:
        Func_020047ec(8, 2);
        Func_020047f4(9, 2);
        if (Func_0200475a(0x109) != 0)
            break;
        Func_020031b2();
        break;
    case 7:
        Func_02002e44();
        break;
    case 70:
        Func_0200323a();
        break;
    case 64:
        Func_0200388c();
        Func_020047a0();
        break;
    case 65:
        Func_02003fa2();
        break;
    case 66:
        Func_020041b8();
        break;
    case 12:
        Func_02004796(324);
        Func_020046f2();
        if (Func_02004798(0x109) != 0)
            break;
        Func_020040bc();
        break;
    case 21:
        Func_020047c0(1);
        Func_020047c6(2);
        Func_020047cc(3);
        Func_020047c2(0x90e);
        Func_0200426a_a();
        break;
    case 67:
        Func_02004428_a();
        break;
    case 68:
        Func_02004512_a();
        break;
    case 31:
        Func_020047ea(1);
        Func_020047f0(2);
        Func_020047f6(3);
        Func_020047ec_a(0x90f);
        Func_02004590();
        break;
    default:
        break;
    }
}

void FieldScene_CallPairWith10(s32 no)
{
    Func_020048f2(no, 0);
    Func_02004840(10);
}

void SceneState_ForwardMaskedHalfwordWith10(s32 arg0, s32 arg1)
{
    extern u8 Data_02000240[];
    extern u8 *Data_03001ebc;

    Func_02004922(arg0, (u16)arg1, 10);
}

/* resource_3b9 owner at 0x02001c6c, 100 bytes. Two alternative layouts. */
void SceneState_ApplyRectsByFlags2384And962(void)
{
    s32 pair;
    s32 a, b;

    if (Func_02004832(2384) != 0) {
        pair = 2;
        Func_02004820(64, 0, 48, 5, pair, pair);
        a = 16;
        b = 8;
        Func_0200483c(14, 8, 2, 1, a, b);
    } else {
        Func_02004936(16, 2);
        if (Func_0200486c(0x962) != 0) {
            a = 14;
            b = 11;
            Func_02004864(30, 22, 1, 2, a, b);
        }
    }
}

void FieldScene_BuildActorPresentationSequence(void)
{
    s32 flag;

    BattleRuntime_Reset();
    ObjectMotion_SetHorizontalPositionWithTerrain(1, 0x3180000, 0x880000);
    ObjectMotion_SetHorizontalPositionWithTerrain(2, 0x3380000, 0x880000);
    ObjectMotion_SetHorizontalPositionWithTerrain(3, 0x3280000, 0x980000);
    Func_0200489c(1);
    ObjectMotion_PlaceWithinCameraBounds(-1, -1, -1, 0);
    Data_0200ad0c(0, 0);
    Func_02004ac0(0, 0);
    Func_02004a4c(1);
    Func_0200489c(1);
    SCENE_PHASE_02001cd4 = 515;
    SCENE_FIELD_1C8_02001cd4 = 1;
    Func_02004a76();
    Func_02004a8a();
    Data_0200ad0c(0, 0);
    Func_02004ac0(0x10002, 0);
    Func_02004a4c(40);
    BattleRuntime_WaitIfModeZero(80);
    ObjectMotion_SetVariantCallbackAndInvokeObject(8, 1);
    BattleRuntime_WaitIfModeZero(20);
    ObjectMotion_SetVariantCallbackAndInvokeObject(2, 2);
    BattleRuntime_WaitIfModeZero(40);
    Func_02003a00(8, 0x3000);
    ObjectMotion_SetVariantCallbackAndInvokeObject(8, 2);
    BattleRuntime_WaitIfModeZero(20);
    Func_02004ac0(0x10000, 0);
    Func_02004a4c(40);
    BattleRuntime_WaitIfModeZero(80);
    BattleEffect_SpawnLinkedResourceObject(2, 0x100, 0);
    ObjectMotion_SetVariantCallbackAndInvokeObject(2, 1);
    BattleRuntime_WaitIfModeZero(20);
    SceneWork_SetStepValue(0x20f8);
    Func_02003a36(2);
    ObjectMotion_CallThenWaitForAnimationChange(8, 3);
    Func_02003a36(8);
    BattleEffect_SpawnLinkedResourceObject(3, 0x102, 40);
    Func_02003a36(3);
    ObjectMotion_ArmCallback(8, 0x8000, 0);
    ObjectMotion_ArmCallback(9, 0, 40);
    ObjectMotion_ArmCallback(8, 0x5000, 0);
    Func_02003a00(9, 0x3000);
    BattleEffect_SpawnLinkedResourceObject(9, 0x105, 20);
    Func_02003a36(9);
    BattleEffect_SpawnLinkedResourceObject(1, 0x103, 20);
    ObjectMotion_SetVariantCallback(1, 2);
    Func_02003a36(1);
    BattleEffect_SpawnLinkedResourceObject(10, 0x102, 40);
    Func_02003a36(10);
    ObjectMotion_CallThenWaitForAnimationChange(11, 3);
    Func_02003a36(11);
    Func_02003a00(2, 0xa000);
    ObjectMotion_CallThenWaitForAnimationChange(2, 4);
    Func_02003a36(2);
    ObjectMotion_SetVariantCallbackAndInvokeObject(3, 1);
    Func_02003a36(3);
    BattleEffect_SpawnLinkedResourceObject(1, 0x103, 20);
    Func_02003a00(1, 0);
    ObjectMotion_SetVariantCallback(1, 2);
    Func_02003a36(1);
    Func_02003a00(0, 0x6000);
    BattleEffect_SpawnLinkedResourceObject(0, 0x102, 60);
    ObjectMotion_SetVariantCallbackAndInvokeObject(2, 1);
    Func_02003a00(2, 0x8000);
    Func_02003a36(2);
    Object_SetModeById(3, 4);
    BattleRuntime_WaitIfModeZero(20);
    Func_02003a36(3);
    ObjectMotion_SetVariantCallbackAndInvokeObject(1, 1);
    Func_02003a00(1, 0x2000);
    Func_02003a36(1);
    BattleEffect_SpawnLinkedResourceObject(2, 0x105, 60);
    ObjectMotion_ArmCallback(2, 0x2000, 0);
    ObjectMotion_ArmCallback(1, 0, 0);
    ObjectMotion_ArmCallback(3, 0xe000, 20);
    BattleEffect_SpawnLinkedResourceObject(0, 0x102, 0);
    BattleEffect_SpawnLinkedResourceObject(1, 0x102, 0);
    BattleEffect_SpawnLinkedResourceObject(3, 0x102, 80);
    BattleEffect_SpawnLinkedResourceObject(2, 0x106, 0);
    ObjectMotion_SetVariantCallbackAndInvokeObject(2, 1);
    Func_02003a00(2, 0xc000);
    Func_02003a36(2);
    ObjectMotion_ArmCallback(0, 0xc000, 0);
    ObjectMotion_ArmCallback(1, 0xc000, 0);
    ObjectMotion_ArmCallback(3, 0xc000, 20);
    Func_02003a00(8, 0x3000);
    ObjectMotion_CallThenWaitForAnimationChange(8, 3);
    Func_02003a36(8);
    BattleEffect_SpawnLinkedResourceObject(0, 0x100, 0);
    BattleEffect_SpawnLinkedResourceObject(1, 0x100, 0);
    BattleEffect_SpawnLinkedResourceObject(3, 0x100, 40);
    ObjectMotion_SetVariantCallbackAndInvokeObject(9, 1);
    Func_02003a36(9);
    ObjectMotion_ArmCallback(0, 0x6000, 0);
    ObjectMotion_ArmCallback(1, 0xe000, 0);
    ObjectMotion_ArmCallback(2, 0x6000, 0);
    ObjectMotion_ArmCallback(3, 0xe000, 40);
    ObjectMotion_SetVariantCallbackAndInvokeObject(10, 1);
    Func_02003a36(10);
    ObjectMotion_ArmCallback(0, 0xc000, 0);
    ObjectMotion_ArmCallback(1, 0xc000, 0);
    ObjectMotion_ArmCallback(2, 0xc000, 0);
    Func_02003a00(3, 0xc000);
    ObjectMotion_CallThenWaitForAnimationChange(11, 3);
    Func_02003a36(11);
    ObjectMotion_ArmCallback(0, 0x6000, 0);
    ObjectMotion_ArmCallback(1, 0xe000, 0);
    ObjectMotion_ArmCallback(2, 0x6000, 0);
    ObjectMotion_ArmCallback(3, 0xe000, 20);
    BattleEffect_SpawnLinkedResourceObject(1, 0x105, 0);
    BattleEffect_SpawnLinkedResourceObject(2, 0x105, 0);
    BattleEffect_SpawnLinkedResourceObject(3, 0x105, 80);
    BattleEffect_SpawnLinkedResourceObject(2, 0x106, 0);
    ObjectMotion_SetVariantCallbackAndInvokeObject(2, 1);
    Func_02003a00(2, 0xe000);
    Func_02003a36(2);
    ObjectMotion_ArmCallback(0, 0, 0);
    ObjectMotion_ArmCallback(1, 0, 20);
    BattleEffect_SpawnLinkedResourceObject(11, 0x101, 60);
    Value2(Func_02004da2, 2, 0);
    ObjectMotion_ArmCallback(0, 0x2000, 0);
    ObjectMotion_ArmCallback(1, 0xe000, 0);
    ObjectMotion_ArmCallback(2, 0xa000, 0);
    ObjectMotion_ArmCallback(3, 0xc000, 0);
    flag = 0;
    if (UiWork_WaitThenFinalizeCapacity(0, 0) == 1) {
        Func_02003a36(2);
        flag = 1;
    } else {
        SCENE_STEP++;
        ObjectMotion_CallThenWaitForAnimationChange(2, 3);
        Func_02003a00(2, 0xc000);
        Func_02003a36(2);
    }
    if (flag != 0) {
        SCENE_STEP++;
    }
    ObjectMotion_ArmCallback(8, 0x8000, 0);
    ObjectMotion_ArmCallback(9, 0, 0);
    ObjectMotion_ArmCallback(10, 0x3000, 0);
    ObjectMotion_ArmCallback(11, 0xb000, 20);
    BattleEffect_SpawnLinkedResourceObject(8, 0x105, 0);
    BattleEffect_SpawnLinkedResourceObject(9, 0x105, 0);
    BattleEffect_SpawnLinkedResourceObject(10, 0x105, 0);
    BattleEffect_SpawnLinkedResourceObject(11, 0x105, 60);
    ObjectMotion_SetVariantCallbackAndInvokeObject(8, 1);
    Func_02003a00(8, 0x3000);
    Func_02003a36(8);
    ObjectMotion_ArmCallback(0, 0xc000, 0);
    ObjectMotion_ArmCallback(1, 0xc000, 0);
    ObjectMotion_ArmCallback(2, 0xc000, 0);
    ObjectMotion_ArmCallback(3, 0xc000, 20);
    ObjectMotion_SetVariantCallbackAndInvokeObject(9, 1);
    Func_02003a00(9, 0x3000);
    Func_02003a36(9);
    ObjectMotion_SetVariantCallbackAndInvokeObject(10, 1);
    Func_02003a00(10, 0x5000);
    Func_02003a36(10);
    Func_02003a00(11, 0x8000);
    ObjectMotion_CallThenWaitForAnimationChange(11, 3);
    Func_02003a36(11);
    ObjectMotion_ArmCallback(1, 0xe000, 0);
    ObjectMotion_ArmCallback(2, 0xa000, 20);
    Object_SetModeById(1, 3);
    Object_SetModeById(2, 3);
    ObjectMotion_CallThenWaitForAnimationChange(3, 3);
    ObjectMotion_SetSpeedParameters(1, 0x10000, 0x8000);
    ObjectMotion_SetSpeedParameters(2, 0x10000, 0x8000);
    ObjectMotion_SetSpeedParameters(3, 0x10000, 0x8000);
    ObjectMotion_EnableActionAndSetCallback_1_02001cd4(1, (s32)Data_0200adac);
    ObjectMotion_EnableActionAndSetCallback_2(2, (s32)Data_0200adac);
    ObjectMotion_MarkActiveAndSetActionCallback(3, (s32)Data_0200adac);
    BattleRuntime_WaitIfModeZero(20);
    Func_02003a00(0, 0);
    ObjectMotion_CallThenWaitForAnimationChange(0, 3);
    ObjectMotion_CallThenWaitForAnimationChange(11, 3);
    ObjectMotion_SetSpeedParameters(11, 0x10000, 0x8000);
    ObjectMotion_SetSpeedParameters(0, 0x10000, 0x8000);
    Object_SetModeById(11, 2);
    ObjectMotion_SetPositionAndCommit(11, 830, 152);
    ObjectMotion_SetPositionAndCommit(11, 808, 164);
    ObjectMotion_ResetAndSetPosition(11, 808, 312);
    BattleRuntime_WaitIfModeZero(20);
    ObjectMotion_SetSpeedLimitAndAcceleration(0x6666, 0xccc);
    ObjectMotion_PlaceWithinCameraBounds(0x3280000, -1, 0x1380000, 1);
    ObjectMotion_SetPositionAndReset(0, 808, 164);
    ObjectMotion_ResetAndSetPositionInMode2(0, 808, 312);
    BattleRuntime_WaitIfModeZero(60);
    SCENE_PHASE_02001cd4 = 256;
    SCENE_FIELD_1C8_02001cd4 = 40;
    Func_020050d0();
    Func_02004a8a();
    Func_020050b2(64);
}

void FieldScene_RunScene3b9_020023e0(void)
{
    extern u8 Data_03001ebc[];

    u32 i;
    s32 record;

    if (Value1(Func_02004fa2, 5) != 0) {
        Call1(Func_02004fb4, 0x16d);
        Func_02004fd2(5);
        Func_02004fd0(3);
    }
    Func_02004ff4();
    Call3(Func_02005062, 11, 0x2c80000, 0x24c0000);
    Func_02004fa0(1);
    Func_020050f0(11, 1);
    Call3(Func_0200503a, 11, 0x19999, 0xcccc);
    Call3(Func_02005044, 0, 0x19999, 0xcccc);
    record = Func_02005042(11);
    {
        s32 shown = 0;

        *(volatile u16 *)(record + 6) = shown;
    }
    Func_0200515a();
    Func_020050a2(0, 2);
    Func_020050aa(11, 2);
    Call3(Func_02005090, 0, 0x30c, 0x24c);
    Call3(Func_020050a6, 11, 0x32c, 0x24c);
    Call3(Func_020050ac, 0, 0x370, 0x24c);
    Call3(Func_020050c2, 11, 0x390, 0x24c);
    Call3(Func_020050c8, 0, 0x3d4, 0x24c);
    Call3(Func_020050d6, 11, 0x3f4, 0x24c);
    Func_020051ca();
    Func_020051d6();
    if (Value1(Func_0200506c, 0x90f) != 0) {
        Func_020051b6(31);
    } else {
        Func_020051be(65);
    }
}

void FieldScene_RunScene3b9_020024d8(void)
{
    extern u8 Data_03001ebc[];

    u32 i;
    s32 rec7;
    s32 record;

    rec7 = Value1(Func_020050ea, 13);
    Func_020050d8();
    Func_02005204_a();
    Func_02005218();
    Func_020050de(40);
    Func_02005176_a(8, 2);
    Func_02005124(13);
    Func_02005092(1);
    Call3(Func_020051ce, 0, 0xe000, 0);
    Func_02005176_b(13, 1);
    Call3(Func_020051e2, 12, 0xd000, 0);
    Func_020051ec_a(13, 0, 0);
    Call3(Func_020051f8, 14, 0x8000, 0);
    Call3(Func_02005204_b, 15, 0xd000, 0);
    Call3(Func_02005210, 16, 0x8000, 0);
    Call3(Func_0200521c, 17, 0xb000, 0);
    Call3(Func_02005228, 18, 0xb000, 0);
    Call1(Func_02005206, 0x2112);
    Func_020041c0(8);
    Func_020051ec_b(0, 3);
    {
        u16 *target = (u16 *)(rec7 + 100);
        s32 shown = 0x2d0;

        *target = shown;
    }
    {
        u16 *target = (u16 *)(rec7 + 102);
        s32 shown = 112;

        *target = shown;
    }
    Func_020051b4(13, 2);
    Call3(Func_02005260, 12, 0x3000, 0);
    Call3(Func_0200526c, 14, 0xb000, 0);
    Call3(Func_02005278, 15, 0x5000, 0);
    Func_02005282(16, 0, 0);
    Call3(Func_0200528e, 17, 0x5000, 0);
    Call3(Func_0200529a, 18, 0x5000, 0);
    Func_020051de();
}

void FieldScene_RunScene3b9_020025f0(void)
{
    extern u8 Data_03001ebc[];

    u32 i;
    s32 record;

    Func_020051e8();
    Call3(Func_02005212, 0, 0x19999, 0xcccc);
    Func_0200531e();
    Func_02005266(0, 2);
    Call3(Func_02005254, 0, 0x30c, 0x1ac);
    Call3(Func_02005262, 0, 0x370, 0x1ac);
    Call3(Func_02005268, 0, 0x3d4, 0x1ac);
    Func_0200535c();
    Func_02005368();
    if (Value1_020025f0(Func_020051fe, 0x90f) != 0) {
        Func_02005348(32);
    } else {
        Func_02005350(12);
    }
}

void FieldScene_RunScene3b9_02002668(void)
{
    extern u8 Data_03001ebc[];

    u32 i;
    s32 record;
    s32 base5_200adac;

    Func_02005260_a();
    Call3(Func_020052ce, 1, 0x3180000, 0x880000);
    Call3(Func_020052dc, 2, 0x3380000, 0x880000);
    Call3(Func_020052ea, 3, 0x3280000, 0x980000);
    Func_020053b6();
    Func_020053ca();
    Func_02005290(40);
    Func_02005328(8, 1);
    Func_02005310(8, 3);
    Call1(Func_0200534e, 0x2134);
    Func_02004308_a(8);
    Func_02005344(9, 1);
    Func_02004316(9);
    Func_02005352(10, 1);
    Func_02004324(10);
    Func_02005360(11, 1);
    Func_02005348_a(11, 3);
    Func_0200433a(11);
    Call3(Func_020053ba, 1, 0xe000, 0);
    Call3(Func_020053c6, 2, 0xa000, 20);
    Call3(Func_0200532c, 1, 0x10000, 0x8000);
    Call3(Func_0200533a, 2, 0x10000, 0x8000);
    Call3(Func_02005348_b, 3, 0x10000, 0x8000);
    base5_200adac = (s32)Data_0200adac;
    Func_0200535a(1, base5_200adac);
    Value2(Func_02005362, 2, base5_200adac);
    Func_0200537a(3, base5_200adac);
    Func_02005340(20);
    Func_020043b8(0, 0);
    Func_020053d0(0, 3);
    Func_020053d8(11, 3);
    Call3(Func_0200538e, 11, 0x10000, 0x8000);
    Call3(Func_0200539c, 0, 0x10000, 0x8000);
    Func_020053ec(11, 2);
    Call3(Func_020053d6, 11, 0x33e, 152);
    Call3(Func_020053e2, 11, 0x328, 164);
    Call3(Func_020053e8, 11, 0x328, 0x138);
    Func_020053a6(20);
    Call2(Func_0200549e, 0x6666, 0xccc);
    Call4(Func_020054b8, 0x3280000, -1, 0x1380000, 1);
    Call3(Func_0200542c, 0, 0x328, 164);
    Call3(Func_02005432, 0, 0x328, 0x138);
    Func_020053e0(60);
    Func_0200551c();
    Func_02005528();
    Func_020054fe(67);
}

void FieldScene_RunScene3b9_02002820(void)
{
    extern u8 Data_03001ebc[];

    u32 i;
    s32 record;

    if (Value1(Func_020053e2_a, 5) != 0) {
        Call1(Func_020053f4, 0x16d);
        Func_02005412(5);
        Func_02005410(3);
    }
    Func_02005434();
    Call3(Func_020054a2, 11, 0x3640000, 0x24c0000);
    Func_020053e0_a(1);
    Func_02005530(11, 1);
    Call3(Func_0200547a, 11, 0x19999, 0xcccc);
    Call3(Func_02005484, 0, 0x19999, 0xcccc);
    record = Func_02005482(11);
    {
        s32 shown = 0x8000;

        *(volatile u16 *)(record + 6) = shown;
    }
    Func_0200559c();
    Func_020054e4(0, 2);
    Func_020054ec(11, 2);
    Call3(Func_020054d2, 0, 0x320, 0x24c);
    Call3(Func_020054e8, 11, 0x300, 0x24c);
    Call3(Func_020054ee, 0, 0x2bc, 0x24c);
    Call3(Func_02005504, 11, 0x29c, 0x24c);
    Call3(Func_0200550a, 0, 0x258, 0x24c);
    Call3(Func_02005518, 11, 0x238, 0x24c);
    Func_0200560c();
    Func_02005618();
    Func_020055ee(21);
}

void FieldScene_RunScene3b9_02002904(void)
{
    extern u8 Data_03001ebc[];

    u32 i;
    s32 record;

    Func_020054fc();
    Call3_02002904(Func_02005526, 0, 0x19999, 0xcccc);
    Func_02005632();
    Func_0200557a(0, 2);
    Call3_02002904(Func_02005568, 0, 0x320, 0x1ac);
    Call3_02002904(Func_02005576, 0, 0x2bc, 0x1ac);
    Call3_02002904(Func_0200557c, 0, 0x258, 0x1ac);
    Func_02005670();
    Func_0200567c();
    Func_02005652(22);
}

void FieldScene_RunScene3b9_02002964(void)
{
    extern u8 Data_03001ebc[];

    u32 i;
    s32 record;
    s32 base5_200adac;

    Func_0200555c();
    Call3(Func_020055ca, 1, 0x3180000, 0x880000);
    Call3(Func_020055d8, 2, 0x3380000, 0x880000);
    Call3(Func_020055e6, 3, 0x3280000, 0x980000);
    Func_020056b2();
    Func_020056c6();
    Func_0200558c(40);
    Func_02005624(8, 1);
    Func_0200560c_a(8, 3);
    Call1(Func_0200564a, SCENE_STEP_VALUE);
    Func_02004604(8);
    Func_02005640(9, 1);
    Func_02004612(9);
    Func_0200564e(10, 1);
    Func_02004620(10);
    Func_0200565c(11, 1);
    Func_02005644_a(11, 3);
    Func_02004636(11);
    Call3(Func_020056b6, 1, 0xe000, 0);
    Call3(Func_020056c2, 2, 0xa000, 20);
    Call3(Func_02005628, 1, 0x10000, 0x8000);
    Call3(Func_02005636, 2, 0x10000, 0x8000);
    Call3(Func_02005644_b, 3, 0x10000, 0x8000);
    base5_200adac = (s32)Data_0200adac;
    Func_02005656(1, base5_200adac);
    Value2(Func_0200565e, 2, base5_200adac);
    Func_02005676(3, base5_200adac);
    Func_0200563c(20);
    Func_020046b4(0, 0);
    Func_020056cc(0, 3);
    Func_020056d4(11, 3);
    Call3(Func_0200568a, 11, 0x10000, 0x8000);
    Call3(Func_02005698, 0, 0x10000, 0x8000);
    Func_020056e8(11, 2);
    Call3(Func_020056d2, 11, 0x33e, 152);
    Call3(Func_020056de, 11, 0x328, 164);
    Call3(Func_020056e4, 11, 0x328, 0x138);
    Func_020056a2(20);
    Call2(Func_0200579a, 0x6666, 0xccc);
    Call4(Func_020057b4, 0x3280000, -1, 0x1380000, 1);
    Call3(Func_02005728, 0, 0x328, 164);
    Call3(Func_0200572e, 0, 0x328, 0x138);
    Func_020056dc(60);
    Func_02005818();
    Func_02005824();
    Func_020057fa(64);
}

void ActorPresentation_SetActorsTwelveToEighteen(void)
{
    Func_020057b0(12, 3);
    Func_020057b8(13, 0);
    Func_020057c0_a(14, 4);
    Func_020057c8(15, 1);
    Func_020057d0(16, 5);
    Func_020057d8(17, 2);
    Func_020057e0(18, 6);
    Func_020057c0_b(13, 10);
    Func_020057c8_a(14, 20);
    Func_020057c8_b(15, 0);
    Func_020057d8_a(16, 40);
    Func_020057e0_a(17, 50);
    Func_020057e8(18, 60);
}
