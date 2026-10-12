#include "types.h"
#include "irq.h"

void BattlePresentation_SetPaletteLevel(s32 a, s32 b);

void Func_0812756c(void)
{
    BattlePresentation_SetPaletteLevel(0, 0);
}

void Func_08127578(void)
{
    Runtime_SetIrqHandler(2, 0, 0);
}
