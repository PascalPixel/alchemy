/* @file */
/* Kolima Forest, while Tret's curse lasts (FLAG_KOLIMA_CURSE_LIFTED clear): */
/* the forest's colours turned redder and darker, and both sets of colours kept
 */
/* for the event where the curse lifts. */

#include "system.h"

/* Background colour `color` of palette `palette`, counted from colour 0. */
#define BG_COLOR_INDEX(palette, color) ((palette) * PALETTE_COLORS + (color))

/* The background palettes whose first colours are the water's blues, colours */
/* 1 to 7 of the one and 1 to 8 of the other, which the curse leaves as they */
/* are. At Tret's tree the second palette's are tans. */
#define KOLIMA_WATER_PALETTE		1
#define KOLIMA_SECOND_WATER_PALETTE 12

/* The size of a kept set of colours: the palette fade's background and sprite
 */
/* colours. */
#define KOLIMA_PALETTE_WORDS (2 * PALETTE_FADE_BG_COLORS * sizeof(u16) / 4)

/* The healthy colours, kept by scKolimaPaletteSaveHealthy; each scene defines
 */
/* the 1792 bytes. */
extern u8 KolimaPalette_Healthy[];
/* The cursed colours, kept by scKolimaPaletteSaveCursed. */
extern u8 KolimaPalette_Cursed[];

static u16 scKolimaPaletteCurseCol(u16 color, s32 divisor);
static void scKolimaPaletteSaveHealthy(void);
static void scKolimaPaletteSaveCursed(void);
static void scKolimaPaletteCapture(void);

/* Keeps the healthy colours, curses the background colours but the water's */
/* by divisor, keeps the cursed colours too, and shows them. */
void scKolimaPaletteCurse(s32 divisor) {
	u16 i;

	scKolimaPaletteSaveHealthy();
	for(i = 0; i < PALETTE_FADE_BG_COLORS; i++) {
		if((i < BG_COLOR_INDEX(KOLIMA_WATER_PALETTE, 1) ||
			i > BG_COLOR_INDEX(KOLIMA_WATER_PALETTE, 7)) &&
		   (i < BG_COLOR_INDEX(KOLIMA_SECOND_WATER_PALETTE, 1) ||
			i > BG_COLOR_INDEX(KOLIMA_SECOND_WATER_PALETTE, 8))) {
			u16 *pal = (u16 *)&BG_PLTT[i];
			*pal = scKolimaPaletteCurseCol(*pal, divisor);
		}
	}
	scKolimaPaletteCapture();
	scKolimaPaletteSaveCursed();
	fdPaletteFadeSetTarget(PALETTE_TINT_NONE, PALETTES_ALL);
}

/* One cursed colour: red gains 1 / (4 * divisor) of itself, up to */
/* RGB_LEVEL_MAX, and green and blue lose 1 / divisor. */
static u16 scKolimaPaletteCurseCol(u16 color, s32 divisor) {
	s16 green = RGB_GREEN(color);
	s16 red = RGB_RED(color);
	s16 blue = RGB_BLUE(color);
	u32 packed;

	red = (red + syMathDivide(red, (divisor << 2)));
	green = (green - syMathDivide(green, divisor));
	blue = (blue - syMathDivide(blue, divisor));

	if(red > RGB_LEVEL_MAX)
		red = RGB_LEVEL_MAX;

	packed = red | ((blue << BLUE_SHIFT) | (green << GREEN_SHIFT));
	return packed;
}

/* Keeps the healthy colours: the palette fade's colours into */
/* KolimaPalette_Healthy. */
static void scKolimaPaletteSaveHealthy(void) {
	syDMAXCopy(PaletteFadeState->pal, KolimaPalette_Healthy,
			   DMA_COPY_WORDS(KOLIMA_PALETTE_WORDS), REG_DMA3);
}

/* Keeps the cursed colours: the palette fade's colours into */
/* KolimaPalette_Cursed. */
static void scKolimaPaletteSaveCursed(void) {
	syDMAXCopy(PaletteFadeState->pal, KolimaPalette_Cursed,
			   DMA_COPY_WORDS(KOLIMA_PALETTE_WORDS), REG_DMA3);
}

/* Copies the palettes on the screen, background and sprites, into the palette
 */
/* fade's buffer as the colours to fade from. */
static void scKolimaPaletteCapture(void) {
	u16 *buf = PaletteFadeState->pal;

	syDMAXCopy((const void *)BG_PLTT, buf,
			   DMA_COPY_WORDS(PALETTE_FADE_BG_COLORS * sizeof(u16) / 4),
			   REG_DMA3);
	syDMAXCopy((const void *)OBJ_PLTT, buf + PALETTE_FADE_BG_COLORS,
			   DMA_COPY_WORDS(PALETTE_FADE_BG_COLORS * sizeof(u16) / 4),
			   REG_DMA3);
	fdPaletteFadeSetSource(PALETTE_TINT_NONE, PALETTES_ALL);
}

/* Makes the healthy colours (second 0) or the cursed ones (second 1) the */
/* colours to fade to; the event then fades with */
/* fdPaletteFadeStartInterpolation. */
void scKolimaPaletteRestore(s32 second) {
	u16 *buf = PaletteFadeState->pal;

	if(second)
		syDMAXCopy(KolimaPalette_Cursed, buf,
				   DMA_COPY_WORDS(KOLIMA_PALETTE_WORDS), REG_DMA3);
	else
		syDMAXCopy(KolimaPalette_Healthy, buf,
				   DMA_COPY_WORDS(KOLIMA_PALETTE_WORDS), REG_DMA3);
	fdPaletteFadeSetTarget(PALETTE_TINT_NONE, PALETTES_ALL);
	scKolimaPaletteCapture();
}

#undef BG_COLOR_INDEX
#undef KOLIMA_WATER_PALETTE
#undef KOLIMA_SECOND_WATER_PALETTE
#undef KOLIMA_PALETTE_WORDS
