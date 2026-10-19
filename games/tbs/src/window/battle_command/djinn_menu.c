/// @file
/// Part of battle_command.c, the menu object, compiled only through it.
/// The battle Djinn menu.

#include "../battle_command_internal.h"

// A recovering Djinni's turns left: its digits and their x in the list, in
// pixels.
#if TBS_PATCH_1
#define TURNS_DIGITS 1
#define TURNS_X 48
#else
#define TURNS_DIGITS 2
#define TURNS_X 40
#endif

#define CURSOR_OWNER (*(struct PartyStatusWindow **)gPartyStatusWork)

/// A selected Djinni packs its element/index, standby bit and recovery turns.
#define DJINN_ENTRY_STANDBY 0x10000
#define DJINN_ENTRY_RECOVERY_MASK 0x3e0000
#define DJINN_ENTRY_RECOVERY_SHIFT 17
#define DJINN_ENTRY_END 0x80000000

/// Select a Djinni for the current battle command. Entries encode its element,
/// index, standby state and recovery turns; -1 means cancel. R opens the
/// stat/Psynergy preview, and tutorial input follows the normal input path.
s32 Battle_SelectAbility(s32 owner, s32 unused) {
  struct CursorSprite cursor;
  u16 messageText[64];
  struct UiRenderWork *render;
  struct UiWindow *previewWindow;
  s32 drawnRow;
  s32 cursorSlot;
  u32 *djinnEntries;
  s32 djinnCount;
  s32 drawnFirstEntry;
  struct UiWindow *messageWindow;
  s32 standbyElements;
  s32 preferredRow;
  s32 previewPage;
  s32 redraw;
  s32 alreadySelected;
  s32 previewPages;
  struct MenuCell *navigation;
  struct BattleUnit *unit;
  struct DjinnRecoveryList *recovery;
  struct UiWindow *listWindow;
  struct UiWindow *iconWindow;
  struct PartyStatusWindow *partyStatus;
  u32 *entryCursor;
  u32 selectedDjinn;
  u32 blinkPhase;
  s32 firstEntry;
  s32 selectedRow;
  u32 encodedDjinn;
  s32 listPages;
  s32 palette;
  struct MenuPoint cursorPos;
  s32 pressed;
  s32 repeated;
  s32 held;
  s32 element;
  s32 djinnIndex;
  s32 i;
  s32 j;
  struct CursorSprite *cursorSprite;

  render = UI_WORK;
  previewWindow = 0;
  drawnRow = -1;
  cursorSlot = VramCache_AllocEntry(0x80);
  djinnEntries = Runtime_BumpAllocateAlternatePool(0xa8 * 2);
  djinnCount = 0;
  drawnFirstEntry = -1;
  encodedDjinn = 0;
  standbyElements = 0;
  previewPage = 0;
  previewPages = 0;
  redraw = 0;
  alreadySelected = 0;
  messageWindow = UiWindow_Create(0, 4, 30, 4, 42);
  UiWork_SetAltFlagAndClearTable(1);
  listWindow = UiWindow_Create(21, 9, 9, 11, 6);
  MenuCell_GetCursor(firstEntry, selectedRow, preferredRow);
  unit = UnitGetFar(owner);

  // Pack Djinn state and recovery turns before paging the selection list.
  for (element = 0; element <= 3; element++) {
    for (djinnIndex = 0,
        entryCursor = (u32 *)((djinnCount << 2) + (u32)djinnEntries);
         djinnIndex <= 19; djinnIndex++) {
      if (unit->djinnActive[element] & (u32)(1 << djinnIndex)) {
        *entryCursor = (u32)((element << 8) | djinnIndex);
        entryCursor++;
        djinnCount++;
      } else if (unit->djinnAvailable[element] & (u32)(1 << djinnIndex)) {
        u32 v;
        s32 k;
        recovery = &((struct DjinnRecoveryTable *)Djinn_GetRecoveryTableFar(
                         (u32)owner > 7 ? 1 : 0))
                        ->list;
        for (k = 0; k < recovery->count; k++) {
          if (recovery->entries[k].unitId == owner &&
              recovery->entries[k].element == element &&
              recovery->entries[k].index == djinnIndex) {
            break;
          }
        }
        v = (u32)((element << 8) | djinnIndex | DJINN_ENTRY_STANDBY);
        *entryCursor = v;
        if (recovery->entries[k].turns > 0) {
          v |= (u32)(recovery->entries[k].turns << DJINN_ENTRY_RECOVERY_SHIFT);
          *entryCursor = v;
        }
        djinnCount++;
        entryCursor++;
      }
    }
  }
  djinnEntries[djinnCount] = DJINN_ENTRY_END;
  render->dirty = 1;

  /* FAKEMATCH(dead): cursorSprite set before the loop, which sets it again,
   * keeps the native allocation; without it, 197 of 1201 instructions differ
   * in tbs-en. */
  cursorSprite = &cursor;
  for (;;) {
    // Rebuild descriptions only when the selection or preview page changes.
    if (firstEntry != drawnFirstEntry || selectedRow != drawnRow ||
        redraw != 0) {
      selectedDjinn = djinnEntries[firstEntry + selectedRow];
      standbyElements = 0;
      render->menuBusy = 1;
      Ui_SetRectHighlight(listWindow->x + 1, listWindow->y + drawnRow * 2 + 1,
                          listWindow->width - 2, 1, 15);
      if (previewPage != 0) {
        UiWork_Finalize(messageWindow, 1);
        messageWindow = UiWindow_Create(0, 4, 30, 4, 42);
        Ui_FillVramBlockPattern();
      }
      redraw = 0;
      if (djinnCount != 0) {
        s32 k;
        u32 selected;
        UiWork_ClearValueNameTables();
        alreadySelected = 0;
        /* FAKEMATCH(one-pass): a loop that runs once keeps the redraw reload
         * after the selected-entry load, and none of its instructions
         * survive; as k = redraw, 3 of 1201 instructions differ in tbs-en. */
        selected = COUNTDOWN->selected[redraw];
        for (k = redraw; k == redraw;) {
          k = redraw;
          break;
        }
        if (selected == selectedDjinn) {
          alreadySelected = 1;
        } else {
          for (;;) {
            /* FAKEMATCH: needed to match: isUsed is zero on every search-loop
             * continuation. Its step dependency prevents loop.c from
             * strength-reducing k into a pointer; CSE later removes the flag
             * term. Plain k differs 35 instructions in EN; this form restores
             * the native reload and indexing (differ 0). */
            k += 1 - alreadySelected;
            if (k > 7)
              break;
            if ((u32)COUNTDOWN->selected[k] == selectedDjinn) {
              alreadySelected = 1;
              break;
            }
          }
        }
        if (alreadySelected != 0) {
          BATTLE_MENU_MESSAGE_COPY(MsgBattleDjinnTwice, messageText,
                                   BATTLE_MENU_MESSAGE_CAPACITY);
          if (previewWindow != 0) {
            UiWork_Finalize(previewWindow, 1);
            previewWindow = 0;
            previewPage = 0;
          }
        } else if (selectedDjinn & DJINN_ENTRY_STANDBY) {
          if (selectedDjinn & DJINN_ENTRY_RECOVERY_MASK) {
            UiWork_PushValueSlot(
                (s32)((selectedDjinn & DJINN_ENTRY_RECOVERY_MASK) >>
                      DJINN_ENTRY_RECOVERY_SHIFT),
                5);
            BATTLE_MENU_MESSAGE_COPY((s32)(((selectedDjinn & 0xf00) >> 8) * 20 +
                                           (selectedDjinn & 0xff)) +
                                         MsgDjinnEffect,
                                     messageText, BATTLE_MENU_MESSAGE_CAPACITY);
            if (previewWindow != 0) {
              UiWork_Finalize(previewWindow, 1);
              previewWindow = 0;
              previewPage = 0;
            }
          } else {
            previewWindow = (struct UiWindow *)DjinnMenu_ShowChangePreview(
                previewWindow, owner, (s32)selectedDjinn, previewPage,
                &previewPages);
            UiText_CopyMessageString(MsgBattleSummonStandby, messageText,
                                     BATTLE_MENU_MESSAGE_CAPACITY);
            standbyElements = 1 << ((selectedDjinn & 0xf00) >> 8);
          }
        } else {
          previewWindow = (struct UiWindow *)DjinnMenu_ShowChangePreview(
              previewWindow, owner, (s32)selectedDjinn, previewPage,
              &previewPages);
          BATTLE_MENU_MESSAGE_COPY((s32)(((selectedDjinn & 0xf00) >> 8) * 20 +
                                         (selectedDjinn & 0xff)) +
                                       MsgDjinnEffect,
                                   messageText, BATTLE_MENU_MESSAGE_CAPACITY);
        }
      } else {
        BATTLE_MENU_MESSAGE_COPY(MsgBattleNoDjinn, messageText,
                                 BATTLE_MENU_MESSAGE_CAPACITY);
      }
      render->menuBusy = 0;
      if (previewPage == 0) {
        render->menuBusy = 1;
        UiWork_Finalize(messageWindow, 1);
        messageWindow = UiWindow_Create(0, 4, 30, 4, 42);
        render->menuBusy = (u8)previewPage;
      }
      UiText_RenderWideStringAtOffset(messageText, messageWindow, 0, 4);
      drawnRow = selectedRow;
      if (firstEntry != drawnFirstEntry) {
        RenderOutput_RedrawSavedRect(listWindow);
        j = 0;
        while ((encodedDjinn = djinnEntries[firstEntry + j]) !=
               DJINN_ENTRY_END) {
          UiWindow_SetTilemapEntry(listWindow,
                                   (s32)((encodedDjinn & 0xf00) >> 8) + 0x5001,
                                   0, j * 2, 0);
          if (encodedDjinn & DJINN_ENTRY_RECOVERY_MASK) {
            UiWork_SetParamNibble(4);
          } else if (encodedDjinn & DJINN_ENTRY_STANDBY) {
            UiWork_SetParamNibble(2);
          }
          UiText_DrawCharacterAtOffset(
              (s32)(((encodedDjinn & 0xf00) >> 8) * 20 +
                    (encodedDjinn & 0xff)) +
                  MsgDjinnName,
              listWindow, 8, j * 16);
          if (encodedDjinn & DJINN_ENTRY_RECOVERY_MASK) {
            UiText_DrawNumberAtOffset(
                (s32)((encodedDjinn & DJINN_ENTRY_RECOVERY_MASK) >>
                      DJINN_ENTRY_RECOVERY_SHIFT),
                TURNS_DIGITS, listWindow, TURNS_X, j * 16);
          }
          UiWork_SetParamNibble(15);
          j++;
          if (j >= BATTLE_LIST_ROWS) {
            break;
          }
        }
        drawnFirstEntry = firstEntry;
      }
      if (djinnCount > BATTLE_LIST_ROWS) {
        for (j = 0; j < (listPages = ((djinnCount + 4) / BATTLE_LIST_ROWS));
             j++) {
          s32 tile = j + 0xf301;
          if (j == (firstEntry / BATTLE_LIST_ROWS)) {
            tile = j + 0xf30b;
          }
          UiWindow_SetTilemapEntry(
              listWindow, tile, listWindow->width - listPages + j - 2, -1, 0);
        }
      }
      Ui_SetRectHighlight(listWindow->x + 1,
                          listWindow->y + selectedRow * 2 + 1,
                          listWindow->width - 2, 1, 14);
      render->dirty = 1;
      render->menuBusy = 0;
    }

    if (djinnCount > BATTLE_LIST_ROWS) {
      for (j = 0; j < (djinnCount + 4) / BATTLE_LIST_ROWS; j++) {
        s32 tile = j + 0xf301;
        if ((gKeysHeld & KEY_R) || (gFrameCount & 15) <= 11) {
          if (j == (firstEntry / BATTLE_LIST_ROWS)) {
            tile = j + 0xf30b;
          }
        }
        UiWindow_SetTilemapEntry(
            listWindow, tile,
            listWindow->width - ((djinnCount + 4) / BATTLE_LIST_ROWS) + j - 2,
            -1, 0);
      }
      if ((gKeysHeld & KEY_R) == 0) {
        UiWindow_SetTilemapEntry(
            listWindow, 0xf334,
            listWindow->width - (djinnCount + 4) / BATTLE_LIST_ROWS - 3, -1, 0);
        UiWindow_SetTilemapEntry(listWindow, 0xf335, listWindow->width - 2, -1,
                                 0);
      } else {
        UiWindow_SetTilemapEntry(
            listWindow, UI_TILE_FRAME_TOP,
            listWindow->width - (djinnCount + 4) / BATTLE_LIST_ROWS - 3, -1, 0);
        UiWindow_SetTilemapEntry(listWindow, UI_TILE_FRAME_TOP_RIGHT,
                                 listWindow->width - 2, -1, 0);
      }
      UiRender_MarkRow(render, listWindow->y - 1);
    }

    // Tutorial input replaces live keys but uses the same navigation below.
    pressed = gKeysPressed;
    repeated = gKeysRepeat;
    held = gKeysHeld;
    navigation = gLinkCountdownWork;
    if (navigation->autoEnabled != 0) {
      repeated = 0;
      pressed = 0;
      held = 0;
      if (navigation->autoDelay == 0) {
        navigation->autoDelay = 60;
        repeated = 1;
        pressed = 1;
      } else {
        navigation->autoDelay--;
      }
    }
    navigation = gLinkCountdownWork;
    if (navigation->active == 0 || (pressed & KEY_B)) {
      AudioCommand_PlayFar(SE_MENU_CANCEL);
      encodedDjinn = -1;
      break;
    }
    if (pressed & KEY_A) {
      u32 selection;
      u32 turns;
      if (djinnCount != 0) {
        selection = djinnEntries[firstEntry + selectedRow];
        turns = selection & DJINN_ENTRY_RECOVERY_MASK;
        if (turns == 0) {
          if (alreadySelected == 0) {
            navigation->page = firstEntry;
            navigation->row = selectedRow;
            navigation->preferredRow = preferredRow;
            encodedDjinn = selection;
            break;
          }
        } else if (alreadySelected == 0) {
          Ui_FillVramBlockPattern();
          UiWork_ClearValueNameTables();
          UiWork_PushValueSlot((s32)((turns & DJINN_ENTRY_RECOVERY_MASK) >>
                                     DJINN_ENTRY_RECOVERY_SHIFT),
                               5);
          UiText_CopyMessageString(MsgBattleDjinnRecovering, messageText,
                                   BATTLE_MENU_MESSAGE_CAPACITY);
          UiText_RenderWideStringAtOffset(messageText, messageWindow, 0, 4);
        }
        AudioCommand_PlayFar(SE_MENU_ERROR);
      } else {
        encodedDjinn = -1;
        break;
      }
    }

    if (djinnCount != 0) {
      if (repeated & KEY_DOWN) {
        AudioCommand_PlayFar(SE_MENU_CURSOR_MOVE);
        selectedRow++;
        if (selectedRow == BATTLE_LIST_ROWS ||
            firstEntry + selectedRow == djinnCount) {
          selectedRow = 0;
        }
        preferredRow = selectedRow;
      } else if (repeated & KEY_UP) {
        AudioCommand_PlayFar(SE_MENU_CURSOR_MOVE);
        selectedRow--;
        if (selectedRow < 0) {
          if (firstEntry ==
              ((djinnCount - 1) / BATTLE_LIST_ROWS) * BATTLE_LIST_ROWS) {
            selectedRow = djinnCount - firstEntry - 1;
          } else {
            selectedRow = 4;
          }
        }
        preferredRow = selectedRow;
      } else if (held & KEY_R) {
        if (previewWindow != 0) {
          for (i = 0; i < previewPages; i++) {
            s32 tile = i + 0xf301;
            if ((gFrameCount & 15) <= 11 && i == previewPage - 1) {
              tile = previewPage - 1 + 0xf30b;
            }
            UiWindow_SetTilemapEntry(
                previewWindow, tile,
                previewWindow->width - previewPages + i - 2, -1, 0);
          }
          UiWindow_SetTilemapEntry(previewWindow, 0xf334,
                                   previewWindow->width - previewPages - 3, -1,
                                   0);
          UiWindow_SetTilemapEntry(previewWindow, 0xf335,
                                   previewWindow->width - 2, -1, 0);
          UiRender_MarkRow(render, previewWindow->y);
        }
        if (previewPage == 0 && previewPages != 0) {
          if (previewWindow != 0) {
            RenderOutput_ClearList(previewWindow);
          }
          previewPage = 1;
          redraw = 1;
        } else {
          if (previewPage > previewPages) {
            previewPage = previewPages;
          }
          if (previewPage != 0) {
            if (repeated & KEY_RIGHT) {
              AudioCommand_PlayFar(SE_MENU_CURSOR_MOVE);
              previewPage++;
              if (previewPage > previewPages) {
                previewPage = 1;
              }
              redraw = 1;
            } else if (repeated & KEY_LEFT) {
              AudioCommand_PlayFar(SE_MENU_CURSOR_MOVE);
              previewPage--;
              if (previewPage <= 0) {
                previewPage = previewPages;
              }
              redraw = 1;
            }
          }
        }
      } else if (previewPage != 0) {
        if (previewWindow != 0) {
          RenderOutput_ClearList(previewWindow);
        }
        previewPage = 0;
        redraw = 1;
      } else if (repeated & KEY_RIGHT) {
        AudioCommand_PlayFar(SE_MENU_CURSOR_MOVE);
        Input_ResetKeyRepeat();
        if (firstEntry + BATTLE_LIST_ROWS >= djinnCount) {
          if (firstEntry != 0) {
            firstEntry = 0;
            selectedRow = preferredRow;
          }
        } else {
          firstEntry += BATTLE_LIST_ROWS;
          selectedRow = preferredRow;
          if (firstEntry ==
              ((djinnCount - 1) / BATTLE_LIST_ROWS) * BATTLE_LIST_ROWS) {
            selectedRow = djinnCount - firstEntry - 1;
            if (selectedRow > preferredRow) {
              selectedRow = preferredRow;
            }
          }
        }
      } else if (repeated & KEY_LEFT) {
        AudioCommand_PlayFar(SE_MENU_CURSOR_MOVE);
        Input_ResetKeyRepeat();
        if (firstEntry != 0) {
          firstEntry -= BATTLE_LIST_ROWS;
          selectedRow = preferredRow;
        } else {
          firstEntry = ((djinnCount - 1) / BATTLE_LIST_ROWS) * BATTLE_LIST_ROWS;
          selectedRow = preferredRow;
          if (firstEntry != 0) {
            selectedRow = djinnCount - firstEntry - 1;
            if (selectedRow > preferredRow) {
              selectedRow = preferredRow;
            }
          }
        }
      }
    }

    {
      s32 cx;

      /* FAKEMATCH(one-pass): the x computed in a block of its own keeps the
       * row's reload after the x shift; as no block, mov r2, r10 fills the
       * ldrh delay slot before lsls and 2 of 1201 instructions differ in
       * tbs-en. */
      do {
        cx = listWindow->x * 8;
      } while (0);
      /* FAKEMATCH(barrier): an empty asm reading the row in a low register,
       * so it is reloaded before the subtract and row * 2 inherits that
       * reload: mov r2, r10 precedes subs r3, #2 as native; without it, 2 of
       * 1201 instructions differ in tbs-en. */
      asm("" : : "l"(selectedRow));
      cursorPos.x = cx - 2;
    }
    cursorPos.y = (selectedRow * 2 + listWindow->y) * 8 + 20;
    /* FAKEMATCH(dead): cursorSprite set again to the cursor it already points
     * at keeps the native reload; without it, 18 of 1201 instructions differ
     * in tbs-en. */
    cursorSprite = &cursor;
    ((u32 *)cursorSprite)[1] = MENU_SPRITE_SIZE_16;
    ((u32 *)cursorSprite)[2] = 0;
    cursorSprite->tile =
        VramCache_Reload(cursorSlot, (s32)gResource_FixedBlockBTiles);
    cursorSprite->x = cursorPos.x + ((gFrameCount & 4) >> 1) - 6;
    cursorSprite->y = cursorPos.y - ((gFrameCount & 4) >> 2) - 8;
    if (djinnCount != 0) {
      Runtime_PushSlotEntry(cursorSprite, 242);
    }
    partyStatus = CURSOR_OWNER;
    blinkPhase = gFrameCount & 4;
    iconWindow = partyStatus->window;
    if (partyStatus->flags & 2) {
      for (i = 0; i <= 3; i++) {
        palette = 15 - (blinkPhase != 0);
        if (((1 << i) & standbyElements) == 0) {
          palette = 15;
        }
        Ui_SetRectHighlight(iconWindow->x + gDjinnMenu_ElementXOffsets[i] + 1,
                            iconWindow->y + gDjinnMenu_ElementYOffsets[i] + 1,
                            2, 2, palette);
      }
    }
    if (gFrameCount & 4) {
      Iwram_CopyWords((void *)0x06006500, gDjinnMenu_BlinkTile, 32);
      Iwram_CopyWords((void *)0x06006520, gDjinnMenu_BlinkTile, 32);
    } else {
      Iwram_FillWords((void *)0x06006500, 32, 0x44444444);
      Iwram_CopyWords((void *)0x06006520, gDjinnMenu_BlinkTile, 32);
    }
    WaitFrames(1);
  }

  if (CURSOR_OWNER->flags & 2) {
    Ui_SetRectHighlight(CURSOR_OWNER->window->x + 1,
                        CURSOR_OWNER->window->y + 1, 4, 4, 15);
  }
  // Restore the party strip before releasing the Djinn menu's resources.
  Resource_ResetEntry(cursorSlot);
  UiWork_Finalize(messageWindow, 1);
  UiWork_Finalize(previewWindow, 1);
  UiWork_Finalize(listWindow, 1);
  UiWindow_MarkVisibleTileAttributes();
  UiWork_SetAltFlagAndClearTable(0);
  MemFree(djinnEntries);
  WaitFrames(1);
  return (s32)encodedDjinn;
}
