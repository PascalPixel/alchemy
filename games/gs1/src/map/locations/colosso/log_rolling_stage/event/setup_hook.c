#include "scene.h"
#include "abi/map/locations/colosso/log_rolling_stage/event/setup_hook.h"
#include "colosso_log_rolling_stage.h"

void Colosso_RunSetupHook(void)
{
    Map_Run();
}
