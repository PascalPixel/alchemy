/// @file
/// The battle item menu.
// Part of screens.c.

#include "window_internal.h"
#include "sounds.h"
#include "game/item.h"
#include "field/field_sequence.h"
#include "field/actor.h"
#include "battle/battle_system.h"
#include "types.h"
#include "system/scene.h"
#include "edition.h"
#include "graphics/uiwindow.h"
#include "battle_effect/battle_effect.h"
#include "field/object.h"
#include "shop/shop.h"
#include "window/screens.h"

/// Says whether party member `unit` can use inventory entry `itemId` in battle:
/// 0 yes; 1 no, "Using this will have no effect." (no item, an item that only
/// teaches Psynergy while equipped, one with no action, or equipment the party
/// member cannot equip); 2 not in battle, "Cannot be used during battle." (its
/// action lacks ABILITY_USABLE_IN_BATTLE).
s32 Item_CheckBattleUse(s32 unit, s32 itemId) {
  struct ItemDefinition *item;

  if (itemId == 0) {
    return 1;
  }
  item = Item_GetDirectFar(itemId);
  if (item->useType == ITEM_USE_TEACHES) {
    return 1;
  }
  if (item->abilityId == 0) {
    return 1;
  }
  if ((item->type != ITEM_TYPE_NONE) &&
      (Item_CanUnitEquip(unit, itemId) == 0)) {
    return 1;
  }
  if ((ABILITY_USABLE_IN_BATTLE & Ability_Get(item->abilityId)->flags) == 0) {
    return 2;
  }
  return 0;
}

/// Copies one 4bpp tile of BG character block 0 to another, each given as a
/// tilemap entry whose palette and flip bits are ignored.
void Vram_CopyTile(u32 sourceTile, u32 destinationTile) {
  Dma_Set(BG_CHAR_BLOCK(0) + (sourceTile & TILE_NUMBER_MASK) * TILE_SIZE_4BPP,
          BG_CHAR_BLOCK(0) +
              (destinationTile & TILE_NUMBER_MASK) * TILE_SIZE_4BPP,
          DMA_COPY_WORDS(8), REG_DMA3);
}

/// How much of a refusal is copied: more in German, Spanish, French and
/// Italian.
#if (LANGUAGE == LANGUAGE_ENGLISH) || (LANGUAGE == LANGUAGE_JAPANESE)
#define REFUSAL_TEXT_LENGTH 32
#else
#define REFUSAL_TEXT_LENGTH 52
#endif

/// The battle item list for unit `member`: `count` inventory entries (ended by
/// 0), five to a page, each with its icon and name: plain when usable,
/// TEXT_COLOR_YELLOW when Item_CheckBattleUse refuses it, TEXT_COLOR_RED
/// when broken; the window above names the one under the cursor
/// (MsgItemDescription), or says it cannot be used in battle.
s32 BattleItemMenu_Run(s32 member, u16 *entries, s32 count) {
  struct WindowState *state;
  s32 drawnFirstEntry;
  s32 drawnRow;
  s32 cursorHandle;
  struct Window *messageWindow;
  s32 iconCount;
  s32 preferredRow;
  struct {
    s32 x, y;
  } cursorPosition;
  struct BattleMenuState *navigation;
  struct Window *window;
  s32 firstEntry;
  s32 selectedRow;
  s32 item;
  s32 iconTile;
  s32 unusableReason;
  s32 result;
  s32 i;
  struct MenuSprite cursorStorage;
  struct MenuSprite *iconSprite;
  u16 messageText[64];
  s32 iconSlots[BATTLE_LIST_ROWS];
  struct MenuSprite sprites[BATTLE_LIST_ROWS];

  state = Ram_HeapSlots->windowState;
  drawnFirstEntry = -1;
  drawnRow = -1;
  cursorHandle = VramCache_AllocEntry(128);
  messageWindow = Window_Create(0, 5, 30, 4,
                                WINDOW_FLAG_INSTANT | WINDOW_FLAG_TEXT_CANVAS |
                                    WINDOW_FLAG_BORDERED_CANVAS);
  iconCount = 0;
  BATTLE_MENU_GET_CURSOR(firstEntry, selectedRow, preferredRow);
  window = Window_Create(15, 9, 15, BATTLE_LIST_ROWS * 2 + 1,
                         WINDOW_FLAG_INSTANT | WINDOW_FLAG_UNREAD);

  for (i = 0; i < BATTLE_LIST_ROWS; i++) {
    s32 y;
    /* FAKEMATCH(temporary): windowY holds 0 for the icon's attr23 word before
     * it holds the window's y; as 0 and window->y at their uses. */
    s32 windowY = 0;

    y = i * 2;
    /* FAKEMATCH(dead): an empty once-only block after the row. */
    do {
    } while (0);
    iconSprite = &sprites[i];
    sprites[i].oam.word.attr01 = OBJ_SIZE_16X16;
    sprites[i].oam.word.attr23 = windowY;
    iconSprite->oam.f.x = window->x * 8 + 8;
    windowY = window->y;
    iconSprite->oam.f.y = (y + windowY) * 8 + 4;
  }
  // One icon slot a visible row; paging reloads these same slots.
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
    iconSprite = &cursorStorage;

    if (firstEntry != drawnFirstEntry || selectedRow != drawnRow) {
      state->holdTilemapUpload = 1;
      Window_HighlightRect(window->x + 1, window->y + drawnRow * 2 + 1,
                           window->width - 2, 1, WINDOW_PALETTE);
      Text_FillCanvasBackground();
      if (count != 0) {
        if (Item_CheckBattleUse(member, entries[firstEntry + selectedRow]) == 2)
          BATTLE_MENU_MESSAGE_COPY((s32)MsgBattleItemUnavailable, messageText,
                                   BATTLE_MENU_MESSAGE_CAPACITY);
        else
          BATTLE_MENU_MESSAGE_COPY(
              (entries[firstEntry + selectedRow] & INVENTORY_ITEM_ID_MASK) +
                  (s32)MsgItemDescription,
              messageText, BATTLE_MENU_MESSAGE_CAPACITY);
      } else {
        BATTLE_MENU_MESSAGE_COPY((s32)MsgBattleNoItems, messageText,
                                 BATTLE_MENU_MESSAGE_CAPACITY);
      }
      Text_DrawWideStringAtOffset(messageText, messageWindow, 0, 4);
      drawnRow = selectedRow;
      // Only a page change redraws the rows and their icons.
      if (firstEntry != drawnFirstEntry) {
        WindowSprite_RedrawSavedRect(window);
        for (i = 0; i < BATTLE_LIST_ROWS; i++) {
          item = entries[firstEntry + i];
          if (item == 0)
            break;
          Item_GetDirectFar(item);
          Text_SetColor(TEXT_COLOR_WHITE);
          if (Item_CheckBattleUse(member, item) != 0)
            Text_SetColor(TEXT_COLOR_YELLOW);
          else if (item & INVENTORY_BROKEN)
            Text_SetColor(TEXT_COLOR_RED);
          Text_DrawMessageAsTiles((item & INVENTORY_ITEM_ID_MASK) +
                                      (s32)MsgItemName,
                                  window, 16, i * 16);
          Text_SetColor(TEXT_COLOR_WHITE);
          sprites[i].oam.f.tile = ItemIcon_LoadIntoSlot(item, iconSlots[i]);
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
      Window_HighlightRect(window->x + 1, window->y + selectedRow * 2 + 1,
                           window->width - 2, 1, HIGHLIGHT_PALETTE);
      state->dirty = 1;
      state->holdTilemapUpload = 0;
    }

    // Each frame: the current page's number lit 12 frames in 16, and the page
    // arrows on the window's top edge (y -1).
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
      Window_SetTilemapEntry(
          window, LIST_TILE_PAGES_LEFT,
          window->width - (count + BATTLE_LIST_ROWS - 1) / BATTLE_LIST_ROWS - 3,
          -1, 0);
      Window_SetTilemapEntry(window, LIST_TILE_PAGES_RIGHT, window->width - 2,
                             -1, 0);
      WINDOW_MARK_TILEMAP_ROW(state, window->y - 1);
    }

    for (i = 0; i < iconCount; i++)
      Oam_AddSpriteAtDepth(&sprites[i], OAM_DEPTH_WINDOW);

    MENU_PLACE_CURSOR(cursorPosition, window, selectedRow, 2, iconSprite);
    iconSprite->oam.f.tile = VramCache_Reload(cursorHandle, gPointingHandTiles);
    iconSprite->oam.f.x = cursorPosition.x + ((gFrameCount & 4) >> 1) - 4;
    iconSprite->oam.f.y = cursorPosition.y - ((gFrameCount & 4) >> 2) - 8;
    if (count != 0)
      Oam_AddSpriteAtDepth(&cursorStorage, OAM_DEPTH_WINDOW + 2);

    BATTLE_MENU_SET_CURSOR(firstEntry, selectedRow, preferredRow);
    navigation = BATTLE_MENU_STATE;
    // A takes a usable item; a broken or refused one plays the error sound and
    // shows why.
    if (gInput.pressed & KEY_A) {
      if (count != 0) {
        result = firstEntry + selectedRow;
        Item_GetDirectFar(entries[result]);
        unusableReason = 0;
        if ((entries[result] & INVENTORY_BROKEN) == 0) {
          unusableReason = Item_CheckBattleUse(member, entries[result]);
          if (unusableReason == 0)
            break;
        }
        Sound_PlayFar(SE_MENU_ERROR);
        if (unusableReason == 2)
          BATTLE_MENU_MESSAGE_COPY((s32)MsgBattleItemUnavailable, messageText,
                                   REFUSAL_TEXT_LENGTH);
        else if (entries[result] & INVENTORY_BROKEN)
          BATTLE_MENU_MESSAGE_COPY((s32)MsgBattleItemBroken, messageText,
                                   REFUSAL_TEXT_LENGTH);
        else
          BATTLE_MENU_MESSAGE_COPY((s32)MsgBattleItemNoEffect, messageText,
                                   REFUSAL_TEXT_LENGTH);
        Text_FillCanvasBackground();
        Text_DrawWideStringAtOffset(messageText, messageWindow, 0, 4);
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
  VramCache_FreeEntry(cursorHandle);
  for (i = 0; i < BATTLE_LIST_ROWS; i++)
    VramCache_FreeEntry(iconSlots[i]);
  wait(1);
  return result;
}

#undef REFUSAL_TEXT_LENGTH
