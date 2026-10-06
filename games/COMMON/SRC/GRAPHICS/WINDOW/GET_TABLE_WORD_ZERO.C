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


s32 Ui_GetTableWordZero(s32 index)
{
    if (index != 0)
        index = 0;
    return Ui_FixedTileBlocks[index];
}
