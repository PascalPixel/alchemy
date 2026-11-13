/// @file
/// The battle Psynergy menu.
// Part of window.c.

#include "../window_internal.h"
#include "sounds.h"
#include "window_tile.h"
#include "module/window/screens.h"
#include "menu/psynergy_menu.h"

#define NAVIGATION ((struct PsynergyMenuNavigation *)Ram_HeapSlots->unke4[0])

// How much of the description under the cursor is copied: more outside
// Japanese.
#if LANGUAGE == LANGUAGE_JAPANESE
#define CURSOR_TEXT_LENGTH 32
#else
#define CURSOR_TEXT_LENGTH 52
#endif

// Japanese decodes the text under the cursor raw where the other languages
// copy it expanded.
#if LANGUAGE != LANGUAGE_JAPANESE
#define CURSOR_TEXT_COPY Message_CopyExpanded
#else
#define CURSOR_TEXT_COPY Message_DecodeRaw
#endif

// The list is a tile wider in Spanish and French, and its columns move with it.
#if (LANGUAGE == LANGUAGE_SPANISH) || (LANGUAGE == LANGUAGE_FRENCH)
#define LIST_WINDOW_X 8
#define LIST_WINDOW_WIDTH 22
#define COST_COLUMN 12
#define COST_X 112
#define RANGE_COLUMN 16
#else
#define LIST_WINDOW_X 9
#define LIST_WINDOW_WIDTH 21
#define COST_COLUMN 11
#define COST_X 104
#define RANGE_COLUMN 15
#endif

// Sets a menu sprite's attribute words, as The Broken Seal's
// MenuSprite_Initialize.
#define MENU_SPRITE_INITIALIZE(sprite, shape)                                  \
  do {                                                                         \
    (sprite).oam.word.attr01 = (shape);                                        \
    (sprite).oam.word.attr23 = 0;                                              \
  } while (0)

// The menu's page, row and preferred row, as The Broken Seal's
// BATTLE_MENU_GET_CURSOR and BATTLE_MENU_SET_CURSOR.
#define MENU_GET_CURSOR(page_, row_, preferred)                                \
  do {                                                                         \
    (page_) = NAVIGATION->page;                                                \
    (row_) = NAVIGATION->row;                                                  \
    (preferred) = NAVIGATION->preferredRow;                                    \
  } while (0)

#define MENU_SET_CURSOR(page_, row_, preferred)                                \
  do {                                                                         \
    NAVIGATION->page = (page_);                                                \
    NAVIGATION->row = (row_);                                                  \
    NAVIGATION->preferredRow = (preferred);                                    \
  } while (0)

// Marks a window's tilemap row for upload, as The Broken Seal's
// Window_MarkTilemapRow.
#define WINDOW_MARK_TILEMAP_ROW(work, row)                                     \
  do {                                                                         \
    (work)->dirty |= 2 << ((u32)(row) >> 2);                                   \
  } while (0)

/// The battle Psynergy list for unit `unitId`: `count` abilities of `actions`
/// (ended by 0), five to a page, each with its icon, name, PP cost, element
/// and range icon, in TEXT_COLOR_YELLOW when not usable in battle,
/// TEXT_COLOR_RED when the unit's PP fall short and TEXT_COLOR_PINK while its
/// Psynergy is sealed; the window above describes the one under the cursor.
/// Returns the chosen ability's place in `actions`, or -1 when cancelled, when
/// the link battle's time runs out or when the list is empty. Only an ability
/// not usable in battle is refused here, silently: one the PP cannot pay for,
/// or a sealed party member's, can be chosen. The Psynergy command runs it
/// (Battle_CollectPartyCommands), listing only abilities usable in battle.
s32 BattlePsynergyMenu_Run(s32 unitId, u16 *actions, s32 count) {
  struct PsynergyMenuWork *work;
  s32 drawnFirstEntry;
  s32 firstEntry;
  s32 drawnRow;
  s32 cursorHandle;
  struct PsynergyMenuUnit *unit;
  struct PsynergyMenuWindow *messageWindow;
  s32 iconCount;
  s32 preferredRow;
  struct {
    s32 x, y;
  } cursorPosition;
  struct PsynergyMenuNavigation *navigation;
  struct PsynergyMenuWindow *window;
  s32 selectedRow;
  s32 encodedAction;
  s32 iconTile;
  s32 pageTile;
  s32 rangePattern;
  s32 result;
  s32 i;
  struct PsynergyMenuSprite cursor;
  struct PsynergyMenuSprite *iconSprite;
  u16 messageText[64];
  s32 iconSlots[5];
  struct PsynergyMenuSprite sprites[5];
  s32 glyph;

  work = (struct PsynergyMenuWork *)Ram_HeapSlots->windowTiles;
  drawnRow = -1;
  drawnFirstEntry = -1;
  cursorHandle = VramCache_AllocEntry(128);
  unit = Unit_Get(unitId);
  messageWindow = Window_Create(0, 5, 30, 4, 42);
  iconCount = 5;
  MENU_GET_CURSOR(firstEntry, selectedRow, preferredRow);
  window = Window_Create(LIST_WINDOW_X, 9, LIST_WINDOW_WIDTH, 11, 6);

  for (i = 0; i < 5; i++) {
    s32 y;
    // t holds the attribute zero, then is reused for the window's y; as plain
    // stores, 2 of 817 instructions differ in tla-en.
    u32 t = 0;

    y = i * 2;
    /* FAKEMATCH(dead): an empty once-only block after the row, which keeps it
     * and the zero ahead of the stores; without it, 10 of 817 instructions
     * differ in tla-en. */
    do {
    } while (0);
    iconSprite = &sprites[i];
    sprites[i].oam.word.attr01 = 0x40000000;
    sprites[i].oam.word.attr23 = t;
    iconSprite->oam.f.x = window->x * 8 + 8;
    t = window->y;
    iconSprite->oam.f.y = (y + t) * 8 + 4;
  }
  for (i = 0; i < 5; i++) {
    iconSlots[i] = VramCache_AllocEntry(128);
    sprites[i].oam.f.tile = VramCache_Reload(iconSlots[i], (const void *)-1);
  }
  Vram_CopyTile(0xf018, 0x200);
  Vram_CopyTile(0xf018, 0x201);
  Vram_CopyTile(0xf019, 0x210);
  Vram_CopyTile(0xf019, 0x211);

  for (;;) {
    /* FAKEMATCH(temporary): iconSprite holds &cursor; as cursor used directly,
     * 304 of 817 instructions differ in tla-en. */
    iconSprite = &cursor;
    if (firstEntry != drawnFirstEntry || selectedRow != drawnRow) {
      work->holdTilemapUpload = 1;
      Window_HighlightRect(window->x + 1, window->y + drawnRow * 2 + 1,
                           window->width - 2, 1, 15);
      Text_FillCanvasBackground();
      if (count != 0)
        CURSOR_TEXT_COPY((actions[firstEntry + selectedRow] & 0x3fff) +
                             (s32)MsgAbilityDescription,
                         messageText, CURSOR_TEXT_LENGTH);
      else
        CURSOR_TEXT_COPY((s32)MsgKey0d48, messageText, CURSOR_TEXT_LENGTH);
      Text_DrawWideStringAtOffset(messageText, messageWindow, 0, 4);
      drawnRow = selectedRow;
      if (firstEntry != drawnFirstEntry) {
        WindowSprite_RedrawSavedRect(window);
        for (i = 0; i < 5; i++) {
          struct PsynergyAbility *action;

          encodedAction = actions[firstEntry + i];
          if (encodedAction == 0)
            break;
          action = Ability_Get(encodedAction);
          Window_SetTilemapEntry(window, WINDOW_TILE_PP_LABEL, COST_COLUMN,
                                 i * 2, 0);
          Window_SetTilemapEntry(window, WINDOW_TILE_PP_LABEL_END,
                                 COST_COLUMN + 1, i * 2, 0);
          Ability_LoadGlyph(encodedAction & 0x3fff, 0, &iconSlots[i], &glyph,
                            1);
          sprites[i].oam.f.tile = glyph;
          if ((action->flags & 0x80) == 0)
            Text_SetColor(4);
          else if (action->ppCost > unit->pp)
            Text_SetColor(2);
          else if (unit->psySeal != 0)
            Text_SetColor(9);
          work->textPalette = 5;
          Text_DrawMessageAsTiles(encodedAction + (s32)MsgAbilityNothing,
                                  window, 16, i * 16);
          Text_DrawNumberAtOffset(action->ppCost, 2, window, COST_X, i * 16);
          Text_SetColor(15);
          work->textPalette = 15;
          if (action->element != 4)
            Window_SetTilemapEntry(window, action->element + 0x5001,
                                   RANGE_COLUMN, i * 2, 0);
          rangePattern = action->range;
          if (rangePattern == 255)
            rangePattern = 11;
          else
            rangePattern--;
          Window_DrawThreeTileColumn(window, RANGE_COLUMN + 1, i * 2,
                                     rangePattern, 0);
        }
        iconCount = i;
        drawnFirstEntry = firstEntry;
      }
      if (count > 5) {
        for (i = 0; i < (count + 4) / 5; i++) {
          iconTile = i + 0xf301;
          if (i == firstEntry / 5)
            iconTile = i + 0xf30b;
          Window_SetTilemapEntry(
              window, iconTile, window->width - (count + 4) / 5 + i - 2, -1, 0);
        }
      }
      Window_HighlightRect(window->x + 1, window->y + selectedRow * 2 + 1,
                           window->width - 2, 1, 14);
      work->dirty = 1;
      work->holdTilemapUpload = 0;
    }

    if (count > 5) {
      for (i = 0; i < (count + 4) / 5; i++) {
        iconTile = i + 0xf301;
        if ((gFrameCount & 15) <= 11) {
          if (i == firstEntry / 5)
            iconTile = i + 0xf30b;
        }
        Window_SetTilemapEntry(window, iconTile,
                               window->width - (count + 4) / 5 + i - 2, -1, 0);
      }
      pageTile = -1;
      Window_SetTilemapEntry(window, 0xf334,
                             window->width - (count + 4) / 5 - 3, pageTile, 0);
      Window_SetTilemapEntry(window, 0xf335, window->width - 2, pageTile, 0);
      WINDOW_MARK_TILEMAP_ROW(work, window->y - 1);
    }

    for (i = 0; i < iconCount; i++)
      Oam_AddSpriteAtDepth(&sprites[i], 240);

    cursorPosition.x = window->x * 8 - 4;
    cursorPosition.y = (selectedRow * 2 + window->y) * 8 + 20;
    ((u32 *)iconSprite)[1] = 0x40000000;
    ((u32 *)iconSprite)[2] = 0;
    iconSprite->oam.f.tile =
        VramCache_Reload(cursorHandle, gResource_FixedBlockBTiles);
    iconSprite->oam.f.x = cursorPosition.x + ((gFrameCount & 4) >> 1) - 4;
    iconSprite->oam.f.y = cursorPosition.y - ((gFrameCount & 4) >> 2) - 8;
    if (count != 0)
      Oam_AddSpriteAtDepth(iconSprite, 242);

    MENU_SET_CURSOR(firstEntry, selectedRow, preferredRow);
    navigation = NAVIGATION;
    if (gInput.pressed & 1) {
      if (count != 0) {
        result = firstEntry + selectedRow;
        if ((Ability_Get(actions[result])->flags & 0x80) == 0) {
          // Not usable in battle: the menu stays open.
        } else {
          break;
        }
      } else {
        result = -1;
        break;
      }
    } else if (navigation->timer == 0 || (gInput.pressed & 2)) {
      Sound_PlayFar(SE_MENU_CANCEL);
      result = -1;
      break;
    }

    if (count != 0) {
      if (gInput.repeat & 0x80) {
        Sound_PlayFar(SE_MENU_CURSOR_MOVE);
        selectedRow++;
        if (selectedRow == 5 || firstEntry + selectedRow == count)
          selectedRow = 0;
        preferredRow = selectedRow;
      } else if (gInput.repeat & 0x40) {
        Sound_PlayFar(SE_MENU_CURSOR_MOVE);
        selectedRow--;
        if (selectedRow < 0) {
          if (firstEntry == (count - 1) / 5 * 5)
            selectedRow = count - firstEntry - 1;
          else
            selectedRow = 4;
        }
        preferredRow = selectedRow;
      } else if (gInput.repeat & 0x10) {
        Sound_PlayFar(SE_MENU_CURSOR_MOVE);
        Input_ResetKeyRepeat();
        if (firstEntry + 5 >= count) {
          if (firstEntry != 0) {
            firstEntry = 0;
            selectedRow = preferredRow;
          }
        } else {
          firstEntry += 5;
          selectedRow = preferredRow;
          if (firstEntry == (count - 1) / 5 * 5) {
            selectedRow = count - firstEntry - 1;
            if (selectedRow > preferredRow)
              selectedRow = preferredRow;
          }
        }
      } else if (gInput.repeat & 0x20) {
        Sound_PlayFar(SE_MENU_CURSOR_MOVE);
        Input_ResetKeyRepeat();
        if (firstEntry != 0) {
          firstEntry -= 5;
          selectedRow = preferredRow;
        } else {
          firstEntry = (count - 1) / 5 * 5;
          selectedRow = preferredRow;
          if (firstEntry != 0) {
            selectedRow = count - firstEntry - 1;
            if (selectedRow > preferredRow)
              selectedRow = preferredRow;
          }
        }
      }
    }
    wait(1);
  }

  Window_Close(messageWindow, 1);
  Window_Close(window, 1);
  wait(1);
  for (i = 0; i < 5; i++)
    VramCache_FreeEntry(iconSlots[i]);
  VramCache_FreeEntry(cursorHandle);
  wait(1);
  return result;
}

#undef NAVIGATION
#undef CURSOR_TEXT_LENGTH
#undef CURSOR_TEXT_COPY
#undef LIST_WINDOW_X
#undef LIST_WINDOW_WIDTH
#undef COST_COLUMN
#undef COST_X
#undef RANGE_COLUMN
#undef MENU_SPRITE_INITIALIZE
#undef MENU_GET_CURSOR
#undef MENU_SET_CURSOR
#undef WINDOW_MARK_TILEMAP_ROW
