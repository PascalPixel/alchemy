#include "types.h"
#include "event_runtime.h"
#include "game_state.h"

extern struct EventRuntime *gEventWork;

void Event_SetDefeatDestination(u16 first, u16 second)
{
    gGameState.defeat_scene = first;
    gGameState.defeat_entrance = second;
}

void Event_RequestDestination(u16 first, u16 second)
{
    struct EventRuntime *work = gEventWork;
    gGameState.scene = first;
    gGameState.entrance = second;
    work->value_170 = 999;
}
