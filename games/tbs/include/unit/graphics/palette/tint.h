#ifndef ALCHEMY_UNIT_GRAPHICS_PALETTE_TINT_H
#define ALCHEMY_UNIT_GRAPHICS_PALETTE_TINT_H

#include "types.h"

extern const u8 Title_IntroSpriteColumns[];
extern const u8 Title_PromptBlendLevels[60];
void Title_PlaceIntroSprites(void);
void Title_LoadIntroBackgrounds(void);
s32 Title_ShowIntro(s32 prompt);
void Title_NoOpPalette(void);
s32 Title_ShowSplashScreen(s32 mode);
s32 Title_ShowAnimatedSplash(void);
void Graphics_NoOpColor(void);
void Graphics_InterpolatePaletteBuffers(s16 *src, s16 *dst, s16 *step, s32 frames);
void TitlePalette_UpdateFade(void);
void Graphics_TransformPaletteBuffer(u32 mode, u16 *src, u16 *dst, s32 half);
void TitlePalette_InitializeBuffers(void);
void Runtime_ScheduleCallbackAndReleaseBlock32A(void);
void Graphics_TransformLargePalette(s32 index, s32 transform);
/* Graphics_TransformSmallPalette is called before its definition with no prototype in scope. */
void Graphics_SetPaletteTransformValue(s32 value);
void Graphics_UpdatePaletteInterpolation(s32 value);
s32 Graphics_ClampRgb555Channel(s32 val);
s32 Graphics_ClampRgb555Component(s32 val);

#endif
