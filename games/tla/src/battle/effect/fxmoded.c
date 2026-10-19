#include "types.h"

void Func_081548d0(void *object, s32 mode);

/// Abilities: Growth, Mad Growth, Wild Growth (kind 4).
void BattleFx_RunGrowth(void *object) { Func_081548d0(object, 0); }

/// Abilities: Punji, Punji Trap, Punji Strike (kind 56).
void BattleFx_RunPunji(void *object) { Func_081548d0(object, 1); }
