/* Draft of overlay 65f Func_0200132c. differ 2 of 10: the listing builds
 * the first argument (r0 = 1) last, after r1 and r2; this draft builds it
 * first. A u8 or pointer first parameter, an unprototyped callee with a
 * local, and an inline returning the id all gave the same order. Overlay
 * 6aa's Func_02000070 and Func_0200196c show the same r0-last order. */
#include "types.h"

void GameFlag_SetBit(s32 flag);
void Func_02005354(s32 a, s32 b, s32 c);

void Func_0200132c(void)
{
    Func_02005354(1, 10, 11);
    GameFlag_SetBit(0x203);
}
