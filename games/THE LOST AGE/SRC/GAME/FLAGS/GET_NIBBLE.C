#include "BATTLE.H"
#include "FIELD.H"
#include "GAME.H"
#include "GRAPHICS.H"
#include "LIB.H"
#include "MENU.H"
#include "SOUND.H"
#include "SYSTEM.H"
#include "TYPES.H"


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
