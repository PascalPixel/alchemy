/* Number text, the debug text screen and the window graphics. Part of
   system.c. */

/* Debug text: a 32x20-character screen of window font tiles in BG0, written
   from DebugTextCursor onwards; characters are tile numbers in
   WINDOW_PALETTE. */

/* The screen block BG0 shows the debug text from, and its 32 by 20 entries'
   bytes. */
#define DEBUG_TEXT_SCREEN		BG_SCREEN_BLOCK(4)
#define DEBUG_TEXT_SCREEN_BYTES (BG_SCREEN_WIDTH * DISPLAY_TILE_HEIGHT * 2)

/* The number buffer's hexadecimal: eight digits. */
#define HEX_DIGITS			  8
/* The number buffer's decimal: a sign place and up to ten digits. */
#define DECIMAL_DIGITS_MAX	  10
/* The most characters syPrint writes of a string. */
#define DEBUG_TEXT_MAX_LENGTH 32
/* The colours of the basic sprite palettes, 0 to 13. */
#define BASIC_SPRITE_COLORS	  (14 * PALETTE_COLORS)

/* Writes `value` as HEX_DIGITS hexadecimal digits to the number buffer. */
void syStringFormatHex(Uint32 value)
{
	Sint32 i;

	for(i = HEX_DIGITS - 1; i >= 0; i--) {
		NumberBuffer[i] = StringHexDigits[value & 0xf];
		value >>= 4;
	}
	NumberBuffer[HEX_DIGITS] = 0;
}

/* Writes `num` to the number buffer as DECIMAL_DIGITS_MAX + 1 characters:
   spaces, a sign (a space or '-') and its decimal digits, right-aligned. */
void syStringFormatDecimal(Sint32 num)
{
	const Uint32 *placeValues = (const Uint32 *)StringPowersOfTen;
	Sint8		  sign = ' ';
	Uint8		 *output = NumberBuffer;
	Sint32		  places;
	Uint32		  placeValue;
	Sint32		  digit;
	Uint32		  magnitude = num;

	if(num < 0) {
		magnitude = 0U - magnitude;
		sign = '-';
	}
	/* A space for each power of ten above the number, then its sign, then a
	   digit for each power of ten left and the units. */
	placeValue = *placeValues++;
	places = DECIMAL_DIGITS_MAX - 1;
	if(magnitude < placeValue) {
		do {
			places--;
			*output++ = ' ';
			if(places == 0)
				break;
			placeValue = *placeValues++;
		} while(magnitude < placeValue);
	}
	*output++ = sign;
	placeValues--;
	while(places != 0) {
		placeValue = *placeValues++;
		digit = magnitude / placeValue;
		*output++ = digit + '0';
		magnitude -= digit * placeValue;
		places--;
	}
	output[0] = magnitude + '0';
	output[1] = 0;
}

/* Writes `cnt` blank characters (WINDOW_TILE_BLANK) at the debug text cursor
   and moves it on. */
void syPrintBlanks(Uint32 cnt)
{
	Uint16 *dst = DebugTextCursor;
	Uint32	idx;

	for(idx = 0; idx < cnt; idx++)
		*dst++ = WINDOW_TILE_BLANK;
	DebugTextCursor = dst;
}

#ifdef MODERN
/* Copies size bytes from source to destination, for the modern build. */
void *memcpy(void *destination, const void *source, unsigned long size)
{
	Uint8		*to = destination;
	const Uint8 *from = source;

	while(size-- != 0)
		*to++ = *from++;
	return destination;
}
#endif

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
					(Uint16 *)(DEBUG_TEXT_SCREEN + DEBUG_TEXT_SCREEN_BYTES))
					cursor = (Uint16 *)DEBUG_TEXT_SCREEN;
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
	syPrint(NumberBufferHexEnd - digits);
}

/* Prints the last `digits` characters of `value` in signed decimal (1 to
   DECIMAL_DIGITS_MAX; anything else prints DECIMAL_DIGITS_MAX). */
void syPrintDec(Sint32 value, Sint32 digits)
{
	if(digits < 1 || digits > DECIMAL_DIGITS_MAX)
		digits = DECIMAL_DIGITS_MAX;
	syStringFormatDecimal(value);
	syPrint(NumberBufferDecimalEnd - digits);
}

/* Blanks the debug text screen, homes the cursor and points BG0 at it. */
void syPrintClear(void)
{
	DMA_FILL32(WINDOW_TILE_PAIR(WINDOW_TILE_BLANK), DEBUG_TEXT_SCREEN,
		DEBUG_TEXT_SCREEN_BYTES);
	DebugTextCursor = (Uint16 *)DEBUG_TEXT_SCREEN;
	IO_WRITE16(BGCNT_SCREEN_BASE(4), &REG_BG0CNT);
}

/* Loads the window font and border tiles into BG character block 0, the window
   palette into WINDOW_PALETTE with its gradient colours, and the basic sprite
   palettes. */
void syWindowLoadGraphics(void)
{
	/* 2048 words: 8 KB, 256 tiles of 4 bits a pixel */
	syDMAXCopy(syResourceGetTableEntry(RESOURCE_WINDOW_TILES), BG_CHAR_BLOCK(0),
		DMA_COPY_WORDS(2048), REG_DMA3);
	syDMAXCopy(Window_Palette, (void *)&BG_PLTT_COLOR(WINDOW_PALETTE, 0),
		DMA_COPY_HALFWORDS(16), REG_DMA3);
	IO_WRITE16(0, &PLTT_BACKDROP);
	/* The window's background colour, three darker shades and three lighter. */
	IO_WRITE16(RGB(0, 12, 16),
		&BG_PLTT_COLOR(WINDOW_PALETTE, WINDOW_BACKGROUND_COLOR));
	IO_WRITE16(RGB(0, 11, 14),
		&BG_PLTT_COLOR(WINDOW_PALETTE, WINDOW_BACKGROUND_COLOR + 1));
	IO_WRITE16(RGB(0, 10, 12),
		&BG_PLTT_COLOR(WINDOW_PALETTE, WINDOW_BACKGROUND_COLOR + 2));
	IO_WRITE16(RGB(0, 9, 10),
		&BG_PLTT_COLOR(WINDOW_PALETTE, WINDOW_BACKGROUND_COLOR + 3));
	IO_WRITE16(RGB(0, 13, 18),
		&BG_PLTT_COLOR(WINDOW_PALETTE, WINDOW_BACKGROUND_COLOR + 4));
	IO_WRITE16(RGB(0, 14, 20),
		&BG_PLTT_COLOR(WINDOW_PALETTE, WINDOW_BACKGROUND_COLOR + 5));
	IO_WRITE16(RGB(0, 15, 22),
		&BG_PLTT_COLOR(WINDOW_PALETTE, WINDOW_BACKGROUND_COLOR + 6));
	syDMAXCopy(BasicSpritePalettes, (void *)OBJ_PLTT,
		DMA_COPY_HALFWORDS(BASIC_SPRITE_COLORS), REG_DMA3);
}

/* Loads the basic sprite palettes (palettes 0 to 13). */
void syPaletteLoadBasicSprCols(void)
{
	syDMAXCopy(BasicSpritePalettes, (void *)OBJ_PLTT,
		DMA_COPY_HALFWORDS(BASIC_SPRITE_COLORS), REG_DMA3);
}

#undef HEX_DIGITS
#undef DECIMAL_DIGITS_MAX
#undef DEBUG_TEXT_MAX_LENGTH
#undef BASIC_SPRITE_COLORS
#undef DEBUG_TEXT_SCREEN
#undef DEBUG_TEXT_SCREEN_BYTES
