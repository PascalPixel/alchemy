#include "SCENE.H"
#include "EVENT_RUNTIME.H"
#include "GAME_STATE.H"
#include "TYPES.H"

void DisplayTransition_Start(s32 mode, s32 frames);

void DisplayTransition_Finish(s32 mode, s32 frames);

s32 WaitFrames(s32);

extern struct EventRuntime *gEventWork;

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
    gGameState.scene = first;
    gGameState.entrance = second;
}

void Event_SetPair1c4AndResetValue170(u16 first, u16 second)
{
    struct EventRuntime *state = gEventWork;
    state->value_170 = 999;
    gGameState.saved_scene = first;
    gGameState.saved_entrance = second;
}

void Event_SetValue170(u16 value)
{
    struct EventRuntime *state = gEventWork;
    state->value_170 = value;
}
