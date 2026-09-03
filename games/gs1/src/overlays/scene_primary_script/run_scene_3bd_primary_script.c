#include "types.h"

#define FieldScene_RunBranchingCutsceneSequence Func_02002c44

/*
 * resource_3bd cutscene owner 0x02002c44-0x020034bb (2168 bytes).
 *
 * The owner is a no-argument, void, almost-linear scene script.  Its only
 * choice is Func_0808a070(0, 0) at 0x02002f7c; the two arms rejoin at
 * 0x02003034.  Three final record lookups are null checked before their signed
 * coordinate halfwords at +10/+18 are forwarded to Func_0808a0b8.
 *
 * Control-flow-derived data ranges are 0x02002fd0-0x02002fe7 (24 bytes),
 * 0x020033e4-0x020033f3 (16 bytes), and 0x020034b4-0x020034bb (8 bytes).
 * Thus the owner contains 2120 executable bytes and 48 data bytes.
 *
 * All 251 call sites resolve to 28 import veneers under the overlay-specific
 * stored-displacement + 2 rule.  This source preserves every site separately;
 * repeated calls must not be folded because the byte-level call multiset is
 * part of the completeness proof.
 */

void Func_080770c8();
void Func_080f9010();
void Func_0808a010();
void Func_0808a018();
void Func_0808a020();
s32 Func_0808a070();
u8 *Func_0808a080();
void Func_0808a090();
void Func_0808a0b8();
void Func_0808a0d0();
void Func_0808a0e8();
void Func_0808a0f0();
void Func_0808a100();
void Func_0808a110();
void Func_0808a138();
void Func_0808a148();
void Func_0808a150();
void Func_0808a170();
void Func_0808a178();
void Func_0808a180();
void Func_0808a1b8();
void Func_0808a1e8();
void Func_0808a1f0();
void Func_0808a210();
void Func_0808a460();
void Func_0808a4f0();
void Func_0808a570();
void Func_0808a580();

void Func_020068dc();
void Func_02006ac2();
void Func_02006906();
void Func_02006a42();
void Func_020069c0();
void Func_020069a8();
void Func_02006916();
void Func_020069e6();
void Func_02006924();
void Func_02006a26();
void Func_02006968();
void Func_020069a2();
void Func_02006a26_a();
void Func_02006af6();
void Func_02006b04();
void Func_02006b12();
void Func_020069f0();
void Func_0200698e();
void Func_02006a72();
void Func_020069a0();
void Func_02006a70();
void Func_02006a48();
void Func_020069b6();
void Func_02006a86();
void Func_020069c4();
void Func_02006aa8();
void Func_02006aa0();
void Func_020069de();
void Func_02006a90();
void Func_02006ac4();
void Func_02006ad0();
void Func_02006a06();
void Func_02006ad6();
void Func_02006a14();
void Func_02006afe();
void Func_02006abe();
void Func_02006a2c();
void Func_02006afc();
void Func_02006a3a();
void Func_02006aec();
void Func_02006b20();
void Func_02006b2c();
void Func_02006a62();
void Func_02006b32();
void Func_02006a70_a();
void Func_02006b54();
void Func_02006b4c();
void Func_02006a8a();
void Func_02006b12_a();
void Func_02006a98();
void Func_02006b68();
void Func_02006aa6();
void Func_02006b88();
void Func_02006b80();
void Func_02006abe_a();
void Func_02006b68_a();
void Func_02006baa();
void Func_02006ba2();
void Func_02006bb6();
void Func_02006aec_a();
void Func_02006bbc();
void Func_02006afa();
void Func_02006bde();
void Func_02006bd6();
void Func_02006bae();
void Func_02006b1c();
void Func_02006ba4();
void Func_02006b2a();
void Func_02006bfa();
void Func_02006b38();
void Func_02006c1c();
void Func_02006c14();
void Func_02006b52();
void Func_02006c34();
s32 Func_02006c3e();
void Func_02006c48();
void Func_02006c52();
void Func_02006b80_a();
void Func_02006c20();
void Func_02006b8e();
void Func_02006c5e();
void Func_02006b9c();
void Func_02006c6c();
void Func_02006baa_a();
void Func_02006c8e();
void Func_02006c86();
void Func_02006bc4();
void Func_02006c64();
void Func_02006bd2();
void Func_02006ca2();
void Func_02006c7a();
void Func_02006be8();
void Func_02006cb8();
void Func_02006bf6();
void Func_02006cda();
s32 Func_02006cca();
void Func_02006cb4();
void Func_02006cbe();
s32 Func_02006c3e_a();
void Func_02006ce8();
void Func_02006c36();
void Func_02006d1a();
void Func_02006c78();
void Func_02006dfc();
void Func_02006c62();
void Func_02006d14();
void Func_02006d3c();
void Func_02006d4c();
void Func_02006c9a();
void Func_02006cd0();
void Func_02006e54();
void Func_02006cba();
void Func_02006d6c();
void Func_02006d4c_a();
void Func_02006cd2();
void Func_02006da2();
void Func_02006d9a();
void Func_02006dc4();
void Func_02006d22();
void Func_02006ea6();
void Func_02006d0c();
void Func_02006dbe();
void Func_02006de6();
void Func_02006da6();
void Func_02006d2c();
void Func_02006db4();
void Func_02006d3a();
void Func_02006de4();
void Func_02006dee();
void Func_02006df8();
void Func_02006d5e();
void Func_02006e2e();
void Func_02006d6c_a();
void Func_02006da2_a();
void Func_02006f24();
void Func_02006d8a();
void Func_02006e12();
void Func_02006d98();
void Func_02006e42();
void Func_02006da8();
void Func_02006e78();
void Func_02006db6();
void Func_02006e68();
void Func_02006e48();
void Func_02006e50();
void Func_02006dd6();
void Func_02006e0c();
void Func_02006f8e();
void Func_02006df4();
void Func_02006ed0();
void Func_02006edc();
void Func_02006ee8();
void Func_02006ef6();
void Func_02006ee4();
void Func_02006e32();
void Func_02006f16();
void Func_02006f0e();
void Func_02006e4c();
void Func_02006ed4();
void Func_02006e5a();
void Func_02006f2a();
void Func_02006e68_a();
void Func_02006f4c();
void Func_02006f44();
void Func_02006e82();
void Func_02006f0a();
void Func_02006e90();
void Func_02006f60();
void Func_02006e9e();
void Func_02006f26();
void Func_02006eac();
void Func_02006f7c();
void Func_02006eba();
void Func_02006f6c();
void Func_02006fa0();
void Func_02006fac();
void Func_02006ee2();
void Func_02006fb2();
void Func_02006ef0();
void Func_02006fd2();
void Func_02006fca();
void Func_02006f08();
void Func_02006fd8();
void Func_02006f16_a();
void Func_02006fc8();
void Func_02006ffc();
void Func_02007008();
void Func_02006f3e();
void Func_0200700e();
void Func_02006f4c_a();
void Func_02006ffe();
void Func_02007032();
void Func_0200703e();
void Func_02006f74();
void Func_02007044();
void Func_02006f82();
void Func_02007022();
void Func_02006f90();
void Func_02007060();
void Func_02006f9e();
void Func_0200706e();
void Func_02006fac_a();
void Func_0200704c();
void Func_02006fba();
void Func_0200708a();
void Func_02006fc8_a();
void Func_020070ac();
void Func_020070a4();
void Func_02006fe2();
void Func_020070c4();
void Func_020070bc();
void Func_02006ffa();
void Func_020070de();
void Func_020070d6();
void Func_020070ae();
void Func_0200701c();
void Func_02007022_a();
void Func_020070aa();
void Func_02007030();
void Func_02007100();
void Func_0200703e_a();
void Func_020070e8();
void Func_020070d0();
void Func_02007056();
void Func_02007100_a();
void Func_0200710a();
void Func_02007070();
void Func_020070f0();
void Func_02007100_b();
void Func_02007086();
void Func_02007266();
void Func_020070d0_a();
void Func_020070da();
void Func_020070e4();
void Func_0200713c();
u8 * Func_020070ea();
void Func_02007124();
s32 Func_0200714a();
void Func_0200715c();
void Func_0200716c();
u8 * Func_0200711a();
void Func_02007154();
void Func_0200717a();
void Func_0200718c();
void Func_0200719c();
u8 * Func_0200714a_a();
void Func_02007184();
void Func_020071aa();
void Func_020071bc();
void Func_02007298();
void Func_02007164();

/* Resolved engine calls: each pseudo symbol is the per-site call word the
 * overlay image holds (a word can serve two sites with different targets),
 * and the macro names the engine function the site reaches through the
 * overlay veneer and the main-image veneer island, keeping the site's own
 * calling form. Names without a repository binding are provisional.
 */
#define GameFlag_Set_1(a0) Value1(Func_020068dc, a0)
#define Audio_PlayCue_1(a0) Value1(Func_02006ac2, a0)
#define BattleRuntime_Reset_1() Value0(Func_02006906)
#define SceneWork_SetStepValue_1(a0) Call1(Func_020069c0, a0)
#define ObjectMotion_SetVariantCallbackAndInvokeObject_1(a0, a1) Value2(Func_020069a8, a0, a1)
#define BattleRuntime_WaitIfModeZero_1(a0) Value1(Func_02006916, a0)
#define BattleEvent_RunActionAndWait_1(a0, a1) Value2(Func_020069e6, a0, a1)
#define BattleRuntime_WaitIfModeZero_2(a0) Value1(Func_02006924, a0)
#define ObjectMotion_PlaceWithinCameraBounds_1(a0, a1, a2, a3) Call4(Func_02006a26, a0, a1, a2, a3)
#define ObjectMotion_SetSpeedParameters_1(a0, a1, a2) Call3(Func_02006968, a0, a1, a2)
#define ObjectMotion_SetPositionAndReset_1(a0, a1, a2) Call3(Func_020069a2, a0, a1, a2)
#define ObjectMotion_ArmCallback_1(a0, a1, a2) Call3(Func_02006a26, a0, a1, a2)
#define ObjectMotion_RealignToTrackedObjectAndArmCallback_1(a0, a1, a2, a3) Call4(Func_02006af6, a0, a1, a2, a3)
#define ObjectMotion_RealignToTrackedObjectAndArmCallback_2(a0, a1, a2, a3) Call4(Func_02006b04, a0, a1, a2, a3)
#define ObjectMotion_RealignToTrackedObjectAndArmCallback_3(a0, a1, a2, a3) Value4(Func_02006b12, a0, a1, a2, a3)
#define ObjectMotion_CommitCurrentPositionAndActivate_1(a0) Value1(Func_020069f0, a0)
#define BattleRuntime_WaitIfModeZero_3(a0) Value1(Func_0200698e, a0)
#define BattleEffect_SpawnLinkedResourceObject_1(a0, a1, a2) Value3(Func_02006a72, a0, a1, a2)
#define BattleRuntime_WaitIfModeZero_4(a0) Value1(Func_020069a0, a0)
#define BattleEvent_RunActionAndWait_2(a0, a1) Call2(Func_02006a70, a0, a1)
#define ObjectMotion_SetVariantCallbackAndInvokeObject_2(a0, a1) Value2(Func_02006a48, a0, a1)
#define BattleRuntime_WaitIfModeZero_5(a0) Value1(Func_020069b6, a0)
#define BattleEvent_RunActionAndWait_3(a0, a1) Value2(Func_02006a86, a0, a1)
#define BattleRuntime_WaitIfModeZero_6(a0) Value1(Func_020069c4, a0)
#define BattleEffect_SpawnLinkedResourceObject_2(a0, a1, a2) Call3(Func_02006aa8, a0, a1, a2)
#define BattleEvent_RunActionAndWait_4(a0, a1) Value2(Func_02006aa0, a0, a1)
#define BattleRuntime_WaitIfModeZero_7(a0) Value1(Func_020069de, a0)
#define Object_LinkPair_1(a0, a1, a2) Call3(Func_02006a90, a0, a1, a2)
#define ObjectMotion_ArmCallback_2(a0, a1, a2) Call3(Func_02006ac4, a0, a1, a2)
#define ObjectMotion_ArmCallback_3(a0, a1, a2) Value3(Func_02006ad0, a0, a1, a2)
#define BattleRuntime_WaitIfModeZero_8(a0) Value1(Func_02006a06, a0)
#define BattleEvent_RunActionAndWait_5(a0, a1) Value2(Func_02006ad6, a0, a1)
#define BattleRuntime_WaitIfModeZero_9(a0) Value1(Func_02006a14, a0)
#define BattleRuntime_WaitIfModeZero_10(a0, a1) Call2(Func_02006afe, a0, a1)
#define ObjectMotion_SetVariantCallbackAndInvokeObject_3(a0, a1) Value2(Func_02006abe, a0, a1)
#define BattleRuntime_WaitIfModeZero_11(a0) Value1(Func_02006a2c, a0)
#define BattleEvent_RunActionAndWait_6(a0, a1) Value2(Func_02006afc, a0, a1)
#define BattleRuntime_WaitIfModeZero_12(a0) Value1(Func_02006a3a, a0)
#define Object_LinkPair_2(a0, a1, a2) Call3(Func_02006aec, a0, a1, a2)
#define ObjectMotion_ArmCallback_4(a0, a1, a2) Call3(Func_02006b20, a0, a1, a2)
#define ObjectMotion_ArmCallback_5(a0, a1, a2) Value3(Func_02006b2c, a0, a1, a2)
#define BattleRuntime_WaitIfModeZero_13(a0) Value1(Func_02006a62, a0)
#define BattleEvent_RunActionAndWait_7(a0, a1) Value2(Func_02006b32, a0, a1)
#define BattleRuntime_WaitIfModeZero_14(a0) Call1(Func_02006a70, a0)
#define BattleEffect_SpawnLinkedResourceObject_3(a0, a1, a2) Call3(Func_02006b54, a0, a1, a2)
#define BattleEvent_RunActionAndWait_8(a0, a1) Value2(Func_02006b4c, a0, a1)
#define BattleRuntime_WaitIfModeZero_15(a0) Value1(Func_02006a8a, a0)
#define ObjectMotion_CallThenWaitForAnimationChange_1(a0, a1) Call2(Func_02006b12, a0, a1)
#define BattleRuntime_WaitIfModeZero_16(a0) Value1(Func_02006a98, a0)
#define BattleEvent_RunActionAndWait_9(a0, a1) Value2(Func_02006b68, a0, a1)
#define BattleRuntime_WaitIfModeZero_17(a0) Value1(Func_02006aa6, a0)
#define BattleEffect_SpawnLinkedResourceObject_4(a0, a1, a2) Call3(Func_02006b88, a0, a1, a2)
#define BattleEvent_RunActionAndWait_10(a0, a1) Value2(Func_02006b80, a0, a1)
#define BattleRuntime_WaitIfModeZero_18(a0) Call1(Func_02006abe, a0)
#define ObjectMotion_SetAngleToward_1(a0, a1, a2) Call3(Func_02006b68, a0, a1, a2)
#define BattleEffect_SpawnLinkedResourceObject_5(a0, a1, a2) Call3(Func_02006baa, a0, a1, a2)
#define BattleEvent_RunActionAndWait_11(a0, a1) Call2(Func_02006ba2, a0, a1)
#define ObjectMotion_ArmCallback_6(a0, a1, a2) Value3(Func_02006bb6, a0, a1, a2)
#define BattleRuntime_WaitIfModeZero_19(a0) Call1(Func_02006aec, a0)
#define BattleEvent_RunActionAndWait_12(a0, a1) Value2(Func_02006bbc, a0, a1)
#define BattleRuntime_WaitIfModeZero_20(a0) Value1(Func_02006afa, a0)
#define BattleEffect_SpawnLinkedResourceObject_6(a0, a1, a2) Call3(Func_02006bde, a0, a1, a2)
#define BattleEvent_RunActionAndWait_13(a0, a1) Call2(Func_02006bd6, a0, a1)
#define ObjectMotion_SetVariantCallbackAndInvokeObject_4(a0, a1) Value2(Func_02006bae, a0, a1)
#define BattleRuntime_WaitIfModeZero_21(a0) Value1(Func_02006b1c, a0)
#define ObjectMotion_CallThenWaitForAnimationChange_2(a0, a1) Value2(Func_02006ba4, a0, a1)
#define BattleRuntime_WaitIfModeZero_22(a0) Value1(Func_02006b2a, a0)
#define BattleEvent_RunActionAndWait_14(a0, a1) Value2(Func_02006bfa, a0, a1)
#define BattleRuntime_WaitIfModeZero_23(a0) Value1(Func_02006b38, a0)
#define BattleEffect_SpawnLinkedResourceObject_7(a0, a1, a2) Call3(Func_02006c1c, a0, a1, a2)
#define BattleEvent_RunActionAndWait_15(a0, a1) Value2(Func_02006c14, a0, a1)
#define BattleRuntime_WaitIfModeZero_24(a0) Value1(Func_02006b52, a0)
#define BattleEffect_SpawnLinkedResourceObject_8(a0, a1, a2) Call3(Func_02006c34, a0, a1, a2)
#define BattleEffect_SpawnLinkedResourceObject_9(a0, a1, a2) Call3(Func_02006c3e, a0, a1, a2)
#define BattleEffect_SpawnLinkedResourceObject_10(a0, a1, a2) Call3(Func_02006c48, a0, a1, a2)
#define BattleEffect_SpawnLinkedResourceObject_11(a0, a1, a2) Value3(Func_02006c52, a0, a1, a2)
#define BattleRuntime_WaitIfModeZero_25(a0) Call1(Func_02006b80, a0)
#define ObjectMotion_SetVariantCallbackAndInvokeObject_5(a0, a1) Value2(Func_02006c20, a0, a1)
#define BattleRuntime_WaitIfModeZero_26(a0) Value1(Func_02006b8e, a0)
#define BattleEvent_RunActionAndWait_16(a0, a1) Value2(Func_02006c5e, a0, a1)
#define BattleRuntime_WaitIfModeZero_27(a0) Value1(Func_02006b9c, a0)
#define BattleEvent_RunActionAndWait_17(a0, a1) Value2(Func_02006c6c, a0, a1)
#define BattleRuntime_WaitIfModeZero_28(a0) Call1(Func_02006baa, a0)
#define BattleEffect_SpawnLinkedResourceObject_12(a0, a1, a2) Call3(Func_02006c8e, a0, a1, a2)
#define BattleEvent_RunActionAndWait_18(a0, a1) Value2(Func_02006c86, a0, a1)
#define BattleRuntime_WaitIfModeZero_29(a0) Value1(Func_02006bc4, a0)
#define ObjectMotion_SetVariantCallbackAndInvokeObject_6(a0, a1) Value2(Func_02006c64, a0, a1)
#define BattleRuntime_WaitIfModeZero_30(a0) Value1(Func_02006bd2, a0)
#define BattleEvent_RunActionAndWait_19(a0, a1) Call2(Func_02006ca2, a0, a1)
#define ObjectMotion_SetVariantCallbackAndInvokeObject_7(a0, a1) Value2(Func_02006c7a, a0, a1)
#define BattleRuntime_WaitIfModeZero_31(a0) Value1(Func_02006be8, a0)
#define BattleEvent_RunActionAndWait_20(a0, a1) Value2(Func_02006cb8, a0, a1)
#define BattleRuntime_WaitIfModeZero_32(a0) Value1(Func_02006bf6, a0)
#define BattleEffect_SpawnLinkedResourceObject_13(a0, a1, a2) Call3(Func_02006cda, a0, a1, a2)
#define ObjectMotion_SetSpeedParameters_2(a0, a1) Value2(Func_02006cca, a0, a1)
#define ObjectMotion_SetAngleToward_2(a0, a1, a2) Call3(Func_02006cb4, a0, a1, a2)
#define ObjectMotion_SetAngleToward_3(a0, a1, a2) Call3(Func_02006cbe, a0, a1, a2)
#define UiWork_WaitThenFinalizeCapacity_1(a0, a1) Value2(Func_02006c3e, a0, a1)
#define SceneWork_SetStepValue_2(a0) Value1(Func_02006ce8, a0)
#define BattleRuntime_WaitIfModeZero_33(a0) Value1(Func_02006c36, a0)
#define BattleEffect_SpawnLinkedResourceObject_14(a0, a1, a2) Call3(Func_02006d1a, a0, a1, a2)
#define ObjectMotion_SetSpeedParameters_3(a0, a1, a2) Call3(Func_02006c78, a0, a1, a2)
#define ObjectMotion_CommitPositionAndActivate_1(a0, a1, a2) Value3(Func_02006dfc, a0, a1, a2)
#define BattleRuntime_WaitIfModeZero_34(a0) Value1(Func_02006c62, a0)
#define Object_LinkPair_3(a0, a1, a2) Call3(Func_02006d14, a0, a1, a2)
#define BattleEvent_RunActionAndWait_21(a0, a1) Value2(Func_02006d3c, a0, a1)
#define SceneWork_SetStepValue_3(a0) Value1(Func_02006d4c, a0)
#define BattleRuntime_WaitIfModeZero_35(a0) Value1(Func_02006c9a, a0)
#define ObjectMotion_SetSpeedParameters_4(a0, a1, a2) Call3(Func_02006cd0, a0, a1, a2)
#define ObjectMotion_CommitPositionAndActivate_2(a0, a1, a2) Value3(Func_02006e54, a0, a1, a2)
#define BattleRuntime_WaitIfModeZero_36(a0) Value1(Func_02006cba, a0)
#define Object_LinkPair_4(a0, a1, a2) Call3(Func_02006d6c, a0, a1, a2)
#define ObjectMotion_CallThenWaitForAnimationChange_3(a0, a1) Call2(Func_02006d4c, a0, a1)
#define BattleRuntime_WaitIfModeZero_37(a0) Value1(Func_02006cd2, a0)
#define BattleEvent_RunActionAndWait_22(a0, a1) Value2(Func_02006da2, a0, a1)
#define SceneWork_SetStepValue_4(a0) Value1(Func_02006d9a, a0)
#define BattleEffect_SpawnLinkedResourceObject_15(a0, a1, a2) Call3(Func_02006dc4, a0, a1, a2)
#define ObjectMotion_SetSpeedParameters_5(a0, a1, a2) Call3(Func_02006d22, a0, a1, a2)
#define ObjectMotion_CommitPositionAndActivate_3(a0, a1, a2) Value3(Func_02006ea6, a0, a1, a2)
#define BattleRuntime_WaitIfModeZero_38(a0) Value1(Func_02006d0c, a0)
#define Object_LinkPair_5(a0, a1, a2) Call3(Func_02006dbe, a0, a1, a2)
#define BattleEvent_RunActionAndWait_23(a0, a1) Call2(Func_02006de6, a0, a1)
#define ObjectMotion_CallThenWaitForAnimationChange_4(a0, a1) Value2(Func_02006da6, a0, a1)
#define BattleRuntime_WaitIfModeZero_39(a0) Value1(Func_02006d2c, a0)
#define ObjectMotion_CallThenWaitForAnimationChange_5(a0, a1) Value2(Func_02006db4, a0, a1)
#define BattleRuntime_WaitIfModeZero_40(a0) Value1(Func_02006d3a, a0)
#define ObjectMotion_SetAngleToward_4(a0, a1, a2) Call3(Func_02006de4, a0, a1, a2)
#define ObjectMotion_SetAngleToward_5(a0, a1, a2) Call3(Func_02006dee, a0, a1, a2)
#define ObjectMotion_SetAngleToward_6(a0, a1, a2) Value3(Func_02006df8, a0, a1, a2)
#define BattleRuntime_WaitIfModeZero_41(a0) Value1(Func_02006d5e, a0)
#define BattleEvent_RunActionAndWait_24(a0, a1) Value2(Func_02006e2e, a0, a1)
#define BattleRuntime_WaitIfModeZero_42(a0) Call1(Func_02006d6c, a0)
#define ObjectMotion_SetSpeedParameters_6(a0, a1, a2) Call3(Func_02006da2, a0, a1, a2)
#define ObjectMotion_CommitPositionAndActivate_4(a0, a1, a2) Value3(Func_02006f24, a0, a1, a2)
#define BattleRuntime_WaitIfModeZero_43(a0) Value1(Func_02006d8a, a0)
#define ObjectMotion_CallThenWaitForAnimationChange_6(a0, a1) Value2(Func_02006e12, a0, a1)
#define BattleRuntime_WaitIfModeZero_44(a0) Value1(Func_02006d98, a0)
#define ObjectMotion_SetAngleToward_7(a0, a1, a2) Value3(Func_02006e42, a0, a1, a2)
#define BattleRuntime_WaitIfModeZero_45(a0) Value1(Func_02006da8, a0)
#define BattleEvent_RunActionAndWait_25(a0, a1) Value2(Func_02006e78, a0, a1)
#define BattleRuntime_WaitIfModeZero_46(a0) Value1(Func_02006db6, a0)
#define Object_LinkPair_6(a0, a1, a2) Call3(Func_02006e68, a0, a1, a2)
#define ObjectMotion_CallThenWaitForAnimationChange_7(a0, a1) Call2(Func_02006e48, a0, a1)
#define ObjectMotion_CallThenWaitForAnimationChange_8(a0, a1) Call2(Func_02006e50, a0, a1)
#define BattleRuntime_WaitIfModeZero_47(a0) Value1(Func_02006dd6, a0)
#define ObjectMotion_SetSpeedParameters_7(a0, a1, a2) Call3(Func_02006e0c, a0, a1, a2)
#define ObjectMotion_CommitPositionAndActivate_5(a0, a1, a2) Value3(Func_02006f8e, a0, a1, a2)
#define BattleRuntime_WaitIfModeZero_48(a0) Value1(Func_02006df4, a0)
#define ObjectMotion_ArmCallback_7(a0, a1, a2) Call3(Func_02006ed0, a0, a1, a2)
#define ObjectMotion_ArmCallback_8(a0, a1, a2) Call3(Func_02006edc, a0, a1, a2)
#define ObjectMotion_ArmCallback_9(a0, a1, a2) Call3(Func_02006ee8, a0, a1, a2)
#define ObjectMotion_ArmCallback_10(a0, a1, a2) Value3(Func_02006ef6, a0, a1, a2)
#define SceneWork_SetStepValue_5(a0) Value1(Func_02006ee4, a0)
#define BattleRuntime_WaitIfModeZero_49(a0) Value1(Func_02006e32, a0)
#define BattleEffect_SpawnLinkedResourceObject_16(a0, a1, a2) Call3(Func_02006f16, a0, a1, a2)
#define BattleEvent_RunActionAndWait_26(a0, a1) Value2(Func_02006f0e, a0, a1)
#define BattleRuntime_WaitIfModeZero_50(a0) Value1(Func_02006e4c, a0)
#define ObjectMotion_CallThenWaitForAnimationChange_9(a0, a1) Value2(Func_02006ed4, a0, a1)
#define BattleRuntime_WaitIfModeZero_51(a0) Value1(Func_02006e5a, a0)
#define BattleEvent_RunActionAndWait_27(a0, a1) Value2(Func_02006f2a, a0, a1)
#define BattleRuntime_WaitIfModeZero_52(a0) Call1(Func_02006e68, a0)
#define BattleEffect_SpawnLinkedResourceObject_17(a0, a1, a2) Call3(Func_02006f4c, a0, a1, a2)
#define BattleEvent_RunActionAndWait_28(a0, a1) Value2(Func_02006f44, a0, a1)
#define BattleRuntime_WaitIfModeZero_53(a0) Value1(Func_02006e82, a0)
#define ObjectMotion_CallThenWaitForAnimationChange_10(a0, a1) Value2(Func_02006f0a, a0, a1)
#define BattleRuntime_WaitIfModeZero_54(a0) Value1(Func_02006e90, a0)
#define BattleEvent_RunActionAndWait_29(a0, a1) Value2(Func_02006f60, a0, a1)
#define BattleRuntime_WaitIfModeZero_55(a0) Value1(Func_02006e9e, a0)
#define ObjectMotion_CallThenWaitForAnimationChange_11(a0, a1) Value2(Func_02006f26, a0, a1)
#define BattleRuntime_WaitIfModeZero_56(a0) Value1(Func_02006eac, a0)
#define BattleEvent_RunActionAndWait_30(a0, a1) Value2(Func_02006f7c, a0, a1)
#define BattleRuntime_WaitIfModeZero_57(a0) Value1(Func_02006eba, a0)
#define Object_LinkPair_7(a0, a1, a2) Call3(Func_02006f6c, a0, a1, a2)
#define ObjectMotion_ArmCallback_11(a0, a1, a2) Call3(Func_02006fa0, a0, a1, a2)
#define ObjectMotion_ArmCallback_12(a0, a1, a2) Value3(Func_02006fac, a0, a1, a2)
#define BattleRuntime_WaitIfModeZero_58(a0) Value1(Func_02006ee2, a0)
#define BattleEvent_RunActionAndWait_31(a0, a1) Value2(Func_02006fb2, a0, a1)
#define BattleRuntime_WaitIfModeZero_59(a0) Value1(Func_02006ef0, a0)
#define BattleEffect_SpawnLinkedResourceObject_18(a0, a1, a2) Call3(Func_02006fd2, a0, a1, a2)
#define BattleEvent_RunActionAndWait_32(a0, a1) Value2(Func_02006fca, a0, a1)
#define BattleRuntime_WaitIfModeZero_60(a0) Value1(Func_02006f08, a0)
#define BattleEvent_RunActionAndWait_33(a0, a1) Value2(Func_02006fd8, a0, a1)
#define BattleRuntime_WaitIfModeZero_61(a0) Call1(Func_02006f16, a0)
#define Object_LinkPair_8(a0, a1, a2) Call3(Func_02006fc8, a0, a1, a2)
#define ObjectMotion_ArmCallback_13(a0, a1, a2) Call3(Func_02006ffc, a0, a1, a2)
#define ObjectMotion_ArmCallback_14(a0, a1, a2) Value3(Func_02007008, a0, a1, a2)
#define BattleRuntime_WaitIfModeZero_62(a0) Value1(Func_02006f3e, a0)
#define BattleEvent_RunActionAndWait_34(a0, a1) Value2(Func_0200700e, a0, a1)
#define BattleRuntime_WaitIfModeZero_63(a0) Call1(Func_02006f4c, a0)
#define Object_LinkPair_9(a0, a1, a2) Call3(Func_02006ffe, a0, a1, a2)
#define ObjectMotion_ArmCallback_15(a0, a1, a2) Call3(Func_02007032, a0, a1, a2)
#define ObjectMotion_ArmCallback_16(a0, a1, a2) Value3(Func_0200703e, a0, a1, a2)
#define BattleRuntime_WaitIfModeZero_64(a0) Value1(Func_02006f74, a0)
#define BattleEvent_RunActionAndWait_35(a0, a1) Value2(Func_02007044, a0, a1)
#define BattleRuntime_WaitIfModeZero_65(a0) Value1(Func_02006f82, a0)
#define ObjectMotion_SetVariantCallbackAndInvokeObject_8(a0, a1) Value2(Func_02007022, a0, a1)
#define BattleRuntime_WaitIfModeZero_66(a0) Value1(Func_02006f90, a0)
#define BattleEvent_RunActionAndWait_36(a0, a1) Value2(Func_02007060, a0, a1)
#define BattleRuntime_WaitIfModeZero_67(a0) Value1(Func_02006f9e, a0)
#define BattleEvent_RunActionAndWait_37(a0, a1) Value2(Func_0200706e, a0, a1)
#define BattleRuntime_WaitIfModeZero_68(a0) Call1(Func_02006fac, a0)
#define ObjectMotion_SetVariantCallbackAndInvokeObject_9(a0, a1) Value2(Func_0200704c, a0, a1)
#define BattleRuntime_WaitIfModeZero_69(a0) Value1(Func_02006fba, a0)
#define BattleEvent_RunActionAndWait_38(a0, a1) Value2(Func_0200708a, a0, a1)
#define BattleRuntime_WaitIfModeZero_70(a0) Call1(Func_02006fc8, a0)
#define BattleEffect_SpawnLinkedResourceObject_19(a0, a1, a2) Call3(Func_020070ac, a0, a1, a2)
#define BattleEvent_RunActionAndWait_39(a0, a1) Value2(Func_020070a4, a0, a1)
#define BattleRuntime_WaitIfModeZero_71(a0) Value1(Func_02006fe2, a0)
#define BattleEffect_SpawnLinkedResourceObject_20(a0, a1, a2) Call3(Func_020070c4, a0, a1, a2)
#define BattleEvent_RunActionAndWait_40(a0, a1) Value2(Func_020070bc, a0, a1)
#define BattleRuntime_WaitIfModeZero_72(a0) Value1(Func_02006ffa, a0)
#define BattleEffect_SpawnLinkedResourceObject_21(a0, a1, a2) Call3(Func_020070de, a0, a1, a2)
#define BattleEvent_RunActionAndWait_41(a0, a1) Call2(Func_020070d6, a0, a1)
#define ObjectMotion_SetVariantCallbackAndInvokeObject_10(a0, a1) Value2(Func_020070ae, a0, a1)
#define BattleRuntime_WaitIfModeZero_73(a0) Value1(Func_0200701c, a0)
#define BattleRuntime_WaitIfModeZero_74(a0) Call1(Func_02007022, a0)
#define ObjectMotion_CallThenWaitForAnimationChange_12(a0, a1) Value2(Func_020070aa, a0, a1)
#define BattleRuntime_WaitIfModeZero_75(a0) Value1(Func_02007030, a0)
#define BattleEvent_RunActionAndWait_42(a0, a1) Value2(Func_02007100, a0, a1)
#define BattleRuntime_WaitIfModeZero_76(a0) Call1(Func_0200703e, a0)
#define ObjectMotion_SetAngleToward_8(a0, a1, a2) Call3(Func_020070e8, a0, a1, a2)
#define ObjectMotion_CallThenWaitForAnimationChange_13(a0, a1) Value2(Func_020070d0, a0, a1)
#define BattleRuntime_WaitIfModeZero_77(a0) Value1(Func_02007056, a0)
#define ObjectMotion_SetAngleToward_9(a0, a1, a2) Call3(Func_02007100, a0, a1, a2)
#define ObjectMotion_SetAngleToward_10(a0, a1, a2) Value3(Func_0200710a, a0, a1, a2)
#define BattleRuntime_WaitIfModeZero_78(a0) Value1(Func_02007070, a0)
#define Object_SetModeById_1(a0, a1) Call2(Func_020070f0, a0, a1)
#define ObjectMotion_CallThenWaitForAnimationChange_14(a0, a1) Call2(Func_02007100, a0, a1)
#define BattleRuntime_WaitIfModeZero_79(a0) Value1(Func_02007086, a0)
#define Audio_PlayCue_2(a0) Value1(Func_02007266, a0)
#define ObjectMotion_SetSpeedParameters_8(a0, a1, a2) Call3(Func_020070d0, a0, a1, a2)
#define ObjectMotion_SetSpeedParameters_9(a0, a1, a2) Call3(Func_020070da, a0, a1, a2)
#define ObjectMotion_SetSpeedParameters_10(a0, a1, a2) Call3(Func_020070e4, a0, a1, a2)
#define Object_SetModeById_2(a0, a1) Value2(Func_0200713c, a0, a1)
#define Scene_GetRecord_1(a0) Value1(Func_020070ea, a0)
#define ObjectMotion_ResetAndSetPosition_1(a0, a1, a2) Value3(Func_02007124, a0, a1, a2)
#define ObjectMotion_CommitCurrentPositionAndActivate_2(a0) Value1(Func_0200714a, a0)
#define ObjectMotion_SetHorizontalPositionWithTerrain_1(a0, a1, a2) Call3(Func_0200715c, a0, a1, a2)
#define Object_SetModeById_3(a0, a1) Value2(Func_0200716c, a0, a1)
#define Scene_GetRecord_2(a0) Value1(Func_0200711a, a0)
#define ObjectMotion_ResetAndSetPosition_2(a0, a1, a2) Value3(Func_02007154, a0, a1, a2)
#define ObjectMotion_CommitCurrentPositionAndActivate_3(a0) Value1(Func_0200717a, a0)
#define ObjectMotion_SetHorizontalPositionWithTerrain_2(a0, a1, a2) Call3(Func_0200718c, a0, a1, a2)
#define Object_SetModeById_4(a0, a1) Value2(Func_0200719c, a0, a1)
#define Scene_GetRecord_3(a0) Value1(Func_0200714a, a0)
#define ObjectMotion_ResetAndSetPosition_3(a0, a1, a2) Value3(Func_02007184, a0, a1, a2)
#define ObjectMotion_CommitCurrentPositionAndActivate_4(a0) Value1(Func_020071aa, a0)
#define ObjectMotion_SetHorizontalPositionWithTerrain_3(a0, a1, a2) Call3(Func_020071bc, a0, a1, a2)
#define Audio_PlayCueForPartyMember_1() Call0(Func_02007298)
#define BattleRuntime_ScheduleShoulderButtonModeUpdate_1() Call0(Func_02007164)

/* Newly identified engine calls: named from the target's own reconstructed
 * source (see the trailing provisional comment on each), keeping the site's
 * existing calling form. */

/* Loader-relocated ROM calls: each site names the pre-relocation call word the image holds. */


static __inline__ void Call1(void (*f)(), s32 a0)
{
    f(a0);
}

static __inline__ void Call2(void (*f)(), s32 a0, s32 a1)
{
    f(a0, a1);
}

static __inline__ void Call3(void (*f)(), s32 a0, s32 a1, s32 a2)
{
    f(a0, a1, a2);
}

static __inline__ void Call4(void (*f)(), s32 a0, s32 a1, s32 a2, s32 a3)
{
    f(a0, a1, a2, a3);
}

static __inline__ void Call0(void (*f)())
{
    f();
}

static __inline__ s32 Value0(s32 (*f)())
{
    return f();
}

static __inline__ s32 Value1(s32 (*f)(), s32 a0)
{
    return f(a0);
}

static __inline__ s32 Value2(s32 (*f)(), s32 a0, s32 a1)
{
    return f(a0, a1);
}

static __inline__ s32 Value3(s32 (*f)(), s32 a0, s32 a1, s32 a2)
{
    return f(a0, a1, a2);
}

static __inline__ s32 Value4(s32 (*f)(), s32 a0, s32 a1, s32 a2, s32 a3)
{
    return f(a0, a1, a2, a3);
}

/* Signed coordinate halfwords within a placed-actor record (see header). */
#define RECORD_COORD_X_OFFSET 10
#define RECORD_COORD_Y_OFFSET 18

void Func_02002c44(void)
{
    u8 *record;
    s32 line;
    GameFlag_Set_1(0x960);
    Audio_PlayCue_1(24);
    BattleRuntime_Reset_1();
    Value0(Func_02006a42); /* main:0808a460 */
    SceneWork_SetStepValue_1(0x214f);
    ObjectMotion_SetVariantCallbackAndInvokeObject_1(8, 2);
    BattleRuntime_WaitIfModeZero_1(20);
    BattleEvent_RunActionAndWait_1(8, 0);
    BattleRuntime_WaitIfModeZero_2(10);
    ObjectMotion_PlaceWithinCameraBounds_1(0xf80000, -1, 0xb80000, 1);
    ObjectMotion_SetSpeedParameters_1(0, 0xcccc, 0x6666);
    ObjectMotion_SetPositionAndReset_1(0, 248, 192);
    ObjectMotion_ArmCallback_1(0, 0xc000, 0);
    ObjectMotion_RealignToTrackedObjectAndArmCallback_1(1, -16, 16, 0xc000);
    ObjectMotion_RealignToTrackedObjectAndArmCallback_2(3, 0, 16, 0xc000);
    ObjectMotion_RealignToTrackedObjectAndArmCallback_3(2, 16, 16, 0xc000);
    ObjectMotion_CommitCurrentPositionAndActivate_1(1);
    BattleRuntime_WaitIfModeZero_3(20);
    BattleEffect_SpawnLinkedResourceObject_1(2, 0x102, 0);
    BattleRuntime_WaitIfModeZero_4(40);
    BattleEvent_RunActionAndWait_2(2, 0);
    ObjectMotion_SetVariantCallbackAndInvokeObject_2(3, 2);
    BattleRuntime_WaitIfModeZero_5(20);
    BattleEvent_RunActionAndWait_3(3, 0);
    BattleRuntime_WaitIfModeZero_6(10);
    BattleEffect_SpawnLinkedResourceObject_2(8, 0x100, 40);
    BattleEvent_RunActionAndWait_4(8, 0);
    BattleRuntime_WaitIfModeZero_7(10);
    Object_LinkPair_1(1, 2, 50);
    ObjectMotion_ArmCallback_2(1, 0xc000, 0);
    ObjectMotion_ArmCallback_3(2, 0xc000, 0);
    BattleRuntime_WaitIfModeZero_8(30);
    BattleEvent_RunActionAndWait_5(1, 0);
    BattleRuntime_WaitIfModeZero_9(10);
    BattleRuntime_WaitIfModeZero_10(8, 0x102); /* main:0808a1f0 */
    ObjectMotion_SetVariantCallbackAndInvokeObject_3(8, 2);
    BattleRuntime_WaitIfModeZero_11(30);
    BattleEvent_RunActionAndWait_6(8, 0);
    BattleRuntime_WaitIfModeZero_12(20);
    Object_LinkPair_2(3, 2, 50);
    ObjectMotion_ArmCallback_4(3, 0xc000, 0);
    ObjectMotion_ArmCallback_5(2, 0xc000, 0);
    BattleRuntime_WaitIfModeZero_13(30);
    BattleEvent_RunActionAndWait_7(2, 0);
    BattleRuntime_WaitIfModeZero_14(10);
    BattleEffect_SpawnLinkedResourceObject_3(8, 0x100, 40);
    BattleEvent_RunActionAndWait_8(8, 0);
    BattleRuntime_WaitIfModeZero_15(10);
    ObjectMotion_CallThenWaitForAnimationChange_1(3, 4);
    BattleRuntime_WaitIfModeZero_16(20);
    BattleEvent_RunActionAndWait_9(3, 0);
    BattleRuntime_WaitIfModeZero_17(10);
    BattleEffect_SpawnLinkedResourceObject_4(8, 0x105, 40);
    BattleEvent_RunActionAndWait_10(8, 0);
    BattleRuntime_WaitIfModeZero_18(10);
    ObjectMotion_SetAngleToward_1(1, 0, 30);
    BattleEffect_SpawnLinkedResourceObject_5(1, 0x101, 40);
    BattleEvent_RunActionAndWait_11(1, 0);
    ObjectMotion_ArmCallback_6(1, 0xc000, 0);
    BattleRuntime_WaitIfModeZero_19(20);
    BattleEvent_RunActionAndWait_12(1, 0);
    BattleRuntime_WaitIfModeZero_20(10);
    BattleEffect_SpawnLinkedResourceObject_6(8, 0x102, 40);
    BattleEvent_RunActionAndWait_13(8, 0);
    ObjectMotion_SetVariantCallbackAndInvokeObject_4(8, 2);
    BattleRuntime_WaitIfModeZero_21(20);
    ObjectMotion_CallThenWaitForAnimationChange_2(3, 4);
    BattleRuntime_WaitIfModeZero_22(20);
    BattleEvent_RunActionAndWait_14(3, 0);
    BattleRuntime_WaitIfModeZero_23(30);
    BattleEffect_SpawnLinkedResourceObject_7(8, 0x106, 40);
    BattleEvent_RunActionAndWait_15(8, 0);
    BattleRuntime_WaitIfModeZero_24(10);
    BattleEffect_SpawnLinkedResourceObject_8(0, 0x101, 0);
    BattleEffect_SpawnLinkedResourceObject_9(1, 0x101, 0);
    BattleEffect_SpawnLinkedResourceObject_10(3, 0x101, 0);
    BattleEffect_SpawnLinkedResourceObject_11(2, 0x101, 0);
    BattleRuntime_WaitIfModeZero_25(60);
    ObjectMotion_SetVariantCallbackAndInvokeObject_5(2, 2);
    BattleRuntime_WaitIfModeZero_26(20);
    BattleEvent_RunActionAndWait_16(2, 0);
    BattleRuntime_WaitIfModeZero_27(20);
    BattleEvent_RunActionAndWait_17(8, 0);
    BattleRuntime_WaitIfModeZero_28(10);
    BattleEffect_SpawnLinkedResourceObject_12(1, 0x102, 40);
    BattleEvent_RunActionAndWait_18(1, 0);
    BattleRuntime_WaitIfModeZero_29(10);
    ObjectMotion_SetVariantCallbackAndInvokeObject_6(8, 2);
    BattleRuntime_WaitIfModeZero_30(20);
    BattleEvent_RunActionAndWait_19(8, 0);
    ObjectMotion_SetVariantCallbackAndInvokeObject_7(3, 2);
    BattleRuntime_WaitIfModeZero_31(20);
    BattleEvent_RunActionAndWait_20(3, 0);
    BattleRuntime_WaitIfModeZero_32(10);
    BattleEffect_SpawnLinkedResourceObject_13(8, 0x102, 40);
    ObjectMotion_SetSpeedParameters_2(8, 0); /* main:0808a178 */
    ObjectMotion_SetAngleToward_2(1, 0, 0);
    ObjectMotion_SetAngleToward_3(2, 0, 0);
    if (UiWork_WaitThenFinalizeCapacity_1(0, 0) == 0) {
        SceneWork_SetStepValue_2(0x2164);
        BattleRuntime_WaitIfModeZero_33(20);
        BattleEffect_SpawnLinkedResourceObject_14(1, 0x100, 40);
        ObjectMotion_SetSpeedParameters_3(1, 0x20000, 0x10000);
        ObjectMotion_CommitPositionAndActivate_1(1, 0, -16);
        BattleRuntime_WaitIfModeZero_34(10);
        Object_LinkPair_3(1, 0, 30);
        BattleEvent_RunActionAndWait_21(1, 0);
    } else {
        SceneWork_SetStepValue_3(0x2168);
        BattleRuntime_WaitIfModeZero_35(10);
        ObjectMotion_SetSpeedParameters_4(1, 0x10000, 0x8000);
        ObjectMotion_CommitPositionAndActivate_2(1, 0, -16);
        BattleRuntime_WaitIfModeZero_36(10);
        Object_LinkPair_4(1, 0, 30);
        ObjectMotion_CallThenWaitForAnimationChange_3(1, 3);
        BattleRuntime_WaitIfModeZero_37(20);
        BattleEvent_RunActionAndWait_22(1, 0);
    }

    line = 0x2165;
    SceneWork_SetStepValue_4(line);
    BattleEffect_SpawnLinkedResourceObject_15(2, 0x103, 40);
    ObjectMotion_SetSpeedParameters_5(2, 0x20000, 0x10000);
    ObjectMotion_CommitPositionAndActivate_3(2, 0, -16);
    BattleRuntime_WaitIfModeZero_38(10);
    Object_LinkPair_5(2, 0, 30);
    BattleEvent_RunActionAndWait_23(2, 0);
    ObjectMotion_CallThenWaitForAnimationChange_4(3, 3);
    BattleRuntime_WaitIfModeZero_39(10);
    ObjectMotion_CallThenWaitForAnimationChange_5(3, 3);
    BattleRuntime_WaitIfModeZero_40(20);
    ObjectMotion_SetAngleToward_4(0, 3, 0);
    ObjectMotion_SetAngleToward_5(1, 3, 0);
    ObjectMotion_SetAngleToward_6(2, 3, 0);
    BattleRuntime_WaitIfModeZero_41(20);
    BattleEvent_RunActionAndWait_24(3, 0);
    BattleRuntime_WaitIfModeZero_42(20);
    ObjectMotion_SetSpeedParameters_6(1, 0x10000, 0x8000);
    ObjectMotion_CommitPositionAndActivate_4(1, 0, 16);
    BattleRuntime_WaitIfModeZero_43(30);
    ObjectMotion_CallThenWaitForAnimationChange_6(1, 4);
    BattleRuntime_WaitIfModeZero_44(30);
    ObjectMotion_SetAngleToward_7(1, 0, 0);
    BattleRuntime_WaitIfModeZero_45(20);
    BattleEvent_RunActionAndWait_25(1, 0);
    BattleRuntime_WaitIfModeZero_46(30);
    Object_LinkPair_6(3, 2, 30);
    ObjectMotion_CallThenWaitForAnimationChange_7(2, 3);
    ObjectMotion_CallThenWaitForAnimationChange_8(3, 3);
    BattleRuntime_WaitIfModeZero_47(30);
    ObjectMotion_SetSpeedParameters_7(2, 0x10000, 0x8000);
    ObjectMotion_CommitPositionAndActivate_5(2, 0, 16);
    BattleRuntime_WaitIfModeZero_48(10);
    ObjectMotion_ArmCallback_7(0, 0xc000, 0);
    ObjectMotion_ArmCallback_8(1, 0xc000, 0);
    ObjectMotion_ArmCallback_9(3, 0xc000, 0);
    ObjectMotion_ArmCallback_10(2, 0xc000, 0);
    line += 7;
    SceneWork_SetStepValue_5(line);
    BattleRuntime_WaitIfModeZero_49(30);
    BattleEffect_SpawnLinkedResourceObject_16(8, 0x100, 40);
    BattleEvent_RunActionAndWait_26(8, 0);
    BattleRuntime_WaitIfModeZero_50(10);
    ObjectMotion_CallThenWaitForAnimationChange_9(2, 3);
    BattleRuntime_WaitIfModeZero_51(30);
    BattleEvent_RunActionAndWait_27(2, 0);
    BattleRuntime_WaitIfModeZero_52(10);
    BattleEffect_SpawnLinkedResourceObject_17(8, 0x108, 40);
    BattleEvent_RunActionAndWait_28(8, 0);
    BattleRuntime_WaitIfModeZero_53(10);
    ObjectMotion_CallThenWaitForAnimationChange_10(2, 3);
    BattleRuntime_WaitIfModeZero_54(30);
    BattleEvent_RunActionAndWait_29(8, 0);
    BattleRuntime_WaitIfModeZero_55(10);
    ObjectMotion_CallThenWaitForAnimationChange_11(3, 3);
    BattleRuntime_WaitIfModeZero_56(30);
    BattleEvent_RunActionAndWait_30(8, 0);
    BattleRuntime_WaitIfModeZero_57(10);
    Object_LinkPair_7(1, 0, 50);
    ObjectMotion_ArmCallback_11(0, 0xc000, 0);
    ObjectMotion_ArmCallback_12(1, 0xc000, 0);
    BattleRuntime_WaitIfModeZero_58(30);
    BattleEvent_RunActionAndWait_31(8, 0);
    BattleRuntime_WaitIfModeZero_59(10);
    BattleEffect_SpawnLinkedResourceObject_18(3, 0x101, 40);
    BattleEvent_RunActionAndWait_32(3, 0);
    BattleRuntime_WaitIfModeZero_60(30);
    BattleEvent_RunActionAndWait_33(8, 0);
    BattleRuntime_WaitIfModeZero_61(10);
    Object_LinkPair_8(1, 0, 50);
    ObjectMotion_ArmCallback_13(0, 0xc000, 0);
    ObjectMotion_ArmCallback_14(1, 0xc000, 0);
    BattleRuntime_WaitIfModeZero_62(30);
    BattleEvent_RunActionAndWait_34(8, 0);
    BattleRuntime_WaitIfModeZero_63(10);
    Object_LinkPair_9(3, 2, 50);
    ObjectMotion_ArmCallback_15(3, 0xc000, 0);
    ObjectMotion_ArmCallback_16(2, 0xc000, 0);
    BattleRuntime_WaitIfModeZero_64(30);
    BattleEvent_RunActionAndWait_35(8, 0);
    BattleRuntime_WaitIfModeZero_65(10);
    ObjectMotion_SetVariantCallbackAndInvokeObject_8(1, 2);
    BattleRuntime_WaitIfModeZero_66(20);
    BattleEvent_RunActionAndWait_36(1, 0);
    BattleRuntime_WaitIfModeZero_67(30);
    BattleEvent_RunActionAndWait_37(8, 0);
    BattleRuntime_WaitIfModeZero_68(10);
    ObjectMotion_SetVariantCallbackAndInvokeObject_9(2, 2);
    BattleRuntime_WaitIfModeZero_69(20);
    BattleEvent_RunActionAndWait_38(2, 0);
    BattleRuntime_WaitIfModeZero_70(20);
    BattleEffect_SpawnLinkedResourceObject_19(8, 0x108, 50);
    BattleEvent_RunActionAndWait_39(8, 0);
    BattleRuntime_WaitIfModeZero_71(10);
    BattleEffect_SpawnLinkedResourceObject_20(1, 0x101, 40);
    BattleEvent_RunActionAndWait_40(1, 0);
    BattleRuntime_WaitIfModeZero_72(10);
    BattleEffect_SpawnLinkedResourceObject_21(8, 0x102, 40);
    BattleEvent_RunActionAndWait_41(8, 0);
    ObjectMotion_SetVariantCallbackAndInvokeObject_10(8, 2);
    BattleRuntime_WaitIfModeZero_73(20);
    BattleRuntime_WaitIfModeZero_74(10);
    ObjectMotion_CallThenWaitForAnimationChange_12(3, 4);
    BattleRuntime_WaitIfModeZero_75(20);
    BattleEvent_RunActionAndWait_42(3, 0);
    BattleRuntime_WaitIfModeZero_76(10);
    ObjectMotion_SetAngleToward_8(0, 3, 40);
    ObjectMotion_CallThenWaitForAnimationChange_13(0, 3);
    BattleRuntime_WaitIfModeZero_77(30);
    ObjectMotion_SetAngleToward_9(1, 3, 0);
    ObjectMotion_SetAngleToward_10(2, 3, 0);
    BattleRuntime_WaitIfModeZero_78(20);
    Object_SetModeById_1(1, 3);
    ObjectMotion_CallThenWaitForAnimationChange_14(2, 3);
    BattleRuntime_WaitIfModeZero_79(30);
    Audio_PlayCue_2(17);
    ObjectMotion_SetSpeedParameters_8(1, 0x13333, 0x9999);
    ObjectMotion_SetSpeedParameters_9(2, 0x13333, 0x9999);
    ObjectMotion_SetSpeedParameters_10(3, 0x13333, 0x9999);
    Object_SetModeById_2(1, 2);
    /* If the id-1 record lookup succeeds, forward its stored coordinates. */
    record = Scene_GetRecord_1(0);
    if (record != 0) {
        ObjectMotion_ResetAndSetPosition_1(1, *(s16 *)(record + RECORD_COORD_X_OFFSET), *(s16 *)(record + RECORD_COORD_Y_OFFSET));
    }
    ObjectMotion_CommitCurrentPositionAndActivate_2(1);
    ObjectMotion_SetHorizontalPositionWithTerrain_1(1, 0, 0);
    Object_SetModeById_3(2, 2);
    /* If the id-2 record lookup succeeds, forward its stored coordinates. */
    record = Scene_GetRecord_2(0);
    if (record != 0) {
        ObjectMotion_ResetAndSetPosition_2(2, *(s16 *)(record + RECORD_COORD_X_OFFSET), *(s16 *)(record + RECORD_COORD_Y_OFFSET));
    }
    ObjectMotion_CommitCurrentPositionAndActivate_3(2);
    ObjectMotion_SetHorizontalPositionWithTerrain_2(2, 0, 0);
    Object_SetModeById_4(3, 2);
    /* If the id-3 record lookup succeeds, forward its stored coordinates. */
    record = Scene_GetRecord_3(0);
    if (record != 0) {
        ObjectMotion_ResetAndSetPosition_3(3, *(s16 *)(record + RECORD_COORD_X_OFFSET), *(s16 *)(record + RECORD_COORD_Y_OFFSET));
    }
    ObjectMotion_CommitCurrentPositionAndActivate_4(3);
    ObjectMotion_SetHorizontalPositionWithTerrain_3(3, 0, 0);
    Audio_PlayCueForPartyMember_1();
    BattleRuntime_ScheduleShoulderButtonModeUpdate_1();
}
