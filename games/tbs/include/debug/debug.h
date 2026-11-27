#ifndef ALCHEMY_DEBUG_H
#define ALCHEMY_DEBUG_H

/* @file */
/* What the debug rooms and windows share: the main-image headers their menus */
/* call into, the palette editor's swatch tiles, and the browsers' steps. */

#include "edition.h"
#include "types.h"
#include "field/field_sequence.h"
#include "game/game.h"
#include "stubs.h"
#include "menu/menu.h"
#include "window/window.h"

/* The palette editor's swatch tiles, compressed; it decompresses them to */
/* tile 208 of character block 0, one tile a colour. */
extern const u8 Debug_PaletteSwatchTiles[];

/* The debug item browsers' item numbers, 0 to 269, wrapping. */
#define DEBUG_ITEM_COUNT 270
/* The debug browsers' steps: Right and Left move the number ten, R and L */
/* thirty. */
#define DEBUG_BROWSER_ROW_STEP	10
#define DEBUG_BROWSER_PAGE_STEP 30

#endif
