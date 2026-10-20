#ifndef ALCHEMY_SOL_SANCTUM_ENTRANCE_H
#define ALCHEMY_SOL_SANCTUM_ENTRANCE_H

#include "types.h"
#include "field_event.h"
#include "field_scene.h"
#include "flags.h"

/// In Sol Sanctum (scene 16), Kraden follows the party as actor 8.
enum { ACTOR_KRADEN = ACTOR_FIRST_PLACED };

/// At Sol Entrance (scene 19), actor 8 is a man who stands in the path once the
/// party has left Vale and warns everyone away while Mt. Aleph is active; read
/// with Mind Read, he is all alone in this dangerous place and asks how the
/// Great Healer can treat him like this.
enum { ACTOR_WARNING_MAN = ACTOR_FIRST_PLACED };

/// The statues that can be pushed, placed after Kraden: two of the eleven in
/// the room of entrances 11 to 13, and all six of the row in the room of
/// entrances 14 to 16. They share sprite 0xcb, which Kraden calls statues
/// ("More statues...").
enum { ACTOR_WEST_STATUE = ACTOR_FIRST_PLACED + 1, ACTOR_EAST_STATUE };
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
