/// @file
/// The debug palette editor.
// Part of field_sequence.c.

#include "field_sequence_internal.h"
#include "system/system.h"

#define PALETTE ((u16 *)BG_PLTT)
#define EDITOR_MAP ((u16 *)0x0600205a)
#define HEX_DIGIT 0xf0e0

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
  u16 *map;
  u16 *p;
  u16 *colors;
  u32 tile;
  u32 value;
  u32 red;
  u32 green;
  u32 blue;
  u32 i;

  Decompress_Halfwords(gDebug_PaletteSwatchTiles, (void *)0x06001a00);
redraw:
  tile = (palette << 12) + 0xd1;
  colors = &PALETTE[palette * 16 + 1];
  map = EDITOR_MAP;
  p = map;
  *p = HEX_DIGIT + palette;
  p += 32;
  *p = 0xf000 | 'R';
  p += 32;
  *p = 0xf000 | 'G';
  p += 32;
  *p = 0xf000 | 'B';
  map++;
  for (i = 1; i < 16; i++) {
    p = map;
    *p = tile++;
    value = *colors++;
    p += 32;
    *p = (value & 31) + HEX_DIGIT;
    p += 32;
    *p = RGB_GREEN(value) + HEX_DIGIT;
    p += 32;
    *p = RGB_BLUE(value) + HEX_DIGIT;
    map++;
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
        color = 15;
    }
    if (gInput.repeat & KEY_RIGHT) {
      color++;
      if (color > 15)
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
      colors = &PALETTE[palette * 16 + color];
      value = *colors;
      red = value & 31;
      green = (value >> 5) & 31;
      blue = (value >> 10) & 31;
      if (channel == 1 && red < RGB_LEVEL_MAX)
        red++;
      if (channel == 2 && green < RGB_LEVEL_MAX)
        green++;
      if (channel == 3 && blue < RGB_LEVEL_MAX)
        blue++;
      *colors = (blue << 10) | (green << 5) | red;
      goto redraw;
    }
    if (gInput.repeat & KEY_B) {
      colors = &PALETTE[palette * 16 + color];
      value = *colors;
      red = value & 31;
      green = (value >> 5) & 31;
      blue = (value >> 10) & 31;
      if (channel == 1 && red != 0)
        red--;
      if (channel == 2 && green != 0)
        green--;
      if (channel == 3 && blue != 0)
        blue--;
      *colors = (blue << 10) | (green << 5) | red;
      goto redraw;
    }
    if (gInput.repeat & KEY_START) {
      colors = &PALETTE[palette * 16 + color];
      value = *colors;
      red = 0;
      for (;;) {
        wait(1);
        if (!(gInput.held & KEY_START))
          break;
        if (red == 0)
          *colors = RGB(31, 31, 31);
        if (red == EDITOR_BLINK_FRAMES)
          *colors = value;
        if (red == EDITOR_BLINK_FRAMES * 2)
          *colors = 0;
        if (red == EDITOR_BLINK_FRAMES * 3)
          *colors = value;
        if (++red >= EDITOR_BLINK_FRAMES * 4)
          red = 0;
      }
      *colors = value;
    }
    if (gInput.repeat & KEY_SELECT)
      break;
    wait(1);
  }
  Window_LoadGraphics();
  DebugText_Clear();
}
