#include "types.h"
#include "ram_buffer.h"
#include "window.h"
#include "uiwindow.h"
#include "edition.h"

// Where the glyph's sprite sits against the text position, in pixels, as in
// Func_0803c068: the Japanese font's glyphs are drawn into their tiles
// differently (see Func_0803a8fc).
#if (LANGUAGE == LANGUAGE_JAPANESE)
#define GLYPH_SPRITE_OFFSET_X 1
#define GLYPH_SPRITE_OFFSET_Y 0
#else
#define GLYPH_SPRITE_OFFSET_X 2
#define GLYPH_SPRITE_OFFSET_Y (-2)
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
  ((u16 *)&output->sprite.oam)[0] =
      y + (spacingY >> 1) + window->y * 8 + GLYPH_SPRITE_OFFSET_Y;
  ((u16 *)&output->sprite.oam)[1] =
      (x + (spacingX >> 1) + window->x * 8 + GLYPH_SPRITE_OFFSET_X) | 0x4000;
  ((u16 *)&output->sprite.oam)[2] = work->glyphTiles + index;
  /* FAKEMATCH(temporary): the cell read back through a pointer to output's
   * sprite; as output->sprite.oam.x and .y, 27 of 99 instructions differ in
   * tla-en. */
  sprite = &output->sprite;
  output->sentinel = 253;
  output->x = sprite->oam.x;
  output->y = sprite->oam.y;
  output->index = index;
  output->next = NULL;
  /* FAKEMATCH(dead): active stored a second time with the 1 it already holds;
   * as the first store alone, 46 of 99 instructions differ in tla-en. */
  output->active = 1;
  WindowSprite_AppendToList(&window->output, output);
  return vram;
}
