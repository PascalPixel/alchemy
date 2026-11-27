#ifndef ALCHEMY_PALETTE_CYCLES_H
#define ALCHEMY_PALETTE_CYCLES_H

/* Palette cycles: runs of background colours rotated one place every few
   ticks. Nothing in the game ever adds one. */

#include "edition.h"
#include "static_assert.h"
#include "types.h"

/* A palette cycle: a run of up to 16 background palette colours, saved and
   rotated by one colour every delay + 1 ticks. */
typedef struct PALETTE_CYCLE {
	Uint16 *src;	   /* the first colour of the run, in BG palette RAM */
	Uint16	pos;	   /* colours rotated so far, 0 to count - 1 */
	Uint16	timer;	   /* ticks until the next rotation */
	Sint16	delay;	   /* ticks; the timer restarts from it */
	Uint16	count;	   /* colours in the run */
	Uint16 colors[16]; /* the run's colours as saved when the cycle was added */
} PALETTE_CYCLE;

/* The palette cycling queue (HEAP_SLOT_PALETTE_CYCLES): four cycles and how
   many are in use. */
typedef struct PALETTE_CYCLE_STATE {
	PALETTE_CYCLE entries[4];
	/* Cycles in use, 0 to 4; fdPaletteCycleStep advances none of four (a
	   bug). */
	Uint16 count;
} PALETTE_CYCLE_STATE;

STATIC_ASSERT(sizeof(PALETTE_CYCLE) == 44);
STATIC_ASSERT(offsetof(PALETTE_CYCLE, pos) == 4);
STATIC_ASSERT(offsetof(PALETTE_CYCLE, timer) == 6);
STATIC_ASSERT(offsetof(PALETTE_CYCLE, delay) == 8);
STATIC_ASSERT(offsetof(PALETTE_CYCLE, colors) == 12);
STATIC_ASSERT(sizeof(PALETTE_CYCLE_STATE) == 180);
STATIC_ASSERT(offsetof(PALETTE_CYCLE_STATE, count) == 176);

#endif
