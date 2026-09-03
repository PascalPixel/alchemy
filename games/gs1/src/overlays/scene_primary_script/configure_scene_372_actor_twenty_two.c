#include "types.h"

#define FieldScene_ConfigureActorTwentyTwoScene Func_02003fb0

/* Loader-relocated overlay calls: each symbol names the pre-relocation call
 * word the image holds. */
void Func_0200861e();
void Func_02008656();
void Func_02008676();
void Func_020086b0();
void Func_02008720();
u8 *Scene_GetRecord_1();
void Func_02008752();
void Func_0200875c();
void Func_02008776();
void Func_020087a0();
void Func_020087b0();
void Func_020087c0();
void Func_020087ea();
void Func_020087ec();
void Func_020087f2();
void Func_020087f4();
void Func_02008802();
void Func_0200880c();
void Func_02008836();
void Func_02008838();
void Func_0200883e();
void Func_02008842();
void Func_0200885c();
void Func_02008862();
void Func_02008878();
void Func_0200887a();
void Func_02008884();
void Func_02008896();

/* Resolved engine calls: each pseudo symbol is the per-site call word the
 * overlay image holds (a word can serve two sites with different targets),
 * and the macro names the engine function the site reaches through the
 * overlay veneer and the main-image veneer island, keeping the site's own
 * calling form. Names without a repository binding are provisional.
 */
#define BattleRuntime_Reset_1(args...) Func_020086b0(args)
#define ObjectMotion_PlaceWithinCameraBounds_1(a0, a1, a2, a3) Call4(Func_020087ea, a0, a1, a2, a3)
#define ObjectMotion_EnableActionAndResetMotion_1(args...) Func_02008720(args)
#define ObjectMotion_SetPositionAndReset_1(a0, a1, a2) Call3(Func_0200875c, a0, a1, a2)
#define ObjectMotion_SetHorizontalPositionWithTerrain_1(args...) Func_02008776(args)
#define ObjectMotion_ArmCallback_1(a0, a1, a2) Call3(Func_020087f2, a0, a1, a2)
#define Scene_GetRecord_1(args...) Func_02008728(args)
#define ObjectMotion_SetHorizontalPositionWithTerrain_2(a0, a1, a2) Call3(Func_020087a0, a0, a1, a2)
#define SceneWork_SetStepValue_1(a0) Call1(Func_020087f4, a0)
#define BattleEvent_RunActionAndWait_1(a0, a1) Call2(Func_0200880c, a0, a1)
#define ObjectMotion_SetHorizontalPositionWithTerrain_3(a0, a1, a2) Call3(Func_020087c0, a0, a1, a2)
#define ObjectMotion_SetSpeedLimitAndAcceleration_1(a0, a1) Call2(Func_02008862, a0, a1)
#define ObjectMotion_PlaceWithinCameraBounds_2(a0, a1, a2, a3) Call4(Func_02008878, a0, a1, a2, a3)
#define Object_CommitPositionThenWaitIfModeZero_1(args...) Func_02008884(args)
#define BattleRuntime_WaitIfModeZero_1(args...) Func_02008752(args)
#define ObjectMotion_CallThenWaitForAnimationChange_1(args...) Func_02008802(args)
#define BattleRuntime_RunThenWaitIfModeZero_1(a0, a1, a2) Call3(Func_0200885c, a0, a1, a2)
#define ObjectMotion_ArmCallback_2(a0, a1, a2) Call3(Func_02008878, a0, a1, a2)
#define ObjectMotion_SetVariantCallbackAndInvokeObject_1(args...) Func_02008838(args)
#define BattleRuntime_RunThenWaitIfModeZero_2(a0, a1, a2) Call3(Func_0200887a, a0, a1, a2)
#define ObjectMotion_ArmCallback_3(a0, a1, a2) Call3(Func_02008896, a0, a1, a2)
#define ObjectMotion_CallThenWaitForAnimationChange_2(args...) Func_0200883e(args)
#define ObjectMotion_SetSpeedParameters_1(a0, a1, a2) Call3(Func_020087ec, a0, a1, a2)
#define ObjectMotion_SetPositionAndReset_2(a0, a1, a2) Call3(Func_02008836, a0, a1, a2)
#define ObjectMotion_SetPositionAndReset_3(a0, a1, a2) Call3(Func_02008842, a0, a1, a2)
#define GameFlag_Set_1(a0) Call1(Func_020087b0, a0)
u8 *Func_02008728();

/* Call sites spelled through these wrappers pass their constants straight
 * into the argument registers; a direct call precomputes a costly constant
 * into a pseudo that the compiler then shares with later uses in the block.
 * A value-returning call also sets r0 last of its arguments. */

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

/* The scene step counter at 0x1d8 of the shared scene work record. */
static __inline__ void bump_step(s32 amount)
{
    u8 *work = *(u8 **)0x03001ebc;

    *(u16 *)(work + 0x1d8) = (u16)(*(u16 *)(work + 0x1d8) + amount);
}

/* Configures actor 22 (position, pose, and movement/sprite flags) for the
 * scene. */
#define ACTOR_ID 22

void FieldScene_ConfigureActorTwentyTwoScene(void)
{
    u32 i;
    u8 *record;

    BattleRuntime_Reset_1();
    ObjectMotion_PlaceWithinCameraBounds_1(-1, -1, -1, 0);
    ObjectMotion_EnableActionAndResetMotion_1(ACTOR_ID);
    Call1(Func_0200861e, 0x200c5b9);
    ObjectMotion_SetPositionAndReset_1(0, 0x1e0, 0x570);
    ObjectMotion_SetHorizontalPositionWithTerrain_1(0, 0, 0);
    ObjectMotion_ArmCallback_1(ACTOR_ID, 0x3000, 20);
    {
        /* Set bit 0 of the flag byte at +35. */
        u8 *record = Scene_GetRecord_1(ACTOR_ID);
        u8 flags = *(volatile u8 *)&record[35];

        record[35] = (u8)(flags | 1);
    }
    ObjectMotion_SetHorizontalPositionWithTerrain_2(ACTOR_ID, 0xf90000, 0x4d80000);
    Func_02008656(1);
    SceneWork_SetStepValue_1(0xed3);
    BattleEvent_RunActionAndWait_1(0x1016, 0);
    ObjectMotion_SetHorizontalPositionWithTerrain_3(ACTOR_ID, 0xac0000, 0x4fe0000);
    Func_02008676(1);
    ObjectMotion_SetSpeedLimitAndAcceleration_1(0x40000, 0x8000);
    ObjectMotion_PlaceWithinCameraBounds_2(0xa20000, 0, 0x5050000, 1);
    Object_CommitPositionThenWaitIfModeZero_1();
    BattleRuntime_WaitIfModeZero_1(40);
    ObjectMotion_CallThenWaitForAnimationChange_1(ACTOR_ID, 4);
    BattleRuntime_RunThenWaitIfModeZero_1(0x1016, 0, 10);
    ObjectMotion_ArmCallback_2(ACTOR_ID, 0xc000, 20);
    ObjectMotion_SetVariantCallbackAndInvokeObject_1(ACTOR_ID, 2);
    BattleRuntime_RunThenWaitIfModeZero_2(0x1016, 0, 10);
    ObjectMotion_ArmCallback_3(ACTOR_ID, 0x1000, 20);
    ObjectMotion_CallThenWaitForAnimationChange_2(ACTOR_ID, 3);
    ObjectMotion_SetSpeedParameters_1(ACTOR_ID, 0x20000, 0x10000);
    ObjectMotion_SetPositionAndReset_2(ACTOR_ID, 165, 0x514);
    ObjectMotion_SetPositionAndReset_3(ACTOR_ID, 195, 0x598);
    GameFlag_Set_1(0x842);
}
