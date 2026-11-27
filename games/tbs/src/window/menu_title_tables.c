/* The pulse of a menu's title sprite and the row patterns of the battle menus'
   range icons. */

#include "graphics/graphics.h"

/* The scale of mnWindowSprPulse's eight steps: about 1.10 up to 1.22 and
   back. */
const FIXED WindowSpritePulseScales[] = {
	toFIXED(1.1),
	toFIXED(1.13),
	toFIXED(1.16),
	toFIXED(1.19),
	toFIXED(1.22),
	toFIXED(1.19),
	toFIXED(1.16),
	toFIXED(1.13),
};

/* A row of 4-bit pixels for the centre tile and one for the end tile, colour 5
   where the pattern marks (mnRangeIconBuildTiles); a word's lowest nibble is
   its leftmost pixel. */
const TILE_MASK MenuBarPatternRows[] = {
	{0x00055000, 0x00000000},
	{0x05055050, 0x00000000},
	{0x05055050, 0x00000005},
	{0x05055050, 0x00000505},
	{0x05055050, 0x00050505},
	{0x05055050, 0x05050505},
};
