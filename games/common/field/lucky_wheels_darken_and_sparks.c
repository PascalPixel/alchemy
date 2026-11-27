/* fdLuckyWheelsDarkenStep and fdLuckyWheelsStepSparkCols, the same in both
   games. Both games' field/lucky_wheels.c include it where the functions
   stand. */

/* Darkens the screen one step: lowers each channel of the machine's 16 colours
   and of the icons' by 1. */
void fdLuckyWheelsDarkenStep(void)
{
	Uint16 *col;
	Sint32	i;

	col = (Uint16 *)&BG_PLTT_COLOR(LUCKY_WHEELS_MACHINE_PALETTE, 0);
	for(i = 0; i != PALETTE_COLORS; i++) {
		Sint32 b = RGB_BLUE(*col);
		Sint32 g = RGB_GREEN(*col);
		Sint32 r = RGB_RED(*col);

		b--;
		g--;
		r--;

		if(b < 0)
			b = 0;
		if(g < 0)
			g = 0;
		if(r < 0)
			r = 0;
		*col++ = RGB(r, g, b);
	}
	col = (Uint16 *)&OBJ_PLTT[1];
	for(i = 0; i != LUCKY_WHEELS_ICON_COLORS - 1; i++) {
		Sint32 b = RGB_BLUE(*col);
		Sint32 g = RGB_GREEN(*col);
		Sint32 r = RGB_RED(*col);

		b--;
		g--;
		r--;

		if(b < 0)
			b = 0;
		if(g < 0)
			g = 0;
		if(r < 0)
			r = 0;
		*col++ = RGB(r, g, b);
	}
}

/* Moves the sparks' colours one step toward the palette of resource; the
   backdrop colour is not written back. */
void fdLuckyWheelsStepSparkCols(Sint32 resource)
{
	Uint16	dst[LUCKY_WHEELS_SPARK_COLORS];
	Uint16 *col = (Uint16 *)BG_PLTT;
	Sint32	i;

	syDMAXCopy(syResourceGetTableEntry(resource), dst,
		DMA_COPY_WORDS(sizeof dst / 4), REG_DMA3);
	for(i = 0; i != LUCKY_WHEELS_SPARK_COLORS; i++, col++) {
		Sint32 r = RGB_RED(*col);
		Sint32 g = RGB_GREEN(*col);
		Sint32 b = RGB_BLUE(*col);
		Sint32 targetRed = RGB_RED(dst[i]);
		Sint32 targetGreen = RGB_GREEN(dst[i]);
		Sint32 targetBlue = RGB_BLUE(dst[i]);

		if(r < targetRed)
			r++;
		else if(r > targetRed)
			r--;
		if(g < targetGreen)
			g++;
		else if(g > targetGreen)
			g--;
		if(b < targetBlue)
			b++;
		else if(b > targetBlue)
			b--;
		dst[i] = RGB(r, g, b);
	}
	syDMAXCopy(dst + 1, (void *)&BG_PLTT[1],
		DMA_COPY_HALFWORDS(LUCKY_WHEELS_SPARK_COLORS - 1), REG_DMA3);
}
