#include "BATTLE.H"
#include "FIELD.H"
#include "GAME.H"
#include "GRAPHICS.H"
#include "LIB.H"
#include "MENU.H"
#include "SOUND.H"
#include "SYSTEM.H"
#include "TYPES.H"


void BattlePlacement_GetStepPair(s32 index, s32 *x, s32 *y)
{
    index *= 2;
    *x = BattlePlacement_StepPairs[index];
    *y = BattlePlacement_StepPairs[index + 1];
}
