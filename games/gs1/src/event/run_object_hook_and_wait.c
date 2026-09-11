#include "scene.h"
#include "abi/event/run_object_hook_and_wait.h"
#include "event_runtime.h"
#include "object_lookup.h"

extern struct EventValueWork gCell;

void WaitFrames(s32);

void Event_RunObjectHookAndWait(s32 object_id)
{
    Sys_Run();
    Sys_Do(object_id);
    WaitFrames(1);
    ObjectTable_Get(gCell.value);
}
