#ifndef ALCHEMY_SCALES_H
#define ALCHEMY_SCALES_H

/// @file
/// The window sprites' and cursors' motion curves, the window colour wheel, the
/// settings and Pause menus' icons and colour presets, and the speakers'
/// portraits (window/scales.c).

#include "types.h"

extern const u16 gWindowSprite_BounceScales[32];
extern const s8 gWindowSprite_BounceScalesTail[8];
extern const s8 gMenuCursor_BobOffsets[16];
extern const u8 gWindowColor_WaveTable[121];

extern const u8 gSettingsMenu_HuePresets[6];
extern const u8 gSettingsMenu_BrightnessPresets[6];
extern const s8 gPauseMenu_Icons[8];

extern const s16 gPortrait_CharacterMap[20];
extern const s16 gPortrait_SpriteMap[100];

#endif
