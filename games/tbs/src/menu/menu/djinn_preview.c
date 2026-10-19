/// @file
/// Part of menu.c, the menu object, compiled only through it.
/// Previewing a Djinn change before it is made, the chosen member's status,
/// and the checks that keep the members' Djinn counts even.

#include "../menu_internal.h"

/// Previews a give, trade, set or standby change without committing it.
/// It saves and restores the unit records before polling for confirmation.
s32 OwnerAction_RunCompareLoop(u32 mode) {
  struct DjinnMenuWork *menu;
  struct DjinnMenuBackdrop *backdrop;
  struct UiRenderWork *render;
  struct UiWindow *list;
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
  s32 ownerIndex;

  menu = gMenuWork;
  confirmationChoice = 0;
  result = 0;
  redraw = 1;
  frame = 0;
  selectedPages[0] = 0;
  page = 2;
  menu->cursor->active = ICON_HIDDEN;
  selectedPages[1] = 0;

  for (ownerIndex = 3; ownerIndex >= 0; ownerIndex--) {
    menu->ownerY[ownerIndex] = 0xc8;
  }
  menu->ownerX[menu->selectedColumn[1]] = 0x78;
  menu->ownerY[menu->selectedColumn[1]] = 0x20;
  menu->ownerX[menu->selectedColumn[0]] = 0x10;
  menu->ownerY[menu->selectedColumn[0]] = 0x20;

  RenderOutput_ClearListFar(menu->listWindow);
  WaitFrames(1);
  UiWindow_CloseIfOpen(&menu->helpWindow, 1);
  UiWindow_UpdateOrCreate(&menu->helpWindow, 0, 0, 30, 5, 2);

  pageCounts[0] = 1;
  pageCounts[1] = 1;

  // Measure the action-list pages using temporary unit mutations.
  if (mode <= DJINN_COMPARE_GIVE) {
    u16 *diffWork;
    struct BattleUnit *originalUnit;
    struct BattleUnit *unit;
    s32 ownerId;
    s32 firstElement;
    s32 firstIndex;
    s32 secondElement;
    s32 secondIndex;
    s32 firstWasSet;
    s32 secondWasSet;
    s32 lostCount;
    s32 gainedCount;
    s32 size;

    diffWork = Runtime_BumpAllocateAlternatePool(96);
    size = sizeof(*originalUnit);
    originalUnit = Runtime_BumpAllocateAlternatePool(size);
    secondElement = 0;
    secondIndex = 0;
    ownerId = menu->shownOwner[0];
    firstElement = menu->pairElement[0];
    firstIndex = menu->pairIndex[0] & 31;
    firstWasSet = (u16)(menu->pairEntries[0] & DJINN_ENTRY_IS_SET);

    unit = UnitGetFar(ownerId);
    Iwram_CopyWords(originalUnit, unit, size);
    Djinn_DeactivateFar(ownerId, firstElement, firstIndex);
    if (mode == DJINN_COMPARE_TRADE) {
      secondElement = menu->pairElement[1];
      secondIndex = menu->pairIndex[1] & 31;
      secondWasSet = (u16)(menu->pairEntries[1] & DJINN_ENTRY_IS_SET);
      Djinn_AddToOwnerFar(ownerId, secondElement, secondIndex);
      if (secondWasSet != 0) {
        Djinn_ActivateFar(ownerId, secondElement, secondIndex);
      }
    }
    UnitCalcFar(ownerId);
    pageCounts[0] =
        OwnerAction_DiffSlots(originalUnit->actionSlots, unit->actionSlots,
                              diffWork, &gainedCount, &lostCount);
    Iwram_CopyWords(unit, originalUnit, size);

    ownerId = menu->shownOwner[1];
    unit = UnitGetFar(ownerId);
    Iwram_CopyWords(originalUnit, unit, size);
    if (mode == DJINN_COMPARE_TRADE) {
      Djinn_DeactivateFar(ownerId, secondElement, secondIndex);
    }
    Djinn_AddToOwnerFar(ownerId, firstElement, firstIndex);
    if (firstWasSet != 0) {
      Djinn_ActivateFar(ownerId, firstElement, firstIndex);
    }
    UnitCalcFar(ownerId);
    pageCounts[1] =
        OwnerAction_DiffSlots(originalUnit->actionSlots, unit->actionSlots,
                              diffWork, &gainedCount, &lostCount);
    Iwram_CopyWords(unit, originalUnit, size);
    MemFree(originalUnit);
    MemFree(diffWork);
  } else if (mode - 2 <= 1) {
    u16 *diffWork;
    struct BattleUnit *originalUnit;
    struct BattleUnit *unit;
    s32 firstOwnerId;
    s32 firstElement;
    s32 firstIndex;
    s32 applySecond;
    s32 lostCount;
    s32 gainedCount;

    diffWork = Runtime_BumpAllocateAlternatePool(96);
    originalUnit = Runtime_BumpAllocateAlternatePool(sizeof(*originalUnit));
    firstOwnerId = menu->shownOwner[0];
    firstElement = menu->pairElement[0];
    firstIndex = menu->pairIndex[0] & 31;
    applySecond = (mode == DJINN_COMPARE_SET);

    unit = UnitGetFar(firstOwnerId);
    Iwram_CopyWords(originalUnit, unit, sizeof(*originalUnit));
    Djinn_DeactivateFar(firstOwnerId, firstElement, firstIndex);
    if (applySecond != 0) {
      Djinn_ActivateFar(firstOwnerId, firstElement, firstIndex);
    }
    UnitCalcFar(firstOwnerId);
    pageCounts[0] =
        OwnerAction_DiffSlots(originalUnit->actionSlots, unit->actionSlots,
                              diffWork, &gainedCount, &lostCount);
    Iwram_CopyWords(unit, originalUnit, sizeof(*originalUnit));
    pageCounts[1] = pageCounts[0];
    page = 0;
    MemFree(originalUnit);
    MemFree(diffWork);
  }

  pageCounts[0] = ((pageCounts[0] - 1) / 5) + 1;
  if (pageCounts[0] == 0) {
    pageCounts[0] = 1;
  }
  pageCounts[1] = ((pageCounts[1] - 1) / 5) + 1;
  if (pageCounts[1] == 0) {
    pageCounts[1] = 1;
  }

  // Open the paired panes only after both unit records have been restored.
  UiWindow_UpdateOrCreate(&menu->window, 0, 5, 15, 15, 2);
  UiWindow_UpdateOrCreate(&menu->secondWindow, 15, 5, 15, 15, 2);
  RenderOutput_RedrawSavedRectFar(menu->helpWindow);

  if (mode == DJINN_COMPARE_STANDBY) {
    UiText_DrawCharacterAtOffsetFar(MsgDjinnCompareStandby, menu->helpWindow,
                                    96, 0);
    UiText_DrawCharacterAtOffsetFar(MsgDjinnCompareConfirm, menu->helpWindow,
                                    96, 16);
  } else if (mode == DJINN_COMPARE_SET) {
    UiText_DrawCharacterAtOffsetFar(MsgDjinnCompareSet, menu->helpWindow, 96,
                                    0);
    UiText_DrawCharacterAtOffsetFar(MsgDjinnCompareConfirm, menu->helpWindow,
                                    96, 16);
  } else if (mode == DJINN_COMPARE_TRADE) {
    UiText_DrawCharacterAtOffsetFar(MsgDjinnCompareTrade, menu->helpWindow, 128,
                                    0);
    UiText_DrawCharacterAtOffsetFar(MsgDjinnCompareConfirm, menu->helpWindow,
                                    128, 8);
  } else {
    UiText_DrawCharacterAtOffsetFar(MsgDjinnCompareGive, menu->helpWindow, 128,
                                    0);
    UiText_DrawCharacterAtOffsetFar(MsgDjinnCompareConfirm, menu->helpWindow,
                                    128, 8);
  }

  row = (mode == DJINN_COMPARE_GIVE);
  if ((menu->pairEntries[0] & DJINN_ENTRY_IS_SET) == 0) {
    UiWork_SetParamNibbleFar(2);
  }
  UiText_DrawCharacterAtOffsetFar(((menu->pairEntries[0] & 0xe0) >> 5) * 20 +
                                      (menu->pairEntries[0] & 31) +
                                      MsgDjinnName,
                                  menu->helpWindow, 48, row * 8);
  UiWindow_SetTilemapEntryFar(menu->helpWindow,
                              ((menu->pairEntries[0] & 0xe0) >> 5) + 0x5001, 5,
                              row, zero = 0);
  UiWork_SetParamNibbleFar(TEXT_COLOR_NORMAL);

  if (mode == DJINN_COMPARE_TRADE) {
    if ((menu->pairEntries[1] & DJINN_ENTRY_IS_SET) == 0) {
      UiWork_SetParamNibbleFar(2);
    }
    UiText_DrawCharacterAtOffsetFar(((menu->pairEntries[1] & 0xe0) >> 5) * 20 +
                                        (menu->pairEntries[1] & 31) +
                                        MsgDjinnName,
                                    menu->helpWindow, 48, 16);
    UiWindow_SetTilemapEntryFar(menu->helpWindow,
                                ((menu->pairEntries[1] & 0xe0) >> 5) + 0x5001,
                                5, 2, mode);
    UiWork_SetParamNibbleFar(TEXT_COLOR_NORMAL);
  } else if (mode - 2 <= 1) {
    if ((menu->pairEntries[0] & DJINN_ENTRY_IS_SET) != 0) {
      UiWork_SetParamNibbleFar(2);
    }
    UiText_DrawCharacterAtOffsetFar(((menu->pairEntries[0] & 0xe0) >> 5) * 20 +
                                        (menu->pairEntries[0] & 31) +
                                        MsgDjinnName,
                                    menu->helpWindow, 48, 16);
    UiWindow_SetTilemapEntryFar(menu->helpWindow,
                                ((menu->pairEntries[0] & 0xe0) >> 5) + 0x5001,
                                5, 2, zero);
    UiWork_SetParamNibbleFar(TEXT_COLOR_NORMAL);
    UiWindow_SetTilemapEntryFar(menu->helpWindow, 0xf296, 7, 1, zero);
  }

  for (;;) {
    s32 pressedKeys;
    s32 repeatedKeys;

    render = (struct UiRenderWork *)gWindowWork[0];
    backdrop = menu->backdrop;
    pressedKeys = gKeysPressed;
    repeatedKeys = gKeysRepeat;

    if (backdrop->tutorialStep != 0) {
      s16 *saved;
      s32 size;

      size = 0x200;
      saved = Runtime_BumpAllocateAlternatePool(size);
      pressedKeys = 0;
      repeatedKeys = 0;
      backdrop->tutorialTimer += 1;

      switch (backdrop->tutorialStep) {
      case 5:
      case 12:
      case 23:
        if (backdrop->tutorialTimer == 100) {
          repeatedKeys = 1;
          pressedKeys = 1;
          backdrop->tutorialTimer = 0;
          backdrop->tutorialStep += 1;
        }
        break;
      case 10:
        if (backdrop->tutorialTimer == 60) {
          struct UiWindow *work;

          Iwram_CopyWords(saved, render, size);
          work =
              UiText_OpenMessageWindowFar(MsgDjinnTutorialSetChanges, 8, 0, 1);
          /* FAKEMATCH(one-pass): the cursor's store in a one-pass block, which
           * keeps the window copy before it as the ROM does; as a plain
           * statement, 11 of 1045 instructions differ in tbs-en. */
          do {
            menu->cursor->active = ICON_SHOWN;
          } while (0);
          UiMenu_SlideCursor(2, 96);
          gGameState.messageSpeed = 1;
          while (UiWork_IsCompleteFar() == 0) {
            WaitFrames(1);
          }
          UiWork_FinalizeFar(work, 1);
          Iwram_CopyWords(render, saved, size);
          UiWindow_MarkVisibleTileAttributesFar();
          render->dirty = 1;
          redraw = 1;
          backdrop->tutorialTimer = 0;
          backdrop->tutorialStep = 11;
          /* FAKEMATCH(one-pass): the cursor's store in a one-pass block, which
           * keeps the step's store before it as the ROM does; as a plain
           * statement, 6 of 1045 instructions differ in tbs-en. */
          do {
            menu->cursor->active = ICON_HIDDEN;
          } while (0);
        }
        break;
      case 11:
        if (backdrop->tutorialTimer == 60) {
          struct UiWindow *work;

          Iwram_CopyWords(saved, render, size);
          work = UiText_OpenMessageWindowFar(MsgDjinnTutorialClassPsynergy, 8,
                                             0, 1);
          /* FAKEMATCH(one-pass): the cursor's store in a one-pass block, which
           * keeps the window copy before it as the ROM does; as a plain
           * statement, 13 of 1045 instructions differ in tbs-en. */
          do {
            menu->cursor->active = ICON_SHOWN;
          } while (0);
          UiMenu_SlideCursor(106, 56);
          gGameState.messageSpeed = 1;
          while (UiWork_IsCompleteFar() == 0) {
            WaitFrames(1);
          }
          UiWork_FinalizeFar(work, 1);
          Iwram_CopyWords(render, saved, size);
          UiWindow_MarkVisibleTileAttributesFar();
          render->dirty = 1;
          WaitFrames(1);
          redraw = 1;
          backdrop->tutorialTimer = 0;
          backdrop->tutorialStep = 12;
          /* FAKEMATCH(one-pass): the cursor's store in a one-pass block, which
           * keeps the step's store before it as the ROM does; as a plain
           * statement, 5 of 1045 instructions differ in tbs-en. */
          do {
            menu->cursor->active = ICON_HIDDEN;
          } while (0);
        }
        break;
      default:
        break;
      }
      MemFree(saved);
    }

    if (redraw != 0) {
      addPairSlot = 1;
      if (mode <= DJINN_COMPARE_GIVE) {
        UiWindow_ClearInteriorTilesFar(menu->helpWindow, 128, 16, 224, 24);
        if (page == 1) {
          UiText_DrawCharacterAtOffsetFar(MsgDjinnStatusHelp, menu->helpWindow,
                                          128, 16);
        } else {
          UiWork_PushValueSlotFar(menu->shownOwner[page != 2], 1);
          UiText_DrawCharacterAtOffsetFar(MsgDjinnPsyHelp, menu->helpWindow,
                                          128, 16);
        }
      }
      render->menuBusy = 1;
      RenderOutput_PrepareForRedrawFar(menu->window);
      RenderOutput_PrepareForRedrawFar(menu->secondWindow);
      if ((redraw >> 1) != 0) {
        WaitFrames(1);
      }

      if (mode == DJINN_COMPARE_SET) {
        previewMode = 0;
        addPairSlot = 0;
      } else if (mode == DJINN_COMPARE_STANDBY) {
        previewMode = 1;
        addPairSlot = 0;
      } else {
        previewMode = 2;
      }

      switch (page) {
      default:
        DjinnMenu_DrawPairPreview(mode, 0);
        break;
      case 1:
        if (mode == DJINN_COMPARE_GIVE) {
          previewMode = 4;
        }
        DjinnMenu_DrawStatPreview(menu->window, 0, 0, menu->shownOwner[1], page,
                                  0, previewMode, 0, 0);
        DjinnMenu_DrawStatPreview(menu->secondWindow, 0, 0, menu->shownOwner[1],
                                  page, 0, previewMode, selectedPages[1] + 1,
                                  0);
        break;
      case 0:
        if (mode == DJINN_COMPARE_GIVE) {
          previewMode = 1;
          addPairSlot = 0;
        }
        DjinnMenu_DrawStatPreview(menu->window, 0, 0, menu->pairOwner[0], page,
                                  addPairSlot, previewMode, page, 1);
        DjinnMenu_DrawStatPreview(menu->secondWindow, 0, 0, menu->pairOwner[0],
                                  page, addPairSlot, previewMode,
                                  selectedPages[0] + 1, 1);
        break;
      }
      render->menuBusy = 0;
    }

    if (page <= 1) {
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
          UiWindow_SetTilemapEntryFar(
              list, tile, list->width - pageCounts[page] + index - 2, -1, 0);
        }
        top = -1;
        UiWindow_SetTilemapEntryFar(list, UI_PAGE_ARROW_LEFT,
                                    list->width - pageCounts[page] - 3, top, 0);
        UiWindow_SetTilemapEntryFar(list, UI_PAGE_ARROW_RIGHT, list->width - 2,
                                    top, 0);
        UiRender_MarkRow(render, list->y);
      }
    }

    frame++;
    step = (frame % 60) - 5;
    if (step < 0) {
      step = 0;
    }
    if (step > 29) {
      step = 29;
    }
    FourObjectMotion_SetSlotPhase(0, 0xffff4000);
    FourObjectMotion_SetSlotPhase(1, 0xffff4000);

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
      FourObjectMotion_SetSlotPosition(0, slide + 34, rise + 20, 0);
      if (mode == DJINN_COMPARE_TRADE) {
        FourObjectMotion_SetSlotPosition(1, 99 - slide, 36 - rise, 0);
      }
    } else {
      FourObjectMotion_SetSlotPosition(0, 32, 30, 0);
    }

    if (redraw != 0) {
      redraw = 0;
      confirmationChoice = Menu_WrapIndex(confirmationChoice, 2);
    }
    UiMenu_PositionCursor(confirmationChoice * 56 + 80, 16);

    if ((frame & 3) == 0) {
      void *tile;
      u32 fill;

      tile = (void *)0x060052c0;
      fill = 0x44444444;
      if ((frame & 4) != 0) {
        Iwram_CopyWords(tile, gMenu_BackdropFrameTile, 32);
      } else {
        Iwram_FillWords(tile, 32, fill);
      }
    }

    if ((pressedKeys & KEY_A) != 0) {
      if (confirmationChoice == 0) {
        result = 1;
        break;
      }
      AudioCommand_PlayFar(SE_MENU_CANCEL);
      result = -1;
      break;
    }
    if ((pressedKeys & KEY_START) != 0) {
      AudioCommand_PlayFar(SE_MENU_CANCEL);
      result = -2;
      break;
    } else if ((pressedKeys & KEY_B) != 0) {
      AudioCommand_PlayFar(SE_MENU_CANCEL);
      result = -1;
      break;
    } else if ((pressedKeys & KEY_R) != 0) {
      if (mode <= DJINN_COMPARE_GIVE) {
        page = ((page + 1) % 3);
        redraw = 2;
        AudioCommand_PlayFar(SE_MENU_CURSOR_MOVE);
      }
    } else if ((repeatedKeys & KEY_LEFT) != 0) {
      if (page <= 1) {
        selectedPages[page]--;
        selectedPages[page] =
            Menu_WrapIndex(selectedPages[page], pageCounts[page]);
        AudioCommand_PlayFar(SE_MENU_CURSOR_MOVE);
        Input_ResetKeyRepeat();
        redraw = 1;
      }
    } else if ((repeatedKeys & KEY_RIGHT) != 0) {
      if (page <= 1) {
        AudioCommand_PlayFar(SE_MENU_CURSOR_MOVE);
        Input_ResetKeyRepeat();
        selectedPages[page]++;
        redraw = 1;
        selectedPages[page] =
            Menu_WrapIndex(selectedPages[page], pageCounts[page]);
      }
    }
    WaitFrames(1);
  }

  // Restore the ordinary list drawing tasks and close the preview panes.
  FourObjectMotion_SetSlotPhase(0, 0);
  FourObjectMotion_SetSlotPhase(1, 0);
  Scheduler_AddOrUpdateCallback(Menu_UpdateEntryObjectTransforms, 0xc80);

  ((struct UiRenderWork *)gWindowWork[0])->menuBusy = 1;
  UiWindow_CloseIfOpen(&menu->helpWindow, 1);
  WaitFrames(1);
  UiWindow_UpdateOrCreate(&menu->helpWindow, 13, 0, 17, 5, 2);
  UiWindow_CloseIfOpen((struct UiWindow **)&menu->window, 1);
  UiWindow_CloseIfOpen(&menu->secondWindow, 1);
  RenderOutput_RedrawSavedRectFar(menu->listWindow);
  RenderOutput_RedrawSavedRectFar((struct UiWindow *)menu->unknownWindow028);
  RenderOutput_RedrawSavedRectFar(menu->statusWindow);
  ((struct UiRenderWork *)gWindowWork[0])->menuBusy = 0;
  WaitFrames(1);
  return result;
}

/// Shows the chosen owner's current stats and paged Psynergy list.
s32 DjinnMenu_ShowOwnerStatus(void) {
  s32 selectedPages[2];
  s32 pageCounts[2];
  s32 gained;
  s32 lost;
  s32 phase;
  s32 redraw;
  s32 frame;
  struct UiWindow **leftWindowSlot;
  struct UiWindow **rightWindowSlot;
  struct UiRenderWork *render;
  u32 pressedKeys;
  u32 repeatedKeys;
  struct DjinnMenuWork *menu;
  struct BattleUnit *unit;
  struct UiWindow *window;
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
  menu->cursor->active = ICON_HIDDEN;
  selectedPages[1] = selectedPages[0] = result;
  for (side = 3; side >= 0; side--)
    menu->ownerY[side] = 200;
  RenderOutput_ClearListFar(menu->listWindow);
  WaitFrames(1);
  counts = pageCounts;
  counts[1] = counts[0] = 1;
  listedActions = Runtime_BumpAllocateAlternatePool(96);
  saved = Runtime_BumpAllocateAlternatePool(332);
  unit = UnitGetFar(menu->shownOwner[0]);
  counts[1] = counts[0] = OwnerAction_DiffSlots(
      unit->actionSlots, unit->actionSlots, listedActions, &gained, &lost);
  MemFree(saved);
  MemFree(listedActions);
  counts[0] = (counts[0] - 1) / 6 + 1;
  if (counts[0] == 0)
    counts[0] = 1;
  counts[1] = (counts[1] - 1) / 6 + 1;
  if (counts[1] == 0)
    counts[1] = 1;
  leftWindowSlot = (struct UiWindow **)&menu->window;
  UiWindow_UpdateOrCreate(leftWindowSlot, 0, 5, 15, 15, 2);
  rightWindowSlot = &menu->secondWindow;
  UiWindow_UpdateOrCreate(rightWindowSlot, 15, 5, 15, 15, 2);
  RenderOutput_RedrawSavedRectFar(menu->helpWindow);
  RenderOutput_RedrawSavedRectFar(menu->statusWindow);
  UiText_DrawCharacterAtOffsetFar(MsgCurrentStatus, menu->statusWindow, 0, 0);
  UiText_DrawCharacterAtOffsetFar(MsgCurrentStatus + 2, menu->statusWindow, 0,
                                  16);
  side = 0;
  while (1) {
    render = (struct UiRenderWork *)gWindowWork[0];
    pressedKeys = gKeysPressed;
    repeatedKeys = gKeysRepeat;
    if (redraw) {
      render->menuBusy = 1;
      RenderOutput_PrepareForRedrawFar(menu->window);
      RenderOutput_PrepareForRedrawFar(menu->secondWindow);
      DjinnMenu_DrawStatPreview(menu->window, 0, 0, menu->pairOwner[0], 0, 0, 3,
                                0, 1);
      DjinnMenu_DrawStatPreview(menu->secondWindow, 0, 0, menu->pairOwner[0], 0,
                                0, 3, selectedPages[0] + 1, 1);
      render->menuBusy = 0;
    }
    if (counts[side] > 1) {
      window = menu->secondWindow;
      for (pageIndex = 0; pageIndex < counts[side]; pageIndex++) {
        tile = 0xf031 + pageIndex;
        if (pageIndex > 9)
          tile = 0xf030;
        if (pageIndex == selectedPages[side])
          tile -= 0x1000;
        UiWindow_SetTilemapEntryFar(
            window, tile, window->width - counts[side] + pageIndex - 2, -1, 0);
      }
      UiWindow_SetTilemapEntryFar(window, UI_PAGE_ARROW_LEFT,
                                  window->width - counts[side] - 3, -1, 0);
      UiWindow_SetTilemapEntryFar(window, UI_PAGE_ARROW_RIGHT,
                                  window->width - 2, -1, 0);
      UiRender_MarkRow(render, window->y);
    }
    frame++;
    blink = frame % 60 - 5;
    CLAMP(blink, 0, 29);
    FourObjectMotion_SetSlotPosition(0, 32, 200, 0);
    if (redraw) {
      redraw = 0;
      phase = Menu_WrapIndex(phase, 2);
    }
    if ((frame & 3) == 0) {
      if (frame & 4)
        Iwram_CopyWords((void *)0x060052c0, gMenu_BackdropFrameTile, 32);
      else
        Iwram_FillWords((void *)0x060052c0, 32, 0x44444444);
    }
    if (pressedKeys & KEY_START) {
      AudioCommand_PlayFar(SE_MENU_CANCEL);
      result = -2;
      break;
    }
    if (pressedKeys & (KEY_A | KEY_B | KEY_R | KEY_L)) {
      AudioCommand_PlayFar(SE_MENU_CANCEL);
      result = -1;
      break;
    }
    if (repeatedKeys & KEY_LEFT) {
      selectedPages[side]--;
      selectedPages[side] = Menu_WrapIndex(selectedPages[side], counts[side]);
      AudioCommand_PlayFar(SE_MENU_CURSOR_MOVE);
      Input_ResetKeyRepeat();
      redraw = 1;
    } else if (repeatedKeys & KEY_RIGHT) {
      selectedPages[side]++;
      AudioCommand_PlayFar(SE_MENU_CURSOR_MOVE);
      Input_ResetKeyRepeat();
      redraw = 1;
      selectedPages[side] = Menu_WrapIndex(selectedPages[side], counts[side]);
    }
    WaitFrames(1);
  }
  Scheduler_AddOrUpdateCallback(Menu_UpdateEntryObjectTransforms, 0xc80);
  ((struct UiRenderWork *)gWindowWork[0])->menuBusy = 1;
  UiWindow_CloseIfOpen(&menu->helpWindow, 1);
  WaitFrames(1);
  UiWindow_UpdateOrCreate(&menu->helpWindow, 13, 0, 17, 5, 2);
  UiWindow_CloseIfOpen(leftWindowSlot, 1);
  UiWindow_CloseIfOpen(rightWindowSlot, 1);
  RenderOutput_RedrawSavedRectFar(menu->listWindow);
  RenderOutput_RedrawSavedRectFar((struct UiWindow *)menu->unknownWindow028);
  RenderOutput_RedrawSavedRectFar(menu->statusWindow);
  ((struct UiRenderWork *)gWindowWork[0])->menuBusy = 0;
  WaitFrames(1);
  return result;
}

/// Marks recipients that would leave unequal Djinn totals after a direct give.
/// Returns how many recipients would need a trade instead.
s32 Djinn_MarkBalancedEntries(s8 *requiresTrade, s32 sourceOwnerColumn) {
  s32 tradeCount;
  s32 ownerColumn;
  s8 *entry;
  struct DjinnMenuWork *menu;

  menu = gMenuWork;
  tradeCount = 0;
  ownerColumn = 0;
  if (tradeCount < menu->ownerCount) {
    entry = requiresTrade;
    do {
      *entry = 0;
      if (ownerColumn != sourceOwnerColumn) {
        if (Djinn_CheckTurnBalance(sourceOwnerColumn, ownerColumn) == 0) {
          *entry = 1;
          tradeCount += 1;
        }
      }
      ownerColumn += 1;
      entry += 1;
    } while (ownerColumn < menu->ownerCount);
  }
  return tradeCount;
}

/// Tests whether a hypothetical give leaves every pair of owners within one
/// Djinn.
s32 Djinn_CheckTurnBalance(s32 sourceOwnerColumn, s32 targetOwnerColumn) {
  struct DjinnMenuWork *menu;
  s8 ownedCounts[16];
  u8 firstOwner;
  u8 secondOwner;
  s32 difference;
  s32 balanced;

  menu = gMenuWork;
  Djinn_CountTurns((u8 *)ownedCounts);
  ownedCounts[sourceOwnerColumn] -= 1;
  ownedCounts[targetOwnerColumn] += 1;
  balanced = 1;
  for (firstOwner = 0; firstOwner < menu->ownerCount; firstOwner++) {
    secondOwner = firstOwner;
    if (firstOwner < menu->ownerCount) {
      while (1) {
        secondOwner++;
        if (secondOwner >= menu->ownerCount)
          break;
        difference = ownedCounts[firstOwner] - ownedCounts[secondOwner];
        if (difference < -1 || difference > 1) {
          balanced = 0;
          break;
        }
      }
    }
  }
  return balanced;
}

/// Counts each owner's Djinn across its set and available bitmasks.
void Djinn_CountTurns(u8 *ownedCounts) {
  struct DjinnMenuWork *menu;
  struct BattleUnit *unit;
  s32 ownerIndex;
  s32 element;
  s32 djinnIndex;
  s32 ownedCount;

  menu = gMenuWork;
  for (ownerIndex = 0; ownerIndex < menu->ownerCount; ownerIndex++) {
    unit = UnitGetFar(menu->owners[ownerIndex]);
    ownedCount = 0;
    for (element = 0; element <= 3; element++) {
      for (djinnIndex = 0; djinnIndex <= 19; djinnIndex++) {
        if (unit->djinnActive[element] & (1 << djinnIndex))
          ownedCount++;
        else if (unit->djinnAvailable[element] & (1 << djinnIndex))
          ownedCount++;
      }
    }
    ownedCounts[ownerIndex] = ownedCount;
  }
}
