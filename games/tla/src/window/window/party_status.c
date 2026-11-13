/// @file
/// The party status window.
// Part of window.c.

#include "../window_internal.h"
#include "window/menu_list.h"
#include "module/system/system.h"
#include "module/window/screens.h"
#include "module/window/window.h"

void PartyStatus_BuildLayoutBounds(s32 flags) {
  struct UiWindowBounds *state = Ram_HeapSlots->windowLayout;
  u8 *base = Ram_HeapSlots->windowTiles;
  s32 height = 4;
  s32 n;
  s32 right;
  s32 left;

  if (base[5] != 0) {
    n = BattleParty_PrepareMembersFar(0);
    height = 3;
  } else {
    n = Party_CountMembersFar();
    if ((u32)n > 4)
      n = 4;
  }
  if (flags & 1)
    height++;
  else
    flags &= -3;

  n *= 6;
  right = n + 1;
  if (flags & 2)
    right = n + 6;

  left = 30;
  left -= right;
  state->left = left;
  state->top = 0;
  state->right = right;
  state->height = height;
  state->flags = flags;
}

void PartyStatus_CreateWindow(s32 flags) {
  s32 zero;
  struct UiWindowBounds *window;
  u8 *tiles;

  window = HeapSlot_AllocEwram(64, sizeof(struct UiWindowBounds));
  tiles = Ram_HeapSlots->windowTiles;
  zero = 0;
  tiles[6] = 1;
  PartyStatus_BuildLayoutBounds(flags);
  window->handle = (s32)Window_Create(window->left, window->top, window->right,
                                      window->height, 6);
  PartyStatus_Draw(flags);
  tiles[6] = zero;
}

/// Draws the vertical lines between the party members' columns of the party
/// status `window`, with their top and bottom joints, at the offsets listed in
/// gWindow_PartyColumnOffsets (shifted right past the Djinn column, whose own
/// line comes first in the list). In battle it also draws the window's bottom
/// edge.
void Window_DrawColumnBorders(struct Window *window, u32 flags) {
  struct WindowWork *canvas = (struct WindowWork *)Ram_HeapSlots->windowTiles;
  s32 first = 1;
  s32 bias = 0;
  u32 max = window->width - 1;
  s32 i;
  u8 state = canvas->menuState;
  u32 rows = window->height;
  u32 col;
  u32 row;
  u16 *dest;
  const s8 *table;

  if ((flags & 1) == 0)
    flags &= ~2;
  if (flags & 2) {
    bias = 5;
    first = 0;
  }
  {
    // columns holds the table's address before i is set; as
    // table = gWindow_PartyColumnOffsets after i = first, 41 of 137
    // instructions differ in tla-en.
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
    state = canvas->menuState;
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
  struct WindowWork *canvas = (struct WindowWork *)Ram_HeapSlots->windowTiles;
  s32 filled = value;
  s32 row;
  s32 col;
  u32 id;
  u32 light;
  u32 dark;
  u32 pixels;
  u32 result;

  if (canvas->menuState == 0) {
    Dma_Set((const void *)Palette_GetBasicSpriteColors(),
            (void *)&BG_PLTT_COLOR(14, 0), 0x80000010, REG_DMA3);
    BG_PLTT_COLOR(14, 14) = BG_PLTT_COLOR(15, 4);
  }
  x += (s16)window->x;
  y += (s16)window->y;
  for (i = 0; i < 5; i++, x++) {
    id = canvas->tilemap[0][y * 32 + x];
    light = 0x22222222;
    dark = 0xcccccccc;
    id &= 0x3ff;
    if (value > 7) {
      light = 0x88888888;
      dark = 0xdddddddd;
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

#define PARTY_STATUS_KEEP_FLAGS -1
#define PARTY_STATUS_SHOW_PP 1
#define PARTY_STATUS_SHOW_DJINN 2
#define PARTY_STATUS_CLOSED 9
#define PARTY_STATUS_DJINN_WIDTH 5
#define PARTY_STATUS_COLUMN_WIDTH 6

/* VIEW(screens): routine BattleDjinn_CountStandbyFar as s32
 * BattleDjinn_CountStandbyFar(s32 a, u8 *counts); module/window/window.h has
 * void BattleDjinn_CountStandbyFar(s32 kind, u8 *counts); not tried:
 * BattleDjinn_CountStandbyFar's bound declaration was not found. */
extern s32
Func_08118130__screens(s32 a,
                       u8 *counts) __asm__("BattleDjinn_CountStandbyFar");
// Text_DrawPrefixedNumberAtOffset is called with no prototype in scope, as The
// Broken Seal's screens.c calls it: with one, hp reaches r0 before the call's
// other arguments rather than last (2 of 495 instructions in tla-en).

#define DJINN_COUNT_TILE(n) ((n) > 9 ? (n) + 0xf196 : ((n) + '0') | 0xf000)

void PartyStatus_Draw(s32 flags) {
  struct PartyStatusWindow *layout = Ram_HeapSlots->windowLayout;
  struct BattleWork *battle = Ram_HeapSlots->battleWork;
  struct Window *window = layout->window;
  struct WindowWork *work = (struct WindowWork *)Ram_HeapSlots->windowTiles;
  s32 x = 0;
  u32 count;
  s32 y = 0;
  u32 i;
  u16 members[5];
  u8 standbyDjinn[4];

  if (work->menuState != 0) {
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
      members[i] = gMainRoutineWork.activeOwners[i];
    members[i] = 255;
  }
  count = i;

  if (flags == PARTY_STATUS_KEEP_FLAGS)
    flags = layout->flags;
  if (!(flags & PARTY_STATUS_SHOW_PP))
    flags &= ~PARTY_STATUS_SHOW_DJINN;
  if (work->menuState == 0 || Func_08118130__screens(0, 0) == 0)
    flags &= ~PARTY_STATUS_SHOW_DJINN;
  if (flags == PARTY_STATUS_CLOSED) {
    Window_EraseBorderRect(layout->x, layout->y, layout->width, layout->height);
    return;
  }

  work->menuBusy = 1;
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
    struct StatusUnit *unit = Unit_Get(members[i]);
    s32 hp = unit->hp;
    s32 maxHp = unit->maxHp;
    s32 barLength;
    s32 current;

    if (hp == 0)
      Text_SetColor(2);
    else if (hp <= maxHp / 4)
      Text_SetColor(4);
    else
      Text_SetColor(15);
    work->level = 14;
    if (work->menuState != 0)
      work->level = 5;
    Text_DrawPrefixedNumberAtOffset(
        hp, window, (x + i * PARTY_STATUS_COLUMN_WIDTH) * 8, y * 8 + 8, 0);
    work->level = 15;
    Text_DrawStringAtOffset(unit->name, (struct RenderInput *)window,
                            (x + i * PARTY_STATUS_COLUMN_WIDTH) * 8, y * 8);
    Text_SetColor(15);
    if (unit->maxHp != 0) {
      current = unit->hp;
      barLength = (current * 40) / unit->maxHp;
      if (barLength == 0 && current != 0)
        barLength = 1;
      PartyStatus_DrawBars((struct RenderInput *)window,
                           x + i * PARTY_STATUS_COLUMN_WIDTH + 1, y + 2,
                           barLength);
    }
    if (flags & PARTY_STATUS_SHOW_PP) {
      work->level = 14;
      if (work->menuState != 0)
        work->level = 5;
      Text_DrawPrefixedNumberAtOffset(unit->pp, window,
                                      (x + i * PARTY_STATUS_COLUMN_WIDTH) * 8,
                                      y * 8 + 16, 1);
      if (unit->maxPp != 0) {
        current = unit->pp;
        barLength = (current * 40) / unit->maxPp;
        if (barLength == 0 && current != 0)
          barLength = 1;
        PartyStatus_DrawBars((struct RenderInput *)window,
                             x + i * PARTY_STATUS_COLUMN_WIDTH + 1, y + 3,
                             barLength);
      }
    }
  }
  work->level = 15;

  if (work->menuState != 0 && (flags & PARTY_STATUS_SHOW_DJINN)) {
    s32 row = y;

    /* FAKEMATCH(dead): i set to a 0 never read. */
    i = 0;
    if (flags & PARTY_STATUS_SHOW_PP)
      row++;
    Func_08118130__screens(0, standbyDjinn);
    Window_SetTilemapEntry(window, 0x5001, 0, row, 0);
    Window_SetTilemapEntry(window, 0x5002, 2, row, 0);
    Window_SetTilemapEntry(window, 0x5003, 0, row + 1, 0);
    Window_SetTilemapEntry(window, 0x5004, 2, row + 1, 0);
    if (standbyDjinn[0] > 9 || standbyDjinn[1] > 9 || standbyDjinn[2] > 9 ||
        standbyDjinn[3] > 9)
      Dma_Set(Data_080aa1b8, (void *)0x06003400, 0x84000048, REG_DMA3);
    Window_SetTilemapEntry(window, DJINN_COUNT_TILE(standbyDjinn[0]), 1, row,
                           0);
    Window_SetTilemapEntry(window, DJINN_COUNT_TILE(standbyDjinn[1]), 3, row,
                           0);
    Window_SetTilemapEntry(window, DJINN_COUNT_TILE(standbyDjinn[2]), 1,
                           row + 1, 0);
    Window_SetTilemapEntry(window, DJINN_COUNT_TILE(standbyDjinn[3]), 3,
                           row + 1, 0);
  }
  work->menuBusy = 0;
}

void PartyStatus_Close(void) {
  Window_Close(*(void **)Ram_HeapSlots->windowLayout, 1);
  HeapSlot_Release(64);
}

#undef TILE_ROW_FROM_BOTTOM
#undef PARTY_STATUS_KEEP_FLAGS
#undef PARTY_STATUS_SHOW_PP
#undef PARTY_STATUS_SHOW_DJINN
#undef PARTY_STATUS_CLOSED
#undef PARTY_STATUS_DJINN_WIDTH
#undef PARTY_STATUS_COLUMN_WIDTH
#undef DJINN_COUNT_TILE
