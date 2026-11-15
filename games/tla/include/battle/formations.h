#ifndef ALCHEMY_FORMATIONS_H
#define ALCHEMY_FORMATIONS_H

/// @file
/// The enemy formations.

#include "types.h"

// Formation.layout and BattleWork.layout: the order
// BattleFormation_BuildEnemyList draws a formation's enemies in, and how
// BattleUnit_Collect stands them in their line. The two random orders stand
// left to right as drawn; BattleUnit_Collect takes any other value as
// FORMATION_LAYOUT_CENTERED. Every formation of this game has
// FORMATION_LAYOUT_SHUFFLED.

/// Each kind's enemies together, the kinds in a random order. A link battle's
/// opponents stand this way too.
#define FORMATION_LAYOUT_SHUFFLED 0
/// Unused: each enemy drawn from a random kind with any left; no formation
/// has it.
#define FORMATION_LAYOUT_INTERLEAVED 1
/// The formation's own order, its first enemy in the centre and the rest
/// alternately either side; no formation has it.
#define FORMATION_LAYOUT_CENTERED 2

/// The kinds of enemy a formation may hold.
#define FORMATION_MEMBERS 5

/// An enemy formation: up to five kinds of enemy, each with the fewest and
/// most of it that appear. A formation's number is a battle's, and a Djinni
/// battle's is DJINNI_BATTLE_EVENT_FIRST plus the Djinni's.
struct Formation {
  u16 memberIds[FORMATION_MEMBERS];    ///< enemy ids less 8
  u8 minimumCounts[FORMATION_MEMBERS]; ///< each always appears this often
  /// Each may appear up to this often while the six sprite slots hold them;
  /// 0 for a member that does not appear.
  u8 maximumCounts[FORMATION_MEMBERS];
  u8 layout;      ///< a FORMATION_LAYOUT_
  u8 reserved[3]; ///< 0 in every row
};

/// The rows of gFormations.
#define FORMATION_COUNT 660
/// The formations, by formation id (battle_tables.s).
extern struct Formation gFormations[];

#endif
