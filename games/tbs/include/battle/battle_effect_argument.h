#ifndef ALCHEMY_BATTLE_EFFECT_ARGUMENT_H
#define ALCHEMY_BATTLE_EFFECT_ARGUMENT_H

/// @file
/// BattleEffectArgument, the command an effect's caller, dispatcher and
/// handlers share, and the macros that define effect kinds.

#include "types.h"
#include "static_assert.h"

/// The command shared by an effect's caller, dispatcher and handlers.
struct BattleEffectArgument {
  s32 kind;        ///< handler table index; 200 more for a critical hit
  s32 side;        ///< 1 when the targets are the party, 0 for enemies
  s32 actor;       ///< the user's unit id
  s32 firstTarget; ///< the action's first target, standing or not
  s32 range;       ///< forwarded action range, otherwise initialized to 1
  s32 count;       ///< targets in actors, at least 1
  s32 variant;     ///< the ability's size, from 0 (Quake, then Earthquake)
  s32 prelude;     ///< 1 plays a prelude: in some effects a Djinni appears
  s32 unk0020;     ///< not named by TBS code
  s16 actors[8];   ///< the targets' unit ids, count of them
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
