#ifndef ALCHEMY_TITLE_INTRO_H
#define ALCHEMY_TITLE_INTRO_H

/* @file */
/* The publisher's and Camelot's logos, the intro that scrolls in to the title
 */
/* picture, and the palette fades they use (menu/title_intro.c). */

#include "types.h"

extern const u8 Title_FlareColumns[];
extern const u8 Title_PromptBlendLevels[60];
void mnTitleUpdateIntroSprs(void);
void mnTitleLoadIntroBackgrounds(void);
/* The title module's functions in the order of its stub table */
/* (src/menu/title_stubs.s). */
s32 mnTitleShowTitleScr(s32 prompt);
/* Unused: an empty entry of the title's stub table, after */
/* mnTitleShowTitleScr's. */
void mnTitleAfterIntroNoOp(void);
/* Unused: an empty entry of the title's stub table, before the logos. */
void mnTitleBeforeLogosNoOp(void);
s32 mnTitleShowPublisherLogo(s32 fadeOut);
s32 mnTitleShowCamelotLogo(void);

void mnTitlePaletteFadeCalcSteps(s16 *sourceComponents, s16 *targetComponents,
								 s16 *componentSteps, s32 frames);
void mnTitlePaletteFadeUpdate(void);
void mnTitlePaletteFadeBuildCols(u32 mode, u16 *source, u16 *destination,
								 s32 selectedPalettes);
void mnTitlePaletteFadeInit(void);
void mnTitlePaletteFadeRelease(void);
void mnTitlePaletteFadeSetTarget(s32 mode, s32 selectedPalettes);
/* UB: the early call implicitly returns int, but the definition returns void.
 */
/* A plain prototype reverses the two argument moves in mnTitleShowTitleScr */
/* (EN). */
#ifdef UBFIX
void mnTitlePaletteFadeSetCurrent(s32 mode, s32 selectedPalettes);
#endif
void mnTitlePaletteFadeSetBackdrop(s32 color);
void mnTitlePaletteFadeStart(s32 frames);
s32 mnTitlePaletteFadeClampChannel(s32 value);
s32 mnTitlePaletteFadeClampShiftedChannel(s32 value);

#endif
