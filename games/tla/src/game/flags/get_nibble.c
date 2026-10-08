#include "battle.h"
#include "field.h"
#include "game.h"
#include "graphics.h"
#include "lib.h"
#include "menu.h"
#include "sound.h"
#include "system.h"
#include "types.h"


s32 GameFlag_GetNibble(u32 flag)
{
    u32 shifted = flag << 20;
    u32 shift;
    u32 mask;
    s32 result;

    shift = flag & 4;
    mask = 15;
    flag = shifted >> 23;
    result = GameFlagBytes[flag];
    result &= mask << shift;
    return result >> shift;
}
