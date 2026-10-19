#ifndef ALCHEMY_BG_SCROLL_H
#define ALCHEMY_BG_SCROLL_H

#include "types.h"

/// A background's hardware scroll offsets.
struct BgScroll {
  u16 x, y; ///< pixels
};

extern struct BgScroll gBgScroll[];

#endif
