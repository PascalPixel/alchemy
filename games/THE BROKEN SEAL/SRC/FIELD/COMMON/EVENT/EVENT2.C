#include "SCENE.H"
#include "EVENT_RUNTIME.H"
#include "TYPES.H"

/* event/set_status_1c6.c */
void DisplayTransition_Start(s32 mode, s32 frames);

/* event/clear_status_1c6.c */
void DisplayTransition_Finish(s32 mode, s32 frames);

/* event/wait_value_1c8_frames.c */
s32 WaitFrames(s32);

/* event/set_pair_work_1c0.c */
extern struct EventPairWork1c0 gGameState;

struct State_08091e6c {
    u8 filler0[0x170];
    u16 value;
};

struct Data_08091e6c {
    u8 filler0[0x1C4];
    u16 first;
    u16 second;
};

extern struct State_08091e6c *gEventWork;

void Event_SetStatus1c6(void)
{
    struct EventRuntime *runtime = gWork;

    DisplayTransition_Start(runtime->value_1c0, runtime->value_1c8);
    runtime->status_1c6 = 1;
}

void Event_ClearStatus1c6(void)
{
    struct EventRuntime *runtime = gWork;

    DisplayTransition_Finish(runtime->value_1c0, runtime->value_1c8);
    runtime->status_1c6 = 0;
}

void Event_WaitValue1c8Frames(void)
{
    WaitFrames(gWork->value_1c8);
}

void Event_SetPairWork1c0(u16 first, u16 second)
{
    gWork->value_170 = 999;
    gGameState.first = first;
    gGameState.second = second;
}

void Event_SetPair1c4AndResetValue170(u16 first, u16 second)
{
    struct State_08091e6c *state = gEventWork;
    state->value = 999;
    (*(struct Data_08091e6c *)&gGameState).first = first;
    (*(struct Data_08091e6c *)&gGameState).second = second;
}

void Event_SetValue170(u16 value)
{
    struct State_08091e6c *state = gEventWork;
    state->value = value;
}
