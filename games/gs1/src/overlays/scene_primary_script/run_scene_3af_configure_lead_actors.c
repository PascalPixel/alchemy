#include "types.h"

#define FieldScene_ConfigureLeadActors Func_020029d4

/* Loader-relocated overlay calls: each symbol names the pre-relocation call
 * word the image holds. */
extern u8 Data_03001ebc[];
void Func_020036be();
void Func_02006476();
s32 Func_02006480();
void Func_020064bc();
void Func_020064ca();
void Func_02006502();
void Func_02006c40();
void Func_02006cc4();
void Func_02006cda();
u8 *Scene_GetRecord_1();
u8 *Scene_GetRecord_2();
void Func_02006d3c();
void Func_02006d3e();
void Func_02006d50();
void Func_02006d5c();
void Func_02006d60();
void Func_02006d68();
void Func_02006d74();
void Func_02006d7e();
void Func_02006d86();
void Func_02006dca();
void Func_02006df0();
void Func_02006e02();
void Func_02006e10();
void Func_02006e18();
void Func_02006e1c();
void Func_02006e2c();
void Func_02006e3a();
void Func_02006e48();
void Func_02006e54();
void Func_02006e5c();
void Func_02006e74();
void Func_02006e82();
void Func_02006e86();
void Func_02006e8a();
void Func_02006e8e();
void Func_02006e9e();
void Func_02006ea4();

/* Resolved engine calls: each pseudo symbol is the per-site call word the
 * overlay image holds (a word can serve two sites with different targets),
 * and the macro names the engine function the site reaches through the
 * overlay veneer and the main-image veneer island, keeping the site's own
 * calling form. Names without a repository binding are provisional.
 */
#define BattleRuntime_Reset_1(args...) Func_02006cc4(args)
#define Object_NotifyLastActiveOfEvent_1(a0) Call1(Func_02006cda, a0)
#define ObjectMotion_SetHorizontalPositionWithTerrain_1(a0, a1, a2) Call3(Func_02006d5c, a0, a1, a2)
#define ObjectMotion_SetHorizontalPositionWithTerrain_2(a0, a1, a2) Call3(Func_02006d68, a0, a1, a2)
#define ObjectMotion_SetHorizontalPositionWithTerrain_3(a0, a1, a2) Call3(Func_02006d74, a0, a1, a2)
#define Scene_GetRecord_1(args...) Func_02006d1a(args)
#define ObjectMotion_EnableActionAndSetCallback_1(a0, a1) Call2(Func_02006d3e, a0, a1)
#define Scene_GetRecord_2(args...) Func_02006d2c(args)
#define ObjectMotion_SetSpeedParameters_1(a0, a1, a2) Call3(Func_02006d50, a0, a1, a2)
#define ObjectMotion_EnableActionAndSetCallback_2(a0, a1) Call2(Func_02006d60, a0, a1)
#define BattleRuntime_WaitIfModeZero_1(args...) Func_02006e8a(args)
#define ObjectMotion_SetSpeedParameters_2(args...) Func_02006e9e(args)
#define BattleRuntime_WaitIfModeZero_2(args...) Func_02006d3c(args)
#define ObjectMotion_SetSpeedParameters_3(a0, a1, a2) Call3(Func_02006d7e, a0, a1, a2)
#define ObjectMotion_SetPositionAndReset_1(a0, a1, a2) Call3(Func_02006dca, a0, a1, a2)
#define ObjectMotion_SetVariantCallbackAndInvokeObject_1(args...) Func_02006e1c(args)
#define SceneWork_SetStepValue_1(a0) Call1(Func_02006e3a, a0)
#define ObjectMotion_CallThenWaitForAnimationChange_1(args...) Func_02006e18(args)
#define BattleRuntime_WaitIfModeZero_3(args...) Func_02006d86(args)
#define ObjectMotion_ArmCallback_1(a0, a1, a2) Call3(Func_02006e82, a0, a1, a2)
#define BattleEffect_SpawnLinkedResourceObject_1(a0, a1, a2) Call3(Func_02006ea4, a0, a1, a2)
#define BattleRuntime_RunThenWaitIfModeZero_1(args...) Func_02006e86(args)
#define ObjectMotion_CallThenWaitForAnimationChange_2(args...) Func_02006e54(args)
#define ObjectMotion_CallThenWaitForAnimationChange_3(args...) Func_02006e5c(args)
#define ObjectMotion_SetPositionAndReset_2(a0, a1, a2) Call3(Func_02006e48, a0, a1, a2)
#define ObjectMotion_SetPositionAndReset_3(a0, a1, a2) Call3(Func_02006e54, a0, a1, a2)
#define BattleRuntime_WaitIfModeZero_4(args...) Func_02006df0(args)
#define ObjectMotion_SetPositionAndReset_4(a0, a1, a2) Call3(Func_02006e74, a0, a1, a2)
#define ObjectMotion_SetHorizontalPositionWithTerrain_4(args...) Func_02006e8e(args)
#define GameFlag_Set_1(a0) Call1(Func_02006e02, a0)
#define GameFlag_Clear_1(a0) Call1(Func_02006e10, a0)
#define BattleRuntime_ScheduleShoulderButtonModeUpdate_1(args...) Func_02006e2c(args)
u8 *Func_02006d1a();
u8 *Func_02006d2c();

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

/* Phase/status word at 0x1c0 of the shared scene work record. */
#define SCENE_PHASE (*(s32 *)(*(u8 *volatile *)Data_03001ebc + 0x1c0))

/* Configures actors 20, 21, 22 and 23 (position, pose, and movement/sprite
 * flags) and advances the shared scene phase before the scene runs. */
void FieldScene_ConfigureLeadActors(void)
{
    u8 *record;

    BattleRuntime_Reset_1();
    Object_NotifyLastActiveOfEvent_1(0x200d160);
    Func_02006c40(1);
    ObjectMotion_SetHorizontalPositionWithTerrain_1(20, 0xb60000, 0x26a0000);
    ObjectMotion_SetHorizontalPositionWithTerrain_2(23, 0xee0000, 0x2720000);
    ObjectMotion_SetHorizontalPositionWithTerrain_3(22, 0x10c0000, 0x2a60000);
    record = Scene_GetRecord_1(22);
    {
        /* Clear the visibility/active flag at +6. */
        s32 shown = 0;

        *(volatile u16 *)(record + 6) = shown;
    }
    ObjectMotion_EnableActionAndSetCallback_1(22, 0x200c980);
    {
        /* Set the high bit of the flag byte at +89. */
        u8 *record = Scene_GetRecord_2(21);
        u8 flags = *(volatile u8 *)&record[89];

        record[89] = (u8)(flags | 128);
    }
    ObjectMotion_SetSpeedParameters_1(21, 0xcccc, 0x6666);
    ObjectMotion_EnableActionAndSetCallback_2(21, 0x200c628);
    SCENE_PHASE = 0x100;
    BattleRuntime_WaitIfModeZero_1();
    ObjectMotion_SetSpeedParameters_2();
    BattleRuntime_WaitIfModeZero_2(20);
    ObjectMotion_SetSpeedParameters_3(20, 0x19999, 0xcccc);
    ObjectMotion_SetPositionAndReset_1(20, 182, 0x224);
    Func_02006476(20, 0);
    Value2(Func_02006480, 0, 0x8000);
    ObjectMotion_SetVariantCallbackAndInvokeObject_1(20, 1);
    SceneWork_SetStepValue_1(0x1ee1);
    ((void (*)())Func_02006480)(20);
    ObjectMotion_CallThenWaitForAnimationChange_1(0, 3);
    BattleRuntime_WaitIfModeZero_3(40);
    ObjectMotion_ArmCallback_1(20, 0x5000, 20);
    BattleEffect_SpawnLinkedResourceObject_1(20, 0x105, 60);
    BattleRuntime_RunThenWaitIfModeZero_1(20, 0, 40);
    Func_020064ca(20, 0);
    Func_020064bc(20);
    ObjectMotion_CallThenWaitForAnimationChange_2(0, 3);
    ObjectMotion_CallThenWaitForAnimationChange_3(20, 3);
    ObjectMotion_SetPositionAndReset_2(20, 182, 0x258);
    ObjectMotion_SetPositionAndReset_3(20, 216, 0x258);
    Call2(Func_02006502, 20, 0xc000);
    Func_020036be();
    BattleRuntime_WaitIfModeZero_4(10);
    ObjectMotion_SetPositionAndReset_4(20, 216, 0x244);
    ObjectMotion_SetHorizontalPositionWithTerrain_4(20, 0, 0);
    SCENE_PHASE = 0x209;
    GameFlag_Set_1(0x92b);
    GameFlag_Clear_1(0x302);
    BattleRuntime_ScheduleShoulderButtonModeUpdate_1();
}
