#include "scene.h"
#include "abi/map/locations/colosso/log_rolling_stage/event/set_values.h"
#include "colosso_log_rolling_stage.h"

s32 Colosso_SetSceneEventValues(void)
{
    Map_Do(1);
    Map_Do2(2);
    Map_Do3(288);
    Map_Do4(217);
    return 0;
}
