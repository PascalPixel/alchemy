#include "types.h"

void Func_020076a0();
void Func_020076f8();
void Func_0200771e();
void Func_0200772c();
void Func_0200773a();
void Func_0200774a();
void Func_02007752();
void Func_0200775a();
void Func_0200778c();
void Func_02007834();

/* Resolved engine calls: each pseudo symbol is the per-site call word the
 * overlay image holds (a word can serve two sites with different targets),
 * and the macro names the engine function the site reaches through the
 * overlay veneer and the main-image veneer island, keeping the site's own
 * calling form. Names without a repository binding are provisional.
 */
#define BattleRuntime_Reset_1() Call0(Func_020076a0)
#define ObjectMotion_SetHorizontalPositionWithTerrain_1(a0, a1, a2) Call3(Func_0200771e, a0, a1, a2)
#define ObjectMotion_SetHorizontalPositionWithTerrain_2(a0, a1, a2) Call3(Func_0200772c, a0, a1, a2)
#define ObjectMotion_SetHorizontalPositionWithTerrain_3(a0, a1, a2) Call3(Func_0200773a, a0, a1, a2)
#define Object_SetModeById_1(a0, a1) Call2(Func_0200774a, a0, a1)
#define Object_SetModeById_2(a0, a1) Call2(Func_02007752, a0, a1)
#define Object_SetModeById_3(a0, a1) Call2(Func_0200775a, a0, a1)
#define ObjectMotion_SetAngleToward_1(a0, a1, a2) Call3(Func_0200778c, a0, a1, a2)
#define BattleRuntime_ScheduleShoulderButtonModeUpdate_1() Call0(Func_020076f8)
#define BattleRuntime_WaitIfModeZero_1() Call0(Func_02007834)
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

void FieldScene_RunSequenceTail(void)
{
    BattleRuntime_Reset_1();
    ObjectMotion_SetHorizontalPositionWithTerrain_1(12, 45088768, 5767168); /* object_id 12, x, z */
    ObjectMotion_SetHorizontalPositionWithTerrain_2(13, 46137344, 5767168); /* object_id 13, x, z */
    ObjectMotion_SetHorizontalPositionWithTerrain_3(14, 47185920, 6291456); /* object_id 14, x, z */
    Object_SetModeById_1(12, 5); /* object_id 12, action 5 */
    Object_SetModeById_2(13, 5); /* object_id 13, action 5 */
    Object_SetModeById_3(14, 5); /* object_id 14, action 5 */
    ObjectMotion_SetAngleToward_1(0, 13, 0);
    BattleRuntime_ScheduleShoulderButtonModeUpdate_1();
    BattleRuntime_WaitIfModeZero_1(); /* main:0808a360 */
}
