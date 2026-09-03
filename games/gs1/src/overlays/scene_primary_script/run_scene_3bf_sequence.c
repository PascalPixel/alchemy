#include "types.h"

#define FieldScene_RunSupplementalSequenceTwo Func_020049a0

/* Audited retained supplemental scene body.
 * The complete production span preserves 49 calls, 0 loop(s), and 0 explicit memory operation(s).
 * Approved GCC changes scheduling, allocation, control-flow lowering, and
 * literal placement, so the reviewed assembly remains authoritative. */

/* Two id arguments recur across most of the calls below: 24 is passed as the
 * first argument throughout the main sequence, and 25 is introduced only
 * after ObjectMotion_SetAngleToward_2(25, 24, 0), whose second argument is 24 -- so 25 is
 * derived from 24 partway through the sequence. */
#define PRIMARY_ID 24
#define DERIVED_ID 25

s32 Func_02009fb6();
s32 Func_02009fc2();
s32 Func_02009fd0();
void Func_0200a00c();
void Func_0200a036();
void Func_0200a046();
void Func_0200a070();
void Func_0200a08a();
void Func_0200a090();
void Func_0200a098();
void Func_0200a09e();
void Func_0200a0a6();
void Func_0200a0b0();
void Func_0200a0b4();
void Func_0200a0b6();
void Func_0200a0cc();
void Func_0200a0ce();
void Func_0200a0d8();
void Func_0200a0e2();
void Func_0200a0fe();
void Func_0200a10c();
void Func_0200a112();
void Func_0200a118();
void Func_0200a122();
void Func_0200a124();
void Func_0200a12a();
void Func_0200a12c();
void Func_0200a132();
void Func_0200a13a();
void Func_0200a142();
void Func_0200a154();
void Func_0200a15a();
void Func_0200a15c();
void Func_0200a162();
void Func_0200a16e();
void Func_0200a17c();
void Func_0200a18c();
void Func_0200a1a4();
void Func_0200a1a6();
void Func_0200a1b4();
void Func_0200a1c2();
void Func_0200a1d6();
void Func_0200a1da();
void Func_0200a20a();
void Func_0200a21a();
void Func_0200a222();
void Func_0200a232();

/* Resolved engine calls: each pseudo symbol is the per-site call word the
 * overlay image holds (a word can serve two sites with different targets),
 * and the macro names the engine function the site reaches through the
 * overlay veneer and the main-image veneer island, keeping the site's own
 * calling form. Names without a repository binding are provisional.
 */
#define GameFlag_IsSet_1(a0) Value1(Func_02009fb6, a0)
#define GameFlag_IsSet_2(a0) Value1(Func_02009fc2, a0)
#define GameFlag_IsSet_3(a0) Value1(Func_02009fd0, a0)
#define SceneWork_SetStepValue_1(a0) Call1(Func_0200a0b6, a0)
#define BattleEvent_RunActionAndWait_1(a0, a1) Call2(Func_0200a0ce, a0, a1)
#define ObjectMotion_SetVariantCallbackAndInvokeObject_1(a0, a1) Value2(Func_0200a0a6, a0, a1)
#define BattleRuntime_WaitIfModeZero_1(a0) Value1(Func_0200a00c, a0)
#define ObjectMotion_SetSpeedParameters_1(a0, a1, a2) Call3(Func_0200a046, a0, a1, a2)
#define ObjectMotion_OffsetPositionAndResetMotion_1(a0, a1, a2) Value3(Func_0200a08a, a0, a1, a2)
#define ObjectMotion_CommitCurrentPositionAndActivate_1(a0) Value1(Func_0200a098, a0)
#define Object_SetModeById_1(a0, a1) Value2(Func_0200a0b0, a0, a1)
#define BattleRuntime_WaitIfModeZero_2(a0) Value1(Func_0200a036, a0)
#define ObjectMotion_SetSpeedParameters_2(a0, a1, a2) Call3(Func_0200a070, a0, a1, a2)
#define ObjectMotion_OffsetPositionAndResetMotion_2(a0, a1, a2) Call3(Func_0200a0b4, a0, a1, a2)
#define ObjectMotion_SetAngleToward_1(a0, a1, a2) Value3(Func_0200a0fe, a0, a1, a2)
#define ObjectMotion_CommitCurrentPositionAndActivate_2(a0) Value1(Func_0200a0cc, a0)
#define SceneWork_SetStepValue_2(a0) Value1(Func_0200a122, a0)
#define BattleEvent_RunActionAndWait_2(a0, a1) Call2(Func_0200a13a, a0, a1)
#define ObjectMotion_SetVariantCallbackAndInvokeObject_2(a0, a1) Call2(Func_0200a112, a0, a1)
#define ObjectMotion_SetAngleToward_2(a0, a1, a2) Value3(Func_0200a124, a0, a1, a2)
#define SceneWork_SetStepValue_3(a0) Value1(Func_0200a142, a0)
#define BattleEvent_RunActionAndWait_3(a0, a1) Value2(Func_0200a15a, a0, a1)
#define BattleRuntime_WaitIfModeZero_3(a0) Value1(Func_0200a090, a0)
#define Object_SetModeById_2(a0, a1) Value2(Func_0200a118, a0, a1)
#define BattleRuntime_WaitIfModeZero_4(a0) Value1(Func_0200a09e, a0)
#define ObjectMotion_SetSpeedParameters_3(a0, a1, a2) Call3(Func_0200a0d8, a0, a1, a2)
#define ObjectMotion_ResetAndSetPositionInMode2_1(a0, a1, a2) Value3(Func_0200a10c, a0, a1, a2)
#define ObjectMotion_CommitCurrentPositionAndActivate_3(a0) Value1(Func_0200a12a, a0)
#define ObjectMotion_ArmCallback_1(a0, a1, a2) Value3(Func_0200a1a6, a0, a1, a2)
#define SceneWork_SetStepValue_4(a0) Value1(Func_0200a18c, a0)
#define BattleEvent_RunActionAndWait_4(a0, a1) Call2(Func_0200a1a4, a0, a1)
#define Object_SetModeById_3(a0, a1) Value2(Func_0200a15c, a0, a1)
#define BattleRuntime_WaitIfModeZero_5(a0) Value1(Func_0200a0e2, a0)
#define ObjectMotion_OffsetPositionAndResetMotion_3(a0, a1, a2) Value3(Func_0200a154, a0, a1, a2)
#define ObjectMotion_CommitCurrentPositionAndActivate_4(a0) Value1(Func_0200a162, a0)
#define Object_SetModeById_4(a0, a1) Value2(Func_0200a17c, a0, a1)
#define SceneWork_SetStepValue_5(a0) Value1(Func_0200a1c2, a0)
#define BattleEvent_RunActionAndWait_5(a0, a1) Call2(Func_0200a1da, a0, a1)
#define ObjectMotion_ResetAndSetPositionInMode2_2(a0, a1, a2) Value3(Func_0200a16e, a0, a1, a2)
#define ObjectMotion_CommitCurrentPositionAndActivate_5(a0) Value1(Func_0200a18c, a0)
#define Object_LinkPair_1(a0, a1, a2) Value3(Func_0200a1d6, a0, a1, a2)
#define BattleRuntime_WaitIfModeZero_6(a0) Value1(Func_0200a12c, a0)
#define Object_SetModeById_5(a0, a1) Call2(Func_0200a1b4, a0, a1)
#define BattleRuntime_WaitIfModeZero_7(a0) Value1(Func_0200a13a, a0)
#define GameFlag_Set_1(a0) Value1(Func_0200a132, a0)
#define SceneWork_SetStepValue_6(a0) Call1(Func_0200a20a, a0)
#define BattleEvent_RunActionAndWait_6(a0, a1) Call2(Func_0200a222, a0, a1)
#define SceneWork_SetStepValue_7(a0) Call1(Func_0200a21a, a0)
#define BattleEvent_RunActionAndWait_7(a0, a1) Call2(Func_0200a232, a0, a1)

/* Loader-relocated ROM calls: each site names the pre-relocation call word the image holds. */


static __inline__ void Call0(void (*f)())
{
    f();
}

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

static __inline__ s32 Value0(s32 (*f)())
{
    return f();
}

static __inline__ s32 Value1(s32 (*f)(), s32 a0)
{
    return f(a0);
}

static __inline__ s32 Value2(s32 (*f)(), s32 a0, s32 a1)
{
    return f(a0, a1);
}

static __inline__ s32 Value3(s32 (*f)(), s32 a0, s32 a1, s32 a2)
{
    return f(a0, a1, a2);
}

/* Runs a gated sequence of parameterized calls on PRIMARY_ID (24) and, once
 * derived partway through, DERIVED_ID (25); a sequence id counting up from
 * 9569 is threaded through four of the calls one apart. Each of the two
 * outer gating checks has its own short fallback branch on PRIMARY_ID. */
void FieldScene_RunSupplementalSequenceTwo(void)
{
    s32 sequence_id;

    if (GameFlag_IsSet_1(2369) != 0) {
        if (GameFlag_IsSet_2(2382) == 0 && GameFlag_IsSet_3(788) == 0) {
        sequence_id = 9569;
        SceneWork_SetStepValue_1(sequence_id);
        BattleEvent_RunActionAndWait_1(PRIMARY_ID, 0);
        ObjectMotion_SetVariantCallbackAndInvokeObject_1(PRIMARY_ID, 1);
        BattleRuntime_WaitIfModeZero_1(30);
        ObjectMotion_SetSpeedParameters_1(PRIMARY_ID, 6553, 3276);
        ObjectMotion_OffsetPositionAndResetMotion_1(PRIMARY_ID, -4, 0);
        ObjectMotion_CommitCurrentPositionAndActivate_1(PRIMARY_ID);
        Object_SetModeById_1(PRIMARY_ID, 3);
        BattleRuntime_WaitIfModeZero_2(60);
        ObjectMotion_SetSpeedParameters_2(PRIMARY_ID, 13107, 6553);
        ObjectMotion_OffsetPositionAndResetMotion_2(PRIMARY_ID, -6, 0);
        ObjectMotion_SetAngleToward_1(PRIMARY_ID, 0, 0);
        ObjectMotion_CommitCurrentPositionAndActivate_2(PRIMARY_ID);
        SceneWork_SetStepValue_2(sequence_id + 1);
        BattleEvent_RunActionAndWait_2(PRIMARY_ID, 0);
        ObjectMotion_SetVariantCallbackAndInvokeObject_2(PRIMARY_ID, 1);
        ObjectMotion_SetAngleToward_2(DERIVED_ID, PRIMARY_ID, 0);
        SceneWork_SetStepValue_3(sequence_id + 2);
        BattleEvent_RunActionAndWait_3(PRIMARY_ID, 0);
        BattleRuntime_WaitIfModeZero_3(70);
        Object_SetModeById_2(DERIVED_ID, 3);
        BattleRuntime_WaitIfModeZero_4(60);
        ObjectMotion_SetSpeedParameters_3(DERIVED_ID, 26214, 13107);
        ObjectMotion_ResetAndSetPositionInMode2_1(DERIVED_ID, 880, 112);
        ObjectMotion_CommitCurrentPositionAndActivate_3(DERIVED_ID);
        ObjectMotion_ArmCallback_1(DERIVED_ID, 53248, 0);
        SceneWork_SetStepValue_4(sequence_id + 3);
        BattleEvent_RunActionAndWait_4(PRIMARY_ID, 0);
        Object_SetModeById_3(PRIMARY_ID, 3);
        BattleRuntime_WaitIfModeZero_5(70);
        ObjectMotion_OffsetPositionAndResetMotion_3(PRIMARY_ID, 8, 0);
        ObjectMotion_CommitCurrentPositionAndActivate_4(PRIMARY_ID);
        Object_SetModeById_4(PRIMARY_ID, 5);
        SceneWork_SetStepValue_5(sequence_id + 4);
        BattleEvent_RunActionAndWait_5(PRIMARY_ID, 0);
        ObjectMotion_ResetAndSetPositionInMode2_2(0, 896, 120);
        ObjectMotion_CommitCurrentPositionAndActivate_5(0);
        Object_LinkPair_1(0, DERIVED_ID, 0);
        BattleRuntime_WaitIfModeZero_6(60);
        Object_SetModeById_5(DERIVED_ID, 3);
        BattleRuntime_WaitIfModeZero_7(30);
        GameFlag_Set_1(788);
        } else {
            SceneWork_SetStepValue_6(9575);
            BattleEvent_RunActionAndWait_6(PRIMARY_ID, 0);
        }
    } else {
        SceneWork_SetStepValue_7(0x244d);
        BattleEvent_RunActionAndWait_7(PRIMARY_ID, 0);
    }
}
