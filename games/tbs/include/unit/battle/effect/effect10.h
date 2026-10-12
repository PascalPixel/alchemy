#ifndef ALCHEMY_UNIT_BATTLE_EFFECT_EFFECT10_H
#define ALCHEMY_UNIT_BATTLE_EFFECT_EFFECT10_H

#include "types.h"
#include "battle_effect_work.h"
#include "battle_efx.h"
#include "unit/battle/effect/effect.h"
#include "unit/battle/effect/effect7.h"
#include "unit/battle/effect/effect8.h"
#include "unit/battle/effect/effect9.h"

struct BattleEffectArgument;

void BattleFx_DispatchByIdRange(s32 *arg0);
void BattlePres_RunBeamSequence(struct BattleEffectArgument *effect);
void BattlePres_RunRingAndSparkScene(struct BattleEffectArgument *effect);
void BattleFx_StepPaletteToResource(s32 resource_id);
void BattleFx_RunCastingImpact(struct BattleEffectArgument *command, s32 kind);

#endif
