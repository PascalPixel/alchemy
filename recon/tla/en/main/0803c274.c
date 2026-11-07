// Matches: differ 0 of 116 (tla-en, 2026-11-07).
// The y byte read into a temporary before the index store replaced the
// barrier after the x copy (2 before, 8 on 2026-10-31).
// ☀️'s Window_PutGlyph (the international form) on this game's
// WindowWork, the tilemap address taken up front.
#include "types.h"
#include "ram_buffer.h"
#include "window.h"
#include "uiwindow.h"
#include "output.h"
#include "resource.h"

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

struct GlyphOutput {
  struct GlyphOutput *next;
  u8 kind;
  u8 active;
  u16 x;
  u16 y;
  u16 unk0a;
  u16 animStep;
  u8 index;
  u8 sentinel;
  struct GlyphCell attr;
};

void Func_0803c274(struct Window *win, u32 tile, u32 x, u32 y, s32 mode) {
  struct WindowWork *base =
      (struct WindowWork *)Ram_HeapSlots->windowTiles;
  void *work = base->tilemap;
  struct GlyphOutput *out;
  s32 idx;
  u16 *slot;
  struct GlyphCell *attr;
  u32 pos;
  u16 row;

  if (y > (win->height - 2))
    return;
  if (x > (win->width - 2))
    return;
  if (mode == 1) {
    s32 column;
    work = WindowSprite_AcquireFree();
    if (work == NULL)
      return;
    out = work;
    idx = ((struct WindowSprite *)out - base->outputs) * 4;
    out->active = 2;
    attr = &out->attr;
    slot = &base->glyphResource;
    if (*slot == 99)
      *slot = Resource_FindFreeEntry();
    column = 0xfffe;
    attr->x = (win->x + (column + *(u16 *)&win->width)) * 8 + 4;
    row = (u8)win->y + (row = (u8)win->height + 254);
    attr->y = row * 8 - 1;
    out->x = attr->x;
    {
      /* FAKEMATCH(temporary): the y byte read into yy before the index
       * store; as out->index = idx; out->y = attr->y;, 4 of 116
       * instructions differ in tla-en. */
      u8 yy = attr->y;
      out->index = idx;
      out->y = yy;
    }
    out->next = NULL;
    if (out->active == 0)
      out->active = mode;
    WindowSprite_AppendToList(&win->output, (struct WindowSprite *)out);
  } else if (tile <= 0xff) {
    x++;
    y++;
    pos = ((s16)win->y + y) * 32 + ((s16)win->x + x);
    if (pos < 32 * 20)
      *(u16 *)((u32)work + (pos << 1)) = tile | 0xf000;
  }
}
