#ifndef ALCHEMY_GRAPHICS_H
#define ALCHEMY_GRAPHICS_H

// Other parts' code run from RAM, and the sprite colour remap. The screen fade's state and the
// decoders belong to the system library (system.h).
//
// Several functions declared here are handwritten ARM code that the game copies to RAM before it
// runs it (RAM_CODE_LOAD, system.h). Such a function is declared as an array of bytes, and a symbol
// ending in CodeSize is the length of the copy: no object, but an address the linker sets equal to
// the length, which the loader reads by casting it to a number.

#include "fixed_math.h"
#include "types.h"
#include "resource_ids.h"
#include "edition.h"
#include "system.h"

// The text decoder's symbol lookup, which mnMessageFind copies to the heap to run
// (window/lookup_symbol.s).
extern const Uint8 TextLookupSymbolCode[];
// The size of TextLookupSymbolCode.
extern Uint8       TextLookupSymbolCodeSize[];
// The size of mnScanlineWindowBuildSpansCode, which mnScanlineWindowFillSpans copies to the heap to
// run.
extern Uint8       mnScanlineWindowBuildSpansCodeSize[];

// Remaps size bytes of pixels in place through SpriteColorGreyTable.
void               fdSpriteColorRemapPixelsStub(void *buf, Sint32 size);

#endif
