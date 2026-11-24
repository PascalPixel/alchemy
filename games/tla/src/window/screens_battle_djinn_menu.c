/// @file
/// The battle Djinn menu.
// Part of screens.c.

#include "window_internal.h"
#include "menu/djinn_menu.h"
#include "sounds.h"
#include "version.h"
#include "window/screens.h"
#include "window/window.h"

/// The Japanese list window is 9 tiles wide from column 21 and a recovering
/// Djinni's turns stand at x 40.
#if LANGUAGE == LANGUAGE_JAPANESE
#define DJINN_LIST_X 21
#define DJINN_LIST_WIDTH 9
#define DJINN_TURNS_X 40
#else
#define DJINN_LIST_X 20
#define DJINN_LIST_WIDTH 10
#define DJINN_TURNS_X 48
#endif

#define DJINN_MENU_ARROW_VRAM                                                  \
  (BG_CHAR_BLOCK(0) + DJINN_MENU_ARROW_TILE * TILE_SIZE_4BPP)

/// The battle Djinn menu for unit `member`: its Djinn in element and number
/// order, five to a page, set ones plain, standby ones in TEXT_COLOR_RED and
/// recovering ones in TEXT_COLOR_YELLOW with their turns, the one under the
/// cursor described above the list with its change preview beside it
/// (BattleDjinnMenu_ShowChangePreview; holding R shows the Psynergy pages, Left
/// and Right turn them).
/// The battle commands pass 1 as unused, which no code reads.
s32 BattleDjinnMenu_Run(s32 member, s32 unused) {
  struct QueuedSprite cursorSprite;
  u16 messageText[64];
  struct WindowState *state;
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
#if TLA_PATCH_1
  s32 redrawList;
#endif
  struct BattleMenuState *navigation;
  struct Unit *unit;
  struct DjinnRecoveryList *list;
  struct Window *listWindow;
  struct Window *iconWindow;
  struct PartyStatusWindow *partyStatus;
  u32 *entryCursor;
  s32 listEntry;
  u32 selectedDjinn;
  u32 blinkPhase;
  s32 firstEntry;
  s32 selectedRow;
  u32 encodedDjinn;
  struct {
    s32 x, y;
  } cursorPos;
  s32 pressed;
  s32 repeated;
  s32 held;
  s32 element;
  s32 djinnIndex;
  s32 i;
  s32 j;

  state = (struct WindowState *)Ram_HeapSlots->windowState;
  previewWindow = NULL;
  drawnRow = -1;
  cursorSlot = VramCache_AllocEntry(128);
  djinnEntries = Heap_AllocEwram(84 * sizeof(*djinnEntries));
  djinnCount = 0;
  drawnFirstEntry = drawnRow;
  encodedDjinn = 0;
  standbyElements = 0;
  previewPage = 0;
  previewPages = 0;
  redraw = 0;
  alreadySelected = 0;
#if TLA_PATCH_1
  redrawList = 0;
#endif
  messageWindow = Window_Create(0, 4, 30, 4,
                                WINDOW_FLAG_INSTANT | WINDOW_FLAG_TEXT_CANVAS |
                                    WINDOW_FLAG_BORDERED_CANVAS);
  Text_UseExtraTiles(1);
  listWindow = Window_Create(DJINN_LIST_X, 9, DJINN_LIST_WIDTH, 11,
                             WINDOW_FLAG_INSTANT | WINDOW_FLAG_UNREAD);
  BATTLE_MENU_GET_CURSOR(firstEntry, selectedRow, preferredRow);
  unit = Unit_Get(member);

  for (element = 0; element <= 3; element++) {
    /* FAKEMATCH(signature): the entry cursor summed as integers; as
     * djinnEntries + djinnCount. */
    for (djinnIndex = 0,
        entryCursor = (u32 *)((djinnCount << 2) + (u32)djinnEntries);
         djinnIndex < DJINN_PER_ELEMENT; djinnIndex++) {
      if (unit->djinnSet[element] & (u32)(1 << djinnIndex)) {
        *entryCursor = (u32)DJINN_COMMAND(element, djinnIndex);
        entryCursor++;
        djinnCount++;
      } else if (unit->djinnHeld[element] & (u32)(1 << djinnIndex)) {
        u32 v;
        s32 k;

        list =
            &(Djinn_GetRecoveryTableFar((u32)member > ALLY_MAX ? 1 : 0))->list;
        for (k = 0; k < list->count; k++) {
          if (list->entries[k].unitId == member &&
              list->entries[k].element == element &&
              list->entries[k].index == djinnIndex) {
            break;
          }
        }
        v = (u32)(DJINN_COMMAND(element, djinnIndex) | BATTLE_DJINN_UNSET);
        *entryCursor = v;
        if (list->entries[k].turns > 0) {
          v |= (u32)(list->entries[k].turns << BATTLE_DJINN_TURNS_SHIFT);
          *entryCursor = v;
        }
        djinnCount++;
        entryCursor++;
      }
    }
  }
  djinnEntries[djinnCount] = BATTLE_DJINN_END;
  state->dirty = 1;

  for (;;) {
    /* LEFTOVER(template): sprite holds &cursorSprite, as
     * BattlePsynergyMenu_Run's iconSprite holds its cursor; as &cursorSprite at
     * each use. */
    struct QueuedSprite *sprite = &cursorSprite;

    if (firstEntry != drawnFirstEntry || selectedRow != drawnRow ||
        redraw != 0) {
      selectedDjinn = djinnEntries[firstEntry + selectedRow];
      standbyElements = 0;
      state->holdTilemapUpload = 1;
      Window_HighlightRect((s16)listWindow->x + 1,
                           (s16)listWindow->y + drawnRow * 2 + 1,
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
        s32 k = 0;

        MessageArgument_ClearAll();
        alreadySelected = 0;
        for (k = 0; k <= 7; k++) {
          if (BATTLE_MENU_STATE->chosenDjinn[k] == selectedDjinn) {
            alreadySelected = 1;
            break;
          }
        }
        if (alreadySelected != 0) {
          BATTLE_MENU_MESSAGE_COPY((s32)MsgCannotUseSameDjinniTwice,
                                   messageText, BATTLE_MENU_MESSAGE_CAPACITY);
          if (previewWindow != NULL) {
            Window_Close(previewWindow, 1);
            previewWindow = NULL;
#if TLA_PATCH_4
            redrawList = 1;
#endif
            previewPage = 0;
          }
        } else if (selectedDjinn & BATTLE_DJINN_UNSET) {
          if (selectedDjinn & BATTLE_DJINN_TURNS) {
            MessageArgument_Push((selectedDjinn & BATTLE_DJINN_TURNS) >>
                                     BATTLE_DJINN_TURNS_SHIFT,
                                 MESSAGE_ARGUMENT_NUMBER);
            BATTLE_MENU_MESSAGE_COPY(
                BATTLE_DJINN_ELEMENT_OF(selectedDjinn) * DJINN_PER_ELEMENT +
                    DJINN_COMMAND_INDEX(selectedDjinn) + (s32)MsgDjinnEffect,
                messageText, BATTLE_MENU_MESSAGE_CAPACITY);
            if (previewWindow != NULL) {
              Window_Close(previewWindow, 1);
              previewWindow = NULL;
#if TLA_PATCH_4
              redrawList = 1;
#endif
              previewPage = 0;
            }
          } else {
            previewWindow = BattleDjinnMenu_ShowChangePreview(
                previewWindow, member, selectedDjinn, previewPage,
                &previewPages);
            Message_CopyExpanded((s32)MsgReadyToSummon, messageText,
                                 BATTLE_MENU_MESSAGE_CAPACITY);
            standbyElements = 1 << BATTLE_DJINN_ELEMENT_OF(selectedDjinn);
          }
        } else {
          previewWindow = BattleDjinnMenu_ShowChangePreview(
              previewWindow, member, selectedDjinn, previewPage, &previewPages);
          BATTLE_MENU_MESSAGE_COPY(
              BATTLE_DJINN_ELEMENT_OF(selectedDjinn) * DJINN_PER_ELEMENT +
                  DJINN_COMMAND_INDEX(selectedDjinn) + (s32)MsgDjinnEffect,
              messageText, BATTLE_MENU_MESSAGE_CAPACITY);
        }
      } else {
        BATTLE_MENU_MESSAGE_COPY((s32)MsgNoDjinn, messageText,
                                 BATTLE_MENU_MESSAGE_CAPACITY);
      }
      state->holdTilemapUpload = 0;
      if (previewPage == 0) {
        state->holdTilemapUpload = 1;
        Window_Close(messageWindow, 1);
        messageWindow =
            Window_Create(0, 4, 30, 4,
                          WINDOW_FLAG_INSTANT | WINDOW_FLAG_TEXT_CANVAS |
                              WINDOW_FLAG_BORDERED_CANVAS);
        state->holdTilemapUpload = previewPage;
      }
      Text_DrawWideStringAtOffset(messageText, messageWindow, 0, 4);
      drawnRow = selectedRow;
#if TLA_PATCH_1
      if (firstEntry != drawnFirstEntry || redrawList == 1) {
#else
      if (firstEntry != drawnFirstEntry) {
#endif
        WindowSprite_RedrawSavedRect(listWindow);
        j = 0;
        for (listEntry = firstEntry;
             (encodedDjinn = djinnEntries[listEntry]) != BATTLE_DJINN_END;
             listEntry++) {
          Window_SetTilemapEntry(listWindow,
                                 BATTLE_DJINN_ELEMENT_OF(encodedDjinn) +
                                     ELEMENT_ICON_TILE,
                                 0, j * 2, 0);
          if (encodedDjinn & BATTLE_DJINN_TURNS)
            Text_SetColor(TEXT_COLOR_YELLOW);
          else if (encodedDjinn & BATTLE_DJINN_UNSET)
            Text_SetColor(TEXT_COLOR_RED);
          Text_DrawMessageAsTiles(
              BATTLE_DJINN_ELEMENT_OF(encodedDjinn) * DJINN_PER_ELEMENT +
                  DJINN_COMMAND_INDEX(encodedDjinn) + (s32)MsgDjinnName,
              listWindow, 8, j * 16);
          if (encodedDjinn & BATTLE_DJINN_TURNS)
            Text_DrawNumberAtOffset((encodedDjinn & BATTLE_DJINN_TURNS) >>
                                        BATTLE_DJINN_TURNS_SHIFT,
                                    2, listWindow, DJINN_TURNS_X, j * 16);
          Text_SetColor(TEXT_COLOR_WHITE);
          j++;
          if (j > 4)
            break;
        }
        drawnFirstEntry = firstEntry;
#if TLA_PATCH_1
        redrawList = 0;
#endif
      }
      if (djinnCount > BATTLE_LIST_ROWS) {
        s32 listPages;

        for (j = 0; j < (listPages = (djinnCount + 4) / BATTLE_LIST_ROWS);
             j++) {
          s32 tile = j + PAGE_NUMBER_TILE;

          if (j == firstEntry / BATTLE_LIST_ROWS)
            tile = j + PAGE_NUMBER_LIT_TILE;
          Window_SetTilemapEntry(listWindow, tile,
                                 listWindow->width - listPages + j - 2, -1, 0);
        }
      }
      Window_HighlightRect((s16)listWindow->x + 1,
                           (s16)listWindow->y + selectedRow * 2 + 1,
                           listWindow->width - 2, 1, HIGHLIGHT_PALETTE);
      state->dirty = 1;
      state->holdTilemapUpload = 0;
    }

    if (djinnCount > BATTLE_LIST_ROWS) {
      for (j = 0; j < (djinnCount + 4) / BATTLE_LIST_ROWS; j++) {
        s32 tile = j + PAGE_NUMBER_TILE;

        if ((gInput.held & KEY_R) || (gFrameCount & 15) <= 11) {
          if (j == firstEntry / BATTLE_LIST_ROWS)
            tile = j + PAGE_NUMBER_LIT_TILE;
        }
        Window_SetTilemapEntry(listWindow, tile,
                               listWindow->width - (djinnCount + 4) / 5 + j - 2,
                               -1, 0);
      }
      if ((gInput.held & KEY_R) == 0) {
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
      WINDOW_MARK_TILEMAP_ROW(state, (u32)((s16)listWindow->y - 1));
    }

    pressed = gInput.pressed;
    repeated = gInput.repeat;
    held = gInput.held;
    navigation = BATTLE_MENU_STATE;
    if (navigation->tutorialStep != 0) {
      repeated = 0;
      pressed = 0;
      held = 0;
      if (navigation->tutorialDelay == 0) {
        navigation->tutorialDelay = 60;
        repeated = KEY_A;
        pressed = KEY_A;
      } else {
        navigation->tutorialDelay--;
      }
    }
    navigation = BATTLE_MENU_STATE;
    if (navigation->timer == 0 || (pressed & KEY_B)) {
      Sound_PlayFar(SE_MENU_CANCEL);
      encodedDjinn = -1;
      break;
    }
    if (pressed & KEY_A) {
      if (djinnCount != 0) {
        u32 selection = djinnEntries[firstEntry + selectedRow];
        u32 turns = selection & BATTLE_DJINN_TURNS;

        if (turns == 0) {
          if (alreadySelected == 0) {
            encodedDjinn = selection;
            navigation->page = firstEntry;
            navigation->row = selectedRow;
            navigation->preferredRow = preferredRow;
            break;
          }
        } else if (alreadySelected == 0) {
          Text_FillCanvasBackground();
          MessageArgument_ClearAll();
          MessageArgument_Push((turns & BATTLE_DJINN_TURNS) >>
                                   BATTLE_DJINN_TURNS_SHIFT,
                               MESSAGE_ARGUMENT_NUMBER);
          Message_CopyExpanded((s32)MsgDjinniStillRecovering, messageText,
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
            firstEntry + selectedRow == djinnCount)
          selectedRow = 0;
        preferredRow = selectedRow;
      } else if (repeated & KEY_UP) {
        Sound_PlayFar(SE_MENU_CURSOR_MOVE);
        selectedRow--;
        if (selectedRow < 0) {
          if (firstEntry == (djinnCount - 1) / 5 * BATTLE_LIST_ROWS)
            selectedRow = djinnCount - firstEntry - 1;
          else
            selectedRow = 4;
        }
        preferredRow = selectedRow;
      } else if (held & KEY_R) {
        if (previewWindow != NULL) {
          for (i = 0; i < previewPages; i++) {
            s32 tile = i + PAGE_NUMBER_TILE;

            if ((gFrameCount & 15) <= 11 && i == previewPage - 1)
              tile = previewPage + PAGE_NUMBER_LIT_TILE - 1;
            Window_SetTilemapEntry(previewWindow, tile,
                                   previewWindow->width - previewPages + i - 2,
                                   -1, 0);
          }
          Window_SetTilemapEntry(previewWindow, LIST_TILE_PAGES_LEFT,
                                 previewWindow->width - previewPages - 3, -1,
                                 0);
          Window_SetTilemapEntry(previewWindow, LIST_TILE_PAGES_RIGHT,
                                 previewWindow->width - 2, -1, 0);
          WINDOW_MARK_TILEMAP_ROW(state, (s16)previewWindow->y);
        }
        if (previewPage == 0 && previewPages != 0) {
          if (previewWindow != NULL)
            WindowSprite_ClearList(previewWindow);
          previewPage = 1;
          redraw = 1;
        } else {
          if (previewPage > previewPages)
            previewPage = previewPages;
          if (previewPage != 0) {
            if (repeated & KEY_RIGHT) {
              Sound_PlayFar(SE_MENU_CURSOR_MOVE);
              previewPage++;
              if (previewPage > previewPages)
                previewPage = 1;
              redraw = 1;
            } else if (repeated & KEY_LEFT) {
              Sound_PlayFar(SE_MENU_CURSOR_MOVE);
              previewPage--;
              if (previewPage <= 0)
                previewPage = previewPages;
              redraw = 1;
            }
          }
        }
      } else if (previewPage != 0) {
        if (previewWindow != NULL)
          WindowSprite_ClearList(previewWindow);
        previewPage = 0;
        redraw = 1;
#if TLA_PATCH_1
        redrawList = 1;
#endif
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
          if (firstEntry == (djinnCount - 1) / 5 * BATTLE_LIST_ROWS) {
            selectedRow = djinnCount - firstEntry - 1;
            if (selectedRow > preferredRow)
              selectedRow = preferredRow;
          }
        }
      } else if (repeated & KEY_LEFT) {
        Sound_PlayFar(SE_MENU_CURSOR_MOVE);
        Input_ResetKeyRepeat();
        if (firstEntry != 0) {
          firstEntry -= BATTLE_LIST_ROWS;
          selectedRow = preferredRow;
        } else {
          firstEntry = (djinnCount - 1) / 5 * BATTLE_LIST_ROWS;
          selectedRow = preferredRow;
          if (firstEntry != 0) {
            selectedRow = djinnCount - firstEntry - 1;
            if (selectedRow > preferredRow)
              selectedRow = preferredRow;
          }
        }
      }
    }

    MENU_PLACE_CURSOR(cursorPos, listWindow, selectedRow, 2, sprite);
    sprite->oam.index = VramCache_Reload(cursorSlot, gPointingHandTiles);
    sprite->oam.x = cursorPos.x + ((gFrameCount & 4) >> 1) - 6;
    sprite->oam.y = cursorPos.y - ((gFrameCount & 4) >> 2) - 8;
    if (djinnCount != 0)
      Oam_AddSpriteAtDepth(sprite, OAM_DEPTH_WINDOW + 2);

    partyStatus = Ram_HeapSlots->partyStatus;
    blinkPhase = gFrameCount & 4;
    iconWindow = (struct Window *)partyStatus->window;
    if (partyStatus->flags & 2) {
      for (i = 0; i <= 3; i++) {
        // HIGHLIGHT_PALETTE, one below, while the blink is on.
        s32 palette = WINDOW_PALETTE - (blinkPhase != 0);

        if (((1 << i) & standbyElements) == 0)
          palette = WINDOW_PALETTE;
        Window_HighlightRect(
            (s16)iconWindow->x + gDjinnMenu_ElementXOffsets[i] + 1,
            (s16)iconWindow->y + gDjinnMenu_ElementYOffsets[i] + 1, 2, 1,
            palette);
      }
    }
    if (gFrameCount & 4) {
      Iwram_CopyWordsEntry((void *)DJINN_MENU_ARROW_VRAM, gDjinnMenu_BlinkTile,
                           TILE_SIZE_4BPP);
      Iwram_CopyWordsEntry((void *)(DJINN_MENU_ARROW_VRAM + TILE_SIZE_4BPP),
                           gDjinnMenu_BlinkTile, TILE_SIZE_4BPP);
    } else {
      Iwram_FillWordsEntry((void *)DJINN_MENU_ARROW_VRAM, TILE_SIZE_4BPP,
                           TEXT_CANVAS_BACKGROUND_WORD);
      Iwram_CopyWordsEntry((void *)(DJINN_MENU_ARROW_VRAM + TILE_SIZE_4BPP),
                           gDjinnMenu_BlinkTile, TILE_SIZE_4BPP);
    }
    wait(1);
  }

  {
    struct PartyStatusWindow *owner = Ram_HeapSlots->partyStatus;

    if (owner->flags & 2)
      Window_HighlightRect((s16)owner->window->x + 1, (s16)owner->window->y + 1,
                           4, 4, WINDOW_PALETTE);
  }
  VramCache_FreeEntry(cursorSlot);
  Window_Close(messageWindow, 1);
  Window_Close(previewWindow, 1);
  Window_Close(listWindow, 1);
  Window_MarkVisibleTextTiles();
  Text_UseExtraTiles(0);
  Heap_Release(djinnEntries);
  wait(1);
  return encodedDjinn;
}

#undef DJINN_LIST_X
#undef DJINN_LIST_WIDTH
#undef DJINN_TURNS_X
