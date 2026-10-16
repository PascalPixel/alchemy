#ifndef ALCHEMY_UNIT_GRAPHICS_PALETTE_PALETTE_H
#define ALCHEMY_UNIT_GRAPHICS_PALETTE_PALETTE_H

#include "types.h"

extern const u8 gTitle_IntroSpriteColumns[];
extern const u8 gTitle_PromptBlendLevels[60];
void Title_UpdateIntroSprites(void);
void Title_LoadIntroBackgrounds(void);
s32 Title_ShowIntro(s32 prompt);
void Title_NoOpPalette(void);
s32 Title_ShowSplashScreen(s32 mode);
s32 Title_ShowAnimatedSplash(void);
void Graphics_NoOpColor(void);
void PaletteFade_ComputeSteps(s16 *src, s16 *dst, s16 *step, s32 frames);
void PaletteFade_Update(void);
void Palette_Transform(u32 mode, u16 *src, u16 *dst, s32 half);
void PaletteFade_Init(void);
void PaletteFade_Release(void);
void PaletteFade_SetTarget(s32 index, s32 transform);
// UB: the early call implicitly returns int, but the definition returns void.
// A plain prototype reverses the two argument moves in Title_ShowIntro (EN).
#ifdef UBFIX
void PaletteFade_SetCurrent(s32 mode, s32 paletteHalf);
#endif
void PaletteFade_SetBackdrop(s32 value);
void PaletteFade_Start(s32 value);
s32 Graphics_ClampRgb555Channel(s32 val);
s32 Graphics_ClampRgb555Component(s32 val);

#endif
