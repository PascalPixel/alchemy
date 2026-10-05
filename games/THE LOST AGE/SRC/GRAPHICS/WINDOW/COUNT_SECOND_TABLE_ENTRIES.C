#include "BATTLE.H"
#include "FIELD.H"
#include "GAME.H"
#include "GRAPHICS.H"
#include "LIB.H"
#include "MENU.H"
#include "SOUND.H"
#include "SYSTEM.H"
#include "TYPES.H"


s32 Ui_CountSecondTableEntries(void)
{
    return (UiIcon_PsynergyIconPointersEnd - UiIcon_PsynergyIconPointers) >> 2;
}
