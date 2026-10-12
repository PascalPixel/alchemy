#ifndef ALCHEMY_UNIT_EFFECT_DJINNFX_H
#define ALCHEMY_UNIT_EFFECT_DJINNFX_H

#include "types.h"
#include "battle_effect_work.h"
#include "battle_efx.h"
#include "unit/effect/effect.h"
#include "unit/effect/fxdirect.h"
#include "unit/effect/twopic.h"
#include "unit/effect/strike.h"
#include "unit/effect/dispatch.h"
#include "unit/effect/scene.h"
#include "unit/effect/burst.h"
#include "unit/effect/step.h"

struct BattleEffectArgument;

void BattleFx_DispatchByIdRange(struct BattleEffectArgument *effect);
void BattlePres_RunBeamSequence(struct BattleEffectArgument *effect);
void BattlePres_RunRingAndSparkScene(struct BattleEffectArgument *effect);
void BattleFx_StepPaletteToResource(s32 resource_id);
void BattleFx_RunCastingImpact(struct BattleEffectArgument *command, s32 kind);

#endif
