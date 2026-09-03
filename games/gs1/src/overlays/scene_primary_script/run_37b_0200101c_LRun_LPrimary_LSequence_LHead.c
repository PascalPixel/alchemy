#include "types.h"

#define FieldScene_RunPrimarySequenceHead Func_0200101c

s32 Func_02003456();
void Func_0200347e();
void Func_0200349c();
void Func_020034a6();
void Func_020034ae();
void Func_020034da();

/* Resolved engine calls: each pseudo symbol is the per-site call word the
 * overlay image holds (a word can serve two sites with different targets),
 * and the macro names the engine function the site reaches through the
 * overlay veneer and the main-image veneer island, keeping the site's own
 * calling form. Names without a repository binding are provisional.
 */
#define GameFlag_IsSet_1(args...) Func_02003456(args)
#define BattleRuntime_Reset_1() Call0(Func_0200347e)
#define Scene_GetRecord_1(a0) Call1(Func_0200349c, a0)
#define ObjectMotion_SetSpeedParameters_1(a0, a1, a2) Call3(Func_020034ae, a0, a1, a2)
#define ObjectMotion_SetPositionAndReset_1(a0, a1, a2) Call3(Func_020034da, a0, a1, a2)
#define BattleRuntime_ScheduleShoulderButtonModeUpdate_1() Call0(Func_020034a6)
static __inline__ s32 Value2(s32 (*f)(), s32 a0, s32 a1)
{
    return f(a0, a1);
}

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

/* Argument shared by every configuration call below. */
#define TARGET_ID 9

/* Condition code passed to the gating check; the body below runs only when
 * that check reports 0. */
#define GATE_CODE 2059

/* If the code-2059 check passes, runs a short setup/configuration sequence
 * for id 9: two no-argument calls bracket a select call and two calls each
 * taking a pair of numeric arguments. */
void FieldScene_RunPrimarySequenceHead(void)
{
    if (GameFlag_IsSet_1(GATE_CODE) == 0) {
        BattleRuntime_Reset_1();
        Scene_GetRecord_1(TARGET_ID);
        ObjectMotion_SetSpeedParameters_1(TARGET_ID, 13107, 0x00001999); /* object_id, speed_limit, acceleration */
        ObjectMotion_SetPositionAndReset_1(TARGET_ID, 504, 152); /* object_id, x=504, z=152 */
        BattleRuntime_ScheduleShoulderButtonModeUpdate_1();
    }
}
