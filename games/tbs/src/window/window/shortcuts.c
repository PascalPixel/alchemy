/// @file
/// The area name notice, the Psynergy shortcut picker and heap block 19.
/// Part of window.c, the window object, compiled only through it.

#include "../window_internal.h"

/// The area name notice's fields, which live in the event work block.
struct TimedNoticeWork {
  u8 eventWork[0x230]; ///< the event work's other fields
  struct UiWindow *window;
  u16 countdown;
};

/// Shows the area's name in a window near the top of the screen for 90 frames.
/// The event work owns the window and countdown until the scheduler closes it.
void UiTimedNotice_Create(void) {
  struct TimedNoticeWork *notice;
  s32 height;
  s32 width;
  s32 y;
  s32 x;
  s32 areaMessage;
  struct UiWindow *nameWindow;

  notice = (struct TimedNoticeWork *)gEventWork;
  x = 8;
  y = 8;
  areaMessage =
      Scene_GetAreaNameIndexFar(gGameState.scene, gGameState.entrance) +
      RENDER_RESOURCE_BASE;
  UiText_GetResourceDimensions(areaMessage, &x, &y, &width, &height);
  x = (30 - width) >> 1;
  y = (10 - height) >> 1;
  nameWindow = UiWindow_Create(x, y, width, height, 2);
  notice->window = nameWindow;
  UiText_DrawResource(areaMessage, nameWindow, 0, 0);
  notice->countdown = UI_AREA_NOTICE_FRAMES;
  Scheduler_AddOrUpdateCallback(UiTimedNotice_Tick, 0xc80);
}

/// Counts the area name down, and closes it at 0.
void UiTimedNotice_Tick(void) {
  struct TimedNoticeWork *notice;
  u16 framesLeft;

  notice = (struct TimedNoticeWork *)gEventWork;
  notice->countdown = (framesLeft = notice->countdown + 0xffff);
  if ((framesLeft << 16) == 0) {
    UiWork_Finalize(notice->window, 2);
    Scheduler_RemoveCallback(UiTimedNotice_Tick);
  }
}

/// Closes the area name at once, if it is shown.
void UiTimedNotice_CloseIfActive(void) {
  struct UiWindow *window;

  window = ((struct TimedNoticeWork *)gEventWork)->window;
  if (window != NULL && window->flags != 0) {
    UiWork_Finalize(window, 2);
    Scheduler_RemoveCallback(UiTimedNotice_Tick);
  }
}

s32 Item_RemoveFromInventory(s32 unused, s32 itemId) {
  PartyInventory_RemoveFar(itemId);
  return 0;
}

s32 Item_ReturnTrue(void) { return 1; }

/// Steps the window hue one step: back when keys contains LEFT, on
/// otherwise.
/// Nothing calls it.
UNUSED void OptionMenu_StepWindowHue(s32 keys) {
  u8 hue = gGameState.paletteGlow[0];
  if (keys & KEY_LEFT)
    hue += 0xff;
  else
    hue += 1;
  gGameState.paletteGlow[0] = hue;
}

/// A Psynergy the debug shortcut picker lists: its owner and the Psynergy.
struct AbilityListEntry {
  u16 owner;
  u16 ability;
};

/// Lets the player pick two Psynergy shortcuts from all the party's Psynergy,
/// showing each one's cost and description, and stores them in
/// gGameState.firstShortcut and secondShortcut (owner << 10 | Psynergy).
/// This debug path seeds extra Psynergy before enumerating the party's
/// shortcuts.
void Debug_SelectAbilityPair(void) {
  struct AbilityListEntry *list;
  struct UiWindow *win;
  struct UiWindow *title;
  struct UiWindow *info;
  struct RenderOutput *cursor;
  u32 first;
  u32 second;
  u32 count;
  s32 done = 0;
  s32 row = 0;
  s32 redraw;
  s32 slot;
  s32 y;
  s32 cost;
  s32 ability;

  cursor = NULL;
  redraw = 1;
  // Build the debug shortcut candidates before creating their three windows.
  list = MemAlloc(0x700);
  Unit_LearnPsynergyFar(0, 140);
  Unit_LearnPsynergyFar(1, 140);
  Unit_LearnPsynergyFar(2, 140);
  Unit_LearnPsynergyFar(2, 141);
  Unit_LearnPsynergyFar(2, 78);
  Unit_LearnPsynergyFar(3, 93);
  Unit_LearnPsynergyFar(5, 140);
  second = 0;
  first = 0;
  count = Party_ListPsynergy(list);
  if (count != 0) {
    Menu_FindShortcutEntries(&first, &second, list);
    win = UiWindow_Create(4, 6, 20, 7, 2);
    title = UiWindow_Create(4, 3, 20, 3, 2);
    info = UiWindow_Create(4, 14, 20, 5, 2);
    slot = Resource_FindFreeEntry();
    if (slot != 0) {
      VramCache_Load(slot, 128, gResource_FixedBlockBTiles);
      cursor = RenderOutput_Create(slot, 0x40000000, win, 0, 0);
    }
    /* FAKEMATCH(temporary): the title message passed through slot; as the
     * message passed directly, 2 of 362 instructions differ in tbs-en. */
    slot = MsgPanelHpMaxLabel;
    UiText_DrawCharacterAtOffset(slot, title, 16, 0);
    // Redraw both shortcut rows and the selected ability's detail pane.
    for (;;) {
      if (redraw != 0) {
        redraw = 0;
        first = (first + count) % count;
        second = (second + count) % count;
        row = (row + 2) % 2;
        y = (row << 4) + (win->y << 3) + 28;
        cursor->y = y;
        cursor->oam.y = y;
        RenderOutput_RedrawSavedRect(win);
        UiWindow_DrawDividerLine(win, 1, 2, 17, 2);
        UiText_DrawCharacterAtOffset(MsgPanelStatusLabel, win, 48, 0);
        UiText_DrawCharacterAtOffset(list[first].ability + MsgAbilityName, win,
                                     56, 16);
        UiText_DrawCharacterAtOffset(list[second].ability + MsgAbilityName, win,
                                     56, 32);
        UiText_DrawCharacterAtOffset(MsgPanelStatusLabel - 2, win, 16, 16);
        UiText_DrawCharacterAtOffset(MsgPanelStatusLabel - 1, win, 16, 32);
        UiText_DrawCharacterAtOffset(list[first].owner + MsgCharacterName, win,
                                     104, 16);
        UiText_DrawCharacterAtOffset(list[second].owner + MsgCharacterName, win,
                                     104, 32);
        RenderOutput_RedrawSavedRect(info);
        UiText_DrawCharacterAtOffset(MsgPsynergyNeedMorePp, info, 0, 16);
        if (row != 0) {
          cost = BattleActGet(list[second].ability)->ppCost;
          ability = list[second].ability;
        } else {
          cost = BattleActGet(list[first].ability)->ppCost;
          ability = list[first].ability;
        }
        UiText_DrawNumberAtOffset(cost, 2, info, 64, 16);
        UiText_DrawCharacterAtOffset(ability + MsgAbilityDescription, info, 0,
                                     0);
      }
      WaitFrames(1);
      if (gKeysRepeat & KEY_LEFT) {
        AudioCommand_PlayFar(SE_MENU_CURSOR_MOVE);
        if (row != 0)
          second--;
        else
          first--;
        redraw = 1;
      }
      if (gKeysRepeat & KEY_RIGHT) {
        AudioCommand_PlayFar(SE_MENU_CURSOR_MOVE);
        if (row != 0)
          second++;
        else
          first++;
        redraw = 1;
      }
      if (gKeysRepeat & KEY_UP) {
        AudioCommand_PlayFar(SE_MENU_CURSOR_MOVE);
        row--;
        redraw = 1;
      }
      if (gKeysRepeat & KEY_DOWN) {
        AudioCommand_PlayFar(SE_MENU_CURSOR_MOVE);
        row++;
        redraw = 1;
      }
      if (gKeysPressed & KEY_A)
        AudioCommand_PlayFar(SE_MENU_CONFIRM);
      else if (gKeysPressed & KEY_B)
        AudioCommand_PlayFar(SE_MENU_CANCEL);
      else if (gKeysPressed & KEY_START)
        AudioCommand_PlayFar(SE_MENU_CANCEL);
      else
        continue;
      /* FAKEMATCH(dead): an unread flag, which forces fresh zeros for both
       * entries and the cursor argument instead of reusing the row register;
       * without the flag, 5 of 362 instructions differ in tbs-en. */
      done = 1;
      break;
    }
    // Persist both owner/ability pairs before releasing the picker resources.
    gGameState.firstShortcut = (list[first].owner << 10) | list[first].ability;
    gGameState.secondShortcut =
        (list[second].owner << 10) | list[second].ability;
    UiWork_Finalize(win, 1);
    UiWork_Finalize(title, 1);
    UiWork_Finalize(info, 1);
    WaitFrames(1);
  }
  MemFree(list);
}

/// Lists every Psynergy of the party's active members as (owner, Psynergy)
/// pairs in output and returns how many.
/// output must hold every owner's nonzero action slots; there is no capacity
/// argument.
s32 Party_ListPsynergy(struct AbilityListEntry *output) {
  u16 ownerIds[14];
  s32 outputCount = 0;
  s32 ownerCount = Party_ListActiveOwnersFar(ownerIds);
  s32 i;

  for (i = 0; i < ownerCount; i++) {
    struct BattleUnit *unit;
    u32 owner;
    s32 slotIndex;
    u32 ability;

    owner = ownerIds[i];
    unit = UnitGetFar(owner);
    for (slotIndex = 0; slotIndex < (s32)(sizeof(unit->actionSlots) /
                                          sizeof(unit->actionSlots[0]));
         slotIndex++) {
      ability = unit->actionSlots[slotIndex].encodedAction & ACTION_ID_MASK;
      if (ability == 0)
        break;
      BattleActGet(ability);
      output[outputCount].owner = owner;
      output[outputCount].ability = ability;
      outputCount++;
    }
  }

  return outputCount;
}

/// Finds the two Psynergy shortcuts in a list from Party_ListPsynergy, and
/// returns their indexes (0 for one not found).
/// A shortcut absent from the list falls back to entry 0.
void Menu_FindShortcutEntries(u32 *firstIndex, u32 *secondIndex,
                              const struct AbilityListEntry *entries) {
  s32 entryIndex;

  *firstIndex = 0;
  *secondIndex = 0;

  for (entryIndex = 0; entryIndex <= 447; entryIndex++) {
    if (entries[entryIndex].ability == (gGameState.firstShortcut & 0x3ff) &&
        entries[entryIndex].owner == (gGameState.firstShortcut >> 10)) {
      *firstIndex = entryIndex;
      break;
    }
  }

  for (entryIndex = 0; entryIndex <= 447; entryIndex++) {
    if (entries[entryIndex].ability == (gGameState.secondShortcut & 0x3ff) &&
        entries[entryIndex].owner == (gGameState.secondShortcut >> 10)) {
      *secondIndex = entryIndex;
      break;
    }
  }
}

/// Runs the Psynergy shortcut picker.
void Menu_RunSelectionHook(void) { Debug_SelectAbilityPair(); }

/// The work block of heap slot 19: two icon flags and resources and a window,
/// among fields nothing here uses.
struct SelectionWorkspace {
  u8 unk000[0x46];
  u16 firstActive;
  u16 firstResource;
  u8 unk04a[0x352 - 0x4a];
  u16 secondActive;
  u16 secondResource;
  u8 unk356[0xff4 - 0x356];
  struct UiWindow *window;
  u8 unkff8[12];
};

/// Allocates the work block of heap slot 19 and clears its two icon flags.
/// Nothing calls it.
UNUSED void Menu_AllocateSelectionWorkspace(void) {
  struct SelectionWorkspace *work;
  u32 zero;

  work = Runtime_AllocateBlock(0x13, 0x1004);
  zero = 0;
  work->firstActive = zero;
  work->secondActive = zero;
}

/// Closes the window of heap slot 19's work block, frees its two icons and
/// frees the block.
/// Nothing calls it.
UNUSED void Menu_ReleaseSelectionWorkspace(void) {
  struct SelectionWorkspace *state;

  state = *(struct SelectionWorkspace **)gSelectionWorkspace;
  UiWork_Finalize(state->window, 0);
  while (UiWork_IsIdle(state->window) == 0) {
    WaitFrames(1);
  }
  if (state->firstActive != 0) {
    Resource_ResetEntry(state->firstResource);
  }
  if (state->secondActive != 0) {
    Resource_ResetEntry(state->secondResource);
  }
  Runtime_ReleaseHeapBlock(0x13);
}
