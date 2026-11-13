/// @file
/// Previewing a change before it is made, and the check that keeps the
/// party's Djinn balanced.
// Part of menu.c.

#include "../menu_internal.h"

void Func_08104e38(s32 first, s32 second);

s32 PsynergyList_RunCompareLoop(u32 mode) {
  struct DjinnMenuWork *menu;
  struct DjinnMenuBackdrop *backdrop;
  struct WindowWork *work;
  struct DjinnPreviewWindow *list;
  s32 pageCounts[2];
  s32 selectedPages[2];
  u16 owners[14];
  s32 confirmationChoice;
  u32 redraw;
  s32 frame;
  s32 page;
  s32 help;
  s32 previewMode;
  s32 addPairSlot;
  s32 step;
  s32 result;
  s32 index;
  s32 memberIndex;
  s32 first;
  s32 second;
  u32 entry0;
  u32 entry1;
  s32 row;

  menu = Ram_HeapSlots->menuRuntime;
  confirmationChoice = 0;
  result = 0;
  redraw = 1;
  frame = 0;
  page = 2;
  help = menu->helpWindow;
  menu->cursor->active = 13;
  selectedPages[0] = 0;
  selectedPages[1] = 0;
  Party_ListMembersFar(owners);
  first = owners[menu->selectedColumn[0]];
  second = owners[menu->selectedColumn[1]];
  Func_08104e38(first, second);
  for (memberIndex = 3; memberIndex >= 0; memberIndex--)
    menu->memberY[memberIndex] = 200;
  menu->memberX[0] = 16;
  menu->memberY[0] = 32;
  if (first != second) {
    menu->memberX[1] = 120;
    menu->memberY[1] = 32;
  }
  WindowSprite_ClearListFar(menu->listWindow);
  wait(1);
  Window_CloseIfOpen(&menu->helpWindow, 1);
  Window_UpdateOrCreate(&menu->helpWindow, 0, 0, 30, 5, 2);
  pageCounts[0] = 1;
  pageCounts[1] = 1;
  if (mode <= 1) {
    u16 *diffWork;
    u8 *original;
    u8 *unit;
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

    diffWork = Heap_AllocEwram(96);
    size = 332;
    original = Heap_AllocEwram(size);
    secondElement = 0;
    secondIndex = 0;
    memberId = menu->shownMember[0];
    firstElement = menu->pairElement[0];
    firstIndex = menu->pairIndex[0] & 31;
    firstWasSet = (u16)(menu->pairEntries[0] & 0x8000);
    unit = Unit_Get(memberId);
    Iwram_CopyWords(original, unit, size);
    Djinn_PutOnStandbyFar(memberId, firstElement, firstIndex);
    if (mode == 0) {
      secondElement = menu->pairElement[1];
      secondIndex = menu->pairIndex[1] & 31;
      secondWasSet = (u16)(menu->pairEntries[1] & 0x8000);
      Djinn_GiveFar(memberId, secondElement, secondIndex);
      if (secondWasSet != 0)
        Djinn_SetFar(memberId, secondElement, secondIndex);
    }
    Unit_CalculateFar(memberId);
    pageCounts[0] =
        PsynergyList_Diff((u16 *)(original + 88), (u16 *)(unit + 88), diffWork,
                          &gainedCount, &lostCount);
    Iwram_CopyWords(unit, original, size);

    memberId = menu->shownMember[1];
    unit = Unit_Get(memberId);
    Iwram_CopyWords(original, unit, size);
    if (mode == 0)
      Djinn_PutOnStandbyFar(memberId, secondElement, secondIndex);
    Djinn_GiveFar(memberId, firstElement, firstIndex);
    if (firstWasSet != 0)
      Djinn_SetFar(memberId, firstElement, firstIndex);
    Unit_CalculateFar(memberId);
    pageCounts[1] =
        PsynergyList_Diff((u16 *)(original + 88), (u16 *)(unit + 88), diffWork,
                          &gainedCount, &lostCount);
    Iwram_CopyWords(unit, original, size);
    Heap_Release(original);
    Heap_Release(diffWork);
  } else if (mode == 2 || mode == 3) {
    u16 *diffWork;
    u8 *original;
    u8 *unit;
    s32 memberId;
    s32 firstElement;
    s32 firstIndex;
    s32 applySecond;
    s32 lostCount;
    s32 gainedCount;

    diffWork = Heap_AllocEwram(96);
    original = Heap_AllocEwram(332);
    memberId = menu->shownMember[0];
    firstElement = menu->pairElement[0];
    firstIndex = menu->pairIndex[0] & 31;
    applySecond = (mode == 3);
    unit = Unit_Get(memberId);
    Iwram_CopyWords(original, unit, 332);
    Djinn_PutOnStandbyFar(memberId, firstElement, firstIndex);
    if (applySecond != 0)
      Djinn_SetFar(memberId, firstElement, firstIndex);
    Unit_CalculateFar(memberId);
    pageCounts[0] =
        PsynergyList_Diff((u16 *)(original + 88), (u16 *)(unit + 88), diffWork,
                          &gainedCount, &lostCount);
    Iwram_CopyWords(unit, original, 332);
    pageCounts[1] = pageCounts[0];
    page = 0;
    Heap_Release(original);
    Heap_Release(diffWork);
  }
  pageCounts[0] = (pageCounts[0] - 1) / 5 + 1;
  if (pageCounts[0] == 0)
    pageCounts[0] = 1;
  pageCounts[1] = (pageCounts[1] - 1) / 5 + 1;
  if (pageCounts[1] == 0)
    pageCounts[1] = 1;
  Window_UpdateOrCreate(&menu->window, 0, 5, 15, 15, 2);
  Window_UpdateOrCreate(&menu->secondWindow, 15, 5, 15, 15, 2);
  WindowSprite_RedrawSavedRectFar(help);
  if (mode == 2) {
    Text_DrawMessageAsTilesFar((s32)MsgDjinnCompareStandby, help, 96, 0);
    Text_DrawMessageAsTilesFar((s32)MsgDjinnCompareConfirm, help, 96, 16);
  } else if (mode == 3) {
    Text_DrawMessageAsTilesFar((s32)MsgDjinnCompareSet, help, 96, 0);
    Text_DrawMessageAsTilesFar((s32)MsgDjinnCompareConfirm, help, 96, 16);
  } else if (mode == 0) {
    Text_DrawMessageAsTilesFar((s32)MsgDjinnCompareTrade, help, 128, 0);
    Text_DrawMessageAsTilesFar((s32)MsgDjinnCompareConfirm, help, 128, 8);
  } else {
    Text_DrawMessageAsTilesFar((s32)MsgDjinnCompareGive, help, 128, 0);
    Text_DrawMessageAsTilesFar((s32)MsgDjinnCompareConfirm, help, 128, 8);
  }
  row = 0;
  entry0 = menu->pairEntries[0];
  entry1 = menu->pairEntries[1];
  if (mode == 1)
    row = 1;
  if ((entry0 & 0x8000) == 0)
    Text_SetColorFar(2);
  Func_08101a54(help, 40, row * 8, entry0);
  if (mode == 0) {
    if ((entry1 & 0x8000) == 0)
      Text_SetColorFar(2);
    Func_08101a54(help, 40, 16, entry1);
  } else if (mode - 2 <= 1) {
    if ((entry0 & 0x8000) != 0)
      Text_SetColorFar(2);
    Func_08101a54(help, 40, 16, entry0);
    Window_SetTilemapEntryFar(help, 0xf296, 7, 1, 0);
  }

  for (;;) {
    u32 pressed;
    u32 repeat;

    work = (struct WindowWork *)Ram_HeapSlots->windowTiles;
    backdrop = menu->backdrop;
    pressed = gInput.pressed;
    repeat = gInput.repeat;
    if (backdrop->tutorialStep != 0) {
      u8 *saved;
      s32 size;

      size = 0x200;
      saved = Heap_AllocEwram(size);
      pressed = 0;
      repeat = 0;
      backdrop->tutorialTimer++;
      switch (backdrop->tutorialStep) {
      case 5:
      case 12:
      case 23:
        if (backdrop->tutorialTimer == 100) {
          repeat = 1;
          pressed = 1;
          backdrop->tutorialTimer = 0;
          backdrop->tutorialStep++;
        }
        break;
      case 10:
        if (backdrop->tutorialTimer == 60) {
          s32 window;

          Iwram_CopyWords(saved, work->tilemap, size);
          window =
              Message_OpenWindowFar(Menu_GetDjinnGiveMessage() + 3, 8, 0, 1);
          /* FAKEMATCH(dead): the cursor's store in a once-only block, as The
           * Broken Seal's twin has it; as a plain statement, 4 of 1244
           * instructions differ in tla-en. */
          do {
            menu->cursor->active = 1;
          } while (0);
          MenuCursor_Slide(2, 96);
          gMainRoutineWork.eventMode = 1;
          while (Message_AllTypedFar() == 0)
            wait(1);
          Window_CloseFar(window, 1);
          Iwram_CopyWords(work->tilemap, saved, size);
          Func_08038290();
          work->dirty = 1;
          redraw = 1;
          backdrop->tutorialTimer = 0;
          backdrop->tutorialStep = 11;
          /* FAKEMATCH(dead): the cursor's store in a once-only block, as The
           * Broken Seal's twin has it; as a plain statement, 2 of 1244
           * instructions differ in tla-en. */
          do {
            menu->cursor->active = 13;
          } while (0);
        }
        break;
      case 11:
        if (backdrop->tutorialTimer == 60) {
          s32 window;

          Iwram_CopyWords(saved, work->tilemap, size);
          window =
              Message_OpenWindowFar(Menu_GetDjinnGiveMessage() + 2, 8, 0, 1);
          /* FAKEMATCH(dead): the cursor's store in a once-only block, as The
           * Broken Seal's twin has it; as a plain statement, 4 of 1244
           * instructions differ in tla-en. */
          do {
            menu->cursor->active = 1;
          } while (0);
          MenuCursor_Slide(106, 56);
          gMainRoutineWork.eventMode = 1;
          while (Message_AllTypedFar() == 0)
            wait(1);
          Window_CloseFar(window, 1);
          Iwram_CopyWords(work->tilemap, saved, size);
          Func_08038290();
          work->dirty = 1;
          wait(1);
          redraw = 1;
          backdrop->tutorialTimer = 0;
          backdrop->tutorialStep = 12;
          /* FAKEMATCH(dead): the cursor's store in a once-only block, as The
           * Broken Seal's twin has it; as a plain statement, 2 of 1244
           * instructions differ in tla-en. */
          do {
            menu->cursor->active = 13;
          } while (0);
        }
        break;
      }
      Heap_Release(saved);
    }

    if (redraw != 0) {
      addPairSlot = 1;
      if (mode <= 1) {
        Window_ClearInteriorTilesFar(help, 128, 16, 224, 24);
        if (page == 1) {
          Text_DrawMessageAsTilesFar((s32)MsgDjinnStatusHelp, help, 128, 16);
        } else {
          MessageArgument_PushFar(menu->shownMember[page != 2], 1);
          Text_DrawMessageAsTilesFar((s32)MsgDjinnPsyHelp, help, 128, 16);
        }
      }
      work->menuBusy = 1;
      WindowSprite_PrepareForRedrawFar(menu->window);
      WindowSprite_PrepareForRedrawFar((s32)menu->secondWindow);
      if ((redraw >> 1) != 0)
        wait(1);
      if (mode == 3) {
        previewMode = 0;
        addPairSlot = 0;
      } else if (mode == 2) {
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
        if (mode == 1)
          previewMode = 4;
        DjinnMenu_DrawStatPreview(menu->window, 0, 0, menu->shownMember[1], page, 0,
                      previewMode, 0, 0);
        DjinnMenu_DrawStatPreview((s32)menu->secondWindow, 0, 0, menu->shownMember[1], page,
                      0, previewMode, selectedPages[1] + 1, 0);
        break;
      case 0:
        if (mode == 1) {
          previewMode = 1;
          addPairSlot = 0;
        }
        DjinnMenu_DrawStatPreview(menu->window, 0, 0, menu->pairMember[0], page,
                      addPairSlot, previewMode, page, 1);
        DjinnMenu_DrawStatPreview((s32)menu->secondWindow, 0, 0, menu->pairMember[0], page,
                      addPairSlot, previewMode, selectedPages[0] + 1, 1);
        break;
      }
      work->menuBusy = 0;
    }

    if (page <= 1) {
      list = menu->secondWindow;
      if (pageCounts[page] > 1) {
        for (index = 0; index < pageCounts[page]; index++) {
          s32 tile;

          tile = 0xf031 + index;
          if (index > 8)
            tile = 0xf03f;
          if (index == selectedPages[page])
            tile -= 0x1000;
          Window_SetTilemapEntryFar((s32)list, tile,
                                    list->width - pageCounts[page] + index - 2,
                                    -1, 0);
        }
        Window_SetTilemapEntryFar((s32)list, 0xf128,
                                  list->width - pageCounts[page] - 3, -1, 0);
        Window_SetTilemapEntryFar((s32)list, 0xf129, list->width - 2, -1, 0);
        UiRender_MarkRow(work, list->y);
      }
    }

    frame++;
    step = frame % 60 - 5;
    if (step < 0)
      step = 0;
    if (step > 29)
      step = 29;
    Func_08105350(0, 0xffff4000);
    Func_08105350(1, 0xffff4000);
    if (mode <= 1) {
      s32 rise;
      s32 slide;

      rise = -Iwram_MulQ16(16, Trig_Sin(step % 30 * 0x444));
      if (rise < -6)
        rise = -6;
      if (rise > 12)
        rise = 12;
      slide = step % 35 * 2;
      MenuIcon_SetPosition(0, slide + 34, rise + 20);
      if (mode == 0)
        MenuIcon_SetPosition(1, 99 - slide, 36 - rise);
    } else {
      MenuIcon_SetPosition(0, 32, 30);
    }
    if (redraw != 0) {
      redraw = 0;
      confirmationChoice = Menu_GetModuloOfSum(confirmationChoice, 2);
    }
    Func_080f8a44(confirmationChoice * 56 + 80, 16);
    if ((frame & 3) == 0) {
      /* FAKEMATCH(wrapper): the copy forwarded through shared.c's CopyWords
       * inline, so the source loads before the routine and the destination,
       * as the native does; as a direct call, 6 of 1244 instructions differ
       * in tla-en. */
      if ((frame & 4) != 0)
        CopyWords(gMenu_BackdropBorderTile, Iwram_CopyWords, (void *)0x060052c0,
                  32);
      else
        /* FAKEMATCH(wrapper): the fill forwarded through shared.c's FillWords
         * inline, so the routine loads before its arguments; as a direct
         * call, 4 of 1244 instructions differ in tla-en. */
        FillWords(Iwram_FillWords, (void *)0x060052c0, 32, 0x44444444);
    }

    if ((pressed & 1) != 0) {
      if (confirmationChoice == 0) {
        result = 1;
        break;
      }
      Sound_PlayFar(113);
      result = -1;
      break;
    }
    if ((pressed & 8) != 0) {
      Sound_PlayFar(113);
      result = -2;
      break;
    } else if ((pressed & 2) != 0) {
      Sound_PlayFar(113);
      result = -1;
      break;
    } else if ((pressed & 0x100) != 0) {
      if (mode <= 1) {
        page = (page + 1) % 3;
        redraw = 2;
        Sound_PlayFar(111);
      }
    } else if ((repeat & 0x20) != 0) {
      if (page <= 1) {
        selectedPages[page]--;
        selectedPages[page] =
            Menu_GetModuloOfSum(selectedPages[page], pageCounts[page]);
        Sound_PlayFar(111);
        Input_ResetKeyRepeat();
        redraw = 1;
      }
    } else if ((repeat & 0x10) != 0) {
      if (page <= 1) {
        Sound_PlayFar(111);
        Input_ResetKeyRepeat();
        selectedPages[page]++;
        redraw = 1;
        selectedPages[page] =
            Menu_GetModuloOfSum(selectedPages[page], pageCounts[page]);
      }
    }
    wait(1);
  }

  Func_08105350(0, 0);
  Func_08105350(1, 0);
  Task_Add(Menu_UpdateEntryObjectTransforms, 0x480);
  ((struct WindowWork *)Ram_HeapSlots->windowTiles)->menuBusy = 1;
  Window_CloseIfOpen(&menu->helpWindow, 1);
  wait(1);
  Window_UpdateOrCreate(&menu->helpWindow, 13, 0, 17, 5, 2);
  Window_CloseIfOpen(&menu->window, 1);
  Window_CloseIfOpen(&menu->secondWindow, 1);
  WindowSprite_RedrawSavedRectFar(menu->listWindow);
  WindowSprite_RedrawSavedRectFar(menu->unopenedWindow);
  WindowSprite_RedrawSavedRectFar(menu->statusWindow);
  ((struct WindowWork *)Ram_HeapSlots->windowTiles)->menuBusy = 0;
  wait(1);
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
  s32 redraw;
  s32 frame;
  void *leftWindowSlot;
  void *rightWindowSlot;
  s32 *helpWindowSlot;
  struct WindowWork *render;
  u32 pressedKeys;
  u32 repeatedKeys;
  struct DjinnMenuWork *menu;
  struct OwnerActionState *unit;
  struct DjinnPreviewWindow *window;
  u16 *listedActions;
  void *saved;
  s32 result;
  s32 tile;
  s32 pageIndex;
  s32 side;
  u8 *pairMember;
  s32 *counts;
  s32 *selected;
  s32 blink;

  menu = Ram_HeapSlots->menuRuntime;
  redraw = 1;
  result = 0;
  frame = 0;
  menu->cursor->active = 13;
  /* FAKEMATCH(temporary): selected = selectedPages, the page array read through
   * a pointer; as selectedPages itself, 12 of 436 instructions differ in
   * tla-en. */
  selected = selectedPages;
  selected[1] = selected[0] = result;
  for (side = 3; side >= 0; side--)
    menu->memberY[side] = 200;
  WindowSprite_ClearListFar(menu->listWindow);
  wait(1);
  /* FAKEMATCH(temporary): counts = pageCounts, the count array read through a
   * pointer; as pageCounts itself, 55 of 436 instructions differ in tla-en. */
  counts = pageCounts;
  counts[1] = counts[0] = 1;
  listedActions = Heap_AllocEwram(96);
  saved = Heap_AllocEwram(332);
  unit = Unit_Get(menu->shownMember[0]);
  counts[1] = counts[0] = PsynergyList_Diff(
      unit->actionSlots, unit->actionSlots, listedActions, &gained, &lost);
  Heap_Release(saved);
  Heap_Release(listedActions);
  counts[0] = (counts[0] - 1) / 6 + 1;
  if (counts[0] == 0)
    counts[0] = 1;
  counts[1] = (counts[1] - 1) / 6 + 1;
  if (counts[1] == 0)
    counts[1] = 1;
  leftWindowSlot = &menu->window;
  Window_UpdateOrCreate(leftWindowSlot, 0, 5, 15, 15, 2);
  rightWindowSlot = &menu->secondWindow;
  Window_UpdateOrCreate(rightWindowSlot, 15, 5, 15, 15, 2);
  helpWindowSlot = &menu->helpWindow;
  WindowSprite_RedrawSavedRectFar(*helpWindowSlot);
  WindowSprite_RedrawSavedRectFar(menu->statusWindow);
  Text_DrawMessageAsTilesFar((s32)MsgCurrentStatus, menu->statusWindow, 0,
                                0);
  Text_DrawMessageAsTilesFar((s32)MsgCurrentStatus + 2, menu->statusWindow,
                                0, 16);
  side = 0;
  while (1) {
    s32 *pageCount;
    s32 *selectedPage;
    /* FAKEMATCH(temporary): selectedPage = &selected[side], taken at the top of
     * the loop; as selected[side] at each page turn, 22 of 436 instructions
     * differ in tla-en. */
    selectedPage = &selected[side];
    render = (struct WindowWork *)Ram_HeapSlots->windowTiles;
    pressedKeys = gInput.pressed;
    repeatedKeys = gInput.repeat;
    /* FAKEMATCH(temporary): pageCount = &pageCounts[side], taken at the top of
     * the loop; as pageCounts[side] at each page turn, 227 of 436 instructions
     * differ in tla-en. */
    pageCount = &pageCounts[side];
    pairMember = menu->pairMember;
    if (redraw) {
      render->menuBusy = 1;
      WindowSprite_PrepareForRedrawFar(menu->window);
      WindowSprite_PrepareForRedrawFar((s32)menu->secondWindow);
      DjinnMenu_DrawStatPreview(menu->window, 0, 0, *pairMember, 0, 0, 3, 0, 1);
      DjinnMenu_DrawStatPreview((s32)menu->secondWindow, 0, 0, *pairMember, 0, 0, 3,
                    selectedPages[0] + 1, 1);
      render->menuBusy = 0;
    }
    if (counts[side] > 1) {
      window = menu->secondWindow;
      for (pageIndex = 0; pageIndex < counts[side]; pageIndex++) {
        tile = 0xf031 + pageIndex;
        if (pageIndex > 8)
          tile = 0xf03f;
        if (pageIndex == selected[side])
          tile -= 0x1000;
        Window_SetTilemapEntryFar((s32)window, tile,
                                  window->width - counts[side] + pageIndex - 2,
                                  -1, 0);
      }
      Window_SetTilemapEntryFar((s32)window, 0xf128,
                                window->width - counts[side] - 3, -1, 0);
      Window_SetTilemapEntryFar((s32)window, 0xf129, window->width - 2, -1, 0);
      UiRender_MarkRow(render, window->y);
    }
    frame++;
    blink = frame % 60 - 5;
    CLAMP(blink, 0, 29);
    MenuIcon_SetPosition(0, 32, 200);
    // DEAD: the test cannot change the result, redraw = 0 alone does the same;
    // the ROM keeps the compare (its twin also steps a phase under it).
    if (redraw)
      redraw = 0;
    if ((frame & 3) == 0) {
      if (frame & 4)
        Iwram_CopyWords((void *)0x060052c0, gMenu_BackdropBorderTile, 32);
      else
        Iwram_FillWords((void *)0x060052c0, 32, 0x44444444);
    }
    if (pressedKeys & KEY_START) {
      Sound_PlayFar(113);
      result = -2;
      break;
    }
    if (pressedKeys & (KEY_A | KEY_B | KEY_R | KEY_L)) {
      Sound_PlayFar(113);
      result = -1;
      break;
    }
    if (repeatedKeys & KEY_LEFT) {
      (*selectedPage)--;
      *selectedPage = Menu_GetModuloOfSum(*selectedPage, *pageCount);
      Sound_PlayFar(111);
      Input_ResetKeyRepeat();
      redraw = 1;
    } else if (repeatedKeys & KEY_RIGHT) {
      (*selectedPage)++;
      *selectedPage = Menu_GetModuloOfSum(*selectedPage, *pageCount);
      Sound_PlayFar(111);
      Input_ResetKeyRepeat();
      redraw = 1;
    }
    wait(1);
  }
  {
    // A local holding one constant: as a literal argument, 2 of 436
    // instructions differ in tla-en.
    s32 priority = TASK_PASS_VBLANK;
    Task_Add(Menu_UpdateEntryObjectTransforms, priority);
  }
  ((struct WindowWork *)Ram_HeapSlots->windowTiles)->menuBusy = 1;
  Window_CloseIfOpen(helpWindowSlot, 1);
  wait(1);
  Window_UpdateOrCreate(helpWindowSlot, 13, 0, 17, 5, 2);
  Window_CloseIfOpen(leftWindowSlot, 1);
  Window_CloseIfOpen(rightWindowSlot, 1);
  WindowSprite_RedrawSavedRectFar(menu->listWindow);
  WindowSprite_RedrawSavedRectFar(menu->unopenedWindow);
  WindowSprite_RedrawSavedRectFar(menu->statusWindow);
  ((struct WindowWork *)Ram_HeapSlots->windowTiles)->menuBusy = 0;
  wait(1);
  return result;
}

s32 Djinn_CheckGiveBalance(s32 self, s32 other);

/// Marks in table each other owner a djinn can move to from self and keep the
/// party's djinn balanced, and returns how many there are.
// The Broken Seal's Djinn_MarkBalancedEntries.
/// Marks recipients that would leave unequal Djinn totals after a direct give.
/// Returns how many recipients would need a trade instead.
///
/// requiresTrade[i] becomes 1 for each other party member i who cannot take one
/// of sourceMemberColumn's Djinn without two party members' totals differing by
/// more than one (Djinn_CheckGiveBalance), else 0. The second Djinn chooser
/// offers such a party member only Trade.
s32 Djinn_MarkTradeOnlyRecipients(s8 *requiresTrade, s32 sourceMemberColumn) {
  s32 cnt;
  s32 i;
  s8 *p;
  struct DjinnMenuWork *state;

  state = Ram_HeapSlots->menuRuntime;
  cnt = 0;
  i = 0;
  if (cnt < state->memberCount) {
    p = requiresTrade;
    do {
      *p = 0;
      if (i != sourceMemberColumn) {
        if (Djinn_CheckGiveBalance(sourceMemberColumn, i) == 0) {
          *p = 1;
          cnt += 1;
        }
      }
      i += 1;
      p += 1;
    } while (i < state->memberCount);
  }
  return cnt;
}

void Djinn_CountOwned(u8 *counts);

// Test whether moving one from from to to preserves the balance.
s32 Djinn_CheckGiveBalance(s32 from, s32 to) {
  s8 counts[16];
  s32 balanced;

  Djinn_CountOwned((u8 *)counts);
  counts[from] -= 1;
  counts[to] += 1;
  balanced = 1;
  if (counts[from] + 2 <= counts[to])
    balanced = 0;
  return balanced;
}

// Djinn_CountOwned: The Broken Seal's Djinn_CountTurns.

/// Counts each party member's Djinn across its set and available bitmasks:
/// writes every party member's total, set or not, into ownedCounts by list
/// column.
void Djinn_CountOwned(u8 *counts) {
  struct DjinnMenuWork *work;
  struct DjinnUnit *unit;
  s32 ownerIndex;
  s32 element;
  s32 bit;
  s32 count;

  work = Ram_HeapSlots->menuRuntime;
  for (ownerIndex = 0; ownerIndex < work->memberCount; ownerIndex++) {
    unit = Unit_Get(work->owners[ownerIndex]);
    count = 0;
    for (element = 0; element <= 3; element++) {
      for (bit = 0; bit <= 19; bit++) {
        if (unit->djinnSet[element] & (1 << bit))
          count++;
        else if (unit->djinnHeld[element] & (1 << bit))
          count++;
      }
    }
    counts[ownerIndex] = count;
  }
}
