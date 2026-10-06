#include "BATTLE.H"
#include "DEBUG.H"
#include "FIELD.H"
#include "GAME.H"
#include "GRAPHICS.H"
#include "LIB.H"
#include "MENU.H"
#include "SOUND.H"
#include "SYSTEM.H"
#include "TYPES.H"


s32 GameFlag_SetBit(s32);

void Djinn_SetFoundFlagAndAdd(s32 arg0, s32 arg1, s32 arg2)
{
    GameFlag_SetBit((arg1 * 0x14) + arg2 + 0x30);
    Djinn_AddToOwnerFar(arg0, arg1, arg2);
}
