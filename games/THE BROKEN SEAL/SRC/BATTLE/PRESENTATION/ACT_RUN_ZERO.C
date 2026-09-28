#include "TYPES.H"
#include "SCENE.H"
volatile unsigned long long BattlePresentation_SetPaletteLevel(s32, s32);

void BattlePres_RunWithZeroArguments(void)
{
  BattlePresentation_SetPaletteLevel((unsigned long) 0, 0);
}
