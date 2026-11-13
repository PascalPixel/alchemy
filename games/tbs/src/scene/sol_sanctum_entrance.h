#ifndef ALCHEMY_SOL_SANCTUM_ENTRANCE_H
#define ALCHEMY_SOL_SANCTUM_ENTRANCE_H

#include "types.h"
#include "event.h"
#include "scene_tables.h"
#include "flags.h"

/// In Sol Sanctum (SceneId_SolSanctumReliefs), Kraden follows the party; Garet
/// and Jenna, placed before him, take their own party slots.
enum { ACTOR_KRADEN = ACTOR_FIRST_PLACED };

/// At Sol Entrance (SceneId_SolEntrance), the one placed actor is a man
/// with long brown hair and a beard in a long robe, who stands in the path once
/// the party has left Vale and warns everyone away while Mt. Aleph is active;
/// read with Mind Read, he is all alone in this dangerous place and asks how
/// the Great Healer can treat him like this.
enum { ACTOR_WARNING_MAN = ACTOR_FIRST_PLACED };

/// The statues, placed after Kraden. They share one sprite, which Kraden calls
/// statues ("More statues..."). Outside the two rooms below, Sol Sanctum places
/// one, in the statue nook off the first relief room
/// (SOL_SANCTUM_RELIEFS_STATUE_NOOK_FROM_FIRST_RELIEF_ROOM).
enum { ACTOR_STATUE = ACTOR_FIRST_PLACED + 1 };

/// The first relief room's eleven statues: the two that can be pushed,
/// then nine that cannot.
enum {
  ACTOR_WEST_STATUE = ACTOR_FIRST_PLACED + 1,
  ACTOR_EAST_STATUE,
  /// The first of the nine.
  ACTOR_FIRST_FIXED_STATUE
};

/// The second relief room's row of six, each of which can be pushed a cell
/// east or west; alike, they are numbered west to east, each starting 64 pixels
/// east of the one before (SolSanctumEntrance_RestoreInteriorState).
enum {
  ACTOR_ROW_STATUE_1 = ACTOR_FIRST_PLACED + 1,
  ACTOR_ROW_STATUE_2,
  ACTOR_ROW_STATUE_3,
  ACTOR_ROW_STATUE_4,
  ACTOR_ROW_STATUE_5,
  ACTOR_ROW_STATUE_6
};

extern s32 gSolSanctumEntrance_RumbleTimer;

#endif
