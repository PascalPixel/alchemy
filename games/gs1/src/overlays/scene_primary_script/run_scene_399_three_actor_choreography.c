#include "types.h"

#define FieldScene_RunThreeActorChoreography Func_020019e8

/* Loader-relocated overlay calls: each symbol names the pre-relocation call
 * word the image holds. */
extern u8 Data_03001ebc[];
void Func_02003c1c();
void Func_02003c52();
s32 Func_02003c94();
s32 Func_02003cac();
void Func_02003cae();
void Func_02003cc6();
void Func_02003cf4();
void Func_02003cfa();
void Func_02003d0c();
void Func_02003d18();
void Func_02003d22();
void Func_02003d30();
void Func_02003d40();
s32 Func_02003d5e();
void Func_02003d60();
s32 Func_02003d68();
void Func_02003d70();
void Func_02003d74();
void Func_02003d82();
void Func_02003d94();
void Func_02003da4();
void Func_02003da8();
void Func_02003db0();
void Func_02003db8();
void Func_02003dbe();
void Func_02003dc2();
void Func_02003dc6();
void Func_02003dda();
void Func_02003de4();
void Func_02003dec();
void Func_02003e04();
void Func_02003e08();
void Func_02003e0a();
void Func_02003e32();
void Func_02003e3e();
void Func_02003e4a();
void Func_02003e4e();
void Func_02003e5a();
void Func_02003e66();
void Func_02003e72();
void Func_02003e7e();
void Func_02003e86();
void Func_02003e8a();
s32 Func_02003e8c();
void Func_02003e8e();
void Func_02003ea0();
void Func_02003ea2();
s32 Func_02003eb0();
void Func_02003eb8();
void Func_02003eca();
void Func_02003ed4();
void Func_02003ed6();
void Func_02003ede();
void Func_02003eec();
void Func_02003ef2();
void Func_02003efe();
void Func_02003f0a();
void Func_02003f10();
void Func_02003f16();
void Func_02003f2c();
void Func_02003f38();
void Func_02003f44();
void Func_02003f50();
void Func_02003f54();
void Func_02003f62();
void Func_02003f74();
void Func_02003f92();
void Func_02003fa2();
void Func_02003fb6();
void Func_02003fbc();
void Func_02003fcc();
void Func_02003fd2();
void Func_02003fd4();
void Func_02003fe6();
void Func_02003ffe();
void Func_02004002();
void Func_02004004();
void Func_02004008();
void Func_0200400c();
void Func_02004016();
void Func_0200401e();
void Func_02004020();
void Func_02004032();
void Func_0200403e();
void Func_02004046();
void Func_0200404e();
void Func_02004050();
void Func_02004078();
void Func_0200407a();
void Func_02004080();
void Func_0200409e();
void Func_020040a0();
void Func_020040a4();
void Func_020040ae();
void Func_020040b2();
void Func_020040c8();
void Func_020040d4();
void Func_020040da();
void Func_020040de();
void Func_020040f6();
void Func_020040f8();
void Func_0200410a();
s32 Func_02004110();
void Func_0200411a();
void Func_0200411c();
void Func_02004126();
void Func_02004128();
s32 Func_02004130();
void Func_02004136();
void Func_02004144();
void Func_02004148();
void Func_02004150();
void Func_0200415c();
void Func_02004166();
void Func_02004168();
void Func_0200416c();
void Func_02004172();
void Func_02004174();
void Func_02004176();
void Func_020041a2();
void Func_020041a8();
void Func_020041be();

/* Resolved engine calls: each pseudo symbol is the per-site call word the
 * overlay image holds (a word can serve two sites with different targets),
 * and the macro names the engine function the site reaches through the
 * overlay veneer and the main-image veneer island, keeping the site's own
 * calling form. Names without a repository binding are provisional.
 */
#define BattleRuntime_Reset_1(args...) Func_02003c1c(args)
#define ObjectMotion_ArmCallback_1(a0, a1, a2) Call3(Func_02003d18, a0, a1, a2)
#define ObjectMotion_SetSpeedParameters_1(a0, a1, a2) Call3(Func_02003c52, a0, a1, a2)
#define ObjectMotion_ResetAndSetPositionInMode2_1(a0, a1, a2) Value3(Func_02003c94, a0, a1, a2)
#define RuntimeBlock_GetOffset1e0Pointer_1(args...) Func_02003d68(args)
#define ObjectMotion_SetSpeedLimitAndAcceleration_1(a0, a1) Value2(Func_02003d5e, a0, a1)
#define ObjectMotion_PlaceWithinCameraBounds_1(a0, a1, a2, a3) Call4(Func_02003d74, a0, a1, a2, a3)
#define BattleRuntime_WaitIfModeZero_1(args...) Func_02003dc6(args)
#define ObjectMotion_CommitCurrentPositionAndActivate_1(args...) Func_02003cf4(args)
#define Object_SetModeById_1(args...) Func_02003d0c(args)
#define ObjectMotion_SetSpeedParameters_2(a0, a1, a2) Call3(Func_02003cae, a0, a1, a2)
#define Scene_GetRecord_1(a0) Value1(Func_02003cac, a0)
#define ObjectMotion_SetHorizontalPositionWithTerrain_1(args...) Func_02003d22(args)
#define ObjectMotion_SetPositionAndReset_1(a0, a1, a2) Call3(Func_02003d0c, a0, a1, a2)
#define ObjectMotion_ArmCallback_2(a0, a1, a2) Call3(Func_02003da8, a0, a1, a2)
#define ObjectMotion_SetVariantCallback_1(args...) Func_02003d60(args)
#define ObjectMotion_SetVariantCallbackAndInvokeObject_1(args...) Func_02003d70(args)
#define BattleRuntime_WaitIfModeZero_2(args...) Func_02003cc6(args)
#define SceneWork_SetStepValue_1(a0) Call1(Func_02003da4, a0)
#define BattleRuntime_RunThenWaitIfModeZero_1(args...) Func_02003dbe(args)
#define ObjectMotion_ArmCallback_3(a0, a1, a2) Call3(Func_02003dda, a0, a1, a2)
#define ObjectMotion_CallThenWaitForAnimationChange_1(args...) Func_02003d82(args)
#define BattleRuntime_WaitIfModeZero_3(a0, a1) Call2(Func_02003e04, a0, a1)
#define BattleRuntime_WaitIfModeZero_4(args...) Func_02003cfa(args)
#define BattleRuntime_RunThenWaitIfModeZero_2(a0, a1, a2) Call3(Func_02003dec, a0, a1, a2)
#define ObjectMotion_ArmCallback_4(a0, a1, a2) Call3(Func_02003e08, a0, a1, a2)
#define ObjectMotion_CallThenWaitForAnimationChange_2(args...) Func_02003db0(args)
#define BattleRuntime_RunThenWaitIfModeZero_3(a0, a1, a2) Call3(Func_02003e0a, a0, a1, a2)
#define ObjectMotion_CallThenWaitForAnimationChange_3(args...) Func_02003dc2(args)
#define BattleRuntime_WaitIfModeZero_5(args...) Func_02003d30(args)
#define BattleRuntime_WaitIfModeZero_6(a0, a1) Call2(Func_02003e4a, a0, a1)
#define BattleRuntime_WaitIfModeZero_7(args...) Func_02003d40(args)
#define BattleRuntime_RunThenWaitIfModeZero_4(args...) Func_02003e32(args)
#define ObjectMotion_ArmCallback_5(a0, a1, a2) Call3(Func_02003e4e, a0, a1, a2)
#define ObjectMotion_ArmCallback_6(a0, a1, a2) Call3(Func_02003e5a, a0, a1, a2)
#define ObjectMotion_ArmCallback_7(a0, a1, a2) Call3(Func_02003e66, a0, a1, a2)
#define ObjectMotion_ArmCallback_8(a0, a1, a2) Call3(Func_02003e72, a0, a1, a2)
#define ObjectMotion_ArmCallback_9(a0, a1, a2) Call3(Func_02003e7e, a0, a1, a2)
#define ObjectMotion_SetVariantCallbackAndInvokeObject_2(args...) Func_02003e3e(args)
#define BattleRuntime_WaitIfModeZero_8(args...) Func_02003d94(args)
#define BattleRuntime_RunThenWaitIfModeZero_5(a0, a1, a2) Call3(Func_02003e86, a0, a1, a2)
#define ObjectMotion_ArmCallback_10(a0, a1, a2) Call3(Func_02003ea2, a0, a1, a2)
#define ObjectMotion_CallThenWaitForAnimationChange_4(args...) Func_02003e4a(args)
#define BattleRuntime_WaitIfModeZero_9(args...) Func_02003db8(args)
#define BattleEffect_SpawnLinkedResourceObject_1(a0, a1, a2) Call3(Func_02003eca, a0, a1, a2)
#define BattleEffect_SpawnLinkedResourceObject_2(a0, a1, a2) Call3(Func_02003ed4, a0, a1, a2)
#define BattleEffect_SpawnLinkedResourceObject_3(a0, a1, a2) Call3(Func_02003ede, a0, a1, a2)
#define ObjectMotion_SetVariantCallbackAndInvokeObject_3(args...) Func_02003e8e(args)
#define BattleRuntime_WaitIfModeZero_10(args...) Func_02003de4(args)
#define BattleRuntime_RunThenWaitIfModeZero_6(args...) Func_02003ed6(args)
#define ObjectMotion_ArmCallback_11(a0, a1, a2) Call3(Func_02003ef2, a0, a1, a2)
#define ObjectMotion_ArmCallback_12(a0, a1, a2) Call3(Func_02003efe, a0, a1, a2)
#define ObjectMotion_ArmCallback_13(a0, a1, a2) Call3(Func_02003f0a, a0, a1, a2)
#define ObjectMotion_ArmCallback_14(a0, a1, a2) Call3(Func_02003f16, a0, a1, a2)
#define BattleRuntime_RunThenWaitIfModeZero_7(a0, a1, a2) Call3(Func_02003f10, a0, a1, a2)
#define ObjectMotion_ArmCallback_15(a0, a1, a2) Call3(Func_02003f2c, a0, a1, a2)
#define ObjectMotion_ArmCallback_16(a0, a1, a2) Call3(Func_02003f38, a0, a1, a2)
#define ObjectMotion_ArmCallback_17(a0, a1, a2) Call3(Func_02003f44, a0, a1, a2)
#define ObjectMotion_ArmCallback_18(a0, a1, a2) Call3(Func_02003f50, a0, a1, a2)
#define ObjectMotion_SetSpeedParameters_3(a0, a1, a2) Call3(Func_02003e8e, a0, a1, a2)
#define Scene_GetRecord_2(args...) Func_02003e8c(args)
#define ObjectMotion_SetPositionAndReset_2(a0, a1, a2) Call3(Func_02003eec, a0, a1, a2)
#define BattleRuntime_WaitIfModeZero_11(args...) Func_02003e8a(args)
#define Scene_GetRecord_3(args...) Func_02003eb0(args)
#define BattleRuntime_WaitIfModeZero_12(args...) Func_02003ea0(args)
#define BattleRuntime_RunThenWaitIfModeZero_8(a0, a1, a2) Call3(Func_02003f92, a0, a1, a2)
#define ObjectMotion_SetVariantCallbackAndInvokeObject_4(args...) Func_02003f62(args)
#define BattleRuntime_WaitIfModeZero_13(args...) Func_02003eb8(args)
#define ObjectMotion_ArmCallback_19(a0, a1, a2) Call3(Func_02003fbc, a0, a1, a2)
#define BattleRuntime_RunThenWaitIfModeZero_9(a0, a1, a2) Call3(Func_02003fb6, a0, a1, a2)
#define ObjectMotion_ArmCallback_20(a0, a1, a2) Call3(Func_02003fd2, a0, a1, a2)
#define BattleRuntime_RunThenWaitIfModeZero_10(a0, a1, a2) Call3(Func_02003fcc, a0, a1, a2)
#define BattleRuntime_WaitIfModeZero_14(a0, a1) Call2(Func_02003ffe, a0, a1)
#define BattleRuntime_WaitIfModeZero_15(a0, a1) Call2(Func_02004008, a0, a1)
#define BattleRuntime_WaitIfModeZero_16(args...) Func_02003efe(args)
#define ObjectMotion_ArmCallback_21(a0, a1, a2) Call3(Func_02004002, a0, a1, a2)
#define Object_SetModeById_2(args...) Func_02003fa2(args)
#define BattleRuntime_RunThenWaitIfModeZero_11(a0, a1, a2) Call3(Func_02004004, a0, a1, a2)
#define ObjectMotion_SetVariantCallbackAndInvokeObject_5(args...) Func_02003fd4(args)
#define BattleRuntime_RunThenWaitIfModeZero_12(args...) Func_02004016(args)
#define ObjectMotion_ArmCallback_22(a0, a1, a2) Call3(Func_02004032, a0, a1, a2)
#define ObjectMotion_ArmCallback_23(a0, a1, a2) Call3(Func_0200403e, a0, a1, a2)
#define ObjectMotion_CallThenWaitForAnimationChange_5(args...) Func_02003fe6(args)
#define BattleRuntime_WaitIfModeZero_17(args...) Func_02003f54(args)
#define ObjectMotion_SetVariantCallbackAndInvokeObject_6(args...) Func_0200400c(args)
#define BattleRuntime_RunThenWaitIfModeZero_13(a0, a1, a2) Call3(Func_0200404e, a0, a1, a2)
#define ObjectMotion_SetVariantCallbackAndInvokeObject_7(args...) Func_0200401e(args)
#define BattleRuntime_WaitIfModeZero_18(args...) Func_02003f74(args)
#define ObjectMotion_ArmCallback_24(a0, a1, a2) Call3(Func_02004078, a0, a1, a2)
#define ObjectMotion_CallThenWaitForAnimationChange_6(args...) Func_02004020(args)
#define BattleRuntime_RunThenWaitIfModeZero_14(a0, a1, a2) Call3(Func_0200407a, a0, a1, a2)
#define BattleEffect_SpawnLinkedResourceObject_4(a0, a1, a2) Call3(Func_020040a4, a0, a1, a2)
#define BattleEffect_SpawnLinkedResourceObject_5(a0, a1, a2) Call3(Func_020040ae, a0, a1, a2)
#define ObjectMotion_CallThenWaitForAnimationChange_7(args...) Func_02004046(args)
#define BattleRuntime_RunThenWaitIfModeZero_15(args...) Func_020040a0(args)
#define Object_SetModeById_3(args...) Func_02004050(args)
#define BattleRuntime_RunThenWaitIfModeZero_16(a0, a1, a2) Call3(Func_020040b2, a0, a1, a2)
#define BattleEffect_SpawnLinkedResourceObject_6(a0, a1, a2) Call3(Func_020040de, a0, a1, a2)
#define BattleRuntime_RunThenWaitIfModeZero_17(a0, a1, a2) Call3(Func_020040c8, a0, a1, a2)
#define ObjectMotion_CallThenWaitForAnimationChange_8(args...) Func_02004080(args)
#define BattleRuntime_RunThenWaitIfModeZero_18(args...) Func_020040da(args)
#define ObjectMotion_ArmCallback_25(a0, a1, a2) Call3(Func_020040f6, a0, a1, a2)
#define ObjectMotion_CallThenWaitForAnimationChange_9(args...) Func_0200409e(args)
#define BattleRuntime_RunThenWaitIfModeZero_19(a0, a1, a2) Call3(Func_020040f8, a0, a1, a2)
#define ObjectMotion_ArmCallback_26(a0, a1, a2) Call3(Func_02004150, a0, a1, a2)
#define ObjectMotion_ArmCallback_27(a0, a1, a2) Call3(Func_0200415c, a0, a1, a2)
#define ObjectMotion_ArmCallback_28(a0, a1, a2) Call3(Func_02004168, a0, a1, a2)
#define ObjectMotion_ArmCallback_29(a0, a1, a2) Call3(Func_02004174, a0, a1, a2)
#define ObjectMotion_CallThenWaitForAnimationChange_10(args...) Func_0200411c(args)
#define BattleRuntime_RunThenWaitIfModeZero_20(a0, a1, a2) Call3(Func_02004176, a0, a1, a2)
#define Object_SetModeById_4(args...) Func_02004126(args)
#define ObjectMotion_CallThenWaitForAnimationChange_11(args...) Func_02004136(args)
#define BattleRuntime_WaitIfModeZero_19(args...) Func_020040a4(args)
#define ObjectMotion_ArmCallback_30(a0, a1, a2) Call3(Func_020041a8, a0, a1, a2)
#define BattleRuntime_RunThenWaitIfModeZero_21(a0, a1, a2) Call3(Func_020041a2, a0, a1, a2)
#define ObjectMotion_ArmCallback_31(a0, a1, a2) Call3(Func_020041be, a0, a1, a2)
#define ObjectMotion_CallThenWaitForAnimationChange_12(args...) Func_02004166(args)
#define BattleRuntime_WaitIfModeZero_20(args...) Func_020040d4(args)
#define ObjectMotion_SetPositionAndReset_3(a0, a1, a2) Call3(Func_02004148, a0, a1, a2)
#define ObjectMotion_SetHorizontalPositionWithTerrain_2(args...) Func_02004172(args)
#define Scene_GetRecord_4(args...) Func_02004110(args)
#define ObjectMotion_SetPositionAndReset_4(a0, a1, a2) Call3(Func_0200416c, a0, a1, a2)
#define BattleRuntime_WaitIfModeZero_21(args...) Func_0200410a(args)
#define Scene_GetRecord_5(args...) Func_02004130(args)
#define GameFlag_Set_1(a0) Call1(Func_0200411a, a0)
#define GameFlag_Clear_1(a0) Call1(Func_02004128, a0)
#define BattleRuntime_ScheduleShoulderButtonModeUpdate_1(args...) Func_02004144(args)

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

static __inline__ s32 Value3(s32 (*f)(), s32 a0, s32 a1, s32 a2)
{
    return f(a0, a1, a2);
}

static __inline__ void Call4(void (*f)(), s32 a0, s32 a1, s32 a2, s32 a3)
{
    f(a0, a1, a2, a3);
}

/* The scene step counter at 0x1d8 of the shared scene work record. */
static __inline__ void bump_step(s32 amount)
{
    u8 *work = *(u8 **)Data_03001ebc;

    *(u16 *)(work + 0x1d8) = (u16)(*(u16 *)(work + 0x1d8) + amount);
}

/* Byte offset of a flags field on the actor records touched below; bit 0
 * of that byte is cleared and later set back for actors 20 (twice). */
#define ACTOR_FLAGS_OFFSET 90

/* Runs a long scripted sequence of position, animation, and timing calls
 * against actor records 0, 3, 19, and 20, with a scene phase word at
 * offset 0x1c0 of the shared scene work record set at the start and near
 * the end. */
void FieldScene_RunThreeActorChoreography(void)
{
    u32 i;
    s32 record;
    u8 *work;

    BattleRuntime_Reset_1();
    ObjectMotion_ArmCallback_1(3, 0xa000, 0);
    ObjectMotion_SetSpeedParameters_1(0, 0x9999, 0x4ccc);
    ObjectMotion_ResetAndSetPositionInMode2_1(0, 0x2b2, 200);
    /* Clear the byte at offset 85 of the returned record. */
    *(u8 *)(RuntimeBlock_GetOffset1e0Pointer_1() + 85) = 0;
    ObjectMotion_SetSpeedLimitAndAcceleration_1(0xcccc, 0x1999);
    ObjectMotion_PlaceWithinCameraBounds_1(0x2b20000, 0, 0xa40000, 1);
    /* Set the scene phase word (offset 0x1c0) and a related word at
     * offset 0x1c8 of the shared scene work record. */
    work = *(u8 *volatile *)Data_03001ebc;
    *(volatile s32 *)((work + 0x1c0)) = 0x100;
    *(volatile s32 *)((work + 0x1c8)) = 48;
    BattleRuntime_WaitIfModeZero_1();
    ObjectMotion_CommitCurrentPositionAndActivate_1(0);
    Object_SetModeById_1(0, 1);
    ObjectMotion_SetSpeedParameters_2(3, 0x9999, 0x4ccc);
    record = Scene_GetRecord_1(0);
    if (record != 0) {
        ObjectMotion_SetHorizontalPositionWithTerrain_1(3, *(volatile s32 *)(record + 8), *(volatile s32 *)(record + 16));
    }
    ObjectMotion_SetPositionAndReset_1(3, 0x2a1, 183);
    ObjectMotion_ArmCallback_2(3, 0xc000, 0);
    ObjectMotion_SetVariantCallback_1(19, 2);
    ObjectMotion_SetVariantCallbackAndInvokeObject_1(20, 2);
    BattleRuntime_WaitIfModeZero_2(40);
    SceneWork_SetStepValue_1(0x165b);
    BattleRuntime_RunThenWaitIfModeZero_1(19, 0, 10);
    ObjectMotion_ArmCallback_3(3, 0xe000, 40);
    ObjectMotion_CallThenWaitForAnimationChange_1(3, 3);
    BattleRuntime_WaitIfModeZero_3(20, 0x102);
    BattleRuntime_WaitIfModeZero_4(20);
    BattleRuntime_RunThenWaitIfModeZero_2(0x4014, 0, 10);
    ObjectMotion_ArmCallback_4(3, 0xa000, 40);
    ObjectMotion_CallThenWaitForAnimationChange_2(3, 4);
    BattleRuntime_RunThenWaitIfModeZero_3(0x2003, 0, 10);
    ObjectMotion_CallThenWaitForAnimationChange_3(20, 3);
    BattleRuntime_WaitIfModeZero_5(20);
    BattleRuntime_WaitIfModeZero_6(19, 0x102);
    BattleRuntime_WaitIfModeZero_7(20);
    BattleRuntime_RunThenWaitIfModeZero_4(19, 0, 10);
    ObjectMotion_ArmCallback_5(0, 0xa000, 0);
    ObjectMotion_ArmCallback_6(3, 0xf000, 10);
    ObjectMotion_ArmCallback_7(3, 0x2000, 40);
    ObjectMotion_ArmCallback_8(0, 0xc000, 0);
    ObjectMotion_ArmCallback_9(3, 0xc000, 40);
    ObjectMotion_SetVariantCallbackAndInvokeObject_2(20, 2);
    BattleRuntime_WaitIfModeZero_8(20);
    BattleRuntime_RunThenWaitIfModeZero_5(0x4014, 0, 20);
    ObjectMotion_ArmCallback_10(3, 0xa000, 20);
    ObjectMotion_CallThenWaitForAnimationChange_4(3, 3);
    BattleRuntime_WaitIfModeZero_9(60);
    BattleEffect_SpawnLinkedResourceObject_1(3, 0x105, 60);
    BattleEffect_SpawnLinkedResourceObject_2(19, 0x101, 0);
    BattleEffect_SpawnLinkedResourceObject_3(20, 0x101, 60);
    ObjectMotion_SetVariantCallbackAndInvokeObject_3(19, 1);
    BattleRuntime_WaitIfModeZero_10(20);
    BattleRuntime_RunThenWaitIfModeZero_6(19, 0, 10);
    ObjectMotion_ArmCallback_11(3, 0xe000, 40);
    ObjectMotion_ArmCallback_12(3, 0xa000, 40);
    ObjectMotion_ArmCallback_13(3, 0xe000, 20);
    ObjectMotion_ArmCallback_14(3, 0x6000, 80);
    BattleRuntime_RunThenWaitIfModeZero_7(0x2003, 0, 20);
    ObjectMotion_ArmCallback_15(20, 0xf000, 0);
    ObjectMotion_ArmCallback_16(19, 0x7000, 40);
    ObjectMotion_ArmCallback_17(19, 0x5000, 0);
    ObjectMotion_ArmCallback_18(20, 0x3000, 20);
    ObjectMotion_SetSpeedParameters_3(20, 0x10000, 0x8000);
    *(u8 *)(Scene_GetRecord_2(20) + ACTOR_FLAGS_OFFSET) &= 254;
    ObjectMotion_SetPositionAndReset_2(20, 0x290, 166);
    BattleRuntime_WaitIfModeZero_11(1);
    *(u8 *)(Scene_GetRecord_3(20) + ACTOR_FLAGS_OFFSET) |= 1;
    BattleRuntime_WaitIfModeZero_12(20);
    BattleRuntime_RunThenWaitIfModeZero_8(0x4014, 0, 10);
    ObjectMotion_SetVariantCallbackAndInvokeObject_4(3, 2);
    BattleRuntime_WaitIfModeZero_13(40);
    ObjectMotion_ArmCallback_19(3, 0xa000, 10);
    BattleRuntime_RunThenWaitIfModeZero_9(0x2003, 0, 40);
    ObjectMotion_ArmCallback_20(3, 0x2000, 20);
    BattleRuntime_RunThenWaitIfModeZero_10(0x4003, 0, 10);
    BattleRuntime_WaitIfModeZero_14(19, 0x102);
    BattleRuntime_WaitIfModeZero_15(20, 0x102);
    BattleRuntime_WaitIfModeZero_16(40);
    ObjectMotion_ArmCallback_21(3, 0xc000, 20);
    Object_SetModeById_2(3, 4);
    BattleRuntime_RunThenWaitIfModeZero_11(0x2003, 0, 20);
    ObjectMotion_SetVariantCallbackAndInvokeObject_5(19, 1);
    BattleRuntime_RunThenWaitIfModeZero_12(19, 0, 10);
    ObjectMotion_ArmCallback_22(3, 0x2000, 40);
    ObjectMotion_ArmCallback_23(3, 0xc000, 20);
    ObjectMotion_CallThenWaitForAnimationChange_5(3, 3);
    BattleRuntime_WaitIfModeZero_17(20);
    ObjectMotion_SetVariantCallbackAndInvokeObject_6(20, 1);
    BattleRuntime_RunThenWaitIfModeZero_13(0x4014, 0, 20);
    ObjectMotion_SetVariantCallbackAndInvokeObject_7(3, 1);
    BattleRuntime_WaitIfModeZero_18(20);
    ObjectMotion_ArmCallback_24(3, 0xa000, 20);
    ObjectMotion_CallThenWaitForAnimationChange_6(3, 3);
    BattleRuntime_RunThenWaitIfModeZero_14(0x2003, 0, 80);
    BattleEffect_SpawnLinkedResourceObject_4(19, 0x105, 0);
    BattleEffect_SpawnLinkedResourceObject_5(20, 0x105, 60);
    ObjectMotion_CallThenWaitForAnimationChange_7(19, 4);
    BattleRuntime_RunThenWaitIfModeZero_15(19, 0, 10);
    Object_SetModeById_3(20, 4);
    BattleRuntime_RunThenWaitIfModeZero_16(0x4014, 0, 20);
    BattleEffect_SpawnLinkedResourceObject_6(3, 0x102, 60);
    BattleRuntime_RunThenWaitIfModeZero_17(0x2003, 0, 40);
    ObjectMotion_CallThenWaitForAnimationChange_8(19, 3);
    BattleRuntime_RunThenWaitIfModeZero_18(19, 0, 10);
    ObjectMotion_ArmCallback_25(3, 0xe000, 20);
    ObjectMotion_CallThenWaitForAnimationChange_9(20, 3);
    BattleRuntime_RunThenWaitIfModeZero_19(0x4014, 0, 10);
    ObjectMotion_ArmCallback_26(3, 0xa000, 60);
    ObjectMotion_ArmCallback_27(3, 0xe000, 20);
    ObjectMotion_ArmCallback_28(3, 0xa000, 20);
    ObjectMotion_ArmCallback_29(3, 0xc000, 40);
    ObjectMotion_CallThenWaitForAnimationChange_10(3, 3);
    BattleRuntime_RunThenWaitIfModeZero_20(0x2003, 0, 10);
    Object_SetModeById_4(19, 3);
    ObjectMotion_CallThenWaitForAnimationChange_11(20, 3);
    BattleRuntime_WaitIfModeZero_19(40);
    ObjectMotion_ArmCallback_30(3, 0x2000, 20);
    BattleRuntime_RunThenWaitIfModeZero_21(0x4003, 0, 20);
    ObjectMotion_ArmCallback_31(0, 0xa000, 20);
    ObjectMotion_CallThenWaitForAnimationChange_12(0, 3);
    BattleRuntime_WaitIfModeZero_20(20);
    ObjectMotion_SetPositionAndReset_3(3, 0x2b0, 200);
    ObjectMotion_SetHorizontalPositionWithTerrain_2(3, 0, 0);
    *(u8 *)(Scene_GetRecord_4(20) + ACTOR_FLAGS_OFFSET) &= 254;
    ObjectMotion_SetPositionAndReset_4(20, 0x284, 166);
    BattleRuntime_WaitIfModeZero_21(1);
    *(u8 *)(Scene_GetRecord_5(20) + ACTOR_FLAGS_OFFSET) |= 1;
    /* Advance the scene phase word to its next value. */
    *(s32 *)((*(u8 *volatile *)Data_03001ebc + 0x1c0)) = 0x209;
    GameFlag_Set_1(0x82e);
    GameFlag_Clear_1(0x82d);
    BattleRuntime_ScheduleShoulderButtonModeUpdate_1();
}
