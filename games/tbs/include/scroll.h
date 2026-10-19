#ifndef ALCHEMY_SCROLL_H
#define ALCHEMY_SCROLL_H

/// @file
/// The display scroll work, the HBlank scroll work and its wave.

#include "map_scroll.h"
#include "static_assert.h"

struct DisplayScrollWork {
  struct BgScroll rows[2][160][3];
  u8 page;
  u8 mode;
  u16 frame;
  u8 unkf04[4];
  s32 freqX;
  s32 freqY;
  s32 stepX;
  s32 stepY;
  s32 ampX;
  s32 ampY;
};

STATIC_ASSERT(sizeof(struct DisplayScrollWork) == 0xf20);
STATIC_ASSERT(offsetof(struct DisplayScrollWork, page) == 0xf00);
STATIC_ASSERT(offsetof(struct DisplayScrollWork, freqX) == 0xf08);

extern struct DisplayScrollWork *gHBlankScrollWork;
extern const s16 gDisplayScroll_WaveSine[];

#endif
