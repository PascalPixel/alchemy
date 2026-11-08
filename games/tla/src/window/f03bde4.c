#include "types.h"
#include "dma.h"
#include "heap_release.h"
#include "ram_buffer.h"
#include "window.h"
#include "uiwindow.h"
#include "output.h"
#include "resource.h"
#include "ram_code.h"

typedef s32 (*GlyphRoutine)(struct Window *, u32, u32, u32, const u8 *);

extern const u8 Glyph_DrawCode[];
extern u8 Glyph_DrawCodeSize[];
extern char ResourceId_WindowTiles;

s32 Func_0803a8fc(s32 character, u32 *tiles);

s32 Func_0803bde4(struct Window *window, s32 character, s32 x, s32 y,
                  s32 mode) {
  struct WindowWork *work = (struct WindowWork *)Ram_HeapSlots->windowTiles;
  u32 spacingX = work->glyphSpacing;
  s32 width;
  u32 spacingY = work->lineSpacing;

  if (mode != 1 && (window->flags & 8) != 0) {
    u8 *font;
    GlyphRoutine draw;
    s32 result;

    if (**(struct Window ***)&((void **)Ram_HeapSlots)[37] == window) {
      Resource_GetTableEntry((s32)&ResourceId_WindowTiles);
      width = 3;
      if (character == ' ')
        goto done;
    }
    font = Resource_GetTableEntry((s32)&ResourceId_WindowTiles);
    width = 4;
    if (character != ' ') {
      RamCode_Load(draw, Glyph_DrawCode, Glyph_DrawCodeSize);
      result = draw(window, character, x, y, font);
      Heap_Release(draw);
      return result;
    }
  } else if (character == ' ') {
    width = 5;
  } else {
    u32 tiles[32];
    struct WindowSprite *output;
    struct QueuedSprite *sprite;
    s32 index;

    output = WindowSprite_AcquireFree();
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
      sprite = &output->sprite;
      if (work->glyphResource == 99)
        work->glyphResource = Resource_FindFreeEntry();
      sprite->oam.x = (window->x << 3) + ((window->width - 2) << 3) + 4;
      sprite->oam.y = (window->y << 3) + ((window->height - 2) << 3) - 1;
    } else {
      u16 *attr;

      Dma_Set(tiles, (void *)(0x06010000 + (work->glyphTiles + index) * 32),
              0x84000020, (volatile u32 *)0x040000d4);
      attr = (u16 *)&output->sprite.oam;
      if (work->menuState & 4)
        *attr++ = (y + (spacingY >> 1) + window->y * 8 - 2) | 0x400;
      else
        *attr++ = y + (spacingY >> 1) + window->y * 8 - 2;
      *attr = (x + (spacingX >> 1) + window->x * 8 + 2) | 0x4000;
      attr[1] = work->glyphTiles + index;
      sprite = &output->sprite;
    }
    output->sentinel = 253;
    output->x = sprite->oam.x;
    output->y = sprite->oam.y;
    output->index = index;
    output->next = NULL;
    WindowSprite_AppendToList(&window->output, output);
  }
done:
  return width;
}
