/* The palette fade: palettes moved toward target colours. Part of
   field_mode.c. */

#include "field_mode_internal.h"

/* The colours a palette fade moves: the BG's 224, then the OBJ's 224. */
#define COLOR_HALF			224
#define COLOR_MAX			(2 * COLOR_HALF)
/* Palette fade state holds each channel of a colour in bits 14..10 of a
   halfword, where RGB555 keeps blue: green's bits move up BLUE_SHIFT -
   GREEN_SHIFT to get there, red's BLUE_SHIFT. */
#define BLUE_CHANNEL(color) ((color) & BLUE_BITS)
#define GREEN_CHANNEL(color) \
	(((color) & GREEN_BITS) << (BLUE_SHIFT - GREEN_SHIFT))
#define RED_CHANNEL(color) (((color) & RED_BITS) << BLUE_SHIFT)
/* For the brightness 2R + 4G + B: red's level doubled and green's quadrupled,
   each in the channel's place. */
#define RED_CHANNEL_DOUBLED(color) \
	(((color) << BRIGHTNESS_RED_SHIFT) & (RED_BITS << BRIGHTNESS_RED_SHIFT))
#define GREEN_CHANNEL_QUADRUPLED(color) \
	(((color) << BRIGHTNESS_GREEN_SHIFT) & \
		(GREEN_BITS << BRIGHTNESS_GREEN_SHIFT))

/* Sets each channel of step (three for each of the 448 colours) to the amount
   to add each tick for source to reach target in ticks ticks; with ticks 0
   or less step is left as it was. */
void fdPaletteFadeCalcSteps(
	Sint16 *src, Sint16 *tgt, Sint16 *step, Sint32 ticks)
{
	Sint32 i;
	Sint32 srcChannel;
	Sint32 tgtChannel;

	if(ticks > 0) {
		for(i = 0; i < PALETTE_FADE_CHANNELS; i++) {
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
	PALETTE_FADE *state = PaletteFadeState;
	Uint16		 *channelDelta = state->delta;
	Sint32		  i;

	if(gmFlagGetStub(FLAG_PALETTE_BLEND_HELD) != 0) {
		return;
	}
	if(state->duration == 0) {
		return;
	}
	/* Accumulate modular 16-bit channel steps; on the last tick copy the
	   target itself, so rounding in the steps never leaves the colours
	   short. */
	if(++state->step < state->duration) {
		Uint16 *curChannel = state->cur;
		for(i = 0; i < PALETTE_FADE_CHANNELS; i++) {
			*curChannel++ += *channelDelta++;
		}
	} else {
		syWordsCopy(state->cur, state->tgt, sizeof(state->cur));
		state->duration = 0;
	}

	{
		/* Pack the channels into the hidden page as RGB555: each channel's bits
		   14..10 become its colour's level. */
		Uint16 *packed = state->packed[1 ^ state->page];
		Sint32	blueMask = BLUE_BITS;
		Uint16 *cur;

		i = COLOR_MAX;
		cur = state->cur;
		for(; i != 0; i--) {
			*packed++ = (cur[0] & blueMask) |
				(((Sint16)cur[1] >> (BLUE_SHIFT - GREEN_SHIFT)) & GREEN_BITS) |
				(((Sint16)cur[2] >> BLUE_SHIFT) & RED_BITS);
			cur += 3;
		}
	}

	/* Show the page just written: queue its BG half and OBJ half (224 colours,
	   112 words each) for the palettes. */
	state->page ^= 1;
	{
		Uint16 *packed = state->packed[state->page];

		IO_WRITE_QUEUE_PUSH(
			(Uint32)BG_PLTT, (Uint32)packed, DMA_COPY_WORDS(COLOR_HALF / 2));
		IO_WRITE_QUEUE_PUSH((Uint32)OBJ_PLTT, (Uint32)(packed + COLOR_HALF),
			DMA_COPY_WORDS(COLOR_HALF / 2));
	}
}

/* Builds palette fade colours from the palette src into dst, three channels a
   colour. mode is one RGB555 colour, below PALETTE_TINT_FROM_SOURCE, for every
   colour alike; PALETTE_TINT_FROM_SOURCE for the backdrop colour alike; a
   PALETTE_TINT_* value to tint each colour; or the address of ready channels to
   copy. half is the palette selection: PALETTES_ALL for all 448 colours,
   PALETTES_BG for the BG's 224, PALETTES_SPRITES for the OBJ's 224. */
void fdPaletteFadeBuild(Uint32 mode, Uint16 *src, Uint16 *dst, Sint32 half)
{
	Uint32 i;
	Uint32 colCnt = COLOR_MAX;
	Sint32 red;
	Sint32 blue;
	Uint32 col;
	Sint32 green;

	if(mode == PALETTE_TINT_FROM_SOURCE)
		mode = *(Uint16 *)BG_PLTT;
	if(half == PALETTES_BG) {
		colCnt = COLOR_HALF;
	} else if(half == PALETTES_SPRITES) {
		dst += COLOR_HALF * 3;
		src += COLOR_HALF;
		colCnt = COLOR_HALF;
	}

	/* One colour: write it once, then let DMA copy it over the rest. */
	if(mode < PALETTE_TINT_FROM_SOURCE) {
		*dst++ = BLUE_CHANNEL(mode);
		*dst++ = GREEN_CHANNEL(mode);
		*dst++ = RED_CHANNEL(mode);
		syDMAXCopy(dst - 3, dst, DMA_COPY_HALFWORDS(((colCnt - 1) * 6) >> 1),
			REG_DMA3);
	} else if(mode < PALETTE_TINT_FLAGS) {
		switch(mode) {
		case PALETTE_TINT_GRAYSCALE:
			/* Grey, of brightness (2R + 4G + B) / 7. */
			for(i = 0; i < colCnt; i++) {
				col = *src++;
				red = RED_CHANNEL_DOUBLED(col);
				green = GREEN_CHANNEL_QUADRUPLED(col);
				blue = BLUE_CHANNEL(col);
				col = syIntegerDivide(red + green + blue, 7);
				*dst++ = col;
				*dst++ = col;
				*dst++ = col;
			}
			break;
		case PALETTE_TINT_SEPIA:
			/* The brightness, red stronger, each level within 8 to 28. */
			for(i = 0; i < colCnt; i++) {
				col = *src++;
				red = RGB_RED(col);
				col = syIntegerDivide(
					red + RGB_GREEN(col) + RGB_BLUE(col), SEPIA_SHADE_DIVISOR);
				red = col * 4 + 5;
				blue = col * 3 + 5;
				green = blue;
				if(red < 8)
					red = 8;
				if(green < 8)
					green = 8;
				if(blue < 8)
					blue = 8;
				if(red > 28)
					red = 28;
				if(green > 28)
					green = 28;
				if(blue > 28)
					blue = 28;
				*dst++ = ColorBufferChannelCurveLight[blue];
				*dst++ = ColorBufferChannelCurveLight[green];
				*dst++ = ColorBufferChannelCurveLight[red];
			}
			break;
		case PALETTE_TINT_DIM:
			/* Half R + 10, two thirds G + 8, B - 7; black stays black. */
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
					red = fdPaletteFadeClampChannel(red);
					green = fdPaletteFadeClampChannel(green);
					blue = fdPaletteFadeClampChannel(blue);
				}
				*dst++ = ColorBufferChannelCurveContrast[blue];
				*dst++ = ColorBufferChannelCurveLight[green];
				*dst++ = ColorBufferChannelCurve[red];
			}
			break;
		case PALETTE_TINT_MUTED:
			/* Each level narrowed to a middle range, G and B + 2. */
			for(i = 0; i < colCnt; i++) {
				col = *src++;
				red = RGB_RED(col);
				green = RGB_GREEN(col);
				blue = RGB_BLUE(col);
				if(red < 10)
					red = 10;
				if(green < 16)
					green = 16;
				if(blue < 16)
					blue = 16;
				if(red > 28)
					red = 28;
				if(green > 24)
					green = 24;
				if(blue > 26)
					blue = 26;
				green += 2;
				blue += 2;
				red = fdPaletteFadeClampChannel(red);
				green = fdPaletteFadeClampChannel(green);
				blue = fdPaletteFadeClampChannel(blue);
				*dst++ = ColorBufferChannelCurve[blue];
				*dst++ = ColorBufferChannelCurve[green];
				*dst++ = ColorBufferChannelCurve[red];
			}
			break;
		case PALETTE_TINT_PALE:
			/* Half of each level, plus the mean. */
			for(i = 0; i < colCnt; i++) {
				Sint32 mean;

				col = *src++;
				red = RGB_RED(col);
				green = RGB_GREEN(col);
				blue = RGB_BLUE(col);
				mean = fdPaletteFadeClampChannel((red + green + blue) / 3);
				red = (red >> 1) + mean;
				green = (green >> 1) + mean;
				blue = (blue >> 1) + mean;
				red = fdPaletteFadeClampChannel(red);
				green = fdPaletteFadeClampChannel(green);
				blue = fdPaletteFadeClampChannel(blue);
				dst[0] = ColorBufferChannelCurveContrast[blue];
				dst[1] = ColorBufferChannelCurveContrast[green];
				dst[2] = ColorBufferChannelCurveContrast[red];
				dst += 3;
			}
			break;
		case PALETTE_TINT_RED:
			/* An eighth of G and B added to R; G and B two thirds. */
			for(i = 0; i < colCnt; i++) {
				col = *src++;
				red = RGB_RED(col);
				green = RGB_GREEN(col);
				blue = RGB_BLUE(col);
				red += ((green >> 3) + (blue >> 3));
				red = fdPaletteFadeClampChannel(red);
				green -= green / 3;
				blue -= blue / 3;
				dst[0] = ColorBufferChannelCurve[blue];
				dst[1] = ColorBufferChannelCurve[green];
				dst[2] = ColorBufferChannelCurveLight[red];
				dst += 3;
			}
			break;
		case PALETTE_TINT_DIMMER:
			/* Half R + 6, two thirds G + 4, B - 6. */
			for(i = 0; i < colCnt; i++) {
				col = *src++;
				red = RGB_RED(col);
				green = RGB_GREEN(col);
				blue = RGB_BLUE(col);
				red -= (Uint32)red >> 1;
				green -= green / 3;
				red += 6;
				green += 4;
				blue -= 6;
				red = fdPaletteFadeClampChannel(red);
				green = fdPaletteFadeClampChannel(green);
				blue = fdPaletteFadeClampChannel(blue);
				dst[0] = ColorBufferChannelCurveContrast[blue];
				dst[1] = ColorBufferChannelCurveLight[green];
				dst[2] = ColorBufferChannelCurve[red];
				dst += 3;
			}
			break;
		default:
			/* Unchanged. */
			for(i = 0; i < colCnt; i++) {
				col = *src++;
				dst[i * 3 + 0] = BLUE_CHANNEL(col);
				dst[i * 3 + 1] = GREEN_CHANNEL(col);
				dst[i * 3 + 2] = RED_CHANNEL(col);
			}
			break;
		}
	} else if(mode & PALETTE_TINT_CUSTOM) {
		/* The brightness (2R + 4G + B) / 96 times each level of mode's
		   colour. */
		Uint32 tintRed;
		Uint32 tintGreen;
		Uint32 tintBlue;
		Sint32 red;
		Sint32 green;

		tintRed = RGB_RED(mode);
		tintGreen = RGB_GREEN(mode);
		tintBlue = RGB_BLUE(mode);
		for(i = 0; i < colCnt; i++) {
			col = *src++;
			red = RED_CHANNEL_DOUBLED(col);
			green = GREEN_CHANNEL_QUADRUPLED(col);
			blue = BLUE_CHANNEL(col);
			col = syIntegerDivide(red + green + blue, CUSTOM_TINT_DIVISOR);
			red = col * tintRed;
			green = col * tintGreen;
			blue = col * tintBlue;
			red = fdPaletteFadeClampShiftedChannel(red);
			green = fdPaletteFadeClampShiftedChannel(green);
			blue = fdPaletteFadeClampShiftedChannel(blue);
			dst[0] = blue;
			dst[1] = green;
			dst[2] = red;
			dst += 3;
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

		tintRed = RGB_RED(mode);
		tintGreen = RGB_GREEN(mode);
		tintBlue = RGB_BLUE(mode);
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
			blendedRed =
				fdPaletteFadeClampChannel(FIXEDtoINT((Uint32)blendedRed));
			blendedGreen =
				fdPaletteFadeClampChannel(FIXEDtoINT((Uint32)blendedGreen));
			blendedBlue =
				fdPaletteFadeClampChannel(FIXEDtoINT((Uint32)blendedBlue));
			*dst++ = ColorBufferChannelCurve[blendedBlue];
			*dst++ = ColorBufferChannelCurve[blendedGreen];
			*dst++ = ColorBufferChannelCurve[blendedRed];
		}
	} else if(mode & PALETTE_TINT_SPLIT) {
		/* The colours unchanged, as the default tint writes them. */
		for(i = 0; i < colCnt; i++) {
			col = *src++;
			dst[i * 3 + 0] = BLUE_CHANNEL(col);
			dst[i * 3 + 1] = GREEN_CHANNEL(col);
			dst[i * 3 + 2] = RED_CHANNEL(col);
		}
	} else {
		/* Copy a ready buffer: mode is its address, and the OBJ half starts 224
		   colours of three halfwords in. */
		if(half == PALETTES_SPRITES)
			mode += COLOR_HALF * 3 * sizeof(Uint16);
		syDMAXCopy((const void *)mode, dst, DMA_COPY_WORDS((colCnt * 6) >> 2),
			REG_DMA3);
	}
}

/* Sets up the palette fade (PaletteFadeState) from the BG and OBJ palettes and
   starts updating the palettes from it each tick. */
void fdPaletteFadeInit(void)
{
	PALETTE_FADE *state;

	state = syHeapSlotAllocEWRAM(HEAP_SLOT_PALETTE_FADE, sizeof(*state));
	DMA_FILL32(0, state, sizeof(*state));
	syDMAXCopy((void *)BG_PLTT, &state->pal[0], DMA_COPY_WORDS(COLOR_HALF / 2),
		REG_DMA3);
	syDMAXCopy((void *)OBJ_PLTT, &state->pal[COLOR_HALF],
		DMA_COPY_WORDS(COLOR_HALF / 2), REG_DMA3);
	fdPaletteFadeBuild(PALETTE_TINT_NONE, state->pal, state->tgt, PALETTES_ALL);
	sySetEvent(fdPaletteFadeUpdate, EVENT_PASS_TICK + 15);
}

/* Stops the palette fade and frees it. */
void fdPaletteFadeRelease(void)
{
	syCloseEvent(&fdPaletteFadeUpdate);
	syHeapSlotRelease(HEAP_SLOT_PALETTE_FADE);
}

/* Sets the colours the buffer moves toward (see fdPaletteFadeBuild); does
   nothing without a palette fade. */
void fdPaletteFadeSetTarget(Sint32 mode, Sint32 half)
{
	PALETTE_FADE *state = PaletteFadeState;

	if(state != NULL) {
		fdPaletteFadeBuild(mode, state->pal, state->tgt, half);
	}
}

/* Sets the colours the buffer shows now (see fdPaletteFadeBuild). */
void fdPaletteFadeSetSource(Sint32 mode, Sint32 half)
{
	PALETTE_FADE *state = PaletteFadeState;

	if(state != NULL) {
		fdPaletteFadeBuild(mode, state->pal, state->cur, half);
	}
}

/* Sets the buffer's source backdrop colour (BG colour 0 of its palette
   copy). */
void fdPaletteFadeSetBackdrop(Uint32 col)
{
	PALETTE_FADE *state = PaletteFadeState;

	if(state != NULL) {
		state->pal[0] = col;
	}
}

/* Starts moving the shown colours to the target colours over ticks ticks:
   sets state->duration, clears state->step and fills state->delta. */
void fdPaletteFadeStartInterpolation(Sint32 ticks)
{
	PALETTE_FADE *state = PaletteFadeState;

	if(state != NULL) {
		state->duration = ticks;
		state->step = 0;
		fdPaletteFadeCalcSteps(state->cur, state->tgt, state->delta, ticks);
	}
}

/* Clamps a 5-bit colour channel to 0 to 31. */
Sint32 fdPaletteFadeClampChannel(Sint32 value)
{
	if(value > RGB_LEVEL_MAX)
		return RGB_LEVEL_MAX;
	if(value < 0)
		value = 0;
	return value;
}

/* Clamps a colour channel in its place (bits 14..10) to at most the brightest
   level. */
Sint32 fdPaletteFadeClampShiftedChannel(Sint32 value)
{
	if(value > RGB_LEVEL_MAX << BLUE_SHIFT)
		value = RGB_LEVEL_MAX << BLUE_SHIFT;
	return value;
}

#undef COLOR_MAX
#undef COLOR_HALF
#undef BLUE_CHANNEL
#undef GREEN_CHANNEL
#undef RED_CHANNEL
#undef RED_CHANNEL_DOUBLED
#undef GREEN_CHANNEL_QUADRUPLED
