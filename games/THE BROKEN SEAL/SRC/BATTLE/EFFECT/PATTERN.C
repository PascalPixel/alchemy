#include "BATTLE.H"
#include "DEBUG.H"
#include "FIELD.H"
#include "GAME.H"
#include "GRAPHICS.H"
#include "MENU.H"
#include "MINIGAME.H"
#include "SOUND.H"
#include "SYSTEM.H"
#include "TYPES.H"

extern u32 gKeysHeld;

u16 BattleFx_GetCycledTableWord(void)
{
	return BattleFx_CyclePatternWords[(gKeysHeld >> 4) & 15];
}
