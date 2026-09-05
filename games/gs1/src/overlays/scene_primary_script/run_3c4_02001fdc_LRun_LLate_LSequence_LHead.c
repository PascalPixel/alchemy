#include "types.h"

#define FieldScene_RunLateSequenceHead Func_02001fdc

void Func_0200508e();
void Func_020050a0();
void Func_02005162();
void Func_02005170();
void Func_020051a2();
void Func_020051ac();

/* Resolved engine calls: each pseudo symbol is the per-site call word the
 * overlay image holds (a word can serve two sites with different targets),
 * and the macro names the engine function the site reaches through the
 * overlay veneer and the main-image veneer island, keeping the site's own
 * calling form. Names without a repository binding are provisional.
 */
#define EffectRuntime_SetCurrentPosition_1(a0, a1, a2) Call3(Func_020051a2, a0, a1, a2)
#define EffectRuntime_SetCurrentPosition_2(a0, a1, a2) Call3(Func_020051ac, a0, a1, a2)
#define ObjectMotion_SetHorizontalPositionWithTerrain_1(a0, a1, a2) Call3(Func_02005162, a0, a1, a2)
#define ObjectMotion_SetHorizontalPositionWithTerrain_2(a0, a1, a2) Call3(Func_02005170, a0, a1, a2)
static __inline__ void Call3(void (*f)(), s32 a0, s32 a1, s32 a2)
{
    f(a0, a1, a2);
}

static __inline__ void Call6(void (*f)(), s32 a0, s32 a1, s32 a2, s32 a3, s32 a4, s32 a5)
{
    f(a0, a1, a2, a3, a4, a5);
}

/* Runs six queued setup calls for this scene: two paired 6-argument calls
 * whose last two args repeat the first two (72,49 / 113,43), two 3-argument
 * id calls (100, 101) with zeroed remaining args, and two more 3-argument
 * calls (id 15, 16) passing a start/end pair where the second call's start
 * value (198<<18) equals the first call's end value. */
void FieldScene_RunLateSequenceHead(void)
{
    Call6(Func_0200508e, 72, 49, 1, 1, 8, 49); /* main:080091c0 */
    Call6(Func_020050a0, 113, 43, 1, 1, 49, 43); /* main:080091c0 */
    EffectRuntime_SetCurrentPosition_1(100, 0, 0);
    EffectRuntime_SetCurrentPosition_2(101, 0, 0);
    ObjectMotion_SetHorizontalPositionWithTerrain_1(15, 8912896, 51904512); /* 136<<16, 198<<18 */
    ObjectMotion_SetHorizontalPositionWithTerrain_2(16, 51904512, 45613056); /* 198<<18, 174<<18 */
}
