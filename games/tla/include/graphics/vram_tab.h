#ifndef ALCHEMY_VRAM_TAB_H
#define ALCHEMY_VRAM_TAB_H

// The VRAM cache entry the windows keep for one use. The cache itself, its sizes, entries and
// functions, belongs to the system library (system.h).

#include "system.h"

// The window sprites' glyph tiles (WIN_STATE.glyphTiles), a cache entry kept for this one use.
#define VRAM_CACHE_ENTRY_GLYPH_TILES 95

#endif
