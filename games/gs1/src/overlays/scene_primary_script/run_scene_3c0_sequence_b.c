#include "types.h"

#define FieldScene_RunActorThirteenRestoration Func_02000838

/* Loader-relocated overlay calls: each symbol names the pre-relocation call
 * word the image holds. */
s32 Func_02001a8c();
s32 Func_02001a98();
s32 Func_02001aa6();
void Func_02001aba();
void Func_02001ae6();
u8 *Scene_GetRecord_1();
void Func_02001b50();
void Func_02001b5a();
void Func_02001b6c();
void Func_02001b74();
void Func_02001b76();
void Func_02001b7c();
void Func_02001ba0();
void Func_02001bb4();
void Func_02001bbe();
void Func_02001be0();
void Func_02001be4();
void Func_02001bee();
void Func_02001c00();
void Func_02001c06();
void Func_02001c10();
u8 *Scene_GetRecord_2();
void Func_02001c16();
void Func_02001c24();
void Func_02001c30();
void Func_02001c32();
void Func_02001c44();
void Func_02001c54();
void Func_02001c62();
void Func_02001c74();

/* Resolved engine calls: each pseudo symbol is the per-site call word the
 * overlay image holds (a word can serve two sites with different targets),
 * and the macro names the engine function the site reaches through the
 * overlay veneer and the main-image veneer island, keeping the site's own
 * calling form. Names without a repository binding are provisional.
 */
#define GameFlag_IsSet_1(a0) Value1(Func_02001a8c, a0)
#define GameFlag_IsSet_2(a0) Value1(Func_02001a98, a0)
#define GameFlag_IsSet_3(a0) Value1(Func_02001aa6, a0)
#define GameFlag_Set_1(a0) Call1(Func_02001aba, a0)
#define BattleRuntime_Reset_1(args...) Func_02001ae6(args)
#define SceneWork_SetStepValue_1(a0) Call1(Func_02001b6c, a0)
#define Scene_GetRecord_1(args...) Func_02001b02(args)
#define ObjectMotion_SetHorizontalPositionWithTerrain_1(args...) Func_02001b50(args)
#define ObjectMotion_SetAngleToward_1(a0, a1, a2) Call3(Func_02001b7c, a0, a1, a2)
#define ObjectMotion_SetPositionAndReset_1(a0, a1, a2) Call3(Func_02001b5a, a0, a1, a2)
#define ObjectMotion_ArmCallback_1(a0, a1, a2) Call3(Func_02001bbe, a0, a1, a2)
#define ObjectMotion_SetPositionAndReset_2(a0, a1, a2) Call3(Func_02001b74, a0, a1, a2)
#define BattleEffect_SpawnLinkedResourceObject_1(a0, a1, a2) Call3(Func_02001be0, a0, a1, a2)
#define ObjectMotion_ArmCallback_2(a0, a1, a2) Call3(Func_02001be4, a0, a1, a2)
#define ObjectMotion_CallThenWaitForAnimationChange_1(args...) Func_02001bb4(args)
#define BattleEvent_RunActionAndWait_1(args...) Func_02001be4(args)
#define BattleEffect_SpawnLinkedResourceObject_2(a0, a1, a2) Call3(Func_02001c06, a0, a1, a2)
#define BattleEffect_SpawnLinkedResourceObject_3(a0, a1, a2) Call3(Func_02001c10, a0, a1, a2)
#define BattleEvent_RunActionAndWait_2(args...) Func_02001c00(args)
#define BattleRuntime_WaitIfModeZero_1(args...) Func_02001b76(args)
#define ObjectMotion_SetVariantCallbackAndInvokeObject_1(args...) Func_02001bee(args)
#define BattleEvent_RunActionAndWait_3(args...) Func_02001c16(args)
#define ObjectMotion_ArmCallback_3(a0, a1, a2) Call3(Func_02001c32, a0, a1, a2)
#define BattleEventRuntime_ProcessAction_1(args...) Func_02001c32(args)
#define BattleRuntime_WaitIfModeZero_2(args...) Func_02001ba0(args)
#define BattleEffect_SpawnLinkedResourceObject_4(a0, a1, a2) Call3(Func_02001c54, a0, a1, a2)
#define BattleEvent_RunActionAndWait_4(args...) Func_02001c44(args)
#define ObjectMotion_CallThenWaitForAnimationChange_2(args...) Func_02001c24(args)
#define BattleEvent_RunActionAndWait_5(args...) Func_02001c54(args)
#define ObjectMotion_SetSpeedParameters_1(a0, a1, a2) Call3(Func_02001bee, a0, a1, a2)
#define ObjectMotion_SetPositionAndReset_3(a0, a1, a2) Call3(Func_02001c24, a0, a1, a2)
#define BattleEvent_RunActionAndWait_6(args...) Func_02001c74(args)
#define ObjectMotion_CallThenWaitForAnimationChange_3(args...) Func_02001c54(args)
#define Object_SetModeById_1(args...) Func_02001c54(args)
#define Scene_GetRecord_2(args...) Func_02001c12(args)
#define ObjectMotion_ResetAndSetPosition_1(args...) Func_02001c44(args)
#define ObjectMotion_CommitCurrentPositionAndActivate_1(args...) Func_02001c62(args)
#define ObjectMotion_SetHorizontalPositionWithTerrain_2(args...) Func_02001c74(args)
#define BattleRuntime_ScheduleShoulderButtonModeUpdate_1(args...) Func_02001c30(args)
u8 *Func_02001b02();
u8 *Func_02001c12();

/* Resolved from the main-image function's own source (not registered in the
 * overlay veneer table): each macro names what that source shows the
 * function doing, keeping the site's own calling form. */

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

static __inline__ void Call3(void (*f)(), s32 a0, s32 a1, s32 a2)
{
    f(a0, a1, a2);
}

/* The scene step counter at 0x1d8 of the shared scene work record. */
static __inline__ void bump_step(s32 amount)
{
    u8 *work = *(u8 **)0x03001ebc;

    *(u16 *)(work + 0x1d8) = (u16)(*(u16 *)(work + 0x1d8) + amount);
}

/* Actor id operated on throughout this scene. */
#define ACTOR_ID 13

/* Runs a scene step only when three preceding gate checks all pass;
 * otherwise the function does nothing. Drives actor ACTOR_ID through a
 * long sequence of position/pose/flag updates, applying two record-driven
 * position fixups read back from lookup calls partway through. */
void FieldScene_RunActorThirteenRestoration(void)
{
    u32 i;
    u8 *record;

    if (GameFlag_IsSet_1(0x9a0) == 0) {
    } else {
        if (GameFlag_IsSet_2(0x1b7) != 0) {
        } else {
            if (GameFlag_IsSet_3(0x9b0) == 0) {
            } else {
                GameFlag_Set_1(0x9b5);
                BattleRuntime_Reset_1();
                SceneWork_SetStepValue_1(0x2633);
                /* Record layout observed here: s32 at +8, s32 at +16. */
                record = Scene_GetRecord_1(0);
                if (record != 0) {
                    ObjectMotion_SetHorizontalPositionWithTerrain_1(ACTOR_ID, *(s32 *)(record + 8), *(s32 *)(record + 16));
                }
                ObjectMotion_SetAngleToward_1(ACTOR_ID, 0xc000, 0);
                ObjectMotion_SetPositionAndReset_1(0, 0x1b8, 0x4e8);
                ObjectMotion_ArmCallback_1(ACTOR_ID, 0x4000, 0);
                ObjectMotion_SetPositionAndReset_2(0, 0x1bc, 0x4d8);
                BattleEffect_SpawnLinkedResourceObject_1(0, 0x100, 40);
                ObjectMotion_ArmCallback_2(0, 0x4000, 30);
                ObjectMotion_CallThenWaitForAnimationChange_1(ACTOR_ID, 4);
                BattleEvent_RunActionAndWait_1(ACTOR_ID, 0);
                BattleEffect_SpawnLinkedResourceObject_2(0, 0x105, 60);
                BattleEffect_SpawnLinkedResourceObject_3(ACTOR_ID, 0x105, 60);
                BattleEvent_RunActionAndWait_2(ACTOR_ID, 0);
                BattleRuntime_WaitIfModeZero_1(30);
                ObjectMotion_SetVariantCallbackAndInvokeObject_1(ACTOR_ID, 2);
                BattleEvent_RunActionAndWait_3(ACTOR_ID, 0);
                ObjectMotion_ArmCallback_3(ACTOR_ID, 0xc000, 30);
                BattleEventRuntime_ProcessAction_1(ACTOR_ID, 0);
                BattleRuntime_WaitIfModeZero_2(30);
                BattleEffect_SpawnLinkedResourceObject_4(ACTOR_ID, 0x106, 60);
                BattleEvent_RunActionAndWait_4(ACTOR_ID, 0);
                ObjectMotion_CallThenWaitForAnimationChange_2(ACTOR_ID, 3);
                BattleEvent_RunActionAndWait_5(ACTOR_ID, 0);
                ObjectMotion_SetSpeedParameters_1(ACTOR_ID, 0xb333, 0x5999);
                ObjectMotion_SetPositionAndReset_3(ACTOR_ID, 0x1b8, 0x4e8);
                BattleEvent_RunActionAndWait_6(ACTOR_ID, 0);
                ObjectMotion_CallThenWaitForAnimationChange_3(0, 3);
                Object_SetModeById_1(ACTOR_ID, 2);
                /* Record layout observed here: s16 at +10, s16 at +18. */
                record = Scene_GetRecord_2(0);
                if (record != 0) {
                    ObjectMotion_ResetAndSetPosition_1(ACTOR_ID, *(s16 *)(record + 10), *(s16 *)(record + 18));
                }
                ObjectMotion_CommitCurrentPositionAndActivate_1(ACTOR_ID);
                ObjectMotion_SetHorizontalPositionWithTerrain_2(ACTOR_ID, 0, 0);
                BattleRuntime_ScheduleShoulderButtonModeUpdate_1();
            }
        }
    }
}
