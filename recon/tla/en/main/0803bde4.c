// Draft, differ 8 of 292 in tla-en under -mthumb-slow-loads (2026-10-26). No
// single empty asm, one-pass block, wrapping block or swap of neighbouring
// statements brings it closer (device search).
// Draft, differ 18 of 292 (2026-10-18): only scheduling remains; the native
// stores x before y's copy, takes the character and both spacing loads
// earlier, loads the window's x before it builds 0xfffe, and moves the
// window argument after the stores of y and next (the lane's load-earlier
// near misses). The RAM routine's size is a linker constant in the listing
// (0x31c): UiText_DrawGlyphCodeSize stands for it.
// ☀️'s UiText_DrawGlyph (the international form) on this game's
// UiRenderWork: a text canvas window draws the glyph with the routine copied
// to RAM, other windows show it as a sprite.
#include "types.h"
#include "dma.h"
#include "mem.h"
#include "ram_buffer.h"
#include "window.h"
#include "uiwindow.h"
#include "output.h"
#include "resource.h"
#include "ramcode.h"

struct GlyphCell {
  struct GlyphCell *next;
  u16 y : 8;
  u16 affine : 2;
  u16 mode : 2;
  u16 mosaic : 1;
  u16 colors : 1;
  u16 shape : 2;
  u16 x : 9;
  u16 affineIndex : 5;
  u16 size : 2;
};

typedef s32 (*GlyphRoutine)(struct UiWindow *, u32, u32, u32, const u8 *);

extern const u8 Data_080385e0[];
extern u8 UiText_DrawGlyphCodeSize[];
extern char ResourceId_WindowTiles;

s32 Func_0803a8fc(s32 character, u32 *tiles);

#define SPRITE_CELL(output) ((struct GlyphCell *)(output)->unk10)

s32 Func_0803bde4(struct UiWindow *window, s32 character, s32 x, s32 y,
                  s32 mode) {
  struct UiRenderWork *work = (struct UiRenderWork *)Ram_HeapSlots->windowTiles;
  u32 spacingX = work->glyphSpacing;
  s32 width;
  u32 spacingY = work->lineSpacing;

  if (mode != 1 && (window->flags & 8) != 0) {
    u8 *font;
    GlyphRoutine draw;
    s32 result;

    if (**(struct UiWindow ***)&((void **)Ram_HeapSlots)[37] == window) {
      Resource_GetTableEntry((s32)&ResourceId_WindowTiles);
      width = 3;
      if (character == ' ')
        goto done;
    }
    font = Resource_GetTableEntry((s32)&ResourceId_WindowTiles);
    width = 4;
    if (character != ' ') {
      RamCode_Load(draw, Data_080385e0, UiText_DrawGlyphCodeSize);
      result = draw(window, character, x, y, font);
      MemFree(draw);
      return result;
    }
  } else if (character == ' ') {
    width = 5;
  } else {
    u32 tiles[32];
    struct RenderOutput *output;
    struct GlyphCell *sprite;
    s32 index;

    output = RenderOutput_AcquireFree();
    if (output == NULL)
      return 0;
    index = (output - work->outputs) * 4;
    output->active = 1;
    output->kind = 0;
    if (mode == 1) {
      width = 1;
      output->active = 2;
    } else {
      switch (work->outline) {
      case 3:
        output->active = 5;
        break;
      case 4:
        output->active = 6;
        output->animStep = 8;
        break;
      case 5:
        output->active = 7;
        output->animStep = 0;
        break;
      case 2:
        output->active = 4;
        output->animStep = 0;
        break;
      }
      {
        s32 drawn = Func_0803a8fc(character, tiles);

        if (drawn == 0)
          drawn = 1;
        width = drawn;
      }
    }

    if ((u8)output->active == 2) {
      s32 column;
      u16 row;

      sprite = SPRITE_CELL(output);
      if (work->glyphResource == 99)
        work->glyphResource = Resource_FindFreeEntry();
      column = 0xfffe;
      sprite->x = (window->x + (column + window->width)) * 8 + 4;
      row = (u8)window->y + (row = (u8)window->height + 254);
      sprite->y = row * 8 - 1;
    } else {
      u16 *attr;

      Dma_Set(tiles, (void *)(0x06010000 + (work->glyphTiles + index) * 32),
              0x84000020, (volatile u32 *)0x040000d4);
      attr = (u16 *)&output->oam;
      if (work->menuState & 4)
        *attr++ = (y + (spacingY >> 1) + window->y * 8 - 2) | 0x400;
      else
        *attr++ = y + (spacingY >> 1) + window->y * 8 - 2;
      *attr = (x + (spacingX >> 1) + window->x * 8 + 2) | 0x4000;
      attr[1] = work->glyphTiles + index;
      sprite = SPRITE_CELL(output);
    }
    output->sentinel = 253;
    output->x = sprite->x;
    output->next = NULL;
    output->y = sprite->y;
    output->index = index;
    RenderOutput_AppendToList(&window->output, output);
  }
done:
  return width;
}
