#include "types.h"

void Func_020050f2();
void Func_02005104();
void Func_020051ca();
void Func_020051d4();
void Func_0200520a();
void Func_02005218();

/* Resolved engine calls: each pseudo symbol is the per-site call word the
 * overlay image holds (a word can serve two sites with different targets),
 * and the macro names the engine function the site reaches through the
 * overlay veneer and the main-image veneer island, keeping the site's own
 * calling form. Names without a repository binding are provisional.
 */
#define EffectRuntime_SetCurrentPosition_1(a0, a1, a2) Call3(Func_0200520a, a0, a1, a2)
#define EffectRuntime_SetCurrentPosition_2(a0, a1, a2) Call3(Func_02005218, a0, a1, a2)
#define ObjectMotion_SetHorizontalPositionWithTerrain_1(a0, a1, a2) Call3(Func_020051ca, a0, a1, a2)
#define ObjectMotion_SetHorizontalPositionWithTerrain_2(a0, a1, a2) Call3(Func_020051d4, a0, a1, a2)
static __inline__ void Call3(void (*f)(), s32 a0, s32 a1, s32 a2)
{
    f(a0, a1, a2);
}

static __inline__ void Call6(void (*f)(), s32 a0, s32 a1, s32 a2, s32 a3, s32 a4, s32 a5)
{
    f(a0, a1, a2, a3, a4, a5);
}

/* Two six-argument calls whose first and fifth arguments repeat the same id
 * (8 and 49 respectively), followed by four three-argument calls each keyed
 * by an id with a trailing pair of values (-1, -1 or 0, 0). */
void FieldScene_RunLateSequenceSecond(void)
{
    Call6(Func_020050f2, 8, 113, 1, 1, 8, 49);
    Call6(Func_02005104, 49, 107, 1, 1, 49, 43);
    EffectRuntime_SetCurrentPosition_1(100, -1, -1);
    EffectRuntime_SetCurrentPosition_2(101, -1, -1);
    ObjectMotion_SetHorizontalPositionWithTerrain_1(15, 0, 0);
    ObjectMotion_SetHorizontalPositionWithTerrain_2(16, 0, 0);
}
