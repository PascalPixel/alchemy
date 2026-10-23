#ifndef ALCHEMY_SCROLL_H
#define ALCHEMY_SCROLL_H

/// @file
/// The map scroll work and the scanline effect's work and wave.

#include "map_grid_scroll.h"
#include "static_assert.h"

/// The scanline effect's work: two pages of BG1 to BG3 scroll offsets for each
/// of the 160 scanlines, and the sine wave that bends them on each axis.
struct ScanlineEffectWork {
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

STATIC_ASSERT(sizeof(struct ScanlineEffectWork) == 0xf20);
STATIC_ASSERT(offsetof(struct ScanlineEffectWork, page) == 0xf00);
STATIC_ASSERT(offsetof(struct ScanlineEffectWork, freqX) == 0xf08);

/// The scanline effect's work, HEAP_SLOT_SCANLINE_EFFECT under its own name,
/// which ScanlineEffect_InitWave takes for a map's wave.
extern struct ScanlineEffectWork *gScanlineEffect;
/// One cycle of sine in 256 entries, 4.12 fixed point (4096 at the peak).
extern const s16 gScanlineEffect_WaveSine[];

#endif
