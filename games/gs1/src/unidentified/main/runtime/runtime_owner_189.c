#include "global_progress.h"
#include "types.h"

s32 Func_08079754(s32 amount)
{
    s32 value;

    struct GlobalProgressPartialView *progress = GlobalProgress_Get();

    value = progress->value_11c;
    value += amount;
    if (value > 28)
        value = 28;
    if (value < 0)
        value = 0;
    progress->value_11c = (s8)value;
    return value;
}
