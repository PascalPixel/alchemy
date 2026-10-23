/// @file
/// The battle item menu.
// Part of battle_command.c.

#include "../battle_command_internal.h"

/// Says whether member `unit` can use inventory entry `item` in battle: 0
/// yes; 1 no, "Using this will have no effect." (no item, an item that only
/// teaches Psynergy while equipped, one with no action, or equipment the
/// member cannot equip); 2 not in battle, "Cannot be used during battle."
/// (its action lacks BATTLE_OK). A broken item (INVENTORY_BROKEN) is not
/// tested here. The item command lists the usable items first by it.
s32 Item_ClassifyUseAbility(s32 unit, s32 item) {
  s32 itemUseStatus;
  struct ItemDefinition *definition;

  if (item == 0) {
    return 1;
  }
  definition = Item_GetDirectFar(item);
  itemUseStatus = 1;
  // Equipment that grants Psynergy.
  if (definition->useType == 3) {
    return itemUseStatus;
  }
  if (definition->abilityId == 0) {
    return itemUseStatus;
  }
  if ((definition->type != 0) && (Item_CanUnitEquip(unit, item) == 0)) {
    return itemUseStatus;
  }
  if ((BATTLE_OK & Ability_Get(definition->abilityId)->flags) == 0) {
    return 2;
  }
  return 0;
}

/// Copies one 4bpp tile of BG character block 0 to another, each given as a
/// tilemap entry whose palette and flip bits are ignored. The battle lists
/// copy the frame's top and bottom tees to tiles 0x200, 0x201, 0x210 and
/// 0x211 as they open. XXX: what shows those copies is not found; tiles
/// 0x200 to 0x27f are also the second text-tile range
/// (UiWork_SetAltFlagAndClearTable), and a tilemap entry naming one of the
/// four would settle it.
void Vram_CopyTile(u32 sourceTile, u32 destinationTile) {
  Dma_Set(BG_CHAR_BLOCK(0) + (sourceTile & 0x3ff) * TILE_SIZE_4BPP,
          BG_CHAR_BLOCK(0) + (destinationTile & 0x3ff) * TILE_SIZE_4BPP,
          DMA_COPY32(8), REG_DMA3);
}

/// The battle item list for unit `owner`: `count` inventory entries (ended by
/// 0), five to a page, each with its icon and name: plain when usable,
/// colour 4 when Item_ClassifyUseAbility refuses it, colour 2 when broken;
/// the window above names the one under the cursor (MsgItemPlainName), or
/// says it cannot be used in battle. Returns the chosen entry's
/// place in `entries`, or -1 when cancelled, when the link battle's time
/// runs out, or when there are no items. A refused item stays on the list
/// with its reason ("It's broken.", and the classifier's messages). The
/// item command runs it (Battle_CollectPartyCommands).
s32 ItemList_SelectEntry(s32 owner, u16 *entries, s32 count) {
  struct UiRenderWork *work;
  s32 drawnFirstEntry;
  s32 drawnRow;
  s32 cursorHandle;
  struct UiWindow *messageWindow;
  s32 iconCount;
  s32 preferredRow;
  struct MenuPoint cursorPosition;
  struct BattleMenuWork *navigation;
  struct UiWindow *window;
  s32 firstEntry;
  s32 selectedRow;
  s32 item;
  s32 iconTile;
  s32 pageTile;
  s32 unusableReason;
  s32 result;
  s32 i;
  struct CursorSprite cursorStorage;
  struct MenuSprite *iconSprite;
  struct CursorSprite *sprite;
  u16 messageText[64];
  s32 iconSlots[BATTLE_LIST_ROWS];
  struct MenuSprite sprites[BATTLE_LIST_ROWS];

  work = UI_WORK;
  drawnFirstEntry = -1;
  drawnRow = -1;
  cursorHandle = VramCache_AllocEntry(128);
  messageWindow = UiWindow_Create(0, 5, 30, 4, 42);
  iconCount = 0;
  BATTLE_MENU_GET_CURSOR(firstEntry, selectedRow, preferredRow);
  window = UiWindow_Create(15, 9, 15, BATTLE_LIST_ROWS * 2 + 1, 6);

  for (i = 0; i < BATTLE_LIST_ROWS; i++) {
    s32 y = i * 2;

    iconSprite = &sprites[i];
    MenuSprite_Initialize(sprites[i], MENU_SPRITE_SIZE_16);
    iconSprite->oam.f.x = window->x * 8 + 8;
    iconSprite->oam.f.y = (y + window->y) * 8 + 4;
  }

  // Reserve one glyph slot per visible row; paging reloads these same slots.
  for (i = 0; i < BATTLE_LIST_ROWS; i++) {
    iconSlots[i] = VramCache_AllocEntry(128);
    sprites[i].oam.f.tile = VramCache_Reload(iconSlots[i], -1);
  }

  Vram_CopyTile(UI_TILE_FRAME_TOP_TEE, 0x200);
  Vram_CopyTile(UI_TILE_FRAME_TOP_TEE, 0x201);
  Vram_CopyTile(UI_TILE_FRAME_BOTTOM_TEE, 0x210);
  Vram_CopyTile(UI_TILE_FRAME_BOTTOM_TEE, 0x211);

  sprite = &cursorStorage;
  for (;;) {
    if (firstEntry != drawnFirstEntry || selectedRow != drawnRow) {
      work->menuBusy = 1;
      Ui_SetRectHighlight(window->x + 1, window->y + drawnRow * 2 + 1,
                          window->width - 2, 1, 15);
      Ui_FillVramBlockPattern();
      if (count != 0) {
        if (Item_ClassifyUseAbility(owner, entries[firstEntry + selectedRow]) ==
            2) {
          BATTLE_MENU_MESSAGE_COPY(MsgBattleItemUnavailable, messageText,
                                   BATTLE_MENU_MESSAGE_CAPACITY);
        } else {
          BATTLE_MENU_MESSAGE_COPY(
              (entries[firstEntry + selectedRow] & INVENTORY_ITEM_ID_MASK) +
                  MsgItemPlainName,
              messageText, BATTLE_MENU_MESSAGE_CAPACITY);
        }
      } else {
        BATTLE_MENU_MESSAGE_COPY(MsgBattleNoItems, messageText,
                                 BATTLE_MENU_MESSAGE_CAPACITY);
      }
      UiText_RenderWideStringAtOffset(messageText, messageWindow, 0, 4);
      drawnRow = selectedRow;

      // Cursor moves update help; only a page change reloads the row icons.
      if (firstEntry != drawnFirstEntry) {
        RenderOutput_RedrawSavedRect(window);
        for (i = 0; i < BATTLE_LIST_ROWS; i++) {
          item = entries[firstEntry + i];
          if (item == 0) {
            break;
          }
          Item_GetDirectFar(item);
          UiWork_SetParameterNibble(15);
          if (Item_ClassifyUseAbility(owner, item) != 0) {
            UiWork_SetParameterNibble(4);
          } else if ((item & INVENTORY_BROKEN) != 0) {
            UiWork_SetParameterNibble(2);
          }
          UiText_DrawCharacterAtOffset((item & INVENTORY_ITEM_ID_MASK) +
                                           MsgItemName,
                                       window, 16, i * 16);
          UiWork_SetParameterNibble(15);
          sprites[i].oam.f.tile = ItemIcon_LoadIntoSlot(item, iconSlots[i]);
        }
        iconCount = i;
        drawnFirstEntry = firstEntry;
      }

      if (count > BATTLE_LIST_ROWS) {
        for (i = 0; i < (count + 4) / BATTLE_LIST_ROWS; i++) {
          iconTile = i + LIST_TILE_PAGE_NUMBER;
          if (i == firstEntry / BATTLE_LIST_ROWS) {
            iconTile = i + LIST_TILE_PAGE_NUMBER_LIT;
          }
          UiWindow_SetTilemapEntry(
              window, iconTile,
              window->width - (count + 4) / BATTLE_LIST_ROWS + i - 2, -1, 0);
        }
      }

      Ui_SetRectHighlight(window->x + 1, window->y + selectedRow * 2 + 1,
                          window->width - 2, 1, 14);
      work->dirty = 1;
      work->menuBusy = 0;
    }

    // Each frame: the current page's number lit 12 frames in 16, and the
    // page arrows on the window's top edge (y -1).
    if (count > BATTLE_LIST_ROWS) {
      for (i = 0; i < (count + 4) / BATTLE_LIST_ROWS; i++) {
        iconTile = i + LIST_TILE_PAGE_NUMBER;
        if ((gFrameCount & 15) <= 11) {
          if (i == firstEntry / BATTLE_LIST_ROWS) {
            iconTile = i + LIST_TILE_PAGE_NUMBER_LIT;
          }
        }
        UiWindow_SetTilemapEntry(
            window, iconTile,
            window->width - (count + 4) / BATTLE_LIST_ROWS + i - 2, -1, 0);
      }
      pageTile = -1;
      UiWindow_SetTilemapEntry(
          window, LIST_TILE_PAGES_LEFT,
          window->width - (count + 4) / BATTLE_LIST_ROWS - 3, pageTile, 0);
      UiWindow_SetTilemapEntry(window, LIST_TILE_PAGES_RIGHT, window->width - 2,
                               pageTile, 0);
      UiRender_MarkRow(work, window->y - 1);
    }

    for (i = 0; i < iconCount; i++) {
      iconSprite = &sprites[i];
      Runtime_PushSlotEntry(iconSprite, 240);
    }

    cursorPosition.x = window->x * 8 - 2;
    cursorPosition.y = (selectedRow * 2 + window->y) * 8 + 20;
    /* FAKEMATCH(dead): sprite set again to the cursor each frame orders
     * the spill slots of the cursor address and row * 2 as the native frame
     * does; without it, 28 of 770 instructions differ in tbs-en. */
    sprite = &cursorStorage;
    ((u32 *)sprite)[1] = MENU_SPRITE_SIZE_16;
    ((u32 *)sprite)[2] = 0;
    sprite->tile =
        VramCache_Reload(cursorHandle, (s32)gResource_FixedBlockBTiles);
    sprite->x = cursorPosition.x + ((gFrameCount & 4) >> 1) - 4;
    sprite->y = cursorPosition.y - ((gFrameCount & 4) >> 2) - 8;
    if (count != 0) {
      Runtime_PushSlotEntry(sprite, 242);
    }

    BATTLE_MENU_SET_CURSOR(firstEntry, selectedRow, preferredRow);
    navigation = gBattleMenuWork;

    // A takes a usable item; a broken or refused one plays the error sound
    // and shows why.
    if ((gKeysPressed & KEY_A) != 0) {
      if (count != 0) {
        result = firstEntry + selectedRow;
        Item_GetDirectFar(entries[result]);
        unusableReason = 0;
        if ((entries[result] & INVENTORY_BROKEN) == 0) {
          unusableReason = Item_ClassifyUseAbility(owner, entries[result]);
          if (unusableReason == 0) {
            break;
          }
        }
        Sound_PlaySongFar(SE_MENU_ERROR);
        if (unusableReason == 2) {
          BATTLE_MENU_MESSAGE_COPY(MsgBattleItemUnavailable, messageText,
                                   BATTLE_MENU_MESSAGE_CAPACITY);
        } else if ((entries[result] & INVENTORY_BROKEN) != 0) {
          BATTLE_MENU_MESSAGE_COPY(MsgBattleItemBroken, messageText,
                                   BATTLE_MENU_MESSAGE_CAPACITY);
        } else {
          BATTLE_MENU_MESSAGE_COPY(MsgBattleItemNoEffect, messageText,
                                   BATTLE_MENU_MESSAGE_CAPACITY);
        }
        Ui_FillVramBlockPattern();
        UiText_RenderWideStringAtOffset(messageText, messageWindow, 0, 4);
      } else {
        result = -1;
        break;
      }
    } else if (navigation->timer == 0 || (gKeysPressed & KEY_B) != 0) {
      Sound_PlaySongFar(SE_MENU_CANCEL);
      result = -1;
      break;
    }

    if (count != 0) {
      if ((gKeysRepeat & KEY_DOWN) != 0) {
        Sound_PlaySongFar(SE_MENU_CURSOR_MOVE);
        selectedRow++;
        if (selectedRow == BATTLE_LIST_ROWS ||
            firstEntry + selectedRow == count) {
          selectedRow = 0;
        }
        preferredRow = selectedRow;
      } else if ((gKeysRepeat & KEY_UP) != 0) {
        Sound_PlaySongFar(SE_MENU_CURSOR_MOVE);
        selectedRow--;
        if (selectedRow < 0) {
          if (firstEntry == (count - 1) / BATTLE_LIST_ROWS * BATTLE_LIST_ROWS) {
            selectedRow = count - firstEntry - 1;
          } else {
            selectedRow = BATTLE_LIST_ROWS - 1;
          }
        }
        preferredRow = selectedRow;
      } else if ((gKeysRepeat & KEY_RIGHT) != 0) {
        Sound_PlaySongFar(SE_MENU_CURSOR_MOVE);
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
            if (selectedRow > preferredRow) {
              selectedRow = preferredRow;
            }
          }
        }
      } else if ((gKeysRepeat & KEY_LEFT) != 0) {
        Sound_PlaySongFar(SE_MENU_CURSOR_MOVE);
        Input_ResetKeyRepeat();
        if (firstEntry != 0) {
          firstEntry -= BATTLE_LIST_ROWS;
          selectedRow = preferredRow;
        } else {
          firstEntry = (count - 1) / BATTLE_LIST_ROWS * BATTLE_LIST_ROWS;
          selectedRow = preferredRow;
          if (firstEntry != 0) {
            selectedRow = count - firstEntry - 1;
            if (selectedRow > preferredRow) {
              selectedRow = preferredRow;
            }
          }
        }
      }
    }
    WaitFrames(1);
  }

  UiWork_Finalize(messageWindow, 1);
  UiWork_Finalize(window, 1);
  WaitFrames(1);
  Resource_ResetEntry(cursorHandle);
  for (i = 0; i < BATTLE_LIST_ROWS; i++) {
    Resource_ResetEntry(iconSlots[i]);
  }
  WaitFrames(1);
  return result;
}
