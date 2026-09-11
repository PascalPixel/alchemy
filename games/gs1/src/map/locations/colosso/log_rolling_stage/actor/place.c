#include "scene.h"
#include "abi/map/locations/colosso/log_rolling_stage/actor/place.h"
#include "colosso_log_rolling_stage.h"

s32 *Actor_Run();          /* entity by selector, established */
void Actor_Run2();          /* unestablished */
void Actor_Run3();          /* established (record, mode) */
void Actor_Run4();          /* established (record, x, y, z) */

void Colosso_PositionActor(s32 selector, s32 x, s32 z)
{
    s32 *record;

    record = Actor_Run(selector);
    if (record != 0) {
        Actor_Run2();
        Actor_Run3(record, 5);
        Actor_Run4(record, x << 16, record[3], z << 16);
    }
}
