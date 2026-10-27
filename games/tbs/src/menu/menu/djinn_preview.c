/// @file
/// Previewing a Djinn change before it is made, the chosen party member's
/// status, and the checks that keep the party members' Djinn counts even.
// Part of menu.c.

#include "../menu_internal.h"

/// Previews a give, trade, set or standby change without committing it.
/// It saves and restores the units before polling for confirmation.
///
/// mode is a DjinnCompareOperation. Two 15 by 15 windows show the change and
/// the top window asks "Is this OK?". For a give or trade R cycles three
/// pages: both party members' stats (the first), then the giver's and then the
/// receiver's stats beside their Psynergy list; Left and Right turn the
/// Psynergy list's pages. A set or standby shows the party member's stats and
/// Psynergy. Returns DJINN_RESULT_CONFIRMED for A, DJINN_RESULT_BACK for B and
/// DJINN_RESULT_START for Start; the caller makes the change. The page counts
/// are measured on the real units, which are put back from a copy at once.
s32 PsynergyList_RunCompareLoop(u32 mode) {
  struct DjinnMenuWork *menu;
  struct DjinnMenuBackdrop *backdrop;
  struct WindowWork *work;
  struct Window *list;
  s32 pageCounts[2];
  s32 selectedPages[2];
  s32 confirmationChoice;
  u32 redraw;
  s32 frame;
  s32 page;
  s32 previewMode;
  s32 addPairSlot;
  s32 step;
  s32 row;
  s32 result;
  s32 zero;
  s32 index;
  s32 memberIndex;

  menu = gMenuWork;
  confirmationChoice = YES;
  result = 0;
  redraw = 1;
  frame = 0;
  selectedPages[0] = 0;
  page = DJINN_COMPARE_PAGE_BOTH;
  menu->cursor->motion = ICON_HIDDEN;
  selectedPages[1] = 0;

  // The party figures go below the screen (y 200), except the two party members
  // concerned: the first over the left window, the second over the right.
  for (memberIndex = 3; memberIndex >= 0; memberIndex--) {
    menu->memberY[memberIndex] = 0xc8;
  }
  menu->memberX[menu->selectedColumn[1]] = 0x78;
  menu->memberY[menu->selectedColumn[1]] = 0x20;
  menu->memberX[menu->selectedColumn[0]] = 0x10;
  menu->memberY[menu->selectedColumn[0]] = 0x20;

  WindowSprite_ClearListFar(menu->listWindow);
  WaitFrames(1);
  Window_CloseIfOpen(&menu->helpWindow, 1);
  Window_UpdateOrCreate(&menu->helpWindow, 0, 0, 30, 5, 2);

  pageCounts[0] = 1;
  pageCounts[1] = 1;

  // Measure the action-list pages using temporary unit mutations.
  if (mode <= DJINN_COMPARE_GIVE) {
    u16 *diffWork;
    struct Unit *originalUnit;
    struct Unit *unit;
    s32 memberId;
    s32 firstElement;
    s32 firstIndex;
    s32 secondElement;
    s32 secondIndex;
    s32 firstWasSet;
    s32 secondWasSet;
    s32 lostCount;
    s32 gainedCount;
    s32 size;

    diffWork = AllocEwram(96);
    size = sizeof(*originalUnit);
    originalUnit = AllocEwram(size);
    secondElement = 0;
    secondIndex = 0;
    memberId = menu->shownMember[0];
    firstElement = menu->pairElement[0];
    firstIndex = menu->pairIndex[0] & DJINN_ENTRY_INDEX;
    firstWasSet = (u16)(menu->pairEntries[0] & DJINN_ENTRY_IS_SET);

    unit = Unit_GetFar(memberId);
    Iwram_CopyWords(originalUnit, unit, size);
    Djinn_PutOnStandbyFar(memberId, firstElement, firstIndex);
    if (mode == DJINN_COMPARE_TRADE) {
      secondElement = menu->pairElement[1];
      secondIndex = menu->pairIndex[1] & DJINN_ENTRY_INDEX;
      secondWasSet = (u16)(menu->pairEntries[1] & DJINN_ENTRY_IS_SET);
      Djinn_GiveFar(memberId, secondElement, secondIndex);
      if (secondWasSet != 0) {
        Djinn_SetFar(memberId, secondElement, secondIndex);
      }
    }
    Unit_CalculateFar(memberId);
    pageCounts[0] = PsynergyList_Diff(originalUnit->psynergy, unit->psynergy,
                                      diffWork, &gainedCount, &lostCount);
    Iwram_CopyWords(unit, originalUnit, size);

    memberId = menu->shownMember[1];
    unit = Unit_GetFar(memberId);
    Iwram_CopyWords(originalUnit, unit, size);
    if (mode == DJINN_COMPARE_TRADE) {
      Djinn_PutOnStandbyFar(memberId, secondElement, secondIndex);
    }
    Djinn_GiveFar(memberId, firstElement, firstIndex);
    if (firstWasSet != 0) {
      Djinn_SetFar(memberId, firstElement, firstIndex);
    }
    Unit_CalculateFar(memberId);
    pageCounts[1] = PsynergyList_Diff(originalUnit->psynergy, unit->psynergy,
                                      diffWork, &gainedCount, &lostCount);
    Iwram_CopyWords(unit, originalUnit, size);
    Free(originalUnit);
    Free(diffWork);
  } else if (mode == DJINN_COMPARE_STANDBY || mode == DJINN_COMPARE_SET) {
    u16 *diffWork;
    struct Unit *originalUnit;
    struct Unit *unit;
    s32 firstMemberId;
    s32 firstElement;
    s32 firstIndex;
    s32 applySecond;
    s32 lostCount;
    s32 gainedCount;

    diffWork = AllocEwram(96);
    originalUnit = AllocEwram(sizeof(*originalUnit));
    firstMemberId = menu->shownMember[0];
    firstElement = menu->pairElement[0];
    firstIndex = menu->pairIndex[0] & DJINN_ENTRY_INDEX;
    applySecond = (mode == DJINN_COMPARE_SET);

    unit = Unit_GetFar(firstMemberId);
    Iwram_CopyWords(originalUnit, unit, sizeof(*originalUnit));
    Djinn_PutOnStandbyFar(firstMemberId, firstElement, firstIndex);
    if (applySecond != 0) {
      Djinn_SetFar(firstMemberId, firstElement, firstIndex);
    }
    Unit_CalculateFar(firstMemberId);
    pageCounts[0] = PsynergyList_Diff(originalUnit->psynergy, unit->psynergy,
                                      diffWork, &gainedCount, &lostCount);
    Iwram_CopyWords(unit, originalUnit, sizeof(*originalUnit));
    pageCounts[1] = pageCounts[0];
    page = DJINN_COMPARE_PAGE_GIVER;
    Free(originalUnit);
    Free(diffWork);
  }

  // Psynergy entries to pages of five.
  pageCounts[0] = ((pageCounts[0] - 1) / 5) + 1;
  if (pageCounts[0] == 0) {
    pageCounts[0] = 1;
  }
  pageCounts[1] = ((pageCounts[1] - 1) / 5) + 1;
  if (pageCounts[1] == 0) {
    pageCounts[1] = 1;
  }

  // Open the paired sides only after both units have been restored.
  Window_UpdateOrCreate(&menu->window, 0, 5, 15, 15, 2);
  Window_UpdateOrCreate(&menu->secondWindow, 15, 5, 15, 15, 2);
  WindowSprite_RedrawSavedRectFar(menu->helpWindow);

  if (mode == DJINN_COMPARE_STANDBY) {
    Text_DrawMessageAsTilesFar(MsgDjinnCompareStandby, menu->helpWindow, 96, 0);
    Text_DrawMessageAsTilesFar(MsgDjinnCompareConfirm, menu->helpWindow, 96,
                               16);
  } else if (mode == DJINN_COMPARE_SET) {
    Text_DrawMessageAsTilesFar(MsgDjinnCompareSet, menu->helpWindow, 96, 0);
    Text_DrawMessageAsTilesFar(MsgDjinnCompareConfirm, menu->helpWindow, 96,
                               16);
  } else if (mode == DJINN_COMPARE_TRADE) {
    Text_DrawMessageAsTilesFar(MsgDjinnCompareTrade, menu->helpWindow, 128, 0);
    Text_DrawMessageAsTilesFar(MsgDjinnCompareConfirm, menu->helpWindow, 128,
                               8);
  } else {
    Text_DrawMessageAsTilesFar(MsgDjinnCompareGive, menu->helpWindow, 128, 0);
    Text_DrawMessageAsTilesFar(MsgDjinnCompareConfirm, menu->helpWindow, 128,
                               8);
  }

  // The Djinni concerned, after its element icon (BASIC_COLORS_PALETTE): in
  // TEXT_COLOR_RED when it is not set. A trade adds the other Djinni below it;
  // a set or standby shows the same Djinni again below in its new colour, with
  // tile 0x296 of WINDOW_PALETTE between the two.
  row = (mode == DJINN_COMPARE_GIVE);
  if ((menu->pairEntries[0] & DJINN_ENTRY_IS_SET) == 0) {
    Text_SetColorFar(TEXT_COLOR_RED);
  }
  Text_DrawMessageAsTilesFar(
      DJINN_ELEMENT(menu->pairEntries[0]) * DJINN_PER_ELEMENT +
          DJINN_NUMBER(menu->pairEntries[0]) + MsgDjinnName,
      menu->helpWindow, 48, row * 8);
  Window_SetTilemapEntryFar(menu->helpWindow,
                            DJINN_ELEMENT(menu->pairEntries[0]) +
                                DJINN_ELEMENT_TILE,
                            5, row, zero = 0);
  Text_SetColorFar(TEXT_COLOR_NORMAL);

  if (mode == DJINN_COMPARE_TRADE) {
    if ((menu->pairEntries[1] & DJINN_ENTRY_IS_SET) == 0) {
      Text_SetColorFar(TEXT_COLOR_RED);
    }
    Text_DrawMessageAsTilesFar(
        DJINN_ELEMENT(menu->pairEntries[1]) * DJINN_PER_ELEMENT +
            DJINN_NUMBER(menu->pairEntries[1]) + MsgDjinnName,
        menu->helpWindow, 48, 16);
    Window_SetTilemapEntryFar(
        menu->helpWindow,
        DJINN_ELEMENT(menu->pairEntries[1]) + DJINN_ELEMENT_TILE, 5, 2, mode);
    Text_SetColorFar(TEXT_COLOR_NORMAL);
  } else if (mode - 2 <= 1) {
    if ((menu->pairEntries[0] & DJINN_ENTRY_IS_SET) != 0) {
      Text_SetColorFar(TEXT_COLOR_RED);
    }
    Text_DrawMessageAsTilesFar(
        DJINN_ELEMENT(menu->pairEntries[0]) * DJINN_PER_ELEMENT +
            DJINN_NUMBER(menu->pairEntries[0]) + MsgDjinnName,
        menu->helpWindow, 48, 16);
    Window_SetTilemapEntryFar(
        menu->helpWindow,
        DJINN_ELEMENT(menu->pairEntries[0]) + DJINN_ELEMENT_TILE, 5, 2, zero);
    Text_SetColorFar(TEXT_COLOR_NORMAL);
    Window_SetTilemapEntryFar(menu->helpWindow, 0xf296, 7, 1, zero);
  }

  for (;;) {
    s32 pressedKeys;
    s32 repeatedKeys;

    work = (struct WindowWork *)gWindowWork[0];
    backdrop = menu->backdrop;
    pressedKeys = gKeysPressed;
    repeatedKeys = gKeysRepeat;

    // The tutorial presses A for the player after 100 frames, or shows
    // its messages; around each message the first 0x200 bytes of the window
    // work (the tilemap's top eight rows) are saved and put back.
    if (backdrop->tutorialStep != 0) {
      s16 *saved;
      s32 size;

      size = 0x200;
      saved = AllocEwram(size);
      pressedKeys = 0;
      repeatedKeys = 0;
      backdrop->tutorialTimer += 1;

      switch (backdrop->tutorialStep) {
      case 5:
      case 12:
      case 23:
        if (backdrop->tutorialTimer == 100) {
          repeatedKeys = KEY_A;
          pressedKeys = KEY_A;
          backdrop->tutorialTimer = 0;
          backdrop->tutorialStep += 1;
        }
        break;
      case 10:
        if (backdrop->tutorialTimer == 60) {
          struct Window *window;

          Iwram_CopyWords(saved, work, size);
          window = Message_OpenWindowFar(MsgDjinnTutorialSetChanges, 8, 0, 1);
          /* FAKEMATCH(one-pass): the cursor's store in a one-pass block, which
           * keeps the window copy before it as the ROM does; as a plain
           * statement, 11 of 1045 instructions differ in tbs-en. */
          do {
            menu->cursor->motion = ICON_SHOWN;
          } while (0);
          MenuCursor_Slide(2, 96);
          gCommon.messageSpeed = MESSAGE_SPEED_NORMAL;
          while (TextPrinter_AllFinishedFar() == 0) {
            WaitFrames(1);
          }
          Window_CloseFar(window, 1);
          Iwram_CopyWords(work, saved, size);
          Window_MarkVisibleTextTilesFar();
          work->dirty = 1;
          redraw = 1;
          backdrop->tutorialTimer = 0;
          backdrop->tutorialStep = 11;
          /* FAKEMATCH(one-pass): the cursor's store in a one-pass block, which
           * keeps the step's store before it as the ROM does; as a plain
           * statement, 6 of 1045 instructions differ in tbs-en. */
          do {
            menu->cursor->motion = ICON_HIDDEN;
          } while (0);
        }
        break;
      case 11:
        if (backdrop->tutorialTimer == 60) {
          struct Window *window;

          Iwram_CopyWords(saved, work, size);
          window =
              Message_OpenWindowFar(MsgDjinnTutorialClassPsynergy, 8, 0, 1);
          /* FAKEMATCH(one-pass): the cursor's store in a one-pass block, which
           * keeps the window copy before it as the ROM does; as a plain
           * statement, 13 of 1045 instructions differ in tbs-en. */
          do {
            menu->cursor->motion = ICON_SHOWN;
          } while (0);
          MenuCursor_Slide(106, 56);
          gCommon.messageSpeed = MESSAGE_SPEED_NORMAL;
          while (TextPrinter_AllFinishedFar() == 0) {
            WaitFrames(1);
          }
          Window_CloseFar(window, 1);
          Iwram_CopyWords(work, saved, size);
          Window_MarkVisibleTextTilesFar();
          work->dirty = 1;
          WaitFrames(1);
          redraw = 1;
          backdrop->tutorialTimer = 0;
          backdrop->tutorialStep = 12;
          /* FAKEMATCH(one-pass): the cursor's store in a one-pass block, which
           * keeps the step's store before it as the ROM does; as a plain
           * statement, 5 of 1045 instructions differ in tbs-en. */
          do {
            menu->cursor->motion = ICON_HIDDEN;
          } while (0);
        }
        break;
      default:
        break;
      }
      Free(saved);
    }

    // The help names what R shows next: `R: <giver>'s Psy` on
    // DJINN_COMPARE_PAGE_BOTH, the receiver's on DJINN_COMPARE_PAGE_GIVER,
    // "R: Status" on DJINN_COMPARE_PAGE_RECEIVER.
    if (redraw != 0) {
      addPairSlot = 1;
      if (mode <= DJINN_COMPARE_GIVE) {
        Window_ClearInteriorTilesFar(menu->helpWindow, 128, 16, 224, 24);
        if (page == DJINN_COMPARE_PAGE_RECEIVER) {
          Text_DrawMessageAsTilesFar(MsgDjinnStatusHelp, menu->helpWindow, 128,
                                     16);
        } else {
          MessageArgument_PushFar(
              menu->shownMember[page != DJINN_COMPARE_PAGE_BOTH],
              MESSAGE_ARGUMENT_ACTOR);
          Text_DrawMessageAsTilesFar(MsgDjinnPsyHelp, menu->helpWindow, 128,
                                     16);
        }
      }
      work->holdTilemapUpload = 1;
      WindowSprite_PrepareForRedrawFar(menu->window);
      WindowSprite_PrepareForRedrawFar(menu->secondWindow);
      if ((redraw >> 1) != 0) {
        WaitFrames(1);
      }

      if (mode == DJINN_COMPARE_SET) {
        previewMode = DJINN_PREVIEW_ADD_AND_SET;
        addPairSlot = 0;
      } else if (mode == DJINN_COMPARE_STANDBY) {
        previewMode = DJINN_PREVIEW_UNSET;
        addPairSlot = 0;
      } else {
        previewMode = DJINN_PREVIEW_TRADE;
      }

      switch (page) {
      default:
        DjinnMenu_DrawPairPreview(mode, 0);
        break;
      case DJINN_COMPARE_PAGE_RECEIVER:
        if (mode == DJINN_COMPARE_GIVE) {
          previewMode = DJINN_PREVIEW_ADD;
        }
        DjinnMenu_DrawStatPreview(menu->window, 0, 0, menu->shownMember[1],
                                  page, 0, previewMode, 0, 0);
        DjinnMenu_DrawStatPreview(menu->secondWindow, 0, 0,
                                  menu->shownMember[1], page, 0, previewMode,
                                  selectedPages[1] + 1, 0);
        break;
      case DJINN_COMPARE_PAGE_GIVER:
        if (mode == DJINN_COMPARE_GIVE) {
          previewMode = DJINN_PREVIEW_UNSET;
          addPairSlot = 0;
        }
        DjinnMenu_DrawStatPreview(menu->window, 0, 0, menu->pairMember[0], page,
                                  addPairSlot, previewMode, page, 1);
        DjinnMenu_DrawStatPreview(menu->secondWindow, 0, 0, menu->pairMember[0],
                                  page, addPairSlot, previewMode,
                                  selectedPages[0] + 1, 1);
        break;
      }
      work->holdTilemapUpload = 0;
    }

    // The Psynergy list's page numbers on the right window's top border:
    // 0xf031 + n is the window font's digit n + 1 in WINDOW_PALETTE ('0' past
    // the tenth page); less 0x1000, HIGHLIGHT_PALETTE for the page shown.
    if (page <= DJINN_COMPARE_PAGE_RECEIVER) {
      list = menu->secondWindow;
      if (pageCounts[page] > 1) {
        s32 top;

        for (index = 0; index < pageCounts[page]; index++) {
          s32 tile;

          tile = 0xf031 + index;
          if (index > 9) {
            tile = 0xf030;
          }
          if (index == selectedPages[page]) {
            tile -= 0x1000;
          }
          Window_SetTilemapEntryFar(
              list, tile, list->width - pageCounts[page] + index - 2, -1, 0);
        }
        top = -1;
        Window_SetTilemapEntryFar(list, PAGE_ARROW_LEFT,
                                  list->width - pageCounts[page] - 3, top, 0);
        Window_SetTilemapEntryFar(list, PAGE_ARROW_RIGHT, list->width - 2, top,
                                  0);
        Window_MarkTilemapRow(work, list->y);
      }
    }

    // The Djinni figures, at a fixed 3/4 size (phase -0xc000), move once a
    // second: step runs 0 to 29 over frames 5 to 34 of each 60 and holds.
    // A given Djinni slides right 2 pixels a step from x 34 while hopping up
    // to 6 pixels (half a sine turn over the 30 steps); in a trade the other
    // slides left from x 99 the same way, mirrored.
    frame++;
    step = (frame % 60) - 5;
    if (step < 0) {
      step = 0;
    }
    if (step > 29) {
      step = 29;
    }
    DjinnFigures_SetSlotPhase(0, 0xffff4000);
    DjinnFigures_SetSlotPhase(1, 0xffff4000);

    if (mode <= DJINN_COMPARE_GIVE) {
      s32 rise;
      s32 slide;

      rise = -Iwram_MulQ16(16, Trig_Sin((step % 30) * 0x444));
      if (rise < -6) {
        rise = -6;
      }
      if (rise > 12) {
        rise = 12;
      }
      slide = (step % 35) * 2;
      DjinnFigures_SetSlotPosition(0, slide + 34, rise + 20, 0);
      if (mode == DJINN_COMPARE_TRADE) {
        DjinnFigures_SetSlotPosition(1, 99 - slide, 36 - rise, 0);
      }
    } else {
      DjinnFigures_SetSlotPosition(0, 32, 30, 0);
    }

    // No key moves confirmationChoice: the cursor stays on the first answer,
    // so A always confirms and B cancels.
    if (redraw != 0) {
      redraw = 0;
      confirmationChoice = Menu_WrapIndex(confirmationChoice, 2);
    }
    MenuCursor_Position(confirmationChoice * 56 + 80, 16);

    // Blink the backdrop's frame tile: 0x060052c0 is tile 150 of BG character
    // block 1 (DJINN_BACKDROP_FRAME_TILE), drawn and filled with colour 4 in
    // turn every 4 frames.
    if ((frame & 3) == 0) {
      void *tile;
      u32 fill;

      tile = (void *)0x060052c0;
      fill = TEXT_CANVAS_BACKGROUND_WORD;
      if ((frame & 4) != 0) {
        Iwram_CopyWords(tile, gMenu_BackdropFrameTile, 32);
      } else {
        Iwram_FillWords(tile, 32, fill);
      }
    }

    if ((pressedKeys & KEY_A) != 0) {
      if (confirmationChoice == YES) {
        result = DJINN_RESULT_CONFIRMED;
        break;
      }
      Sound_PlaySongFar(SE_MENU_CANCEL);
      result = DJINN_RESULT_BACK;
      break;
    }
    if ((pressedKeys & KEY_START) != 0) {
      Sound_PlaySongFar(SE_MENU_CANCEL);
      result = DJINN_RESULT_START;
      break;
    } else if ((pressedKeys & KEY_B) != 0) {
      Sound_PlaySongFar(SE_MENU_CANCEL);
      result = DJINN_RESULT_BACK;
      break;
    } else if ((pressedKeys & KEY_R) != 0) {
      if (mode <= DJINN_COMPARE_GIVE) {
        page = ((page + 1) % 3);
        redraw = 2;
        Sound_PlaySongFar(SE_MENU_CURSOR_MOVE);
      }
    } else if ((repeatedKeys & KEY_LEFT) != 0) {
      if (page <= DJINN_COMPARE_PAGE_RECEIVER) {
        selectedPages[page]--;
        selectedPages[page] =
            Menu_WrapIndex(selectedPages[page], pageCounts[page]);
        Sound_PlaySongFar(SE_MENU_CURSOR_MOVE);
        Input_ResetKeyRepeat();
        redraw = 1;
      }
    } else if ((repeatedKeys & KEY_RIGHT) != 0) {
      if (page <= DJINN_COMPARE_PAGE_RECEIVER) {
        Sound_PlaySongFar(SE_MENU_CURSOR_MOVE);
        Input_ResetKeyRepeat();
        selectedPages[page]++;
        redraw = 1;
        selectedPages[page] =
            Menu_WrapIndex(selectedPages[page], pageCounts[page]);
      }
    }
    WaitFrames(1);
  }

  // Restore the ordinary list drawing tasks and close the preview sides; the
  // figures grow in again from phase 0.
  DjinnFigures_SetSlotPhase(0, 0);
  DjinnFigures_SetSlotPhase(1, 0);
  Task_Add(Menu_UpdateEntryActorTransforms, TASK_PASS_FRAME);

  ((struct WindowWork *)gWindowWork[0])->holdTilemapUpload = 1;
  Window_CloseIfOpen(&menu->helpWindow, 1);
  WaitFrames(1);
  Window_UpdateOrCreate(&menu->helpWindow, 13, 0, 17, 5, 2);
  Window_CloseIfOpen((struct Window **)&menu->window, 1);
  Window_CloseIfOpen(&menu->secondWindow, 1);
  WindowSprite_RedrawSavedRectFar(menu->listWindow);
  // BUG: no window is ever made in unopenedWindow (DjinnMenuWork), so this
  // redraws through NULL; see the field. BUGFIX skips it.
#ifndef BUGFIX
  WindowSprite_RedrawSavedRectFar((struct Window *)menu->unopenedWindow);
#endif
  WindowSprite_RedrawSavedRectFar(menu->statusWindow);
  ((struct WindowWork *)gWindowWork[0])->holdTilemapUpload = 0;
  WaitFrames(1);
  return result;
}

/// Shows the chosen party member's current stats and paged Psynergy list.
/// Opened by L on the Djinn list ("Char. Status"): the party member's stats on
/// the left, their Psynergy six to a page on the right, which Left and Right
/// turn. Returns DJINN_RESULT_BACK for A, B, L or R, and DJINN_RESULT_START for
/// Start, which closes the Djinn menu.
s32 DjinnMenu_ShowMemberStatus(void) {
  s32 selectedPages[2];
  s32 pageCounts[2];
  s32 gained;
  s32 lost;
  s32 phase;
  s32 redraw;
  s32 frame;
  struct Window **leftWindowSlot;
  struct Window **rightWindowSlot;
  struct WindowWork *work;
  u32 pressedKeys;
  u32 repeatedKeys;
  struct DjinnMenuWork *menu;
  struct Unit *unit;
  struct Window *window;
  u16 *listedActions;
  void *saved;
  s32 result;
  s32 tile;
  s32 pageIndex;
  s32 side;
  s32 *counts;
  s32 blink;

  menu = gMenuWork;
  phase = 0;
  redraw = 1;
  result = 0;
  frame = 0;
  menu->cursor->motion = ICON_HIDDEN;
  selectedPages[1] = selectedPages[0] = result;
  for (side = 3; side >= 0; side--)
    menu->memberY[side] = 200;
  WindowSprite_ClearListFar(menu->listWindow);
  WaitFrames(1);
  // Count the party member's Psynergy by diffing their list with itself; saved
  // is taken and given back unused.
  counts = pageCounts;
  counts[1] = counts[0] = 1;
  listedActions = AllocEwram(96);
  saved = AllocEwram(332);
  unit = Unit_GetFar(menu->shownMember[0]);
  counts[1] = counts[0] = PsynergyList_Diff(unit->psynergy, unit->psynergy,
                                            listedActions, &gained, &lost);
  Free(saved);
  Free(listedActions);
  counts[0] = (counts[0] - 1) / 6 + 1;
  if (counts[0] == 0)
    counts[0] = 1;
  counts[1] = (counts[1] - 1) / 6 + 1;
  if (counts[1] == 0)
    counts[1] = 1;
  leftWindowSlot = (struct Window **)&menu->window;
  Window_UpdateOrCreate(leftWindowSlot, 0, 5, 15, 15, 2);
  rightWindowSlot = &menu->secondWindow;
  Window_UpdateOrCreate(rightWindowSlot, 15, 5, 15, 15, 2);
  WindowSprite_RedrawSavedRectFar(menu->helpWindow);
  WindowSprite_RedrawSavedRectFar(menu->statusWindow);
  Text_DrawMessageAsTilesFar(MsgCurrentStatus, menu->statusWindow, 0, 0);
  Text_DrawMessageAsTilesFar(MsgCurrentStatus + 2, menu->statusWindow, 0, 16);
  side = 0;
  while (1) {
    work = (struct WindowWork *)gWindowWork[0];
    pressedKeys = gKeysPressed;
    repeatedKeys = gKeysRepeat;
    if (redraw) {
      work->holdTilemapUpload = 1;
      WindowSprite_PrepareForRedrawFar(menu->window);
      WindowSprite_PrepareForRedrawFar(menu->secondWindow);
      DjinnMenu_DrawStatPreview(menu->window, 0, 0, menu->pairMember[0], 0, 0,
                                DJINN_PREVIEW_NO_CHANGE, 0, 1);
      DjinnMenu_DrawStatPreview(menu->secondWindow, 0, 0, menu->pairMember[0],
                                0, 0, DJINN_PREVIEW_NO_CHANGE,
                                selectedPages[0] + 1, 1);
      work->holdTilemapUpload = 0;
    }
    if (counts[side] > 1) {
      window = menu->secondWindow;
      for (pageIndex = 0; pageIndex < counts[side]; pageIndex++) {
        tile = 0xf031 + pageIndex;
        if (pageIndex > 9)
          tile = 0xf030;
        if (pageIndex == selectedPages[side])
          tile -= 0x1000;
        Window_SetTilemapEntryFar(
            window, tile, window->width - counts[side] + pageIndex - 2, -1, 0);
      }
      Window_SetTilemapEntryFar(window, PAGE_ARROW_LEFT,
                                window->width - counts[side] - 3, -1, 0);
      Window_SetTilemapEntryFar(window, PAGE_ARROW_RIGHT, window->width - 2, -1,
                                0);
      Window_MarkTilemapRow(work, window->y);
    }
    // blink is worked out as the preview's step is, and not used; the Djinni
    // figure stays below the screen.
    frame++;
    blink = frame % 60 - 5;
    CLAMP(blink, 0, 29);
    DjinnFigures_SetSlotPosition(0, 32, 200, 0);
    if (redraw) {
      redraw = 0;
      phase = Menu_WrapIndex(phase, 2);
    }
    if ((frame & 3) == 0) {
      if (frame & 4)
        Iwram_CopyWords((void *)0x060052c0, gMenu_BackdropFrameTile, 32);
      else
        Iwram_FillWords((void *)0x060052c0, 32, TEXT_CANVAS_BACKGROUND_WORD);
    }
    if (pressedKeys & KEY_START) {
      Sound_PlaySongFar(SE_MENU_CANCEL);
      result = DJINN_RESULT_START;
      break;
    }
    if (pressedKeys & (KEY_A | KEY_B | KEY_R | KEY_L)) {
      Sound_PlaySongFar(SE_MENU_CANCEL);
      result = DJINN_RESULT_BACK;
      break;
    }
    if (repeatedKeys & KEY_LEFT) {
      selectedPages[side]--;
      selectedPages[side] = Menu_WrapIndex(selectedPages[side], counts[side]);
      Sound_PlaySongFar(SE_MENU_CURSOR_MOVE);
      Input_ResetKeyRepeat();
      redraw = 1;
    } else if (repeatedKeys & KEY_RIGHT) {
      selectedPages[side]++;
      Sound_PlaySongFar(SE_MENU_CURSOR_MOVE);
      Input_ResetKeyRepeat();
      redraw = 1;
      selectedPages[side] = Menu_WrapIndex(selectedPages[side], counts[side]);
    }
    WaitFrames(1);
  }
  Task_Add(Menu_UpdateEntryActorTransforms, TASK_PASS_FRAME);
  ((struct WindowWork *)gWindowWork[0])->holdTilemapUpload = 1;
  Window_CloseIfOpen(&menu->helpWindow, 1);
  WaitFrames(1);
  Window_UpdateOrCreate(&menu->helpWindow, 13, 0, 17, 5, 2);
  Window_CloseIfOpen(leftWindowSlot, 1);
  Window_CloseIfOpen(rightWindowSlot, 1);
  WindowSprite_RedrawSavedRectFar(menu->listWindow);
  // BUG: no window is ever made in unopenedWindow (DjinnMenuWork), so this
  // redraws through NULL; see the field. BUGFIX skips it.
#ifndef BUGFIX
  WindowSprite_RedrawSavedRectFar((struct Window *)menu->unopenedWindow);
#endif
  WindowSprite_RedrawSavedRectFar(menu->statusWindow);
  ((struct WindowWork *)gWindowWork[0])->holdTilemapUpload = 0;
  WaitFrames(1);
  return result;
}

/// Marks recipients that would leave unequal Djinn totals after a direct give.
/// Returns how many recipients would need a trade instead.
///
/// requiresTrade[i] becomes 1 for each other party member i who cannot take one
/// of sourceMemberColumn's Djinn without two party members' totals differing by
/// more than one (Djinn_CheckTurnBalance), else 0. The second Djinn chooser
/// offers such a party member only Trade.
s32 Djinn_MarkBalancedEntries(s8 *requiresTrade, s32 sourceMemberColumn) {
  s32 tradeCount;
  s32 memberColumn;
  s8 *entry;
  struct DjinnMenuWork *menu;

  menu = gMenuWork;
  tradeCount = 0;
  memberColumn = 0;
  if (tradeCount < menu->memberCount) {
    entry = requiresTrade;
    do {
      *entry = 0;
      if (memberColumn != sourceMemberColumn) {
        if (Djinn_CheckTurnBalance(sourceMemberColumn, memberColumn) == 0) {
          *entry = 1;
          tradeCount += 1;
        }
      }
      memberColumn += 1;
      entry += 1;
    } while (memberColumn < menu->memberCount);
  }
  return tradeCount;
}

/// Tests whether a hypothetical give leaves every pair of party members within
/// one Djinn. Returns 1 when, with one Djinni moved from sourceMemberColumn to
/// targetMemberColumn, no two party members' totals (Djinn_CountTurns) differ
/// by more than one; 0 otherwise.
s32 Djinn_CheckTurnBalance(s32 sourceMemberColumn, s32 targetMemberColumn) {
  struct DjinnMenuWork *menu;
  s8 ownedCounts[16];
  u8 firstMember;
  u8 secondMember;
  s32 difference;
  s32 balanced;

  menu = gMenuWork;
  Djinn_CountTurns((u8 *)ownedCounts);
  ownedCounts[sourceMemberColumn] -= 1;
  ownedCounts[targetMemberColumn] += 1;
  balanced = 1;
  for (firstMember = 0; firstMember < menu->memberCount; firstMember++) {
    secondMember = firstMember;
    if (firstMember < menu->memberCount) {
      while (1) {
        secondMember++;
        if (secondMember >= menu->memberCount)
          break;
        difference = ownedCounts[firstMember] - ownedCounts[secondMember];
        if (difference < -1 || difference > 1) {
          balanced = 0;
          break;
        }
      }
    }
  }
  return balanced;
}

/// Counts each party member's Djinn across its set and available bitmasks:
/// writes every party member's total, set or not, into ownedCounts by list
/// column.
void Djinn_CountTurns(u8 *ownedCounts) {
  struct DjinnMenuWork *menu;
  struct Unit *unit;
  s32 memberIndex;
  s32 element;
  s32 djinnIndex;
  s32 ownedCount;

  menu = gMenuWork;
  for (memberIndex = 0; memberIndex < menu->memberCount; memberIndex++) {
    unit = Unit_GetFar(menu->members[memberIndex]);
    ownedCount = 0;
    for (element = 0; element <= 3; element++) {
      for (djinnIndex = 0; djinnIndex <= 19; djinnIndex++) {
        if (unit->djinnSet[element] & (1 << djinnIndex))
          ownedCount++;
        else if (unit->djinnHeld[element] & (1 << djinnIndex))
          ownedCount++;
      }
    }
    ownedCounts[memberIndex] = ownedCount;
  }
}
