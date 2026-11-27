/* The palette fades: a fade between two built palettes over a number of ticks,
   and the palettes it builds from the shown one. Part of field_mode.c. */

/* The palette fade works on 224 BG colours and then 224 OBJ colours, each as B,
   G, R with every channel moved to where blue is (BLUE_BITS, bits 14..10). */
#define FADE_HALF_COLORS 224
#define FADE_COLORS		 (FADE_HALF_COLORS * 2)
#define FADE_CHANNELS	 (FADE_COLORS * 3)

/* Sets each colour channel of step to the amount to add each tick for source
   to reach target in ticks ticks; with ticks 0 or less step is left as it
   was. */
void fdPaletteFadeCalcSteps(
	Sint16 *src, Sint16 *tgt, Sint16 *step, Sint32 ticks)
{
	Sint32 i;
	Sint32 srcChannel;
	Sint32 tgtChannel;

	if(ticks > 0) {
		for(i = 0; i < FADE_CHANNELS; i++) {
			srcChannel = *src;
			tgtChannel = *tgt;
			*step = syIntegerDivide(tgtChannel - srcChannel, ticks);
			src++;
			tgt++;
			step++;
		}
	}
}

/* Each tick (unless FLAG_PALETTE_BLEND_HELD is set): moves the palette fade
   one step toward its target, packs it into RGB555 and queues it for the BG and
   OBJ palettes. */
void fdPaletteFadeUpdate(void)
{
	PALETTE_FADE *state = Ram_HeapSlots->palFade;
	Uint16		 *channelDelta = state->delta;
	Sint32		  i;

	if(syGameFlagGet(FLAG_PALETTE_BLEND_HELD) != 0) {
		return;
	}
	if(state->duration == 0) {
		return;
	}
	/* On the last tick the target itself is copied, so rounding in the steps
	   never leaves the colours short. */
	if(++state->step < state->duration) {
		Uint16 *curChannel = state->cur;
		for(i = 0; i < FADE_CHANNELS; i++) {
			*curChannel++ += *channelDelta++;
		}
	} else {
		syWordsCopy(state->cur, state->tgt, sizeof(state->cur));
		state->duration = 0;
	}

	/* Packs the channels into the page not shown, then shows it. */
	{
		Uint16 *packed = state->packed[1 ^ state->page];
		Sint32	blueMask = BLUE_BITS;
		Uint16 *cur;

		i = FADE_COLORS;
		cur = state->cur;
		for(; i != 0; i--) {
			*packed++ = (cur[0] & blueMask) |
				(((Sint16)cur[1] >> (RGB_BLUE_SHIFT - RGB_GREEN_SHIFT)) &
					GREEN_BITS) |
				(((Sint16)cur[2] >> RGB_BLUE_SHIFT) & RED_BITS);
			cur += 3;
		}
	}

	state->page ^= 1;
	{
		Uint16 *packed = state->packed[state->page];

		IO_WRITE_QUEUE_PUSH((Uint32)BG_PLTT, (Uint32)packed,
			DMA_COPY_WORDS(FADE_HALF_COLORS / 2));
		IO_WRITE_QUEUE_PUSH((Uint32)OBJ_PLTT,
			(Uint32)(packed + FADE_HALF_COLORS),
			DMA_COPY_WORDS(FADE_HALF_COLORS / 2));
	}
}

/* Builds colours for the palette fade from the palette src into dst, which
   holds each colour as B, G and R channels, each where RGB555 keeps blue. mode
   below PALETTE_TINT_FROM_SOURCE is one colour for all;
   PALETTE_TINT_FROM_SOURCE the backdrop colour; the other PALETTE_TINT_* values
   tint each colour; any other value is the address of a ready buffer to copy.
   half is the palette selection: PALETTES_ALL for every colour, PALETTES_BG for
   the BG's, PALETTES_OBJECTS for the OBJ's. */
void fdPaletteFadeBuild(Uint32 mode, Uint16 *src, Uint16 *dst, Sint32 half)
{
	Uint32 i;
	Uint32 colCnt = FADE_COLORS;
	Sint32 red;
	Sint32 blue;
	Uint32 col;
	Sint32 green;

	if(mode == PALETTE_TINT_FROM_SOURCE)
		mode = *(Uint16 *)BG_PLTT;
	if(half == PALETTES_BG) {
		colCnt = FADE_HALF_COLORS;
	} else if(half == PALETTES_OBJECTS) {
		dst += FADE_HALF_COLORS * 3;
		src += FADE_HALF_COLORS;
		colCnt = FADE_HALF_COLORS;
	}

	/* One colour: write it once, then let DMA copy it over the rest. The copy
	   reads three halfwords behind where it writes, so the colour repeats. */
	if(mode < PALETTE_TINT_FROM_SOURCE) {
		*dst++ = mode & BLUE_BITS;
		*dst++ = (mode & GREEN_BITS) << (RGB_BLUE_SHIFT - RGB_GREEN_SHIFT);
		*dst++ = (mode & RED_BITS) << RGB_BLUE_SHIFT;
		syDMAXCopy(dst - 3, dst, DMA_COPY_HALFWORDS(((colCnt - 1) * 6) >> 1),
			REG_DMA3);
	} else if(mode < PALETTE_TINT_FLAGS) {
		switch(mode) {
		case PALETTE_TINT_GRAYSCALE: /* grey of brightness (2R + 4G + B) / 7 */
			for(i = 0; i < colCnt; i++) {
				col = *src++;
				red = ((col << BRIGHTNESS_RED_SHIFT) &
					(RED_BITS << BRIGHTNESS_RED_SHIFT));
				green = ((col << BRIGHTNESS_GREEN_SHIFT) &
					(GREEN_BITS << BRIGHTNESS_GREEN_SHIFT));
				blue = (col & BLUE_BITS);
				col = syIntegerDivide(red + green + blue, 7);
				dst[i * 3 + 0] = col;
				dst[i * 3 + 1] = col;
				dst[i * 3 + 2] = col;
			}
			break;
		/* a third of R + G + B, B three steps darker */
		case PALETTE_TINT_DIM_GRAY:
			for(i = 0; i < colCnt; i++) {
				col = *src++;
				red = RGB_RED(col);
				col = syIntegerDivide(red + RGB_GREEN(col) + RGB_BLUE(col), 3);
				red = col - 1;
				green = red;
				red = fdPaletteFadeClampComponent(red);
				green = fdPaletteFadeClampComponent(green);
				blue = col - 3;
				blue = fdPaletteFadeClampComponent(blue);
				dst[0] = FieldEffectChannelCurveLight[blue];
				dst[1] = FieldEffectChannelCurveLight[green];
				dst[2] = FieldEffectChannelCurveLight[red];
				dst += 3;
			}
			break;
		case PALETTE_TINT_SEPIA: /* the brightness, red stronger */
			for(i = 0; i < colCnt; i++) {
				col = *src++;
				red = RGB_RED(col);
				green = RGB_GREEN(col);
				blue = RGB_BLUE(col);
				col = syIntegerDivide(red + green + blue, SEPIA_SHADE_DIVISOR);
				red = col * 4 + 5;
				blue = col * 3 + 5;
				green = blue;
				if(red < SEPIA_LEVEL_MIN)
					red = SEPIA_LEVEL_MIN;
				if(green < SEPIA_LEVEL_MIN)
					green = SEPIA_LEVEL_MIN;
				if(blue < SEPIA_LEVEL_MIN)
					blue = SEPIA_LEVEL_MIN;
				if(red > SEPIA_LEVEL_MAX)
					red = SEPIA_LEVEL_MAX;
				if(green > SEPIA_LEVEL_MAX)
					green = SEPIA_LEVEL_MAX;
				if(blue > SEPIA_LEVEL_MAX)
					blue = SEPIA_LEVEL_MAX;
				*dst++ = FieldEffectChannelCurveLight[blue];
				*dst++ = FieldEffectChannelCurveLight[green];
				*dst++ = FieldEffectChannelCurveLight[red];
			}
			break;
		/* half R + 10, two thirds G + 8, B - 7 (black stays black) */
		case PALETTE_TINT_WARM:
			for(i = 0; i < colCnt; i++) {
				col = *src++;
				red = RGB_RED(col);
				green = RGB_GREEN(col);
				blue = RGB_BLUE(col);
				if((red | green | blue) != 0) {
					red -= (Uint32)red >> 1;
					green -= green / 3;
					red += 10;
					green += 8;
					blue -= 7;
					red = fdPaletteFadeClampComponent(red);
					green = fdPaletteFadeClampComponent(green);
					blue = fdPaletteFadeClampComponent(blue);
				}
				*dst++ = FieldEffectChannelCurveContrast[blue];
				*dst++ = FieldEffectChannelCurveLight[green];
				*dst++ = FieldEffectChannelCurve[red];
			}
			break;
		case PALETTE_TINT_MUTED: /* narrow the range, G and B + 2 */
			for(i = 0; i < colCnt; i++) {
				col = *src++;
				red = RGB_RED(col);
				green = RGB_GREEN(col);
				blue = RGB_BLUE(col);
				/* The first colour stays as it is. */
				if(i != 0) {
					if(red < 8)
						red = 8;
					if(green < 14)
						green = 14;
					if(blue < 14)
						blue = 14;
					if(red > 29)
						red = 29;
					if(green > 25)
						green = 25;
					if(blue > 27)
						blue = 27;
					green += 2;
					blue += 2;
					red = fdPaletteFadeClampComponent(red);
					green = fdPaletteFadeClampComponent(green);
					blue = fdPaletteFadeClampComponent(blue);
				}
				*dst++ = FieldEffectChannelCurve[blue];
				*dst++ = FieldEffectChannelCurve[green];
				*dst++ = FieldEffectChannelCurve[red];
			}
			break;
		case PALETTE_TINT_PALE: /* half of each, plus the mean */
			for(i = 0; i < colCnt; i++) {
				Sint32 mean;

				col = *src++;
				red = RGB_RED(col);
				green = RGB_GREEN(col);
				blue = RGB_BLUE(col);
				mean = fdPaletteFadeClampComponent((red + green + blue) / 3);
				red = (red >> 1) + mean;
				green = (green >> 1) + mean;
				blue = (blue >> 1) + mean;
				red = fdPaletteFadeClampComponent(red);
				green = fdPaletteFadeClampComponent(green);
				blue = fdPaletteFadeClampComponent(blue);
				dst[0] = FieldEffectChannelCurveContrast[blue];
				dst[1] = FieldEffectChannelCurveContrast[green];
				dst[2] = FieldEffectChannelCurveContrast[red];
				dst += 3;
			}
			break;
		/* an eighth of G and B added to R; G and B two thirds */
		case PALETTE_TINT_RED:
			for(i = 0; i < colCnt; i++) {
				col = *src++;
				red = RGB_RED(col);
				green = RGB_GREEN(col);
				blue = RGB_BLUE(col);
				red += ((green >> 3) + (blue >> 3));
				red = fdPaletteFadeClampComponent(red);
				green -= green / 3;
				blue -= blue / 3;
				dst[0] = FieldEffectChannelCurve[blue];
				dst[1] = FieldEffectChannelCurve[green];
				dst[2] = FieldEffectChannelCurveLight[red];
				dst += 3;
			}
			break;
		case PALETTE_TINT_WARM_DARK: /* half R + 6, two thirds G + 4, B - 6 */
			for(i = 0; i < colCnt; i++) {
				col = *src++;
				red = RGB_RED(col);
				red -= (Uint32)red >> 1;
				green = RGB_GREEN(col);
				blue = RGB_BLUE(col);
				green -= green / 3;
				red += 6;
				green += 4;
				blue -= 6;
				red = fdPaletteFadeClampComponent(red);
				green = fdPaletteFadeClampComponent(green);
				blue = fdPaletteFadeClampComponent(blue);
				dst[0] = FieldEffectChannelCurveContrast[blue];
				dst[1] = FieldEffectChannelCurveLight[green];
				dst[2] = FieldEffectChannelCurve[red];
				dst += 3;
			}
			break;
		/* a fifth off each channel of a bright colour */
		case PALETTE_TINT_DIM_BRIGHT:
			for(i = 0; i < colCnt; i++) {
				col = *src++;
				red = RGB_RED(col);
				green = RGB_GREEN(col);
				blue = RGB_BLUE(col);
				if(red + green + blue > 60) {
					red -= red / 5;
					green -= green / 5;
					blue -= blue / 5;
				}
				red = fdPaletteFadeClampComponent(red);
				green = fdPaletteFadeClampComponent(green);
				blue = fdPaletteFadeClampComponent(blue);
				*dst++ = FieldEffectChannelCurve[blue];
				*dst++ = FieldEffectChannelCurve[green];
				*dst++ = FieldEffectChannelCurve[red];
			}
			break;
		default: /* unchanged */
			for(i = 0; i < colCnt; i++) {
				col = *src++;
				dst[i * 3 + 0] = col & BLUE_BITS;
				dst[i * 3 + 1] = (col & GREEN_BITS)
					<< (RGB_BLUE_SHIFT - RGB_GREEN_SHIFT);
				dst[i * 3 + 2] = (col & RED_BITS) << RGB_BLUE_SHIFT;
			}
			break;
		}
	} else if(mode & PALETTE_TINT_CUSTOM) {
		/* The brightness (2R + 4G + B) times the colour in mode. */
		Uint32 tintRed;
		Uint32 tintGreen;
		Uint32 tintBlue;
		Sint32 red;
		Sint32 green;

		tintRed = mode;
		tintRed &= RGB_LEVEL_MAX;
		tintGreen = RGB_GREEN(mode);
		tintBlue = mode >> RGB_BLUE_SHIFT;
		tintBlue &= RGB_LEVEL_MAX;
		for(i = 0; i < colCnt; i++) {
			col = *src++;
			red = ((col << BRIGHTNESS_RED_SHIFT) &
				(RED_BITS << BRIGHTNESS_RED_SHIFT));
			green = ((col << BRIGHTNESS_GREEN_SHIFT) &
				(GREEN_BITS << BRIGHTNESS_GREEN_SHIFT));
			blue = (col & BLUE_BITS);
			col = syIntegerDivide(red + green + blue, CUSTOM_TINT_DIVISOR);
			red = col * tintRed;
			green = col * tintGreen;
			blue = col * tintBlue;
			red = fdPaletteFadeClampShiftedComponent(red);
			green = fdPaletteFadeClampShiftedComponent(green);
			blue = fdPaletteFadeClampShiftedComponent(blue);
			dst[i * 3 + 0] = blue;
			dst[i * 3 + 1] = green;
			dst[i * 3 + 2] = red;
		}
	} else if(mode & PALETTE_TINT_BLEND) {
		/* The brightness R + G + B shared out in the proportions of mode's
		   colour. */
		Uint32 tintRed;
		Uint32 tintGreen;
		Uint32 tintBlue;
		FIXED  blendedRed;
		FIXED  blendedGreen;
		FIXED  blendedBlue;
		Sint32 clampedRed;
		Sint32 clampedGreen;
		Sint32 clampedBlue;

		tintRed = mode;
		tintRed &= RGB_LEVEL_MAX;
		tintGreen = RGB_GREEN(mode);
		tintBlue = mode >> RGB_BLUE_SHIFT;
		tintBlue &= RGB_LEVEL_MAX;
		for(i = 0; i < colCnt; i++) {
			col = *src++;
			red = RGB_RED(col);
			col = syIntegerDivide((red + RGB_GREEN(col) + RGB_BLUE(col)) << 4,
				tintRed + tintGreen + tintBlue);
			blendedRed = syMulFX(
				toFIXED((col * tintRed) >> 4), (Sint32)toFIXED(tintRed) >> 4);
			blendedGreen = syMulFX(toFIXED((col * tintGreen) >> 4),
				(Sint32)toFIXED(tintGreen) >> 4);
			blendedBlue = syMulFX(
				toFIXED((col * tintBlue) >> 4), (Sint32)toFIXED(tintBlue) >> 4);
			clampedRed =
				fdPaletteFadeClampComponent(FIXEDtoINT((Uint32)blendedRed));
			clampedGreen =
				fdPaletteFadeClampComponent(FIXEDtoINT((Uint32)blendedGreen));
			clampedBlue =
				fdPaletteFadeClampComponent(FIXEDtoINT((Uint32)blendedBlue));
			dst[i * 3 + 0] = FieldEffectChannelCurve[clampedBlue];
			dst[i * 3 + 1] = FieldEffectChannelCurve[clampedGreen];
			dst[i * 3 + 2] = FieldEffectChannelCurve[clampedRed];
		}
	} else if(mode & PALETTE_TINT_SPLIT) {
		/* The colours unchanged, as the default tint writes them. */
		for(i = 0; i < colCnt; i++) {
			col = *src++;
			dst[i * 3 + 0] = col & BLUE_BITS;
			dst[i * 3 + 1] = (col & GREEN_BITS)
				<< (RGB_BLUE_SHIFT - RGB_GREEN_SHIFT);
			dst[i * 3 + 2] = (col & RED_BITS) << RGB_BLUE_SHIFT;
		}
	} else {
		/* Copy a ready buffer: mode is its address, and the OBJ half starts
		   after the BG's colours of three halfwords. */
		if(half == PALETTES_OBJECTS)
			mode += FADE_HALF_COLORS * 3 * sizeof(Uint16);
		syDMAXCopy((const void *)mode, dst, DMA_COPY_WORDS((colCnt * 6) >> 2),
			REG_DMA3);
	}
}

/* Sets up the palette fade (HEAP_SLOT_PALETTE_FADE) from the BG and OBJ
   palettes and starts updating the palettes from it each tick. */
void fdPaletteFadeInit(void)
{
	PALETTE_FADE *state;

	state = syHeapSlotAllocEWRAM(HEAP_SLOT_PALETTE_FADE, sizeof(*state));
	DMA_FILL32(0, state, sizeof(*state));
	syDMAXCopy((void *)BG_PLTT, &state->pal[0],
		DMA_COPY_WORDS(FADE_HALF_COLORS / 2), REG_DMA3);
	syDMAXCopy((void *)OBJ_PLTT, &state->pal[FADE_HALF_COLORS],
		DMA_COPY_WORDS(FADE_HALF_COLORS / 2), REG_DMA3);
	fdPaletteFadeBuild(PALETTE_TINT_NONE, state->pal, state->tgt, PALETTES_ALL);
	sySetEvent(fdPaletteFadeUpdate, EVENT_PASS_TICK + 15);
}

/* Stops the palette fade and frees it. */
void fdPaletteFadeRelease(void)
{
	syCloseEvent(&fdPaletteFadeUpdate);
	syHeapSlotRelease(HEAP_SLOT_PALETTE_FADE);
}

/* Sets the colours the palette fade moves toward (see fdPaletteFadeBuild); does
   nothing without a palette fade. */
void fdPaletteFadeSetTarget(Sint32 mode, Sint32 half)
{
	PALETTE_FADE *state = Ram_HeapSlots->palFade;

	if(state != NULL) {
		fdPaletteFadeBuild(mode, state->pal, state->tgt, half);
	}
}

/* Sets the colours the palette fade shows now (see fdPaletteFadeBuild). */
void fdPaletteFadeSetSource(Sint32 mode, Sint32 half)
{
	PALETTE_FADE *state = Ram_HeapSlots->palFade;

	if(state != NULL) {
		fdPaletteFadeBuild(mode, state->pal, state->cur, half);
	}
}

/* Sets the backdrop colour (BG colour 0) of the palette fade's copy of the
   palettes, from which it builds its colours. */
void fdPaletteFadeSetBackdrop(Uint32 col)
{
	PALETTE_FADE *state = Ram_HeapSlots->palFade;

	if(state != NULL) {
		state->pal[0] = col;
	}
}

/* Packs the target colours, three channels each, back into the palette copy as
   RGB555. */
void fdPaletteFadePackTarget(void)
{
	PALETTE_FADE *state = Ram_HeapSlots->palFade;
	Uint16		 *packed = state->pal;
	Sint32		  blueMask = BLUE_BITS;
	Uint16		 *cur;
	Sint32		  i;

	i = FADE_COLORS;
	cur = state->tgt;
	for(; i != 0; i--) {
		*packed++ = (cur[0] & blueMask) |
			((cur[1] >> (RGB_BLUE_SHIFT - RGB_GREEN_SHIFT)) & GREEN_BITS) |
			((cur[2] >> RGB_BLUE_SHIFT) & RED_BITS);
		cur += 3;
	}
	/* FAKEMATCH(dead): a decrement of state, which nothing reads after it. */
	state--;
}

/* Starts the fade from the shown colours to the target colours, over ticks
   ticks. */
void fdPaletteFadeStartInterpolation(Sint32 ticks)
{
	PALETTE_FADE *state = Ram_HeapSlots->palFade;

	if(state != NULL) {
		state->duration = ticks;
		state->step = 0;
		fdPaletteFadeCalcSteps(state->cur, state->tgt, state->delta, ticks);
	}
}

/* Takes colors as the BG colours of the palette copy and makes them, unchanged,
   the BG colours' target. */
void fdPaletteFadeLoadPalette(const Uint16 *cols)
{
	PALETTE_FADE *state = Ram_HeapSlots->palFade;

	if(state != NULL) {
		Sint32 (*copy)(void *destination, const void *source, Sint32 size) =
			syWordsCopyEntry;

		copy(state->pal, cols, FADE_HALF_COLORS * sizeof(Uint16));
		fdPaletteFadeBuild(
			PALETTE_TINT_NONE, state->pal, state->tgt, PALETTES_BG);
	}
}

/* Clamps a colour channel to 0 to RGB_LEVEL_MAX. */
Sint32 fdPaletteFadeClampComponent(Sint32 value)
{
	if(value > RGB_LEVEL_MAX)
		return RGB_LEVEL_MAX;
	if(value < 0)
		value = 0;
	return value;
}

/* Clamps a colour channel held where blue is to at most RGB_LEVEL_MAX there. */
Sint32 fdPaletteFadeClampShiftedComponent(Sint32 value)
{
	if(value > RGB_LEVEL_MAX << RGB_BLUE_SHIFT)
		value = RGB_LEVEL_MAX << RGB_BLUE_SHIFT;
	return value;
}

#undef FADE_HALF_COLORS
#undef FADE_COLORS
#undef FADE_CHANNELS
