#ifndef ALCHEMY_UNIT_EFFECT_STEP_H
#define ALCHEMY_UNIT_EFFECT_STEP_H

#include "types.h"
#include "battle_effect_work.h"
#include "battle_efx.h"
#include "unit/effect/effect.h"
#include "unit/effect/burst.h"

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
