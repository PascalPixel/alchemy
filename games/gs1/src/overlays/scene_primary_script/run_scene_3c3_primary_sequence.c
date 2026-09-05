#include "types.h"

void Func_02000bd6();
void Func_02000c04();
void Func_02000c0e();
void Func_02000c12();
void Func_02000c20();
void Func_02000c2e();
void Func_02000c42();
void Func_02000c5c();
void Func_02000c82();
void Func_02000cbe();
void Func_02000cca();

/* Resolved engine calls: each pseudo symbol is the per-site call word the
 * overlay image holds (a word can serve two sites with different targets),
 * and the macro names the engine function the site reaches through the
 * overlay veneer and the main-image veneer island, keeping the site's own
 * calling form. Names without a repository binding are provisional.
 */
#define BattleRuntime_Reset_1() Call0(Func_02000bd6)
#define ObjectMotion_SetSpeedParameters_1(a0, a1, a2) Call3(Func_02000c04, a0, a1, a2)
#define ObjectMotion_SetSpeedParameters_2(a0, a1, a2) Call3(Func_02000c12, a0, a1, a2)
#define ObjectMotion_ResetAndSetPositionInMode2_1(a0, a1, a2) Call3(Func_02000c2e, a0, a1, a2)
#define ObjectMotion_SetPositionAndReset_1(a0, a1, a2) Call3(Func_02000c42, a0, a1, a2)
#define ObjectMotion_ArmCallback_1(a0, a1, a2) Call3(Func_02000cbe, a0, a1, a2)
#define ObjectMotion_ArmCallback_2(a0, a1, a2) Call3(Func_02000cca, a0, a1, a2)
#define Object_SetModeById_1(a0, a1) Call2(Func_02000c82, a0, a1)
#define BattleRuntime_ScheduleShoulderButtonModeUpdate_1() Call0(Func_02000c5c)
static __inline__ void Call0(void (*f)())
{
    f();
}

static __inline__ void Call2(void (*f)(), s32 a0, s32 a1)
{
    f(a0, a1);
}

static __inline__ void Call3(void (*f)(), s32 a0, s32 a1, s32 a2)
{
    f(a0, a1, a2);
}

static __inline__ void Call6(void (*f)(), s32 a0, s32 a1, s32 a2, s32 a3, s32 a4, s32 a5)
{
    f(a0, a1, a2, a3, a4, a5);
}

/* Runs a fixed sequence of setup calls, mostly in mirrored pairs for
 * entities 8 and 9, followed by two six-argument calls whose second and
 * last arguments match (entity 6/27 and entity 9/26). */
void FieldScene_RunPrimarySequence(void)
{
    BattleRuntime_Reset_1();
    ObjectMotion_SetSpeedParameters_1(8, 65536, 32768);
    ObjectMotion_SetSpeedParameters_2(9, 65536, 32768);
    ObjectMotion_ResetAndSetPositionInMode2_1(8, 136, 384);
    ObjectMotion_SetPositionAndReset_1(9, 152, 384);
    ObjectMotion_ArmCallback_1(8, 16384, 0);
    ObjectMotion_ArmCallback_2(9, 16384, 0);
    Object_SetModeById_1(8, 1);
    Call6(Func_02000c0e, 6, 27, 1, 1, 7, 27);
    Call6(Func_02000c20, 9, 26, 2, 1, 7, 26);
    BattleRuntime_ScheduleShoulderButtonModeUpdate_1();
}
