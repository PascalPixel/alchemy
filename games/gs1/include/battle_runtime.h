#ifndef ALCHEMY_BATTLE_RUNTIME_H
#define ALCHEMY_BATTLE_RUNTIME_H

#include "battle_event.h"
#include "battle_types.h"

/* Battle-facing views of the polymorphic runtime lookup functions. */

void BattleUnit_Recalculate(s32 unit_id);
s32 BattleFlag_Test(s32 flag_id);
void BattleUnit_UpdateRatios(s32 unit_id);
s32 BattleRandom_Next(void);
s32 BattleEffect_Classify(s32 effect_id);

#endif
