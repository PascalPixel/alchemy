#include "colosso_log_rolling_stage.h"

/* Complete eight-byte state setter plus its sole four-byte pool word. */
void ColossoLogRollingStage_SetBalanceStateReady(void)
{
    u16 *p = (u16 *)0x02001000;
    u16 v = 9;
    *p = v;
}
