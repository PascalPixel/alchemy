#include "scene.h"
#include "event_runtime.h"
#include "types.h"

/* event/set_status_1c6.c */
void Event_SetStatus1c6(void)
{
    struct EventRuntime *runtime = gWork;

    Sys_Apply(runtime->value_1c0, runtime->value_1c8);
    runtime->status_1c6 = 1;
}

/* event/clear_status_1c6.c */
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

/* event/set_pair1c4_and_reset_value170.c */
struct State_08091e6c {
    u8 filler0[0x170];
    u16 value;
};

struct gRom {
    u8 filler0[0x1C4];
    u16 first;
    u16 second;
};

extern struct State_08091e6c *volatile gWork;
extern struct gRom gCell;

void Event_SetPair1c4AndResetValue170(u16 first, u16 second)
{
    struct State_08091e6c *state = gWork;
    state->value = 999;
    gCell.first = first;
    gCell.second = second;
}

void Sys_Run(u16 value)
{
    struct State_08091e6c *state = gWork;
    state->value = value;
}
