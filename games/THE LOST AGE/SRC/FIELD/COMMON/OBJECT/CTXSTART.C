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

void GameFlag_SetBit(s32 flag);

void ObjectEffect_BeginContextEffect26(void)
{
    ObjectEffect_PrepareContextEffect(26);
    GameFlag_SetBit(0x120);
}

void ObjectEffect_BeginContextEffect25(void)
{
    ObjectEffect_PrepareContextEffect(25);
    GameFlag_SetBit(0x121);
}
