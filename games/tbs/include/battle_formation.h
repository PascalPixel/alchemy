#ifndef ALCHEMY_BATTLE_FORMATION_H
#define ALCHEMY_BATTLE_FORMATION_H

/// @file
/// The enemy formations.

#include "types.h"

/// Formation.layout and BattleWork.layout (BattleFormation_BuildEnemyList):
/// the random formations stand left to right in the order they were drawn,
/// any other is laid out from the centre.
#define FORMATION_LAYOUT_SHUFFLED 0
#define FORMATION_LAYOUT_INTERLEAVED 1

/// An enemy formation: its battle type and up to five kinds of enemy, each
/// with the fewest and most of it that appear. BattleFormation_BuildEnemyList
/// draws a battle's enemies from it.
struct Formation {
  /// FORMATION_LAYOUT_SHUFFLED, FORMATION_LAYOUT_INTERLEAVED, or any other
  /// value for the formation's own order.
  u8 layout;
  u8 memberIds[5];     ///< rows of enemy_table.c (enemy id less ENEMY_ID_FIRST)
  u8 minimumCounts[5]; ///< each always appears this often
  /// Each may appear up to this often while the six sprite slots hold them;
  /// 0 for a member that does not appear.
  u8 maximumCounts[5];
};

/// The 380 formations, by formation id (battle/formations.c).
extern const struct Formation gFormations[];

#endif
