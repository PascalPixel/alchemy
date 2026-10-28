// Draft, differ 758 of 827 (tla-en): Palette_Transform, the palette fade's
// target builder, rewritten from The Broken Seal's matched
// TitlePaletteFade_BuildColors (games/tbs/src/menu/title_intro.c) with
// this game's tables (Data_081a877a, Data_081a87ba and Data_081a87fa for its
// three channel curves) and clamps. The listing has every case in The Broken
// Seal's form, including the warm tints' +6, +4 and -6 (the earlier draft's
// black test is not in it), except the blend tint, which writes its three
// channels with SET_VECTOR. Measured: the earlier draft 970; The Broken
// Seal's C as it stands 1007; the blend tint through SET_VECTOR 766; The
// Broken Seal's two XOR devices dropped 758. Left: register allocation. The
// native keeps source in r9, the colour index in r11 and each tint loop's 31
// in r10 across its calls, with a 48-byte stack frame; here source takes r10 and
// the stack frame is 44. permute (25 minutes) found nothing below 602 but
// meaningless rewrites.
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
        colorWork = Iwram_SignedDivide(
            red + ((colorWork >> 5) & 31) + ((colorWork >> 10) & 31), 10);
        red = colorWork * 4 + 5;
        blue = colorWork * 3 + 5;
        green = blue;
        if (red < 8)
          red = 8;
        if (blue < 8)
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
        red = colorWork & 31;
        green = (colorWork >> 5) & 31;
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
      red = colorWork * tintRed;
      green = colorWork * tintGreen;
      blue = colorWork * tintBlue;
      red = BattleEffect_ClampRgb555Component(red);
      green = BattleEffect_ClampRgb555Component(green);
      blue = BattleEffect_ClampRgb555Component(blue);
      SET_VECTOR(destination, blue, green, red);
      destination += 3;
    }
  } else if (mode & 0x400000) {
    u32 tintRed;
    u32 tintGreen;
    u32 tintBlue;

    tintRed = mode & 31;
    tintGreen = (mode >> 5) & 31;
    tintBlue = (mode >> 10) & 31;
    for (colorIndex = 0; colorIndex < colorCount; colorIndex++) {
      colorWork = *source++;
      red = colorWork & 31;
      colorWork = Iwram_SignedDivide(
          (red + ((colorWork >> 5) & 31) + ((colorWork >> 10) & 31)) << 4,
          tintRed + tintGreen + tintBlue);
      red = Iwram_MulQ16(((colorWork * tintRed) >> 4) << 16,
                         (s32)(tintRed << 16) >> 4);
      green = Iwram_MulQ16(((colorWork * tintGreen) >> 4) << 16,
                           (s32)(tintGreen << 16) >> 4);
      blue = Iwram_MulQ16(((colorWork * tintBlue) >> 4) << 16,
                          (s32)(tintBlue << 16) >> 4);
      red = BattleEffect_ClampRgb555Channel((u32)red >> 16);
      green = BattleEffect_ClampRgb555Channel((u32)green >> 16);
      blue = BattleEffect_ClampRgb555Channel((u32)blue >> 16);
      SET_VECTOR(destination, Data_081a877a[blue], Data_081a877a[green],
                 Data_081a877a[red]);
      destination += 3;
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
