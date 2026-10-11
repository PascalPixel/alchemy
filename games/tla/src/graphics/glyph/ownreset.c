#include "types.h"
#include "callback_scheduler.h"

void Func_0803df24(void);

void Resource_ScheduleOwnerReset(void)
{
    Scheduler_RemoveCallback(Func_0803df24);
}
