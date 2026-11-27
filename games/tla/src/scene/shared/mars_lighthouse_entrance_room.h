/* What the Mars Lighthouse rooms with flame throwers share: the flag bytes of
   the crack the leader falls through, and the functions they call. */
#ifndef ALCHEMY_MARS_LIGHTHOUSE_ENTRANCE_ROOM_H
#define ALCHEMY_MARS_LIGHTHOUSE_ENTRANCE_ROOM_H

#include "fixed_math.h"
#include "types.h"
#include "flags.h"
#include "system.h"
#include "game/main_routine_state.h"
#include "field/actor.h"
#include "field/script_inline.h"
#include "field/map_grid.h"

/* The metatile the leader fell through, its column and row, a byte of area
   flags each: scMarsLighthouseEntranceFallThroughCrack keeps them, and the star
   room below lands him under it. */
#define FLAG_BYTE_FALL_COLUMN FLAG_AREA_TEMP_40
#define FLAG_BYTE_FALL_ROW	  FLAG_AREA_TEMP_48

Sint32 fdMapGetTerrainHeight(Sint32 terrain, Sint32 x, Sint32 z);

Sint32 scFlameThrowerGetFlags(Sint32 idx);

#endif
