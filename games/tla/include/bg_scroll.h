#ifndef ALCHEMY_BG_SCROLL_H
#define ALCHEMY_BG_SCROLL_H

#include "types.h"

/// A background's hardware scroll offsets.
struct BgScroll {
  u16 x, y; ///< pixels
};

/// BG0 to BG3's scroll offsets, which the VBlank handler copies to
/// REG_BG0HOFS to REG_BG3VOFS in the VBlank after wait() sets
/// gFrameRenderPending.
extern struct BgScroll gBgScroll[];

#endif
