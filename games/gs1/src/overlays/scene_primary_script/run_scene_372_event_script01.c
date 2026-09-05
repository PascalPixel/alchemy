#include "types.h"

/* Loader-relocated overlay calls: each symbol names the pre-relocation call
 * word the image holds. */
extern u8 Data_00000e9b[];
extern u8 Data_00000ea1[];
extern u8 Data_0200cec8[];
void Func_02006fea();
void Func_02007002();
s32 Func_02007008();
s32 Func_02007020();
s32 Func_02007022();
s32 Func_02007038();
s32 Func_0200703a();
s32 Func_02007052();
void Func_02007058();
void Func_02007070();
void Func_0200707a();
void Func_0200707c();
void Func_02007084();
s32 Func_0200708a();
void Func_0200708c();
void Func_02007094();
void Func_020070a4();
void Func_020070aa();
void Func_020070ac();
void Func_020070b8();
void Func_020070c4();
void Func_020070cc();
void Func_020070d0();
void Func_020070dc();
void Func_020070ea();
s32 Func_020070f4();
void Func_020070f6();
void Func_020070fe();
void Func_02007104();
s32 Func_0200710c();
s32 Func_02007124();
void Func_0200712c();
void Func_02007136();
void Func_0200713a();
void Func_02007146();
void Func_0200714e();
void Func_02007156();
void Func_02007158();
s32 Func_02007160();
void Func_02007164();
void Func_02007168();
void Func_02007176();
void Func_02007184();
void Func_0200718e();
void Func_0200719a();
void Func_020071a4();
void Func_020071a6();
void Func_020071c0();
void Func_020071ce();
void Func_020071de();
void Func_020071f2();
void Func_020071f4();
void Func_02007208();
void Func_02007232();
s32 Func_02007236();
void Func_0200723e();
void Func_02007266();
void Func_0200726e();
void Func_02007270();
void Func_0200727e();
void Func_02007280();
void Func_0200728c();
s32 Func_020072aa();
void Func_020072ae();
void Func_020072c0();
void Func_020072e0();
void Func_020072ea();
void Func_020072f6();
s32 Func_02007304();
void Func_02007314();
void Func_0200731a();
void Func_02007320();
void Func_02007328();
void Func_0200732e();
void Func_02007340();
void Func_02007344();
void Func_0200734a();
s32 Func_02007350();
void Func_02007354();
void Func_02007356();
void Func_02007364();
void Func_0200736c();
void Func_0200737a();
void Func_0200737c();
void Func_02007388();
void Func_02007394();
void Func_0200739e();
void Func_020073a4();
void Func_020073ac();
s32 Func_020073b4();
void Func_020073b8();
void Func_020073bc();
s32 Func_020073c0();
void Func_020073c6();
void Func_020073d4();
void Func_020073d8();
void Func_020073e8();
void Func_02007404();
void Func_0200740a();
void Func_0200740c();
void Func_0200741c();
void Func_0200741e();
void Func_02007422();
void Func_0200742e();
void Func_02007430();
void Func_02007436();
void Func_0200743e();
void Func_02007442();
void Func_02007448();

/* Resolved engine calls: each pseudo symbol is the per-site call word the
 * overlay image holds (a word can serve two sites with different targets),
 * and the macro names the engine function the site reaches through the
 * overlay veneer and the main-image veneer island, keeping the site's own
 * calling form. Names without a repository binding are provisional.
 */
#define ObjectMotion_ArmCallback_1(a0, a1, a2) Call3(Func_020070ac, a0, a1, a2)
#define ObjectMotion_ArmCallback_2(a0, a1, a2) Call3(Func_020070b8, a0, a1, a2)
#define ObjectMotion_ArmCallback_3(a0, a1, a2) Call3(Func_020070c4, a0, a1, a2)
#define ObjectMotion_ArmCallback_4(a0, a1, a2) Call3(Func_020070d0, a0, a1, a2)
#define ObjectMotion_ArmCallback_5(a0, a1, a2) Call3(Func_020070dc, a0, a1, a2)
#define Object_SetModeById_1(args...) Func_0200707c(args)
#define Object_SetModeById_2(args...) Func_02007084(args)
#define Object_SetModeById_3(args...) Func_0200708c(args)
#define Object_SetModeById_4(args...) Func_02007094(args)
#define ObjectMotion_CallThenWaitForAnimationChange_1(args...) Func_020070a4(args)
#define BattleRuntime_WaitIfModeZero_1(args...) Func_02007002(args)
#define ObjectMotion_SetSpeedLimitAndAcceleration_1(a0, a1) Call2(Func_02007136, a0, a1)
#define ObjectMotion_PlaceWithinCameraBounds_1(a0, a1, a2, a3) Call4(Func_0200714e, a0, a1, a2, a3)
#define ObjectMotion_SetSpeedParameters_1(a0, a1, a2) Call3(Func_02007070, a0, a1, a2)
#define ObjectMotion_SetSpeedParameters_2(a0, a1, a2) Call3(Func_0200707a, a0, a1, a2)
#define ObjectMotion_EnableActionAndSetCallback_1(a0, a1) Value2(Func_0200708a, a0, a1)
#define ObjectMotion_MarkActiveAndSetActionCallback_1(a0, a1) Call2(Func_020070aa, a0, a1)
#define Audio_PlayCue_1(args...) Func_02007208(args)
#define BattleRuntime_WaitIfModeZero_2(args...) Func_02007058(args)
#define ObjectMotion_SetPositionAndReset_1(a0, a1, a2) Call3(Func_020070ea, a0, a1, a2)
#define ObjectMotion_SetHorizontalPositionWithTerrain_1(args...) Func_02007104(args)
#define ObjectMotion_SetPositionAndReset_2(a0, a1, a2) Call3(Func_020070fe, a0, a1, a2)
#define BattleEffect_PlayQueuedSound_1(args...) Func_02007232(args)
#define ObjectMotion_ArmCallback_6(a0, a1, a2) Call3(Func_0200718e, a0, a1, a2)
#define ObjectMotion_ArmCallback_7(a0, a1, a2) Call3(Func_0200719a, a0, a1, a2)
#define ObjectMotion_ArmCallback_8(a0, a1, a2) Call3(Func_020071a6, a0, a1, a2)
#define Object_SetModeById_5(args...) Func_02007146(args)
#define Object_SetModeById_6(args...) Func_0200714e(args)
#define Object_SetModeById_7(args...) Func_02007156(args)
#define Scene_GetRecord_1(a0) Value1(Func_020070f4, a0)
#define Scene_GetRecord_2(a0) Value1(Func_0200710c, a0)
#define Scene_GetRecord_3(a0) Value1(Func_02007124, a0)
#define ObjectMotion_EnableActionAndSetCallback_2(args...) Func_02007158(args)
#define ObjectMotion_EnableActionAndSetCallback_3(a0, a1) Value2(Func_02007160, a0, a1)
#define ObjectMotion_EnableActionAndSetCallback_4(args...) Func_02007168(args)
#define Object_LookupAndStep_1(args...) Func_02007176(args)
#define BattleRuntime_WaitIfModeZero_3(args...) ((void (*)())Func_02007124)(args)
#define Audio_PlayCue_2(args...) Func_020072ea(args)
#define BattleRuntime_WaitIfModeZero_4(args...) Func_0200713a(args)
#define BattleEffect_PlayQueuedSound_2(args...) Func_020072f6(args)
#define ObjectMotion_PlaceWithinCameraBounds_2(a0, a1, a2, a3) Call4(Func_0200727e, a0, a1, a2, a3)
#define Audio_PlayCue_3(args...) Func_02007314(args)
#define BattleRuntime_WaitIfModeZero_5(args...) Func_02007164(args)
#define BattleEffect_PlayQueuedSound_3(args...) Func_02007320(args)
#define ObjectMotion_EnableActionAndSetCallback_5(a0, a1) Call2(Func_020071c0, a0, a1)
#define BattleRuntime_WaitIfModeZero_6(args...) Func_02007176(args)
#define ObjectMotion_EnableActionAndSetCallback_6(a0, a1) Call2(Func_020071ce, a0, a1)
#define BattleRuntime_WaitIfModeZero_7(args...) Func_02007184(args)
#define Audio_PlayCue_4(args...) Func_0200734a(args)
#define Object_LookupAndStep_2(args...) Func_020071f2(args)
#define BattleEffect_PlayQueuedSound_4(args...) Func_02007356(args)
#define BattleRuntime_WaitIfModeZero_8(args...) Func_020071a4(args)
#define SceneWork_SetStepValue_1(args...) Func_0200728c(args)
#define BattleRuntime_RunThenWaitIfModeZero_1(args...) Func_020072ae(args)
#define ObjectMotion_CallThenWaitForAnimationChange_2(args...) Func_02007266(args)
#define BattleRuntime_RunThenWaitIfModeZero_2(a0, a1, a2) Call3(Func_020072c0, a0, a1, a2)
#define Object_SetModeById_8(args...) Func_02007270(args)
#define ObjectMotion_CallThenWaitForAnimationChange_3(args...) Func_02007280(args)
#define BattleRuntime_WaitIfModeZero_9(args...) Func_020071de(args)
#define ObjectMotion_EnableActionAndSetCallback_7(a0, a1) Value2(Func_02007236, a0, a1)
#define ObjectMotion_EnableActionAndSetCallback_8(a0, a1) Call2(Func_0200723e, a0, a1)
#define BattleRuntime_WaitIfModeZero_10(args...) Func_020071f4(args)
#define ObjectMotion_SetSpeedLimitAndAcceleration_2(a0, a1) Call2(Func_02007328, a0, a1)
#define ObjectMotion_PlaceWithinCameraBounds_3(a0, a1, a2, a3) Call4(Func_02007340, a0, a1, a2, a3)
#define Object_LookupAndStep_3(args...) Func_0200726e(args)
#define ObjectMotion_ArmCallback_9(args...) Func_02007328(args)
#define BattleEffect_SpawnLinkedResourceObject_1(a0, a1, a2) Call3(Func_02007344, a0, a1, a2)
#define BattleRuntime_RunThenWaitIfModeZero_3(args...) Func_0200732e(args)
#define ObjectMotion_ArmCallback_10(a0, a1, a2) Call3(Func_0200734a, a0, a1, a2)
#define ObjectMotion_ArmCallback_11(a0, a1, a2) Call3(Func_02007356, a0, a1, a2)
#define ObjectMotion_SetPositionAndReset_3(a0, a1, a2) Call3(Func_020072e0, a0, a1, a2)
#define ObjectMotion_SetVariantCallbackAndInvokeObject_1(args...) Func_02007320(args)
#define ObjectMotion_SetSpeedParameters_3(a0, a1) Value2(Func_02007350, a0, a1)
#define ObjectMotion_ArmCallback_12(args...) Func_0200737a(args)
#define UiWork_WaitThenFinalizeCapacity_1(a0, a1) Value2(Func_020072aa, a0, a1)
#define ObjectMotion_CallThenWaitForAnimationChange_4(args...) Func_0200732e(args)
#define SceneWork_SetStepValue_2(args...) Func_0200736c(args)
#define ObjectMotion_SetVariantCallbackAndInvokeObject_2(args...) Func_02007356(args)
#define SceneWork_SetStepValue_3(args...) Func_0200737c(args)
#define BattleEvent_RunActionAndWait_1(a0, a1) Call2(Func_02007394, a0, a1)
#define ObjectMotion_ArmCallback_13(a0, a1, a2) Call3(Func_020073b8, a0, a1, a2)
#define BattleEffect_SpawnLinkedResourceObject_2(a0, a1, a2) Call3(Func_020073d4, a0, a1, a2)
#define SceneWork_SetStepValue_4(args...) Func_020073a4(args)
#define ObjectMotion_SetSpeedParameters_4(a0, a1) Value2(Func_020073b4, a0, a1)
#define UiWork_WaitThenFinalizeCapacity_2(a0, a1) Value2(Func_02007304, a0, a1)
#define ObjectMotion_CallThenWaitForAnimationChange_5(args...) Func_02007388(args)
#define SceneWork_SetStepValue_5(args...) Func_020073c6(args)
#define BattleRuntime_RunThenWaitIfModeZero_4(a0, a1, a2) Call3(Func_020073e8, a0, a1, a2)
#define ObjectMotion_ArmCallback_14(a0, a1, a2) Call3(Func_02007404, a0, a1, a2)
#define Object_SetModeById_9(args...) Func_020073a4(args)
#define Object_SetModeById_10(args...) Func_020073ac(args)
#define ObjectMotion_CallThenWaitForAnimationChange_6(args...) Func_020073bc(args)
#define BattleRuntime_WaitIfModeZero_11(args...) Func_0200731a(args)
#define BattleEffect_SpawnLinkedResourceObject_3(a0, a1, a2) Call3(Func_0200743e, a0, a1, a2)
#define BattleEffect_SpawnLinkedResourceObject_4(a0, a1, a2) Call3(Func_02007448, a0, a1, a2)
#define Object_SetModeById_11(args...) Func_020073d8(args)
#define SceneWork_SetStepValue_6(args...) Func_0200741e(args)
#define BattleEvent_RunActionAndWait_2(a0, a1) Call2(Func_02007436, a0, a1)
#define ObjectMotion_EnableActionAndSetCallback_9(a0, a1) Call2(Func_0200739e, a0, a1)
#define BattleRuntime_WaitIfModeZero_12(args...) Func_02007354(args)
#define Object_LinkPair_1(args...) Func_0200742e(args)
#define BattleRuntime_WaitIfModeZero_13(args...) Func_02007364(args)
#define Object_SetModeById_12(args...) Func_0200740c(args)
#define ObjectMotion_CallThenWaitForAnimationChange_7(args...) Func_0200741c(args)
#define BattleRuntime_WaitIfModeZero_14(args...) Func_0200737a(args)
#define Object_SetModeById_13(args...) Func_02007422(args)
#define Scene_GetRecord_4(a0) Value1(Func_020073c0, a0)
#define ObjectMotion_ResetAndSetPosition_1(args...) Func_0200740a(args)
#define ObjectMotion_CommitCurrentPositionAndActivate_1(args...) Func_02007430(args)
#define ObjectMotion_SetHorizontalPositionWithTerrain_2(args...) Func_02007442(args)

/* Call sites spelled through these wrappers pass their constants straight
 * into the argument registers; a direct call precomputes a costly constant
 * into a pseudo that the compiler then shares with later uses in the block.
 * A value-returning call also sets r0 last of its arguments. */

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

/* The scene step counter at 0x1d8 of the shared scene work record. */
static __inline__ void bump_step(s32 amount)
{
    u8 *work = *(u8 **)0x03001ebc;

    *(u16 *)(work + 0x1d8) = (u16)(*(u16 *)(work + 0x1d8) + amount);
}

/* Sets up records 26, 24, 25, 9 and 10 (position/speed, facing, movement),
 * runs several timed particle/object sequences against constant tables, then
 * takes two branches whose outcome picks entries out of the Data_00000e9b
 * and Data_00000ea1 byte tables to drive further record 9/22 setup calls. */
void RunEventScript01(void)
{
    u32 i;
    s32 entry;
    s32 record;
    s32 base5_200cec8;
    s32 base5_e9b;
    s32 base5_ea1;

    ObjectMotion_ArmCallback_1(26, 0x3000, 0);
    ObjectMotion_ArmCallback_2(24, 0xd000, 0);
    ObjectMotion_ArmCallback_3(25, 0xb000, 0);
    ObjectMotion_ArmCallback_4(9, 0x3000, 0);
    ObjectMotion_ArmCallback_5(10, 0xd000, 20);
    Object_SetModeById_1(26, 3);
    Object_SetModeById_2(24, 3);
    Object_SetModeById_3(25, 3);
    Object_SetModeById_4(9, 3);
    ObjectMotion_CallThenWaitForAnimationChange_1(25, 3);
    BattleRuntime_WaitIfModeZero_1(20);
    ObjectMotion_SetSpeedLimitAndAcceleration_1(0x10000, 0x2000);
    ObjectMotion_PlaceWithinCameraBounds_1(0x860000, -1, 0x4ab0000, 1);
    ObjectMotion_SetSpeedParameters_1(26, 0x19999, 0xcccc);
    ObjectMotion_SetSpeedParameters_2(9, 0x19999, 0xcccc);
    ObjectMotion_EnableActionAndSetCallback_1(26, 0x200cab4);
    ObjectMotion_MarkActiveAndSetActionCallback_1(9, 0x200ca78);
    Audio_PlayCue_1(158);
    Call3(Func_02006fea, 0x200d7a0, 38, 72);
    BattleRuntime_WaitIfModeZero_2(10);
    ObjectMotion_SetPositionAndReset_1(9, 149, 0x497);
    ObjectMotion_SetHorizontalPositionWithTerrain_1(9, 0, 0);
    ObjectMotion_SetPositionAndReset_2(25, 250, 0x4be);
    BattleEffect_PlayQueuedSound_1();
    ObjectMotion_ArmCallback_6(10, 0x3000, 0);
    ObjectMotion_ArmCallback_7(24, 0x3000, 0);
    ObjectMotion_ArmCallback_8(25, 0x3000, 0);
    Object_SetModeById_5(10, 5);
    Object_SetModeById_6(24, 6);
    Object_SetModeById_7(25, 6);
    /* For records 10, 24 and 25: fetch the record's entry pointer, fetch a
     * value from that record's own state, and store a derived value into
     * the entry's field at offset 100. */
    entry = Scene_GetRecord_1(10);
    record = Func_02007022();
    *(u16 *)(entry + 100) = (Func_02007008(record, 90) + 60);
    entry = Scene_GetRecord_2(24);
    record = Func_0200703a();
    *(u16 *)(entry + 100) = (Func_02007020(record, 90) + 60);
    entry = Scene_GetRecord_3(25);
    record = Func_02007052();
    *(u16 *)(entry + 100) = (Func_02007038(record, 90) + 60);
    base5_200cec8 = (s32)Data_0200cec8;
    ObjectMotion_EnableActionAndSetCallback_2(10, base5_200cec8);
    ObjectMotion_EnableActionAndSetCallback_3(24, base5_200cec8);
    ObjectMotion_EnableActionAndSetCallback_4(25, base5_200cec8);
    Object_LookupAndStep_1(26);
    BattleRuntime_WaitIfModeZero_3(10);
    Audio_PlayCue_2(159);
    Call3(Func_020070cc, 0x200d7e2, 38, 72);
    BattleRuntime_WaitIfModeZero_4(30);
    BattleEffect_PlayQueuedSound_2();
    ObjectMotion_PlaceWithinCameraBounds_2(0x700000, -1, 0x4c90000, 1);
    Audio_PlayCue_3(158);
    Call3(Func_020070f6, 0x200d78a, 35, 73);
    BattleRuntime_WaitIfModeZero_5(20);
    BattleEffect_PlayQueuedSound_3();
    ObjectMotion_EnableActionAndSetCallback_5(9, 0x200cb28);
    BattleRuntime_WaitIfModeZero_6(20);
    ObjectMotion_EnableActionAndSetCallback_6(26, 0x200cb9c);
    BattleRuntime_WaitIfModeZero_7(40);
    Audio_PlayCue_4(159);
    Call3(Func_0200712c, 0x200d7cc, 35, 73);
    Object_LookupAndStep_2(26);
    BattleEffect_PlayQueuedSound_4();
    BattleRuntime_WaitIfModeZero_8(40);
    base5_e9b = (s32)Data_00000e9b;
    SceneWork_SetStepValue_1(base5_e9b);
    BattleRuntime_RunThenWaitIfModeZero_1(9, 0, 20);
    ObjectMotion_CallThenWaitForAnimationChange_2(26, 3);
    BattleRuntime_RunThenWaitIfModeZero_2(0x201a, 0, 40);
    Object_SetModeById_8(9, 3);
    ObjectMotion_CallThenWaitForAnimationChange_3(26, 3);
    BattleRuntime_WaitIfModeZero_9(30);
    ObjectMotion_EnableActionAndSetCallback_7(9, 0x200cc0c);
    ObjectMotion_EnableActionAndSetCallback_8(26, 0x200cc5c);
    BattleRuntime_WaitIfModeZero_10(40);
    ObjectMotion_SetSpeedLimitAndAcceleration_2(0x20000, 0x4000);
    ObjectMotion_PlaceWithinCameraBounds_3(0x690000, -1, 0x43e0000, 1);
    Object_LookupAndStep_3(9);
    ObjectMotion_ArmCallback_9(9, 0, 0);
    BattleEffect_SpawnLinkedResourceObject_1(9, 0x100, 40);
    BattleRuntime_RunThenWaitIfModeZero_3(9, 0, 10);
    ObjectMotion_ArmCallback_10(0, 0x8000, 0);
    ObjectMotion_ArmCallback_11(22, 0x8000, 10);
    ObjectMotion_SetPositionAndReset_3(9, 105, 0x43e);
    ObjectMotion_SetVariantCallbackAndInvokeObject_1(9, 2);
    ObjectMotion_SetSpeedParameters_3(0x8009, 0);
    ObjectMotion_ArmCallback_12(22, 0, 0);
    /* Branch on a condition; pass byte 4 or byte 5 of the Data_00000e9b
     * table to the corresponding follow-up call. */
    if (UiWork_WaitThenFinalizeCapacity_1(0, 0) == 0) {
        ObjectMotion_CallThenWaitForAnimationChange_4(9, 3);
        SceneWork_SetStepValue_2((base5_e9b + 4));
    } else {
        ObjectMotion_SetVariantCallbackAndInvokeObject_2(9, 2);
        SceneWork_SetStepValue_3((base5_e9b + 5));
    }
    BattleEvent_RunActionAndWait_1(0x8009, 0);
    ObjectMotion_ArmCallback_13(22, 0x8000, 40);
    BattleEffect_SpawnLinkedResourceObject_2(9, 0x100, 30);
    base5_ea1 = (s32)Data_00000ea1;
    SceneWork_SetStepValue_4(base5_ea1);
    ObjectMotion_SetSpeedParameters_4(0x8009, 0);
    /* Branch on a condition; each side reads a different byte of the
     * Data_00000ea1 table and runs its own follow-up sequence. */
    if (UiWork_WaitThenFinalizeCapacity_2(0, 0) == 0) {
        ObjectMotion_CallThenWaitForAnimationChange_5(9, 3);
        SceneWork_SetStepValue_5((base5_ea1 + 1));
        BattleRuntime_RunThenWaitIfModeZero_4(0x8009, 0, 30);
        ObjectMotion_ArmCallback_14(22, 0x8000, 20);
        Object_SetModeById_9(0, 3);
        Object_SetModeById_10(22, 3);
        ObjectMotion_CallThenWaitForAnimationChange_6(9, 3);
        BattleRuntime_WaitIfModeZero_11(40);
    } else {
        BattleEffect_SpawnLinkedResourceObject_3(9, 0x105, 90);
        BattleEffect_SpawnLinkedResourceObject_4(9, 0x103, 40);
        Object_SetModeById_11(9, 4);
        SceneWork_SetStepValue_6((base5_ea1 + 2));
        BattleEvent_RunActionAndWait_2(0x8009, 0);
    }
    ObjectMotion_EnableActionAndSetCallback_9(9, 0x200cca8);
    BattleRuntime_WaitIfModeZero_12(90);
    Object_LinkPair_1(0, 22, 0);
    BattleRuntime_WaitIfModeZero_13(40);
    Object_SetModeById_12(0, 3);
    ObjectMotion_CallThenWaitForAnimationChange_7(22, 3);
    BattleRuntime_WaitIfModeZero_14(20);
    Object_SetModeById_13(22, 2);
    /* If a record pointer is returned, pass its s16 fields at offsets 10
     * and 18 through to the follow-up call. */
    record = Scene_GetRecord_4(0);
    if (record != 0) {
        ObjectMotion_ResetAndSetPosition_1(22, *(s16 *)(record + 10), *(s16 *)(record + 18));
    }
    ObjectMotion_CommitCurrentPositionAndActivate_1(22);
    ObjectMotion_SetHorizontalPositionWithTerrain_2(22, 0, 0);
}
