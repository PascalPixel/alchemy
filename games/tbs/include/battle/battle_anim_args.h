#ifndef ALCHEMY_BATTLE_ANIM_ARGS_H
#define ALCHEMY_BATTLE_ANIM_ARGS_H

/// @file
/// BattleAnimArgs, the command an animation's caller, dispatcher and
/// handlers share, and the macros that define animation kinds.

#include "types.h"
#include "static_assert.h"

/// BattleAnimArgs.side: the side the targets are on.
enum BattleAnimSide { ANIM_SIDE_ENEMIES, ANIM_SIDE_PARTY };

/// What a critical hit adds to an attack's animation kind:
/// BattleStaging_RunAttack plays the critical version of the blow. Only
/// plain attacks score critical hits, and their kinds are 1 to 5, all
/// dispatched by id range, so it reaches kinds 201 to 205: the ring and spark
/// animation, the same for each.
#define ANIM_ID_CRITICAL_OFFSET 200

/// The command shared by an animation's caller, dispatcher and handlers.
struct BattleAnimArgs {
  s32 kind;        ///< handler table index; a critical hit adds
                   ///< ANIM_ID_CRITICAL_OFFSET
  s32 side;        ///< enum BattleAnimSide
  s32 user;        ///< the user's unit id
  s32 firstTarget; ///< the action's first target, standing or not
  s32 range;       ///< forwarded action range, otherwise initialized to 1
  s32 targetCount; ///< targets in actors, at least 1
  s32 variant;     ///< the ability's size, from 0 (Quake, then Earthquake)
  s32 prelude;     ///< 1 plays a prelude: in some animations a Djinni appears
  s32 unused;      ///< no code names it
  s16 targets[8];  ///< the targets' unit ids, count of them
};

STATIC_ASSERT(sizeof(struct BattleAnimArgs) == 0x34);
STATIC_ASSERT(offsetof(struct BattleAnimArgs, targetCount) == 0x14);
STATIC_ASSERT(offsetof(struct BattleAnimArgs, targets) == 0x24);

/// Defines the battle animation kind `name` as a call of a shared runner with
/// one fixed argument, its style or variant. The strike, volley, shower, mote
/// and stat-change families each have one runner and a kind like this for each
/// ability that uses it.
#define DEFINE_EFFECT_KIND(name, runner, arg)                                  \
  void name(struct BattleAnimArgs *effect) { runner(effect, arg); }

/// Loops `i` over the animation's targets, effect->actors[0] to
/// effect->actors[effect->count - 1]: the loops that flash, shake, strike or
/// aim at each target in turn.
#define ANIM_FOR_EACH_TARGET(i, effect)                                        \
  for ((i) = 0; (i) != (effect)->targetCount; (i)++)

#endif
