/* The fade of the Lucky Dice and Lucky Wheels screens, which darken and
   brighten by scaling their palettes; each game's object includes it. */

/* Copies count RGB555 colours from source to destination with each channel
   multiplied by scale (16.16: 0 is black, toFIXED(1) unchanged), for fades. */
static Sint32 fdPaletteScaleRGB555(
	Uint16 *src, Uint16 *dst, FIXED scale, Sint32 cnt)
{
	Sint32 remaining;
	Uint32 col;
	Uint32 red;
	Uint32 green;
	Uint32 blue;

	if(cnt > 0) {
		Uint32 redMask = RGB(31, 0, 0);
		Uint32 greenMask = RGB(0, 31, 0);
		Uint32 blueMask = RGB(0, 0, 31);

		remaining = cnt;
		do {
			col = *src;
			red = col & redMask;
			green = col & greenMask;
			blue = blueMask & col;
			red *= scale;
			green *= scale;
			blue *= scale;
			col = (FIXEDtoINT(red) & redMask) | (FIXEDtoINT(green) & greenMask);
			col |= FIXEDtoINT(blue) & blueMask;
			*dst = col;
			src++;
			dst++;
			remaining--;
		} while(remaining != 0);
	}
	return 0;
}
