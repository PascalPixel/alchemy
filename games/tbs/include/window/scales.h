#ifndef ALCHEMY_SCALES_H
#define ALCHEMY_SCALES_H

/* The window sprites' and cursors' motion curves, the window colour wheel, the
   settings and Pause menus' icons and colour presets, and the speakers'
   portraits (window/scales.c). */

#include "types.h"

extern const Uint16 WindowSprite_BounceScales[32];
extern const Sint8	WindowSprite_BounceScalesTail[8];
extern const Sint8	MenuCursor_BobOffsets[16];
extern const Uint8	WindowColor_WaveTable[121];

extern const Uint8 SettingsMenu_HuePresets[6];
extern const Uint8 SettingsMenu_BrightnessPresets[6];
extern const Sint8 PauseMenu_Icons[8];

extern const Sint16 Portrait_CharacterMap[20];
extern const Sint16 Portrait_SpriteMap[100];

#endif
