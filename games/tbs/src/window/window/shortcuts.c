/// @file
/// The area name notice, the Psynergy shortcut picker and heap block 19.
// Part of window.c.

#include "../window_internal.h"

/// The area name notice's fields, which live in the field work block
/// (gFieldWork, a map's work), for as long as the map runs.
struct TimedNoticeWork {
  u8 fieldWork[0x230];   ///< the field work's other fields
  struct Window *window; ///< the notice's window; it may since have closed
  u16 countdown;         ///< frames left to show it, from 90
};

/// Shows the area's name in a window near the top of the screen for 90 frames.
/// The field work owns the window and countdown until a task closes it.
/// The window is centred across the screen and in its top 10 rows, and opens
/// at once; MapNamePopup_Update counts it down each frame in the main loop's
/// pass.
void MapNamePopup_Show(void) {
  struct TimedNoticeWork *notice;
  s32 height;
  s32 width;
  s32 y;
  s32 x;
  s32 areaMessage;
  struct Window *nameWindow;

  notice = (struct TimedNoticeWork *)gFieldWork;
  x = 8;
  y = 8;
  areaMessage =
      MapSec_GetNameIndexFar(gSaveBlock.sceneId, gSaveBlock.warpDest) +
      RENDER_RESOURCE_BASE;
  Text_GetMessageSize(areaMessage, &x, &y, &width, &height);
  x = (30 - width) >> 1;
  y = (10 - height) >> 1;
  nameWindow = Window_Create(x, y, width, height, WINDOW_FLAG_INSTANT);
  notice->window = nameWindow;
  Text_DrawMessage(areaMessage, nameWindow, 0, 0);
  notice->countdown = MAP_NAME_POPUP_FRAMES;
  Task_Add(MapNamePopup_Update, TASK_PRIORITY_MAIN_LOOP);
}

/// Counts the area name down, and closes it at 0.
void MapNamePopup_Update(void) {
  struct TimedNoticeWork *notice;
  u16 framesLeft;

  notice = (struct TimedNoticeWork *)gFieldWork;
  // Adding 0xffff takes one off in 16 bits.
  notice->countdown = (framesLeft = notice->countdown + 0xffff);
  if ((framesLeft << 16) == 0) {
    Window_Close(notice->window, 2);
    Task_Remove(MapNamePopup_Update);
  }
}

/// Closes the area name at once, if it is shown.
void MapNamePopup_Hide(void) {
  struct Window *window;

  window = ((struct TimedNoticeWork *)gFieldWork)->window;
  if (window != NULL && window->flags != 0) {
    Window_Close(window, 2);
    Task_Remove(MapNamePopup_Update);
  }
}

/// Removes itemId from the party's inventory and returns 0. Reached only
/// through a bare entry of the window far-call table
/// (system/far_call/WINDOW.S).
s32 Item_RemoveFromInventory(s32 unused, s32 itemId) {
  PartyInventory_RemoveFar(itemId);
  return 0;
}

/// Returns 1. Reached only through a bare entry of the window far-call table.
s32 Item_ReturnTrue(void) { return 1; }

/// Steps the window hue one step: back when keys contains LEFT, on
/// otherwise. The hue wraps in a byte (0 back is 255), not at the option
/// screen's 24 hues.
UNUSED void OptionMenu_StepWindowHue(s32 keys) {
  u8 hue = gSaveBlock.windowColor[0];
  if (keys & KEY_LEFT)
    hue += 0xff;
  else
    hue += 1;
  gSaveBlock.windowColor[0] = hue;
}

/// A Psynergy the debug shortcut picker lists: its member and the Psynergy.
/// The picker's list is a Alloc block of 448 of them.
struct AbilityListEntry {
  u16 member;  ///< the party member's unit id
  u16 ability; ///< the action id (10 bits in a shortcut)
};

/// Lets the player pick two Psynergy shortcuts from all the party's Psynergy,
/// showing each one's cost and description, and stores them in
/// gSaveBlock.firstShortcut and secondShortcut (member << 10 | Psynergy).
/// This debug path seeds extra Psynergy before enumerating the party's
/// shortcuts.
void Debug_SelectAbilityPair(void) {
  struct AbilityListEntry *list;
  struct Window *win;
  struct Window *title;
  struct Window *info;
  struct WindowSprite *cursor;
  u32 first;
  u32 second;
  u32 count;
  UNUSED s32 done = 0;
  s32 row = 0;
  s32 redraw;
  s32 slot;
  s32 y;
  s32 cost;
  s32 ability;

  cursor = NULL;
  redraw = 1;
  // Build the debug shortcut candidates before creating their three windows.
  // 0x700 bytes hold 448 entries, as many as Menu_FindShortcutEntries
  // searches.
  list = Alloc(0x700);
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
    win = Window_Create(4, 6, 20, 7, WINDOW_FLAG_INSTANT);
    title = Window_Create(4, 3, 20, 3, WINDOW_FLAG_INSTANT);
    info = Window_Create(4, 14, 20, 5, WINDOW_FLAG_INSTANT);
    // VramCache_FindFreeEntry reports no free entry as VRAM_CACHE_ENTRY_COUNT,
    // not 0: this test lets that through, and with entry 0 it leaves cursor
    // NULL for the writes below.
    slot = VramCache_FindFreeEntry();
    if (slot != 0) {
      VramCache_Load(slot, 128, gPointingHandTiles);
      cursor = WindowSprite_Create(slot, OUTPUT_SIZE_16X16, win, 0, 0);
    }
    /* FAKEMATCH(temporary): the title message passed through slot; as the
     * message passed directly, 2 of 362 instructions differ in tbs-en. */
    slot = MsgPanelHpMaxLabel;
    Text_DrawMessageAsTiles(slot, title, 16, 0);
    // Redraw both shortcut rows and the selected ability's detail side.
    for (;;) {
      if (redraw != 0) {
        redraw = 0;
        // Wrap both picks and the row; a step below 0 wraps through the
        // unsigned sum. The cursor points at the row's line, 16 pixels a row
        // and 28 below the window's top.
        first = (first + count) % count;
        second = (second + count) % count;
        row = (row + 2) % 2;
        y = (row << 4) + (win->y << 3) + 28;
        cursor->y = y;
        cursor->oam.y = y;
        WindowSprite_RedrawSavedRect(win);
        Window_DrawDividerLine(win, 1, 2, 17, 2);
        Text_DrawMessageAsTiles(MsgPanelStatusLabel, win, 48, 0);
        Text_DrawMessageAsTiles(list[first].ability + MsgAbilityName, win, 56,
                                16);
        Text_DrawMessageAsTiles(list[second].ability + MsgAbilityName, win, 56,
                                32);
        Text_DrawMessageAsTiles(MsgPanelStatusLabel - 2, win, 16, 16);
        Text_DrawMessageAsTiles(MsgPanelStatusLabel - 1, win, 16, 32);
        Text_DrawMessageAsTiles(list[first].member + MsgCharacterName, win, 104,
                                16);
        Text_DrawMessageAsTiles(list[second].member + MsgCharacterName, win,
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
      WaitFrames(1);
      if (gKeysRepeat & KEY_LEFT) {
        Sound_PlaySongFar(SE_MENU_CURSOR_MOVE);
        if (row != 0)
          second--;
        else
          first--;
        redraw = 1;
      }
      if (gKeysRepeat & KEY_RIGHT) {
        Sound_PlaySongFar(SE_MENU_CURSOR_MOVE);
        if (row != 0)
          second++;
        else
          first++;
        redraw = 1;
      }
      if (gKeysRepeat & KEY_UP) {
        Sound_PlaySongFar(SE_MENU_CURSOR_MOVE);
        row--;
        redraw = 1;
      }
      if (gKeysRepeat & KEY_DOWN) {
        Sound_PlaySongFar(SE_MENU_CURSOR_MOVE);
        row++;
        redraw = 1;
      }
      if (gKeysPressed & KEY_A)
        Sound_PlaySongFar(SE_MENU_CONFIRM);
      else if (gKeysPressed & KEY_B)
        Sound_PlaySongFar(SE_MENU_CANCEL);
      else if (gKeysPressed & KEY_START)
        Sound_PlaySongFar(SE_MENU_CANCEL);
      else
        continue;
      /* FAKEMATCH(dead): an unread flag, which forces fresh zeros for both
       * entries and the cursor argument instead of reusing the row register;
       * without the flag, 5 of 362 instructions differ in tbs-en. */
      done = 1;
      break;
    }
    // Persist both member/ability pairs before releasing the picker resources.
    // A, B and START all keep the picks. The cursor's window sprite and VRAM
    // entry go with its window.
    gSaveBlock.firstShortcut = (list[first].member << 10) | list[first].ability;
    gSaveBlock.secondShortcut =
        (list[second].member << 10) | list[second].ability;
    Window_Close(win, 1);
    Window_Close(title, 1);
    Window_Close(info, 1);
    WaitFrames(1);
  }
  Free(list);
}

/// Lists every Psynergy of the party's active members as (member, Psynergy)
/// pairs in output and returns how many.
/// output must hold every member's nonzero action slots; there is no capacity
/// argument. Each member's list stops at its first empty slot.
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
/// A shortcut absent from the list falls back to entry 0.
/// A shortcut is member << 10 | action. The search runs over all 448 places
/// of the list's block, past its last entry too.
void Menu_FindShortcutEntries(u32 *firstIndex, u32 *secondIndex,
                              const struct AbilityListEntry *entries) {
  s32 entryIndex;

  *firstIndex = 0;
  *secondIndex = 0;

  for (entryIndex = 0; entryIndex <= 447; entryIndex++) {
    if (entries[entryIndex].ability == (gSaveBlock.firstShortcut & 0x3ff) &&
        entries[entryIndex].member == (gSaveBlock.firstShortcut >> 10)) {
      *firstIndex = entryIndex;
      break;
    }
  }

  for (entryIndex = 0; entryIndex <= 447; entryIndex++) {
    if (entries[entryIndex].ability == (gSaveBlock.secondShortcut & 0x3ff) &&
        entries[entryIndex].member == (gSaveBlock.secondShortcut >> 10)) {
      *secondIndex = entryIndex;
      break;
    }
  }
}

/// Runs the Psynergy shortcut picker. Reached only through a bare entry of the
/// window far-call table.
void Menu_RunSelectionHook(void) { Debug_SelectAbilityPair(); }

/// The work block of HEAP_SLOT_SELECTION_WORKSPACE (gSelectionWorkspace): two
/// VRAM cache entries with their flags and a window, among fields nothing here
/// uses. Only the two unused functions below touch it.
struct SelectionWorkspace {
  u8 unusedBeforeFirst[0x46]; ///< no code names it
  /// Nonzero when firstResource is a VRAM cache entry to free on release.
  u16 firstActive;
  u16 firstResource;                       ///< a VRAM cache entry
  u8 unusedBetweenResources[0x352 - 0x4a]; ///< no code names it
  /// Nonzero when secondResource is a VRAM cache entry to free on release.
  u16 secondActive;
  u16 secondResource;                   ///< a VRAM cache entry
  u8 unusedBeforeWindow[0xff4 - 0x356]; ///< no code names it
  struct Window *window;                ///< closed, and waited for, on release
  u8 unusedAfterWindow[12];             ///< no code names it
};

/// Allocates the work block of HEAP_SLOT_SELECTION_WORKSPACE and clears its two
/// flags. Nothing calls it.
UNUSED void Menu_AllocateSelectionWorkspace(void) {
  struct SelectionWorkspace *work;
  u32 zero;

  work = AllocHeapSlotEwram(HEAP_SLOT_SELECTION_WORKSPACE, 0x1004);
  zero = 0;
  work->firstActive = zero;
  work->secondActive = zero;
}

/// Closes the window of HEAP_SLOT_SELECTION_WORKSPACE's work block, frees the
/// VRAM cache entries its flags mark and frees the block. Nothing calls it.
UNUSED void Menu_ReleaseSelectionWorkspace(void) {
  struct SelectionWorkspace *state;

  state = *(struct SelectionWorkspace **)gSelectionWorkspace;
  Window_Close(state->window, 0);
  while (Window_IsClosed(state->window) == 0) {
    WaitFrames(1);
  }
  if (state->firstActive != 0) {
    VramCache_FreeEntry(state->firstResource);
  }
  if (state->secondActive != 0) {
    VramCache_FreeEntry(state->secondResource);
  }
  FreeHeapSlot(HEAP_SLOT_SELECTION_WORKSPACE);
}
