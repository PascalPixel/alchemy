#include "scene.h"
#include "abi/map/locations/colosso/log_rolling_stage/event/setup_done.h"
#include "colosso_log_rolling_stage.h"

void Colosso_RunSetupCompletionHooks(void)
{
    Map_Run();
    Map_Run2();
}
