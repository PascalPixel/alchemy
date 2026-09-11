#include "scene.h"
#include "abi/map/locations/colosso/log_rolling_stage/event/run_if_ready.h"
#include "colosso_log_rolling_stage.h"

void Colosso_RunSceneEventIfReady(void)
{
    if (Map_Check() == 0) {
        Map_Run();
    }
}
