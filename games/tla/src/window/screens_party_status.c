/// @file
/// The party status window.
// Part of screens.c.

#include "window_internal.h"
#include "window/menu_list.h"
#include "system/system.h"
#include "window/screens.h"
#include "window/window.h"

/// The party status window's column for each party member, in tiles.
#define PARTY_STATUS_COLUMN_WIDTH 6
#define PARTY_STATUS_DJINN_WIDTH 5 ///< the Djinn column, in battle

/// Sizes the party status window by the party: a column of six tiles a party
/// member (four at most outside battle), one more row with PARTY_STATUS_SHOW_PP
/// and the Djinn column with PARTY_STATUS_SHOW_DJINN, against the screen's
/// right edge.
void PartyStatus_BuildLayoutBounds(s32 flags) {
  struct PartyStatusWindow *state = Ram_HeapSlots->partyStatus;
  struct WindowState *windowState =
      (struct WindowState *)Ram_HeapSlots->windowState;
  s32 height = 4;
  s32 members;
  s32 width;
  s32 x;

  if (windowState->battleMode != WINDOW_FIELD) {
    members = BattleParty_PrepareMembersFar(0);
    height = 3;
  } else {
    members = Party_CountMembersFar();
    if ((u32)members > 4)
      members = 4;
  }
  if (flags & PARTY_STATUS_SHOW_PP)
    height++;
  else
    flags &= ~PARTY_STATUS_SHOW_DJINN;

  members *= PARTY_STATUS_COLUMN_WIDTH;
  width = members + 1;
  if (flags & PARTY_STATUS_SHOW_DJINN)
    width = members + 1 + PARTY_STATUS_DJINN_WIDTH;

  x = DISPLAY_TILE_WIDTH;
  x -= width;
  state->x = x;
  state->y = 0;
  state->width = width;
  state->height = height;
  state->flags = flags;
}

/// Opens the party status window, sized by PartyStatus_BuildLayoutBounds, and
/// draws it, holding the tilemap upload meanwhile.
void PartyStatus_CreateWindow(s32 flags) {
  struct PartyStatusWindow *window;
  struct WindowState *state;

  window = HeapSlot_AllocEwram(HEAP_SLOT_PARTY_STATUS,
                               sizeof(struct PartyStatusWindow));
  state = (struct WindowState *)Ram_HeapSlots->windowState;
  state->holdTilemapUpload = 1;
  PartyStatus_BuildLayoutBounds(flags);
  window->window = (struct Window *)Window_Create(
      window->x, window->y, window->width, window->height,
      WINDOW_FLAG_INSTANT | WINDOW_FLAG_UNREAD);
  PartyStatus_Draw(flags);
  state->holdTilemapUpload = 0;
}

/// Draws the vertical lines between the party members' columns of the party
/// status `window`, with their top and bottom joints, at the offsets listed in
/// gWindow_PartyColumnOffsets (shifted right past the Djinn column, whose own
/// line comes first in the list). In battle it also draws the window's bottom
/// edge.
void Window_DrawColumnBorders(struct Window *window, u32 flags) {
  struct WindowState *windowState =
      (struct WindowState *)Ram_HeapSlots->windowState;
  s32 first = 1;
  s32 shift = 0;
  u32 lastColumn = window->width - 1;
  s32 i;
  u8 state = windowState->battleMode;
  u32 rows = window->height;
  u32 column;
  u32 row;
  u16 *cell;
  /* FAKEMATCH(temporary): table holds the column offsets' address for the
   * reads in the loop, where the loop's test reads them by name. */
  const s8 *table;
  /* FAKEMATCH(temporary): columns, the column offsets' address on its way to
   * table. */
  const s8 *columns;

  if ((flags & PARTY_STATUS_SHOW_PP) == 0)
    flags &= ~PARTY_STATUS_SHOW_DJINN;
  if (flags & PARTY_STATUS_SHOW_DJINN) {
    shift = PARTY_STATUS_DJINN_WIDTH;
    first = 0;
  }
  columns = gWindow_PartyColumnOffsets;
  i = first;
  table = columns;
  if (gWindow_PartyColumnOffsets[i] >= 0) {
    do {
      column = table[i] + shift;
      if (column < lastColumn) {
        for (row = 0; row != rows; row++) {
          cell = windowState->tilemap[(s16)window->y + row] +
                 ((s16)window->x + column);
          if (row == 0)
            *cell = WINDOW_TILE_BORDER_TOP_TEE;
          else if (row == rows - 1)
            *cell = WINDOW_TILE_BORDER_BOTTOM_TEE;
          else
            *cell = WINDOW_TILE_DIVIDER_VERTICAL;
        }
      }
      i++;
      /* FAKEMATCH(dead): table set again to the address it holds. */
      table = gWindow_PartyColumnOffsets;
    } while (gWindow_PartyColumnOffsets[i] >= 0);
    state = windowState->battleMode;
  }
  {
    /* FAKEMATCH(temporary): shown, a copy of state for the bottom edge's
     * tests; as state. */
    u32 shown = state;

    /* FAKEMATCH(dead): state cleared after its copy, where nothing reads it. */
    state = 0;
    if (shown) {
      /* FAKEMATCH(dead): a test of shown that is never true, around a store
       * to column, which the next lines set again. */
      if (shown > 255)
        column = 0;
      cell = windowState->tilemap[(s16)window->y + window->height - 1] +
             (s16)window->x;
      column = 1;
      *cell++ = WINDOW_TILE_BATTLE_BORDER_BOTTOM_LEFT;
      while (column < lastColumn) {
        column++;
        *cell++ = WINDOW_TILE_BATTLE_BORDER_BOTTOM;
      }
      *cell = WINDOW_TILE_BATTLE_BORDER_BOTTOM_RIGHT;
    }
  }
  windowState->dirty = 1;
}

/// The address of pixel row 7 - `up` of BG tile `tile` (4bpp, 32 bytes a tile,
/// 4 bytes a row): the status bars are drawn from a tile's bottom row up.
#define TILE_ROW_FROM_BOTTOM(tile, up)                                         \
  ((u32 *)((tile) * 32 - (up) * 4 + ((s32)BG_CHAR_BLOCK(0) + 7 * 4)))

/// A party member's HP and PP bars: PARTY_STATUS_BAR_LENGTH pixels across
/// five tiles, whose own pixels are drawn in BAR_TILE_COLOR over a shadow of
/// BAR_TILE_SHADOW_COLOR.
#define PARTY_STATUS_BAR_LENGTH 40
#define BAR_TILE_COLOR 14
#define BAR_TILE_SHADOW_COLOR 1
/// The colours the filled part of a bar and the rest of it are drawn in, each
/// over its shadow: on the field, the basic sprite colours copied into
/// HIGHLIGHT_PALETTE.
#define BAR_FILLED_COLOR 8
#define BAR_FILLED_SHADOW_COLOR 13
#define BAR_EMPTY_COLOR 2
#define BAR_EMPTY_SHADOW_COLOR 12
/// A 4bpp tile row: eight pixels of four bits, one word.
#define TILE_ROW_BITS 32

/// Recolours the five bar tiles at (x, y) in `window` to show a bar `length`
/// pixels long (0 to PARTY_STATUS_BAR_LENGTH).
UNREAD_RESULT PartyStatus_DrawBars(struct Window *window, s32 x, s32 y,
                                   s32 length) {
  s32 i;
  struct WindowState *state = (struct WindowState *)Ram_HeapSlots->windowState;
  s32 fullLength = length;
  s32 row;
  s32 column;
  u32 tile;
  u32 bar;
  u32 shadow;
  u32 pixels;
  u32 recoloured;

  if (state->battleMode == WINDOW_FIELD) {
    Dma_Set((const void *)Palette_GetBasicSpriteColors(),
            (void *)&BG_PLTT_COLOR(HIGHLIGHT_PALETTE, 0),
            DMA_COPY_HALFWORDS(16), REG_DMA3);
    BG_PLTT_COLOR(HIGHLIGHT_PALETTE, TEXT_BACKGROUND_COLOR) =
        BG_PLTT_COLOR(WINDOW_PALETTE, WINDOW_BACKGROUND_COLOR);
  }
  x += (s16)window->x;
  y += (s16)window->y;
  for (i = 0; i < PARTY_STATUS_BAR_LENGTH / 8; i++, x++) {
    // This tile's eight pixels: filled up to `length`, empty after.
    tile = state->tilemap[0][y * BG_SCREEN_WIDTH + x];
    bar = PIXELS_4BPP(BAR_EMPTY_COLOR);
    shadow = PIXELS_4BPP(BAR_EMPTY_SHADOW_COLOR);
    tile &= TILE_NUMBER_MASK;
    if (length > 7) {
      bar = PIXELS_4BPP(BAR_FILLED_COLOR);
      shadow = PIXELS_4BPP(BAR_FILLED_SHADOW_COLOR);
      // UB: at a length of zero the shift below is by 32, which the GBA
      // takes as zero.
#ifdef UBFIX
    } else if (length > 0) {
#else
    } else if (length >= 0) {
#endif
      bar <<= length * 4;
      bar |= PIXELS_4BPP(BAR_FILLED_COLOR) >> (TILE_ROW_BITS - length * 4);
      shadow <<= length * 4;
      shadow |=
          PIXELS_4BPP(BAR_FILLED_SHADOW_COLOR) >> (TILE_ROW_BITS - length * 4);
    }
    for (row = 0; row <= (fullLength != 0 ? 2 : 0); row++) {
      pixels = *TILE_ROW_FROM_BOTTOM(tile, row);
      recoloured = 0;
      for (column = 0; column <= 7; column++) {
        u32 colour = pixels & 0xf;

        if (colour == BAR_TILE_COLOR)
          recoloured |= bar & (0xfU << (column * 4));
        else if (colour == BAR_TILE_SHADOW_COLOR)
          recoloured |= shadow & (0xfU << (column * 4));
        else
          recoloured |= colour << (column * 4);
        pixels >>= 4;
      }
      *TILE_ROW_FROM_BOTTOM(tile, row) = recoloured;
    }
    length -= 8;
  }
}

/// Draws the party status window with `flags` (see PARTY_STATUS_*): each party
/// member's name, HP and optionally PP with their bars, and in battle the
/// standby Djinn of each element.
void PartyStatus_Draw(s32 flags) {
  struct PartyStatusWindow *layout = Ram_HeapSlots->partyStatus;
  struct BattleState *battle = Ram_HeapSlots->battleState;
  struct Window *window = layout->window;
  struct WindowState *state = (struct WindowState *)Ram_HeapSlots->windowState;
  s32 x = 0;
  u32 count;
  s32 y = 0;
  u32 i;
  u16 members[5];
  u8 standbyDjinn[4];

  if (state->battleMode != WINDOW_FIELD) {
    count = BattleParty_PrepareMembersFar(0);
    y = -1;
    for (i = 0; i < count; i++) {
      members[i] = battle->partyUnits[i];
      if (members[i] == BATTLE_UNIT_LIST_END)
        break;
    }
  } else {
    count = Party_CountMembersFar();
    if (count > 4)
      count = 4;
    for (i = 0; i < count; i++)
      members[i] = gMainRoutineState.party[i];
    members[i] = BATTLE_UNIT_LIST_END;
  }
  count = i;

  if (flags == PARTY_STATUS_KEEP_FLAGS)
    flags = layout->flags;
  if (!(flags & PARTY_STATUS_SHOW_PP))
    flags &= ~PARTY_STATUS_SHOW_DJINN;
  if (state->battleMode == WINDOW_FIELD ||
      BattleDjinn_CountStandbyFar(0, 0) == 0)
    flags &= ~PARTY_STATUS_SHOW_DJINN;
  if (flags == PARTY_STATUS_CLOSED) {
    Window_EraseBorderRect(layout->x, layout->y, layout->width, layout->height);
    return;
  }

  state->holdTilemapUpload = 1;
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

  if (flags & PARTY_STATUS_SHOW_DJINN)
    x = PARTY_STATUS_DJINN_WIDTH;
  for (i = 0; i != count; i++) {
    struct Unit *unit = Unit_Get(members[i]);
    s32 hp = unit->hp;
    s32 maxHp = unit->hpMax;
    s32 barLength;
    s32 current;

    if (hp == 0)
      Text_SetColor(TEXT_COLOR_RED);
    else if (hp <= maxHp / 4)
      Text_SetColor(TEXT_COLOR_YELLOW);
    else
      Text_SetColor(TEXT_COLOR_WHITE);
    state->textPalette = HIGHLIGHT_PALETTE;
    if (state->battleMode != WINDOW_FIELD)
      state->textPalette = BASIC_COLORS_PALETTE;
    Text_DrawPrefixedNumberAtOffset(
        hp, window, (x + i * PARTY_STATUS_COLUMN_WIDTH) * 8, y * 8 + 8, 0);
    state->textPalette = WINDOW_PALETTE;
    Text_DrawStringAtOffset(unit->name, (struct Window *)window,
                            (x + i * PARTY_STATUS_COLUMN_WIDTH) * 8, y * 8);
    Text_SetColor(TEXT_COLOR_WHITE);
    if (unit->hpMax != 0) {
      current = unit->hp;
      barLength = (current * PARTY_STATUS_BAR_LENGTH) / unit->hpMax;
      if (barLength == 0 && current != 0)
        barLength = 1;
      PartyStatus_DrawBars((struct Window *)window,
                           x + i * PARTY_STATUS_COLUMN_WIDTH + 1, y + 2,
                           barLength);
    }
    if (flags & PARTY_STATUS_SHOW_PP) {
      state->textPalette = HIGHLIGHT_PALETTE;
      if (state->battleMode != WINDOW_FIELD)
        state->textPalette = BASIC_COLORS_PALETTE;
      Text_DrawPrefixedNumberAtOffset(unit->pp, window,
                                      (x + i * PARTY_STATUS_COLUMN_WIDTH) * 8,
                                      y * 8 + 16, 1);
      if (unit->ppMax != 0) {
        current = unit->pp;
        barLength = (current * PARTY_STATUS_BAR_LENGTH) / unit->ppMax;
        if (barLength == 0 && current != 0)
          barLength = 1;
        PartyStatus_DrawBars((struct Window *)window,
                             x + i * PARTY_STATUS_COLUMN_WIDTH + 1, y + 3,
                             barLength);
      }
    }
  }
  state->textPalette = WINDOW_PALETTE;

  if (state->battleMode != WINDOW_FIELD && (flags & PARTY_STATUS_SHOW_DJINN)) {
    s32 row = y;

    /* FAKEMATCH(dead): i set to a 0 never read. */
    i = 0;
    if (flags & PARTY_STATUS_SHOW_PP)
      row++;
    BattleDjinn_CountStandbyFar(0, standbyDjinn);
    Window_SetTilemapEntry(window, ELEMENT_ICON_TILE + ELEMENT_VENUS, 0, row,
                           0);
    Window_SetTilemapEntry(window, ELEMENT_ICON_TILE + ELEMENT_MERCURY, 2, row,
                           0);
    Window_SetTilemapEntry(window, ELEMENT_ICON_TILE + ELEMENT_MARS, 0, row + 1,
                           0);
    Window_SetTilemapEntry(window, ELEMENT_ICON_TILE + ELEMENT_JUPITER, 2,
                           row + 1, 0);
    if (standbyDjinn[0] > 9 || standbyDjinn[1] > 9 || standbyDjinn[2] > 9 ||
        standbyDjinn[3] > 9)
      Dma_Set(gPartyStatus_DigitTiles, DJINN_COUNT_TENS_VRAM,
              DMA_COPY_WORDS(9 * TILE_SIZE_4BPP / 4), REG_DMA3);
    Window_SetTilemapEntry(window, DJINN_COUNT_TILE(standbyDjinn[0]), 1, row,
                           0);
    Window_SetTilemapEntry(window, DJINN_COUNT_TILE(standbyDjinn[1]), 3, row,
                           0);
    Window_SetTilemapEntry(window, DJINN_COUNT_TILE(standbyDjinn[2]), 1,
                           row + 1, 0);
    Window_SetTilemapEntry(window, DJINN_COUNT_TILE(standbyDjinn[3]), 3,
                           row + 1, 0);
  }
  state->holdTilemapUpload = 0;
}

/// Closes the party status window and frees its heap block.
void PartyStatus_Close(void) {
  Window_Close(*(void **)Ram_HeapSlots->partyStatus, 1);
  HeapSlot_Release(HEAP_SLOT_PARTY_STATUS);
}

#undef TILE_ROW_FROM_BOTTOM
#undef PARTY_STATUS_BAR_LENGTH
#undef BAR_TILE_COLOR
#undef BAR_TILE_SHADOW_COLOR
#undef BAR_FILLED_COLOR
#undef BAR_FILLED_SHADOW_COLOR
#undef BAR_EMPTY_COLOR
#undef BAR_EMPTY_SHADOW_COLOR
#undef TILE_ROW_BITS
#undef PARTY_STATUS_DJINN_WIDTH
#undef PARTY_STATUS_COLUMN_WIDTH
