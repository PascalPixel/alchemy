/* Mercury Lighthouse and its entrance: the shimmer of the pool once it is woken
   (FLAG_MERCURY_LIGHTHOUSE_HEART_HONORED). */

/* The background palette, and its colours, the shimmer runs through. */
#define SHIMMER_PALETTE		  6
#define SHIMMER_FIRST_COLOR	  9
#define SHIMMER_LAST_COLOR	  15
/* The top colours, which take the red level below them dimmed. */
#define SHIMMER_DIMMED_COLORS 3

/* An event: every fifth tick SHIMMER_FIRST_COLOR's red level steps on by 4,
   and each colour above it takes the red level of the colour below it (the top
   SHIMMER_DIMMED_COLORS dimmed by four tenths), so the shimmer runs up the
   ramp; green and blue stay those of PaletteCycleGreen and PaletteCycleBlue. */
static void scMercuryLighthouseCyclePalette(void)
{
	Uint16			 below;
	Uint32			 i;
	Sint32			 red;
	Sint32			 greenAndBlue;
	volatile Uint16 *color;

	/* below is a halfword in the tick (its address taken, plain by ruling);
	   the ROM clears it on entry, a store nothing reads. */
	*&below = 0;
	if(TickCount % 5 != 0)
		return;
	PaletteCycleRed = (PaletteCycleRed + 4) & RGB_LEVEL_MAX;
	for(i = 0; i < SHIMMER_LAST_COLOR - SHIMMER_FIRST_COLOR; i++) {
		below =
			RGB_RED(BG_PLTT_COLOR(SHIMMER_PALETTE, SHIMMER_LAST_COLOR - 1 - i));
		red = below;
		if(i < SHIMMER_DIMMED_COLORS)
			red -= red * 4 / 10;
		color = &BG_PLTT_COLOR(SHIMMER_PALETTE, SHIMMER_LAST_COLOR - i);
		greenAndBlue = RGB(0, PaletteCycleGreen, PaletteCycleBlue);
		*color = red | greenAndBlue;
	}
	BG_PLTT_COLOR(SHIMMER_PALETTE, SHIMMER_FIRST_COLOR) =
		PaletteCycleRed | greenAndBlue;
}

/* While the pool sleeps: the shimmer's colours are black. */
static void scMercuryLighthouseClearPalette(void)
{
	Uint32 i;

	for(i = 0; i <= SHIMMER_LAST_COLOR - SHIMMER_FIRST_COLOR; i++)
		BG_PLTT_COLOR(SHIMMER_PALETTE, SHIMMER_LAST_COLOR - i) = 0;
}
