#include "BATTLE.H"
#include "FIELD.H"
#include "GAME.H"
#include "GRAPHICS.H"
#include "LIB.H"
#include "MENU.H"
#include "SOUND.H"
#include "SYSTEM.H"
#include "TYPES.H"


u8 GameFlag_GetByte(s32 flag)
{
    flag = ((u32)flag << 20) >> 23;
    return GameFlagBytes[flag];
}
