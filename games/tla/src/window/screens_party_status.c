/// @file
/// The party status window.
// Part of window.c.

#include "window_internal.h"
#include "window/menu_list.h"
#include "system/system.h"
#include "window/screens.h"
#include "window/window.h"

/// The party status window's column a party member, in tiles.
#define PARTY_STATUS_COLUMN_WIDTH 6
#define PARTY_STATUS_DJINN_WIDTH 5 ///< the Djinn column, in battle

/// Sizes the party status window by the party: a column of six tiles a party
/// member (four at most outside battle), one more row with flag 1 and five more
/// columns with flag 2, against the screen's right edge.
void PartyStatus_BuildLayoutBounds(s32 flags) {
  struct PartyStatusWindow *state = Ram_HeapSlots->partyStatus;
  struct WindowWork *work = (struct WindowWork *)Ram_HeapSlots->windowWork;
  s32 height = 4;
  s32 n;
  s32 width;
  s32 x;

  if (work->battleMode != WINDOW_FIELD) {
    n = BattleParty_PrepareMembersFar(0);
    height = 3;
  } else {
    n = Party_CountMembersFar();
    if ((u32)n > 4)
      n = 4;
  }
  if (flags & PARTY_STATUS_SHOW_PP)
    height++;
  else
    flags &= -3;

  n *= PARTY_STATUS_COLUMN_WIDTH;
  width = n + 1;
  if (flags & PARTY_STATUS_SHOW_DJINN)
    width = n + 6;

  x = 30;
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
  s32 zero;
  struct PartyStatusWindow *window;
  struct WindowWork *work;

  window = HeapSlot_AllocEwram(HEAP_SLOT_PARTY_STATUS,
                               sizeof(struct PartyStatusWindow));
  work = (struct WindowWork *)Ram_HeapSlots->windowWork;
  zero = 0;
  work->holdTilemapUpload = 1;
  PartyStatus_BuildLayoutBounds(flags);
  window->window = (struct Window *)Window_Create(
      window->x, window->y, window->width, window->height, 6);
  PartyStatus_Draw(flags);
  work->holdTilemapUpload = zero;
}

/// Draws the vertical lines between the party members' columns of the party
/// status `window`, with their top and bottom joints, at the offsets listed in
/// gWindow_PartyColumnOffsets (shifted right past the Djinn column, whose own
/// line comes first in the list). In battle it also draws the window's bottom
/// edge.
void Window_DrawColumnBorders(struct Window *window, u32 flags) {
  struct WindowWork *work = (struct WindowWork *)Ram_HeapSlots->windowWork;
  s32 first = 1;
  s32 shift = 0;
  u32 lastColumn = window->width - 1;
  s32 i;
  u8 state = work->battleMode;
  u32 rows = window->height;
  u32 column;
  u32 row;
  u16 *cell;
  const s8 *table;

  if ((flags & PARTY_STATUS_SHOW_PP) == 0)
    flags &= ~PARTY_STATUS_SHOW_DJINN;
  if (flags & PARTY_STATUS_SHOW_DJINN) {
    shift = PARTY_STATUS_DJINN_WIDTH;
    first = 0;
  }
  {
    const s8 *columns = gWindow_PartyColumnOffsets;

    i = first;
    table = columns;
  }
  if (gWindow_PartyColumnOffsets[i] >= 0) {
    do {
      column = table[i] + shift;
      if (column < lastColumn) {
        for (row = 0; row != rows; row++) {
          cell =
              work->tilemap[(s16)window->y + row] + ((s16)window->x + column);
          if (row == 0)
            *cell = WINDOW_TILE_BORDER_TOP_TEE;
          else if (row == rows - 1)
            *cell = WINDOW_TILE_BORDER_BOTTOM_TEE;
          else
            *cell = WINDOW_TILE_DIVIDER_VERTICAL;
        }
      }
      i++;
      table = gWindow_PartyColumnOffsets;
    } while (gWindow_PartyColumnOffsets[i] >= 0);
    state = work->battleMode;
  }
  {
    /* FAKEMATCH(barrier): an empty asm copies the state into a word local. */
    u32 shown;

    asm("" : "=r"(shown) : "0"(state));
    if (shown) {
      cell =
          work->tilemap[(s16)window->y + window->height - 1] + (s16)window->x;
      column = 1;
      *cell++ = WINDOW_TILE_BATTLE_BORDER_BOTTOM_LEFT;
      while (column < lastColumn) {
        column++;
        *cell++ = WINDOW_TILE_BATTLE_BORDER_BOTTOM;
      }
      *cell = WINDOW_TILE_BATTLE_BORDER_BOTTOM_RIGHT;
    }
  }
  work->dirty = 1;
}

/// The address of pixel row 7 - `up` of BG tile `tile` (4bpp, 32 bytes a tile,
/// 4 bytes a row): the status bars are drawn from a tile's bottom row up.
#define TILE_ROW_FROM_BOTTOM(tile, up)                                         \
  ((u32 *)((tile) * 32 - (up) * 4 + ((s32)BG_CHAR_BLOCK(0) + 7 * 4)))

/// Recolours the five bar tiles at (x, y) in `window` to show a bar `length`
/// pixels long (0-40).
UNREAD_RESULT PartyStatus_DrawBars(struct Window *window, s32 x, s32 y,
                                   s32 length) {
  s32 i;
  struct WindowWork *work = (struct WindowWork *)Ram_HeapSlots->windowWork;
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
            (void *)&BG_PLTT_COLOR(HIGHLIGHT_PALETTE, 0),
            DMA_COPY_HALFWORDS(16), REG_DMA3);
    BG_PLTT_COLOR(HIGHLIGHT_PALETTE, TEXT_BACKGROUND_COLOR) =
        BG_PLTT_COLOR(WINDOW_PALETTE, WINDOW_BACKGROUND_COLOR);
  }
  x += (s16)window->x;
  y += (s16)window->y;
  for (i = 0; i < 5; i++, x++) {
    // This tile's eight pixels: filled up to `length`, empty after.
    tile = work->tilemap[0][y * BG_SCREEN_WIDTH + x];
    bar = PIXELS_4BPP(2);
    shadow = PIXELS_4BPP(12);
    tile &= TILE_NUMBER_MASK;
    if (length > 7) {
      bar = PIXELS_4BPP(8);
      shadow = PIXELS_4BPP(13);
      // UB: at a length of zero the shift below is by 32, which the GBA
      // takes as zero.
#ifdef UBFIX
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

/// Draws the party status window with `flags` (see PARTY_STATUS_*): each party
/// member's name, HP and optionally PP with their bars, and in battle the
/// standby Djinn of each element.
void PartyStatus_Draw(s32 flags) {
  struct PartyStatusWindow *layout = Ram_HeapSlots->partyStatus;
  struct BattleWork *battle = Ram_HeapSlots->battleWork;
  struct Window *window = layout->window;
  struct WindowWork *work = (struct WindowWork *)Ram_HeapSlots->windowWork;
  s32 x = 0;
  u32 count;
  s32 y = 0;
  u32 i;
  u16 members[5];
  u8 standbyDjinn[4];

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
    if (count > 4)
      count = 4;
    for (i = 0; i < count; i++)
      members[i] = gMainRoutineWork.party[i];
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
    work->textPalette = HIGHLIGHT_PALETTE;
    if (work->battleMode != WINDOW_FIELD)
      work->textPalette = BASIC_COLORS_PALETTE;
    Text_DrawPrefixedNumberAtOffset(
        hp, window, (x + i * PARTY_STATUS_COLUMN_WIDTH) * 8, y * 8 + 8, 0);
    work->textPalette = WINDOW_PALETTE;
    Text_DrawStringAtOffset(unit->name, (struct Window *)window,
                            (x + i * PARTY_STATUS_COLUMN_WIDTH) * 8, y * 8);
    Text_SetColor(TEXT_COLOR_WHITE);
    if (unit->hpMax != 0) {
      current = unit->hp;
      barLength = (current * 40) / unit->hpMax;
      if (barLength == 0 && current != 0)
        barLength = 1;
      PartyStatus_DrawBars((struct Window *)window,
                           x + i * PARTY_STATUS_COLUMN_WIDTH + 1, y + 2,
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
        barLength = (current * 40) / unit->ppMax;
        if (barLength == 0 && current != 0)
          barLength = 1;
        PartyStatus_DrawBars((struct Window *)window,
                             x + i * PARTY_STATUS_COLUMN_WIDTH + 1, y + 3,
                             barLength);
      }
    }
  }
  work->textPalette = WINDOW_PALETTE;

  if (work->battleMode != WINDOW_FIELD && (flags & PARTY_STATUS_SHOW_DJINN)) {
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
  work->holdTilemapUpload = 0;
}

/// Closes the party status window and frees its heap block.
void PartyStatus_Close(void) {
  Window_Close(*(void **)Ram_HeapSlots->partyStatus, 1);
  HeapSlot_Release(HEAP_SLOT_PARTY_STATUS);
}

#undef TILE_ROW_FROM_BOTTOM
#undef PARTY_STATUS_DJINN_WIDTH
#undef PARTY_STATUS_COLUMN_WIDTH
