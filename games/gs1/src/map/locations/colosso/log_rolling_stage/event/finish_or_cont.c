#include "scene.h"
#include "abi/map/locations/colosso/log_rolling_stage/event/finish_or_cont.h"
#include "colosso_log_rolling_stage.h"

void Colosso_FinishOrContinueSceneEvent(void)
{
    if (Map_Check() == 0) {
        Map_Run();
    } else {
        Map_Run2();
    }
}
