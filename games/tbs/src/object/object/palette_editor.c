// Part of object.c, the field object, compiled only through it.
// The debug palette editor.

#include "../object_internal.h"

// The palettes, as plain halfwords, and where the editor draws: from column
// 13 of row 1 of screen block 4.
#define EDITOR_PALETTE ((u16 *)0x05000000)
#define EDITOR_MAP ((u16 *)0x0600205a)
#define EDITOR_HEX_DIGIT 0xf0e0 // the tile of digit 0, in BG palette 15

// The debug palette editor (L on the pause menu in debug mode). Shows colours
// 1 to 15 of a palette with their red, green and blue values in hex. Left and
// right choose the colour, up and down the channel, L and R the palette (0 to
// 13); A raises the channel and B lowers it, Start makes the colour blink
// while held, and Select leaves.
void Debug_RunPaletteEditor(void) {
  s32 palette = 0;
  s32 channel = 1;
  s32 color = 1;
  u16 *swatchColumn;
  u16 *tileEntry;
  u16 *paletteColor;
  u32 tile;
  u32 rgb555;
  u32 red;
  u32 green;
  u32 blue;
  u32 i;

  Resource_DecompressHalfwords(gDebug_PaletteSwatchTiles, (void *)0x06001a00);
redraw:
  // Column 0 shows the palette number and R, G, B; columns 1 to 15 show each
  // colour's swatch and its three channels.
  tile = (palette << 12) + 0xd1;
  paletteColor = &EDITOR_PALETTE[palette * 16 + 1];
  swatchColumn = EDITOR_MAP;
  tileEntry = swatchColumn;
  *tileEntry = EDITOR_HEX_DIGIT + palette;
  tileEntry += 32;
  *tileEntry = 0xf000 | 'R';
  tileEntry += 32;
  *tileEntry = 0xf000 | 'G';
  tileEntry += 32;
  *tileEntry = 0xf000 | 'B';
  swatchColumn++;
  for (i = 1; i < 16; i++) {
    tileEntry = swatchColumn;
    *tileEntry = tile++;
    rgb555 = *paletteColor++;
    tileEntry += 32;
    *tileEntry = (rgb555 & 31) + EDITOR_HEX_DIGIT;
    tileEntry += 32;
    *tileEntry = ((rgb555 >> 5) & 31) + EDITOR_HEX_DIGIT;
    tileEntry += 32;
    *tileEntry = ((rgb555 >> 10) & 31) + EDITOR_HEX_DIGIT;
    swatchColumn++;
  }
  WaitFrames(1);
  for (;;) {
    if (gKeysRepeat & KEY_UP) {
      channel--;
      if (channel <= 0)
        channel = 3;
    }
    if (gKeysRepeat & KEY_DOWN) {
      channel++;
      if (channel > 3)
        channel = 1;
    }
    if (gKeysRepeat & KEY_LEFT) {
      color--;
      if (color <= 0)
        color = 15;
    }
    if (gKeysRepeat & KEY_RIGHT) {
      color++;
      if (color > 15)
        color = 1;
    }
    if (gKeysRepeat & KEY_L) {
      palette--;
      if (palette < 0)
        palette = 13;
      goto redraw;
    }
    if (gKeysRepeat & KEY_R) {
      palette++;
      if (palette > 13)
        palette = 0;
      goto redraw;
    }
    if (gKeysRepeat & KEY_A) {
      paletteColor = &EDITOR_PALETTE[palette * 16 + color];
      rgb555 = *paletteColor;
      red = rgb555 & 31;
      green = (rgb555 >> 5) & 31;
      blue = (rgb555 >> 10) & 31;
      if (channel == 1 && red < 31)
        red++;
      if (channel == 2 && green < 31)
        green++;
      if (channel == 3 && blue < 31)
        blue++;
      *paletteColor = RGB(red, green, blue);
      goto redraw;
    }
    if (gKeysRepeat & KEY_B) {
      paletteColor = &EDITOR_PALETTE[palette * 16 + color];
      rgb555 = *paletteColor;
      red = rgb555 & 31;
      green = (rgb555 >> 5) & 31;
      blue = (rgb555 >> 10) & 31;
      if (channel == 1 && red != 0)
        red--;
      if (channel == 2 && green != 0)
        green--;
      if (channel == 3 && blue != 0)
        blue--;
      *paletteColor = RGB(red, green, blue);
      goto redraw;
    }
    if (gKeysRepeat & KEY_START) {
      // Blink white, the colour, black, the colour, every 10 frames; red
      // counts the frames here.
      paletteColor = &EDITOR_PALETTE[palette * 16 + color];
      rgb555 = *paletteColor;
      red = 0;
      for (;;) {
        WaitFrames(1);
        if (!(gKeysHeld & KEY_START))
          break;
        if (red == 0)
          *paletteColor = 0x7fff;
        if (red == 10)
          *paletteColor = rgb555;
        if (red == 20)
          *paletteColor = 0;
        if (red == 30)
          *paletteColor = rgb555;
        if (++red >= 40)
          red = 0;
      }
      *paletteColor = rgb555;
    }
    if (gKeysRepeat & KEY_SELECT)
      break;
    gFrameCount; // a read whose value is not used
    WaitFrames(1);
  }
  Ui_LoadWindowGraphics();
  DebugText_Clear();
}

#undef EDITOR_PALETTE
#undef EDITOR_MAP
#undef EDITOR_HEX_DIGIT
