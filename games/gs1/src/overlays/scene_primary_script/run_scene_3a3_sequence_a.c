#include "types.h"

#define FieldScene_RunOpeningAuxiliarySequence Func_020001d0

/* Loader-relocated overlay calls: each symbol names the pre-relocation call
 * word the image holds. */
s32 Func_02001062();
void Func_0200107c();
s32 Func_02001094();
u8 *Scene_GetRecord_1();
void Func_020010f6();
void Func_020010fc();
void Func_02001114();
u8 *Scene_GetRecord_2();
void Func_0200112c();
void Func_02001130();
void Func_0200113c();
void Func_0200114e();
void Func_0200115e();
void Func_02001184();
void Func_0200119e();
s32 Func_020011a6();
void Func_020011aa();
void Func_020011b2();
void Func_020011d6();
void Func_020011e8();
void Func_020011f4();

/* Resolved engine calls: each pseudo symbol is the per-site call word the
 * overlay image holds (a word can serve two sites with different targets),
 * and the macro names the engine function the site reaches through the
 * overlay veneer and the main-image veneer island, keeping the site's own
 * calling form. Names without a repository binding are provisional.
 */
#define BattleRuntime_Reset_1(args...) Func_0200107c(args)
#define GameFlag_IsSet_1(a0) Value1(Func_02001062, a0)
#define SceneWork_SetStepValue_1(a0) Call1(Func_02001114, a0)
#define BattleEventRuntime_ProcessAction_1(args...) Func_0200113c(args)
#define Object_SetModeById_1(args...) Func_020010f6(args)
#define SceneWork_SetStepValue_2(a0) Call1(Func_0200112c, a0)
#define BattleRuntime_RunThenWaitIfModeZero_1(args...) Func_0200114e(args)
#define GameFlag_IsSet_2(a0) Value1(Func_02001094, a0)
#define Scene_GetRecord_1(args...) Func_020010e0(args)
#define RuntimeBlock_GetOffset1e0Pointer_1(args...) Func_020011a6(args)
#define ObjectMotion_SetSpeedLimitAndAcceleration_1(a0, a1) Call2(Func_0200119e, a0, a1)
#define ObjectMotion_PlaceWithinCameraBounds_1(args...) Func_020011b2(args)
#define ObjectMotion_SetAngleToward_1(a0, a1, a2) Call3(Func_0200115e, a0, a1, a2)
#define ObjectMotion_ArmCallback_1(a0, a1, a2) Call3(Func_020011aa, a0, a1, a2)
#define Object_CommitPositionThenWaitIfModeZero_1(args...) Func_020011d6(args)
#define BattleRuntime_WaitIfModeZero_1(args...) Func_020010fc(args)
#define Scene_GetRecord_2(args...) Func_0200112a(args)
#define ObjectMotion_PlaceWithinCameraBounds_2(args...) Func_020011e8(args)
#define Object_CommitPositionThenWaitIfModeZero_2(args...) Func_020011f4(args)
#define ObjectMotion_CallThenWaitForAnimationChange_1(args...) Func_02001184(args)
#define BattleRuntime_ScheduleShoulderButtonModeUpdate_1(args...) Func_02001130(args)
u8 *Func_020010e0();
u8 *Func_0200112a();

/* Newly named per-site calls: the engine function has no registered name yet,
 * but its own reconstructed source (recorded as "usource" in mains.json)
 * shows what it does. Each name states only what that source shows. */

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

/* Runs one of two branches of a scene setup: a short branch that just moves
 * actor 14, or a longer branch that positions actor 18 and a second record
 * from their x/y/z fields at +8/+12/+16, clearing a byte at +85 on a
 * separately looked-up record along the way. */
void FieldScene_RunOpeningAuxiliarySequence(void)
{
    u8 *rec18;
    u8 *ready_flag;
    u8 *record;

    BattleRuntime_Reset_1();
    if (GameFlag_IsSet_1(0x909) != 0) {
        SceneWork_SetStepValue_1(0x191f);
        BattleEventRuntime_ProcessAction_1(14, 0); /* object 14, action 0 */
    } else {
        Object_SetModeById_1(14, 4); /* object 14, action 4 */
        SceneWork_SetStepValue_2(0x18c7);
        BattleRuntime_RunThenWaitIfModeZero_1(14, 0, 10);
        ready_flag = GameFlag_IsSet_2(0x8ff);
        if (ready_flag == 0) {
            rec18 = Scene_GetRecord_1(18);
            /* Clear the byte at +85 of the lookup result. */
            *(u8 *)(RuntimeBlock_GetOffset1e0Pointer_1() + 85) = ready_flag;
            ObjectMotion_SetSpeedLimitAndAcceleration_1(0x10000, 0x2000);
            ObjectMotion_PlaceWithinCameraBounds_1(*(s32 *)(rec18 + 8), *(s32 *)(rec18 + 12), *(s32 *)(rec18 + 16), 1); /* use_setter 1 */
            ObjectMotion_SetAngleToward_1(0, 0x4000, 0);
            ObjectMotion_ArmCallback_1(14, 0x3000, 0);
            Object_CommitPositionThenWaitIfModeZero_1();
            BattleRuntime_WaitIfModeZero_1(120); /* should_wait 120 */
            record = Scene_GetRecord_2(0);
            ObjectMotion_PlaceWithinCameraBounds_2(*(s32 *)(record + 8), *(s32 *)(record + 12), *(s32 *)(record + 16), 1); /* use_setter 1 */
            Object_CommitPositionThenWaitIfModeZero_2();
        }
        ObjectMotion_CallThenWaitForAnimationChange_1(14, 4);
    }
    BattleRuntime_ScheduleShoulderButtonModeUpdate_1();
}
