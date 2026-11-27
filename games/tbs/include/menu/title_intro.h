#ifndef ALCHEMY_TITLE_INTRO_H
#define ALCHEMY_TITLE_INTRO_H

/* The publisher's and Camelot's logos, the intro that scrolls in to the title
   picture, and the palette fades they use (menu/title_intro.c). */

#include "types.h"

extern const Uint8 TitleFlareColumns[];
extern const Uint8 TitlePromptBlendLevels[60];
void			   mnTitleUpdateIntroSprs(void);
void			   mnTitleLoadIntroBackgrounds(void);
/* The title module's functions in the order of its stub table
   (src/menu/title_stubs.s). */
Sint32 mnTitleShowTitleScreen(Sint32 prompt);
/* Unused: an empty entry of the title's stub table, after
   mnTitleShowTitleScreen's. */
void mnTitleAfterIntroNoOperation(void);
/* Unused: an empty entry of the title's stub table, before the logos. */
void   mnTitleBeforeLogosNoOperation(void);
Sint32 mnTitleShowPublisherLogo(Sint32 fadeOut);
Sint32 mnTitleShowCamelotLogo(void);

void mnTitlePaletteFadeCalcSteps(Sint16 *srcComponents, Sint16 *tgtComponents,
	Sint16 *componentSteps, Sint32 ticks);
void mnTitlePaletteFadeUpdate(void);
void mnTitlePaletteFadeBuildCols(
	Uint32 mode, Uint16 *src, Uint16 *dst, Sint32 selectedPalettes);
void mnTitlePaletteFadeInit(void);
void mnTitlePaletteFadeRelease(void);
void mnTitlePaletteFadeSetTarget(Sint32 mode, Sint32 selectedPalettes);
/* UB: the early call implicitly returns int, but the definition returns void. A
   plain prototype reverses the two argument moves in mnTitleShowTitleScreen
   (EN). */
#if FIX_FUNCTION_SIGNATURES
void mnTitlePaletteFadeSetCurrent(Sint32 mode, Sint32 selectedPalettes);
#endif
void   mnTitlePaletteFadeSetBackdrop(Sint32 col);
void   mnTitlePaletteFadeStart(Sint32 ticks);
Sint32 mnTitlePaletteFadeClampChannel(Sint32 value);
Sint32 mnTitlePaletteFadeClampShiftedChannel(Sint32 value);

#endif
