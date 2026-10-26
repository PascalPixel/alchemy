/// @file
/// The member chooser and the Status menu: its member row, status page,
/// Psynergy page and equipment page.
// Part of menu.c.

#include "../menu_internal.h"

// The international editions reset eight party figure rows rather than four,
// and their longer words need more window.
//
// How many party figure rows MemberChooser_Open resets.
#if TBS_PATCH_1
#define MEMBER_CHOOSER_MEMBER_COUNT 8
#else
#define MEMBER_CHOOSER_MEMBER_COUNT 4
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
#define STATUS_ITEM_PAGE_ICONS_X 119
#define STATUS_ITEM_NAME_X 128
#define STATUS_ITEM_STAT_X 80
#else
#define STATUS_ITEM_PAGE_ICONS_X 123
#define STATUS_ITEM_NAME_X 136
#define STATUS_ITEM_STAT_X 72
#endif

#if LANGUAGE == LANGUAGE_GERMAN
#define STATUS_ITEM_EMPTY_MESSAGE_X 112
#else
#define STATUS_ITEM_EMPTY_MESSAGE_X 120
#endif

// The equipment window, and the equipped items' names in it.
#if LANGUAGE != LANGUAGE_JAPANESE
#define STATUS_EQUIP_WINDOW_WIDTH 15
#define STATUS_EQUIPPED_NAME_X 8
#else
#define STATUS_EQUIP_WINDOW_WIDTH 13
#define STATUS_EQUIPPED_NAME_X 16
#endif

/// Lets the player choose one party member, over the field (the Colosso maps
/// use it, through MENU.S, and so does the debug test). Returns the chosen
/// member's id, or -1 when the player backs out. Owns the menu work
/// (HEAP_SLOT_MENU) while it runs and frees it before returning.
s32 MemberChooser_Open(void) {
  struct SummaryScreenWork *menu =
      AllocHeapSlot(HEAP_SLOT_MENU, MENU_WORK_SIZE);
  s32 result;
  s32 index;

  gObjectSystemWork->suspended = 1;
  Window_DrawFrameFar(0, 0, 30, 20);
  WaitFrames(1);
  Window_InitializeWork(0);

  menu->partyCount = Party_ListMembersFar(menu->memberIds);
  Menu_InitializeSelectorCursorAndEntries(0, 3, 0, 7);
  menu->selectorWindow = Window_CreateFar(13, 0, 17, 5, WINDOW_FLAG_INSTANT);
  for (index = 0; index < MEMBER_CHOOSER_MEMBER_COUNT; index++)
    menu->memberY[index] = 30;
  menu->page = SUMMARY_SCREEN_PAGE_CHOOSER;

  result = MemberChooser_Run();

  WindowSprite_ClearListFar(menu->statusWindow);
  ItemMenu_Close();
  gObjectSystemWork->suspended = 0;
  WaitFrames(1);
  FreeHeapSlot(HEAP_SLOT_MENU);
  return result;
}

/// The member chooser's loop. Returns the chosen member's id, or -1.
s32 MemberChooser_Run(void) {
  struct SummaryScreenWork *work = gMenuWork;
  s32 result = 0;

  /* FAKEMATCH(one-pass): the slot's clearing in a one-pass block stores the
   * zero before the slot's offset is built; as a plain statement, 4 of 23
   * instructions differ in tbs-en. */
  do {
    work->selectedSlots[0] = result;
  } while (0);
  if (SummaryScreen_SelectMember(0) == -1)
    result = -1;
  else
    result = work->selectedIds[0];
  return result;
}

/// The summary screen, the field menu's Status (FieldMenu_Run,
/// FIELD_MENU_STATUS), over the field: saves the first two background palettes
/// and the window tiles it draws over, sets up the party row and, when the
/// party has Djinn, the four Djinn sprites, runs the status pages, then puts
/// everything back. Returns their result; -1 (backed out or Start) opens the
/// field menu again. The menu work and the two saved copies, taken from the
/// EWRAM pool, are freed before returning.
s32 SummaryScreen_Run(void) {
  struct SummaryScreenWork *menu =
      AllocHeapSlot(HEAP_SLOT_MENU, MENU_WORK_SIZE);
  void *palette = AllocEwram(64);
  void *tiles = AllocEwram(STATUS_SAVED_TILE_BYTES);
  s32 result;
  s32 index;

  ((struct ObjectSystemWork *)HEAP_SLOT_BLOCK(HEAP_SLOT_OBJECT_SYSTEM))
      ->suspended = 1;
  Window_DrawFrameFar(0, 0, 30, 20);
  WaitFrames(1);
  Menu_SuspendFieldTasks();
  Window_InitializeWork(0);
  menu->page = SUMMARY_SCREEN_PAGE_STATUS;
  menu->partyCount = Party_ListMembersFar(menu->memberIds);
  Menu_InitializeSelectorCursorAndEntries(0, 3, 0, 7);
  Iwram_CopyWords(palette, (void *)BG_PLTT, 64);
#if !TBS_PATCH_1
  Iwram_CopyWords(tiles, BG_CHAR_BLOCK(1), STATUS_SAVED_TILE_BYTES);
#endif
  Palette_LightenBankHighlight(HIGHLIGHT_PALETTE);
  // The first sprite palette into background palettes 0 and 1, for the
  // element icons drawn as window tiles: palette 0, for the highlighted row,
  // takes HIGHLIGHT_PALETTE's WINDOW_BACKGROUND_COLOR as its
  // TEXT_BACKGROUND_COLOR, palette 1 WINDOW_PALETTE's
  // (PsynergyMenu_DrawRangePage).
  Dma_Set((void *)OBJ_PLTT, (void *)BG_PLTT, DMA_ENABLE << 16 | 16, REG_DMA3);
  Dma_Set((void *)&BG_PLTT_COLOR(HIGHLIGHT_PALETTE, WINDOW_BACKGROUND_COLOR),
          (void *)&BG_PLTT_COLOR(0, TEXT_BACKGROUND_COLOR),
          DMA_ENABLE << 16 | 1, REG_DMA3);
  Dma_Set((void *)OBJ_PLTT, (void *)&BG_PLTT_COLOR(1, 0), DMA_ENABLE << 16 | 16,
          REG_DMA3);
  Dma_Set((void *)&BG_PLTT_COLOR(WINDOW_PALETTE, WINDOW_BACKGROUND_COLOR),
          (void *)&BG_PLTT_COLOR(1, TEXT_BACKGROUND_COLOR),
          DMA_ENABLE << 16 | 1, REG_DMA3);
#if TBS_PATCH_1
  Iwram_CopyWords(tiles, BG_CHAR_BLOCK(1), STATUS_SAVED_TILE_BYTES);
  Iwram_FillWords(BG_CHAR_BLOCK(1), STATUS_SAVED_TILE_BYTES, 0x33333333);
  Text_UseExtraTilesFar(1);
#endif
  menu->selectorWindow = Window_CreateFar(13, 0, 17, 5, WINDOW_FLAG_INSTANT);
  for (index = 0; index < MENU_ROW_COUNT; index++)
    menu->memberY[index] = 30;
  if (Party_SumDjinnCountsFar(-1) != 0)
    DjinnFigures_InitializeTopRow(menu->selectorWindow, 0);
  // The four Djinn figures in a row 32 pixels apart, at screen y 128.
  for (index = 0; index < 4; index++) {
    menu->rowX[index] = 130 + index * 32;
    menu->rowY[index] = 128;
  }
  Link_DrawPageArrowsFar(PAGE_ARROW_VRAM);
  // Run the pages.
  Menu_CancelSoundReset();
  menu->page = SUMMARY_SCREEN_PAGE_STATUS;

  result = SummaryScreen_RunPages();

  // Close, and put the palettes and tiles back.
  Menu_EnsureCancelSound();
  WindowSprite_ClearListFar(menu->statusWindow);
  DjinnFigures_ReleaseTopRow();
  Menu_ResumeFieldTasks();
  Window_DrawFrameFar(0, 0, 30, 20);
  WaitFrames(1);
#if TBS_PATCH_1
  Window_MarkVisibleTextTilesFar();
  Text_UseExtraTilesFar(0);
  WaitFrames(1);
#endif
  Iwram_CopyWords((void *)BG_PLTT, palette, 64);
  Iwram_CopyWords(BG_CHAR_BLOCK(1), tiles, STATUS_SAVED_TILE_BYTES);
  Free(tiles);
  Free(palette);
  ((struct WindowWork *)HEAP_SLOT_BLOCK(HEAP_SLOT_WINDOW))->holdTilemapUpload =
      1;
  ItemMenu_Close();
  Window_DrawFrameFar(0, 0, 30, 20);
  FreeHeapSlot(HEAP_SLOT_MENU);
  ((struct ObjectSystemWork *)HEAP_SLOT_BLOCK(HEAP_SLOT_OBJECT_SYSTEM))
      ->suspended = 0;
  WaitFrames(1);
  Window_EraseBorderRectFar(0, 0, 30, 20);
  ((struct WindowWork *)HEAP_SLOT_BLOCK(HEAP_SLOT_WINDOW))->holdTilemapUpload =
      0;
  return result;
}

/// The steps of the Status menu (SummaryScreen_RunPages).
enum SummaryScreenStep {
  STATUS_STEP_MEMBER = 0,   ///< choose a member
  STATUS_STEP_STATUS = 1,   ///< their status page
  STATUS_STEP_PSYNERGY = 2, ///< their Psynergy page
  STATUS_STEP_EQUIPMENT = 3 ///< their equipment page
};

/// The Status menu's pages: choose a party member, then their status page, A
/// for their Psynergy page and A again for their equipment page, whose A goes
/// back round to the status page; B on any page goes back to the member row.
/// Returns -1 when the player backs out of the member row or presses Start.
s32 SummaryScreen_RunPages(void) {
  struct SummaryScreenWork *menu = gMenuWork;
  s32 step = STATUS_STEP_MEMBER;
  s32 finished = step;
  s32 result = 0;
  u32 changed;

  while (!finished && !GameFlag_GetFar(FLAG_MENU_START_PRESSED)) {
    switch (step) {
    case STATUS_STEP_MEMBER:
      menu->selectedSlots[0] = finished;
      if (SummaryScreen_SelectMember(0) == -1) {
        result = -1;
        finished = 1;
      }
      step = STATUS_STEP_STATUS;
      break;
    case STATUS_STEP_STATUS:
      menu->memberCursors[0]->motion = ICON_HIDDEN;
      result = SummaryScreen_SelectCommand();
      step = result == -1 ? STATUS_STEP_MEMBER : STATUS_STEP_PSYNERGY;
      break;
    case STATUS_STEP_PSYNERGY:
      menu->memberCursors[0]->motion = ICON_HIDDEN;
      result = PsynergyMenu_SelectAction();
      step = STATUS_STEP_MEMBER;
      if (result != -1)
        step = STATUS_STEP_EQUIPMENT;
      break;
    case STATUS_STEP_EQUIPMENT:
      menu->memberCursors[0]->motion = ICON_HIDDEN;
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

  if (GameFlag_GetFar(FLAG_MENU_START_PRESSED))
    result = -1;

  return result;
}

/// Lets the player choose a party member in slot, from the one chosen last: the
/// member chooser on SUMMARY_SCREEN_PAGE_CHOOSER, else the Status menu's party
/// row. Returns the chooser's result: 1 when a member was chosen, -1 when the
/// player backed out, and on the party row 0 when Start ended it.
s32 SummaryScreen_SelectMember(s32 slot) {
  struct SummaryScreenWork *menu;
  struct WindowSprite *cursor;
  s32 result;
  s32 index;

  menu = gMenuWork;
  index = menu->memberIndex[slot];
  result = 0;
  cursor = menu->memberCursors[slot];
  cursor->motion = ICON_SHOWN;
  cursor->animationStep = 0;
  WindowSprite_RedrawSavedRectFar(menu->memberWindow);
  // FLAG_LINK_BATTLE_ARRANGED is set while a link battle is arranged in the
  // Battle Arena, in which only the first three members fight
  // (BattleArena_PartyContains); the divider falls between the third and
  // fourth figures, which stand three tiles apart.
  if (GameFlag_GetFar(FLAG_LINK_BATTLE_ARRANGED))
    Window_DrawDividerLineFar(menu->memberWindow, 9, 1, 9, 3);
  if (index == -1)
    menu->memberIndex[slot] = 0;
  else
    MenuCursor_Slide(index * 24 - 10, 16);
  if (menu->page == SUMMARY_SCREEN_PAGE_CHOOSER)
    result = PsynergyMenu_ChooseMember();
  else
    result = MemberChooser_RunRearrange();
  cursor = menu->memberCursors[slot];
  Icon_PrepareObject(cursor);
  WaitFrames(1);
  return result;
}

/// Shows every Djinn the party has found, a column of seven for each element,
/// until A, B or Select, then puts the screen back. Select on the Status
/// menu's party row opens it, once the first Djinni is caught. Reads the menu
/// work as the Djinn menu's view (struct DjinnMenuWork).
s32 DjinnMenu_ShowCurrentList(void) {
  struct DjinnMenuWork *menu;
  struct Window *window;
  s32 element;
  s32 djinn;

  menu = gMenuWork;
  ItemMenu_ResetCategory();
  Sound_PlaySongFar(SE_MENU_CONFIRM);
  WindowSprite_RedrawSavedRectFar(menu->helpWindow);
  Text_DrawMessageAsTilesFar(MsgReturnHelp, menu->helpWindow, 0, 16);
  menu->cursor->motion = ICON_HIDDEN;
  menu->secondCursor->motion = ICON_HIDDEN;
  WaitFrames(1);
  window = menu->window;
  {
    s32 i;

    for (i = 0; i < 4; i++) {
      menu->slotX[i] = 32 + i * 56;
      menu->slotY[i] = 70;
      menu->memberY[i] = 30;
    }
  }
  WindowSprite_RedrawSavedRectFar(window);
  Window_DrawDividerLineFar(window, 0, 11, 28, 11);
  Text_DrawMessageFar(MsgCurrentDjinn, menu->helpWindow, -96, 132);
  for (element = 0; element < ELEMENT_COUNT; element++) {
    for (djinn = 0; djinn < 7; djinn++) {
      s32 id = element * DJINN_PER_ELEMENT + djinn;

      // FLAG_DJINN_FIRST on: one per Djinni found, DJINN_PER_ELEMENT to an
      // element. Each name
      // follows its element's icon: window tile 1 + element in palette 1
      // (0x1000).
      if (GameFlag_GetFar(FLAG_DJINNI(id))) {
        Window_SetTilemapEntryFar(window, 0x1001 + element, element * 7 + 1,
                                  djinn + 3, 0);
        Text_DrawMessageAsTilesFar(MsgDjinnName + id, window, element * 56 + 16,
                                   djinn * 8 + 24);
      }
    }
  }
  ((struct WindowWork *)gWindowWork[0])->dirty = 1;
  for (;;) {
    if (GameFlag_GetFar(FLAG_MENU_START_PRESSED))
      break;
    WaitFrames(1);
    if (gKeysPressed & (KEY_A | KEY_B | KEY_SELECT))
      break;
  }
  WindowSprite_RedrawSavedRectFar(menu->window);
  WindowSprite_ClearListFar(menu->helpWindow);
  {
    s32 i;

    for (i = 0; i < 4; i++) {
      menu->slotX[i] = 130 + i * 32;
      menu->slotY[i] = 128;
    }
  }
  menu->cursor->motion = ICON_SHOWN;
  menu->secondCursor->motion = ICON_SHOWN;
  Sound_PlaySongFar(SE_MENU_CANCEL);
  // Returns no value: the bytes have the s32 epilogue (pop {r1}; bx r1) after
  // this void call.
#ifdef UBFIX
  return 0;
#endif
}

struct ItemMenuWork;

/// Lowers the party figures, then raises the currently selected member.
static __inline__ void
SummaryScreen_SetSelectedMemberRow(struct SummaryScreenWork *menu,
                                   s32 selectedMember) {
  s32 rowIndex;

  for (rowIndex = MENU_ROW_COUNT - 1; rowIndex >= 0; rowIndex--)
    menu->memberY[rowIndex] = 30;
  menu->memberY[selectedMember] = 26;
}

/// The Status menu's party row: Left and Right choose a member, showing their
/// status; L and R move the member one place along the party order; Select
/// lists the Djinn found (once the first is caught, FLAG_DJINN_FIRST); A
/// chooses the member and B backs out. Returns 1 or -1, or 0 when Start ends
/// it, with the member in the work's memberIndex[0] and selectedIds[0]. Moving
/// a member changes the party order for good (MemberChooser_MoveEntry).
s32 MemberChooser_RunRearrange(void) {
  struct SummaryScreenWork *menu;
  s32 memberIndex;
  s32 memberCount;
  s32 page;
  s32 redraw;
  s32 result;
  s32 rowIndex;

  menu = gMenuWork;
  memberIndex = menu->memberIndex[0];
  memberCount = menu->memberCount[0];
  redraw = 1;
  result = 0;
  page = menu->page;
  Unit_GetFar(menu->memberIds[memberIndex]);
  for (rowIndex = 0; rowIndex < 4; rowIndex++) {
    menu->rowX[rowIndex] = 130 + rowIndex * 32;
    menu->rowY[rowIndex] = 128;
  }
  // The highlight palette, and the first sprite palette into background
  // palettes 0 and 1, as SummaryScreen_Run sets them.
  Palette_LightenBankHighlight(HIGHLIGHT_PALETTE);
  Dma_Set((void *)OBJ_PLTT, (void *)BG_PLTT, DMA_ENABLE << 16 | 16, REG_DMA3);
  Dma_Set((void *)&BG_PLTT_COLOR(HIGHLIGHT_PALETTE, WINDOW_BACKGROUND_COLOR),
          (void *)&BG_PLTT_COLOR(0, TEXT_BACKGROUND_COLOR),
          DMA_ENABLE << 16 | 1, REG_DMA3);
  Dma_Set((void *)OBJ_PLTT, (void *)&BG_PLTT_COLOR(1, 0), DMA_ENABLE << 16 | 16,
          REG_DMA3);
  Dma_Set((void *)&BG_PLTT_COLOR(WINDOW_PALETTE, WINDOW_BACKGROUND_COLOR),
          (void *)&BG_PLTT_COLOR(1, TEXT_BACKGROUND_COLOR),
          DMA_ENABLE << 16 | 1, REG_DMA3);
  while (!GameFlag_GetFar(FLAG_MENU_START_PRESSED)) {
    if (redraw) {
      redraw = 0;
      // The help lines, and the member's status and figure.
      WindowSprite_RedrawSavedRectFar(menu->selectorWindow);
      Text_DrawMessageAsTilesFar(MsgRearrangeHelp, menu->selectorWindow, 0, 0);
      if (GameFlag_GetFar(FLAG_DJINN_FIRST))
        Text_DrawMessageAsTilesFar(MsgDjinnListHelp, menu->selectorWindow, 0,
                                   16);
      Text_DrawMessageAsTilesFar(MsgRearrangeHelp - 3, // "A: Details"
                                 menu->selectorWindow, 0, 8);
      memberIndex = (memberIndex + memberCount) % memberCount;
      Unit_GetFar(menu->memberIds[memberIndex]);
      page = (page + 3) % 3;
      SummaryScreen_ShowMemberStatus(menu->memberIds[memberIndex], page);
      PsynergyMenu_CallIconRoutineWithValue(menu, menu->memberIds[memberIndex]);
      SummaryScreen_SetSelectedMemberRow(menu, memberIndex);
    }
    MenuCursor_Position(memberIndex * 24 - 10, 16);
    WaitFrames(1);
    if (gKeysPressed & KEY_A) {
      Sound_PlaySongFar(SE_MENU_CONFIRM);
      result = 1;
      break;
    }
    if (gKeysPressed & KEY_B) {
      Sound_PlaySongFar(SE_MENU_CANCEL);
      result = -1;
      break;
    }
    // L and R move the member along the party order.
    if (gKeysRepeat & KEY_R) {
      if (MemberChooser_MoveEntry(memberIndex, 1)) {
        Sound_PlaySongFar(SE_MENU_CONFIRM);
        memberIndex++;
        Menu_ReleaseEntryObjects();
        PsynergyMenu_InitializeEntryObjects(menu->memberWindow, 2, 2, 8, 0);
        SummaryScreen_SetSelectedMemberRow(menu, memberIndex);
      } else {
        Sound_PlaySongFar(SE_MENU_ERROR);
      }
      WaitFrames(1);
    } else if (gKeysRepeat & KEY_L) {
      if (MemberChooser_MoveEntry(memberIndex, 0)) {
        Sound_PlaySongFar(SE_MENU_CONFIRM);
        memberIndex--;
        Menu_ReleaseEntryObjects();
        PsynergyMenu_InitializeEntryObjects(menu->memberWindow, 2, 2, 8, 0);
        SummaryScreen_SetSelectedMemberRow(menu, memberIndex);
      } else {
        Sound_PlaySongFar(SE_MENU_ERROR);
      }
      WaitFrames(1);
    } else if ((gKeysPressed & KEY_SELECT) &&
               GameFlag_GetFar(FLAG_DJINN_FIRST)) {
      DjinnMenu_ShowCurrentList();
      redraw = 1;
    } else {
      if (gKeysRepeat & KEY_LEFT) {
        Sound_PlaySongFar(SE_MENU_CURSOR_MOVE);
        if (memberCount > 1) {
          memberIndex--;
          redraw = 1;
        }
      }
      if (gKeysRepeat & KEY_RIGHT) {
        Sound_PlaySongFar(SE_MENU_CURSOR_MOVE);
        if (memberCount > 1) {
          redraw = 1;
          memberIndex++;
        }
      }
    }
  }
  menu->memberIndex[0] = memberIndex;
  menu->selectedMember = menu->memberIds[memberIndex];
  menu->selectedIds[0] = menu->memberIds[memberIndex];
  return result;
}

/// The member chooser (menu page 3): Left and Right choose a party member,
/// showing their status and Psynergy icons; A chooses and B backs out. Returns
/// 1 or -1.
s32 PsynergyMenu_ChooseMember(void) {
  struct PsynergyMenuWork *menu = gMenuWork;
  s32 memberIndex = menu->tabIndex[0];
  s32 memberCount = menu->tabCounts[0];
  s32 redraw = 1;
  s32 page = menu->flags;
  s32 result;
  s32 rowIndex;

  Unit_GetFar(menu->memberTable[memberIndex]);
  WindowSprite_RedrawSavedRectFar(menu->messageWindow);
  Text_DrawMessageAsTilesFar(MsgChooseCharacter, menu->messageWindow, 0, 0);
  Text_DrawMessageAsTilesFar(MsgChooseCharacter + 1, menu->messageWindow, 0,
                             16);
  for (;;) {
    if (redraw) {
      redraw = 0;
      memberIndex = (memberIndex + memberCount) % memberCount;
      Unit_GetFar(menu->memberTable[memberIndex]);
      page = (page + 3) % 3;
      SummaryScreen_ShowMemberStatus(menu->memberTable[memberIndex], page);
      PsynergyMenu_CallIconRoutineWithValue(menu,
                                            menu->memberTable[memberIndex]);
      for (rowIndex = 0; rowIndex < MENU_ROW_COUNT; rowIndex++)
        menu->rowPositions[rowIndex] = 30;
      menu->rowPositions[memberIndex] = 26;
      menu->psynergyCount = PsynergyMenu_CollectActions(
          Unit_GetFar(menu->memberTable[memberIndex]), (u16 *)menu->psynergies,
          PSYNERGY_LIST_ALL);
      PsynergyMenu_DrawPsynergyIcons(menu->psynergies);
      Menu_PlaceEntryObjectsInGrid(96, 96, 8);
      Menu_HideEmptyEntryIcons(menu->psynergies);
    }
    MenuCursor_Position(memberIndex * 24 - 10, 16);
    WaitFrames(1);
    if (gKeysPressed & KEY_A) {
      Sound_PlaySongFar(SE_MENU_CONFIRM);
      result = 1;
      break;
    }
    if (gKeysPressed & KEY_B) {
      Sound_PlaySongFar(SE_MENU_CANCEL);
      result = -1;
      break;
    }
    {
      if (gKeysRepeat & KEY_LEFT) {
        Sound_PlaySongFar(SE_MENU_CURSOR_MOVE);
        if (memberCount > 1) {
          memberIndex--;
          redraw = 1;
        }
      }
      if (gKeysRepeat & KEY_RIGHT) {
        Sound_PlaySongFar(SE_MENU_CURSOR_MOVE);
        if (memberCount > 1) {
          memberIndex++;
          redraw = 1;
        }
      }
    }
  }
  menu->tabIndex[0] = memberIndex;
  menu->selectedMember = menu->memberTable[memberIndex];
  menu->memberIds[0] = menu->memberTable[memberIndex];
  return result;
}

/// Moves the party member at selectedIndex one place later (direction 1) or
/// earlier (0) in the party order. Returns 1, or 0 when it cannot move.
s32 MemberChooser_MoveEntry(s32 selectedIndex, s32 direction) {
  struct SummaryScreenWork *menu = gMenuWork;
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
    reordered[index] = menu->memberIds[index];
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
    Party_RemoveMemberFar(menu->memberIds[index]);
  }
  for (index = 0; index < menu->partyCount; index++) {
    Party_AddMemberFar(reordered[index]);
  }
  menu->partyCount = Party_ListMembersFar(menu->memberIds);
  return 1;
}

/// Sets up the Status menu's first screen: the cursor window with the party's
/// figures, no other windows yet, and an 8 by 2 cursor grid. The parameters are
/// not used.
void Menu_InitializeSelectorCursorAndEntries(s32 unused0, s32 unused1,
                                             s32 unused2, s32 unused3) {
  struct SummaryScreenWork *menu;

  menu = gMenuWork;
  PsynergyMenu_InitializeEntryObjects(MenuCursor_Create(menu), 2, 2, 8, 0);
  menu->unopenedWindow = 0;
  menu->statusWindow = 0;
  menu->helpWindow = 0;
  menu->actionWindow = 0;
  menu->selectedColumn = 0;
  menu->selectedRow = 0;
  menu->columnCount = 8;
  menu->rowCount = 2;
}

/// Shows `member`'s status in the status window (0, 5, 30 by 15),
/// opening it first with the member's portrait (kept as the work's
/// statusCursor), the 32 entry icons (the chooser's Psynergy icons) on the
/// chooser page, and the eight category sprites that show the status icons;
/// then draws the stats and statuses. frame is not used.
void SummaryScreen_ShowMemberStatus(s32 member, s32 frame) {
  s32 created;
  struct Window *window;
  struct WindowSprite *cursor;
  struct SummaryScreenWork *menu;

  menu = gMenuWork;
  created = 0;
  window = menu->statusWindow;
  if (window == 0) {
    created = Window_UpdateOrCreate(&menu->statusWindow, 0, 5, 30, 15, 2);
    window = menu->statusWindow;
  }
  if (created != 0) {
    cursor = Portrait_CreateFar(member, 0, 0, window, 0, 0);
    menu->statusCursor = cursor;
    // The portrait goes to depth slot 240, as the category sprites do.
    if ((cursor->sentinel = 0xF0, menu->page) == SUMMARY_SCREEN_PAGE_CHOOSER) {
      Menu_SpawnIconEntries((struct ItemMenuWork *)menu, window);
    }
    Menu_CreateEightEntryObjects(window);
    SummaryScreen_DrawStatusAilments(window, member, 0x100);
    return;
  }
  SummaryScreen_DrawStatusAilments(window, member, 0);
}

/// The member's status page. The left side lists their statuses (or the stat
/// help when they have none) and the right side the status entries (name and
/// level, experience, HP, PP, Djinn when the party has any, elemental levels,
/// Power and Resist); the cursor's entry is highlighted and explained in the
/// help window. Up and Down move in a side, Left and Right switch sides, L and
/// R switch to the previous or next member; A goes on to the Psynergy page and
/// B backs out. Returns 1 or -1.
s32 SummaryScreen_SelectCommand(void) {
  struct SummaryScreenWork *menu;
  s32 side;
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
  side = 0;
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
  Window_UpdateOrCreate(&menu->helpWindow, 0, 0, 30, 5, 2);
  Task_Remove(Menu_UpdateEntryObjectTransforms);
  for (i = 3; i >= 0; i--)
    menu->rowY[i] = 104;
  done = 0;
  MenuCursor_Slide(-10, 88);

  // One member's page at a time; L and R start the next.
  while (done == 0 && GameFlag_GetFar(FLAG_MENU_START_PRESSED) == 0) {
    Unit_GetFar(menu->selectedIds[0]);
    SummaryScreen_DrawStatusAilments(menu->statusWindow, menu->selectedIds[0],
                                     1);
    count = SummaryScreen_BuildAvailability(entries, 1, menu->selectedIds[0]);
    hasAilments = 0;
    if (count == 0)
      count = 1;
    else
      hasAilments = 1;
    redraw = 1;

    while (GameFlag_GetFar(FLAG_MENU_START_PRESSED) == 0) {
      if (redraw != 0) {
        s32 limit;

        redraw = 0;
        limit = count;
        side = (side + 2) % 2;
        if (side == 0) {
          selected = (selected + limit) % limit;
          WindowSprite_RedrawSavedRectFar(menu->helpWindow);
          if (hasAilments == 0) {
            Text_DrawMessageAsTilesFar(MsgSwitchCharacterHelp,
                                       menu->statusWindow, SWITCH_HELP_X, -24);
            Text_DrawMessageAsTilesFar(MsgSwitchCharacterHelp + 1,
                                       menu->statusWindow, 0, -24);
          }
        } else {
          // The right side has eight entries, seven without the Djinn row.
          WindowSprite_RedrawSavedRectFar(menu->helpWindow);
          if (hasDjinn != 0)
            selected = (selected + 8) % 8;
          else
            selected = (selected + 7) % 7;
        }
        SummaryScreen_DrawSelectionCursor(side, selected, entries, 0);
        WindowSprite_ClearListFar(menu->helpWindow);
        WaitFrames(1);
        if (side == 0)
          SummaryScreen_DrawSelectionLabels(menu->helpWindow, selected,
                                            entries);
        else
          SummaryScreen_ShowMemberProgressMessage(menu->helpWindow, selected,
                                                  hasDjinn);
      }
      // The statuses are 16 pixels apart; the right side's first four rows
      // (name to PP) are 8 apart, the rows below them 8 apart from y 112.
      menu->memberCursors[0]->motion = ICON_SHOWN;
      if (side == 0)
        MenuCursor_Position(-10, selected * 16 + 88);
      else if (selected <= 3)
        MenuCursor_Position(24, selected * 8 + 48);
      else
        MenuCursor_Position(48, selected * 8 + 80);
      WaitFrames(1);
      // Take the highlight off before the cursor moves.
      if (gKeysRepeat & KEYS_DPAD)
        SummaryScreen_DrawSelectionCursor(side, selected, entries, 1);
      if (gKeysPressed & KEY_A) {
        Sound_PlaySongFar(SE_MENU_CONFIRM);
        done = 1;
        result = 1;
        break;
      }
      if (gKeysPressed & KEY_B) {
        Sound_PlaySongFar(SE_MENU_CANCEL);
        done = 1;
        result = -1;
        break;
      }
      if (gKeysRepeat & KEY_UP) {
        Sound_PlaySongFar(SE_MENU_CURSOR_MOVE);
        redraw = 1;
        selected--;
      }
      if (gKeysRepeat & KEY_DOWN) {
        Sound_PlaySongFar(SE_MENU_CURSOR_MOVE);
        redraw = 1;
        selected++;
      }
      if (gKeysRepeat & KEY_RIGHT) {
        Sound_PlaySongFar(SE_MENU_CURSOR_MOVE);
        redraw = 1;
        side++;
      }
      if (gKeysRepeat & KEY_LEFT) {
        Sound_PlaySongFar(SE_MENU_CURSOR_MOVE);
        redraw = 1;
        side--;
      }
      // L and R: the previous or next member.
      if ((gKeysRepeat & KEY_R) || (gKeysRepeat & KEY_L)) {
        Sound_PlaySongFar(SE_MENU_CURSOR_MOVE);
        member = menu->memberIndex[0];
        if (gKeysRepeat & KEY_R)
          member++;
        else
          member--;
        member = (member + menu->partyCount) % menu->partyCount;
        menu->selectedMember = menu->memberIds[member];
        menu->selectedIds[0] = menu->memberIds[member];
        menu->memberIndex[0] = member;
        PsynergyMenu_CallIconRoutineWithValue(menu, menu->memberIds[member]);
        break;
      }
    }
  }
  WindowSprite_ClearListFar(menu->helpWindow);
  WindowSprite_RedrawSavedRectFar(menu->helpWindow);
  Window_ClearInteriorTilesFar(menu->statusWindow, 64, 56, 224, 96);
  Task_Add(Menu_UpdateEntryObjectTransforms, TASK_PRIORITY_MAIN_LOOP);
  for (i = 3; i >= 0; i--)
    menu->rowY[i] = 128;
  ItemMenu_ResetCategory();
  return result;
}

/// Highlights (invert 0) or unhighlights (invert 1) the selected entry of the
/// status page: in side 0 the selected status row, as wide as its label
/// (gSummaryScreen_CursorWidths); in side 1 one of the first four status rows
/// or of the rows below.
void SummaryScreen_DrawSelectionCursor(s32 mode, s32 selected, u8 *entries,
                                       s32 invert) {
  struct SummaryScreenWork *menu = gMenuWork;
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
          width = gSummaryScreen_CursorWidths[index];
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

/// Writes the advice for the selected status (two lines, for example how to
/// revive a member who is down) into window target, or that the member is well
/// when they have no status.
void SummaryScreen_DrawSelectionLabels(struct Window *target, s32 selected,
                                       const u8 *entries) {
  s32 labelCount = 0;
  s32 labelIndex = 0;

  do {
    if (entries[labelIndex] != 0) {
      if (selected == labelCount) {
        s32 messageId = MsgStatusAdvice + labelIndex * 2;
        Text_DrawMessageFar(messageId, target, 0, -1);
        Text_DrawMessageFar(messageId + 1, target, 0, 15);
      }
      labelCount++;
    }
    labelIndex++;
  } while (labelIndex <= 4);

  if (labelCount == 0)
    Text_DrawMessageFar(MsgStatusNormal, target, 0, 0);
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
void SummaryScreen_ShowMemberProgressMessage(struct Window *destination,
                                             s32 messageVariant,
                                             s32 preserveVariant) {
  struct SummaryScreenWork *menu = gMenuWork;
  void *buffer;

  // Entry 4 is the Djinn row's help: without Djinn the rows below it come
  // one entry earlier.
  if (preserveVariant == 0 && messageVariant > 3) {
    messageVariant++;
  }
  if (messageVariant == 1) {
    struct Unit *member = Unit_GetFar(menu->selectedIds[0]);

    if (member->level == 99) {
      messageVariant = 8; // "These are your experience points."
    } else {
      ExperienceDifference remaining =
          Unit_GetExperienceForLevelFar(menu->selectedIds[0],
                                        member->level + 1) -
          member->experience;
      MessageArgument_PushFar(remaining, MESSAGE_ARGUMENT_NUMBER);
    }
  }

  buffer = Alloc(STATUS_HELP_TEXT_MAX * 2);
  Message_CopyExpandedFar(messageVariant + MsgProgressHelp, buffer,
                          STATUS_HELP_TEXT_MAX);
  Text_DrawWideStringAtOffsetFar(buffer, destination, 0, -1);
  Free(buffer);
}

/// Draws member memberId's status page into window: the top rows
/// (ItemMenu_DrawMemberStatus), the status list or "Normal", and except on the
/// chooser page the Djinn counts by element and, in mode 1, the elemental
/// levels, Power and Resist. mode's 0x100 keeps what is drawn instead of
/// clearing it.
void SummaryScreen_DrawStatusAilments(struct Window *window, s32 memberId,
                                      s32 mode) {
  struct SummaryScreenWork *menu = gMenuWork;
  struct Unit *unit;
  s32 djinnRow;
  s32 hasDjinn;
  s32 keepDrawn;
  s32 ailmentIndex;
  u8 ailments[8];

  if (Party_SumDjinnCountsFar(-1) != 0)
    hasDjinn = 1;
  else
    hasDjinn = 0;
  unit = Unit_GetFar(memberId);
  // The Djinn row, in tiles: higher on the full page (mode 1), which has the
  // elemental rows below it.
  if ((mode & 0xff) == 1)
    djinnRow = 7;
  else
    djinnRow = 10;
  menu->statusCursor->motion = ICON_SHOWN;
  ItemMenu_DrawMemberStatus(window, memberId, mode);
  SummaryScreen_BuildAvailability(ailments, 1, memberId);
  SummaryScreen_UpdateSelectionIcons(ailments);
  keepDrawn = mode & 0x100;
  if (keepDrawn == 0)
    Window_ClearInteriorTilesFar(window, 0, 40, 96, 96);
  // The statuses, one a row, or "Normal".
  ailmentIndex = 0;
  if (ailments[CHARACTER_DOWN] != 0) {
    Text_DrawMessageAsTilesFar(MsgDownLabel, window, 16, 40);
    ailmentIndex = 1;
  }
  if (ailments[CHARACTER_POISON] != 0) {
    Text_DrawMessageAsTilesFar(MsgPoisonLabel, window, 16,
                               ailmentIndex * 16 + 40);
    ailmentIndex++;
  }
  if (ailments[CHARACTER_VENOM] != 0) {
    Text_DrawMessageAsTilesFar(MsgVenomLabel, window, 16,
                               ailmentIndex * 16 + 40);
    ailmentIndex++;
  }
  if (ailments[CHARACTER_CURSE] != 0) {
    Text_DrawMessageAsTilesFar(MsgCurseLabel, window, 16,
                               ailmentIndex * 16 + 40);
    ailmentIndex++;
  }
  if (ailments[CHARACTER_HAUNT] != 0) {
    Text_DrawMessageAsTilesFar(MsgHauntLabel, window, 16,
                               ailmentIndex * 16 + 40);
    ailmentIndex++;
  }
  if (ailmentIndex == 0)
    Text_DrawMessageAsTilesFar(MsgNormalLabel, window, 0, 40);
  SummaryScreen_UpdateSelectionIcons(ailments);
  ItemMenu_ApplyFlags(ailments);
  if (menu->page == 3)
    return;
  if (keepDrawn == 0) {
    WaitFrames(1);
    Window_ClearInteriorTilesFar(window, 64, 56, 224, 96);
  }
  // The four element icons, then the Djinn counts and, in mode 1, the
  // elemental levels, Power and Resist.
  Text_SetColorFar(TEXT_COLOR_NORMAL);
  if (mode == 1 || hasDjinn == 1) {
    Window_SetTilemapEntryFar(window, 1, 15, djinnRow, TILEMAP_ENTRY_ICON);
    Window_SetTilemapEntryFar(window, 2, 19, djinnRow, TILEMAP_ENTRY_ICON);
    Window_SetTilemapEntryFar(window, 3, 23, djinnRow, TILEMAP_ENTRY_ICON);
    Window_SetTilemapEntryFar(window, 4, 27, djinnRow, TILEMAP_ENTRY_ICON);
  }
  if (hasDjinn)
    Text_DrawMessageAsTilesFar(MsgDjinnLabel, window, 64, djinnRow * 8 + 8);
  if (mode == 1) {
    if (!hasDjinn)
      djinnRow--;
    MENU_DRAW_STATUS_LABEL(gMenu_LvString, window, 64, djinnRow * 8 + 16);
    Text_DrawMessageAsTilesFar(MsgPowerLabel, window, 64, djinnRow * 8 + 24);
    Text_DrawMessageAsTilesFar(MsgPowerLabel + 1, window, 64,
                               djinnRow * 8 + 32);
  }
  for (ailmentIndex = 0; ailmentIndex <= 3; ailmentIndex++) {
    if (hasDjinn)
      Text_DrawNumberInWindowFar(unit->djinnHeldCounts[ailmentIndex], 1, window,
                                 ailmentIndex * 32 + 120, djinnRow * 8 + 8);
    if ((mode & 0xff) == 1) {
      if (hasDjinn) {
        Text_DrawNumberInWindowFar(unit->djinnSetCounts[ailmentIndex], 1,
                                   window, ailmentIndex * 32 + 104,
                                   djinnRow * 8 + 8);
        Text_DrawStringInWindowFar(gMenu_SlashString, window,
                                   ailmentIndex * 32 + 112, djinnRow * 8 + 8);
      }
      Text_DrawNumberInWindowFar(
          Unit_GetElementLevelFar(memberId, ailmentIndex), 2, window,
          ailmentIndex * 32 + 112, djinnRow * 8 + 16);
      Text_DrawNumberInWindowFar(unit->element[ailmentIndex].power, 3, window,
                                 ailmentIndex * 32 + 104, djinnRow * 8 + 24);
      Text_DrawNumberInWindowFar(unit->element[ailmentIndex].resistance, 3,
                                 window, ailmentIndex * 32 + 104,
                                 djinnRow * 8 + 32);
    }
  }
}

#if TBS_PATCH_1
/// Spins for 256 loop passes (international editions only). Nothing calls it.
void Menu_BusyWait(void) {
  s32 count = 0xff;

  // Written with goto (MATCHING.md): as a do-while loop, the loop optimizer
  // deletes the empty loop and leaves 1 of the 7 instructions.
spin:
  count--;
  if (count >= 0)
    goto spin;
}

#endif

/// Draws the top rows of member memberId's status page into window: name,
/// class, level, experience, HP and PP on the left, Attack, Defense, Agility
/// and Luck on the right. flags' 0x100 keeps what is drawn instead of clearing
/// it.
void ItemMenu_DrawMemberStatus(struct Window *window, s32 memberId, s32 flags) {
  struct ItemMenuWork *menu;
  struct Unit *unit;

  menu = gMenuWork;
  unit = Unit_GetFar(memberId);
  menu->cursor->motion = ICON_SHOWN;
  flags &= 0x100;
  if (flags == 0) {
    Window_ClearInteriorTilesFar(window, 0, 0, 128, 40);
  }
  Text_DrawStringAtOffsetFar(unit->name, window, 40, 0);
  Text_DrawMessageAsTilesFar(unit->classId + MsgClassName, window, 0, 32);
  MENU_DRAW_STATUS_LABEL(gMenu_LvString, window, 104, 0);
  Text_SetColorFar(TEXT_COLOR_NORMAL);
  Text_DrawNumberInWindowFar(unit->level, 2, window, 128, 0);
  MENU_DRAW_STATUS_LABEL(gMenu_HpString, window, 40, 16);
  Text_DrawNumberInWindowFar(unit->hp, 4, window, 72, 16);
  Text_DrawNumberInWindowFar(unit->hpMax, 4, window, 112, 16);
  Text_DrawStringInWindowFar(gMenu_SlashString, window, 104, 16);
  MENU_DRAW_STATUS_LABEL(gMenu_PpString, window, 40, 24);
  Text_DrawNumberInWindowFar(unit->pp, 4, window, 72, 24);
  Text_DrawNumberInWindowFar(unit->ppMax, 4, window, 112, 24);
  Text_DrawStringInWindowFar(gMenu_SlashString, window, 104, 24);
  Text_DrawMessageAsTilesFar(MsgExpLabel, window, 40, 8);
  Text_DrawNumberInWindowFar(unit->experience, 7, window, 88, 8);
  if (flags == 0) {
    WaitFrames(1);
    Window_ClearInteriorTilesFar(window, 144, 0, 224, 40);
  }
  // The labels "Attack", "Defense", "Agility" and "Luck", 23 to 20 messages
  // before "Exp".
  Text_DrawMessageAsTilesFar(MsgExpLabel - 23, window, 152, 0);
  Text_DrawMessageAsTilesFar(MsgExpLabel - 22, window, 152, 8);
  Text_DrawMessageAsTilesFar(MsgExpLabel - 21, window, 152, 16);
  Text_DrawMessageAsTilesFar(MsgExpLabel - 20, window, 152, 24);
  Text_DrawNumberInWindowFar(unit->atk, STATUS_STAT_DIGITS, window,
                             STATUS_STAT_X, 0);
  Text_DrawNumberInWindowFar(unit->def, STATUS_STAT_DIGITS, window,
                             STATUS_STAT_X, 8);
  Text_DrawNumberInWindowFar(unit->agi, STATUS_STAT_DIGITS, window,
                             STATUS_STAT_X, 16);
  Text_DrawNumberInWindowFar(unit->luck, STATUS_STAT_DIGITS, window,
                             STATUS_STAT_X, 24);
}

/// Fills ailments[0..4] (UnitStatus) with member memberId's statuses: down
/// (only when requested is 1), poison or venom, curse and haunt. Returns how
/// many they have.
s32 SummaryScreen_BuildAvailability(u8 *ailments, s32 requested, s32 memberId) {
  struct Unit *unit;
  s32 ailmentIndex;
  s32 count;
  s32 poison;

  unit = Unit_GetFar(memberId);
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
  if (unit->curse != 0) {
    ailments[CHARACTER_CURSE] = 1;
    count++;
  }
  if (unit->haunt != 0) {
    ailments[CHARACTER_HAUNT] = 1;
    count++;
  }
  return count;
}

/// Fills result with the item list of the member in side index for the
/// equipment pages: their unit, the menu's item count, and the page and row of
/// the item chosen last for them, STATUS_EQUIP_PAGE_SIZE items to a page.
/// Returns 1.
s32 PagedList_Build(struct PagedList *result, s32 index) {
  s32 unitBits;
  struct ItemMenuWork *menu = gMenuWork;
  s32 itemCount;
  s32 row;
  s32 page;
  s32 pageCount;
  s32 selected;

  unitBits = (s32)Unit_GetFar(menu->sideMember[index]);
  itemCount = menu->itemCount;
  selected = menu->selectedIndexByMember[menu->sideMember[index]];
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
  result->memberState = unitBits;
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
/// six tile pairs (24 8x8 tiles), each a fill of WINDOW_BACKGROUND_COLOR with
/// the rows of gMenu_PatternRowMasks XORed in, the first set growing row by row
/// and the second skipping its top row. Pair n is range icon n, at
/// RANGE_ICON_TILE + 2n (tile 0x200, or 0x280 in the international editions,
/// whose tiles start 0x1000 bytes further on), which PsynergyMenu_DrawRange
/// draws. Called as the Status menu's Psynergy page opens
/// (PsynergyMenu_SelectAction).
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

      Iwram_FillWords(tile, 64, TEXT_CANVAS_BACKGROUND_WORD);
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
            XorWord(tile[rowIndex], gMenu_PatternRowMasks[mask].firstTile);
        tile[rowIndex + 8] =
            XorWord(tile[rowIndex + 8], gMenu_PatternRowMasks[mask].secondTile);
      }
    }
  }
}

/// Draws a Psynergy range icon at tile (x, y) of window: three tiles from
/// pair range of the range icon tiles (Menu_BuildPatternTiles), the side tile
/// on both ends, mirrored on the left, and the centre tile between. range is
/// the ability's range less one (0 for the aimed unit alone), or 11 for
/// ABILITY_RANGE_ALL (PsynergyMenu_DrawListPage).
void PsynergyMenu_DrawRange(struct Window *window, s32 x, s32 y, s32 range,
                            s32 unused) {
  s32 iconIndex;
  s32 tile;

  (void)unused;

  iconIndex = range * 2;
  tile = iconIndex + RANGE_ICON_TILE + 1;
  // 0x400 is the map entry's horizontal flip bit.
  Window_SetTilemapEntryFar(window, 0x400 | tile, x, y, 0); // flipped
  Window_SetTilemapEntryFar(window, iconIndex + RANGE_ICON_TILE, x + 1, y, 0);
  Window_SetTilemapEntryFar(window, tile, x + 2, y, 0);
}

/// Updates the Psynergy page for the row under the cursor: the Psynergy's
/// description in the info window, where it can be used (in the field, in
/// battle, or anywhere), and the row highlights with the range icons.
s32 PsynergyMenu_DrawRangePage(struct Window *window, s32 unused,
                               struct PagedList *state) {
  struct PsynergyMenuWork *menu;
  struct AbilityDefinition *ability;
  s32 row;
  s32 base;

  menu = gMenuWork;
  state->selectedIndex = state->page * PAGE_ROWS + state->row;
#if TBS_PATCH_1 && !TBS_PATCH_4
  WindowSprite_RedrawSavedRectFar(menu->infoWindow);
#elif TBS_PATCH_4
  Window_ClearInteriorTilesFar(menu->infoWindow, 0, 0, 224, 8);
#else
  WindowSprite_ClearListFar(menu->infoWindow);
#endif
  WaitFrames(1);
  if (menu->psynergies[state->selectedIndex] != 0) {
    MENU_DRAW_DESCRIPTION(
        (menu->psynergies[state->selectedIndex] & PSYNERGY_ABILITY_MASK) +
            MsgAbilityDescription,
        menu->infoWindow, 0, 0);
    ability = Ability_Get(menu->psynergies[state->selectedIndex] &
                          PSYNERGY_ABILITY_MASK);
#if LANGUAGE != LANGUAGE_JAPANESE
    Window_ClearInteriorTilesFar(window, 0, 96, 224, 104);
    row = 0;
    if (ability->fieldPsynergy != 0 ||
        (ability->flags & ABILITY_USABLE_IN_FIELD) != 0) {
      row = 2;
    }
    if ((ability->flags & ABILITY_USABLE_IN_BATTLE) != 0) {
      row |= 1;
    }
    if (row == 3) {
      Text_DrawMessageAsTilesFar(MsgUsableAnywhere, window, 0, 96);
    } else if (row == 2) {
      Text_DrawMessageAsTilesFar(MsgUsableInField, window, 0, 96);
    } else if (row == 1) {
      Text_DrawMessageAsTilesFar(MsgUsableInBattle, window, 0, 96);
    }
#else
    Window_ClearInteriorTilesFar(window, 0, 72, 64, 96);
    row = 0;
    if (ability->fieldPsynergy != 0 ||
        (ability->flags & ABILITY_USABLE_IN_FIELD) != 0) {
      Text_DrawMessageAsTilesFar(MsgUsableInField, window, 0, 72);
      row = 1;
    }
    if ((ability->flags & ABILITY_USABLE_IN_BATTLE) != 0) {
      Text_DrawMessageAsTilesFar(MsgUsableInBattle, window, 0, row * 8 + 72);
      row++;
    }
    Text_DrawMessageAsTilesFar(MsgCanBeUsed, window, 0, row * 8 + 72);
#endif
  }

  // Highlight the cursor's row. A Psynergy with an element shows its icon at
  // tile column 24, in palette 0 on the highlighted row and palette 1
  // (TILEMAP_ENTRY_ICON) elsewhere, and the highlight leaves that column out.
  base = state->page * PAGE_ROWS;
  for (row = 0; row <= PAGE_ROWS - 1; row++) {
    if (row == state->row) {
      ability =
          Ability_Get(menu->psynergies[base + row] & PSYNERGY_ABILITY_MASK);
      if (ability->element != ELEMENT_NONE) {
        Window_SetTilemapEntryFar(window, ability->element + 1, 24,
                                  row * 2 + LIST_FIRST_ROW, 0);
        Render_SetTilemapFlagRect(window, 9, row * 2 + LIST_FIRST_ROW, 15, 1,
                                  HIGHLIGHT_PALETTE);
        Render_SetTilemapFlagRect(window, 25, row * 2 + LIST_FIRST_ROW, 3, 1,
                                  HIGHLIGHT_PALETTE);
      } else {
        Render_SetTilemapFlagRect(window, 9, row * 2 + LIST_FIRST_ROW, 19, 1,
                                  HIGHLIGHT_PALETTE);
      }
    } else {
      ability =
          Ability_Get(menu->psynergies[base + row] & PSYNERGY_ABILITY_MASK);
      if (ability->element != ELEMENT_NONE) {
        Window_SetTilemapEntryFar(window, ability->element + 1, 24,
                                  row * 2 + LIST_FIRST_ROW, TILEMAP_ENTRY_ICON);
        Render_SetTilemapFlagRect(window, 9, row * 2 + LIST_FIRST_ROW, 15, 1,
                                  WINDOW_PALETTE);
        Render_SetTilemapFlagRect(window, 25, row * 2 + LIST_FIRST_ROW, 3, 1,
                                  WINDOW_PALETTE);
      } else {
        Render_SetTilemapFlagRect(window, 9, row * 2 + LIST_FIRST_ROW, 19, 1,
                                  WINDOW_PALETTE);
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
s32 PsynergyMenu_DrawListPage(struct Window *window, s32 unused,
                              const struct PagedList *res) {
  struct PsynergyMenuWork *menu = gMenuWork;
  struct Unit *member;
  u32 firstEntry;
  u32 rows;
  u8 row;
  s32 entryIndex;

  (void)unused;

  member = Unit_GetFar(menu->memberIds[0]);

  WindowSprite_RedrawSavedRectFar(window);

  firstEntry = res->page * PAGE_ROWS;
  rows = (u8)(res->entryCount - firstEntry);
  if (rows > PAGE_ROWS) {
    rows = PAGE_ROWS;
  }

  Menu_SetPageIcons(PAGE_ROWS, firstEntry, window, LIST_ICONS_X, LIST_ICONS_Y);
  PagedList_DrawPageNumbers(window, res->entryCount, PAGE_ROWS, res->page, 28);

  Text_DrawMessageAsTilesFar(MsgPsynergyPp, window, LIST_PP_X, 0);

  row = 0;
  if (rows > row) {
    entryIndex = firstEntry;
    do {
      struct AbilityDefinition *action;
      s32 messageId;
      s32 y;
      s32 range;
      action =
          Ability_Get(PSYNERGY_ABILITY_MASK & menu->psynergies[entryIndex]);
      messageId = (menu->psynergies[entryIndex] & PSYNERGY_ABILITY_MASK) +
                  MsgAbilityName;
      y = row * 16 + LIST_FIRST_ROW * 8;

      Text_DrawMessageAsTilesFar(messageId, window, LIST_NAME_X, y);
      Text_DrawNumberAtOffsetFar(action->ppCost, 2, window, LIST_PP_NUMBER_X,
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
    Text_DrawMessageAsTilesFar(MsgNoPsynergy, window, 96, LIST_EMPTY_Y);
  }

  Text_DrawStringAtOffsetFar(member->name, (void *)window, 40, 0);
  Text_DrawMessageAsTilesFar(member->classId + MsgClassName, window, 0, 32);
#if LANGUAGE == LANGUAGE_SPANISH || LANGUAGE == LANGUAGE_FRENCH
  Text_DrawStringAtOffsetFar(gMenu_LvString, (void *)window, 0, 48);
#else
  Text_DrawStringInWindowFar(gMenu_LvString, window, 0, 48);
#endif
  Text_DrawNumberInWindowFar(member->level, 2, window, 24, 48);

  return 1;
}

/// The Status menu's Psynergy page: the member's Psynergy, PAGE_ROWS to a page,
/// each with its PP cost and range icon; the cursor's Psynergy is described in
/// the info window with where it can be used. Up and Down move the cursor,
/// Left and Right turn pages, L and R switch member. Returns 1 for A (on to
/// the equipment page) or -1 for B (back to the member row).
s32 PsynergyMenu_SelectAction(void) {

  struct PsynergyMenuWork *menu;
  s32 result;
  s32 done;
  s32 redraw;
  s32 listNeedsRedraw;
  s32 navigationResult;
  s32 memberIndex;
  s32 iconIndex;
  struct PagedList state;

  menu = gMenuWork;
  result = 0;
  done = 0;
  Menu_BuildPatternTiles();
  WindowSprite_RedrawSavedRectFar(menu->messageWindow);
  Window_UpdateOrCreate(&menu->infoWindow, 0, 0, 30, 5, 2);
  // slotY is SummaryScreenWork's rowY: the four Djinn figures go above the
  // screen (y -16) while this page is up, until the member row or the status
  // page puts them back.
  for (iconIndex = 3; iconIndex >= 0; iconIndex--)
    menu->slotY[iconIndex] = -16;
  {
    s32 priority = 245;
    struct WindowSprite **icons = menu->entryIcons;

    for (iconIndex = 31; iconIndex >= 0; iconIndex--) {
      struct WindowSprite *icon = *icons++;

      if (icon != 0)
        icon->sentinel = priority;
    }
  }
  Task_Remove(Menu_UpdateEntryObjectTransforms);
  Menu_SpawnIconEntries((struct ItemMenuWork *)menu, menu->messageWindow);
  Text_DrawMessageAsTilesFar(MsgSwitchCharacterHelp, menu->statusWindow,
                             SWITCH_HELP_X, -24);
  Text_DrawMessageAsTilesFar(MsgSwitchCharacterHelp + 2, menu->statusWindow, 0,
                             -24);

  while (done == 0 && GameFlag_GetFar(FLAG_MENU_START_PRESSED) == 0) {
    ItemMenu_PositionCategory();
    WindowSprite_RedrawSavedRectFar(menu->statusWindow);
    menu->psynergyCount = PsynergyMenu_CollectActions(
        Unit_GetFar(menu->memberIds[0]), menu->psynergies, PSYNERGY_LIST_ALL);
    WaitFrames(1);
    PagedList_Build(&state, 0);
    PsynergyMenu_DrawPreparedPsynergyIcons(menu->statusWindow,
                                           menu->memberIds[0]);
    redraw = 1;
    listNeedsRedraw = 1;

    while (GameFlag_GetFar(FLAG_MENU_START_PRESSED) == 0) {
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
      navigationResult = PagedList_HandleInput(0, state.entryCount, PAGE_ROWS,
                                               &state.row, &state.page);
      menu->sideIcon[0]->motion = ICON_SHOWN;
      MenuCursor_Position(STATUS_PSYNERGY_CURSOR_X,
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
        Sound_PlaySongFar(SE_MENU_CONFIRM);
        result = 1;
        done = 1;
        break;
      }
      if (gKeysPressed & KEY_B) {
        Sound_PlaySongFar(SE_MENU_CANCEL);
        result = -1;
        done = 1;
        Task_Add(Menu_UpdateEntryObjectTransforms, TASK_PRIORITY_MAIN_LOOP);
        break;
      }
      if ((gKeysRepeat & KEY_R) || (gKeysRepeat & KEY_L)) {
        Sound_PlaySongFar(SE_MENU_CURSOR_MOVE);
        memberIndex = menu->tabIndex[0];
        menu->selectedIndexByMember[menu->memberTable[memberIndex]] =
            state.selectedIndex;
        if (gKeysRepeat & KEY_R)
          memberIndex++;
        else
          memberIndex--;
        memberIndex = (memberIndex + menu->memberCount) % menu->memberCount;
        menu->selectedMember = menu->memberTable[memberIndex];
        menu->memberIds[0] = menu->memberTable[memberIndex];
        menu->tabIndex[0] = memberIndex;
        PsynergyMenu_CallIconRoutineWithValue(menu,
                                              menu->memberTable[memberIndex]);
        break;
      }
    }
  }
  WindowSprite_ClearListFar(menu->infoWindow);
  WindowSprite_RedrawSavedRectFar(menu->infoWindow);
  ItemMenu_HideAllIcons();
  WindowSprite_ClearListFar(menu->messageWindow);
  WindowSprite_RedrawSavedRectFar(menu->statusWindow);
  return result;
}

/// Nothing calls it.
s32 PsynergyMenu_ActionReturnTrue(void) { return 1; }

/// Hides every entry icon, then loads the icons of the Psynergy already listed
/// in the work (psynergies) into them. memberId is only looked up.
void PsynergyMenu_DrawPreparedPsynergyIcons(const void *unused, s32 memberId) {
  struct PsynergyMenuWork *menu = gMenuWork;

  Unit_GetFar(memberId);
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
s32 ItemMenu_DrawEquipPage(struct Window *window, s32 unused,
                           struct PagedList *state) {
  struct ItemMenuWork *menu;
  struct ItemDefinition *item;
  s32 firstEntry;
  s32 rowIndex;

  menu = gMenuWork;
  firstEntry = state->page * PAGE_ROWS;
  state->selectedIndex = firstEntry + state->row;
#if TBS_PATCH_1 && !TBS_PATCH_4
  WindowSprite_RedrawSavedRectFar(menu->descriptionWindow);
#elif TBS_PATCH_4
  Window_ClearInteriorTilesFar(menu->descriptionWindow, 0, 0, 224, 8);
#else
  WindowSprite_ClearListFar(menu->descriptionWindow);
#endif
  WaitFrames(1);
  if (menu->items[state->selectedIndex] != 0) {
    MENU_DRAW_DESCRIPTION(
        (menu->items[state->selectedIndex] & INVENTORY_ITEM_ID_MASK) +
            MsgItemPlainName,
        menu->descriptionWindow, 0, 0);
  }

  // An item with an element shows its icon at tile column 27, in palette 0 on
  // the highlighted row and palette 1 (TILEMAP_ENTRY_ICON) elsewhere, and the
  // highlight stops short of it.
  for (rowIndex = 0; rowIndex < PAGE_ROWS; rowIndex++) {
    if (rowIndex == state->row) {
      item = Item_GetDirectFar(menu->items[state->selectedIndex] &
                               INVENTORY_ITEM_ID_MASK);
      if (item->element != ELEMENT_NONE) {
        Window_SetTilemapEntryFar(window, item->element + 1, 27,
                                  rowIndex * 2 + 1, 0);
        Render_SetTilemapFlagRect(window, 14, rowIndex * 2 + 1, 13, 1,
                                  HIGHLIGHT_PALETTE);
      } else {
        Render_SetTilemapFlagRect(window, 14, rowIndex * 2 + 1, 14, 1,
                                  HIGHLIGHT_PALETTE);
      }
    } else {
      item = Item_GetDirectFar(menu->items[firstEntry + rowIndex] &
                               INVENTORY_ITEM_ID_MASK);
      if (item->element != ELEMENT_NONE) {
        Window_SetTilemapEntryFar(window, item->element + 1, 27,
                                  rowIndex * 2 + 1, TILEMAP_ENTRY_ICON);
        Render_SetTilemapFlagRect(window, 14, rowIndex * 2 + 1, 13, 1,
                                  WINDOW_PALETTE);
      } else {
        Render_SetTilemapFlagRect(window, 14, rowIndex * 2 + 1, 14, 1,
                                  WINDOW_PALETTE);
      }
    }
  }

  for (rowIndex = 0; rowIndex <= 3; rowIndex++) {
    Render_SetTilemapFlagRect(menu->equipWindow, STATUS_EQUIP_SLOT_X,
                              rowIndex * 2 + 1, STATUS_EQUIP_SLOT_WIDTH, 1,
                              WINDOW_PALETTE);
  }

  // The slots' name rows: weapon 1, head 3, shield 5, chest 7, each under
  // its label (ItemMenu_DrawCategory). Other worn items have no row.
  if (menu->items[state->selectedIndex] & INVENTORY_EQUIPPED) {
    item = Item_GetDirectFar(menu->items[state->selectedIndex] &
                             INVENTORY_ITEM_ID_MASK);
    switch (item->type) {
    case ITEM_TYPE_WEAPON:
      Render_SetTilemapFlagRect(menu->equipWindow, STATUS_EQUIP_SLOT_X, 1,
                                STATUS_EQUIP_SLOT_WIDTH, 1, HIGHLIGHT_PALETTE);
      break;
    case ITEM_TYPE_HELM:
      Render_SetTilemapFlagRect(menu->equipWindow, STATUS_EQUIP_SLOT_X, 3,
                                STATUS_EQUIP_SLOT_WIDTH, 1, HIGHLIGHT_PALETTE);
      break;
    case ITEM_TYPE_SHIELD:
      Render_SetTilemapFlagRect(menu->equipWindow, STATUS_EQUIP_SLOT_X, 5,
                                STATUS_EQUIP_SLOT_WIDTH, 1, HIGHLIGHT_PALETTE);
      break;
    case ITEM_TYPE_ARMOR:
      Render_SetTilemapFlagRect(menu->equipWindow, STATUS_EQUIP_SLOT_X, 7,
                                STATUS_EQUIP_SLOT_WIDTH, 1, HIGHLIGHT_PALETTE);
      break;
    }
  }
  WaitFrames(1);
  return 1;
}

/// Draws one page of the Status menu's equipment page into window: up to
/// PAGE_ROWS item names with their icons and the page numbers, or "no items",
/// and the member's name with their Attack and Defense. Returns 1. Called by
/// ItemMenu_SelectItem for each page.
s32 ItemMenu_DrawItemPage(struct Window *window, s32 unused,
                          struct PagedList *state) {
  struct ItemMenuWork *menu = gMenuWork;
  struct Unit *unit;
  u16 *inventoryEntry;
  s32 firstEntry;
  u32 rowCount;
  u32 rowIndex;
  s32 message;

  unit = Unit_GetFar(menu->sideMember[0]);
  Window_ClearInteriorTilesFar(window, 128, 8, 224, 96);
  firstEntry = state->page * PAGE_ROWS;
  rowCount = (u8)(state->entryCount - firstEntry);
  if (rowCount > PAGE_ROWS)
    rowCount = PAGE_ROWS;
  Menu_SetPageIcons(PAGE_ROWS, firstEntry, window, STATUS_ITEM_PAGE_ICONS_X,
                    52);
  PagedList_DrawPageNumbers(window, state->entryCount, PAGE_ROWS, state->page,
                            28);
  if (menu->itemCount == 0) {
    Text_DrawMessageAsTilesFar(MsgItemMenuEmpty, window,
                               STATUS_ITEM_EMPTY_MESSAGE_X, 8);
  } else {
    rowIndex = 0;
    if (rowCount > rowIndex) {
      inventoryEntry = &menu->items[firstEntry];
      do {
        Text_DrawMessageAsTilesFar(
            (*inventoryEntry & INVENTORY_ITEM_ID_MASK) + MsgItemName, window,
            STATUS_ITEM_NAME_X, (rowIndex << 4) + 8);
        rowIndex = (u8)(rowIndex + 1);
        inventoryEntry++;
      } while (rowCount > rowIndex);
    }
  }
  Text_DrawStringAtOffsetFar(unit->name, window, 40, 0);
  message = MsgStatLabel;
  Text_DrawMessageAsTilesFar(message, window, 32, 16);
  Text_DrawMessageAsTilesFar(message + 1, window, 32, 24);
  Text_DrawNumberInWindowFar(unit->atk, 3, window, STATUS_ITEM_STAT_X, 16);
  Text_DrawNumberInWindowFar(unit->def, 3, window, STATUS_ITEM_STAT_X, 24);
  return 1;
}

/// The Status menu's equipment page: the member's items in the first list
/// order (LIST_SORT_EQUIPPED_THEN_ITEMS), PAGE_ROWS to a page, beside
/// the equipment window of what they wear. Up and Down move the cursor, Left
/// and Right turn pages, L and R switch member. Returns 1 for A (back to the
/// status page) or -1 for B (back to the member row). Only the list is
/// sorted; the inventory itself is left as it is.
s32 ItemMenu_SelectItem(void) {

  struct ItemMenuWork *menu;
  struct Unit *member;
  s32 result;
  s32 done;
  s32 redraw;
  s32 listNeedsRedraw;
  s32 navigationResult;
  s32 memberIndex;
  s32 iconIndex;
  u16 savedInventory[15];
  struct PagedList state;

  menu = gMenuWork;
  {
    s32 memberId = menu->sideMember[0];

    result = 0;
    Unit_GetFar(menu->memberIds[memberId]);
  }
  Window_UpdateOrCreate(&menu->equipWindow, 0, 10, STATUS_EQUIP_WINDOW_WIDTH,
                        10, 2);
  ItemMenu_PositionCategory();

  for (iconIndex = 0; iconIndex < 32; iconIndex++) {
    struct WindowSprite *icon = menu->entryIcons[iconIndex];

    if (icon != 0)
      icon->sentinel = 240;
  }

  Task_Remove(Menu_UpdateEntryObjectTransforms);
  Text_DrawMessageAsTilesFar(MsgSwitchCharacterHelp, menu->statusWindow, 64,
                             -24);
  Text_DrawMessageAsTilesFar(MsgSwitchCharacterHelp + 3, menu->statusWindow, 0,
                             -24);
  ItemMenu_PositionCategory();
  WaitFrames(1);
  Menu_SpawnIconEntries((struct ItemMenuWork *)menu, menu->questionWindow);

  done = 0;
  while (done == 0 && GameFlag_GetFar(FLAG_MENU_START_PRESSED) == 0) {
    ItemMenu_PositionCategory();
    WindowSprite_RedrawSavedRectFar(menu->statusWindow);
    member = Unit_GetFar(menu->sideMember[0]);
    for (iconIndex = 0; iconIndex <= 14; iconIndex++)
      savedInventory[iconIndex] = member->inventory[iconIndex];
    menu->itemCount = ItemMenu_CollectItems(member, menu->items, 0);
    ItemMenu_Sort(menu->items, LIST_SORT_EQUIPPED_THEN_ITEMS);
    PagedList_Build(&state, 0);
    WindowSprite_RedrawSavedRectFar(menu->equipWindow);
    ItemMenu_DrawCategory(menu->equipWindow, menu->sideMember[0], 1);
    WaitFrames(1);
    ItemMenu_DrawIcons(menu->items, 0);
    redraw = 1;
    listNeedsRedraw = 1;

    while (GameFlag_GetFar(FLAG_MENU_START_PRESSED) == 0) {
      if (redraw != 0) {
        WindowSprite_ClearListFar(menu->descriptionWindow);
        redraw = 0;
        if (listNeedsRedraw != 0) {
          listNeedsRedraw = 0;
          ItemMenu_DrawItemPage(menu->statusWindow, 0, &state);
        }
        ItemMenu_DrawEquipPage(menu->statusWindow, 0, &state);
      }
      menu->sideIcons[0]->motion = ICON_SHOWN;
      MenuCursor_Position(96, state.row * 16 + 52);
      WaitFrames(1);

      navigationResult = PagedList_HandleInput(0, state.entryCount, PAGE_ROWS,
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
        Sound_PlaySongFar(SE_MENU_CONFIRM);
        result = 1;
        done = 1;
        break;
      }
      if (gKeysPressed & KEY_B) {
        Sound_PlaySongFar(SE_MENU_CANCEL);
        result = -1;
        done = 1;
        break;
      }
      if ((gKeysRepeat & KEY_R) || (gKeysRepeat & KEY_L)) {
        Sound_PlaySongFar(SE_MENU_CURSOR_MOVE);
        memberIndex = menu->sideIndex[0];
        menu->selectedIndexByMember[menu->memberIds[memberIndex]] =
            state.selectedIndex;
        if (gKeysRepeat & KEY_R)
          memberIndex++;
        else
          memberIndex--;
        // Copy back the inventory saved as the list was built; only the list
        // was sorted, so this writes what is already there.
        for (iconIndex = 0; iconIndex <= 14; iconIndex++)
          member->inventory[iconIndex] = savedInventory[iconIndex];
        memberIndex = (memberIndex + menu->partyCount) % menu->partyCount;
        menu->selectedMember = menu->memberIds[memberIndex];
        menu->sideMember[0] = menu->memberIds[memberIndex];
        menu->sideIndex[0] = memberIndex;
        PsynergyMenu_CallIconRoutineWithValue(menu,
                                              menu->memberIds[memberIndex]);
        break;
      }
    }
  }

  WindowSprite_ClearListFar(menu->descriptionWindow);
  WindowSprite_RedrawSavedRectFar(menu->descriptionWindow);
  WindowSprite_ClearListFar(menu->questionWindow);
  ItemMenu_HideAllIcons();
  WindowSprite_RedrawSavedRectFar(menu->statusWindow);
  Task_Add(Menu_UpdateEntryObjectTransforms, TASK_PRIORITY_MAIN_LOOP);
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
void ItemMenu_DrawCategory(struct Window *window, s32 memberId, s32 mode) {
  struct ItemMenuWork *menu = gMenuWork;
  u16 *items;

  Unit_GetFar(memberId);
  ItemMenu_PositionCategory();
  ItemMenu_HideAllIcons();
  Text_DrawMessageAsTilesFar(MsgEquipSlotLabels, window, 0, 0);
  Text_DrawMessageAsTilesFar(MsgEquipSlotLabels + 1, window, 0, 32);
  Text_DrawMessageAsTilesFar(MsgEquipSlotLabels + 2, window, 0, 16);
  Text_DrawMessageAsTilesFar(MsgEquipSlotLabels + 3, window, 0, 48);
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
void ItemMenu_DrawEquippedItemNames(struct Window *window, u16 *items) {
  s32 slotIndex;
  s32 itemId;

  for (slotIndex = 0; slotIndex < INVENTORY_SLOTS; slotIndex++) {
    if (items[slotIndex] & INVENTORY_EQUIPPED) {
      itemId = items[slotIndex] & INVENTORY_ITEM_ID_MASK;
      switch (Item_GetDirectFar(itemId)->type) {
      case ITEM_TYPE_WEAPON:
        Text_DrawMessageAsTilesFar(itemId + MsgItemName, window,
                                   STATUS_EQUIPPED_NAME_X, 8);
        break;
      case ITEM_TYPE_ARMOR:
        Text_DrawMessageAsTilesFar(itemId + MsgItemName, window,
                                   STATUS_EQUIPPED_NAME_X, 56);
        break;
      case ITEM_TYPE_SHIELD:
        Text_DrawMessageAsTilesFar(itemId + MsgItemName, window,
                                   STATUS_EQUIPPED_NAME_X, 40);
        break;
      case ITEM_TYPE_HELM:
        Text_DrawMessageAsTilesFar(itemId + MsgItemName, window,
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
  struct WindowSprite *icon;
  struct WindowSprite **icons;

  entryIndex = 0;
  icons = ((struct ItemMenuWork *)gMenuWork)->entryIcons;
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
void Menu_PlaceEntryObjectInGrid(struct WindowSprite *icon, s32 index,
                                 s32 originX, s32 originY, s32 columns) {
  s32 entryIndex;

  entryIndex = index;
  if (entryIndex > 0x1F) {
    entryIndex = 0;
  }
  icon->y = (entryIndex / columns * 0x10) + originY;
  icon->x = (entryIndex % columns * 0x10) + originX;
  Icon_PrepareObject(icon);
}

/// Places the icon of each worn weapon, armor, shield and helm at x 216, on
/// its slot's row of ItemMenu_DrawCategory's window; the others stay off
/// screen. The icons are loaded already (ItemMenu_DrawIcons).
void ItemMenu_ArrangeCategoryItemIcons(u16 *items) {
  struct ItemMenuWork *menu;
  struct WindowSprite *icon;
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
        Icon_PrepareObject(icon);
      }
    }
  }
}

/// Moves all 32 entry icons off screen, to (248, 168), before a screen lays
/// them out again.
void ItemMenu_PositionCategory(void) {
  struct ItemMenuWork *menu = gMenuWork;
  s32 x = 248;
  struct WindowSprite **entry = menu->entryIcons;
  s32 y = 168;
  s32 remaining = 31;

  do {
    struct WindowSprite *object = *entry++;

    if (object != 0) {
      object->x = x;
      object->y = y;
      Icon_PrepareObject(object);
    }
    remaining--;
  } while (remaining >= 0);
}

/// Creates the eight category sprites in window, off screen at (248, 168),
/// each on item graphic slot of the same number; the Status menu shows the
/// status icons with the first five (SummaryScreen_UpdateSelectionIcons).
/// Returns 1.
s32 Menu_CreateEightEntryObjects(struct Window *window) {
  struct WindowSprite **slot;
  struct WindowSprite *icon;
  s32 slotIndex;
  struct ItemMenuWork *menu;
  s32 spriteFlags;

  menu = gMenuWork;
  slotIndex = 0;
  spriteFlags = 0xA8;
  slot = menu->categoryIcons;
  do {
    icon = WindowSprite_CreateFromResourceFar(ICON_KIND_ITEM, slotIndex, window,
                                              0xF8, spriteFlags);
    slotIndex += 1;
    *slot = icon;
    slot += 1;
  } while (slotIndex <= 7);
  return 1;
}

/// Shows the icon of each status that flags marks
/// (SummaryScreen_BuildAvailability) down the left edge from y 88, 16 pixels
/// apart, beside the status labels of SummaryScreen_DrawStatusAilments.
void ItemMenu_ApplyFlags(const u8 *flags) {
  struct ItemMenuWork *menu;
  struct WindowSprite **slot;
  struct WindowSprite *entry;
  s32 index;
  s32 shown;

  menu = gMenuWork;
  ItemMenu_ResetCategory();
  index = 0;
  slot = menu->categoryIcons;
  shown = 0;
  do {
    entry = *slot++;
    if (entry != 0 && flags[index] != 0) {
      entry->x = 8;
      entry->y = shown * 16 + 88;
      entry->sentinel = 240;
      Icon_PrepareObject(entry);
      shown++;
    }
    index++;
  } while (index <= 4);
}

/// Moves the five status icons (the first category sprites) off screen,
/// before they are shown again or while another view covers them.
void ItemMenu_ResetCategory(void) {
  struct ItemMenuWork *menu = gMenuWork;
  s32 index;

  for (index = 0; index < 5; index++) {
    struct WindowSprite *object = menu->categoryIcons[index];

    if (object != 0) {
      object->x = 248;
      object->y = 168;
      object->sentinel = 240;
      Icon_PrepareObject(object);
    }
  }
}

/// Loads each enabled status's icon into its category resource slot.
s32 SummaryScreen_UpdateSelectionIcons(const u8 *enabled) {
  struct ItemMenuWork *menu = gMenuWork;
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

#undef MEMBER_CHOOSER_MEMBER_COUNT
#undef STATUS_SAVED_TILE_BYTES
#undef STATUS_HELP_TEXT_MAX
#undef STATUS_STAT_DIGITS
#undef STATUS_STAT_X
#undef STATUS_EQUIP_PAGE_SIZE
#undef STATUS_PSYNERGY_CURSOR_X
#undef STATUS_PSYNERGY_CURSOR_Y
#undef STATUS_EQUIP_SLOT_X
#undef STATUS_EQUIP_SLOT_WIDTH
#undef STATUS_ITEM_PAGE_ICONS_X
#undef STATUS_ITEM_NAME_X
#undef STATUS_ITEM_STAT_X
#undef STATUS_ITEM_EMPTY_MESSAGE_X
#undef STATUS_EQUIP_WINDOW_WIDTH
#undef STATUS_EQUIPPED_NAME_X
