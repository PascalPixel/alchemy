#ifndef ALCHEMY_FIELD_BLEND_H
#define ALCHEMY_FIELD_BLEND_H

/* FIELD_BLEND_STATE, the field's colour blend state. */

#include "static_assert.h"
#include "types.h"

/* The storm's lightning state: the colours it flashes between and their
   difference, the countdown to the next flash and whether the thunder is
   loud. */
typedef struct FIELD_BLEND_STATE {
	/* The storm's dark colours (PALETTE_TINT_DIM), as the palette fade's */
	/* B, G and R channels: each flash fades back to them. */
	Sint16 from[1344];
	Sint16 to[1344];	/* the lit colours (PALETTE_TINT_PALE), as from */
	Sint16 delta[1344]; /* the 12-tick steps from to back to from */
	/* Ticks to the next flash, which comes in three, 5 ticks apart; below 0
	   stops the lightning. */
	Sint16 tmr;
	/* Nonzero arms the next flash a random 51 to 549 ticks after each, with a
	   thunderclap. */
	Sint16 enabled;
	Sint16 loud; /* nonzero plays the louder thunderclap, SE_THUNDER_LOUD for */
	/* SE_THUNDER */
	Uint8 padding[2]; /* up to the struct's word-aligned size */
} FIELD_BLEND_STATE;

STATIC_ASSERT(sizeof(FIELD_BLEND_STATE) == 8072);
STATIC_ASSERT(offsetof(FIELD_BLEND_STATE, to) == 2688);
STATIC_ASSERT(offsetof(FIELD_BLEND_STATE, delta) == 5376);
STATIC_ASSERT(offsetof(FIELD_BLEND_STATE, tmr) == 8064);
STATIC_ASSERT(offsetof(FIELD_BLEND_STATE, loud) == 8068);

#endif
