// Draft, differ 737 of 976 (766 before; 2026-10-26, three plain reorders: the
// second half's dst step after its count, the dark tint's halving before
// green, a tint declaration) (2026-10-25): The Broken Seal's PaletteFade_Build,
// which builds the battle effects' colour buffers from a palette (each colour
// as B, G and R channels shifted up 10 bits). This game adds two tints after
// the seven The Broken Seal has: 0x10008 darkens bright colours (a fifth off
// each channel when their sum is over 60), and 0x10009, which the switch places
// between the grey and sepia tints, a grey of a third of R + G + B, blue
// three steps darker. Left: a 52-byte stack frame for the native's 48, which keeps
// the colour count on the stack at 44, and the allocation that follows.
#include "types.h"
#include "iwram_call.h"
#include "system/dma.h"
#include "system/io_reg.h"

#define COLOR_MAX 448
#define COLOR_HALF 224

enum PaletteTint {
  PALETTE_TINT_FROM_SOURCE = 0x8000,
  PALETTE_TINT_NONE = 0x10000,
  PALETTE_TINT_GRAYSCALE = 0x10001,
  PALETTE_TINT_SEPIA = 0x10002,
  PALETTE_TINT_WARM = 0x10003,
  PALETTE_TINT_MUTED = 0x10004,
  PALETTE_TINT_PALE = 0x10005,
  PALETTE_TINT_RED = 0x10006,
  PALETTE_TINT_WARM_DARK = 0x10007,
  PALETTE_TINT_DIM_BRIGHT = 0x10008,
  PALETTE_TINT_DIM_GRAY = 0x10009,
  PALETTE_TINT_FLAGS = 0x100000,
  PALETTE_TINT_CUSTOM = 0x200000,
  PALETTE_TINT_BLEND = 0x400000,
  PALETTE_TINT_SPLIT = 0x800000
};

s32 Graphics_ClampRgb555Channel(s32 value);
s32 Graphics_ClampRgb555Component(s32 value);

extern u16 Data_080f0246[];
extern u16 Data_080f0286[];
extern u16 Data_080f02c6[];

#define gColorBuffer_ChannelCurve Data_080f0246
#define gColorBuffer_ChannelCurveLight Data_080f0286
#define gColorBuffer_ChannelCurveContrast Data_080f02c6
#define PaletteFade_ClampComponent Graphics_ClampRgb555Channel
#define PaletteFade_ClampShiftedComponent Graphics_ClampRgb555Component

void BattleEffect_BuildBuffer(u32 mode, u16 *src, u16 *dst, s32 half) {
  u32 i;
  u32 colorCount = COLOR_MAX;
  s32 red;
  s32 blue;
  u32 color;
  s32 green;

  if (mode == PALETTE_TINT_FROM_SOURCE)
    mode = *(u16 *)BG_PLTT;
  if (half == 1) {
    colorCount = COLOR_HALF;
  } else if (half == 2) {
    src += COLOR_HALF;
    colorCount = COLOR_HALF;
    dst += COLOR_HALF * 3;
  }

  // One colour: write it once, then let DMA copy it over the rest. The copy
  // reads three halfwords behind where it writes, so the colour repeats.
  if (mode < PALETTE_TINT_FROM_SOURCE) {
    *dst++ = mode & 0x7c00;
    *dst++ = (mode & 0x03e0) << 5;
    *dst++ = (mode & 0x001f) << 10;
    Dma_Set(dst - 3, dst, (((colorCount - 1) * 6) >> 1) | (DMA_ENABLE << 16),
            REG_DMA3);
  } else if (mode < PALETTE_TINT_FLAGS) {
    switch (mode) {
    case PALETTE_TINT_GRAYSCALE: // grey of brightness (2R + 4G + B) / 7
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
    case PALETTE_TINT_DIM_GRAY: // a third of R + G + B, B three steps darker
      for (i = 0; i < colorCount; i++) {
        color = *src++;
        red = color & 31;
        color = Iwram_SignedDivide(
            red + ((color >> 5) & 31) + ((color >> 10) & 31), 3);
        red = PaletteFade_ClampComponent(color - 1);
        green = PaletteFade_ClampComponent(color - 1);
        blue = PaletteFade_ClampComponent(color - 3);
        dst[0] = gColorBuffer_ChannelCurveLight[blue];
        dst[1] = gColorBuffer_ChannelCurveLight[green];
        dst[2] = gColorBuffer_ChannelCurveLight[red];
        dst += 3;
      }
      break;
    case PALETTE_TINT_SEPIA: // the brightness, red stronger, within 8 to
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
    case PALETTE_TINT_WARM: // half R + 10, two thirds G + 8, B - 7 (black
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
          red = PaletteFade_ClampComponent(red);
          green = PaletteFade_ClampComponent(green);
          blue = PaletteFade_ClampComponent(blue);
        }
        *dst++ = gColorBuffer_ChannelCurveContrast[blue];
        *dst++ = gColorBuffer_ChannelCurveLight[green];
        *dst++ = gColorBuffer_ChannelCurve[red];
      }
      break;
    case PALETTE_TINT_MUTED: // narrow the range, G and B + 2
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
        red = PaletteFade_ClampComponent(red);
        green = PaletteFade_ClampComponent(green);
        blue = PaletteFade_ClampComponent(blue);
        *dst++ = gColorBuffer_ChannelCurve[blue];
        *dst++ = gColorBuffer_ChannelCurve[green];
        *dst++ = gColorBuffer_ChannelCurve[red];
      }
      break;
    case PALETTE_TINT_PALE: // half of each, plus the mean
      for (i = 0; i < colorCount; i++) {
        s32 mean;

        color = *src++;
        red = color & 31;
        green = (color >> 5) & 31;
        blue = (color >> 10) & 31;
        mean = PaletteFade_ClampComponent((red + green + blue) / 3);
        red = (red >> 1) + mean;
        green = (green >> 1) + mean;
        blue = (blue >> 1) + mean;
        red = PaletteFade_ClampComponent(red);
        green = PaletteFade_ClampComponent(green);
        blue = PaletteFade_ClampComponent(blue);
        dst[0] = gColorBuffer_ChannelCurveContrast[blue];
        dst[1] = gColorBuffer_ChannelCurveContrast[green];
        dst[2] = gColorBuffer_ChannelCurveContrast[red];
        dst += 3;
      }
      break;
    case PALETTE_TINT_RED: // an eighth of G and B added to R; G and B two
                           // thirds
      for (i = 0; i < colorCount; i++) {
        color = *src++;
        red = color & 31;
        green = (color >> 5) & 31;
        blue = (color >> 10) & 31;
        red += ((green >> 3) + (blue >> 3));
        red = PaletteFade_ClampComponent(red);
        green -= green / 3;
        blue -= blue / 3;
        dst[0] = gColorBuffer_ChannelCurve[blue];
        dst[1] = gColorBuffer_ChannelCurve[green];
        dst[2] = gColorBuffer_ChannelCurveLight[red];
        dst += 3;
      }
      break;
    case PALETTE_TINT_WARM_DARK: // half R + 6, two thirds G + 4, B - 6
      for (i = 0; i < colorCount; i++) {
        color = *src++;
        red = color & 31;
        red -= (u32)red >> 1;
        green = (color >> 5) & 31;
        blue = (color >> 10) & 31;
        green -= green / 3;
        red += 6;
        green += 4;
        blue -= 6;
        red = PaletteFade_ClampComponent(red);
        green = PaletteFade_ClampComponent(green);
        blue = PaletteFade_ClampComponent(blue);
        dst[0] = gColorBuffer_ChannelCurveContrast[blue];
        dst[1] = gColorBuffer_ChannelCurveLight[green];
        dst[2] = gColorBuffer_ChannelCurve[red];
        dst += 3;
      }
      break;
    case PALETTE_TINT_DIM_BRIGHT: // a fifth off each channel of a bright colour
      for (i = 0; i < colorCount; i++) {
        color = *src++;
        red = color & 31;
        green = (color >> 5) & 31;
        blue = (color >> 10) & 31;
        if (red + green + blue > 60) {
          red -= red / 5;
          green -= green / 5;
          blue -= blue / 5;
        }
        red = PaletteFade_ClampComponent(red);
        green = PaletteFade_ClampComponent(green);
        blue = PaletteFade_ClampComponent(blue);
        *dst++ = gColorBuffer_ChannelCurve[blue];
        *dst++ = gColorBuffer_ChannelCurve[green];
        *dst++ = gColorBuffer_ChannelCurve[red];
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
  } else if (mode & PALETTE_TINT_CUSTOM) {
    // The brightness (2R + 4G + B) times the colour in mode.
    u32 tintGreen;
    u32 tintRed;
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
      red = PaletteFade_ClampShiftedComponent(red);
      green = PaletteFade_ClampShiftedComponent(green);
      blue = PaletteFade_ClampShiftedComponent(blue);
      dst[0] = blue;
      dst[1] = green;
      dst[2] = red;
      dst += 3;
    }
  } else if (mode & PALETTE_TINT_BLEND) {
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
      blendedRed = PaletteFade_ClampComponent((u32)blendedRed >> 16);
      blendedGreen = PaletteFade_ClampComponent((u32)blendedGreen >> 16);
      blendedBlue = PaletteFade_ClampComponent((u32)blendedBlue >> 16);
      *dst++ = gColorBuffer_ChannelCurve[blendedBlue];
      *dst++ = gColorBuffer_ChannelCurve[blendedGreen];
      *dst++ = gColorBuffer_ChannelCurve[blendedRed];
    }
  } else if (mode & PALETTE_TINT_SPLIT) {
    // The colours unchanged, as the default tint writes them.
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
