// Differ 33 of 99 (tla-en, 2026-11-06).
// Register choice only. The
// native keeps the 0 for kind and next in r4 and passes the 1 for active through
// ip, computing the sprite cell (output + 16) late into r2; here the cell takes
// r4 early and the 0 goes through ip. Computing the cell late (as
// Func_0803c068 does) gives the native's r4 = 0 but swaps window and output
// (r7/r6) and keeps the 1 in r3: differ 46. Tried, 6 November: every order of
// the active, kind, vram, oam, cell and next statements (33 to 111); vram
// before the stores; next beside kind; one return at the end (50); a
// 4-minute permute (best 49).
// Also 6 November (second round): the cell late plus the 253 for sentinel
// set into a local right after the NULL test (a temporary) measures 16:
// window r6, output r7 and the 0 in r4 all as native, only the long-lived
// constant in ip is the 253 rather than the native's 1. So the native's 1 is
// born early (before index) and stored late or CSE-shared; the same local
// holding the 1 instead measures 62, and active stored last 35. Not kept: the
// device stops short of 0.
// Reserves a glyph sprite for the window as Func_0803c068 does, without
// drawing a glyph, and returns the address of its tiles in OBJ VRAM for the
// caller to fill. The 128-byte frame is the unused tile buffer of
// Func_0803c068.
#include "types.h"
#include "ram_buffer.h"
#include "window.h"
#include "uiwindow.h"

// The sprite cell at WindowSprite.unk10: a link, then the OAM words.
struct GlyphSprite {
  struct GlyphSprite *next;
  u16 y : 8;
  u16 attr0 : 8;
  u16 x : 9;
  u16 attr1 : 7;
  u16 tile;
};

void *Func_0803c198(struct Window *window, s32 character, s32 x, s32 y) {
  struct WindowWork *work = Ram_HeapSlots->windowTiles;
  u32 tiles[32];
  u32 spacingX = work->glyphSpacing;
  u32 spacingY = work->lineSpacing;
  struct WindowSprite *output;
  struct GlyphSprite *sprite;
  s32 index;
  void *vram;

  output = WindowSprite_AcquireFree();
  if (output == NULL)
    return NULL;
  index = (output - work->outputs) * 4;
  output->active = 1;
  output->kind = 0;
  sprite = (struct GlyphSprite *)output->unk10;
  vram = (void *)(0x06010000 + (work->glyphTiles + index) * 32);
  ((u16 *)&output->oam)[0] = y + (spacingY >> 1) + window->y * 8 - 2;
  ((u16 *)&output->oam)[1] =
      (x + (spacingX >> 1) + window->x * 8 + 2) | 0x4000;
  ((u16 *)&output->oam)[2] = work->glyphTiles + index;
  output->sentinel = 253;
  output->x = sprite->x;
  output->y = sprite->y;
  output->index = index;
  output->next = NULL;
  WindowSprite_AppendToList(&window->output, output);
  return vram;
}
