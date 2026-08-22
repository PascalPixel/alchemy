#include "types.h"



/* Advance actor eleven through the two presentation states used at scene end. */
extern void Func_02003900(s32 actorId, s32 state);
extern void Func_02003908(s32 actorId, s32 state);
void Func_02001bfc(void)
{
    Func_02003900(11, 1);
    Func_02003908(11, 2);
}
