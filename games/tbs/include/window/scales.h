#ifndef ALCHEMY_SCALES_H
#define ALCHEMY_SCALES_H

/* @file */
/* The window sprites' and cursors' motion curves, the window colour wheel, the
 */
/* settings and Pause menus' icons and colour presets, and the speakers' */
/* portraits (window/scales.c). */

#include "types.h"

extern const u16 WindowSprite_BounceScales[32];
extern const s8 WindowSprite_BounceScalesTail[8];
extern const s8 MenuCursor_BobOffsets[16];
extern const u8 WindowColor_WaveTable[121];

extern const u8 SettingsMenu_HuePresets[6];
extern const u8 SettingsMenu_BrightnessPresets[6];
extern const s8 PauseMenu_Icons[8];

extern const s16 Portrait_CharacterMap[20];
extern const s16 Portrait_SpriteMap[100];

#endif
