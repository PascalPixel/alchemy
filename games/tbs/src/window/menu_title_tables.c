/// @file
/// The pulse of a menu's title sprite and the row patterns of the battle
/// menus' range icons.

#include "graphics/graphics.h"

/// The scale, 16.16, of WindowSprite_Pulse's eight steps: about 1.10 up to
/// 1.22 and back.
const s32 gWindowSprite_PulseScales[] = {
    0x11999, 0x12147, 0x128f5, 0x130a3, 0x13851, 0x130a3, 0x128f5, 0x12147,
};

/// A row of 4-bit pixels for the centre tile and one for the end tile, colour
/// 5 where the pattern marks (Menu_BuildLocalizedPatternTiles).
const struct TileMask gMenu_BarPatternRows[] = {
    {0x00055000, 0x00000000}, {0x05055050, 0x00000000},
    {0x05055050, 0x00000005}, {0x05055050, 0x00000505},
    {0x05055050, 0x00050505}, {0x05055050, 0x05050505},
};
