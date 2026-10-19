#ifndef ALCHEMY_BATTLE_EFFECT_ARGUMENT_H
#define ALCHEMY_BATTLE_EFFECT_ARGUMENT_H

#include "types.h"
#include "static_assert.h"

/// The command shared by an effect's caller, dispatcher and handlers.
struct BattleEffectArgument {
  s32 kind;
  s32 side;
  s32 actor;
  s32 firstTarget;
  s32 range; ///< forwarded action range, otherwise initialized to 1
  s32 count;
  s32 variant;
  s32 prelude;
  s32 unk0020;
  s16 actors[8];
};

STATIC_ASSERT(sizeof(struct BattleEffectArgument) == 0x34);
STATIC_ASSERT(offsetof(struct BattleEffectArgument, count) == 0x14);
STATIC_ASSERT(offsetof(struct BattleEffectArgument, actors) == 0x24);

/// Defines the battle effect kind `name` as a call of a shared runner with one
/// fixed argument, its style or variant. The strike, volley, shower, mote and
/// stat-change families each have one runner and a kind like this for each
/// ability that uses it.
#define DEFINE_EFFECT_KIND(name, runner, arg)                                  \
  void name(struct BattleEffectArgument *effect) { runner(effect, arg); }

/// Loops `i` over the effect's targets, effect->actors[0] to
/// effect->actors[effect->count - 1]: the loops that flash, shake, strike or
/// aim at each target in turn.
#define EFFECT_FOR_EACH_TARGET(i, effect)                                      \
  for ((i) = 0; (i) != (effect)->count; (i)++)

#endif
