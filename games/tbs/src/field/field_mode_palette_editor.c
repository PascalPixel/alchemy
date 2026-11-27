/* The debug palette editor. Part of field_mode.c. */

#include "field_mode_internal.h"

/* The palettes, as plain halfwords, and where the editor draws: from column 13
   of row 1 of screen block 4. */
#define EDITOR_PALETTE	   ((Uint16 *)PLTT)
#define EDITOR_MAP		   ((Uint16 *)BG_SCREEN_BLOCK(4) + BG_SCREEN_WIDTH + 13)
/* The swatch tile sheet's tiles, from tile 208 of character block 0: colour n
   is drawn with EDITOR_SWATCH_TILE + n, and the 32 tiles after the swatches
   show the numbers 0 to 31. */
#define EDITOR_SWATCH_TILE 208
/* The map entry of the tile showing the number 0, in WINDOW_PALETTE: number n
   is EDITOR_NUMBER_TILE + n. */
#define EDITOR_NUMBER_TILE WINDOW_TILE(EDITOR_SWATCH_TILE + PALETTE_COLORS)
/* The editor cycles through the map's 14 background palettes. */
#define EDITOR_PALETTES	   14
/* START blinks the colour every 10 ticks: white, the colour, black, the
   colour. */
#define EDITOR_BLINK_TICKS 10

/* The channel up and down choose, by its row under the swatch. */
typedef enum EDITOR_CHANNEL {
	EDITOR_RED = 1,
	EDITOR_GREEN = 2,
	EDITOR_BLUE = 3
} EDITOR_CHANNEL;

/* The debug palette editor (L on the Pause menu in debug mode). Shows colours 1
   to 15 of a palette with their red, green and blue levels, 0 to 31. */
void fdPaletteEditorRun(void)
{
	Sint32	palette = 0;
	Sint32	channel = EDITOR_RED;
	Sint32	color = 1;
	Uint16 *swatchColumn;
	Uint16 *tileEntry;
	Uint16 *paletteColor;
	Uint32	tile;
	Uint32	rgb555;
	Uint32	red;
	Uint32	green;
	Uint32	blue;
	Uint32	i;

	syDecompressHalfwords(DebugPaletteSwatchTiles,
		BG_CHAR_BLOCK(0) + EDITOR_SWATCH_TILE * TILE_SIZE_4BPP);
redraw:
	/* Column 0 shows the palette number and R, G, B; columns 1 to 15 show each
	   colour's swatch, drawn in the palette being edited, and its three
	   channels. */
	tile = BG_TILE_PALETTE(palette) + EDITOR_SWATCH_TILE + 1;
	paletteColor = &EDITOR_PALETTE[palette * PALETTE_COLORS + 1];
	swatchColumn = EDITOR_MAP;
	tileEntry = swatchColumn;
	*tileEntry = EDITOR_NUMBER_TILE + palette;
	tileEntry += BG_SCREEN_WIDTH;
	*tileEntry = WINDOW_TILE('R');
	tileEntry += BG_SCREEN_WIDTH;
	*tileEntry = WINDOW_TILE('G');
	tileEntry += BG_SCREEN_WIDTH;
	*tileEntry = WINDOW_TILE('B');
	swatchColumn++;
	for(i = 1; i < PALETTE_COLORS; i++) {
		tileEntry = swatchColumn;
		*tileEntry = tile++;
		rgb555 = *paletteColor++;
		tileEntry += BG_SCREEN_WIDTH;
		*tileEntry = RGB_RED(rgb555) + EDITOR_NUMBER_TILE;
		tileEntry += BG_SCREEN_WIDTH;
		*tileEntry = RGB_GREEN(rgb555) + EDITOR_NUMBER_TILE;
		tileEntry += BG_SCREEN_WIDTH;
		*tileEntry = RGB_BLUE(rgb555) + EDITOR_NUMBER_TILE;
		swatchColumn++;
	}
	sySynch(1);
	for(;;) {
		if(PerRepeat & KEY_UP) {
			channel--;
			if(channel < EDITOR_RED)
				channel = EDITOR_BLUE;
		}
		if(PerRepeat & KEY_DOWN) {
			channel++;
			if(channel > EDITOR_BLUE)
				channel = EDITOR_RED;
		}
		if(PerRepeat & KEY_LEFT) {
			color--;
			if(color <= 0)
				color = PALETTE_COLORS - 1;
		}
		if(PerRepeat & KEY_RIGHT) {
			color++;
			if(color > PALETTE_COLORS - 1)
				color = 1;
		}
		if(PerRepeat & KEY_L) {
			palette--;
			if(palette < 0)
				palette = EDITOR_PALETTES - 1;
			goto redraw;
		}
		if(PerRepeat & KEY_R) {
			palette++;
			if(palette >= EDITOR_PALETTES)
				palette = 0;
			goto redraw;
		}
		if(PerRepeat & KEY_A) {
			paletteColor = &EDITOR_PALETTE[palette * PALETTE_COLORS + color];
			rgb555 = *paletteColor;
			red = RGB_RED(rgb555);
			green = RGB_GREEN(rgb555);
			blue = RGB_BLUE(rgb555);
			if(channel == EDITOR_RED && red < RGB_LEVEL_MAX)
				red++;
			if(channel == EDITOR_GREEN && green < RGB_LEVEL_MAX)
				green++;
			if(channel == EDITOR_BLUE && blue < RGB_LEVEL_MAX)
				blue++;
			*paletteColor = RGB(red, green, blue);
			goto redraw;
		}
		if(PerRepeat & KEY_B) {
			paletteColor = &EDITOR_PALETTE[palette * PALETTE_COLORS + color];
			rgb555 = *paletteColor;
			red = RGB_RED(rgb555);
			green = RGB_GREEN(rgb555);
			blue = RGB_BLUE(rgb555);
			if(channel == EDITOR_RED && red != 0)
				red--;
			if(channel == EDITOR_GREEN && green != 0)
				green--;
			if(channel == EDITOR_BLUE && blue != 0)
				blue--;
			*paletteColor = RGB(red, green, blue);
			goto redraw;
		}
		if(PerRepeat & KEY_START) {
			/* red counts the ticks here. */
			paletteColor = &EDITOR_PALETTE[palette * PALETTE_COLORS + color];
			rgb555 = *paletteColor;
			red = 0;
			for(;;) {
				sySynch(1);
				if(!(PerData & KEY_START))
					break;
				if(red == 0)
					*paletteColor = RGB_WHITE;
				if(red == EDITOR_BLINK_TICKS)
					*paletteColor = rgb555;
				if(red == EDITOR_BLINK_TICKS * 2)
					*paletteColor = 0;
				if(red == EDITOR_BLINK_TICKS * 3)
					*paletteColor = rgb555;
				if(++red >= EDITOR_BLINK_TICKS * 4)
					red = 0;
			}
			*paletteColor = rgb555;
		}
		if(PerRepeat & KEY_SELECT)
			break;
		/* DEAD: a read of TickCount nothing uses; the ROM keeps its load. */
		TickCount;
		sySynch(1);
	}
	syWindowLoadGraphics();
	syPrintClear();
}

#undef EDITOR_PALETTE
#undef EDITOR_MAP
#undef EDITOR_NUMBER_TILE
#undef EDITOR_SWATCH_TILE
#undef EDITOR_PALETTES
#undef EDITOR_BLINK_TICKS
