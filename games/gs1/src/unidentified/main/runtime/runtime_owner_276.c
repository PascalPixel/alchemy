#include "global_progress.h"
#include "types.h"

s32 Func_08079728(s32 amount)
{
    s32 value;

    struct GlobalProgressPartialView *progress = GlobalProgress_Get();

    value = progress->value_118;
    value += amount;
    if (value > 0xf423f)
        value = 0xf423f;
    if (value < 0)
        value = 0;
    progress->value_118 = value;
    return value;
}
