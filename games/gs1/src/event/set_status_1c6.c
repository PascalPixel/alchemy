#include "scene.h"
#include "event_runtime.h"
#include "types.h"

/* event/set_status_1c6.c */
void FunctionHead_0808fefc(s32 first, s32 second);

void Event_SetStatus1c6(void)
{
    struct EventRuntime *runtime = gWork;

    FunctionHead_0808fefc(runtime->value_1c0, runtime->value_1c8);
    runtime->status_1c6 = 1;
}

/* event/clear_status_1c6.c */
u32 Sys_Apply(s32 first, s32 second);

void Event_ClearStatus1c6(void)
{
    struct EventRuntime *runtime = gWork;

    Sys_Apply(runtime->value_1c0, runtime->value_1c8);
    runtime->status_1c6 = 0;
}

/* event/wait_value_1c8_frames.c */
s32 WaitFrames(s32);

void Event_WaitValue1c8Frames(void)
{
    WaitFrames(gWork->value_1c8);
}

/* event/set_pair_work_1c0.c */
extern struct EventPairWork1c0 gCell;

void Event_SetPairWork1c0(u16 first, u16 second)
{
    gWork->value_170 = 999;
    gCell.first = first;
    gCell.second = second;
}
