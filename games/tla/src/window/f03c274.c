#include "types.h"
#include "ram_buffer.h"
#include "window.h"
#include "uiwindow.h"
#include "output.h"
#include "resource.h"

/// Puts a character into a window's tilemap as a tile at tile (x, y) of its
/// interior, as ☀️'s Window_PutGlyph; GLYPH_DRAW_PAGE_ARROW instead shows the
/// next-page arrow sprite on the interior's last tile.
void Func_0803c274(struct Window *win, u32 tile, u32 x, u32 y, s32 mode) {
  struct WindowWork *base = (struct WindowWork *)Ram_HeapSlots->windowTiles;
  u16 *tilemap = base->tilemap[0];
  struct WindowSprite *out;
  s32 idx;
  u16 *slot;
  struct QueuedSprite *sprite;
  u32 pos;
  u16 row;

  if (y > (win->height - 2))
    return;
  if (x > (win->width - 2))
    return;
  if (mode == GLYPH_DRAW_PAGE_ARROW) {
    s32 column;
    out = WindowSprite_AcquireFree();
    if (out == NULL)
      return;
    idx = (out - base->outputs) * 4;
    out->active = WINDOW_SPRITE_MOTION_PAGE_ARROW;
    sprite = &out->sprite;
    slot = &base->glyphResource;
    if (*slot == NO_GLYPH_RESOURCE)
      *slot = Resource_FindFreeEntry();
    // The arrow sits on the interior's last tile, 4 pixels right and 1 up.
    // column holds its one constant, -2 as a halfword: added inline, it folds
    // into the width check above (52 of 116 instructions differ in tla-en).
    column = (u16)-2;
    sprite->oam.x = (win->x + (column + win->width)) * 8 + 4;
#ifdef UBFIX
    row = (u8)win->height + (u8)-2;
    row = (u8)win->y + row;
#else
    // UB: row is written twice with no sequence point between; as the two
    // statements above, 6 of 116 instructions differ in tla-en.
    row = (u8)win->y + (row = (u8)win->height + (u8)-2);
#endif
    sprite->oam.y = row * 8 - 1;
    out->x = sprite->oam.x;
    out->y = sprite->oam.y;
    out->index = idx;
    out->next = NULL;
    // Read as a byte: active is never negative, and the code loads it
    // unsigned (read signed, 5 of 116 instructions differ in tla-en).
    if ((u8)out->active == 0)
      out->active = mode;
    WindowSprite_AppendToList(&win->output, out);
  } else if (tile <= TEXT_LAST_FONT_CODE) {
    // Past the border's top-left tile, and only inside the tilemap.
    x++;
    y++;
    pos = ((s16)win->y + y) * 32 + ((s16)win->x + x);
    // The entry's address as an integer: tilemap[pos] takes its registers
    // the other way round (2 of 116 instructions differ in tla-en).
    if (pos < 32 * 20)
      *(u16 *)((u32)tilemap + (pos << 1)) = WINDOW_TILE(tile);
  }
}
