// Draft, differ 33 (48 before statements reordered 2026-10-26).
// Draft, differ 50 of 99: reserves a glyph sprite for the window as
// Func_0803c068 does, without drawing a glyph, and returns the address of its
// tiles in OBJ VRAM for the caller to fill. The 128-byte frame is the unused
// tile buffer of Func_0803c068. Left: register choice: the window and the output swap r6 and r7, the
// constant 1 passes through ip in the native, and the stores after the oam
// words follow in a different order. Tried: next cleared last, as in
// Func_0803c068 (same).
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
