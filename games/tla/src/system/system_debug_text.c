/* Number text, the debug text screen and the window graphics. Part of
   system.c. */

#include "window/window.h"

/* Debug text: a 32x20-character screen of window font tiles in BG0, written
   from DebugTextCursor onwards; characters are tile numbers in
   WINDOW_PALETTE. */

/* The debug text screen's bytes: 32 by 20 entries of a halfword. */
#define DEBUG_TEXT_SCREEN_BYTES (BG_SCREEN_WIDTH * DISPLAY_TILE_HEIGHT * 2)
/* The basic sprite palettes, sprite palettes 0 to 13. */
#define BASIC_SPRITE_PALETTES	14

/* The number buffer's hexadecimal: eight digits. */
#define HEX_DIGITS			  8
/* The number buffer's decimal: a sign place and up to ten digits. */
#define DECIMAL_DIGITS_MAX	  10
/* The most characters syPrint writes of a string. */
#define DEBUG_TEXT_MAX_LENGTH 32

/* Writes `value` as HEX_DIGITS hexadecimal digits to the number buffer. */
void syStringFormatHex(Uint32 value)
{
	Uint8		*buf = NumberTextBuffer;
	const Uint8 *digits = StringHexDigits;
	Sint32		 idx;

	for(idx = HEX_DIGITS - 1; idx >= 0; idx--) {
		buf[idx] = digits[value & 0xf];
		value >>= 4;
	}
	NumberTextBuffer[HEX_DIGITS] = 0;
}

/* Writes `value` to the number buffer as DECIMAL_DIGITS_MAX + 1 characters:
   spaces, a sign (a space or '-') and its decimal digits, right-aligned. */
void syStringFormatDecimal(Sint32 value)
{
	Sint32 negative;
	Sint32 i;
	Sint32 quotient;

	negative = 0;
	if(value < 0) {
		value = -value;
		negative = 1;
	}
	NumberTextBuffer[DECIMAL_DIGITS_MAX + 1] = 0;
	for(i = DECIMAL_DIGITS_MAX; value != 0; i--) {
		quotient = syMathUnsignedMulHigh(value, RECIPROCAL_OF(10));
		value -= quotient * 10;
		NumberTextBuffer[i] = value + '0';
		value = quotient;
	}
	if(i == DECIMAL_DIGITS_MAX) {
		NumberTextBuffer[i] = '0';
		i = DECIMAL_DIGITS_MAX - 1;
	}
	if(negative) {
		NumberTextBuffer[i] = '-';
		i--;
	}
	for(; i >= 0; i--)
		NumberTextBuffer[i] = ' ';
}

/* Writes `cnt` blank characters (WINDOW_TILE_BLANK) at the debug text cursor
   and moves it on, when debug text is on. */
void syPrintBlanks(Uint32 cnt)
{
	Uint16 *dst = DebugTextCursor;
	Uint32	i;

	if(DebugTextEnabled != 0) {
		for(i = 0; i < cnt; i++)
			*dst++ = WINDOW_TILE_BLANK;
		DebugTextCursor = dst;
	}
}

/* Prints the string `text` (at most DEBUG_TEXT_MAX_LENGTH characters) at the
   debug text cursor, wrapping from the end of the screen to its start, when
   debug text is on; the cursor moves past it. */
void syPrint(const Uint8 *text)
{
	if(DebugTextEnabled != 0) {
		Uint16 *cursor = DebugTextCursor;
		Uint32	c = *text;
		Uint32	cnt = 0;

		text++;
		if(c != 0) {
			Uint32 palette = BG_TILE_PALETTE(WINDOW_PALETTE);

			/* The end of the 32 by 20 entry screen wraps to its start. */
			do {
				*cursor++ = c | palette;
				if(cursor ==
					(Uint16 *)(BG_SCREEN_BLOCK(DEBUG_TEXT_SCREEN_BLOCK) +
						DEBUG_TEXT_SCREEN_BYTES))
					cursor = (Uint16 *)BG_SCREEN_BLOCK(DEBUG_TEXT_SCREEN_BLOCK);
				if(++cnt > DEBUG_TEXT_MAX_LENGTH - 1)
					break;
				c = *text++;
			} while(c != 0);
		}
		DebugTextCursor = cursor;
	}
}

/* Prints the last `digits` hexadecimal digits of `value` (1 to HEX_DIGITS;
   anything else prints all of them). Reached through the stub table. */
void syPrintHex(Uint32 value, Sint32 digits)
{
	if(digits < 1 || digits > HEX_DIGITS)
		digits = HEX_DIGITS;
	syStringFormatHex(value);
	syPrint(NumberTextHexEnd - digits);
}

/* Prints the last `digits` characters of `value` in signed decimal (1 to
   DECIMAL_DIGITS_MAX; anything else prints DECIMAL_DIGITS_MAX). Reached through
   the stub table. */
void syPrintDec(Sint32 value, Sint32 digits)
{
	if(digits < 1 || digits > DECIMAL_DIGITS_MAX)
		digits = DECIMAL_DIGITS_MAX;
	syStringFormatDecimal(value);
	syPrint(NumberTextDecimalEnd - digits);
}

/* Blanks the debug text screen, homes the cursor and points BG0 at it. */
void syPrintClear(void)
{
	DMA_FILL32(WINDOW_TILE_PAIR(WINDOW_TILE_BLANK),
		BG_SCREEN_BLOCK(DEBUG_TEXT_SCREEN_BLOCK), DEBUG_TEXT_SCREEN_BYTES);
	DebugTextCursor = (Uint16 *)BG_SCREEN_BLOCK(DEBUG_TEXT_SCREEN_BLOCK);
	syIOWrite16(BGCNT_SCREEN_BASE(DEBUG_TEXT_SCREEN_BLOCK), &REG_BG0CNT);
}

/* Loads the window font and border tiles into BG character block 0, the window
   palette into WINDOW_PALETTE with its gradient colours, and the basic sprite
   palettes. */
void syWindowLoadGraphics(void)
{
	/* The window font and borders: 256 tiles of 16 colours. */
	syDMAXCopy(syResourceGetTableEntry(RESOURCE_WINDOW_TILES),
		(void *)BG_CHAR_BLOCK(0), DMA_COPY_WORDS(256 * TILE_SIZE_4BPP / 4),
		REG_DMA3);
	syDMAXCopy((const void *)Window_Palette,
		(void *)&BG_PLTT_COLOR(WINDOW_PALETTE, 0),
		DMA_COPY_HALFWORDS(PALETTE_COLORS), REG_DMA3);
	syIOWrite16(0, &PLTT_BACKDROP);
	/* The window's background colour, three darker shades and three lighter. */
	syIOWrite16(RGB(0, 12, 16),
		&BG_PLTT_COLOR(WINDOW_PALETTE, WINDOW_BACKGROUND_COLOR));
	syIOWrite16(RGB(0, 11, 14),
		&BG_PLTT_COLOR(WINDOW_PALETTE, WINDOW_BACKGROUND_COLOR + 1));
	syIOWrite16(RGB(0, 10, 12),
		&BG_PLTT_COLOR(WINDOW_PALETTE, WINDOW_BACKGROUND_COLOR + 2));
	syIOWrite16(RGB(0, 9, 10),
		&BG_PLTT_COLOR(WINDOW_PALETTE, WINDOW_BACKGROUND_COLOR + 3));
	syIOWrite16(RGB(0, 13, 18),
		&BG_PLTT_COLOR(WINDOW_PALETTE, WINDOW_BACKGROUND_COLOR + 4));
	syIOWrite16(RGB(0, 14, 20),
		&BG_PLTT_COLOR(WINDOW_PALETTE, WINDOW_BACKGROUND_COLOR + 5));
	syIOWrite16(RGB(0, 15, 22),
		&BG_PLTT_COLOR(WINDOW_PALETTE, WINDOW_BACKGROUND_COLOR + 6));
	syDMAXCopy((const void *)System_BasicColorPalette, (void *)OBJ_PLTT,
		DMA_COPY_HALFWORDS(BASIC_SPRITE_PALETTES * PALETTE_COLORS), REG_DMA3);
}

/* Loads the basic sprite palettes. */
void syPaletteLoadBasicSprCols(void)
{
	syDMAXCopy((const void *)System_BasicColorPalette, (void *)OBJ_PLTT,
		DMA_COPY_HALFWORDS(BASIC_SPRITE_PALETTES * PALETTE_COLORS), REG_DMA3);
}

#undef DEBUG_TEXT_SCREEN_BYTES
#undef BASIC_SPRITE_PALETTES
#undef HEX_DIGITS
#undef DECIMAL_DIGITS_MAX
#undef DEBUG_TEXT_MAX_LENGTH
