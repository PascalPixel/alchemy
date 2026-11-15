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

// A bit over the page mode of StatusScreen_DrawStatusAilments and
// ItemMenu_DrawMemberStatus: draw without clearing the window first, as for a
// window just opened.
#define STATUS_PAGE_NO_CLEAR 0x100

/// The worn items' rows in ItemMenu_DrawCategory's window, 16 pixels apart:
/// weapon, helm, shield and armor from the top. A slot's label stands on its
/// row, its item's name a line below, and its icon at EQUIPPED_ICON_X on
/// screen.
#define EQUIPPED_ROW_Y(row) ((row) * 16)
#define EQUIPPED_ICON_X 216
#define EQUIPPED_ICON_Y(row) (EQUIPPED_ROW_Y(row) + 32)

/// The status page's four element columns, 4 tiles apart from tile 15, where
/// each element's icon stands; its figures end left of it.
#define STATUS_ELEMENT_X(element) (((element) * 4 + 15) * 8)

void StatusScreen_Initialize(s32 unusedA, s32 unusedB, s32 unusedC,
                             s32 unusedD);

s32 PartyMemberChooser_Run(void);

/// Lets the player choose one party member, over the field. Returns the chosen
/// party member's id, or -1 when the player backs out. Owns the menu work
/// (HEAP_SLOT_MENU) while it runs and frees it before returning.
s32 PartyMemberChooser_Open(void) {
  struct StatusScreenWork *state =
      (struct StatusScreenWork *)HeapSlot_Alloc(HEAP_SLOT_MENU, MENU_WORK_SIZE);
  s32 result;
  s32 index;

  Ram_HeapSlots->actorSystem->suspended = 1;
  Window_DrawBorderFar(0, 0, 30, 20);
  wait(1);
  Window_InitializeWork(0);

  state->partyCount = Party_ListMembersFar(state->memberIds);
  StatusScreen_Initialize(0, 3, 0, 7);
  state->selectorWindow = Window_CreateFar(13, 0, 17, 5, WINDOW_FLAG_INSTANT);
  {
    struct {
      u16 value;
    } y;

    y.value = 26;
    for (index = 3; index >= 0; index--)
      state->memberY[index] = y.value;
  }
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
  struct StatusScreenWork *work = Ram_HeapSlots->menuWork;
  s32 result = 0;

  /* FAKEMATCH(dead): the slot's clearing in both arms of a test. */
  if ((s32)work < 0)
    work->selectedSlots[0] = result;
  else
    work->selectedSlots[0] = result;
  if (StatusScreen_SelectMember(0) == -1)
    result = -1;
  else
    result = work->selectedIds[0];
  return result;
}

s32 StatusScreen_RunPages(void);

void MenuIcon_LoadSlots(void);
void MenuIcon_ReleaseSlots(void);

s32 StatusScreen_Run(void) {
  struct StatusScreenWork *state =
      (struct StatusScreenWork *)HeapSlot_Alloc(HEAP_SLOT_MENU, 0x0cec);
  void *palette = Heap_AllocEwram(64);
  void *tiles = Heap_AllocEwram(0x2800);
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
  saved = *(u16 *)&transition->active;
  transition->active = 17;
  Menu_SuspendFieldTasks();
  Window_InitializeWork(0);
  page = &state->page;
  *page = 0;
  state->partyCount = Party_ListMembersFar(state->memberIds);
  StatusScreen_Initialize(0, 3, 0, 7);
  Iwram_CopyWords(palette, (void *)BG_PLTT, 64);
  Iwram_CopyWords(tiles, (void *)BG_CHAR_BLOCK(1), 0x2800);
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
  Iwram_CopyWords((void *)BG_CHAR_BLOCK(1), tiles, 0x2800);
  Heap_Release(tiles);
  Heap_Release(palette);
  ((struct WindowWork *)heap->windowWork)->holdTilemapUpload = 1;
  ItemMenu_Close();
  Window_DrawBorderFar(0, 0, 30, 20);
  HeapSlot_Release(HEAP_SLOT_MENU);
  heap->actorSystem->suspended = 0;
  transition->active = saved;
  wait(1);
  Window_EraseBorderRectFar(0, 0, 30, 20);
  ((struct WindowWork *)heap->windowWork)->holdTilemapUpload = 0;
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
  struct StatusScreenWork *state =
      (struct StatusScreenWork *)Ram_HeapSlots->menuWork;
  s32 step = STATUS_STEP_MEMBER;
  s32 finished = step;
  s32 result = 0;
  u32 changed;

  while (!finished && !GameFlag_Get(FLAG_MENU_START_PRESSED)) {
    switch (step) {
    case STATUS_STEP_MEMBER:
      state->selectedSlots[0] = finished;
      Task_Add(Menu_UpdateEntryObjectTransforms, TASK_PASS_VBLANK);
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
      changed = ~result;
      step = (-changed | changed) >> 31;
      step <<= 1;
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
      changed = ~result;
      step = (-changed | changed) >> 31;
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
  struct StatusScreenWork *menu;
  struct WindowSprite *cursor;
  s32 result;
  s32 index;

  menu = (struct StatusScreenWork *)Ram_HeapSlots->menuWork;
  index = menu->memberIndex[slot];
  result = 0;
  cursor = menu->memberCursors[slot];
  cursor->motion = ICON_SHOWN;
  cursor->animStep = 0;
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

/// Shows every Djinn the party has found, a column of seven for each element,
/// until A, B or Select, then puts the screen back.
void DjinnMenu_ShowCurrentList(void) {
  struct DjinnMenuWork *menu = Ram_HeapSlots->menuWork;
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
    menu->slotX[i] = 32 + i * 56;
    menu->slotY[i] = 56;
    menu->memberY[i] = 26;
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
        for (row = 0; row <= 8; row++) {
          djinn =
              element * DJINN_PER_ELEMENT + (row + (page * 8 + page) + 7) % 18;
          if (GameFlag_Get(FLAG_DJINNI(djinn))) {
            Window_SetTilemapEntryFar(window, element + 0x1001, element * 7 + 1,
                                      row + 1, 0);
            Text_DrawMessageAsTilesFar(djinn + MsgDjinnName, window,
                                       element * 56 + 16, row * 8 + 8);
          }
        }
      }
      ((struct WindowWork *)Ram_HeapSlots->windowWork)->dirty = 1;
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
    menu->slotX[i] = 130 + i * 32;
    menu->slotY[i] = 128;
  }
  menu->cursor->motion = ICON_SHOWN;
  menu->secondCursor->motion = ICON_SHOWN;
  MenuIcon_SetActive(1);
  Sound_PlayFar(SE_MENU_CANCEL);
}

// The pressed keys, read again at each test.
#define KEYS_PRESSED (((struct InputState *)&gInput)->pressed)

void StatusScreen_ShowMemberStatus(s32 resource, s32 frame);

s32 PartyMemberChooser_MoveEntry(s32 selection, s32 direction);

/// The Status menu's party row: Left and Right choose a party member, showing
/// their status; L and R move the party member one place along the party order;
/// Select lists the Djinn found (once the first is caught, FLAG_DJINN_FIRST); A
/// chooses the party member and B backs out.
s32 PartyMemberChooser_RunRearrange(void) {
  struct StatusScreenWork *menu;
  s32 ownerIndex;
  s32 ownerCount;
  s32 page;
  s32 redraw;
  s32 result;
  s32 rowIndex;

  menu = Ram_HeapSlots->menuWork;
  ownerIndex = menu->memberIndex[0];
  ownerCount = menu->memberCount[0];
  redraw = 2;
  result = 0;
  page = menu->page;
  Unit_Get(menu->memberIds[ownerIndex]);
  for (rowIndex = 0; rowIndex < 4; rowIndex++) {
    menu->rowX[rowIndex] = 130 + rowIndex * 32;
    menu->rowY[rowIndex] = 128;
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
      Unit_Get(menu->memberIds[ownerIndex]);
      page = (page + 3) % 3;
      StatusScreen_ShowMemberStatus(menu->memberIds[ownerIndex], page);
      PsynergyMenu_CallIconRoutineWithValue(menu, menu->memberIds[ownerIndex]);
      if (redraw == 2) {
        Menu_CreateOwnerEntryObjects(ownerIndex / 4 * 4);
        wait(1);
      }
      Menu_HighlightPageEntry(menu->memberWindow, ownerIndex, ownerCount);
      redraw = 0;
    }
    MenuCursor_Position((ownerIndex % 4) * 24 - 10, 16);
    wait(1);
    if (KEYS_PRESSED & KEY_A) {
      Sound_PlayFar(SE_MENU_CONFIRM);
      result = 1;
      break;
    }
    if (KEYS_PRESSED & KEY_B) {
      Sound_PlayFar(SE_MENU_CANCEL);
      result = -1;
      break;
    }
    if (gInput.repeat & KEY_R) {
      if (PartyMemberChooser_MoveEntry(ownerIndex, 1)) {
        Sound_PlayFar(SE_MENU_CONFIRM);
        ownerIndex++;
        Menu_CreateOwnerEntryObjects(ownerIndex / 4 * 4);
        Menu_HighlightPageEntry(menu->memberWindow, ownerIndex, ownerCount);
      } else {
        Sound_PlayFar(SE_MENU_ERROR);
      }
      wait(1);
    } else if (gInput.repeat & KEY_L) {
      if (PartyMemberChooser_MoveEntry(ownerIndex, 0)) {
        Sound_PlayFar(SE_MENU_CONFIRM);
        ownerIndex--;
        Menu_CreateOwnerEntryObjects(ownerIndex / 4 * 4);
        Menu_HighlightPageEntry(menu->memberWindow, ownerIndex, ownerCount);
      } else {
        Sound_PlayFar(SE_MENU_ERROR);
      }
      wait(1);
    } else if ((KEYS_PRESSED & KEY_SELECT) &&
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

#undef KEYS_PRESSED

void Menu_PlaceEntryActorsInGrid(s32 x, s32 y, s32 columns);

/// The party member chooser (menu page 3): Left and Right choose a party
/// member, showing their status and Psynergy icons; A chooses and B backs out.
s32 PsynergyMenu_ChooseMember(void) {
  struct StatusScreenWork *menu =
      (struct StatusScreenWork *)Ram_HeapSlots->menuWork;
  s32 selection = menu->memberIndex[0];
  s32 count = menu->memberCount[0];
  s32 pending = 2;
  s32 page = menu->page;
  s32 result;

  Unit_Get(menu->memberIds[selection]);
  WindowSprite_RedrawSavedRectFar(menu->selectorWindow);
  Text_DrawMessageAsTilesFar((s32)MsgChooseCharacter, menu->selectorWindow, 0,
                             0);
  Text_DrawMessageAsTilesFar((s32)MsgChooseCharacter + 1, menu->selectorWindow,
                             0, 16);
  while (1) {
    // PERM
    if (pending) {
      selection = (selection + count) % count;
      Unit_Get(menu->memberIds[selection]);
      page = (page + 3) % 3;
      StatusScreen_ShowMemberStatus(menu->memberIds[selection], page);
      PsynergyMenu_CallIconRoutineWithValue(menu, menu->memberIds[selection]);
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
      Menu_HighlightPageEntry(menu->memberWindow, selection, count);
    }
    MenuCursor_Position((selection % 4) * 24 - 10, 16);
    wait(1);
    if (((struct InputState *)&gInput)->pressed & KEY_A) {
      Sound_PlayFar(SE_MENU_CONFIRM);
      result = 1;
      break;
    }
    if (((struct InputState *)&gInput)->pressed & KEY_B) {
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
  struct StatusScreenWork *menu = Ram_HeapSlots->menuWork;
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

  {
    u32 zero = 0;
    u32 *clear = &reordered[13];

    do {
      *clear = zero;
      clear--;
    } while ((s32)clear >= (s32)reordered);
  }
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

void Menu_PlaceOwnerEntryObjects(struct RenderInput *window, s32 originX,
                                 s32 originY);

/// Its callers pass four values it does not read.
void StatusScreen_Initialize(s32 unusedA, s32 unusedB, s32 unusedC,
                             s32 unusedD) {
  struct StatusScreenWork *work = Ram_HeapSlots->menuWork;

  Menu_PlaceOwnerEntryObjects(MenuCursor_Create(work), 2, 2);
  work->unopenedWindow = 0;
  work->statusWindow = 0;
  work->helpWindow = 0;
  work->actionWindow = 0;
  work->selectedColumn = 0;
  work->selectedRow = 0;
  work->columnCount = 8;
  work->rowCount = 2;
}

/// The character menu work block, as this function reads it.
typedef struct {
  u8 padding[15];
  u8 field0f;
} Object0f;

void StatusScreen_DrawStatusAilments(s32, s32,
                                     s32); ///< draws the status ailments

/// Creates the status window and its side object the first time, spawns the
/// icon entries in mode 3, then draws the status ailments (with 0x100 when the
/// window was just made). The work block holds the window at 0x28, the object
/// at 0x170 and the mode at 0x21c. frame is not used.
void StatusScreen_ShowMemberStatus(s32 resource, s32 frame) {
  s32 created;
  s32 handle;
  void *object;
  struct StatusScreenWork *state;

  state = Ram_HeapSlots->menuWork;
  created = 0;
  handle = state->statusWindow;
  if (handle == 0) {
    created = Window_UpdateOrCreate(&state->statusWindow, 0, 5, 0x1E, 0xF, 2);
    handle = state->statusWindow;
  }
  if (created != 0) {
    object = Portrait_CreateFar(resource, 0, 0, handle, 0, 0);
    state->statusCursor = object;
    if ((((Object0f *)object)->field0f = OAM_DEPTH_WINDOW, state->page) ==
        STATUS_SCREEN_PAGE_CHOOSER) {
      Menu_SpawnIconEntries(state, handle);
    }
    StatusScreen_DrawStatusAilments(handle, resource, STATUS_PAGE_NO_CLEAR);
    return;
  }
  StatusScreen_DrawStatusAilments(handle, resource, 0);
}

// The pressed keys, read again at each test.
#define KEYS_PRESSED (((struct InputState *)&gInput)->pressed)

// The switch-characters help follows the A-button label on the help line:
// Japanese starts it 64 pixels in, the other editions 80.
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

/// The party member's status page.
s32 StatusScreen_SelectCommand(void) {
  struct StatusScreenWork *menu;
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

  menu = Ram_HeapSlots->menuWork;
  pane = 0;
  selected = 0;
  result = 0;
  window = menu->statusWindow;
  Window_UpdateOrCreate(&menu->helpWindow, 0, 0, 30, 5, 2);
  helpWindow = menu->helpWindow;
  if (Party_SumDjinnCountsFar(-1) != 0)
    hasDjinn = 1;
  else
    hasDjinn = 0;
  for (i = 3; i >= 0; i--)
    menu->rowY[i] = 104;
  done = 0;
  MenuCursor_Slide(-10, 88);

  while (done == 0 && !GameFlag_Get(FLAG_MENU_START_PRESSED)) {
    Unit_Get(menu->selectedIds[0]);
    StatusScreen_DrawStatusAilments(window, menu->selectedIds[0], 1);
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
      if (KEYS_PRESSED & KEY_A) {
        Sound_PlayFar(SE_MENU_CONFIRM);
        done = 1;
        result = 1;
        break;
      }
      if (KEYS_PRESSED & KEY_B) {
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
        PsynergyMenu_CallIconRoutineWithValue(menu, menu->memberIds[member]);
        break;
      }
    }
  }
  WindowSprite_PrepareForRedrawFar(menu->helpWindow);
  WindowSprite_RedrawSavedRectFar(window);
  for (i = 3; i >= 0; i--)
    menu->rowY[i] = -16;
  return result;
}

#undef KEYS_PRESSED
#undef SWITCH_CHARACTER_HELP_X

/// Highlights (invert 0) or unhighlights (invert 1) the selected entry of the
/// status page: in side 0 the selected status row, as wide as its label
/// (gStatusScreen_CursorWidths); in side 1 one of the first four status rows
/// or of the rows below.
void StatusScreen_DrawSelectionCursor(s32 mode, s32 selected, u8 *entries,
                                      s32 invert) {
  struct StatusScreenWork *menu = Ram_HeapSlots->menuWork;
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
          width = gStatusScreen_CursorWidths[index];
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
/// revive a party member who is down) into window target, or that the party
/// member is well when they have no status.
void StatusScreen_DrawSelectionLabels(s32 target, s32 selected,
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

#if (LANGUAGE == LANGUAGE_GERMAN) || (LANGUAGE == LANGUAGE_SPANISH) ||         \
    (LANGUAGE == LANGUAGE_FRENCH) || (LANGUAGE == LANGUAGE_ITALIAN)
#define HELP_LENGTH 80
#else
#define HELP_LENGTH 64
#endif

u32 Unit_GetExperienceForLevelFar(s32 owner_id, s32 level);
void Message_CopyExpandedFar(s32 messageId, void *buffer, s32 length);
void Text_DrawWideStringAtOffsetFar(void *buffer, void *destination, s32 offset,
                                    s32 terminator);

/// Writes the help text of status entry messageVariant into destination: the
/// entries skip Djinn when the party has none (preserveVariant 0); the
/// experience entry says how many points the next level needs, or, at level 99,
/// gives the plain experience text. The text is copied into a heap buffer of
/// STATUS_HELP_TEXT_MAX characters, freed before returning.
void StatusScreen_ShowMemberProgressMessage(void *window, s32 help, s32 full) {
  u8 *menu = Ram_HeapSlots->menuWork;
  u8 *buffer;

  if (full == 0 && help > 3)
    help++;
  if (help == 1) {
    u8 *owner = menu + 0x216;
    struct Unit *state = Unit_Get(*owner);

    if (state->level == UNIT_LEVEL_MAX)
      help = 8;
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

extern const u8 gMenu_LvString[];
extern const u8 gMenu_SlashString[];

void ItemMenu_DrawMemberStatus(s32 window, s32 owner, s32 flags);

void MenuPanel_DrawEntryList(struct RenderInput *panel, s32 x, s32 y,
                             const u8 *enabled);

s32 Unit_GetElementLevelFar(s32 ownerId, s32 element);

/// Draws party member memberId's status page into window: the top rows
/// (ItemMenu_DrawMemberStatus), the status list or "Normal", and except on the
/// chooser page the Djinn counts by element and, in mode 1, the elemental
/// levels, Power and Resist. With STATUS_PAGE_NO_CLEAR in mode the window is
/// not cleared first.
void StatusScreen_DrawStatusAilments(s32 window, s32 ownerId, s32 mode) {
  struct StatusScreenWork *menu = Ram_HeapSlots->menuWork;
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
  if ((mode & 0xff) == 1)
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
  if (mode == 1 || hasDjinn == 1) {
    Window_SetTilemapEntryFar(window, 1, 15, djinnRow, TILEMAP_ENTRY_ICON);
    Window_SetTilemapEntryFar(window, 2, 19, djinnRow, TILEMAP_ENTRY_ICON);
    Window_SetTilemapEntryFar(window, 3, 23, djinnRow, TILEMAP_ENTRY_ICON);
    Window_SetTilemapEntryFar(window, 4, 27, djinnRow, TILEMAP_ENTRY_ICON);
  }
  if (hasDjinn)
    Text_DrawMessageAsTilesFar(MsgDjinnLabel, window, 64, (djinnRow + 1) * 8);
  if (mode == 1) {
    if (!hasDjinn)
      djinnRow--;
    MENU_DRAW_STATUS_LABEL(gMenu_LvString, window, 64, (djinnRow + 2) * 8);
    Text_DrawMessageAsTilesFar(MsgPowerLabel, window, 64, (djinnRow + 3) * 8);
    Text_DrawMessageAsTilesFar(MsgPowerLabel + 1, window, 64,
                               (djinnRow + 4) * 8);
  }
  for (element = 0; element <= 3; element++) {
    if (hasDjinn)
      Text_DrawNumberInWindowFar(unit->djinnOwnedCounts[element], 1, window,
                                 STATUS_ELEMENT_X(element), (djinnRow + 1) * 8);
    if ((mode & 0xff) == 1) {
      if (hasDjinn) {
        Text_DrawNumberInWindowFar(unit->djinnActiveCounts[element], 1, window,
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

extern const u8 gMenu_HpString[];
extern const u8 gMenu_PpString[];

// The stat column of the status page's top rows: three digits from 200 or
// four from 192, both ending at pixel 224.
#if LANGUAGE != LANGUAGE_JAPANESE
#define STATUS_STAT_DIGITS 3
#define STATUS_STAT_X 200
#else
#define STATUS_STAT_DIGITS 4
#define STATUS_STAT_X 192
#endif

/// Draws the top rows of party member memberId's status page into window: name,
/// class, level, experience, HP and PP on the left, Attack, Defense, Agility
/// and Luck on the right.
void ItemMenu_DrawMemberStatus(s32 window, s32 owner, s32 flags) {
  struct ItemMenuWork *menu;
  struct Unit *unit;

  menu = Ram_HeapSlots->menuWork;
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
  // The labels "Attack", "Defense", "Agility" and "Luck", 23 to 20 messages
  // before "Exp".
  Text_DrawMessageAsTilesFar((s32)MsgExpLabel - 23, window, 152, 0);
  Text_DrawMessageAsTilesFar((s32)MsgExpLabel - 22, window, 152, 8);
  Text_DrawMessageAsTilesFar((s32)MsgExpLabel - 21, window, 152, 16);
  Text_DrawMessageAsTilesFar((s32)MsgExpLabel - 20, window, 152, 24);
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

// The rows one page of the list shows: Japanese fits six, the other editions
// five.
#if LANGUAGE == LANGUAGE_JAPANESE
#define PAGE_ROWS 6
#else
#define PAGE_ROWS 5
#endif

/// Fills result with the item list of the party member in side index for the
/// equipment pages: their unit, the menu's item count, and the page and row of
/// the item chosen last for them, STATUS_EQUIP_PAGE_SIZE items to a page.
/// Returns 1.
s32 PagedList_Build(struct PagedList *result, s32 index) {
  struct ItemMenuWork *menu = Ram_HeapSlots->menuWork;
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

extern u32 gMenu_PatternRowMasks[][2];

/// Builds two sets of six tile pairs (24 8x8 tiles) in background tile block
/// 1 at 0x06006000: each pair filled with colour 4, then rows 1 to 7 XORed
/// with a pair of row masks (gMenu_PatternRowMasks), the first set's masks
/// clamped to the row so its patterns grow row by row, the second set skipping
/// row 1. The mask table is declared without const: as const its second load
/// is lifted above the first store.
void Menu_BuildPatternTiles(void) {
  s32 set;
  s32 n;

  for (set = 0; set < 2; set++) {
    for (n = 0; n < 6; n++) {
      u32 *tile = (u32 *)BG_SCREEN_BLOCK(12) + set * 0x60 + n * 0x10;
      s32 x;

      Iwram_FillWordsEntry(tile, 64, TEXT_CANVAS_BACKGROUND_WORD);
      for (x = 1; x <= 7; x++) {
        s32 mi = n;

        if (set == 1 && x <= 1)
          continue;
        if (set == 0 && n > x - 2) {
          mi = x - 2;
          if (mi < 0)
            mi = 0;
        }
        tile[x] ^= gMenu_PatternRowMasks[mi][0];
        tile[x + 8] ^= gMenu_PatternRowMasks[mi][1];
      }
    }
  }
}

#define RANGE_ICON_TILE 0xf300

/// Draws a Psynergy range icon at tile (x, y) of window: three tiles from pair
/// range of the range icon tiles (Menu_BuildPatternTiles), the side tile on
/// both ends, mirrored on the left, and the centre tile between. range is the
/// ability's range less one (0 for the aimed unit alone), or 11 for
/// ABILITY_RANGE_ALL (PsynergyMenu_DrawListPage).
void PsynergyMenu_DrawRange(s32 window, s32 x, s32 y, s32 range, s32 unused) {
  s32 n;
  s32 tile;

  n = range * 2;
  tile = n + RANGE_ICON_TILE + 1;
  Window_SetTilemapEntryFar(window, TILE_HFLIP | tile, x, y, 0);
  Window_SetTilemapEntryFar(window, n + RANGE_ICON_TILE, x + 1, y, 0);
  Window_SetTilemapEntryFar(window, tile, x + 2, y, 0);
}

#undef RANGE_ICON_TILE

// The psynergy menu work block, as this function reads it.
#define ACTION_ID_MASK PSYNERGY_ABILITY_MASK

// The Japanese page lists six abilities rather than five, a row higher, and
// says where the chosen ability can be used in short lines stacked at the
// window's left edge rather than in one sentence across it. FIRST_TILE_ROW is
// the tile row of the page's first ability; USABLE_TOP, USABLE_RIGHT and
// USABLE_BOTTOM bound that text in pixels inside the window.
#if LANGUAGE == LANGUAGE_JAPANESE
#define PAGE_ROWS 6
#define FIRST_TILE_ROW 1
#define USABLE_TOP 72
#define USABLE_RIGHT 64
#define USABLE_BOTTOM 96
// まちやダンジョン ("towns and dungeons"), the message two after MsgExpLabel.
#define MSG_USABLE_IN_FIELD ((s32)MsgExpLabel + 2)
// せんとう ("battle"), the message after MsgExpLabel.
#define MSG_USABLE_IN_BATTLE ((s32)MsgExpLabel + 1)
// でつかえます ("can be used"), the message before MsgDjinnListHelp.
#define MSG_CAN_BE_USED ((s32)MsgDjinnListHelp - 1)
#else
#define PAGE_ROWS 5
#define FIRST_TILE_ROW 2
#define USABLE_TOP 96
#define USABLE_RIGHT 224
#define USABLE_BOTTOM 104
#endif

#if LANGUAGE == LANGUAGE_JAPANESE
extern u8 MsgExpLabel[];
extern u8 MsgDjinnListHelp[];
#else
extern u8 MsgUsableInBattle[];
extern u8 MsgUsableInField[];
// In the English release its text is MsgUsableInField's again.
extern u8 MsgUsableAnywhere[];
#endif

/// Updates the Psynergy page for the row under the cursor: the Psynergy's
/// description in the info window, where it can be used (in the field, in
/// battle, or anywhere), and the row highlights with the range icons.
s32 PsynergyMenu_DrawRangePage(s32 window, s32 unused,
                               struct PagedList *state) {
  struct PsynergyMenuWork *menu;
  struct BattleAct *ability;
  s32 row;
  s32 base;

  menu = (struct PsynergyMenuWork *)Ram_HeapSlots->menuWork;
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
    if (ability->fieldEffect != 0 ||
        (ability->flags & ACTION_USABLE_IN_FIELD) != 0) {
      Text_DrawMessageAsTilesFar(MSG_USABLE_IN_FIELD, window, 0, USABLE_TOP);
      row = 1;
    }
    if ((ability->flags & ACTION_USABLE_IN_BATTLE) != 0) {
      Text_DrawMessageAsTilesFar(MSG_USABLE_IN_BATTLE, window, 0,
                                 row * 8 + USABLE_TOP);
      row++;
    }
    Text_DrawMessageAsTilesFar(MSG_CAN_BE_USED, window, 0,
                               row * 8 + USABLE_TOP);
#else
    if (ability->fieldEffect != 0 ||
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

// The psynergy menu work block, as this function reads it.
#define PSY_LIST_OFFSET 0x1c4
#define ACT_ID_MASK PSYNERGY_ABILITY_MASK
#define OWNER_CLASS_MSG_OFFSET 0x129
#define OWNER_LEVEL_OFFSET 15

// The Japanese page lists six abilities rather than five, a row higher, and
// lays its heading, names, costs, page icons and "no Psynergy" message out on
// its own columns.
#if LANGUAGE == LANGUAGE_JAPANESE
#define PAGE_ROWS 6
#define PAGE_ICONS_X 86
#define PAGE_ICONS_Y 53
#define HEADING_X 144
#define FIRST_ROW_Y 8
#define NAME_X 96
#define COST_X 168
#define FIRST_RANGE_ROW 1
#define EMPTY_MESSAGE_Y 8
#else
#define PAGE_ROWS 5
#define PAGE_ICONS_X 80
#define PAGE_ICONS_Y 58
#define HEADING_X 176
#define FIRST_ROW_Y 16
#define NAME_X 88
#define COST_X 176
#define FIRST_RANGE_ROW 2
#define EMPTY_MESSAGE_Y 17
#endif

/// The Psynergy menu's list page: each ability's name, PP cost and range, "no
/// Psynergy" when empty, then the owner's name, class and level.
s32 PsynergyMenu_DrawListPage(s32 window, s32 unused,
                              const struct PagedList *res) {
  struct PsynergyMenuWork *menu = Ram_HeapSlots->menuWork;
  u8 *owner;
  u32 first;
  u32 rows;
  u8 row;
  s32 offset;

  (void)unused;

  owner = Unit_Get(menu->memberIds[0]);

  WindowSprite_RedrawSavedRectFar(window);

  first = res->page * PAGE_ROWS;
  rows = (u8)(res->entryCount - first);
  if (rows > PAGE_ROWS) {
    rows = PAGE_ROWS;
  }

  Menu_SetPageIcons(PAGE_ROWS, first, window, PAGE_ICONS_X, PAGE_ICONS_Y);
  PagedList_DrawPageNumbers(window, res->entryCount, PAGE_ROWS, res->page, 28);

  Text_DrawMessageAsTilesFar((s32)MsgPsynergyChooseTarget + 2, window,
                             HEADING_X, 0);

  row = 0;
  if (rows > row) {
    offset = (s32)(first * 2) + PSY_LIST_OFFSET;
    do {
      struct BattleAct *act;
      s32 msg;
      s32 y;
      s32 range;
      act = Ability_Get(ACT_ID_MASK & *(u16 *)(offset + (s32)menu));
      msg = (*(u16 *)(offset + (s32)menu) & ACT_ID_MASK) + (s32)MsgAbilityName;
      y = row * 16 + FIRST_ROW_Y;

      Text_DrawMessageAsTilesFar(msg, window, NAME_X, y);
      Text_DrawNumberAtOffsetFar(act->ppCost, 2, window, COST_X, y);

      range = act->range;
      if (range == 0xff) {
        range = 11;
      } else {
        range--;
      }
      PsynergyMenu_DrawRange(window, 25, row * 2 + FIRST_RANGE_ROW, range, 0);

      row++;
      offset += 2;
    } while (rows > row);
  }

  if (menu->psynergyCount == 0) {
    Text_DrawMessageAsTilesFar((s32)MsgPsynergyChooseForR - 1, window, 96,
                               EMPTY_MESSAGE_Y);
  }

  Text_DrawStringAtOffsetFar(owner, window, 40, 0);
  Text_DrawMessageAsTilesFar(owner[OWNER_CLASS_MSG_OFFSET] + (s32)MsgClassName,
                             window, 0, 32);
  // Spanish and French draw the level label at an offset, as the owner's name.
#if LANGUAGE == LANGUAGE_SPANISH || LANGUAGE == LANGUAGE_FRENCH
  Text_DrawStringAtOffsetFar(gMenu_LvString, window, 0, 48);
#else
  Text_DrawStringInWindowFar(gMenu_LvString, window, 0, 48);
#endif
  Text_DrawNumberInWindowFar(owner[OWNER_LEVEL_OFFSET], 2, window, 24, 48);

  return 1;
}

#undef PAGE_ROWS
#undef PAGE_ICONS_X
#undef COST_X
#undef FIRST_RANGE_ROW
#undef EMPTY_MESSAGE_Y

// The pressed keys, read again at each test.
#define KEYS_PRESSED (((struct InputState *)&gInput)->pressed)

// The rows of a page of the list and where the cursor points at its first row:
// the Japanese page lists six abilities, a row higher.
#if LANGUAGE == LANGUAGE_JAPANESE
#define PAGE_ROWS 6
#define CURSOR_X 58
#define CURSOR_Y 52
#else
#define PAGE_ROWS 5
#define CURSOR_X 55
#define CURSOR_Y 60
#endif

/// The Status menu's Psynergy page: the party member's Psynergy, PAGE_ROWS to a
/// page, each with its PP cost and range icon; the cursor's Psynergy is
/// described in the info window with where it can be used.
s32 PsynergyMenu_SelectAction(void) {
  struct PsynergyMenuWork *menu;
  s32 result;
  s32 window;
  s32 refresh;
  s32 listNeedsRedraw;
  s32 navigation;
  s32 ownerIndex;
  struct PagedList state;

  menu = Ram_HeapSlots->menuWork;
  window = menu->statusWindow;
  result = 0;
  refresh = 2;
  listNeedsRedraw = 1;
  Window_UpdateOrCreate(&menu->infoWindow, 0, 0, 30, 5, 2);
  Menu_SpawnIconEntries(menu, menu->messageWindow);
  Text_DrawMessageAsTilesFar(MsgSwitchCharacterHelp + 2, window, 0, -24);
  Text_DrawMessageAsTilesFar(MsgSwitchCharacterHelp, window, 64, -24);
  Menu_BuildPatternTiles();
  while (!GameFlag_Get(FLAG_MENU_START_PRESSED)) {
    if (refresh) {
      if (refresh == 2) {
        menu->psynergyCount = PsynergyMenu_CollectActions(
            Unit_Get(menu->memberIds[0]), menu->psynergies, 0);
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
    MenuCursor_Position(CURSOR_X, state.row * 16 + CURSOR_Y);
    if (navigation == 1) {
      refresh = 1;
      listNeedsRedraw = 1;
    }
    if (navigation == 0)
      refresh = 1;
    if (navigation == -1)
      refresh = 0;
    if (KEYS_PRESSED & KEY_A) {
      Sound_PlayFar(SE_MENU_CONFIRM);
      result = 1;
      break;
    }
    if (KEYS_PRESSED & KEY_B) {
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
      PsynergyMenu_CallIconRoutineWithValue(menu,
                                            menu->memberTable[ownerIndex]);
      refresh = 2;
    }
  }
  WindowSprite_PrepareForRedrawFar(menu->infoWindow);
  WindowSprite_RedrawSavedRectFar(window);
  WindowSprite_ClearListFar(menu->messageWindow);
  return result;
}

#undef KEYS_PRESSED
#undef PAGE_ROWS
#undef CURSOR_X
#undef CURSOR_Y

// The rows one page of the item list shows: Japanese fits six, the other
// editions five.
#if LANGUAGE == LANGUAGE_JAPANESE
#define PAGE_ROWS 6
#else
#define PAGE_ROWS 5
#endif

#define EQUIP_SLOT_X 2
// The equipment slots' highlight is a tile wider in the European editions.
#if LANGUAGE == LANGUAGE_ENGLISH || LANGUAGE == LANGUAGE_JAPANESE
#define EQUIP_SLOT_WIDTH 9
#else
#define EQUIP_SLOT_WIDTH 10
#endif

/// Updates the Status menu's equipment page for the row under the cursor: the
/// item's plain name in the info window, the row highlights, and in the
/// equipment window the highlight of the slot the item is worn in, if it is.
s32 ItemMenu_DrawEquipPage(s32 window, s32 unused, struct PagedList *state) {
  struct ItemMenuWork *menu;
  struct ItemDefinition *item;
  s32 base;
  s32 row;
  u32 entry;
  u32 mask;

  menu = Ram_HeapSlots->menuWork;
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

// The page icons, the item names, the "no items" message and the Attack and
// Defense figures, as each translation lays them out.
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

/// Draws one page of the member's items into window: five names a page, or
/// "no items", then the member's name, Attack and Defense.
s32 ItemMenu_DrawItemPage(s32 window, s32 unused, struct PagedList *state) {
  struct ItemMenuWork *menu = Ram_HeapSlots->menuWork;
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

// The pressed keys, read again at each test.
#define KEYS_PRESSED (((struct InputState *)&gInput)->pressed)

// The equip window's width in tiles, narrower in Japanese.
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

// The French list puts its cursor 4 pixels further right.
#if LANGUAGE == LANGUAGE_FRENCH
#define CURSOR_X 100
#else
#define CURSOR_X 96
#endif

s32 ItemMenu_SelectItem(void) {
  struct ItemMenuWork *menu;
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

  menu = Ram_HeapSlots->menuWork;
  window = menu->statusWindow;
  result = 0;
  refresh = 2;
  listNeedsRedraw = 1;
  owner = Unit_Get(menu->sideMember[0]);
  Window_UpdateOrCreate(&menu->equipWindow, 0, 10, EQUIP_WINDOW_WIDTH, 10, 2);
  Menu_SpawnIconEntries(menu, menu->questionWindow);
  Text_DrawMessageAsTilesFar(MsgSwitchCharacterHelp + 3, window, 0, -24);
  Text_DrawMessageAsTilesFar(MsgSwitchCharacterHelp, window, 64, -24);
  while (!GameFlag_Get(FLAG_MENU_START_PRESSED)) {
    if (refresh) {
      if (refresh == 2) {
        owner = Unit_Get(menu->sideMember[0]);
        menu->itemCount = ItemMenu_CollectItems(owner, menu->items, 0);
        WindowSprite_RedrawSavedRectFar(window);
        for (i = 0; i <= 14; i++)
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
    MenuCursor_Position(CURSOR_X, state.row * 16 + 52);
    if (navigation == 1) {
      refresh = 1;
      listNeedsRedraw = 1;
    }
    if (navigation == 0)
      refresh = 1;
    if (navigation == -1)
      refresh = 0;
    if (KEYS_PRESSED & KEY_A) {
      Sound_PlayFar(SE_MENU_CONFIRM);
      result = 1;
      break;
    }
    if (KEYS_PRESSED & KEY_B) {
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
      for (i = 0; i <= 14; i++)
        owner->inventory[i] = savedInventory[i];
      ownerIndex = (ownerIndex + menu->partyCount) % menu->partyCount;
      menu->selectedMember = (u16)menu->memberIds[ownerIndex];
      menu->sideMember[0] = (u16)menu->memberIds[ownerIndex];
      menu->sideIndex[0] = ownerIndex;
      PsynergyMenu_CallIconRoutineWithValue(menu,
                                            (u16)menu->memberIds[ownerIndex]);
      refresh = 2;
    }
  }
  WindowSprite_PrepareForRedrawFar(menu->infoWindow);
  WindowSprite_RedrawSavedRectFar(window);
  WindowSprite_ClearListFar(menu->questionWindow);
  return result;
}

#undef PAGE_ROWS
#undef CURSOR_X

/// Writes the four equipment slot labels (Weapon, Head, Shield and Chest, one a
/// 16-pixel row) and the names of the items worn in them into window; mode 0
/// also shows their icons at the right.
void ItemMenu_DrawCategory(s32 window, s32 ownerId, s32 mode) {
  struct ItemMenuWork *menu = Ram_HeapSlots->menuWork;
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

/// Writes the name of each equipped item of the inventory into window, on the
/// row of its equipment slot: item type 1 on the first row, 4 on the second,
/// 3 on the third and 2 on the fourth.
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
      icons = ((struct ItemMenuWork *)Ram_HeapSlots->menuWork)->entryIcons;
       entryIndex <= MENU_ENTRY_ICONS - 1; entryIndex++) {
    icon = *icons++;
    if (icon != NULL) {
      Menu_PlaceEntryActorInGrid(icon, entryIndex, originX, originY, columns);
    }
  }
}

/// Places icon at cell index (an index above 31 is taken as 0) of a grid of
/// 16x16 cells, columns wide, from (originX, originY), and shows it.
void Menu_PlaceEntryActorInGrid(struct WindowSprite *icon, s32 index,
                                s32 originX, s32 originY, s32 columns) {
  s32 entryIndex;

  entryIndex = index;
  if (entryIndex > MENU_ENTRY_ICONS - 1) {
    entryIndex = 0;
  }
  icon->y = (entryIndex / columns * 16) + originY;
  icon->x = (entryIndex % columns * 16) + originX;
  Icon_PrepareActor(icon);
}

/// Places the icon of each worn weapon, armor, shield and helm at x 216, on its
/// slot's row of ItemMenu_DrawCategory's window; the others stay off screen.
void ItemMenu_ArrangeCategoryItemIcons(u16 *items) {
  struct ItemMenuWork *menu;
  struct WindowSprite *icon;
  s32 slotIndex;

  menu = Ram_HeapSlots->menuWork;
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
  struct ItemMenuWork *menu = Ram_HeapSlots->menuWork;
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
