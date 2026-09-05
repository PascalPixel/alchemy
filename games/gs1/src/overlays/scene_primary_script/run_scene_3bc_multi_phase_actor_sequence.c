#include "types.h"

/* Loader-relocated overlay calls: each symbol names the pre-relocation call
 * word the image holds. */
extern u8 Data_0200d950[];
void Func_02005d9e();
void Func_02005e7e();
void Func_02005e98();
void Func_02005ea2();
void Func_02005ea8();
s32 Func_02005ef0();
s32 Func_02005ef4();
void Func_02005f0e();
s32 Func_02005f14();
void Func_02005f22();
void Func_02005f2c();
void Func_02005f30();
void Func_02005f36();
void Func_02005f3a();
void Func_02005f3e();
void Func_02005f5c();
s32 Func_02005f5e();
s32 Func_02005f66();
s32 Func_02005f6e();
void Func_02005f8a();
void Func_02005f8c();
void Func_02005f9a();
void Func_02005fa8();
s32 Func_02005fba();
s32 Func_02005fc8();
void Func_02005fca();
void Func_02005fcc();
s32 Func_02005fda();
void Func_02005fdc();
void Func_02005fee();
s32 Func_02005ff0();
void Func_02005ff2();
void Func_02006002();
void Func_02006014();
s32 Func_0200601c();
void Func_02006028();
void Func_0200602a();
void Func_02006034();
void Func_0200603c();
void Func_0200603e();
void Func_02006052();
void Func_02006056();
void Func_0200606a();
void Func_0200607c();
void Func_0200607e();
void Func_0200608a();
void Func_0200608e();
void Func_02006092();
void Func_02006098();
void Func_020060a6();
void Func_020060b4();
void Func_020060bc();
void Func_020060be();
void Func_020060ce();
void Func_020060d0();
void Func_020060d2();
void Func_020060e2();
void Func_020060e4();
void Func_020060e6();
void Func_020060ee();
s32 Func_020060fc();
void Func_02006106();
void Func_02006112();
void Func_02006114();
void Func_02006138();
void Func_0200613a();
void Func_02006140();
void Func_02006146();
void Func_02006156();
s32 Func_0200615a();
void Func_02006168();
void Func_02006174();
void Func_02006186();
void Func_0200618a();
void Func_020061a4();
void Func_020061b0();
void Func_020061b4();
void Func_020061ba();
void Func_020061bc();
void Func_020061c0();
void Func_020061ca();
void Func_020061d6();
void Func_020061d8();
void Func_020061e2();
void Func_020061f4();
void Func_020061f6();
void Func_02006200();
void Func_02006208();
void Func_02006214();
void Func_02006220();
void Func_02006226();
void Func_0200622c();
void Func_02006238();
void Func_02006240();
void Func_02006244();
void Func_0200624e();
void Func_02006254();
void Func_0200625c();
void Func_02006270();
void Func_02006272();
void Func_02006278();
void Func_0200627e();
void Func_02006294();
void Func_0200629c();
void Func_020062a2();
void Func_020062c2();
void Func_020062c8();
void Func_020062e0();
void Func_020062f4();
void Func_020062fe();
void Func_0200630e();
void Func_0200631a();
void Func_02006320();
void Func_02006322();
void Func_02006332();
void Func_02006366();
void Func_0200636a();
void Func_02006370();
void Func_02006372();
void Func_02006378();
void Func_0200637c();
void Func_02006380();
void Func_0200638c();
void Func_02006396();
void Func_0200639a();
void Func_020063b8();
void Func_020063ca();
void Func_020063ce();
void Func_020063d0();
void Func_020063d8();
void Func_020063e6();
void Func_020063ec();
void Func_020063ee();
void Func_02006400();
void Func_0200640a();
void Func_02006418();
void Func_02006426();
void Func_0200643a();
void Func_02006472();
void Func_02006486();
void Func_0200649e();
void Func_020064a6();
void Func_020064ae();
void Func_020064e2();
void Func_020064ee();
void Func_020064f0();
void Func_0200650a();
void Func_02006518();
void Func_020093c1();

/* Resolved engine calls: each pseudo symbol is the per-site call word the
 * overlay image holds (a word can serve two sites with different targets),
 * and the macro names the engine function the site reaches through the
 * overlay veneer and the main-image veneer island, keeping the site's own
 * calling form. Names without a repository binding are provisional.
 */
#define ObjectTable_DestroyById_1(args...) Func_02005ea2(args)
#define ObjectTable_DestroyById_2(args...) Func_02005ea8(args)
#define Audio_PlayCue_1(args...) Func_02006034(args)
#define BattleRuntime_Reset_1(args...) Func_02005e98(args)
#define ObjectMotion_SetHorizontalPositionWithTerrain_1(a0, a1, a2) Call3(Func_02005f0e, a0, a1, a2)
#define Object_SetModeById_1(args...) Func_02005f22(args)
#define Object_SetModeById_2(args...) Func_02005f2c(args)
#define ObjectMotion_EnableActionAndSetCallback_1(a0, a1) Value2(Func_02005ef4, a0, a1)
#define ObjectMotion_SetHorizontalPositionWithTerrain_2(a0, a1, a2) Call3(Func_02005f3a, a0, a1, a2)
#define Scene_GetRecord_1(args...) Func_02005ef0(args)
#define ObjectMotion_EnableActionAndSetCallback_2(a0, a1) Value2(Func_02005f14, a0, a1)
#define Object_SetModeById_3(args...) Func_02005f5c(args)
#define ObjectMotion_SetSpeedParameters_1(a0, a1, a2) Call3(Func_02005f22, a0, a1, a2)
#define ObjectMotion_SetSpeedParameters_2(a0, a1, a2) Call3(Func_02005f30, a0, a1, a2)
#define ObjectMotion_SetSpeedParameters_3(a0, a1, a2) Call3(Func_02005f3e, a0, a1, a2)
#define ObjectMotion_SetHorizontalPositionWithTerrain_3(a0, a1, a2) Call3(Func_02005f8c, a0, a1, a2)
#define ObjectMotion_SetHorizontalPositionWithTerrain_4(a0, a1, a2) Call3(Func_02005f9a, a0, a1, a2)
#define ObjectMotion_SetHorizontalPositionWithTerrain_5(a0, a1, a2) Call3(Func_02005fa8, a0, a1, a2)
#define Scene_GetRecord_2(a0) Value1(Func_02005f5e, a0)
#define Scene_GetRecord_3(a0) Value1(Func_02005f66, a0)
#define Scene_GetRecord_4(a0) Value1(Func_02005f6e, a0)
#define ObjectMotion_SetPositionAndReset_1(args...) Func_02006056(args)
#define BattleRuntime_WaitIfModeZero_1(a0, a1) Call2(Func_020060be, a0, a1)
#define BattleRuntime_WaitIfModeZero_2(args...) Func_020060d2(args)
#define ObjectMotion_SetSpeedParameters_4(args...) Func_020060e6(args)
#define SceneWork_SetStepValue_1(a0) Call1(Func_0200603c, a0)
#define BattleRuntime_WaitIfModeZero_3(args...) Func_02005f8a(args)
#define ObjectMotion_EnableActionAndSetCallback_3(args...) Func_02005fcc(args)
#define Scene_GetRecord_5(args...) Func_02005fba(args)
#define Scene_GetRecord_6(args...) Func_02005fc8(args)
#define ObjectMotion_CallThenWaitForAnimationChange_1(args...) Func_02006034(args)
#define Scene_GetRecord_7(args...) Func_02005fda(args)
#define BattleRuntime_WaitIfModeZero_4(args...) Func_02005fca(args)
#define Scene_GetRecord_8(args...) Func_02005ff0(args)
#define BattleRuntime_WaitIfModeZero_5(args...) Func_02005fdc(args)
#define ObjectMotion_EnableActionAndSetCallback_4(a0, a1) Value2(Func_0200601c, a0, a1)
#define BattleEvent_RunActionAndWait_1(args...) Func_020060b4(args)
#define BattleRuntime_WaitIfModeZero_6(args...) Func_02005ff2(args)
#define ObjectMotion_SetPositionAndReset_2(a0, a1, a2) Call3(Func_02006056, a0, a1, a2)
#define ObjectMotion_ArmCallback_1(a0, a1, a2) Call3(Func_020060e2, a0, a1, a2)
#define BattleEffect_SpawnLinkedResourceObject_1(a0, a1, a2) Value3(Func_020060fc, a0, a1, a2)
#define BattleEvent_RunActionAndWait_2(args...) Func_020060e4(args)
#define Object_LinkObjectAndSetCallback_1(args...) Func_020061b4(args)
#define BattleRuntime_WaitIfModeZero_7(args...) Func_0200602a(args)
#define ObjectMotion_SetPositionAndReset_3(a0, a1, a2) Call3(Func_0200608e, a0, a1, a2)
#define ObjectMotion_ResetAndSetPositionInMode2_1(a0, a1, a2) Call3(Func_02006092, a0, a1, a2)
#define ObjectMotion_SetPositionAndReset_4(a0, a1, a2) Call3(Func_020060a6, a0, a1, a2)
#define Object_SetModeById_4(args...) Func_020060ce(args)
#define ObjectMotion_ArmCallback_2(a0, a1, a2) Call3(Func_0200613a, a0, a1, a2)
#define ObjectMotion_ArmCallback_3(a0, a1, a2) Call3(Func_02006146, a0, a1, a2)
#define ObjectMotion_SetVariantCallbackAndInvokeObject_1(args...) Func_02006106(args)
#define BattleRuntime_WaitIfModeZero_8(args...) Func_0200607c(args)
#define Object_LinkObjectAndSetCallback_2(args...) Func_02006214(args)
#define BattleRuntime_WaitIfModeZero_9(args...) Func_0200608a(args)
#define ObjectMotion_CallThenWaitForAnimationChange_2(args...) Func_02006112(args)
#define BattleRuntime_WaitIfModeZero_10(args...) Func_02006098(args)
#define BattleEvent_RunActionAndWait_3(args...) Func_02006168(args)
#define Object_LinkObjectAndSetCallback_3(args...) Func_02006238(args)
#define Object_LinkObjectAndSetCallback_4(args...) Func_02006240(args)
#define ObjectMotion_SetPositionAndReset_5(a0, a1, a2) Call3(Func_02006114, a0, a1, a2)
#define Object_LinkObjectAndSetCallback_5(args...) Func_02006254(args)
#define Object_LinkObjectAndSetCallback_6(args...) Func_0200625c(args)
#define BattleEffect_SpawnLinkedResourceObject_2(a0, a1, a2) Call3(Func_020061c0, a0, a1, a2)
#define Object_LinkObjectAndSetCallback_7(args...) Func_02006270(args)
#define Object_LinkObjectAndSetCallback_8(args...) Func_02006278(args)
#define BattleRuntime_WaitIfModeZero_11(args...) Func_020060ee(args)
#define ObjectMotion_SetVariantCallbackAndInvokeObject_2(args...) Func_02006186(args)
#define BattleRuntime_WaitIfModeZero_12(args...) ((void (*)())Func_020060fc)(args)
#define Object_LinkObjectAndSetCallback_9(args...) Func_02006294(args)
#define Object_LinkObjectAndSetCallback_10(args...) Func_0200629c(args)
#define BattleRuntime_WaitIfModeZero_13(args...) Func_02006112(args)
#define BattleEvent_RunActionAndWait_4(args...) Func_020061e2(args)
#define ObjectMotion_EnableActionAndSetCallback_5(a0, a1) Value2(Func_0200615a, a0, a1)
#define ObjectMotion_SetVariantCallbackAndInvokeObject_3(args...) Func_020061ba(args)
#define Object_LinkObjectAndSetCallback_11(args...) Func_020062c2(args)
#define BattleRuntime_WaitIfModeZero_14(args...) Func_02006138(args)
#define BattleEvent_RunActionAndWait_5(args...) Func_02006208(args)
#define ObjectMotion_SetVariantCallbackAndInvokeObject_4(args...) Func_020061d8(args)
#define Object_LinkObjectAndSetCallback_12(args...) Func_020062e0(args)
#define BattleRuntime_WaitIfModeZero_15(args...) Func_02006156(args)
#define BattleEvent_RunActionAndWait_6(args...) Func_02006226(args)
#define ObjectMotion_SetVariantCallbackAndInvokeObject_5(args...) Func_020061f6(args)
#define Object_LinkObjectAndSetCallback_13(args...) Func_020062fe(args)
#define BattleRuntime_WaitIfModeZero_16(args...) Func_02006174(args)
#define BattleEvent_RunActionAndWait_7(args...) Func_02006244(args)
#define ObjectMotion_EnableActionAndSetCallback_6(args...) Func_020061bc(args)
#define BattleRuntime_WaitIfModeZero_17(args...) Func_0200618a(args)
#define BattleRuntime_WaitIfModeZero_18(args...) Func_020061a4(args)
#define BattleRuntime_WaitIfModeZero_19(args...) Func_020061b0(args)
#define BattleEffect_SpawnLinkedResourceObject_3(a0, a1, a2) Call3(Func_020062a2, a0, a1, a2)
#define BattleRuntime_WaitIfModeZero_20(args...) Func_020061ca(args)
#define BattleRuntime_WaitIfModeZero_21(args...) Func_020061d6(args)
#define BattleEffect_SpawnLinkedResourceObject_4(a0, a1, a2) Call3(Func_020062c8, a0, a1, a2)
#define BattleRuntime_WaitIfModeZero_22(args...) Func_020061f4(args)
#define BattleRuntime_WaitIfModeZero_23(args...) Func_02006200(args)
#define BattleEffect_SpawnLinkedResourceObject_5(a0, a1, a2) Call3(Func_020062f4, a0, a1, a2)
#define BattleRuntime_WaitIfModeZero_24(args...) Func_02006220(args)
#define BattleRuntime_WaitIfModeZero_25(args...) Func_0200622c(args)
#define BattleEffect_SpawnLinkedResourceObject_6(a0, a1, a2) Call3(Func_02006320, a0, a1, a2)
#define Object_LinkObjectAndSetCallback_14(args...) Func_020063d0(args)
#define Object_LinkObjectAndSetCallback_15(args...) Func_020063d8(args)
#define BattleRuntime_WaitIfModeZero_26(args...) Func_0200624e(args)
#define Object_LinkObjectAndSetCallback_16(args...) Func_020063e6(args)
#define Object_LinkObjectAndSetCallback_17(args...) Func_020063ee(args)
#define BattleRuntime_WaitIfModeZero_27(args...) Func_02006272(args)
#define BattleRuntime_WaitIfModeZero_28(args...) Func_0200627e(args)
#define BattleEffect_SpawnLinkedResourceObject_7(a0, a1, a2) Call3(Func_02006372, a0, a1, a2)
#define ObjectMotion_SetVariantCallback_1(args...) Func_0200631a(args)
#define ObjectMotion_SetVariantCallback_2(args...) Func_02006322(args)
#define ObjectMotion_SetVariantCallbackAndInvokeObject_6(args...) Func_02006332(args)
#define Object_LinkObjectAndSetCallback_18(args...) Func_0200643a(args)
#define Object_LinkObjectAndSetCallback_19(args...) Func_02006472(args)
#define Object_SetModeById_5(args...) Func_02006370(args)
#define Object_SetModeById_6(args...) Func_02006378(args)
#define Object_SetModeById_7(args...) Func_02006380(args)
#define BattleRuntime_WaitIfModeZero_29(args...) Func_0200630e(args)
#define ObjectMotion_ResetAndSetPositionInMode2_2(a0, a1, a2) Call3(Func_0200636a, a0, a1, a2)
#define BattleRuntime_WaitIfModeZero_30(args...) Func_02006320(args)
#define ObjectMotion_ResetAndSetPositionInMode2_3(a0, a1, a2) Call3(Func_0200637c, a0, a1, a2)
#define BattleRuntime_WaitIfModeZero_31(args...) Func_02006332(args)
#define ObjectMotion_SetPositionAndReset_6(a0, a1, a2) Call3(Func_02006396, a0, a1, a2)
#define ObjectMotion_ResetAndSetPositionInMode2_4(a0, a1, a2) Call3(Func_0200639a, a0, a1, a2)
#define ObjectMotion_CommitCurrentPositionAndActivate_1(args...) Func_020063b8(args)
#define Object_SetModeById_8(args...) Func_020063d0(args)
#define Object_LinkObjectAndSetCallback_20(args...) Func_020064f0(args)
#define BattleRuntime_WaitIfModeZero_32(args...) Func_02006366(args)
#define ObjectMotion_SetPositionAndReset_7(a0, a1, a2) Call3(Func_020063ca, a0, a1, a2)
#define ObjectMotion_ResetAndSetPositionInMode2_5(a0, a1, a2) Call3(Func_020063ce, a0, a1, a2)
#define ObjectMotion_SetSpeedParameters_5(args...) Func_020064ee(args)
#define BattleRuntime_WaitIfModeZero_33(args...) Func_0200638c(args)
#define ObjectMotion_SetHorizontalPositionWithTerrain_6(a0, a1, a2) Call3(Func_0200640a, a0, a1, a2)
#define ObjectMotion_SetHorizontalPositionWithTerrain_7(a0, a1, a2) Call3(Func_02006418, a0, a1, a2)
#define ObjectMotion_SetHorizontalPositionWithTerrain_8(a0, a1, a2) Call3(Func_02006426, a0, a1, a2)
#define BattleRuntime_WaitIfModeZero_34(args...) Func_0200650a(args)
#define BattleRuntime_WaitIfModeZero_35(args...) Func_02006518(args)
#define SceneWork_SetStepValue_2(a0) Call1(Func_02006486, a0)
#define BattleEvent_RunActionAndWait_8(args...) Func_0200649e(args)
#define BattleEvent_RunActionAndWait_9(args...) Func_020064a6(args)
#define BattleEvent_RunActionAndWait_10(args...) Func_020064ae(args)
#define BattleRuntime_WaitIfModeZero_36(args...) Func_020063ec(args)
#define BattleRuntime_ScheduleShoulderButtonModeUpdate_1(args...) Func_02006400(args)

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

/* The scene step counter at 0x1d8 of the shared scene work record. */
static __inline__ void bump_step(s32 amount)
{
    u8 *work = *(u8 **)0x03001ebc;

    *(u16 *)(work + 0x1d8) = (u16)(*(u16 *)(work + 0x1d8) + amount);
}

/* Phase/status word at 0x1c0 of the shared scene work record. */
#define SCENE_PHASE (*(s32 *)(*(u8 **)0x03001ebc + 0x1c0))

/* Slot at 0x0200db80 set just before installing one of the callbacks below. */
#define PENDING_CALLBACK_FLAG (*(s32 *)0x0200db80)

/* Sets up actors 0-3 and a batch of movement/scale/rotation animation
 * records, then plays them through a long fixed sequence of moves,
 * rotations, and callback-driven steps. */
void FieldScene_RunMultiPhaseActorSequence(s32 a0)
{
    s32 record;
    s32 data_table_addr;
    s32 callback_target;

    ObjectTable_DestroyById_1(39);
    ObjectTable_DestroyById_2(40);
    Func_02005e7e(1);
    Audio_PlayCue_1(17);
    BattleRuntime_Reset_1();
    ObjectMotion_SetHorizontalPositionWithTerrain_1(8, 0x6080000, 0xc00000);
    if (a0 < 0) {
        Object_SetModeById_1(8, 10);
    } else {
        Object_SetModeById_2(8, 8);
    }
    ObjectMotion_EnableActionAndSetCallback_1(8, 0x200d668);
    ObjectMotion_SetHorizontalPositionWithTerrain_2(0, 0x5e00000, 0xc00000);
    record = Scene_GetRecord_1(0);
    {
        /* Clear the visibility/active flag at +6. */
        s32 shown = 0;

        *(u16 *)(record + 6) = shown;
    }
    ObjectMotion_EnableActionAndSetCallback_2(0, 0x200d738);
    Object_SetModeById_3(0, 35);
    ObjectMotion_SetSpeedParameters_1(1, 0x10000, 0x8000);
    ObjectMotion_SetSpeedParameters_2(2, 0x10000, 0x8000);
    ObjectMotion_SetSpeedParameters_3(3, 0x10000, 0x8000);
    ObjectMotion_SetHorizontalPositionWithTerrain_3(1, 0x5b80000, 0xb80000);
    ObjectMotion_SetHorizontalPositionWithTerrain_4(2, 0x5b80000, 0xc80000);
    ObjectMotion_SetHorizontalPositionWithTerrain_5(3, 0x5a80000, 0xc00000);
    record = Scene_GetRecord_2(1);
    {
        /* Clear the visibility/active flag at +6. */
        s32 shown = 0;

        *(u16 *)(record + 6) = shown;
    }
    record = Scene_GetRecord_3(2);
    {
        /* Clear the visibility/active flag at +6. */
        s32 shown = 0;

        *(u16 *)(record + 6) = shown;
    }
    record = Scene_GetRecord_4(3);
    {
        /* Clear the visibility/active flag at +6. */
        s32 shown = 0;

        *(u16 *)(record + 6) = shown;
    }
    Func_02005d9e(1);
    ObjectMotion_SetPositionAndReset_1(0, 0);
    SCENE_PHASE = 0x100;
    BattleRuntime_WaitIfModeZero_1(0x10001, 1);
    BattleRuntime_WaitIfModeZero_2();
    ObjectMotion_SetSpeedParameters_4();
    SceneWork_SetStepValue_1(0x20f1);
    BattleRuntime_WaitIfModeZero_3(60);
    data_table_addr = (s32)Data_0200d950;
    ObjectMotion_EnableActionAndSetCallback_3(0, data_table_addr);
    record = Scene_GetRecord_5(0);
    *(s32 *)(record + 24) = 0x10000;
    record = Scene_GetRecord_6(0);
    *(s32 *)(record + 28) = 0x10000;
    ObjectMotion_CallThenWaitForAnimationChange_1(0, 36);
    record = Scene_GetRecord_7(0);
    *(s32 *)(record + 8) += 0x30000;
    BattleRuntime_WaitIfModeZero_4(10);
    record = Scene_GetRecord_8(0);
    Func_02005f36(record, 0);
    BattleRuntime_WaitIfModeZero_5(20);
    ObjectMotion_EnableActionAndSetCallback_4(0, 0x200d808);
    BattleEvent_RunActionAndWait_1(1, 0);
    BattleRuntime_WaitIfModeZero_6(20);
    ObjectMotion_SetPositionAndReset_2(1, 0x5e0, 176);
    ObjectMotion_ArmCallback_1(1, 0x4000, 10);
    BattleEffect_SpawnLinkedResourceObject_1(1, 0x100, 20);
    BattleEvent_RunActionAndWait_2(2, 0);
    Object_LinkObjectAndSetCallback_1(1, 2);
    BattleRuntime_WaitIfModeZero_7(30);
    ObjectMotion_SetPositionAndReset_3(2, 0x5d0, 176);
    ObjectMotion_ResetAndSetPositionInMode2_1(1, 0x5f0, 184);
    ObjectMotion_SetPositionAndReset_4(2, 0x5e0, 176);
    Object_SetModeById_4(1, 1);
    ObjectMotion_ArmCallback_2(1, 0x6000, 0);
    ObjectMotion_ArmCallback_3(2, 0x4000, 10);
    ObjectMotion_SetVariantCallbackAndInvokeObject_1(2, 2);
    BattleRuntime_WaitIfModeZero_8(10);
    Object_LinkObjectAndSetCallback_2(2, 1);
    BattleRuntime_WaitIfModeZero_9(30);
    ObjectMotion_CallThenWaitForAnimationChange_2(1, 4);
    BattleRuntime_WaitIfModeZero_10(30);
    BattleEvent_RunActionAndWait_3(3, 0);
    Object_LinkObjectAndSetCallback_3(1, 3);
    Object_LinkObjectAndSetCallback_4(2, 3);
    ObjectMotion_SetPositionAndReset_5(3, 0x5d0, 184);
    Object_LinkObjectAndSetCallback_5(2, 0);
    Object_LinkObjectAndSetCallback_6(1, 0);
    BattleEffect_SpawnLinkedResourceObject_2(1, 0x102, 60);
    Object_LinkObjectAndSetCallback_7(2, 1);
    Object_LinkObjectAndSetCallback_8(1, 2);
    BattleRuntime_WaitIfModeZero_11(40);
    ObjectMotion_SetVariantCallbackAndInvokeObject_2(3, 2);
    BattleRuntime_WaitIfModeZero_12(10);
    Object_LinkObjectAndSetCallback_9(2, 3);
    Object_LinkObjectAndSetCallback_10(1, 3);
    BattleRuntime_WaitIfModeZero_13(20);
    BattleEvent_RunActionAndWait_4(3, 0);
    ObjectMotion_EnableActionAndSetCallback_5(0, 0x200d8ac);
    ObjectMotion_SetVariantCallbackAndInvokeObject_3(1, 2);
    Object_LinkObjectAndSetCallback_11(1, 0);
    BattleRuntime_WaitIfModeZero_14(20);
    BattleEvent_RunActionAndWait_5(1, 0);
    ObjectMotion_SetVariantCallbackAndInvokeObject_4(2, 2);
    Object_LinkObjectAndSetCallback_12(2, 0);
    BattleRuntime_WaitIfModeZero_15(20);
    BattleEvent_RunActionAndWait_6(2, 0);
    ObjectMotion_SetVariantCallbackAndInvokeObject_5(3, 2);
    Object_LinkObjectAndSetCallback_13(3, 0);
    BattleRuntime_WaitIfModeZero_16(20);
    BattleEvent_RunActionAndWait_7(3, 0);
    ObjectMotion_EnableActionAndSetCallback_6(0, data_table_addr);
    BattleRuntime_WaitIfModeZero_17(60);
    callback_target = (s32)Func_020093c1;
    PENDING_CALLBACK_FLAG = 9;
    Call2(Func_02005fee, callback_target, 0xc80);
    BattleRuntime_WaitIfModeZero_18(5);
    Func_02006002(callback_target);
    BattleRuntime_WaitIfModeZero_19(55);
    BattleEffect_SpawnLinkedResourceObject_3(1, 0x101, 60);
    Call2(Func_02006014, callback_target, 0xc80);
    BattleRuntime_WaitIfModeZero_20(20);
    Func_02006028(callback_target);
    BattleRuntime_WaitIfModeZero_21(40);
    BattleEffect_SpawnLinkedResourceObject_4(2, 0x101, 60);
    PENDING_CALLBACK_FLAG = 9;
    Call2(Func_0200603e, callback_target, 0xc80);
    BattleRuntime_WaitIfModeZero_22(35);
    Func_02006052(callback_target);
    BattleRuntime_WaitIfModeZero_23(25);
    BattleEffect_SpawnLinkedResourceObject_5(3, 0x102, 60);
    PENDING_CALLBACK_FLAG = 9;
    Call2(Func_0200606a, callback_target, 0xc80);
    BattleRuntime_WaitIfModeZero_24(35);
    Func_0200607e(callback_target);
    BattleRuntime_WaitIfModeZero_25(25);
    BattleEffect_SpawnLinkedResourceObject_6(2, 0x102, 60);
    Object_LinkObjectAndSetCallback_14(3, 2);
    Object_LinkObjectAndSetCallback_15(2, 3);
    BattleRuntime_WaitIfModeZero_26(60);
    Object_LinkObjectAndSetCallback_16(3, 0);
    Object_LinkObjectAndSetCallback_17(2, 0);
    PENDING_CALLBACK_FLAG = 9;
    Call2(Func_020060bc, callback_target, 0xc80);
    BattleRuntime_WaitIfModeZero_27(35);
    Func_020060d0(callback_target);
    BattleRuntime_WaitIfModeZero_28(25);
    BattleEffect_SpawnLinkedResourceObject_7(3, 0x108, 60);
    ObjectMotion_SetVariantCallback_1(1, 3);
    ObjectMotion_SetVariantCallback_2(2, 3);
    ObjectMotion_SetVariantCallbackAndInvokeObject_6(3, 3);
    Object_LinkObjectAndSetCallback_18(3, 2);
    Object_LinkObjectAndSetCallback_19(1, 2);
    PENDING_CALLBACK_FLAG = 9;
    Call2(Func_02006140, callback_target, 0xc80);
    Object_SetModeById_5(1, 3);
    Object_SetModeById_6(2, 3);
    Object_SetModeById_7(3, 3);
    BattleRuntime_WaitIfModeZero_29(60);
    ObjectMotion_ResetAndSetPositionInMode2_2(3, 0x5b8, 200);
    BattleRuntime_WaitIfModeZero_30(5);
    ObjectMotion_ResetAndSetPositionInMode2_3(2, 0x558, 184);
    BattleRuntime_WaitIfModeZero_31(3);
    ObjectMotion_SetPositionAndReset_6(1, 0x5e8, 184);
    ObjectMotion_ResetAndSetPositionInMode2_4(1, 0x558, 184);
    ObjectMotion_CommitCurrentPositionAndActivate_1(3);
    Object_SetModeById_8(3, 1);
    Object_LinkObjectAndSetCallback_20(3, 0);
    BattleRuntime_WaitIfModeZero_32(60);
    ObjectMotion_SetPositionAndReset_7(3, 0x598, 200);
    ObjectMotion_ResetAndSetPositionInMode2_5(3, 0x558, 184);
    Func_020064e2();
    ObjectMotion_SetSpeedParameters_5();
    BattleRuntime_WaitIfModeZero_33(30);
    ObjectMotion_SetHorizontalPositionWithTerrain_6(1, 0x5e80000, 0xb00000);
    ObjectMotion_SetHorizontalPositionWithTerrain_7(2, 0x5b80000, 0xc00000);
    ObjectMotion_SetHorizontalPositionWithTerrain_8(3, 0x6180000, 0xc80000);
    Func_02006322();
    BattleRuntime_WaitIfModeZero_34(0x10000, 2);
    BattleRuntime_WaitIfModeZero_35(1);
    SceneWork_SetStepValue_2(0x214c);
    BattleEvent_RunActionAndWait_8(1, 0);
    BattleEvent_RunActionAndWait_9(2, 0);
    BattleEvent_RunActionAndWait_10(3, 0);
    BattleRuntime_WaitIfModeZero_36(60);
    BattleRuntime_ScheduleShoulderButtonModeUpdate_1();
}
