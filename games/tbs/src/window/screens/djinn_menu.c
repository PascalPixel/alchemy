/// @file
/// The battle Djinn menu.
// Part of screens.c.

#include "../screens_internal.h"

// A recovering Djinni's turns left: its digits and their x in the list, in
// pixels.
#if LANGUAGE != LANGUAGE_JAPANESE
#define DJINN_RECOVERY_DIGITS 1
#define DJINN_RECOVERY_X 48
#else
#define DJINN_RECOVERY_DIGITS 2
#define DJINN_RECOVERY_X 40
#endif

/// The party status window's layout, whose Djinn column the menu blinks.
#define CURSOR_OWNER (*(struct PartyStatusWindow **)gPartyStatusWork)

// A selected Djinni packs its element/index, standby bit and recovery turns:
// bits 11..8 element | 7..0 number in the element, and these.
#define DJINN_ENTRY_STANDBY 0x10000 ///< on standby or recovering (not Set)
/// bits 21..17: turns until a recovering Djinni is back; 0 when ready.
#define DJINN_ENTRY_RECOVERY_MASK 0x3e0000
#define DJINN_ENTRY_RECOVERY_SHIFT 17
#define DJINN_ENTRY_END 0x80000000 ///< ends the list

/// The battle Djinn menu for unit `member`: its Djinn in element and number
/// order, five to a page, set ones plain, standby ones in TEXT_COLOR_RED and
/// recovering ones in TEXT_COLOR_YELLOW with their turns, the one under the
/// cursor described above the list with its change preview beside it
/// (BattleDjinnMenu_ShowChangePreview; holding R shows the Psynergy pages, Left
/// and Right turn them). Returns the chosen Djinni's entry (element << 8 |
/// number, with DJINN_ENTRY_STANDBY when on standby), or -1 when cancelled,
/// when the link battle's time runs out, or when the party member has no Djinn.
/// A recovering Djinni, or one an earlier command this turn chose
/// (BattleMenuWork.chosenDjinn), is refused. Battle_CollectPartyCommands runs
/// it for the Djinn command; the Djinn tutorial presses A every 60 frames.
s32 BattleDjinnMenu_Run(s32 member, s32 unused) {
  struct CursorSprite cursorStorage;
  u16 messageText[64];
  struct WindowWork *work;
  struct Window *previewWindow;
  s32 drawnRow;
  s32 cursorSlot;
  u32 *djinnEntries;
  s32 djinnCount;
  s32 drawnFirstEntry;
  struct Window *messageWindow;
  s32 standbyElements;
  s32 preferredRow;
  s32 previewPage;
  s32 redraw;
  s32 alreadySelected;
  s32 previewPages;
  struct BattleMenuWork *navigation;
  struct Unit *unit;
  struct DjinnRecoveryList *list;
  struct Window *listWindow;
  struct Window *iconWindow;
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
  struct CursorSprite *sprite;

  work = WINDOW_WORK;
  previewWindow = 0;
  drawnRow = -1;
  cursorSlot = VramCache_AllocEntry(0x80);
  // 84 words: room for 4 elements of 20 Djinn and the end.
  djinnEntries = Heap_AllocEwram(0xa8 * 2);
  djinnCount = 0;
  drawnFirstEntry = -1;
  encodedDjinn = 0;
  standbyElements = 0;
  previewPage = 0;
  previewPages = 0;
  redraw = 0;
  alreadySelected = 0;
  messageWindow = Window_Create(0, 4, 30, 4,
                                WINDOW_FLAG_INSTANT | WINDOW_FLAG_TEXT_CANVAS |
                                    WINDOW_FLAG_BORDERED_CANVAS);
  Text_UseExtraTiles(1);
  listWindow = Window_Create(21, 9, 9, 11, 6);
  BATTLE_MENU_GET_CURSOR(firstEntry, selectedRow, preferredRow);
  unit = Unit_GetFar(member);

  // Pack Djinn state and recovery turns before paging the selection list.
  for (element = 0; element <= 3; element++) {
    for (djinnIndex = 0,
        entryCursor = (u32 *)((djinnCount << 2) + (u32)djinnEntries);
         djinnIndex <= 19; djinnIndex++) {
      if (unit->djinnSet[element] & (u32)(1 << djinnIndex)) {
        *entryCursor = (u32)((element << 8) | djinnIndex);
        entryCursor++;
        djinnCount++;
      } else if (unit->djinnHeld[element] & (u32)(1 << djinnIndex)) {
        u32 v;
        s32 k;
        // Owned but not set: look for its recovery in its side's list
        // (units above 7 are enemies). When it is not there, k ends at the
        // count and the turns read come from the first unused entry.
        list = &((struct DjinnRecoveryTable *)Djinn_GetRecoveryTableFar(
                     (u32)member > 7 ? 1 : 0))
                    ->list;
        for (k = 0; k < list->count; k++) {
          if (list->entries[k].unitId == member &&
              list->entries[k].element == element &&
              list->entries[k].index == djinnIndex) {
            break;
          }
        }
        v = (u32)((element << 8) | djinnIndex | DJINN_ENTRY_STANDBY);
        *entryCursor = v;
        if (list->entries[k].turns > 0) {
          v |= (u32)(list->entries[k].turns << DJINN_ENTRY_RECOVERY_SHIFT);
          *entryCursor = v;
        }
        djinnCount++;
        entryCursor++;
      }
    }
  }
  djinnEntries[djinnCount] = DJINN_ENTRY_END;
  work->dirty = 1;

  /* FAKEMATCH(dead): sprite set before the loop, which sets it again,
   * keeps the native allocation; without it, 197 of 1201 instructions differ
   * in tbs-en. */
  sprite = &cursorStorage;
  for (;;) {
    // Rebuild descriptions only when the selection or preview page changes:
    // the message ("Cannot use the same Djinni twice.", "Ready to summon."
    // for a ready standby Djinni, the Djinni's effect otherwise, or "You have
    // no Djinn."), the preview, the list page, the page numbers and the row's
    // highlight.
    if (firstEntry != drawnFirstEntry || selectedRow != drawnRow ||
        redraw != 0) {
      selectedDjinn = djinnEntries[firstEntry + selectedRow];
      standbyElements = 0;
      work->holdTilemapUpload = 1;
      Window_HighlightRect(listWindow->x + 1, listWindow->y + drawnRow * 2 + 1,
                           listWindow->width - 2, 1, WINDOW_PALETTE);
      if (previewPage != 0) {
        Window_Close(messageWindow, 1);
        messageWindow =
            Window_Create(0, 4, 30, 4,
                          WINDOW_FLAG_INSTANT | WINDOW_FLAG_TEXT_CANVAS |
                              WINDOW_FLAG_BORDERED_CANVAS);
        Text_FillCanvasBackground();
      }
      redraw = 0;
      if (djinnCount != 0) {
        s32 k;
        u32 selected;
        MessageArgument_ClearAll();
        alreadySelected = 0;
        /* FAKEMATCH(one-pass): a loop that runs once keeps the redraw reload
         * after the selected-entry load, and none of its instructions
         * survive; as k = redraw, 3 of 1201 instructions differ in tbs-en. */
        selected = gBattleMenuWork->chosenDjinn[redraw];
        for (k = redraw; k == redraw;) {
          k = redraw;
          break;
        }
        if (selected == selectedDjinn) {
          alreadySelected = 1;
        } else {
          for (;;) {
            /* FAKEMATCH(dead): the flag, zero on every continuation, keeps k
             * an index rather than a pointer; as k++, 1 of 1201 instructions
             * differ in tbs-en. */
            k += 1 - alreadySelected;
            if (k > 7)
              break;
            if ((u32)gBattleMenuWork->chosenDjinn[k] == selectedDjinn) {
              alreadySelected = 1;
              break;
            }
          }
        }
        if (alreadySelected != 0) {
          BATTLE_MENU_MESSAGE_COPY(MsgBattleDjinnTwice, messageText,
                                   BATTLE_MENU_MESSAGE_CAPACITY);
          if (previewWindow != 0) {
            Window_Close(previewWindow, 1);
            previewWindow = 0;
            previewPage = 0;
          }
        } else if (selectedDjinn & DJINN_ENTRY_STANDBY) {
          // A recovering Djinni: its effect, with the turns as the argument
          // (MESSAGE_ARGUMENT_NUMBER), and no preview. A ready one: the preview
          // and "Ready to summon.", its element blinking in the party window.
          if (selectedDjinn & DJINN_ENTRY_RECOVERY_MASK) {
            MessageArgument_Push(
                (s32)((selectedDjinn & DJINN_ENTRY_RECOVERY_MASK) >>
                      DJINN_ENTRY_RECOVERY_SHIFT),
                MESSAGE_ARGUMENT_NUMBER);
            BATTLE_MENU_MESSAGE_COPY((s32)(((selectedDjinn & 0xf00) >> 8) * 20 +
                                           (selectedDjinn & 0xff)) +
                                         MsgDjinnEffect,
                                     messageText, BATTLE_MENU_MESSAGE_CAPACITY);
            if (previewWindow != 0) {
              Window_Close(previewWindow, 1);
              previewWindow = 0;
              previewPage = 0;
            }
          } else {
            previewWindow = (struct Window *)BattleDjinnMenu_ShowChangePreview(
                previewWindow, member, (s32)selectedDjinn, previewPage,
                &previewPages);
            Message_CopyExpanded(MsgBattleSummonStandby, messageText,
                                 BATTLE_MENU_MESSAGE_CAPACITY);
            standbyElements = 1 << ((selectedDjinn & 0xf00) >> 8);
          }
        } else {
          previewWindow = (struct Window *)BattleDjinnMenu_ShowChangePreview(
              previewWindow, member, (s32)selectedDjinn, previewPage,
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
      work->holdTilemapUpload = 0;
      if (previewPage == 0) {
        work->holdTilemapUpload = 1;
        Window_Close(messageWindow, 1);
        messageWindow =
            Window_Create(0, 4, 30, 4,
                          WINDOW_FLAG_INSTANT | WINDOW_FLAG_TEXT_CANVAS |
                              WINDOW_FLAG_BORDERED_CANVAS);
        work->holdTilemapUpload = (u8)previewPage;
      }
      Text_DrawWideStringAtOffset(messageText, messageWindow, 0, 4);
      drawnRow = selectedRow;
      if (firstEntry != drawnFirstEntry) {
        WindowSprite_RedrawSavedRect(listWindow);
        j = 0;
        while ((encodedDjinn = djinnEntries[firstEntry + j]) !=
               DJINN_ENTRY_END) {
          Window_SetTilemapEntry(listWindow,
                                 (s32)((encodedDjinn & 0xf00) >> 8) + 0x5001, 0,
                                 j * 2, 0);
          if (encodedDjinn & DJINN_ENTRY_RECOVERY_MASK) {
            Text_SetColor(TEXT_COLOR_YELLOW);
          } else if (encodedDjinn & DJINN_ENTRY_STANDBY) {
            Text_SetColor(TEXT_COLOR_RED);
          }
          Text_DrawMessageAsTiles((s32)(((encodedDjinn & 0xf00) >> 8) * 20 +
                                        (encodedDjinn & 0xff)) +
                                      MsgDjinnName,
                                  listWindow, 8, j * 16);
          if (encodedDjinn & DJINN_ENTRY_RECOVERY_MASK) {
            Text_DrawNumberAtOffset(
                (s32)((encodedDjinn & DJINN_ENTRY_RECOVERY_MASK) >>
                      DJINN_ENTRY_RECOVERY_SHIFT),
                DJINN_RECOVERY_DIGITS, listWindow, DJINN_RECOVERY_X, j * 16);
          }
          Text_SetColor(TEXT_COLOR_WHITE);
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
          Window_SetTilemapEntry(listWindow, tile,
                                 listWindow->width - listPages + j - 2, -1, 0);
        }
      }
      Window_HighlightRect(listWindow->x + 1,
                           listWindow->y + selectedRow * 2 + 1,
                           listWindow->width - 2, 1, HIGHLIGHT_PALETTE);
      work->dirty = 1;
      work->holdTilemapUpload = 0;
    }

    // Each frame: the current page's number lit 12 frames in 16 (steady while
    // R is held), and the page arrows, which give way to the plain frame
    // while R turns the preview's pages instead.
    if (djinnCount > BATTLE_LIST_ROWS) {
      for (j = 0; j < (djinnCount + 4) / BATTLE_LIST_ROWS; j++) {
        s32 tile = j + 0xf301;
        if ((gKeysHeld & KEY_R) || (gFrameCount & 15) <= 11) {
          if (j == (firstEntry / BATTLE_LIST_ROWS)) {
            tile = j + 0xf30b;
          }
        }
        Window_SetTilemapEntry(
            listWindow, tile,
            listWindow->width - ((djinnCount + 4) / BATTLE_LIST_ROWS) + j - 2,
            -1, 0);
      }
      if ((gKeysHeld & KEY_R) == 0) {
        Window_SetTilemapEntry(
            listWindow, LIST_TILE_PAGES_LEFT,
            listWindow->width - (djinnCount + 4) / BATTLE_LIST_ROWS - 3, -1, 0);
        Window_SetTilemapEntry(listWindow, LIST_TILE_PAGES_RIGHT,
                               listWindow->width - 2, -1, 0);
      } else {
        Window_SetTilemapEntry(
            listWindow, WINDOW_TILE_BORDER_TOP,
            listWindow->width - (djinnCount + 4) / BATTLE_LIST_ROWS - 3, -1, 0);
        Window_SetTilemapEntry(listWindow, WINDOW_TILE_BORDER_TOP_RIGHT,
                               listWindow->width - 2, -1, 0);
      }
      Window_MarkTilemapRow(work, listWindow->y - 1);
    }

    // Tutorial input replaces live keys but uses the same navigation below.
    pressed = gKeysPressed;
    repeated = gKeysRepeat;
    held = gKeysHeld;
    navigation = gBattleMenuWork;
    if (navigation->tutorialStep != 0) {
      repeated = 0;
      pressed = 0;
      held = 0;
      if (navigation->tutorialDelay == 0) {
        navigation->tutorialDelay = 60;
        repeated = 1;
        pressed = 1;
      } else {
        navigation->tutorialDelay--;
      }
    }
    navigation = gBattleMenuWork;
    if (navigation->timer == 0 || (pressed & KEY_B)) {
      Sound_PlayFar(SE_MENU_CANCEL);
      encodedDjinn = -1;
      break;
    }
    // A takes a Djinni that is ready and not chosen already; a recovering
    // one is refused with "Your Djinni is still recovering.".
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
          Text_FillCanvasBackground();
          MessageArgument_ClearAll();
          MessageArgument_Push((s32)((turns & DJINN_ENTRY_RECOVERY_MASK) >>
                                     DJINN_ENTRY_RECOVERY_SHIFT),
                               MESSAGE_ARGUMENT_NUMBER);
          Message_CopyExpanded(MsgBattleDjinnRecovering, messageText,
                               BATTLE_MENU_MESSAGE_CAPACITY);
          Text_DrawWideStringAtOffset(messageText, messageWindow, 0, 4);
        }
        Sound_PlayFar(SE_MENU_ERROR);
      } else {
        encodedDjinn = -1;
        break;
      }
    }

    if (djinnCount != 0) {
      if (repeated & KEY_DOWN) {
        Sound_PlayFar(SE_MENU_CURSOR_MOVE);
        selectedRow++;
        if (selectedRow == BATTLE_LIST_ROWS ||
            firstEntry + selectedRow == djinnCount) {
          selectedRow = 0;
        }
        preferredRow = selectedRow;
      } else if (repeated & KEY_UP) {
        Sound_PlayFar(SE_MENU_CURSOR_MOVE);
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
        // R held: the preview's page numbers and arrows on its top edge; the
        // first frame turns to page 1, then Left and Right turn the pages.
        if (previewWindow != 0) {
          for (i = 0; i < previewPages; i++) {
            s32 tile = i + 0xf301;
            if ((gFrameCount & 15) <= 11 && i == previewPage - 1) {
              tile = previewPage - 1 + 0xf30b;
            }
            Window_SetTilemapEntry(previewWindow, tile,
                                   previewWindow->width - previewPages + i - 2,
                                   -1, 0);
          }
          Window_SetTilemapEntry(previewWindow, LIST_TILE_PAGES_LEFT,
                                 previewWindow->width - previewPages - 3, -1,
                                 0);
          Window_SetTilemapEntry(previewWindow, LIST_TILE_PAGES_RIGHT,
                                 previewWindow->width - 2, -1, 0);
          Window_MarkTilemapRow(work, previewWindow->y);
        }
        if (previewPage == 0 && previewPages != 0) {
          if (previewWindow != 0) {
            WindowSprite_ClearList(previewWindow);
          }
          previewPage = 1;
          redraw = 1;
        } else {
          if (previewPage > previewPages) {
            previewPage = previewPages;
          }
          if (previewPage != 0) {
            if (repeated & KEY_RIGHT) {
              Sound_PlayFar(SE_MENU_CURSOR_MOVE);
              previewPage++;
              if (previewPage > previewPages) {
                previewPage = 1;
              }
              redraw = 1;
            } else if (repeated & KEY_LEFT) {
              Sound_PlayFar(SE_MENU_CURSOR_MOVE);
              previewPage--;
              if (previewPage <= 0) {
                previewPage = previewPages;
              }
              redraw = 1;
            }
          }
        }
      } else if (previewPage != 0) {
        // R let go: back to the stats page.
        if (previewWindow != 0) {
          WindowSprite_ClearList(previewWindow);
        }
        previewPage = 0;
        redraw = 1;
      } else if (repeated & KEY_RIGHT) {
        Sound_PlayFar(SE_MENU_CURSOR_MOVE);
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
        Sound_PlayFar(SE_MENU_CURSOR_MOVE);
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
       * row's reload after the x shift; as no block, 2 of 1201 instructions
       * differ in tbs-en. */
      do {
        cx = listWindow->x * 8;
      } while (0);
      /* FAKEMATCH(barrier): an empty asm reading the row in a low register
       * reloads it before the subtract; without it, 2 of 1201 instructions
       * differ in tbs-en. */
      __asm__("" : : "l"(selectedRow));
      cursorPos.x = cx - 2;
    }
    cursorPos.y = (selectedRow * 2 + listWindow->y) * 8 + 20;
    /* FAKEMATCH(dead): sprite set again to the cursor it already points
     * at keeps the native reload; without it, 18 of 1201 instructions differ
     * in tbs-en. */
    sprite = &cursorStorage;
    ((u32 *)sprite)[1] = MENU_SPRITE_SHAPE_16X16;
    ((u32 *)sprite)[2] = 0;
    sprite->tile = VramCache_Reload(cursorSlot, (s32)gPointingHandTiles);
    sprite->x = cursorPos.x + ((gFrameCount & 4) >> 1) - 6;
    sprite->y = cursorPos.y - ((gFrameCount & 4) >> 2) - 8;
    if (djinnCount != 0) {
      Oam_AddSpriteAtDepth(sprite, 242);
    }
    // In the party status window's Djinn column (PARTY_STATUS_SHOW_DJINN),
    // the element of a ready standby Djinni under the cursor blinks between
    // HIGHLIGHT_PALETTE and WINDOW_PALETTE every four frames; then the arrow
    // tiles' blink.
    partyStatus = CURSOR_OWNER;
    blinkPhase = gFrameCount & 4;
    iconWindow = partyStatus->window;
    if (partyStatus->flags & 2) {
      for (i = 0; i <= 3; i++) {
        palette = 15 - (blinkPhase != 0);
        if (((1 << i) & standbyElements) == 0) {
          palette = 15;
        }
        Window_HighlightRect(iconWindow->x + gDjinnMenu_ElementXOffsets[i] + 1,
                             iconWindow->y + gDjinnMenu_ElementYOffsets[i] + 1,
                             2, 2, palette);
      }
    }
    if (gFrameCount & 4) {
      Iwram_CopyWords((void *)0x06006500, gDjinnMenu_BlinkTile, 32);
      Iwram_CopyWords((void *)0x06006520, gDjinnMenu_BlinkTile, 32);
    } else {
      Iwram_FillWords((void *)0x06006500, 32, TEXT_CANVAS_BACKGROUND_WORD);
      Iwram_CopyWords((void *)0x06006520, gDjinnMenu_BlinkTile, 32);
    }
    WaitFrames(1);
  }

  // Leave the Djinn column in WINDOW_PALETTE.
  if (CURSOR_OWNER->flags & 2) {
    Window_HighlightRect(CURSOR_OWNER->window->x + 1,
                         CURSOR_OWNER->window->y + 1, 4, 4, WINDOW_PALETTE);
  }
  // Restore the party strip before releasing the Djinn menu's resources.
  VramCache_FreeEntry(cursorSlot);
  Window_Close(messageWindow, 1);
  Window_Close(previewWindow, 1);
  Window_Close(listWindow, 1);
  Window_MarkVisibleTextTiles();
  Text_UseExtraTiles(0);
  Heap_Release(djinnEntries);
  WaitFrames(1);
  return (s32)encodedDjinn;
}
