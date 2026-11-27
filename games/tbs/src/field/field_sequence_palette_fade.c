/* @file */
/* The palette fade: palettes moved toward target colours. */
/* Part of field_sequence.c. */

#include "field_sequence_internal.h"

/* The colours a palette fade moves: the BG's 224, then the OBJ's 224. */
#define COLOR_HALF 224
#define COLOR_MAX  (2 * COLOR_HALF)
/* Palette fade state holds each channel of a colour in bits 14..10 of a */
/* halfword, where RGB555 keeps blue: green's bits move up */
/* BLUE_SHIFT - GREEN_SHIFT to get there, red's BLUE_SHIFT. */
#define BLUE_CHANNEL(color) ((color) & BLUE_BITS)
#define GREEN_CHANNEL(color)                                                   \
	(((color) & GREEN_BITS) << (BLUE_SHIFT - GREEN_SHIFT))
#define RED_CHANNEL(color) (((color) & RED_BITS) << BLUE_SHIFT)
/* For the brightness 2R + 4G + B: red's level doubled and green's */
/* quadrupled, each in the channel's place. */
#define RED_CHANNEL_DOUBLED(color)                                             \
	(((color) << BRIGHTNESS_RED_SHIFT) & (RED_BITS << BRIGHTNESS_RED_SHIFT))
#define GREEN_CHANNEL_QUADRUPLED(color)                                        \
	(((color) << BRIGHTNESS_GREEN_SHIFT) &                                     \
	 (GREEN_BITS << BRIGHTNESS_GREEN_SHIFT))

/* Sets each channel of step (three for each of the 448 colours) to the amount
 */
/* to add each frame for source to reach target in frames frames; with frames */
/* 0 or less step is left as it was. */
void fdPaletteFadeCalcSteps(s16 *source, s16 *target, s16 *step, s32 frames) {
	s32 i;
	s32 sourceChannel;
	s32 targetChannel;

	if(frames > 0) {
		for(i = 0; i < PALETTE_FADE_CHANNELS; i++) {
			sourceChannel = *source;
			targetChannel = *target;
			*step = syIntegerDivide(targetChannel - sourceChannel, frames);
			source++;
			target++;
			step++;
		}
	}
}

/* Each frame (unless FLAG_PALETTE_BLEND_HELD is set): moves the palette fade */
/* one step toward its target, packs it into RGB555 and queues it for the BG */
/* and OBJ palettes. */
void fdPaletteFadeUpdate(void) {
	PALETTE_FADE *state = PaletteFadeState;
	u16 *channelDelta = state->delta;
	s32 i;

	if(gmFlagGetStub(FLAG_PALETTE_BLEND_HELD) != 0) {
		return;
	}
	if(state->duration == 0) {
		return;
	}
	/* Accumulate modular 16-bit channel steps; on the last frame copy the */
	/* target itself, so rounding in the steps never leaves the colours short.
	 */
	if(++state->step < state->duration) {
		u16 *currentChannel = state->cur;
		for(i = 0; i < PALETTE_FADE_CHANNELS; i++) {
			*currentChannel++ += *channelDelta++;
		}
	}
	else {
		syWordsCopy(state->cur, state->tgt, sizeof(state->cur));
		state->duration = 0;
	}

	{
		/* Pack the channels into the hidden page as RGB555: each channel's bits
		 */
		/* 14..10 become its colour's level. */
		u16 *packed = state->packed[1 ^ state->page];
		s32 blueMask = BLUE_BITS;
		u16 *current;

		i = COLOR_MAX;
		current = state->cur;
		for(; i != 0; i--) {
			*packed++ =
				(current[0] & blueMask) |
				(((s16)current[1] >> (BLUE_SHIFT - GREEN_SHIFT)) & GREEN_BITS) |
				(((s16)current[2] >> BLUE_SHIFT) & RED_BITS);
			current += 3;
		}
	}

	/* Show the page just written: queue its BG half and OBJ half (224 colours,
	 */
	/* 112 words each) for the palettes. */
	state->page ^= 1;
	{
		u16 *packed = state->packed[state->page];

		IO_WRITE_QUEUE_PUSH((u32)BG_PLTT, (u32)packed,
							DMA_COPY_WORDS(COLOR_HALF / 2));
		IO_WRITE_QUEUE_PUSH((u32)OBJ_PLTT, (u32)(packed + COLOR_HALF),
							DMA_COPY_WORDS(COLOR_HALF / 2));
	}
}

/* Builds palette fade colours from the palette src into dst, three channels */
/* a colour. mode is one RGB555 colour, below PALETTE_TINT_FROM_SOURCE, for */
/* every colour alike; PALETTE_TINT_FROM_SOURCE for the backdrop colour alike;
 */
/* a PALETTE_TINT_* value to tint each colour; or the address of ready */
/* channels to copy. half is the palette selection: PALETTES_ALL for all 448 */
/* colours, PALETTES_BG for the BG's 224, PALETTES_SPRITES for the OBJ's 224. */
void fdPaletteFadeBuild(u32 mode, u16 *src, u16 *dst, s32 half) {
	u32 i;
	u32 colorCount = COLOR_MAX;
	s32 red;
	s32 blue;
	u32 color;
	s32 green;

	if(mode == PALETTE_TINT_FROM_SOURCE)
		mode = *(u16 *)BG_PLTT;
	if(half == PALETTES_BG) {
		colorCount = COLOR_HALF;
	}
	else if(half == PALETTES_SPRITES) {
		dst += COLOR_HALF * 3;
		src += COLOR_HALF;
		colorCount = COLOR_HALF;
	}

	/* One colour: write it once, then let DMA copy it over the rest. */
	if(mode < PALETTE_TINT_FROM_SOURCE) {
		*dst++ = BLUE_CHANNEL(mode);
		*dst++ = GREEN_CHANNEL(mode);
		*dst++ = RED_CHANNEL(mode);
		syDMAXCopy(dst - 3, dst,
				   DMA_COPY_HALFWORDS(((colorCount - 1) * 6) >> 1), REG_DMA3);
	}
	else if(mode < PALETTE_TINT_FLAGS) {
		switch(mode) {
		case PALETTE_TINT_GRAYSCALE:
			/* Grey, of brightness (2R + 4G + B) / 7. */
			for(i = 0; i < colorCount; i++) {
				color = *src++;
				red = RED_CHANNEL_DOUBLED(color);
				green = GREEN_CHANNEL_QUADRUPLED(color);
				blue = BLUE_CHANNEL(color);
				color = syIntegerDivide(red + green + blue, 7);
				*dst++ = color;
				*dst++ = color;
				*dst++ = color;
			}
			break;
		case PALETTE_TINT_SEPIA:
			/* The brightness, red stronger, each level within 8 to 28. */
			for(i = 0; i < colorCount; i++) {
				color = *src++;
				red = RGB_RED(color);
				color =
					syIntegerDivide(red + RGB_GREEN(color) + RGB_BLUE(color),
									SEPIA_SHADE_DIVISOR);
				red = color * 4 + 5;
				blue = color * 3 + 5;
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
				*dst++ = ColorBuffer_ChannelCurveLight[blue];
				*dst++ = ColorBuffer_ChannelCurveLight[green];
				*dst++ = ColorBuffer_ChannelCurveLight[red];
			}
			break;
		case PALETTE_TINT_DIM:
			/* Half R + 10, two thirds G + 8, B - 7; black stays black. */
			for(i = 0; i < colorCount; i++) {
				color = *src++;
				red = RGB_RED(color);
				green = RGB_GREEN(color);
				blue = RGB_BLUE(color);
				if((red | green | blue) != 0) {
					red -= (u32)red >> 1;
					green -= green / 3;
					red += 10;
					green += 8;
					blue -= 7;
					red = fdPaletteFadeClampChannel(red);
					green = fdPaletteFadeClampChannel(green);
					blue = fdPaletteFadeClampChannel(blue);
				}
				*dst++ = ColorBuffer_ChannelCurveContrast[blue];
				*dst++ = ColorBuffer_ChannelCurveLight[green];
				*dst++ = ColorBuffer_ChannelCurve[red];
			}
			break;
		case PALETTE_TINT_MUTED:
			/* Each level narrowed to a middle range, G and B + 2. */
			for(i = 0; i < colorCount; i++) {
				color = *src++;
				red = RGB_RED(color);
				green = RGB_GREEN(color);
				blue = RGB_BLUE(color);
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
				*dst++ = ColorBuffer_ChannelCurve[blue];
				*dst++ = ColorBuffer_ChannelCurve[green];
				*dst++ = ColorBuffer_ChannelCurve[red];
			}
			break;
		case PALETTE_TINT_PALE:
			/* Half of each level, plus the mean. */
			for(i = 0; i < colorCount; i++) {
				s32 mean;

				color = *src++;
				red = RGB_RED(color);
				green = RGB_GREEN(color);
				blue = RGB_BLUE(color);
				mean = fdPaletteFadeClampChannel((red + green + blue) / 3);
				red = (red >> 1) + mean;
				green = (green >> 1) + mean;
				blue = (blue >> 1) + mean;
				red = fdPaletteFadeClampChannel(red);
				green = fdPaletteFadeClampChannel(green);
				blue = fdPaletteFadeClampChannel(blue);
				dst[0] = ColorBuffer_ChannelCurveContrast[blue];
				dst[1] = ColorBuffer_ChannelCurveContrast[green];
				dst[2] = ColorBuffer_ChannelCurveContrast[red];
				dst += 3;
			}
			break;
		case PALETTE_TINT_RED:
			/* An eighth of G and B added to R; G and B two thirds. */
			for(i = 0; i < colorCount; i++) {
				color = *src++;
				red = RGB_RED(color);
				green = RGB_GREEN(color);
				blue = RGB_BLUE(color);
				red += ((green >> 3) + (blue >> 3));
				red = fdPaletteFadeClampChannel(red);
				green -= green / 3;
				blue -= blue / 3;
				dst[0] = ColorBuffer_ChannelCurve[blue];
				dst[1] = ColorBuffer_ChannelCurve[green];
				dst[2] = ColorBuffer_ChannelCurveLight[red];
				dst += 3;
			}
			break;
		case PALETTE_TINT_DIMMER:
			/* Half R + 6, two thirds G + 4, B - 6. */
			for(i = 0; i < colorCount; i++) {
				color = *src++;
				red = RGB_RED(color);
				green = RGB_GREEN(color);
				blue = RGB_BLUE(color);
				red -= (u32)red >> 1;
				green -= green / 3;
				red += 6;
				green += 4;
				blue -= 6;
				red = fdPaletteFadeClampChannel(red);
				green = fdPaletteFadeClampChannel(green);
				blue = fdPaletteFadeClampChannel(blue);
				dst[0] = ColorBuffer_ChannelCurveContrast[blue];
				dst[1] = ColorBuffer_ChannelCurveLight[green];
				dst[2] = ColorBuffer_ChannelCurve[red];
				dst += 3;
			}
			break;
		default:
			/* Unchanged. */
			for(i = 0; i < colorCount; i++) {
				color = *src++;
				dst[i * 3 + 0] = BLUE_CHANNEL(color);
				dst[i * 3 + 1] = GREEN_CHANNEL(color);
				dst[i * 3 + 2] = RED_CHANNEL(color);
			}
			break;
		}
	}
	else if(mode & PALETTE_TINT_CUSTOM) {
		/* The brightness (2R + 4G + B) / 96 times each level of mode's colour.
		 */
		u32 tintRed;
		u32 tintGreen;
		u32 tintBlue;
		s32 red;
		s32 green;

		tintRed = RGB_RED(mode);
		tintGreen = RGB_GREEN(mode);
		tintBlue = RGB_BLUE(mode);
		for(i = 0; i < colorCount; i++) {
			color = *src++;
			red = RED_CHANNEL_DOUBLED(color);
			green = GREEN_CHANNEL_QUADRUPLED(color);
			blue = BLUE_CHANNEL(color);
			color = syIntegerDivide(red + green + blue, CUSTOM_TINT_DIVISOR);
			red = color * tintRed;
			green = color * tintGreen;
			blue = color * tintBlue;
			red = fdPaletteFadeClampShiftedChannel(red);
			green = fdPaletteFadeClampShiftedChannel(green);
			blue = fdPaletteFadeClampShiftedChannel(blue);
			dst[0] = blue;
			dst[1] = green;
			dst[2] = red;
			dst += 3;
		}
	}
	else if(mode & PALETTE_TINT_BLEND) {
		/* The brightness R + G + B shared out in the proportions of mode's
		 * colour. */
		u32 tintRed;
		u32 tintGreen;
		u32 tintBlue;
		FIXED blendedRed;
		FIXED blendedGreen;
		FIXED blendedBlue;

		tintRed = RGB_RED(mode);
		tintGreen = RGB_GREEN(mode);
		tintBlue = RGB_BLUE(mode);
		for(i = 0; i < colorCount; i++) {
			color = *src++;
			red = RGB_RED(color);
			color =
				syIntegerDivide((red + RGB_GREEN(color) + RGB_BLUE(color)) << 4,
								tintRed + tintGreen + tintBlue);
			blendedRed = syMulFX(INTtoFIXED((color * tintRed) >> 4),
								 (s32)INTtoFIXED(tintRed) >> 4);
			blendedGreen = syMulFX(INTtoFIXED((color * tintGreen) >> 4),
								   (s32)INTtoFIXED(tintGreen) >> 4);
			blendedBlue = syMulFX(INTtoFIXED((color * tintBlue) >> 4),
								  (s32)INTtoFIXED(tintBlue) >> 4);
			blendedRed = fdPaletteFadeClampChannel(FIXEDtoINT((u32)blendedRed));
			blendedGreen =
				fdPaletteFadeClampChannel(FIXEDtoINT((u32)blendedGreen));
			blendedBlue =
				fdPaletteFadeClampChannel(FIXEDtoINT((u32)blendedBlue));
			*dst++ = ColorBuffer_ChannelCurve[blendedBlue];
			*dst++ = ColorBuffer_ChannelCurve[blendedGreen];
			*dst++ = ColorBuffer_ChannelCurve[blendedRed];
		}
	}
	else if(mode & PALETTE_TINT_SPLIT) {
		/* The colours unchanged, as the default tint writes them. */
		for(i = 0; i < colorCount; i++) {
			color = *src++;
			dst[i * 3 + 0] = BLUE_CHANNEL(color);
			dst[i * 3 + 1] = GREEN_CHANNEL(color);
			dst[i * 3 + 2] = RED_CHANNEL(color);
		}
	}
	else {
		/* Copy a ready buffer: mode is its address, and the OBJ half starts 224
		 */
		/* colours of three halfwords in. */
		if(half == PALETTES_SPRITES)
			mode += COLOR_HALF * 3 * sizeof(u16);
		syDMAXCopy((const void *)mode, dst,
				   DMA_COPY_WORDS((colorCount * 6) >> 2), REG_DMA3);
	}
}

/* Sets up the palette fade (PaletteFadeState) from the BG and OBJ palettes */
/* and starts updating the palettes from it each frame. */
void fdPaletteFadeInit(void) {
	PALETTE_FADE *state;

	state = syHeapSlotAllocEwram(HEAP_SLOT_PALETTE_FADE, sizeof(*state));
	btDmaFill32(0, state, sizeof(*state));
	syDMAXCopy((void *)BG_PLTT, &state->pal[0], DMA_COPY_WORDS(COLOR_HALF / 2),
			   REG_DMA3);
	syDMAXCopy((void *)OBJ_PLTT, &state->pal[COLOR_HALF],
			   DMA_COPY_WORDS(COLOR_HALF / 2), REG_DMA3);
	fdPaletteFadeBuild(PALETTE_TINT_NONE, state->pal, state->tgt, PALETTES_ALL);
	sySetEvent(fdPaletteFadeUpdate, TASK_PASS_FRAME + 15);
}

/* Stops the palette fade and frees it. */
void fdPaletteFadeRelease(void) {
	syCloseEvent(&fdPaletteFadeUpdate);
	syHeapSlotRelease(HEAP_SLOT_PALETTE_FADE);
}

/* Sets the colours the buffer moves toward (see fdPaletteFadeBuild); does */
/* nothing without a palette fade. */
void fdPaletteFadeSetTarget(s32 mode, s32 half) {
	PALETTE_FADE *state = PaletteFadeState;

	if(state != NULL) {
		fdPaletteFadeBuild(mode, state->pal, state->tgt, half);
	}
}

/* Sets the colours the buffer shows now (see fdPaletteFadeBuild). */
void fdPaletteFadeSetSource(s32 mode, s32 half) {
	PALETTE_FADE *state = PaletteFadeState;

	if(state != NULL) {
		fdPaletteFadeBuild(mode, state->pal, state->cur, half);
	}
}

/* Sets the buffer's source backdrop colour (BG colour 0 of its palette copy).
 */
void fdPaletteFadeSetBackdrop(u32 color) {
	PALETTE_FADE *state = PaletteFadeState;

	if(state != NULL) {
		state->pal[0] = color;
	}
}

/* Starts moving the shown colours to the target colours over frames frames: */
/* sets state->duration, clears state->step and fills state->delta. */
void fdPaletteFadeStartInterpolation(s32 frames) {
	PALETTE_FADE *state = PaletteFadeState;

	if(state != NULL) {
		state->duration = frames;
		state->step = 0;
		fdPaletteFadeCalcSteps(state->cur, state->tgt, state->delta, frames);
	}
}

/* Clamps a 5-bit colour channel to 0 to 31. */
s32 fdPaletteFadeClampChannel(s32 value) {
	if(value > RGB_LEVEL_MAX)
		return RGB_LEVEL_MAX;
	if(value < 0)
		value = 0;
	return value;
}

/* Clamps a colour channel in its place (bits 14..10) to at most the */
/* brightest level. */
s32 fdPaletteFadeClampShiftedChannel(s32 value) {
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
