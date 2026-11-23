/// @file
/// The debug palette editor.
// Part of field_sequence.c.

#include "field_sequence_internal.h"
#include "system/system.h"
#include "window/window_tile.h"

/// The palettes, as plain halfwords, and where the editor draws: from column
/// 13 of row 1 of screen block 4.
#define EDITOR_PALETTE ((u16 *)BG_PLTT)
#define EDITOR_MAP ((u16 *)BG_SCREEN_BLOCK(4) + BG_SCREEN_WIDTH + 13)
/// The swatch tile sheet's tiles, from tile 208 of character block 0: colour n
/// is drawn with EDITOR_SWATCH_TILE + n, and the 32 tiles after the swatches
/// show the numbers 0 to 31.
#define EDITOR_SWATCH_TILE 208
/// The map entry of the tile showing the number 0, in WINDOW_PALETTE: number n
/// is EDITOR_NUMBER_TILE + n.
#define EDITOR_NUMBER_TILE WINDOW_TILE(EDITOR_SWATCH_TILE + PALETTE_COLORS)
/// The editor cycles through the map's 14 background palettes.
#define EDITOR_PALETTES 14
/// START blinks the colour every 10 frames: white, the colour, black, the
/// colour.
#define EDITOR_BLINK_FRAMES 10

/// The debug palette editor (L on the Pause menu in debug mode). Shows colours
/// 1 to 15 of a palette with their red, green and blue levels, 0 to 31.
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
  tile = BG_TILE_PALETTE(palette) + EDITOR_SWATCH_TILE + 1;
  paletteColor = &EDITOR_PALETTE[palette * PALETTE_COLORS + 1];
  swatchColumn = EDITOR_MAP;
  tileEntry = swatchColumn;
  *tileEntry = EDITOR_NUMBER_TILE + palette;
  tileEntry += BG_SCREEN_WIDTH;
  *tileEntry = WINDOW_TILE('R');
  tileEntry += BG_SCREEN_WIDTH;
  *tileEntry = WINDOW_TILE('G');
  tileEntry += BG_SCREEN_WIDTH;
  *tileEntry = WINDOW_TILE('B');
  swatchColumn++;
  for (i = 1; i < PALETTE_COLORS; i++) {
    tileEntry = swatchColumn;
    *tileEntry = tile++;
    rgb555 = *paletteColor++;
    tileEntry += BG_SCREEN_WIDTH;
    *tileEntry = RGB_RED(rgb555) + EDITOR_NUMBER_TILE;
    tileEntry += BG_SCREEN_WIDTH;
    *tileEntry = RGB_GREEN(rgb555) + EDITOR_NUMBER_TILE;
    tileEntry += BG_SCREEN_WIDTH;
    *tileEntry = RGB_BLUE(rgb555) + EDITOR_NUMBER_TILE;
    swatchColumn++;
  }
  wait(1);
  for (;;) {
    if (gInput.repeat & KEY_UP) {
      channel--;
      if (channel <= 0)
        channel = 3;
    }
    if (gInput.repeat & KEY_DOWN) {
      channel++;
      if (channel > 3)
        channel = 1;
    }
    if (gInput.repeat & KEY_LEFT) {
      color--;
      if (color <= 0)
        color = PALETTE_COLORS - 1;
    }
    if (gInput.repeat & KEY_RIGHT) {
      color++;
      if (color > PALETTE_COLORS - 1)
        color = 1;
    }
    if (gInput.repeat & KEY_L) {
      palette--;
      if (palette < 0)
        palette = EDITOR_PALETTES - 1;
      goto redraw;
    }
    if (gInput.repeat & KEY_R) {
      palette++;
      if (palette >= EDITOR_PALETTES)
        palette = 0;
      goto redraw;
    }
    if (gInput.repeat & KEY_A) {
      paletteColor = &EDITOR_PALETTE[palette * PALETTE_COLORS + color];
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
    if (gInput.repeat & KEY_B) {
      paletteColor = &EDITOR_PALETTE[palette * PALETTE_COLORS + color];
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
    if (gInput.repeat & KEY_START) {
      // red counts the frames here.
      paletteColor = &EDITOR_PALETTE[palette * PALETTE_COLORS + color];
      rgb555 = *paletteColor;
      red = 0;
      for (;;) {
        wait(1);
        if (!(gInput.held & KEY_START))
          break;
        if (red == 0)
          *paletteColor = RGB_WHITE;
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
    if (gInput.repeat & KEY_SELECT)
      break;
    // DEAD: a read of gFrameCount nothing uses; the ROM keeps its load.
    gFrameCount;
    wait(1);
  }
  Window_LoadGraphics();
  DebugText_Clear();
}

#undef EDITOR_PALETTE
#undef EDITOR_MAP
#undef EDITOR_NUMBER_TILE
#undef EDITOR_SWATCH_TILE
#undef EDITOR_PALETTES
#undef EDITOR_BLINK_FRAMES
