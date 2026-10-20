#ifndef ALCHEMY_SOL_SANCTUM_ENTRANCE_H
#define ALCHEMY_SOL_SANCTUM_ENTRANCE_H

#include "types.h"
#include "field_event.h"
#include "field_scene.h"
#include "flags.h"

/// Inside (scene 16), Kraden follows the party as actor 8.
enum { ACTOR_KRADEN = ACTOR_FIRST_PLACED };

/// On the approach (scene 19), actor 8 is the man left on watch, who warns the
/// party away while the volcano is active and, read with Mind Read, complains
/// that the Great Healer left him there alone.
enum { ACTOR_APPROACH_SENTRY = ACTOR_FIRST_PLACED };

/// The blocks Move pushes, placed after Kraden: two in the outer block room
/// (entrances 11 to 13) and six in the inner one (entrances 14 to 16).
enum {
  ACTOR_OUTER_LEFT_BLOCK = ACTOR_FIRST_PLACED + 1,
  ACTOR_OUTER_RIGHT_BLOCK
};
enum {
  ACTOR_INNER_BLOCK_1 = ACTOR_FIRST_PLACED + 1,
  ACTOR_INNER_BLOCK_2,
  ACTOR_INNER_BLOCK_3,
  ACTOR_INNER_BLOCK_4,
  ACTOR_INNER_BLOCK_5,
  ACTOR_INNER_BLOCK_6
};

extern s32 gSolSanctumEntrance_RumbleTimer;

#endif
