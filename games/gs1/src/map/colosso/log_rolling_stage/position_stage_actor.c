#include "colosso_log_rolling_stage.h"


s32 *Func_02006e34();          /* entity by selector, established */
void Func_02006bd6();          /* unestablished */
void Func_02006bbe();          /* established (record, mode) */
void Func_02006bfa();          /* established (record, x, y, z) */

void ColossoLogRollingStage_PositionActor(s32 selector, s32 x, s32 z)
{
    s32 *record;

    record = Func_02006e34(selector);
    if (record != 0) {
        Func_02006bd6();
        Func_02006bbe(record, 5);
        Func_02006bfa(record, x << 16, record[3], z << 16);
    }
}
