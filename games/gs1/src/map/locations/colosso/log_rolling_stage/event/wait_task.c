#include "scene.h"
#include "abi/map/locations/colosso/log_rolling_stage/event/wait_task.h"
#include "colosso_log_rolling_stage.h"

void Colosso_WaitForSceneEventTask(void)
{
    s32 *status;
    s32 value;

    Map_Do(28);
    Map_Do2(0x361);
    Map_Do3(10);
    value = *(s32 *)0x0200D480;
    if (value != 1 && value != 3) {
        status = (s32 *)0x0200D480;
        do {
            Map_Do4(1);
            value = *status;
        } while (value != 1 && value != 3);
    }
    Map_Do5(1);
    Map_Do6(0x0200804D);
}
