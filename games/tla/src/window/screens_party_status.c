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
  struct WindowWork *canvas = (struct WindowWork *)Ram_HeapSlots->windowWork;
  s32 first = 1;
  s32 bias = 0;
  u32 max = window->width - 1;
  s32 i;
  u8 state = canvas->battleMode;
  u32 rows = window->height;
  u32 col;
  u32 row;
  u16 *dest;
  const s8 *table;

  if ((flags & PARTY_STATUS_SHOW_PP) == 0)
    flags &= ~PARTY_STATUS_SHOW_DJINN;
  if (flags & PARTY_STATUS_SHOW_DJINN) {
    bias = 5;
    first = 0;
  }
  {
    const s8 *columns = gWindow_PartyColumnOffsets;

    i = first;
    table = columns;
  }
  if (gWindow_PartyColumnOffsets[i] >= 0) {
    do {
      col = table[i] + bias;
      if (col < max) {
        for (row = 0; row != rows; row++) {
          dest = canvas->tilemap[(s16)window->y + row] + ((s16)window->x + col);
          if (row == 0)
            *dest = 0xf018;
          else if (row == rows - 1)
            *dest = 0xf019;
          else
            *dest = 0xf00f;
        }
      }
      i++;
      table = gWindow_PartyColumnOffsets;
    } while (gWindow_PartyColumnOffsets[i] >= 0);
    state = canvas->battleMode;
  }
  {
    /* FAKEMATCH(barrier): an empty asm copies the state into a word local. */
    u32 shown;

    asm("" : "=r"(shown) : "0"(state));
    if (shown) {
      dest =
          canvas->tilemap[(s16)window->y + window->height - 1] + (s16)window->x;
      col = 1;
      *dest++ = 0xf080;
      while (col < max) {
        col++;
        *dest++ = 0xf081;
      }
      *dest = 0xf082;
    }
  }
  canvas->dirty = 1;
}

#define TILE_ROW_FROM_BOTTOM(tile, up)                                         \
  ((u32 *)((tile) * 32 - (up) * 4 + 0x0600001c))

s32 PartyStatus_DrawBars(struct RenderInput *window, s32 x, s32 y, s32 value) {
  s32 i;
  struct WindowWork *canvas = (struct WindowWork *)Ram_HeapSlots->windowWork;
  s32 filled = value;
  s32 row;
  s32 col;
  u32 id;
  u32 light;
  u32 dark;
  u32 pixels;
  u32 result;

  if (canvas->battleMode == WINDOW_FIELD) {
    Dma_Set((const void *)Palette_GetBasicSpriteColors(),
            (void *)&BG_PLTT_COLOR(HIGHLIGHT_PALETTE, 0),
            DMA_COPY_HALFWORDS(16), REG_DMA3);
    BG_PLTT_COLOR(HIGHLIGHT_PALETTE, TEXT_BACKGROUND_COLOR) =
        BG_PLTT_COLOR(WINDOW_PALETTE, WINDOW_BACKGROUND_COLOR);
  }
  x += (s16)window->x;
  y += (s16)window->y;
  for (i = 0; i < 5; i++, x++) {
    id = canvas->tilemap[0][y * BG_SCREEN_WIDTH + x];
    light = PIXELS_4BPP(2);
    dark = PIXELS_4BPP(12);
    id &= TILE_NUMBER_MASK;
    if (value > 7) {
      light = PIXELS_4BPP(8);
      dark = PIXELS_4BPP(13);
    } else if (value >= 0) {
      light <<= value * 4;
      light |= 0x88888888U >> (32 - value * 4);
      dark <<= value * 4;
      dark |= 0xddddddddU >> (32 - value * 4);
    }
    for (row = 0; row <= (filled != 0 ? 2 : 0); row++) {
      pixels = *TILE_ROW_FROM_BOTTOM(id, row);
      result = 0;
      for (col = 0; col <= 7; col++) {
        u32 color = pixels & 15;

        if (color == 14)
          result |= light & (15U << (col * 4));
        else if (color == 1)
          result |= dark & (15U << (col * 4));
        else
          result |= color << (col * 4);
        pixels >>= 4;
      }
      *TILE_ROW_FROM_BOTTOM(id, row) = result;
    }
    value -= 8;
  }
}

#define PARTY_STATUS_DJINN_WIDTH 5

/* VIEW(screens): routine BattleDjinn_CountStandbyFar as s32
 * BattleDjinn_CountStandbyFar(s32 a, u8 *counts); window/window.h has void
 * BattleDjinn_CountStandbyFar(s32 kind, u8 *counts); tried as the rest of the
 * object declares it, PartyStatus_Draw moves: 2 of 495 instructions,
 * BattleCommandRow_Run moves: 1 of 1120 instructions in tla-ja. */
extern s32
Func_08118130__screens(s32 a,
                       u8 *counts) __asm__("BattleDjinn_CountStandbyFar");

#define DJINN_COUNT_TILE(n) ((n) > 9 ? (n) + 0xf196 : ((n) + '0') | 0xf000)

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
  if (work->battleMode == WINDOW_FIELD || Func_08118130__screens(0, 0) == 0)
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
    Text_DrawStringAtOffset(unit->name, (struct RenderInput *)window,
                            (x + i * PARTY_STATUS_COLUMN_WIDTH) * 8, y * 8);
    Text_SetColor(TEXT_COLOR_WHITE);
    if (unit->hpMax != 0) {
      current = unit->hp;
      barLength = (current * 40) / unit->hpMax;
      if (barLength == 0 && current != 0)
        barLength = 1;
      PartyStatus_DrawBars((struct RenderInput *)window,
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
        PartyStatus_DrawBars((struct RenderInput *)window,
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
    Func_08118130__screens(0, standbyDjinn);
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
      Dma_Set(gPartyStatus_DigitTiles, (void *)0x06003400, DMA_COPY_WORDS(72),
              REG_DMA3);
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
#undef DJINN_COUNT_TILE
