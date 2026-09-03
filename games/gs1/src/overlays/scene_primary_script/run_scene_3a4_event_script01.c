#include "types.h"

#define RunEventScript01 Func_02003028

/* Loader-relocated overlay calls: each symbol names the pre-relocation call
 * word the image holds. */
extern u8 Data_0200c0e4[];
extern u8 Data_0200c12c[];
extern u8 Data_03001ebc[];
void Func_02006b9c();
void Func_02006bdc();
s32 Func_02006c12();
void Func_02006c22();
s32 Func_02006c26();
void Func_02006c30();
s32 Func_02006c3a();
void Func_02006c40();
void Func_02006c5a();
void Func_02006c64();
void Func_02006c6e();
void Func_02006c70();
s32 Func_02006c7e();
void Func_02006c84();
s32 Func_02006c86();
void Func_02006c98();
s32 Func_02006c9e();
s32 Func_02006cee();
void Func_02006d1a();
void Func_02006d22();
void Func_02006d26();
void Func_02006d34();
void Func_02006d42();
void Func_02006d46();
void Func_02006d4e();
void Func_02006d54();
void Func_02006d5a();
void Func_02006d62();
void Func_02006d6c();
void Func_02006d6e();
void Func_02006d76();
void Func_02006d84();
void Func_02006d8c();
s32 Func_02006d8e();
void Func_02006d92();
void Func_02006d94();
void Func_02006d9e();
void Func_02006da4();
void Func_02006dca();
void Func_02006dd4();
void Func_02006df0();
void Func_02006e08();
void Func_02006e24();
void Func_02006e34();
void Func_02006e3a();
void Func_02006e3c();
void Func_02006e40();
void Func_02006e48();
void Func_02006e4e();
void Func_02006e50();
void Func_02006e66();
void Func_02006e6a();
void Func_02006e74();
void Func_02006e7e();
void Func_02006e86();
void Func_02006e88();
void Func_02006e9e();
void Func_02006eaa();
void Func_02006eb6();
void Func_02006ec2();
void Func_02006ede();
void Func_02006ee8();
void Func_02006eea();
void Func_02006ef6();
void Func_02006ef8();
void Func_02006f00();
void Func_02006f02();
void Func_02006f08();
void Func_02006f10();
void Func_02006f14();
void Func_02006f1e();
void Func_02006f28();
void Func_02006f32();
void Func_02006f36();
s32 Func_02006f3a();
void Func_02006f42();
void Func_02006f44();
void Func_02006f48();
void Func_02006f4e();
void Func_02006f50();
void Func_02006f52();
void Func_02006f5a();
void Func_02006f62();
void Func_02006f72();
void Func_02006f78();
void Func_02006f8c();
void Func_02006f98();
void Func_02006fa2();
void Func_02006fc8();
void Func_0200affd();

/* Resolved engine calls: each pseudo symbol is the per-site call word the
 * overlay image holds (a word can serve two sites with different targets),
 * and the macro names the engine function the site reaches through the
 * overlay veneer and the main-image veneer island, keeping the site's own
 * calling form. Names without a repository binding are provisional.
 */
#define BattleRuntime_Reset_1(args...) Func_02006b9c(args)
#define ObjectMotion_SetHorizontalPositionWithTerrain_1(a0, a1, a2) Call3(Func_02006c22, a0, a1, a2)
#define ObjectMotion_SetHorizontalPositionWithTerrain_2(a0, a1, a2) Call3(Func_02006c30, a0, a1, a2)
#define Object_SetModeById_1(args...) Func_02006c40(args)
#define BattleRuntime_WaitIfModeZero_1(args...) Func_02006d1a(args)
#define ObjectMotion_SetSpeedParameters_1(args...) Func_02006d26(args)
#define BattleRuntime_WaitIfModeZero_2(args...) Func_02006bdc(args)
#define Scene_GetRecord_1(a0) Value1(Func_02006c12, a0)
#define ObjectMotion_SetHorizontalPositionWithTerrain_3(args...) Func_02006c70(args)
#define Scene_GetRecord_2(a0) Value1(Func_02006c26, a0)
#define ObjectMotion_SetHorizontalPositionWithTerrain_4(args...) Func_02006c84(args)
#define Scene_GetRecord_3(a0) Value1(Func_02006c3a, a0)
#define ObjectMotion_SetHorizontalPositionWithTerrain_5(args...) Func_02006c98(args)
#define ObjectMotion_SetSpeedParameters_2(a0, a1, a2) Call3(Func_02006c5a, a0, a1, a2)
#define ObjectMotion_SetSpeedParameters_3(a0, a1, a2) Call3(Func_02006c64, a0, a1, a2)
#define ObjectMotion_SetSpeedParameters_4(a0, a1, a2) Call3(Func_02006c6e, a0, a1, a2)
#define ObjectMotion_EnableActionAndSetCallback_1(a0, a1) Value2(Func_02006c7e, a0, a1)
#define ObjectMotion_EnableActionAndSetCallback_2(a0, a1) Value2(Func_02006c86, a0, a1)
#define ObjectMotion_MarkActiveAndSetActionCallback_1(a0, a1) Value2(Func_02006c9e, a0, a1)
#define ObjectMotion_ArmCallback_1(a0, a1, a2) Call3(Func_02006d42, a0, a1, a2)
#define ObjectMotion_ArmCallback_2(a0, a1, a2) Call3(Func_02006d4e, a0, a1, a2)
#define ObjectMotion_ArmCallback_3(a0, a1, a2) Call3(Func_02006d5a, a0, a1, a2)
#define ObjectMotion_SetVariantCallbackAndInvokeObject_1(args...) Func_02006d22(args)
#define ObjectMotion_ArmCallback_4(a0, a1, a2) Call3(Func_02006d6e, a0, a1, a2)
#define SceneWork_SetStepValue_1(a0) Call1(Func_02006d54, a0)
#define BattleRuntime_RunThenWaitIfModeZero_1(args...) Func_02006d76(args)
#define ObjectMotion_SetVariantCallbackAndInvokeObject_2(args...) Func_02006d46(args)
#define ObjectMotion_ArmCallback_5(a0, a1, a2) Call3(Func_02006d92, a0, a1, a2)
#define ObjectMotion_ArmCallback_6(a0, a1, a2) Call3(Func_02006d9e, a0, a1, a2)
#define ObjectMotion_SetSpeedParameters_5(a0, a1) Value2(Func_02006d8e, a0, a1)
#define UiWork_WaitThenFinalizeCapacity_1(a0, a1) Value2(Func_02006cee, a0, a1)
#define ObjectMotion_CallThenWaitForAnimationChange_1(args...) Func_02006d62(args)
#define ObjectMotion_CallThenWaitForAnimationChange_2(args...) Func_02006d6c(args)
#define BattleRuntime_RunThenWaitIfModeZero_2(args...) Func_02006dd4(args)
#define Object_SetModeById_2(args...) Func_02006d84(args)
#define Object_SetModeById_3(args...) Func_02006d8c(args)
#define Object_SetModeById_4(args...) Func_02006d94(args)
#define ObjectMotion_CallThenWaitForAnimationChange_3(args...) Func_02006da4(args)
#define BattleRuntime_WaitIfModeZero_3(args...) Func_02006d1a(args)
#define ObjectMotion_SetVariantCallbackAndInvokeObject_3(args...) Func_02006dca(args)
#define SceneWork_SetStepValue_2(a0) Call1(Func_02006df0, a0)
#define BattleEvent_RunActionAndWait_1(args...) Func_02006e08(args)
#define ObjectMotion_ArmCallback_7(a0, a1, a2) Call3(Func_02006e24, a0, a1, a2)
#define BattleEffect_SpawnLinkedResourceObject_1(a0, a1, a2) Call3(Func_02006e40, a0, a1, a2)
#define ObjectMotion_ArmCallback_8(a0, a1, a2) Call3(Func_02006e3c, a0, a1, a2)
#define BattleEvent_RunActionAndWait_2(args...) Func_02006e34(args)
#define ObjectMotion_ArmCallback_9(a0, a1, a2) Call3(Func_02006e50, a0, a1, a2)
#define BattleEffect_SpawnLinkedResourceObject_2(a0, a1, a2) Call3(Func_02006e6a, a0, a1, a2)
#define BattleEffect_SpawnLinkedResourceObject_3(a0, a1, a2) Call3(Func_02006e74, a0, a1, a2)
#define BattleEffect_SpawnLinkedResourceObject_4(a0, a1, a2) Call3(Func_02006e7e, a0, a1, a2)
#define BattleEffect_SpawnLinkedResourceObject_5(a0, a1, a2) Call3(Func_02006e88, a0, a1, a2)
#define Audio_PlayCue_1(args...) Func_02006f4e(args)
#define ObjectGroup_ConfigureChildValue_1(args...) Func_02006e4e(args)
#define BattleRuntime_WaitIfModeZero_4(args...) Func_02006da4(args)
#define Audio_PlayCue_2(a0) Call1(Func_02006f62, a0)
#define ObjectMotion_ArmCallback_10(a0, a1, a2) Call3(Func_02006e9e, a0, a1, a2)
#define ObjectMotion_ArmCallback_11(a0, a1, a2) Call3(Func_02006eaa, a0, a1, a2)
#define ObjectMotion_ArmCallback_12(a0, a1, a2) Call3(Func_02006eb6, a0, a1, a2)
#define ObjectMotion_ArmCallback_13(a0, a1, a2) Call3(Func_02006ec2, a0, a1, a2)
#define BattleEffect_SpawnLinkedResourceObject_6(a0, a1, a2) Call3(Func_02006ede, a0, a1, a2)
#define BattleEffect_SpawnLinkedResourceObject_7(a0, a1, a2) Call3(Func_02006eea, a0, a1, a2)
#define BattleEffect_SpawnLinkedResourceObject_8(a0, a1, a2) Call3(Func_02006ef6, a0, a1, a2)
#define BattleEffect_SpawnLinkedResourceObject_9(a0, a1, a2) Call3(Func_02006f02, a0, a1, a2)
#define Audio_PlayCue_3(args...) Func_02006fc8(args)
#define ObjectMotion_EnableActionAndSetCallback_3(args...) Func_02006e66(args)
#define ObjectMotion_MarkActiveAndSetActionCallback_2(args...) Func_02006e7e(args)
#define BattleRuntime_WaitIfModeZero_5(args...) Func_02006e3a(args)
#define ObjectMotion_SetVariantCallbackAndInvokeObject_4(args...) Func_02006eea(args)
#define BattleRuntime_WaitIfModeZero_6(args...) Func_02006e48(args)
#define BattleEvent_RunActionAndWait_3(args...) Func_02006f28(args)
#define ObjectMotion_ArmCallback_14(a0, a1, a2) Call3(Func_02006f44, a0, a1, a2)
#define ObjectMotion_ArmCallback_15(a0, a1, a2) Call3(Func_02006f50, a0, a1, a2)
#define Object_SetModeById_5(args...) Func_02006ef8(args)
#define Object_SetModeById_6(args...) Func_02006f00(args)
#define ObjectMotion_CallThenWaitForAnimationChange_4(args...) Func_02006f10(args)
#define BattleRuntime_WaitIfModeZero_7(args...) Func_02006e86(args)
#define ObjectMotion_SetVariantCallbackAndInvokeObject_5(args...) Func_02006f36(args)
#define BattleRuntime_RunThenWaitIfModeZero_3(args...) Func_02006f78(args)
#define ObjectMotion_ArmCallback_16(a0, a1, a2) Call3(Func_02006f8c, a0, a1, a2)
#define ObjectMotion_ArmCallback_17(a0, a1, a2) Call3(Func_02006f98, a0, a1, a2)
#define ObjectMotion_CallThenWaitForAnimationChange_5(args...) Func_02006f48(args)
#define BattleRuntime_RunThenWaitIfModeZero_4(args...) Func_02006fa2(args)
#define ObjectMotion_CallThenWaitForAnimationChange_6(args...) Func_02006f5a(args)
#define Object_SetModeById_7(args...) Func_02006f5a(args)
#define Object_SetModeById_8(args...) Func_02006f62(args)
#define ObjectMotion_CallThenWaitForAnimationChange_7(args...) Func_02006f72(args)
#define BattleRuntime_WaitIfModeZero_8(args...) Func_02006ee8(args)
#define ObjectMotion_EnableActionAndSetCallback_4(args...) Func_02006f32(args)
#define ObjectMotion_EnableActionAndSetCallback_5(a0, a1) Value2(Func_02006f3a, a0, a1)
#define ObjectMotion_MarkActiveAndSetActionCallback_3(args...) Func_02006f52(args)
#define BattleRuntime_WaitIfModeZero_9(args...) Func_02006f08(args)
#define GameFlag_Clear_1(a0) Call1(Func_02006f14, a0)
#define GameFlag_Set_1(a0) Call1(Func_02006f1e, a0)
#define BattleRuntime_ScheduleShoulderButtonModeUpdate_1(args...) Func_02006f42(args)

/* Remaining raw call sites, named from the engine function's own source. */

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

static __inline__ void Call2(void (*f)(), s32 a0, s32 a1)
{
    f(a0, a1);
}

static __inline__ s32 Value2(s32 (*f)(), s32 a0, s32 a1)
{
    return f(a0, a1);
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

/* Phase/status word at 0x1c0, and a companion word at 0x1c8, of the shared
 * scene work record. */
#define SCENE_PHASE (*(volatile s32 *)(work + 0x1c0))
#define SCENE_FIELD_1C8 (*(volatile s32 *)(work + 0x1c8))

/* Record returned by Func_02006c12/26/3a: a pair of s32 fields at +8 and
 * +16 that get forwarded straight into the matching setup call. */

void RunEventScript01(void)
{
    u32 i;
    s32 record;
    u8 *work;
    s32 addr_0200affd;
    s32 addr_0200c0e4;
    s32 addr_0200c12c;

    BattleRuntime_Reset_1();
    ObjectMotion_SetHorizontalPositionWithTerrain_1(8, 0x1480000, 0x580000);
    ObjectMotion_SetHorizontalPositionWithTerrain_2(9, 0x1480000, 0x580000);
    Object_SetModeById_1(8, 0);
    work = *(u8 *volatile *)Data_03001ebc;
    SCENE_PHASE = 0x100;
    SCENE_FIELD_1C8 = 40;
    BattleRuntime_WaitIfModeZero_1();
    ObjectMotion_SetSpeedParameters_1(); /* main:0808a370 */
    BattleRuntime_WaitIfModeZero_2(20);
    record = Scene_GetRecord_1(0);
    if (record != 0) {
        ObjectMotion_SetHorizontalPositionWithTerrain_3(1, *(volatile s32 *)(record + 8), *(volatile s32 *)(record + 16));
    }
    record = Scene_GetRecord_2(0);
    if (record != 0) {
        ObjectMotion_SetHorizontalPositionWithTerrain_4(2, *(volatile s32 *)(record + 8), *(volatile s32 *)(record + 16));
    }
    record = Scene_GetRecord_3(0);
    if (record != 0) {
        ObjectMotion_SetHorizontalPositionWithTerrain_5(3, *(volatile s32 *)(record + 8), *(volatile s32 *)(record + 16));
    }
    ObjectMotion_SetSpeedParameters_2(1, 0x9999, 0x4ccc);
    ObjectMotion_SetSpeedParameters_3(2, 0x9999, 0x4ccc);
    ObjectMotion_SetSpeedParameters_4(3, 0x9999, 0x4ccc);
    ObjectMotion_EnableActionAndSetCallback_1(1, 0x200c054);
    ObjectMotion_EnableActionAndSetCallback_2(2, 0x200c084);
    ObjectMotion_MarkActiveAndSetActionCallback_1(3, 0x200c0b4);
    ObjectMotion_ArmCallback_1(1, 0xc000, 0);
    ObjectMotion_ArmCallback_2(2, 0xc000, 0);
    ObjectMotion_ArmCallback_3(3, 0xc000, 40);
    ObjectMotion_SetVariantCallbackAndInvokeObject_1(1, 1);
    ObjectMotion_ArmCallback_4(1, 0xe000, 10);
    SceneWork_SetStepValue_1(0x190c);
    BattleRuntime_RunThenWaitIfModeZero_1(1, 0, 10);
    ObjectMotion_SetVariantCallbackAndInvokeObject_2(2, 1);
    ObjectMotion_ArmCallback_5(2, 0xa000, 10);
    ObjectMotion_ArmCallback_6(0, 0x2000, 0);
    ObjectMotion_SetSpeedParameters_5(2, 0); /* main:0808a178 */
    if (UiWork_WaitThenFinalizeCapacity_1(0, 0) == 0) {
        ObjectMotion_CallThenWaitForAnimationChange_1(2, 3);
    } else {
        ObjectMotion_CallThenWaitForAnimationChange_2(2, 4);
        bump_step(1);
    }
    BattleRuntime_RunThenWaitIfModeZero_2(2, 0, 20);
    Object_SetModeById_2(0, 3);
    Object_SetModeById_3(1, 3);
    Object_SetModeById_4(0, 3);
    ObjectMotion_CallThenWaitForAnimationChange_3(0, 3);
    BattleRuntime_WaitIfModeZero_3(20);
    ObjectMotion_SetVariantCallbackAndInvokeObject_3(3, 2);
    SceneWork_SetStepValue_2(0x1910);
    BattleEvent_RunActionAndWait_1(3, 0);
    ObjectMotion_ArmCallback_7(0, 0x4000, 10);
    BattleEffect_SpawnLinkedResourceObject_1(1, 0x102, 60);
    ObjectMotion_ArmCallback_8(1, 0x2000, 40);
    BattleEvent_RunActionAndWait_2(1, 0);
    ObjectMotion_ArmCallback_9(0, 0x6000, 10);
    BattleEffect_SpawnLinkedResourceObject_2(0, 0x101, 0);
    BattleEffect_SpawnLinkedResourceObject_3(1, 0x101, 0);
    BattleEffect_SpawnLinkedResourceObject_4(2, 0x101, 0);
    BattleEffect_SpawnLinkedResourceObject_5(3, 0x101, 40);
    Audio_PlayCue_1(190);
    ObjectGroup_ConfigureChildValue_1(8, 7);
    BattleRuntime_WaitIfModeZero_4(10);
    Audio_PlayCue_2(0x121);
    ObjectMotion_ArmCallback_10(0, 0xc000, 0);
    ObjectMotion_ArmCallback_11(1, 0xc000, 0);
    ObjectMotion_ArmCallback_12(2, 0xc000, 0);
    ObjectMotion_ArmCallback_13(3, 0xc000, 0);
    BattleEffect_SpawnLinkedResourceObject_6(0, 0x100, 0);
    BattleEffect_SpawnLinkedResourceObject_7(1, 0x100, 0);
    BattleEffect_SpawnLinkedResourceObject_8(2, 0x100, 0);
    BattleEffect_SpawnLinkedResourceObject_9(3, 0x100, 40);
    Audio_PlayCue_3(103);
    addr_0200affd = (s32)Func_0200affd;
    Call2(Func_02006d34, addr_0200affd, 0xc80); /* main:080000d0 */
    addr_0200c0e4 = (s32)Data_0200c0e4;
    ObjectMotion_EnableActionAndSetCallback_3(9, addr_0200c0e4);
    ObjectMotion_MarkActiveAndSetActionCallback_2(8, addr_0200c0e4);
    Func_02006d54(addr_0200affd); /* main:080000d8 */
    BattleRuntime_WaitIfModeZero_5(60);
    ObjectMotion_SetVariantCallbackAndInvokeObject_4(2, 2);
    BattleRuntime_WaitIfModeZero_6(20);
    BattleEvent_RunActionAndWait_3(2, 0);
    ObjectMotion_ArmCallback_14(1, 0xe000, 0);
    ObjectMotion_ArmCallback_15(2, 0xa000, 20);
    Object_SetModeById_5(1, 3);
    Object_SetModeById_6(2, 3);
    ObjectMotion_CallThenWaitForAnimationChange_4(3, 3);
    BattleRuntime_WaitIfModeZero_7(20);
    ObjectMotion_SetVariantCallbackAndInvokeObject_5(3, 1);
    BattleRuntime_RunThenWaitIfModeZero_3(3, 0, 20);
    ObjectMotion_ArmCallback_16(1, 0xe000, 0);
    ObjectMotion_ArmCallback_17(0, 0x6000, 40);
    ObjectMotion_CallThenWaitForAnimationChange_5(1, 3);
    BattleRuntime_RunThenWaitIfModeZero_4(1, 0, 10);
    ObjectMotion_CallThenWaitForAnimationChange_6(0, 3);
    Object_SetModeById_7(1, 3);
    Object_SetModeById_8(2, 3);
    ObjectMotion_CallThenWaitForAnimationChange_7(3, 3);
    BattleRuntime_WaitIfModeZero_8(20);
    addr_0200c12c = (s32)Data_0200c12c;
    ObjectMotion_EnableActionAndSetCallback_4(1, addr_0200c12c);
    ObjectMotion_EnableActionAndSetCallback_5(2, addr_0200c12c);
    ObjectMotion_MarkActiveAndSetActionCallback_3(3, addr_0200c12c);
    BattleRuntime_WaitIfModeZero_9(20);
    *(s32 *)((*(u8 *volatile *)Data_03001ebc + 0x1c0)) = 0x204;
    GameFlag_Clear_1(0x12f);
    *(s32 *)((*(u8 *volatile *)Data_03001ebc + 0x1c8)) = 16;
    GameFlag_Set_1(0x909);
    BattleRuntime_ScheduleShoulderButtonModeUpdate_1();
}
