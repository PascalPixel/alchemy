#include "types.h"

/* Loader-relocated overlay calls: each symbol names the pre-relocation call
 * word the image holds. */
extern u8 Data_000012a0[];
void Func_02006498();
void Func_020064bc();
void Func_020064d2();
void Func_02006bc4();
void Func_02006bd2();
void Func_02006c12();
void Func_02006c20();
void Func_02006c70();
void Func_02006c7e();
void Func_02006c8c();
s32 Func_02006c8e();
u8 *Scene_GetRecord_1();
void Func_02006c96();
s32 Func_02006c9e();
void Func_02006ca4();
void Func_02006cac();
void Func_02006cb0();
void Func_02006cbc();
void Func_02006cda();
void Func_02006cfc();

/* Resolved engine calls: each pseudo symbol is the per-site call word the
 * overlay image holds (a word can serve two sites with different targets),
 * and the macro names the engine function the site reaches through the
 * overlay veneer and the main-image veneer island, keeping the site's own
 * calling form. Names without a repository binding are provisional.
 */
#define BattleRuntime_WaitIfModeZero_1(args...) Func_02006bc4(args)
#define ObjectMotion_SetVariantCallbackAndInvokeObject_1(args...) Func_02006c8c(args)
#define BattleRuntime_WaitIfModeZero_2(args...) Func_02006bd2(args)
#define SceneWork_SetStepValue_1(args...) Func_02006cb0(args)
#define ObjectMotion_ArmCallback_1(args...) Func_02006cda(args)
#define BattleRuntime_WaitIfModeZero_3(a0, a1) Call2(Func_02006cfc, a0, a1)
#define ObjectMotion_SetVariantCallbackAndInvokeObject_2(args...) Func_02006cbc(args)
#define ObjectMotion_CallThenWaitForAnimationChange_1(args...) Func_02006cac(args)
#define BattleRuntime_WaitIfModeZero_4(args...) Func_02006c12(args)
#define ObjectMotion_SetSpeedParameters_1(a0, a1, a2) Call3(Func_02006c70, a0, a1, a2)
#define ObjectMotion_SetSpeedParameters_2(a0, a1, a2) Call3(Func_02006c7e, a0, a1, a2)
#define ObjectMotion_EnableActionAndSetCallback_1(a0, a1) Value2(Func_02006c8e, a0, a1)
#define ObjectMotion_EnableActionAndSetCallback_2(a0, a1) Call2(Func_02006c96, a0, a1)
#define Object_LookupAndStep_1(args...) Func_02006ca4(args)
#define Scene_GetRecord_1(args...) Func_02006c92(args)
#define Scene_GetRecord_2(a0) Value1(Func_02006c9e, a0)
u8 *Func_02006c92();

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

static __inline__ void Call6(void (*f)(), s32 a0, s32 a1, s32 a2, s32 a3, s32 a4, s32 a5)
{
    f(a0, a1, a2, a3, a4, a5);
}

/* The scene step counter at 0x1d8 of the shared scene work record. */
static __inline__ void bump_step(s32 amount)
{
    u8 *work = *(u8 **)0x03001ebc;

    *(u16 *)(work + 0x1d8) = (u16)(*(u16 *)(work + 0x1d8) + amount);
}

/* The "shown" half word at +100 of an actor record. */
#define ACTOR_SHOWN_OFFSET 100

/* Configures actor records 24 and 25 (their +100 "shown" half words end up
 * set to 1 and 3 respectively) and finishes with a six-argument call that
 * repeats actor id 14. */
void FieldScene_ConfigurePairedActors(void)
{
    u32 i;
    u8 *record;

    BattleRuntime_WaitIfModeZero_1(30);
    ObjectMotion_SetVariantCallbackAndInvokeObject_1(24, 1);
    BattleRuntime_WaitIfModeZero_2(20);
    SceneWork_SetStepValue_1((s32)Data_000012a0);
    Func_02006498(24, 20);
    ObjectMotion_ArmCallback_1(25, 0, 20);
    BattleRuntime_WaitIfModeZero_3(25, 0x102);
    ObjectMotion_SetVariantCallbackAndInvokeObject_2(25, 2);
    Func_020064bc(25, 20);
    ObjectMotion_CallThenWaitForAnimationChange_1(24, 4);
    BattleRuntime_WaitIfModeZero_4(20);
    Func_020064d2(24, 20);
    ObjectMotion_SetSpeedParameters_1(24, 0x40000, 0x20000);
    ObjectMotion_SetSpeedParameters_2(25, 0x38000, 0x1c000);
    ObjectMotion_EnableActionAndSetCallback_1(25, 0x200d830);
    ObjectMotion_EnableActionAndSetCallback_2(24, 0x200d560);
    Object_LookupAndStep_1(24);
    {
        u8 *record = Scene_GetRecord_1(24);
        s32 shown = 1;

        *(u16 *)(record + ACTOR_SHOWN_OFFSET) = shown;
    }
    {
        u8 *record = Scene_GetRecord_2(25);
        s32 shown = 3;

        *(u16 *)(record + ACTOR_SHOWN_OFFSET) = shown;
    }
    Call6(Func_02006c20, 14, 48, 4, 1, 14, 44);
}
