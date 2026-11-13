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

/// The kinds of enemy a formation may hold.
#define FORMATION_MEMBERS 5

/// An enemy formation: its battle type and up to five kinds of enemy, each
/// with the fewest and most of it that appear. BattleFormation_BuildEnemyList
/// draws a battle's enemies from it.
struct Formation {
  u8 layout;                           ///< a FORMATION_LAYOUT_
  u8 memberIds[FORMATION_MEMBERS];     ///< rows of enemy_table.c (enemy id less
                                       ///< ENEMY_ID_FIRST)
  u8 minimumCounts[FORMATION_MEMBERS]; ///< each always appears this often
  /// Each may appear up to this often while the six sprite slots hold them;
  /// 0 for a member that does not appear.
  u8 maximumCounts[FORMATION_MEMBERS];
};

/// The rows of gFormations.
#define FORMATION_COUNT 380
/// The formations, by formation id (battle/formations.c).
extern struct Formation gFormations[FORMATION_COUNT];

/// The 20 enemies whose defeated flags a monster battle clears before it picks
/// a formation (battle/formations.c).
extern u16 gBattleFormation_LevelMatchedEnemies[20];

#endif
