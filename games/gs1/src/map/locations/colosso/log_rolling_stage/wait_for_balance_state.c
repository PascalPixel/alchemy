#include "colosso_log_rolling_stage.h"

extern s16 Data_02001000;

void Func_02008520();          /* advance the task scheduler, established (veneer to Func_080000c0) */

void ColossoLogRollingStage_WaitForBalanceState(void)
{
    s16 *status = &Data_02001000;

    while (*status != 9) {
        Func_02008520(1);
    }
}
