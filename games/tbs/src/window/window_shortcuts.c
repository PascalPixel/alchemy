/// @file
/// The area name shown on entering an area, the debug Psynergy shortcut picker
/// and the unused selection state.
// Part of window.c.

#include "window_internal.h"

/// The entries of the debug shortcut picker's list, which
/// Menu_FindShortcutEntries searches whole.
#define SHORTCUT_LIST_ENTRIES 448
/// A shortcut (gMainRoutineState.firstShortcut and secondShortcut) is
/// member << SHORTCUT_MEMBER_SHIFT | action, as Debug_SelectShortcuts stores
/// it.
#define SHORTCUT_MEMBER_SHIFT 10
#define SHORTCUT_ABILITY_MASK 0x3ff

/// Shows the area's name in a window centred in the screen's top ten rows, for
/// AREA_NAME_WINDOW_FRAMES; a per-frame task closes it.
void AreaNameWindow_Show(void) {
  struct FieldState *notice;
  s32 height;
  s32 width;
  s32 y;
  s32 x;
  s32 areaMessage;
  struct Window *nameWindow;

  notice = gFieldState;
  x = 8;
  y = 8;
  areaMessage =
      Area_GetNameIndexFar(gMainRoutineState.next, gMainRoutineState.entrance) +
      MsgDebugAreaName;
  Text_GetMessageSize(areaMessage, &x, &y, &width, &height);
  x = (DISPLAY_TILE_WIDTH - width) >> 1;
  y = (10 - height) >> 1;
  nameWindow = Window_Create(x, y, width, height, WINDOW_FLAG_INSTANT);
  notice->areaNameWindow = nameWindow;
  Text_DrawMessage(areaMessage, nameWindow, 0, 0);
  notice->areaNameCountdown = AREA_NAME_WINDOW_FRAMES;
  Task_Add(AreaNameWindow_Update, TASK_PASS_FRAME);
}

/// Counts the area name down, and closes it at 0.
void AreaNameWindow_Update(void) {
  struct FieldState *notice;
  u16 framesLeft;

  notice = gFieldState;
  // Adding (u16)-1 takes one off in 16 bits.
  notice->areaNameCountdown =
      (framesLeft = notice->areaNameCountdown + (u16)-1);
  if (framesLeft == 0) {
    Window_Close(notice->areaNameWindow, WINDOW_FLAG_INSTANT);
    Task_Remove(AreaNameWindow_Update);
  }
}

/// Closes the area name at once, if it is shown.
void AreaNameWindow_Hide(void) {
  struct Window *window;

  window = gFieldState->areaNameWindow;
  if (window != NULL && window->flags != 0) {
    Window_Close(window, WINDOW_FLAG_INSTANT);
    Task_Remove(AreaNameWindow_Update);
  }
}

/// Removes itemId from the party's inventory and returns 0.
s32 Item_RemoveFromInventory(s32 unused, s32 itemId) {
  PartyInventory_RemoveFar(itemId);
  return 0;
}

/// Returns 1; the overlays reach it through the far-call table.
s32 Item_ReturnTrue(void) { return 1; }

/// Steps the window hue one step: back when keys contains LEFT, on otherwise.
UNUSED void SettingsMenu_StepWindowHue(s32 keys) {
  u8 hue = gMainRoutineState.windowHue;
  if (keys & KEY_LEFT)
    hue += (u8)-1;
  else
    hue += 1;
  gMainRoutineState.windowHue = hue;
}

/// A Psynergy the debug shortcut picker lists: its party member and the
/// Psynergy.
struct AbilityListEntry {
  u16 member;  ///< the party member's unit id
  u16 ability; ///< the action id (SHORTCUT_ABILITY_MASK in a shortcut)
};

/// Lets the player pick two Psynergy shortcuts from all the party's Psynergy,
/// showing each one's cost and description, and stores them in
/// gMainRoutineState.firstShortcut and secondShortcut (member <<
/// SHORTCUT_MEMBER_SHIFT | Psynergy).
void Debug_SelectShortcuts(void) {
  struct AbilityListEntry *list;
  struct Window *picks;
  struct Window *title;
  struct Window *info;
  struct WindowSprite *cursor;
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
  list = Heap_Alloc(SHORTCUT_LIST_ENTRIES * sizeof(struct AbilityListEntry));
  Unit_LearnPsynergyFar(ISAAC, ABILITY_MOVE);
  Unit_LearnPsynergyFar(GARET, ABILITY_MOVE);
  Unit_LearnPsynergyFar(IVAN, ABILITY_MOVE);
  Unit_LearnPsynergyFar(IVAN, ABILITY_MIND_READ);
  Unit_LearnPsynergyFar(IVAN, ABILITY_WHIRLWIND);
  Unit_LearnPsynergyFar(MIA, ABILITY_PLY);
  Unit_LearnPsynergyFar(JENNA, ABILITY_MOVE);
  second = 0;
  first = 0;
  count = Party_ListPsynergy(list);
  if (count != 0) {
    Menu_FindShortcutEntries(&first, &second, list);
    picks = Window_Create(4, 6, 20, 7, WINDOW_FLAG_INSTANT);
    title = Window_Create(4, 3, 20, 3, WINDOW_FLAG_INSTANT);
    info = Window_Create(4, 14, 20, 5, WINDOW_FLAG_INSTANT);
    // VramCache_FindFreeEntry reports no free entry as VRAM_CACHE_ENTRY_COUNT,
    // not 0: this test lets that through, and with entry 0 it leaves cursor
    // NULL for the writes below.
    slot = VramCache_FindFreeEntry();
    if (slot != 0) {
      VramCache_Load(slot, 128, gPointingHandTiles);
      cursor = WindowSprite_Create(slot, WINDOW_SPRITE_SIZE_16X16, picks, 0, 0);
    }
    // The picker borrows the status panel's labels: "HP/Max" over it,
    // "Status" over the picks, "Attack" and "Defense" (the two messages before
    // "Status") before them. slot, done with the cursor's VRAM entry, carries
    // the first.
    slot = MsgPanelHpMaxLabel;
    Text_DrawMessageAsTiles(slot, title, 16, 0);
    // Redraw both shortcut rows and the selected ability's detail side.
    while (!done) {
      if (redraw != 0) {
        redraw = 0;
        // Wrap both picks and the row; a step below 0 wraps through the
        // unsigned sum.
        first = (first + count) % count;
        second = (second + count) % count;
        row = (row + 2) % 2;
        y = (row << 4) + (picks->y << 3) + 28;
        cursor->y = y;
        cursor->oam.y = y;
        WindowSprite_RedrawSavedRect(picks);
        Window_DrawDividerLine(picks, 1, 2, 17, 2);
        Text_DrawMessageAsTiles(MsgPanelStatusLabel, picks, 48, 0);
        Text_DrawMessageAsTiles(list[first].ability + MsgAbilityName, picks, 56,
                                16);
        Text_DrawMessageAsTiles(list[second].ability + MsgAbilityName, picks,
                                56, 32);
        Text_DrawMessageAsTiles(MsgPanelStatusLabel - 2, picks, 16, 16);
        Text_DrawMessageAsTiles(MsgPanelStatusLabel - 1, picks, 16, 32);
        Text_DrawMessageAsTiles(list[first].member + MsgCharacterName, picks,
                                104, 16);
        Text_DrawMessageAsTiles(list[second].member + MsgCharacterName, picks,
                                104, 32);
        WindowSprite_RedrawSavedRect(info);
        Text_DrawMessageAsTiles(MsgPsynergyNeedMorePp, info, 0, 16);
        if (row != 0) {
          cost = Ability_Get(list[second].ability)->ppCost;
          ability = list[second].ability;
        } else {
          cost = Ability_Get(list[first].ability)->ppCost;
          ability = list[first].ability;
        }
        Text_DrawNumberAtOffset(cost, 2, info, 64, 16);
        Text_DrawMessageAsTiles(ability + MsgAbilityDescription, info, 0, 0);
      }
      wait(1);
      if (gKeysRepeat & KEY_LEFT) {
        Sound_PlayFar(SE_MENU_CURSOR_MOVE);
        if (row != 0)
          second--;
        else
          first--;
        redraw = 1;
      }
      if (gKeysRepeat & KEY_RIGHT) {
        Sound_PlayFar(SE_MENU_CURSOR_MOVE);
        if (row != 0)
          second++;
        else
          first++;
        redraw = 1;
      }
      if (gKeysRepeat & KEY_UP) {
        Sound_PlayFar(SE_MENU_CURSOR_MOVE);
        row--;
        redraw = 1;
      }
      if (gKeysRepeat & KEY_DOWN) {
        Sound_PlayFar(SE_MENU_CURSOR_MOVE);
        row++;
        redraw = 1;
      }
      if (gKeysPressed & KEY_A)
        Sound_PlayFar(SE_MENU_CONFIRM);
      else if (gKeysPressed & KEY_B)
        Sound_PlayFar(SE_MENU_CANCEL);
      else if (gKeysPressed & KEY_START)
        Sound_PlayFar(SE_MENU_CANCEL);
      else
        continue;
      done = 1;
      break;
    }
    // Persist both member/ability pairs before releasing the picker resources.
    // A, B and START all keep the picks.
    gMainRoutineState.firstShortcut =
        (list[first].member << SHORTCUT_MEMBER_SHIFT) | list[first].ability;
    gMainRoutineState.secondShortcut =
        (list[second].member << SHORTCUT_MEMBER_SHIFT) | list[second].ability;
    Window_Close(picks, 1);
    Window_Close(title, 1);
    Window_Close(info, 1);
    wait(1);
  }
  Heap_Release(list);
}

/// Lists every Psynergy of the active party members as (party member, Psynergy)
/// pairs in output and returns how many. output must hold every party member's
/// nonzero action slots; there is no capacity argument.
s32 Party_ListPsynergy(struct AbilityListEntry *output) {
  u16 memberIds[14];
  s32 outputCount = 0;
  s32 memberCount = Party_ListMembersFar(memberIds);
  s32 i;

  for (i = 0; i < memberCount; i++) {
    struct Unit *unit;
    u32 member;
    s32 slotIndex;
    u32 ability;

    member = memberIds[i];
    unit = Unit_GetFar(member);
    for (slotIndex = 0;
         slotIndex < (s32)(sizeof(unit->psynergy) / sizeof(unit->psynergy[0]));
         slotIndex++) {
      ability = unit->psynergy[slotIndex].encodedAction & ACTION_ID_MASK;
      if (ability == 0)
        break;
      Ability_Get(ability);
      output[outputCount].member = member;
      output[outputCount].ability = ability;
      outputCount++;
    }
  }

  return outputCount;
}

/// Finds the two Psynergy shortcuts in a list from Party_ListPsynergy, and
/// returns their indexes (0 for one not found).
void Menu_FindShortcutEntries(u32 *firstIndex, u32 *secondIndex,
                              const struct AbilityListEntry *entries) {
  s32 entryIndex;

  *firstIndex = 0;
  *secondIndex = 0;

  for (entryIndex = 0; entryIndex < SHORTCUT_LIST_ENTRIES; entryIndex++) {
    if (entries[entryIndex].ability ==
            (gMainRoutineState.firstShortcut & SHORTCUT_ABILITY_MASK) &&
        entries[entryIndex].member ==
            (gMainRoutineState.firstShortcut >> SHORTCUT_MEMBER_SHIFT)) {
      *firstIndex = entryIndex;
      break;
    }
  }

  for (entryIndex = 0; entryIndex < SHORTCUT_LIST_ENTRIES; entryIndex++) {
    if (entries[entryIndex].ability ==
            (gMainRoutineState.secondShortcut & SHORTCUT_ABILITY_MASK) &&
        entries[entryIndex].member ==
            (gMainRoutineState.secondShortcut >> SHORTCUT_MEMBER_SHIFT)) {
      *secondIndex = entryIndex;
      break;
    }
  }
}

/// Runs the debug Psynergy shortcut picker (Debug_SelectShortcuts).
void Menu_RunSelectionHook(void) { Debug_SelectShortcuts(); }

/// The state block of HEAP_SLOT_SELECTION_STATE (gSelectionState): two
/// VRAM cache entries with their flags and a window, among fields nothing here
/// uses. Only the two unused functions below touch it.
struct SelectionState {
  u8 unusedBeforeFirst[70];
  /// Nonzero when firstResource is a VRAM cache entry to free on release.
  u16 firstActive;
  u16 firstResource; ///< a VRAM cache entry
  u8 unusedBetweenResources[850 - 74];
  /// Nonzero when secondResource is a VRAM cache entry to free on release.
  u16 secondActive;
  u16 secondResource; ///< a VRAM cache entry
  u8 unusedBeforeWindow[4084 - 854];
  struct Window *window; ///< closed, and waited for, on release
  u8 unusedAfterWindow[12];
};

STATIC_ASSERT(sizeof(struct SelectionState) == 4100);

/// Unused: allocates the state block of HEAP_SLOT_SELECTION_STATE and clears
/// its two flags.
UNUSED void Menu_AllocateSelectionState(void) {
  struct SelectionState *state;

  state = HeapSlot_AllocEwram(HEAP_SLOT_SELECTION_STATE,
                              sizeof(struct SelectionState));
  state->firstActive = 0;
  state->secondActive = 0;
}

/// Unused: closes the window of HEAP_SLOT_SELECTION_STATE's state block,
/// frees the VRAM cache entries its flags mark and frees the block.
UNUSED void Menu_ReleaseSelectionState(void) {
  struct SelectionState *state;

  state = *(struct SelectionState **)gSelectionState;
  Window_Close(state->window, 0);
  while (Window_IsClosed(state->window) == 0) {
    wait(1);
  }
  if (state->firstActive != 0) {
    VramCache_FreeEntry(state->firstResource);
  }
  if (state->secondActive != 0) {
    VramCache_FreeEntry(state->secondResource);
  }
  HeapSlot_Release(HEAP_SLOT_SELECTION_STATE);
}

#undef SHORTCUT_LIST_ENTRIES
#undef SHORTCUT_MEMBER_SHIFT
#undef SHORTCUT_ABILITY_MASK
