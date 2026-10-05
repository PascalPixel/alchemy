#ifndef ALCHEMY_BG_SCROLL_H
#define ALCHEMY_BG_SCROLL_H

#include "TYPES.H"

/* Each background's scroll offsets, as the display copies them to the
   BG offset registers. */
struct BgScroll { u16 x, y; };

extern struct BgScroll gBgScroll[];

#endif
