#ifndef ALCHEMY_TITLE_INTRO_H
#define ALCHEMY_TITLE_INTRO_H

/// @file
/// The publisher's and Camelot's logos, the intro that scrolls in to the title
/// picture, and the palette fades they use (menu/title_intro.c).

#include "types.h"

extern const u8 gTitle_FlareColumns[];
extern const u8 gTitle_PromptBlendLevels[60];
void Title_UpdateIntroSprites(void);
void Title_LoadIntroBackgrounds(void);
s32 Title_ShowTitleScreen(s32 prompt);
/// Unused: an empty entry of the title's far-call table, before the logos.
void Title_BeforeLogosNoOp(void);
s32 Title_ShowPublisherLogo(s32 fadeOut);
s32 Title_ShowCamelotLogo(void);
/// Unused: an empty entry of the title's far-call table, after
/// Title_ShowTitleScreen's.
void Title_AfterIntroNoOp(void);
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
// A plain prototype reverses the two argument moves in Title_ShowTitleScreen
// (EN).
#ifdef UBFIX
void TitlePaletteFade_SetCurrent(s32 mode, s32 selectedPalettes);
#endif
void TitlePaletteFade_SetBackdrop(s32 color);
void TitlePaletteFade_Start(s32 frames);
s32 TitlePaletteFade_ClampChannel(s32 value);
s32 TitlePaletteFade_ClampShiftedChannel(s32 value);

#endif
