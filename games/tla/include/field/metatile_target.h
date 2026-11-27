#ifndef ALCHEMY_METATILE_TARGET_H
#define ALCHEMY_METATILE_TARGET_H

/* The cracked floor metatiles that Jupiter Lighthouse, Mars Lighthouse and
   Anemos Inner Sanctum watch (scene/shared/cracked_tile_*.h), and the sprite
   field/cracked_tile_mask.c draws over the one the leader stands on. */

#include "fixed_math.h"
#include "types.h"
#include "field/field_coordinate.h"

/* One watched metatile, in map metatiles. */
typedef struct METATILE_TARGET_ENTRY {
	Sint16 x;
	Sint16 z;
	Sint16 flg;	   /* set the first time the leader steps on the metatile */
	Sint16 unused; /* no code names it */
} METATILE_TARGET_ENTRY;

/* A map's watched metatiles and the sprite drawn over the current one. */
typedef struct METATILE_TARGET {
	/* the first entry's flag; each later entry takes the next */
	Sint16 firstFlg;
	/* The first of two flag bytes that keep the leader's metatile, x then z (at
	   flagByte + 8). */
	Sint16 flgByte;
	/* the metatile script marking a watched metatile, kind or kind + 1 */
	Sint16 kind;
	Sint16 x; /* column of the copied metatile, whose row is z + 1 */
	Sint16 z;
	Sint16 cnt;				/* entries in use */
	Sint16 cur;				/* the entry the leader stands on, or -1 */
	Sint16 shown;			/* nonzero draws the sprite */
	Sint16 tile;			/* resource entry of the sprite's tiles */
	Uint16 unusedAfterTile; /* no code names it */
	/* The sprite as syOAMAddSprAtDepth takes it: the queue's link, then OAM
	   attributes 0 and 1 in one word and attribute 2 in the next. */
	unsigned long oam[3];
	/* Room for 128, as the ROM spaces Mars Lighthouse room F's table. */
	METATILE_TARGET_ENTRY entries[128];
} METATILE_TARGET;

/* The bytes of the hole mask's picture (a METATILE_TARGET's tile), the sprite
   drawn in the OBJ window over the leader's metatile as he sinks through the
   floor: 16 by 32 pixels, eight tiles of colour 1. TILE_SIZE_4BPP is
   system/definitions.h's. */
#define HOLE_MASK_BYTES (8 * TILE_SIZE_4BPP)

/* The map metatile of a 16.16 coordinate with a watched metatile's 64 bit ORed
   in, as each scene's copy of the cracked tile handler builds its row. */
static inline Sint32 syMetatileTargetMetatileOf(FIXED position, Sint16 bit)
{
	return FIELD_METATILE(position) | bit;
}

#endif
