#include "types.h"

#define FieldScene_RunOpeningSequenceSecond Func_020019e4

s32 Func_0200671a();
extern u8 Value_00000854;
extern u8 Value_000012c3;
void Func_02006752();
void Func_0200676c();
void Func_02006776();
void Func_020067b4();
void Func_02006828();
void Func_02006840();
void Func_020068a6();
void Func_02006958();

/* Resolved engine calls: each pseudo symbol is the per-site call word the
 * overlay image holds (a word can serve two sites with different targets),
 * and the macro names the engine function the site reaches through the
 * overlay veneer and the main-image veneer island, keeping the site's own
 * calling form. Names without a repository binding are provisional.
 */
#define ObjectMotion_SetSpeedParameters_1(a0, a1, a2) Call3(Func_02006776, a0, a1, a2)
#define ObjectMotion_SetPositionAndReset_1(a0, a1, a2) Call3(Func_020067b4, a0, a1, a2)
#define GameFlag_IsSet_1(args...) Func_0200671a(args)
#define BattleRuntime_Reset_1() Call0(Func_02006752)
#define SceneWork_SetStepValue_1(a0) Value1(Func_02006828, a0)
#define BattleEvent_RunActionAndWait_1(a0, a1) Call2(Func_02006840, a0, a1)
#define BattleRuntime_ScheduleShoulderButtonModeUpdate_1() Call0(Func_0200676c)
#define Audio_PlayCue_1(a0) Value1(Func_02006958, a0)
static __inline__ s32 Value1(s32 (*f)(), s32 a0)
{
    return f(a0);
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

/* Field at +456 of the shared scene work record, addressed through the
 * loader-fixed pointer at 0x03001ebc. */
#define SCENE_WORK_FIELD_456 (*(u32 *)(*(u8 **)0x03001ebc + 456))

/* Runs the second part of an opening sequence: two parameterised calls, then
 * (only if a flag read via Value_00000854 is clear) a short block of setup
 * calls, then unconditionally sets a scene work field and two more calls. */
void FieldScene_RunOpeningSequenceSecond(void)
{
    ObjectMotion_SetSpeedParameters_1(0, 32768, 16384); /* object 0, speed_limit 32768, acceleration 16384 */
    ObjectMotion_SetPositionAndReset_1(0, 744, 408); /* object 0, x 744, z 408 */
    if (GameFlag_IsSet_1((s32)&Value_00000854) == 0) {
        BattleRuntime_Reset_1();
        SceneWork_SetStepValue_1((s32)&Value_000012c3);
        BattleEvent_RunActionAndWait_1(8, 0); /* action 8 */
        BattleRuntime_ScheduleShoulderButtonModeUpdate_1();
    }
    SCENE_WORK_FIELD_456 = 16;
    Audio_PlayCue_1(123);
    Call1(Func_020068a6, 14); /* main:0808a248 */
}
