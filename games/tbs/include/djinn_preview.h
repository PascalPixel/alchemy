#ifndef ALCHEMY_DJINN_PREVIEW_H
#define ALCHEMY_DJINN_PREVIEW_H

/// @file
/// The Djinn preview's sprites and arrow tiles.

#include "output.h"
#include "resource.h"

/// The OAM entry of a render output, as the Djinn preview places its stat
/// arrows.
struct PreviewSprite {
  u32 unknown;
  union {
    u32 word;
    struct {
      unsigned y : 8; ///< screen pixels
      unsigned flags : 8;
      unsigned x : 9; ///< screen pixels
      unsigned high : 7;
    } bits;
  } attributes;
  union RenderTableValue tile;
};

extern u8 gDjinnMenu_ArrowUpTiles[];
extern u8 gDjinnMenu_ArrowDownTiles[];

#endif
