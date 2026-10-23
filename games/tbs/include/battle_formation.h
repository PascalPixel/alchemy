#ifndef ALCHEMY_BATTLE_FORMATION_H
#define ALCHEMY_BATTLE_FORMATION_H

/// @file
/// The enemy formation records.

#include "types.h"

/// An enemy formation: its battle type and up to five kinds of enemy, each
/// with the fewest and most of it that appear. BattleFormation_BuildEnemyList
/// draws a battle's enemies from it.
struct Formation {
  /// FORMATION_LAYOUT_SHUFFLED, FORMATION_LAYOUT_INTERLEAVED, or any other
  /// value for the record's own order.
  u8 layout;
  u8 memberIds[5];     ///< rows of enemies.tsv (class id less ENEMY_ID_FIRST)
  u8 minimumCounts[5]; ///< each always appears this often
  /// Each may appear up to this often while the six sprite slots hold them;
  /// 0 for a member that does not appear.
  u8 maximumCounts[5];
};

/// The 380 formations, by enemy group (battle/formatn.tsv).
extern struct Formation gFormations[];

#endif
