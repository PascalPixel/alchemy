/// @file
/// What the files of Jupiter Lighthouse's left-tower room share: its flag and
/// its actors.
#ifndef ALCHEMY_SCENE_JUPITER_LIGHTHOUSE_ROOM_8_H
#define ALCHEMY_SCENE_JUPITER_LIGHTHOUSE_ROOM_8_H

#include "scene_tables.h"

// Set once Move has dropped the pillar into the gap in the floor; cleared as
// the party comes back unless the party comes back from a battle
// (FLAG_KEEP_PARTY_POSITION).
#define FLAG_PILLAR_DROPPED 0xa18

/// The room's statue, pillar and logs, numbering the rows of gActorPlacements.
enum JupiterLighthouseLeftTowerActorId {
  /// ★ Named from what the player sees: a stone archer (spr_340.png)
  ACTOR_ARCHER_STATUE = ACTOR_FIRST_PLACED,
  /// ★ Named from what the player sees: a grey stone pillar (spr_312.png)
  ACTOR_PILLAR = ACTOR_FIRST_PLACED + 2,
  /// The first of five blue logs, two standing and three lying, that the
  /// leader pushes.
  /// ★ Named from what the player sees: blue logs (spr_331.png, spr_332.png)
  ACTOR_FIRST_LOG
};

#endif
