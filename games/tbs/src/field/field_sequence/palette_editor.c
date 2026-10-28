/// @file
/// The debug palette editor.
// Part of field_sequence.c.

#include "../field_sequence_internal.h"

// The palettes, as plain halfwords, and where the editor draws: from column
// 13 of row 1 of screen block 4.
#define EDITOR_PALETTE ((u16 *)0x05000000)
#define EDITOR_MAP ((u16 *)0x0600205a)
#define EDITOR_HEX_DIGIT 0xf0e0 ///< the tile of digit 0, in WINDOW_PALETTE

/// The debug palette editor (L on the Pause menu in debug mode). Shows colours
/// 1 to 15 of a palette with their red, green and blue values in hex. Left and
/// right choose the colour, up and down the channel, L and R the palette (0 to
/// 13); A raises the channel and B lowers it, Start makes the colour blink
/// while held, and Select leaves. Called by FieldSequence_Run for the pause
/// request when L is held in debug mode. Writes the edited colours straight to
/// BG palette RAM and draws over screen block 4; on leaving it reloads the
/// window graphics and clears the debug text.
void PaletteEditor_Run(void) {
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

  // The swatch tiles go to tile 0xd0 of character block 0; colour n is drawn
  // with tile 0xd0 + n.
  Decompress_Halfwords(gDebug_PaletteSwatchTiles, (void *)0x06001a00);
redraw:
  // Column 0 shows the palette number and R, G, B; columns 1 to 15 show each
  // colour's swatch, drawn in the palette being edited, and its three
  // channels. The letters are tiles at their ASCII codes in WINDOW_PALETTE.
  tile = (palette << 12) + 0xd1;
  paletteColor = &EDITOR_PALETTE[palette * 16 + 1];
  swatchColumn = EDITOR_MAP;
  tileEntry = swatchColumn;
  *tileEntry = EDITOR_HEX_DIGIT + palette;
  tileEntry += 32;
  *tileEntry = WINDOW_TILE('R');
  tileEntry += 32;
  *tileEntry = WINDOW_TILE('G');
  tileEntry += 32;
  *tileEntry = WINDOW_TILE('B');
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
  wait(1);
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
        wait(1);
        if (!(gKeysHeld & KEY_START))
          break;
        if (red == 0)
          *paletteColor = RGB(31, 31, 31);
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
    wait(1);
  }
  Window_LoadGraphics();
  DebugText_Clear();
}

#undef EDITOR_PALETTE
#undef EDITOR_MAP
#undef EDITOR_HEX_DIGIT
