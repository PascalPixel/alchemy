//--------------------------------------------------------------------------
// wait_then_set_field.c
// Battle effects: sparks after approaching the target (type 4).
//--------------------------------------------------------------------------

#include "types.h"
#include "scene.h"
#include "system.h"
#include "battle_effect_work.h"

void BattleMotion_ApproachTargetFar(s32, s16, s32, s32);
s32 BattleFx_RunSparkGroups(void *, s32);

//--------------------------------------------------------------------------
// Jump the user toward the first target and emit type 4 sparks after 29 frames.
// effect: effect arguments.
//--------------------------------------------------------------------------
void BattleFx_RunThrash(struct BattleEffectArgument *effect) {
  // Jump 3/4 of the distance at displacement / 24, with initial vertical
  // speed 12.2.
  BattleMotion_ApproachTargetFar(effect->actor, effect->actors[0], 24, 0xc3333);
  WaitFrames(29);
  effect->variant = 4;
  BattleFx_RunSparkGroups(effect, 2); // 2: from the user's position.
}
