/// @file
/// The battle Psynergy menu.
// Part of screens.c.

#include "window_internal.h"
#include "sounds.h"
#include "window/window_tile.h"
#include "window/screens.h"

/// The list is a tile wider in Spanish and French, and its columns move with
/// it.
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
  struct WindowWork *work;
  s32 drawnFirstEntry;
  s32 firstEntry;
  s32 drawnRow;
  s32 cursorHandle;
  struct Unit *unit;
  struct Window *messageWindow;
  s32 iconCount;
  s32 preferredRow;
  struct {
    s32 x, y;
  } cursorPosition;
  struct BattleMenuWork *navigation;
  struct Window *window;
  s32 selectedRow;
  s32 encodedAction;
  s32 iconTile;
  s32 pageTile;
  s32 rangePattern;
  s32 result;
  s32 i;
  struct MenuSprite cursor;
  struct MenuSprite *iconSprite;
  u16 messageText[64];
  s32 iconSlots[BATTLE_LIST_ROWS];
  struct MenuSprite sprites[BATTLE_LIST_ROWS];
  s32 glyph;

  work = (struct WindowWork *)Ram_HeapSlots->windowWork;
  drawnRow = -1;
  drawnFirstEntry = -1;
  cursorHandle = VramCache_AllocEntry(128);
  unit = Unit_Get(unitId);
  messageWindow = Window_Create(0, 5, 30, 4,
                                WINDOW_FLAG_INSTANT | WINDOW_FLAG_TEXT_CANVAS |
                                    WINDOW_FLAG_BORDERED_CANVAS);
  iconCount = BATTLE_LIST_ROWS;
  BATTLE_MENU_GET_CURSOR(firstEntry, selectedRow, preferredRow);
  window = Window_Create(LIST_WINDOW_X, 9, LIST_WINDOW_WIDTH,
                         BATTLE_LIST_ROWS * 2 + 1, 6);

  for (i = 0; i < BATTLE_LIST_ROWS; i++) {
    s32 y;
    u32 t = 0;

    y = i * 2;
    iconSprite = &sprites[i];
    sprites[i].oam.word.attr01 = OBJ_SIZE_16X16;
    Io_Write32(t, &iconSprite->oam.word.attr23);
    iconSprite->oam.f.x = (s16)window->x * 8 + 8;
    t = (s16)window->y;
    iconSprite->oam.f.y = (y + t) * 8 + 4;
  }
  // Reserve one glyph slot per visible row; paging reloads these same slots.
  for (i = 0; i < BATTLE_LIST_ROWS; i++) {
    iconSlots[i] = VramCache_AllocEntry(128);
    sprites[i].oam.f.tile = VramCache_Reload(iconSlots[i], (const void *)-1);
  }
  // The border's tees go into tiles 0, 1, 16 and 17 of the second text range,
  // which nothing shows here. In battle BG1 takes 8bpp tiles from character
  // block 0 (BATTLE_BG1CNT), so these are its tiles COMMAND_ROW_ICON_TILE and
  // COMMAND_ROW_ICON_TILE + 8, the first command icon's top-left and
  // bottom-left; BattleCommandRow_Run turns BG1 off as it returns, before a
  // list opens, and draws its icons there again when it opens next.
  Vram_CopyTile(WINDOW_TILE_BORDER_TOP_TEE, TEXT_EXTRA_TILE_FIRST);
  Vram_CopyTile(WINDOW_TILE_BORDER_TOP_TEE, TEXT_EXTRA_TILE_FIRST + 1);
  Vram_CopyTile(WINDOW_TILE_BORDER_BOTTOM_TEE, TEXT_EXTRA_TILE_FIRST + 16);
  Vram_CopyTile(WINDOW_TILE_BORDER_BOTTOM_TEE, TEXT_EXTRA_TILE_FIRST + 17);

  for (;;) {
    iconSprite = &cursor;
    if (firstEntry != drawnFirstEntry || selectedRow != drawnRow) {
      work->holdTilemapUpload = 1;
      Window_HighlightRect((s16)window->x + 1,
                           (s16)window->y + drawnRow * 2 + 1, window->width - 2,
                           1, WINDOW_PALETTE);
      Text_FillCanvasBackground();
      if (count != 0)
        BATTLE_MENU_MESSAGE_COPY(
            (actions[firstEntry + selectedRow] & ACTION_ID_MASK) +
                (s32)MsgAbilityDescription,
            messageText, BATTLE_MENU_MESSAGE_CAPACITY);
      else
        BATTLE_MENU_MESSAGE_COPY((s32)MsgBattleNoPsynergy, messageText,
                                 BATTLE_MENU_MESSAGE_CAPACITY);
      Text_DrawWideStringAtOffset(messageText, messageWindow, 0, 4);
      drawnRow = selectedRow;
      // Cursor moves update help; only a page change reloads the row icons.
      if (firstEntry != drawnFirstEntry) {
        WindowSprite_RedrawSavedRect(window);
        for (i = 0; i < BATTLE_LIST_ROWS; i++) {
          struct AbilityDefinition *action;

          encodedAction = actions[firstEntry + i];
          if (encodedAction == 0)
            break;
          action = Ability_Get(encodedAction);
          Window_SetTilemapEntry(window, WINDOW_TILE_PP_LABEL, COST_COLUMN,
                                 i * 2, 0);
          Window_SetTilemapEntry(window, WINDOW_TILE_PP_LABEL_END,
                                 COST_COLUMN + 1, i * 2, 0);
          Ability_LoadGlyph(encodedAction & PSYNERGY_ABILITY_MASK, 0,
                            &iconSlots[i], &glyph, 1);
          sprites[i].oam.f.tile = glyph;
          if ((action->flags & ABILITY_USABLE_IN_BATTLE) == 0)
            Text_SetColor(TEXT_COLOR_YELLOW);
          else if (action->ppCost > unit->pp)
            Text_SetColor(TEXT_COLOR_RED);
          else if (unit->psySeal != 0)
            Text_SetColor(TEXT_COLOR_PINK);
          work->textPalette = BASIC_COLORS_PALETTE;
          Text_DrawMessageAsTiles(encodedAction + (s32)MsgAbilityName, window,
                                  16, i * 16);
          Text_DrawNumberAtOffset(action->ppCost, 2, window, COST_X, i * 16);
          Text_SetColor(TEXT_COLOR_WHITE);
          work->textPalette = WINDOW_PALETTE;
          if (action->element != ELEMENT_NONE)
            Window_SetTilemapEntry(window, action->element + ELEMENT_ICON_TILE,
                                   RANGE_COLUMN, i * 2, 0);
          rangePattern = action->range;
          if (rangePattern == ABILITY_RANGE_ALL)
            rangePattern = RANGE_ICON_ALL;
          else
            rangePattern--;
          Window_DrawThreeTileColumn(window, RANGE_COLUMN + 1, i * 2,
                                     rangePattern, 0);
        }
        iconCount = i;
        drawnFirstEntry = firstEntry;
      }
      if (count > BATTLE_LIST_ROWS) {
        for (i = 0; i < (count + BATTLE_LIST_ROWS - 1) / BATTLE_LIST_ROWS;
             i++) {
          iconTile = i + PAGE_NUMBER_TILE;
          if (i == firstEntry / BATTLE_LIST_ROWS)
            iconTile = i + PAGE_NUMBER_LIT_TILE;
          Window_SetTilemapEntry(
              window, iconTile,
              window->width -
                  (count + BATTLE_LIST_ROWS - 1) / BATTLE_LIST_ROWS + i - 2,
              -1, 0);
        }
      }
      Window_HighlightRect((s16)window->x + 1,
                           (s16)window->y + selectedRow * 2 + 1,
                           window->width - 2, 1, HIGHLIGHT_PALETTE);
      work->dirty = 1;
      work->holdTilemapUpload = 0;
    }

    // Each frame: the current page's number lit 12 frames in 16, and the page
    // arrows on the window's top edge.
    if (count > BATTLE_LIST_ROWS) {
      for (i = 0; i < (count + BATTLE_LIST_ROWS - 1) / BATTLE_LIST_ROWS; i++) {
        iconTile = i + PAGE_NUMBER_TILE;
        if ((gFrameCount & 15) <= 11) {
          if (i == firstEntry / BATTLE_LIST_ROWS)
            iconTile = i + PAGE_NUMBER_LIT_TILE;
        }
        Window_SetTilemapEntry(
            window, iconTile,
            window->width - (count + BATTLE_LIST_ROWS - 1) / BATTLE_LIST_ROWS +
                i - 2,
            -1, 0);
      }
      pageTile = -1;
      Window_SetTilemapEntry(
          window, LIST_TILE_PAGES_LEFT,
          window->width - (count + BATTLE_LIST_ROWS - 1) / BATTLE_LIST_ROWS - 3,
          pageTile, 0);
      Window_SetTilemapEntry(window, LIST_TILE_PAGES_RIGHT, window->width - 2,
                             pageTile, 0);
      WINDOW_MARK_TILEMAP_ROW(work, (u32)((s16)window->y - 1));
    }

    for (i = 0; i < iconCount; i++)
      Oam_AddSpriteAtDepth(&sprites[i], OAM_DEPTH_WINDOW);

    MENU_PLACE_CURSOR(cursorPosition, window, selectedRow, 4, iconSprite);
    iconSprite->oam.f.tile = VramCache_Reload(cursorHandle, gPointingHandTiles);
    iconSprite->oam.f.x = cursorPosition.x + ((gFrameCount & 4) >> 1) - 4;
    iconSprite->oam.f.y = cursorPosition.y - ((gFrameCount & 4) >> 2) - 8;
    if (count != 0)
      Oam_AddSpriteAtDepth(iconSprite, OAM_DEPTH_WINDOW + 2);

    BATTLE_MENU_SET_CURSOR(firstEntry, selectedRow, preferredRow);
    navigation = BATTLE_MENU_WORK;
    if (gInput.pressed & KEY_A) {
      if (count != 0) {
        result = firstEntry + selectedRow;
        if ((Ability_Get(actions[result])->flags & ABILITY_USABLE_IN_BATTLE) ==
            0) {
          // Not usable in battle: the menu stays open.
        } else {
          break;
        }
      } else {
        result = -1;
        break;
      }
    } else if (navigation->timer == 0 || (gInput.pressed & KEY_B)) {
      Sound_PlayFar(SE_MENU_CANCEL);
      result = -1;
      break;
    }

    if (count != 0) {
      if (gInput.repeat & KEY_DOWN) {
        Sound_PlayFar(SE_MENU_CURSOR_MOVE);
        selectedRow++;
        if (selectedRow == BATTLE_LIST_ROWS ||
            firstEntry + selectedRow == count)
          selectedRow = 0;
        preferredRow = selectedRow;
      } else if (gInput.repeat & KEY_UP) {
        Sound_PlayFar(SE_MENU_CURSOR_MOVE);
        selectedRow--;
        if (selectedRow < 0) {
          if (firstEntry == (count - 1) / BATTLE_LIST_ROWS * BATTLE_LIST_ROWS)
            selectedRow = count - firstEntry - 1;
          else
            selectedRow = BATTLE_LIST_ROWS - 1;
        }
        preferredRow = selectedRow;
      } else if (gInput.repeat & KEY_RIGHT) {
        Sound_PlayFar(SE_MENU_CURSOR_MOVE);
        Input_ResetKeyRepeat();
        if (firstEntry + BATTLE_LIST_ROWS >= count) {
          if (firstEntry != 0) {
            firstEntry = 0;
            selectedRow = preferredRow;
          }
        } else {
          firstEntry += BATTLE_LIST_ROWS;
          selectedRow = preferredRow;
          if (firstEntry == (count - 1) / BATTLE_LIST_ROWS * BATTLE_LIST_ROWS) {
            selectedRow = count - firstEntry - 1;
            if (selectedRow > preferredRow)
              selectedRow = preferredRow;
          }
        }
      } else if (gInput.repeat & KEY_LEFT) {
        Sound_PlayFar(SE_MENU_CURSOR_MOVE);
        Input_ResetKeyRepeat();
        if (firstEntry != 0) {
          firstEntry -= BATTLE_LIST_ROWS;
          selectedRow = preferredRow;
        } else {
          firstEntry = (count - 1) / BATTLE_LIST_ROWS * BATTLE_LIST_ROWS;
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
  for (i = 0; i < BATTLE_LIST_ROWS; i++)
    VramCache_FreeEntry(iconSlots[i]);
  VramCache_FreeEntry(cursorHandle);
  wait(1);
  return result;
}

#undef LIST_WINDOW_X
#undef LIST_WINDOW_WIDTH
#undef COST_COLUMN
#undef COST_X
#undef RANGE_COLUMN
