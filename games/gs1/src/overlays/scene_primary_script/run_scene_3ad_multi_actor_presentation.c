#include "types.h"

#define FieldScene_RunMultiActorPresentation Func_020004a8

/* Loader-relocated overlay calls: each symbol names the pre-relocation call
 * word the image holds. */
extern u8 Data_0000250d[];
extern u8 Data_00002512[];
extern u8 Data_03001ebc[];
void Func_02001026();
s32 Func_02001fb2();
void Func_02001fc6();
void Func_02001fe2();
void Func_0200201e();
void Func_0200202a();
void Func_02002038();
void Func_0200203a();
void Func_02002050();
void Func_02002068();
s32 Func_0200206e();
void Func_0200208e();
void Func_02002094();
s32 Func_020020aa();
void Func_020020ac();
void Func_020020bc();
void Func_020020c0();
void Func_020020ca();
s32 Func_020020e6();
void Func_020020e8();
void Func_020020fc();
void Func_020020fe();
void Func_02002106();
void Func_02002118();
s32 Func_02002122();
void Func_02002124();
void Func_02002138();
void Func_0200213e();
void Func_02002142();
void Func_0200215c();
void Func_0200215e();
void Func_02002160();
void Func_0200216a();
void Func_02002174();
void Func_0200219c();
void Func_020021b6();
void Func_020021c0();
void Func_020021c4();
void Func_020021d6();
void Func_020021d8();
s32 Func_020021e0();
void Func_020021ee();
void Func_020021f8();
void Func_02002206();
void Func_02002210();
void Func_02002226();
void Func_02002228();
void Func_02002232();
void Func_02002234();
s32 Func_02002236();
void Func_0200225a();
void Func_0200226a();
void Func_02002272();
void Func_02002276();
void Func_0200227e();
void Func_02002282();
void Func_02002286();
void Func_0200228e();
void Func_0200229e();
void Func_020022a6();
void Func_020022ae();
void Func_020022b6();
void Func_020022b8();
void Func_020022d0();
void Func_020022d2();
void Func_020022de();
void Func_020022e0();
void Func_020022ea();
void Func_020022f6();
void Func_02002304();
void Func_02002306();
void Func_02002308();
void Func_02002312();
void Func_0200231e();
void Func_02002320();
void Func_02002326();
void Func_02002336();
void Func_02002338();
void Func_02002342();
void Func_02002372();
void Func_02002376();
void Func_0200238a();
void Func_02002394();
void Func_020023cc();
void Func_02002400();
void Func_0200240c();

/* Resolved engine calls: each pseudo symbol is the per-site call word the
 * overlay image holds (a word can serve two sites with different targets),
 * and the macro names the engine function the site reaches through the
 * overlay veneer and the main-image veneer island, keeping the site's own
 * calling form. Names without a repository binding are provisional.
 */
#define GameFlag_IsSet_1(a0) Value1(Func_02001fb2, a0)
#define GameFlag_Set_1(a0) Call1(Func_02001fc6, a0)
#define BattleRuntime_Reset_1(args...) Func_02001fe2(args)
#define ObjectMotion_SetHorizontalPositionWithTerrain_1(a0, a1, a2) Call3(Func_02002038, a0, a1, a2)
#define ObjectMotion_SetSpeedParameters_1(a0, a1, a2) Call3(Func_0200201e, a0, a1, a2)
#define ObjectMotion_ResetAndSetPositionInMode2_1(a0, a1, a2) Call3(Func_0200203a, a0, a1, a2)
#define ObjectMotion_CommitCurrentPositionAndActivate_1(args...) Func_02002050(args)
#define Object_SetModeById_1(args...) Func_02002068(args)
#define ObjectMotion_ArmCallback_1(a0, a1, a2) Call3(Func_020020bc, a0, a1, a2)
#define ObjectMotion_SetVariantCallbackAndInvokeObject_1(args...) Func_02002094(args)
#define BattleRuntime_WaitIfModeZero_1(args...) Func_0200202a(args)
#define ObjectMotion_SetSpeedLimitAndAcceleration_1(a0, a1) Call2(Func_020020fe, a0, a1)
#define ObjectMotion_PlaceWithinCameraBounds_1(a0, a1, a2, a3) Call4(Func_02002118, a0, a1, a2, a3)
#define Scene_GetRecord_1(a0) Value1(Func_0200206e, a0)
#define ObjectMotion_SetHorizontalPositionWithTerrain_2(args...) Func_020020ac(args)
#define ObjectMotion_SetSpeedParameters_2(a0, a1, a2) Call3(Func_0200208e, a0, a1, a2)
#define ObjectMotion_ResetAndSetPositionInMode2_2(a0, a1, a2) Call3(Func_020020aa, a0, a1, a2)
#define ObjectMotion_CommitCurrentPositionAndActivate_2(args...) Func_020020c0(args)
#define ObjectMotion_ArmCallback_2(a0, a1, a2) Call3(Func_02002124, a0, a1, a2)
#define Scene_GetRecord_2(a0) Value1(Func_020020aa, a0)
#define ObjectMotion_SetHorizontalPositionWithTerrain_3(args...) Func_020020e8(args)
#define ObjectMotion_SetSpeedParameters_3(a0, a1, a2) Call3(Func_020020ca, a0, a1, a2)
#define ObjectMotion_ResetAndSetPositionInMode2_3(a0, a1, a2) Call3(Func_020020e6, a0, a1, a2)
#define ObjectMotion_CommitCurrentPositionAndActivate_3(args...) Func_020020fc(args)
#define ObjectMotion_ArmCallback_3(a0, a1, a2) Call3(Func_02002160, a0, a1, a2)
#define Scene_GetRecord_3(a0) Value1(Func_020020e6, a0)
#define ObjectMotion_SetHorizontalPositionWithTerrain_4(args...) Func_02002124(args)
#define ObjectMotion_SetSpeedParameters_4(a0, a1, a2) Call3(Func_02002106, a0, a1, a2)
#define ObjectMotion_ResetAndSetPositionInMode2_4(a0, a1, a2) Call3(Func_02002122, a0, a1, a2)
#define ObjectMotion_CommitCurrentPositionAndActivate_4(args...) Func_02002138(args)
#define ObjectMotion_ArmCallback_4(a0, a1, a2) Call3(Func_0200219c, a0, a1, a2)
#define Scene_GetRecord_4(a0) Value1(Func_02002122, a0)
#define ObjectMotion_SetHorizontalPositionWithTerrain_5(args...) Func_02002160(args)
#define ObjectMotion_SetSpeedParameters_5(a0, a1, a2) Call3(Func_02002142, a0, a1, a2)
#define ObjectMotion_ResetAndSetPositionInMode2_5(a0, a1, a2) Call3(Func_0200215e, a0, a1, a2)
#define ObjectMotion_CommitCurrentPositionAndActivate_5(args...) Func_02002174(args)
#define ObjectMotion_ArmCallback_5(a0, a1, a2) Call3(Func_020021d8, a0, a1, a2)
#define BattleRuntime_WaitIfModeZero_2(args...) Func_0200213e(args)
#define ObjectMotion_SetVariantCallbackAndInvokeObject_2(args...) Func_020021b6(args)
#define SceneWork_SetStepValue_1(args...) Func_020021d6(args)
#define BattleEvent_RunActionAndWait_1(args...) Func_020021ee(args)
#define BattleRuntime_WaitIfModeZero_3(args...) Func_0200215c(args)
#define ObjectMotion_CallThenWaitForAnimationChange_1(args...) Func_020021c4(args)
#define BattleRuntime_WaitIfModeZero_4(args...) Func_0200216a(args)
#define SceneWork_SetStepValue_2(args...) Func_020021f8(args)
#define BattleEvent_RunActionAndWait_2(args...) Func_02002210(args)
#define BattleEffect_SpawnLinkedResourceObject_1(a0, a1, a2) Call3(Func_02002234, a0, a1, a2)
#define ObjectMotion_SetAngleToward_1(args...) Func_02002206(args)
#define ObjectMotion_SetAngleToward_2(args...) Func_02002210(args)
#define SceneWork_SetStepValue_3(args...) Func_02002226(args)
#define ObjectMotion_SetSpeedParameters_6(a0, a1) Value2(Func_02002236, a0, a1)
#define ObjectMotion_SetAngleToward_3(args...) Func_02002228(args)
#define ObjectMotion_SetAngleToward_4(args...) Func_02002232(args)
#define BattleRuntime_WaitIfModeZero_5(args...) Func_020021c0(args)
#define UiWork_WaitThenFinalizeCapacity_1(a0, a1) Value2(Func_020021e0, a0, a1)
#define SceneWork_SetStepValue_4(args...) Func_0200225a(args)
#define BattleEvent_RunActionAndWait_3(args...) Func_02002272(args)
#define SceneWork_SetStepValue_5(args...) Func_0200226a(args)
#define BattleEvent_RunActionAndWait_4(args...) Func_02002282(args)
#define BattleEffect_SpawnLinkedResourceObject_2(a0, a1, a2) Call3(Func_020022a6, a0, a1, a2)
#define SceneWork_SetStepValue_6(args...) Func_02002286(args)
#define BattleEvent_RunActionAndWait_5(args...) Func_0200229e(args)
#define ObjectMotion_SetVariantCallback_1(args...) Func_02002276(args)
#define ObjectMotion_SetVariantCallback_2(args...) Func_0200227e(args)
#define ObjectMotion_SetVariantCallback_3(args...) Func_02002286(args)
#define ObjectMotion_SetVariantCallback_4(args...) Func_0200228e(args)
#define ObjectMotion_ArmCallback_6(a0, a1, a2) Call3(Func_020022d2, a0, a1, a2)
#define ObjectMotion_ArmCallback_7(a0, a1, a2) Call3(Func_020022de, a0, a1, a2)
#define ObjectMotion_ArmCallback_8(a0, a1, a2) Call3(Func_020022ea, a0, a1, a2)
#define ObjectMotion_ArmCallback_9(a0, a1, a2) Call3(Func_020022f6, a0, a1, a2)
#define Object_SetModeById_2(args...) Func_020022b6(args)
#define ObjectMotion_OffsetPositionAndReset_1(a0, a1, a2) Call3(Func_02002372, a0, a1, a2)
#define ObjectMotion_CommitCurrentPositionAndActivate_6(args...) Func_020022b8(args)
#define Object_SetModeById_3(args...) Func_020022d0(args)
#define SceneWork_SetStepValue_7(args...) Func_02002306(args)
#define BattleEvent_RunActionAndWait_6(args...) Func_0200231e(args)
#define BattleEffect_SpawnLinkedResourceObject_3(a0, a1, a2) Call3(Func_02002342, a0, a1, a2)
#define SceneWork_SetStepValue_8(args...) Func_02002320(args)
#define BattleEvent_RunActionAndWait_7(args...) Func_02002338(args)
#define ObjectMotion_CallThenWaitForAnimationChange_2(args...) Func_02002308(args)
#define BattleRuntime_WaitIfModeZero_6(args...) Func_020022ae(args)
#define ObjectMotion_SetSpeedParameters_7(a0, a1, a2) Call3(Func_020022e0, a0, a1, a2)
#define ObjectMotion_OffsetPositionAndReset_2(a0, a1, a2) Call3(Func_020023cc, a0, a1, a2)
#define ObjectMotion_CommitCurrentPositionAndActivate_7(args...) Func_02002312(args)
#define ObjectMotion_ArmCallback_10(a0, a1, a2) Call3(Func_02002376, a0, a1, a2)
#define BattleEffect_SpawnLinkedResourceObject_4(a0, a1, a2) Call3(Func_02002394, a0, a1, a2)
#define SceneWork_SetStepValue_9(args...) Func_02002372(args)
#define BattleEvent_RunActionAndWait_8(args...) Func_0200238a(args)
#define ObjectMotion_ResetAndSetPositionInMode2_6(a0, a1, a2) Call3(Func_02002336, a0, a1, a2)
#define BattleRuntime_WaitIfModeZero_7(args...) Func_02002304(args)
#define ObjectMotion_SetSpeedParameters_8(args...) Func_0200240c(args)
#define BattleRuntime_WaitIfModeZero_8(args...) Func_02002312(args)
#define BattleRuntime_ScheduleShoulderButtonModeUpdate_1(args...) Func_02002326(args)

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
    u8 *work = *(u8 **)Data_03001ebc;

    *(u16 *)(work + 0x1d8) = (u16)(*(u16 *)(work + 0x1d8) + amount);
}

/* Configures actors 12, 13, 2, 3 and 1 (position, scale, and movement) and
 * runs a scripted multi-actor sequence keyed off two text/index tables
 * (Data_0000250d, Data_00002512), skipping entirely if the guard call at
 * 0x02001fb2 reports the scene is not ready. */
void FieldScene_RunMultiActorPresentation(void)
{
    s32 prior_actor;
    s32 script_a;
    s32 script_b;

    if (GameFlag_IsSet_1(0x941) == 0) {
    } else {
        GameFlag_Set_1(0x94d);
        BattleRuntime_Reset_1();
        ObjectMotion_SetHorizontalPositionWithTerrain_1(12, 0x900000, 0x1900000);
        ObjectMotion_SetSpeedParameters_1(12, 0x18000, 0xc000);
        ObjectMotion_ResetAndSetPositionInMode2_1(12, 184, 0x190);
        ObjectMotion_CommitCurrentPositionAndActivate_1(12);
        Object_SetModeById_1(12, 1);
        ObjectMotion_ArmCallback_1(12, 0x3000, 0);
        ObjectMotion_SetVariantCallbackAndInvokeObject_1(0, 1);
        BattleRuntime_WaitIfModeZero_1(30);
        ObjectMotion_SetSpeedLimitAndAcceleration_1(0x8000, 0x1000);
        ObjectMotion_PlaceWithinCameraBounds_1(0xc00000, -1, 0x1b00000, 1);
        prior_actor = Scene_GetRecord_1(0);
        if (prior_actor != 0) {
            ObjectMotion_SetHorizontalPositionWithTerrain_2(13, *(volatile s32 *)(prior_actor + 8), *(volatile s32 *)(prior_actor + 16));
        }
        ObjectMotion_SetSpeedParameters_2(13, 0x14ccc, 0xa666);
        ObjectMotion_ResetAndSetPositionInMode2_2(13, 168, 0x1d0);
        ObjectMotion_CommitCurrentPositionAndActivate_2(13);
        ObjectMotion_ArmCallback_2(13, 0xc000, 0);
        prior_actor = Scene_GetRecord_2(0);
        if (prior_actor != 0) {
            ObjectMotion_SetHorizontalPositionWithTerrain_3(2, *(volatile s32 *)(prior_actor + 8), *(volatile s32 *)(prior_actor + 16));
        }
        ObjectMotion_SetSpeedParameters_3(2, 0x14ccc, 0xa666);
        ObjectMotion_ResetAndSetPositionInMode2_3(2, 152, 0x1e8);
        ObjectMotion_CommitCurrentPositionAndActivate_3(2);
        ObjectMotion_ArmCallback_3(2, 0xc000, 0);
        prior_actor = Scene_GetRecord_3(0);
        if (prior_actor != 0) {
            ObjectMotion_SetHorizontalPositionWithTerrain_4(3, *(volatile s32 *)(prior_actor + 8), *(volatile s32 *)(prior_actor + 16));
        }
        ObjectMotion_SetSpeedParameters_4(3, 0x14ccc, 0xa666);
        ObjectMotion_ResetAndSetPositionInMode2_4(3, 168, 0x1e8);
        ObjectMotion_CommitCurrentPositionAndActivate_4(3);
        ObjectMotion_ArmCallback_4(3, 0xc000, 0);
        prior_actor = Scene_GetRecord_4(0);
        if (prior_actor != 0) {
            ObjectMotion_SetHorizontalPositionWithTerrain_5(1, *(volatile s32 *)(prior_actor + 8), *(volatile s32 *)(prior_actor + 16));
        }
        ObjectMotion_SetSpeedParameters_5(1, 0x14ccc, 0xa666);
        ObjectMotion_ResetAndSetPositionInMode2_5(1, 184, 0x1e8);
        ObjectMotion_CommitCurrentPositionAndActivate_5(1);
        ObjectMotion_ArmCallback_5(1, 0xc000, 0);
        BattleRuntime_WaitIfModeZero_2(30);
        ObjectMotion_SetVariantCallbackAndInvokeObject_2(1, 1);
        script_a = (s32)Data_0000250d;
        SceneWork_SetStepValue_1(script_a);
        BattleEvent_RunActionAndWait_1(1, 0);
        BattleRuntime_WaitIfModeZero_3(30);
        ObjectMotion_CallThenWaitForAnimationChange_1(3, 3);
        BattleRuntime_WaitIfModeZero_4(10);
        SceneWork_SetStepValue_2((script_a + 1));
        BattleEvent_RunActionAndWait_2(3, 0);
        BattleEffect_SpawnLinkedResourceObject_1(2, 0x102, 70);
        ObjectMotion_SetAngleToward_1(2, 0, 0);
        ObjectMotion_SetAngleToward_2(0, 2, 0);
        SceneWork_SetStepValue_3((script_a + 2));
        ObjectMotion_SetSpeedParameters_6(2, 0);
        ObjectMotion_SetAngleToward_3(3, 0, 0);
        ObjectMotion_SetAngleToward_4(1, 0, 0);
        BattleRuntime_WaitIfModeZero_5(30);
        if (UiWork_WaitThenFinalizeCapacity_1(0, 0) == 0) {
            SceneWork_SetStepValue_4((script_a + 3));
            BattleEvent_RunActionAndWait_3(1, 0);
        } else {
            SceneWork_SetStepValue_5((script_a + 4));
            BattleEvent_RunActionAndWait_4(1, 0);
        }
        BattleEffect_SpawnLinkedResourceObject_2(13, 0x100, 70);
        script_b = (s32)Data_00002512;
        SceneWork_SetStepValue_6(script_b);
        BattleEvent_RunActionAndWait_5(13, 0);
        ObjectMotion_SetVariantCallback_1(0, 2);
        ObjectMotion_SetVariantCallback_2(1, 2);
        ObjectMotion_SetVariantCallback_3(2, 2);
        ObjectMotion_SetVariantCallback_4(3, 2);
        ObjectMotion_ArmCallback_6(0, 0xc000, 0);
        ObjectMotion_ArmCallback_7(1, 0xc000, 0);
        ObjectMotion_ArmCallback_8(2, 0xc000, 0);
        ObjectMotion_ArmCallback_9(3, 0xc000, 0);
        Object_SetModeById_2(13, 2);
        ObjectMotion_OffsetPositionAndReset_1(13, 0, -16);
        ObjectMotion_CommitCurrentPositionAndActivate_6(13);
        Object_SetModeById_3(13, 1);
        SceneWork_SetStepValue_7((script_b + 1));
        BattleEvent_RunActionAndWait_6(13, 0);
        BattleEffect_SpawnLinkedResourceObject_3(12, 0x100, 65);
        SceneWork_SetStepValue_8((script_b + 2));
        BattleEvent_RunActionAndWait_7(12, 0);
        ObjectMotion_CallThenWaitForAnimationChange_2(13, 3);
        BattleRuntime_WaitIfModeZero_6(80);
        ObjectMotion_SetSpeedParameters_7(12, 0x6666, 0x3333);
        ObjectMotion_OffsetPositionAndReset_2(12, -13, 0);
        ObjectMotion_CommitCurrentPositionAndActivate_7(12);
        ObjectMotion_ArmCallback_10(12, 0x4000, 0);
        BattleEffect_SpawnLinkedResourceObject_4(12, 0x102, 70);
        SceneWork_SetStepValue_9((script_b + 3));
        BattleEvent_RunActionAndWait_8(12, 0);
        ObjectMotion_ResetAndSetPositionInMode2_6(12, 168, 0x1b0);
        BattleRuntime_WaitIfModeZero_7(40);
        Func_02002400();
        ObjectMotion_SetSpeedParameters_8();
        BattleRuntime_WaitIfModeZero_8(20);
        BattleRuntime_ScheduleShoulderButtonModeUpdate_1();
        Func_02001026();
    }
}
