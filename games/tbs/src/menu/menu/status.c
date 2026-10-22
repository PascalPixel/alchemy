/// @file
/// The member chooser and the Status menu: its member row, status page,
/// Psynergy page and equipment page.
// Part of menu.c.

#include "../menu_internal.h"

// The international editions reset eight party figure rows rather than four,
// and their longer words need more window.
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
#if LANGUAGE != LANGUAGE_JAPANESE
#define STATUS_HELP_TEXT_MAX 0x80
#else
#define STATUS_HELP_TEXT_MAX 0x40
#endif

// The stat column of the status page's top rows: three digits from 200 or
// four from 192, both ending at pixel 224.
#if LANGUAGE != LANGUAGE_JAPANESE
#define STATUS_STAT_DIGITS 3
#define STATUS_STAT_X 200
#else
#define STATUS_STAT_DIGITS 4
#define STATUS_STAT_X 192
#endif

// Items to a page of the equipment page's list.
#if LANGUAGE != LANGUAGE_JAPANESE
#define STATUS_EQUIP_PAGE_SIZE 5
#else
#define STATUS_EQUIP_PAGE_SIZE 6
#endif

// The hand cursor on the Psynergy page.
#if LANGUAGE != LANGUAGE_JAPANESE
#define STATUS_PSYNERGY_CURSOR_X 55
#define STATUS_PSYNERGY_CURSOR_Y 60
#else
#define STATUS_PSYNERGY_CURSOR_X 58
#define STATUS_PSYNERGY_CURSOR_Y 52
#endif

// The highlight over the equipment window's four slot rows.
#if LANGUAGE != LANGUAGE_JAPANESE
#define STATUS_EQUIP_SLOT_X 1
#define STATUS_EQUIP_SLOT_WIDTH 12
#else
#define STATUS_EQUIP_SLOT_X 2
#define STATUS_EQUIP_SLOT_WIDTH 9
#endif

// The equipment page's item list: its page icons, the item names, and the
// Attack and Defense figures.
#if LANGUAGE != LANGUAGE_JAPANESE
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
#if LANGUAGE != LANGUAGE_JAPANESE
#define STATUS_EQUIP_WINDOW_WIDTH 15
#define STATUS_EQUIPPED_NAME_X 8
#else
#define STATUS_EQUIP_WINDOW_WIDTH 13
#define STATUS_EQUIPPED_NAME_X 16
#endif

/// Lets the player choose one party member, over the field (the Colosso scenes
/// use it, through MENU.S, and so does the debug test). Returns the chosen
/// member's id, or -1 when the player backs out. Owns the menu work
/// (HEAP_SLOT_MENU) while it runs and frees it before returning.
s32 Menu_OpenCharacterSelector(void) {
  struct CharacterMenuState *menu =
      Runtime_AllocateHeapBlock(HEAP_SLOT_MENU, MENU_WORK_SIZE);
  s32 result;
  s32 index;

  gMenuCtrlWork->suspended = 1;
  UiWindow_DrawFrameFar(0, 0, 30, 20);
  WaitFrames(1);
  UiWindow_InitializeWork(0);

  menu->partyCount = Party_ListActiveOwnersFar(menu->ownerIds);
  Menu_InitializeSelectorCursorAndEntries(0, 3, 0, 7);
  menu->selectorWindow = UiWindow_CreateFar(13, 0, 17, 5, 2);
  for (index = 0; index < CHARACTER_SELECTOR_OWNER_COUNT; index++)
    menu->ownerY[index] = 30;
  menu->page = CHARACTER_MENU_PAGE_CHOOSER;

  result = CharacterSelector_Run();

  RenderOutput_ClearListFar(menu->statusWindow);
  ItemMenu_Close();
  gMenuCtrlWork->suspended = 0;
  WaitFrames(1);
  Runtime_ReleaseHeapBlock(HEAP_SLOT_MENU);
  return result;
}

/// The member chooser's loop. Returns the chosen member's id, or -1.
s32 CharacterSelector_Run(void) {
  struct CharacterMenuState *work = gMenuWork;
  s32 result = 0;

  /* FAKEMATCH(one-pass): the slot's clearing in a one-pass block stores the
   * zero before the slot's offset is built; as a plain statement, 4 of 23
   * instructions differ in tbs-en. */
  do {
    work->selectedSlots[0] = result;
  } while (0);
  if (CharacterMenu_SelectOwner(0) == -1)
    result = -1;
  else
    result = work->selectedIds[0];
  return result;
}

/// The field menu's Status screen (Menu_RunTopSelection, FIELD_MENU_STATUS),
/// over the field: saves the first two background palettes and the window tiles
/// it draws over, sets up the party row and, when the party has Djinn, the four
/// Djinn sprites, runs the status pages, then puts everything back. Returns
/// their result; -1 (backed out or Start) opens the field menu again. The menu
/// work and the two saved copies, taken from the alternate pool, are freed
/// before returning.
s32 StatusMenu_Run(void) {
  struct CharacterMenuState *menu =
      Runtime_AllocateHeapBlock(HEAP_SLOT_MENU, MENU_WORK_SIZE);
  void *palette = Runtime_BumpAllocateAlternatePool(64);
  void *tiles = Runtime_BumpAllocateAlternatePool(STATUS_SAVED_TILE_BYTES);
  s32 result;
  s32 index;

  ((struct ObjectSystemWork *)WORK_SLOT(HEAP_SLOT_MENU_CONTROL))->suspended = 1;
  UiWindow_DrawFrameFar(0, 0, 30, 20);
  WaitFrames(1);
  Menu_SuspendFieldTasks();
  UiWindow_InitializeWork(0);
  menu->page = CHARACTER_MENU_PAGE_STATUS;
  menu->partyCount = Party_ListActiveOwnersFar(menu->ownerIds);
  Menu_InitializeSelectorCursorAndEntries(0, 3, 0, 7);
  Iwram_CopyWords(palette, (void *)BG_PLTT, 64);
#if !TBS_PATCH_1
  Iwram_CopyWords(tiles, BG_CHAR_BLOCK(1), STATUS_SAVED_TILE_BYTES);
#endif
  Palette_LightenBankHighlight(14);
  // The first sprite palette into background palettes 0 and 1, for the
  // element icons drawn as window tiles: palette 0, for the highlighted row,
  // takes the highlight (colour 4 of palette 14) as its colour 14, palette 1
  // the plain row's colour 4 of palette 15 (PsynergyMenu_DrawRangePage).
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
  menu->selectorWindow = UiWindow_CreateFar(13, 0, 17, 5, 2);
  for (index = 0; index < MENU_ROW_COUNT; index++)
    menu->ownerY[index] = 30;
  if (Party_SumDjinnCountsFar(-1) != 0)
    FourObjectMotion_InitializeTopRow(menu->selectorWindow, 0);
  // The four Djinn figures in a row 32 pixels apart, at screen y 128.
  for (index = 0; index < 4; index++) {
    menu->rowX[index] = 130 + index * 32;
    menu->rowY[index] = 128;
  }
  Link_DrawShiftedTilePairFar(UI_PAGE_ARROW_VRAM);
  // Run the pages.
  Menu_CancelSoundReset();
  menu->page = CHARACTER_MENU_PAGE_STATUS;

  result = Menu_RunActionFlow();

  // Close, and put the palettes and tiles back.
  Menu_EnsureCancelSound();
  RenderOutput_ClearListFar(menu->statusWindow);
  FourObjectMotion_ClearSlotsAndSchedule();
  Menu_ResumeFieldTasks();
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

/// The steps of the Status menu (Menu_RunActionFlow).
enum StatusMenuStep {
  STATUS_STEP_MEMBER = 0,   ///< choose a member
  STATUS_STEP_STATUS = 1,   ///< their status page
  STATUS_STEP_PSYNERGY = 2, ///< their Psynergy page
  STATUS_STEP_EQUIPMENT = 3 ///< their equipment page
};

/// The Status menu's pages: choose a party member, then their status page, A
/// for their Psynergy page and A again for their equipment page, whose A goes
/// back round to the status page; B on any page goes back to the member row.
/// Returns -1 when the player backs out of the member row or presses Start.
s32 Menu_RunActionFlow(void) {
  struct CharacterMenuState *menu = gMenuWork;
  s32 step = STATUS_STEP_MEMBER;
  s32 finished = step;
  s32 result = 0;
  u32 changed;

  while (!finished && !GameFlag_TestFar(FLAG_MENU_START_PRESSED)) {
    switch (step) {
    case STATUS_STEP_MEMBER:
      menu->selectedSlots[0] = finished;
      if (CharacterMenu_SelectOwner(0) == -1) {
        result = -1;
        finished = 1;
      }
      step = STATUS_STEP_STATUS;
      break;
    case STATUS_STEP_STATUS:
      menu->ownerCursors[0]->active = ICON_HIDDEN;
      result = CharacterMenu_SelectCommand();
      step = result == -1 ? STATUS_STEP_MEMBER : STATUS_STEP_PSYNERGY;
      break;
    case STATUS_STEP_PSYNERGY:
      menu->ownerCursors[0]->active = ICON_HIDDEN;
      result = PsynergyMenu_SelectAction();
      step = STATUS_STEP_MEMBER;
      if (result != -1)
        step = STATUS_STEP_EQUIPMENT;
      break;
    case STATUS_STEP_EQUIPMENT:
      menu->ownerCursors[0]->active = ICON_HIDDEN;
      result = ItemMenu_SelectItem();
      // STATUS_STEP_MEMBER for -1, STATUS_STEP_STATUS otherwise.
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

/// Lets the player choose a party member in slot, from the one chosen last: the
/// member chooser on CHARACTER_MENU_PAGE_CHOOSER, else the Status menu's party
/// row. Returns the chooser's result: 1 when a member was chosen, -1 when the
/// player backed out, and on the party row 0 when Start ended it.
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
  // FLAG_LINK_BATTLE_ARRANGED is set while a link battle is arranged in the
  // Battle Arena, in which only the first three members fight
  // (LinkLobby_PartyContains); the divider falls between the third and
  // fourth figures, which stand three tiles apart.
  if (GameFlag_TestFar(FLAG_LINK_BATTLE_ARRANGED))
    UiWindow_DrawDividerLineFar(menu->ownerWindow, 9, 1, 9, 3);
  if (index == -1)
    menu->ownerIndex[slot] = 0;
  else
    UiMenu_SlideCursor(index * 24 - 10, 16);
  if (menu->page == CHARACTER_MENU_PAGE_CHOOSER)
    result = PsynergyMenu_SelectOwner();
  else
    result = CharacterSelector_RunRearrange();
  cursor = menu->ownerCursors[slot];
  UiIcon_PrepareObject(cursor);
  WaitFrames(1);
  return result;
}

/// Shows every Djinn the party has found, a column of seven for each element,
/// until A, B or Select, then puts the screen back. Select on the Status
/// menu's party row opens it, once the first Djinni is caught. Reads the menu
/// work as the Djinn menu's view (struct DjinnMenuWork).
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
  for (element = 0; element < ELEMENT_COUNT; element++) {
    for (djinn = 0; djinn < 7; djinn++) {
      s32 id = element * DJINN_PER_ELEMENT + djinn;

      // FLAG_DJINNI_0 on: one per Djinni found, DJINN_PER_ELEMENT to an
      // element. Each name
      // follows its element's icon: window tile 1 + element in palette 1
      // (0x1000).
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
#ifdef UBFIX
  return 0;
#endif
}

struct InventoryMenuState;

/// Lowers the party figures, then raises the currently selected member.
static __inline__ void
CharacterMenu_SetSelectedOwnerRow(struct CharacterMenuState *menu,
                                  s32 selectedOwner) {
  s32 rowIndex;

  for (rowIndex = MENU_ROW_COUNT - 1; rowIndex >= 0; rowIndex--)
    menu->ownerY[rowIndex] = 30;
  menu->ownerY[selectedOwner] = 26;
}

/// The Status menu's party row: Left and Right choose a member, showing their
/// status; L and R move the member one place along the party order; Select
/// lists the Djinn found (once the first is caught, FLAG_DJINNI_0); A chooses
/// the member and B backs out. Returns 1 or -1, or 0 when Start ends it, with
/// the member in the work's ownerIndex[0] and selectedIds[0]. Moving a member
/// changes the party order for good (CharacterSelector_MoveEntry).
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
  Unit_GetFar(menu->ownerIds[ownerIndex]);
  for (rowIndex = 0; rowIndex < 4; rowIndex++) {
    menu->rowX[rowIndex] = 130 + rowIndex * 32;
    menu->rowY[rowIndex] = 128;
  }
  // The highlight palette, and the first sprite palette into background
  // palettes 0 and 1, as StatusMenu_Run sets them.
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
      Unit_GetFar(menu->ownerIds[ownerIndex]);
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

/// The member chooser (menu page 3): Left and Right choose a party member,
/// showing their status and Psynergy icons; A chooses and B backs out. Returns
/// 1 or -1.
s32 PsynergyMenu_SelectOwner(void) {
  struct PsynergyMenuState *menu = gMenuWork;
  s32 ownerIndex = menu->tabIndex[0];
  s32 ownerCount = menu->tabCounts[0];
  s32 redraw = 1;
  s32 page = menu->flags;
  s32 result;
  s32 rowIndex;

  Unit_GetFar(menu->ownerTable[ownerIndex]);
  RenderOutput_RedrawSavedRectFar(menu->messageWindow);
  UiText_DrawCharacterAtOffsetFar(MsgChooseCharacter, menu->messageWindow, 0,
                                  0);
  UiText_DrawCharacterAtOffsetFar(MsgChooseCharacter + 1, menu->messageWindow,
                                  0, 16);
  for (;;) {
    if (redraw) {
      redraw = 0;
      ownerIndex = (ownerIndex + ownerCount) % ownerCount;
      Unit_GetFar(menu->ownerTable[ownerIndex]);
      page = (page + 3) % 3;
      Menu_CreateWindowAndEntryObjects(menu->ownerTable[ownerIndex], page);
      PsynergyMenu_CallIconRoutineWithValue(menu, menu->ownerTable[ownerIndex]);
      for (rowIndex = 0; rowIndex < MENU_ROW_COUNT; rowIndex++)
        menu->rowPositions[rowIndex] = 30;
      menu->rowPositions[ownerIndex] = 26;
      menu->psynergyCount = PsynergyMenu_CollectActions(
          Unit_GetFar(menu->ownerTable[ownerIndex]), (u16 *)menu->psynergies,
          PSYNERGY_LIST_ALL);
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

/// Moves the party member at selectedIndex one place later (direction 1) or
/// earlier (0) in the party order. Returns 1, or 0 when it cannot move.
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

/// Sets up the Status menu's first screen: the cursor window with the party's
/// figures, no other windows yet, and an 8 by 2 cursor grid. The parameters are
/// not used.
void Menu_InitializeSelectorCursorAndEntries(s32 unused0, s32 unused1,
                                             s32 unused2, s32 unused3) {
  struct CharacterMenuState *menu;

  menu = gMenuWork;
  PsynergyMenu_InitializeEntryObjects(UiMenu_CreateCursor(menu), 2, 2, 8, 0);
  menu->unknownWindow028 = 0;
  menu->statusWindow = 0;
  menu->helpWindow = 0;
  menu->actionWindow = 0;
  menu->selectedColumn = 0;
  menu->selectedRow = 0;
  menu->columnCount = 8;
  menu->rowCount = 2;
}

/// Shows member resource's status in the status window (0, 5, 30 by 15),
/// opening it first with the member's portrait (kept as the work's
/// statusCursor), the 32 entry icons (the chooser's Psynergy icons) on the
/// chooser page, and the eight category sprites that show the ailment icons;
/// then draws the status and ailments. frame is not used.
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
    // The portrait goes to depth slot 240, as the category sprites do.
    if ((cursor->sentinel = 0xF0, menu->page) == CHARACTER_MENU_PAGE_CHOOSER) {
      Menu_SpawnIconEntries((struct InventoryMenuState *)menu, window);
    }
    Menu_CreateEightEntryObjects(window);
    CharacterMenu_DrawStatusAilments(window, resource, 0x100);
    return;
  }
  CharacterMenu_DrawStatusAilments(window, resource, 0);
}

/// The member's status page. The left pane lists their ailments (or the stat
/// help when they have none) and the right pane the status entries (name and
/// level, experience, HP, PP, Djinn when the party has any, elemental levels,
/// Power and Resist); the cursor's entry is highlighted and explained in the
/// help window. Up and Down move in a pane, Left and Right switch panes, L and
/// R switch to the previous or next member; A goes on to the Psynergy page and
/// B backs out. Returns 1 or -1.
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
  // The help window covers the top of the screen. Without their placement
  // callback the party figures under it are not drawn (the depth slots are
  // emptied every frame); the four Djinn figures go up from y 128 to 104
  // until the page closes.
  UiWindow_UpdateOrCreate(&menu->helpWindow, 0, 0, 30, 5, 2);
  Scheduler_RemoveCallback(Menu_UpdateEntryObjectTransforms);
  for (i = 3; i >= 0; i--)
    menu->rowY[i] = 104;
  done = 0;
  UiMenu_SlideCursor(-10, 88);

  // One member's page at a time; L and R start the next.
  while (done == 0 && GameFlag_TestFar(FLAG_MENU_START_PRESSED) == 0) {
    Unit_GetFar(menu->selectedIds[0]);
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
          // The right pane has eight entries, seven without the Djinn row.
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
      // The ailments are 16 pixels apart; the right pane's first four rows
      // (name to PP) are 8 apart, the rows below them 8 apart from y 112.
      menu->ownerCursors[0]->active = ICON_SHOWN;
      if (pane == 0)
        UiMenu_PositionCursor(-10, selected * 16 + 88);
      else if (selected <= 3)
        UiMenu_PositionCursor(24, selected * 8 + 48);
      else
        UiMenu_PositionCursor(48, selected * 8 + 80);
      WaitFrames(1);
      // Take the highlight off before the cursor moves.
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

/// Highlights (invert 0) or unhighlights (invert 1) the selected entry of the
/// status page: in pane 0 the selected ailment row, as wide as its label
/// (gCharacterMenu_CursorWidths); in pane 1 one of the first four status rows
/// or of the rows below.
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

/// Writes the advice for the selected ailment (two lines, for example how to
/// revive a member who is down) into window target, or that the member is well
/// when they have no ailment.
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

// The experience still needed is a u64 in the international editions and a
// u32 in the Japanese ones, as each edition's code matches; the message
// argument takes 32 bits either way.
#if TBS_PATCH_1
typedef u64 ExperienceDifference;
#else
typedef u32 ExperienceDifference;
#endif

/// Writes the help text of status entry messageVariant into destination: the
/// entries skip Djinn when the party has none (preserveVariant 0); the
/// experience entry says how many points the next level needs, or, at level 99,
/// gives the plain experience text. The text is copied into a heap buffer of
/// STATUS_HELP_TEXT_MAX characters, freed before returning.
void StatusMenu_ShowOwnerProgressMessage(struct UiWindow *destination,
                                         s32 messageVariant,
                                         s32 preserveVariant) {
  struct CharacterMenuState *menu = gMenuWork;
  void *buffer;

  // Entry 4 is the Djinn row's help: without Djinn the rows below it come
  // one entry earlier.
  if (preserveVariant == 0 && messageVariant > 3) {
    messageVariant++;
  }
  if (messageVariant == 1) {
    struct BattleUnit *owner = Unit_GetFar(menu->selectedIds[0]);

    if (owner->level == 99) {
      messageVariant = 8; // "These are your experience points."
    } else {
      ExperienceDifference remaining =
          Unit_GetExperienceForLevelFar(menu->selectedIds[0],
                                        owner->level + 1) -
          owner->experience;
      UiWork_PushValueSlotFar(remaining, MESSAGE_ARGUMENT_NUMBER);
    }
  }

  buffer = MemAlloc(STATUS_HELP_TEXT_MAX * 2);
  UiText_CopyMessageStringFar(messageVariant + MsgProgressHelp, buffer,
                              STATUS_HELP_TEXT_MAX);
  UiText_RenderWideStringAtOffsetFar(buffer, destination, 0, -1);
  MemFree(buffer);
}

/// Draws member ownerId's status page into window: the top rows (ItemMenu_
/// DrawOwnerStatus), the ailment list or "Normal", and except on the chooser
/// page the Djinn counts by element and, in mode 1, the elemental levels,
/// Power and Resist. mode's 0x100 keeps what is drawn instead of clearing it.
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
  unit = Unit_GetFar(ownerId);
  // The Djinn row, in tiles: higher on the full page (mode 1), which has the
  // elemental rows below it.
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
/// Spins for 256 loop passes (international editions only). Nothing calls it.
void Runtime_RunBusyWaitDelay(void) {
  s32 count = 0xff;

  // Written with goto (MATCHING.md): as a do-while loop, the loop optimizer
  // deletes the empty loop and leaves 1 of the 7 instructions.
spin:
  count--;
  if (count >= 0)
    goto spin;
}

#endif

/// Draws the top rows of member ownerId's status page into window: name, class,
/// level, experience, HP and PP on the left, Attack, Defense, Agility and Luck
/// on the right. flags' 0x100 keeps what is drawn instead of clearing it.
void ItemMenu_DrawOwnerStatus(struct UiWindow *window, s32 ownerId, s32 flags) {
  struct InventoryMenuState *menu;
  struct BattleUnit *unit;

  menu = gMenuWork;
  unit = Unit_GetFar(ownerId);
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
  UiText_DrawNumberInWindowFar(unit->hpMax, 4, window, 112, 16);
  UiText_DrawStringInWindowFar(gMenu_SlashString, window, 104, 16);
  MENU_DRAW_STATUS_LABEL(gMenu_PpString, window, 40, 24);
  UiText_DrawNumberInWindowFar(unit->pp, 4, window, 72, 24);
  UiText_DrawNumberInWindowFar(unit->ppMax, 4, window, 112, 24);
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

/// Fills ailments[0..4] (CharacterAilment) with member ownerId's ailments: down
/// (only when requested is 1), poison or venom, curse and haunt. Returns how
/// many they have.
s32 CharacterMenu_BuildAvailability(u8 *ailments, s32 requested, s32 ownerId) {
  struct BattleUnit *unit;
  s32 ailmentIndex;
  s32 count;
  s32 poison;

  unit = Unit_GetFar(ownerId);
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

/// Fills result with the item list of the member in pane index for the
/// equipment pages: their unit, the menu's item count, and the page and row of
/// the item chosen last for them, STATUS_EQUIP_PAGE_SIZE items to a page.
/// Returns 1.
s32 Menu_BuildPageResult(struct MenuResult *result, s32 index) {
  s32 unitBits;
  struct InventoryMenuState *menu = gMenuWork;
  s32 itemCount;
  s32 row;
  s32 page;
  s32 pageCount;
  s32 selected;

  unitBits = (s32)Unit_GetFar(menu->paneOwner[index]);
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

static __inline__ u32 XorWord(u32 word, u32 mask) { return word ^ mask; }

extern const struct TileMask gMenu_PatternRowMasks[];

/// Builds the Psynergy range icons' tiles in the window tile area: two sets of
/// six tile pairs (24 8x8 tiles), each a fill of colour 4 with the rows of
/// gMenu_PatternRowMasks XORed in, the first set growing row by row and the
/// second skipping its top row. Pair n is range icon n, at RANGE_ICON_TILE +
/// 2n (tile 0x200, or 0x280 in the international editions, whose tiles start
/// 0x1000 bytes further on), which PsynergyMenu_DrawRange draws. Called as
/// the Status menu's Psynergy page opens (PsynergyMenu_SelectAction).
void Menu_BuildPatternTiles(void) {
#if TBS_PATCH_1
  u32 *vram = (u32 *)(BG_CHAR_BLOCK(1) + 0x1000);
#else
  u32 *vram = (u32 *)BG_CHAR_BLOCK(1);
#endif
  s32 set;
  s32 tileIndex;

  // A 4bpp tile row is one word: a pair is 16 words (64 bytes), a set 0x60
  // words (384 bytes).
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

/// Draws a Psynergy range icon at tile (x, y) of window: three tiles from
/// pair range of the range icon tiles (Menu_BuildPatternTiles), the side tile
/// on both ends, mirrored on the left, and the centre tile between. range is
/// the ability's range less one (0 for the aimed unit alone), or 11 for
/// ABILITY_RANGE_ALL (PsynergyMenu_DrawListPage).
void PsynergyMenu_DrawRange(struct UiWindow *window, s32 x, s32 y, s32 range,
                            s32 unused) {
  s32 iconIndex;
  s32 tile;

  (void)unused;

  iconIndex = range * 2;
  tile = iconIndex + RANGE_ICON_TILE + 1;
  // 0x400 is the map entry's horizontal flip bit.
  UiWindow_SetTilemapEntryFar(window, 0x400 | tile, x, y, 0); // flipped
  UiWindow_SetTilemapEntryFar(window, iconIndex + RANGE_ICON_TILE, x + 1, y, 0);
  UiWindow_SetTilemapEntryFar(window, tile, x + 2, y, 0);
}

/// Updates the Psynergy page for the row under the cursor: the Psynergy's
/// description in the info window, where it can be used (in the field, in
/// battle, or anywhere), and the row highlights with the range icons.
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
    ability = BattleAction_Get(menu->psynergies[state->selectedIndex] &
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

  // Highlight the cursor's row. A Psynergy with an element shows its icon at
  // tile column 24, in palette 0 on the highlighted row and palette 1
  // (mode 4) elsewhere, and the highlight leaves that column out.
  base = state->page * PAGE_ROWS;
  for (row = 0; row <= PAGE_ROWS - 1; row++) {
    if (row == state->row) {
      ability =
          BattleAction_Get(menu->psynergies[base + row] & OWNER_ACTION_ID_MASK);
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
          BattleAction_Get(menu->psynergies[base + row] & OWNER_ACTION_ID_MASK);
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

/// Draws one page of the Status menu's Psynergy list into window: up to
/// PAGE_ROWS names with their icons, PP costs and range icons, the page
/// numbers or "no Psynergy", and the member's name, class and level. Returns
/// 1.
s32 PsynergyMenu_DrawListPage(struct UiWindow *window, s32 unused,
                              const struct MenuResult *res) {
  struct PsynergyMenuState *menu = gMenuWork;
  struct BattleUnit *owner;
  u32 firstEntry;
  u32 rows;
  u8 row;
  s32 entryIndex;

  (void)unused;

  owner = Unit_GetFar(menu->ownerIds[0]);

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
          BattleAction_Get(OWNER_ACTION_ID_MASK & menu->psynergies[entryIndex]);
      messageId = (menu->psynergies[entryIndex] & OWNER_ACTION_ID_MASK) +
                  MsgAbilityName;
      y = row * 16 + LIST_FIRST_ROW * 8;

      UiText_DrawCharacterAtOffsetFar(messageId, window, LIST_NAME_X, y);
      UiText_DrawNumberAtOffsetFar(action->ppCost, 2, window, LIST_PP_NUMBER_X,
                                   y);

      // ABILITY_RANGE_ALL takes the last range icon, 11; range n takes n - 1.
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

/// The Status menu's Psynergy page: the member's Psynergy, PAGE_ROWS to a page,
/// each with its PP cost and range icon; the cursor's Psynergy is described in
/// the info window with where it can be used. Up and Down move the cursor,
/// Left and Right turn pages, L and R switch member. Returns 1 for A (on to
/// the equipment page) or -1 for B (back to the member row).
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
  // slotY is CharacterMenuState's rowY: the four Djinn figures go above the
  // screen (y -16) while this page is up, until the member row or the status
  // page puts them back.
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
    ItemMenu_PositionCategory();
    RenderOutput_RedrawSavedRectFar(menu->statusWindow);
    menu->psynergyCount = PsynergyMenu_CollectActions(
        Unit_GetFar(menu->ownerIds[0]), menu->psynergies, PSYNERGY_LIST_ALL);
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

/// Nothing calls it.
s32 PsynergyMenu_ActionReturnTrue(void) { return 1; }

/// Hides every entry icon, then loads the icons of the Psynergy already listed
/// in the work (psynergies) into them. ownerId is only looked up.
void PsynergyMenu_DrawPreparedPsynergyIcons(const void *unused, s32 ownerId) {
  struct PsynergyMenuState *menu = gMenuWork;

  Unit_GetFar(ownerId);
  ItemMenu_HideAllIcons();
  PsynergyMenu_DrawPsynergyIcons(menu->psynergies);
}

/// Nothing calls it.
void PsynergyMenu_PreparedIconsNoOp(void) {}

/// Nothing calls it.
void PsynergyMenu_PreparedIconsSecondNoOp(void) {}

/// Updates the Status menu's equipment page for the row under the cursor: the
/// item's plain name in the info window, the row highlights, and in the
/// equipment window the highlight of the slot the item is worn in, if it is.
/// Returns 1.
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
    MENU_DRAW_INFO_TEXT(
        (menu->items[state->selectedIndex] & INVENTORY_ITEM_ID_MASK) +
            MsgItemPlainName,
        menu->infoWindow, 0, 0);
  }

  // An item with an element shows its icon at tile column 27, in palette 0 on
  // the highlighted row and palette 1 (mode 4) elsewhere, and the highlight
  // stops short of it.
  for (rowIndex = 0; rowIndex < PAGE_ROWS; rowIndex++) {
    if (rowIndex == state->row) {
      item = Item_GetDirectFar(menu->items[state->selectedIndex] &
                               INVENTORY_ITEM_ID_MASK);
      if (item->element != ELEMENT_NONE) {
        UiWindow_SetTilemapEntryFar(window, item->element + 1, 27,
                                    rowIndex * 2 + 1, 0);
        Render_SetTilemapFlagRect(window, 14, rowIndex * 2 + 1, 13, 1, 14);
      } else {
        Render_SetTilemapFlagRect(window, 14, rowIndex * 2 + 1, 14, 1, 14);
      }
    } else {
      item = Item_GetDirectFar(menu->items[firstEntry + rowIndex] &
                               INVENTORY_ITEM_ID_MASK);
      if (item->element != ELEMENT_NONE) {
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

  // The slots' name rows: weapon 1, head 3, shield 5, chest 7, each under
  // its label (ItemMenu_DrawCategory). Other worn items have no row.
  if (menu->items[state->selectedIndex] & INVENTORY_EQUIPPED) {
    item = Item_GetDirectFar(menu->items[state->selectedIndex] &
                             INVENTORY_ITEM_ID_MASK);
    switch (item->type) {
    case ITEM_TYPE_WEAPON:
      Render_SetTilemapFlagRect(menu->equipWindow, STATUS_EQUIP_SLOT_X, 1,
                                STATUS_EQUIP_SLOT_WIDTH, 1, 14);
      break;
    case ITEM_TYPE_HELM:
      Render_SetTilemapFlagRect(menu->equipWindow, STATUS_EQUIP_SLOT_X, 3,
                                STATUS_EQUIP_SLOT_WIDTH, 1, 14);
      break;
    case ITEM_TYPE_SHIELD:
      Render_SetTilemapFlagRect(menu->equipWindow, STATUS_EQUIP_SLOT_X, 5,
                                STATUS_EQUIP_SLOT_WIDTH, 1, 14);
      break;
    case ITEM_TYPE_ARMOR:
      Render_SetTilemapFlagRect(menu->equipWindow, STATUS_EQUIP_SLOT_X, 7,
                                STATUS_EQUIP_SLOT_WIDTH, 1, 14);
      break;
    }
  }
  WaitFrames(1);
  return 1;
}

/// Draws one page of the Status menu's equipment page into window: up to
/// PAGE_ROWS item names with their icons and the page numbers, or "no items",
/// and the member's name with their Attack and Defense. Returns 1.
/// Despite its name, no shop calls it: ItemMenu_SelectItem draws each page with
/// it.
s32 Shop_DrawItemPage(struct UiWindow *window, s32 unused,
                      struct MenuResult *state) {
  struct InventoryMenuState *menu = gMenuWork;
  struct BattleUnit *unit;
  u16 *inventoryEntry;
  s32 firstEntry;
  u32 rowCount;
  u32 rowIndex;
  s32 message;

  unit = Unit_GetFar(menu->paneOwner[0]);
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
            (*inventoryEntry & INVENTORY_ITEM_ID_MASK) + MsgItemName, window,
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

/// The Status menu's equipment page: the member's items in the first list
/// order (LIST_SORT_EQUIPPED_THEN_ITEMS), PAGE_ROWS to a page, beside
/// the equipment window of what they wear. Up and Down move the cursor, Left
/// and Right turn pages, L and R switch member. Returns 1 for A (back to the
/// status page) or -1 for B (back to the member row). Only the list is
/// sorted; the inventory itself is left as it is.
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
    Unit_GetFar(menu->ownerIds[ownerId]);
  }
  UiWindow_UpdateOrCreate(&menu->equipWindow, 0, 10, STATUS_EQUIP_WINDOW_WIDTH,
                          10, 2);
  ItemMenu_PositionCategory();

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
  ItemMenu_PositionCategory();
  WaitFrames(1);
  Menu_SpawnIconEntries((struct InventoryMenuState *)menu, menu->messageWindow);

  done = 0;
  while (done == 0 && GameFlag_TestFar(FLAG_MENU_START_PRESSED) == 0) {
    ItemMenu_PositionCategory();
    RenderOutput_RedrawSavedRectFar(menu->statusWindow);
    owner = Unit_GetFar(menu->paneOwner[0]);
    for (iconIndex = 0; iconIndex <= 14; iconIndex++)
      savedInventory[iconIndex] = owner->inventory[iconIndex];
    menu->itemCount = ItemMenu_CollectItems(owner, menu->items, 0);
    InventoryMenu_SortByListOrder(menu->items, LIST_SORT_EQUIPPED_THEN_ITEMS);
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
        // Copy back the inventory saved as the list was built; only the list
        // was sorted, so this writes what is already there.
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

/// Moves through the status grid's irregular rows and returns row * 9 + column.
/// Nothing in TBS calls it.
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

/// Nothing calls it.
s32 Menu_ReservedStatusOne(void) { return 1; }

/// Writes the four equipment slot labels (Weapon, Head, Shield and Chest, one
/// a 16-pixel row) and the names of the items worn in them into window; mode 0
/// also shows their icons at the right. Reads the work's items list, which the
/// caller fills (ItemMenu_CollectItems). Used by the item screen's R view and
/// the Status menu's equipment page.
void ItemMenu_DrawCategory(struct UiWindow *window, s32 ownerId, s32 mode) {
  struct InventoryMenuState *menu = gMenuWork;
  u16 *items;

  Unit_GetFar(ownerId);
  ItemMenu_PositionCategory();
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

/// Writes the name of each worn weapon, armor, shield and helm under its slot
/// label (ItemMenu_DrawCategory). Other worn items are not listed.
void ItemMenu_DrawEquippedItemNames(struct UiWindow *window, u16 *items) {
  s32 slotIndex;
  s32 itemId;

  for (slotIndex = 0; slotIndex < INVENTORY_SLOTS; slotIndex++) {
    if (items[slotIndex] & INVENTORY_EQUIPPED) {
      itemId = items[slotIndex] & INVENTORY_ITEM_ID_MASK;
      switch (Item_GetDirectFar(itemId)->type) {
      case ITEM_TYPE_WEAPON:
        UiText_DrawCharacterAtOffsetFar(itemId + MsgItemName, window,
                                        STATUS_EQUIPPED_NAME_X, 8);
        break;
      case ITEM_TYPE_ARMOR:
        UiText_DrawCharacterAtOffsetFar(itemId + MsgItemName, window,
                                        STATUS_EQUIPPED_NAME_X, 56);
        break;
      case ITEM_TYPE_SHIELD:
        UiText_DrawCharacterAtOffsetFar(itemId + MsgItemName, window,
                                        STATUS_EQUIPPED_NAME_X, 40);
        break;
      case ITEM_TYPE_HELM:
        UiText_DrawCharacterAtOffsetFar(itemId + MsgItemName, window,
                                        STATUS_EQUIPPED_NAME_X, 24);
        break;
      }
    }
  }
}

/// Places the 32 entry icons in a grid of 16x16 cells, columns wide, from
/// (originX, originY), and shows them: the member chooser's Psynergy icons.
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

/// Places icon at cell index (an index above 31 is taken as 0) of a grid of
/// 16x16 cells, columns wide, from (originX, originY), and shows it.
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

/// Places the icon of each worn weapon, armor, shield and helm at x 216, on
/// its slot's row of ItemMenu_DrawCategory's window; the others stay off
/// screen. The icons are loaded already (ItemMenu_DrawIcons).
void ItemMenu_ArrangeCategoryItemIcons(u16 *items) {
  struct InventoryMenuState *menu;
  struct RenderOutput *icon;
  s32 slotIndex;

  menu = gMenuWork;
  ItemMenu_PositionCategory();
  for (slotIndex = 0; slotIndex < INVENTORY_SLOTS; slotIndex++) {
    if (items[slotIndex] != 0 && (items[slotIndex] & INVENTORY_EQUIPPED) != 0) {
      icon = menu->entryIcons[slotIndex];
      if (icon != 0) {
        switch (Item_GetDirectFar(items[slotIndex] & INVENTORY_ITEM_ID_MASK)
                    ->type) {
        case ITEM_TYPE_WEAPON:
          icon->x = 216;
          icon->y = 32;
          break;
        case ITEM_TYPE_ARMOR:
          icon->x = 216;
          icon->y = 80;
          break;
        case ITEM_TYPE_SHIELD:
          icon->x = 216;
          icon->y = 64;
          break;
        case ITEM_TYPE_HELM:
          icon->x = 216;
          icon->y = 48;
          break;
        }
        UiIcon_PrepareObject(icon);
      }
    }
  }
}

/// Moves all 32 entry icons off screen, to (248, 168), before a screen lays
/// them out again.
void ItemMenu_PositionCategory(void) {
  struct InventoryMenuState *menu = gMenuWork;
  s32 x = 248;
  struct RenderOutput **entry = menu->entryIcons;
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

/// Creates the eight category sprites in window, off screen at (248, 168),
/// each on item graphic slot of the same number; the Status menu shows the
/// ailment icons with the first five (CharacterMenu_UpdateSelectionIcons).
/// Returns 1.
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
    icon = RenderOutput_CreateFromResourceFar(ICON_KIND_ITEM, slotIndex, window,
                                              0xF8, spriteFlags);
    slotIndex += 1;
    *slot = icon;
    slot += 1;
  } while (slotIndex <= 7);
  return 1;
}

/// Shows the icon of each ailment that flags marks
/// (CharacterMenu_BuildAvailability) down the left edge from y 88, 16 pixels
/// apart, beside the ailment labels of CharacterMenu_DrawStatusAilments.
void ItemMenu_ApplyFlags(const u8 *flags) {
  /* FAKEMATCH(temporary): y stored through a pointer to the field keeps the
   * flag index a counter instead of a walking pointer; as entry->y = nextY,
   * 11 of 33 instructions differ in tbs-en. */
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

/// Moves the five ailment icons (the first category sprites) off screen,
/// before they are shown again or while another view covers them.
void ItemMenu_ResetCategory(void) {
  struct InventoryMenuState *menu = gMenuWork;
  s32 index;

  for (index = 0; index < 5; index++) {
    struct RenderOutput *object = menu->categoryIcons[index];

    if (object != 0) {
      object->x = 248;
      object->y = 168;
      object->sentinel = 240;
      UiIcon_PrepareObject(object);
    }
  }
}

/// Loads each enabled ailment's icon into its category resource slot.
s32 CharacterMenu_UpdateSelectionIcons(const u8 *enabled) {
  struct InventoryMenuState *menu = gMenuWork;
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
      Resource_LoadByModeIntoSlotFar(ICON_KIND_PATTERN, kind,
                                     (u8)menu->categoryIcons[index]->index, 0);
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
