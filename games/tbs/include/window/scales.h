#ifndef ALCHEMY_SCALES_H
#define ALCHEMY_SCALES_H

/* The window sprites' and cursors' motion curves, the window colour wheel, the
   settings and Pause menus' icons and colour presets, and the speakers'
   portraits (window/scales.c). */

#include "types.h"

extern const Uint16 WindowSpriteBounceScales[32];
extern const Sint8	WindowSpriteBounceScalesTail[8];
extern const Sint8	MenuCursorBobOffsets[16];
extern const Uint8	WindowColorWaveTable[121];

extern const Uint8 SettingsMenuHuePresets[6];
extern const Uint8 SettingsMenuBrightnessPresets[6];
extern const Sint8 PauseMenuIcons[8];

extern const Sint16 PortraitCharacterMap[20];
extern const Sint16 PortraitSpriteMap[100];

#endif
