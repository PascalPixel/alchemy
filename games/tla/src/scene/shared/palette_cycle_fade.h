/* Steps a palette cycle that fades as it slows. */
void fdPaletteCycleFade(void)
{
	Sint32			 step;
	Uint32			 i;
	Uint16			*src;
	volatile Uint16 *dest;
	int				 red, green, blue;

	step = 0;
	if(PaletteCycleTimer != 0) {
		if(--PaletteCycleTimer == 0) {
			step = 1;
			PaletteCycleTimer = PaletteCycleDelay;
			if(PaletteCycleDelay > 2)
				PaletteCycleDelay--;
		}
	}
	if(step == 0)
		return;
	src = PaletteCycleColors;
	for(i = 0; i < PaletteCycleLength; i++) {
		dest =
			&BG_PLTT[PaletteCycleSlots[PaletteCycleIndex * PaletteCycleLength +
				i]];
		red = *src++;
		green = *src++;
		blue = *src++;
		SET_RGB(*dest, red, green, blue);
	}
	dest = &BG_PLTT[PaletteSlots[PaletteCycleLength]];
	if(PaletteCycleBright != 0)
		src = PaletteColors + PaletteCycleLength * 3;
	else
		src = PaletteCycleColors + PaletteCycleLength * 3;
	red = *src++;
	green = *src++;
	blue = *src++;
	SET_RGB(*dest, red, green, blue);
	PaletteCycleBright ^= 1;
	if(++PaletteCycleIndex == PaletteCycleLength)
		PaletteCycleIndex = 0;
}
