#ifndef ALCHEMY_MODULE_GRAPHICS_TITLE_INTRO_H
#define ALCHEMY_MODULE_GRAPHICS_TITLE_INTRO_H

#include "types.h"

extern const u8 gTitle_IntroSpriteColumns[];
extern const u8 gTitle_PromptBlendLevels[60];
void Title_UpdateIntroSprites(void);
void Title_LoadIntroBackgrounds(void);
s32 Title_ShowIntro(s32 prompt);
void Title_NoOpPalette(void);
s32 Title_ShowNintendoLogo(s32 mode);
s32 Title_ShowCamelotLogo(void);
void Title_NoOpColor(void);
void TitlePaletteFade_CalculateSteps(s16 *sourceComponents,
                                     s16 *targetComponents, s16 *componentSteps,
                                     s32 frames);
void TitlePaletteFade_Update(void);
void TitlePaletteFade_BuildColors(u32 mode, u16 *source, u16 *destination,
                                  s32 selectedPalettes);
void TitlePaletteFade_Initialize(void);
void TitlePaletteFade_Release(void);
void TitlePaletteFade_SetTarget(s32 mode, s32 selectedPalettes);
// UB: the early call implicitly returns int, but the definition returns void.
// A plain prototype reverses the two argument moves in Title_ShowIntro (EN).
#ifdef UBFIX
void TitlePaletteFade_SetCurrent(s32 mode, s32 selectedPalettes);
#endif
void TitlePaletteFade_SetBackdrop(s32 color);
void TitlePaletteFade_Start(s32 frames);
s32 TitlePaletteFade_ClampComponent(s32 value);
s32 TitlePaletteFade_ClampShiftedComponent(s32 value);

#endif
