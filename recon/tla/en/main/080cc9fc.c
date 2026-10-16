#include "types.h"
#include "io_reg.h"

extern volatile u32 gKeysRepeat;
extern volatile u32 gData_03001ae8;
extern volatile u32 gData_03001e40;

extern const u8 gDebug_PaletteSwatchTiles[];

void Resource_DecompressHalfwords(const void *source, void *destination);
void WaitFrames(s32 count);
void Ui_LoadWindowGraphics(void);
void DebugText_Clear(void);

#define PALETTE ((u16 *)0x05000000)
#define EDITOR_MAP ((u16 *)0x0600205a)
#define HEX_DIGIT 0xf0e0

void Debug_RunPaletteEditor(void) {
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

  Resource_DecompressHalfwords(gDebug_PaletteSwatchTiles, (void *)0x06001a00);
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
    *p = ((value >> 5) & 31) + HEX_DIGIT;
    p += 32;
    *p = ((value >> 10) & 31) + HEX_DIGIT;
    map++;
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
      colors = &PALETTE[palette * 16 + color];
      value = *colors;
      red = value & 31;
      green = (value >> 5) & 31;
      blue = (value >> 10) & 31;
      if (channel == 1 && red < 31)
        red++;
      if (channel == 2 && green < 31)
        green++;
      if (channel == 3 && blue < 31)
        blue++;
      *colors = (blue << 10) | (green << 5) | red;
      goto redraw;
    }
    if (gKeysRepeat & KEY_B) {
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
    if (gKeysRepeat & KEY_START) {
      colors = &PALETTE[palette * 16 + color];
      value = *colors;
      red = 0;
      for (;;) {
        WaitFrames(1);
        if (!(gData_03001ae8 & 8))
          break;
        if (red == 0)
          *colors = 0x7fff;
        if (red == 10)
          *colors = value;
        if (red == 20)
          *colors = 0;
        if (red == 30)
          *colors = value;
        if (++red >= 40)
          red = 0;
      }
      *colors = value;
    }
    if (gKeysRepeat & KEY_SELECT)
      break;
    gData_03001e40;
    WaitFrames(1);
  }
  Ui_LoadWindowGraphics();
  DebugText_Clear();
}
