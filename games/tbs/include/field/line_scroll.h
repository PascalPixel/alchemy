#ifndef ALCHEMY_LINE_SCROLL_H
#define ALCHEMY_LINE_SCROLL_H

// The map scroll state and the line scroll's state and wave.

#include "fixed_math.h"
#include "static_assert.h"
#include "field/map_grid_scroll.h"
#include "system.h"

// The line scroll's state: two pages of BG1 to BG3 scroll offsets for each of the 160 scanlines,
// and the sine wave that bends them on each axis.
typedef struct LINE_SCROLL_STATE
{
	BG_SCROLL rows[2][160][3]; // per page and scanline: BG3, BG2, BG1
	Uint8     page;            // the page the HBlank DMA shows; the other is built
	Uint8     mode;            // set at the start; not read by name
	Uint16    frame;           // ticks built so far, added to BG3's y for the wave's phase
	Uint8     unused[4];       // zeroed with the block; nothing reads or writes it
	FIXED     frequencyX;      // phase per tick and per BG3 row, 16.16 wave table
	                           // entries
	FIXED     frequencyY;      // phase per tick and per BG3 row, 16.16 wave table
	                           // entries
	FIXED     stepX;           // 16.16 wave table entries a scanline, 256 a cycle
	FIXED     stepY;           // 16.16 wave table entries a scanline, 256 a cycle
	FIXED
	ampX;                      // 16.16 scale on the sine entry, which is then divided by 256
	FIXED
	ampY;                      // 16.16 scale on the sine entry, which is then divided by 256
} LINE_SCROLL_STATE;

STATIC_ASSERT(sizeof(LINE_SCROLL_STATE) == 3872);
STATIC_ASSERT(offsetof(LINE_SCROLL_STATE, page) == 3840);
STATIC_ASSERT(offsetof(LINE_SCROLL_STATE, frequencyX) == 3848);

// The line scroll's state, HEAP_SLOT_LINE_SCROLL under its own name, which scLineScrollInitWave
// takes for a scene's wave.
#define LineScroll (*(LINE_SCROLL_STATE **)&HeapSlots[HEAP_SLOT_LINE_SCROLL])
// One cycle of sine in 256 entries, 4.12 fixed point (4096 at the peak).
extern const Sint16 LineScrollWaveSine[];
// The entries of a wave's sine, one cycle.
#define LINE_SCROLL_WAVE_ENTRIES 256
// A wave's offset is its sine entry times its 16.16 amplitude, over this: an amplitude of
// toFIXED(1) swings 16 pixels each way.
#define LINE_SCROLL_WAVE_SCALE   256

#endif
