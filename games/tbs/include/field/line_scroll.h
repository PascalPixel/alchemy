#ifndef ALCHEMY_LINE_SCROLL_H
#define ALCHEMY_LINE_SCROLL_H

/// @file
/// The map scroll work and the line scroll's work and wave.

#include "map_grid_scroll.h"
#include "static_assert.h"

/// The line scroll's work: two pages of BG1 to BG3 scroll offsets for each
/// of the 160 scanlines, and the sine wave that bends them on each axis.
struct LineScrollWork {
  struct BgScroll rows[2][160][3]; ///< per page and scanline: BG3, BG2, BG1
  u8 page;      ///< the page the HBlank DMA shows; the other is built
  u8 mode;      ///< set at the start; not read by name
  u16 frame;    ///< frames built so far, added to BG3's y for the wave's phase
  u8 unused[4]; ///< zeroed with the block; nothing reads or writes it
  s32 freqX;    ///< phase per frame and per BG3 row, 16.16 wave table entries
  s32 freqY;    ///< phase per frame and per BG3 row, 16.16 wave table entries
  s32 stepX;    ///< 16.16 wave table entries a scanline, 256 a cycle
  s32 stepY;    ///< 16.16 wave table entries a scanline, 256 a cycle
  s32 ampX;     ///< 16.16 scale on the sine entry, which is then divided by 256
  s32 ampY;     ///< 16.16 scale on the sine entry, which is then divided by 256
};

STATIC_ASSERT(sizeof(struct LineScrollWork) == 0xf20);
STATIC_ASSERT(offsetof(struct LineScrollWork, page) == 0xf00);
STATIC_ASSERT(offsetof(struct LineScrollWork, freqX) == 0xf08);

/// The line scroll's work, HEAP_SLOT_LINE_SCROLL under its own name,
/// which LineScroll_InitWave takes for a scene's wave.
extern struct LineScrollWork *gLineScroll;
/// One cycle of sine in 256 entries, 4.12 fixed point (4096 at the peak).
extern const s16 gLineScroll_WaveSine[];

#endif
