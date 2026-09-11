#include "scene.h"
#include "colosso_log_rolling_stage.h"

s32 *Actor_Run();          /* entity by selector, established */
void Actor_unk2_4();          /* unestablished */
void Actor_unk3_4();          /* established (record, mode) */
void Actor_unk4_4();          /* established (record, x, y, z) */

void Colosso_PositionActor(s32 selector, s32 x, s32 z)
{
    s32 *record;

    record = Actor_Run(selector);
    if (record != 0) {
        Actor_unk2_4();
        Actor_unk3_4(record, 5);
        Actor_unk4_4(record, x << 16, record[3], z << 16);
    }
}
