// Draft, differ 14 of 827, skeleton gap 6 (tla-en, 2026-11-01)
// Palette_Transform, the palette fade's target builder, from The Broken
// Seal's matched TitlePaletteFade_BuildColors (games/tbs/src/menu/title_intro.c)
// with this game's tables and clamps, no device. Registers and stack frame
// agree everywhere. Left, four scheduling pairs that decide nothing else:
// the selected-objects branch, where the listing has colorCount's 256 ahead of
// source's 512 but reload hands them r1 and r3 only in the order destination,
// source, colorCount (other orders swap them or move the add); the `ldr` of the
// next table after the first store, not before it, in cases 0x10003 and
// 0x10007 and after the second in 0x10006; and the restore of red after `mov
// r2, r8` in 0x10004.
// Tried on those without gain: every order of the three statements, colorCount
// halved or added to source, const or sized tables, const source, plain
// statements for SET_VECTOR, split increments in 0x10004.
// What the shape depends on: red after green in 0x10006 keeps colorWork out of
// blue's register; green and blue named in the sepia and blend sums keep blue's
// last use after green's, so CSE leaves blue holding the sum in sepia; green
// clamped through `green < 8` keeps sepia's second compare; tintedRed is shared
// by the tint and blend blocks so it takes r8; the blend's scaled values are
// local and clamped into new locals, which shift the table indexes in place.
#include "types.h"
#include "dma.h"
#include "iwram_call.h"
#include "macros.h"

extern u16 Data_081a877a[];
extern u16 Data_081a87ba[];
extern u16 Data_081a87fa[];
s32 BattleEffect_ClampRgb555Channel(s32 value);
s32 BattleEffect_ClampRgb555Component(s32 value);

void Palette_Transform(u32 mode, u16 *source, u16 *destination,
                       s32 selectedPalettes) {
  u32 colorIndex;
  u32 colorCount = 512;
  s32 red;
  s32 blue;
  u32 colorWork;
  s32 green;
  s32 tintedRed;

  if (mode == 0x8000)
    mode = *source;
  if (selectedPalettes == 1) {
    colorCount = 256;
  } else if (selectedPalettes == 2) {
    destination += 768;
    source += 256;
    colorCount = 256;
  }

  if (mode < 0x8000) {
    *destination++ = mode & 0x7c00;
    *destination++ = (mode & 0x03e0) << 5;
    *destination++ = (mode & 0x001f) << 10;
    Dma_Set(destination - 3, destination,
            DMA_ENABLE << 16 | (((colorCount - 1) * 6) >> 1), REG_DMA3);
  } else if (mode < 0x100000) {
    switch (mode) {
    case 0x10001:
      for (colorIndex = 0; colorIndex < colorCount; colorIndex++) {
        colorWork = *source++;
        red = ((colorWork << 11) & 0xf800);
        green = ((colorWork << 7) & 0x1f000);
        blue = (colorWork & 0x7c00);
        colorWork = Iwram_SignedDivide(red + green + blue, 7);
        *destination++ = colorWork;
        *destination++ = colorWork;
        *destination++ = colorWork;
      }
      break;
    case 0x10002:
      for (colorIndex = 0; colorIndex < colorCount; colorIndex++) {
        colorWork = *source++;
        red = colorWork & 31;
        green = (colorWork >> 5) & 31;
        blue = (colorWork >> 10) & 31;
        colorWork = Iwram_SignedDivide(red + green + blue, 10);
        red = colorWork * 4 + 5;
        blue = colorWork * 3 + 5;
        green = colorWork * 3 + 5;
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
        *destination++ = Data_081a87ba[blue];
        *destination++ = Data_081a87ba[green];
        *destination++ = Data_081a87ba[red];
      }
      break;
    case 0x10003:
      for (colorIndex = 0; colorIndex < colorCount; colorIndex++) {
        colorWork = *source++;
        red = colorWork & 31;
        green = (colorWork >> 5) & 31;
        blue = (colorWork >> 10) & 31;
        red -= (u32)red >> 1;
        green -= green / 3;
        red += 6;
        green += 4;
        blue -= 6;
        red = BattleEffect_ClampRgb555Channel(red);
        green = BattleEffect_ClampRgb555Channel(green);
        blue = BattleEffect_ClampRgb555Channel(blue);
        SET_VECTOR(destination, Data_081a87fa[blue],
                   Data_081a87ba[green],
                   Data_081a877a[red]);
        destination += 3;
      }
      break;
    case 0x10004:
      for (colorIndex = 0; colorIndex < colorCount; colorIndex++) {
        colorWork = *source++;
        red = colorWork & 31;
        green = (colorWork >> 5) & 31;
        blue = (colorWork >> 10) & 31;
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
        *destination++ = Data_081a877a[blue];
        *destination++ = Data_081a877a[green];
        *destination++ = Data_081a877a[red];
      }
      break;
    case 0x10005:
      for (colorIndex = 0; colorIndex < colorCount; colorIndex++) {
        s32 mean;

        colorWork = *source++;
        red = colorWork & 31;
        green = (colorWork >> 5) & 31;
        blue = (colorWork >> 10) & 31;
        mean = BattleEffect_ClampRgb555Channel((red + green + blue) / 3);
        red = (red >> 1) + mean;
        green = (green >> 1) + mean;
        blue = (blue >> 1) + mean;
        red = BattleEffect_ClampRgb555Channel(red);
        green = BattleEffect_ClampRgb555Channel(green);
        blue = BattleEffect_ClampRgb555Channel(blue);
        SET_VECTOR(destination, Data_081a87fa[blue],
                   Data_081a87fa[green],
                   Data_081a87fa[red]);
        destination += 3;
      }
      break;
    case 0x10006:
      for (colorIndex = 0; colorIndex < colorCount; colorIndex++) {
        colorWork = *source++;
        green = (colorWork >> 5) & 31;
        red = colorWork & 31;
        blue = (colorWork >> 10) & 31;
        red += ((green >> 3) + (blue >> 3));
        red = BattleEffect_ClampRgb555Channel(red);
        green -= green / 3;
        blue -= blue / 3;
        SET_VECTOR(destination, Data_081a877a[blue],
                   Data_081a877a[green],
                   Data_081a87ba[red]);
        destination += 3;
      }
      break;
    case 0x10007:
      for (colorIndex = 0; colorIndex < colorCount; colorIndex++) {
        colorWork = *source++;
        red = colorWork & 31;
        green = (colorWork >> 5) & 31;
        blue = (colorWork >> 10) & 31;
        red -= (u32)red >> 1;
        green -= green / 3;
        red += 6;
        green += 4;
        blue -= 6;
        red = BattleEffect_ClampRgb555Channel(red);
        green = BattleEffect_ClampRgb555Channel(green);
        blue = BattleEffect_ClampRgb555Channel(blue);
        SET_VECTOR(destination, Data_081a87fa[blue],
                   Data_081a87ba[green],
                   Data_081a877a[red]);
        destination += 3;
      }
      break;
    default:
      for (colorIndex = 0; colorIndex < colorCount; colorIndex++) {
        colorWork = *source++;
        destination[colorIndex * 3 + 0] = colorWork & 0x7c00;
        destination[colorIndex * 3 + 1] = (colorWork & 0x03e0) << 5;
        destination[colorIndex * 3 + 2] = (colorWork & 0x001f) << 10;
      }
      break;
    }
  } else if (mode & 0x200000) {
    u32 tintRed;
    u32 tintGreen;
    u32 tintBlue;

    tintRed = mode & 31;
    tintGreen = (mode >> 5) & 31;
    tintBlue = (mode >> 10) & 31;
    for (colorIndex = 0; colorIndex < colorCount; colorIndex++) {
      colorWork = *source++;
      red = ((colorWork << 11) & 0xf800);
      green = ((colorWork << 7) & 0x1f000);
      blue = (colorWork & 0x7c00);
      colorWork = Iwram_SignedDivide(red + green + blue, 96);
      tintedRed = colorWork * tintRed;
      green = colorWork * tintGreen;
      blue = colorWork * tintBlue;
      tintedRed = BattleEffect_ClampRgb555Component(tintedRed);
      green = BattleEffect_ClampRgb555Component(green);
      blue = BattleEffect_ClampRgb555Component(blue);
      destination[colorIndex * 3 + 0] = blue;
      destination[colorIndex * 3 + 1] = green;
      destination[colorIndex * 3 + 2] = tintedRed;
    }
  } else if (mode & 0x400000) {
    u32 tintRed;
    u32 tintGreen;
    u32 tintBlue;
    s32 scaledGreen;
    s32 scaledBlue;
    s32 clampedRed;
    s32 clampedGreen;
    s32 clampedBlue;

    tintRed = mode & 31;
    tintGreen = (mode >> 5) & 31;
    tintBlue = (mode >> 10) & 31;
    for (colorIndex = 0; colorIndex < colorCount; colorIndex++) {
      colorWork = *source++;
      red = colorWork & 31;
      green = (colorWork >> 5) & 31;
      blue = (colorWork >> 10) & 31;
      colorWork = Iwram_SignedDivide((red + green + blue) << 4,
                                     tintRed + tintGreen + tintBlue);
      tintedRed = Iwram_MulQ16(((colorWork * tintRed) >> 4) << 16,
                               (s32)(tintRed << 16) >> 4);
      scaledGreen = Iwram_MulQ16(((colorWork * tintGreen) >> 4) << 16,
                                 (s32)(tintGreen << 16) >> 4);
      scaledBlue = Iwram_MulQ16(((colorWork * tintBlue) >> 4) << 16,
                                (s32)(tintBlue << 16) >> 4);
      tintedRed = (u32)tintedRed >> 16;
      clampedRed = BattleEffect_ClampRgb555Channel(tintedRed);
      scaledGreen = (u32)scaledGreen >> 16;
      clampedGreen = BattleEffect_ClampRgb555Channel(scaledGreen);
      scaledBlue = (u32)scaledBlue >> 16;
      clampedBlue = BattleEffect_ClampRgb555Channel(scaledBlue);
      destination[colorIndex * 3 + 0] = Data_081a877a[clampedBlue];
      destination[colorIndex * 3 + 1] = Data_081a877a[clampedGreen];
      destination[colorIndex * 3 + 2] = Data_081a877a[clampedRed];
    }
  } else if (mode & 0x800000) {
    for (colorIndex = 0; colorIndex < colorCount; colorIndex++) {
      colorWork = *source++;
      destination[colorIndex * 3 + 0] = colorWork & 0x7c00;
      destination[colorIndex * 3 + 1] = (colorWork & 0x03e0) << 5;
      destination[colorIndex * 3 + 2] = (colorWork & 0x001f) << 10;
    }
  } else {
    if (selectedPalettes == 2)
      mode += 0x600;
    Dma_Set((const void *)mode, destination,
            DMA_COPY_WORDS((colorCount * 6) >> 2), REG_DMA3);
  }
}
