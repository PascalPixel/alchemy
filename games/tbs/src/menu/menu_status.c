/// @file
/// The party member chooser and the Status menu: its party member row, status
/// page, Psynergy page and equipment page.
// Part of menu.c.

#include "menu_internal.h"

/// From patch 1 the menu resets eight party figure rows rather than four, and
/// saves and clears more window tiles for the extra text tiles it then uses;
/// the editions outside Japanese also lay the menu out for longer words.
#if TBS_PATCH_1
#define MEMBER_CHOOSER_MEMBER_COUNT 8
#else
#define MEMBER_CHOOSER_MEMBER_COUNT 4
#endif

/// The window tiles the Status menu saves and restores.
#if TBS_PATCH_1
#define STATUS_SAVED_TILE_BYTES 8192
#else
#define STATUS_SAVED_TILE_BYTES 2048
#endif

/// The page modes of StatusScreen_DrawStatusAilments and
/// ItemMenu_DrawMemberStatus, in the mode's low byte: the party row's summary,
/// or the full status page, with the elemental levels, Power and Resist.
enum { STATUS_PAGE_SUMMARY = 0, STATUS_PAGE_FULL = 1 };
/// A bit over the page mode: draw without clearing the window first, as for a
/// window just opened.
#define STATUS_PAGE_NO_CLEAR (1 << 8)
#define STATUS_PAGE_MODE_MASK (STATUS_PAGE_NO_CLEAR - 1)

/// The longest status help text, in characters.
#if LANGUAGE != LANGUAGE_JAPANESE
#define STATUS_HELP_TEXT_MAX 128
#else
#define STATUS_HELP_TEXT_MAX 64
#endif

/// The stat column of the status page's top rows: three digits from 200 or
/// four from 192, both ending at pixel 224.
#if LANGUAGE != LANGUAGE_JAPANESE
#define STATUS_STAT_DIGITS 3
#define STATUS_STAT_X 200
#else
#define STATUS_STAT_DIGITS 4
#define STATUS_STAT_X 192
#endif

/// Items to a page of the equipment page's list.
#if LANGUAGE != LANGUAGE_JAPANESE
#define STATUS_EQUIP_PAGE_SIZE 5
#else
#define STATUS_EQUIP_PAGE_SIZE 6
#endif

/// The hand cursor on the Psynergy page.
#if LANGUAGE != LANGUAGE_JAPANESE
#define STATUS_PSYNERGY_CURSOR_X 55
#define STATUS_PSYNERGY_CURSOR_Y 60
#else
#define STATUS_PSYNERGY_CURSOR_X 58
#define STATUS_PSYNERGY_CURSOR_Y 52
#endif

/// The highlight over the equipment window's four slot rows.
#if LANGUAGE != LANGUAGE_JAPANESE
#define STATUS_EQUIP_SLOT_X 1
#define STATUS_EQUIP_SLOT_WIDTH 12
#else
#define STATUS_EQUIP_SLOT_X 2
#define STATUS_EQUIP_SLOT_WIDTH 9
#endif

/// The equipment page's item list: its page icons, the item names, and the
/// Attack and Defense figures.
#if LANGUAGE != LANGUAGE_JAPANESE
#define STATUS_ITEM_PAGE_ICONS_X 119
#define STATUS_ITEM_NAME_X 128
#define STATUS_ITEM_STAT_X 80
#else
#define STATUS_ITEM_PAGE_ICONS_X 123
#define STATUS_ITEM_NAME_X 136
#define STATUS_ITEM_STAT_X 72
#endif

/// The x of "You have no items." in the equipment page's list, in pixels.
#if LANGUAGE == LANGUAGE_GERMAN
#define STATUS_ITEM_EMPTY_MESSAGE_X 112
#else
#define STATUS_ITEM_EMPTY_MESSAGE_X 120
#endif

/// The equipment window, and the equipped items' names in it.
#if LANGUAGE != LANGUAGE_JAPANESE
#define STATUS_EQUIP_WINDOW_WIDTH 15
#define STATUS_EQUIPPED_NAME_X 8
#else
#define STATUS_EQUIP_WINDOW_WIDTH 13
#define STATUS_EQUIPPED_NAME_X 16
#endif

/// The Djinn figures stand in a row 32 pixels apart, at y STATUS_FIGURE_ROW_Y,
/// over the status page's Djinn counts. On the full status page those stand
/// three tiles higher, and the figures at STATUS_FIGURE_FULL_PAGE_ROW_Y; the
/// Psynergy page puts them above the screen.
#define STATUS_FIGURE_X(index) (130 + (index) * 32)
#define STATUS_FIGURE_ROW_Y 128
#define STATUS_FIGURE_FULL_PAGE_ROW_Y 104
#define STATUS_FIGURE_ABOVE_SCREEN_Y -16

/// The Djinn list (StatusScreen_ShowDjinnList): a column for each element, its
/// Djinni figure standing over it.
#define DJINN_LIST_COLUMN_WIDTH 56
#define DJINN_LIST_FIGURE_X(element) (32 + (element) * DJINN_LIST_COLUMN_WIDTH)
#define DJINN_LIST_FIGURE_Y 70
/// The window map entry of the first element icon, in palette 1 as
/// TILEMAP_ENTRY_ICON gives it; the four elements follow it in order.
#define STATUS_ELEMENT_TILE (BG_TILE_PALETTE(1) | 1)

/// The switch help's two lines, drawn above the status window's interior.
#define SWITCH_HELP_Y -24

/// The status page's ailments, one a row of 16 pixels from y 40.
#define STATUS_AILMENT_Y(row) ((row) * 16 + 40)

/// The status page's four element columns, 4 tiles apart from tile 15, where
/// each element's icon stands; its figures end left of it.
#define STATUS_ELEMENT_X(element) (((element) * 4 + 15) * 8)

/// Lets the player choose one party member, over the field (the Colosso scenes
/// use it, through menu.s, and so does the debug test).
s32 PartyMemberChooser_Open(void) {
  struct StatusScreenState *menu =
      HeapSlot_Alloc(HEAP_SLOT_MENU, MENU_STATE_SIZE);
  s32 result;
  s32 index;

  gActorSystemState->suspended = 1;
  Window_DrawBorderFar(0, 0, DISPLAY_TILE_WIDTH, DISPLAY_TILE_HEIGHT);
  wait(1);
  Window_InitializeState(0);

  menu->partyCount = Party_ListMembersFar(menu->memberIds);
  StatusScreen_Initialize(0, 3, 0, 7);
  menu->selectorWindow = Window_CreateFar(13, 0, 17, 5, WINDOW_FLAG_INSTANT);
  for (index = 0; index < MEMBER_CHOOSER_MEMBER_COUNT; index++)
    menu->memberY[index] = MENU_MEMBER_Y;
  menu->page = STATUS_SCREEN_PAGE_CHOOSER;

  result = PartyMemberChooser_Run();

  WindowSprite_ClearListFar(menu->statusWindow);
  ItemMenu_Close();
  gActorSystemState->suspended = 0;
  wait(1);
  HeapSlot_Release(HEAP_SLOT_MENU);
  return result;
}

/// The party member chooser's loop. Returns the chosen party member's id, or
/// -1.
s32 PartyMemberChooser_Run(void) {
  struct StatusScreenState *state = gMenuState;
  s32 finished = 0;
  s32 result = 0;

  /* FAKEMATCH(dead): StatusScreen_RunPages's loop over its steps, with its
   * first step alone: it runs once. */
  while (!finished) {
    state->selectedSlots[0] = finished;
    if (StatusScreen_SelectMember(0) == -1)
      result = -1;
    else
      result = state->selectedIds[0];
    finished = 1;
  }
  return result;
}

/// The Status screen, the field menu's Status (FieldMenu_Run,
/// FIELD_MENU_STATUS), over the field: saves the first two background palettes
/// and the window tiles it draws over, sets up the party row and, when the
/// party has Djinn, the four Djinn sprites, runs the status pages, then puts
/// everything back.
s32 StatusScreen_Run(void) {
  struct StatusScreenState *menu =
      HeapSlot_Alloc(HEAP_SLOT_MENU, MENU_STATE_SIZE);
  void *palette = Heap_AllocEwram(64);
  void *tiles = Heap_AllocEwram(STATUS_SAVED_TILE_BYTES);
  s32 result;
  s32 index;

  gActorSystemState->suspended = 1;
  Window_DrawBorderFar(0, 0, DISPLAY_TILE_WIDTH, DISPLAY_TILE_HEIGHT);
  wait(1);
  Menu_SuspendFieldTasks();
  Window_InitializeState(0);
  menu->page = STATUS_SCREEN_PAGE_STATUS;
  menu->partyCount = Party_ListMembersFar(menu->memberIds);
  StatusScreen_Initialize(0, 3, 0, 7);
  Iwram_CopyWords(palette, (void *)BG_PLTT, 64);
#if !TBS_PATCH_1
  Iwram_CopyWords(tiles, BG_CHAR_BLOCK(1), STATUS_SAVED_TILE_BYTES);
#endif
  Palette_LightenHighlight(HIGHLIGHT_PALETTE);
  // The first sprite palette into background palettes 0 and 1, for the
  // element icons drawn as window tiles: palette 0, for the highlighted row,
  // takes HIGHLIGHT_PALETTE's WINDOW_BACKGROUND_COLOR as its
  // TEXT_BACKGROUND_COLOR, palette 1 WINDOW_PALETTE's
  // (PsynergyMenu_DrawRangePage).
  Dma_Set((void *)OBJ_PLTT, (void *)BG_PLTT, DMA_COPY_HALFWORDS(16), REG_DMA3);
  Dma_Set((void *)&BG_PLTT_COLOR(HIGHLIGHT_PALETTE, WINDOW_BACKGROUND_COLOR),
          (void *)&BG_PLTT_COLOR(0, TEXT_BACKGROUND_COLOR),
          DMA_COPY_HALFWORDS(1), REG_DMA3);
  Dma_Set((void *)OBJ_PLTT, (void *)&BG_PLTT_COLOR(1, 0),
          DMA_COPY_HALFWORDS(16), REG_DMA3);
  Dma_Set((void *)&BG_PLTT_COLOR(WINDOW_PALETTE, WINDOW_BACKGROUND_COLOR),
          (void *)&BG_PLTT_COLOR(1, TEXT_BACKGROUND_COLOR),
          DMA_COPY_HALFWORDS(1), REG_DMA3);
#if TBS_PATCH_1
  Iwram_CopyWords(tiles, BG_CHAR_BLOCK(1), STATUS_SAVED_TILE_BYTES);
  Iwram_FillWords(BG_CHAR_BLOCK(1), STATUS_SAVED_TILE_BYTES, PIXELS_4BPP(3));
  Text_UseExtraTilesFar(1);
#endif
  menu->selectorWindow = Window_CreateFar(13, 0, 17, 5, WINDOW_FLAG_INSTANT);
  for (index = 0; index < STATUS_MEMBER_FIGURE_COUNT; index++)
    menu->memberY[index] = MENU_MEMBER_Y;
  if (Party_SumDjinnCountsFar(-1) != 0)
    DjinnFigures_InitializeTopRow(menu->selectorWindow, 0);
  for (index = 0; index < 4; index++) {
    menu->rowX[index] = STATUS_FIGURE_X(index);
    menu->rowY[index] = STATUS_FIGURE_ROW_Y;
  }
  PageArrows_DrawFar(PAGE_ARROW_VRAM);
  // Run the pages.
  Menu_StartWatchingStart();
  menu->page = STATUS_SCREEN_PAGE_STATUS;

  result = StatusScreen_RunPages();

  // Close, and put the palettes and tiles back.
  Menu_StopWatchingStart();
  WindowSprite_ClearListFar(menu->statusWindow);
  DjinnFigures_ReleaseTopRow();
  Menu_ResumeFieldTasks();
  Window_DrawBorderFar(0, 0, DISPLAY_TILE_WIDTH, DISPLAY_TILE_HEIGHT);
  wait(1);
#if TBS_PATCH_1
  Window_MarkVisibleTextTilesFar();
  Text_UseExtraTilesFar(0);
  wait(1);
#endif
  Iwram_CopyWords((void *)BG_PLTT, palette, 64);
  Iwram_CopyWords(BG_CHAR_BLOCK(1), tiles, STATUS_SAVED_TILE_BYTES);
  Heap_Release(tiles);
  Heap_Release(palette);
  WINDOW_STATE->holdTilemapUpload = 1;
  ItemMenu_Close();
  Window_DrawBorderFar(0, 0, DISPLAY_TILE_WIDTH, DISPLAY_TILE_HEIGHT);
  HeapSlot_Release(HEAP_SLOT_MENU);
  gActorSystemState->suspended = 0;
  wait(1);
  Window_EraseBorderRectFar(0, 0, 30, 20);
  WINDOW_STATE->holdTilemapUpload = 0;
  return result;
}

/// The steps of the Status menu (StatusScreen_RunPages).
enum StatusScreenStep {
  STATUS_STEP_MEMBER = 0,   ///< choose a party member
  STATUS_STEP_STATUS = 1,   ///< their status page
  STATUS_STEP_PSYNERGY = 2, ///< their Psynergy page
  STATUS_STEP_EQUIPMENT = 3 ///< their equipment page
};

/// The Status menu's pages: choose a party member, then their status page, A
/// for their Psynergy page and A again for their equipment page, whose A goes
/// back round to the status page; B on any page goes back to the party member
/// row.
s32 StatusScreen_RunPages(void) {
  struct StatusScreenState *menu = gMenuState;
  s32 step = STATUS_STEP_MEMBER;
  s32 finished = step;
  s32 result = 0;

  while (!finished && !GameFlag_GetFar(FLAG_MENU_START_PRESSED)) {
    switch (step) {
    case STATUS_STEP_MEMBER:
      menu->selectedSlots[0] = finished;
      if (StatusScreen_SelectMember(0) == -1) {
        result = -1;
        finished = 1;
      }
      step = STATUS_STEP_STATUS;
      break;
    case STATUS_STEP_STATUS:
      menu->memberCursors[0]->motion = ICON_HIDDEN;
      result = StatusScreen_RunStatusPage();
      step = result == -1 ? STATUS_STEP_MEMBER : STATUS_STEP_PSYNERGY;
      break;
    case STATUS_STEP_PSYNERGY:
      menu->memberCursors[0]->motion = ICON_HIDDEN;
      result = StatusScreen_RunPsynergyPage();
      step = STATUS_STEP_MEMBER;
      if (result != -1)
        step = STATUS_STEP_EQUIPMENT;
      break;
    case STATUS_STEP_EQUIPMENT:
      menu->memberCursors[0]->motion = ICON_HIDDEN;
      result = StatusScreen_RunEquipmentPage();
      // STATUS_STEP_MEMBER for -1, STATUS_STEP_STATUS otherwise.
      step = NONZERO(~result);
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
/// party member chooser on STATUS_SCREEN_PAGE_CHOOSER, else the Status menu's
/// party row.
s32 StatusScreen_SelectMember(s32 slot) {
  struct StatusScreenState *menu;
  struct WindowSprite *cursor;
  s32 result;
  s32 index;

  menu = gMenuState;
  index = menu->memberIndex[slot];
  result = 0;
  cursor = menu->memberCursors[slot];
  cursor->motion = ICON_SHOWN;
  cursor->animationStep = 0;
  WindowSprite_RedrawSavedRectFar(menu->memberWindow);
  // FLAG_LINK_BATTLE_ARRANGED is set while a link battle is arranged in the
  // Battle Arena, in which only the first three party members fight
  // (BattleArena_PartyContains); the divider falls between the third and
  // fourth figures, which stand three tiles apart.
  if (GameFlag_GetFar(FLAG_LINK_BATTLE_ARRANGED))
    Window_DrawDividerLineFar(menu->memberWindow, 9, 1, 9, 3);
  if (index == -1)
    menu->memberIndex[slot] = 0;
  else
    MenuCursor_Slide(MENU_MEMBER_CURSOR_X(index), 16);
  if (menu->page == STATUS_SCREEN_PAGE_CHOOSER)
    result = PartyMemberChooser_ChooseMember();
  else
    result = StatusScreen_RunPartyRow();
  cursor = menu->memberCursors[slot];
  Icon_Show(cursor);
  wait(1);
  return result;
}

/// Shows every Djinn the party has found, a column of seven for each element,
/// until A, B or Select, then puts the screen back.
s32 StatusScreen_ShowDjinnList(void) {
  struct DjinnMenuState *menu;
  struct Window *window;
  s32 element;
  s32 djinn;

  menu = gMenuState;
  StatusScreen_HideStatusIcons();
  Sound_PlayFar(SE_MENU_CONFIRM);
  WindowSprite_RedrawSavedRectFar(menu->helpWindow);
  Text_DrawMessageAsTilesFar(MsgReturnHelp, menu->helpWindow, 0, 16);
  menu->cursor->motion = ICON_HIDDEN;
  menu->secondCursor->motion = ICON_HIDDEN;
  wait(1);
  window = menu->window;
  {
    s32 i;

    for (i = 0; i < 4; i++) {
      menu->slotX[i] = DJINN_LIST_FIGURE_X(i);
      menu->slotY[i] = DJINN_LIST_FIGURE_Y;
      menu->memberY[i] = MENU_MEMBER_Y;
    }
  }
  WindowSprite_RedrawSavedRectFar(window);
  Window_DrawDividerLineFar(window, 0, 11, 28, 11);
  Text_DrawMessageFar(MsgCurrentDjinn, menu->helpWindow, -96, 132);
  for (element = 0; element < ELEMENT_COUNT; element++) {
    for (djinn = 0; djinn < 7; djinn++) {
      s32 id = element * DJINN_PER_ELEMENT + djinn;

      // FLAG_DJINN_FIRST on: one per Djinni found, DJINN_PER_ELEMENT to an
      // element.
      if (GameFlag_GetFar(FLAG_DJINNI(id))) {
        Window_SetTilemapEntryFar(window, STATUS_ELEMENT_TILE + element,
                                  element * (DJINN_LIST_COLUMN_WIDTH / 8) + 1,
                                  djinn + 3, TILEMAP_ENTRY_PLAIN);
        Text_DrawMessageAsTilesFar(MsgDjinnName + id, window,
                                   element * DJINN_LIST_COLUMN_WIDTH + 16,
                                   djinn * 8 + 24);
      }
    }
  }
  WINDOW_STATE->dirty = 1;
  for (;;) {
    if (GameFlag_GetFar(FLAG_MENU_START_PRESSED))
      break;
    wait(1);
    if (gKeysPressed & (KEY_A | KEY_B | KEY_SELECT))
      break;
  }
  WindowSprite_RedrawSavedRectFar(menu->window);
  WindowSprite_ClearListFar(menu->helpWindow);
  {
    s32 i;

    for (i = 0; i < 4; i++) {
      menu->slotX[i] = STATUS_FIGURE_X(i);
      menu->slotY[i] = STATUS_FIGURE_ROW_Y;
    }
  }
  menu->cursor->motion = ICON_SHOWN;
  menu->secondCursor->motion = ICON_SHOWN;
  Sound_PlayFar(SE_MENU_CANCEL);
  // UB: returns no value.
#ifdef UBFIX
  return 0;
#endif
}

struct ItemMenuState;

/// Lowers the party figures, then raises the currently selected party member.
static __inline__ void
StatusScreen_SetSelectedMemberRow(struct StatusScreenState *menu,
                                  s32 selectedMember) {
  s32 rowIndex;

  for (rowIndex = 0; rowIndex < STATUS_MEMBER_FIGURE_COUNT; rowIndex++)
    menu->memberY[rowIndex] = MENU_MEMBER_Y;
  menu->memberY[selectedMember] = MENU_MEMBER_RAISED_Y;
}

/// The Status menu's party row: Left and Right choose a party member, showing
/// their status; L and R move the party member one place along the party order;
/// Select lists the Djinn found (once the first is caught, FLAG_DJINN_FIRST); A
/// chooses the party member and B backs out.
s32 StatusScreen_RunPartyRow(void) {
  struct StatusScreenState *menu;
  s32 memberIndex;
  s32 memberCount;
  s32 page;
  s32 redraw;
  s32 result;
  s32 rowIndex;

  menu = gMenuState;
  memberIndex = menu->memberIndex[0];
  memberCount = menu->memberCount[0];
  redraw = 1;
  result = 0;
  page = menu->page;
  Unit_GetFar(menu->memberIds[memberIndex]);
  for (rowIndex = 0; rowIndex < 4; rowIndex++) {
    menu->rowX[rowIndex] = STATUS_FIGURE_X(rowIndex);
    menu->rowY[rowIndex] = STATUS_FIGURE_ROW_Y;
  }
  // The highlight palette, and the first sprite palette into background
  // palettes 0 and 1, as StatusScreen_Run sets them.
  Palette_LightenHighlight(HIGHLIGHT_PALETTE);
  Dma_Set((void *)OBJ_PLTT, (void *)BG_PLTT, DMA_COPY_HALFWORDS(16), REG_DMA3);
  Dma_Set((void *)&BG_PLTT_COLOR(HIGHLIGHT_PALETTE, WINDOW_BACKGROUND_COLOR),
          (void *)&BG_PLTT_COLOR(0, TEXT_BACKGROUND_COLOR),
          DMA_COPY_HALFWORDS(1), REG_DMA3);
  Dma_Set((void *)OBJ_PLTT, (void *)&BG_PLTT_COLOR(1, 0),
          DMA_COPY_HALFWORDS(16), REG_DMA3);
  Dma_Set((void *)&BG_PLTT_COLOR(WINDOW_PALETTE, WINDOW_BACKGROUND_COLOR),
          (void *)&BG_PLTT_COLOR(1, TEXT_BACKGROUND_COLOR),
          DMA_COPY_HALFWORDS(1), REG_DMA3);
  while (!GameFlag_GetFar(FLAG_MENU_START_PRESSED)) {
    if (redraw) {
      redraw = 0;
      // The help lines, and the party member's status and figure.
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
      StatusScreen_ShowMemberStatus(menu->memberIds[memberIndex], page);
      Menu_ReloadMemberPortrait(menu, menu->memberIds[memberIndex]);
      StatusScreen_SetSelectedMemberRow(menu, memberIndex);
    }
    MenuCursor_Position(MENU_MEMBER_CURSOR_X(memberIndex), 16);
    wait(1);
    if (gKeysPressed & KEY_A) {
      Sound_PlayFar(SE_MENU_CONFIRM);
      result = 1;
      break;
    }
    if (gKeysPressed & KEY_B) {
      Sound_PlayFar(SE_MENU_CANCEL);
      result = -1;
      break;
    }
    // L and R move the party member along the party order.
    if (gKeysRepeat & KEY_R) {
      if (StatusScreen_MovePartyMember(memberIndex, 1)) {
        Sound_PlayFar(SE_MENU_CONFIRM);
        memberIndex++;
        Menu_ReleasePartyFigures();
        Menu_CreatePartyFigures(menu->memberWindow, 2, 2, 8, 0);
        StatusScreen_SetSelectedMemberRow(menu, memberIndex);
      } else {
        Sound_PlayFar(SE_MENU_ERROR);
      }
      wait(1);
    } else if (gKeysRepeat & KEY_L) {
      if (StatusScreen_MovePartyMember(memberIndex, 0)) {
        Sound_PlayFar(SE_MENU_CONFIRM);
        memberIndex--;
        Menu_ReleasePartyFigures();
        Menu_CreatePartyFigures(menu->memberWindow, 2, 2, 8, 0);
        StatusScreen_SetSelectedMemberRow(menu, memberIndex);
      } else {
        Sound_PlayFar(SE_MENU_ERROR);
      }
      wait(1);
    } else if ((gKeysPressed & KEY_SELECT) &&
               GameFlag_GetFar(FLAG_DJINN_FIRST)) {
      StatusScreen_ShowDjinnList();
      redraw = 1;
    } else {
      if (gKeysRepeat & KEY_LEFT) {
        Sound_PlayFar(SE_MENU_CURSOR_MOVE);
        if (memberCount > 1) {
          memberIndex--;
          redraw = 1;
        }
      }
      if (gKeysRepeat & KEY_RIGHT) {
        Sound_PlayFar(SE_MENU_CURSOR_MOVE);
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

/// The party member chooser (STATUS_SCREEN_PAGE_CHOOSER): Left and Right choose
/// a party member, showing their status and Psynergy icons; A chooses and B
/// backs out.
s32 PartyMemberChooser_ChooseMember(void) {
  struct PsynergyMenuState *menu = gMenuState;
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
      StatusScreen_ShowMemberStatus(menu->memberTable[memberIndex], page);
      Menu_ReloadMemberPortrait(menu, menu->memberTable[memberIndex]);
      for (rowIndex = 0; rowIndex < STATUS_MEMBER_FIGURE_COUNT; rowIndex++)
        menu->rowPositions[rowIndex] = MENU_MEMBER_Y;
      menu->rowPositions[memberIndex] = MENU_MEMBER_RAISED_Y;
      menu->psynergyCount = PsynergyMenu_CollectActions(
          Unit_GetFar(menu->memberTable[memberIndex]), (u16 *)menu->psynergies,
          PSYNERGY_LIST_ALL);
      PsynergyMenu_DrawPsynergyIcons(menu->psynergies);
      Menu_PlaceEntryActorsInGrid(96, 96, 8);
      Menu_HideEmptyEntryIcons(menu->psynergies);
    }
    MenuCursor_Position(MENU_MEMBER_CURSOR_X(memberIndex), 16);
    wait(1);
    if (gKeysPressed & KEY_A) {
      Sound_PlayFar(SE_MENU_CONFIRM);
      result = 1;
      break;
    }
    if (gKeysPressed & KEY_B) {
      Sound_PlayFar(SE_MENU_CANCEL);
      result = -1;
      break;
    }
    {
      if (gKeysRepeat & KEY_LEFT) {
        Sound_PlayFar(SE_MENU_CURSOR_MOVE);
        if (memberCount > 1) {
          memberIndex--;
          redraw = 1;
        }
      }
      if (gKeysRepeat & KEY_RIGHT) {
        Sound_PlayFar(SE_MENU_CURSOR_MOVE);
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
s32 StatusScreen_MovePartyMember(s32 selectedIndex, s32 direction) {
  struct StatusScreenState *menu = gMenuState;
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

  for (index = 0; index < (s32)ARRAY_COUNT(reordered); index++) {
    reordered[index] = 0;
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
/// figures, no other windows yet, and an 8 by 2 cursor grid.
void StatusScreen_Initialize(s32 x, s32 y, s32 mode, s32 columns) {
  struct StatusScreenState *menu;

  menu = gMenuState;
  Menu_CreatePartyFigures(MenuCursor_Create(menu), 2, 2, 8, 0);
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
/// opening it first with the party member's portrait (kept as the state's
/// statusCursor), the 32 entry icons (the chooser's Psynergy icons) on the
/// chooser page, and the eight category sprites that show the status icons;
/// then draws the stats and statuses. frame is not used.
void StatusScreen_ShowMemberStatus(s32 member, s32 frame) {
  s32 created;
  struct Window *window;
  struct WindowSprite *cursor;
  struct StatusScreenState *menu;

  menu = gMenuState;
  created = 0;
  window = menu->statusWindow;
  if (window == 0) {
    created = Window_UpdateOrCreate(&menu->statusWindow, 0, 5, 30, 15,
                                    WINDOW_FLAG_INSTANT);
    window = menu->statusWindow;
  }
  if (created != 0) {
    cursor = Portrait_CreateFar(member, 0, 0, window, 0, 0);
    menu->statusCursor = cursor;
    // The portrait goes to depth slot 240, as the category sprites do.
    if ((cursor->depth = OAM_DEPTH_WINDOW, menu->page) ==
        STATUS_SCREEN_PAGE_CHOOSER) {
      Menu_SpawnIconEntries((struct ItemMenuState *)menu, window);
    }
    Menu_CreateEightEntryActors(window);
    StatusScreen_DrawStatusAilments(window, member, STATUS_PAGE_NO_CLEAR);
    return;
  }
  StatusScreen_DrawStatusAilments(window, member, STATUS_PAGE_SUMMARY);
}

/// The party member's status page.
s32 StatusScreen_RunStatusPage(void) {
  struct StatusScreenState *menu;
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

  menu = gMenuState;
  side = 0;
  result = 0;
  selected = 0;
  djinnTotal = Party_SumDjinnCountsFar(-1);
  hasDjinn = 0;
  if (djinnTotal != 0)
    hasDjinn = 1;
  // The help window covers the top of the screen.
  Window_UpdateOrCreate(&menu->helpWindow, 0, 0, 30, 5, WINDOW_FLAG_INSTANT);
  Task_Remove(Menu_PlacePartyFigures);
  for (i = 0; i < 4; i++)
    menu->rowY[i] = STATUS_FIGURE_FULL_PAGE_ROW_Y;
  done = 0;
  MenuCursor_Slide(-10, 88);

  // One party member's page at a time; L and R start the next.
  while (done == 0 && GameFlag_GetFar(FLAG_MENU_START_PRESSED) == 0) {
    Unit_GetFar(menu->selectedIds[0]);
    StatusScreen_DrawStatusAilments(menu->statusWindow, menu->selectedIds[0],
                                    STATUS_PAGE_FULL);
    count = StatusScreen_ListStatuses(entries, 1, menu->selectedIds[0]);
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
                                       menu->statusWindow, SWITCH_HELP_X,
                                       SWITCH_HELP_Y);
            Text_DrawMessageAsTilesFar(MsgSwitchCharacterHelp + 1,
                                       menu->statusWindow, 0, SWITCH_HELP_Y);
          }
        } else {
          // The right side has eight entries, seven without the Djinn row.
          WindowSprite_RedrawSavedRectFar(menu->helpWindow);
          if (hasDjinn != 0)
            selected = (selected + 8) % 8;
          else
            selected = (selected + 7) % 7;
        }
        StatusScreen_DrawSelectionCursor(side, selected, entries, 0);
        WindowSprite_ClearListFar(menu->helpWindow);
        wait(1);
        if (side == 0)
          StatusScreen_DrawSelectionLabels(menu->helpWindow, selected, entries);
        else
          StatusScreen_ShowMemberProgressMessage(menu->helpWindow, selected,
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
      wait(1);
      // Take the highlight off before the cursor moves.
      if (gKeysRepeat & KEYS_DPAD)
        StatusScreen_DrawSelectionCursor(side, selected, entries, 1);
      if (gKeysPressed & KEY_A) {
        Sound_PlayFar(SE_MENU_CONFIRM);
        done = 1;
        result = 1;
        break;
      }
      if (gKeysPressed & KEY_B) {
        Sound_PlayFar(SE_MENU_CANCEL);
        done = 1;
        result = -1;
        break;
      }
      if (gKeysRepeat & KEY_UP) {
        Sound_PlayFar(SE_MENU_CURSOR_MOVE);
        redraw = 1;
        selected--;
      }
      if (gKeysRepeat & KEY_DOWN) {
        Sound_PlayFar(SE_MENU_CURSOR_MOVE);
        redraw = 1;
        selected++;
      }
      if (gKeysRepeat & KEY_RIGHT) {
        Sound_PlayFar(SE_MENU_CURSOR_MOVE);
        redraw = 1;
        side++;
      }
      if (gKeysRepeat & KEY_LEFT) {
        Sound_PlayFar(SE_MENU_CURSOR_MOVE);
        redraw = 1;
        side--;
      }
      // L and R: the previous or next party member.
      if ((gKeysRepeat & KEY_R) || (gKeysRepeat & KEY_L)) {
        Sound_PlayFar(SE_MENU_CURSOR_MOVE);
        member = menu->memberIndex[0];
        if (gKeysRepeat & KEY_R)
          member++;
        else
          member--;
        member = (member + menu->partyCount) % menu->partyCount;
        menu->selectedMember = menu->memberIds[member];
        menu->selectedIds[0] = menu->memberIds[member];
        menu->memberIndex[0] = member;
        Menu_ReloadMemberPortrait(menu, menu->memberIds[member]);
        break;
      }
    }
  }
  WindowSprite_ClearListFar(menu->helpWindow);
  WindowSprite_RedrawSavedRectFar(menu->helpWindow);
  Window_ClearInteriorTilesFar(menu->statusWindow, 64, 56, 224, 96);
  Task_Add(Menu_PlacePartyFigures, TASK_PASS_FRAME);
  for (i = 0; i < 4; i++)
    menu->rowY[i] = STATUS_FIGURE_ROW_Y;
  StatusScreen_HideStatusIcons();
  return result;
}

/// Highlights (invert 0) or unhighlights (invert 1) the selected entry of the
/// status page: on side 0 the selected status row, as wide as its label
/// (gStatusScreen_CursorWidths); on side 1 one of the first four stat rows
/// or of the rows below.
void StatusScreen_DrawSelectionCursor(s32 side, s32 selected, u8 *entries,
                                      s32 invert) {
  struct StatusScreenState *menu = gMenuState;
  s32 count;
  s32 index;
  s32 x;
  s32 y;
  s32 palette;
  u8 width;

  if (side == 0) {
    y = selected * 2 + 5;
    x = 0;
    width = 5;
    count = 0;
    for (index = 0; index <= 4; index++) {
      if (entries[index] != 0) {
        if (selected == count) {
          width = gStatusScreen_CursorWidths[index];
          break;
        }
        count++;
      }
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

  // HIGHLIGHT_PALETTE when invert is 0, WINDOW_PALETTE when it is 1.
  palette = WINDOW_PALETTE - NONZERO(invert ^ 1);
  Window_SetHighlightRect(menu->statusWindow, x, y, width, 1, palette);
}

/// Writes the advice for the selected status (two lines, for example how to
/// revive a party member who is down) into window target, or that the party
/// member is well when they have no status.
void StatusScreen_DrawSelectionLabels(struct Window *target, s32 selected,
                                      const u8 *entries) {
  s32 labelCount = 0;
  s32 labelIndex;

  for (labelIndex = 0; labelIndex <= 4; labelIndex++) {
    if (entries[labelIndex] != 0) {
      if (selected == labelCount) {
        s32 messageId = MsgStatusAdvice + labelIndex * 2;
        Text_DrawMessageFar(messageId, target, 0, -1);
        Text_DrawMessageFar(messageId + 1, target, 0, 15);
      }
      labelCount++;
    }
  }

  if (labelCount == 0)
    Text_DrawMessageFar(MsgStatusNormal, target, 0, 0);
}

// The experience still needed is worked out in 64 bits outside Japanese; the
// message argument takes 32 bits either way.
#if TBS_PATCH_1
typedef u64 ExperienceDifference;
#else
typedef u32 ExperienceDifference;
#endif

/// Writes the help text of the status page's right-hand row `row` into
/// destination: the rows skip the Djinn row when the party has no Djinn
/// (hasDjinn 0); the experience row says how many points the next level needs,
/// or, at level 99, gives the plain experience text.
void StatusScreen_ShowMemberProgressMessage(struct Window *destination, s32 row,
                                            s32 hasDjinn) {
  struct StatusScreenState *menu = gMenuState;
  void *buffer;

  // Entry 4 is the Djinn row's help: without Djinn the rows below it come
  // one entry earlier.
  if (hasDjinn == 0 && row > 3) {
    row++;
  }
  if (row == 1) {
    struct Unit *member = Unit_GetFar(menu->selectedIds[0]);

    if (member->level == UNIT_LEVEL_MAX) {
      row = 8; // "These are your experience points."
    } else {
      ExperienceDifference remaining =
          Unit_GetExperienceForLevelFar(menu->selectedIds[0],
                                        member->level + 1) -
          member->experience;
      MessageArgument_PushFar(remaining, MESSAGE_ARGUMENT_NUMBER);
    }
  }

  buffer = Heap_Alloc(STATUS_HELP_TEXT_MAX * 2);
  Message_CopyExpandedFar(row + MsgProgressHelp, buffer, STATUS_HELP_TEXT_MAX);
  Text_DrawWideStringAtOffsetFar(buffer, destination, 0, -1);
  Heap_Release(buffer);
}

/// Draws party member memberId's status page into window: the top rows
/// (ItemMenu_DrawMemberStatus), the status list or "Normal", and except on the
/// chooser page the Djinn counts by element and, on STATUS_PAGE_FULL, the
/// elemental levels, Power and Resist.
void StatusScreen_DrawStatusAilments(struct Window *window, s32 memberId,
                                     s32 mode) {
  struct StatusScreenState *menu = gMenuState;
  struct Unit *unit;
  s32 djinnRow;
  s32 hasDjinn;
  s32 keepDrawn;
  s32 i;
  u8 ailments[8];

  if (Party_SumDjinnCountsFar(-1) != 0)
    hasDjinn = 1;
  else
    hasDjinn = 0;
  unit = Unit_GetFar(memberId);
  // The Djinn row, in tiles: higher on STATUS_PAGE_FULL, which has the
  // elemental rows below it.
  if ((mode & STATUS_PAGE_MODE_MASK) == STATUS_PAGE_FULL)
    djinnRow = 7;
  else
    djinnRow = 10;
  menu->statusCursor->motion = ICON_SHOWN;
  ItemMenu_DrawMemberStatus(window, memberId, mode);
  StatusScreen_ListStatuses(ailments, 1, memberId);
  StatusScreen_UpdateSelectionIcons(ailments);
  keepDrawn = mode & STATUS_PAGE_NO_CLEAR;
  if (keepDrawn == 0)
    Window_ClearInteriorTilesFar(window, 0, 40, 96, 96);
  // The statuses, one a row, or "Normal".
  i = 0;
  if (ailments[CHARACTER_DOWN] != 0) {
    Text_DrawMessageAsTilesFar(MsgDownLabel, window, 16, STATUS_AILMENT_Y(0));
    i = 1;
  }
  if (ailments[CHARACTER_POISON] != 0) {
    Text_DrawMessageAsTilesFar(MsgPoisonLabel, window, 16, STATUS_AILMENT_Y(i));
    i++;
  }
  if (ailments[CHARACTER_VENOM] != 0) {
    Text_DrawMessageAsTilesFar(MsgVenomLabel, window, 16, STATUS_AILMENT_Y(i));
    i++;
  }
  if (ailments[CHARACTER_CURSE] != 0) {
    Text_DrawMessageAsTilesFar(MsgCurseLabel, window, 16, STATUS_AILMENT_Y(i));
    i++;
  }
  if (ailments[CHARACTER_HAUNT] != 0) {
    Text_DrawMessageAsTilesFar(MsgHauntLabel, window, 16, STATUS_AILMENT_Y(i));
    i++;
  }
  if (i == 0)
    Text_DrawMessageAsTilesFar(MsgNormalLabel, window, 0, STATUS_AILMENT_Y(0));
  StatusScreen_UpdateSelectionIcons(ailments);
  StatusScreen_ShowStatusIcons(ailments);
  if (menu->page == STATUS_SCREEN_PAGE_CHOOSER)
    return;
  if (keepDrawn == 0) {
    wait(1);
    Window_ClearInteriorTilesFar(window, 64, 56, 224, 96);
  }
  // The four element icons, then the Djinn counts and, on STATUS_PAGE_FULL,
  // the elemental levels, Power and Resist.
  Text_SetColorFar(TEXT_COLOR_NORMAL);
  if (mode == STATUS_PAGE_FULL || hasDjinn == 1) {
    Window_SetTilemapEntryFar(window, 1, 15, djinnRow, TILEMAP_ENTRY_ICON);
    Window_SetTilemapEntryFar(window, 2, 19, djinnRow, TILEMAP_ENTRY_ICON);
    Window_SetTilemapEntryFar(window, 3, 23, djinnRow, TILEMAP_ENTRY_ICON);
    Window_SetTilemapEntryFar(window, 4, 27, djinnRow, TILEMAP_ENTRY_ICON);
  }
  if (hasDjinn)
    Text_DrawMessageAsTilesFar(MsgDjinnLabel, window, 64, (djinnRow + 1) * 8);
  if (mode == STATUS_PAGE_FULL) {
    if (!hasDjinn)
      djinnRow--;
    MENU_DRAW_STATUS_LABEL(gMenu_LvString, window, 64, (djinnRow + 2) * 8);
    Text_DrawMessageAsTilesFar(MsgPowerLabel, window, 64, (djinnRow + 3) * 8);
    Text_DrawMessageAsTilesFar(MsgPowerLabel + 1, window, 64,
                               (djinnRow + 4) * 8);
  }
  for (i = 0; i <= 3; i++) {
    if (hasDjinn)
      Text_DrawNumberInWindowFar(unit->djinnHeldCounts[i], 1, window,
                                 STATUS_ELEMENT_X(i), (djinnRow + 1) * 8);
    if ((mode & STATUS_PAGE_MODE_MASK) == STATUS_PAGE_FULL) {
      if (hasDjinn) {
        Text_DrawNumberInWindowFar(unit->djinnSetCounts[i], 1, window,
                                   STATUS_ELEMENT_X(i) - 16,
                                   (djinnRow + 1) * 8);
        Text_DrawStringInWindowFar(gMenu_SlashString, window,
                                   STATUS_ELEMENT_X(i) - 8, (djinnRow + 1) * 8);
      }
      Text_DrawNumberInWindowFar(Unit_GetElementLevelFar(memberId, i), 2,
                                 window, STATUS_ELEMENT_X(i) - 8,
                                 (djinnRow + 2) * 8);
      Text_DrawNumberInWindowFar(unit->element[i].power, 3, window,
                                 STATUS_ELEMENT_X(i) - 16, (djinnRow + 3) * 8);
      Text_DrawNumberInWindowFar(unit->element[i].resistance, 3, window,
                                 STATUS_ELEMENT_X(i) - 16, (djinnRow + 4) * 8);
    }
  }
}

#if TBS_PATCH_1
/// Spins for 256 loop passes (international editions only). Nothing calls it.
void Menu_BusyWait(void) {
  s16 count;

  for (count = 0; count < 256; count++) {
  }
}

#endif

/// Stat label `n` of "Attack", "Defense", "Agility" and "Luck" (MsgStatLabel +
/// n), counted back from "Exp" (MsgExpLabel), which the status page also shows.
#define STAT_LABEL(n)                                                          \
  (MsgExpLabel - (MSGNO(MsgExpLabel) - MSGNO(MsgStatLabel)) + (n))

/// Draws the top rows of party member memberId's status page into window: name,
/// class, level, experience, HP and PP on the left, Attack, Defense, Agility
/// and Luck on the right.
void ItemMenu_DrawMemberStatus(struct Window *window, s32 memberId, s32 flags) {
  struct ItemMenuState *menu;
  struct Unit *unit;

  menu = gMenuState;
  unit = Unit_GetFar(memberId);
  menu->cursor->motion = ICON_SHOWN;
  flags &= STATUS_PAGE_NO_CLEAR;
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
    wait(1);
    Window_ClearInteriorTilesFar(window, 144, 0, 224, 40);
  }
  // "Attack", "Defense", "Agility" and "Luck".
  Text_DrawMessageAsTilesFar(STAT_LABEL(0), window, 152, 0);
  Text_DrawMessageAsTilesFar(STAT_LABEL(1), window, 152, 8);
  Text_DrawMessageAsTilesFar(STAT_LABEL(2), window, 152, 16);
  Text_DrawMessageAsTilesFar(STAT_LABEL(3), window, 152, 24);
  Text_DrawNumberInWindowFar(unit->attack, STATUS_STAT_DIGITS, window,
                             STATUS_STAT_X, 0);
  Text_DrawNumberInWindowFar(unit->defense, STATUS_STAT_DIGITS, window,
                             STATUS_STAT_X, 8);
  Text_DrawNumberInWindowFar(unit->agility, STATUS_STAT_DIGITS, window,
                             STATUS_STAT_X, 16);
  Text_DrawNumberInWindowFar(unit->luck, STATUS_STAT_DIGITS, window,
                             STATUS_STAT_X, 24);
}

/// Fills ailments[0..4] (UnitStatus) with party member memberId's statuses:
/// down (only when requested is 1), poison or venom, curse and haunt.
s32 StatusScreen_ListStatuses(u8 *ailments, s32 requested, s32 memberId) {
  struct Unit *unit;
  s32 ailmentIndex;
  s32 count;
  s32 poison;

  unit = Unit_GetFar(memberId);
  for (ailmentIndex = 0; ailmentIndex < CHARACTER_AILMENT_COUNT; ailmentIndex++)
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

/// Fills result with the item list of the party member in side index for the
/// equipment pages: their unit, the menu's item count, and the page and row of
/// the item chosen last for them, STATUS_EQUIP_PAGE_SIZE items to a page.
s32 PagedList_Build(struct PagedList *result, s32 index) {
  s32 unitBits;
  struct ItemMenuState *menu = gMenuState;
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

/// The Psynergy range icons' tiles: two sets of RANGE_ICON_PAIRS tile pairs,
/// a pair RANGE_ICON_PAIR_WORDS of tile data, a word to a 4bpp tile row.
/// RANGE_ICON_ALL, the last pair, stands for ABILITY_RANGE_ALL.
enum { RANGE_ICON_PAIRS = 6, RANGE_ICON_PAIR_WORDS = 2 * TILE_SIZE_4BPP / 4 };

/// Builds the Psynergy range icons' tiles in the window tile area, each a fill
/// of WINDOW_BACKGROUND_COLOR with the rows of gMenu_PatternRowMasks XORed in,
/// the first set growing row by row and the second skipping its top row.
void Menu_BuildPatternTiles(void) {
  u32 *vram = (u32 *)(BG_CHAR_BLOCK(0) +
                      (RANGE_ICON_TILE & TILE_NUMBER_MASK) * TILE_SIZE_4BPP);
  s32 set;
  s32 tileIndex;

  for (set = 0; set < 2; set++) {
    for (tileIndex = 0; tileIndex < RANGE_ICON_PAIRS; tileIndex++) {
      u32 *tile = vram + set * RANGE_ICON_PAIRS * RANGE_ICON_PAIR_WORDS +
                  tileIndex * RANGE_ICON_PAIR_WORDS;
      s32 rowIndex;

      Iwram_FillWords(tile, 2 * TILE_SIZE_4BPP, TEXT_CANVAS_BACKGROUND_WORD);
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
/// the ability's range less one (0 for the aimed unit alone), or
/// RANGE_ICON_ALL for ABILITY_RANGE_ALL (PsynergyMenu_DrawListPage).
void PsynergyMenu_DrawRange(struct Window *window, s32 x, s32 y, s32 range,
                            s32 unused) {
  s32 iconIndex;
  s32 tile;

  (void)unused;

  iconIndex = range * 2;
  tile = iconIndex + RANGE_ICON_TILE + 1;
  Window_SetTilemapEntryFar(window, TILE_HFLIP | tile, x, y,
                            TILEMAP_ENTRY_PLAIN);
  Window_SetTilemapEntryFar(window, iconIndex + RANGE_ICON_TILE, x + 1, y,
                            TILEMAP_ENTRY_PLAIN);
  Window_SetTilemapEntryFar(window, tile, x + 2, y, TILEMAP_ENTRY_PLAIN);
}

/// Updates the Psynergy page for the row under the cursor: the Psynergy's
/// description in the info window, where it can be used (in the field, in
/// battle, or anywhere), and the row highlights with the range icons.
s32 PsynergyMenu_DrawRangePage(struct Window *window, s32 unused,
                               struct PagedList *state) {
  struct PsynergyMenuState *menu;
  struct AbilityDefinition *ability;
  s32 row;
  s32 base;

  menu = gMenuState;
  state->selectedIndex = state->page * STATUS_LIST_PAGE_ROWS + state->row;
#if TBS_PATCH_1 && !TBS_PATCH_4
  WindowSprite_RedrawSavedRectFar(menu->infoWindow);
#elif TBS_PATCH_4
  Window_ClearInteriorTilesFar(menu->infoWindow, 0, 0, 224, 8);
#else
  WindowSprite_ClearListFar(menu->infoWindow);
#endif
  wait(1);
  if (menu->psynergies[state->selectedIndex] != 0) {
    MENU_DRAW_DESCRIPTION(
        (menu->psynergies[state->selectedIndex] & PSYNERGY_ABILITY_MASK) +
            MsgAbilityDescription,
        menu->infoWindow, 0, 0);
    ability = Ability_Get(menu->psynergies[state->selectedIndex] &
                          PSYNERGY_ABILITY_MASK);
#if LANGUAGE != LANGUAGE_JAPANESE
    Window_ClearInteriorTilesFar(window, 0, 96, 224, 104);
    // row holds where it can be used meanwhile: 2 for the field, 1 for battle.
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

  // Highlight the cursor's row.
  base = state->page * STATUS_LIST_PAGE_ROWS;
  for (row = 0; row <= STATUS_LIST_PAGE_ROWS - 1; row++) {
    if (row == state->row) {
      ability =
          Ability_Get(menu->psynergies[base + row] & PSYNERGY_ABILITY_MASK);
      if (ability->element != ELEMENT_NONE) {
        Window_SetTilemapEntryFar(window, ability->element + 1, 24,
                                  row * 2 + STATUS_PSYNERGY_LIST_FIRST_ROW,
                                  TILEMAP_ENTRY_PLAIN);
        Window_SetHighlightRect(window, 9,
                                row * 2 + STATUS_PSYNERGY_LIST_FIRST_ROW, 15, 1,
                                HIGHLIGHT_PALETTE);
        Window_SetHighlightRect(window, 25,
                                row * 2 + STATUS_PSYNERGY_LIST_FIRST_ROW, 3, 1,
                                HIGHLIGHT_PALETTE);
      } else {
        Window_SetHighlightRect(window, 9,
                                row * 2 + STATUS_PSYNERGY_LIST_FIRST_ROW, 19, 1,
                                HIGHLIGHT_PALETTE);
      }
    } else {
      ability =
          Ability_Get(menu->psynergies[base + row] & PSYNERGY_ABILITY_MASK);
      if (ability->element != ELEMENT_NONE) {
        Window_SetTilemapEntryFar(window, ability->element + 1, 24,
                                  row * 2 + STATUS_PSYNERGY_LIST_FIRST_ROW,
                                  TILEMAP_ENTRY_ICON);
        Window_SetHighlightRect(window, 9,
                                row * 2 + STATUS_PSYNERGY_LIST_FIRST_ROW, 15, 1,
                                WINDOW_PALETTE);
        Window_SetHighlightRect(window, 25,
                                row * 2 + STATUS_PSYNERGY_LIST_FIRST_ROW, 3, 1,
                                WINDOW_PALETTE);
      } else {
        Window_SetHighlightRect(window, 9,
                                row * 2 + STATUS_PSYNERGY_LIST_FIRST_ROW, 19, 1,
                                WINDOW_PALETTE);
      }
    }
  }
  wait(1);
  return 1;
}

/// Draws one page of the Status menu's Psynergy list into window: up to
/// STATUS_LIST_PAGE_ROWS names with their icons, PP costs and range icons, the
/// page numbers or "no Psynergy", and the party member's name, class and level.
s32 PsynergyMenu_DrawListPage(struct Window *window, s32 unused,
                              const struct PagedList *res) {
  struct PsynergyMenuState *menu = gMenuState;
  struct Unit *member;
  u32 firstEntry;
  u32 rows;
  u8 row;
  s32 entryIndex;

  (void)unused;

  member = Unit_GetFar(menu->memberIds[0]);

  WindowSprite_RedrawSavedRectFar(window);

  firstEntry = res->page * STATUS_LIST_PAGE_ROWS;
  rows = (u8)(res->entryCount - firstEntry);
  if (rows > STATUS_LIST_PAGE_ROWS) {
    rows = STATUS_LIST_PAGE_ROWS;
  }

  Menu_SetPageIcons(STATUS_LIST_PAGE_ROWS, firstEntry, window,
                    STATUS_PSYNERGY_LIST_ICONS_X, STATUS_PSYNERGY_LIST_ICONS_Y);
  PagedList_DrawPageNumbers(window, res->entryCount, STATUS_LIST_PAGE_ROWS,
                            res->page, 28);

  Text_DrawMessageAsTilesFar(MsgPsynergyPp, window, STATUS_PSYNERGY_LIST_PP_X,
                             0);

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
      y = row * 16 + STATUS_PSYNERGY_LIST_FIRST_ROW * 8;

      Text_DrawMessageAsTilesFar(messageId, window, STATUS_PSYNERGY_LIST_NAME_X,
                                 y);
      Text_DrawNumberAtOffsetFar(action->ppCost, 2, window,
                                 STATUS_PSYNERGY_LIST_PP_NUMBER_X, y);

      // Range n takes range icon n - 1.
      range = action->range;
      if (range == ABILITY_RANGE_ALL) {
        range = RANGE_ICON_ALL;
      } else {
        range--;
      }
      PsynergyMenu_DrawRange(
          window, 25, row * 2 + STATUS_PSYNERGY_LIST_FIRST_ROW, range, 0);

      row++;
      entryIndex++;
    } while (rows > row);
  }

  if (menu->psynergyCount == 0) {
    Text_DrawMessageAsTilesFar(MsgNoPsynergy, window, 96,
                               STATUS_PSYNERGY_LIST_EMPTY_Y);
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

/// The Status menu's Psynergy page: the party member's Psynergy,
/// STATUS_LIST_PAGE_ROWS to a page, each with its PP cost and range icon; the
/// cursor's Psynergy is described in the info window with where it can be used.
s32 StatusScreen_RunPsynergyPage(void) {

  struct PsynergyMenuState *menu;
  s32 result;
  s32 done;
  s32 redraw;
  s32 listNeedsRedraw;
  s32 navigationResult;
  s32 memberIndex;
  s32 iconIndex;
  struct PagedList state;

  menu = gMenuState;
  result = 0;
  done = 0;
  Menu_BuildPatternTiles();
  WindowSprite_RedrawSavedRectFar(menu->messageWindow);
  Window_UpdateOrCreate(&menu->infoWindow, 0, 0, 30, 5, WINDOW_FLAG_INSTANT);
  // slotY is StatusScreenState's rowY: the four Djinn figures go above the
  // screen while this page is up, until the party member row or the status
  // page puts them back.
  for (iconIndex = 0; iconIndex < 4; iconIndex++)
    menu->slotY[iconIndex] = STATUS_FIGURE_ABOVE_SCREEN_Y;
  {
    s32 depth = OAM_DEPTH_WINDOW + 5;
    struct WindowSprite **icons = menu->entryIcons;

    for (iconIndex = 0; iconIndex < MENU_ENTRY_ICONS; iconIndex++) {
      struct WindowSprite *icon = *icons++;

      if (icon != 0)
        icon->depth = depth;
    }
  }
  Task_Remove(Menu_PlacePartyFigures);
  Menu_SpawnIconEntries((struct ItemMenuState *)menu, menu->messageWindow);
  Text_DrawMessageAsTilesFar(MsgSwitchCharacterHelp, menu->statusWindow,
                             SWITCH_HELP_X, SWITCH_HELP_Y);
  Text_DrawMessageAsTilesFar(MsgSwitchCharacterHelp + 2, menu->statusWindow, 0,
                             SWITCH_HELP_Y);

  while (done == 0 && GameFlag_GetFar(FLAG_MENU_START_PRESSED) == 0) {
    Menu_HideEntryIcons();
    WindowSprite_RedrawSavedRectFar(menu->statusWindow);
    menu->psynergyCount = PsynergyMenu_CollectActions(
        Unit_GetFar(menu->memberIds[0]), menu->psynergies, PSYNERGY_LIST_ALL);
    wait(1);
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
        wait(1);
      }
      wait(1);
      navigationResult = PagedList_HandleInput(
          0, state.entryCount, STATUS_LIST_PAGE_ROWS, &state.row, &state.page);
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
        Sound_PlayFar(SE_MENU_CONFIRM);
        result = 1;
        done = 1;
        break;
      }
      if (gKeysPressed & KEY_B) {
        Sound_PlayFar(SE_MENU_CANCEL);
        result = -1;
        done = 1;
        Task_Add(Menu_PlacePartyFigures, TASK_PASS_FRAME);
        break;
      }
      if ((gKeysRepeat & KEY_R) || (gKeysRepeat & KEY_L)) {
        Sound_PlayFar(SE_MENU_CURSOR_MOVE);
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
        Menu_ReloadMemberPortrait(menu, menu->memberTable[memberIndex]);
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
/// in the state (psynergies) into them. memberId is only looked up.
void PsynergyMenu_DrawPreparedPsynergyIcons(const void *unused, s32 memberId) {
  struct PsynergyMenuState *menu = gMenuState;

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
s32 ItemMenu_DrawEquipPage(struct Window *window, s32 unused,
                           struct PagedList *state) {
  struct ItemMenuState *menu;
  struct ItemDefinition *item;
  s32 firstEntry;
  s32 rowIndex;

  menu = gMenuState;
  firstEntry = state->page * STATUS_LIST_PAGE_ROWS;
  state->selectedIndex = firstEntry + state->row;
#if TBS_PATCH_1 && !TBS_PATCH_4
  WindowSprite_RedrawSavedRectFar(menu->descriptionWindow);
#elif TBS_PATCH_4
  Window_ClearInteriorTilesFar(menu->descriptionWindow, 0, 0, 224, 8);
#else
  WindowSprite_ClearListFar(menu->descriptionWindow);
#endif
  wait(1);
  if (menu->items[state->selectedIndex] != 0) {
    MENU_DRAW_DESCRIPTION(
        (menu->items[state->selectedIndex] & INVENTORY_ITEM_ID_MASK) +
            MsgItemPlainName,
        menu->descriptionWindow, 0, 0);
  }

  // An item with an element shows its icon at tile column 27, in palette 0 on
  // the highlighted row and palette 1 (TILEMAP_ENTRY_ICON) elsewhere, and the
  // highlight stops short of it.
  for (rowIndex = 0; rowIndex < STATUS_LIST_PAGE_ROWS; rowIndex++) {
    if (rowIndex == state->row) {
      item = Item_GetDirectFar(menu->items[state->selectedIndex] &
                               INVENTORY_ITEM_ID_MASK);
      if (item->element != ELEMENT_NONE) {
        Window_SetTilemapEntryFar(window, item->element + 1, 27,
                                  rowIndex * 2 + 1, TILEMAP_ENTRY_PLAIN);
        Window_SetHighlightRect(window, 14, rowIndex * 2 + 1, 13, 1,
                                HIGHLIGHT_PALETTE);
      } else {
        Window_SetHighlightRect(window, 14, rowIndex * 2 + 1, 14, 1,
                                HIGHLIGHT_PALETTE);
      }
    } else {
      item = Item_GetDirectFar(menu->items[firstEntry + rowIndex] &
                               INVENTORY_ITEM_ID_MASK);
      if (item->element != ELEMENT_NONE) {
        Window_SetTilemapEntryFar(window, item->element + 1, 27,
                                  rowIndex * 2 + 1, TILEMAP_ENTRY_ICON);
        Window_SetHighlightRect(window, 14, rowIndex * 2 + 1, 13, 1,
                                WINDOW_PALETTE);
      } else {
        Window_SetHighlightRect(window, 14, rowIndex * 2 + 1, 14, 1,
                                WINDOW_PALETTE);
      }
    }
  }

  for (rowIndex = 0; rowIndex <= 3; rowIndex++) {
    Window_SetHighlightRect(menu->equipWindow, STATUS_EQUIP_SLOT_X,
                            rowIndex * 2 + 1, STATUS_EQUIP_SLOT_WIDTH, 1,
                            WINDOW_PALETTE);
  }

  // The slots' name rows: weapon 1, head 3, shield 5, chest 7, each under
  // its label (ItemMenu_DrawEquipment). Other worn items have no row.
  if (menu->items[state->selectedIndex] & INVENTORY_EQUIPPED) {
    item = Item_GetDirectFar(menu->items[state->selectedIndex] &
                             INVENTORY_ITEM_ID_MASK);
    switch (item->type) {
    case ITEM_TYPE_WEAPON:
      Window_SetHighlightRect(menu->equipWindow, STATUS_EQUIP_SLOT_X, 1,
                              STATUS_EQUIP_SLOT_WIDTH, 1, HIGHLIGHT_PALETTE);
      break;
    case ITEM_TYPE_HELM:
      Window_SetHighlightRect(menu->equipWindow, STATUS_EQUIP_SLOT_X, 3,
                              STATUS_EQUIP_SLOT_WIDTH, 1, HIGHLIGHT_PALETTE);
      break;
    case ITEM_TYPE_SHIELD:
      Window_SetHighlightRect(menu->equipWindow, STATUS_EQUIP_SLOT_X, 5,
                              STATUS_EQUIP_SLOT_WIDTH, 1, HIGHLIGHT_PALETTE);
      break;
    case ITEM_TYPE_ARMOR:
      Window_SetHighlightRect(menu->equipWindow, STATUS_EQUIP_SLOT_X, 7,
                              STATUS_EQUIP_SLOT_WIDTH, 1, HIGHLIGHT_PALETTE);
      break;
    }
  }
  wait(1);
  return 1;
}

/// Draws one page of the Status menu's equipment page into window: up to
/// STATUS_LIST_PAGE_ROWS item names with their icons and the page numbers, or
/// "no items", and the party member's name with their Attack and Defense.
/// Returns 1.
s32 ItemMenu_DrawItemPage(struct Window *window, s32 unused,
                          struct PagedList *state) {
  struct ItemMenuState *menu = gMenuState;
  struct Unit *unit;
  u16 *inventoryEntry;
  s32 firstEntry;
  u32 rowCount;
  u32 rowIndex;
  s32 message;

  unit = Unit_GetFar(menu->sideMember[0]);
  Window_ClearInteriorTilesFar(window, 128, 8, 224, 96);
  firstEntry = state->page * STATUS_LIST_PAGE_ROWS;
  rowCount = (u8)(state->entryCount - firstEntry);
  if (rowCount > STATUS_LIST_PAGE_ROWS)
    rowCount = STATUS_LIST_PAGE_ROWS;
  Menu_SetPageIcons(STATUS_LIST_PAGE_ROWS, firstEntry, window,
                    STATUS_ITEM_PAGE_ICONS_X, 52);
  PagedList_DrawPageNumbers(window, state->entryCount, STATUS_LIST_PAGE_ROWS,
                            state->page, 28);
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
  Text_DrawNumberInWindowFar(unit->attack, 3, window, STATUS_ITEM_STAT_X, 16);
  Text_DrawNumberInWindowFar(unit->defense, 3, window, STATUS_ITEM_STAT_X, 24);
  return 1;
}

/// The Status menu's equipment page: the party member's items in the first list
/// order (LIST_SORT_EQUIPPED_THEN_ITEMS), STATUS_LIST_PAGE_ROWS to a page,
/// beside the equipment window of what they wear.
s32 StatusScreen_RunEquipmentPage(void) {

  struct ItemMenuState *menu;
  struct Unit *member;
  s32 result;
  s32 done;
  s32 redraw;
  s32 listNeedsRedraw;
  s32 navigationResult;
  s32 memberIndex;
  s32 iconIndex;
  u16 savedInventory[INVENTORY_SLOTS];
  struct PagedList state;

  menu = gMenuState;
  {
    s32 memberId = menu->sideMember[0];

    result = 0;
    Unit_GetFar(menu->memberIds[memberId]);
  }
  Window_UpdateOrCreate(&menu->equipWindow, 0, 10, STATUS_EQUIP_WINDOW_WIDTH,
                        10, WINDOW_FLAG_INSTANT);
  Menu_HideEntryIcons();

  for (iconIndex = 0; iconIndex < MENU_ENTRY_ICONS; iconIndex++) {
    struct WindowSprite *icon = menu->entryIcons[iconIndex];

    if (icon != 0)
      icon->depth = OAM_DEPTH_WINDOW;
  }

  Task_Remove(Menu_PlacePartyFigures);
  Text_DrawMessageAsTilesFar(MsgSwitchCharacterHelp, menu->statusWindow, 64,
                             SWITCH_HELP_Y);
  Text_DrawMessageAsTilesFar(MsgSwitchCharacterHelp + 3, menu->statusWindow, 0,
                             SWITCH_HELP_Y);
  Menu_HideEntryIcons();
  wait(1);
  Menu_SpawnIconEntries((struct ItemMenuState *)menu, menu->questionWindow);

  done = 0;
  while (done == 0 && GameFlag_GetFar(FLAG_MENU_START_PRESSED) == 0) {
    Menu_HideEntryIcons();
    WindowSprite_RedrawSavedRectFar(menu->statusWindow);
    member = Unit_GetFar(menu->sideMember[0]);
    for (iconIndex = 0; iconIndex < INVENTORY_SLOTS; iconIndex++)
      savedInventory[iconIndex] = member->inventory[iconIndex];
    menu->itemCount = ItemMenu_CollectItems(member, menu->items, 0);
    ItemMenu_Sort(menu->items, LIST_SORT_EQUIPPED_THEN_ITEMS);
    PagedList_Build(&state, 0);
    WindowSprite_RedrawSavedRectFar(menu->equipWindow);
    ItemMenu_DrawEquipment(menu->equipWindow, menu->sideMember[0], 1);
    wait(1);
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
      wait(1);

      navigationResult = PagedList_HandleInput(
          0, state.entryCount, STATUS_LIST_PAGE_ROWS, &state.row, &state.page);
      if (navigationResult == 1) {
        listNeedsRedraw = 1;
        redraw = 1;
      }
      if (navigationResult == 0)
        redraw = 1;
      if (navigationResult == -1)
        redraw = 0;

      if (gKeysPressed & KEY_A) {
        Sound_PlayFar(SE_MENU_CONFIRM);
        result = 1;
        done = 1;
        break;
      }
      if (gKeysPressed & KEY_B) {
        Sound_PlayFar(SE_MENU_CANCEL);
        result = -1;
        done = 1;
        break;
      }
      if ((gKeysRepeat & KEY_R) || (gKeysRepeat & KEY_L)) {
        Sound_PlayFar(SE_MENU_CURSOR_MOVE);
        memberIndex = menu->sideIndex[0];
        menu->selectedIndexByMember[menu->memberIds[memberIndex]] =
            state.selectedIndex;
        if (gKeysRepeat & KEY_R)
          memberIndex++;
        else
          memberIndex--;
        // Copy back the inventory saved as the list was built; only the list
        // was sorted, so this writes what is already there.
        for (iconIndex = 0; iconIndex < INVENTORY_SLOTS; iconIndex++)
          member->inventory[iconIndex] = savedInventory[iconIndex];
        memberIndex = (memberIndex + menu->partyCount) % menu->partyCount;
        menu->selectedMember = menu->memberIds[memberIndex];
        menu->sideMember[0] = menu->memberIds[memberIndex];
        menu->sideIndex[0] = memberIndex;
        Menu_ReloadMemberPortrait(menu, menu->memberIds[memberIndex]);
        break;
      }
    }
  }

  WindowSprite_ClearListFar(menu->descriptionWindow);
  WindowSprite_RedrawSavedRectFar(menu->descriptionWindow);
  WindowSprite_ClearListFar(menu->questionWindow);
  ItemMenu_HideAllIcons();
  WindowSprite_RedrawSavedRectFar(menu->statusWindow);
  Task_Add(Menu_PlacePartyFigures, TASK_PASS_FRAME);
  return result;
}

/// Moves through the status grid's irregular rows and returns row * 9 + column.
/// Nothing calls it.
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

/// The worn items' rows in ItemMenu_DrawEquipment's window, 16 pixels apart:
/// weapon, helm, shield and armor from the top. A slot's label stands on its
/// row, its item's name a line below, and its icon at EQUIPPED_ICON_X on
/// screen.
#define EQUIPPED_ROW_Y(row) ((row) * 16)
#define EQUIPPED_ICON_X 216
#define EQUIPPED_ICON_Y(row) (EQUIPPED_ROW_Y(row) + 32)

/// Writes the four equipment slot labels (Weapon, Head, Shield and Chest, one a
/// 16-pixel row) and the names of the items worn in them into window; mode 0
/// also shows their icons at the right.
void ItemMenu_DrawEquipment(struct Window *window, s32 memberId, s32 mode) {
  struct ItemMenuState *menu = gMenuState;
  u16 *items;

  Unit_GetFar(memberId);
  Menu_HideEntryIcons();
  ItemMenu_HideAllIcons();
  Text_DrawMessageAsTilesFar(MsgEquipSlotLabels, window, 0, 0);
  Text_DrawMessageAsTilesFar(MsgEquipSlotLabels + 1, window, 0,
                             EQUIPPED_ROW_Y(2));
  Text_DrawMessageAsTilesFar(MsgEquipSlotLabels + 2, window, 0,
                             EQUIPPED_ROW_Y(1));
  Text_DrawMessageAsTilesFar(MsgEquipSlotLabels + 3, window, 0,
                             EQUIPPED_ROW_Y(3));
  items = menu->items;
  ItemMenu_DrawEquippedItemNames(window, items);
  if (mode == 0) {
    wait(1);
    ItemMenu_DrawIcons(items, 1);
    ItemMenu_ArrangeCategoryItemIcons(items);
  }
}

/// Writes the name of each worn weapon, armor, shield and helm under its slot
/// label (ItemMenu_DrawEquipment). Other worn items are not listed.
void ItemMenu_DrawEquippedItemNames(struct Window *window, u16 *items) {
  s32 slotIndex;
  s32 itemId;

  for (slotIndex = 0; slotIndex < INVENTORY_SLOTS; slotIndex++) {
    if (items[slotIndex] & INVENTORY_EQUIPPED) {
      itemId = items[slotIndex] & INVENTORY_ITEM_ID_MASK;
      switch (Item_GetDirectFar(itemId)->type) {
      case ITEM_TYPE_WEAPON:
        Text_DrawMessageAsTilesFar(itemId + MsgItemName, window,
                                   STATUS_EQUIPPED_NAME_X,
                                   EQUIPPED_ROW_Y(0) + 8);
        break;
      case ITEM_TYPE_ARMOR:
        Text_DrawMessageAsTilesFar(itemId + MsgItemName, window,
                                   STATUS_EQUIPPED_NAME_X,
                                   EQUIPPED_ROW_Y(3) + 8);
        break;
      case ITEM_TYPE_SHIELD:
        Text_DrawMessageAsTilesFar(itemId + MsgItemName, window,
                                   STATUS_EQUIPPED_NAME_X,
                                   EQUIPPED_ROW_Y(2) + 8);
        break;
      case ITEM_TYPE_HELM:
        Text_DrawMessageAsTilesFar(itemId + MsgItemName, window,
                                   STATUS_EQUIPPED_NAME_X,
                                   EQUIPPED_ROW_Y(1) + 8);
        break;
      }
    }
  }
}

/// Places the 32 entry icons in a grid of 16x16 cells, columns wide, from
/// (originX, originY), and shows them: the party member chooser's Psynergy
/// icons.
void Menu_PlaceEntryActorsInGrid(s32 originX, s32 originY, s32 columns) {
  s32 entryIndex;
  struct WindowSprite *icon;
  struct WindowSprite **icons;

  for (entryIndex = 0, icons = ((struct ItemMenuState *)gMenuState)->entryIcons;
       entryIndex <= MENU_ENTRY_ICONS - 1; entryIndex++) {
    icon = *icons++;
    if (icon != NULL) {
      Menu_PlaceEntryActorInGrid(icon, entryIndex, originX, originY, columns);
    }
  }
}

/// The side of a cell of the grids Menu_PlaceEntryActorInGrid places icons in.
#define ENTRY_ICON_CELL_SIZE 16

/// Places icon at cell index (an index above 31 is taken as 0) of a grid of
/// ENTRY_ICON_CELL_SIZE square cells, columns wide, from (originX, originY),
/// and shows it.
void Menu_PlaceEntryActorInGrid(struct WindowSprite *icon, s32 index,
                                s32 originX, s32 originY, s32 columns) {
  s32 entryIndex;

  entryIndex = index;
  if (entryIndex > MENU_ENTRY_ICONS - 1) {
    entryIndex = 0;
  }
  icon->y = (entryIndex / columns * ENTRY_ICON_CELL_SIZE) + originY;
  icon->x = (entryIndex % columns * ENTRY_ICON_CELL_SIZE) + originX;
  Icon_Show(icon);
}

/// Places the icon of each worn weapon, armor, shield and helm at
/// EQUIPPED_ICON_X, on its slot's row; the others stay off screen.
void ItemMenu_ArrangeCategoryItemIcons(u16 *items) {
  struct ItemMenuState *menu;
  struct WindowSprite *icon;
  s32 slotIndex;

  menu = gMenuState;
  Menu_HideEntryIcons();
  for (slotIndex = 0; slotIndex < INVENTORY_SLOTS; slotIndex++) {
    if (items[slotIndex] != 0 && (items[slotIndex] & INVENTORY_EQUIPPED) != 0) {
      icon = menu->entryIcons[slotIndex];
      if (icon != 0) {
        switch (Item_GetDirectFar(items[slotIndex] & INVENTORY_ITEM_ID_MASK)
                    ->type) {
        case ITEM_TYPE_WEAPON:
          icon->x = EQUIPPED_ICON_X;
          icon->y = EQUIPPED_ICON_Y(0);
          break;
        case ITEM_TYPE_ARMOR:
          icon->x = EQUIPPED_ICON_X;
          icon->y = EQUIPPED_ICON_Y(3);
          break;
        case ITEM_TYPE_SHIELD:
          icon->x = EQUIPPED_ICON_X;
          icon->y = EQUIPPED_ICON_Y(2);
          break;
        case ITEM_TYPE_HELM:
          icon->x = EQUIPPED_ICON_X;
          icon->y = EQUIPPED_ICON_Y(1);
          break;
        }
        Icon_Show(icon);
      }
    }
  }
}

/// Moves all 32 entry icons off screen, to MENU_ICON_HIDDEN_X and _Y, before a
/// screen lays them out again.
void Menu_HideEntryIcons(void) {
  struct ItemMenuState *menu = gMenuState;
  s32 x = MENU_ICON_HIDDEN_X;
  struct WindowSprite **entry = menu->entryIcons;
  s32 y = MENU_ICON_HIDDEN_Y;
  s32 i;

  for (i = 0; i < MENU_ENTRY_ICONS; i++) {
    struct WindowSprite *object = *entry++;

    if (object != 0) {
      object->x = x;
      object->y = y;
      Icon_Show(object);
    }
  }
}

/// Creates the eight category sprites in window, off screen at
/// MENU_ICON_HIDDEN_X and _Y, each on item graphic slot of the same number; the
/// Status menu shows the status icons with the first five
/// (StatusScreen_UpdateSelectionIcons).
s32 Menu_CreateEightEntryActors(struct Window *window) {
  struct WindowSprite **slot;
  struct WindowSprite *icon;
  s32 slotIndex;
  struct ItemMenuState *menu;

  menu = gMenuState;
  slotIndex = 0;
  {
    s32 y = MENU_ICON_HIDDEN_Y;

    slot = menu->categoryIcons;
    do {
      icon = WindowSprite_CreateFromResourceFar(ICON_KIND_ITEM, slotIndex,
                                                window, MENU_ICON_HIDDEN_X, y);
      slotIndex += 1;
      *slot = icon;
      slot += 1;
    } while (slotIndex <= 7);
    return 1;
  }
}

/// Shows the icon of each status that flags marks
/// (StatusScreen_ListStatuses) down the left edge from y 88, 16 pixels
/// apart, beside the status labels of StatusScreen_DrawStatusAilments.
void StatusScreen_ShowStatusIcons(const u8 *flags) {
  struct ItemMenuState *menu;
  struct WindowSprite **slot;
  struct WindowSprite *entry;
  s32 index;
  s32 shown;

  menu = gMenuState;
  StatusScreen_HideStatusIcons();
  for (index = 0, slot = menu->categoryIcons, shown = 0; index <= 4; index++) {
    entry = *slot++;
    if (entry != 0 && flags[index] != 0) {
      entry->x = 8;
      entry->y = shown * 16 + 88;
      entry->depth = OAM_DEPTH_WINDOW;
      Icon_Show(entry);
      shown++;
    }
  }
}

/// Moves the five status icons (the first category sprites) off screen,
/// before they are shown again or while another view covers them.
void StatusScreen_HideStatusIcons(void) {
  struct ItemMenuState *menu = gMenuState;
  s32 index;

  for (index = 0; index < 5; index++) {
    struct WindowSprite *object = menu->categoryIcons[index];

    if (object != 0) {
      object->x = MENU_ICON_HIDDEN_X;
      object->y = MENU_ICON_HIDDEN_Y;
      object->depth = OAM_DEPTH_WINDOW;
      Icon_Show(object);
    }
  }
}

/// Loads each enabled status's icon into its category resource slot: kind is
/// the icon's number in gIcon_MiscIconPointers, which the battle Status screen
/// shows for the same status.
s32 StatusScreen_UpdateSelectionIcons(const u8 *enabled) {
  struct ItemMenuState *menu = gMenuState;
  s32 index;

  for (index = 0; index <= 4; index++) {
    if (enabled[index] != 0) {
      s32 kind;
      switch (index) {
      case CHARACTER_DOWN:
        kind = OWNER_CONDITION_DOWNED;
        break;
      case CHARACTER_POISON:
        kind = OWNER_CONDITION_POISON;
        break;
      case CHARACTER_VENOM:
        kind = OWNER_CONDITION_DEADLY_POISON;
        break;
      case CHARACTER_CURSE:
        kind = OWNER_CONDITION_CURSED_ITEM;
        break;
      case CHARACTER_HAUNT:
        kind = OWNER_CONDITION_HAUNT;
        break;
      default:
        kind = OWNER_CONDITION_NORMAL;
        break;
      }
      Resource_LoadByModeIntoSlotFar(ICON_KIND_PATTERN, kind,
                                     (u8)menu->categoryIcons[index]->index, 0);
    }
  }
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
#undef SWITCH_HELP_Y
#undef STATUS_AILMENT_Y
#undef STATUS_ELEMENT_X
#undef EQUIPPED_ICON_X
#undef EQUIPPED_ICON_Y
#undef EQUIPPED_ROW_Y
#undef STAT_LABEL
