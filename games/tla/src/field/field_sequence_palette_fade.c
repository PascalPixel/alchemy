/// @file
/// The palette fades: a fade between two built palettes over a number of
/// frames, and the palettes it builds from the shown one.
// Part of field_sequence.c.

/// Palette work stores 224 BG colours and 224 OBJ colours, each as B, G, R
/// with every channel moved to where blue is (BLUE_BITS, bits 14..10).
#define COLOR_MAX 448
#define COLOR_HALF 224
#define COLOR_CHANNEL_COUNT (COLOR_MAX * 3)

/// Sets each colour channel of step to the amount to add each frame for source
/// to reach target in frames frames; with frames 0 or less step is left as it
/// was.
void PaletteFade_CalculateSteps(s16 *source, s16 *target, s16 *step,
                                s32 frames) {
  s32 remainingChannels;
  s32 sourceChannel;
  s32 targetChannel;
  s32 (*divide)(s32, s32);

  if (frames > 0) {
    divide = Iwram_SignedDivideEntry;
    remainingChannels = COLOR_CHANNEL_COUNT - 1;
    do {
      sourceChannel = *source;
      targetChannel = *target;
      *step = divide(targetChannel - sourceChannel, frames);
      remainingChannels--;
      source++;
      target++;
      step++;
    } while (remainingChannels >= 0);
  }
}

/// Each frame (unless FLAG_PALETTE_BLEND_HELD is set): moves the palette fade
/// one step toward its target, packs it into RGB555 and queues it for the BG
/// and OBJ palettes.
void PaletteFade_Update(void) {
  struct PaletteFade *work = Ram_HeapSlots->paletteFade;
  u16 *channelDelta = work->delta;
  s32 i;

  if (GameFlag_Get(FLAG_PALETTE_BLEND_HELD) != 0) {
    return;
  }
  if (work->duration == 0) {
    return;
  }
  // On the last frame the target itself is copied, so rounding in the steps
  // never leaves the colours short.
  if (++work->step < work->duration) {
    u16 *currentChannel = work->current;
    for (i = 0; i < COLOR_CHANNEL_COUNT; i++) {
      *currentChannel++ += *channelDelta++;
    }
  } else {
    s32 (*copy)(void *, const void *, s32) = Iwram_CopyWordsEntry;

    copy(work->current, work->target, sizeof(work->current));
    work->duration = 0;
  }

  // Packs the channels into the page not shown, then shows it.
  {
    u16 *packed = work->packed[1 ^ work->page];
    s32 blueMask = BLUE_BITS;
    u16 *current;

    i = COLOR_MAX;
    current = work->current;
    for (; i != 0; i--) {
      *packed++ = (current[0] & blueMask) |
                  (((s16)current[1] >> (RGB_BLUE_SHIFT - RGB_GREEN_SHIFT)) &
                   GREEN_BITS) |
                  (((s16)current[2] >> RGB_BLUE_SHIFT) & RED_BITS);
      current += 3;
    }
  }

  work->page ^= 1;
  {
    u16 *packed = work->packed[work->page];

    IO_WRITE_QUEUE_PUSH((u32)BG_PLTT, (u32)packed,
                        DMA_COPY_WORDS(COLOR_HALF / 2));
    IO_WRITE_QUEUE_PUSH((u32)OBJ_PLTT, (u32)(packed + COLOR_HALF),
                        DMA_COPY_WORDS(COLOR_HALF / 2));
  }
}

/// Builds colours for the palette fade from the palette src into dst, which
/// holds each colour as B, G and R channels, each where RGB555 keeps blue.
/// mode below PALETTE_TINT_FROM_SOURCE is one colour for all;
/// PALETTE_TINT_FROM_SOURCE the backdrop colour; the other PALETTE_TINT_*
/// values tint each colour; any other value is the address of a ready buffer
/// to copy. half is the palette selection: PALETTES_ALL for every colour,
/// PALETTES_BG for the BG's, PALETTES_OBJECTS for the OBJ's.
void PaletteFade_Build(u32 mode, u16 *src, u16 *dst, s32 half) {
  u32 i;
  u32 colorCount = COLOR_MAX;
  s32 red;
  s32 blue;
  u32 color;
  s32 green;

  if (mode == PALETTE_TINT_FROM_SOURCE)
    mode = *(u16 *)BG_PLTT;
  if (half == PALETTES_BG) {
    colorCount = COLOR_HALF;
  } else if (half == PALETTES_OBJECTS) {
    dst += COLOR_HALF * 3;
    src += COLOR_HALF;
    colorCount = COLOR_HALF;
  }

  // One colour: write it once, then let DMA copy it over the rest. The copy
  // reads three halfwords behind where it writes, so the colour repeats.
  if (mode < PALETTE_TINT_FROM_SOURCE) {
    *dst++ = mode & BLUE_BITS;
    *dst++ = (mode & GREEN_BITS) << (RGB_BLUE_SHIFT - RGB_GREEN_SHIFT);
    *dst++ = (mode & RED_BITS) << RGB_BLUE_SHIFT;
    Dma_Set(dst - 3, dst, DMA_COPY_HALFWORDS(((colorCount - 1) * 6) >> 1),
            REG_DMA3);
  } else if (mode < PALETTE_TINT_FLAGS) {
    switch (mode) {
    case PALETTE_TINT_GRAYSCALE: // grey of brightness (2R + 4G + B) / 7
      for (i = 0; i < colorCount; i++) {
        color = *src++;
        red = ((color << BRIGHTNESS_RED_SHIFT) &
               (RED_BITS << BRIGHTNESS_RED_SHIFT));
        green = ((color << BRIGHTNESS_GREEN_SHIFT) &
                 (GREEN_BITS << BRIGHTNESS_GREEN_SHIFT));
        blue = (color & BLUE_BITS);
        color = Iwram_SignedDivide(red + green + blue, 7);
        dst[i * 3 + 0] = color;
        dst[i * 3 + 1] = color;
        dst[i * 3 + 2] = color;
      }
      break;
    case PALETTE_TINT_DIM_GRAY: // a third of R + G + B, B three steps darker
      for (i = 0; i < colorCount; i++) {
        color = *src++;
        red = RGB_RED(color);
        color = Iwram_SignedDivide(red + RGB_GREEN(color) + RGB_BLUE(color), 3);
        red = color - 1;
        green = red;
        red = PaletteFade_ClampComponent(red);
        green = PaletteFade_ClampComponent(green);
        blue = color - 3;
        blue = PaletteFade_ClampComponent(blue);
        dst[0] = gFieldEffect_ChannelCurveLight[blue];
        dst[1] = gFieldEffect_ChannelCurveLight[green];
        dst[2] = gFieldEffect_ChannelCurveLight[red];
        dst += 3;
      }
      break;
    case PALETTE_TINT_SEPIA: // the brightness, red stronger
      for (i = 0; i < colorCount; i++) {
        color = *src++;
        red = RGB_RED(color);
        green = RGB_GREEN(color);
        blue = RGB_BLUE(color);
        color = Iwram_SignedDivide(red + green + blue, SEPIA_SHADE_DIVISOR);
        red = color * 4 + 5;
        blue = color * 3 + 5;
        green = blue;
        if (red < SEPIA_LEVEL_MIN)
          red = SEPIA_LEVEL_MIN;
        if (green < SEPIA_LEVEL_MIN)
          green = SEPIA_LEVEL_MIN;
        if (blue < SEPIA_LEVEL_MIN)
          blue = SEPIA_LEVEL_MIN;
        if (red > SEPIA_LEVEL_MAX)
          red = SEPIA_LEVEL_MAX;
        if (green > SEPIA_LEVEL_MAX)
          green = SEPIA_LEVEL_MAX;
        if (blue > SEPIA_LEVEL_MAX)
          blue = SEPIA_LEVEL_MAX;
        *dst++ = gFieldEffect_ChannelCurveLight[blue];
        *dst++ = gFieldEffect_ChannelCurveLight[green];
        *dst++ = gFieldEffect_ChannelCurveLight[red];
      }
      break;
    case PALETTE_TINT_WARM: // half R + 10, two thirds G + 8, B - 7 (black
                            // stays black)
      for (i = 0; i < colorCount; i++) {
        color = *src++;
        red = RGB_RED(color);
        green = RGB_GREEN(color);
        blue = RGB_BLUE(color);
        if ((red | green | blue) != 0) {
          red -= (u32)red >> 1;
          green -= green / 3;
          red += 10;
          green += 8;
          blue -= 7;
          red = PaletteFade_ClampComponent(red);
          green = PaletteFade_ClampComponent(green);
          blue = PaletteFade_ClampComponent(blue);
        }
        *dst++ = gFieldEffect_ChannelCurveContrast[blue];
        *dst++ = gFieldEffect_ChannelCurveLight[green];
        *dst++ = gFieldEffect_ChannelCurve[red];
      }
      break;
    case PALETTE_TINT_MUTED: // narrow the range, G and B + 2
      for (i = 0; i < colorCount; i++) {
        color = *src++;
        red = RGB_RED(color);
        green = RGB_GREEN(color);
        blue = RGB_BLUE(color);
        // The first colour stays as it is.
        if (i != 0) {
          if (red < 8)
            red = 8;
          if (green < 14)
            green = 14;
          if (blue < 14)
            blue = 14;
          if (red > 29)
            red = 29;
          if (green > 25)
            green = 25;
          if (blue > 27)
            blue = 27;
          green += 2;
          blue += 2;
          red = PaletteFade_ClampComponent(red);
          green = PaletteFade_ClampComponent(green);
          blue = PaletteFade_ClampComponent(blue);
        }
        *dst++ = gFieldEffect_ChannelCurve[blue];
        *dst++ = gFieldEffect_ChannelCurve[green];
        *dst++ = gFieldEffect_ChannelCurve[red];
      }
      break;
    case PALETTE_TINT_PALE: // half of each, plus the mean
      for (i = 0; i < colorCount; i++) {
        s32 mean;

        color = *src++;
        red = RGB_RED(color);
        green = RGB_GREEN(color);
        blue = RGB_BLUE(color);
        mean = PaletteFade_ClampComponent((red + green + blue) / 3);
        red = (red >> 1) + mean;
        green = (green >> 1) + mean;
        blue = (blue >> 1) + mean;
        red = PaletteFade_ClampComponent(red);
        green = PaletteFade_ClampComponent(green);
        blue = PaletteFade_ClampComponent(blue);
        dst[0] = gFieldEffect_ChannelCurveContrast[blue];
        dst[1] = gFieldEffect_ChannelCurveContrast[green];
        dst[2] = gFieldEffect_ChannelCurveContrast[red];
        dst += 3;
      }
      break;
    case PALETTE_TINT_RED: // an eighth of G and B added to R; G and B two
                           // thirds
      for (i = 0; i < colorCount; i++) {
        color = *src++;
        red = RGB_RED(color);
        green = RGB_GREEN(color);
        blue = RGB_BLUE(color);
        red += ((green >> 3) + (blue >> 3));
        red = PaletteFade_ClampComponent(red);
        green -= green / 3;
        blue -= blue / 3;
        dst[0] = gFieldEffect_ChannelCurve[blue];
        dst[1] = gFieldEffect_ChannelCurve[green];
        dst[2] = gFieldEffect_ChannelCurveLight[red];
        dst += 3;
      }
      break;
    case PALETTE_TINT_WARM_DARK: // half R + 6, two thirds G + 4, B - 6
      for (i = 0; i < colorCount; i++) {
        color = *src++;
        red = RGB_RED(color);
        red -= (u32)red >> 1;
        green = RGB_GREEN(color);
        blue = RGB_BLUE(color);
        green -= green / 3;
        red += 6;
        green += 4;
        blue -= 6;
        red = PaletteFade_ClampComponent(red);
        green = PaletteFade_ClampComponent(green);
        blue = PaletteFade_ClampComponent(blue);
        dst[0] = gFieldEffect_ChannelCurveContrast[blue];
        dst[1] = gFieldEffect_ChannelCurveLight[green];
        dst[2] = gFieldEffect_ChannelCurve[red];
        dst += 3;
      }
      break;
    case PALETTE_TINT_DIM_BRIGHT: // a fifth off each channel of a bright colour
      for (i = 0; i < colorCount; i++) {
        color = *src++;
        red = RGB_RED(color);
        green = RGB_GREEN(color);
        blue = RGB_BLUE(color);
        if (red + green + blue > 60) {
          red -= red / 5;
          green -= green / 5;
          blue -= blue / 5;
        }
        red = PaletteFade_ClampComponent(red);
        green = PaletteFade_ClampComponent(green);
        blue = PaletteFade_ClampComponent(blue);
        *dst++ = gFieldEffect_ChannelCurve[blue];
        *dst++ = gFieldEffect_ChannelCurve[green];
        *dst++ = gFieldEffect_ChannelCurve[red];
      }
      break;
    default: // unchanged
      for (i = 0; i < colorCount; i++) {
        color = *src++;
        dst[i * 3 + 0] = color & BLUE_BITS;
        dst[i * 3 + 1] = (color & GREEN_BITS)
                         << (RGB_BLUE_SHIFT - RGB_GREEN_SHIFT);
        dst[i * 3 + 2] = (color & RED_BITS) << RGB_BLUE_SHIFT;
      }
      break;
    }
  } else if (mode & PALETTE_TINT_CUSTOM) {
    // The brightness (2R + 4G + B) times the colour in mode.
    u32 tintRed;
    u32 tintGreen;
    u32 tintBlue;
    s32 red;
    s32 green;

    tintRed = mode;
    tintRed &= RGB_LEVEL_MAX;
    tintGreen = RGB_GREEN(mode);
    tintBlue = mode >> RGB_BLUE_SHIFT;
    tintBlue &= RGB_LEVEL_MAX;
    for (i = 0; i < colorCount; i++) {
      color = *src++;
      red = ((color << BRIGHTNESS_RED_SHIFT) &
             (RED_BITS << BRIGHTNESS_RED_SHIFT));
      green = ((color << BRIGHTNESS_GREEN_SHIFT) &
               (GREEN_BITS << BRIGHTNESS_GREEN_SHIFT));
      blue = (color & BLUE_BITS);
      color = Iwram_SignedDivide(red + green + blue, CUSTOM_TINT_DIVISOR);
      red = color * tintRed;
      green = color * tintGreen;
      blue = color * tintBlue;
      red = PaletteFade_ClampShiftedComponent(red);
      green = PaletteFade_ClampShiftedComponent(green);
      blue = PaletteFade_ClampShiftedComponent(blue);
      dst[i * 3 + 0] = blue;
      dst[i * 3 + 1] = green;
      dst[i * 3 + 2] = red;
    }
  } else if (mode & PALETTE_TINT_BLEND) {
    // The brightness R + G + B shared out in the proportions of mode's colour.
    u32 tintRed;
    u32 tintGreen;
    u32 tintBlue;
    FIXED blendedRed;
    FIXED blendedGreen;
    FIXED blendedBlue;
    s32 clampedRed;
    s32 clampedGreen;
    s32 clampedBlue;

    tintRed = mode;
    tintRed &= RGB_LEVEL_MAX;
    tintGreen = RGB_GREEN(mode);
    tintBlue = mode >> RGB_BLUE_SHIFT;
    tintBlue &= RGB_LEVEL_MAX;
    for (i = 0; i < colorCount; i++) {
      color = *src++;
      red = RGB_RED(color);
      color =
          Iwram_SignedDivide((red + RGB_GREEN(color) + RGB_BLUE(color)) << 4,
                             tintRed + tintGreen + tintBlue);
      blendedRed = Iwram_MulFX(INTtoFIXED((color * tintRed) >> 4),
                               (s32)INTtoFIXED(tintRed) >> 4);
      blendedGreen = Iwram_MulFX(INTtoFIXED((color * tintGreen) >> 4),
                                 (s32)INTtoFIXED(tintGreen) >> 4);
      blendedBlue = Iwram_MulFX(INTtoFIXED((color * tintBlue) >> 4),
                                (s32)INTtoFIXED(tintBlue) >> 4);
      clampedRed = PaletteFade_ClampComponent(FIXEDtoINT((u32)blendedRed));
      clampedGreen = PaletteFade_ClampComponent(FIXEDtoINT((u32)blendedGreen));
      clampedBlue = PaletteFade_ClampComponent(FIXEDtoINT((u32)blendedBlue));
      dst[i * 3 + 0] = gFieldEffect_ChannelCurve[clampedBlue];
      dst[i * 3 + 1] = gFieldEffect_ChannelCurve[clampedGreen];
      dst[i * 3 + 2] = gFieldEffect_ChannelCurve[clampedRed];
    }
  } else if (mode & PALETTE_TINT_SPLIT) {
    // The colours unchanged, as the default tint writes them.
    for (i = 0; i < colorCount; i++) {
      color = *src++;
      dst[i * 3 + 0] = color & BLUE_BITS;
      dst[i * 3 + 1] = (color & GREEN_BITS)
                       << (RGB_BLUE_SHIFT - RGB_GREEN_SHIFT);
      dst[i * 3 + 2] = (color & RED_BITS) << RGB_BLUE_SHIFT;
    }
  } else {
    // Copy a ready buffer: mode is its address, and the OBJ half starts after
    // the BG's colours of three halfwords.
    if (half == PALETTES_OBJECTS)
      mode += COLOR_HALF * 3 * sizeof(u16);
    Dma_Set((const void *)mode, dst, DMA_COPY_WORDS((colorCount * 6) >> 2),
            REG_DMA3);
  }
}

/// Sets up the palette fade (HEAP_SLOT_PALETTE_FADE) from the BG and OBJ
/// palettes and starts updating the palettes from it each frame.
void PaletteFade_Initialize(void) {
  struct PaletteFade *work;

  work = HeapSlot_AllocEwram(HEAP_SLOT_PALETTE_FADE, sizeof(*work));
  Dma_Fill32(0, work, sizeof(*work));
  Dma_Set((void *)BG_PLTT, &work->palette[0], DMA_COPY_WORDS(COLOR_HALF / 2),
          REG_DMA3);
  Dma_Set((void *)OBJ_PLTT, &work->palette[COLOR_HALF],
          DMA_COPY_WORDS(COLOR_HALF / 2), REG_DMA3);
  PaletteFade_Build(PALETTE_TINT_NONE, work->palette, work->target,
                    PALETTES_ALL);
  Task_Add(PaletteFade_Update, TASK_PASS_FRAME + 15);
}

/// Stops the palette fade and frees it.
void PaletteFade_Release(void) {
  Task_Remove(&PaletteFade_Update);
  HeapSlot_Release(HEAP_SLOT_PALETTE_FADE);
}

/// Sets the colours the buffer moves toward (see PaletteFade_Build); does
/// nothing without a palette fade.
void PaletteFade_SetTarget(s32 mode, s32 half) {
  struct PaletteFade *work = Ram_HeapSlots->paletteFade;

  if (work != NULL) {
    PaletteFade_Build(mode, work->palette, work->target, half);
  }
}

/// Sets the colours the buffer shows now (see PaletteFade_Build).
void PaletteFade_SetSource(s32 mode, s32 half) {
  struct PaletteFade *work = Ram_HeapSlots->paletteFade;

  if (work != NULL) {
    PaletteFade_Build(mode, work->palette, work->current, half);
  }
}

/// Sets the buffer's source backdrop colour (BG colour 0 of its palette copy).
void PaletteFade_SetBackdrop(u32 color) {
  struct PaletteFade *work = Ram_HeapSlots->paletteFade;

  if (work != NULL) {
    work->palette[0] = color;
  }
}

/// Packs the target colours, three channels each, back into the palette copy as
/// RGB555.
void PaletteFade_PackTarget(void) {
  struct PaletteFade *work = Ram_HeapSlots->paletteFade;
  u16 *packed = work->palette;
  s32 blueMask = BLUE_BITS;
  u16 *current;
  s32 i;

  i = COLOR_MAX;
  current = work->target;
  for (; i != 0; i--) {
    *packed++ =
        (current[0] & blueMask) |
        ((current[1] >> (RGB_BLUE_SHIFT - RGB_GREEN_SHIFT)) & GREEN_BITS) |
        ((current[2] >> RGB_BLUE_SHIFT) & RED_BITS);
    current += 3;
  }
  // FAKEMATCH(dead): a decrement of work, which nothing reads after it.
  work--;
}

/// Starts moving the shown colours to the target colours over frames frames:
/// sets work->duration, clears work->step and fills work->delta.
void PaletteFade_StartInterpolation(s32 frames) {
  struct PaletteFade *work = Ram_HeapSlots->paletteFade;

  if (work != NULL) {
    work->duration = frames;
    work->step = 0;
    PaletteFade_CalculateSteps(work->current, work->target, work->delta,
                               frames);
  }
}

/// Takes colours as the BG colours of the palette copy and makes them,
/// unchanged, the BG colours' target.
void PaletteFade_LoadPalette(const u16 *colours) {
  struct PaletteFade *work = Ram_HeapSlots->paletteFade;

  if (work != NULL) {
    s32 (*copy)(void *destination, const void *source, s32 size) =
        Iwram_CopyWordsEntry;

    copy(work->palette, colours, COLOR_HALF * sizeof(u16));
    PaletteFade_Build(PALETTE_TINT_NONE, work->palette, work->target,
                      PALETTES_BG);
  }
}

/// Clamps a colour channel to 0 to RGB_LEVEL_MAX.
s32 PaletteFade_ClampComponent(s32 value) {
  if (value > RGB_LEVEL_MAX)
    return RGB_LEVEL_MAX;
  if (value < 0)
    value = 0;
  return value;
}

/// Clamps a colour channel held where blue is to at most RGB_LEVEL_MAX there.
s32 PaletteFade_ClampShiftedComponent(s32 value) {
  if (value > RGB_LEVEL_MAX << RGB_BLUE_SHIFT)
    value = RGB_LEVEL_MAX << RGB_BLUE_SHIFT;
  return value;
}

#undef COLOR_MAX
#undef COLOR_HALF
#undef COLOR_CHANNEL_COUNT
