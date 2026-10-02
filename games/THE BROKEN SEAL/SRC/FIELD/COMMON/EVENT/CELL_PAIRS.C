#include "TYPES.H"
#include "EVENT_RUNTIME.H"
#include "GAME_STATE.H"

extern struct EventRuntime *gEventWork;

void Event_SetPair1d4(u16 first, u16 second)
{
    gGameState.defeat_scene = first;
    gGameState.defeat_entrance = second;
}

void Event_SetPair1c0AndSetValue170(u16 first, u16 second)
{
    struct EventRuntime *work = gEventWork;
    gGameState.scene = first;
    gGameState.entrance = second;
    work->value_170 = 999;
}
