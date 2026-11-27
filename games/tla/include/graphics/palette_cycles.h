#ifndef ALCHEMY_PALETTE_CYCLES_H
#define ALCHEMY_PALETTE_CYCLES_H

/* @file */
/* Palette cycles: runs of background palette colours that rotate one place */
/* every few frames, up to four at once. */

#include "edition.h"
#include "types.h"
#include "static_assert.h"

/* A palette cycle: a run of up to 16 background palette colours, saved and */
/* rotated by one colour every delay + 1 frames. */
typedef struct PALETTE_CYCLE {
	u16 *src;	  /* the first colour of the run, in BG palette RAM */
	u16 pos;	  /* colours rotated so far, 0 to count - 1 */
	u16 tmr;	  /* frames until the next rotation */
	s16 delay;	  /* frames; the timer restarts from it */
	u16 cnt;	  /* colours in the run */
	u16 cols[16]; /* the run's colours as saved when the cycle was added */
} PALETTE_CYCLE;

/* The palette cycling queue (HEAP_SLOT_PALETTE_CYCLES): four cycles and how */
/* many are in use. */
typedef struct PALETTE_CYCLE_STATE {
	PALETTE_CYCLE entries[4];
	/* Cycles in use, 0 to 4; fdPaletteCycleStep runs none when all four are */
	/* taken (a BUG there). */
	u16 cnt;
} PALETTE_CYCLE_STATE;

STATIC_ASSERT(sizeof(PALETTE_CYCLE) == 44);
STATIC_ASSERT(offsetof(PALETTE_CYCLE, pos) == 4);
STATIC_ASSERT(offsetof(PALETTE_CYCLE, tmr) == 6);
STATIC_ASSERT(offsetof(PALETTE_CYCLE, delay) == 8);
STATIC_ASSERT(offsetof(PALETTE_CYCLE, cols) == 12);
STATIC_ASSERT(sizeof(PALETTE_CYCLE_STATE) == 180);
STATIC_ASSERT(offsetof(PALETTE_CYCLE_STATE, cnt) == 176);

/* The palette cycling queue, in its heap slot (HEAP_SLOT_PALETTE_CYCLES). */
#define PaletteState ((PALETTE_CYCLE_STATE *)Ram_HeapSlots->palCycles)

void fdPaletteCycleInit(void);
s32 fdPaletteCycleAdd(s16 paletteNumber, s16 firstColor, s16 delay,
					  s16 colorCount);
void fdPaletteQueueStop(void);
void fdPaletteCycleStep(void);

#endif
