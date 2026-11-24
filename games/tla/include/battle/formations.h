#ifndef ALCHEMY_FORMATIONS_H
#define ALCHEMY_FORMATIONS_H

/// @file
/// The enemy formations.

#include "types.h"

// Formation.layout and BattleState.layout: the order
// BattleFormation_BuildEnemyList draws a formation's enemies in, and how
// BattleUnit_Collect stands them in their line. The two random orders stand
// left to right as drawn; BattleUnit_Collect takes any other value as
// FORMATION_LAYOUT_CENTERED. The 24 formations of the bosses
// and the scripted battles have FORMATION_LAYOUT_CENTERED, every other
// FORMATION_LAYOUT_SHUFFLED.

/// Each kind's enemies together, the kinds in a random order. A link battle's
/// opponents stand this way too.
#define FORMATION_LAYOUT_SHUFFLED 0
/// Unused: each enemy drawn from a random kind with any left; no formation
/// has it.
#define FORMATION_LAYOUT_INTERLEAVED 1
/// The formation's own order, its first enemy in the centre and the rest
/// alternately either side: the bosses' and the scripted battles'.
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

// The formations the battle code itself tests for.
/// Karst and Agatio's battle, fought in three parts: the first two end after
/// two turns, and each part takes up the five units as the last left them
/// (ScriptedBattle_SaveUnits); the third is ENEMY_KARST_B and ENEMY_AGATIO_B.
///@{
#define FORMATION_KARST_AND_AGATIO_A 74
#define FORMATION_KARST_AND_AGATIO_B 75
#define FORMATION_KARST_AND_AGATIO_C 76
///@}
/// The Doom Dragon's three forms, each changing into the next
/// (Battle_PlayFormChange).
///@{
#define FORMATION_DOOM_DRAGON_A 78
#define FORMATION_DOOM_DRAGON_B 79
#define FORMATION_DOOM_DRAGON_C 80
///@}
/// Karst and Agatio against Isaac's party (ENEMY_KARST_VS_ISAAC), lost after
/// the first action.
#define FORMATION_KARST_AND_AGATIO_VS_ISAAC 638
/// The formations, by formation id (formations.c).
extern struct Formation gFormations[];

/// The 87 enemies whose defeated flags (0x600 + id) a monster battle clears
/// before it picks a formation (BattleFormation_SelectLevelMatchedCandidate).
extern u16 gBattleFormation_LevelMatchedEnemies[87];

#endif
