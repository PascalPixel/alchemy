/// @file
/// Battle effects: sparks after approaching the target (type 3).

#include "types.h"
#include "system.h"
#include "battle_effect_work.h"

void BattleMotion_ApproachTargetFar(s32, s16, s32, s32);
s32 BattleEffect_RunSparkGroups(void *, s32);

/// Jumps the user toward the first target and emits type 3 sparks after 12
/// frames.
/// effect: effect arguments.
void BattleEffect_RunSparkApproachVariant3(struct BattleEffectArgument *effect) {
  // Jump 3/4 of the distance at displacement / 24, with initial vertical
  // speed 7.2.
  BattleMotion_ApproachTargetFar(effect->actor, effect->actors[0], 24, 0x73333);
  wait(12);
  effect->variant = 3;
  BattleEffect_RunSparkGroups(effect, 2); // 2: from the user's position.
}
