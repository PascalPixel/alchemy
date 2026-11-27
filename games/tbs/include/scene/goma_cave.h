#ifndef ALCHEMY_GOMA_CAVE_H
#define ALCHEMY_GOMA_CAVE_H

/* What the scenes of Goma Cave share: the stump that leaps out of the way at
   the Goma Cave Entrance (goma_cave_entrance.c) and in the cave's first room
   (goma_cave_stream.c), and the area flags of the room of two pillars and a
   stump (goma_cave_pillar.c) and the room below it (goma_cave_pond.c). */

#include "flags.h"

/* The ticks a leaping stump tips before it leaps. */
#define STUMP_TIP_TICKS		18
/* The puffs of dust that burst round a leaping stump as it lands. */
#define STUMP_LANDING_PUFFS 17

/* Goma Cave's area flags: gmArrive clears FLAG_AREA_TEMP_FIRST to
   FLAG_AREA_TEMP_LAST when the party enters another area, and other areas use
   the numbers for their own. */

/* The stump has been pushed over into the room below. */
#define FLAG_GOMA_CAVE_STUMP_FELL			FLAG_AREA_TEMP_00
/* The east stone pillar has sunk into its place. */
#define FLAG_GOMA_CAVE_EAST_PILLAR_IN_PLACE FLAG_AREA_TEMP_01
/* The west stone pillar stands one step east of its start. */
#define FLAG_GOMA_CAVE_WEST_PILLAR_MOVED	FLAG_AREA_TEMP_02

#endif
