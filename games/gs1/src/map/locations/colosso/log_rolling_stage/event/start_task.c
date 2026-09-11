#include "scene.h"
#include "abi/map/locations/colosso/log_rolling_stage/event/start_task.h"
#include "colosso_log_rolling_stage.h"

void Colosso_StartSceneTask(void)
{
    Map_Apply(0x0200804D, 0xC80);
}
