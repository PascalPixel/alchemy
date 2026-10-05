#include "BATTLE.H"
#include "DEBUG.H"
#include "FIELD.H"
#include "GAME.H"
#include "GRAPHICS.H"
#include "LIB.H"
#include "MENU.H"
#include "MINIGAME.H"
#include "SOUND.H"
#include "SYSTEM.H"
#include "TYPES.H"


void PsynergyMenu_CallIconRoutineWithValue(s32 arg0, s32 arg1)
{
    Ui_LoadCharacterEntryForSlotFar(0, arg1, 0);
}
