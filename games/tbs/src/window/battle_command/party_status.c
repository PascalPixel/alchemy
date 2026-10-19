/// @file
/// Part of battle_command.c, the menu object, compiled only through it.
/// The party status window.

#include "../battle_command_internal.h"

// The party status window: one column of six tiles per party member (name,
// HP and, when shown, PP with a bar under each), and in battle an optional
// five-tile column of standby Djinn counts on its left. It sits at the top
// right of the screen. Its flags:
#define PARTY_STATUS_SHOW_PP 1     ///< add the PP row
#define PARTY_STATUS_SHOW_DJINN 2  ///< add the Djinn column (needs the PP row)
#define PARTY_STATUS_CLOSED 9      ///< erase the window
#define PARTY_STATUS_KEEP_FLAGS -1 ///< redraw with the flags it has
#define PARTY_STATUS_COLUMN_WIDTH 6
#define PARTY_STATUS_DJINN_WIDTH 5

/// The heap block the party status window's layout lives in.
#define PARTY_STATUS_BLOCK 16

/// The address of pixel row 7 - `up` of BG tile `tile` (4bpp, 32 bytes a tile,
/// 4 bytes a row): the status bars are drawn from a tile's bottom row up.
#define TILE_ROW_FROM_BOTTOM(tile, up)                                         \
  ((u32 *)((tile) * 32 - (up) * 4 + 0x0600001c))

/// Works out where the party status window goes for `flags`: one column per
/// party member (those in battle when a battle is on), plus the Djinn column,
/// right-aligned on the 30-tile screen row at the top. It is four tiles high
/// outside battle and three in battle, plus one with the PP row.
void UiWindow_BuildLayoutBounds(s32 flags) {
  // UB: slot[-1] steps before the single party-status work slot. Reading
  // the actual owners grows the EN function from 0x68 to 0x6c bytes.
  // Keep the original address formation only in the matching build.
#ifdef UBFIX
  struct PartyStatusWindow *layout = WORK_SLOT(PARTY_STATUS_BLOCK);
  struct UiRenderWork *work = UI_WORK;
#else
  void **slot = (void **)&gPartyStatusWork;
  struct PartyStatusWindow *layout = *slot;
  struct UiRenderWork *work = slot[-1];
#endif
  s32 height = 4;
  s32 members;
  s32 width;
  s32 x;

  if (work->menuState != 0) {
    members = BattleParty_PrepareActiveOwnersFar(0);
    height = 3;
  } else {
    members = Party_CountActiveOwnersFar();
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
/// heap block and draws it. The UI is held busy while it is drawn.
void UiWindow_CreateWithLayoutBounds(s32 flags) {
  s32 zero;
  struct PartyStatusWindow *layout;
  s8 *busy;

  layout = Runtime_AllocateBlock(PARTY_STATUS_BLOCK, sizeof(*layout));
  busy = (s8 *)&UI_WORK->menuBusy;
  zero = 0;
  *busy = 1;
  UiWindow_BuildLayoutBounds(flags);
  layout->window =
      UiWindow_Create(layout->x, layout->y, layout->width, layout->height, 6);
  PartyStatusDraw(flags);
  *busy = zero;
}

/// Draws the vertical lines between the members' columns of the party status
/// `window`, with their top and bottom joints, at the offsets listed in
/// gUiWindow_PartyColumnOffsets (shifted right past the Djinn column, whose own
/// line comes first in the list). In battle it also draws the window's bottom
/// edge.
void UiWindow_DrawColumnBorders(struct UiWindow *window, u32 flags) {
  struct UiRenderWork *work = UI_WORK;
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
  while (gUiWindow_PartyColumnOffsets[i] >= 0) {
    column = gUiWindow_PartyColumnOffsets[i] + shift;
    if (column < lastColumn) {
      for (row = 0; row != rows; row++) {
        cell = (u16 *)(((window->y + row) * 32 + (window->x + column)) * 2 +
                       (u32)work->tilemap);
        if (row == 0)
          *cell = UI_TILE_FRAME_TOP_TEE;
        else if (row == rows - 1)
          *cell = UI_TILE_FRAME_BOTTOM_TEE;
        else
          *cell = UI_TILE_DIVIDER_VERTICAL;
      }
    }
    i++;
  }
  if (work->menuState) {
    cell = work->tilemap + (window->y + window->height - 1) * 32 + window->x;
    column = 1;
    *cell++ = UI_TILE_BATTLE_FRAME_BOTTOM_LEFT;
    while (column < lastColumn) {
      column++;
      *cell++ = UI_TILE_BATTLE_FRAME_BOTTOM;
    }
    *cell = UI_TILE_BATTLE_FRAME_BOTTOM_RIGHT;
  }
  work->dirty = 1;
}

// Recolours the five bar tiles at (x, y) in `window` to show a bar `length`
// pixels long (0-40). The bar tiles are drawn in colour 14 with a colour 1
// shadow; each pixel left of the length becomes colour 8 (shadow 13), the
// rest colour 2 (shadow 12). Only the bottom row of each tile changes for an
// empty bar, the bottom three otherwise. Outside battle it first loads the
// basic colours into BG palette 14, its colour 14 taken from colour 4 of
// palette 15.
// The matching return type keeps pop {r1}; bx r1; void uses r0 instead.
#ifdef UBFIX
void UiWindow_DrawStatusBarTiles(struct UiWindow *window, s32 x, s32 y,
                                 s32 length) {
#else
s32 UiWindow_DrawStatusBarTiles(struct UiWindow *window, s32 x, s32 y,
                                s32 length) {
#endif
  s32 i;
  struct UiRenderWork *work = UI_WORK;
  s32 fullLength = length;
  s32 row;
  s32 column;
  u32 tile;
  u32 bar;
  u32 shadow;
  u32 pixels;
  u32 recoloured;

  if (work->menuState == 0) {
    Dma_Set((const void *)Palette_GetBasicObjColors(),
            (void *)&BG_PLTT_COLOR(14, 0), DMA_COPY16(16), REG_DMA3);
    BG_PLTT_COLOR(14, 14) = BG_PLTT_COLOR(15, 4);
  }
  x += window->x;
  y += window->y;
  for (i = 0; i < 5; i++, x++) {
    // This tile's eight pixels: filled up to `length`, empty after.
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
/// member's name, HP and optionally PP with their bars, and in battle the
/// standby Djinn of each element. The HP is drawn in text colour 2 at zero and
/// colour 4 at a quarter or less. When the flags change, the window is erased
/// and laid out again first.
void PartyStatusDraw(s32 flags) {
  // UB: the negative indices leave the single party-status work slot.
  // Named owners grow the EN function from 0x3d4 to 0x3dc bytes. Keep the
  // original address formation and load order in the matching build.
#ifdef UBFIX
  struct PartyStatusWindow *layout = WORK_SLOT(PARTY_STATUS_BLOCK);
  struct BattleWork *battle = gBattleWork;
  struct UiWindow *window = layout->window;
  struct UiRenderWork *work = UI_WORK;
#else
  void **slot = (void **)gPartyStatusWork;
  struct PartyStatusWindow *layout = slot[0];
  struct BattleWork *battle = slot[-7];
  struct UiWindow *window = layout->window;
  struct UiRenderWork *work = slot[-1];
#endif
  s32 x = 0;
  u32 count;
  s32 y = 0;
  u32 i;
  u16 members[5];
  u8 standbyDjinn[4];

  // List the members shown: those fighting in battle, the party otherwise.
  if (work->menuState != 0) {
    count = BattleParty_PrepareActiveOwnersFar(0);
    y = -1;
    for (i = 0; i < count; i++) {
      members[i] = battle->partyUnits[i];
      if (members[i] == 255)
        break;
    }
  } else {
    count = Party_CountActiveOwnersFar();
    for (i = 0; i < count; i++)
      members[i] = gGameState.activeOwners[i];
    members[i] = 255;
  }
  count = i;

  if (flags == PARTY_STATUS_KEEP_FLAGS)
    flags = layout->flags;
  if (!(flags & PARTY_STATUS_SHOW_PP))
    flags &= ~PARTY_STATUS_SHOW_DJINN;
  if (work->menuState == 0 || BattleParty_CountStandbyDjinnFar(0, 0) == 0)
    flags &= ~PARTY_STATUS_SHOW_DJINN;
  if (flags == PARTY_STATUS_CLOSED) {
    UiWindow_EraseBorderRect(layout->x, layout->y, layout->width,
                             layout->height);
    return;
  }

  // Redraw the frame, laying it out again when the flags changed.
  work->menuBusy = 1;
  if (layout->flags == flags) {
    RenderOutput_RedrawSavedRect(window);
    UiWindow_DrawColumnBorders(window, flags);
  } else {
    UiWindow_EraseBorderRect(layout->x, layout->y, layout->width,
                             layout->height);
    UiWindow_BuildLayoutBounds(flags);
    window->width = layout->width;
    window->height = layout->height;
    window->x = layout->x;
    UiWindow_DrawFrame(layout->x, layout->y, layout->width, layout->height);
    UiWindow_DrawColumnBorders(window, flags);
  }

  // Each member's column: name, HP and PP, each bar 40 pixels at full.
  if (flags & PARTY_STATUS_SHOW_DJINN)
    x = PARTY_STATUS_DJINN_WIDTH;
  for (i = 0; i != count; i++) {
    struct BattleUnit *unit = UnitGetFar(members[i]);
    s32 hp = unit->hp;
    s32 maxHp = unit->maxHp;
    s32 barLength;
    s32 current;

    if (hp == 0)
      UiWork_SetParamNibble(2);
    else if (hp <= maxHp / 4)
      UiWork_SetParamNibble(4);
    else
      UiWork_SetParamNibble(15);
    work->level = 14;
    if (work->menuState != 0)
      work->level = 5;
    UiText_DrawPrefixedNumberAtOffset(
        hp, window, (x + i * PARTY_STATUS_COLUMN_WIDTH) * 8, y * 8 + 8, 0);
    work->level = 15;
    UiText_DrawStringAtOffset(unit->name, window,
                              (x + i * PARTY_STATUS_COLUMN_WIDTH) * 8, y * 8);
    UiWork_SetParamNibble(15);
    if (unit->maxHp != 0) {
      // A member with any HP left shows at least one pixel of bar.
      current = unit->hp;
      barLength = ((current * 40) / unit->maxHp);
      if (barLength == 0 && current != 0)
        barLength = 1;
      UiWindow_DrawStatusBarTiles(window, x + i * PARTY_STATUS_COLUMN_WIDTH + 1,
                                  y + 2, barLength);
    }
    if (flags & PARTY_STATUS_SHOW_PP) {
      work->level = 14;
      if (work->menuState != 0)
        work->level = 5;
      UiText_DrawPrefixedNumberAtOffset(unit->pp, window,
                                        (x + i * PARTY_STATUS_COLUMN_WIDTH) * 8,
                                        y * 8 + 16, 1);
      if (unit->maxPp != 0) {
        current = unit->pp;
        barLength = ((current * 40) / unit->maxPp);
        if (barLength == 0 && current != 0)
          barLength = 1;
        UiWindow_DrawStatusBarTiles(
            window, x + i * PARTY_STATUS_COLUMN_WIDTH + 1, y + 3, barLength);
      }
    }
  }
  work->level = 15;

  // The Djinn column: each element's icon and its count of standby Djinn.
  if (work->menuState != 0 && (flags & PARTY_STATUS_SHOW_DJINN)) {
    s32 row = y;

    if (flags & PARTY_STATUS_SHOW_PP)
      row++;
    BattleParty_CountStandbyDjinnFar(0, standbyDjinn);
    UiWindow_SetTilemapEntry(window, 0x5001, 0, row, 0);
    UiWindow_SetTilemapEntry(window, 0x5002, 2, row, 0);
    UiWindow_SetTilemapEntry(window, 0x5003, 0, row + 1, 0);
    UiWindow_SetTilemapEntry(window, 0x5004, 2, row + 1, 0);
    UiWindow_PutGlyph(window, standbyDjinn[0] + '0', 1, row);
    UiWindow_PutGlyph(window, standbyDjinn[1] + '0', 3, row);
    UiWindow_PutGlyph(window, standbyDjinn[2] + '0', 1, row + 1);
    UiWindow_PutGlyph(window, standbyDjinn[3] + '0', 3, row + 1);
  }
  work->menuBusy = 0;
}

/// Closes the party status window and frees its heap block.
void PartyStatus_Close(void) {
  UiWork_Finalize((struct UiWindow *)**(s32 **)(u32)&gWindowWork[1], 1);
  Runtime_ReleaseHeapBlock(PARTY_STATUS_BLOCK);
}

/// Sets (`set` 1) or clears (0) bit 12 of the tilemap entries of the
/// `width` x `height` tile rectangle at (x, y) inside `window`, clipped to the
/// screen: bit 12 is the lowest bit of the entry's palette number, so this
/// switches the rectangle between two palettes. The clip uses 29 for the
/// bottom edge test and 20 for the clipped height.
void UiWindow_SetTileAttributeBitRect(const struct UiWindow *window, s32 x,
                                      s32 y, s32 width, s32 height, u32 set) {
  struct UiRenderWork *work = UI_WORK;

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
