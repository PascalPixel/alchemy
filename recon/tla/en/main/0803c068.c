// Draft, differ 12 of 119: draws one text glyph as a 16x16 sprite carried by
// the window, as the sprite branch of ☀️ UiText_DrawGlyph
// (games/tbs/src/window/window.c) with no text style animation and sentinel
// 253. Left: instruction order only, in three places: the y argument's spill
// against the frame's first stores, the line spacing's constant built before
// the glyph spacing's spill, and the sprite cell pointer (output + 16) set
// up after the tile store instead of before it. Tried: the cell pointer
// taken before the tile store (same).
#include "types.h"
#include "io_reg.h"
#include "dma.h"
#include "ram_buffer.h"
#include "window.h"
#include "uiwindow.h"

// The sprite cell at RenderOutput.unk10: a link, then the OAM words.
struct GlyphSprite {
  struct GlyphSprite *next;
  u16 y : 8;
  u16 attr0 : 8;
  u16 x : 9;
  u16 attr1 : 7;
  u16 tile;
};

s32 Func_0803a8fc(s32 code, u32 *tiles);

s32 Func_0803c068(struct UiWindow *window, s32 character, s32 x, s32 y) {
  struct UiRenderWork *work = Ram_HeapSlots->windowTiles;
  u32 tiles[32];
  u32 spacingX = work->glyphSpacing;
  u32 spacingY = work->lineSpacing;
  struct RenderOutput *output;
  struct GlyphSprite *sprite;
  s32 index;
  s32 width;

  output = RenderOutput_AcquireFree();
  if (output == NULL)
    return 0;
  index = (output - work->outputs) * 4;
  output->active = 1;
  output->kind = 0;
  width = Func_0803a8fc(character, tiles);
  Dma_Set(tiles, (void *)(0x06010000 + (work->glyphTiles + index) * 32),
          0x84000020, REG_DMA3);
  ((u16 *)&output->oam)[0] = y + (spacingY >> 1) + window->y * 8 - 2;
  ((u16 *)&output->oam)[1] =
      (x + (spacingX >> 1) + window->x * 8 + 2) | 0x4000;
  ((u16 *)&output->oam)[2] = work->glyphTiles + index;
  sprite = (struct GlyphSprite *)output->unk10;
  output->sentinel = 253;
  output->x = sprite->x;
  output->y = sprite->y;
  output->index = index;
  output->next = NULL;
  RenderOutput_AppendToList(&window->output, output);
  return width;
}
