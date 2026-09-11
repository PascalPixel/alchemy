#include "types.h"
#include "scene.h"
#include "abi/map/locations/heidia/village/scene/seq_tail.h"

void Scene_RunSequenceTail(void)
{
    Battle_Reset_1();
    Motion_SetHPosTerrain_1(12, 45088768, 5767168); /* object_id 12, x, z */
    Motion_SetHPosTerrain_2(13, 46137344, 5767168); /* object_id 13, x, z */
    Motion_SetHPosTerrain_3(14, 47185920, 6291456); /* object_id 14, x, z */
    Object_SetModeById_1(12, 5); /* object_id 12, action 5 */
    Object_SetModeById_2(13, 5); /* object_id 13, action 5 */
    Object_SetModeById_3(14, 5); /* object_id 14, action 5 */
    Motion_SetAngleToward_1(0, 13, 0);
    Battle_SchedShoulder_1();
    Battle_WaitMode0_1(); /* main:0808a360 */
}
