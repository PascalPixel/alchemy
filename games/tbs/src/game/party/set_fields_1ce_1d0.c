#include "types.h"
#include "scene.h"

#include "game_state.h"

void Party_SetFields1ceAnd1d0(u16 first, u16 second)
{
    gGameState.next_scene = first;
    gGameState.next_entrance = second;
}
