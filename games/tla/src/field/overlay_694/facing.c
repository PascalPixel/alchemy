#include "types.h"
#include "object_runtime.h"
#include "party_state.h"

struct ObjectRuntime *Object_GetById();

s32 Func_02000cc4(void)
{
    u16 turn;
    s32 facing;

    turn = Object_GetById(gPartyState.current_owner)->angle + 0x5fff;
    facing = 1;
    if (turn >= 0x3fff)
        facing = 0;
    return facing;
}
