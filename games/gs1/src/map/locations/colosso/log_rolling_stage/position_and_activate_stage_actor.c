#include "colosso_log_rolling_stage.h"

s32 *Func_02006e64();          /* entity by selector, established */
void Func_02006c06();          /* unestablished */
void Func_02006bee();          /* established (record, mode) */
void Func_02006c2a();          /* established (record, x, y, z) */
void Func_02006c38();          /* unestablished, single argument */
void Func_02006c08();          /* established (record, mode) */

void ColossoLogRollingStage_PositionAndActivateActor(s32 selector, s32 x, s32 z)
{
    s32 *record;

    record = Func_02006e64(selector);
    if (record != 0) {
        Func_02006c06();
        Func_02006bee(record, 5);
        Func_02006c2a(record, x << 16, record[3], z << 16);
        Func_02006c38(record);
        Func_02006c08(record, 1);
    }
}
