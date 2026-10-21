/// @file
/// The colour buffer: palettes moved toward target colours.
// Part of object.c.

#include "../object_internal.h"

// Palette work stores 224 BG colours and 224 OBJ colours, each as B, G, R.
#define COLOR_MAX 448
#define COLOR_HALF 224
#define COLOR_CHANNEL_COUNT (COLOR_MAX * 3)

/// Sets each of the 1344 colour channels of step to the amount to add each
/// frame for source to reach target in frames frames; with frames 0 or less
/// step is left as it was. Called by ColorBuffer_StartInterpolation and by
/// the weather effects (psynergy_effects/weather.c).
void ColorBuffer_ComputeSteps(s16 *source, s16 *target, s16 *step, s32 frames) {
  s32 remainingChannels;
  s32 sourceChannel;
  s32 targetChannel;

  if (frames > 0) {
    remainingChannels = COLOR_CHANNEL_COUNT - 1;
    do {
      sourceChannel = *source;
      targetChannel = *target;
      *step = Iwram_SignedDivide(targetChannel - sourceChannel, frames);
      remainingChannels--;
      source++;
      target++;
      step++;
    } while (remainingChannels >= 0);
  }
}

/// Each frame (unless FLAG_PALETTE_BLEND_HELD is set): moves the colour buffer
/// one step toward its target, packs it into RGB555 and queues it for the BG
/// and OBJ palettes. The packed colours are double-buffered: work->page names
/// the page the queued DMA reads, and the other is written. Registered by
/// ColorBuffer_Initialize as a scheduler callback; the menus clear
/// and set FLAG_PALETTE_BLEND_HELD around it (menu/common.c).
void ColorBuffer_Update(void) {
  struct BattleEffectBuffers *work = gPaletteBufferWork;
  u16 *channelDelta = work->delta;
  s32 i;

  if (GameFlag_TestFar(FLAG_PALETTE_BLEND_HELD) != 0) {
    return;
  }
  if (work->duration == 0) {
    return;
  }
  // Accumulate modular 16-bit channel steps; on the last frame copy the
  // target itself, so rounding in the steps never leaves the colours short.
  if (++work->step < work->duration) {
    u16 *currentChannel = work->current;
    for (i = 0; i < COLOR_CHANNEL_COUNT; i++) {
      *currentChannel++ += *channelDelta++;
    }
  } else {
    Iwram_CopyWords(work->current, work->target, sizeof(work->current));
    work->duration = 0;
  }

  {
    // Pack B, G, R fixed-point channels into the hidden RGB555 page: each
    // channel's top 5 bits (bits 14..10) become its RGB555 field.
    u16 *packed = work->packed[1 ^ work->page];
    s32 blueMask = 0x7c00;
    u16 *current;

    i = COLOR_MAX;
    current = work->current;
    for (; i != 0; i--) {
      *packed++ = (current[0] & blueMask) | (((s16)current[1] >> 5) & 0x3e0) |
                  (((s16)current[2] >> 10) & 0x1f);
      current += 3;
    }
  }

  // Show the page just written: queue its BG half and OBJ half (224 colours,
  // 112 words each) for the palettes.
  work->page ^= 1;
  {
    u16 *packed = work->packed[work->page];

    IO_WRITE_QUEUE_PUSH((u32)BG_PLTT, (u32)packed,
                        (DMA_ENABLE | DMA_32BIT) << 16 | (COLOR_HALF / 2));
    IO_WRITE_QUEUE_PUSH((u32)OBJ_PLTT, (u32)(packed + COLOR_HALF),
                        (DMA_ENABLE | DMA_32BIT) << 16 | (COLOR_HALF / 2));
  }
}

// Curves mapping a 5-bit channel to the buffer's channel shifted up 10 bits.
extern u16 gColorBuffer_ChannelCurve[];
extern u16 gColorBuffer_ChannelCurveLight[];
extern u16 gColorBuffer_ChannelCurveContrast[];

/// Builds colours for the colour buffer from the palette src into dst, which
/// holds each colour as B, G and R channels shifted up 10 bits. mode below
/// 0x8000 is one colour for all; PALETTE_TRANSFORM_FROM_SOURCE the backdrop
/// colour; the other PALETTE_TRANSFORM_* values transform each colour; any
/// other value is the address of a ready buffer to copy. half: 0 for all 448
/// colours, 1 for the BG's 224, 2 for the OBJ's 224. Called by the
/// ApplyColorTo functions below, ColorBuffer_Initialize and the
/// weather effects (psynergy_effects/weather.c).
void ColorBuffer_Build(u32 mode, u16 *src, u16 *dst, s32 half) {
  u32 i;
  u32 colorCount = COLOR_MAX;
  s32 red;
  s32 blue;
  u32 color;
  s32 green;

  if (mode == PALETTE_TRANSFORM_FROM_SOURCE)
    mode = *(u16 *)BG_PLTT;
  if (half == 1) {
    colorCount = COLOR_HALF;
  } else if (half == 2) {
    dst += COLOR_HALF * 3;
    src += COLOR_HALF;
    colorCount = COLOR_HALF;
  }

  // One colour: write it once, then let DMA copy it over the rest. The copy
  // reads three halfwords behind where it writes, so the colour repeats.
  if (mode < PALETTE_TRANSFORM_FROM_SOURCE) {
    *dst++ = mode & 0x7c00;
    *dst++ = (mode & 0x03e0) << 5;
    *dst++ = (mode & 0x001f) << 10;
    Dma_Set(dst - 3, dst, (((colorCount - 1) * 6) >> 1) | (DMA_ENABLE << 16),
            REG_DMA3);
  } else if (mode < PALETTE_TRANSFORM_FLAGS) {
    switch (mode) {
    case PALETTE_TRANSFORM_GREY: // grey of brightness (2R + 4G + B) / 7
      for (i = 0; i < colorCount; i++) {
        color = *src++;
        red = ((color << 11) & 0xf800);
        green = ((color << 7) & 0x1f000);
        blue = (color & 0x7c00);
        color = Iwram_SignedDivide(red + green + blue, 7);
        *dst++ = color;
        *dst++ = color;
        *dst++ = color;
      }
      break;
    case PALETTE_TRANSFORM_SEPIA: // the brightness, red stronger, within 8 to
                                  // 28
      for (i = 0; i < colorCount; i++) {
        color = *src++;
        red = color & 31;
        color = Iwram_SignedDivide(
            red + ((color >> 5) & 31) + ((color >> 10) & 31), 10);
        red = color * 4 + 5;
        blue = color * 3 + 5;
        green = blue;
        if (red < 8)
          red = 8;
        if (green < 8)
          green = 8;
        if (blue < 8)
          blue = 8;
        if (red > 28)
          red = 28;
        if (green > 28)
          green = 28;
        if (blue > 28)
          blue = 28;
        *dst++ = gColorBuffer_ChannelCurveLight[blue];
        *dst++ = gColorBuffer_ChannelCurveLight[green];
        *dst++ = gColorBuffer_ChannelCurveLight[red];
      }
      break;
    case PALETTE_TRANSFORM_WARM: // half R + 10, two thirds G + 8, B - 7 (black
                                 // stays black)
      for (i = 0; i < colorCount; i++) {
        color = *src++;
        red = color & 31;
        green = (color >> 5) & 31;
        blue = (color >> 10) & 31;
        if ((red | green | blue) != 0) {
          red -= (u32)red >> 1;
          green -= green / 3;
          red += 10;
          green += 8;
          blue -= 7;
          red = ColorBuffer_ClampChannel(red);
          green = ColorBuffer_ClampChannel(green);
          blue = ColorBuffer_ClampChannel(blue);
        }
        *dst++ = gColorBuffer_ChannelCurveContrast[blue];
        *dst++ = gColorBuffer_ChannelCurveLight[green];
        *dst++ = gColorBuffer_ChannelCurve[red];
      }
      break;
    case PALETTE_TRANSFORM_MUTED: // narrow the range, G and B + 2
      for (i = 0; i < colorCount; i++) {
        color = *src++;
        red = color & 31;
        green = (color >> 5) & 31;
        blue = (color >> 10) & 31;
        if (red < 10)
          red = 10;
        if (green < 16)
          green = 16;
        if (blue < 16)
          blue = 16;
        if (red > 28)
          red = 28;
        if (green > 24)
          green = 24;
        if (blue > 26)
          blue = 26;
        green += 2;
        blue += 2;
        red = ColorBuffer_ClampChannel(red);
        green = ColorBuffer_ClampChannel(green);
        blue = ColorBuffer_ClampChannel(blue);
        *dst++ = gColorBuffer_ChannelCurve[blue];
        *dst++ = gColorBuffer_ChannelCurve[green];
        *dst++ = gColorBuffer_ChannelCurve[red];
      }
      break;
    case PALETTE_TRANSFORM_PALE: // half of each, plus the mean
      for (i = 0; i < colorCount; i++) {
        s32 mean;

        color = *src++;
        red = color & 31;
        green = (color >> 5) & 31;
        blue = (color >> 10) & 31;
        mean = ColorBuffer_ClampChannel((red + green + blue) / 3);
        red = (red >> 1) + mean;
        green = (green >> 1) + mean;
        blue = (blue >> 1) + mean;
        red = ColorBuffer_ClampChannel(red);
        green = ColorBuffer_ClampChannel(green);
        blue = ColorBuffer_ClampChannel(blue);
        dst[0] = gColorBuffer_ChannelCurveContrast[blue];
        dst[1] = gColorBuffer_ChannelCurveContrast[green];
        dst[2] = gColorBuffer_ChannelCurveContrast[red];
        dst += 3;
      }
      break;
    case PALETTE_TRANSFORM_RED: // an eighth of G and B added to R; G and B two
                                // thirds
      for (i = 0; i < colorCount; i++) {
        color = *src++;
        red = color & 31;
        green = (color >> 5) & 31;
        blue = (color >> 10) & 31;
        red += ((green >> 3) + (blue >> 3));
        red = ColorBuffer_ClampChannel(red);
        green -= green / 3;
        blue -= blue / 3;
        dst[0] = gColorBuffer_ChannelCurve[blue];
        dst[1] = gColorBuffer_ChannelCurve[green];
        dst[2] = gColorBuffer_ChannelCurveLight[red];
        dst += 3;
      }
      break;
    case PALETTE_TRANSFORM_WARM2: // half R + 6, two thirds G + 4, B - 6
      for (i = 0; i < colorCount; i++) {
        color = *src++;
        red = color & 31;
        green = (color >> 5) & 31;
        blue = (color >> 10) & 31;
        red -= (u32)red >> 1;
        green -= green / 3;
        red += 6;
        green += 4;
        blue -= 6;
        red = ColorBuffer_ClampChannel(red);
        green = ColorBuffer_ClampChannel(green);
        blue = ColorBuffer_ClampChannel(blue);
        dst[0] = gColorBuffer_ChannelCurveContrast[blue];
        dst[1] = gColorBuffer_ChannelCurveLight[green];
        dst[2] = gColorBuffer_ChannelCurve[red];
        dst += 3;
      }
      break;
    default: // unchanged
      for (i = 0; i < colorCount; i++) {
        color = *src++;
        dst[i * 3 + 0] = color & 0x7c00;
        dst[i * 3 + 1] = (color & 0x03e0) << 5;
        dst[i * 3 + 2] = (color & 0x001f) << 10;
      }
      break;
    }
  } else if (mode & PALETTE_TRANSFORM_TINT) {
    // The brightness (2R + 4G + B) times the colour in mode.
    u32 tintRed;
    u32 tintGreen;
    u32 tintBlue;
    s32 red;
    s32 green;

    tintRed = mode & 31;
    tintGreen = (mode >> 5) & 31;
    tintBlue = (mode >> 10) & 31;
    for (i = 0; i < colorCount; i++) {
      color = *src++;
      red = ((color << 11) & 0xf800);
      green = ((color << 7) & 0x1f000);
      blue = (color & 0x7c00);
      color = Iwram_SignedDivide(red + green + blue, 96);
      red = color * tintRed;
      green = color * tintGreen;
      blue = color * tintBlue;
      red = ColorBuffer_ClampShiftedChannel(red);
      green = ColorBuffer_ClampShiftedChannel(green);
      blue = ColorBuffer_ClampShiftedChannel(blue);
      dst[0] = blue;
      dst[1] = green;
      dst[2] = red;
      dst += 3;
    }
  } else if (mode & PALETTE_TRANSFORM_BLEND) {
    // The brightness R + G + B shared out in the proportions of mode's colour.
    u32 tintRed;
    u32 tintGreen;
    u32 tintBlue;
    s32 blendedRed;
    s32 blendedGreen;
    s32 blendedBlue;

    tintRed = mode & 31;
    tintGreen = (mode >> 5) & 31;
    tintBlue = (mode >> 10) & 31;
    for (i = 0; i < colorCount; i++) {
      color = *src++;
      red = color & 31;
      color = Iwram_SignedDivide(
          (red + ((color >> 5) & 31) + ((color >> 10) & 31)) << 4,
          tintRed + tintGreen + tintBlue);
      blendedRed = Iwram_MulQ16(((color * tintRed) >> 4) << 16,
                                (s32)(tintRed << 16) >> 4);
      blendedGreen = Iwram_MulQ16(((color * tintGreen) >> 4) << 16,
                                  (s32)(tintGreen << 16) >> 4);
      blendedBlue = Iwram_MulQ16(((color * tintBlue) >> 4) << 16,
                                 (s32)(tintBlue << 16) >> 4);
      blendedRed = ColorBuffer_ClampChannel((u32)blendedRed >> 16);
      blendedGreen = ColorBuffer_ClampChannel((u32)blendedGreen >> 16);
      blendedBlue = ColorBuffer_ClampChannel((u32)blendedBlue >> 16);
      *dst++ = gColorBuffer_ChannelCurve[blendedBlue];
      *dst++ = gColorBuffer_ChannelCurve[blendedGreen];
      *dst++ = gColorBuffer_ChannelCurve[blendedRed];
    }
  } else if (mode & PALETTE_TRANSFORM_SPLIT) {
    // The colours unchanged, as the default transform writes them.
    for (i = 0; i < colorCount; i++) {
      color = *src++;
      dst[i * 3 + 0] = color & 0x7c00;
      dst[i * 3 + 1] = (color & 0x03e0) << 5;
      dst[i * 3 + 2] = (color & 0x001f) << 10;
    }
  } else {
    // Copy a ready buffer: mode is its address, and the OBJ half starts 224
    // colours of three halfwords in.
    if (half == 2)
      mode += COLOR_HALF * 3 * sizeof(u16);
    Dma_Set((const void *)mode, dst,
            ((colorCount * 6) >> 2) | (DMA_ENABLE | DMA_32BIT) << 16, REG_DMA3);
  }
}

/// Sets up the colour buffer (gPaletteBufferWork) from the BG and OBJ palettes
/// and starts updating the palettes from it each frame. Called by
/// Field_RunScene on scene load.
void ColorBuffer_Initialize(void) {
  struct BattleEffectBuffers *work;

  work = Runtime_AllocateBlock(HEAP_SLOT_PALETTE_BUFFER, sizeof(*work));
  Dma_Fill32(0, work, sizeof(*work));
  Dma_Set((void *)BG_PLTT, &work->palette[0],
          (DMA_ENABLE | DMA_32BIT) << 16 | (COLOR_HALF / 2), REG_DMA3);
  Dma_Set((void *)OBJ_PLTT, &work->palette[COLOR_HALF],
          (DMA_ENABLE | DMA_32BIT) << 16 | (COLOR_HALF / 2), REG_DMA3);
  ColorBuffer_Build(PALETTE_TRANSFORM_PLAIN, work->palette, work->target, 0);
  Scheduler_AddOrUpdateCallback(ColorBuffer_Update, 0xc8f);
}

/// Stops the colour buffer and frees it. Reached only through a bare far-call
/// veneer; no ☀️ code names it.
void ColorBuffer_Release(void) {
  Scheduler_RemoveCallback(&ColorBuffer_Update);
  Runtime_ReleaseHeapBlock(HEAP_SLOT_PALETTE_BUFFER);
}

/// Sets the colours the buffer moves toward (see ColorBuffer_Build); does
/// nothing without a colour buffer. Called by the field Psynergy effects,
/// the scene transitions, the shop cursor, and scene overlays as
/// Engine_ColorBufferApplyTarget.
void ColorBuffer_SetTarget(s32 mode, s32 half) {
  struct BattleEffectBuffers *work = gPaletteBufferWork;

  if (work != NULL) {
    ColorBuffer_Build(mode, work->palette, work->target, half);
  }
}

/// Sets the colours the buffer shows now (see ColorBuffer_Build). Called by
/// Reveal, the leader's steps (step.c), the transitions, and scene overlays
/// as Engine_ColorBufferApplySource.
void ColorBuffer_SetSource(s32 mode, s32 half) {
  struct BattleEffectBuffers *work = gPaletteBufferWork;

  if (work != NULL) {
    ColorBuffer_Build(mode, work->palette, work->current, half);
  }
}

/// Sets the buffer's source backdrop colour (BG colour 0 of its palette
/// copy). Called by the transitions (transition.c).
void ColorBuffer_SetBackdrop(u32 color) {
  struct BattleEffectBuffers *work = gPaletteBufferWork;

  if (work != NULL) {
    work->palette[0] = color;
  }
}

/// Starts moving the shown colours to the target colours over frames frames:
/// sets work->duration, clears work->step and fills work->delta. Called after
/// ColorBuffer_SetTarget by the same callers, and by scene
/// overlays as Engine_ColorBufferInterpolate.
void ColorBuffer_StartInterpolation(s32 frames) {
  struct BattleEffectBuffers *work = gPaletteBufferWork;

  if (work != NULL) {
    work->duration = frames;
    work->step = 0;
    ColorBuffer_ComputeSteps(work->current, work->target, work->delta, frames);
  }
}

/// Clamps a 5-bit colour channel to 0 to 31.
s32 ColorBuffer_ClampChannel(s32 value) {
  if (value > 31)
    return 31;
  if (value < 0)
    value = 0;
  return value;
}

/// Clamps a colour channel shifted up 10 bits to at most 31 << 10.
s32 ColorBuffer_ClampShiftedChannel(s32 value) {
  // 31744 is 31 << 10.
  if (value > 31744)
    value = 31744;
  return value;
}

#undef COLOR_MAX
#undef COLOR_HALF
#undef COLOR_CHANNEL_COUNT
