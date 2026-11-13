#ifndef ALCHEMY_VENUS_LIGHTHOUSE_AERIE_H
#define ALCHEMY_VENUS_LIGHTHOUSE_AERIE_H

/// @file
/// The Lighthouse Aerie's actors; its entrances and exits are in
/// scene_exits.h.

#include "scene_tables.h"
#include "types.h"

/// The aerie's actors, in the order of its actor placements;
/// Garet, Ivan, Mia and Sheba keep their character numbers.
enum {
  /// The four statues around the beacon (SPRITE_STATUE_WITH_WREATH), the east
  /// two drawn mirrored. When the beacon erupts and splits the aerie, the west
  /// two move west with their half and the east two east.
  ACTOR_BEACON_STATUE_SOUTHWEST = 8,
  ACTOR_BEACON_STATUE_NORTHWEST = 9,
  ACTOR_BEACON_STATUE_NORTHEAST = 10,
  ACTOR_BEACON_STATUE_SOUTHEAST = 11,
  /// Six round platforms (SPRITE_FLOATING_PLATFORM) the leader jumps across,
  /// each dipping under him as he lands, and the one that carries him up to
  /// the aerie.
  ACTOR_FIRST_PLATFORM = 12,
  ACTOR_LAST_PLATFORM = 17,
  ACTOR_RISING_PLATFORM = 18,
  ACTOR_MENARDI = 19,
  ACTOR_SATUROS = 20,
  ACTOR_FELIX = 21,
  /// A blank actor (SPRITE_BLANK) that runs across the floor under the
  /// Venus
  /// Star as Saturos throws it into the beacon, so that only its shadow shows,
  /// the star's shadow.
  ACTOR_VENUS_STAR_SHADOW = 22,
  /// The orb the rising particles circle as the party is lifted away:
  /// SPRITE_BLUE_ORB drawn in SPRITE_COLOR_GBR, which turns its blue to green,
  /// and later solid white (SPRITE_COLOR_WHITE).
  ACTOR_BEACON_ORB = 23,
  /// The beacon's beam, in five segments (SPRITE_PIPE_NORTH_SOUTH) from the
  /// bottom up; particles circle the lowest.
  ACTOR_BEAM_BOTTOM = 24,
  ACTOR_BEAM_LOWER = 25,
  ACTOR_BEAM_MIDDLE = 26,
  ACTOR_BEAM_UPPER = 27,
  ACTOR_BEAM_TOP = 28
};

#endif
