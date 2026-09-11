#include "scene.h"
#include "abi/event/set_status_1c6.h"
#include "event_runtime.h"

void Event_SetStatus1c6(void)
{
    struct EventRuntime *runtime = gWork;

    Sys_Apply(runtime->value_1c0, runtime->value_1c8);
    runtime->status_1c6 = 1;
}
