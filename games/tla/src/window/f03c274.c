#include "types.h"
#include "ram_buffer.h"
#include "window.h"
#include "uiwindow.h"
#include "output.h"
#include "resource.h"
#include "edition.h"
#include "text_font.h"

// Where the next-page arrow sits on the interior's last tile, in pixels right
// and down from its corner. It moves with the text: the Japanese glyph sprites
// also sit 1 pixel further left and 2 lower (Func_0803c068).
#if (LANGUAGE == LANGUAGE_JAPANESE)
#define PAGE_ARROW_OFFSET_X 3
#define PAGE_ARROW_OFFSET_Y 1
#else
#define PAGE_ARROW_OFFSET_X 4
#define PAGE_ARROW_OFFSET_Y (-1)
#endif

/// The window tiles a kana mark is drawn over differently, as ☀️'s;
/// Window_DrawDividerLine here joins its lines to the same two.
enum {
  WINDOW_TILE_DIVIDER_HORIZONTAL = WINDOW_TILE(14), ///< a horizontal divider
  WINDOW_TILE_BORDER_TOP = WINDOW_TILE(17)          ///< the frame's top edge
};

/// Puts a character into a window's tilemap as a tile at tile (x, y) of its
/// interior, as ☀️'s Window_PutGlyph; GLYPH_DRAW_PAGE_ARROW instead shows the
/// next-page arrow sprite on the interior's last tile. In Japanese, a kana mark
/// goes in the tile above the character before it.
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
    u16 left;
    out = WindowSprite_AcquireFree();
    if (out == NULL)
      return;
    idx = (out - base->outputs) * 4;
    out->active = WINDOW_SPRITE_MOTION_PAGE_ARROW;
    sprite = &out->sprite;
    slot = &base->glyphResource;
    if (*slot == NO_GLYPH_RESOURCE)
      *slot = Resource_FindFreeEntry();
    // column holds its one constant, -2 as a halfword: added inline, it folds
    // into the width check above (52 of 116 instructions differ in tla-en).
    column = (u16)-2;
    left = win->x;
    sprite->oam.x = (left + (column + win->width)) * 8 + PAGE_ARROW_OFFSET_X;
#ifdef UBFIX
    row = (u8)win->height + (u8)-2;
    row = (u8)win->y + row;
#else
    // UB: row is written twice with no sequence point between; as the two
    // statements above, 6 of 116 instructions differ in tla-en.
    row = (u8)win->y + (row = (u8)win->height + (u8)-2);
#endif
    sprite->oam.y = row * 8 + PAGE_ARROW_OFFSET_Y;
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
#if (LANGUAGE == LANGUAGE_JAPANESE)
    if (tile == TEXT_MARK_VOICED || tile == TEXT_MARK_SEMIVOICED) {
      // One up and one left of where a character at (x, y) goes. Over the
      // frame's top edge or a divider, the mark takes another tile.
      u32 line;

      line = ((s16)win->y + y) * 32;
      // XXX: what tiles 14, 15, 30 and 31 show in the Japanese editions; a
      // tla-ja render of a kana mark over a window's top edge and over a
      // divider would settle it.
      switch (tilemap[line + ((s16)win->x + x)]) {
      case WINDOW_TILE_BORDER_TOP:
        tile -= 192;
        break;
      case WINDOW_TILE_DIVIDER_HORIZONTAL:
        tile -= 208;
        break;
      }
    } else {
      // Past the border's top-left tile, and only inside the tilemap.
      x++;
      y++;
    }
#else
    // Past the border's top-left tile, and only inside the tilemap.
    x++;
    y++;
#endif
    pos = ((s16)win->y + y) * 32 + ((s16)win->x + x);
    // The entry's address as an integer: tilemap[pos] takes its registers
    // the other way round (2 of 116 instructions differ in tla-en).
    if (pos < 32 * 20)
      *(u16 *)((u32)tilemap + (pos << 1)) = WINDOW_TILE(tile);
  }
}
