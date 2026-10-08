#include "SCENE.H"
#include "EVENT_RUNTIME.H"
#include "GAME_STATE.H"
#include "TYPES.H"
#include "DISPTRAN.H"

s32 WaitFrames(s32);

extern struct EventRuntime *gEventWork;

void Event_StartScreenTransition(void)
{
    struct EventRuntime *runtime = gEventWork;

    DisplayTransition_Start(runtime->value_1c0, runtime->value_1c8);
    runtime->status_1c6 = 1;
}

void Event_FinishScreenTransition(void)
{
    struct EventRuntime *runtime = gEventWork;

    DisplayTransition_Finish(runtime->value_1c0, runtime->value_1c8);
    runtime->status_1c6 = 0;
}

void Event_WaitTransitionFrames(void)
{
    WaitFrames(gEventWork->value_1c8);
}

void Event_SetDestination(u16 first, u16 second)
{
    gEventWork->value_170 = 999;
    gGameState.scene = first;
    gGameState.entrance = second;
}

void Event_SetSavedDestination(u16 first, u16 second)
{
    struct EventRuntime *state = gEventWork;
    state->value_170 = 999;
    gGameState.saved_scene = first;
    gGameState.saved_entrance = second;
}

void Event_SetExitRequest(u16 value)
{
    struct EventRuntime *state = gEventWork;
    state->value_170 = value;
}
