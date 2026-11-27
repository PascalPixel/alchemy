#ifndef ALCHEMY_SPARKLE_WARP_H
#define ALCHEMY_SPARKLE_WARP_H

/* The warp in a column of sparkles by which Saturos and Alex leave and arrive
   on Mercury Lighthouse's aerie (mercury_lighthouse_aerie.c), and by which Alex
   arrives in Sol Sanctum's Elemental Star room
   (sol_sanctum_elemental_star_room.c). */

/* The ticks a warp lasts, through which sparkles rise about those warping out
   or fall about those warping in: on the aerie one a tick, about Saturos and
   Alex by turns, and in Sol Sanctum one every other tick, about Alex. */
#define WARP_TICKS 32

/* The tick of a warp in from which those arriving blink into sight. */
#define WARP_IN_BLINK_TICK 20

#endif
