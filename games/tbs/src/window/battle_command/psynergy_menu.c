/// @file
/// The battle Psynergy menu.
// Part of battle_command.c.

#include "../battle_command_internal.h"

/// The battle Psynergy list for unit `unitId`: `count` abilities of `actions`
/// (ended by 0), five to a page, each with its icon, name, PP cost, element
/// and range icon, in colour 4 when not usable in battle, colour 2 when the
/// unit's PP fall short and colour 9 while its Psynergy is sealed; the
/// window above describes the one under the cursor. Returns the chosen
/// ability's place in `actions`, or -1 when cancelled, when the link
/// battle's time runs out or when the list is empty. Only an ability not
/// usable in battle is refused here, silently: one the PP cannot pay for, or
/// a sealed member's, can be chosen. The Psynergy command runs it
/// (Battle_CollectPartyCommands), listing only abilities usable in battle.
s32 BattleMenu_RunActionSelection(s32 unitId, u16 *actions, s32 count) {
  struct UiRenderWork *work;
  s32 drawnFirstEntry;
  s32 firstEntry;
  s32 drawnRow;
  s32 cursorHandle;
  struct BattleUnit *unit;
  struct UiWindow *messageWindow;
  s32 iconCount;
  s32 preferredRow;
  struct MenuPoint cursorPosition;
  struct BattleMenuWork *navigation;
  struct UiWindow *window;
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

  work = UI_WORK;
  drawnRow = -1;
  drawnFirstEntry = -1;
  cursorHandle = VramCache_AllocEntry(128);
  unit = Unit_GetFar(unitId);
  messageWindow = UiWindow_Create(0, 5, 30, 4, 42);
  iconCount = BATTLE_LIST_ROWS;
  BATTLE_MENU_GET_CURSOR(firstEntry, selectedRow, preferredRow);
  window = UiWindow_Create(9, 9, 21, BATTLE_LIST_ROWS * 2 + 1, 6);

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

  iconSprite = &cursor;
  for (;;) {
    if (firstEntry != drawnFirstEntry || selectedRow != drawnRow) {
      work->menuBusy = 1;
      Ui_SetRectHighlight(window->x + 1, window->y + drawnRow * 2 + 1,
                          window->width - 2, 1, 15);
      Ui_FillVramBlockPattern();
      if (count != 0) {
        BATTLE_MENU_MESSAGE_COPY(
            (actions[firstEntry + selectedRow] & ACTION_ID_MASK) +
                MsgAbilityDescription,
            messageText, BATTLE_MENU_MESSAGE_CAPACITY);
      } else {
        BATTLE_MENU_MESSAGE_COPY(MSGNO(MsgBattleNoPsynergy), messageText,
                                 BATTLE_MENU_MESSAGE_CAPACITY);
      }
      UiText_RenderWideStringAtOffset(messageText, messageWindow, 0, 4);
      drawnRow = selectedRow;

      // Cursor moves update help; only a page change reloads the row icons.
      if (firstEntry != drawnFirstEntry) {
        RenderOutput_RedrawSavedRect(window);
        for (i = 0; i < BATTLE_LIST_ROWS; i++) {
          struct BattleAct *action;

          encodedAction = actions[firstEntry + i];
          if (encodedAction == 0) {
            break;
          }
          action = BattleAction_Get(encodedAction);
          UiWindow_SetTilemapEntry(window, 0xf01f, 11, i * 2, 0);
          UiWindow_SetTilemapEntry(window, PSYNERGY_COST_SECOND_TILE, 12, i * 2,
                                   0);
          Ability_LoadGlyph(encodedAction & ACTION_ID_MASK, 0, &iconSlots[i],
                            &glyph, 1);
          sprites[i].oam.f.tile = glyph;

          if ((action->flags & ACTION_USABLE_IN_BATTLE) == 0) {
            UiWork_SetParameterNibble(4);
          } else if (action->ppCost > unit->pp) {
            UiWork_SetParameterNibble(2);
          } else if (unit->psySeal != 0) {
            UiWork_SetParameterNibble(9);
          }

          // The name and cost take BG palette 5 (UiRenderWork.level).
          work->level = 5;
          UiText_DrawCharacterAtOffset(encodedAction + MsgAbilityName, window,
                                       16, i * 16);
          UiText_DrawNumberAtOffset(action->ppCost, 2, window, 104, i * 16);
          UiWork_SetParameterNibble(15);
          work->level = 15;

          // Element 4 is none: no element icon.
          if (action->element != 4) {
            UiWindow_SetTilemapEntry(window, action->element + 0x5001, 15,
                                     i * 2, 0);
          }

          rangePattern = action->range;
          if (rangePattern == ABILITY_RANGE_ALL) {
            rangePattern = 11;
          } else {
            rangePattern--;
          }
          UiWindow_DrawThreeTileColumn(window, 16, i * 2, rangePattern, 0);
        }
        iconCount = i;
        drawnFirstEntry = firstEntry;
      }

      if (count > BATTLE_LIST_ROWS) {
        for (i = 0; i < (count + 4) / BATTLE_LIST_ROWS; i++) {
          iconTile = i + 0xf301;
          if (i == firstEntry / BATTLE_LIST_ROWS) {
            iconTile = i + 0xf30b;
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
        iconTile = i + 0xf301;
        if ((gFrameCount & 15) <= 11) {
          if (i == firstEntry / BATTLE_LIST_ROWS) {
            iconTile = i + 0xf30b;
          }
        }
        UiWindow_SetTilemapEntry(
            window, iconTile,
            window->width - (count + 4) / BATTLE_LIST_ROWS + i - 2, -1, 0);
      }
      pageTile = -1;
      UiWindow_SetTilemapEntry(
          window, 0xf334, window->width - (count + 4) / BATTLE_LIST_ROWS - 3,
          pageTile, 0);
      UiWindow_SetTilemapEntry(window, 0xf335, window->width - 2, pageTile, 0);
      UiRender_MarkRow(work, window->y - 1);
    }

    for (i = 0; i < iconCount; i++) {
      Runtime_PushSlotEntry(&sprites[i], 240);
    }

    cursorPosition.x = window->x * 8 - 4;
    cursorPosition.y = (selectedRow * 2 + window->y) * 8 + 20;
    ((u32 *)iconSprite)[1] = 0x40000000;
    ((u32 *)iconSprite)[2] = 0;
    iconSprite->oam.f.tile =
        VramCache_Reload(cursorHandle, (s32)gResource_FixedBlockBTiles);
    iconSprite->oam.f.x = cursorPosition.x + ((gFrameCount & 4) >> 1) - 4;
    iconSprite->oam.f.y = cursorPosition.y - ((gFrameCount & 4) >> 2) - 8;
    if (count != 0) {
      Runtime_PushSlotEntry(iconSprite, 242);
    }

    BATTLE_MENU_SET_CURSOR(firstEntry, selectedRow, preferredRow);
    navigation = gBattleMenuWork;

    if ((gKeysPressed & KEY_A) != 0) {
      if (count != 0) {
        result = firstEntry + selectedRow;
        if ((BattleAction_Get(actions[result])->flags &
             ACTION_USABLE_IN_BATTLE) != 0) {
          break;
        }
        /* FAKEMATCH(dead): a dead store keeps the test above from being
         * threaded past the break; without it, 2 of 794 instructions differ in
         * tbs-en. */
        result = -1;
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
          selectedRow = preferredRow;
          firstEntry -= BATTLE_LIST_ROWS;
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
  for (i = 0; i < BATTLE_LIST_ROWS; i++) {
    Resource_ResetEntry(iconSlots[i]);
  }
  Resource_ResetEntry(cursorHandle);
  WaitFrames(1);
  return result;
}
