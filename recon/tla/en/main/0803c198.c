// Matches: differ 0 (tla-en, 2026-11-08)
// Also differ 0 in tla-de, tla-es, tla-fr and tla-it (99 instructions) and in
// tla-ja (95, at 0x0803c204): adoptable in all six editions.
// Two forms make it match. The cell is read through struct QueuedSprite
// (sprite->oam.x and .y): those loads depend on the sentinel store, so the
// tail schedules as the native's. And active is stored a second time after
// next (tagged dead): CSE shares the 1 between the two stores, so its
// register lives to the end; local-alloc puts it in r6, output (allocated
// first) takes r7, window kicks it out of r6 and it is reallocated to ip, and
// after reload the second store goes as a store of what memory already
// holds. That gives window r6, output r7 and the 1 in ip, as the native has.
// Without the second store output takes r6 and window r7: 46.
// Replaced, 8 November: the 253 temporary (temporary, 10 at best, the 253 in
// ip instead of the 1) and the once-only block over the OAM words (its loop
// note became a scheduling barrier before the cell loads). Earlier rounds (6
// to 8 November, 33 to 10) tried every order of the active, kind, vram, oam,
// cell and next statements, one return at the end, the 1 in a local set at
// each place, a reused local, asm operands and a pin; a permute from 10 found
// the second store.
// Reserves a glyph sprite for the window as Func_0803c068 does, without
// drawing a glyph, and returns the address of its tiles in OBJ VRAM for the
// caller to fill. The 128-byte frame is the unused tile buffer of
// Func_0803c068.
#include "types.h"
#include "ram_buffer.h"
#include "window.h"
#include "uiwindow.h"
#include "edition.h"

// The glyph sprite's offset from the text position, in pixels: the Japanese
// font's glyph sits 1 right of it, the Latin font's 2 right and 2 up.
// XXX: whether the font alone decides it; Func_0803c068's Japanese form would
// show the same pair.
#if (LANGUAGE != LANGUAGE_JAPANESE)
#define UNK_GLYPH_SPRITE_DX 2
#define UNK_GLYPH_SPRITE_DY (-2)
#else
#define UNK_GLYPH_SPRITE_DX 1
#define UNK_GLYPH_SPRITE_DY 0
#endif

void *Func_0803c198(struct Window *window, s32 character, s32 x, s32 y) {
  struct WindowWork *work = Ram_HeapSlots->windowTiles;
  // Unused: gives the stack frame its 128 bytes.
  u32 tiles[32];
  u32 spacingX = work->glyphSpacing;
  u32 spacingY = work->lineSpacing;
  struct WindowSprite *output;
  struct QueuedSprite *sprite;
  s32 index;
  void *vram;

  output = WindowSprite_AcquireFree();
  if (output == NULL)
    return NULL;
  index = (output - work->outputs) * 4;
  output->active = 1;
  output->kind = 0;
  vram = (void *)(0x06010000 + (work->glyphTiles + index) * 32);
  ((u16 *)&output->sprite.oam)[0] = y + (spacingY >> 1) + window->y * 8 + UNK_GLYPH_SPRITE_DY;
  ((u16 *)&output->sprite.oam)[1] =
      (x + (spacingX >> 1) + window->x * 8 + UNK_GLYPH_SPRITE_DX) | 0x4000;
  ((u16 *)&output->sprite.oam)[2] = work->glyphTiles + index;
  sprite = &output->sprite;
  output->sentinel = 253;
  output->x = sprite->oam.x;
  output->y = sprite->oam.y;
  output->index = index;
  output->next = NULL;
  /* FAKEMATCH(dead): active stored a second time with the 1 it already holds;
     as the first store alone, 46 of 99 instructions differ in tla-en. */
  output->active = 1;
  WindowSprite_AppendToList(&window->output, output);
  return vram;
}
