#ifndef ALCHEMY_UNIT_BATTLE_EFFECT_EFFECT9_H
#define ALCHEMY_UNIT_BATTLE_EFFECT_EFFECT9_H

#include "types.h"
#include "battle_effect_work.h"
#include "battle_efx.h"
#include "unit/battle/effect/effect.h"
#include "unit/battle/effect/effect2.h"
#include "unit/battle/effect/effect3.h"
#include "unit/battle/effect/effect4.h"
#include "unit/battle/effect/effect5.h"
#include "unit/battle/effect/effect6.h"
#include "unit/battle/effect/effect7.h"
#include "unit/battle/effect/effect8.h"

struct EffectPosition;
struct EffectStep;

void EffectStep_AdvanceWithGravity3D(struct EffectStep *step, s32 damping, s32 gravity);
void EffectStep_AdvanceWithGravity2D(struct EffectStep *step, s32 damping, s32 gravity);
s32 EffectPosition_ApplyBaseAndYOffset(s32 *point, struct EffectPosition *position);
void EffectPosition_ApplyAnimationAndYOffset(s32 id, struct EffectPosition *position);
void EffectPosition_ApplyStepAndYOffset(s32 id, struct EffectPosition *position);
void EffectPosition_ApplyAlternateStepAndYOffset(s32 id, struct EffectPosition *position);
s32 BattleUnit_ProjectToScreen(s32 unit, s32 *screen);
void BattleFx_ProbeKeysForExtendedActor(void);

#endif
