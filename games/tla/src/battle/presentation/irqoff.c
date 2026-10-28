#include "types.h"
#include "irq.h"

void BattleBackground_SetPaletteLevel(s32 a, s32 b);

void Func_0812756c(void) { BattleBackground_SetPaletteLevel(0, 0); }

void Func_08127578(void) { Irq_SetHandler(2, 0, 0); }
