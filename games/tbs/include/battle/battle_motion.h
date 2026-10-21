#ifndef ALCHEMY_BATTLE_MOTION_H
#define ALCHEMY_BATTLE_MOTION_H

/// @file
/// The knockbacks a struck unit takes.

#include "edition.h"
#include "types.h"

/// BattleMotion_ApplyVariantMotion's knockbacks, the columns of the
/// gBattleMotion_Variant* tables in battle_vars.s. Each is named by how far
/// and how high it sends the unit, as percent of its spot's distance from the
/// centre and its hop in pixels a frame; all but KNOCKBACK_HURL accelerate by
/// 1 pixel a frame to at most 4.
enum BattleKnockback {
  KNOCKBACK_NUDGE,  ///< 140 percent, a hop of 3
  KNOCKBACK_HOP,    ///< 140 percent, a hop of 6
  KNOCKBACK_LEAP,   ///< 150 percent, a hop of 7
  KNOCKBACK_LAUNCH, ///< 200 percent, a hop of 10
  /// The hardest: 200 percent at 8 pixels a frame more each frame, to at
  /// most 20, with a hop of 5.
  KNOCKBACK_HURL,
  /// 150 percent, a hop of 2. From here on a unit already in the air takes
  /// the new hop too.
  KNOCKBACK_SKID,
  KNOCKBACK_BOUNCE, ///< the spot itself, a hop of 3
  KNOCKBACK_JOLT    ///< the spot itself, a hop of 5.5
};

#endif
