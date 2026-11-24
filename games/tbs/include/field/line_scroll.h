#ifndef ALCHEMY_LINE_SCROLL_H
#define ALCHEMY_LINE_SCROLL_H

/// @file
/// The map scroll state and the line scroll's state and wave.

#include "fixed_math.h"
#include "static_assert.h"
#include "field/map_grid_scroll.h"
#include "system/heap.h"

/// The line scroll's state: two pages of BG1 to BG3 scroll offsets for each
/// of the 160 scanlines, and the sine wave that bends them on each axis.
struct LineScrollState {
  struct BgScroll rows[2][160][3]; ///< per page and scanline: BG3, BG2, BG1
  u8 page;      ///< the page the HBlank DMA shows; the other is built
  u8 mode;      ///< set at the start; not read by name
  u16 frame;    ///< frames built so far, added to BG3's y for the wave's phase
  u8 unused[4]; ///< zeroed with the block; nothing reads or writes it
  FIXED freqX;  ///< phase per frame and per BG3 row, 16.16 wave table entries
  FIXED freqY;  ///< phase per frame and per BG3 row, 16.16 wave table entries
  FIXED stepX;  ///< 16.16 wave table entries a scanline, 256 a cycle
  FIXED stepY;  ///< 16.16 wave table entries a scanline, 256 a cycle
  FIXED ampX;   ///< 16.16 scale on the sine entry, which is then divided by 256
  FIXED ampY;   ///< 16.16 scale on the sine entry, which is then divided by 256
};

STATIC_ASSERT(sizeof(struct LineScrollState) == 3872);
STATIC_ASSERT(offsetof(struct LineScrollState, page) == 3840);
STATIC_ASSERT(offsetof(struct LineScrollState, freqX) == 3848);

/// The line scroll's state, HEAP_SLOT_LINE_SCROLL under its own name,
/// which LineScroll_InitWave takes for a scene's wave.
#define gLineScroll                                                            \
  (*(struct LineScrollState **)&gHeapSlots[HEAP_SLOT_LINE_SCROLL])
/// One cycle of sine in 256 entries, 4.12 fixed point (4096 at the peak).
extern const s16 gLineScroll_WaveSine[];
/// The entries of a wave's sine, one cycle.
#define LINE_SCROLL_WAVE_ENTRIES 256
/// A wave's offset is its sine entry times its 16.16 amplitude, over this: an
/// amplitude of FIXED_ONE swings 16 pixels each way.
#define LINE_SCROLL_WAVE_SCALE 256

#endif
