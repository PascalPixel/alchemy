#include "types.h"

extern u8 Value_00000fe0;
void Func_080770c8();
void Func_080770d0();
void Func_0808a010();
void Func_0808a018();
void Func_0808a020();
int Func_0808a070();
u8 *Func_0808a080();
void Func_0808a090();
void Func_0808a0b8();
void Func_0808a0e0();
void Func_0808a0e8();
s32 Func_0808a0f0();
s32 Func_0808a100();
void Func_0808a110();
void Func_0808a128();
void Func_0808a130();
s32 Func_0808a138();
void Func_0808a150();
void Func_0808a170();
void Func_0808a178();
void Func_0808a180();
void Func_0808a188();
s32 Func_0808a1b8();
s32 Func_0808a1e8();
void Func_0808a1f0();
void Func_0808a208();
void Func_0808a210();
void Func_0808a218();
void Func_0808a360();
void Func_0808a370();

/*
 * resource_37f owner at 0x02000f8c, 1,680 bytes: full scene sequence
 * for entities 0, 1, 5 and 8.
 *
 * Complete owner: saved-link prologue at 0x02000f8c through the sole
 * interworking return at 0x02001600, alignment and six-word trailing pool
 * through 0x0200161b.  A branch at 0x020013d0 skips an eleven-word interior
 * constant pool and resumes at 0x02001400.  All 174 BL sites resolve to 30
 * import veneers and are represented below in machine order.
 */
/* Loader-relocated ROM calls: each site names the pre-relocation call word the image holds. */
s32 Func_02002b3c();
s32 Func_02002b6a();
s32 Func_02002b7e();
s32 Func_02002b92();
s32 Func_02002ba8();
s32 Func_02002bb2();
s32 Func_02002bbc();
s32 Func_02002bc6();
s32 Func_02002bd0();
s32 Func_02002bfe();
s32 Func_02002c02();
s32 Func_02002c06();
s32 Func_02002c0c();
s32 Func_02002c0e();
s32 Func_02002c18();
s32 Func_02002c26();
s32 Func_02002c38();
s32 Func_02002c3e();
s32 Func_02002c46();
s32 Func_02002c4c();
s32 Func_02002c4e();
s32 Func_02002c56();
s32 Func_02002cb4();
s32 Func_02002cba();
s32 Func_02002cc6();
s32 Func_02002cd2();
s32 Func_02002cdc();
s32 Func_02002cde();
s32 Func_02002ce8();
s32 Func_02002cf4();
s32 Func_02002d00();
s32 Func_02002d0c();
s32 Func_02002d10();
s32 Func_02002d18();
s32 Func_02002d1c();
s32 Func_02002d1e();
s32 Func_02002d22();
s32 Func_02002d2e();
s32 Func_02002d36();
s32 Func_02002d3a();
s32 Func_02002d46();
s32 Func_02002d52();
s32 Func_02002d54();
s32 Func_02002d5e();
s32 Func_02002d68();
s32 Func_02002d74();
s32 Func_02002d76();
s32 Func_02002d86();
s32 Func_02002d8e();
s32 Func_02002da0();
s32 Func_02002da8();
s32 Func_02002daa();
s32 Func_02002db6();
s32 Func_02002dd2();
s32 Func_02002dec();
s32 Func_02002e12();
s32 Func_02002e2a();
s32 Func_02002e34();
s32 Func_02002e36();
s32 Func_02002e4c();
s32 Func_02002e58();
s32 Func_02002e5e();
s32 Func_02002e60();
s32 Func_02002e6c();
s32 Func_02002e7e();
s32 Func_02002e88();
s32 Func_02002e8a();
s32 Func_02002e90();
s32 Func_02002e96();
s32 Func_02002ea0();
s32 Func_02002ea2();
s32 Func_02002eaa();
s32 Func_02002eac();
s32 Func_02002eb2();
s32 Func_02002ebc();
s32 Func_02002eca();
s32 Func_02002ed8();
s32 Func_02002ee4();
s32 Func_02002ee6();
s32 Func_02002f14();
s32 Func_02002f16();
s32 Func_02002f18();
s32 Func_02002f1e();
s32 Func_02002f26();
s32 Func_02002f28();
s32 Func_02002f2e();
s32 Func_02002f46();
s32 Func_02002f4c();
s32 Func_02002f52();
s32 Func_02002f70();
s32 Func_02002f7c();
s32 Func_02002f7e();
s32 Func_02002f90();
s32 Func_02002f92();
s32 Func_02002fa6();
s32 Func_02002fac();
s32 Func_02002fc2();
s32 Func_02002fcc();
s32 Func_02002fe2();
s32 Func_02002fe6();
s32 Func_02002ff0();
s32 Func_02002ff2();
s32 Func_02002ffc();
s32 Func_02003004();
s32 Func_02003020();
s32 Func_0200303e();
s32 Func_0200304c();
s32 Func_02003054();
s32 Func_02003056();
s32 Func_02003058();
s32 Func_02003060();
s32 Func_0200306e();
s32 Func_02003070();
s32 Func_0200307a();
s32 Func_0200308a();
s32 Func_020030a8();
s32 Func_020030aa();
s32 Func_020030ae();
s32 Func_020030b4();
s32 Func_020030b8();
s32 Func_020030c4();
s32 Func_020030d0();
s32 Func_020030d6();
s32 Func_020030dc();
s32 Func_020030de();
s32 Func_020030ea();
s32 Func_020030ec();
s32 Func_020030f4();
s32 Func_020030f8();
s32 Func_02003104();
s32 Func_0200310e();
s32 Func_02003118();
s32 Func_0200311a();
s32 Func_02003122();
s32 Func_0200312a();
s32 Func_02003130();
s32 Func_0200313e();
s32 Func_02003146();
s32 Func_0200314a();
s32 Func_0200314c();
s32 Func_02003156();
s32 Func_0200315c();
s32 Func_0200316a();
s32 Func_0200316c();
s32 Func_0200316e();
s32 Func_0200317a();
s32 Func_0200317c();
s32 Func_02003186();
s32 Func_0200318c();
s32 Func_02003194();
s32 Func_0200319c();
s32 Func_020031aa();
s32 Func_020031b0();
s32 Func_020031bc();
s32 Func_020031c6();
s32 Func_020031d0();
s32 Func_020031e0();
s32 Func_020031e8();
s32 Func_020031f0();

/* Resolved engine calls: each pseudo symbol is the per-site call word the
 * overlay image holds (a word can serve two sites with different targets),
 * and the macro names the engine function the site reaches through the
 * overlay veneer and the main-image veneer island, keeping the site's own
 * calling form. Names without a repository binding are provisional.
 */
#define BattleRuntime_Reset_1() Value0(Func_02002b3c)
#define BattleRuntime_WaitIfModeZero_1() Value0(Func_02002c38)
#define ObjectMotion_SetSpeedParameters_1() Value0(Func_02002c4c)
#define Scene_GetRecord_1(a0) Value1(Func_02002b6a, a0)
#define ObjectMotion_SetHorizontalPositionWithTerrain_1(a0, a1, a2) Value3(Func_02002ba8, a0, a1, a2)
#define Scene_GetRecord_2(a0) Value1(Func_02002b7e, a0)
#define ObjectMotion_SetHorizontalPositionWithTerrain_2(a0, a1, a2) Value3(Func_02002bbc, a0, a1, a2)
#define Scene_GetRecord_3(a0) Value1(Func_02002b92, a0)
#define ObjectMotion_SetHorizontalPositionWithTerrain_3(a0, a1, a2) Value3(Func_02002bd0, a0, a1, a2)
#define ObjectMotion_SetSpeedParameters_2(a0, a1, a2) Call3(Func_02002bb2, a0, a1, a2)
#define ObjectMotion_SetSpeedParameters_3(a0, a1, a2) Call3(Func_02002bbc, a0, a1, a2)
#define ObjectMotion_SetSpeedParameters_4(a0, a1, a2) Call3(Func_02002bc6, a0, a1, a2)
#define Object_SetModeById_1(a0, a1) Call2(Func_02002bfe, a0, a1)
#define Object_SetModeById_2(a0, a1) Call2(Func_02002c06, a0, a1)
#define Object_SetModeById_3(a0, a1) Call2(Func_02002c0e, a0, a1)
#define ObjectMotion_OffsetPositionAndResetMotion_1(a0, a1, a2) Call3(Func_02002c02, a0, a1, a2)
#define ObjectMotion_OffsetPositionAndResetMotion_2(a0, a1, a2) Call3(Func_02002c0c, a0, a1, a2)
#define ObjectMotion_OffsetPositionAndResetMotion_3(a0, a1, a2) Value3(Func_02002c18, a0, a1, a2)
#define ObjectMotion_CommitCurrentPositionAndActivate_1(a0) Value1(Func_02002c26, a0)
#define Object_SetModeById_4(a0, a1) Call2(Func_02002c3e, a0, a1)
#define Object_SetModeById_5(a0, a1) Call2(Func_02002c46, a0, a1)
#define Object_SetModeById_6(a0, a1) Call2(Func_02002c4e, a0, a1)
#define Object_SetModeById_7(a0, a1) Call2(Func_02002c56, a0, a1)
#define ObjectMotion_ArmCallback_1(a0, a1, a2) Call3(Func_02002cba, a0, a1, a2)
#define ObjectMotion_ArmCallback_2(a0, a1, a2) Call3(Func_02002cc6, a0, a1, a2)
#define ObjectMotion_ArmCallback_3(a0, a1, a2) Call3(Func_02002cd2, a0, a1, a2)
#define ObjectMotion_ArmCallback_4(a0, a1, a2) Call3(Func_02002cde, a0, a1, a2)
#define ObjectMotion_ArmCallback_5(a0, a1, a2) Call3(Func_02002ce8, a0, a1, a2)
#define ObjectMotion_ArmCallback_6(a0, a1, a2) Call3(Func_02002cf4, a0, a1, a2)
#define ObjectMotion_ArmCallback_7(a0, a1, a2) Call3(Func_02002d00, a0, a1, a2)
#define ObjectMotion_ArmCallback_8(a0, a1, a2) Call3(Func_02002d0c, a0, a1, a2)
#define ObjectMotion_ArmCallback_9(a0, a1, a2) Call3(Func_02002d18, a0, a1, a2)
#define ObjectMotion_ArmCallback_10(a0, a1, a2) Call3(Func_02002d22, a0, a1, a2)
#define ObjectMotion_ArmCallback_11(a0, a1, a2) Call3(Func_02002d2e, a0, a1, a2)
#define ObjectMotion_ArmCallback_12(a0, a1, a2) Call3(Func_02002d3a, a0, a1, a2)
#define ObjectMotion_ArmCallback_13(a0, a1, a2) Call3(Func_02002d46, a0, a1, a2)
#define ObjectMotion_ArmCallback_14(a0, a1, a2) Call3(Func_02002d52, a0, a1, a2)
#define ObjectMotion_ArmCallback_15(a0, a1, a2) Call3(Func_02002d5e, a0, a1, a2)
#define ObjectMotion_SetVariantCallbackAndInvokeObject_1(a0, a1) Value2(Func_02002d2e, a0, a1)
#define BattleRuntime_WaitIfModeZero_2(a0) Value1(Func_02002cb4, a0)
#define Object_SetModeById_8(a0, a1) Call2(Func_02002d1c, a0, a1)
#define ObjectMotion_OffsetPositionAndResetMotion_4(a0, a1, a2) Value3(Func_02002d10, a0, a1, a2)
#define ObjectMotion_CommitCurrentPositionAndActivate_2(a0) Value1(Func_02002d1e, a0)
#define Object_SetModeById_9(a0, a1) Value2(Func_02002d36, a0, a1)
#define BattleRuntime_WaitIfModeZero_3(a0) Value1(Func_02002cdc, a0)
#define ObjectMotion_ArmCallback_16(a0, a1, a2) Call3(Func_02002da0, a0, a1, a2)
#define ObjectMotion_ArmCallback_17(a0, a1, a2) Call3(Func_02002daa, a0, a1, a2)
#define ObjectMotion_ArmCallback_18(a0, a1, a2) Call3(Func_02002db6, a0, a1, a2)
#define ObjectMotion_SetVariantCallbackAndInvokeObject_2(a0, a1) Value2(Func_02002d86, a0, a1)
#define BattleRuntime_WaitIfModeZero_4(a0) Value1(Func_02002d0c, a0)
#define Object_SetModeById_10(a0, a1) Call2(Func_02002d74, a0, a1)
#define ObjectMotion_OffsetPositionAndResetMotion_5(a0, a1, a2) Value3(Func_02002d68, a0, a1, a2)
#define ObjectMotion_CommitCurrentPositionAndActivate_3(a0) Value1(Func_02002d76, a0)
#define Object_SetModeById_11(a0, a1) Call2(Func_02002d8e, a0, a1)
#define ObjectMotion_SetSpeedLimitAndAcceleration_1(a0, a1) Call2(Func_02002e12, a0, a1)
#define ObjectMotion_PlaceWithinCameraBounds_1(a0, a1, a2, a3) Call4(Func_02002e2a, a0, a1, a2, a3)
#define Object_CommitPositionThenWaitIfModeZero_1() Value0(Func_02002e36)
#define BattleRuntime_WaitIfModeZero_5(a0) Value1(Func_02002d54, a0)
#define ObjectMotion_SetSpeedLimitAndAcceleration_2(a0, a1) Call2(Func_02002e34, a0, a1)
#define ObjectMotion_PlaceWithinCameraBounds_2(a0, a1, a2, a3) Call4(Func_02002e4c, a0, a1, a2, a3)
#define Object_CommitPositionThenWaitIfModeZero_2() Call0(Func_02002e58)
#define ObjectMotion_PlaceWithinCameraBounds_3(a0, a1, a2, a3) Call4(Func_02002e60, a0, a1, a2, a3)
#define Object_CommitPositionThenWaitIfModeZero_3() Call0(Func_02002e6c)
#define Object_SetModeById_12(a0, a1) Call2(Func_02002dec, a0, a1)
#define ObjectMotion_PlaceWithinCameraBounds_4(a0, a1, a2, a3) Value4(Func_02002e7e, a0, a1, a2, a3)
#define Object_CommitPositionThenWaitIfModeZero_4() Value0(Func_02002e8a)
#define BattleRuntime_WaitIfModeZero_6(a0) Value1(Func_02002da8, a0)
#define ObjectMotion_SetSpeedLimitAndAcceleration_3(a0, a1) Call2(Func_02002e88, a0, a1)
#define ObjectMotion_PlaceWithinCameraBounds_5(a0, a1, a2, a3) Call4(Func_02002ea0, a0, a1, a2, a3)
#define Object_CommitPositionThenWaitIfModeZero_5() Call0(Func_02002eac)
#define ObjectMotion_CallThenWaitForAnimationChange_1(a0, a1) Value2(Func_02002e34, a0, a1)
#define BattleRuntime_WaitIfModeZero_7(a0) Value1(Func_02002dd2, a0)
#define ObjectMotion_ArmCallback_19(a0, a1, a2) Call3(Func_02002e96, a0, a1, a2)
#define ObjectMotion_ArmCallback_20(a0, a1, a2) Call3(Func_02002ea2, a0, a1, a2)
#define BattleEffect_SpawnLinkedResourceObject_1(a0, a1, a2) Call3(Func_02002ebc, a0, a1, a2)
#define SceneWork_SetStepValue_1(a0) Call1(Func_02002e8a, a0)
#define BattleRuntime_RunThenWaitIfModeZero_1(a0, a1, a2) Call3(Func_02002eac, a0, a1, a2)
#define BattleEffect_SpawnLinkedResourceObject_2(a0, a1, a2) Call3(Func_02002ed8, a0, a1, a2)
#define ObjectMotion_SetVariantCallback_1(a0, a1) Call2(Func_02002e90, a0, a1)
#define BattleRuntime_RunThenWaitIfModeZero_2(a0, a1, a2) Call3(Func_02002eca, a0, a1, a2)
#define ObjectMotion_SetVariantCallback_2(a0, a1) Call2(Func_02002ea2, a0, a1)
#define ObjectMotion_SetVariantCallback_3(a0, a1) Call2(Func_02002eaa, a0, a1)
#define ObjectMotion_SetVariantCallback_4(a0, a1) Call2(Func_02002eb2, a0, a1)
#define BattleRuntime_WaitIfModeZero_8(a0, a1) Call2(Func_02002f14, a0, a1)
#define BattleRuntime_WaitIfModeZero_9(a0, a1) Call2(Func_02002f1e, a0, a1)
#define BattleRuntime_WaitIfModeZero_10(a0, a1) Value2(Func_02002f28, a0, a1)
#define BattleRuntime_WaitIfModeZero_11(a0) Value1(Func_02002e5e, a0)
#define ObjectMotion_SetVariantCallbackAndInvokeObject_3(a0, a1) Value2(Func_02002ee6, a0, a1)
#define BattleRuntime_WaitIfModeZero_12(a0) Value1(Func_02002e6c, a0)
#define BattleEvent_RunActionAndWait_1(a0, a1) Call2(Func_02002f14, a0, a1)
#define ObjectMotion_CallThenWaitForAnimationChange_2(a0, a1) Call2(Func_02002ee4, a0, a1)
#define BattleRuntime_RunThenWaitIfModeZero_3(a0, a1, a2) Call3(Func_02002f2e, a0, a1, a2)
#define Object_LinkPair_1(a0, a1, a2) Value3(Func_02002f18, a0, a1, a2)
#define BattleRuntime_WaitIfModeZero_13(a0) Value1(Func_02002e96, a0)
#define ObjectMotion_SetVariantCallback_5(a0, a1) Call2(Func_02002f16, a0, a1)
#define ObjectMotion_SetVariantCallbackAndInvokeObject_4(a0, a1) Value2(Func_02002f26, a0, a1)
#define BattleRuntime_WaitIfModeZero_14(a0) Value1(Func_02002eac, a0)
#define ObjectMotion_ArmCallback_21(a0, a1, a2) Call3(Func_02002f70, a0, a1, a2)
#define ObjectMotion_ArmCallback_22(a0, a1, a2) Call3(Func_02002f7c, a0, a1, a2)
#define ObjectMotion_SetVariantCallbackAndInvokeObject_5(a0, a1) Call2(Func_02002f4c, a0, a1)
#define BattleRuntime_RunThenWaitIfModeZero_4(a0, a1, a2) Call3(Func_02002f7e, a0, a1, a2)
#define ObjectMotion_CallThenWaitForAnimationChange_3(a0, a1) Call2(Func_02002f46, a0, a1)
#define BattleRuntime_RunThenWaitIfModeZero_5(a0, a1, a2) Call3(Func_02002f90, a0, a1, a2)
#define ObjectMotion_ArmCallback_23(a0, a1, a2) Call3(Func_02002fac, a0, a1, a2)
#define BattleRuntime_RunThenWaitIfModeZero_6(a0, a1, a2) Call3(Func_02002fa6, a0, a1, a2)
#define ObjectMotion_ArmCallback_24(a0, a1, a2) Call3(Func_02002fc2, a0, a1, a2)
#define ObjectMotion_SetVariantCallbackAndInvokeObject_6(a0, a1) Value2(Func_02002f92, a0, a1)
#define BattleRuntime_WaitIfModeZero_15(a0) Value1(Func_02002f18, a0)
#define BattleRuntime_WaitIfModeZero_16(a0, a1) Call2(Func_02002ff2, a0, a1)
#define ObjectMotion_ArmCallback_25(a0, a1, a2) Call3(Func_02002fe6, a0, a1, a2)
#define ObjectMotion_ArmCallback_26(a0, a1, a2) Call3(Func_02002ff0, a0, a1, a2)
#define ObjectMotion_ArmCallback_27(a0, a1, a2) Call3(Func_02002ffc, a0, a1, a2)
#define ObjectMotion_SetVariantCallbackAndInvokeObject_7(a0, a1) Value2(Func_02002fcc, a0, a1)
#define BattleRuntime_WaitIfModeZero_17(a0) Value1(Func_02002f52, a0)
#define BattleRuntime_RunThenWaitIfModeZero_7(a0, a1, a2) Call3(Func_02003004, a0, a1, a2)
#define ObjectMotion_ArmCallback_28(a0, a1, a2) Call3(Func_02003020, a0, a1, a2)
#define ObjectMotion_Launch_1(a0, a1, a2) Call3(Func_02002fe2, a0, a1, a2)
#define BattleRuntime_RunThenWaitIfModeZero_8(a0, a1, a2) Call3(Func_02003054, a0, a1, a2)
#define ObjectMotion_ArmCallback_29(a0, a1, a2) Call3(Func_0200306e, a0, a1, a2)
#define ObjectMotion_SetSpeedParameters_5(a0, a1) Value2(Func_02003056, a0, a1)
#define UiWork_WaitThenFinalizeCapacity_1(a0, a1) Value2(Func_02002fe6, a0, a1)
#define SceneWork_SetStepValue_2(a0) Value1(Func_02003060, a0)
#define ObjectMotion_SetVariantCallbackAndInvokeObject_8(a0, a1) Call2(Func_02003058, a0, a1)
#define BattleRuntime_RunThenWaitIfModeZero_9(a0, a1, a2) Value3(Func_0200308a, a0, a1, a2)
#define SceneWork_SetStepValue_3(a0) Value1(Func_0200307a, a0)
#define ObjectMotion_ArmCallback_30(a0, a1, a2) Call3(Func_020030ae, a0, a1, a2)
#define BattleRuntime_RunThenWaitIfModeZero_10(a0, a1, a2) Call3(Func_020030a8, a0, a1, a2)
#define ObjectMotion_ArmCallback_31(a0, a1, a2) Call3(Func_020030c4, a0, a1, a2)
#define ObjectMotion_ArmCallback_32(a0, a1, a2) Call3(Func_020030d0, a0, a1, a2)
#define ObjectMotion_ArmCallback_33(a0, a1, a2) Call3(Func_020030dc, a0, a1, a2)
#define BattleEffect_SpawnLinkedResourceObject_3(a0, a1, a2) Call3(Func_020030f8, a0, a1, a2)
#define ObjectMotion_SetVariantCallbackAndInvokeObject_9(a0, a1) Value2(Func_020030b8, a0, a1)
#define BattleRuntime_WaitIfModeZero_18(a0) Value1(Func_0200303e, a0)
#define ObjectMotion_CallThenWaitForAnimationChange_4(a0, a1) Value2(Func_020030ae, a0, a1)
#define BattleRuntime_WaitIfModeZero_19(a0) Value1(Func_0200304c, a0)
#define ObjectMotion_ArmCallback_34(a0, a1, a2) Call3(Func_0200310e, a0, a1, a2)
#define ObjectMotion_ArmCallback_35(a0, a1, a2) Call3(Func_0200311a, a0, a1, a2)
#define ObjectMotion_SetVariantCallbackAndInvokeObject_10(a0, a1) Value2(Func_020030ea, a0, a1)
#define BattleRuntime_WaitIfModeZero_20(a0) Value1(Func_02003070, a0)
#define BattleRuntime_RunThenWaitIfModeZero_11(a0, a1, a2) Value3(Func_02003122, a0, a1, a2)
#define ObjectMotion_SetSpeedParameters_6(a0, a1, a2) Call3(Func_020030b4, a0, a1, a2)
#define Object_SetModeById_13(a0, a1) Call2(Func_020030ec, a0, a1)
#define ObjectMotion_OffsetPositionAndResetMotion_6(a0, a1, a2) Value3(Func_020030de, a0, a1, a2)
#define ObjectMotion_CommitCurrentPositionAndActivate_4(a0) Value1(Func_020030ec, a0)
#define Object_SetModeById_14(a0, a1) Value2(Func_02003104, a0, a1)
#define BattleRuntime_WaitIfModeZero_21(a0) Value1(Func_020030aa, a0)
#define ObjectMotion_ArmCallback_36(a0, a1, a2) Call3(Func_0200316e, a0, a1, a2)
#define ObjectMotion_ArmCallback_37(a0, a1, a2) Value3(Func_0200317a, a0, a1, a2)
#define ObjectMotion_CommitCurrentPositionAndActivate_5(a0) Value1(Func_02003118, a0)
#define Object_SetModeById_15(a0, a1) Value2(Func_02003130, a0, a1)
#define BattleRuntime_WaitIfModeZero_22(a0) Value1(Func_020030d6, a0)
#define Object_SetModeById_16(a0, a1) Call2(Func_0200313e, a0, a1)
#define Object_SetModeById_17(a0, a1) Call2(Func_02003146, a0, a1)
#define ObjectMotion_CallThenWaitForAnimationChange_5(a0, a1) Value2(Func_02003156, a0, a1)
#define BattleRuntime_WaitIfModeZero_23(a0) Value1(Func_020030f4, a0)
#define Object_SetModeById_18(a0, a1) Value2(Func_0200315c, a0, a1)
#define Scene_GetRecord_4(a0) Value1(Func_0200312a, a0)
#define ObjectMotion_ResetAndSetPosition_1(a0, a1, a2) Value3(Func_0200314c, a0, a1, a2)
#define Object_SetModeById_19(a0, a1) Value2(Func_0200317c, a0, a1)
#define Scene_GetRecord_5(a0) Value1(Func_0200314a, a0)
#define ObjectMotion_ResetAndSetPosition_2(a0, a1, a2) Value3(Func_0200316c, a0, a1, a2)
#define Object_SetModeById_20(a0, a1) Value2(Func_0200319c, a0, a1)
#define Scene_GetRecord_6(a0) Value1(Func_0200316a, a0)
#define ObjectMotion_ResetAndSetPosition_3(a0, a1, a2) Value3(Func_0200318c, a0, a1, a2)
#define ObjectMotion_CommitCurrentPositionAndActivate_6(a0) Value1(Func_020031aa, a0)
#define ObjectMotion_SetHorizontalPositionWithTerrain_4(a0, a1, a2) Call3(Func_020031bc, a0, a1, a2)
#define ObjectMotion_SetHorizontalPositionWithTerrain_5(a0, a1, a2) Call3(Func_020031c6, a0, a1, a2)
#define ObjectMotion_SetHorizontalPositionWithTerrain_6(a0, a1, a2) Call3(Func_020031d0, a0, a1, a2)
#define Object_SetModeById_21(a0, a1) Call2(Func_020031e0, a0, a1)
#define Object_SetModeById_22(a0, a1) Call2(Func_020031e8, a0, a1)
#define Object_SetModeById_23(a0, a1) Value2(Func_020031f0, a0, a1)
#define GameFlag_Set_1(a0) Value1(Func_02003186, a0)
#define GameFlag_Clear_1(a0) Value1(Func_02003194, a0)
#define BattleRuntime_ScheduleShoulderButtonModeUpdate_1() Value0(Func_020031b0)

/* Additional resolved engine calls: named directly from each target's own
 * reconstructed C source (see the trailing "provisional" comment on each). */

static __inline__ void Call0(void (*f)())
{
    f();
}

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

/* Two-field record read used when copying a found record's s32 pair into a
 * setter call alongside an entity id. */
#define RECORD_A32(rec) (*(s32 *)((rec) + 8))
#define RECORD_B32(rec) (*(s32 *)((rec) + 16))

/* Two-field record read used when copying a found record's s16 pair into a
 * setter call alongside an entity id. */
#define RECORD_A16(rec) (*(s16 *)((rec) + 10))
#define RECORD_B16(rec) (*(s16 *)((rec) + 18))

/* Runs the full scene sequence for entities 0, 1, 5 and 8: per-entity setup,
 * a shared movement/pose pass, a branch on one entity's saved state, and a
 * closing pass over the same four entities. */
void FieldScene_RunFourEntitySequence(void)
{
    u8 *record;

    BattleRuntime_Reset_1();
    BattleRuntime_WaitIfModeZero_1(); /* main:0808a360 */
    ObjectMotion_SetSpeedParameters_1(); /* main:0808a370 */

    record = Scene_GetRecord_1(0);
    if (record != 0)
        ObjectMotion_SetHorizontalPositionWithTerrain_1(8, RECORD_A32(record), RECORD_B32(record));
    record = Scene_GetRecord_2(0);
    if (record != 0)
        ObjectMotion_SetHorizontalPositionWithTerrain_2(5, RECORD_A32(record), RECORD_B32(record));
    record = Scene_GetRecord_3(0);
    if (record != 0)
        ObjectMotion_SetHorizontalPositionWithTerrain_3(1, RECORD_A32(record), RECORD_B32(record));

    ObjectMotion_SetSpeedParameters_2(8, 0x9999, 0x4ccc);
    ObjectMotion_SetSpeedParameters_3(5, 0x9999, 0x4ccc);
    ObjectMotion_SetSpeedParameters_4(1, 0x9999, 0x4ccc);
    Object_SetModeById_1(1, 2);
    Object_SetModeById_2(5, 2);
    Object_SetModeById_3(8, 2);
    ObjectMotion_OffsetPositionAndResetMotion_1(1, -16, 0);
    ObjectMotion_OffsetPositionAndResetMotion_2(5, 16, 0);
    ObjectMotion_OffsetPositionAndResetMotion_3(8, 0, -16);
    ObjectMotion_CommitCurrentPositionAndActivate_1(8);
    Object_SetModeById_4(8, 1);
    Object_SetModeById_5(0, 0);
    Object_SetModeById_6(1, 0);
    Object_SetModeById_7(5, 0);
    ObjectMotion_ArmCallback_1(1, 0xe000, 0);
    ObjectMotion_ArmCallback_2(5, 0xa000, 0);
    ObjectMotion_ArmCallback_3(8, 0xc000, 30);
    ObjectMotion_ArmCallback_4(1, 0x8000, 0);
    ObjectMotion_ArmCallback_5(5, 0, 0);
    ObjectMotion_ArmCallback_6(0, 0x4000, 0);
    ObjectMotion_ArmCallback_7(8, 0x8000, 30);
    ObjectMotion_ArmCallback_8(1, 0x4000, 0);
    ObjectMotion_ArmCallback_9(5, 0x8000, 0);
    ObjectMotion_ArmCallback_10(0, 0, 0);
    ObjectMotion_ArmCallback_11(8, 0x4000, 30);
    ObjectMotion_ArmCallback_12(1, 0xe000, 0);
    ObjectMotion_ArmCallback_13(5, 0xa000, 0);
    ObjectMotion_ArmCallback_14(0, 0xc000, 0);
    ObjectMotion_ArmCallback_15(8, 0xc000, 40);
    ObjectMotion_SetVariantCallbackAndInvokeObject_1(8, 2);
    BattleRuntime_WaitIfModeZero_2(10);
    Object_SetModeById_8(8, 2);
    ObjectMotion_OffsetPositionAndResetMotion_4(8, 0, -16);
    ObjectMotion_CommitCurrentPositionAndActivate_2(8);
    Object_SetModeById_9(8, 1);
    BattleRuntime_WaitIfModeZero_3(6);
    ObjectMotion_ArmCallback_16(8, 0x8000, 20);
    ObjectMotion_ArmCallback_17(8, 0, 20);
    ObjectMotion_ArmCallback_18(8, 0xc000, 40);
    ObjectMotion_SetVariantCallbackAndInvokeObject_2(8, 2);
    BattleRuntime_WaitIfModeZero_4(20);
    Object_SetModeById_10(8, 2);
    ObjectMotion_OffsetPositionAndResetMotion_5(8, 0, -32);
    ObjectMotion_CommitCurrentPositionAndActivate_3(8);
    Object_SetModeById_11(8, 1);

    ObjectMotion_SetSpeedLimitAndAcceleration_1(0x20000, 0x4000);
    ObjectMotion_PlaceWithinCameraBounds_1(0x06310000, -1, 0x00960000, 1);
    Object_CommitPositionThenWaitIfModeZero_1();
    BattleRuntime_WaitIfModeZero_5(10);
    ObjectMotion_SetSpeedLimitAndAcceleration_2(0x13333, 0x2666);
    ObjectMotion_PlaceWithinCameraBounds_2(0x06550000, -1, 0x00640000, 1);
    Object_CommitPositionThenWaitIfModeZero_2();
    ObjectMotion_PlaceWithinCameraBounds_3(0x06b60000, -1, 0x00640000, 1);
    Object_CommitPositionThenWaitIfModeZero_3();
    Object_SetModeById_12(8, 1);
    ObjectMotion_PlaceWithinCameraBounds_4(0x06d80000, -1, 0x00960000, 1);
    Object_CommitPositionThenWaitIfModeZero_4();
    BattleRuntime_WaitIfModeZero_6(40);
    ObjectMotion_SetSpeedLimitAndAcceleration_3(0x26666, 0x4ccc);
    ObjectMotion_PlaceWithinCameraBounds_5(0x06840000, -1, 0x01000000, 1);
    Object_CommitPositionThenWaitIfModeZero_5();
    ObjectMotion_CallThenWaitForAnimationChange_1(8, 3);
    BattleRuntime_WaitIfModeZero_7(10);

    ObjectMotion_ArmCallback_19(1, 0xe000, 0);
    ObjectMotion_ArmCallback_20(5, 0xa000, 10);
    BattleEffect_SpawnLinkedResourceObject_1(1, 0x101, 20);
    SceneWork_SetStepValue_1(0xfd6);
    BattleRuntime_RunThenWaitIfModeZero_1(1, 0, 10);
    BattleEffect_SpawnLinkedResourceObject_2(8, 0x102, 60);
    ObjectMotion_SetVariantCallback_1(8, 2);
    BattleRuntime_RunThenWaitIfModeZero_2(8, 0, 10);
    ObjectMotion_SetVariantCallback_2(0, 2);
    ObjectMotion_SetVariantCallback_3(1, 2);
    ObjectMotion_SetVariantCallback_4(5, 2);
    BattleRuntime_WaitIfModeZero_8(0, 0x102);
    BattleRuntime_WaitIfModeZero_9(1, 0x102); /* main:0808a1f0 */
    BattleRuntime_WaitIfModeZero_10(5, 0x102); /* main:0808a1f0 */
    BattleRuntime_WaitIfModeZero_11(40);
    ObjectMotion_SetVariantCallbackAndInvokeObject_3(8, 2);
    BattleRuntime_WaitIfModeZero_12(20);
    BattleEvent_RunActionAndWait_1(8, 0);
    ObjectMotion_CallThenWaitForAnimationChange_2(8, 4);
    BattleRuntime_RunThenWaitIfModeZero_3(8, 0, 10);
    Object_LinkPair_1(0, 5, 0);
    BattleRuntime_WaitIfModeZero_13(40);
    ObjectMotion_SetVariantCallback_5(0, 1);
    ObjectMotion_SetVariantCallbackAndInvokeObject_4(5, 1);
    BattleRuntime_WaitIfModeZero_14(10);
    ObjectMotion_ArmCallback_21(0, 0xc000, 0);
    ObjectMotion_ArmCallback_22(5, 0xa000, 20);
    ObjectMotion_SetVariantCallbackAndInvokeObject_5(5, 2);
    BattleRuntime_RunThenWaitIfModeZero_4(5, 0, 10);
    ObjectMotion_CallThenWaitForAnimationChange_3(8, 4);
    BattleRuntime_RunThenWaitIfModeZero_5(8, 0, 10);
    ObjectMotion_ArmCallback_23(8, 0x3000, 10);
    BattleRuntime_RunThenWaitIfModeZero_6(8, 0, 40);
    ObjectMotion_ArmCallback_24(8, 0xc000, 20);
    ObjectMotion_SetVariantCallbackAndInvokeObject_6(8, 1);
    BattleRuntime_WaitIfModeZero_15(10);
    BattleRuntime_WaitIfModeZero_16(8, 0x102); /* main:0808a1f0 */
    ObjectMotion_ArmCallback_25(8, 0x8000, 20);
    ObjectMotion_ArmCallback_26(8, 0, 20);
    ObjectMotion_ArmCallback_27(8, 0xc000, 60);
    ObjectMotion_SetVariantCallbackAndInvokeObject_7(8, 2);
    BattleRuntime_WaitIfModeZero_17(10);
    BattleRuntime_RunThenWaitIfModeZero_7(8, 0, 10);
    ObjectMotion_ArmCallback_28(8, 0x4000, 20);
    ObjectMotion_Launch_1(8, 2, 20);

    BattleRuntime_RunThenWaitIfModeZero_8(8, 0, 40);
    ObjectMotion_ArmCallback_29(1, 0, 20);
    ObjectMotion_SetSpeedParameters_5(1, 0); /* main:0808a178 */
    if (UiWork_WaitThenFinalizeCapacity_1(0, 0) == 0) {
        /* Passes the address of Value_00000fe0 in place of a record pointer. */
        SceneWork_SetStepValue_2((s32)&Value_00000fe0);
        ObjectMotion_SetVariantCallbackAndInvokeObject_8(1, 1);
        BattleRuntime_RunThenWaitIfModeZero_9(1, 0, 10);
    } else {
        SceneWork_SetStepValue_3(0xfe1);
        ObjectMotion_ArmCallback_30(5, 0x8000, 20);
        BattleRuntime_RunThenWaitIfModeZero_10(5, 0, 10);
        ObjectMotion_ArmCallback_31(0, 0xc000, 0);
        ObjectMotion_ArmCallback_32(1, 0xe000, 0);
        ObjectMotion_ArmCallback_33(5, 0xa000, 60);
        BattleEffect_SpawnLinkedResourceObject_3(0, 0x102, 40);
        ObjectMotion_SetVariantCallbackAndInvokeObject_9(1, 1);
        BattleRuntime_WaitIfModeZero_18(10);
        ObjectMotion_CallThenWaitForAnimationChange_4(1, 3);
        BattleRuntime_WaitIfModeZero_19(10);
        ObjectMotion_ArmCallback_34(1, 0, 0);
        ObjectMotion_ArmCallback_35(5, 0x8000, 30);
        ObjectMotion_SetVariantCallbackAndInvokeObject_10(1, 1);
        BattleRuntime_WaitIfModeZero_20(10);
        BattleRuntime_RunThenWaitIfModeZero_11(1, 0, 10);
    }

    ObjectMotion_SetSpeedParameters_6(8, 0x9999, 0x4ccc);
    Object_SetModeById_13(8, 2);
    ObjectMotion_OffsetPositionAndResetMotion_6(8, 0, 48);
    ObjectMotion_CommitCurrentPositionAndActivate_4(8);
    Object_SetModeById_14(8, 1);
    BattleRuntime_WaitIfModeZero_21(6);
    ObjectMotion_ArmCallback_36(1, 0xe000, 0);
    ObjectMotion_ArmCallback_37(5, 0xa000, 0);
    ObjectMotion_CommitCurrentPositionAndActivate_5(8);
    Object_SetModeById_15(8, 1);
    BattleRuntime_WaitIfModeZero_22(20);
    Object_SetModeById_16(1, 3);
    Object_SetModeById_17(5, 3);
    ObjectMotion_CallThenWaitForAnimationChange_5(0, 3);
    BattleRuntime_WaitIfModeZero_23(6);
    Object_SetModeById_18(1, 2);

    record = Scene_GetRecord_4(0);
    if (record != 0)
        ObjectMotion_ResetAndSetPosition_1(1, RECORD_A16(record), RECORD_B16(record));
    Object_SetModeById_19(5, 2);
    record = Scene_GetRecord_5(0);
    if (record != 0)
        ObjectMotion_ResetAndSetPosition_2(5, RECORD_A16(record), RECORD_B16(record));
    Object_SetModeById_20(8, 2);
    record = Scene_GetRecord_6(0);
    if (record != 0)
        ObjectMotion_ResetAndSetPosition_3(8, RECORD_A16(record), RECORD_B16(record));

    ObjectMotion_CommitCurrentPositionAndActivate_6(8);
    ObjectMotion_SetHorizontalPositionWithTerrain_4(1, 0, 0);
    ObjectMotion_SetHorizontalPositionWithTerrain_5(5, 0, 0);
    ObjectMotion_SetHorizontalPositionWithTerrain_6(8, 0, 0);
    Object_SetModeById_21(8, 1);
    Object_SetModeById_22(1, 1);
    Object_SetModeById_23(5, 1);
    GameFlag_Set_1(0x804);
    GameFlag_Clear_1(0x12f);
    BattleRuntime_ScheduleShoulderButtonModeUpdate_1();
}
