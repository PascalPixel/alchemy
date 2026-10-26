// Draft, differ 8 of 116 (26 before, 2026-10-26): the index stored before y,
// and a barrier after the x copy (without it, 16). Left: the active store
// and the slot's store after Resource_FindFreeEntry each come a row after
// the native's, and the y byte's load a row before the index store.
// ☀️'s UiWindow_PutGlyph (the international form) on this game's
// UiRenderWork, the tilemap address taken up front.
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

void Func_0803c274(struct UiWindow *win, u32 tile, u32 x, u32 y, s32 mode) {
  struct UiRenderWork *base =
      (struct UiRenderWork *)Ram_HeapSlots->windowTiles;
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
    work = RenderOutput_AcquireFree();
    if (work == NULL)
      return;
    out = work;
    idx = ((struct RenderOutput *)out - base->outputs) * 4;
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
    asm("");
    out->index = idx;
    out->y = attr->y;
    out->next = NULL;
    if (out->active == 0)
      out->active = mode;
    RenderOutput_AppendToList(&win->output, (struct RenderOutput *)out);
  } else if (tile <= 0xff) {
    x++;
    y++;
    pos = ((s16)win->y + y) * 32 + ((s16)win->x + x);
    if (pos < 32 * 20)
      *(u16 *)((u32)work + (pos << 1)) = tile | 0xf000;
  }
}
