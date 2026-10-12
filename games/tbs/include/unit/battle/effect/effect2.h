#ifndef ALCHEMY_UNIT_BATTLE_EFFECT_EFFECT2_H
#define ALCHEMY_UNIT_BATTLE_EFFECT_EFFECT2_H

#include "types.h"
#include "battle_effect_work.h"
#include "battle_efx.h"
#include "unit/battle/effect/effect.h"

struct BattleEffectArgument;

void BattleFx_RunIceFleckBurst(struct BattleEffectArgument *effect);
void BattleEffect_RunTileAndPaletteAnimation(struct BattleEffectArgument *effect);
void BattleFx_RunGatheringBurst(struct BattleEffectArgument *effect);
void BattlePresentation_DrawStreaks(void);
void BattlePresentation_PrepareScene(s32 kind);
void BattleFx_ScheduleCallbacksAndReleaseBlocks(void);

#endif
