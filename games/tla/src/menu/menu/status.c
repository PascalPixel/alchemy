/// @file
/// The party member chooser and the Status menu: its party member row and its
/// status, Psynergy, range and equipment pages.
// Part of menu.c.

#include "../menu_internal.h"

void StatusScreen_Initialize(s32 unused0, s32 unused1, s32 unused2, s32 unused3);

s32 PartyMemberChooser_Run(void);

/// Lets the player choose one party member, over the field (the Colosso scenes
/// use it, through MENU.S, and so does the debug test). Returns the chosen
/// party member's id, or -1 when the player backs out. Owns the menu work
/// (HEAP_SLOT_MENU) while it runs and frees it before returning.
s32 PartyMemberChooser_Open(void) {
  struct OwnerMenuState *state =
      (struct OwnerMenuState *)HeapSlot_Alloc(220, 0x0cec);
  s32 result;
  s32 index;

  Ram_HeapSlots->objectSystem->suspended = 1;
  Window_DrawBorderFar(0, 0, 30, 20);
  wait(1);
  Window_InitializeWork(0);

  state->partyCount = Party_ListMembersFar(state->ownerIds);
  StatusScreen_Initialize(0, 3, 0, 7);
  state->selectorWindow = Window_CreateFar(13, 0, 17, 5, 2);
  {
    struct {
      u16 value;
    } y;

    y.value = 26;
    for (index = 3; index >= 0; index--)
      state->rowPositions[index] = y.value;
  }
  state->page = 3;

  result = PartyMemberChooser_Run();

  WindowSprite_ClearListFar(state->statusWindow);
  ItemMenu_Close();
  Ram_HeapSlots->objectSystem->suspended = 0;
  wait(1);
  HeapSlot_Release(220);
  return result;
}

struct MenuRuntimeSelect {
  u8 unk000[0x168];
  u16 selectedSlots[1];
  u8 unk16a[0xac];
  u8 selectedIds[1];
};

s32 StatusScreen_SelectMember(s32 slot);

/// The party member chooser's loop. Returns the chosen party member's id, or
/// -1.
s32 PartyMemberChooser_Run(void) {
  struct MenuRuntimeSelect *work =
      (struct MenuRuntimeSelect *)Ram_HeapSlots->menuRuntime;
  s32 result = 0;

  /* FAKEMATCH(dead): the cursor store in a once-only block, which keeps it in
   * source order, as in The Broken Seal's CharacterSelector_Run; as the plain
   * statement, 2 of 24 instructions differ in tla-en. */
  do {
    work->selectedSlots[0] = result;
  } while (0);
  if (StatusScreen_SelectMember(0) == -1)
    result = -1;
  else
    result = work->selectedIds[0];
  return result;
}

s32 StatusScreen_RunPages(void);

void Func_081053a8(void);
void MenuIcon_ReleaseSlots(void);

s32 StatusScreen_Run(void) {
  struct OwnerMenuState *state =
      (struct OwnerMenuState *)HeapSlot_Alloc(220, 0x0cec);
  void *palette = Heap_AllocEwram(64);
  void *tiles = Heap_AllocEwram(0x2800);
  s32 result = -1;
  struct HeapSlots *heap;
  struct ScreenTransitionState *transition;
  u32 saved;
  u16 *page;

  heap = Ram_HeapSlots;
  transition = heap->screenTransition;
  heap->objectSystem->suspended = 1;
  Window_DrawBorderFar(0, 0, 30, 20);
  wait(1);
  saved = *(u16 *)&transition->active;
  transition->active = 17;
  Menu_SuspendFieldTasks();
  Window_InitializeWork(0);
  page = &state->page;
  *page = 0;
  state->partyCount = Party_ListMembersFar(state->ownerIds);
  StatusScreen_Initialize(0, 3, 0, 7);
  /* FAKEMATCH(wrapper): the copies forwarded through item.c's CopyWords_
   * inline, which keeps each call's own constants; as direct Iwram_CopyWords
   * calls, 48 of 216 instructions differ in tla-en, CSE keeping 0x05000000 in
   * a saved register. */
  CopyWords_(palette, (void *)BG_PLTT, 64);
  CopyWords_(tiles, (void *)0x06004000, 0x2800);
  Palette_LightenHighlight(14);
  Dma_Set((void *)OBJ_PLTT, (void *)BG_PLTT, DMA_COPY_HALFWORDS(16), REG_DMA3);
  Dma_Set((void *)&BG_PLTT_COLOR(14, 4), (void *)&BG_PLTT_COLOR(0, 14),
          DMA_COPY_HALFWORDS(1), REG_DMA3);
  Dma_Set((void *)OBJ_PLTT, (void *)&BG_PLTT_COLOR(1, 0),
          DMA_COPY_HALFWORDS(16), REG_DMA3);
  Dma_Set((void *)&BG_PLTT_COLOR(15, 4), (void *)&BG_PLTT_COLOR(1, 14),
          DMA_COPY_HALFWORDS(1), REG_DMA3);
  Text_UseExtraTilesFar(1);
  state->selectorWindow = Window_CreateFar(13, 0, 17, 5, 2);
  if (Party_SumDjinnCountsFar(result) != 0)
    Func_081051a8();
  Link_DrawPageArrowsFar(0x06002500);
  Func_081053a8();
  Menu_CancelSoundReset();
  *page = 0;

  result = StatusScreen_RunPages();

  Menu_EnsureCancelSound();
  MenuIcon_ReleaseSlots();
  WindowSprite_ClearListFar(state->statusWindow);
  MenuIcon_ReleaseObjects();
  Menu_ResumeFieldTasks();
  Window_DrawBorderFar(0, 0, 30, 20);
  wait(1);
  Func_08038290();
  Text_UseExtraTilesFar(0);
  CopyWords_((void *)BG_PLTT, palette, 64);
  CopyWords_((void *)0x06004000, tiles, 0x2800);
  Heap_Release(tiles);
  Heap_Release(palette);
  ((struct WindowWork *)heap->windowTiles)->menuBusy = 1;
  ItemMenu_Close();
  Window_DrawBorderFar(0, 0, 30, 20);
  HeapSlot_Release(220);
  heap->objectSystem->suspended = 0;
  transition->active = saved;
  wait(1);
  Window_EraseBorderRectFar(0, 0, 30, 20);
  ((struct WindowWork *)heap->windowTiles)->menuBusy = 0;
  return result;
}

void MenuIcon_SetActive(s32 active);
s32 StatusScreen_SelectCommand(void);
s32 Func_080ffcd4(void);
s32 ItemMenu_SelectItem(void);

/// The Status menu's pages: choose a party member, then their status page, A
/// for their Psynergy page and A again for their equipment page, whose A goes
/// back round to the status page; B on any page goes back to the party member
/// row. Returns -1 when the player backs out of the party member row or presses
/// Start.
s32 StatusScreen_RunPages(void) {
  struct MenuRuntimeSelect *state =
      (struct MenuRuntimeSelect *)Ram_HeapSlots->menuRuntime;
  s32 step = 0;
  s32 finished = step;
  s32 result = 0;
  u32 changed;

  while (!finished && !GameFlag_Test(0x150)) {
    switch (step) {
    case 0:
      state->selectedSlots[0] = finished;
      Task_Add(Menu_UpdateEntryObjectTransforms, TASK_PASS_VBLANK);
      MenuIcon_SetActive(1);
      step = StatusScreen_SelectMember(0);
      Task_Remove(Menu_UpdateEntryObjectTransforms);
      if (step == -1) {
        result = step;
        finished = 1;
      }
      step = 1;
      break;
    case 1:
      MenuIcon_SetActive(1);
      result = StatusScreen_SelectCommand();
      changed = ~result;
      step = (-changed | changed) >> 31;
      step <<= 1;
      break;
    case 2:
      MenuIcon_SetActive(0);
      result = Func_080ffcd4();
      step = 0;
      if (result != -1)
        step = 3;
      break;
    case 3:
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

  if (GameFlag_Test(0x150))
    result = -1;

  return result;
}

struct OwnerCursor {
  u8 unk00[5];
  u8 active;
  u8 unk06[6];
  u16 animStep;
};

s32 PsynergyMenu_ChooseMember(void);
s32 PartyMemberChooser_RunRearrange(void);

/// The twin of The Broken Seal's CharacterMenu_SelectOwner.
s32 StatusScreen_SelectMember(s32 slot) {
  struct OwnerMenuState *menu;
  struct OwnerCursor *cursor;
  s32 result;
  s32 index;

  menu = (struct OwnerMenuState *)Ram_HeapSlots->menuRuntime;
  index = menu->tabIndex[slot];
  result = 0;
  cursor = menu->sideIcon[slot];
  cursor->active = 1;
  cursor->animStep = 0;
  WindowSprite_RedrawSavedRectFar(menu->auxiliaryWindow);
  if (GameFlag_Test(0x172))
    Window_DrawDividerLineFar(menu->auxiliaryWindow, 9, 1, 9, 3);
  Window_DrawDividerLineFar(menu->auxiliaryWindow, 0, 3, 12, 3);
  if (index == -1)
    menu->tabIndex[slot] = 0;
  else
    MenuCursor_Slide(index * 24 - 10, 16);
  if (menu->page == 3)
    result = PsynergyMenu_ChooseMember();
  else
    result = PartyMemberChooser_RunRearrange();
  cursor = menu->sideIcon[slot];
  Icon_PrepareActor(cursor);
  wait(1);
  return result;
}

void DjinnMenu_ShowCurrentList(void) {
  struct DjinnMenuWork *menu = Ram_HeapSlots->menuRuntime;
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
  Sound_PlayFar(112);
  redraw = 1;
  WindowSprite_RedrawSavedRectFar(menu->helpWindow);
  Text_DrawMessageAsTilesFar(MsgButtonReturn, menu->helpWindow, 0, 16);
  Text_DrawMessageFar(MsgCurrentDjinn, menu->helpWindow, -96, 132);
  menu->arrows[0]->active = 13;
  menu->arrows[1]->active = 13;
  MenuIcon_SetActive(0);
  menu->cursor->active = 13;
  menu->selector->active = 13;
  for (i = 0; i < 4; i++) {
    menu->rowX[i] = 32 + i * 56;
    menu->rowY[i] = 56;
    menu->memberY[i] = 26;
  }
  wait(1);
  while (!GameFlag_Test(0x150)) {
    if (redraw) {
      window = menu->window;
      WindowSprite_RedrawSavedRectFar(window);
      wait(1);
      Window_DrawDividerLineFar(window, 0, 11, 28, 11);
      Menu_DrawPageIndicator(window, pages, 1, page, 28);
      for (element = 0; element < 4; element++) {
        for (row = 0; row <= 8; row++) {
          djinn = element * 20 + (row + (page * 8 + page) + 7) % 18;
          if (GameFlag_Test(djinn + 48)) {
            Window_SetTilemapEntryFar(window, element + 0x1001,
                                        element * 7 + 1, row + 1, 0);
            Text_DrawMessageAsTilesFar(djinn + MsgDjinnName, window,
                                            element * 56 + 16, row * 8 + 8);
          }
        }
      }
      ((struct WindowWork *)Ram_HeapSlots->windowTiles)->dirty = 1;
      redraw = 0;
    }
    Link_DrawPageArrowsFar((void *)0x06002500);
    wait(1);
    if (gInput.pressed & 7)
      break;
    if (gInput.repeat & 0x30) {
      if (gInput.repeat & 0x10)
        page++;
      else
        page--;
      page = (page + pages) % pages;
      Sound_PlayFar(111);
      Input_ResetKeyRepeat();
      redraw = 1;
    }
  }
  WindowSprite_RedrawSavedRectFar(menu->window);
  WindowSprite_ClearListFar(menu->helpWindow);
  for (i = 0; i < 4; i++) {
    menu->rowX[i] = 130 + i * 32;
    menu->rowY[i] = 128;
  }
  menu->cursor->active = 1;
  menu->selector->active = 1;
  MenuIcon_SetActive(1);
  Sound_PlayFar(113);
}

// The pressed keys, read again at each test.
#define KEYS_PRESSED (((struct InputState *)&gInput)->pressed)

struct CharacterMenuState {
  u8 unk000[8];
  s32 selectedOwner;
  u8 unk00c[4];
  s32 rowWindow;
  u8 unk014[8];
  s8 ownerIndex;
  u8 unk01d;
  s8 ownerCount;
  u8 unk01f[9];
  s32 statusWindow;
  u8 unk02c[4];
  s32 helpWindow;
  u8 unk034[0xbc];
  s32 selectorWindow;
  u8 unk0f4[0x7c];
  struct MenuIcon *statusCursor;
  u8 unk174[4];
  struct MenuIcon *arrows[2];
  u8 unk180[0x84];
  u16 ownerIds[8];
  u8 unk214;
  u8 partyCount;
  u8 selectedIds[2];
  u8 unk218[4];
  u16 page;
  u8 unk21e[0x12];
  u16 rowX[4];
  s16 rowY[4];
};

void Menu_CreateWindowAndEntryObjects(s32 resource, s32 frame);

s32 PartyMemberChooser_MoveEntry(s32 selection, s32 direction);

s32 PartyMemberChooser_RunRearrange(void) {
  struct CharacterMenuState *menu;
  s32 ownerIndex;
  s32 ownerCount;
  s32 page;
  s32 redraw;
  s32 result;
  s32 rowIndex;

  menu = Ram_HeapSlots->menuRuntime;
  ownerIndex = menu->ownerIndex;
  ownerCount = menu->ownerCount;
  redraw = 2;
  result = 0;
  page = menu->page;
  Unit_Get(menu->ownerIds[ownerIndex]);
  for (rowIndex = 0; rowIndex < 4; rowIndex++) {
    menu->rowX[rowIndex] = 130 + rowIndex * 32;
    menu->rowY[rowIndex] = 128;
  }
  Palette_LightenHighlight(14);
  Dma_Set((void *)OBJ_PLTT, (void *)BG_PLTT, DMA_ENABLE << 16 | 16, REG_DMA3);
  Dma_Set((void *)&BG_PLTT_COLOR(14, 4), (void *)&BG_PLTT_COLOR(0, 14),
          DMA_ENABLE << 16 | 1, REG_DMA3);
  Dma_Set((void *)OBJ_PLTT, (void *)&BG_PLTT_COLOR(1, 0), DMA_ENABLE << 16 | 16,
          REG_DMA3);
  Dma_Set((void *)&BG_PLTT_COLOR(15, 4), (void *)&BG_PLTT_COLOR(1, 14),
          DMA_ENABLE << 16 | 1, REG_DMA3);
  while (!GameFlag_Test(0x150)) {
    if (redraw) {
      WindowSprite_RedrawSavedRectFar(menu->selectorWindow);
      Text_DrawMessageAsTilesFar(MsgRearrangeHelp, menu->selectorWindow, 0,
                                    0);
      if (GameFlag_Test(55))
        Text_DrawMessageAsTilesFar(MsgDjinnListHelp, menu->selectorWindow, 0,
                                      16);
      Text_DrawMessageAsTilesFar(MsgRearrangeHelp - 3, menu->selectorWindow,
                                    0, 8);
      ownerIndex = (ownerIndex + ownerCount) % ownerCount;
      Unit_Get(menu->ownerIds[ownerIndex]);
      page = (page + 3) % 3;
      Menu_CreateWindowAndEntryObjects(menu->ownerIds[ownerIndex], page);
      PsynergyMenu_CallIconRoutineWithValue(menu, menu->ownerIds[ownerIndex]);
      if (redraw == 2) {
        Menu_CreateOwnerEntryObjects(ownerIndex / 4 * 4);
        wait(1);
      }
      Menu_HighlightPageEntry(menu->rowWindow, ownerIndex, ownerCount);
      redraw = 0;
    }
    Func_080f8a44((ownerIndex % 4) * 24 - 10, 16);
    wait(1);
    if (KEYS_PRESSED & 1) {
      Sound_PlayFar(112);
      result = 1;
      break;
    }
    if (KEYS_PRESSED & 2) {
      Sound_PlayFar(113);
      result = -1;
      break;
    }
    if (gInput.repeat & 0x100) {
      if (PartyMemberChooser_MoveEntry(ownerIndex, 1)) {
        Sound_PlayFar(112);
        ownerIndex++;
        Menu_CreateOwnerEntryObjects(ownerIndex / 4 * 4);
        Menu_HighlightPageEntry(menu->rowWindow, ownerIndex, ownerCount);
      } else {
        Sound_PlayFar(114);
      }
      wait(1);
    } else if (gInput.repeat & 0x200) {
      if (PartyMemberChooser_MoveEntry(ownerIndex, 0)) {
        Sound_PlayFar(112);
        ownerIndex--;
        Menu_CreateOwnerEntryObjects(ownerIndex / 4 * 4);
        Menu_HighlightPageEntry(menu->rowWindow, ownerIndex, ownerCount);
      } else {
        Sound_PlayFar(114);
      }
      wait(1);
    } else if ((KEYS_PRESSED & 4) && GameFlag_Test(55)) {
      DjinnMenu_ShowCurrentList();
      redraw = 1;
    } else {
      redraw = Menu_MoveGridCursor(&ownerIndex, ownerCount, 4);
    }
  }
  menu->ownerIndex = ownerIndex;
  menu->selectedOwner = menu->ownerIds[ownerIndex];
  menu->selectedIds[0] = menu->ownerIds[ownerIndex];
  menu->arrows[0]->active = 13;
  menu->arrows[1]->active = 13;
  return result;
}

#undef KEYS_PRESSED

struct OwnerPageState {
  u8 unk000[8];
  s32 selectedOwner;
  u8 unk00c[4];
  void *ownerWindow;
  u8 unk014[8];
  s8 ownerIndex;
  u8 unk01d;
  s8 ownerCount;
  u8 unk01f[0xd1];
  void *messageWindow;
  u8 unk0f4[0xd0];
  u16 psynergies[32];
  u16 ownerIds[8];
  u8 psynergyCount;
  u8 unk215;
  u8 selectedIds[6];
  u16 page;
};

void Menu_PlaceEntryActorsInGrid(s32 x, s32 y, s32 columns);

s32 PsynergyMenu_ChooseMember(void) {
  struct OwnerPageState *menu =
      (struct OwnerPageState *)Ram_HeapSlots->menuRuntime;
  s32 selection = menu->ownerIndex;
  s32 count = menu->ownerCount;
  s32 pending = 2;
  s32 page = menu->page;
  s32 result;

  Unit_Get(menu->ownerIds[selection]);
  WindowSprite_RedrawSavedRectFar(menu->messageWindow);
  Text_DrawMessageAsTilesFar((s32)MsgChooseCharacter, menu->messageWindow, 0,
                                0);
  Text_DrawMessageAsTilesFar((s32)MsgChooseCharacter + 1,
                                menu->messageWindow, 0, 16);
  while (1) {
    // PERM
    if (pending) {
      selection = (selection + count) % count;
      Unit_Get(menu->ownerIds[selection]);
      page = (page + 3) % 3;
      Menu_CreateWindowAndEntryObjects(menu->ownerIds[selection], page);
      PsynergyMenu_CallIconRoutineWithValue(menu, menu->ownerIds[selection]);
      menu->psynergyCount = PsynergyMenu_CollectActions(Unit_Get(menu->ownerIds[selection]),
                                          menu->psynergies, 0);
      PsynergyMenu_DrawPsynergyIcons(menu->psynergies);
      Menu_PlaceEntryActorsInGrid(96, 96, 8);
      Menu_HideEmptyEntryIcons(menu->psynergies);
      if (pending == 2) {
        Menu_CreateOwnerEntryObjects(selection / 4 * 4);
        wait(1);
      }
      Menu_HighlightPageEntry(menu->ownerWindow, selection, count);
    }
    Func_080f8a44((selection % 4) * 24 - 10, 16);
    wait(1);
    if (((struct InputState *)&gInput)->pressed & 1) {
      Sound_PlayFar(112);
      result = 1;
      break;
    }
    if (((struct InputState *)&gInput)->pressed & 2) {
      Sound_PlayFar(113);
      result = -1;
      break;
    }
    pending = Menu_MoveGridCursor(&selection, count, 4);
  }
  menu->ownerIndex = selection;
  menu->selectedOwner = menu->ownerIds[selection];
  menu->selectedIds[0] = menu->ownerIds[selection];
  return result;
}

/// Moves the party member at selectedIndex one place later (direction 1) or
/// earlier (0) in the party order. Returns 1, or 0 when it cannot move.
s32 PartyMemberChooser_MoveEntry(s32 selectedIndex, s32 direction) {
  struct CharacterMenuState *menu = Ram_HeapSlots->menuRuntime;
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
    reordered[index] = menu->ownerIds[index];

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
    Party_RemoveMemberFar(menu->ownerIds[index]);
  for (index = 0; index < menu->partyCount; index++)
    Party_AddMemberFar(reordered[index]);
  menu->partyCount = Party_ListMembersFar(menu->ownerIds);
  return 1;
}

struct MenuRuntimeCursor {
  u8 unk00[0x24];
  s32 unk24;
  s32 unk28;
  s32 unk2c;
  s32 unk30;
  u8 unk34[0xc0];
  u8 unkf4;
  u8 unkf5;
  u8 unkf6;
  u8 unkf7;
};

void Menu_PlaceOwnerEntryObjects(struct MenuEntryWindow *window, s32 originX, s32 originY);

/// Its callers pass four values it does not read.
void StatusScreen_Initialize(s32 unused0, s32 unused1, s32 unused2, s32 unused3) {
  struct MenuRuntimeCursor *work = Ram_HeapSlots->menuRuntime;

  Menu_PlaceOwnerEntryObjects(Func_080f88d0(work), 2, 2);
  work->unk2c = 0;
  work->unk28 = 0;
  work->unk30 = 0;
  work->unk24 = 0;
  work->unkf4 = 0;
  work->unkf5 = 0;
  work->unkf6 = 8;
  work->unkf7 = 2;
}

/// The character menu work block, as this function reads it.
struct State080a8088 {
  u8 unk000[0x28];
  s32 handle;
  u8 unk02c[0x144];
  void *object;
  u8 unk174[0xa8];
  u16 mode;
};

typedef struct {
  u8 padding[15];
  u8 field0f;
} Object0f;

void StatusScreen_DrawStatusAilments(s32, s32, s32); ///< draws the status ailments

/// Creates the status window and its side object the first time, spawns the
/// icon entries in mode 3, then draws the status ailments (with 0x100 when the
/// window was just made). The work block holds the window at 0x28, the object
/// at 0x170 and the mode at 0x21c. frame is not used.
// ☀️'s version also creates eight entry objects.
void Menu_CreateWindowAndEntryObjects(s32 resource, s32 frame) {
  s32 created;
  s32 handle;
  void *object;
  struct State080a8088 *state;

  state = Ram_HeapSlots->menuRuntime;
  created = 0;
  handle = state->handle;
  if (handle == 0) {
    created = Window_UpdateOrCreate(&state->handle, 0, 5, 0x1E, 0xF, 2);
    handle = state->handle;
  }
  if (created != 0) {
    object = SideObject_CreateFar(resource, 0, 0, handle, 0, 0);
    state->object = object;
    if ((((Object0f *)object)->field0f = 0xF0, state->mode) == 3) {
      Menu_SpawnIconEntries(state, handle);
    }
    StatusScreen_DrawStatusAilments(handle, resource, 0x100);
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

void StatusScreen_DrawSelectionLabels(s32 target, s32 selected, const u8 *entries);
void StatusScreen_ShowMemberProgressMessage(void *window, s32 help, s32 full);

s32 StatusScreen_SelectCommand(void) {
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
  s32 window;
  s32 helpWindow;
  s32 i;
  u8 entries[8];

  menu = Ram_HeapSlots->menuRuntime;
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

  while (done == 0 && !GameFlag_Test(0x150)) {
    Unit_Get(menu->selectedIds[0]);
    StatusScreen_DrawStatusAilments(window, menu->selectedIds[0], 1);
    count = CharacterMenu_BuildAvailability(entries, 1, menu->selectedIds[0]);
    hasAilments = 0;
    if (count == 0)
      count = 1;
    else
      hasAilments = 1;
    redraw = 1;

    while (!GameFlag_Test(0x150)) {
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
          StatusScreen_ShowMemberProgressMessage(helpWindow, selected, hasDjinn);
      }
      if (pane == 0)
        Func_080f8a44(-10, selected * 16 + 88);
      else if (selected <= 3)
        Func_080f8a44(24, selected * 8 + 48);
      else
        Func_080f8a44(48, selected * 8 + 80);
      wait(1);
      if (gInput.repeat & 0xf0)
        StatusScreen_DrawSelectionCursor(pane, selected, entries, 1);
      if (KEYS_PRESSED & 1) {
        Sound_PlayFar(112);
        done = 1;
        result = 1;
        break;
      }
      if (KEYS_PRESSED & 2) {
        Sound_PlayFar(113);
        done = 1;
        result = -1;
        break;
      }
      if (gInput.repeat & 0x40) {
        Sound_PlayFar(111);
        redraw = 1;
        selected--;
      }
      if (gInput.repeat & 0x80) {
        Sound_PlayFar(111);
        redraw = 1;
        selected++;
      }
      if (gInput.repeat & 0x10) {
        Sound_PlayFar(111);
        redraw = 1;
        pane++;
      }
      if (gInput.repeat & 0x20) {
        Sound_PlayFar(111);
        redraw = 1;
        pane--;
      }
      if ((gInput.repeat & 0x100) || (gInput.repeat & 0x200)) {
        Sound_PlayFar(111);
        member = menu->ownerIndex;
        if (gInput.repeat & 0x100)
          member++;
        else
          member--;
        member = (member + menu->partyCount) % menu->partyCount;
        menu->selectedOwner = menu->ownerIds[member];
        menu->selectedIds[0] = menu->ownerIds[member];
        menu->ownerIndex = member;
        PsynergyMenu_CallIconRoutineWithValue(menu, menu->ownerIds[member]);
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
#undef SWITCH_CHARACTER_HELP_X

struct CursorMenuState {
  u8 padding[40];
  u8 *object;
};

/// Highlights (invert 0) or unhighlights (invert 1) the selected entry of the
/// status page: in side 0 the selected status row, as wide as its label
/// (gStatusScreen_CursorWidths); in side 1 one of the first four status rows
/// or of the rows below.
void StatusScreen_DrawSelectionCursor(s32 mode, s32 selected, u8 *entries,
                                       s32 invert) {
  struct CursorMenuState *state = Ram_HeapSlots->menuRuntime;
  u32 different;
  s32 count;
  s32 index;
  s32 x;
  s32 y;
  s32 last;
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

  different = 1 ^ invert;
  last = 15 - (((0u - different) | different) >> 31);
  Render_SetTilemapFlagRect(state->object, x, y, width, 1, last);
}

/// Writes the advice for the selected status (two lines, for example how to
/// revive a party member who is down) into window target, or that the party
/// member is well when they have no status.
void StatusScreen_DrawSelectionLabels(s32 target, s32 selected,
                                       const u8 *entries) {
  s32 count = 0;
  s32 index = 0;

  do {
    if (entries[index] != 0) {
      if (selected == count) {
        s32 message = (s32)(MsgStatusAdvice + index * 2);
        Text_DrawMessageFar(message, target, 0, -1);
        Text_DrawMessageFar(message + 1, target, 0, 15);
      }
      count++;
    }
    index++;
  } while (index <= 4);

  if (count == 0)
    Text_DrawMessageFar((s32)MsgStatusNormal, target, 0, 0);
}

struct StatusOwner {
  u8 unk000[15];
  u8 level;
  u8 unk010[276];
  s32 experience;
};

#if (LANGUAGE == LANGUAGE_GERMAN) || (LANGUAGE == LANGUAGE_SPANISH) ||                      \
    (LANGUAGE == LANGUAGE_FRENCH) || (LANGUAGE == LANGUAGE_ITALIAN)
#define HELP_LENGTH 80
#else
#define HELP_LENGTH 64
#endif

u32 Unit_GetExperienceForLevelFar(s32 owner_id, s32 level);
void Message_CopyExpandedFar(s32 messageId, void *buffer, s32 length);
void Text_DrawWideStringAtOffsetFar(void *buffer, void *destination,
                                        s32 offset, s32 terminator);

/// Writes the help text of status entry messageVariant into destination: the
/// entries skip Djinn when the party has none (preserveVariant 0); the
/// experience entry says how many points the next level needs, or, at level 99,
/// gives the plain experience text. The text is copied into a heap buffer of
/// STATUS_HELP_TEXT_MAX characters, freed before returning.
void StatusScreen_ShowMemberProgressMessage(void *window, s32 help, s32 full) {
  u8 *menu = Ram_HeapSlots->menuRuntime;
  u8 *buffer;

  if (full == 0 && help > 3)
    help++;
  if (help == 1) {
    u8 *owner = menu + 0x216;
    struct StatusOwner *state = Unit_Get(*owner);

    if (state->level == 99)
      help = 8;
    else
      MessageArgument_PushFar(
          Unit_GetExperienceForLevelFar(*owner, state->level + 1) -
              state->experience,
          5);
  }
  buffer = Heap_Alloc(HELP_LENGTH * 2);
  Message_CopyExpandedFar(help + (s32)MsgYourNameAndLevelExperience, buffer,
                              HELP_LENGTH);
  Text_DrawWideStringAtOffsetFar(buffer, window, 0, -1);
  Heap_Release(buffer);
}

#undef HELP_LENGTH
#undef HELP_LENGTH

// The Japanese edition draws the Lv label through Text_DrawStringInWindowFar.
#if LANGUAGE == LANGUAGE_JAPANESE
#define DRAW_LEVEL_LABEL Text_DrawStringInWindowFar
#else
#define DRAW_LEVEL_LABEL Text_DrawStringAtOffsetFar
#endif

extern const u8 gMenu_LvString[];
extern const u8 gMenu_SlashString[];

void ItemMenu_DrawOwnerStatus(s32 window, s32 owner, s32 flags);

void Func_081054e0(struct MenuPanel *panel, s32 x, s32 y, const u8 *enabled);

s32 Unit_GetElementLevelFar(s32 ownerId, s32 element);

/// Draws party member memberId's status page into window: the top rows
/// (ItemMenu_DrawMemberStatus), the status list or "Normal", and except on the
/// chooser page the Djinn counts by element and, in mode 1, the elemental
/// levels, Power and Resist. With STATUS_PAGE_NO_CLEAR in mode the window is
/// not cleared first.
void StatusScreen_DrawStatusAilments(s32 window, s32 ownerId, s32 mode) {
  struct CharacterMenuState *menu = Ram_HeapSlots->menuRuntime;
  struct BattleUnit *unit;
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
  if ((mode & 0xff) == 1)
    djinnRow = 7;
  else
    djinnRow = 10;
  menu->statusCursor->active = 1;
  ItemMenu_DrawOwnerStatus(window, ownerId, mode);
  CharacterMenu_BuildAvailability(ailments, 1, ownerId);
  keepDrawn = mode & 0x100;
  if (keepDrawn == 0)
    Window_ClearInteriorTilesFar(window, 0, 40, 96, 96);
  Func_081054e0(window, 0, 40, ailments);
  if (menu->page == 3)
    return;
  if (keepDrawn == 0) {
    wait(1);
    Window_ClearInteriorTilesFar(window, 64, 56, 224, 96);
  }
  Text_SetColorFar(15);
  if (mode == 1 || hasDjinn == 1) {
    Window_SetTilemapEntryFar(window, 1, 15, djinnRow, 4);
    Window_SetTilemapEntryFar(window, 2, 19, djinnRow, 4);
    Window_SetTilemapEntryFar(window, 3, 23, djinnRow, 4);
    Window_SetTilemapEntryFar(window, 4, 27, djinnRow, 4);
  }
  if (hasDjinn)
    Text_DrawMessageAsTilesFar(MsgDjinnLabel, window, 64, djinnRow * 8 + 8);
  if (mode == 1) {
    if (!hasDjinn)
      djinnRow--;
    DRAW_LEVEL_LABEL(gMenu_LvString, window, 64, djinnRow * 8 + 16);
    Text_DrawMessageAsTilesFar(MsgPowerLabel, window, 64, djinnRow * 8 + 24);
    Text_DrawMessageAsTilesFar(MsgPowerLabel + 1, window, 64,
                                  djinnRow * 8 + 32);
  }
  for (element = 0; element <= 3; element++) {
    if (hasDjinn)
      Text_DrawNumberInWindowFar(unit->djinnOwnedCounts[element], 1, window,
                                 element * 32 + 120, djinnRow * 8 + 8);
    if ((mode & 0xff) == 1) {
      if (hasDjinn) {
        Text_DrawNumberInWindowFar(unit->djinnActiveCounts[element], 1, window,
                                   element * 32 + 104, djinnRow * 8 + 8);
        Text_DrawStringInWindowFar(gMenu_SlashString, window,
                                   element * 32 + 112, djinnRow * 8 + 8);
      }
      Text_DrawNumberInWindowFar(Unit_GetElementLevelFar(ownerId, element), 2,
                                 window, element * 32 + 112, djinnRow * 8 + 16);
      Text_DrawNumberInWindowFar(unit->element[element].power, 3, window,
                                 element * 32 + 104, djinnRow * 8 + 24);
      Text_DrawNumberInWindowFar(unit->element[element].resistance, 3, window,
                                 element * 32 + 104, djinnRow * 8 + 32);
    }
  }
}

#undef DRAW_LEVEL_LABEL
#undef DRAW_LEVEL_LABEL

struct ItemMenuCursor {
  u8 unk00[5];
  u8 state;
};

struct ItemMenuState {
  u8 unk000[0x170];
  struct ItemMenuCursor *cursor;
};

extern const u8 gMenu_HpString[];
extern const u8 gMenu_PpString[];

// The Japanese edition draws the Lv, HP and PP labels through
// Text_DrawStringInWindowFar, and the four stats a digit wider, starting a
// tile further left.
#if LANGUAGE == LANGUAGE_JAPANESE
#define DRAW_STAT_LABEL Text_DrawStringInWindowFar
#define STAT_DIGITS 4
#define STAT_X 192
#else
#define DRAW_STAT_LABEL Text_DrawStringAtOffsetFar
#define STAT_DIGITS 3
#define STAT_X 200
#endif

void ItemMenu_DrawOwnerStatus(s32 window, s32 owner, s32 flags) {
  struct ItemMenuState *menu;
  struct BattleUnit *unit;

  menu = Ram_HeapSlots->menuRuntime;
  unit = Unit_Get(owner);
  menu->cursor->state = 1;
  flags &= 0x100;
  if (flags == 0)
    Window_ClearInteriorTilesFar(window, 0, 0, 128, 40);
  Text_DrawStringAtOffsetFar(unit->name, window, 40, 0);
  Text_DrawMessageAsTilesFar(unit->classIndex + (s32)MsgClassName, window,
                                  0, 32);
  DRAW_STAT_LABEL(gMenu_LvString, window, 104, 0);
  Text_SetColorFar(15);
  Text_DrawNumberInWindowFar(unit->level, 2, window, 128, 0);
  DRAW_STAT_LABEL(gMenu_HpString, window, 40, 16);
  Text_DrawNumberInWindowFar(unit->hp, 4, window, 72, 16);
  Text_DrawNumberInWindowFar(unit->maxHp, 4, window, 112, 16);
  Text_DrawStringInWindowFar(gMenu_SlashString, window, 104, 16);
  DRAW_STAT_LABEL(gMenu_PpString, window, 40, 24);
  Text_DrawNumberInWindowFar(unit->pp, 4, window, 72, 24);
  Text_DrawNumberInWindowFar(unit->maxPp, 4, window, 112, 24);
  Text_DrawStringInWindowFar(gMenu_SlashString, window, 104, 24);
  Text_DrawMessageAsTilesFar((s32)MsgExpLabel, window, 40, 8);
  Text_DrawNumberInWindowFar(unit->experience, 8, window, 80, 8);
  if (flags == 0) {
    wait(1);
    Window_ClearInteriorTilesFar(window, 144, 0, 224, 40);
  }
  Text_DrawMessageAsTilesFar((s32)MsgExpLabel - 23, window, 152, 0);
  Text_DrawMessageAsTilesFar((s32)MsgExpLabel - 22, window, 152, 8);
  Text_DrawMessageAsTilesFar((s32)MsgExpLabel - 21, window, 152, 16);
  Text_DrawMessageAsTilesFar((s32)MsgExpLabel - 20, window, 152, 24);
  Text_DrawNumberInWindowFar(unit->atk, STAT_DIGITS, window, STAT_X, 0);
  Text_DrawNumberInWindowFar(unit->def, STAT_DIGITS, window, STAT_X, 8);
  Text_DrawNumberInWindowFar(unit->agi, STAT_DIGITS, window, STAT_X, 16);
  Text_DrawNumberInWindowFar(unit->luck, STAT_DIGITS, window, STAT_X, 24);
}

#undef STAT_DIGITS
#undef STAT_X
#undef STAT_DIGITS
#undef STAT_X

// The rows one page of the list shows: Japanese fits six, the other editions
// five.
#if LANGUAGE == LANGUAGE_JAPANESE
#define PAGE_ROWS 6
#else
#define PAGE_ROWS 5
#endif

struct MenuOwnerWork {
  u8 unk000[0x214];
  u8 count;
  u8 unk215;
  u8 owners[0x264 - 0x216];
  s8 selected[8];
};

struct MenuPageResult {
  void *owner;
  s32 unk04;
  s32 page;
  s32 pageCount;
  s32 row;
  s32 count;
  s32 selected;
};

/// Fills result with the item list of the party member in side index for the
/// equipment pages: their unit, the menu's item count, and the page and row of
/// the item chosen last for them, STATUS_EQUIP_PAGE_SIZE items to a page.
/// Returns 1.
s32 PagedList_Build(struct MenuPageResult *result, s32 index) {
  struct MenuOwnerWork *menu = Ram_HeapSlots->menuRuntime;
  void *owner;
  s32 count;
  s32 value;
  s32 page;
  s32 row;
  s32 pages;

  owner = Unit_Get(menu->owners[index]);
  count = menu->count;
  value = menu->selected[menu->owners[index]];

  if (value + 1 > count)
    value = count - 1;
  if (count == 0)
    value = 0;
  page = value / PAGE_ROWS;
  row = value % PAGE_ROWS;
  pages = count / PAGE_ROWS;
  if (count % PAGE_ROWS != 0)
    pages++;
  result->owner = owner;
  result->page = page;
  result->pageCount = pages;
  result->row = row;
  result->count = count;
  result->selected = value;
  return 1;
}

#undef PAGE_ROWS
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
      u32 *tile = (u32 *)0x06006000 + set * 0x60 + n * 0x10;
      s32 x;

      Iwram_FillWords(tile, 64, 0x44444444);
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

void PsynergyMenu_DrawRange(s32 window, s32 x, s32 y, s32 range, s32 unused) {
  s32 n;
  s32 tile;

  n = range * 2;
  tile = n + RANGE_ICON_TILE + 1;
  Window_SetTilemapEntryFar(window, 0x400 | tile, x, y, 0);
  Window_SetTilemapEntryFar(window, n + RANGE_ICON_TILE, x + 1, y, 0);
  Window_SetTilemapEntryFar(window, tile, x + 2, y, 0);
}

#undef RANGE_ICON_TILE

// The psynergy menu work block, as this function reads it.
struct PsynergyListWork {
  u8 unk000[0x30];
  void *infoWindow;
  u8 unk034[0x190];
  u16 psynergies[32];
};

#define ACTION_ID_MASK OWNER_ACTION_ID_MASK

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

s32 PsynergyMenu_DrawRangePage(s32 window, s32 unused,
                               struct MenuResult *state) {
  struct PsynergyListWork *menu;
  struct BattleAct *ability;
  s32 row;
  s32 base;

  menu = (struct PsynergyListWork *)Ram_HeapSlots->menuRuntime;
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
    ability = BattleAction_Get(menu->psynergies[state->selectedIndex] &
                               ACTION_ID_MASK);
    Window_ClearInteriorTilesFar(window, 0, USABLE_TOP, USABLE_RIGHT,
                                 USABLE_BOTTOM);
    row = 0;
#if LANGUAGE == LANGUAGE_JAPANESE
    // One line for each place the ability works, then the end of the
    // sentence on the line below them.
    if (ability->unk04[2] != 0 ||
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
    if (ability->unk04[2] != 0 ||
        (ability->flags & ACTION_USABLE_IN_FIELD) != 0) {
      row = 2;
    }
    if ((ability->flags & ACTION_USABLE_IN_BATTLE) != 0) {
      row |= 1;
    }
    if (row == 3) {
      Text_DrawMessageAsTilesFar((s32)MsgUsableAnywhere, window, 0,
                                    USABLE_TOP);
    } else if (row == 2) {
      Text_DrawMessageAsTilesFar((s32)MsgUsableInField, window, 0,
                                    USABLE_TOP);
    } else if (row == 1) {
      Text_DrawMessageAsTilesFar((s32)MsgUsableInBattle, window, 0,
                                    USABLE_TOP);
    }
#endif
  }

  base = state->page * PAGE_ROWS;
  for (row = 0; row <= PAGE_ROWS - 1; row++) {
    if (row == state->row) {
      ability = BattleAction_Get(menu->psynergies[base + row] & ACTION_ID_MASK);
      if (ability->element != ELEMENT_NONE) {
        Window_SetTilemapEntryFar(window, ability->element + 1, 24,
                                  row * 2 + FIRST_TILE_ROW, 0);
        Render_SetTilemapFlagRect(window, 9, row * 2 + FIRST_TILE_ROW, 15, 1,
                                  14);
        Render_SetTilemapFlagRect(window, 25, row * 2 + FIRST_TILE_ROW, 3, 1,
                                  14);
      } else {
        Render_SetTilemapFlagRect(window, 9, row * 2 + FIRST_TILE_ROW, 19, 1,
                                  14);
      }
    } else {
      ability = BattleAction_Get(menu->psynergies[base + row] & ACTION_ID_MASK);
      if (ability->element != ELEMENT_NONE) {
        Window_SetTilemapEntryFar(window, ability->element + 1, 24,
                                  row * 2 + FIRST_TILE_ROW, 4);
        Render_SetTilemapFlagRect(window, 9, row * 2 + FIRST_TILE_ROW, 15, 1,
                                  15);
        Render_SetTilemapFlagRect(window, 25, row * 2 + FIRST_TILE_ROW, 3, 1,
                                  15);
      } else {
        Render_SetTilemapFlagRect(window, 9, row * 2 + FIRST_TILE_ROW, 19, 1,
                                  15);
      }
    }
  }
  wait(1);
  return 1;
}

#undef PAGE_ROWS
#undef USABLE_RIGHT
#undef USABLE_BOTTOM
#undef PAGE_ROWS
#undef USABLE_RIGHT
#undef USABLE_BOTTOM

// The psynergy menu work block, as this function reads it.
#define PSY_LIST_OFFSET 0x1c4
#define ACT_ID_MASK OWNER_ACTION_ID_MASK
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
                              const struct MenuResult *res) {
  struct PsynergyMenuState *menu = Ram_HeapSlots->menuRuntime;
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
  Menu_DrawPageIndicator(window, res->entryCount, PAGE_ROWS, res->page, 28);

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
      act = BattleAction_Get(ACT_ID_MASK & *(u16 *)(offset + (s32)menu));
      msg =
          (*(u16 *)(offset + (s32)menu) & ACT_ID_MASK) + (s32)MsgAbilityNothing;
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
  Text_DrawMessageAsTilesFar(
      owner[OWNER_CLASS_MSG_OFFSET] + (s32)MsgClassName, window, 0, 32);
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

s32 Func_080ffcd4(void) {
  struct PsynergyMenuState *menu;
  s32 result;
  s32 window;
  s32 refresh;
  s32 listNeedsRedraw;
  s32 navigation;
  s32 ownerIndex;
  struct MenuResult state;

  menu = Ram_HeapSlots->menuRuntime;
  window = menu->statusWindow;
  result = 0;
  refresh = 2;
  listNeedsRedraw = 1;
  Window_UpdateOrCreate(&menu->infoWindow, 0, 0, 30, 5, 2);
  Menu_SpawnIconEntries(menu, menu->messageWindow);
  Text_DrawMessageAsTilesFar(MsgSwitchCharacterHelp + 2, window, 0, -24);
  Text_DrawMessageAsTilesFar(MsgSwitchCharacterHelp, window, 64, -24);
  Menu_BuildPatternTiles();
  while (!GameFlag_Test(0x150)) {
    if (refresh) {
      if (refresh == 2) {
        menu->psynergyCount =
            PsynergyMenu_CollectActions(Unit_Get(menu->memberIds[0]), menu->psynergies, 0);
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
    Func_080f8a44(CURSOR_X, state.row * 16 + CURSOR_Y);
    if (navigation == 1) {
      refresh = 1;
      listNeedsRedraw = 1;
    }
    if (navigation == 0)
      refresh = 1;
    if (navigation == -1)
      refresh = 0;
    if (KEYS_PRESSED & 1) {
      Sound_PlayFar(112);
      result = 1;
      break;
    }
    if (KEYS_PRESSED & 2) {
      Sound_PlayFar(113);
      result = -1;
      break;
    }
    if ((gInput.repeat & 0x100) || (gInput.repeat & 0x200)) {
      Sound_PlayFar(111);
      ownerIndex = menu->tabIndex[0];
      menu->selectedIndexByOwner[menu->ownerIds[ownerIndex]] =
          state.selectedIndex;
      if (gInput.repeat & 0x100)
        ownerIndex++;
      else
        ownerIndex--;
      ownerIndex = (ownerIndex + menu->memberCount) % menu->memberCount;
      menu->selectedOwner = menu->ownerIds[ownerIndex];
      menu->memberIds[0] = menu->ownerIds[ownerIndex];
      menu->tabIndex[0] = ownerIndex;
      PsynergyMenu_CallIconRoutineWithValue(menu, menu->ownerIds[ownerIndex]);
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

s32 ItemMenu_DrawEquipPage(s32 window, s32 unused, struct MenuResult *state) {
  struct InventoryMenuState *menu;
  struct ItemDefinition *item;
  s32 base;
  s32 row;
  u32 entry;
  u32 mask;

  menu = Ram_HeapSlots->menuRuntime;
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
    Text_DrawMessageFar((menu->items[state->selectedIndex] & ITEM_ID_MASK) +
                               (s32)MsgItemPlainName,
                           menu->infoWindow, 0, 0);
#else
    Text_DrawMessageAsTilesFar(
        (menu->items[state->selectedIndex] & ITEM_ID_MASK) + MsgItemPlainName,
        menu->infoWindow, 0, 0);
#endif
  }

  for (row = 0; row < PAGE_ROWS; row++) {
    if (row == state->row) {
      entry = menu->items[state->selectedIndex];
      mask = ITEM_ID_MASK;
      mask &= entry;
      item = Item_Get(mask);
      if (item->element != 4) {
        Window_SetTilemapEntryFar(window, item->element + 1, 27, row * 2 + 1,
                                    0);
        Render_SetTilemapFlagRect(window, 14, row * 2 + 1, 13, 1, 14);
      } else {
        Render_SetTilemapFlagRect(window, 14, row * 2 + 1, 14, 1, 14);
      }
    } else {
      entry = menu->items[base + row];
      mask = ITEM_ID_MASK;
      mask &= entry;
      item = Item_Get(mask);
      if (item->element != 4) {
        Window_SetTilemapEntryFar(window, item->element + 1, 27, row * 2 + 1,
                                    4);
        Render_SetTilemapFlagRect(window, 14, row * 2 + 1, 13, 1, 15);
      } else {
        Render_SetTilemapFlagRect(window, 14, row * 2 + 1, 14, 1, 15);
      }
    }
  }

  for (row = 0; row <= 3; row++)
    Render_SetTilemapFlagRect(menu->equipWindow, EQUIP_SLOT_X, row * 2 + 1,
                              EQUIP_SLOT_WIDTH, 1, 15);

  if (menu->items[state->selectedIndex] & INVENTORY_EQUIPPED) {
    item = Item_Get(menu->items[state->selectedIndex] & ITEM_ID_MASK);
    switch (item->type) {
    case 1:
      Render_SetTilemapFlagRect(menu->equipWindow, EQUIP_SLOT_X, 1,
                                EQUIP_SLOT_WIDTH, 1, 14);
      break;
    case 4:
      Render_SetTilemapFlagRect(menu->equipWindow, EQUIP_SLOT_X, 3,
                                EQUIP_SLOT_WIDTH, 1, 14);
      break;
    case 3:
      Render_SetTilemapFlagRect(menu->equipWindow, EQUIP_SLOT_X, 5,
                                EQUIP_SLOT_WIDTH, 1, 14);
      break;
    case 2:
      Render_SetTilemapFlagRect(menu->equipWindow, EQUIP_SLOT_X, 7,
                                EQUIP_SLOT_WIDTH, 1, 14);
      break;
    }
  }
  wait(1);
  return 1;
}

#undef PAGE_ROWS
#undef PAGE_ROWS
#undef EQUIP_SLOT_WIDTH
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
s32 ItemMenu_DrawItemPage(s32 window, s32 unused, struct MenuResult *state) {
  struct InventoryMenuState *menu = Ram_HeapSlots->menuRuntime;
  struct BattleUnit *unit;
  u16 *inventoryEntry;
  s32 firstEntry;
  u32 rowCount;
  u32 rowIndex;
  u8 *message;

  unit = Unit_Get(menu->paneOwner[0]);
  Window_ClearInteriorTilesFar(window, 128, 8, 224, 96);
  firstEntry = state->page * PAGE_ROWS;
  rowCount = (u8)(state->entryCount - firstEntry);
  if (rowCount > PAGE_ROWS)
    rowCount = PAGE_ROWS;
  Menu_SetPageIcons(PAGE_ROWS, firstEntry, window, PAGE_ICONS_X, 52);
  Menu_DrawPageIndicator(window, state->entryCount, PAGE_ROWS, state->page, 28);
  if (menu->itemCount == 0) {
    Text_DrawMessageAsTilesFar(MsgItemMenuEmpty, window, EMPTY_MESSAGE_X,
                                    8);
  } else {
    rowIndex = 0;
    if (rowCount > rowIndex) {
      inventoryEntry = &menu->items[firstEntry];
      do {
        Text_DrawMessageAsTilesFar(
            (*inventoryEntry & ITEM_ID_MASK) + MsgItemName, window, ITEM_NAME_X,
            (rowIndex << 4) + 8);
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
#undef PAGE_ROWS
#undef EMPTY_MESSAGE_X
#undef EMPTY_MESSAGE_X
#undef STAT_X
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
  struct InventoryMenuState *menu;
  struct BattleUnit *owner;
  s32 result;
  s32 window;
  s32 refresh;
  s32 listNeedsRedraw;
  s32 navigation;
  s32 ownerIndex;
  s32 i;
  struct MenuResult state;
  u16 savedInventory[15];

  menu = Ram_HeapSlots->menuRuntime;
  window = menu->statusWindow;
  result = 0;
  refresh = 2;
  listNeedsRedraw = 1;
  owner = Unit_Get(menu->paneOwner[0]);
  Window_UpdateOrCreate(&menu->equipWindow, 0, 10, EQUIP_WINDOW_WIDTH, 10, 2);
  Menu_SpawnIconEntries(menu, menu->messageWindow);
  Text_DrawMessageAsTilesFar(MsgSwitchCharacterHelp + 3, window, 0, -24);
  Text_DrawMessageAsTilesFar(MsgSwitchCharacterHelp, window, 64, -24);
  while (!GameFlag_Test(0x150)) {
    if (refresh) {
      if (refresh == 2) {
        owner = Unit_Get(menu->paneOwner[0]);
        menu->itemCount = ItemMenu_CollectItems(owner, menu->items, 0);
        WindowSprite_RedrawSavedRectFar(window);
        for (i = 0; i <= 14; i++)
          savedInventory[i] = owner->inventory[i];
        InventoryMenu_SortByListOrder(menu->items, 0);
        PagedList_Build(&state, 0);
        ItemMenu_DrawIcons(menu->items, 0);
        WindowSprite_RedrawSavedRectFar(menu->equipWindow);
        ItemMenu_DrawCategory(menu->equipWindow, menu->paneOwner[0], 1);
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
    Func_080f8a44(CURSOR_X, state.row * 16 + 52);
    if (navigation == 1) {
      refresh = 1;
      listNeedsRedraw = 1;
    }
    if (navigation == 0)
      refresh = 1;
    if (navigation == -1)
      refresh = 0;
    if (KEYS_PRESSED & 1) {
      Sound_PlayFar(112);
      result = 1;
      break;
    }
    if (KEYS_PRESSED & 2) {
      Sound_PlayFar(113);
      result = -1;
      break;
    }
    if ((gInput.repeat & 0x100) || (gInput.repeat & 0x200)) {
      Sound_PlayFar(111);
      ownerIndex = menu->paneIndex;
      // Read unsigned (ldrh): the party's ids, as this routine's bytes load them.
      menu->selectedIndexByOwner[(u16)menu->ownerIds[ownerIndex]] =
          state.selectedIndex;
      if (gInput.repeat & 0x100)
        ownerIndex++;
      else
        ownerIndex--;
      for (i = 0; i <= 14; i++)
        owner->inventory[i] = savedInventory[i];
      ownerIndex = (ownerIndex + menu->partyCount) % menu->partyCount;
      menu->selectedOwner = (u16)menu->ownerIds[ownerIndex];
      menu->paneOwner[0] = (u16)menu->ownerIds[ownerIndex];
      menu->paneIndex = ownerIndex;
      PsynergyMenu_CallIconRoutineWithValue(menu, (u16)menu->ownerIds[ownerIndex]);
      refresh = 2;
    }
  }
  WindowSprite_PrepareForRedrawFar(menu->infoWindow);
  WindowSprite_RedrawSavedRectFar(window);
  WindowSprite_ClearListFar(menu->messageWindow);
  return result;
}

#undef PAGE_ROWS
#undef PAGE_ROWS
#undef CURSOR_X
#undef CURSOR_X

struct CategoryMenuState {
  u8 unk000[0x1c4];
  u16 items[15];
};

void ItemMenu_DrawCategory(s32 window, s32 ownerId, s32 mode) {
  struct CategoryMenuState *menu = Ram_HeapSlots->menuRuntime;
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
      item = items[i] & ITEM_ID_MASK;
      switch (Item_Get(item)->type) {
      case 1:
        Text_DrawMessageAsTilesFar(item + MsgItemName, window, 16, 8);
        break;
      case 2:
        Text_DrawMessageAsTilesFar(item + MsgItemName, window, 16, 56);
        break;
      case 3:
        Text_DrawMessageAsTilesFar(item + MsgItemName, window, 16, 40);
        break;
      case 4:
        Text_DrawMessageAsTilesFar(item + MsgItemName, window, 16, 24);
        break;
      }
    }
  }
}

struct EntryIconMenu {
  u8 unk000[0x4c];
  struct WindowSprite *entryIcons[32];
};

/// Places each of the 32 entry icons on the grid.
// The Broken Seal's Menu_PlaceEntryObjectsInGrid.
void Menu_PlaceEntryActorsInGrid(s32 originX, s32 originY, s32 columns) {
  s32 i;
  struct WindowSprite *obj;
  struct WindowSprite **table;

  i = 0;
  table = ((struct EntryIconMenu *)Ram_HeapSlots->menuRuntime)->entryIcons;
  do {
    obj = *table++;
    if (obj != NULL)
      Menu_PlaceEntryObjectInGrid(obj, i, originX, originY, columns);
    i += 1;
  } while (i <= 0x1F);
}

void Menu_PlaceEntryObjectInGrid(struct WindowSprite *obj, s32 index,
                                 s32 originX, s32 originY, s32 phase) {
  s32 no;

  no = index;
  if (no > 0x1F) {
    no = 0;
  }
  obj->y = no / phase * 0x10 + originY;
  obj->x = no % phase * 0x10 + originX;
  Icon_PrepareActor(obj);
}

struct CategoryItemIcon {
  u8 reserved00[6];
  s16 x;
  s16 y;
};

struct CategoryItemIconState {
  u8 reserved00[76];
  struct CategoryItemIcon *icons[32];
};

void ItemMenu_ArrangeCategoryItemIcons(u16 *items) {
  struct CategoryItemIconState *state;
  struct CategoryItemIcon *icon;
  s32 i;

  state = Ram_HeapSlots->menuRuntime;
  ItemMenu_PositionCategory();
  for (i = 0; i < 15; i++) {
    if (items[i] != 0 && (items[i] & 0x200) != 0) {
      icon = state->icons[i];
      if (icon != 0) {
        switch (Item_Get(items[i] & 0x1ff)->type) {
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
        Icon_PrepareActor(icon);
      }
    }
  }
}

void ItemMenu_PositionCategory(void) {
  struct CategoryItemIconState *state = Ram_HeapSlots->menuRuntime;
  s32 x = 248;
  struct CategoryItemIcon **entry = state->icons;
  s32 y = 168;
  s32 remaining = 31;

  do {
    struct CategoryItemIcon *icon = *entry++;

    if (icon != 0) {
      icon->x = x;
      icon->y = y;
      Icon_PrepareActor(icon);
    }
    remaining--;
  } while (remaining >= 0);
}
