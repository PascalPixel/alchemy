#include "TYPES.H"
#include "SCENE.H"
volatile unsigned long long BattlePresentation_SetPaletteLevel(s32, s32);

void BattlePres_RunWithZeroArguments(void)
{
  BattlePresentation_SetPaletteLevel((unsigned long) 0, 0);
}

typedef void (*InterruptHandler)(void);
void Runtime_SetIrqHandler(u32, s32, InterruptHandler);

void Runtime_RemoveIrqHandlerSlot2(void)
{
    Runtime_SetIrqHandler(2, 0, 0);
}
