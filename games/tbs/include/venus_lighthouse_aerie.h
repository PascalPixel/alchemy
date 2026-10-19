#ifndef ALCHEMY_VENUS_LIGHTHOUSE_AERIE_H
#define ALCHEMY_VENUS_LIGHTHOUSE_AERIE_H

#include "field_scene.h"
#include "types.h"

// The aerie's actors, in the order of its placements; Garet, Ivan, Mia and
// Sheba keep their character numbers.
enum {
  // The aerie floor around the beacon, in four sections: the first two move
  // west and the last two (drawn mirrored) east when the aerie breaks.
  ACTOR_AERIE_WEST_SECTION_A = 8,
  ACTOR_AERIE_WEST_SECTION_B = 9,
  ACTOR_AERIE_EAST_SECTION_A = 10,
  ACTOR_AERIE_EAST_SECTION_B = 11,
  // Six pillars the leader jumps across, and the one that carries him up to
  // the aerie.
  ACTOR_FIRST_PILLAR = 12,
  ACTOR_LAST_PILLAR = 17,
  ACTOR_MOVING_PILLAR = 18,
  ACTOR_MENARDI = 19,
  ACTOR_SATUROS = 20,
  ACTOR_FELIX = 21,
  // What carries the Venus Star to the beacon.
  ACTOR_STAR_CARRIER = 22,
  // The light the rising particles circle as the party is lifted away.
  ACTOR_PARTICLE_SOURCE = 23,
  // The beacon's beam, in five segments from the bottom up; particles circle
  // the first.
  ACTOR_BEAM_1 = 24,
  ACTOR_BEAM_2 = 25,
  ACTOR_BEAM_3 = 26,
  ACTOR_BEAM_4 = 27,
  ACTOR_BEAM_5 = 28
};

enum AerieEntrance {
  // Up from Venus Lighthouse: the party rides a pillar up to the aerie.
  AERIE_ENTRANCE_ARRIVAL = 1,
  // Back from the battle with Saturos and Menardi, and from the one with
  // their fused form.
  AERIE_ENTRANCE_AFTER_FIRST_BATTLE = 2,
  AERIE_ENTRANCE_AFTER_SECOND_BATTLE = 3,
  // From the world map, onto the broken aerie: Sheba falls and Felix leaps
  // after her.
  AERIE_ENTRANCE_SHEBAS_FALL = 4,
  // Back from a lost battle.
  AERIE_ENTRANCE_DEFEAT = 9,
  // From the world map, after the beacon is lit: a look back at Felix and
  // Sheba on the broken aerie.
  AERIE_ENTRANCE_SHEBA_IN_DANGER = 93
};

enum AerieExit {
  AERIE_EXIT_TO_WORLD_MAP = 1,
  AERIE_EXIT_AFTER_SHEBAS_FALL = 2,
  AERIE_EXIT_AFTER_DEFEAT = 9
};

enum { WORLD_MAP_ENTRANCE_FROM_AERIE = 73 };

#endif
