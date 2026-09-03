#include "types.h"

#define FieldScene_RunTwoActorCutsceneSequence Func_02002008

/* Loader-relocated overlay calls: each symbol names the pre-relocation call
 * word the image holds. */
extern u8 Data_03001ebc[];
void Func_02005b3e();
void Func_02005b52();
void Func_02005bd0();
void Func_02005c06();
void Func_02005ca2();
void Func_02005cac();
void Func_02005cbc();
u8 *Scene_GetRecord_1();
void Func_02005cca();
u8 *Scene_GetRecord_2();
s32 Func_02005cde();
void Func_02005cea();
s32 Func_02005cf0();
void Func_02005d0e();
u8 *Scene_GetRecord_5();
u8 *Scene_GetRecord_6();
void Func_02005d38();
void Func_02005d3a();
void Func_02005d4c();
void Func_02005d52();
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
s32 Func_02005f84();
void Func_02005f98();
void Func_02005fbe();
void Func_02005fca();
void Func_02005fd4();
void Func_02005fda();
void Func_02005ff0();
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
u8 *ObjectMotion_SetSpeedParameters_5();
void Func_02006156();
u8 *ObjectMotion_SetSpeedParameters_6();
void Func_02006160();
void Func_02006166();
s32 Func_02006168();
void Func_0200616c();
void Func_02006170();
void Func_02006174();
s32 Func_0200617a();
void Func_02006180();
void Func_02006184();
void Func_02006186();
void Func_0200618a();
void Func_0200618c();
void Func_02006194();
void Func_020061a0();
void Func_020061a8();
u8 *Scene_GetRecord_12();
u8 *Scene_GetRecord_13();
void Func_020061bc();
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
void Func_020062d8();
void Func_020062e2();
void Func_0200633e();
void Func_020063b8();
void Func_020063be();
void Func_020063ca();

/* Resolved engine calls: each pseudo symbol is the per-site call word the
 * overlay image holds (a word can serve two sites with different targets),
 * and the macro names the engine function the site reaches through the
 * overlay veneer and the main-image veneer island, keeping the site's own
 * calling form. Names without a repository binding are provisional.
 */
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
#define Scene_GetRecord_3(args...) Func_02005cde(args)
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
#define BattleRuntime_WaitIfModeZero_5(args...) Func_02005d52(args)
#define Scene_GetRecord_7(a0) Value1(Func_02005d88, a0)
#define ObjectMotion_SetHorizontalPositionWithTerrain_2(args...) Func_02005dee(args)
#define ObjectMotion_SetSpeedParameters_4(a0, a1, a2) Call3(Func_02005db0, a0, a1, a2)
#define ObjectMotion_SetPositionAndReset_3(a0, a1, a2) Call3(Func_02005dec, a0, a1, a2)
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
#define BattleEventRuntime_ProcessAction_1(a0, a1) Call2(Func_02005f80, a0, a1)
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
#define ObjectMotion_SetVariantCallbackAndInvokeObject_7(args...) Func_02005ff0(args)
#define ObjectMotion_SetSpeedParameters_5(a0, a1) Value2(Func_02006020, a0, a1)
#define ObjectMotion_ArmCallback_18(a0, a1, a2) Call3(Func_0200604c, a0, a1, a2)
#define UiWork_WaitThenFinalizeCapacity_1(a0, a1) Value2(Func_02005f84, a0, a1)
#define ObjectMotion_SetVariantCallbackAndInvokeObject_8(args...) Func_02006018(args)
#define BattleRuntime_RunThenWaitIfModeZero_12(a0, a1, a2) Call3(Func_0200605a, a0, a1, a2)
#define ObjectMotion_CallThenWaitForAnimationChange_6(args...) Func_0200605a(args)
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
#define BattleEventRuntime_ProcessAction_2(a0, a1) Call2(Func_020061bc, a0, a1)
#define BattleRuntime_WaitIfModeZero_15(args...) Func_020060d2(args)
#define ObjectMotion_SetVariantCallbackAndInvokeObject_12(args...) Func_0200618a(args)
#define BattleRuntime_RunThenWaitIfModeZero_20(a0, a1, a2) Call3(Func_020061cc, a0, a1, a2)
#define ObjectMotion_CallThenWaitForAnimationChange_11(args...) Func_02006184(args)
#define ObjectMotion_CallThenWaitForAnimationChange_12(args...) Func_0200618c(args)
#define ObjectMotion_ArmCallback_26(a0, a1, a2) Call3(Func_020061f8, a0, a1, a2)
#define ObjectMotion_CallThenWaitForAnimationChange_13(args...) Func_020061a0(args)
#define ObjectMotion_SetSpeedParameters_6(args...) Func_02006152(args)
#define ObjectMotion_SetSpeedParameters_7(args...) Func_0200615c(args)
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
#define ObjectMotion_SetPositionAndReset_10(args...) Func_020062b6(args)
#define ObjectMotion_SetHorizontalPositionWithTerrain_5(args...) Func_020062d8(args)
#define ObjectMotion_SetHorizontalPositionWithTerrain_6(args...) Func_020062e2(args)
#define ObjectMotion_SetSpeedParameters_9(args...) Func_020063ca(args)
#define BattleRuntime_ScheduleShoulderButtonModeUpdate_1(args...) Func_02006294(args)
u8 *Func_02005cc0();
u8 *Func_02005ccc();
u8 *Func_02005d20();
u8 *Func_02005d30();
u8 *Func_02006152();
u8 *Func_0200615c();
u8 *Func_02006152();
u8 *Func_0200615c();
u8 *Func_020061aa();
u8 *Func_020061ba();

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

static __inline__ void Call4(void (*f)(), s32 a0, s32 a1, s32 a2, s32 a3)
{
    f(a0, a1, a2, a3);
}

static __inline__ void Call6(void (*f)(), s32 a0, s32 a1, s32 a2, s32 a3, s32 a4, s32 a5)
{
    f(a0, a1, a2, a3, a4, a5);
}

/* The scene step counter at 0x1d8 of the shared scene work record. */
static __inline__ void bump_step(s32 amount)
{
    u8 *work = *(u8 **)Data_03001ebc;

    *(u16 *)(work + 0x1d8) = (u16)(*(u16 *)(work + 0x1d8) + amount);
}

/* Field at 0x1c8 of the shared scene work record. */
#define SCENE_FIELD_1C8 (*(s32 *)(*(u8 **)Data_03001ebc + 0x1c8))

/* Field at 0x1c0 of the shared scene work record. */
#define SCENE_FIELD_1C0 (*(s32 *)(*(u8 **)Data_03001ebc + 0x1c0))

/* Runs a scripted sequence for two actors (8 and 9): sets up their sprite
 * records, moves and animates them in lockstep through a series of timed
 * steps, then hands off to a third actor (2) and a couple of standalone
 * calls (5) before advancing the shared scene step counter and phase word. */
void FieldScene_RunTwoActorCutsceneSequence(void)
{
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
        bump_step(1);
    } else {
        bump_step(1);
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
    Call3((void (*)())Func_02006152, 8, 0xcccc, 0x6666);
    Call3((void (*)())Func_0200615c, 9, 0xcccc, 0x6666);
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
