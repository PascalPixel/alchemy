#include "types.h"

#define BattleRuntime_Reset_1(args...) Func_020019ce(args)
#define ObjectGroup_ConfigureChildValue_1(args...) Func_02001a46(args)
#define Scene_GetRecord_1(args...) Func_020019fc(args)
#define Object_NotifyLastActiveOfEvent_1(a0) Call1(Func_020019fe, a0)
#define ObjectMotion_EnableActionAndSetCallback_1(args...) Func_02001a5a(args)
#define BattleRuntime_WaitIfModeZero_1(args...) Func_02001b14(args)
#define ObjectMotion_SetSpeedParameters_1(args...) Func_02001b28(args)
#define BattleRuntime_WaitIfModeZero_2(args...) Func_02001a3e(args)
#define Scene_GetRecord_2(args...) Func_02001a6c(args)
#define ObjectMotion_EnableActionAndResetMotion_1(args...) Func_02001a8c(args)
#define BattleRuntime_WaitIfModeZero_3(args...) Func_02001a68(args)
#define ObjectMotion_SetSpeedParameters_2(a0, a1, a2) Call3(Func_02001aa6, a0, a1, a2)
#define Object_SetPosition_1(a0, a1, a2, a3) Call4(Func_02001a5e, a0, a1, a2, a3)
#define Object_CommitPosition_1(args...) Func_02001a6c_a(args)
#define Object_SetPosition_2(a0, a1, a2, a3) Call4(Func_02001a74, a0, a1, a2, a3)
#define Object_CommitPosition_2(args...) Func_02001a82(args)
#define Object_SetPosition_3(a0, a1, a2, a3) Call4(Func_02001a8a, a0, a1, a2, a3)
#define Object_CommitPosition_3(args...) Func_02001a98(args)
#define Object_SetPosition_4(a0, a1, a2, a3) Call4(Func_02001aa0, a0, a1, a2, a3)
#define ObjectMotion_EnableActionAndResetMotion_2(args...) Func_02001b0e(args)
#define ObjectMotion_ArmCallback_1(a0, a1, a2) Call3(Func_02001b70, a0, a1, a2)
#define BattleEffect_SpawnLinkedResourceObject_1(a0, a1, a2) Call3(Func_02001b8a, a0, a1, a2)
#define ObjectMotion_SetSpeedParameters_3(a0, a1, a2) Call3(Func_02001b28_a, a0, a1, a2)
#define BattleRuntime_WaitIfModeZero_4(args...) Func_02001b04(args)
#define ObjectMotion_EnableActionAndSetCallback_2(args...) Func_02001b44(args)
#define BattleRuntime_WaitIfModeZero_5(args...) Func_02001b12(args)
#define ObjectMotion_EnableActionAndResetMotion_3(args...) Func_02001b58(args)
#define BattleRuntime_WaitIfModeZero_6(args...) Func_02001b2e(args)
#define ObjectMotion_SetSpeedParameters_4(a0, a1, a2) Call3(Func_02001b6c, a0, a1, a2)
#define Object_SetPosition_5(a0, a1, a2, a3) Call4(Func_02001b24, a0, a1, a2, a3)
#define Object_CommitPosition_4(args...) Func_02001b32(args)
#define ObjectMotion_SetSpeedParameters_5(a0, a1, a2) Call3(Func_02001b90, a0, a1, a2)
#define Object_SetPosition_6(a0, a1, a2, a3) Call4(Func_02001b48, a0, a1, a2, a3)
#define Object_CommitPosition_5(args...) Func_02001b56(args)
#define Object_SetPosition_7(a0, a1, a2, a3) Call4(Func_02001b5e, a0, a1, a2, a3)
#define Object_CommitPosition_6(args...) Func_02001b6c_a(args)
#define Object_SetPosition_8(a0, a1, a2, a3) Call4(Func_02001b74, a0, a1, a2, a3)
#define Object_CommitPosition_7(args...) Func_02001b82(args)
#define Object_SetPosition_9(a0, a1, a2, a3) Call4(Func_02001b8a_a, a0, a1, a2, a3)
#define Object_CommitPosition_8(args...) Func_02001b98(args)
#define Object_SetPosition_10(a0, a1, a2, a3) Call4(Func_02001ba0, a0, a1, a2, a3)
#define ObjectMotion_EnableActionAndResetMotion_4(args...) Func_02001c0e(args)
#define ObjectMotion_ArmCallback_2(a0, a1, a2) Call3(Func_02001c70, a0, a1, a2)
#define BattleEffect_SpawnLinkedResourceObject_2(a0, a1, a2) Call3(Func_02001c8a, a0, a1, a2)
#define ObjectMotion_SetSpeedParameters_6(a0, a1, a2) Call3(Func_02001c28, a0, a1, a2)
#define ObjectMotion_Launch_1(args...) Func_02001c68(args)
#define ObjectMotion_Launch_2(args...) Func_02001c72(args)
#define Audio_PlayCue_1(args...) Func_02001d18(args)
#define GameFlag_Set_1(a0) Call1(Func_02001c18, a0)
#define SceneWork_SetStepValue_1(a0) Call1(Func_02001ca6, a0)
#define BattleRuntime_RunThenWaitIfModeZero_1(args...) Func_02001cb8(args)
#define BattleEffect_SpawnLinkedResourceObject_3(a0, a1, a2) Call3(Func_02001cdc, a0, a1, a2)
#define ObjectMotion_SetPositionAndReset_1(a0, a1, a2) Call3(Func_02001c98, a0, a1, a2)
#define BattleRuntime_WaitIfModeZero_7(args...) Func_02001c4e(args)
#define ObjectMotion_SetVariantCallbackAndInvokeObject_1(args...) Func_02001cc6(args)
#define ObjectMotion_SetSpeedParameters_7(args...) Func_02001d4e(args)
#define BattleRuntime_ScheduleShoulderButtonModeUpdate_1(args...) Func_02001c78(args)
#define SCENE_PHASE (*(s32 *)(*(u8 **)0x03001ebc + 0x1c0))
#define RECORD_S32(rec, off) (*(s32 *)((rec) + (off)))
#define BattleRuntime_Reset_1_02000af8(args...) Func_02001d10(args)
#define ObjectGroup_ConfigureChildValue_1_02000af8(args...) Func_02001d88(args)
#define Scene_GetRecord_1_02000af8(args...) Func_02001d3e(args)
#define Object_NotifyLastActiveOfEvent_1_02000af8(a0) Call1(Func_02001d40, a0)
#define Object_NotifyLastActiveOfEvent_2(a0) Call1(Func_02001d4c, a0)
#define ObjectMotion_EnableActionAndSetCallback_1_02000af8(a0, a1) Call2(Func_02001da4, a0, a1)
#define BattleRuntime_WaitIfModeZero_1_02000af8(args...) Func_02001e5e(args)
#define ObjectMotion_SetSpeedParameters_1_02000af8(args...) Func_02001e72(args)
#define BattleRuntime_WaitIfModeZero_2_02000af8(a0) Call1(Func_02001d8a, a0)
#define Audio_PlayCue_1_02000af8(args...) Func_02001e90(args)
#define BattleRuntime_WaitIfModeZero_3_02000af8(args...) Func_02001d96(args)
#define ObjectMotion_EnableActionAndResetMotion_1_02000af8(args...) Func_02001ddc(args)
#define ObjectMotion_EnableActionAndResetMotion_2_02000af8(args...) Func_02001de2(args)
#define ObjectMotion_EnableActionAndResetMotion_3_02000af8(args...) Func_02001de8(args)
#define ObjectMotion_EnableActionAndResetMotion_4_02000af8(args...) Func_02001dee(args)
#define ObjectMotion_EnableActionAndResetMotion_5(args...) Func_02001df4(args)
#define ObjectMotion_EnableActionAndResetMotion_6(args...) Func_02001dfa(args)
#define ObjectMotion_EnableActionAndResetMotion_7(args...) Func_02001e00(args)
#define ObjectMotion_SetSpeedParameters_2_02000af8(a0, a1, a2) Call3(Func_02001dfe, a0, a1, a2)
#define ObjectMotion_SetSpeedParameters_3_02000af8(a0, a1, a2) Call3(Func_02001e0c, a0, a1, a2)
#define ObjectMotion_SetSpeedParameters_4_02000af8(a0, a1, a2) Call3(Func_02001e1a, a0, a1, a2)
#define ObjectMotion_SetSpeedParameters_5_02000af8(a0, a1, a2) Call3(Func_02001e28, a0, a1, a2)
#define ObjectMotion_SetSpeedParameters_6_02000af8(a0, a1, a2) Call3(Func_02001e36, a0, a1, a2)
#define ObjectMotion_SetSpeedParameters_7_02000af8(a0, a1, a2) Call3(Func_02001e44, a0, a1, a2)
#define ObjectMotion_SetSpeedParameters_8(a0, a1, a2) Call3(Func_02001e52, a0, a1, a2)
#define ObjectMotion_ResetAndSetPosition_1(args...) Func_02001e74(args)
#define ObjectMotion_ResetAndSetPosition_2(args...) Func_02001e7e(args)
#define ObjectMotion_ResetAndSetPosition_3(args...) Func_02001e88(args)
#define ObjectMotion_ResetAndSetPosition_4(args...) Func_02001e92(args)
#define ObjectMotion_ResetAndSetPosition_5(args...) Func_02001e9c(args)
#define ObjectMotion_ResetAndSetPosition_6(a0, a1, a2) Call3(Func_02001ea8, a0, a1, a2)
#define ObjectMotion_ResetAndSetPosition_7(a0, a1, a2) Call3(Func_02001eb4, a0, a1, a2)
#define BattleRuntime_WaitIfModeZero_4_02000af8(args...) Func_02001e72_a(args)
#define BattleEffect_SpawnLinkedResourceObject_1_02000af8(a0, a1, a2) Call3(Func_02001f1c, a0, a1, a2)
#define BattleRuntime_WaitIfModeZero_5_02000af8(args...) Func_02001e82(args)
#define ObjectMotion_SetHorizontalPositionWithTerrain_1(args...) Func_02001ee4(args)
#define ObjectMotion_SetHorizontalPositionWithTerrain_2(args...) Func_02001eee(args)
#define ObjectMotion_SetHorizontalPositionWithTerrain_3(args...) Func_02001ef8(args)
#define ObjectMotion_SetHorizontalPositionWithTerrain_4(args...) Func_02001f02(args)
#define ObjectMotion_SetHorizontalPositionWithTerrain_5(args...) Func_02001f0c(args)
#define ObjectMotion_SetHorizontalPositionWithTerrain_6(args...) Func_02001f16(args)
#define ObjectMotion_SetHorizontalPositionWithTerrain_7(args...) Func_02001f20(args)
#define BattleRuntime_WaitIfModeZero_6_02000af8(args...) Func_02001ece(args)
#define Scene_GetRecord_2_02000af8(args...) Func_02001efc(args)
#define ObjectMotion_SetHorizontalPositionWithTerrain_8(a0, a1, a2) Call3(Func_02001f42, a0, a1, a2)
#define ObjectMotion_EnableActionAndResetMotion_8(args...) Func_02001f30(args)
#define ObjectMotion_SetVariantCallbackAndInvokeObject_1_02000af8(args...) Func_02001f6e(args)
#define ObjectMotion_ArmCallback_1_02000af8(a0, a1, a2) Call3(Func_02001f9a, a0, a1, a2)
#define Audio_PlayCue_2(args...) Func_02002010(args)
#define GameFlag_Set_1_02000af8(a0) Call1(Func_02001f10, a0)
#define BattleEffect_SpawnLinkedResourceObject_2_02000af8(a0, a1, a2) Call3(Func_02001fde, a0, a1, a2)
#define ObjectMotion_SetVariantCallbackAndInvokeObject_2(args...) Func_02001fb6(args)
#define ObjectMotion_SetPositionAndReset_1_02000af8(a0, a1, a2) Call3(Func_02001fa2, a0, a1, a2)
#define ObjectMotion_SetPositionAndReset_2(a0, a1, a2) Call3(Func_02001fae, a0, a1, a2)
#define ObjectMotion_ArmCallback_2_02000af8(a0, a1, a2) Call3(Func_02001ffa, a0, a1, a2)
#define Scene_GetRecord_3(args...) Func_02001f98(args)
#define BattleRuntime_WaitIfModeZero_7_02000af8(args...) Func_02001f9e(args)
#define ObjectMotion_Launch_1_02000af8(args...) Func_02002008(args)
#define Audio_PlayCue_3(args...) Func_020020ae(args)
#define BattleRuntime_WaitIfModeZero_8(args...) Func_02001fb4(args)
#define ObjectMotion_EnableActionAndSetCallback_2_02000af8(a0, a1) Call2(Func_02001ff4, a0, a1)
#define BattleRuntime_WaitIfModeZero_9(args...) Func_02001fc2(args)
#define ObjectMotion_SetActionVariant_1(args...) Func_02002062(args)
#define ObjectMotion_SetSpeedParameters_9(a0, a1, a2) Call3(Func_02002008_a, a0, a1, a2)
#define Audio_PlayCue_4(args...) Func_020020e6(args)
#define ObjectMotion_ResetAndSetPosition_8(a0, a1, a2) Call3(Func_02002040, a0, a1, a2)
#define ObjectMotion_ResetAndSetPosition_9(a0, a1, a2) Call3(Func_0200204c, a0, a1, a2)
#define BattleRuntime_WaitIfModeZero_10(args...) Func_0200200a(args)
#define ObjectMotion_SetHorizontalPositionWithTerrain_9(args...) Func_0200206c(args)
#define Scene_GetRecord_4(args...) Func_02002042(args)
#define BattleRuntime_WaitIfModeZero_11(args...) Func_02002030(args)
#define ObjectMotion_SetSpeedParameters_10(args...) Func_02002136(args)
#define BattleRuntime_ScheduleShoulderButtonModeUpdate_1_02000af8(args...) Func_02002060(args)
#define SceneActor_SetFacingFromSample Func_0200007c
#define OverlayObject_Add160ToFields18And1c Func_020000a4
#define OverlayObject_ShrinkScaleThenStop Func_02000180
#define SceneData_GetTable95c0 Func_020001c8
#define SceneData_GetTable9680 Func_020001d0
#define SceneData_GetTable96a0 Func_020001d8
#define SceneData_GetTable96C4 Func_020001e0
#define SceneData_GetTable988c Func_020001e8
#define FieldScene_CallHelper14d0 Func_020001f0
#define FieldScene_RunActor232SceneWhenFlag923Or922 Func_020001fc
#define FieldScene_RunScene3b0_0200040c Func_0200040c
#define FieldScene_RunScene3b0_02000468 Func_02000468
#define FieldScene_RunScene3b0_020004b0 Func_020004b0
#define FieldScene_RunActorNinePresentationCycles Func_020007b0
#define OverlayObject_InitWithRandomFields Func_02000a84
#define FieldScene_RunPrimarySequence Func_02000af8
#define SceneData_GetDifferenceOfPairSums Func_02001130
#define SceneData_GetValueByFirstSetFlag Func_0200115c
#define FieldScene_RunSevenActorEnsemble Func_02000e78

extern u8 Data_020095c0[];
extern u8 Data_02009680[];
extern u8 Data_020096a0[];
extern u8 Data_020096c4[];
extern u8 Data_0200988c[];
extern u8 Value_00000923;
extern u8 Value_00000922;
extern u8 Value_00000924;
extern s32 Data_020093a4[];
extern s32 Data_020098f8[];
extern u16 Data_02000240[];
extern u8 Value_0000006f;

void Func_020014d0(void);
s32 Func_020013fe(s32);
s32 Func_02001408(s32);
void Func_02001428(void);
void Func_02001520(s32, s32);
s32 Func_02001450(s32, s32);
void Func_0200142e(s32);
void Func_0200144a(void);
void Func_02000908();
void Func_020015dc();
void Func_020015fe();
void Func_02001620();
void Func_02001624();
void Func_02001678();
void Func_020016b8();
void Func_020016e6();
s32 Func_020016f8();
void Func_02001704();
void Func_02000950();
void Func_02001646();
void Func_02001650();
void Func_02001672();
void Func_02001680();
void Func_02001680_a();
s32 Func_020016ba();
void Func_020016c0();
void Func_020016dc();
void Func_02001704_a();
void Func_020016d8();
void Func_02001716();
void Func_02001740();
void Func_0200177c();
void Func_0200178c();
void Func_02001796();
void Func_02001798();
void Func_020017a4();
void Func_020017a6();
void Func_020017ae();
void Func_020017b0();
void Func_020017c2();
void Func_020017c4();
void Func_020017d6();
void Func_02001824();
void Func_0200183a();
void Func_02001846();
void Func_0200126a();
void Func_02001270();
void Func_02001276();
void Func_0200127c();
void Func_02001282();
void Func_02001288();
void Func_0200128e();
void Func_02001376();
void Func_02001476();
void Func_02001988();
void Func_02001994();
void Func_020019c2();
void Func_020019ce();
void Func_020019fe();
void Func_02001a3e();
void Func_02001a46();
void Func_02001a5a();
void Func_02001a5e();
void Func_02001a68();
void Func_02001a74();
void Func_02001a7c();
void Func_02001a82();
void Func_02001a8a();
void Func_02001a8c();
void Func_02001a98();
void Func_02001aa0();
void Func_02001aa6();
void Func_02001b04();
void Func_02001b0e();
void Func_02001b12();
void Func_02001b14();
void Func_02001b24();
void Func_02001b28();
void Func_02001b28_a();
void Func_02001b2e();
void Func_02001b32();
void Func_02001b44();
void Func_02001b48();
void Func_02001b56();
void Func_02001b58();
void Func_02001b5e();
void Func_02001b6c();
void Func_02001b6c_a();
void Func_02001b70();
void Func_02001b74();
void Func_02001b7c();
void Func_02001b82();
void Func_02001b8a();
void Func_02001b8a_a();
void Func_02001b90();
void Func_02001b98();
void Func_02001ba0();
void Func_02001c0e();
void Func_02001c18();
void Func_02001c28();
void Func_02001c4e();
void Func_02001c68();
void Func_02001c70();
void Func_02001c72();
void Func_02001c78();
void Func_02001c8a();
void Func_02001c98();
void Func_02001ca6();
void Func_02001cb8();
void Func_02001cc6();
void Func_02001cdc();
void Func_02001d18();
void Func_02001d2c();
void Func_02001d42();
void Func_02001d4e();
u8 *Func_020019fc();
u8 *Func_02001a6c();
void Func_02001a6c_a();
u8 *Func_02001cbe(void);
void Func_02001d38(s32, s32);
u32 Func_02001c64(void);
u32 Func_02001c70_a(void);
u32 Func_02001c7c(void);
u32 Func_02001c8e(void);
void Func_02001d2c_a(s32, void *);
void Func_020015b8();
void Func_020015be();
void Func_020015c4();
void Func_020015ca();
void Func_020015d0();
void Func_020015d6();
void Func_020015dc_a();
void Func_02001cca();
void Func_02001cd6();
void Func_02001ce2();
void Func_02001d04();
void Func_02001d10();
void Func_02001d40();
void Func_02001d4c();
void Func_02001d88();
void Func_02001d8a();
void Func_02001d96();
void Func_02001da4();
void Func_02001ddc();
void Func_02001de2();
void Func_02001de8();
void Func_02001dee();
void Func_02001df4();
void Func_02001dfa();
void Func_02001dfe();
void Func_02001e00();
void Func_02001e0c();
void Func_02001e1a();
void Func_02001e28();
void Func_02001e36();
void Func_02001e44();
void Func_02001e52();
void Func_02001e5e();
void Func_02001e72();
void Func_02001e72_a();
void Func_02001e74();
void Func_02001e7e();
void Func_02001e82();
void Func_02001e88();
void Func_02001e90();
void Func_02001e92();
void Func_02001e9c();
void Func_02001e9e();
void Func_02001ea8();
void Func_02001eb4();
void Func_02001ece();
void Func_02001ed8();
void Func_02001ee4();
void Func_02001eee();
void Func_02001ef8();
void Func_02001f02();
void Func_02001f0c();
void Func_02001f10();
void Func_02001f16();
void Func_02001f1c();
void Func_02001f20();
void Func_02001f30();
void Func_02001f42();
void Func_02001f6e();
s32 Func_02001f98();
void Func_02001f9a();
void Func_02001f9e();
void Func_02001fa2();
void Func_02001fae();
void Func_02001fb4();
void Func_02001fb6();
void Func_02001fc2();
void Func_02001fde();
void Func_02001ff4();
void Func_02001ffa();
void Func_02002008();
void Func_02002008_a();
void Func_0200200a();
void Func_02002010();
void Func_02002030();
void Func_02002040();
s32 Func_02002042();
void Func_0200204c();
void Func_02002060();
void Func_02002062();
void Func_0200206c();
void Func_020020ae();
void Func_020020e6();
void Func_02002114();
void Func_0200212a();
void Func_02002136();
s32 Func_02002292(s32);
s32 Func_0200229a(s32);
s32 Func_020022a2(s32);
s32 Func_020022aa(s32);
s32 Func_0200238c(s32);
void Func_02001926();
void Func_0200192c();
void Func_02001932();
void Func_02001938();
void Func_0200193e();
void Func_02001944();
void Func_0200194a();
void Func_02002050();
void Func_02002084();
void Func_02002090();
void Func_020020ba();
s32 Func_020020be();
void Func_020020f6();
void Func_02002108();
void Func_02002112();
void Func_0200213c();
void Func_02002142();
void Func_02002148();
void Func_0200214e();
void Func_02002154();
void Func_0200215a();
void Func_0200215e();
void Func_02002160();
void Func_0200216c();
s32 Func_02002176();
void Func_0200217a();
void Func_02002188();
void Func_02002196();
void Func_020021a4();
void Func_020021b2();
void Func_020021c0();
void Func_020021c2();
void Func_02009244();
void Func_020092ec();
void Func_020021d2();
void Func_020021d8();
void Func_020021da();
void Func_020021de();
void Func_020021e2();
void Func_020021ea();
void Func_020021f2();
void Func_0200222a();
void Func_02002230();
void Func_02002248();
void Func_02002256();
void Func_02002270();
void Func_02002278();
void Func_020092b4();
void Func_0200926c();
void Func_0200228c();
void Func_020022a0();
void Func_020022b2();
void Func_0200231c();
void Func_02002324();
void Func_02002328();
void Func_0200232c();

/* AUDITED GENERATED CALL SCRIPT for FieldScene_RunSevenActorEnsemble:
 * Seven actor setup and motion sequences, two scene-workspace state writes,
 * the closing map-state publication, and the final selector branch. */

/*
 * Scripted per-object step for resource_3b0.  It turns the object to one of two
 * fixed headings when a sampled value picks them out, then reports finished.
 */

/* Old-style declarations: overlay imports vary in arity between call sites. */

/* Loader-relocated overlay calls: each symbol names the pre-relocation call
 * word the image holds. */

/* Resolved engine calls: each pseudo symbol is the per-site call word the
 * overlay image holds (a word can serve two sites with different targets),
 * and the macro names the engine function the site reaches through the
 * overlay veneer and the main-image veneer island, keeping the site's own
 * calling form. Names without a repository binding are provisional.
 */

/* Phase/status word at 0x1c0 of the shared scene work record. */

/* A record's four-byte field at the given byte offset. Used below for the
 * seven fields that get the same reset pattern twice, once per record. */

/* Call sites spelled through these wrappers pass their constants straight
 * into the argument registers; a direct call precomputes a costly constant
 * into a pseudo that the compiler then shares with later uses in the block.
 * A value-returning call also sets r0 last of its arguments. */
static __inline__ void Call3(void (*f)(), s32 a0, s32 a1, s32 a2)
{
    extern u8 Data_03001ebc[];

    f(a0, a1, a2);
}

static __inline__ void Call4(void (*f)(), s32 a0, s32 a1, s32 a2, s32 a3)
{
    extern u8 Data_03001ebc[];

    f(a0, a1, a2, a3);
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
static __inline__ void Call1(void (*f)(), s32 a0)
{
    extern u8 Data_03001ebc[];

    f(a0);
}

static __inline__ void Call3_020004b0(void (*f)(), s32 a0, s32 a1, s32 a2)
{
    extern u8 Data_03001ebc[];

    f(a0, a1, a2);
}

static __inline__ void Call3_020007b0(void (*f)(), s32 a0, s32 a1, s32 a2)
{
    extern u8 Data_03001ebc[];

    u8 *Func_02001d3e();
    u8 *Func_02001efc();

    f(a0, a1, a2);
}

static __inline__ void Call2(void (*f)(), s32 a0, s32 a1)
{
    extern u8 Data_03001ebc[];

    s32 Func_02001d3e();
    s32 Func_02001efc();

    f(a0, a1);
}

static __inline__ void Call3_02000af8(void (*f)(), s32 a0, s32 a1, s32 a2)
{
    extern u8 Data_03001ebc[];

    s32 Func_02001d3e();
    s32 Func_02001efc();

    f(a0, a1, a2);
}

static __inline__ void Call2_02000e78(void (*f)(), s32 a0, s32 a1)
{
    extern u8 *Data_03001ebc;

    f(a0, a1);
}

static __inline__ s32 Value2(s32 (*f)(), s32 a0, s32 a1)
{
    extern u8 *Data_03001ebc;

    return f(a0, a1);
}

static __inline__ void Call3_02000e78(void (*f)(), s32 a0, s32 a1, s32 a2)
{
    extern u8 *Data_03001ebc;

    f(a0, a1, a2);
}

s32 Func_02001246();            /* Sampled value. */

/*
 * The shift pair is a windowed extraction of bits 10..15 of the sample, giving
 * a value in 0..63; a plain right shift would let larger values reach the
 * comparisons.  The headings 0xd000 and 0xb000 are built from an immediate and
 * a shift, and the local in each arm is what forces that.  The halfword at
 * object + 6 is the facing angle.  What Func_02001246 samples is not
 * established here.
 */
s32 SceneActor_SetFacingFromSample(u8 *object)
{
    extern u8 Data_03001ebc[];

    u32 sample = (u32)(Func_02001246() << 6) >> 16;   /* bits 10..15 */

    if (sample == 6) {
        s32 value = 0xd000;

        *(u16 *)(object + 6) = value;
    } else if (sample == 9) {
        s32 value = 0xb000;

        *(u16 *)(object + 6) = value;
    }

    return 1;
}

void OverlayObject_Add160ToFields18And1c(u8 *o)
{
    extern u8 Data_03001ebc[];

    if (*(s32 *)(o + 24) < 0x10000) {
        *(s32 *)(o + 24) += 160;
        *(s32 *)(o + 28) += 160;
    }
}

s32 OverlayObject_ShrinkScaleThenStop(u8 *o)
{
    extern u8 Data_03001ebc[];

    u8 *t = *(u8 **)(o + 0x50);

    t[9] |= 12;
    *(s32 *)(o + 48) = 0x20000;
    *(s32 *)(o + 52) = 0x10000;
    if (*(s32 *)(o + 24) > 0x1000) {
        *(s32 *)(o + 24) += 0xFFFFFC00;
        *(s32 *)(o + 28) += 0xFFFFFC00;
    } else {
        *(s32 *)(o + 8) = 0;
        *(s32 *)(o + 12) = 0;
        *(s32 *)(o + 16) = 0;
        *(s32 *)(o + 36) = 0;
        *(s32 *)(o + 40) = 0;
        *(s32 *)(o + 44) = 0;
    }
    return 1;
}

u8 *SceneData_GetTable95c0(void)
{
    extern u8 Data_03001ebc[];

    return Data_020095c0;
}

u8 *SceneData_GetTable9680(void)
{
    extern u8 Data_03001ebc[];

    return Data_02009680;
}

u8 *SceneData_GetTable96a0(void)
{
    extern u8 Data_03001ebc[];

    return Data_020096a0;
}

u8 *SceneData_GetTable96C4(void)
{
    extern u8 Data_03001ebc[];

    return Data_020096c4;
}

u8 *SceneData_GetTable988c(void)
{
    extern u8 Data_03001ebc[];

    return Data_0200988c;
}

void FieldScene_CallHelper14d0(void)
{
    extern u8 Data_03001ebc[];

    Func_020014d0();
}

void FieldScene_RunActor232SceneWhenFlag923Or922(void)
{
    extern u8 Data_03001ebc[];

    if (Func_020013fe((s32)&Value_00000923) != 0 || Func_02001408((s32)&Value_00000922) != 0) {
        Func_02001428();
        Func_02001520(0xE8, 3);
        Func_02001450(0xE8, 0);
        Func_0200142e((s32)&Value_00000924);
        Func_0200144a();
    }
}

void FieldScene_RunScene3b0_0200040c(void)
{
    extern u8 Data_03001ebc[];

    u32 i;
    s32 record;

    Func_02001624();
    Call4(Func_020016e6, -1, -1, -1, 0);
    Func_020015dc(1);
    *(u8 *)(Func_020016f8() + 85) = 0;
    Call3(Func_02001704, 0xa40000, 0x400000, 0x1410000);
    Func_02001620();
    Func_020015fe(1);
    Func_020016b8(0, 0, 0);
    Func_02000908();
    Func_02001678();
}

void FieldScene_RunScene3b0_02000468(void)
{
    extern u8 Data_03001ebc[];

    u32 i;
    s32 record;

    Func_02001680();
    Call3(Func_020016dc, 0, 0xa40000, 0x1410000);
    Func_02001704_a(0, 15);
    record = Func_020016ba(0);
    Func_02001680_a(record, 0);
    Func_02001646(1);
    Func_02001672();
    Func_02001650(1);
    Func_02000950();
    Func_020016c0();
}

void FieldScene_RunScene3b0_020004b0(void)
{
    extern u8 Data_03001ebc[];

    u32 i;
    s32 record;

    *(s32 *)((*(u8 *volatile *)Data_03001ebc + 0x1c0)) = 0x202;
    Func_020017ae();
    Func_020017c2();
    Func_020016d8(20);
    Call3_020004b0(Func_02001716, 8, 0x10000, 0x8000);
    Call3_020004b0(Func_02001740, 8, 164, 0x141);
    Call3_020004b0(Func_0200178c, 8, 0xd000, 40);
    Call3_020004b0(Func_02001798, 8, 0xb000, 40);
    Call3_020004b0(Func_020017a4, 8, 0xd000, 40);
    Call3_020004b0(Func_020017b0, 8, 0x3000, 10);
    Call3_020004b0(Func_0200177c, 8, 164, 0x14e);
    Func_02001796(8, 4, 40);
    Func_020017a6(8, 2);
    Call1(Func_020017c4, 0x1e3a);
    Func_020017d6(8, 0, 20);
    Func_0200183a();
    Func_02001846();
    Func_02001824(10);
}

/* Runs the record-8/record-9 pair through two near-identical setup-then-move
 * cycles (position waypoints, a movement flag reset, then animation/sound
 * calls), followed by a shorter closing cycle for record 8 alone. */
void FieldScene_RunActorNinePresentationCycles(void)
{
    extern u8 Data_03001ebc[];

    u8 *Func_02001d3e();
    u8 *Func_02001efc();

    u32 i;
    u8 *rec9;
    u8 *record;

    BattleRuntime_Reset_1();
    ObjectGroup_ConfigureChildValue_1(0, 15);
    record = Scene_GetRecord_1(0);
    Func_020019c2(record, 0);
    Func_02001988(1);
    Object_NotifyLastActiveOfEvent_1(0x200976c);
    Func_02001994(1);
    Func_0200126a(9);
    Func_02001270(10);
    Func_02001276(11);
    Func_0200127c(12);
    Func_02001282(13);
    Func_02001288(14);
    Func_0200128e(15);
    ObjectMotion_EnableActionAndSetCallback_1(8, 0x200939c);
    SCENE_PHASE = 0x203;
    BattleRuntime_WaitIfModeZero_1();
    ObjectMotion_SetSpeedParameters_1();
    BattleRuntime_WaitIfModeZero_2(120);
    rec9 = Scene_GetRecord_2(9);
    ObjectMotion_EnableActionAndResetMotion_1(9);
    /* Reset record 9's waypoint/velocity fields: three fields to the
     * minimum s32, then four fields to zero. */
    RECORD_S32(rec9, 56) = -0x80000000;
    RECORD_S32(rec9, 60) = -0x80000000;
    RECORD_S32(rec9, 64) = -0x80000000;
    RECORD_S32(rec9, 36) = 0;
    RECORD_S32(rec9, 40) = 0;
    RECORD_S32(rec9, 44) = 0;
    RECORD_S32(rec9, 76) = 0;
    BattleRuntime_WaitIfModeZero_3(20);
    ObjectMotion_SetSpeedParameters_2(9, 0x80000, 0x40000);
    Object_SetPosition_1(rec9, 0xa40000, 0x900000, 0x1410000);
    Object_CommitPosition_1(rec9);
    Object_SetPosition_2(rec9, 0xa40000, 0x680000, 0x1410000);
    Object_CommitPosition_2(rec9);
    Object_SetPosition_3(rec9, 0xcc0000, 0x7c0000, 0x1410000);
    Object_CommitPosition_3(rec9);
    Object_SetPosition_4(rec9, 0x900000, 0, 0xa90000);
    ObjectMotion_EnableActionAndResetMotion_2(8);
    Func_02001a7c(1);
    ObjectMotion_ArmCallback_1(8, 0x8000, 0);
    BattleEffect_SpawnLinkedResourceObject_1(8, 0x103, 60);
    ObjectMotion_SetSpeedParameters_3(9, 0x20000, 0x10000);
    Func_02001376(9);
    BattleRuntime_WaitIfModeZero_4(20);
    ObjectMotion_EnableActionAndSetCallback_2(8, 0x200939c);
    BattleRuntime_WaitIfModeZero_5(120);
    ObjectMotion_EnableActionAndResetMotion_3(9);
    /* Same reset pattern on record 9 for the second cycle. */
    RECORD_S32(rec9, 56) = -0x80000000;
    RECORD_S32(rec9, 60) = -0x80000000;
    RECORD_S32(rec9, 64) = -0x80000000;
    RECORD_S32(rec9, 36) = 0;
    RECORD_S32(rec9, 40) = 0;
    RECORD_S32(rec9, 44) = 0;
    RECORD_S32(rec9, 76) = 0;
    BattleRuntime_WaitIfModeZero_6(20);
    ObjectMotion_SetSpeedParameters_4(9, 0x80000, 0x40000);
    Object_SetPosition_5(rec9, 0xa40000, 0x900000, 0x1410000);
    Object_CommitPosition_4(rec9);
    ObjectMotion_SetSpeedParameters_5(9, 0x50000, 0x28000);
    Object_SetPosition_6(rec9, 0xa40000, 0x680000, 0x1410000);
    Object_CommitPosition_5(rec9);
    Object_SetPosition_7(rec9, 0xa40000, 0x720000, 0x1410000);
    Object_CommitPosition_6(rec9);
    Object_SetPosition_8(rec9, 0xa40000, 0x680000, 0x1410000);
    Object_CommitPosition_7(rec9);
    Object_SetPosition_9(rec9, 0xcc0000, 0x7c0000, 0x1410000);
    Object_CommitPosition_8(rec9);
    Object_SetPosition_10(rec9, 0x900000, 0, 0xa90000);
    ObjectMotion_EnableActionAndResetMotion_4(8);
    Func_02001b7c(1);
    ObjectMotion_ArmCallback_2(8, 0x8000, 0);
    BattleEffect_SpawnLinkedResourceObject_2(8, 0x103, 60);
    ObjectMotion_SetSpeedParameters_6(9, 0x20000, 0x10000);
    Func_02001476(9);
    ObjectMotion_Launch_1(8, 4, 20);
    ObjectMotion_Launch_2(8, 6, 40);
    Audio_PlayCue_1(29);
    GameFlag_Set_1(0x8f0);
    SceneWork_SetStepValue_1(0x1e49);
    BattleRuntime_RunThenWaitIfModeZero_1(16, 0, 20);
    BattleEffect_SpawnLinkedResourceObject_3(8, 0x100, 0);
    ObjectMotion_SetPositionAndReset_1(8, 164, 0x158);
    BattleRuntime_WaitIfModeZero_7(40);
    ObjectMotion_SetVariantCallbackAndInvokeObject_1(8, 2);
    Func_02001d42();
    ObjectMotion_SetSpeedParameters_7();
    Func_02001d2c(12);
    BattleRuntime_ScheduleShoulderButtonModeUpdate_1();
}

void OverlayObject_InitWithRandomFields(s32 a)
{
    extern u8 Data_03001ebc[];

    u8 *obj;
    u32 x;

    obj = Func_02001cbe();
    Func_02001d38(a, 1);
    obj[0x55] = 0;
    *(u16 *)(obj + 0x64) = Func_02001c64() >> 15;
    *(u16 *)(obj + 0x66) = Func_02001c70_a() >> 15;
    x = Func_02001c7c();
    x <<= 2;
    x >>= 16;
    x <<= 16;
    x += 0x60000;
    *(s32 *)(obj + 0xc) = x;
    x = Func_02001c8e();
    *(s32 *)(obj + 0x4c) = ((x * 3 << 13) >> 16) - 0x3000;
    *(s32 *)(obj + 0x18) = 0x14000;
    *(s32 *)(obj + 0x1c) = 0x14000;
    Func_02001d2c_a(a, Data_020093a4);
}

/* Drives ids 8 through 18 through position, scale, and flag updates in
 * sequence, stepping the shared scene phase at the start and end. */
void FieldScene_RunPrimarySequence(void)
{
    extern u8 Data_03001ebc[];

    s32 Func_02001d3e();
    s32 Func_02001efc();

    u32 i;
    s32 rec;
    s32 id0_state;

    BattleRuntime_Reset_1_02000af8();
    ObjectGroup_ConfigureChildValue_1_02000af8(0, 15);
    id0_state = Scene_GetRecord_1_02000af8(0);
    Func_02001d04(id0_state, 0);
    Func_02001cca(1);
    Object_NotifyLastActiveOfEvent_1_02000af8(0x200976c);
    Func_02001cd6(1);
    Object_NotifyLastActiveOfEvent_2(0x2009844);
    Func_02001ce2(1);
    Func_020015b8(9);
    Func_020015be(10);
    Func_020015c4(11);
    Func_020015ca(12);
    Func_020015d0(13);
    Func_020015d6(14);
    Func_020015dc_a(15);
    ObjectMotion_EnableActionAndSetCallback_1_02000af8(8, 0x200939c);
    SCENE_PHASE = 0x203;
    BattleRuntime_WaitIfModeZero_1_02000af8();
    ObjectMotion_SetSpeedParameters_1_02000af8();
    BattleRuntime_WaitIfModeZero_2_02000af8(0x12c);
    Audio_PlayCue_1_02000af8(147);
    BattleRuntime_WaitIfModeZero_3_02000af8(100);
    ObjectMotion_EnableActionAndResetMotion_1_02000af8(9);
    ObjectMotion_EnableActionAndResetMotion_2_02000af8(10);
    ObjectMotion_EnableActionAndResetMotion_3_02000af8(11);
    ObjectMotion_EnableActionAndResetMotion_4_02000af8(12);
    ObjectMotion_EnableActionAndResetMotion_5(13);
    ObjectMotion_EnableActionAndResetMotion_6(14);
    ObjectMotion_EnableActionAndResetMotion_7(15);
    ObjectMotion_SetSpeedParameters_2_02000af8(9, 0x30000, 0x18000);
    ObjectMotion_SetSpeedParameters_3_02000af8(10, 0x30000, 0x18000);
    ObjectMotion_SetSpeedParameters_4_02000af8(11, 0x30000, 0x18000);
    ObjectMotion_SetSpeedParameters_5_02000af8(12, 0x30000, 0x18000);
    ObjectMotion_SetSpeedParameters_6_02000af8(13, 0x30000, 0x18000);
    ObjectMotion_SetSpeedParameters_7_02000af8(14, 0x30000, 0x18000);
    ObjectMotion_SetSpeedParameters_8(15, 0x30000, 0x18000);
    ObjectMotion_ResetAndSetPosition_1(9, 0, 100);
    ObjectMotion_ResetAndSetPosition_2(10, 60, 100);
    ObjectMotion_ResetAndSetPosition_3(11, 120, 100);
    ObjectMotion_ResetAndSetPosition_4(12, 180, 100);
    ObjectMotion_ResetAndSetPosition_5(13, 240, 100);
    ObjectMotion_ResetAndSetPosition_6(14, 0x140, 100);
    ObjectMotion_ResetAndSetPosition_7(15, 0x17c, 100);
    BattleRuntime_WaitIfModeZero_4_02000af8(40);
    BattleEffect_SpawnLinkedResourceObject_1_02000af8(8, 0x101, 0);
    BattleRuntime_WaitIfModeZero_5_02000af8(20);
    ObjectMotion_SetHorizontalPositionWithTerrain_1(9, 0, 0);
    ObjectMotion_SetHorizontalPositionWithTerrain_2(10, 0, 0);
    ObjectMotion_SetHorizontalPositionWithTerrain_3(11, 0, 0);
    ObjectMotion_SetHorizontalPositionWithTerrain_4(12, 0, 0);
    ObjectMotion_SetHorizontalPositionWithTerrain_5(13, 0, 0);
    ObjectMotion_SetHorizontalPositionWithTerrain_6(14, 0, 0);
    ObjectMotion_SetHorizontalPositionWithTerrain_7(15, 0, 0);
    BattleRuntime_WaitIfModeZero_6_02000af8(100);
    rec = Scene_GetRecord_2_02000af8(18);
    *(s32 *)(rec + 24) = 0x1999;
    *(s32 *)(rec + 28) = 0x1999;
    ObjectMotion_SetHorizontalPositionWithTerrain_8(18, 0xac0000, 0x1540000);
    ObjectMotion_EnableActionAndResetMotion_8(8);
    Func_02001e9e(1);
    ObjectMotion_SetVariantCallbackAndInvokeObject_1_02000af8(8, 1);
    ObjectMotion_ArmCallback_1_02000af8(8, 0x3000, 0);
    Audio_PlayCue_2(29);
    GameFlag_Set_1_02000af8(0x8f0);
    for (i = 0; i < 32; i++) {
        *(s32 *)(rec + 24) += 0xccc;
        *(s32 *)(rec + 28) += 0xccc;
        Func_02001ed8(1);
    }
    BattleEffect_SpawnLinkedResourceObject_2_02000af8(8, 0x101, 60);
    ObjectMotion_SetVariantCallbackAndInvokeObject_2(8, 2);
    ObjectMotion_SetPositionAndReset_1_02000af8(8, 168, 0x154);
    ObjectMotion_SetPositionAndReset_2(8, 200, 0x154);
    ObjectMotion_ArmCallback_2_02000af8(8, 0x8000, 0);
    rec = Scene_GetRecord_3(17);
    *(s32 *)(rec + 24) = 0x12666;
    *(s32 *)(rec + 28) = 0x12666;
    *(s32 *)(rec + 8) = 0xac0000;
    *(s32 *)(rec + 12) = 0xa00000;
    *(s32 *)(rec + 16) = 0x1540000;
    {
        /* Clear the flag word at +6. */
        s32 shown = 0;

        *(u16 *)(rec + 6) = shown;
    }
    *(s32 *)(rec + 68) = 0x6666;
    *(s32 *)(rec + 72) = 0x30000;
    BattleRuntime_WaitIfModeZero_7_02000af8(20);
    ObjectMotion_Launch_1_02000af8(8, 6, 20);
    Audio_PlayCue_3(147);
    BattleRuntime_WaitIfModeZero_8(20);
    ObjectMotion_EnableActionAndSetCallback_2_02000af8(8, 0x20093ac);
    BattleRuntime_WaitIfModeZero_9(80);
    ObjectMotion_SetActionVariant_1(17, 1);
    ObjectMotion_SetSpeedParameters_9(17, 0x10000, 0x8000);
    *(s32 *)(rec + 68) = 0x1999;
    *(s32 *)(rec + 72) = 0xb333;
    Audio_PlayCue_4(153);
    *(s32 *)(rec + 40) = 0x80000;
    ObjectMotion_ResetAndSetPosition_8(17, 132, 0x168);
    ObjectMotion_ResetAndSetPosition_9(18, 132, 0x168);
    BattleRuntime_WaitIfModeZero_10(40);
    ObjectMotion_SetHorizontalPositionWithTerrain_9(17, 0, 0);
    rec = Scene_GetRecord_4(8);
    *(s32 *)(rec + 24) = 0x10000;
    *(s32 *)(rec + 28) = 0x10000;
    {
        /* Set the flag word at +6. */
        s32 shown = 0x5000;

        *(u16 *)(rec + 6) = shown;
    }
    BattleRuntime_WaitIfModeZero_11(40);
    SCENE_PHASE = 0x202;
    Func_0200212a();
    ObjectMotion_SetSpeedParameters_10();
    Func_02002114(13);
    BattleRuntime_ScheduleShoulderButtonModeUpdate_1_02000af8();
}

void Func_02000e78(void)
{
    extern u8 *Data_03001ebc;

    s32 ensemble;
    s32 selector;

    Func_02002090();
    Func_02002108(0, 15);
    ensemble = Func_020020be(0);
    Func_02002084(ensemble, 0);
    Call1(Func_020020ba, 33593196);
    Func_02002050(1);
    Func_02001926(9);
    Func_0200192c(10);
    Func_02001932(11);
    Func_02001938(12);
    Func_0200193e(13);
    Func_02001944(14);
    Func_0200194a(15);
    Call2_02000e78(Func_02002112, 8, 33592220);
    *(s32 *)(Data_03001ebc + 448) = 515;
    Func_020092ec();
    Func_020021de();
    Call1(Func_020020f6, 400);
    Func_0200213c(9);
    Func_02002142(10);
    Func_02002148(11);
    Func_0200214e(12);
    Func_02002154(13);
    Func_0200215a(14);
    Func_02002160(15);
    Call3_02000e78(Func_0200215e, 9, 196608, 98304);
    Call3_02000e78(Func_0200216c, 10, 196608, 98304);
    Call3_02000e78(Func_0200217a, 11, 196608, 98304);
    Call3_02000e78(Func_02002188, 12, 196608, 98304);
    Call3_02000e78(Func_02002196, 13, 196608, 98304);
    Call3_02000e78(Func_020021a4, 14, 196608, 98304);
    Call3_02000e78(Func_020021b2, 15, 196608, 98304);
    Value2(Func_020021c2, 9, 33592400);
    Value2(Func_02009244, 10, 33592448);
    Value2(Func_020021d2, 11, 33592496);
    Value2(Func_020021da, 12, 33592544);
    Value2(Func_020021e2, 13, 33592592);
    Value2(Func_020021ea, 14, 33592640);
    Value2(Func_020021f2, 15, 33592688);
    Func_020021c0(40);
    Func_02002230(8, 3);
    Call2_02000e78(Func_020092b4, 8, 258);
    Func_020021d8(120);
    Func_02002248(8, 1);
    Call3_02000e78(Func_0200228c, 8, 256, 60);
    Call3_02000e78(Func_0200222a, 8, 65536, 32768);
    Call3_02000e78(Func_02002256, 8, 164, 344);
    Func_02002270(8, 4, 10);
    Func_0200926c(8, 6, 20);
    Call1(Func_020022a0, 7908);
    Func_020022b2(8, 0, 20);
    *(s32 *)(Data_03001ebc + 448) = 514;
    Func_0200231c();
    Func_02002328();
    Data_02000240[226] = (s32)&Value_0000006f;
    Data_02000240[227] = 2;
    selector = Func_02002176();
    if (selector == 11) {
        Func_02002324(15);
    } else {
        Func_0200232c(14);
    }
    Func_02002278();
}

s32 SceneData_GetDifferenceOfPairSums(void)
{
    extern u8 Data_03001ebc[];

    s32 a;
    s32 b;

    a = Func_02002292(0);
    a += Func_0200229a(2);
    b = Func_020022a2(1);
    b += Func_020022aa(3);
    return a - b;
}

s32 SceneData_GetValueByFirstSetFlag(u32 a)
{
    extern u8 Data_03001ebc[];

    s32 base = 0;
    u32 i;

    switch (a) {
    case 0:
        base = 0x92c;
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
    for (i = 0; i <= 8; i++) {
        if (Func_0200238c(base + i) != 0)
            return Data_020098f8[i];
    }
    return 0;
}
