/// @file
/// The Pause menu's choices, a tilemap palette-bit rectangle, and the debug
/// browsers of icons and portraits.
// Part of screens.c.

#include "window_internal.h"
#include "window/item_icon.h"
#include "window/window.h"

/// Handles Save your quest, Sleep and Change settings from the Pause menu.
/// Saving is refused, with its message, while FLAG_CANNOT_SAVE_HERE or
/// FLAG_NO_PLACE_TO_SAVE is set. A refused or cancelled save or a cancelled
/// settings menu reopens the Pause menu on the same entry; cancelling the Pause
/// menu returns -1. Sleep shows its message and sets gSleepRequested. Otherwise
/// returns 0.
s32 PauseMenu_Run(void) {
  s32 selection = 0;

again:
  selection = PauseMenu_Choose(selection);
  if (selection == -1)
    return -1;
  if (selection == PAUSE_MENU_SAVE_QUEST) {
    if (GameFlag_Get(FLAG_CANNOT_SAVE_HERE)) {
      // "Cannot save here"
      FieldMessage_Show((s32)MsgCannotSaveHere, FIELD_MESSAGE_ZOOM);
      goto again;
    }
    if (GameFlag_Get(FLAG_NO_PLACE_TO_SAVE)) {
      // "This is no place for that!"
      FieldMessage_Show((s32)MsgNoPlaceForThat, FIELD_MESSAGE_ZOOM);
      goto again;
    }
    if (Save_ToChosenFile() == -1)
      goto again;
  } else if (selection == PAUSE_MENU_SLEEP) {
    // "Activating Sleep Mode. Press Select and the L & R Buttons to exit."
    FieldMessage_Show((s32)MsgSleepMode, FIELD_MESSAGE_ZOOM);
    gSleepRequested = selection;
  } else if (selection == PAUSE_MENU_CHANGE_SETTINGS) {
    if (SettingsMenu_Run() == -1)
      goto again;
  }
  return 0;
}

/// Sets (`set` 1) or clears (0) bit 12 of the tilemap entries of the `width` x
/// `height` tile rectangle at (x, y) inside `window`, clipped to the screen:
/// bit 12 is the lowest bit of the entry's palette number, so this switches the
/// rectangle between two palettes, such as 15 and 14. Nothing calls it.
UNUSED void Window_SetTextTileBitRect(const struct Window *window, s32 x, s32 y,
                                      s32 width, s32 height, u32 set) {
  struct WindowState *state = (struct WindowState *)Ram_HeapSlots->windowState;

  x += (s16)window->x + 1;
  y += (s16)window->y + 1;
  set &= 1;
  set = BG_TILE_PALETTE(set);
  if (x < 0) {
    width += x;
    x = 0;
  }
  if (x + width > DISPLAY_TILE_WIDTH - 1) {
    width = DISPLAY_TILE_WIDTH - x;
  }
  if (y < 0) {
    height += y;
    y = 0;
  }
  // BUG: tests the bottom against the screen's width, so a window reaching
  // below the screen draws into the map's unseen rows.
#ifdef BUGFIX
  if (y + height > DISPLAY_TILE_HEIGHT - 1) {
#else
  if (y + height > DISPLAY_TILE_WIDTH - 1) {
#endif
    height = DISPLAY_TILE_HEIGHT - y;
  }
  if (width > 0 && height > 0) {
    do {
      u16 *cell = &state->tilemap[y][x];
      s32 remaining = width;
      while (remaining != 0) {
        u32 entry = *cell;
        entry &= ~BG_TILE_PALETTE(1);
        entry |= set;
        remaining--;
        *cell = entry;
        cell++;
      }
      height--;
      y++;
    } while (height != 0);
    state->dirty = 1;
  }
}

/// The icon browser's kinds (items, Psynergy and status patterns), and its
/// pages of ICON_BROWSER_PAGE_ICONS icons, ICON_BROWSER_ROW_ICONS to a row.
#define ICON_BROWSER_KINDS 3
#define ICON_BROWSER_PAGES 12
#define ICON_BROWSER_PAGE_ICONS 32
#define ICON_BROWSER_ROW_ICONS 8

/// Browses pages of item, Psynergy and status-pattern icons, suspending the
/// field's actor system while the browser owns its window and cache entries.
/// L and R switch the kind, Left and Right turn its pages (item and Psynergy
/// icons from page * ICON_BROWSER_PAGE_ICONS on; the status patterns are the
/// first page's on every page, whatever the heading says), A or B closes it.
/// Returns 0. The debug rooms open it (debug/level.c, debug/psynergy_test.c,
/// debug/shop_test.c).
s32 DebugBrowser_Icons(void) {
  s32 redraw;
  s32 tile;
  s32 page;
  s32 kind;
  struct Window *window;
  s32 slot;
  s32 base;
  s32 i;
  s32 row;
  s32 x;
  s32 y;

  window = NULL;
  page = 0;
  kind = 0;
  redraw = 1;
  Ram_HeapSlots->actorSystem->suspended = redraw;
  wait(1);

  while (1) {
    if (gInput.repeat & KEY_LEFT) {
      page--;
      redraw = 1;
    }
    if (gInput.repeat & KEY_RIGHT) {
      redraw = 1;
      page++;
    }
    if (gInput.repeat & KEY_L) {
      kind--;
      redraw = 1;
    }
    if (gInput.repeat & KEY_R) {
      redraw = 1;
      kind++;
    }
    if (gInput.repeat & KEY_A)
      break;
    if (gInput.repeat & KEY_B)
      break;
    if (redraw != 0) {
      redraw = 0;
      page = (page + ICON_BROWSER_PAGES) % ICON_BROWSER_PAGES;
      kind = (kind + ICON_BROWSER_KINDS) % ICON_BROWSER_KINDS;
      Window_Close(window, 2);
      window = Window_Create(10, 0, 18, 12, WINDOW_FLAG_INSTANT);
      if (kind == 0)
        Text_DrawStringInWindow(gIconBrowser_ItemTitle, window, 0, 0);
      else if (kind == 1)
        Text_DrawStringInWindow(gIconBrowser_EnergyTitle, window, 0, 0);
      else
        Text_DrawStringInWindow(gIconBrowser_StatusTitle, window, 0, 0);
      Text_DrawStringInWindow(gIconBrowser_PageLabel, window, 0, 8);
      Text_DrawNumberInWindow(page, 0, window, 40, 8);
      base = page * ICON_BROWSER_PAGE_ICONS;
      Text_DrawNumberInWindow(base, 3, window, 64, 8);
      Text_DrawStringInWindow(gIconBrowser_RangeDash, window, 88, 8);
      Text_DrawNumberInWindow(base + ICON_BROWSER_PAGE_ICONS - 1, 3, window, 96,
                              8);
      for (i = 0; i < ICON_BROWSER_PAGE_ICONS; i++) {
        slot = -1;
        row = i / ICON_BROWSER_ROW_ICONS;
        x = (i - row * ICON_BROWSER_ROW_ICONS) * 16;
        y = row * 16 + 16;
        if (kind == 0) {
          Icon_BuildItemIconTiles(base + i, 1, &slot, &tile, kind);
          WindowSprite_Create(slot, OUTPUT_SIZE_16X16, window, x, y);
        } else if (kind == 1) {
          Icon_BuildAbilityIconTiles(base + i, 1, &slot, &tile, 0);
          WindowSprite_Create(slot, OUTPUT_SIZE_16X16, window, x, y);
        } else {
          slot = VramCache_FindFreeEntry();
          Icon_BuildMiscIconTiles(i, 0, slot);
          WindowSprite_Create(slot, OUTPUT_SIZE_16X16, window, x, y);
        }
      }
    }
    wait(1);
  }
  Window_Close(window, 2);
  Ram_HeapSlots->actorSystem->suspended = 0;
  return 0;
}

/// A debug browser of the portraits: left and right step through
/// gPortrait_CharacterMap and then gPortrait_SpriteMap, L and R by
/// DEBUG_LARGE_STEP, each drawn with its number and its name from MsgTestName;
/// A or B leaves.
s32 DebugBrowser_Portraits(void) {
  struct WindowState *state;
  struct Window *portrait;
  s32 redraw;
  struct Window *window;
  s32 characterCount;
  s32 entryCount;
  s32 index;
  s32 i;
  s32 glyph;
  s32 tile;
  s32 slot;
  /* FAKEMATCH(temporary): cancelKey, KEY_B held in a local across the loop. */
  s32 cancelKey;
  s32 palette = PORTRAIT_PALETTE + 1;
  /* FAKEMATCH(temporary): slotOffset, the second portrait slot's offset held
   * in a local and added to state through a byte pointer. */
  u32 slotOffset = offsetof(struct WindowState, portraitSprites[1]);

  state = (struct WindowState *)Ram_HeapSlots->windowState;
  redraw = 1;
  portrait = Portrait_OpenWindow(0, 0, 10, 5);
  window = Window_Create(10, 10, 14, 3, WINDOW_FLAG_INSTANT);
  index = 0;
  for (i = 0; gPortrait_CharacterMap[i * 2] != -1; i++) {
  }
  characterCount = i;
  for (i = 0; gPortrait_SpriteMap[i * 2] != -1; i++) {
  }
  entryCount = characterCount + i;
  /* FAKEMATCH(dead): a zero from the count. */
  cancelKey = ((u32)(u16)characterCount >> 16) | KEY_B;
  while (1) {
    if (gInput.repeat & KEY_LEFT) {
      redraw = 1;
      index--;
    }
    if (gInput.repeat & KEY_RIGHT) {
      redraw = 1;
      index++;
    }
    if (gInput.repeat & KEY_L) {
      redraw = 1;
      index -= DEBUG_LARGE_STEP;
    }
    if (gInput.repeat & KEY_R) {
      redraw = 1;
      index += DEBUG_LARGE_STEP;
    }
    if (gInput.repeat & KEY_A)
      break;
    if (gInput.repeat & cancelKey)
      break;
    if (redraw) {
      redraw = 0;
      index = (index + entryCount) % entryCount;
      WindowSprite_PrepareForRedraw(window);
      if (index < characterCount)
        glyph = gPortrait_CharacterMap[index * 2 + 1];
      else
        glyph = gPortrait_SpriteMap[(index - characterCount) * 2 + 1] +
                PORTRAIT_FIRST_SPRITE_PORTRAIT;
      slot = *(u16 *)((u8 *)state + slotOffset);
      Icon_LoadWithPalette(glyph, 0, &slot, &tile, palette, 1);
      Text_DrawNumberInWindow(index, 2, window, 0, 0);
      Text_DrawMessageAsTiles(index + (s32)MsgTestName, window, 24, 0);
    }
    wait(1);
  }
  Window_Close((struct Window *)window, 2);
  Window_Close((struct Window *)portrait, 2);
  wait(1);
  return 0;
}
