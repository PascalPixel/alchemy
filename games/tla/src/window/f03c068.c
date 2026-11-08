#include "types.h"
#include "io_reg.h"
#include "dma.h"
#include "ram_buffer.h"
#include "window.h"
#include "uiwindow.h"
#include "edition.h"

// Where the glyph's sprite sits against the text position, in pixels: the
// Japanese font's glyphs are drawn into their tiles differently (see
// Func_0803a8fc).
#if (LANGUAGE == LANGUAGE_JAPANESE)
#define GLYPH_SPRITE_OFFSET_X 1
#define GLYPH_SPRITE_OFFSET_Y 0
#else
#define GLYPH_SPRITE_OFFSET_X 2
#define GLYPH_SPRITE_OFFSET_Y (-2)
#endif

// The sprite cell at WindowSprite.sprite: a link, then the OAM words.
struct GlyphSprite {
  struct GlyphSprite *next;
  u16 y : 8;
  u16 attr0 : 8;
  u16 x : 9;
  u16 attr1 : 7;
  u16 tile;
};

s32 Func_0803a8fc(s32 code, u32 *tiles);

s32 Func_0803c068(struct Window *window, s32 character, s32 x, s32 y) {
  struct WindowWork *work = Ram_HeapSlots->windowTiles;
  u32 tiles[32];
  u32 spacingX = work->glyphSpacing;
  u32 spacingY = work->lineSpacing;
  struct WindowSprite *output;
  struct GlyphSprite *sprite;
  s32 index;
  s32 width;

  output = WindowSprite_AcquireFree();
  if (output == NULL)
    return 0;
  index = (output - work->outputs) * 4;
  output->active = 1;
  output->kind = 0;
  width = Func_0803a8fc(character, tiles);
  Dma_Set(tiles, (void *)(0x06010000 + (work->glyphTiles + index) * 32),
          0x84000020, REG_DMA3);
  ((u16 *)&output->sprite.oam)[0] =
      y + (spacingY >> 1) + window->y * 8 + GLYPH_SPRITE_OFFSET_Y;
  ((u16 *)&output->sprite.oam)[1] =
      (x + (spacingX >> 1) + window->x * 8 + GLYPH_SPRITE_OFFSET_X) | 0x4000;
  ((u16 *)&output->sprite.oam)[2] = work->glyphTiles + index;
  sprite = (struct GlyphSprite *)&output->sprite;
  output->sentinel = 253;
  output->x = sprite->x;
  output->y = sprite->y;
  output->index = index;
  output->next = NULL;
  WindowSprite_AppendToList(&window->output, output);
  return width;
}
