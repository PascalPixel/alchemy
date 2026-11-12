#ifndef ALCHEMY_FORMATIONS_H
#define ALCHEMY_FORMATIONS_H

/// @file
/// The enemy formations.

#include "types.h"

// Formation.layout and BattleWork.layout: the order
// BattleFormation_BuildEnemyList draws a formation's enemies in, and how
// BattleUnit_Collect stands them in their line. The two random orders stand
// left to right as drawn; BattleUnit_Collect takes any other value as
// FORMATION_LAYOUT_CENTERED.

/// Each kind's enemies together, the kinds in a random order. A link battle's
/// opponents stand this way too.
#define FORMATION_LAYOUT_SHUFFLED 0
/// Unused: each enemy drawn from a random kind with any left; no formation
/// has it.
#define FORMATION_LAYOUT_INTERLEAVED 1
/// The formation's own order, its first enemy in the centre and the rest
/// alternately either side: eleven formations, among them the Bandit with his
/// two Thieves and Menardi with Saturos.
#define FORMATION_LAYOUT_CENTERED 2

/// An enemy formation: its battle type and up to five kinds of enemy, each
/// with the fewest and most of it that appear. BattleFormation_BuildEnemyList
/// draws a battle's enemies from it.
struct Formation {
  u8 layout;           ///< a FORMATION_LAYOUT_
  u8 memberIds[5];     ///< rows of enemy_table.c (enemy id less ENEMY_ID_FIRST)
  u8 minimumCounts[5]; ///< each always appears this often
  /// Each may appear up to this often while the six sprite slots hold them;
  /// 0 for a member that does not appear.
  u8 maximumCounts[5];
};

/// The 380 formations, by formation id (battle/formations.c).
extern const struct Formation gFormations[];

#endif
