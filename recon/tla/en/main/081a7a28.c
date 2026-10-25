// Draft, differ 970 of 827: Palette_Transform, the palette fade's target
// builder, ported from ☀️'s PaletteFade_Build (games/tbs/src/overworld/
// overworld/color.c) for this game's 256 BG and 256 OBJ colours, with
// mode 0x8000 taking the source's first colour.
// The same cases, calls and control flow; left: register allocation
// throughout (the native keeps src in r9 and dst in r8, and its frame is 8
// bytes larger, 48 against 40), which the alignment counts in every case.
#include "types.h"
#include "dma.h"
#include "iwram_call.h"

extern u16 Data_081a877a[];
extern u16 Data_081a87ba[];
extern u16 Data_081a87fa[];
s32 BattleEffect_ClampRgb555Channel(s32 value);
s32 BattleEffect_ClampRgb555Component(s32 value);

void Palette_Transform(u32 mode, u16 *src, u16 *dst, s32 half) {
  u32 i;
  u32 colorCount = 512;
  s32 red;
  s32 blue;
  u32 color;
  s32 green;

  if (mode == 0x8000)
    mode = *src;
  if (half == 1) {
    colorCount = 256;
  } else if (half == 2) {
    dst += 256 * 3;
    src += 256;
    colorCount = 256;
  }

  // One colour: write it once, then let DMA copy it over the rest. The copy
  // reads three halfwords behind where it writes, so the colour repeats.
  if (mode < 0x8000) {
    *dst++ = mode & 0x7c00;
    *dst++ = (mode & 0x03e0) << 5;
    *dst++ = (mode & 0x001f) << 10;
    Dma_Set(dst - 3, dst, (((colorCount - 1) * 6) >> 1) | (DMA_ENABLE << 16),
            REG_DMA3);
  } else if (mode < 0x100000) {
    switch (mode) {
    case 0x10001: // grey of brightness (2R + 4G + B) / 7
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
    case 0x10002: // the brightness, red stronger, within 8 to
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
        *dst++ = Data_081a87ba[blue];
        *dst++ = Data_081a87ba[green];
        *dst++ = Data_081a87ba[red];
      }
      break;
    case 0x10003: // half R + 10, two thirds G + 8, B - 7 (black
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
          red = BattleEffect_ClampRgb555Channel(red);
          green = BattleEffect_ClampRgb555Channel(green);
          blue = BattleEffect_ClampRgb555Channel(blue);
        }
        *dst++ = Data_081a87fa[blue];
        *dst++ = Data_081a87ba[green];
        *dst++ = Data_081a877a[red];
      }
      break;
    case 0x10004: // narrow the range, G and B + 2
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
        red = BattleEffect_ClampRgb555Channel(red);
        green = BattleEffect_ClampRgb555Channel(green);
        blue = BattleEffect_ClampRgb555Channel(blue);
        *dst++ = Data_081a877a[blue];
        *dst++ = Data_081a877a[green];
        *dst++ = Data_081a877a[red];
      }
      break;
    case 0x10005: // half of each, plus the mean
      for (i = 0; i < colorCount; i++) {
        s32 mean;

        color = *src++;
        red = color & 31;
        green = (color >> 5) & 31;
        blue = (color >> 10) & 31;
        mean = BattleEffect_ClampRgb555Channel((red + green + blue) / 3);
        red = (red >> 1) + mean;
        green = (green >> 1) + mean;
        blue = (blue >> 1) + mean;
        red = BattleEffect_ClampRgb555Channel(red);
        green = BattleEffect_ClampRgb555Channel(green);
        blue = BattleEffect_ClampRgb555Channel(blue);
        dst[0] = Data_081a87fa[blue];
        dst[1] = Data_081a87fa[green];
        dst[2] = Data_081a87fa[red];
        dst += 3;
      }
      break;
    case 0x10006: // an eighth of G and B added to R; G and B two
                  // thirds
      for (i = 0; i < colorCount; i++) {
        color = *src++;
        red = color & 31;
        green = (color >> 5) & 31;
        blue = (color >> 10) & 31;
        red += ((green >> 3) + (blue >> 3));
        red = BattleEffect_ClampRgb555Channel(red);
        green -= green / 3;
        blue -= blue / 3;
        dst[0] = Data_081a877a[blue];
        dst[1] = Data_081a877a[green];
        dst[2] = Data_081a87ba[red];
        dst += 3;
      }
      break;
    case 0x10007: // half R + 6, two thirds G + 4, B - 6
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
        red = BattleEffect_ClampRgb555Channel(red);
        green = BattleEffect_ClampRgb555Channel(green);
        blue = BattleEffect_ClampRgb555Channel(blue);
        dst[0] = Data_081a87fa[blue];
        dst[1] = Data_081a87ba[green];
        dst[2] = Data_081a877a[red];
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
  } else if (mode & 0x200000) {
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
      red = BattleEffect_ClampRgb555Component(red);
      green = BattleEffect_ClampRgb555Component(green);
      blue = BattleEffect_ClampRgb555Component(blue);
      dst[0] = blue;
      dst[1] = green;
      dst[2] = red;
      dst += 3;
    }
  } else if (mode & 0x400000) {
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
      blendedRed = BattleEffect_ClampRgb555Channel((u32)blendedRed >> 16);
      blendedGreen = BattleEffect_ClampRgb555Channel((u32)blendedGreen >> 16);
      blendedBlue = BattleEffect_ClampRgb555Channel((u32)blendedBlue >> 16);
      *dst++ = Data_081a877a[blendedBlue];
      *dst++ = Data_081a877a[blendedGreen];
      *dst++ = Data_081a877a[blendedRed];
    }
  } else if (mode & 0x800000) {
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
      mode += 256 * 3 * sizeof(u16);
    Dma_Set((const void *)mode, dst,
            ((colorCount * 6) >> 2) | (DMA_ENABLE | DMA_32BIT) << 16, REG_DMA3);
  }
}
