#ifndef ALCHEMY_BATTLE_EFFECT_PARAMS_H
#define ALCHEMY_BATTLE_EFFECT_PARAMS_H

/// @file
/// The command a battle effect is run with: its kind, its user and its targets.

#include "types.h"
#include "static_assert.h"

/// BattleEffectParams.side: the side the targets are on.
enum BattleEffectSide { EFFECT_SIDE_ENEMIES, EFFECT_SIDE_PARTY };

/// What a critical hit adds to an attack's effect kind: BattleStaging_RunAttack
/// plays the critical version of the blow, as The Broken Seal's does.
#define EFFECT_ID_CRITICAL_OFFSET 200

/// The command shared by an effect's caller, dispatcher and handlers.
struct BattleEffectParams {
  s32 kind;        ///< the effect's index in the handler table
  s32 side;        ///< 1 when the targets are the party, 0 for enemies
  s32 user;        ///< the user's unit id
  s32 firstTarget; ///< the action's first target
  s32 range;       ///< forwarded action range, otherwise initialized to 1
  s32 targetCount; ///< how many of targets are in use
  s32 variant;     ///< the ability's size, from 0
  s32 prelude;     ///< 1 plays a prelude: in some effects a Djinni appears
  s32 unused;      ///< named by no code in either game
  s16 targets[8];  ///< the targets' unit ids, targetCount of them
};

STATIC_ASSERT(sizeof(struct BattleEffectParams) == 52);
STATIC_ASSERT(offsetof(struct BattleEffectParams, targetCount) == 20);
STATIC_ASSERT(offsetof(struct BattleEffectParams, targets) == 36);

/// Loops `i` over the effect's targets, effect->targets[0] to
/// effect->targets[effect->targetCount - 1]: the loops that flash, shake,
/// strike or aim at each target in turn.
#define EFFECT_FOR_EACH_TARGET(i, effect)                                      \
  for ((i) = 0; (i) != (effect)->targetCount; (i)++)

#endif
