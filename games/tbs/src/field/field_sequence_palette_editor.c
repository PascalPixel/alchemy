/// @file
/// The debug palette editor.
// Part of field_sequence.c.

#include "field_sequence_internal.h"

// The palettes, as plain halfwords, and where the editor draws: from column
// 13 of row 1 of screen block 4.
#define EDITOR_PALETTE ((u16 *)PLTT)
#define EDITOR_MAP ((u16 *)BG_SCREEN_BLOCK(4) + 32 + 13)
#define EDITOR_HEX_DIGIT 0xf0e0 ///< the tile of digit 0, in WINDOW_PALETTE
#define EDITOR_MAP_ROW 32 ///< tile entries from one row of the map to the next
/// The swatch tiles, from tile 208 of character block 0: colour n is drawn with
/// EDITOR_SWATCH_TILE + n.
#define EDITOR_SWATCH_TILE 208
/// The editor cycles through the map's 14 background palettes.
#define EDITOR_PALETTES 14
/// START blinks the colour every 10 frames: white, the colour, black, the
/// colour.
#define EDITOR_BLINK_FRAMES 10

/// The debug palette editor (L on the Pause menu in debug mode). Shows colours
/// 1 to 15 of a palette with their red, green and blue values in hex.
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

  Decompress_Halfwords(gDebug_PaletteSwatchTiles,
                       BG_CHAR_BLOCK(0) + EDITOR_SWATCH_TILE * TILE_SIZE_4BPP);
redraw:
  // Column 0 shows the palette number and R, G, B; columns 1 to 15 show each
  // colour's swatch, drawn in the palette being edited, and its three channels.
  tile = (palette << 12) + EDITOR_SWATCH_TILE + 1;
  paletteColor = &EDITOR_PALETTE[palette * 16 + 1];
  swatchColumn = EDITOR_MAP;
  tileEntry = swatchColumn;
  *tileEntry = EDITOR_HEX_DIGIT + palette;
  tileEntry += EDITOR_MAP_ROW;
  *tileEntry = WINDOW_TILE('R');
  tileEntry += EDITOR_MAP_ROW;
  *tileEntry = WINDOW_TILE('G');
  tileEntry += EDITOR_MAP_ROW;
  *tileEntry = WINDOW_TILE('B');
  swatchColumn++;
  for (i = 1; i < 16; i++) {
    tileEntry = swatchColumn;
    *tileEntry = tile++;
    rgb555 = *paletteColor++;
    tileEntry += EDITOR_MAP_ROW;
    *tileEntry = RGB_RED(rgb555) + EDITOR_HEX_DIGIT;
    tileEntry += EDITOR_MAP_ROW;
    *tileEntry = RGB_GREEN(rgb555) + EDITOR_HEX_DIGIT;
    tileEntry += EDITOR_MAP_ROW;
    *tileEntry = RGB_BLUE(rgb555) + EDITOR_HEX_DIGIT;
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
        palette = EDITOR_PALETTES - 1;
      goto redraw;
    }
    if (gKeysRepeat & KEY_R) {
      palette++;
      if (palette >= EDITOR_PALETTES)
        palette = 0;
      goto redraw;
    }
    if (gKeysRepeat & KEY_A) {
      paletteColor = &EDITOR_PALETTE[palette * 16 + color];
      rgb555 = *paletteColor;
      red = RGB_RED(rgb555);
      green = RGB_GREEN(rgb555);
      blue = RGB_BLUE(rgb555);
      if (channel == 1 && red < RGB_LEVEL_MAX)
        red++;
      if (channel == 2 && green < RGB_LEVEL_MAX)
        green++;
      if (channel == 3 && blue < RGB_LEVEL_MAX)
        blue++;
      *paletteColor = RGB(red, green, blue);
      goto redraw;
    }
    if (gKeysRepeat & KEY_B) {
      paletteColor = &EDITOR_PALETTE[palette * 16 + color];
      rgb555 = *paletteColor;
      red = RGB_RED(rgb555);
      green = RGB_GREEN(rgb555);
      blue = RGB_BLUE(rgb555);
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
      // red counts the frames here.
      paletteColor = &EDITOR_PALETTE[palette * 16 + color];
      rgb555 = *paletteColor;
      red = 0;
      for (;;) {
        wait(1);
        if (!(gKeysHeld & KEY_START))
          break;
        if (red == 0)
          *paletteColor = RGB(31, 31, 31);
        if (red == EDITOR_BLINK_FRAMES)
          *paletteColor = rgb555;
        if (red == EDITOR_BLINK_FRAMES * 2)
          *paletteColor = 0;
        if (red == EDITOR_BLINK_FRAMES * 3)
          *paletteColor = rgb555;
        if (++red >= EDITOR_BLINK_FRAMES * 4)
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
#undef EDITOR_MAP_ROW
#undef EDITOR_SWATCH_TILE
#undef EDITOR_PALETTES
#undef EDITOR_BLINK_FRAMES
