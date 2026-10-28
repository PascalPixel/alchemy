/// @file
/// The party status window.
// Part of screens.c.

#include "../screens_internal.h"

// UB: the standby counts pass four of Window_PutGlyph's five arguments, so
// its mode is whatever the stack slot of the fifth holds; UBFIX passes 0, as
// the summon lists do.
#ifdef UBFIX
#define PARTY_STATUS_PUT_DIGIT(window, glyph, x, y)                            \
  Window_PutGlyph(window, glyph, x, y, GLYPH_DRAW_CHARACTER)
#else
#define PARTY_STATUS_PUT_DIGIT(window, glyph, x, y)                            \
  Window_PutGlyph(window, glyph, x, y)
#endif

// The party status window (enum PartyStatusFlag, ui.h) sits at the top right
// of the screen.
#define PARTY_STATUS_COLUMN_WIDTH 6
#define PARTY_STATUS_DJINN_WIDTH 5

/// The address of pixel row 7 - `up` of BG tile `tile` (4bpp, 32 bytes a tile,
/// 4 bytes a row): the status bars are drawn from a tile's bottom row up.
#define TILE_ROW_FROM_BOTTOM(tile, up)                                         \
  ((u32 *)((tile) * 32 - (up) * 4 + 0x0600001c))

/// Works out where the party status window goes for `flags`: one column per
/// party member (those in battle when a battle is on), plus the Djinn column,
/// right-aligned on the 30-tile screen row at the top. It is four tiles high
/// outside battle and three in battle, plus one with the PP row.
void PartyStatus_BuildLayoutBounds(s32 flags) {
  // UB: slot[-1] steps before the single party-status heap slot. Reading
  // the actual members grows the EN function from 0x68 to 0x6c bytes.
  // Keep the original address formation only in the matching build.
#ifdef UBFIX
  struct PartyStatusWindow *layout = HEAP_SLOT_BLOCK(HEAP_SLOT_PARTY_STATUS);
  struct WindowWork *work = WINDOW_WORK;
#else
  void **slot = (void **)&gPartyStatusWork;
  struct PartyStatusWindow *layout = *slot;
  struct WindowWork *work = slot[-1];
#endif
  s32 height = 4;
  s32 members;
  s32 width;
  s32 x;

  if (work->battleMode != WINDOW_FIELD) {
    members = BattleParty_PrepareMembersFar(0);
    height = 3;
  } else {
    members = Party_CountMembersFar();
  }
  if (flags & PARTY_STATUS_SHOW_PP)
    height++;
  else
    flags &= ~PARTY_STATUS_SHOW_DJINN;

  members *= PARTY_STATUS_COLUMN_WIDTH;
  width = members + 1;
  if (flags & PARTY_STATUS_SHOW_DJINN)
    width += PARTY_STATUS_DJINN_WIDTH;

  x = 30;
  x -= width;
  layout->x = x;
  layout->y = 0;
  layout->width = width;
  layout->height = height;
  layout->flags = flags;
}

/// Opens the party status window with `flags` (see PARTY_STATUS_*) in a new
/// heap block, HEAP_SLOT_PARTY_STATUS, which PartyStatus_Close frees, and draws
/// it; the tilemap upload is held back (holdTilemapUpload) while it is drawn. A
/// battle opens it with flags 9, which lays it out with the PP row and leaves
/// it erased until the battle draws it (Battle_Run); the field opens it with
/// flags 1 (overworld/overworld/triggers.c).
void PartyStatus_CreateWindow(s32 flags) {
  s32 zero;
  struct PartyStatusWindow *layout;
  s8 *busy;

  layout = HeapSlot_AllocEwram(HEAP_SLOT_PARTY_STATUS, sizeof(*layout));
  busy = (s8 *)&WINDOW_WORK->holdTilemapUpload;
  zero = 0;
  *busy = 1;
  PartyStatus_BuildLayoutBounds(flags);
  layout->window =
      Window_Create(layout->x, layout->y, layout->width, layout->height, 6);
  PartyStatus_Draw(flags);
  *busy = zero;
}

/// Draws the vertical lines between the party members' columns of the party
/// status `window`, with their top and bottom joints, at the offsets listed in
/// gWindow_PartyColumnOffsets (shifted right past the Djinn column, whose own
/// line comes first in the list). In battle it also draws the window's bottom
/// edge.
void Window_DrawColumnBorders(struct Window *window, u32 flags) {
  struct WindowWork *work = WINDOW_WORK;
  s32 first = 1;
  s32 shift = 0;
  u32 lastColumn = window->width - 1;
  s32 i;
  u32 rows = window->height;
  u32 column;
  u32 row;
  u16 *cell;

  if ((flags & PARTY_STATUS_SHOW_PP) == 0)
    flags &= ~PARTY_STATUS_SHOW_DJINN;
  if (flags & PARTY_STATUS_SHOW_DJINN) {
    shift = PARTY_STATUS_DJINN_WIDTH;
    first = 0;
  }
  i = first;
  while (gWindow_PartyColumnOffsets[i] >= 0) {
    column = gWindow_PartyColumnOffsets[i] + shift;
    if (column < lastColumn) {
      for (row = 0; row != rows; row++) {
        cell = (u16 *)(((window->y + row) * 32 + (window->x + column)) * 2 +
                       (u32)work->tilemap);
        if (row == 0)
          *cell = WINDOW_TILE_BORDER_TOP_TEE;
        else if (row == rows - 1)
          *cell = WINDOW_TILE_BORDER_BOTTOM_TEE;
        else
          *cell = WINDOW_TILE_DIVIDER_VERTICAL;
      }
    }
    i++;
  }
  if (work->battleMode) {
    cell = work->tilemap + (window->y + window->height - 1) * 32 + window->x;
    column = 1;
    *cell++ = WINDOW_TILE_BATTLE_BORDER_BOTTOM_LEFT;
    while (column < lastColumn) {
      column++;
      *cell++ = WINDOW_TILE_BATTLE_BORDER_BOTTOM;
    }
    *cell = WINDOW_TILE_BATTLE_BORDER_BOTTOM_RIGHT;
  }
  work->dirty = 1;
}

// Recolours the five bar tiles at (x, y) in `window` to show a bar `length`
// pixels long (0-40). The bar tiles are drawn in colour 14 with a colour 1
// shadow; each pixel left of the length becomes colour 8 (shadow 13), the
// rest colour 2 (shadow 12). Only the bottom row of each tile changes for an
// empty bar, the bottom three otherwise. Outside battle it first loads the
// basic colours into HIGHLIGHT_PALETTE, its TEXT_BACKGROUND_COLOR taken from
// WINDOW_BACKGROUND_COLOR.
// It is declared to return s32 but returns no value, and no caller reads one;
// UBFIX makes it void, which changes the register its epilogue returns
// through.
#ifdef UBFIX
void PartyStatus_DrawBars(struct Window *window, s32 x, s32 y, s32 length) {
#else
s32 PartyStatus_DrawBars(struct Window *window, s32 x, s32 y, s32 length) {
#endif
  s32 i;
  struct WindowWork *work = WINDOW_WORK;
  s32 fullLength = length;
  s32 row;
  s32 column;
  u32 tile;
  u32 bar;
  u32 shadow;
  u32 pixels;
  u32 recoloured;

  if (work->battleMode == WINDOW_FIELD) {
    Dma_Set((const void *)Palette_GetBasicSpriteColors(),
            (void *)&BG_PLTT_COLOR(HIGHLIGHT_PALETTE, 0), DMA_COPY16(16),
            REG_DMA3);
    BG_PLTT_COLOR(HIGHLIGHT_PALETTE, TEXT_BACKGROUND_COLOR) =
        BG_PLTT_COLOR(WINDOW_PALETTE, WINDOW_BACKGROUND_COLOR);
  }
  x += window->x;
  y += window->y;
  for (i = 0; i < 5; i++, x++) {
    // This tile's eight pixels: filled up to `length`, empty after. A row is
    // 8 nibbles, the leftmost pixel lowest, so shifting the empty pattern
    // left by 4 * length bits leaves room for that many filled pixels.
    tile = work->tilemap[y * 32 + x];
    bar = 0x22222222;
    shadow = 0xcccccccc;
    tile &= 0x3ff;
    if (length > 7) {
      bar = 0x88888888;
      shadow = 0xdddddddd;
#ifdef UBFIX
      // At zero (including a multiple-of-eight remainder), the native
      // right shifts by 32 yield zero. Keep the empty patterns without
      // evaluating those undefined C shifts.
    } else if (length > 0) {
#else
    } else if (length >= 0) {
#endif
      bar <<= length * 4;
      bar |= 0x88888888U >> (32 - length * 4);
      shadow <<= length * 4;
      shadow |= 0xddddddddU >> (32 - length * 4);
    }
    for (row = 0; row <= (fullLength != 0 ? 2 : 0); row++) {
      pixels = *TILE_ROW_FROM_BOTTOM(tile, row);
      recoloured = 0;
      for (column = 0; column <= 7; column++) {
        u32 colour = pixels & 15;

        if (colour == 14)
          recoloured |= bar & (15U << (column * 4));
        else if (colour == 1)
          recoloured |= shadow & (15U << (column * 4));
        else
          recoloured |= colour << (column * 4);
        pixels >>= 4;
      }
      *TILE_ROW_FROM_BOTTOM(tile, row) = recoloured;
    }
    length -= 8;
  }
}

/// Draws the party status window with `flags` (see PARTY_STATUS_*): each
/// party member's name, HP and optionally PP with their bars, and in battle the
/// standby Djinn of each element. The HP is drawn in TEXT_COLOR_RED at zero and
/// TEXT_COLOR_YELLOW at a quarter or less. When the flags change, the window is
/// erased and laid out again first.
void PartyStatus_Draw(s32 flags) {
  // UB: the negative indices leave the single party-status heap slot.
  // Named members grow the EN function from 0x3d4 to 0x3dc bytes. Keep the
  // original address formation and load order in the matching build.
#ifdef UBFIX
  struct PartyStatusWindow *layout = HEAP_SLOT_BLOCK(HEAP_SLOT_PARTY_STATUS);
  struct BattleWork *battle = gBattleWork;
  struct Window *window = layout->window;
  struct WindowWork *work = WINDOW_WORK;
#else
  void **slot = (void **)gPartyStatusWork;
  struct PartyStatusWindow *layout = slot[0];
  struct BattleWork *battle = slot[-7];
  struct Window *window = layout->window;
  struct WindowWork *work = slot[-1];
#endif
  s32 x = 0;
  u32 count;
  s32 y = 0;
  u32 i;
  u16 members[5];
  u8 standbyDjinn[4];

  // List the party members shown: those fighting in battle, the party
  // otherwise. In battle the window is a row shorter and its text starts a row
  // higher.
  if (work->battleMode != WINDOW_FIELD) {
    count = BattleParty_PrepareMembersFar(0);
    y = -1;
    for (i = 0; i < count; i++) {
      members[i] = battle->partyUnits[i];
      if (members[i] == 255)
        break;
    }
  } else {
    count = Party_CountMembersFar();
    for (i = 0; i < count; i++)
      members[i] = gSequenceParentWork.party[i];
    members[i] = 255;
  }
  count = i;

  if (flags == PARTY_STATUS_KEEP_FLAGS)
    flags = layout->flags;
  if (!(flags & PARTY_STATUS_SHOW_PP))
    flags &= ~PARTY_STATUS_SHOW_DJINN;
  if (work->battleMode == WINDOW_FIELD ||
      BattleDjinn_CountStandbyFar(0, 0) == 0)
    flags &= ~PARTY_STATUS_SHOW_DJINN;
  if (flags == PARTY_STATUS_CLOSED) {
    Window_EraseBorderRect(layout->x, layout->y, layout->width, layout->height);
    return;
  }

  // Redraw the frame, laying it out again when the flags changed.
  work->holdTilemapUpload = 1;
  if (layout->flags == flags) {
    WindowSprite_RedrawSavedRect(window);
    Window_DrawColumnBorders(window, flags);
  } else {
    Window_EraseBorderRect(layout->x, layout->y, layout->width, layout->height);
    PartyStatus_BuildLayoutBounds(flags);
    window->width = layout->width;
    window->height = layout->height;
    window->x = layout->x;
    Window_DrawBorder(layout->x, layout->y, layout->width, layout->height);
    Window_DrawColumnBorders(window, flags);
  }

  // Each party member's column: name, HP and PP, each bar 40 pixels at full.
  if (flags & PARTY_STATUS_SHOW_DJINN)
    x = PARTY_STATUS_DJINN_WIDTH;
  for (i = 0; i != count; i++) {
    struct Unit *unit = Unit_GetFar(members[i]);
    s32 hp = unit->hp;
    s32 hpMax = unit->hpMax;
    s32 barLength;
    s32 current;

    if (hp == 0)
      Text_SetColor(TEXT_COLOR_RED);
    else if (hp <= hpMax / 4)
      Text_SetColor(TEXT_COLOR_YELLOW);
    else
      Text_SetColor(TEXT_COLOR_WHITE);
    work->textPalette = HIGHLIGHT_PALETTE;
    if (work->battleMode != WINDOW_FIELD)
      work->textPalette = BASIC_COLORS_PALETTE;
    Text_DrawPrefixedNumberAtOffset(
        hp, window, (x + i * PARTY_STATUS_COLUMN_WIDTH) * 8, y * 8 + 8, 0);
    work->textPalette = WINDOW_PALETTE;
    Text_DrawStringAtOffset(unit->name, window,
                            (x + i * PARTY_STATUS_COLUMN_WIDTH) * 8, y * 8);
    Text_SetColor(TEXT_COLOR_WHITE);
    if (unit->hpMax != 0) {
      // A party member with any HP left shows at least one pixel of bar.
      current = unit->hp;
      barLength = ((current * 40) / unit->hpMax);
      if (barLength == 0 && current != 0)
        barLength = 1;
      PartyStatus_DrawBars(window, x + i * PARTY_STATUS_COLUMN_WIDTH + 1, y + 2,
                           barLength);
    }
    if (flags & PARTY_STATUS_SHOW_PP) {
      work->textPalette = HIGHLIGHT_PALETTE;
      if (work->battleMode != WINDOW_FIELD)
        work->textPalette = BASIC_COLORS_PALETTE;
      Text_DrawPrefixedNumberAtOffset(unit->pp, window,
                                      (x + i * PARTY_STATUS_COLUMN_WIDTH) * 8,
                                      y * 8 + 16, 1);
      if (unit->ppMax != 0) {
        current = unit->pp;
        barLength = ((current * 40) / unit->ppMax);
        if (barLength == 0 && current != 0)
          barLength = 1;
        PartyStatus_DrawBars(window, x + i * PARTY_STATUS_COLUMN_WIDTH + 1,
                             y + 3, barLength);
      }
    }
  }
  work->textPalette = WINDOW_PALETTE;

  // The Djinn column: each element's icon and its count of standby Djinn.
  if (work->battleMode != WINDOW_FIELD && (flags & PARTY_STATUS_SHOW_DJINN)) {
    s32 row = y;

    if (flags & PARTY_STATUS_SHOW_PP)
      row++;
    BattleDjinn_CountStandbyFar(0, standbyDjinn);
    Window_SetTilemapEntry(window, LIST_TILE_SUMMON_ELEMENT + ELEMENT_VENUS, 0,
                           row, 0);
    Window_SetTilemapEntry(window, LIST_TILE_SUMMON_ELEMENT + ELEMENT_MERCURY,
                           2, row, 0);
    Window_SetTilemapEntry(window, LIST_TILE_SUMMON_ELEMENT + ELEMENT_MARS, 0,
                           row + 1, 0);
    Window_SetTilemapEntry(window, LIST_TILE_SUMMON_ELEMENT + ELEMENT_JUPITER,
                           2, row + 1, 0);
    PARTY_STATUS_PUT_DIGIT(window, standbyDjinn[0] + '0', 1, row);
    PARTY_STATUS_PUT_DIGIT(window, standbyDjinn[1] + '0', 3, row);
    PARTY_STATUS_PUT_DIGIT(window, standbyDjinn[2] + '0', 1, row + 1);
    PARTY_STATUS_PUT_DIGIT(window, standbyDjinn[3] + '0', 3, row + 1);
  }
  work->holdTilemapUpload = 0;
}

/// Closes the party status window and frees its heap block.
void PartyStatus_Close(void) {
  // UB: reads the heap slot's pointer through an s32 pointer; UBFIX reads
  // the window through the block's own struct.
#ifdef UBFIX
  Window_Close(((struct PartyStatusWindow *)gWindowWork[1])->window, 1);
#else
  Window_Close((struct Window *)**(s32 **)(u32)&gWindowWork[1], 1);
#endif
  HeapSlot_Release(HEAP_SLOT_PARTY_STATUS);
}

/// Sets (`set` 1) or clears (0) bit 12 of the tilemap entries of the
/// `width` x `height` tile rectangle at (x, y) inside `window`, clipped to the
/// screen: bit 12 is the lowest bit of the entry's palette number, so this
/// switches the rectangle between two palettes, such as 15 and 14. Nothing
/// calls it.
///
/// The clip, which the other rectangle routines here repeat, tests both edges
/// against 29: a rectangle reaching row 29 or beyond is cut to end at row 19,
/// the tilemap's last, but one ending on rows 20 to 28 is not cut and writes
/// past the 20-row tilemap. The row test is the column test's 29 again, while
/// the height it sets uses the tilemap's 20 rows.
void Window_SetTextTileBitRect(const struct Window *window, s32 x, s32 y,
                               s32 width, s32 height, u32 set) {
  struct WindowWork *work = WINDOW_WORK;

  x += window->x + 1;
  y += window->y + 1;
  set &= 1;
  set <<= 12;
  if (x < 0) {
    width += x;
    x = 0;
  }
  if (x + width > 29) {
    width = 30 - x;
  }
  if (y < 0) {
    height += y;
    y = 0;
  }
  if (y + height > 29) {
    height = 20 - y;
  }
  if (width > 0 && height > 0) {
    // The byte offset of the first cell: 32 entries of 2 bytes a row.
    y <<= 6;
    x = y + (x << 1);
    do {
      u16 *cell = (u16 *)(x + (u32)work->tilemap);
      s32 remaining = width;
      while (remaining != 0) {
        u32 entry = *cell;
        entry &= ~0x1000;
        entry |= set;
        remaining--;
        *cell = entry;
        cell++;
      }
      height--;
      x += 64;
    } while (height != 0);
    work->dirty = 1;
  }
}

#undef PARTY_STATUS_PUT_DIGIT
