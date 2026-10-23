#ifndef ALCHEMY_VENUS_LIGHTHOUSE_AERIE_H
#define ALCHEMY_VENUS_LIGHTHOUSE_AERIE_H

/// @file
/// The Lighthouse Aerie's actors; its entrances and exits are in
/// scene_links.h.

#include "map_tables.h"
#include "types.h"

/// The aerie's actors, in the order of its placements; Garet, Ivan, Mia and
/// Sheba keep their character numbers.
enum {
  /// The four statues around the beacon (SPRITE_STATUE_WITH_WREATH), the east
  /// two drawn mirrored. When the beacon erupts and splits the aerie, the west
  /// two move west with their half and the east two east.
  /// ★ Named from what the player sees: four statues of a woman with a green
  /// wreath around the beacon, spr_258.png (its render)
  ACTOR_BEACON_STATUE_SOUTHWEST = 8,
  /// ★ Named from the game: "checked the stone statue..." (MsgCheckedGround)
  ACTOR_BEACON_STATUE_NORTHWEST = 9,
  /// ★ Named from the game: "checked the stone statue..." (MsgCheckedGround)
  ACTOR_BEACON_STATUE_NORTHEAST = 10,
  /// ★ Named from the game: "checked the stone statue..." (MsgCheckedGround)
  ACTOR_BEACON_STATUE_SOUTHEAST = 11,
  /// Six round platforms (SPRITE_FLOATING_PLATFORM) the leader jumps across,
  /// each dipping under him as he lands, and the one that carries him up to
  /// the aerie.
  /// ★ Named from what the player sees: round floating platforms
  /// in spr_242.png (its render)
  ACTOR_FIRST_PLATFORM = 12,
  /// ★ Named from what the player sees: the last of the round floating
  /// platforms (its render)
  ACTOR_LAST_PLATFORM = 17,
  /// ★ Named from what the player sees: SPRITE_FLOATING_PLATFORM, the round
  /// floating platform the six beside the aerie are drawn as (its render)
  ACTOR_RISING_PLATFORM = 18,
  /// ★ Named from the game: "Menardi!" (MsgSolSanctumKradenCouldThatBeThe)
  ACTOR_MENARDI = 19,
  /// ★ Named from the game: "Saturos and Menardi were their names"
  /// (MsgValeKradenOhIsaac)
  ACTOR_SATUROS = 20,
  /// ★ Named from the game: "Felix" (MsgCharacterName)
  ACTOR_FELIX = 21,
  /// A blank actor (SPRITE_BLANK) that runs across the floor under the Venus
  /// Star as Saturos throws it into the beacon, so that only its shadow shows,
  /// the star's shadow.
  /// ★ Named from what the player sees: a small shadow crossing the floor
  /// under the Venus Star as it flies over the beacon's well (its render)
  ACTOR_VENUS_STAR_SHADOW = 22,
  /// The orb the rising particles circle as the party is lifted away:
  /// SPRITE_BLUE_ORB drawn in SPRITE_COLOR_GBR, which turns its blue to green,
  /// and later solid white (SPRITE_COLOR_WHITE).
  /// ★ Named from what the player sees: a great orb of light over the split
  /// beacon, white and then green (its render)
  ACTOR_BEACON_ORB = 23,
  /// The beacon's beam, in five segments (SPRITE_PIPE_NORTH_SOUTH) from the
  /// bottom up; particles circle the lowest.
  /// ★ Named from the game: "two beams of light shot out from them"
  /// (MsgSolSanctumKradenYouFoundIt)
  ACTOR_BEAM_BOTTOM = 24,
  /// ★ Named from the game: "two beams of light shot out from them"
  /// (MsgSolSanctumKradenYouFoundIt)
  ACTOR_BEAM_LOWER = 25,
  /// ★ Named from the game: "two beams of light shot out from them"
  /// (MsgSolSanctumKradenYouFoundIt)
  ACTOR_BEAM_MIDDLE = 26,
  /// ★ Named from the game: "two beams of light shot out from them"
  /// (MsgSolSanctumKradenYouFoundIt)
  ACTOR_BEAM_UPPER = 27,
  /// ★ Named from the game: "two beams of light shot out from them"
  /// (MsgSolSanctumKradenYouFoundIt)
  ACTOR_BEAM_TOP = 28
};

#endif
