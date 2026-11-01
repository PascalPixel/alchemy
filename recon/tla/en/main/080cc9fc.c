// Draft, differ 29 of 320, skeleton gap 13 (tla-en, 2026-11-01)
// The twin of ☀️'s PaletteEditor_Run: same keys, ranges, swatch tiles, map and
// RGB editing, reading this game's gInput (repeat for the keys, held for Start's
// blink), without the twin's unused gFrameCount read, and closing with
// Func_08014b70. The C is already the twin's form; the gap is one cause, not a
// shape. sched2's dump shows the redraw head's block holds only its two chains,
// (mov, lsl, str tile) and (mov, lsl, mov fp), at equal priority (3, 2, 1) here,
// so they alternate; the str costs 2 in the twin's build, priority 4, 3, 2, and
// the listing's mov, lsl, mov, str, lsl, mov fp needs such a chain. With the head
// identical to the L wrap's copy, jump2 cross-jumps the whole copy, losing the 5
// instructions the listing keeps (it merges only mov fp, as for R's wrap), and
// shifting the branches and one pool load. Also left: adds r2, r1 before the
// colour's ldrh in the swatch loop, a load cost too. Nothing in C reaches the
// block. Tried: statement order at the head and in the loop, declaration order,
// types, spellings of the shifts and of L's and R's wraps, no gain; a permute run
// reached skeleton 8 only with a forced temporary, a new device, refused.
#include "types.h"
#include "io_reg.h"

#include "input.h"

extern const u8 gDebug_PaletteSwatchTiles[];

void Resource_DecompressHalfwords(const void *source, void *destination);
void wait(s32 count);
void Window_LoadGraphics(void);
void Func_08014b70(void);

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
        palette = 13;
      goto redraw;
    }
    if (gInput.repeat & KEY_R) {
      palette++;
      if (palette > 13)
        palette = 0;
      goto redraw;
    }
    if (gInput.repeat & KEY_A) {
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
    if (gInput.repeat & KEY_SELECT)
      break;
    wait(1);
  }
  Window_LoadGraphics();
  Func_08014b70();
}
