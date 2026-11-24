/// @file
/// Previewing a Djinn change before it is made, the chosen party member's
/// status, and the checks that keep the party members' Djinn counts even.
// Part of menu.c.

#include "menu_internal.h"

/// The Djinni figures of a give or trade preview hop across between the two
/// party members once a second, a few frames into it: a hop takes
/// DJINN_PREVIEW_HOP_FRAMES frames along half a sine wave of
/// DJINN_PREVIEW_HOP_SINE_HEIGHT pixels, cut flat at DJINN_PREVIEW_HOP_HEIGHT.
/// It would dip no deeper than DJINN_PREVIEW_HOP_DEPTH below its line, and its
/// slide would start over after DJINN_PREVIEW_SLIDE_FRAMES frames; a hop
/// reaches neither.
#define DJINN_PREVIEW_HOP_FRAMES 30
#define DJINN_PREVIEW_HOP_DELAY 5
#define DJINN_PREVIEW_HOP_SINE_HEIGHT 16
#define DJINN_PREVIEW_HOP_HEIGHT 6
#define DJINN_PREVIEW_HOP_DEPTH 12
#define DJINN_PREVIEW_SLIDE_FRAMES 35

/// The tilemap rows at the top of the screen that a tutorial message covers,
/// saved before it opens and put back after it closes.
#define DJINN_TUTORIAL_COVERED_ROWS 8

/// Shows or hides the menu's cursor around a line of the Djinn tutorial.
#define DjinnTutorial_SetCursor(menu, value)                                   \
  do {                                                                         \
    (menu)->cursor->motion = (value);                                          \
  } while (0)

/// The Djinn menu's preview of a give, trade, set or standby (mode, enum
/// DjinnCompareOperation): both party members' stats and Psynergy as the change
/// would leave them, paged with R, the Djinni figures hopping across, and "Is
/// this OK?". Each unit is changed, measured and put back; nothing is applied.
s32 DjinnMenu_RunPreview(u32 mode) {
  struct DjinnMenuState *menu;
  struct DjinnMenuBackdrop *backdrop;
  struct WindowState *state;
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
  s32 index;
  s32 memberIndex;

  menu = gMenuState;
  confirmationChoice = YES;
  result = 0;
  redraw = 1;
  frame = 0;
  selectedPages[0] = 0;
  page = DJINN_COMPARE_PAGE_BOTH;
  menu->cursor->motion = ICON_HIDDEN;
  selectedPages[1] = 0;

  // The party figures go below the screen, except the two party members
  // concerned: the first over the left window, the second over the right.
  for (memberIndex = 0; memberIndex < 4; memberIndex++) {
    menu->memberY[memberIndex] = MENU_FIGURE_HIDDEN_Y;
  }
  menu->memberX[menu->selectedColumn[1]] = 120;
  menu->memberY[menu->selectedColumn[1]] = 32;
  menu->memberX[menu->selectedColumn[0]] = 16;
  menu->memberY[menu->selectedColumn[0]] = 32;

  WindowSprite_ClearListFar(menu->listWindow);
  wait(1);
  Window_CloseIfOpen(&menu->helpWindow, 1);
  Window_UpdateOrCreate(&menu->helpWindow, 0, 0, 30, 5, WINDOW_FLAG_INSTANT);

  pageCounts[0] = 1;
  pageCounts[1] = 1;

  // Count the Psynergy lists' pages: make the change on each party member's
  // unit, work the unit out again, compare its Psynergy with a copy taken
  // before, and put the unit back from the copy.
  if (mode <= DJINN_COMPARE_GIVE) {
    u16 *diffState;
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

    diffState = Heap_AllocEwram(96);
    size = sizeof(*originalUnit);
    originalUnit = Heap_AllocEwram(size);
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
                                      diffState, &gainedCount, &lostCount);
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
                                      diffState, &gainedCount, &lostCount);
    Iwram_CopyWords(unit, originalUnit, size);
    Heap_Release(originalUnit);
    Heap_Release(diffState);
  } else if (mode == DJINN_COMPARE_STANDBY || mode == DJINN_COMPARE_SET) {
    u16 *diffState;
    struct Unit *originalUnit;
    struct Unit *unit;
    s32 firstMemberId;
    s32 firstElement;
    s32 firstIndex;
    s32 applySecond;
    s32 lostCount;
    s32 gainedCount;

    diffState = Heap_AllocEwram(96);
    originalUnit = Heap_AllocEwram(sizeof(*originalUnit));
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
                                      diffState, &gainedCount, &lostCount);
    Iwram_CopyWords(unit, originalUnit, sizeof(*originalUnit));
    pageCounts[1] = pageCounts[0];
    page = DJINN_COMPARE_PAGE_GIVER;
    Heap_Release(originalUnit);
    Heap_Release(diffState);
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
  Window_UpdateOrCreate(&menu->window, 0, 5, 15, 15, WINDOW_FLAG_INSTANT);
  Window_UpdateOrCreate(&menu->secondWindow, 15, 5, 15, 15,
                        WINDOW_FLAG_INSTANT);
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
  // TEXT_COLOR_RED when it is not set.
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
                            5, row, TILEMAP_ENTRY_PLAIN);
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
  } else if (mode == DJINN_COMPARE_STANDBY || mode == DJINN_COMPARE_SET) {
    if ((menu->pairEntries[0] & DJINN_ENTRY_IS_SET) != 0) {
      Text_SetColorFar(TEXT_COLOR_RED);
    }
    Text_DrawMessageAsTilesFar(
        DJINN_ELEMENT(menu->pairEntries[0]) * DJINN_PER_ELEMENT +
            DJINN_NUMBER(menu->pairEntries[0]) + MsgDjinnName,
        menu->helpWindow, 48, 16);
    Window_SetTilemapEntryFar(menu->helpWindow,
                              DJINN_ELEMENT(menu->pairEntries[0]) +
                                  DJINN_ELEMENT_TILE,
                              5, 2, TILEMAP_ENTRY_PLAIN);
    Text_SetColorFar(TEXT_COLOR_NORMAL);
    // The blinking down arrow between the two, a tile of BG character block
    // 1, the block after the windows' own.
    Window_SetTilemapEntryFar(
        menu->helpWindow,
        WINDOW_TILE(BG_CHAR_SIZE / TILE_SIZE_4BPP + DJINN_CHANGE_ARROW_TILE), 7,
        1, TILEMAP_ENTRY_PLAIN);
  }

  for (;;) {
    s32 pressedKeys;
    s32 repeatedKeys;

    state = WINDOW_STATE;
    backdrop = menu->backdrop;
    pressedKeys = gKeysPressed;
    repeatedKeys = gKeysRepeat;

    // The tutorial presses A for the player after 100 frames, or shows
    // its messages; the windows' tilemap rows a message covers are saved
    // before it opens and put back after it closes.
    if (backdrop->tutorialStep != DJINN_TUTORIAL_OFF) {
      s16 *saved;
      s32 size;

      size = DJINN_TUTORIAL_COVERED_ROWS * BG_SCREEN_WIDTH * sizeof(u16);
      saved = Heap_AllocEwram(size);
      pressedKeys = 0;
      repeatedKeys = 0;
      backdrop->tutorialTimer += 1;

      switch (backdrop->tutorialStep) {
      case DJINN_TUTORIAL_GIVE_PREVIEW:
      case DJINN_TUTORIAL_SET_PREVIEW:
      case DJINN_TUTORIAL_POWERS_PREVIEW:
        if (backdrop->tutorialTimer == 100) {
          repeatedKeys = KEY_A;
          pressedKeys = KEY_A;
          backdrop->tutorialTimer = 0;
          backdrop->tutorialStep += 1;
        }
        break;
      case DJINN_TUTORIAL_SET_CHANGES:
        if (backdrop->tutorialTimer == 60) {
          struct Window *window;

          Iwram_CopyWords(saved, state->tilemap, size);
          // "Setting a Djinni changes your attributes and maybe even your
          // class."
          window = Message_OpenWindowFar(MsgDjinnTutorialSetChanges, 8, 0,
                                         MESSAGE_WINDOW_ZOOM);
          DjinnTutorial_SetCursor(menu, ICON_SHOWN);
          MenuCursor_Slide(2, 96);
          gMainRoutineState.messageSpeed = MESSAGE_SPEED_NORMAL;
          while (Message_AllTypedFar() == 0) {
            wait(1);
          }
          Window_CloseFar(window, 1);
          Iwram_CopyWords(state->tilemap, saved, size);
          Window_MarkVisibleTextTilesFar();
          state->dirty = 1;
          redraw = 1;
          backdrop->tutorialTimer = 0;
          backdrop->tutorialStep = DJINN_TUTORIAL_CLASS_PSYNERGY;
          DjinnTutorial_SetCursor(menu, ICON_HIDDEN);
        }
        break;
      case DJINN_TUTORIAL_CLASS_PSYNERGY:
        if (backdrop->tutorialTimer == 60) {
          struct Window *window;

          Iwram_CopyWords(saved, state->tilemap, size);
          // "When your class changes, the Psynergy you can use may also
          // change."
          window = Message_OpenWindowFar(MsgDjinnTutorialClassPsynergy, 8, 0,
                                         MESSAGE_WINDOW_ZOOM);
          DjinnTutorial_SetCursor(menu, ICON_SHOWN);
          MenuCursor_Slide(106, 56);
          gMainRoutineState.messageSpeed = MESSAGE_SPEED_NORMAL;
          while (Message_AllTypedFar() == 0) {
            wait(1);
          }
          Window_CloseFar(window, 1);
          Iwram_CopyWords(state->tilemap, saved, size);
          Window_MarkVisibleTextTilesFar();
          state->dirty = 1;
          wait(1);
          redraw = 1;
          backdrop->tutorialTimer = 0;
          backdrop->tutorialStep = DJINN_TUTORIAL_SET_PREVIEW;
          DjinnTutorial_SetCursor(menu, ICON_HIDDEN);
        }
        break;
      default:
        break;
      }
      Heap_Release(saved);
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
      state->holdTilemapUpload = 1;
      WindowSprite_PrepareForRedrawFar(menu->window);
      WindowSprite_PrepareForRedrawFar(menu->secondWindow);
      if ((redraw >> 1) != 0) {
        wait(1);
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
      state->holdTilemapUpload = 0;
    }

    // The Psynergy list's page numbers on the right window's top border:
    // WINDOW_TILE('1') + n is the window font's digit n + 1 in WINDOW_PALETTE
    // ('0' past the tenth page), in HIGHLIGHT_PALETTE for the page shown.
    if (page <= DJINN_COMPARE_PAGE_RECEIVER) {
      list = menu->secondWindow;
      if (pageCounts[page] > 1) {

        for (index = 0; index < pageCounts[page]; index++) {
          s32 tile;

          tile = WINDOW_TILE('1') + index;
          if (index > 9) {
            tile = WINDOW_TILE('0');
          }
          if (index == selectedPages[page]) {
            tile -= BG_TILE_PALETTE(WINDOW_PALETTE - HIGHLIGHT_PALETTE);
          }
          Window_SetTilemapEntryFar(list, tile,
                                    list->width - pageCounts[page] + index - 2,
                                    -1, TILEMAP_ENTRY_PLAIN);
        }
        Window_SetTilemapEntryFar(list, PAGE_ARROW_LEFT,
                                  list->width - pageCounts[page] - 3, -1,
                                  TILEMAP_ENTRY_PLAIN);
        Window_SetTilemapEntryFar(list, PAGE_ARROW_RIGHT, list->width - 2, -1,
                                  TILEMAP_ENTRY_PLAIN);
        Window_MarkTilemapRow(state, list->y);
      }
    }

    // The Djinni figures, at a fixed 3/4 size (a negative phase), hop once a
    // second: step runs through the hop's frames and holds at its end.
    frame++;
    step = (frame % 60) - DJINN_PREVIEW_HOP_DELAY;
    if (step < 0) {
      step = 0;
    }
    if (step > DJINN_PREVIEW_HOP_FRAMES - 1) {
      step = DJINN_PREVIEW_HOP_FRAMES - 1;
    }
    DjinnFigures_SetSlotPhase(0, -(FIXED_ONE * 3 / 4));
    DjinnFigures_SetSlotPhase(1, -(FIXED_ONE * 3 / 4));

    // The given Djinni hops from the giver's side to the receiver's, two
    // pixels a frame; in a trade the other one passes it the other way,
    // dipping as far as the first one rises.
    if (mode <= DJINN_COMPARE_GIVE) {
      s32 rise;
      s32 slide;

      // DEAD: step never reaches DJINN_PREVIEW_HOP_FRAMES, so the modulo
      // leaves it as it is; the ROM keeps its division.
      rise = -Iwram_MulFX(DJINN_PREVIEW_HOP_SINE_HEIGHT,
                          Trig_Sin((step % DJINN_PREVIEW_HOP_FRAMES) *
                                   (DEGtoANG(180) / DJINN_PREVIEW_HOP_FRAMES)));
      if (rise < -DJINN_PREVIEW_HOP_HEIGHT) {
        rise = -DJINN_PREVIEW_HOP_HEIGHT;
      }
      // DEAD: rise is never positive, the hop's half sine wave staying above
      // the line, so DJINN_PREVIEW_HOP_DEPTH never applies; the ROM keeps its
      // test.
      if (rise > DJINN_PREVIEW_HOP_DEPTH) {
        rise = DJINN_PREVIEW_HOP_DEPTH;
      }
      // DEAD: step never reaches DJINN_PREVIEW_SLIDE_FRAMES, so the modulo
      // leaves it as it is and the slide never wraps; the ROM keeps its
      // division.
      slide = (step % DJINN_PREVIEW_SLIDE_FRAMES) * 2;
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

    // Blink the down arrow between a thing and what the change makes of it
    // (DJINN_CHANGE_ARROW_TILE): drawn and blanked in turn every 4 frames.
    if ((frame & 3) == 0) {
      void *tile;
      u32 fill = TEXT_CANVAS_BACKGROUND_WORD;

      tile = (void *)DJINN_CHANGE_ARROW_VRAM;
      if ((frame & 4) != 0) {
        Iwram_CopyWords(tile, gMenu_ChangeArrowTile, 32);
      } else {
        Iwram_FillWords(tile, 32, fill);
      }
    }

    if ((pressedKeys & KEY_A) != 0) {
      if (confirmationChoice == YES) {
        result = DJINN_RESULT_CONFIRMED;
        break;
      }
      Sound_PlayFar(SE_MENU_CANCEL);
      result = DJINN_RESULT_BACK;
      break;
    }
    if ((pressedKeys & KEY_START) != 0) {
      Sound_PlayFar(SE_MENU_CANCEL);
      result = DJINN_RESULT_START;
      break;
    } else if ((pressedKeys & KEY_B) != 0) {
      Sound_PlayFar(SE_MENU_CANCEL);
      result = DJINN_RESULT_BACK;
      break;
    } else if ((pressedKeys & KEY_R) != 0) {
      if (mode <= DJINN_COMPARE_GIVE) {
        page = ((page + 1) % 3);
        redraw = 2;
        Sound_PlayFar(SE_MENU_CURSOR_MOVE);
      }
    } else if ((repeatedKeys & KEY_LEFT) != 0) {
      if (page <= DJINN_COMPARE_PAGE_RECEIVER) {
        selectedPages[page]--;
        selectedPages[page] =
            Menu_WrapIndex(selectedPages[page], pageCounts[page]);
        Sound_PlayFar(SE_MENU_CURSOR_MOVE);
        Input_ResetKeyRepeat();
        redraw = 1;
      }
    } else if ((repeatedKeys & KEY_RIGHT) != 0) {
      if (page <= DJINN_COMPARE_PAGE_RECEIVER) {
        Sound_PlayFar(SE_MENU_CURSOR_MOVE);
        Input_ResetKeyRepeat();
        selectedPages[page]++;
        redraw = 1;
        selectedPages[page] =
            Menu_WrapIndex(selectedPages[page], pageCounts[page]);
      }
    }
    wait(1);
  }

  // Restore the ordinary list drawing tasks and close the preview sides; the
  // figures grow in again from phase 0.
  DjinnFigures_SetSlotPhase(0, 0);
  DjinnFigures_SetSlotPhase(1, 0);
  Task_Add(Menu_PlacePartyFigures, TASK_PASS_FRAME);

  WINDOW_STATE->holdTilemapUpload = 1;
  Window_CloseIfOpen(&menu->helpWindow, 1);
  wait(1);
  Window_UpdateOrCreate(&menu->helpWindow, 13, 0, 17, 5, WINDOW_FLAG_INSTANT);
  Window_CloseIfOpen((struct Window **)&menu->window, 1);
  Window_CloseIfOpen(&menu->secondWindow, 1);
  WindowSprite_RedrawSavedRectFar(menu->listWindow);
  // BUG: no window is ever made in unopenedWindow (DjinnMenuState), so this
  // redraws through NULL; see the field. BUGFIX skips it.
#ifndef BUGFIX
  WindowSprite_RedrawSavedRectFar((struct Window *)menu->unopenedWindow);
#endif
  WindowSprite_RedrawSavedRectFar(menu->statusWindow);
  WINDOW_STATE->holdTilemapUpload = 0;
  wait(1);
  return result;
}

/// Shows the chosen party member's current stats and paged Psynergy list.
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
  struct WindowState *state;
  u32 pressedKeys;
  u32 repeatedKeys;
  struct DjinnMenuState *menu;
  struct Unit *unit;
  struct Window *window;
  u16 *listedActions;
  void *saved;
  s32 result;
  s32 tile;
  s32 pageIndex;
  s32 side;
  s32 blink;

  menu = gMenuState;
  phase = 0;
  redraw = 1;
  result = 0;
  frame = 0;
  menu->cursor->motion = ICON_HIDDEN;
  selectedPages[1] = selectedPages[0] = result;
  for (side = 0; side < 4; side++)
    menu->memberY[side] = MENU_FIGURE_HIDDEN_Y;
  WindowSprite_ClearListFar(menu->listWindow);
  wait(1);
  // Count the party member's Psynergy by diffing their list with itself; saved,
  // room for a copy of the unit, is taken and given back unused.
  pageCounts[1] = pageCounts[0] = 1;
  listedActions = Heap_AllocEwram(96);
  saved = Heap_AllocEwram(sizeof(struct Unit));
  unit = Unit_GetFar(menu->shownMember[0]);
  pageCounts[1] = pageCounts[0] = PsynergyList_Diff(
      unit->psynergy, unit->psynergy, listedActions, &gained, &lost);
  Heap_Release(saved);
  Heap_Release(listedActions);
  pageCounts[0] = (pageCounts[0] - 1) / 6 + 1;
  if (pageCounts[0] == 0)
    pageCounts[0] = 1;
  pageCounts[1] = (pageCounts[1] - 1) / 6 + 1;
  if (pageCounts[1] == 0)
    pageCounts[1] = 1;
  leftWindowSlot = (struct Window **)&menu->window;
  Window_UpdateOrCreate(leftWindowSlot, 0, 5, 15, 15, WINDOW_FLAG_INSTANT);
  rightWindowSlot = &menu->secondWindow;
  Window_UpdateOrCreate(rightWindowSlot, 15, 5, 15, 15, WINDOW_FLAG_INSTANT);
  WindowSprite_RedrawSavedRectFar(menu->helpWindow);
  WindowSprite_RedrawSavedRectFar(menu->statusWindow);
  Text_DrawMessageAsTilesFar(MsgCurrentStatus, menu->statusWindow, 0, 0);
  Text_DrawMessageAsTilesFar(MsgCurrentStatus + 2, menu->statusWindow, 0, 16);
  side = 0;
  while (1) {
    state = WINDOW_STATE;
    pressedKeys = gKeysPressed;
    repeatedKeys = gKeysRepeat;
    if (redraw) {
      state->holdTilemapUpload = 1;
      WindowSprite_PrepareForRedrawFar(menu->window);
      WindowSprite_PrepareForRedrawFar(menu->secondWindow);
      DjinnMenu_DrawStatPreview(menu->window, 0, 0, menu->pairMember[0], 0, 0,
                                DJINN_PREVIEW_NO_CHANGE, 0, 1);
      DjinnMenu_DrawStatPreview(menu->secondWindow, 0, 0, menu->pairMember[0],
                                0, 0, DJINN_PREVIEW_NO_CHANGE,
                                selectedPages[0] + 1, 1);
      state->holdTilemapUpload = 0;
    }
    if (pageCounts[side] > 1) {
      window = menu->secondWindow;
      for (pageIndex = 0; pageIndex < pageCounts[side]; pageIndex++) {
        tile = WINDOW_TILE('1') + pageIndex;
        if (pageIndex > 9)
          tile = WINDOW_TILE('0');
        if (pageIndex == selectedPages[side])
          tile -= BG_TILE_PALETTE(WINDOW_PALETTE - HIGHLIGHT_PALETTE);
        Window_SetTilemapEntryFar(
            window, tile, window->width - pageCounts[side] + pageIndex - 2, -1,
            TILEMAP_ENTRY_PLAIN);
      }
      Window_SetTilemapEntryFar(window, PAGE_ARROW_LEFT,
                                window->width - pageCounts[side] - 3, -1,
                                TILEMAP_ENTRY_PLAIN);
      Window_SetTilemapEntryFar(window, PAGE_ARROW_RIGHT, window->width - 2, -1,
                                TILEMAP_ENTRY_PLAIN);
      Window_MarkTilemapRow(state, window->y);
    }
    // blink is worked out as the preview's hop step is, and not used; the
    // Djinni figure stays below the screen.
    frame++;
    blink = frame % 60 - DJINN_PREVIEW_HOP_DELAY;
    CLAMP(blink, 0, DJINN_PREVIEW_HOP_FRAMES - 1);
    DjinnFigures_SetSlotPosition(0, 32, MENU_FIGURE_HIDDEN_Y, 0);
    if (redraw) {
      redraw = 0;
      phase = Menu_WrapIndex(phase, 2);
    }
    if ((frame & 3) == 0) {
      if (frame & 4)
        Iwram_CopyWords((void *)DJINN_CHANGE_ARROW_VRAM, gMenu_ChangeArrowTile,
                        32);
      else
        Iwram_FillWords((void *)DJINN_CHANGE_ARROW_VRAM, 32,
                        TEXT_CANVAS_BACKGROUND_WORD);
    }
    if (pressedKeys & KEY_START) {
      Sound_PlayFar(SE_MENU_CANCEL);
      result = DJINN_RESULT_START;
      break;
    }
    if (pressedKeys & (KEY_A | KEY_B | KEY_R | KEY_L)) {
      Sound_PlayFar(SE_MENU_CANCEL);
      result = DJINN_RESULT_BACK;
      break;
    }
    if (repeatedKeys & KEY_LEFT) {
      selectedPages[side]--;
      selectedPages[side] =
          Menu_WrapIndex(selectedPages[side], pageCounts[side]);
      Sound_PlayFar(SE_MENU_CURSOR_MOVE);
      Input_ResetKeyRepeat();
      redraw = 1;
    } else if (repeatedKeys & KEY_RIGHT) {
      selectedPages[side]++;
      Sound_PlayFar(SE_MENU_CURSOR_MOVE);
      Input_ResetKeyRepeat();
      redraw = 1;
      selectedPages[side] =
          Menu_WrapIndex(selectedPages[side], pageCounts[side]);
    }
    wait(1);
  }
  Task_Add(Menu_PlacePartyFigures, TASK_PASS_FRAME);
  WINDOW_STATE->holdTilemapUpload = 1;
  Window_CloseIfOpen(&menu->helpWindow, 1);
  wait(1);
  Window_UpdateOrCreate(&menu->helpWindow, 13, 0, 17, 5, WINDOW_FLAG_INSTANT);
  Window_CloseIfOpen(leftWindowSlot, 1);
  Window_CloseIfOpen(rightWindowSlot, 1);
  WindowSprite_RedrawSavedRectFar(menu->listWindow);
  // BUG: no window is ever made in unopenedWindow (DjinnMenuState), so this
  // redraws through NULL; see the field. BUGFIX skips it.
#ifndef BUGFIX
  WindowSprite_RedrawSavedRectFar((struct Window *)menu->unopenedWindow);
#endif
  WindowSprite_RedrawSavedRectFar(menu->statusWindow);
  WINDOW_STATE->holdTilemapUpload = 0;
  wait(1);
  return result;
}

/// Marks recipients that would leave unequal Djinn totals after a direct give.
/// Returns how many recipients would need a trade instead.
s32 Djinn_MarkTradeOnlyRecipients(s8 *requiresTrade, s32 sourceMemberColumn) {
  s32 tradeCount;
  s32 memberColumn;
  s8 *entry;
  struct DjinnMenuState *menu;

  menu = gMenuState;
  tradeCount = 0;
  memberColumn = 0;
  if (tradeCount < menu->memberCount) {
    entry = requiresTrade;
    do {
      *entry = 0;
      if (memberColumn != sourceMemberColumn) {
        if (Djinn_CheckGiveBalance(sourceMemberColumn, memberColumn) == 0) {
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
/// one Djinn.
s32 Djinn_CheckGiveBalance(s32 sourceMemberColumn, s32 targetMemberColumn) {
  struct DjinnMenuState *menu;
  s8 ownedCounts[16];
  u8 firstMember;
  u8 secondMember;
  s32 difference;
  s32 balanced;

  menu = gMenuState;
  Djinn_CountOwned((u8 *)ownedCounts);
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
void Djinn_CountOwned(u8 *ownedCounts) {
  struct DjinnMenuState *menu;
  struct Unit *unit;
  s32 memberIndex;
  s32 element;
  s32 djinnIndex;
  s32 ownedCount;

  menu = gMenuState;
  for (memberIndex = 0; memberIndex < menu->memberCount; memberIndex++) {
    unit = Unit_GetFar(menu->members[memberIndex]);
    ownedCount = 0;
    for (element = 0; element <= 3; element++) {
      for (djinnIndex = 0; djinnIndex < DJINN_PER_ELEMENT; djinnIndex++) {
        if (unit->djinnSet[element] & (1 << djinnIndex))
          ownedCount++;
        else if (unit->djinnHeld[element] & (1 << djinnIndex))
          ownedCount++;
      }
    }
    ownedCounts[memberIndex] = ownedCount;
  }
}
