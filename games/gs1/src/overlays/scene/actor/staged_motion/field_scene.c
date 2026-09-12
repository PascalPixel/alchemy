#include "types.h"

/* Resolved engine calls: each pseudo symbol is the per-site call word the
 * overlay image holds (a word can serve two sites with different targets),
 * and the macro names the engine function the site reaches through the
 * overlay veneer and the main-image veneer island, keeping the site's own
 * calling form. Names without a repository binding are provisional.
 */
#define Audio_PlayCue_1(a0) Call1(Func_020070ae, a0)
#define ObjectMotion_ResetAndSetPositionInMode2_1(a0, a1, a2) Call3(Func_02006f94, a0, a1, a2)
/* Resolved engine calls: each pseudo symbol is the per-site call word the
 * overlay image holds (a word can serve two sites with different targets),
 * and the macro names the engine function the site reaches through the
 * overlay veneer and the main-image veneer island, keeping the site's own
 * calling form. Names without a repository binding are provisional.
 */
#define Audio_PlayCue_1_02000f8c(args...) Func_020070de(args)
#define ObjectMotion_ResetAndSetPositionInMode2_1_02000f8c(a0, a1, a2) Call3(Func_02006fc4, a0, a1, a2)
/* Resolved engine calls: each pseudo symbol is the per-site call word the
 * overlay image holds (a word can serve two sites with different targets),
 * and the macro names the engine function the site reaches through the
 * overlay veneer and the main-image veneer island, keeping the site's own
 * calling form. Names without a repository binding are provisional.
 */
#define Audio_PlayCue_1_02000fbc(a0) Call1(Func_0200710e, a0)
#define ObjectMotion_ResetAndSetPositionInMode2_1_02000fbc(a0, a1, a2) Call3(Func_02006ff2, a0, a1, a2)
/* Resolved engine calls: each pseudo symbol is the per-site call word the
 * overlay image holds (a word can serve two sites with different targets),
 * and the macro names the engine function the site reaches through the
 * overlay veneer and the main-image veneer island, keeping the site's own
 * calling form. Names without a repository binding are provisional.
 */
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
/* Resolved engine calls: each pseudo symbol is the per-site call word the
 * overlay image holds (a word can serve two sites with different targets),
 * and the macro names the engine function the site reaches through the
 * overlay veneer and the main-image veneer island, keeping the site's own
 * calling form. Names without a repository binding are provisional.
 */
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
/* Resolved engine calls: each pseudo symbol is the per-site call word the
 * overlay image holds (a word can serve two sites with different targets),
 * and the macro names the engine function the site reaches through the
 * overlay veneer and the main-image veneer island, keeping the site's own
 * calling form. Names without a repository binding are provisional.
 */
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
/* Actor IDs the scene sets up: three actors share a common data pointer,
 * a fourth actor (11) is driven through its own sequence of position/speed
 * calls. */
#define ACTOR_A 12
#define ACTOR_B 13
#define ACTOR_C 14
#define ACTOR_D 11
/* Shared data used for actors A, B, C. */
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

/* Loader-relocated overlay calls: each symbol names the pre-relocation call
 * word the image holds. */
extern u8 Data_03001ebc[];
extern u8 Value_0200f570;
extern u8 Value_0200f55a;
/* Loader-relocated overlay calls: each symbol names the pre-relocation call
 * word the image holds. */
extern u8 Data_00000f4d[];
/* Loader-relocated overlay calls: each symbol names the pre-relocation call
 * word the image holds. */
extern u8 Data_00001c45[];
/* Loader-relocated overlay calls: each symbol names the pre-relocation call
 * word the image holds. */
extern u8 Data_00001c60[]; /* Buffer passed by address to Func_0200918e. */
/* Loader-relocated overlay calls: each symbol names the pre-relocation call
 * word the image holds. */
extern u8 Data_0200e65c[];
/* Loader-relocated overlay calls: each symbol names the pre-relocation call
 * word the image holds. */
extern u8 Data_00000ee8[];
extern u8 Data_0200e590[];
extern u8 Value_00000ee4;

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
void Func_02006c4c();
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
/* Loader-relocated overlay calls: each symbol names the pre-relocation call
 * word the image holds. */
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
static __inline__ void Call3_02000dc0(void (*f)(), s32 a0, s32 a1, s32 a2)
{
    f(a0, a1, a2);
}

/* Call sites spelled through these wrappers pass their constants straight
 * into the argument registers; a direct call precomputes a costly constant
 * into a pseudo that the compiler then shares with later uses in the block.
 * A value-returning call also sets r0 last of its arguments. */
static __inline__ void Call3_02000e54(void (*f)(), s32 a0, s32 a1, s32 a2)
{
    f(a0, a1, a2);
}

/* Call sites spelled through these wrappers pass their constants straight
 * into the argument registers; a direct call precomputes a costly constant
 * into a pseudo that the compiler then shares with later uses in the block.
 * A value-returning call also sets r0 last of its arguments. */
static __inline__ void Call3_02000e84(void (*f)(), s32 a0, s32 a1, s32 a2)
{
    f(a0, a1, a2);
}

/* Call sites spelled through these wrappers pass their constants straight
 * into the argument registers, matching the reference codegen. */
static __inline__ s32 Value2(s32 (*f)(), s32 a0, s32 a1)
{
    return f(a0, a1);
}

static __inline__ s32 Value3(s32 (*f)(), s32 a0, s32 a1, s32 a2)
{
    return f(a0, a1, a2);
}

static __inline__ void Call1_02000f5c(void (*f)(), s32 a0)
{
    f(a0);
}

static __inline__ void Call2(void (*f)(), s32 a0, s32 a1)
{
    f(a0, a1);
}

/* Call sites spelled through these wrappers pass their constants straight
 * into the argument registers; a direct call precomputes a costly constant
 * into a pseudo that the compiler then shares with later uses in the block.
 * A value-returning call also sets r0 last of its arguments. */
static __inline__ void Call3_02000f8c(void (*f)(), s32 a0, s32 a1, s32 a2)
{
    f(a0, a1, a2);
}

/* Runs four fixed steps in order: a single-argument call, a call that takes
 * the address of the byte at Value_0200f55a plus two constants, another
 * three-constant call, and a final single-argument call. */
static __inline__ s32 Value2_02000fbc(s32 (*f)(), s32 a0, s32 a1)
{
    return f(a0, a1);
}

static __inline__ void Call1_02000fbc(void (*f)(), s32 a0)
{
    f(a0);
}

/* Call sites spelled through these wrappers pass their constants straight
 * into the argument registers; a direct call precomputes a costly constant
 * into a pseudo that the compiler then shares with later uses in the block.
 * A value-returning call also sets r0 last of its arguments. */
static __inline__ s32 Value1_020011d8(s32 (*f)(), s32 a0)
{
    return f(a0);
}

static __inline__ s32 Value2_02002cb0(s32 (*f)(), s32 a0, s32 a1)
{
    return f(a0, a1);
}

static __inline__ void Call4(void (*f)(), s32 a0, s32 a1, s32 a2, s32 a3)
{
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
    f(a0);
}

static __inline__ s32 Value2_02002f14(s32 (*f)(), s32 a0, s32 a1)
{
    return f(a0, a1);
}

static __inline__ s32 Value2_020031b4(s32 (*f)(), s32 a0, s32 a1)
{
    return f(a0, a1);
}

/* Call sites spelled through these wrappers pass their constants straight
 * into the argument registers; a direct call precomputes a costly constant
 * into a pseudo that the compiler then shares with later uses in the block.
 * A value-returning call also sets r0 last of its arguments. */
static __inline__ s32 Value1_020034c8(s32 (*f)(), s32 a0)
{
    return f(a0);
}

static __inline__ s32 Value2_020034c8(s32 (*f)(), s32 a0, s32 a1)
{
    return f(a0, a1);
}

static __inline__ void Call6(void (*f)(), s32 a0, s32 a1, s32 a2, s32 a3, s32 a4, s32 a5)
{
    f(a0, a1, a2, a3, a4, a5);
}

static __inline__ void Call11(void (*f)(), s32 a0, s32 a1, s32 a2, s32 a3, s32 a4, s32 a5, s32 a6, s32 a7, s32 a8, s32 a9, s32 a10)
{
    f(a0, a1, a2, a3, a4, a5, a6, a7, a8, a9, a10);
}

void FieldScene_RunFlag807BranchSequence(void)
{
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

void FieldScene_RunScene373_02000cd0(void)
{
    u32 i;
    s32 record;

    Func_02006c90_a();
    Call3(Func_02006c38, 0x30000, 0x30000, 0x10000);
    Func_02006b76(10);
    Call3(Func_02006c4c, -1, -1, 0xe666);
    Call1(Func_02006d82, 0x1c9a);
    Func_02006da4(17, 0, 20);
    Func_02006d7e(17, 0, 20);
    Func_02006dae(17, 0);
    Func_02006ce2_a();
}

void FieldScene_RunScene373_02000dc0(void)
{
    u32 i;
    s32 record;

    Func_02006f12(188);
    Call3_02000dc0(Func_02006cfc, 0x200f544, 45, 11);
    Call3_02000dc0(Func_02006df8, 0, 0x101, 0x1a4);
    Func_02006ede(11);
}

void FieldScene_RunScene373_02000e54(void)
{
    u32 i;
    s32 record;

    Func_02006fa6(158);
    Call3_02000e54(Func_02006d90, 0x200f55a, 54, 32);
    Call3_02000e54(Func_02006e8c, 0, 0x196, 0x2d7);
    Func_02006f72(5);
}

void FieldScene_RunScene373_02000e84(void)
{
    u32 i;
    s32 record;

    Func_02006fd6(158);
    Call3_02000e84(Func_02006dc0, 0x200f570, 45, 39);
    Call3_02000e84(Func_02006ebc, 0, 0x106, 0x325);
    Func_02006fa2(6);
}

/* Runs four scene primitives in sequence: one single-argument call, one call
 * passing the address of Value_0200f570 plus two small constants, one call
 * with a byte-flag-sized first argument (0) and two larger constants, and a
 * final single-argument call. */
void FieldScene_RunPrimarySequence(void)
{
    Audio_PlayCue_1(158);
    Call3(Func_02006e98, (s32)&Value_0200f570, 49, 69); /* main:08009178 */
    ObjectMotion_ResetAndSetPositionInMode2_1(0, 326, 0x466); /* object_id 0, x 326, z 0x466 */
    Call1_02000f5c(Func_0200707a, 8); /* main:0808a248 */
}

/* Runs a short scripted step, then two 3-argument setup calls, then another
 * short scripted step; none of the callees' effects are visible here. */
void FieldScene_RunScene373SequenceA(void)
{
    Audio_PlayCue_1_02000f8c(158);
    Call3_02000f8c(Func_02006ec8, 0x200f586, 52, 76); /* main:08009178 */
    ObjectMotion_ResetAndSetPositionInMode2_1_02000f8c(0, 0x176, 0x4d6); /* object_id 0, x 0x176, z 0x4d6 */
    Func_020070aa(9); /* main:0808a248 */
}

void FieldScene_RunPrimarySequenceSecond(void)
{
    Audio_PlayCue_1_02000fbc(158);
    Call3(Func_02006ef8, (s32)&Value_0200f55a, 35, 74); /* main:08009178 */
    ObjectMotion_ResetAndSetPositionInMode2_1_02000fbc(0, 102, 0x4b6);
    Call1_02000fbc(Func_020070d8, 10); /* main:0808a248 */
}

void FieldScene_RunScene373SequenceC(void)
{
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

void FieldScene_RunScene373SequenceE(void)
{
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

void FieldScene_RunScene373_02001490(s32 a0, s32 a1)
{
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

/* Runs a scripted movement/pose sequence for actors 0, 1 and 8, reading two
 * lookup records along the way (one 32-bit-field record, one 16-bit-field
 * record) to copy their stored values onto actor 1. */
void FieldScene_RunSecondaryActorSequence(void)
{
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

void FieldScene_RunLargeStagingSequence(void)
{
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

void FieldScene_RunStep8C(void)
{
    Func_0200b6c0(0x8c, 0);
}

void FieldScene_RunStep9(void)
{
    Func_0200b5ba(9);
    Func_0200b272();
}

void FieldScene_RunStep17(void)
{
    Func_0200b5ca(0x17);
    Func_0200b282();
}

void FieldScene_RunScriptedStepEE4(void)
{
    Func_0200b9e4();
    Func_0200b9ac((s32)&Value_00000ee4, 1);
    Func_0200b9f8();
}

void FieldScene_RunScene373SequenceB(void)
{
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
