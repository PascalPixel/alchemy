/// @file
/// The party member chooser and the Status menu: its party member row and its
/// status, Psynergy, range and equipment pages.
// Part of menu.c.

#include "menu_internal.h"
#include "sounds.h"

/// The steps of the Status menu (StatusScreen_RunPages).
enum StatusScreenStep {
  STATUS_STEP_MEMBER = 0,   ///< choose a party member
  STATUS_STEP_STATUS = 1,   ///< their status page
  STATUS_STEP_PSYNERGY = 2, ///< their Psynergy page
  STATUS_STEP_EQUIPMENT = 3 ///< their equipment page
};

/// The page modes of StatusScreen_DrawStatusAilments and
/// ItemMenu_DrawMemberStatus, in the mode's low byte: the party row's summary,
/// or the full status page, with the elemental levels, Power and Resist.
enum { STATUS_PAGE_SUMMARY = 0, STATUS_PAGE_FULL = 1 };
/// A bit over the page mode: draw without clearing the window first, as for a
/// window just opened.
#define STATUS_PAGE_NO_CLEAR (1 << 8)
#define STATUS_PAGE_MODE_MASK (STATUS_PAGE_NO_CLEAR - 1)

/// The worn items' rows in ItemMenu_DrawCategory's window, 16 pixels apart:
/// weapon, helm, shield and armor from the top. A slot's label stands on its
/// row, its item's name a line below, and its icon at EQUIPPED_ICON_X on
/// screen.
#define EQUIPPED_ROW_Y(row) ((row) * 16)
#define EQUIPPED_ICON_X 216
#define EQUIPPED_ICON_Y(row) (EQUIPPED_ROW_Y(row) + 32)

/// The Djinn figures stand in a row 32 pixels apart, at y STATUS_FIGURE_ROW_Y,
/// over the status page's Djinn counts. On the full status page those stand
/// three tiles higher, and the figures at STATUS_FIGURE_FULL_PAGE_ROW_Y; the
/// pages after it put them above the screen.
#define STATUS_FIGURE_X(index) (130 + (index) * 32)
#define STATUS_FIGURE_ROW_Y 128
#define STATUS_FIGURE_FULL_PAGE_ROW_Y 104
#define STATUS_FIGURE_HIDDEN_Y -16

/// The Djinn list (DjinnMenu_ShowCurrentList): a column for each element, its
/// Djinni figure standing over it.
#define DJINN_LIST_COLUMN_WIDTH 56
#define DJINN_LIST_FIGURE_X(element) (32 + (element) * DJINN_LIST_COLUMN_WIDTH)
#define DJINN_LIST_FIGURE_Y 56
/// The rows of each element's column on a page of the Djinn list.
#define DJINN_LIST_PAGE_ROWS 9
/// The window map entry of the first element icon, in palette 1 as
/// TILEMAP_ENTRY_ICON gives it; the four elements follow it in order.
#define STATUS_ELEMENT_TILE (BG_TILE_PALETTE(1) | 1)

/// The status page's four element columns, 4 tiles apart from tile 15, where
/// each element's icon stands; its figures end left of it.
#define STATUS_ELEMENT_X(element) (((element) * 4 + 15) * 8)

void StatusScreen_Initialize(s32 unusedA, s32 unusedB, s32 unusedC,
                             s32 unusedD);

s32 PartyMemberChooser_Run(void);

/// Lets the player choose one party member, over the field. Returns the chosen
/// party member's id, or -1 when the player backs out. Owns the menu state
/// (HEAP_SLOT_MENU) while it runs and frees it before returning.
s32 PartyMemberChooser_Open(void) {
  struct StatusScreenState *state = (struct StatusScreenState *)HeapSlot_Alloc(
      HEAP_SLOT_MENU, MENU_STATE_SIZE);
  s32 result;
  s32 index;

  Ram_HeapSlots->actorSystem->suspended = 1;
  Window_DrawBorderFar(0, 0, 30, 20);
  wait(1);
  Window_InitializeState(0);

  state->partyCount = Party_ListMembersFar(state->memberIds);
  StatusScreen_Initialize(0, 3, 0, 7);
  state->selectorWindow = Window_CreateFar(13, 0, 17, 5, WINDOW_FLAG_INSTANT);
  for (index = 0; index < 4; index++)
    state->memberY[index] = MENU_MEMBER_Y;
  state->page = STATUS_SCREEN_PAGE_CHOOSER;

  result = PartyMemberChooser_Run();

  WindowSprite_ClearListFar(state->statusWindow);
  ItemMenu_Close();
  Ram_HeapSlots->actorSystem->suspended = 0;
  wait(1);
  HeapSlot_Release(HEAP_SLOT_MENU);
  return result;
}

s32 StatusScreen_SelectMember(s32 slot);

/// The party member chooser's loop. Returns the chosen party member's id, or
/// -1.
s32 PartyMemberChooser_Run(void) {
  struct StatusScreenState *state = Ram_HeapSlots->menuState;
  s32 result = 0;

  do {
    state->selectedSlots[0] = 0;
    if (StatusScreen_SelectMember(0) == -1) {
      result = -1;
      break;
    }
    result = state->selectedIds[0];
  } while (0);
  return result;
}

s32 StatusScreen_RunPages(void);

void MenuIcon_LoadSlots(void);
void MenuIcon_ReleaseSlots(void);

/// The window tiles the Status menu saves and restores.
#define STATUS_SAVED_TILE_BYTES 10240

/// The Status screen, the field menu's Status, over the field: saves the
/// background palettes and the window tiles it draws over, sets up the party
/// row, the five command icons and, when the party has Djinn, the Djinn
/// figures, runs the status pages, then puts everything back.
s32 StatusScreen_Run(void) {
  struct StatusScreenState *state = (struct StatusScreenState *)HeapSlot_Alloc(
      HEAP_SLOT_MENU, MENU_STATE_SIZE);
  void *palette = Heap_AllocEwram(64);
  void *tiles = Heap_AllocEwram(STATUS_SAVED_TILE_BYTES);
  s32 result = -1;
  struct HeapSlots *heap;
  struct ScreenTransitionState *transition;
  u32 saved;
  u16 *page;

  heap = Ram_HeapSlots;
  transition = heap->screenTransition;
  heap->actorSystem->suspended = 1;
  Window_DrawBorderFar(0, 0, 30, 20);
  wait(1);
  saved = (u16)transition->active;
  // Window 0 shows only the windows' layer and the sprites.
  transition->active = WININ_WIN0_BG0 | WININ_WIN0_OBJ;
  Menu_SuspendFieldTasks();
  Window_InitializeState(0);
  page = &state->page;
  *page = 0;
  state->partyCount = Party_ListMembersFar(state->memberIds);
  StatusScreen_Initialize(0, 3, 0, 7);
  Iwram_CopyWords(palette, (void *)BG_PLTT, 64);
  Iwram_CopyWords(tiles, (void *)BG_CHAR_BLOCK(1), STATUS_SAVED_TILE_BYTES);
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
  Text_UseExtraTilesFar(1);
  state->selectorWindow = Window_CreateFar(13, 0, 17, 5, WINDOW_FLAG_INSTANT);
  if (Party_SumDjinnCountsFar(result) != 0)
    DjinnFigures_InitializeTopRow();
  Link_DrawPageArrowsFar(PAGE_ARROW_VRAM);
  MenuIcon_LoadSlots();
  Menu_StartWatchingStart();
  *page = 0;

  result = StatusScreen_RunPages();

  Menu_StopWatchingStart();
  MenuIcon_ReleaseSlots();
  WindowSprite_ClearListFar(state->statusWindow);
  DjinnFigures_ReleaseTopRow();
  Menu_ResumeFieldTasks();
  Window_DrawBorderFar(0, 0, 30, 20);
  wait(1);
  Window_MarkVisibleTextTilesFar();
  Text_UseExtraTilesFar(0);
  Iwram_CopyWords((void *)BG_PLTT, palette, 64);
  Iwram_CopyWords((void *)BG_CHAR_BLOCK(1), tiles, STATUS_SAVED_TILE_BYTES);
  Heap_Release(tiles);
  Heap_Release(palette);
  ((struct WindowState *)heap->windowState)->holdTilemapUpload = 1;
  ItemMenu_Close();
  Window_DrawBorderFar(0, 0, 30, 20);
  HeapSlot_Release(HEAP_SLOT_MENU);
  heap->actorSystem->suspended = 0;
  transition->active = saved;
  wait(1);
  Window_EraseBorderRectFar(0, 0, 30, 20);
  ((struct WindowState *)heap->windowState)->holdTilemapUpload = 0;
  return result;
}

void MenuIcon_SetActive(s32 active);
s32 StatusScreen_SelectCommand(void);
s32 PsynergyMenu_SelectAction(void);
s32 ItemMenu_SelectItem(void);

/// The Status menu's pages: choose a party member, then their status page, A
/// for their Psynergy page and A again for their equipment page, whose A goes
/// back round to the status page; B on any page goes back to the party member
/// row. Returns -1 when the player backs out of the party member row or presses
/// Start.
s32 StatusScreen_RunPages(void) {
  struct StatusScreenState *state =
      (struct StatusScreenState *)Ram_HeapSlots->menuState;
  s32 step = STATUS_STEP_MEMBER;
  s32 finished = step;
  s32 result = 0;

  while (!finished && !GameFlag_Get(FLAG_MENU_START_PRESSED)) {
    switch (step) {
    case STATUS_STEP_MEMBER:
      state->selectedSlots[0] = finished;
      Task_Add(Menu_UpdateEntryObjectTransforms, TASK_PASS_FRAME);
      MenuIcon_SetActive(1);
      step = StatusScreen_SelectMember(0);
      Task_Remove(Menu_UpdateEntryObjectTransforms);
      if (step == -1) {
        result = step;
        finished = 1;
      }
      step = STATUS_STEP_STATUS;
      break;
    case STATUS_STEP_STATUS:
      MenuIcon_SetActive(1);
      result = StatusScreen_SelectCommand();
      step = result == -1 ? STATUS_STEP_MEMBER : STATUS_STEP_PSYNERGY;
      break;
    case STATUS_STEP_PSYNERGY:
      MenuIcon_SetActive(0);
      result = PsynergyMenu_SelectAction();
      step = STATUS_STEP_MEMBER;
      if (result != -1)
        step = STATUS_STEP_EQUIPMENT;
      break;
    case STATUS_STEP_EQUIPMENT:
      MenuIcon_SetActive(0);
      result = ItemMenu_SelectItem();
      // STATUS_STEP_MEMBER for -1, STATUS_STEP_STATUS otherwise.
      step = NONZERO(~result);
      break;
    default:
      finished = 1;
      break;
    }
  }

  if (GameFlag_Get(FLAG_MENU_START_PRESSED))
    result = -1;

  return result;
}

s32 PsynergyMenu_ChooseMember(void);
s32 PartyMemberChooser_RunRearrange(void);

/// Lets the player choose a party member in slot, from the one chosen last: the
/// party member chooser on the chooser page, else the Status menu's
/// party row.
s32 StatusScreen_SelectMember(s32 slot) {
  struct StatusScreenState *menu;
  struct WindowSprite *cursor;
  s32 result;
  s32 index;

  menu = (struct StatusScreenState *)Ram_HeapSlots->menuState;
  index = menu->memberIndex[slot];
  result = 0;
  cursor = menu->memberCursors[slot];
  cursor->motion = ICON_SHOWN;
  cursor->animationStep = 0;
  WindowSprite_RedrawSavedRectFar(menu->memberWindow);
  if (GameFlag_Get(FLAG_LINK_BATTLE_ARRANGED))
    Window_DrawDividerLineFar(menu->memberWindow, 9, 1, 9, 3);
  Window_DrawDividerLineFar(menu->memberWindow, 0, 3, 12, 3);
  if (index == -1)
    menu->memberIndex[slot] = 0;
  else
    MenuCursor_Slide(index * 24 - 10, 16);
  if (menu->page == STATUS_SCREEN_PAGE_CHOOSER)
    result = PsynergyMenu_ChooseMember();
  else
    result = PartyMemberChooser_RunRearrange();
  cursor = menu->memberCursors[slot];
  Icon_PrepareActor(cursor);
  wait(1);
  return result;
}

/// The Djinn list Select opens from the party row: every Djinni the party has
/// found, a column for each element, nine a page over two pages, The Lost Age's
/// own first and The Broken Seal's after them, until A, B or Select.
void DjinnMenu_ShowCurrentList(void) {
  struct DjinnMenuState *menu = Ram_HeapSlots->menuState;
  s32 page;
  s32 redraw;
  s32 window;
  s32 element;
  s32 row;
  s32 djinn;
  s32 i;
  s32 pages;

  page = 0;
  pages = 2;
  Sound_PlayFar(SE_MENU_CONFIRM);
  redraw = 1;
  WindowSprite_RedrawSavedRectFar(menu->helpWindow);
  Text_DrawMessageAsTilesFar(MsgButtonReturn, menu->helpWindow, 0, 16);
  Text_DrawMessageFar(MsgCurrentDjinn, menu->helpWindow, -96, 132);
  menu->pageArrows[0]->motion = ICON_HIDDEN;
  menu->pageArrows[1]->motion = ICON_HIDDEN;
  MenuIcon_SetActive(0);
  menu->cursor->motion = ICON_HIDDEN;
  menu->secondCursor->motion = ICON_HIDDEN;
  for (i = 0; i < 4; i++) {
    menu->slotX[i] = DJINN_LIST_FIGURE_X(i);
    menu->slotY[i] = DJINN_LIST_FIGURE_Y;
    menu->memberY[i] = MENU_MEMBER_Y;
  }
  wait(1);
  while (!GameFlag_Get(FLAG_MENU_START_PRESSED)) {
    if (redraw) {
      window = menu->window;
      WindowSprite_RedrawSavedRectFar(window);
      wait(1);
      Window_DrawDividerLineFar(window, 0, 11, 28, 11);
      PagedList_DrawPageNumbers(window, pages, 1, page, 28);
      for (element = 0; element < 4; element++) {
        for (row = 0; row < DJINN_LIST_PAGE_ROWS; row++) {
          // page * 9: a page of DJINN_LIST_PAGE_ROWS.
          djinn =
              element * DJINN_PER_ELEMENT +
              (row + (page * 8 + page) + DJINN_OWN_FIRST) % DJINNI_SPARE_FIRST;
          if (GameFlag_Get(FLAG_DJINNI(djinn))) {
            Window_SetTilemapEntryFar(
                window, STATUS_ELEMENT_TILE + element,
                element * (DJINN_LIST_COLUMN_WIDTH / 8) + 1, row + 1, 0);
            Text_DrawMessageAsTilesFar(djinn + MsgDjinnName, window,
                                       element * DJINN_LIST_COLUMN_WIDTH + 16,
                                       row * 8 + 8);
          }
        }
      }
      ((struct WindowState *)Ram_HeapSlots->windowState)->dirty = 1;
      redraw = 0;
    }
    Link_DrawPageArrowsFar(PAGE_ARROW_VRAM);
    wait(1);
    if (gInput.pressed & (KEY_A | KEY_B | KEY_SELECT))
      break;
    if (gInput.repeat & KEYS_HORIZONTAL) {
      if (gInput.repeat & KEY_RIGHT)
        page++;
      else
        page--;
      page = (page + pages) % pages;
      Sound_PlayFar(SE_MENU_CURSOR_MOVE);
      Input_ResetKeyRepeat();
      redraw = 1;
    }
  }
  WindowSprite_RedrawSavedRectFar(menu->window);
  WindowSprite_ClearListFar(menu->helpWindow);
  for (i = 0; i < 4; i++) {
    menu->slotX[i] = STATUS_FIGURE_X(i);
    menu->slotY[i] = STATUS_FIGURE_ROW_Y;
  }
  menu->cursor->motion = ICON_SHOWN;
  menu->secondCursor->motion = ICON_SHOWN;
  MenuIcon_SetActive(1);
  Sound_PlayFar(SE_MENU_CANCEL);
}

void StatusScreen_ShowMemberStatus(s32 resource, s32 frame);

s32 PartyMemberChooser_MoveEntry(s32 selection, s32 direction);

/// The Status menu's party row: Left and Right choose a party member, showing
/// their status; L and R move the party member one place along the party order;
/// Select lists the Djinn found (once the first is caught, FLAG_DJINN_FIRST); A
/// chooses the party member and B backs out.
s32 PartyMemberChooser_RunRearrange(void) {
  struct StatusScreenState *menu;
  s32 ownerIndex;
  s32 ownerCount;
  s32 page;
  s32 redraw;
  s32 result;
  s32 rowIndex;

  menu = Ram_HeapSlots->menuState;
  ownerIndex = menu->memberIndex[0];
  ownerCount = menu->memberCount[0];
  redraw = 2;
  result = 0;
  page = menu->page;
  // DEAD: Unit_Get's result is not used; the ROM keeps the call.
  Unit_Get(menu->memberIds[ownerIndex]);
  for (rowIndex = 0; rowIndex < 4; rowIndex++) {
    menu->rowX[rowIndex] = STATUS_FIGURE_X(rowIndex);
    menu->rowY[rowIndex] = STATUS_FIGURE_ROW_Y;
  }
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
  while (!GameFlag_Get(FLAG_MENU_START_PRESSED)) {
    if (redraw) {
      WindowSprite_RedrawSavedRectFar(menu->selectorWindow);
      Text_DrawMessageAsTilesFar(MsgRearrangeHelp, menu->selectorWindow, 0, 0);
      // The Djinn list opens once Echo, the first Djinni the party meets, has
      // joined.
      if (GameFlag_Get(FLAG_DJINNI(DJINNI_ECHO)))
        Text_DrawMessageAsTilesFar(MsgDjinnListHelp, menu->selectorWindow, 0,
                                   16);
      Text_DrawMessageAsTilesFar(MsgRearrangeHelp - 3, menu->selectorWindow, 0,
                                 8);
      ownerIndex = (ownerIndex + ownerCount) % ownerCount;
      // DEAD: Unit_Get's result is not used; the ROM keeps the call.
      Unit_Get(menu->memberIds[ownerIndex]);
      page = (page + 3) % 3;
      StatusScreen_ShowMemberStatus(menu->memberIds[ownerIndex], page);
      Menu_ReloadMemberPortrait(menu, menu->memberIds[ownerIndex]);
      if (redraw == 2) {
        Menu_CreateOwnerEntryObjects(ownerIndex / 4 * 4);
        wait(1);
      }
      Menu_MarkChosenMember(menu->memberWindow, ownerIndex, ownerCount);
      redraw = 0;
    }
    MenuCursor_Position((ownerIndex % 4) * 24 - 10, 16);
    wait(1);
    if (gInput.pressed & KEY_A) {
      Sound_PlayFar(SE_MENU_CONFIRM);
      result = 1;
      break;
    }
    if (gInput.pressed & KEY_B) {
      Sound_PlayFar(SE_MENU_CANCEL);
      result = -1;
      break;
    }
    if (gInput.repeat & KEY_R) {
      if (PartyMemberChooser_MoveEntry(ownerIndex, 1)) {
        Sound_PlayFar(SE_MENU_CONFIRM);
        ownerIndex++;
        Menu_CreateOwnerEntryObjects(ownerIndex / 4 * 4);
        Menu_MarkChosenMember(menu->memberWindow, ownerIndex, ownerCount);
      } else {
        Sound_PlayFar(SE_MENU_ERROR);
      }
      wait(1);
    } else if (gInput.repeat & KEY_L) {
      if (PartyMemberChooser_MoveEntry(ownerIndex, 0)) {
        Sound_PlayFar(SE_MENU_CONFIRM);
        ownerIndex--;
        Menu_CreateOwnerEntryObjects(ownerIndex / 4 * 4);
        Menu_MarkChosenMember(menu->memberWindow, ownerIndex, ownerCount);
      } else {
        Sound_PlayFar(SE_MENU_ERROR);
      }
      wait(1);
    } else if ((gInput.pressed & KEY_SELECT) &&
               GameFlag_Get(FLAG_DJINNI(DJINNI_ECHO))) {
      DjinnMenu_ShowCurrentList();
      redraw = 1;
    } else {
      redraw = Menu_MoveGridCursor(&ownerIndex, ownerCount, 4);
    }
  }
  menu->memberIndex[0] = ownerIndex;
  menu->selectedMember = menu->memberIds[ownerIndex];
  menu->selectedIds[0] = menu->memberIds[ownerIndex];
  menu->pageArrows[0]->motion = ICON_HIDDEN;
  menu->pageArrows[1]->motion = ICON_HIDDEN;
  return result;
}

void Menu_PlaceEntryActorsInGrid(s32 x, s32 y, s32 columns);

/// The party member chooser (menu page 3): Left and Right choose a party
/// member, showing their status and Psynergy icons; A chooses and B backs out.
s32 PsynergyMenu_ChooseMember(void) {
  struct StatusScreenState *menu =
      (struct StatusScreenState *)Ram_HeapSlots->menuState;
  s32 selection = menu->memberIndex[0];
  s32 count = menu->memberCount[0];
  s32 pending = 2;
  s32 page = menu->page;
  s32 result;

  // DEAD: Unit_Get's result is not used; the ROM keeps the call.
  Unit_Get(menu->memberIds[selection]);
  WindowSprite_RedrawSavedRectFar(menu->selectorWindow);
  Text_DrawMessageAsTilesFar((s32)MsgChooseCharacter, menu->selectorWindow, 0,
                             0);
  Text_DrawMessageAsTilesFar((s32)MsgChooseCharacter + 1, menu->selectorWindow,
                             0, 16);
  while (1) {
    if (pending) {
      selection = (selection + count) % count;
      // DEAD: Unit_Get's result is not used; the ROM keeps the call.
      Unit_Get(menu->memberIds[selection]);
      page = (page + 3) % 3;
      StatusScreen_ShowMemberStatus(menu->memberIds[selection], page);
      Menu_ReloadMemberPortrait(menu, menu->memberIds[selection]);
      menu->entryCount =
          PsynergyMenu_CollectActions(Unit_Get(menu->memberIds[selection]),
                                      menu->entries, PSYNERGY_LIST_ALL);
      PsynergyMenu_DrawPsynergyIcons(menu->entries);
      Menu_PlaceEntryActorsInGrid(96, 96, 8);
      Menu_HideEmptyEntryIcons(menu->entries);
      if (pending == 2) {
        Menu_CreateOwnerEntryObjects(selection / 4 * 4);
        wait(1);
      }
      Menu_MarkChosenMember(menu->memberWindow, selection, count);
    }
    MenuCursor_Position((selection % 4) * 24 - 10, 16);
    wait(1);
    if (gInput.pressed & KEY_A) {
      Sound_PlayFar(SE_MENU_CONFIRM);
      result = 1;
      break;
    }
    if (gInput.pressed & KEY_B) {
      Sound_PlayFar(SE_MENU_CANCEL);
      result = -1;
      break;
    }
    pending = Menu_MoveGridCursor(&selection, count, 4);
  }
  menu->memberIndex[0] = selection;
  menu->selectedMember = menu->memberIds[selection];
  menu->selectedIds[0] = menu->memberIds[selection];
  return result;
}

/// Moves the party member at selectedIndex one place later (direction 1) or
/// earlier (0) in the party order. Returns 1, or 0 when it cannot move.
s32 PartyMemberChooser_MoveEntry(s32 selectedIndex, s32 direction) {
  struct StatusScreenState *menu = Ram_HeapSlots->menuState;
  u32 reordered[14];
  s32 index;

  if (menu->partyCount <= 1)
    return 0;
  if (direction == 1) {
    if (selectedIndex == menu->partyCount - 1)
      return 0;
  } else {
    if (selectedIndex == 0)
      return 0;
  }

  for (index = 0; index < (s32)ARRAY_COUNT(reordered); index++)
    reordered[index] = 0;
  for (index = 0; index < *(u8 *)&menu->partyCount; index++)
    reordered[index] = menu->memberIds[index];

  if (direction == 1) {
    index = reordered[selectedIndex];
    reordered[selectedIndex] = reordered[selectedIndex + 1];
    reordered[selectedIndex + 1] = index;
  } else {
    index = reordered[selectedIndex];
    reordered[selectedIndex] = reordered[selectedIndex - 1];
    reordered[selectedIndex - 1] = index;
  }

  for (index = 0; index < menu->partyCount; index++)
    Party_RemoveMemberFar(menu->memberIds[index]);
  for (index = 0; index < menu->partyCount; index++)
    Party_AddMemberFar(reordered[index]);
  menu->partyCount = Party_ListMembersFar(menu->memberIds);
  return 1;
}

void Menu_PlaceOwnerEntryObjects(struct Window *window, s32 originX,
                                 s32 originY);

/// Sets up the party row of the Status menu and the party member chooser: the
/// top-left window with the hand cursor and the party's figures, no other
/// window yet, and an 8 by 2 cursor grid. Its callers pass four values it does
/// not read.
void StatusScreen_Initialize(s32 unusedA, s32 unusedB, s32 unusedC,
                             s32 unusedD) {
  struct StatusScreenState *state = Ram_HeapSlots->menuState;

  Menu_PlaceOwnerEntryObjects(MenuCursor_Create(state), 2, 2);
  state->unopenedWindow = 0;
  state->statusWindow = 0;
  state->helpWindow = 0;
  state->actionWindow = 0;
  state->selectedColumn = 0;
  state->selectedRow = 0;
  state->columnCount = 8;
  state->rowCount = 2;
}

void StatusScreen_DrawStatusAilments(s32 window, s32 ownerId, s32 mode);

/// Shows party member resource's status in the party row's status window,
/// opening it the first time with the party member's portrait (and, for the
/// party member chooser, the Psynergy icons). frame is not used.
void StatusScreen_ShowMemberStatus(s32 resource, s32 frame) {
  s32 created;
  s32 handle;
  void *object;
  struct StatusScreenState *state;

  state = Ram_HeapSlots->menuState;
  created = 0;
  handle = state->statusWindow;
  if (handle == 0) {
    created = Window_UpdateOrCreate(&state->statusWindow, 0, 5, 30, 15,
                                    WINDOW_FLAG_INSTANT);
    handle = state->statusWindow;
  }
  if (created != 0) {
    object = Portrait_CreateFar(resource, 0, 0, handle, 0, 0);
    state->statusCursor = object;
    if ((((struct WindowSprite *)object)->depth = OAM_DEPTH_WINDOW,
         state->page) == STATUS_SCREEN_PAGE_CHOOSER) {
      Menu_SpawnIconEntries(state, handle);
    }
    StatusScreen_DrawStatusAilments(handle, resource, STATUS_PAGE_NO_CLEAR);
    return;
  }
  StatusScreen_DrawStatusAilments(handle, resource, STATUS_PAGE_SUMMARY);
}

/// The switch-characters help follows the A-button label on the help line:
/// Japanese starts it 64 pixels in, the other editions 80.
#if LANGUAGE == LANGUAGE_JAPANESE
#define SWITCH_CHARACTER_HELP_X 64
#else
#define SWITCH_CHARACTER_HELP_X 80
#endif

void StatusScreen_DrawSelectionCursor(s32 mode, s32 selected, u8 *entries,
                                      s32 invert);

void StatusScreen_DrawSelectionLabels(s32 target, s32 selected,
                                      const u8 *entries);
void StatusScreen_ShowMemberProgressMessage(void *window, s32 help, s32 full);

/// The Status menu's status page: Up and Down choose one of the party member's
/// statuses, whose advice the help window gives, Right the other column, its
/// entries explained in turn; L and R change party member. Returns 1 for A, -1
/// for B.
s32 StatusScreen_SelectCommand(void) {
  struct StatusScreenState *menu;
  s32 pane;
  s32 result;
  s32 selected;
  s8 count;
  s32 hasAilments;
  s32 done;
  s32 hasDjinn;
  s32 redraw;
  s32 member;
  s32 window;
  s32 helpWindow;
  s32 i;
  u8 entries[8];

  menu = Ram_HeapSlots->menuState;
  pane = 0;
  selected = 0;
  result = 0;
  window = menu->statusWindow;
  Window_UpdateOrCreate(&menu->helpWindow, 0, 0, 30, 5, WINDOW_FLAG_INSTANT);
  helpWindow = menu->helpWindow;
  if (Party_SumDjinnCountsFar(-1) != 0)
    hasDjinn = 1;
  else
    hasDjinn = 0;
  for (i = 3; i >= 0; i--)
    menu->rowY[i] = STATUS_FIGURE_FULL_PAGE_ROW_Y;
  done = 0;
  MenuCursor_Slide(-10, 88);

  while (done == 0 && !GameFlag_Get(FLAG_MENU_START_PRESSED)) {
    // DEAD: Unit_Get's result is not used; the ROM keeps the call.
    Unit_Get(menu->selectedIds[0]);
    StatusScreen_DrawStatusAilments(window, menu->selectedIds[0],
                                    STATUS_PAGE_FULL);
    count = StatusScreen_BuildAvailability(entries, 1, menu->selectedIds[0]);
    hasAilments = 0;
    if (count == 0)
      count = 1;
    else
      hasAilments = 1;
    redraw = 1;

    while (!GameFlag_Get(FLAG_MENU_START_PRESSED)) {
      if (redraw) {
        s32 limit;

        redraw = 0;
        limit = count;
        pane = (pane + 2) % 2;
        WindowSprite_RedrawSavedRectFar(helpWindow);
        if (pane == 0) {
          selected = (selected + limit) % limit;
          if (hasAilments == 0) {
            // "A:Psynergy" and "L-R:Switch characters"
            Text_DrawMessageAsTilesFar(MsgSwitchCharacterHelp + 1, window, 0,
                                       -24);
            Text_DrawMessageAsTilesFar(MsgSwitchCharacterHelp, window,
                                       SWITCH_CHARACTER_HELP_X, -24);
          }
        } else {
          if (hasDjinn != 0)
            selected = (selected + 8) % 8;
          else
            selected = (selected + 7) % 7;
        }
        StatusScreen_DrawSelectionCursor(pane, selected, entries, 0);
        WindowSprite_ClearListFar(helpWindow);
        wait(1);
        if (pane == 0)
          StatusScreen_DrawSelectionLabels(helpWindow, selected, entries);
        else
          StatusScreen_ShowMemberProgressMessage(helpWindow, selected,
                                                 hasDjinn);
      }
      if (pane == 0)
        MenuCursor_Position(-10, selected * 16 + 88);
      else if (selected <= 3)
        MenuCursor_Position(24, selected * 8 + 48);
      else
        MenuCursor_Position(48, selected * 8 + 80);
      wait(1);
      if (gInput.repeat & KEYS_DPAD)
        StatusScreen_DrawSelectionCursor(pane, selected, entries, 1);
      if (gInput.pressed & KEY_A) {
        Sound_PlayFar(SE_MENU_CONFIRM);
        done = 1;
        result = 1;
        break;
      }
      if (gInput.pressed & KEY_B) {
        Sound_PlayFar(SE_MENU_CANCEL);
        done = 1;
        result = -1;
        break;
      }
      if (gInput.repeat & KEY_UP) {
        Sound_PlayFar(SE_MENU_CURSOR_MOVE);
        redraw = 1;
        selected--;
      }
      if (gInput.repeat & KEY_DOWN) {
        Sound_PlayFar(SE_MENU_CURSOR_MOVE);
        redraw = 1;
        selected++;
      }
      if (gInput.repeat & KEY_RIGHT) {
        Sound_PlayFar(SE_MENU_CURSOR_MOVE);
        redraw = 1;
        pane++;
      }
      if (gInput.repeat & KEY_LEFT) {
        Sound_PlayFar(SE_MENU_CURSOR_MOVE);
        redraw = 1;
        pane--;
      }
      if ((gInput.repeat & KEY_R) || (gInput.repeat & KEY_L)) {
        Sound_PlayFar(SE_MENU_CURSOR_MOVE);
        member = menu->memberIndex[0];
        if (gInput.repeat & KEY_R)
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
  WindowSprite_PrepareForRedrawFar(menu->helpWindow);
  WindowSprite_RedrawSavedRectFar(window);
  for (i = 3; i >= 0; i--)
    menu->rowY[i] = STATUS_FIGURE_HIDDEN_Y;
  return result;
}

#undef SWITCH_CHARACTER_HELP_X

/// Highlights (invert 0) or unhighlights (invert 1) the selected entry of the
/// status page: in side 0 the selected status row, as wide as its label
/// (gStatusScreen_CursorWidths); in side 1 one of the first four status rows
/// or of the rows below.
void StatusScreen_DrawSelectionCursor(s32 mode, s32 selected, u8 *entries,
                                      s32 invert) {
  struct StatusScreenState *menu = Ram_HeapSlots->menuState;
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
    for (index = 0; index < CHARACTER_AILMENT_COUNT; index++) {
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
  Render_SetTilemapFlagRect(menu->statusWindow, x, y, width, 1, palette);
}

/// Writes the advice for the selected status (two lines, for example how to
/// revive a party member who is down) into window target, or that the party
/// member is well when they have no status.
void StatusScreen_DrawSelectionLabels(s32 target, s32 selected,
                                      const u8 *entries) {
  s32 labelCount = 0;
  s32 labelIndex;

  for (labelIndex = 0; labelIndex < CHARACTER_AILMENT_COUNT; labelIndex++) {
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

/// The help line's length, in characters: longer in the European editions.
#if (LANGUAGE == LANGUAGE_GERMAN) || (LANGUAGE == LANGUAGE_SPANISH) ||         \
    (LANGUAGE == LANGUAGE_FRENCH) || (LANGUAGE == LANGUAGE_ITALIAN)
#define HELP_LENGTH 80
#else
#define HELP_LENGTH 64
#endif

/// Returns the experience party member memberId needs for level.
u32 Unit_GetExperienceForLevelFar(s32 memberId, s32 level);
/// Copies a message's text, its arguments expanded, into buffer: up to length
/// characters with its terminator.
void Message_CopyExpandedFar(s32 messageId, void *buffer, s32 length);
/// Draws a decoded string of 16-bit characters into window at pixel (x, y),
/// all at once.
void Text_DrawWideStringAtOffsetFar(void *text, void *window, s32 x, s32 y);

/// The help lines of the status page's right-hand column, counted from
/// MsgYourNameAndLevelExperience.
enum StatusHelp {
  STATUS_HELP_NAME_AND_LEVEL = 0, ///< "Your name and level. Experience..."
  /// "Experience points. You need ... more to reach the next level."
  STATUS_HELP_EXPERIENCE = 1,
  STATUS_HELP_HP = 2,     ///< "Your current and max Hit Points..."
  STATUS_HELP_PP = 3,     ///< "Your Psynergy Points..."
  STATUS_HELP_DJINN = 4,  ///< "Your Djinn. This also shows which ones are set."
  STATUS_HELP_LEVELS = 5, ///< "Elemental Levels indicate..."
  STATUS_HELP_POWER = 6,  ///< "Power reflects the damage you..."
  STATUS_HELP_RESIST = 7, ///< "Resist reflects your defensive..."
  /// "These are your experience points.", at the highest level.
  STATUS_HELP_EXPERIENCE_AT_MAX = 8
};

/// Writes help line help of the status page's right-hand column into window,
/// skipping the Djinn line unless full says the party has Djinn. The
/// experience line says how many points the next level needs, or at the
/// highest level only what they are.
void StatusScreen_ShowMemberProgressMessage(void *window, s32 help, s32 full) {
  struct StatusScreenState *menu = Ram_HeapSlots->menuState;
  u8 *buffer;

  if (full == 0 && help > STATUS_HELP_PP)
    help++;
  if (help == STATUS_HELP_EXPERIENCE) {
    u8 *owner = &menu->selectedIds[0];
    struct Unit *state = Unit_Get(*owner);

    if (state->level == UNIT_LEVEL_MAX)
      help = STATUS_HELP_EXPERIENCE_AT_MAX;
    else
      MessageArgument_PushFar(
          Unit_GetExperienceForLevelFar(*owner, state->level + 1) -
              state->experience,
          MESSAGE_ARGUMENT_NUMBER);
  }
  buffer = Heap_Alloc(HELP_LENGTH * 2);
  Message_CopyExpandedFar(help + (s32)MsgYourNameAndLevelExperience, buffer,
                          HELP_LENGTH);
  Text_DrawWideStringAtOffsetFar(buffer, window, 0, -1);
  Heap_Release(buffer);
}

#undef HELP_LENGTH

/// The Status screen's level label: `Lv`; `Nv.` in Spanish, `Niv` in French.
extern const u8 gMenu_LvString[];
/// `/` between a current and a maximum HP or PP, and between a set and a held
/// Djinn count.
extern const u8 gMenu_SlashString[];

void ItemMenu_DrawMemberStatus(s32 window, s32 owner, s32 flags);

void MenuPanel_DrawEntryList(struct Window *panel, s32 x, s32 y,
                             const u8 *enabled);

s32 Unit_GetElementLevelFar(s32 ownerId, s32 element);

/// Draws party member memberId's status page into window: the top rows
/// (ItemMenu_DrawMemberStatus), the status list or "Normal", and except on the
/// chooser page the Djinn counts by element and, in mode 1, the elemental
/// levels, Power and Resist. With STATUS_PAGE_NO_CLEAR in mode the window is
/// not cleared first.
void StatusScreen_DrawStatusAilments(s32 window, s32 ownerId, s32 mode) {
  struct StatusScreenState *menu = Ram_HeapSlots->menuState;
  struct Unit *unit;
  s32 element;
  s32 djinnRow;
  s32 keepDrawn;
  s32 hasDjinn;
  u8 ailments[8];

  if (Party_SumDjinnCountsFar(-1) != 0)
    hasDjinn = 1;
  else
    hasDjinn = 0;
  unit = Unit_Get(ownerId);
  // The Djinn row, in tiles: higher on the full page (mode 1), which has the
  // elemental rows below it.
  if ((mode & STATUS_PAGE_MODE_MASK) == STATUS_PAGE_FULL)
    djinnRow = 7;
  else
    djinnRow = 10;
  menu->statusCursor->motion = ICON_SHOWN;
  ItemMenu_DrawMemberStatus(window, ownerId, mode);
  StatusScreen_BuildAvailability(ailments, 1, ownerId);
  keepDrawn = mode & STATUS_PAGE_NO_CLEAR;
  if (keepDrawn == 0)
    Window_ClearInteriorTilesFar(window, 0, 40, 96, 96);
  MenuPanel_DrawEntryList(window, 0, 40, ailments);
  if (menu->page == STATUS_SCREEN_PAGE_CHOOSER)
    return;
  if (keepDrawn == 0) {
    wait(1);
    Window_ClearInteriorTilesFar(window, 64, 56, 224, 96);
  }
  // The four element icons, then the Djinn counts and, in mode 1, the
  // elemental levels, Power and Resist.
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
  for (element = 0; element <= 3; element++) {
    if (hasDjinn)
      Text_DrawNumberInWindowFar(unit->djinnHeldCounts[element], 1, window,
                                 STATUS_ELEMENT_X(element), (djinnRow + 1) * 8);
    if ((mode & STATUS_PAGE_MODE_MASK) == STATUS_PAGE_FULL) {
      if (hasDjinn) {
        Text_DrawNumberInWindowFar(unit->djinnSetCounts[element], 1, window,
                                   STATUS_ELEMENT_X(element) - 16,
                                   (djinnRow + 1) * 8);
        Text_DrawStringInWindowFar(gMenu_SlashString, window,
                                   STATUS_ELEMENT_X(element) - 8,
                                   (djinnRow + 1) * 8);
      }
      Text_DrawNumberInWindowFar(Unit_GetElementLevelFar(ownerId, element), 2,
                                 window, STATUS_ELEMENT_X(element) - 8,
                                 (djinnRow + 2) * 8);
      Text_DrawNumberInWindowFar(unit->element[element].power, 3, window,
                                 STATUS_ELEMENT_X(element) - 16,
                                 (djinnRow + 3) * 8);
      Text_DrawNumberInWindowFar(unit->element[element].resistance, 3, window,
                                 STATUS_ELEMENT_X(element) - 16,
                                 (djinnRow + 4) * 8);
    }
  }
}

/// The HP label: `HP`; `KP` in German, `PV` in Spanish and French, `PS` in
/// Italian.
extern const u8 gMenu_HpString[];
/// The PP label: `PP`; `EP` in German.
extern const u8 gMenu_PpString[];

/// The stat column of the status page's top rows: three digits from 200 or
/// four from 192, both ending at pixel 224.
#if LANGUAGE != LANGUAGE_JAPANESE
#define STATUS_STAT_DIGITS 3
#define STATUS_STAT_X 200
#else
#define STATUS_STAT_DIGITS 4
#define STATUS_STAT_X 192
#endif

/// Stat label `stat`, 0 to 3, of "Attack", "Defense", "Agility" and "Luck",
/// MsgStatLabel and the three after it, counted back from MsgExpLabel.
#define STAT_LABEL(stat)                                                       \
  ((s32)MsgExpLabel - (MSGNO(MsgExpLabel) - MSGNO(MsgStatLabel) - (stat)))

/// Draws the top rows of party member owner's status page into window: name,
/// class, level, experience, HP and PP on the left, Attack, Defense, Agility
/// and Luck on the right.
void ItemMenu_DrawMemberStatus(s32 window, s32 owner, s32 flags) {
  struct ItemMenuState *menu;
  struct Unit *unit;

  menu = Ram_HeapSlots->menuState;
  unit = Unit_Get(owner);
  menu->cursor->motion = ICON_SHOWN;
  flags &= STATUS_PAGE_NO_CLEAR;
  if (flags == 0)
    Window_ClearInteriorTilesFar(window, 0, 0, 128, 40);
  Text_DrawStringAtOffsetFar(unit->name, window, 40, 0);
  Text_DrawMessageAsTilesFar(unit->classId + (s32)MsgClassName, window, 0, 32);
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
  Text_DrawMessageAsTilesFar((s32)MsgExpLabel, window, 40, 8);
  Text_DrawNumberInWindowFar(unit->experience, 8, window, 80, 8);
  if (flags == 0) {
    wait(1);
    Window_ClearInteriorTilesFar(window, 144, 0, 224, 40);
  }
  // "Attack", "Defense", "Agility" and "Luck".
  Text_DrawMessageAsTilesFar(STAT_LABEL(0), window, 152, 0);
  Text_DrawMessageAsTilesFar(STAT_LABEL(1), window, 152, 8);
  Text_DrawMessageAsTilesFar(STAT_LABEL(2), window, 152, 16);
  Text_DrawMessageAsTilesFar(STAT_LABEL(3), window, 152, 24);
  Text_DrawNumberInWindowFar(unit->atk, STATUS_STAT_DIGITS, window,
                             STATUS_STAT_X, 0);
  Text_DrawNumberInWindowFar(unit->def, STATUS_STAT_DIGITS, window,
                             STATUS_STAT_X, 8);
  Text_DrawNumberInWindowFar(unit->agi, STATUS_STAT_DIGITS, window,
                             STATUS_STAT_X, 16);
  Text_DrawNumberInWindowFar(unit->luck, STATUS_STAT_DIGITS, window,
                             STATUS_STAT_X, 24);
}

#undef STATUS_STAT_DIGITS
#undef STATUS_STAT_X
#undef STAT_LABEL

// The rows one page of the list shows: Japanese fits six, the other editions
// five.
#if LANGUAGE == LANGUAGE_JAPANESE
#define PAGE_ROWS 6
#else
#define PAGE_ROWS 5
#endif

/// Fills result with the item list of the party member in side index for the
/// equipment pages: their unit, the menu's item count, and the page and row of
/// the item chosen last for them, PAGE_ROWS items to a page.
/// Returns 1.
s32 PagedList_Build(struct PagedList *result, s32 index) {
  struct ItemMenuState *menu = Ram_HeapSlots->menuState;
  void *owner;
  s32 count;
  s32 value;
  s32 page;
  s32 row;
  s32 pages;

  owner = Unit_Get(menu->sideMember[index]);
  count = menu->itemCount;
  value = menu->selectedIndexByMember[menu->sideMember[index]];

  if (value + 1 > count)
    value = count - 1;
  if (count == 0)
    value = 0;
  page = value / PAGE_ROWS;
  row = value % PAGE_ROWS;
  pages = count / PAGE_ROWS;
  if (count % PAGE_ROWS != 0)
    pages++;
  result->memberState = (s32)owner;
  result->page = page;
  result->pageCount = pages;
  result->row = row;
  result->entryCount = count;
  result->selectedIndex = value;
  return 1;
}

#undef PAGE_ROWS

/// Pairs of 4-bit rows of dots in colour 5, each reaching further right than
/// the last, that Menu_BuildPatternTiles XORs into its tile pairs
/// (menu_strings.s).
extern const u32 gMenu_PatternRowMasks[][2];

/// The words of one range icon's two tiles.
#define RANGE_PATTERN_PAIR_WORDS 16
/// The first of the Status menu's range icon BG tiles, which
/// PsynergyMenu_DrawRange draws as tilemap entries; the battle menu builds its
/// own elsewhere (Menu_BuildLocalizedPatternTiles).
#define STATUS_RANGE_ICON_FIRST_TILE 768

/// Builds the twelve range icons PsynergyMenu_DrawRange draws, as the 24 BG
/// tiles from STATUS_RANGE_ICON_FIRST_TILE: icon n is a centre tile and an end
/// tile, filled with WINDOW_BACKGROUND_COLOR, then marked row by row by XOR
/// with gMenu_PatternRowMasks, the first set's marks growing row by row, the
/// second set's leaving row 1 bare.
void Menu_BuildPatternTiles(void) {
  s32 patternSet;
  s32 patternIndex;

  for (patternSet = 0; patternSet < RANGE_PATTERN_SET_COUNT; patternSet++) {
    for (patternIndex = 0; patternIndex < RANGE_PATTERN_COUNT; patternIndex++) {
      u32 *tile =
          (u32 *)(BG_CHAR_BLOCK(0) +
                  STATUS_RANGE_ICON_FIRST_TILE * TILE_SIZE_4BPP) +
          patternSet * (RANGE_PATTERN_COUNT * RANGE_PATTERN_PAIR_WORDS) +
          patternIndex * RANGE_PATTERN_PAIR_WORDS;
      s32 row;

      Iwram_FillWordsEntry(tile, 2 * TILE_SIZE_4BPP,
                           TEXT_CANVAS_BACKGROUND_WORD);
      for (row = 1; row <= 7; row++) {
        s32 sourcePattern = patternIndex;

        if (patternSet == 1 && row <= 1)
          continue;
        if (patternSet == 0 && patternIndex > row - 2) {
          sourcePattern = row - 2;
          if (sourcePattern < 0)
            sourcePattern = 0;
        }
        tile[row] = XorWord(tile[row], gMenu_PatternRowMasks[sourcePattern][0]);
        tile[row + 8] =
            XorWord(tile[row + 8], gMenu_PatternRowMasks[sourcePattern][1]);
      }
    }
  }
}

/// Draws a Psynergy range icon at tile (x, y) of window: three tiles from pair
/// range of the range icon tiles (Menu_BuildPatternTiles), the side tile on
/// both ends, mirrored on the left, and the centre tile between. range is the
/// ability's range less one (0 for the aimed unit alone), or RANGE_ICON_ALL
/// for ABILITY_RANGE_ALL (PsynergyMenu_DrawListPage).
void PsynergyMenu_DrawRange(s32 window, s32 x, s32 y, s32 range, s32 unused) {
  s32 n;
  s32 tile;

  n = range * 2;
  tile = n + WINDOW_TILE(STATUS_RANGE_ICON_FIRST_TILE) + 1;
  Window_SetTilemapEntryFar(window, TILE_HFLIP | tile, x, y, 0);
  Window_SetTilemapEntryFar(
      window, n + WINDOW_TILE(STATUS_RANGE_ICON_FIRST_TILE), x + 1, y, 0);
  Window_SetTilemapEntryFar(window, tile, x + 2, y, 0);
}

/// The Japanese page lists six abilities rather than five, a row higher, and
/// says where the chosen ability can be used in short lines stacked at the
/// window's left edge rather than in one sentence across it. FIRST_TILE_ROW is
/// the tile row of the page's first ability; USABLE_TOP, USABLE_RIGHT and
/// USABLE_BOTTOM bound that text in pixels inside the window.
#if LANGUAGE == LANGUAGE_JAPANESE
#define PAGE_ROWS 6
#define FIRST_TILE_ROW 1
#define USABLE_TOP 72
#define USABLE_RIGHT 64
#define USABLE_BOTTOM 96
/// まちやダンジョン ("towns and dungeons"), the message two after MsgExpLabel.
#define USABLE_IN_FIELD_MESSAGE ((s32)MsgExpLabel + 2)
/// せんとう ("battle"), the message after MsgExpLabel.
#define USABLE_IN_BATTLE_MESSAGE ((s32)MsgExpLabel + 1)
/// でつかえます ("can be used"), the message before MsgDjinnListHelp.
#define CAN_BE_USED_MESSAGE ((s32)MsgDjinnListHelp - 1)
#else
#define PAGE_ROWS 5
#define FIRST_TILE_ROW 2
#define USABLE_TOP 96
#define USABLE_RIGHT 224
#define USABLE_BOTTOM 104
#endif

/// Updates the Psynergy page for the row under the cursor: the Psynergy's
/// description in the info window, where it can be used (in the field, in
/// battle, or anywhere), and the row highlights with the range icons.
s32 PsynergyMenu_DrawRangePage(s32 window, s32 unused,
                               struct PagedList *state) {
  struct PsynergyMenuState *menu;
  struct AbilityDefinition *ability;
  s32 row;
  s32 base;

  menu = (struct PsynergyMenuState *)Ram_HeapSlots->menuState;
  state->selectedIndex = state->page * PAGE_ROWS + state->row;
  // The Japanese release clears the info window; from patch 1 it redraws the
  // window's saved rectangle.
#if TLA_PATCH_1
  WindowSprite_RedrawSavedRectFar(menu->infoWindow);
#else
  WindowSprite_ClearListFar(menu->infoWindow);
#endif
  wait(1);
  if (menu->psynergies[state->selectedIndex] != 0) {
    // Japanese draws the description as a resource string.
#if LANGUAGE == LANGUAGE_JAPANESE
    Text_DrawMessageFar(
        (menu->psynergies[state->selectedIndex] & ACTION_ID_MASK) +
            (s32)MsgAbilityDescription,
        (s32)menu->infoWindow, 0, 0);
#else
    Text_DrawMessageAsTilesFar(
        (menu->psynergies[state->selectedIndex] & ACTION_ID_MASK) +
            (s32)MsgAbilityDescription,
        (s32)menu->infoWindow, 0, 0);
#endif
    ability =
        Ability_Get(menu->psynergies[state->selectedIndex] & ACTION_ID_MASK);
    Window_ClearInteriorTilesFar(window, 0, USABLE_TOP, USABLE_RIGHT,
                                 USABLE_BOTTOM);
    row = 0;
#if LANGUAGE == LANGUAGE_JAPANESE
    // One line for each place the ability works, then the end of the
    // sentence on the line below them.
    if (ability->fieldPsynergy != 0 ||
        (ability->flags & ACTION_USABLE_IN_FIELD) != 0) {
      Text_DrawMessageAsTilesFar(USABLE_IN_FIELD_MESSAGE, window, 0,
                                 USABLE_TOP);
      row = 1;
    }
    if ((ability->flags & ACTION_USABLE_IN_BATTLE) != 0) {
      Text_DrawMessageAsTilesFar(USABLE_IN_BATTLE_MESSAGE, window, 0,
                                 row * 8 + USABLE_TOP);
      row++;
    }
    Text_DrawMessageAsTilesFar(CAN_BE_USED_MESSAGE, window, 0,
                               row * 8 + USABLE_TOP);
#else
    // row holds where it works for now: 2 in the field, 1 in battle.
    if (ability->fieldPsynergy != 0 ||
        (ability->flags & ACTION_USABLE_IN_FIELD) != 0) {
      row = 2;
    }
    if ((ability->flags & ACTION_USABLE_IN_BATTLE) != 0) {
      row |= 1;
    }
    if (row == 3) {
      Text_DrawMessageAsTilesFar((s32)MsgUsableAnywhere, window, 0, USABLE_TOP);
    } else if (row == 2) {
      Text_DrawMessageAsTilesFar((s32)MsgUsableInField, window, 0, USABLE_TOP);
    } else if (row == 1) {
      Text_DrawMessageAsTilesFar((s32)MsgUsableInBattle, window, 0, USABLE_TOP);
    }
#endif
  }

  base = state->page * PAGE_ROWS;
  for (row = 0; row <= PAGE_ROWS - 1; row++) {
    if (row == state->row) {
      ability = Ability_Get(menu->psynergies[base + row] & ACTION_ID_MASK);
      if (ability->element != ELEMENT_NONE) {
        Window_SetTilemapEntryFar(window, ability->element + 1, 24,
                                  row * 2 + FIRST_TILE_ROW, 0);
        Render_SetTilemapFlagRect(window, 9, row * 2 + FIRST_TILE_ROW, 15, 1,
                                  HIGHLIGHT_PALETTE);
        Render_SetTilemapFlagRect(window, 25, row * 2 + FIRST_TILE_ROW, 3, 1,
                                  HIGHLIGHT_PALETTE);
      } else {
        Render_SetTilemapFlagRect(window, 9, row * 2 + FIRST_TILE_ROW, 19, 1,
                                  HIGHLIGHT_PALETTE);
      }
    } else {
      ability = Ability_Get(menu->psynergies[base + row] & ACTION_ID_MASK);
      if (ability->element != ELEMENT_NONE) {
        Window_SetTilemapEntryFar(window, ability->element + 1, 24,
                                  row * 2 + FIRST_TILE_ROW, TILEMAP_ENTRY_ICON);
        Render_SetTilemapFlagRect(window, 9, row * 2 + FIRST_TILE_ROW, 15, 1,
                                  WINDOW_PALETTE);
        Render_SetTilemapFlagRect(window, 25, row * 2 + FIRST_TILE_ROW, 3, 1,
                                  WINDOW_PALETTE);
      } else {
        Render_SetTilemapFlagRect(window, 9, row * 2 + FIRST_TILE_ROW, 19, 1,
                                  WINDOW_PALETTE);
      }
    }
  }
  wait(1);
  return 1;
}

#undef PAGE_ROWS
#undef USABLE_RIGHT
#undef USABLE_BOTTOM

/// The Japanese page lists six abilities rather than five, a row higher, and
/// lays its heading, names, costs, page icons and "no Psynergy" message out on
/// its own columns.
#if LANGUAGE == LANGUAGE_JAPANESE
#define PAGE_ROWS 6
#define PAGE_ICONS_X 86
#define PAGE_ICONS_Y 53
#define STATUS_PSYNERGY_PP_HEADING_X 144
#define FIRST_ROW_Y 8
#define STATUS_PSYNERGY_NAME_X 96
#define STATUS_PSYNERGY_COST_X 168
#define FIRST_RANGE_ROW 1
#define EMPTY_MESSAGE_Y 8
#else
#define PAGE_ROWS 5
#define PAGE_ICONS_X 80
#define PAGE_ICONS_Y 58
#define STATUS_PSYNERGY_PP_HEADING_X 176
#define FIRST_ROW_Y 16
#define STATUS_PSYNERGY_NAME_X 88
#define STATUS_PSYNERGY_COST_X 176
#define FIRST_RANGE_ROW 2
#define EMPTY_MESSAGE_Y 17
#endif

/// The Psynergy menu's list page: each ability's name, PP cost and range, "no
/// Psynergy" when empty, then the owner's name, class and level.
s32 PsynergyMenu_DrawListPage(s32 window, s32 unused,
                              const struct PagedList *res) {
  struct PsynergyMenuState *menu = Ram_HeapSlots->menuState;
  struct Unit *member;
  u32 firstEntry;
  u32 rows;
  u8 row;
  s32 entryIndex;

  (void)unused;

  member = Unit_Get(menu->memberIds[0]);

  WindowSprite_RedrawSavedRectFar(window);

  firstEntry = res->page * PAGE_ROWS;
  rows = (u8)(res->entryCount - firstEntry);
  if (rows > PAGE_ROWS) {
    rows = PAGE_ROWS;
  }

  Menu_SetPageIcons(PAGE_ROWS, firstEntry, window, PAGE_ICONS_X, PAGE_ICONS_Y);
  PagedList_DrawPageNumbers(window, res->entryCount, PAGE_ROWS, res->page, 28);

  // "PP"
  Text_DrawMessageAsTilesFar((s32)MsgPsynergyChooseTarget + 2, window,
                             STATUS_PSYNERGY_PP_HEADING_X, 0);

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
                  (s32)MsgAbilityName;
      y = row * 16 + FIRST_ROW_Y;

      Text_DrawMessageAsTilesFar(messageId, window, STATUS_PSYNERGY_NAME_X, y);
      Text_DrawNumberAtOffsetFar(action->ppCost, 2, window,
                                 STATUS_PSYNERGY_COST_X, y);

      // ABILITY_RANGE_ALL takes the last range icon; range n takes n - 1.
      range = action->range;
      if (range == ABILITY_RANGE_ALL) {
        range = RANGE_ICON_ALL;
      } else {
        range--;
      }
      PsynergyMenu_DrawRange(window, 25, row * 2 + FIRST_RANGE_ROW, range, 0);

      row++;
      entryIndex++;
    } while (rows > row);
  }

  if (menu->psynergyCount == 0) {
    // "You have no Psynergy."
    Text_DrawMessageAsTilesFar((s32)MsgPsynergyChooseForR - 1, window, 96,
                               EMPTY_MESSAGE_Y);
  }

  Text_DrawStringAtOffsetFar(member->name, window, 40, 0);
  Text_DrawMessageAsTilesFar(member->classId + (s32)MsgClassName, window, 0,
                             32);
  // Spanish and French draw the level label at an offset, as the owner's name.
#if LANGUAGE == LANGUAGE_SPANISH || LANGUAGE == LANGUAGE_FRENCH
  Text_DrawStringAtOffsetFar(gMenu_LvString, window, 0, 48);
#else
  Text_DrawStringInWindowFar(gMenu_LvString, window, 0, 48);
#endif
  Text_DrawNumberInWindowFar(member->level, 2, window, 24, 48);

  return 1;
}

#undef PAGE_ROWS
#undef PAGE_ICONS_X
#undef STATUS_PSYNERGY_PP_HEADING_X
#undef STATUS_PSYNERGY_NAME_X
#undef STATUS_PSYNERGY_COST_X
#undef FIRST_RANGE_ROW
#undef EMPTY_MESSAGE_Y

/// The rows of a page of the list and where the cursor points at its first row:
/// the Japanese page lists six abilities, a row higher.
#if LANGUAGE == LANGUAGE_JAPANESE
#define PAGE_ROWS 6
#define STATUS_PSYNERGY_CURSOR_X 58
#define CURSOR_Y 52
#else
#define PAGE_ROWS 5
#define STATUS_PSYNERGY_CURSOR_X 55
#define CURSOR_Y 60
#endif

/// The Status menu's Psynergy page: the party member's Psynergy, PAGE_ROWS to a
/// page, each with its PP cost and range icon; the cursor's Psynergy is
/// described in the info window with where it can be used.
s32 PsynergyMenu_SelectAction(void) {
  struct PsynergyMenuState *menu;
  s32 result;
  s32 window;
  s32 refresh;
  s32 listNeedsRedraw;
  s32 navigation;
  s32 ownerIndex;
  struct PagedList state;

  menu = Ram_HeapSlots->menuState;
  window = menu->statusWindow;
  result = 0;
  refresh = 2;
  listNeedsRedraw = 1;
  Window_UpdateOrCreate(&menu->infoWindow, 0, 0, 30, 5, WINDOW_FLAG_INSTANT);
  Menu_SpawnIconEntries(menu, menu->messageWindow);
  // "A:Item" and "L-R:Switch characters"
  Text_DrawMessageAsTilesFar(MsgSwitchCharacterHelp + 2, window, 0, -24);
  Text_DrawMessageAsTilesFar(MsgSwitchCharacterHelp, window, 64, -24);
  Menu_BuildPatternTiles();
  while (!GameFlag_Get(FLAG_MENU_START_PRESSED)) {
    if (refresh) {
      if (refresh == 2) {
        menu->psynergyCount = PsynergyMenu_CollectActions(
            Unit_Get(menu->memberIds[0]), menu->psynergies, PSYNERGY_LIST_ALL);
        PagedList_Build(&state, 0);
        ItemMenu_HideAllIcons();
        PsynergyMenu_DrawPsynergyIcons(menu->psynergies);
        listNeedsRedraw = 1;
        wait(1);
      }
      if (listNeedsRedraw) {
        listNeedsRedraw = 0;
        PsynergyMenu_DrawListPage(window, 0, &state);
      }
      PsynergyMenu_DrawRangePage(window, 0, &state);
#if TLA_PATCH_2
      // German, Spanish, French and Italian draw the help again after each
      // page.
      Text_DrawMessageAsTilesFar(MsgSwitchCharacterHelp + 2, window, 0, -24);
      Text_DrawMessageAsTilesFar(MsgSwitchCharacterHelp, window, 64, -24);
#endif
      refresh = 0;
    }
    wait(1);
    navigation = PagedList_HandleInput(0, state.entryCount, PAGE_ROWS,
                                       &state.row, &state.page);
    MenuCursor_Position(STATUS_PSYNERGY_CURSOR_X, state.row * 16 + CURSOR_Y);
    if (navigation == 1) {
      refresh = 1;
      listNeedsRedraw = 1;
    }
    if (navigation == 0)
      refresh = 1;
    if (navigation == -1)
      refresh = 0;
    if (gInput.pressed & KEY_A) {
      Sound_PlayFar(SE_MENU_CONFIRM);
      result = 1;
      break;
    }
    if (gInput.pressed & KEY_B) {
      Sound_PlayFar(SE_MENU_CANCEL);
      result = -1;
      break;
    }
    if ((gInput.repeat & KEY_R) || (gInput.repeat & KEY_L)) {
      Sound_PlayFar(SE_MENU_CURSOR_MOVE);
      ownerIndex = menu->tabIndex[0];
      menu->selectedIndexByMember[menu->memberTable[ownerIndex]] =
          state.selectedIndex;
      if (gInput.repeat & KEY_R)
        ownerIndex++;
      else
        ownerIndex--;
      ownerIndex = (ownerIndex + menu->memberCount) % menu->memberCount;
      menu->selectedMember = menu->memberTable[ownerIndex];
      menu->memberIds[0] = menu->memberTable[ownerIndex];
      menu->tabIndex[0] = ownerIndex;
      Menu_ReloadMemberPortrait(menu, menu->memberTable[ownerIndex]);
      refresh = 2;
    }
  }
  WindowSprite_PrepareForRedrawFar(menu->infoWindow);
  WindowSprite_RedrawSavedRectFar(window);
  WindowSprite_ClearListFar(menu->messageWindow);
  return result;
}

#undef PAGE_ROWS
#undef STATUS_PSYNERGY_CURSOR_X
#undef CURSOR_Y

// The rows one page of the item list shows: Japanese fits six, the other
// editions five.
#if LANGUAGE == LANGUAGE_JAPANESE
#define PAGE_ROWS 6
#else
#define PAGE_ROWS 5
#endif

/// The tile column the equipment slots' highlight starts at.
#define EQUIP_SLOT_X 2
/// The equipment slots' highlight is a tile wider in the European editions.
#if LANGUAGE == LANGUAGE_ENGLISH || LANGUAGE == LANGUAGE_JAPANESE
#define EQUIP_SLOT_WIDTH 9
#else
#define EQUIP_SLOT_WIDTH 10
#endif

/// Updates the Status menu's equipment page for the row under the cursor: the
/// item's plain name in the info window, the row highlights, and in the
/// equipment window the highlight of the slot the item is worn in, if it is.
s32 ItemMenu_DrawEquipPage(s32 window, s32 unused, struct PagedList *state) {
  struct ItemMenuState *menu;
  struct ItemDefinition *item;
  s32 base;
  s32 row;
  u32 entry;
  u32 mask;

  menu = Ram_HeapSlots->menuState;
  base = state->page * PAGE_ROWS;
  state->selectedIndex = base + state->row;
  // The Japanese release clears the name's window; from patch 1 it redraws the
  // window's saved rectangle.
#if TLA_PATCH_1
  WindowSprite_RedrawSavedRectFar(menu->infoWindow);
#else
  WindowSprite_ClearListFar(menu->infoWindow);
#endif
  wait(1);
  if (menu->items[state->selectedIndex] != 0) {
    // Japanese draws the name as a resource string.
#if LANGUAGE == LANGUAGE_JAPANESE
    Text_DrawMessageFar(
        (menu->items[state->selectedIndex] & INVENTORY_ITEM_ID_MASK) +
            (s32)MsgItemPlainName,
        menu->infoWindow, 0, 0);
#else
    Text_DrawMessageAsTilesFar(
        (menu->items[state->selectedIndex] & INVENTORY_ITEM_ID_MASK) +
            MsgItemPlainName,
        menu->infoWindow, 0, 0);
#endif
  }

  for (row = 0; row < PAGE_ROWS; row++) {
    if (row == state->row) {
      entry = menu->items[state->selectedIndex];
      mask = INVENTORY_ITEM_ID_MASK;
      mask &= entry;
      item = Item_GetDirectFar(mask);
      if (item->element != ELEMENT_NONE) {
        Window_SetTilemapEntryFar(window, item->element + 1, 27, row * 2 + 1,
                                  0);
        Render_SetTilemapFlagRect(window, 14, row * 2 + 1, 13, 1,
                                  HIGHLIGHT_PALETTE);
      } else {
        Render_SetTilemapFlagRect(window, 14, row * 2 + 1, 14, 1,
                                  HIGHLIGHT_PALETTE);
      }
    } else {
      entry = menu->items[base + row];
      mask = INVENTORY_ITEM_ID_MASK;
      mask &= entry;
      item = Item_GetDirectFar(mask);
      if (item->element != ELEMENT_NONE) {
        Window_SetTilemapEntryFar(window, item->element + 1, 27, row * 2 + 1,
                                  TILEMAP_ENTRY_ICON);
        Render_SetTilemapFlagRect(window, 14, row * 2 + 1, 13, 1,
                                  WINDOW_PALETTE);
      } else {
        Render_SetTilemapFlagRect(window, 14, row * 2 + 1, 14, 1,
                                  WINDOW_PALETTE);
      }
    }
  }

  for (row = 0; row <= 3; row++)
    Render_SetTilemapFlagRect(menu->equipWindow, EQUIP_SLOT_X, row * 2 + 1,
                              EQUIP_SLOT_WIDTH, 1, WINDOW_PALETTE);

  if (menu->items[state->selectedIndex] & INVENTORY_EQUIPPED) {
    item = Item_GetDirectFar(menu->items[state->selectedIndex] &
                             INVENTORY_ITEM_ID_MASK);
    switch (item->type) {
    case ITEM_TYPE_WEAPON:
      Render_SetTilemapFlagRect(menu->equipWindow, EQUIP_SLOT_X, 1,
                                EQUIP_SLOT_WIDTH, 1, HIGHLIGHT_PALETTE);
      break;
    case ITEM_TYPE_HELM:
      Render_SetTilemapFlagRect(menu->equipWindow, EQUIP_SLOT_X, 3,
                                EQUIP_SLOT_WIDTH, 1, HIGHLIGHT_PALETTE);
      break;
    case ITEM_TYPE_SHIELD:
      Render_SetTilemapFlagRect(menu->equipWindow, EQUIP_SLOT_X, 5,
                                EQUIP_SLOT_WIDTH, 1, HIGHLIGHT_PALETTE);
      break;
    case ITEM_TYPE_ARMOR:
      Render_SetTilemapFlagRect(menu->equipWindow, EQUIP_SLOT_X, 7,
                                EQUIP_SLOT_WIDTH, 1, HIGHLIGHT_PALETTE);
      break;
    }
  }
  wait(1);
  return 1;
}

#undef PAGE_ROWS
#undef EQUIP_SLOT_WIDTH

/// The rows of a page of the item list: six in Japanese.
#if LANGUAGE == LANGUAGE_JAPANESE
#define PAGE_ROWS 6
#else
#define PAGE_ROWS 5
#endif

/// The page icons, the item names, the "no items" message and the Attack and
/// Defense figures, as each translation lays them out.
#if (LANGUAGE == LANGUAGE_SPANISH) || (LANGUAGE == LANGUAGE_ITALIAN)
#define PAGE_ICONS_X 119
#define ITEM_NAME_X 128
#else
#define PAGE_ICONS_X 123
#define ITEM_NAME_X 136
#endif
#if LANGUAGE == LANGUAGE_GERMAN
#define EMPTY_MESSAGE_X 112
#else
#define EMPTY_MESSAGE_X 120
#endif
#if (LANGUAGE == LANGUAGE_SPANISH) || (LANGUAGE == LANGUAGE_FRENCH)
#define STAT_X 80
#else
#define STAT_X 72
#endif

/// Draws one page of the member's items into window: PAGE_ROWS names, or
/// "no items", then the member's name, Attack and Defense.
s32 ItemMenu_DrawItemPage(s32 window, s32 unused, struct PagedList *state) {
  struct ItemMenuState *menu = Ram_HeapSlots->menuState;
  struct Unit *unit;
  u16 *inventoryEntry;
  s32 firstEntry;
  u32 rowCount;
  u32 rowIndex;
  u8 *message;

  unit = Unit_Get(menu->sideMember[0]);
  Window_ClearInteriorTilesFar(window, 128, 8, 224, 96);
  firstEntry = state->page * PAGE_ROWS;
  rowCount = (u8)(state->entryCount - firstEntry);
  if (rowCount > PAGE_ROWS)
    rowCount = PAGE_ROWS;
  Menu_SetPageIcons(PAGE_ROWS, firstEntry, window, PAGE_ICONS_X, 52);
  PagedList_DrawPageNumbers(window, state->entryCount, PAGE_ROWS, state->page,
                            28);
  if (menu->itemCount == 0) {
    Text_DrawMessageAsTilesFar(MsgItemMenuEmpty, window, EMPTY_MESSAGE_X, 8);
  } else {
    rowIndex = 0;
    if (rowCount > rowIndex) {
      inventoryEntry = &menu->items[firstEntry];
      do {
        Text_DrawMessageAsTilesFar((*inventoryEntry & INVENTORY_ITEM_ID_MASK) +
                                       MsgItemName,
                                   window, ITEM_NAME_X, (rowIndex << 4) + 8);
        rowIndex = (u8)(rowIndex + 1);
        inventoryEntry++;
      } while (rowCount > rowIndex);
    }
  }
  Text_DrawStringAtOffsetFar(unit->name, window, 40, 0);
  message = MsgStatLabel;
  Text_DrawMessageAsTilesFar(message, window, 32, 16);
  Text_DrawMessageAsTilesFar(message + 1, window, 32, 24);
  Text_DrawNumberInWindowFar(unit->atk, 3, window, STAT_X, 16);
  Text_DrawNumberInWindowFar(unit->def, 3, window, STAT_X, 24);
  return 1;
}

#undef PAGE_ROWS
#undef EMPTY_MESSAGE_X
#undef STAT_X

/// The equip window's width in tiles, narrower in Japanese.
#if LANGUAGE == LANGUAGE_JAPANESE
#define EQUIP_WINDOW_WIDTH 13
#else
#define EQUIP_WINDOW_WIDTH 15
#endif

// The rows one page of the item list shows: Japanese fits six, the other
// editions five.
#if LANGUAGE == LANGUAGE_JAPANESE
#define PAGE_ROWS 6
#else
#define PAGE_ROWS 5
#endif

/// The French list puts its cursor 4 pixels further right.
#if LANGUAGE == LANGUAGE_FRENCH
#define STATUS_EQUIPMENT_CURSOR_X 100
#else
#define STATUS_EQUIPMENT_CURSOR_X 96
#endif

/// The Status menu's equipment page: the party member's items, PAGE_ROWS to a
/// page, beside the equipment window of what they wear.
s32 ItemMenu_SelectItem(void) {
  struct ItemMenuState *menu;
  struct Unit *owner;
  s32 result;
  s32 window;
  s32 refresh;
  s32 listNeedsRedraw;
  s32 navigation;
  s32 ownerIndex;
  s32 i;
  struct PagedList state;
  u16 savedInventory[INVENTORY_SLOTS];

  menu = Ram_HeapSlots->menuState;
  window = menu->statusWindow;
  result = 0;
  refresh = 2;
  listNeedsRedraw = 1;
  owner = Unit_Get(menu->sideMember[0]);
  Window_UpdateOrCreate(&menu->equipWindow, 0, 10, EQUIP_WINDOW_WIDTH, 10,
                        WINDOW_FLAG_INSTANT);
  Menu_SpawnIconEntries(menu, menu->questionWindow);
  // "A:Status" and "L-R:Switch characters"
  Text_DrawMessageAsTilesFar(MsgSwitchCharacterHelp + 3, window, 0, -24);
  Text_DrawMessageAsTilesFar(MsgSwitchCharacterHelp, window, 64, -24);
  while (!GameFlag_Get(FLAG_MENU_START_PRESSED)) {
    if (refresh) {
      if (refresh == 2) {
        owner = Unit_Get(menu->sideMember[0]);
        menu->itemCount = ItemMenu_CollectItems(owner, menu->items, 0);
        WindowSprite_RedrawSavedRectFar(window);
        for (i = 0; i < INVENTORY_SLOTS; i++)
          savedInventory[i] = owner->inventory[i];
        ItemMenu_Sort(menu->items, 0);
        PagedList_Build(&state, 0);
        ItemMenu_DrawIcons(menu->items, 0);
        WindowSprite_RedrawSavedRectFar(menu->equipWindow);
        ItemMenu_DrawCategory(menu->equipWindow, menu->sideMember[0], 1);
        listNeedsRedraw = 1;
        wait(1);
      }
      if (listNeedsRedraw) {
        listNeedsRedraw = 0;
        ItemMenu_DrawItemPage(window, 0, &state);
      }
      ItemMenu_DrawEquipPage(window, 0, &state);
#if TLA_PATCH_1
      Text_DrawMessageAsTilesFar(MsgSwitchCharacterHelp + 3, window, 0, -24);
      Text_DrawMessageAsTilesFar(MsgSwitchCharacterHelp, window, 64, -24);
#endif
      refresh = 0;
    }
    wait(1);
    navigation = PagedList_HandleInput(0, state.entryCount, PAGE_ROWS,
                                       &state.row, &state.page);
    MenuCursor_Position(STATUS_EQUIPMENT_CURSOR_X, state.row * 16 + 52);
    if (navigation == 1) {
      refresh = 1;
      listNeedsRedraw = 1;
    }
    if (navigation == 0)
      refresh = 1;
    if (navigation == -1)
      refresh = 0;
    if (gInput.pressed & KEY_A) {
      Sound_PlayFar(SE_MENU_CONFIRM);
      result = 1;
      break;
    }
    if (gInput.pressed & KEY_B) {
      Sound_PlayFar(SE_MENU_CANCEL);
      result = -1;
      break;
    }
    if ((gInput.repeat & KEY_R) || (gInput.repeat & KEY_L)) {
      Sound_PlayFar(SE_MENU_CURSOR_MOVE);
      ownerIndex = menu->sideIndex[0];
      menu->selectedIndexByMember[(u16)menu->memberIds[ownerIndex]] =
          state.selectedIndex;
      if (gInput.repeat & KEY_R)
        ownerIndex++;
      else
        ownerIndex--;
      for (i = 0; i < INVENTORY_SLOTS; i++)
        owner->inventory[i] = savedInventory[i];
      ownerIndex = (ownerIndex + menu->partyCount) % menu->partyCount;
      menu->selectedMember = (u16)menu->memberIds[ownerIndex];
      menu->sideMember[0] = (u16)menu->memberIds[ownerIndex];
      menu->sideIndex[0] = ownerIndex;
      Menu_ReloadMemberPortrait(menu, (u16)menu->memberIds[ownerIndex]);
      refresh = 2;
    }
  }
  WindowSprite_PrepareForRedrawFar(menu->infoWindow);
  WindowSprite_RedrawSavedRectFar(window);
  WindowSprite_ClearListFar(menu->questionWindow);
  return result;
}

#undef PAGE_ROWS
#undef STATUS_EQUIPMENT_CURSOR_X

/// Writes the four equipment slot labels (Weapon, Head, Shield and Chest, one a
/// 16-pixel row) and the names of the items worn in them into window; mode 0
/// also shows their icons at the right.
void ItemMenu_DrawCategory(s32 window, s32 ownerId, s32 mode) {
  struct ItemMenuState *menu = Ram_HeapSlots->menuState;
  u8 *items;

  ItemMenu_PositionCategory();
  ItemMenu_HideAllIcons();
  Text_DrawMessageAsTilesFar(MsgEquipSlotLabels, window, 0, 0);
  Text_DrawMessageAsTilesFar(MsgEquipSlotLabels + 1, window, 0, 32);
  Text_DrawMessageAsTilesFar(MsgEquipSlotLabels + 2, window, 0, 16);
  Text_DrawMessageAsTilesFar(MsgEquipSlotLabels + 3, window, 0, 48);
  items = (u8 *)menu->items;
  ItemMenu_DrawEquippedItemNames(window, items);
  if (mode == 0) {
    wait(1);
    ItemMenu_DrawIcons((u16 *)items, 1);
    ItemMenu_ArrangeCategoryItemIcons((u16 *)items);
  }
}

/// Writes the name of each equipped item of the inventory into window, below
/// its slot's label: the weapon, the helm, the shield, then the armor.
void ItemMenu_DrawEquippedItemNames(s32 window, u16 *items) {
  s32 i;
  s32 item;

  for (i = 0; i < INVENTORY_SLOTS; i++) {
    if (items[i] & INVENTORY_EQUIPPED) {
      item = items[i] & INVENTORY_ITEM_ID_MASK;
      switch (Item_GetDirectFar(item)->type) {
      case ITEM_TYPE_WEAPON:
        Text_DrawMessageAsTilesFar(item + MsgItemName, window, 16, 8);
        break;
      case ITEM_TYPE_ARMOR:
        Text_DrawMessageAsTilesFar(item + MsgItemName, window, 16, 56);
        break;
      case ITEM_TYPE_SHIELD:
        Text_DrawMessageAsTilesFar(item + MsgItemName, window, 16, 40);
        break;
      case ITEM_TYPE_HELM:
        Text_DrawMessageAsTilesFar(item + MsgItemName, window, 16, 24);
        break;
      }
    }
  }
}

/// Places each of the 32 entry icons on the grid.
void Menu_PlaceEntryActorsInGrid(s32 originX, s32 originY, s32 columns) {
  s32 entryIndex;
  struct WindowSprite *icon;
  struct WindowSprite **icons;

  for (entryIndex = 0,
      icons = ((struct ItemMenuState *)Ram_HeapSlots->menuState)->entryIcons;
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
  Icon_PrepareActor(icon);
}

/// Places the icon of each worn weapon, armor, shield and helm at x 216, on its
/// slot's row of ItemMenu_DrawCategory's window; the others stay off screen.
void ItemMenu_ArrangeCategoryItemIcons(u16 *items) {
  struct ItemMenuState *menu;
  struct WindowSprite *icon;
  s32 slotIndex;

  menu = Ram_HeapSlots->menuState;
  ItemMenu_PositionCategory();
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
        Icon_PrepareActor(icon);
      }
    }
  }
}

/// Moves all 32 entry icons off screen, to (248, 168), before a screen lays
/// them out again.
void ItemMenu_PositionCategory(void) {
  struct ItemMenuState *menu = Ram_HeapSlots->menuState;
  s32 x = MENU_ICON_HIDDEN_X;
  struct WindowSprite **entry = menu->entryIcons;
  s32 y = MENU_ICON_HIDDEN_Y;
  s32 remaining = MENU_ENTRY_ICONS - 1;

  do {
    struct WindowSprite *object = *entry++;

    if (object != 0) {
      object->x = x;
      object->y = y;
      Icon_PrepareActor(object);
    }
    remaining--;
  } while (remaining >= 0);
}
