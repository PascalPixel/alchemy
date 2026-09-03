#include "types.h"

#define FieldScene_ConfigureThreeActors Func_02002b7c

/* Loader-relocated overlay calls: each symbol names the pre-relocation call
 * word the image holds. */
extern u8 Data_03001ebc[];
void Func_0200661a();
void Func_02006652();
void Func_02006dfc();
void Func_02006e38();
void Func_02006e6c();
void Func_02006e96();
s32 Func_02006e9a();
s32 Func_02006ec0();
s32 Func_02006eda();
void Func_02006ef2();
s32 Func_02006efc();
void Func_02006f1a();
void Func_02006f2c();
void Func_02006f34();
void Func_02006f56();
void Func_02006f60();
void Func_02006f9c();
void Func_02006fa6();
void Func_02006faa();
void Func_02006fb2();
s32 Func_02006fc0();
void Func_02006fd4();
void Func_02006ff4();
s32 Func_0200702c();
void Func_02007040();
void Func_02007054();
void Func_02007090();

/* Resolved engine calls: each pseudo symbol is the per-site call word the
 * overlay image holds (a word can serve two sites with different targets),
 * and the macro names the engine function the site reaches through the
 * overlay veneer and the main-image veneer island, keeping the site's own
 * calling form. Names without a repository binding are provisional.
 */
#define BattleRuntime_Reset_1(args...) Func_02006e6c(args)
#define ObjectGroup_ConfigureChildValue_1(args...) Func_02006f2c(args)
#define Scene_GetRecord_1(args...) Func_02006e9a(args)
#define Object_NotifyLastActiveOfEvent_1(a0) Call1(Func_02006e96, a0)
#define ObjectMotion_SetHorizontalPositionWithTerrain_1(a0, a1, a2) Call3(Func_02006f1a, a0, a1, a2)
#define Scene_GetRecord_2(args...) Func_02006ec0(args)
#define ObjectMotion_SetHorizontalPositionWithTerrain_2(a0, a1, a2) Call3(Func_02006f34, a0, a1, a2)
#define Scene_GetRecord_3(args...) Func_02006eda(args)
#define ObjectMotion_SetActionVariant_1(a0, a1) Value2(Func_02006fc0, a0, a1)
#define ObjectMotion_SetHorizontalPositionWithTerrain_3(a0, a1, a2) Call3(Func_02006f56, a0, a1, a2)
#define Scene_GetRecord_4(args...) Func_02006efc(args)
#define BattleRuntime_WaitIfModeZero_1(args...) Func_02007040(args)
#define ObjectMotion_SetSpeedParameters_1(args...) Func_02007054(args)
#define BattleRuntime_WaitIfModeZero_2(args...) Func_02006ef2(args)
#define ObjectMotion_Launch_1(args...) Func_02006f9c(args)
#define ObjectMotion_Launch_2(args...) Func_02006fa6(args)
#define SceneWork_SetStepValue_1(a0) Call1(Func_02006fd4, a0)
#define ObjectMotion_CallThenWaitForAnimationChange_1(args...) Func_02006fb2(args)
#define ObjectMotion_SetSpeedParameters_2(a0, a1, a2) Call3(Func_02006f60, a0, a1, a2)
#define ObjectMotion_SetPositionAndReset_1(a0, a1, a2) Call3(Func_02006faa, a0, a1, a2)
#define ObjectMotion_ArmCallback_1(a0, a1, a2) Value3(Func_0200702c, a0, a1, a2)
#define ObjectMotion_SetVariantCallbackAndInvokeObject_1(args...) Func_02006ff4(args)

/* Call sites spelled through these wrappers pass their constants straight
 * into the argument registers; a direct call precomputes a costly constant
 * into a pseudo that the compiler then shares with later uses in the block.
 * A value-returning call also sets r0 last of its arguments. */

static __inline__ void Call1(void (*f)(), s32 a0)
{
    f(a0);
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
    u8 *work = *(u8 **)Data_03001ebc;

    *(u16 *)(work + 0x1d8) = (u16)(*(u16 *)(work + 0x1d8) + amount);
}

/* Phase/status word at 0x1c0 of the shared scene work record. */
#define SCENE_PHASE (*(s32 *)(*(u8 *volatile *)Data_03001ebc + 0x1c0))

/* Configures actors 20, 21 and 22 (position and movement/sprite flags) and
 * advances the shared scene phase before the scene runs. */
void FieldScene_ConfigureThreeActors(void)
{
    u32 i;
    s32 record;

    BattleRuntime_Reset_1();
    ObjectGroup_ConfigureChildValue_1(0, 15);
    record = Scene_GetRecord_1(0);
    Func_02006e38(record, 0);
    Object_NotifyLastActiveOfEvent_1(0x200d160);
    Func_02006dfc(1);
    ObjectMotion_SetHorizontalPositionWithTerrain_1(20, 0xc40000, 0x1f60000);
    record = Scene_GetRecord_2(20);
    {
        /* Set the visibility/active flag at +6. */
        s32 shown = 0xa000;

        *(volatile u16 *)(record + 6) = shown;
    }
    ObjectMotion_SetHorizontalPositionWithTerrain_2(22, 0xb80000, 0x20c0000);
    record = Scene_GetRecord_3(22);
    {
        /* Set the visibility/active flag at +6. */
        s32 shown = 0xb000;

        *(volatile u16 *)(record + 6) = shown;
    }
    ObjectMotion_SetActionVariant_1(21, 1);
    ObjectMotion_SetHorizontalPositionWithTerrain_3(21, 0xb80000, 0x2780000);
    record = Scene_GetRecord_4(21);
    {
        /* Set the visibility/active flag at +6. */
        s32 shown = 0xb000;

        *(volatile u16 *)(record + 6) = shown;
    }
    SCENE_PHASE = 0x202;
    BattleRuntime_WaitIfModeZero_1();
    ObjectMotion_SetSpeedParameters_1();
    BattleRuntime_WaitIfModeZero_2(20);
    ObjectMotion_Launch_1(22, 4, 10);
    ObjectMotion_Launch_2(22, 6, 20);
    SceneWork_SetStepValue_1(0x1ee5);
    Func_0200661a(22);
    ObjectMotion_CallThenWaitForAnimationChange_1(20, 3);
    ObjectMotion_SetSpeedParameters_2(21, 0x30000, 0x18000);
    ObjectMotion_SetPositionAndReset_1(21, 180, 0x222);
    ObjectMotion_ArmCallback_1(21, 0xb000, 40);
    ObjectMotion_SetVariantCallbackAndInvokeObject_1(21, 1);
    Func_02006652(21);
    Func_02007090(15);
}
