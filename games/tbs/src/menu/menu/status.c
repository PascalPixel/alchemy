// Part of menu.c, the menu object, compiled only through it.
// The member chooser and the Status menu: its member row, status page,
// Psynergy page and equipment page.

#include "../menu_internal.h"

// The international editions show eight party figure rows, their longer
// words need more window, and their larger numbers need fewer digits.
//
// How many party figure rows Menu_OpenCharacterSelector resets.
#if TBS_PATCH_1
#define CHARACTER_SELECTOR_OWNER_COUNT 8
#else
#define CHARACTER_SELECTOR_OWNER_COUNT 4
#endif

// The window tiles the Status menu saves and restores.
#if TBS_PATCH_1
#define STATUS_SAVED_TILE_BYTES 0x2000
#else
#define STATUS_SAVED_TILE_BYTES 0x800
#endif

// The longest status help text, in characters.
#if TBS_PATCH_1
#define STATUS_HELP_TEXT_MAX 0x80
#else
#define STATUS_HELP_TEXT_MAX 0x40
#endif

// The stat column of the status page's top rows.
#if TBS_PATCH_1
#define STATUS_STAT_DIGITS 3
#define STATUS_STAT_X 200
#else
#define STATUS_STAT_DIGITS 4
#define STATUS_STAT_X 192
#endif

// Items to a page of the equipment page's list.
#if TBS_PATCH_1
#define STATUS_EQUIP_PAGE_SIZE 5
#else
#define STATUS_EQUIP_PAGE_SIZE 6
#endif

// The hand cursor on the Psynergy page.
#if TBS_PATCH_1
#define STATUS_PSYNERGY_CURSOR_X 55
#define STATUS_PSYNERGY_CURSOR_Y 60
#else
#define STATUS_PSYNERGY_CURSOR_X 58
#define STATUS_PSYNERGY_CURSOR_Y 52
#endif

// The highlight over the equipment window's four slot rows.
#if TBS_PATCH_1
#define STATUS_EQUIP_SLOT_X 1
#define STATUS_EQUIP_SLOT_WIDTH 12
#else
#define STATUS_EQUIP_SLOT_X 2
#define STATUS_EQUIP_SLOT_WIDTH 9
#endif

// The equipment page's item list: its page icons, the item names, and the
// Attack and Defense figures.
#if TBS_PATCH_1
#define SHOP_PAGE_ICONS_X 119
#define SHOP_ITEM_NAME_X 128
#define SHOP_STAT_X 80
#else
#define SHOP_PAGE_ICONS_X 123
#define SHOP_ITEM_NAME_X 136
#define SHOP_STAT_X 72
#endif

#if LANGUAGE == LANGUAGE_GERMAN
#define SHOP_EMPTY_MESSAGE_X 112
#else
#define SHOP_EMPTY_MESSAGE_X 120
#endif

// The equipment window, and the equipped items' names in it.
#if TBS_PATCH_1
#define STATUS_EQUIP_WINDOW_WIDTH 15
#define STATUS_EQUIPPED_NAME_X 8
#else
#define STATUS_EQUIP_WINDOW_WIDTH 13
#define STATUS_EQUIPPED_NAME_X 16
#endif

// Lets the player choose one party member, over the field (the Colosso scenes
// use it). Returns the chosen member's id, or -1 when the player backs out.
s32 Menu_OpenCharacterSelector(void) {
  struct CharacterMenuState *state =
      Runtime_AllocateHeapBlock(HEAP_SLOT_MENU, MENU_WORK_SIZE);
  s32 result;
  s32 index;

  gMenuCtrlWork->suspended = 1;
  UiWindow_DrawFrameFar(0, 0, 30, 20);
  WaitFrames(1);
  UiWindow_InitializeWork(0);

  state->partyCount = Party_ListActiveOwnersFar(state->ownerIds);
  Menu_InitSelectorCursorAndEntries(0, 3, 0, 7);
  state->selectorWindow = UiWindow_CreateFar(13, 0, 17, 5, 2);
  for (index = 0; index < CHARACTER_SELECTOR_OWNER_COUNT; index++)
    state->ownerY[index] = 30;
  state->page = 3; // the member chooser

  result = CharacterSelector_Run();

  RenderOutput_ClearListFar(state->statusWindow);
  ItemMenu_Close();
  gMenuCtrlWork->suspended = 0;
  WaitFrames(1);
  Runtime_ReleaseHeapBlock(HEAP_SLOT_MENU);
  return result;
}

// The member chooser's loop. Returns the chosen member's id, or -1.
s32 CharacterSelector_Run(void) {
  struct CharacterMenuState *work = gMenuWork;
  s32 result = 0;

  /* FAKEMATCH(one-pass): the slot's clearing in a one-pass block, which ends
   * the schedule after the work's load so that the zero is stored before the
   * slot's offset is built; as a plain statement, 4 of 23 instructions differ
   * in tbs-en. */
  do {
    work->selectedSlots[0] = result;
  } while (0);
  if (CharacterMenu_SelectOwner(0) == -1)
    result = -1;
  else
    result = work->selectedIds[0];
  return result;
}

// The Status menu an event opens (window command 4) over the field: saves the
// first two background palettes and the window tiles it draws over, sets up
// the party row and, when the party has Djinn, the four Djinn sprites, runs
// the status pages, then puts everything back. Returns their result.
s32 StatusMenu_Run(void) {
  struct CharacterMenuState *state =
      Runtime_AllocateHeapBlock(HEAP_SLOT_MENU, MENU_WORK_SIZE);
  void *palette = Runtime_BumpAllocateAlternatePool(64);
  void *tiles = Runtime_BumpAllocateAlternatePool(STATUS_SAVED_TILE_BYTES);
  s32 result;
  s32 index;

  ((struct ObjectSystemWork *)WORK_SLOT(HEAP_SLOT_MENU_CONTROL))->suspended = 1;
  UiWindow_DrawFrameFar(0, 0, 30, 20);
  WaitFrames(1);
  Scheduler_EnableOverlayCallbacksWithFlags();
  UiWindow_InitializeWork(0);
  state->page = 0;
  state->partyCount = Party_ListActiveOwnersFar(state->ownerIds);
  Menu_InitSelectorCursorAndEntries(0, 3, 0, 7);
  Iwram_CopyWords(palette, (void *)BG_PLTT, 64);
#if !TBS_PATCH_1
  Iwram_CopyWords(tiles, BG_CHAR_BLOCK(1), STATUS_SAVED_TILE_BYTES);
#endif
  Palette_LightenBankHighlight(14);
  // The first sprite palette into background palettes 0 and 1, each with a
  // colour of palette 15 as its colour 14.
  Dma_Set((void *)OBJ_PLTT, (void *)BG_PLTT, DMA_ENABLE << 16 | 16, REG_DMA3);
  Dma_Set((void *)&BG_PLTT_COLOR(14, 4), (void *)&BG_PLTT_COLOR(0, 14),
          DMA_ENABLE << 16 | 1, REG_DMA3);
  Dma_Set((void *)OBJ_PLTT, (void *)&BG_PLTT_COLOR(1, 0), DMA_ENABLE << 16 | 16,
          REG_DMA3);
  Dma_Set((void *)&BG_PLTT_COLOR(15, 4), (void *)&BG_PLTT_COLOR(1, 14),
          DMA_ENABLE << 16 | 1, REG_DMA3);
#if TBS_PATCH_1
  Iwram_CopyWords(tiles, BG_CHAR_BLOCK(1), STATUS_SAVED_TILE_BYTES);
  Iwram_FillWords(BG_CHAR_BLOCK(1), STATUS_SAVED_TILE_BYTES, 0x33333333);
  UiWork_SetAltFlagAndClearTableFar(1);
#endif
  state->selectorWindow = UiWindow_CreateFar(13, 0, 17, 5, 2);
  for (index = 0; index < MENU_ROW_COUNT; index++)
    state->ownerY[index] = 30;
  if (Party_SumDjinnCountsFar(-1) != 0)
    FourObjectMotion_InitializeTopRow(state->selectorWindow, 0);
  for (index = 0; index < 4; index++) {
    state->rowX[index] = 130 + index * 32;
    state->rowY[index] = 128;
  }
  Link_DrawShiftedTilePairFar(UI_PAGE_ARROW_VRAM);
  // Run the pages.
  Menu_CancelSoundReset();
  state->page = 0;

  result = Menu_RunActionFlow();

  // Close, and put the palettes and tiles back.
  Menu_EnsureCancelSound();
  RenderOutput_ClearListFar(state->statusWindow);
  FourObjectMotion_ClearSlotsAndSchedule();
  Scheduler_DisableOverlayCallbacksWithFlags();
  UiWindow_DrawFrameFar(0, 0, 30, 20);
  WaitFrames(1);
#if TBS_PATCH_1
  UiWindow_MarkVisibleTileAttributesFar();
  UiWork_SetAltFlagAndClearTableFar(0);
  WaitFrames(1);
#endif
  Iwram_CopyWords((void *)BG_PLTT, palette, 64);
  Iwram_CopyWords(BG_CHAR_BLOCK(1), tiles, STATUS_SAVED_TILE_BYTES);
  MemFree(tiles);
  MemFree(palette);
  ((struct UiRenderWork *)WORK_SLOT(HEAP_SLOT_WINDOW))->menuBusy = 1;
  ItemMenu_Close();
  UiWindow_DrawFrameFar(0, 0, 30, 20);
  Runtime_ReleaseHeapBlock(HEAP_SLOT_MENU);
  ((struct ObjectSystemWork *)WORK_SLOT(HEAP_SLOT_MENU_CONTROL))->suspended = 0;
  WaitFrames(1);
  UiWindow_EraseBorderRectFar(0, 0, 30, 20);
  ((struct UiRenderWork *)WORK_SLOT(HEAP_SLOT_WINDOW))->menuBusy = 0;
  return result;
}

// The Status menu's pages: choose a party member, then their status page, A
// for their Psynergy page and A again for their equipment page; B steps back.
// Returns the last page's result, or -1 when the player backs out or presses
// Start.
s32 Menu_RunActionFlow(void) {
  struct CharacterMenuState *state = gMenuWork;
  s32 step = 0;
  s32 finished = step;
  s32 result = 0;
  u32 changed;

  while (!finished && !GameFlag_TestFar(FLAG_MENU_START_PRESSED)) {
    switch (step) {
    case 0: // choose a member
      state->selectedSlots[0] = finished;
      if (CharacterMenu_SelectOwner(0) == -1) {
        result = -1;
        finished = 1;
      }
      step = 1;
      break;
    case 1: // their status
      state->ownerCursors[0]->active = ICON_HIDDEN;
      result = CharacterMenu_SelectCommand();
      step = result == -1 ? 0 : 2;
      break;
    case 2: // their Psynergy
      state->ownerCursors[0]->active = ICON_HIDDEN;
      result = PsynergyMenu_SelectAction();
      step = 0;
      if (result != -1)
        step = 3;
      break;
    case 3: // their equipment; back to the member after it
      state->ownerCursors[0]->active = ICON_HIDDEN;
      result = ItemMenu_SelectItem();
      // step 1 for -1, step 0 otherwise.
      changed = ~result;
      step = (-changed | changed) >> 31;
      break;
    default:
      finished = 1;
      break;
    }
  }

  if (GameFlag_TestFar(FLAG_MENU_START_PRESSED))
    result = -1;

  return result;
}

// Lets the player choose a party member in slot, from the one chosen last: the
// member chooser when the menu is on page 3, else the Status menu's party row.
// Game flag 0x172 adds a divider to the party window. Returns the chooser's
// result.
s32 CharacterMenu_SelectOwner(s32 slot) {
  struct CharacterMenuState *menu;
  struct RenderOutput *cursor;
  s32 result;
  s32 index;

  menu = gMenuWork;
  index = menu->ownerIndex[slot];
  result = 0;
  cursor = menu->ownerCursors[slot];
  cursor->active = ICON_SHOWN;
  cursor->animStep = 0;
  RenderOutput_RedrawSavedRectFar(menu->ownerWindow);
  if (GameFlag_TestFar(0x172))
    UiWindow_DrawDividerLineFar(menu->ownerWindow, 9, 1, 9, 3);
  if (index == -1)
    menu->ownerIndex[slot] = 0;
  else
    UiMenu_SlideCursor(index * 24 - 10, 16);
  if (menu->page == 3)
    result = PsynergyMenu_SelectOwner();
  else
    result = CharacterSelector_RunRearrange();
  cursor = menu->ownerCursors[slot];
  UiIcon_PrepareObject(cursor);
  WaitFrames(1);
  return result;
}

// Shows every Djinn the party has found, a column of seven for each element,
// until A, B or Select, then puts the Djinn menu back.
s32 DjinnMenu_ShowCurrentList(void) {
  struct DjinnMenuWork *menu;
  struct UiWindow *window;
  s32 element;
  s32 djinn;

  menu = gMenuWork;
  ItemMenu_ResetCategory();
  AudioCommand_PlayFar(SE_MENU_CONFIRM);
  RenderOutput_RedrawSavedRectFar(menu->helpWindow);
  UiText_DrawCharacterAtOffsetFar(MsgReturnHelp, menu->helpWindow, 0, 16);
  menu->cursor->active = ICON_HIDDEN;
  menu->secondCursor->active = ICON_HIDDEN;
  WaitFrames(1);
  window = menu->window;
  {
    s32 i;

    for (i = 0; i < 4; i++) {
      menu->slotX[i] = 32 + i * 56;
      menu->slotY[i] = 70;
      menu->ownerY[i] = 30;
    }
  }
  RenderOutput_RedrawSavedRectFar(window);
  UiWindow_DrawDividerLineFar(window, 0, 11, 28, 11);
  UiText_DrawResourceFar(MsgCurrentDjinn, menu->helpWindow, -96, 132);
  for (element = 0; element < 4; element++) {
    for (djinn = 0; djinn < 7; djinn++) {
      s32 id = element * 20 + djinn;

      // Game flags 48 on: one per Djinn found, 20 to an element.
      if (GameFlag_TestFar(FLAG_DJINNI_0 + id)) {
        UiWindow_SetTilemapEntryFar(window, 0x1001 + element, element * 7 + 1,
                                    djinn + 3, 0);
        UiText_DrawCharacterAtOffsetFar(MsgDjinnName + id, window,
                                        element * 56 + 16, djinn * 8 + 24);
      }
    }
  }
  ((struct UiWork *)gWindowWork[0])->dirty = 1;
  for (;;) {
    if (GameFlag_TestFar(FLAG_MENU_START_PRESSED))
      break;
    WaitFrames(1);
    if (gKeysPressed & (KEY_A | KEY_B | KEY_SELECT))
      break;
  }
  RenderOutput_RedrawSavedRectFar(menu->window);
  RenderOutput_ClearListFar(menu->helpWindow);
  {
    s32 i;

    for (i = 0; i < 4; i++) {
      menu->slotX[i] = 130 + i * 32;
      menu->slotY[i] = 128;
    }
  }
  menu->cursor->active = ICON_SHOWN;
  menu->secondCursor->active = ICON_SHOWN;
  AudioCommand_PlayFar(SE_MENU_CANCEL);
  // Returns no value: the bytes have the s32 epilogue (pop {r1}; bx r1) after
  // this void call.
}

struct InventoryMenuState;

// Lowers the party figures, then raises the currently selected member.
static __inline__ void
CharacterMenu_SetSelectedOwnerRow(struct CharacterMenuState *menu,
                                  s32 selectedOwner) {
  s32 rowIndex;

  for (rowIndex = MENU_ROW_COUNT - 1; rowIndex >= 0; rowIndex--)
    menu->ownerY[rowIndex] = 30;
  menu->ownerY[selectedOwner] = 26;
}

// The Status menu's party row: Left and Right choose a member, showing their
// status; L and R move the member one place along the party order; Select
// lists the Djinn found (once the first is caught, FLAG_DJINNI_0); A chooses
// the member and B backs out. Returns 1 or -1.
s32 CharacterSelector_RunRearrange(void) {
  struct CharacterMenuState *menu;
  s32 ownerIndex;
  s32 ownerCount;
  s32 page;
  s32 redraw;
  s32 result;
  s32 rowIndex;

  menu = gMenuWork;
  ownerIndex = menu->ownerIndex[0];
  ownerCount = menu->ownerCount[0];
  redraw = 1;
  result = 0;
  page = menu->page;
  UnitGetFar(menu->ownerIds[ownerIndex]);
  for (rowIndex = 0; rowIndex < 4; rowIndex++) {
    menu->rowX[rowIndex] = 130 + rowIndex * 32;
    menu->rowY[rowIndex] = 128;
  }
  // The highlight palette, and the first sprite palette into background
  // palettes 0 and 1.
  Palette_LightenBankHighlight(14);
  Dma_Set((void *)OBJ_PLTT, (void *)BG_PLTT, DMA_ENABLE << 16 | 16, REG_DMA3);
  Dma_Set((void *)&BG_PLTT_COLOR(14, 4), (void *)&BG_PLTT_COLOR(0, 14),
          DMA_ENABLE << 16 | 1, REG_DMA3);
  Dma_Set((void *)OBJ_PLTT, (void *)&BG_PLTT_COLOR(1, 0), DMA_ENABLE << 16 | 16,
          REG_DMA3);
  Dma_Set((void *)&BG_PLTT_COLOR(15, 4), (void *)&BG_PLTT_COLOR(1, 14),
          DMA_ENABLE << 16 | 1, REG_DMA3);
  while (!GameFlag_TestFar(FLAG_MENU_START_PRESSED)) {
    if (redraw) {
      redraw = 0;
      // The help lines, and the member's status and figure.
      RenderOutput_RedrawSavedRectFar(menu->selectorWindow);
      UiText_DrawCharacterAtOffsetFar(MsgRearrangeHelp, menu->selectorWindow, 0,
                                      0);
      if (GameFlag_TestFar(FLAG_DJINNI_0))
        UiText_DrawCharacterAtOffsetFar(MsgDjinnListHelp, menu->selectorWindow,
                                        0, 16);
      UiText_DrawCharacterAtOffsetFar(MsgRearrangeHelp - 3, // "A: Details"
                                      menu->selectorWindow, 0, 8);
      ownerIndex = (ownerIndex + ownerCount) % ownerCount;
      UnitGetFar(menu->ownerIds[ownerIndex]);
      page = (page + 3) % 3;
      Menu_CreateWindowAndEntryObjects(menu->ownerIds[ownerIndex], page);
      PsynergyMenu_CallIconRoutineWithValue(menu, menu->ownerIds[ownerIndex]);
      CharacterMenu_SetSelectedOwnerRow(menu, ownerIndex);
    }
    UiMenu_PositionCursor(ownerIndex * 24 - 10, 16);
    WaitFrames(1);
    if (gKeysPressed & KEY_A) {
      AudioCommand_PlayFar(SE_MENU_CONFIRM);
      result = 1;
      break;
    }
    if (gKeysPressed & KEY_B) {
      AudioCommand_PlayFar(SE_MENU_CANCEL);
      result = -1;
      break;
    }
    // L and R move the member along the party order.
    if (gKeysRepeat & KEY_R) {
      if (CharacterSelector_MoveEntry(ownerIndex, 1)) {
        AudioCommand_PlayFar(SE_MENU_CONFIRM);
        ownerIndex++;
        Menu_ReleaseEntryObjects();
        PsynergyMenu_InitializeEntryObjects(menu->ownerWindow, 2, 2, 8, 0);
        CharacterMenu_SetSelectedOwnerRow(menu, ownerIndex);
      } else {
        AudioCommand_PlayFar(SE_MENU_ERROR);
      }
      WaitFrames(1);
    } else if (gKeysRepeat & KEY_L) {
      if (CharacterSelector_MoveEntry(ownerIndex, 0)) {
        AudioCommand_PlayFar(SE_MENU_CONFIRM);
        ownerIndex--;
        Menu_ReleaseEntryObjects();
        PsynergyMenu_InitializeEntryObjects(menu->ownerWindow, 2, 2, 8, 0);
        CharacterMenu_SetSelectedOwnerRow(menu, ownerIndex);
      } else {
        AudioCommand_PlayFar(SE_MENU_ERROR);
      }
      WaitFrames(1);
    } else if ((gKeysPressed & KEY_SELECT) && GameFlag_TestFar(FLAG_DJINNI_0)) {
      DjinnMenu_ShowCurrentList();
      redraw = 1;
    } else {
      if (gKeysRepeat & KEY_LEFT) {
        AudioCommand_PlayFar(SE_MENU_CURSOR_MOVE);
        if (ownerCount > 1) {
          ownerIndex--;
          redraw = 1;
        }
      }
      if (gKeysRepeat & KEY_RIGHT) {
        AudioCommand_PlayFar(SE_MENU_CURSOR_MOVE);
        if (ownerCount > 1) {
          redraw = 1;
          ownerIndex++;
        }
      }
    }
  }
  menu->ownerIndex[0] = ownerIndex;
  menu->selectedOwner = menu->ownerIds[ownerIndex];
  menu->selectedIds[0] = menu->ownerIds[ownerIndex];
  return result;
}

// The member chooser (menu page 3): Left and Right choose a party member,
// showing their status and Psynergy icons; A chooses and B backs out. Returns
// 1 or -1.
s32 PsynergyMenu_SelectOwner(void) {
  struct PsynergyMenuState *menu = gMenuWork;
  s32 ownerIndex = menu->tabIndex[0];
  s32 ownerCount = menu->tabCounts[0];
  s32 redraw = 1;
  s32 page = menu->flags;
  s32 result;
  s32 rowIndex;

  UnitGetFar(menu->ownerTable[ownerIndex]);
  RenderOutput_RedrawSavedRectFar(menu->messageWindow);
  UiText_DrawCharacterAtOffsetFar(MsgChooseCharacter, menu->messageWindow, 0,
                                  0);
  UiText_DrawCharacterAtOffsetFar(MsgChooseCharacter + 1, menu->messageWindow,
                                  0, 16);
  for (;;) {
    if (redraw) {
      redraw = 0;
      ownerIndex = (ownerIndex + ownerCount) % ownerCount;
      UnitGetFar(menu->ownerTable[ownerIndex]);
      page = (page + 3) % 3;
      Menu_CreateWindowAndEntryObjects(menu->ownerTable[ownerIndex], page);
      PsynergyMenu_CallIconRoutineWithValue(menu, menu->ownerTable[ownerIndex]);
      for (rowIndex = 0; rowIndex < MENU_ROW_COUNT; rowIndex++)
        menu->rowPositions[rowIndex] = 30;
      menu->rowPositions[ownerIndex] = 26;
      menu->psynergyCount = PsynergyMenu_CollectActions(
          UnitGetFar(menu->ownerTable[ownerIndex]), (u16 *)menu->psynergies, 0);
      PsynergyMenu_DrawPsynergyIcons(menu->psynergies);
      Menu_PlaceEntryObjectsInGrid(96, 96, 8);
      Menu_HideEmptyEntryIcons(menu->psynergies);
    }
    UiMenu_PositionCursor(ownerIndex * 24 - 10, 16);
    WaitFrames(1);
    if (gKeysPressed & KEY_A) {
      AudioCommand_PlayFar(SE_MENU_CONFIRM);
      result = 1;
      break;
    }
    if (gKeysPressed & KEY_B) {
      AudioCommand_PlayFar(SE_MENU_CANCEL);
      result = -1;
      break;
    }
    {
      if (gKeysRepeat & KEY_LEFT) {
        AudioCommand_PlayFar(SE_MENU_CURSOR_MOVE);
        if (ownerCount > 1) {
          ownerIndex--;
          redraw = 1;
        }
      }
      if (gKeysRepeat & KEY_RIGHT) {
        AudioCommand_PlayFar(SE_MENU_CURSOR_MOVE);
        if (ownerCount > 1) {
          ownerIndex++;
          redraw = 1;
        }
      }
    }
  }
  menu->tabIndex[0] = ownerIndex;
  menu->selectedOwner = menu->ownerTable[ownerIndex];
  menu->ownerIds[0] = menu->ownerTable[ownerIndex];
  return result;
}

// Moves the party member at selectedIndex one place later (direction 1) or
// earlier (0) in the party order. Returns 1, or 0 when it cannot move.
s32 CharacterSelector_MoveEntry(s32 selectedIndex, s32 direction) {
  struct CharacterMenuState *menu = gMenuWork;
  u32 reordered[14];
  s32 index;

  if (menu->partyCount <= 1) {
    return 0;
  }
  if (direction == 1) {
    if (selectedIndex == menu->partyCount - 1) {
      return 0;
    }
  } else {
    if (selectedIndex == 0) {
      return 0;
    }
  }

  {
    u32 zero = 0;
#ifdef UBFIX
    s32 clearIndex = 13;

    // Avoid forming reordered - 1 after the final store. The plain indexed
    // clear adds four native bytes, so matching builds retain the old cursor.
    do {
      reordered[clearIndex] = zero;
      clearIndex--;
    } while (clearIndex >= 0);
#else
    u32 *clear = &reordered[13];

    do {
      *clear = zero;
      clear--;
    } while ((s32)clear >= (s32)reordered);
#endif
  }
  for (index = 0; index < menu->partyCount; index++) {
    reordered[index] = menu->ownerIds[index];
  }

  if (direction == 1) {
    index = reordered[selectedIndex];
    reordered[selectedIndex] = reordered[selectedIndex + 1];
    reordered[selectedIndex + 1] = index;
  } else {
    index = reordered[selectedIndex];
    reordered[selectedIndex] = reordered[selectedIndex - 1];
    reordered[selectedIndex - 1] = index;
  }

  // Rebuild the party in the new order.
  for (index = 0; index < menu->partyCount; index++) {
    Party_RemoveActiveOwnerFar(menu->ownerIds[index]);
  }
  for (index = 0; index < menu->partyCount; index++) {
    Party_AddActiveOwnerFar(reordered[index]);
  }
  menu->partyCount = Party_ListActiveOwnersFar(menu->ownerIds);
  return 1;
}

// Sets up the Status menu's first screen: the cursor window with the party's
// figures, no other windows yet, and an 8 by 2 cursor grid. The parameters are
// not used.
void Menu_InitSelectorCursorAndEntries(s32 unused0, s32 unused1, s32 unused2,
                                       s32 unused3) {
  struct CharacterMenuState *state;

  state = gMenuWork;
  PsynergyMenu_InitializeEntryObjects(UiMenu_CreateCursor(state), 2, 2, 8, 0);
  state->unknownWindow028 = 0;
  state->statusWindow = 0;
  state->helpWindow = 0;
  state->actionWindow = 0;
  state->selectedColumn = 0;
  state->selectedRow = 0;
  state->columnCount = 8;
  state->rowCount = 2;
}

// Shows member resource's status in the status window (0, 5, 30 by 15),
// opening it first with the member's portrait, the item icons on the chooser
// page, and the eight stat sprites; then draws the status and ailments.
void Menu_CreateWindowAndEntryObjects(s32 resource, s32 frame) {
  s32 created;
  struct UiWindow *window;
  struct RenderOutput *cursor;
  struct CharacterMenuState *menu;

  menu = gMenuWork;
  created = 0;
  window = menu->statusWindow;
  if (window == 0) {
    created = UiWindow_UpdateOrCreate(&menu->statusWindow, 0, 5, 30, 15, 2);
    window = menu->statusWindow;
  }
  if (created != 0) {
    cursor = SideObject_CreateFar(resource, 0, 0, window, 0, 0);
    menu->statusCursor = cursor;
    if ((cursor->sentinel = 0xF0, menu->page) == 3) {
      Menu_SpawnIconEntries((struct InventoryMenuState *)menu, window);
    }
    Menu_CreateEightEntryObjects(window);
    CharacterMenu_DrawStatusAilments(window, resource, 0x100);
    return;
  }
  CharacterMenu_DrawStatusAilments(window, resource, 0);
}

// The member's status page. The left pane lists their ailments (or the stat
// help when they have none) and the right pane the status entries (name and
// level, experience, HP, PP, Djinn when the party has any, elemental levels,
// Power and Resist); the cursor's entry is highlighted and explained in the
// help window. Up and Down move in a pane, Left and Right switch panes, L and
// R switch to the previous or next member; A goes on to the Psynergy page and
// B backs out. Returns 1 or -1.
s32 CharacterMenu_SelectCommand(void) {
  struct CharacterMenuState *menu;
  s32 pane;
  s32 result;
  s32 selected;
  s8 count;
  s32 hasAilments;
  s32 done;
  s32 hasDjinn;
  s32 redraw;
  s32 member;
  s32 djinnTotal;
  s32 i;
  u8 entries[8];

  menu = gMenuWork;
  pane = 0;
  result = 0;
  selected = 0;
  djinnTotal = Party_SumDjinnCountsFar(-1);
  hasDjinn = 0;
  if (djinnTotal != 0)
    hasDjinn = 1;
  UiWindow_UpdateOrCreate(&menu->helpWindow, 0, 0, 30, 5, 2);
  Scheduler_RemoveCallback(Menu_UpdateEntryObjectTransforms);
  for (i = 3; i >= 0; i--)
    menu->rowY[i] = 104;
  done = 0;
  UiMenu_SlideCursor(-10, 88);

  // One member's page at a time; L and R start the next.
  while (done == 0 && GameFlag_TestFar(FLAG_MENU_START_PRESSED) == 0) {
    UnitGetFar(menu->selectedIds[0]);
    CharacterMenu_DrawStatusAilments(menu->statusWindow, menu->selectedIds[0],
                                     1);
    count = CharacterMenu_BuildAvailability(entries, 1, menu->selectedIds[0]);
    hasAilments = 0;
    if (count == 0)
      count = 1;
    else
      hasAilments = 1;
    redraw = 1;

    while (GameFlag_TestFar(FLAG_MENU_START_PRESSED) == 0) {
      if (redraw != 0) {
        s32 limit;

        redraw = 0;
        limit = count;
        pane = (pane + 2) % 2;
        if (pane == 0) {
          selected = (selected + limit) % limit;
          RenderOutput_RedrawSavedRectFar(menu->helpWindow);
          if (hasAilments == 0) {
            UiText_DrawCharacterAtOffsetFar(
                MsgSwitchCharacterHelp, menu->statusWindow, SWITCH_HELP_X, -24);
            UiText_DrawCharacterAtOffsetFar(MsgSwitchCharacterHelp + 1,
                                            menu->statusWindow, 0, -24);
          }
        } else {
          RenderOutput_RedrawSavedRectFar(menu->helpWindow);
          if (hasDjinn != 0)
            selected = (selected + 8) % 8;
          else
            selected = (selected + 7) % 7;
        }
        CharacterMenu_DrawSelectionCursor(pane, selected, entries, 0);
        RenderOutput_ClearListFar(menu->helpWindow);
        WaitFrames(1);
        if (pane == 0)
          CharacterMenu_DrawSelectionLabels(menu->helpWindow, selected,
                                            entries);
        else
          StatusMenu_ShowOwnerProgressMessage(menu->helpWindow, selected,
                                              hasDjinn);
      }
      menu->ownerCursors[0]->active = ICON_SHOWN;
      if (pane == 0)
        UiMenu_PositionCursor(-10, selected * 16 + 88);
      else if (selected <= 3)
        UiMenu_PositionCursor(24, selected * 8 + 48);
      else
        UiMenu_PositionCursor(48, selected * 8 + 80);
      WaitFrames(1);
      if (gKeysRepeat & KEYS_DPAD)
        CharacterMenu_DrawSelectionCursor(pane, selected, entries, 1);
      if (gKeysPressed & KEY_A) {
        AudioCommand_PlayFar(SE_MENU_CONFIRM);
        done = 1;
        result = 1;
        break;
      }
      if (gKeysPressed & KEY_B) {
        AudioCommand_PlayFar(SE_MENU_CANCEL);
        done = 1;
        result = -1;
        break;
      }
      if (gKeysRepeat & KEY_UP) {
        AudioCommand_PlayFar(SE_MENU_CURSOR_MOVE);
        redraw = 1;
        selected--;
      }
      if (gKeysRepeat & KEY_DOWN) {
        AudioCommand_PlayFar(SE_MENU_CURSOR_MOVE);
        redraw = 1;
        selected++;
      }
      if (gKeysRepeat & KEY_RIGHT) {
        AudioCommand_PlayFar(SE_MENU_CURSOR_MOVE);
        redraw = 1;
        pane++;
      }
      if (gKeysRepeat & KEY_LEFT) {
        AudioCommand_PlayFar(SE_MENU_CURSOR_MOVE);
        redraw = 1;
        pane--;
      }
      // L and R: the previous or next member.
      if ((gKeysRepeat & KEY_R) || (gKeysRepeat & KEY_L)) {
        AudioCommand_PlayFar(SE_MENU_CURSOR_MOVE);
        member = menu->ownerIndex[0];
        if (gKeysRepeat & KEY_R)
          member++;
        else
          member--;
        member = (member + menu->partyCount) % menu->partyCount;
        menu->selectedOwner = menu->ownerIds[member];
        menu->selectedIds[0] = menu->ownerIds[member];
        menu->ownerIndex[0] = member;
        PsynergyMenu_CallIconRoutineWithValue(menu, menu->ownerIds[member]);
        break;
      }
    }
  }
  RenderOutput_ClearListFar(menu->helpWindow);
  RenderOutput_RedrawSavedRectFar(menu->helpWindow);
  UiWindow_ClearInteriorTilesFar(menu->statusWindow, 64, 56, 224, 96);
  Scheduler_AddOrUpdateCallback(Menu_UpdateEntryObjectTransforms, 0xc80);
  for (i = 3; i >= 0; i--)
    menu->rowY[i] = 128;
  ItemMenu_ResetCategory();
  return result;
}

// Highlights (invert 0) or unhighlights (invert 1) the selected entry of the
// status page: in pane 0 the selected ailment row, as wide as its label
// (gCharacterMenu_CursorWidths); in pane 1 one of the first four status rows
// or of the rows below.
void CharacterMenu_DrawSelectionCursor(s32 mode, s32 selected, u8 *entries,
                                       s32 invert) {
  struct CharacterMenuState *menu = gMenuWork;
  u32 different;
  s32 count;
  s32 index;
  s32 x;
  s32 y;
  s32 palette;
  u8 width;

  if (mode == 0) {
    y = selected * 2 + 5;
    x = 0;
    width = 5;
    count = 0;
    index = 0;
    while (index <= 4) {
      if (entries[index] != 0) {
        if (selected == count) {
          width = gCharacterMenu_CursorWidths[index];
          break;
        }
        count++;
      }
      index++;
    }
  } else if (selected <= 3) {
    y = selected;
    x = 5;
    width = 13;
  } else {
    y = selected + 4;
    x = 8;
    width = 20;
  }

  // Palette 14 (highlighted) when invert is 0, 15 when it is 1.
  different = 1 ^ invert;
  palette = 15 - (((0u - different) | different) >> 31);
  Render_SetTilemapFlagRect(menu->statusWindow, x, y, width, 1, palette);
}

// Writes the advice for the selected ailment (two lines, e.g. how to revive a
// member who is down) into window target, or that the member is well when
// they have no ailment.
void CharacterMenu_DrawSelectionLabels(struct UiWindow *target, s32 selected,
                                       const u8 *entries) {
  s32 labelCount = 0;
  s32 labelIndex = 0;

  do {
    if (entries[labelIndex] != 0) {
      if (selected == labelCount) {
        s32 messageId = MsgStatusAdvice + labelIndex * 2;
        UiText_DrawResourceFar(messageId, target, 0, -1);
        UiText_DrawResourceFar(messageId + 1, target, 0, 15);
      }
      labelCount++;
    }
    labelIndex++;
  } while (labelIndex <= 4);

  if (labelCount == 0)
    UiText_DrawResourceFar(MsgStatusNormal, target, 0, 0);
}

// Native revisions keep different local widths; the text value consumes u32.
#if TBS_PATCH_1
typedef u64 ExperienceDifference;
#else
typedef u32 ExperienceDifference;
#endif

// Writes the help text of status entry messageVariant into destination: the
// entries skip Djinn when the party has none (preserveVariant 0); the
// experience entry says how many points the next level needs, or, at level 99,
// gives the plain experience text.
void StatusMenu_ShowOwnerProgressMessage(struct UiWindow *destination,
                                         s32 messageVariant,
                                         s32 preserveVariant) {
  struct CharacterMenuState *menu = gMenuWork;
  void *buffer;

  if (preserveVariant == 0 && messageVariant > 3) {
    messageVariant++;
  }
  if (messageVariant == 1) {
    struct BattleUnit *owner = UnitGetFar(menu->selectedIds[0]);

    if (owner->level == 99) {
      messageVariant = 8; // "These are your experience points."
    } else {
      ExperienceDifference remaining =
          Unit_GetExperienceForLevelFar(menu->selectedIds[0],
                                        owner->level + 1) -
          owner->experience;
      UiWork_PushValueSlotFar(remaining, 5);
    }
  }

  buffer = MemAlloc(STATUS_HELP_TEXT_MAX * 2);
  UiText_CopyMessageStringFar(messageVariant + MsgProgressHelp, buffer,
                              STATUS_HELP_TEXT_MAX);
  UiText_RenderWideStringAtOffsetFar(buffer, destination, 0, -1);
  MemFree(buffer);
}

// Draws member ownerId's status page into window: the top rows (ItemMenu_
// DrawOwnerStatus), the ailment list or "Normal", and except on the chooser
// page the Djinn counts by element and, in mode 1, the elemental levels,
// Power and Resist. mode's 0x100 keeps what is drawn instead of clearing it.
void CharacterMenu_DrawStatusAilments(struct UiWindow *window, s32 ownerId,
                                      s32 mode) {
  struct CharacterMenuState *menu = gMenuWork;
  struct BattleUnit *unit;
  s32 djinnRow;
  s32 hasDjinn;
  s32 keepDrawn;
  s32 ailmentIndex;
  u8 ailments[8];

  if (Party_SumDjinnCountsFar(-1) != 0)
    hasDjinn = 1;
  else
    hasDjinn = 0;
  unit = UnitGetFar(ownerId);
  if ((mode & 0xff) == 1)
    djinnRow = 7;
  else
    djinnRow = 10;
  menu->statusCursor->active = ICON_SHOWN;
  ItemMenu_DrawOwnerStatus(window, ownerId, mode);
  CharacterMenu_BuildAvailability(ailments, 1, ownerId);
  CharacterMenu_UpdateSelectionIcons(ailments);
  keepDrawn = mode & 0x100;
  if (keepDrawn == 0)
    UiWindow_ClearInteriorTilesFar(window, 0, 40, 96, 96);
  // The ailments, one a row, or "Normal".
  ailmentIndex = 0;
  if (ailments[CHARACTER_DOWN] != 0) {
    UiText_DrawCharacterAtOffsetFar(MsgDownLabel, window, 16, 40);
    ailmentIndex = 1;
  }
  if (ailments[CHARACTER_POISON] != 0) {
    UiText_DrawCharacterAtOffsetFar(MsgPoisonLabel, window, 16,
                                    ailmentIndex * 16 + 40);
    ailmentIndex++;
  }
  if (ailments[CHARACTER_VENOM] != 0) {
    UiText_DrawCharacterAtOffsetFar(MsgVenomLabel, window, 16,
                                    ailmentIndex * 16 + 40);
    ailmentIndex++;
  }
  if (ailments[CHARACTER_CURSE] != 0) {
    UiText_DrawCharacterAtOffsetFar(MsgCurseLabel, window, 16,
                                    ailmentIndex * 16 + 40);
    ailmentIndex++;
  }
  if (ailments[CHARACTER_HAUNT] != 0) {
    UiText_DrawCharacterAtOffsetFar(MsgHauntLabel, window, 16,
                                    ailmentIndex * 16 + 40);
    ailmentIndex++;
  }
  if (ailmentIndex == 0)
    UiText_DrawCharacterAtOffsetFar(MsgNormalLabel, window, 0, 40);
  CharacterMenu_UpdateSelectionIcons(ailments);
  ItemMenu_ApplyFlags(ailments);
  if (menu->page == 3)
    return;
  if (keepDrawn == 0) {
    WaitFrames(1);
    UiWindow_ClearInteriorTilesFar(window, 64, 56, 224, 96);
  }
  // The four element icons, then the Djinn counts and, in mode 1, the
  // elemental levels, Power and Resist.
  UiWork_SetParamNibbleFar(TEXT_COLOR_NORMAL);
  if (mode == 1 || hasDjinn == 1) {
    UiWindow_SetTilemapEntryFar(window, 1, 15, djinnRow, 4);
    UiWindow_SetTilemapEntryFar(window, 2, 19, djinnRow, 4);
    UiWindow_SetTilemapEntryFar(window, 3, 23, djinnRow, 4);
    UiWindow_SetTilemapEntryFar(window, 4, 27, djinnRow, 4);
  }
  if (hasDjinn)
    UiText_DrawCharacterAtOffsetFar(MsgDjinnLabel, window, 64,
                                    djinnRow * 8 + 8);
  if (mode == 1) {
    if (!hasDjinn)
      djinnRow--;
    MENU_DRAW_STATUS_LABEL(gMenu_LvString, window, 64, djinnRow * 8 + 16);
    UiText_DrawCharacterAtOffsetFar(MsgPowerLabel, window, 64,
                                    djinnRow * 8 + 24);
    UiText_DrawCharacterAtOffsetFar(MsgPowerLabel + 1, window, 64,
                                    djinnRow * 8 + 32);
  }
  for (ailmentIndex = 0; ailmentIndex <= 3; ailmentIndex++) {
    if (hasDjinn)
      UiText_DrawNumberInWindowFar(unit->djinnOwnedCounts[ailmentIndex], 1,
                                   window, ailmentIndex * 32 + 120,
                                   djinnRow * 8 + 8);
    if ((mode & 0xff) == 1) {
      if (hasDjinn) {
        UiText_DrawNumberInWindowFar(unit->djinnActiveCounts[ailmentIndex], 1,
                                     window, ailmentIndex * 32 + 104,
                                     djinnRow * 8 + 8);
        UiText_DrawStringInWindowFar(gMenu_SlashString, window,
                                     ailmentIndex * 32 + 112, djinnRow * 8 + 8);
      }
      UiText_DrawNumberInWindowFar(
          Unit_GetElementLevelFar(ownerId, ailmentIndex), 2, window,
          ailmentIndex * 32 + 112, djinnRow * 8 + 16);
      UiText_DrawNumberInWindowFar(unit->element[ailmentIndex].power, 3, window,
                                   ailmentIndex * 32 + 104, djinnRow * 8 + 24);
      UiText_DrawNumberInWindowFar(unit->element[ailmentIndex].resistance, 3,
                                   window, ailmentIndex * 32 + 104,
                                   djinnRow * 8 + 32);
    }
  }
}

#if TBS_PATCH_1
// Spins for 256 loop passes (international editions only).
void Runtime_RunBusyWaitDelay(void) {
  s32 count = 0xff;

spin:
  count--;
  if (count >= 0)
    goto spin;
}

#endif

// Draws the top rows of member ownerId's status page into window: name, class,
// level, experience, HP and PP on the left, Attack, Defense, Agility and Luck
// on the right. flags' 0x100 keeps what is drawn instead of clearing it.
void ItemMenu_DrawOwnerStatus(struct UiWindow *window, s32 ownerId, s32 flags) {
  struct InventoryMenuState *menu;
  struct BattleUnit *unit;

  menu = gMenuWork;
  unit = UnitGetFar(ownerId);
  menu->cursor->active = ICON_SHOWN;
  flags &= 0x100;
  if (flags == 0) {
    UiWindow_ClearInteriorTilesFar(window, 0, 0, 128, 40);
  }
  UiText_DrawStringAtOffsetFar(unit->name, window, 40, 0);
  UiText_DrawCharacterAtOffsetFar(unit->classIndex + MsgClassName, window, 0,
                                  32);
  MENU_DRAW_STATUS_LABEL(gMenu_LvString, window, 104, 0);
  UiWork_SetParamNibbleFar(TEXT_COLOR_NORMAL);
  UiText_DrawNumberInWindowFar(unit->level, 2, window, 128, 0);
  MENU_DRAW_STATUS_LABEL(gMenu_HpString, window, 40, 16);
  UiText_DrawNumberInWindowFar(unit->hp, 4, window, 72, 16);
  UiText_DrawNumberInWindowFar(unit->maxHp, 4, window, 112, 16);
  UiText_DrawStringInWindowFar(gMenu_SlashString, window, 104, 16);
  MENU_DRAW_STATUS_LABEL(gMenu_PpString, window, 40, 24);
  UiText_DrawNumberInWindowFar(unit->pp, 4, window, 72, 24);
  UiText_DrawNumberInWindowFar(unit->maxPp, 4, window, 112, 24);
  UiText_DrawStringInWindowFar(gMenu_SlashString, window, 104, 24);
  UiText_DrawCharacterAtOffsetFar(MsgExpLabel, window, 40, 8);
  UiText_DrawNumberInWindowFar(unit->experience, 7, window, 88, 8);
  if (flags == 0) {
    WaitFrames(1);
    UiWindow_ClearInteriorTilesFar(window, 144, 0, 224, 40);
  }
  // The labels "Attack", "Defense", "Agility" and "Luck", 23 to 20 messages
  // before "Exp".
  UiText_DrawCharacterAtOffsetFar(MsgExpLabel - 23, window, 152, 0);
  UiText_DrawCharacterAtOffsetFar(MsgExpLabel - 22, window, 152, 8);
  UiText_DrawCharacterAtOffsetFar(MsgExpLabel - 21, window, 152, 16);
  UiText_DrawCharacterAtOffsetFar(MsgExpLabel - 20, window, 152, 24);
  UiText_DrawNumberInWindowFar(unit->atk, STATUS_STAT_DIGITS, window,
                               STATUS_STAT_X, 0);
  UiText_DrawNumberInWindowFar(unit->def, STATUS_STAT_DIGITS, window,
                               STATUS_STAT_X, 8);
  UiText_DrawNumberInWindowFar(unit->agi, STATUS_STAT_DIGITS, window,
                               STATUS_STAT_X, 16);
  UiText_DrawNumberInWindowFar(unit->luck, STATUS_STAT_DIGITS, window,
                               STATUS_STAT_X, 24);
}

// Fills ailments[0..4] (CharacterAilment) with member ownerId's ailments: down
// (only when requested is 1), poison or venom, curse and haunt. Returns how
// many they have.
s32 CharacterMenu_BuildAvailability(u8 *ailments, s32 requested, s32 ownerId) {
  struct BattleUnit *unit;
  s32 ailmentIndex;
  s32 count;
  s32 poison;

  unit = UnitGetFar(ownerId);
  for (ailmentIndex = 4; ailmentIndex >= 0; ailmentIndex--)
    ailments[ailmentIndex] = 0;

  count = 0;
  if (unit->hp == 0 && requested == 1) {
    ailments[CHARACTER_DOWN] = requested;
    count = 1;
  }

  poison = unit->poison;
  if (poison != 0) {
    if (poison == 1) // poison; any other value is venom
      ailments[CHARACTER_POISON] = poison;
    else
      ailments[CHARACTER_VENOM] = 1;
    count++;
  }
  if (unit->restraint != 0) {
    ailments[CHARACTER_CURSE] = 1;
    count++;
  }
  if (unit->haunt != 0) {
    ailments[CHARACTER_HAUNT] = 1;
    count++;
  }
  return count;
}

// Fills result with the item list of the member in pane index for the
// equipment pages: their unit, the menu's item count, and the page and row of
// the item chosen last for them, STATUS_EQUIP_PAGE_SIZE items to a page.
// Returns 1.
s32 Menu_BuildPageResult(struct MenuResult *result, s32 index) {
  s32 unitBits;
  struct InventoryMenuState *menu = gMenuWork;
  s32 itemCount;
  s32 row;
  s32 page;
  s32 pageCount;
  s32 selected;

  unitBits = (s32)UnitGetFar(menu->paneOwner[index]);
  itemCount = menu->itemCount;
  selected = menu->selectedIndexByOwner[menu->paneOwner[index]];
  if ((selected + 1) > itemCount) {
    selected = itemCount - 1;
  }
  if (itemCount == 0) {
    selected = 0;
  }
  page = selected / STATUS_EQUIP_PAGE_SIZE;
  row = selected % STATUS_EQUIP_PAGE_SIZE;
  pageCount = itemCount / STATUS_EQUIP_PAGE_SIZE;
  if (itemCount % STATUS_EQUIP_PAGE_SIZE != 0) {
    pageCount++;
  }
  result->ownerState = unitBits;
  result->page = page;
  result->pageCount = pageCount;
  result->row = row;
  result->entryCount = itemCount;
  result->selectedIndex = selected;
  return 1;
}

// Combines one pattern word with its row mask.
static __inline__ u32 XorWord(u32 word, u32 mask) { return word ^ mask; }

extern const struct TileMask gMenu_PatternRowMasks[];

// Builds twelve 8x8 tiles in the window tile area: two sets of six, each a
// fill of colour 4 with the rows of gMenu_PatternRowMasks XORed in, the first
// set growing row by row and the second skipping its top row.
void Menu_BuildPatternTiles(void) {
#if TBS_PATCH_1
  u32 *vram = (u32 *)(BG_CHAR_BLOCK(1) + 0x1000);
#else
  u32 *vram = (u32 *)BG_CHAR_BLOCK(1);
#endif
  s32 set;
  s32 tileIndex;

  for (set = 0; set < 2; set++) {
    for (tileIndex = 0; tileIndex < 6; tileIndex++) {
      u32 *tile = vram + set * 0x60 + tileIndex * 0x10;
      s32 rowIndex;

      Iwram_FillWords(tile, 64, 0x44444444);
      for (rowIndex = 1; rowIndex <= 7; rowIndex++) {
        s32 mask = tileIndex;

        if (set == 1 && rowIndex <= 1) {
          continue;
        }
        if (set == 0 && tileIndex > rowIndex - 2) {
          mask = rowIndex - 2;
          if (mask < 0) {
            mask = 0;
          }
        }
        tile[rowIndex] =
            XorWord(tile[rowIndex], gMenu_PatternRowMasks[mask].word0);
        tile[rowIndex + 8] =
            XorWord(tile[rowIndex + 8], gMenu_PatternRowMasks[mask].word1);
      }
    }
  }
}

// Draws a Psynergy range icon at (x, y) of window: three tiles from the
// range icon tiles, the left one mirrored.
void PsynergyMenu_DrawRange(struct UiWindow *window, s32 x, s32 y, s32 range,
                            s32 unused) {
  s32 iconIndex;
  s32 tile;

  (void)unused;

  iconIndex = range * 2;
  tile = iconIndex + RANGE_ICON_TILE + 1;
  UiWindow_SetTilemapEntryFar(window, 0x400 | tile, x, y, 0); // flipped
  UiWindow_SetTilemapEntryFar(window, iconIndex + RANGE_ICON_TILE, x + 1, y, 0);
  UiWindow_SetTilemapEntryFar(window, tile, x + 2, y, 0);
}

// Updates the Psynergy page for the row under the cursor: the Psynergy's
// description in the info window, where it can be used (in the field, in
// battle, or anywhere), and the row highlights with the range icons.
s32 PsynergyMenu_DrawRangePage(struct UiWindow *window, s32 unused,
                               struct MenuResult *state) {
  struct PsynergyMenuState *menu;
  struct BattleAct *ability;
  s32 row;
  s32 base;

  menu = gMenuWork;
  state->selectedIndex = state->page * PAGE_ROWS + state->row;
#if TBS_PATCH_1 && !TBS_PATCH_4
  RenderOutput_RedrawSavedRectFar(menu->infoWindow);
#elif TBS_PATCH_4
  UiWindow_ClearInteriorTilesFar(menu->infoWindow, 0, 0, 224, 8);
#else
  RenderOutput_ClearListFar(menu->infoWindow);
#endif
  WaitFrames(1);
  if (menu->psynergies[state->selectedIndex] != 0) {
    MENU_DRAW_INFO_TEXT(
        (menu->psynergies[state->selectedIndex] & OWNER_ACTION_ID_MASK) +
            MsgAbilityDescription,
        menu->infoWindow, 0, 0);
    ability = BattleActGet(menu->psynergies[state->selectedIndex] &
                           OWNER_ACTION_ID_MASK);
#if LANGUAGE != LANGUAGE_JAPANESE
    UiWindow_ClearInteriorTilesFar(window, 0, 96, 224, 104);
    row = 0;
    if (ability->type0c != 0 ||
        (ability->flags & ACTION_USABLE_IN_FIELD) != 0) {
      row = 2;
    }
    if ((ability->flags & ACTION_USABLE_IN_BATTLE) != 0) {
      row |= 1;
    }
    if (row == 3) {
      UiText_DrawCharacterAtOffsetFar(MsgUsableAnywhere, window, 0, 96);
    } else if (row == 2) {
      UiText_DrawCharacterAtOffsetFar(MsgUsableInField, window, 0, 96);
    } else if (row == 1) {
      UiText_DrawCharacterAtOffsetFar(MsgUsableInBattle, window, 0, 96);
    }
#else
    UiWindow_ClearInteriorTilesFar(window, 0, 72, 64, 96);
    row = 0;
    if (ability->type0c != 0 ||
        (ability->flags & ACTION_USABLE_IN_FIELD) != 0) {
      UiText_DrawCharacterAtOffsetFar(MsgUsableInField, window, 0, 72);
      row = 1;
    }
    if ((ability->flags & ACTION_USABLE_IN_BATTLE) != 0) {
      UiText_DrawCharacterAtOffsetFar(MsgUsableInBattle, window, 0,
                                      row * 8 + 72);
      row++;
    }
    UiText_DrawCharacterAtOffsetFar(MsgCanBeUsed, window, 0, row * 8 + 72);
#endif
  }

  base = state->page * PAGE_ROWS;
  for (row = 0; row <= PAGE_ROWS - 1; row++) {
    if (row == state->row) {
      ability =
          BattleActGet(menu->psynergies[base + row] & OWNER_ACTION_ID_MASK);
      if (ability->element != 4) {
        UiWindow_SetTilemapEntryFar(window, ability->element + 1, 24,
                                    row * 2 + LIST_FIRST_ROW, 0);
        Render_SetTilemapFlagRect(window, 9, row * 2 + LIST_FIRST_ROW, 15, 1,
                                  14);
        Render_SetTilemapFlagRect(window, 25, row * 2 + LIST_FIRST_ROW, 3, 1,
                                  14);
      } else {
        Render_SetTilemapFlagRect(window, 9, row * 2 + LIST_FIRST_ROW, 19, 1,
                                  14);
      }
    } else {
      ability =
          BattleActGet(menu->psynergies[base + row] & OWNER_ACTION_ID_MASK);
      if (ability->element != 4) {
        UiWindow_SetTilemapEntryFar(window, ability->element + 1, 24,
                                    row * 2 + LIST_FIRST_ROW, 4);
        Render_SetTilemapFlagRect(window, 9, row * 2 + LIST_FIRST_ROW, 15, 1,
                                  15);
        Render_SetTilemapFlagRect(window, 25, row * 2 + LIST_FIRST_ROW, 3, 1,
                                  15);
      } else {
        Render_SetTilemapFlagRect(window, 9, row * 2 + LIST_FIRST_ROW, 19, 1,
                                  15);
      }
    }
  }
  WaitFrames(1);
  return 1;
}

s32 PsynergyMenu_DrawListPage(struct UiWindow *window, s32 unused,
                              const struct MenuResult *res) {
  struct PsynergyMenuState *menu = gMenuWork;
  struct BattleUnit *owner;
  u32 firstEntry;
  u32 rows;
  u8 row;
  s32 entryIndex;

  (void)unused;

  owner = UnitGetFar(menu->ownerIds[0]);

  RenderOutput_RedrawSavedRectFar(window);

  firstEntry = res->page * PAGE_ROWS;
  rows = (u8)(res->entryCount - firstEntry);
  if (rows > PAGE_ROWS) {
    rows = PAGE_ROWS;
  }

  Menu_SetPageIcons(PAGE_ROWS, firstEntry, window, LIST_ICONS_X, LIST_ICONS_Y);
  Menu_DrawPageIndicator(window, res->entryCount, PAGE_ROWS, res->page, 28);

  UiText_DrawCharacterAtOffsetFar(MsgPsynergyPp, window, LIST_PP_X, 0);

  row = 0;
  if (rows > row) {
    entryIndex = firstEntry;
    do {
      struct BattleAct *action;
      s32 messageId;
      s32 y;
      s32 range;
      action =
          BattleActGet(OWNER_ACTION_ID_MASK & menu->psynergies[entryIndex]);
      messageId = (menu->psynergies[entryIndex] & OWNER_ACTION_ID_MASK) +
                  MsgAbilityName;
      y = row * 16 + LIST_FIRST_ROW * 8;

      UiText_DrawCharacterAtOffsetFar(messageId, window, LIST_NAME_X, y);
      UiText_DrawNumberAtOffsetFar(action->ppCost, 2, window, LIST_PP_NUMBER_X,
                                   y);

      range = action->range;
      if (range == 0xff) {
        range = 11;
      } else {
        range--;
      }
      PsynergyMenu_DrawRange(window, 25, row * 2 + LIST_FIRST_ROW, range, 0);

      row++;
      entryIndex++;
    } while (rows > row);
  }

  if (menu->psynergyCount == 0) {
    UiText_DrawCharacterAtOffsetFar(MsgNoPsynergy, window, 96, LIST_EMPTY_Y);
  }

  UiText_DrawStringAtOffsetFar(owner->name, (void *)window, 40, 0);
  UiText_DrawCharacterAtOffsetFar(owner->classIndex + MsgClassName, window, 0,
                                  32);
#if LANGUAGE == LANGUAGE_SPANISH || LANGUAGE == LANGUAGE_FRENCH
  UiText_DrawStringAtOffsetFar(gMenu_LvString, (void *)window, 0, 48);
#else
  UiText_DrawStringInWindowFar(gMenu_LvString, window, 0, 48);
#endif
  UiText_DrawNumberInWindowFar(owner->level, 2, window, 24, 48);

  return 1;
}

s32 PsynergyMenu_SelectAction(void) {

  struct PsynergyMenuState *menu;
  s32 result;
  s32 done;
  s32 redraw;
  s32 listNeedsRedraw;
  s32 navigationResult;
  s32 ownerIndex;
  s32 iconIndex;
  struct MenuResult state;

  menu = gMenuWork;
  result = 0;
  done = 0;
  Menu_BuildPatternTiles();
  RenderOutput_RedrawSavedRectFar(menu->messageWindow);
  UiWindow_UpdateOrCreate(&menu->infoWindow, 0, 0, 30, 5, 2);
  for (iconIndex = 3; iconIndex >= 0; iconIndex--)
    menu->slotY[iconIndex] = -16;
  {
    s32 priority = 245;
    struct RenderOutput **icons = menu->entryIcons;

    for (iconIndex = 31; iconIndex >= 0; iconIndex--) {
      struct RenderOutput *icon = *icons++;

      if (icon != 0)
        icon->sentinel = priority;
    }
  }
  Scheduler_RemoveCallback(Menu_UpdateEntryObjectTransforms);
  Menu_SpawnIconEntries((struct InventoryMenuState *)menu, menu->messageWindow);
  UiText_DrawCharacterAtOffsetFar(MsgSwitchCharacterHelp, menu->statusWindow,
                                  SWITCH_HELP_X, -24);
  UiText_DrawCharacterAtOffsetFar(MsgSwitchCharacterHelp + 2,
                                  menu->statusWindow, 0, -24);

  while (done == 0 && GameFlag_TestFar(FLAG_MENU_START_PRESSED) == 0) {
    ItemMenu_PosCategory();
    RenderOutput_RedrawSavedRectFar(menu->statusWindow);
    menu->psynergyCount = PsynergyMenu_CollectActions(
        UnitGetFar(menu->ownerIds[0]), menu->psynergies, 0);
    WaitFrames(1);
    Menu_BuildPageResult(&state, 0);
    PsynergyMenu_DrawPreparedPsynergyIcons(menu->statusWindow,
                                           menu->ownerIds[0]);
    redraw = 1;
    listNeedsRedraw = 1;

    while (GameFlag_TestFar(FLAG_MENU_START_PRESSED) == 0) {
      if (redraw != 0) {
        redraw = 0;
        if (listNeedsRedraw != 0) {
          listNeedsRedraw = 0;
          PsynergyMenu_DrawListPage(menu->statusWindow, 0, &state);
        }
        PsynergyMenu_DrawRangePage(menu->statusWindow, 0, &state);
        WaitFrames(1);
      }
      WaitFrames(1);
      navigationResult = Menu_HandlePageInput(0, state.entryCount, PAGE_ROWS,
                                              &state.row, &state.page);
      menu->paneIcon[0]->active = ICON_SHOWN;
      UiMenu_PositionCursor(STATUS_PSYNERGY_CURSOR_X,
                            state.row * 16 + STATUS_PSYNERGY_CURSOR_Y);
      if (navigationResult == 1) {
        listNeedsRedraw = 1;
        redraw = 1;
      }
      if (navigationResult == 0)
        redraw = 1;
      if (navigationResult == -1)
        redraw = 0;
      if (gKeysPressed & KEY_A) {
        AudioCommand_PlayFar(SE_MENU_CONFIRM);
        result = 1;
        done = 1;
        break;
      }
      if (gKeysPressed & KEY_B) {
        AudioCommand_PlayFar(SE_MENU_CANCEL);
        result = -1;
        done = 1;
        Scheduler_AddOrUpdateCallback(Menu_UpdateEntryObjectTransforms, 0xc80);
        break;
      }
      if ((gKeysRepeat & KEY_R) || (gKeysRepeat & KEY_L)) {
        AudioCommand_PlayFar(SE_MENU_CURSOR_MOVE);
        ownerIndex = menu->tabIndex[0];
        menu->selectedIndexByOwner[menu->ownerTable[ownerIndex]] =
            state.selectedIndex;
        if (gKeysRepeat & KEY_R)
          ownerIndex++;
        else
          ownerIndex--;
        ownerIndex = (ownerIndex + menu->ownerCount) % menu->ownerCount;
        menu->selectedOwner = menu->ownerTable[ownerIndex];
        menu->ownerIds[0] = menu->ownerTable[ownerIndex];
        menu->tabIndex[0] = ownerIndex;
        PsynergyMenu_CallIconRoutineWithValue(menu,
                                              menu->ownerTable[ownerIndex]);
        break;
      }
    }
  }
  RenderOutput_ClearListFar(menu->infoWindow);
  RenderOutput_RedrawSavedRectFar(menu->infoWindow);
  ItemMenu_HideAllIcons();
  RenderOutput_ClearListFar(menu->messageWindow);
  RenderOutput_RedrawSavedRectFar(menu->statusWindow);
  return result;
}

// Returns the original page-callback success value.
s32 PsynergyMenu_ActionReturnTrue(void) { return 1; }

// Draws this owner's prepared Psynergy list in the shared entry sprites.
void PsynergyMenu_DrawPreparedPsynergyIcons(const void *unused, s32 ownerId) {
  struct PsynergyMenuState *menu = gMenuWork;

  UnitGetFar(ownerId);
  ItemMenu_HideAllIcons();
  PsynergyMenu_DrawPsynergyIcons(menu->psynergies);
}

// Reserved empty callback for the prepared Psynergy page.
void PsynergyMenu_PreparedIconsNoOp(void) {}

// Reserved second empty callback for the prepared Psynergy page.
void PsynergyMenu_PreparedIconsSecondNoOp(void) {}

s32 ItemMenu_DrawEquipPage(struct UiWindow *window, s32 unused,
                           struct MenuResult *state) {
  struct InventoryMenuState *menu;
  struct ItemDefinition *item;
  s32 firstEntry;
  s32 rowIndex;

  menu = gMenuWork;
  firstEntry = state->page * PAGE_ROWS;
  state->selectedIndex = firstEntry + state->row;
#if TBS_PATCH_1 && !TBS_PATCH_4
  RenderOutput_RedrawSavedRectFar(menu->infoWindow);
#elif TBS_PATCH_4
  UiWindow_ClearInteriorTilesFar(menu->infoWindow, 0, 0, 224, 8);
#else
  RenderOutput_ClearListFar(menu->infoWindow);
#endif
  WaitFrames(1);
  if (menu->items[state->selectedIndex] != 0) {
    MENU_DRAW_INFO_TEXT((menu->items[state->selectedIndex] & ITEM_ID_MASK) +
                            MsgItemPlainName,
                        menu->infoWindow, 0, 0);
  }

  for (rowIndex = 0; rowIndex < PAGE_ROWS; rowIndex++) {
    if (rowIndex == state->row) {
      item =
          Item_GetDirectFar(menu->items[state->selectedIndex] & ITEM_ID_MASK);
      if (item->element != 4) {
        UiWindow_SetTilemapEntryFar(window, item->element + 1, 27,
                                    rowIndex * 2 + 1, 0);
        Render_SetTilemapFlagRect(window, 14, rowIndex * 2 + 1, 13, 1, 14);
      } else {
        Render_SetTilemapFlagRect(window, 14, rowIndex * 2 + 1, 14, 1, 14);
      }
    } else {
      item =
          Item_GetDirectFar(menu->items[firstEntry + rowIndex] & ITEM_ID_MASK);
      if (item->element != 4) {
        UiWindow_SetTilemapEntryFar(window, item->element + 1, 27,
                                    rowIndex * 2 + 1, 4);
        Render_SetTilemapFlagRect(window, 14, rowIndex * 2 + 1, 13, 1, 15);
      } else {
        Render_SetTilemapFlagRect(window, 14, rowIndex * 2 + 1, 14, 1, 15);
      }
    }
  }

  for (rowIndex = 0; rowIndex <= 3; rowIndex++) {
    Render_SetTilemapFlagRect(menu->equipWindow, STATUS_EQUIP_SLOT_X,
                              rowIndex * 2 + 1, STATUS_EQUIP_SLOT_WIDTH, 1, 15);
  }

  if (menu->items[state->selectedIndex] & INVENTORY_EQUIPPED) {
    item = Item_GetDirectFar(menu->items[state->selectedIndex] & ITEM_ID_MASK);
    switch (item->type) {
    case 1:
      Render_SetTilemapFlagRect(menu->equipWindow, STATUS_EQUIP_SLOT_X, 1,
                                STATUS_EQUIP_SLOT_WIDTH, 1, 14);
      break;
    case 4:
      Render_SetTilemapFlagRect(menu->equipWindow, STATUS_EQUIP_SLOT_X, 3,
                                STATUS_EQUIP_SLOT_WIDTH, 1, 14);
      break;
    case 3:
      Render_SetTilemapFlagRect(menu->equipWindow, STATUS_EQUIP_SLOT_X, 5,
                                STATUS_EQUIP_SLOT_WIDTH, 1, 14);
      break;
    case 2:
      Render_SetTilemapFlagRect(menu->equipWindow, STATUS_EQUIP_SLOT_X, 7,
                                STATUS_EQUIP_SLOT_WIDTH, 1, 14);
      break;
    }
  }
  WaitFrames(1);
  return 1;
}

s32 Shop_DrawItemPage(struct UiWindow *window, s32 unused,
                      struct MenuResult *state) {
  struct InventoryMenuState *menu = gMenuWork;
  struct BattleUnit *unit;
  u16 *inventoryEntry;
  s32 firstEntry;
  u32 rowCount;
  u32 rowIndex;
  s32 message;

  unit = UnitGetFar(menu->paneOwner[0]);
  UiWindow_ClearInteriorTilesFar(window, 128, 8, 224, 96);
  firstEntry = state->page * PAGE_ROWS;
  rowCount = (u8)(state->entryCount - firstEntry);
  if (rowCount > PAGE_ROWS)
    rowCount = PAGE_ROWS;
  Menu_SetPageIcons(PAGE_ROWS, firstEntry, window, SHOP_PAGE_ICONS_X, 52);
  Menu_DrawPageIndicator(window, state->entryCount, PAGE_ROWS, state->page, 28);
  if (menu->itemCount == 0) {
    UiText_DrawCharacterAtOffsetFar(MsgItemMenuEmpty, window,
                                    SHOP_EMPTY_MESSAGE_X, 8);
  } else {
    rowIndex = 0;
    if (rowCount > rowIndex) {
      inventoryEntry = &menu->items[firstEntry];
      do {
        UiText_DrawCharacterAtOffsetFar(
            (*inventoryEntry & ITEM_ID_MASK) + MsgItemName, window,
            SHOP_ITEM_NAME_X, (rowIndex << 4) + 8);
        rowIndex = (u8)(rowIndex + 1);
        inventoryEntry++;
      } while (rowCount > rowIndex);
    }
  }
  UiText_DrawStringAtOffsetFar(unit->name, window, 40, 0);
  message = MsgStatLabel;
  UiText_DrawCharacterAtOffsetFar(message, window, 32, 16);
  UiText_DrawCharacterAtOffsetFar(message + 1, window, 32, 24);
  UiText_DrawNumberInWindowFar(unit->atk, 3, window, SHOP_STAT_X, 16);
  UiText_DrawNumberInWindowFar(unit->def, 3, window, SHOP_STAT_X, 24);
  return 1;
}

s32 ItemMenu_SelectItem(void) {

  struct InventoryMenuState *menu;
  struct BattleUnit *owner;
  s32 result;
  s32 done;
  s32 redraw;
  s32 listNeedsRedraw;
  s32 navigationResult;
  s32 ownerIndex;
  s32 iconIndex;
  u16 savedInventory[15];
  struct MenuResult state;

  menu = gMenuWork;
  {
    s32 ownerId = menu->paneOwner[0];

    result = 0;
    UnitGetFar(menu->ownerIds[ownerId]);
  }
  UiWindow_UpdateOrCreate(&menu->equipWindow, 0, 10, STATUS_EQUIP_WINDOW_WIDTH,
                          10, 2);
  ItemMenu_PosCategory();

  for (iconIndex = 0; iconIndex < 32; iconIndex++) {
    struct RenderOutput *icon = menu->entryIcons[iconIndex];

    if (icon != 0)
      icon->sentinel = 240;
  }

  Scheduler_RemoveCallback(Menu_UpdateEntryObjectTransforms);
  UiText_DrawCharacterAtOffsetFar(MsgSwitchCharacterHelp, menu->statusWindow,
                                  64, -24);
  UiText_DrawCharacterAtOffsetFar(MsgSwitchCharacterHelp + 3,
                                  menu->statusWindow, 0, -24);
  ItemMenu_PosCategory();
  WaitFrames(1);
  Menu_SpawnIconEntries((struct InventoryMenuState *)menu, menu->messageWindow);

  done = 0;
  while (done == 0 && GameFlag_TestFar(FLAG_MENU_START_PRESSED) == 0) {
    ItemMenu_PosCategory();
    RenderOutput_RedrawSavedRectFar(menu->statusWindow);
    owner = UnitGetFar(menu->paneOwner[0]);
    for (iconIndex = 0; iconIndex <= 14; iconIndex++)
      savedInventory[iconIndex] = owner->inventory[iconIndex];
    menu->itemCount = ItemMenu_CollectItems(owner, menu->items, 0);
    InventoryMenu_SortByListOrder(menu->items, 0);
    Menu_BuildPageResult(&state, 0);
    RenderOutput_RedrawSavedRectFar(menu->equipWindow);
    ItemMenu_DrawCategory(menu->equipWindow, menu->paneOwner[0], 1);
    WaitFrames(1);
    ItemMenu_DrawIcons(menu->items, 0);
    redraw = 1;
    listNeedsRedraw = 1;

    while (GameFlag_TestFar(FLAG_MENU_START_PRESSED) == 0) {
      if (redraw != 0) {
        RenderOutput_ClearListFar(menu->infoWindow);
        redraw = 0;
        if (listNeedsRedraw != 0) {
          listNeedsRedraw = 0;
          Shop_DrawItemPage(menu->statusWindow, 0, &state);
        }
        ItemMenu_DrawEquipPage(menu->statusWindow, 0, &state);
      }
      menu->paneIcons[0]->active = ICON_SHOWN;
      UiMenu_PositionCursor(96, state.row * 16 + 52);
      WaitFrames(1);

      navigationResult = Menu_HandlePageInput(0, state.entryCount, PAGE_ROWS,
                                              &state.row, &state.page);
      if (navigationResult == 1) {
        listNeedsRedraw = 1;
        redraw = 1;
      }
      if (navigationResult == 0)
        redraw = 1;
      if (navigationResult == -1)
        redraw = 0;

      if (gKeysPressed & KEY_A) {
        AudioCommand_PlayFar(SE_MENU_CONFIRM);
        result = 1;
        done = 1;
        break;
      }
      if (gKeysPressed & KEY_B) {
        AudioCommand_PlayFar(SE_MENU_CANCEL);
        result = -1;
        done = 1;
        break;
      }
      if ((gKeysRepeat & KEY_R) || (gKeysRepeat & KEY_L)) {
        AudioCommand_PlayFar(SE_MENU_CURSOR_MOVE);
        ownerIndex = menu->paneIndex[0];
        menu->selectedIndexByOwner[menu->ownerIds[ownerIndex]] =
            state.selectedIndex;
        if (gKeysRepeat & KEY_R)
          ownerIndex++;
        else
          ownerIndex--;
        for (iconIndex = 0; iconIndex <= 14; iconIndex++)
          owner->inventory[iconIndex] = savedInventory[iconIndex];
        ownerIndex = (ownerIndex + menu->partyCount) % menu->partyCount;
        menu->selectedOwner = menu->ownerIds[ownerIndex];
        menu->paneOwner[0] = menu->ownerIds[ownerIndex];
        menu->paneIndex[0] = ownerIndex;
        PsynergyMenu_CallIconRoutineWithValue(menu, menu->ownerIds[ownerIndex]);
        break;
      }
    }
  }

  RenderOutput_ClearListFar(menu->infoWindow);
  RenderOutput_RedrawSavedRectFar(menu->infoWindow);
  RenderOutput_ClearListFar(menu->messageWindow);
  ItemMenu_HideAllIcons();
  RenderOutput_RedrawSavedRectFar(menu->statusWindow);
  Scheduler_AddOrUpdateCallback(Menu_UpdateEntryObjectTransforms, 0xc80);
  return result;
}

// Moves through the status grid's irregular rows and returns row * 9 + column.
s32 Menu_MoveGridCursor(s32 *columnOut, s32 *rowOut, s32 direction) {
  s32 row;
  s32 column;

  column = *columnOut;
  row = *rowOut;
  switch (direction) {
  case KEY_UP:
    row -= 1;
    if (row < 0) {
      row = 5;
    }
    if (row <= 3) {
      if (row == 3) {
        if (column <= 4) {
          column = 0;
        } else {
          column = 1;
        }
      } else if (column > 1) {
        column = 1;
      }
      if ((row == 3) && (column == 1)) {
        row = 2;
      }
    }
    break;
  case KEY_DOWN:
    row += 1;
    if (row > 5) {
      row = 0;
    }
    if ((row == 3) && (column == 1)) {
      row = 4;
    }
    if (row == 4) {
      column = 0;
    }
    break;
  case KEY_LEFT:
    column -= 1;
    if (row == 3) {
      column += 1;
    } else if (row > 3) {
      if (column < 0) {
        column = 7;
      }
    } else if (column < 0) {
      column = 1;
    }
    break;
  case KEY_RIGHT:
    column += 1;
    if (row == 3) {
      column -= 1;
    } else if (row > 3) {
      if (column > 7) {
        column = 0;
      }
    } else if (column > 1) {
      column = 0;
    }
    break;
  }
  *columnOut = column;
  *rowOut = row;
  return (row * 9) + column;
}

// Returns the original reserved success value.
s32 Menu_ReservedStatusOne(void) { return 1; }

// Shows the owner's equipped items and optionally its category icons.
void ItemMenu_DrawCategory(struct UiWindow *window, s32 ownerId, s32 mode) {
  struct InventoryMenuState *menu = gMenuWork;
  u16 *items;

  UnitGetFar(ownerId);
  ItemMenu_PosCategory();
  ItemMenu_HideAllIcons();
  UiText_DrawCharacterAtOffsetFar(MsgEquipSlotLabels, window, 0, 0);
  UiText_DrawCharacterAtOffsetFar(MsgEquipSlotLabels + 1, window, 0, 32);
  UiText_DrawCharacterAtOffsetFar(MsgEquipSlotLabels + 2, window, 0, 16);
  UiText_DrawCharacterAtOffsetFar(MsgEquipSlotLabels + 3, window, 0, 48);
  items = menu->items;
  ItemMenu_DrawEquippedItemNames(window, items);
  if (mode == 0) {
    WaitFrames(1);
    ItemMenu_DrawIcons(items, 1);
    ItemMenu_ArrangeCategoryItemIcons(items);
  }
}

// Draws the nonempty equipment entries by their item names.
void ItemMenu_DrawEquippedItemNames(struct UiWindow *window, u16 *items) {
  s32 slotIndex;
  s32 itemId;

  for (slotIndex = 0; slotIndex < 15; slotIndex++) {
    if (items[slotIndex] & INVENTORY_EQUIPPED) {
      itemId = items[slotIndex] & ITEM_ID_MASK;
      switch (Item_GetDirectFar(itemId)->type) {
      case 1:
        UiText_DrawCharacterAtOffsetFar(itemId + MsgItemName, window,
                                        STATUS_EQUIPPED_NAME_X, 8);
        break;
      case 2:
        UiText_DrawCharacterAtOffsetFar(itemId + MsgItemName, window,
                                        STATUS_EQUIPPED_NAME_X, 56);
        break;
      case 3:
        UiText_DrawCharacterAtOffsetFar(itemId + MsgItemName, window,
                                        STATUS_EQUIPPED_NAME_X, 40);
        break;
      case 4:
        UiText_DrawCharacterAtOffsetFar(itemId + MsgItemName, window,
                                        STATUS_EQUIPPED_NAME_X, 24);
        break;
      }
    }
  }
}

// Positions the shared entry sprites on the requested grid.
void Menu_PlaceEntryObjectsInGrid(s32 originX, s32 originY, s32 columns) {
  s32 entryIndex;
  struct RenderOutput *icon;
  struct RenderOutput **icons;

  entryIndex = 0;
  icons = ((struct InventoryMenuState *)gMenuWork)->entryIcons;
  do {
    icon = *icons++;
    if (icon != NULL) {
      Menu_PlaceEntryObjectInGrid(icon, entryIndex, originX, originY, columns);
    }
    entryIndex += 1;
  } while (entryIndex <= 0x1F);
}

// Positions one entry sprite in the requested number of grid columns.
void Menu_PlaceEntryObjectInGrid(struct RenderOutput *icon, s32 index,
                                 s32 originX, s32 originY, s32 columns) {
  s32 entryIndex;

  entryIndex = index;
  if (entryIndex > 0x1F) {
    entryIndex = 0;
  }
  icon->y = (entryIndex / columns * 0x10) + originY;
  icon->x = (entryIndex % columns * 0x10) + originX;
  UiIcon_PrepareObject(icon);
}

// Loads and places the owner's equipment-category icons.
void ItemMenu_ArrangeCategoryItemIcons(u16 *items) {
  struct InventoryMenuState *menu;
  struct RenderOutput *icon;
  s32 slotIndex;

  menu = gMenuWork;
  ItemMenu_PosCategory();
  for (slotIndex = 0; slotIndex < 15; slotIndex++) {
    if (items[slotIndex] != 0 && (items[slotIndex] & INVENTORY_EQUIPPED) != 0) {
      icon = menu->entryIcons[slotIndex];
      if (icon != 0) {
        switch (Item_GetDirectFar(items[slotIndex] & ITEM_ID_MASK)->type) {
        case 1:
          icon->x = 216;
          icon->y = 32;
          break;
        case 2:
          icon->x = 216;
          icon->y = 80;
          break;
        case 3:
          icon->x = 216;
          icon->y = 64;
          break;
        case 4:
          icon->x = 216;
          icon->y = 48;
          break;
        }
        UiIcon_PrepareObject(icon);
      }
    }
  }
}

// Moves all entry sprites off screen before arranging another category.
void ItemMenu_PosCategory(void) {
  struct InventoryMenuState *state = gMenuWork;
  s32 x = 248;
  struct RenderOutput **entry = state->entryIcons;
  s32 y = 168;
  s32 remaining = 31;

  do {
    struct RenderOutput *object = *entry++;

    if (object != 0) {
      object->x = x;
      object->y = y;
      UiIcon_PrepareObject(object);
    }
    remaining--;
  } while (remaining >= 0);
}

// Creates the eight shared category sprites, initially off screen.
s32 Menu_CreateEightEntryObjects(struct UiWindow *window) {
  struct RenderOutput **slot;
  struct RenderOutput *icon;
  s32 slotIndex;
  struct InventoryMenuState *menu;
  s32 spriteFlags;

  menu = gMenuWork;
  slotIndex = 0;
  spriteFlags = 0xA8;
  slot = menu->categoryIcons;
  do {
    icon = RenderOutput_CreateFromResourceFar(2, slotIndex, window, 0xF8,
                                              spriteFlags);
    slotIndex += 1;
    *slot = icon;
    slot += 1;
  } while (slotIndex <= 7);
  return 1;
}

// Shows enabled category icons down the left edge, sixteen pixels apart.
void ItemMenu_ApplyFlags(const u8 *flags) {
  /* FAKEMATCH(temporary): y stored through a pointer to the field, one more
   * instruction in the loop before combine, which keeps the flag index a
   * counter instead of a walking pointer as the ROM does; as
   * entry->y = nextY, 11 of 33 instructions differ in tbs-en. */
  struct InventoryMenuState *menu;
  struct RenderOutput **slot;
  struct RenderOutput *entry;
  s32 index;
  s32 nextY;

  menu = gMenuWork;
  ItemMenu_ResetCategory();
  index = 0;
  slot = menu->categoryIcons;
  nextY = 88;
  do {
    entry = *slot++;
    if (entry != 0 && flags[index] != 0) {
      entry->x = 8;
      {
        s16 *y = &entry->y;

        *y = nextY;
      }
      entry->sentinel = 240;
      UiIcon_PrepareObject(entry);
      nextY += 16;
    }
    index++;
  } while (index <= 4);
}

// Moves each category sprite off screen before it is reused.
void ItemMenu_ResetCategory(void) {
  struct InventoryMenuState *state = gMenuWork;
  s32 index;

  for (index = 0; index < 5; index++) {
    struct RenderOutput *object = state->categoryIcons[index];

    if (object != 0) {
      object->x = 248;
      object->y = 168;
      object->sentinel = 240;
      UiIcon_PrepareObject(object);
    }
  }
}

// Loads each enabled ailment's icon into its category resource slot.
s32 CharacterMenu_UpdateSelectionIcons(const u8 *enabled) {
  struct InventoryMenuState *state = gMenuWork;
  s32 index = 0;

  do {
    if (enabled[index] != 0) {
      s32 kind;
      switch (index) {
      case CHARACTER_DOWN:
        kind = 16;
        break;
      case CHARACTER_POISON:
        kind = 1;
        break;
      case CHARACTER_VENOM:
        kind = 2;
        break;
      case CHARACTER_CURSE:
        kind = 15;
        break;
      case CHARACTER_HAUNT:
        kind = 7;
        break;
      default:
        kind = 0;
        break;
      }
      Resource_LoadByModeIntoSlotFar(8, kind,
                                     (u8)state->categoryIcons[index]->index, 0);
    }
    index++;
  } while (index <= 4);
  return 1;
}

#undef CHARACTER_SELECTOR_OWNER_COUNT
#undef STATUS_SAVED_TILE_BYTES
#undef STATUS_HELP_TEXT_MAX
#undef STATUS_STAT_DIGITS
#undef STATUS_STAT_X
#undef STATUS_EQUIP_PAGE_SIZE
#undef STATUS_PSYNERGY_CURSOR_X
#undef STATUS_PSYNERGY_CURSOR_Y
#undef STATUS_EQUIP_SLOT_X
#undef STATUS_EQUIP_SLOT_WIDTH
#undef SHOP_PAGE_ICONS_X
#undef SHOP_ITEM_NAME_X
#undef SHOP_STAT_X
#undef SHOP_EMPTY_MESSAGE_X
#undef STATUS_EQUIP_WINDOW_WIDTH
#undef STATUS_EQUIPPED_NAME_X
