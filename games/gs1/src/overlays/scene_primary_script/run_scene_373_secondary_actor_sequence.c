#include "types.h"

/* Loader-relocated overlay calls: each symbol names the pre-relocation call
 * word the image holds. */
extern u8 Data_00001c45[];
void Func_02008c70();
void Func_02008cf2();
void Func_02008cfc();
s32 Func_02008d20();
void Func_02008d32();
void Func_02008d38();
void Func_02008d42();
void Func_02008d52();
void Func_02008d7c();
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
void Func_02008f08();
void Func_02008f14();
void Func_02008f1a();
void Func_02008f2c();
void Func_02008f32();

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
#define ObjectMotion_SetVariantCallback_1(args...) Func_02008d32(args)
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
#define ObjectMotion_SetPositionAndReset_2(a0, a1, a2) Call3(Func_02008d7c, a0, a1, a2)
#define ObjectMotion_ArmCallback_3(a0, a1, a2) Call3(Func_02008e10, a0, a1, a2)
#define BattleEvent_RunActionAndWait_1(a0, a1) Call2(Func_02008e00, a0, a1)
#define ObjectMotion_ArmCallback_4(a0, a1, a2) Call3(Func_02008e24, a0, a1, a2)
#define ObjectMotion_CallThenWaitForAnimationChange_1(args...) Func_02008dcc(args)
#define BattleEvent_RunActionAndWait_2(a0, a1) Call2(Func_02008e1c, a0, a1)
#define Object_SetModeById_1(args...) Func_02008dd4(args)
#define ObjectMotion_CallThenWaitForAnimationChange_2(args...) Func_02008de4(args)
#define BattleRuntime_WaitIfModeZero_1(args...) Func_02008d52(args)
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
#define BattleRuntime_RunThenWaitIfModeZero_4(a0, a1, a2) Call3(Func_02008eaa, a0, a1, a2)
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
#define BattleRuntime_WaitIfModeZero_3(args...) Func_02008e2e(args)
#define ObjectMotion_ArmCallback_6(a0, a1, a2) Call3(Func_02008f32, a0, a1, a2)
#define BattleRuntime_RunThenWaitIfModeZero_9(a0, a1, a2) Call3(Func_02008f2c, a0, a1, a2)
#define Object_SetModeById_2(args...) Func_02008edc(args)
#define ObjectMotion_CallThenWaitForAnimationChange_6(args...) Func_02008eec(args)
#define BattleRuntime_WaitIfModeZero_4(args...) Func_02008e5a(args)
#define ObjectMotion_CallThenWaitForAnimationChange_7(args...) Func_02008efa(args)
#define Object_SetModeById_3(args...) Func_02008efa(args)
#define Scene_GetRecord_2(a0) Value1(Func_02008ea0, a0)
#define ObjectMotion_ResetAndSetPosition_1(args...) Func_02008eda(args)
#define ObjectMotion_CommitCurrentPositionAndActivate_1(args...) Func_02008f08(args)
#define ObjectMotion_SetHorizontalPositionWithTerrain_2(args...) Func_02008f1a(args)
#define GameFlag_Set_1(a0) Call1(Func_02008e88, a0)
#define BattleRuntime_ScheduleShoulderButtonModeUpdate_1(args...) Func_02008eac(args)

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

/* The scene step counter at 0x1d8 of the shared scene work record. */
static __inline__ void bump_step(s32 amount)
{
    u8 *work = *(u8 **)0x03001ebc;

    *(u16 *)(work + 0x1d8) = (u16)(*(u16 *)(work + 0x1d8) + amount);
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
