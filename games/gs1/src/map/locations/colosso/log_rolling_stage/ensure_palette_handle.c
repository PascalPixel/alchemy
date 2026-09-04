#include "colosso_log_rolling_stage.h"

extern s16 Data_0200dace;

s32 Func_0200829e(void);       /* established (veneer to Func_080153b8) */

void ColossoLogRollingStage_EnsurePaletteHandle(void)
{
    s16 *cursor = &Data_0200dace;

    if (*cursor == -1) {
        *cursor = Func_0200829e();
    }
}
