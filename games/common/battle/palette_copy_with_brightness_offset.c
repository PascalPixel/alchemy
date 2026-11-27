/* btPaletteCopyWithBrightnessOffset, the same in both games. Both games'
   battle/battle_turn.c include it where the function stands. */

/* Copies WINDOW_PALETTE into HIGHLIGHT_PALETTE and BASIC_COLORS_PALETTE into
   the palette before it (BG palettes 15 and 5 into 14 and 4), with offset added
   to each colour channel, held within 0 to RGB_LEVEL_MAX. */
void btPaletteCopyWithBrightnessOffset(Sint32 ofs)
{
	Sint32 bank;
	Sint32 source = WINDOW_PALETTE;
	Sint32 i;

	for(bank = 0; bank < 2; bank++) {
		for(i = 0; i < PALETTE_COLORS; i++) {
			Sint32 idx = source * PALETTE_COLORS + i;
			Uint32 col = ((Uint16 *)BG_PLTT)[idx];
			Sint32 blue = RGB_BLUE(col);
			Sint32 green = RGB_GREEN(col);
			Sint32 red = RGB_RED(col);
			Sint32 adjusted;

			blue += ofs;
			green += ofs;
			red += ofs;
			if(blue > RGB_LEVEL_MAX)
				blue = RGB_LEVEL_MAX;
			if(green > RGB_LEVEL_MAX)
				green = RGB_LEVEL_MAX;
			if(red > RGB_LEVEL_MAX)
				red = RGB_LEVEL_MAX;
			if(blue < 0)
				blue = 0;
			if(green < 0)
				green = 0;
			if(red < 0)
				red = 0;
			adjusted = RGB(red, green, blue);
			((Uint16 *)BG_PLTT)[idx - PALETTE_COLORS] = adjusted;
		}
		source = BASIC_COLORS_PALETTE;
	}
}
