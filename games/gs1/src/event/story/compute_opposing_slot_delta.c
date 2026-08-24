#include "types.h"

/* Complete four-slot opposing-pair total helper. */
extern s32 Func_0200686a(s32 slot);
extern s32 Func_02006872(s32 slot);
extern s32 Func_0200687a(s32 slot);
extern s32 Func_02006882(s32 slot);

s32 StoryScene_ComputeOpposingSlotDelta(void)
{
    s32 positive_total = Func_0200686a(0);
    s32 negative_total;

    positive_total += Func_02006872(2);
    negative_total = Func_0200687a(1);
    negative_total += Func_02006882(3);
    return positive_total - negative_total;
}
