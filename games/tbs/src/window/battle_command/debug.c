/// @file
/// The debug menus (maps, flags, icon browsers) and the system menu's
/// choices.
// Part of battle_command.c.

#include "../battle_command_internal.h"

/// Schedules the debug map and warp destination selector; 0xc80 is a scheduler
/// order, not a number of frames to wait. Nothing in the game calls it.
UNUSED void DebugMenu_ScheduleMapSelector(void) {
#ifdef UBFIX
  Scheduler_AddOrUpdateCallback(Menu_RunSelection, 0xC80);
#else
  Scheduler_AddOrUpdateCallback((SchedulerCallback)Menu_RunSelection, 0xC80);
#endif
}

/// Draws the debug map and warp destination numbers with their category/area
/// labels. The first line has the map's name (MsgDebugCategoryName + map) and,
/// beside it, the area its warp destination leads to (MsgDebugEntryName +
/// area); the numbers, three digits each, go under them.
void Menu_DrawSelectionRow(struct UiWindow *work, s32 mapId,
                           const s16 *warpDest) {
  s16 mapIndex = mapId;
  s32 label = MapSec_GetNameIndexFar(mapIndex, *warpDest) + MsgDebugEntryName;
  RenderOutput_PrepareForRedraw(work);
  UiText_DrawNumber(mapIndex, 3, work, 0, 14);
  UiText_DrawNumber(*warpDest, 3, work, MENU_LABEL_X, 14);
  UiText_DrawString(gMenu_ColonString, work, MENU_TEXT_X, 0);
  UiText_DrawResource(mapIndex + MsgDebugCategoryName, work, 0, 0);
  UiText_DrawString(gMenu_ColonString, work, MENU_TEXT_X, 14);
  UiText_DrawResource(label, work, MENU_LABEL_X, 0);
}

// The debug map selector: a map and a warp destination number, starting from
// the map being run (Menu_HandleSelectionRowInput has the keys). A warps
// there (FieldScript_SetDestinationFar), B closes it. The field opens it in
// debug mode when the Start button's menu (Save your quest, Sleep, Change
// settings) is opened with B held (object/object/run_map.c).
// The scheduler and the debug menu's far call ignore this selection result.
// Plain void removes two result moves (252 to 248 bytes in tbs-en) and
// reorders FieldScript_SetDestinationFar's argument setup.
#ifdef UBFIX
void Menu_RunSelection(void) {
#else
s16 Menu_RunSelection(void) {
#endif
  s32 cursorSlot;
  s16 selectedField;
  s16 warpDest;
  struct MenuSprite cursor;
  s16 mapId;
  struct UiWindow *window;
  s16 result;

  window = 0;
  selectedField = 0;
  mapId = gGameState.defaultMap;
  warpDest = gGameState.defaultWarpDest;
  window = UiWindow_Create(0, 7, 30, 5, 2);
  Menu_DrawSelectionRow(window, mapId, &warpDest);
  UiTextResource_Initialize(&cursor, &cursorSlot);

  // Wait for the keys that opened it to be let go.
  while (gKeysHeld != 0)
    WaitFrames(1);

  for (;;) {
    result =
        Menu_HandleSelectionRowInput(window, mapId, &warpDest, &selectedField);
    if (result == -1) {
      UiTextResource_Release(cursorSlot);
      UiWork_Finalize(window, 2);
      FieldScript_SetDestinationFar(mapId, warpDest);
#ifdef UBFIX
      return;
#else
      return result;
#endif
    }
    if (result == -2) {
      UiTextResource_Release(cursorSlot);
      UiWork_Finalize(window, 2);
#ifdef UBFIX
      return;
#else
      return result;
#endif
    }

    UiTextResource_SetPosition(&cursor, MENU_TEXT_X,
                               selectedField * 14 + MENU_TEXT_Y);
    mapId = result;
    WaitFrames(1);
  }
}

/// Edits the debug map or warp destination, returning -1 to warp and -2 to
/// cancel. Shoulder buttons step by ten; it keeps the original 99-step warp
/// destination wrap. Otherwise it returns the map after this frame's keys. Up
/// and Down switch the field (`selectedField`: 0 map, 1 warp destination), Left
/// and Right step by one, wrapping maps 0 to 200 and warp destinations 0 to 99.
/// R and L step a map by ten, setting the warp destination to 0, and past
/// either end go to the other (195 + 10 gives 0); they step a warp destination
/// by ten, wrapping by 99 (95 + 10 gives 6).
s32 Menu_HandleSelectionRowInput(struct UiWindow *window, s16 mapId,
                                 s16 *warpDest, s16 *selectedField) {
  if (gKeysRepeat & KEY_A)
    return -1;
  if (gKeysRepeat & KEY_B)
    return -2;
  if (gKeysRepeat & KEY_DOWN) {
    *selectedField ^= 1;
  } else if (gKeysRepeat & KEY_UP) {
    *selectedField ^= 1;
  } else if (gKeysRepeat & KEY_RIGHT) {
    if (*selectedField == 0) {
      mapId++;
    } else {
      (*warpDest)++;
      if (*warpDest > 99)
        *warpDest = 0;
    }
    if (mapId > 200)
      mapId = 0;
    Menu_DrawSelectionRow(window, mapId, warpDest);
  } else if (gKeysRepeat & KEY_LEFT) {
    if (*selectedField == 0) {
      mapId--;
    } else {
      (*warpDest)--;
      if (*warpDest < 0)
        *warpDest = 99;
    }
    if (mapId < 0)
      mapId = 200;
    Menu_DrawSelectionRow(window, mapId, warpDest);
  } else if (gKeysRepeat & KEY_R) {
    if (*selectedField == 0) {
      *warpDest = 0;
      mapId += 10;
    } else {
      *warpDest += 10;
      if (*warpDest > 99)
        *warpDest -= 99;
    }
    if (mapId > 200)
      mapId = 0;
    Menu_DrawSelectionRow(window, mapId, warpDest);
  } else if (gKeysRepeat & KEY_L) {
    if (*selectedField == 0) {
      *warpDest = 0;
      mapId -= 10;
    } else {
      *warpDest -= 10;
      if (*warpDest < 0)
        *warpDest += 99;
    }
    if (mapId < 0)
      mapId = 200;
    Menu_DrawSelectionRow(window, mapId, warpDest);
  }
  return mapId;
}

/// Shows a 16x16 flag grid per page and processes input until closed.
/// Starts on page 8 and always returns 0. A page is 256 game flags, page 8
/// flags 0x800 to 0x8ff (Menu_HandleFlagGridInput has the keys). The field
/// opens it in debug mode when the main menu (Psynergy, Djinn, Item, Status)
/// is opened with B and Select held (object/object/run_map.c).
s32 Menu_RunSelectionWithCursorObject(void) {
  struct MenuSprite cursorSprite; // Cursor object
  s32 cursor[2];                  // Cursor column and row
  s32 cursorSlot;
  s32 page;
  struct UiWindow *window;
  s32 change;

  page = 8;
  window = 0;
  cursor[0] = 0;
  cursor[1] = 0;
  window = UiWindow_Create(1, 0, 28, 20, 2);
  Menu_DrawFlagBitTable(window, page);
  UiTextResource_Initialize(&cursorSprite, &cursorSlot);
  for (;;) {
    WaitFrames(1);
    change = Menu_HandleFlagGridInput(window, &page, cursor);
    // B closes the table; a change of flag or page redraws it.
    if (change == -1)
      break;
    if (change == 1)
      Menu_DrawFlagBitTable(window, page);
    UiTextResource_SetPosition(&cursorSprite, cursor[0] * 8 + 58,
                               cursor[1] * 8 + 20);
  }
  UiTextResource_Release(cursorSlot);
  UiWork_Finalize(window, 2);
  return 0;
}

/// Writes the lowest count digits of value, up to five, most significant first
/// as uppercase hexadecimal in out, without a null terminator.
void Text_FormatHex(u32 value, u32 count, u8 *out) {
  u8 digits[8];
  u32 i;

  if (count > 5)
    count = 5;

  for (i = 0; i != count; i++) {
    u32 digit = value & 15;
    if (digit <= 9)
      digits[i] = digit + '0';
    else
      digits[i] = digit + 'A' - 10;
    value >>= 4;
  }

  for (i = count - 1; (s32)i >= 0; i--)
    *out++ = digits[i];
}

/// Draws one page of 256 flags as sixteen rows of hexadecimal labels and bits.
/// Each row: its first flag's number in three hex digits, then its 16 flags
/// as 0 or 1, lowest first, under a heading of hex digits.
void Menu_DrawFlagBitTable(struct UiWindow *window, s32 page) {
  s32 row;
  s32 y;
  s32 flag;
  char label[5];
  char bits[17];

  RenderOutput_PrepareForRedraw(window);
  UiText_DrawStringInWindow((u8 *)gMenu_HexDigitsString, window, 48, 0);

  flag = page << 8;
  for (row = 0; row != 16; row++) {
    s32 i;

    y = row * 8 + 16;

    for (i = 0; i != 5; i++) {
      label[i] = 0;
    }
    Text_FormatHex(flag, 3, label);
    UiText_DrawStringInWindow((u8 *)label, window, 0, y);
    UiText_DrawStringInWindow((u8 *)gMenu_ColonString, window, 32, y);

    for (i = 0; i < 16; i++) {
      s32 set = GameFlag_TestFar(flag);
      bits[i] = (set != 0) + 48;
      flag++;
    }
    bits[i] = 0;
    UiText_DrawStringInWindow((u8 *)bits, window, 48, y);
  }
}

/// Moves within the flag grid, changes pages, or toggles the selected flag.
/// Returns 1 when flags/page need redrawing, -1 to close, and 0 otherwise.
/// A toggles flag page * 256 + row * 16 + column; B or Select closes. L and
/// R turn a page, wrapping 0 to 15; with Start held they jump ten, to the
/// other end when that passes it (5 - 10 gives 15).
s32 Menu_HandleFlagGridInput(struct UiWindow *window, s32 *page, s32 *cursor) {
  s32 *row = &cursor[1];

  if (gKeysRepeat & KEY_A) {
    s32 flag = (((*page << 4) + *row) << 4) + cursor[0];

    if (GameFlag_TestFar(flag))
      GameFlag_ClearBitFar(flag);
    else
      GameFlag_SetBitFar(flag);
    return 1;
  }
  if ((gKeysPressed & KEY_B) || (gKeysRepeat & KEY_SELECT))
    return -1;
  if (gKeysRepeat & KEY_UP) {
    if (--*row < 0)
      *row = 15;
  } else if (gKeysRepeat & KEY_DOWN) {
    if (++*row > 15)
      *row = 0;
  } else if (gKeysRepeat & KEY_LEFT) {
    if (--cursor[0] < 0)
      cursor[0] = 15;
  } else if (gKeysRepeat & KEY_RIGHT) {
    if (++cursor[0] > 15)
      cursor[0] = 0;
  } else if ((gKeysRepeat & KEY_L) && (gKeysRepeat & KEY_START)) {
    *page -= 10;
    if (*page < 0)
      *page = 15;
    return 1;
  } else if ((gKeysRepeat & KEY_R) && (gKeysRepeat & KEY_START)) {
    *page += 10;
    if (*page > 15)
      *page = 0;
    return 1;
  } else if (gKeysRepeat & KEY_L) {
    if (--*page < 0)
      *page = 15;
    return 1;
  } else if (gKeysRepeat & KEY_R) {
    if (++*page > 15)
      *page = 0;
    return 1;
  }
  return 0;
}

/// Reserved menu entry. Nothing calls it.
void Menu_ReservedNoOp(void) {}

/// Sets the Djinni's collected flag, then adds it to the specified owner.
/// The flag is FLAG_DJINNI_0 + 20 * element + index. Nothing calls it.
void Djinn_SetFoundFlagAndAdd(s32 owner, s32 element, s32 index) {
  GameFlag_SetBitFar((element * 0x14) + index + FLAG_DJINNI_0);
  Djinn_AddToOwnerFar(owner, element, index);
}

/// Handles Save, Sleep and Options from the field workspace. A cancelled save
/// or options screen reopens the workspace; cancelling the workspace returns
/// -1. Sleep shows its message and sets gSleepRequested. Otherwise returns
/// 0. The field reaches it by far call (object/object/run_map.c).
s32 Menu_RunWorkspaceResultLoop(void) {
  s32 result;

  result = Menu_RunWorkspaceSelectionLoop();
  if (result == -1) {
    return -1;
  }
  if (result == SYSTEM_MENU_SAVE) {
    if (SaveMenu_WriteSelectedSlot() == -1) {
      return Menu_RunWorkspaceResultLoop();
    }
  } else if (result == SYSTEM_MENU_SLEEP) {
    UiText_ShowPositionedMessageAndWait(MsgSleepMode, 1);
    gSleepRequested = result;
  } else if (result == SYSTEM_MENU_CHANGE_SETTINGS) {
    if (Menu_RunWorkspaceOptions() == -1) {
      return Menu_RunWorkspaceResultLoop();
    }
  }
  return 0;
}

/// Browses pages of item, Psynergy and status-pattern icons, suspending the
/// menu controller while the browser owns its window and cache entries.
/// L and R switch the kind, Left and Right turn its 8 pages of 32 icons
/// (item and Psynergy icons page * 32 on; the status patterns are 0 to 31 on
/// every page, whatever the heading says), A or B closes it. Returns 0. The
/// debug menus open it (debug/test.c, debug/level.c).
s32 DebugMenu_BrowseIcons(void) {
  s32 redraw;
  s32 tile;
  s32 page;
  s32 mode;
  struct UiWindow *window;
  s32 slot;
  s32 base;
  s32 i;
  s32 row;
  s32 x;
  s32 y;

  window = NULL;
  page = 0;
  mode = 0;
  redraw = 1;
  gMenuCtrlWork->suspended = redraw;
  WaitFrames(1);

  while (1) {
    if (gKeysRepeat & KEY_LEFT) {
      page--;
      redraw = 1;
    }
    if (gKeysRepeat & KEY_RIGHT) {
      redraw = 1;
      page++;
    }
    if (gKeysRepeat & KEY_L) {
      mode--;
      redraw = 1;
    }
    if (gKeysRepeat & KEY_R) {
      redraw = 1;
      mode++;
    }
    if (gKeysRepeat & KEY_A)
      break;
    if (gKeysRepeat & KEY_B)
      break;
    if (redraw != 0) {
      redraw = 0;
      page = (page + 8) % 8;
      mode = (mode + 3) % 3;
      UiWork_Finalize(window, 2);
      window = UiWindow_Create(10, 0, 18, 12, 2);
      if (mode == 0)
        UiText_DrawStringInWindow(gWorkspace_ItemTitle, window, 0, 0);
      else if (mode == 1)
        UiText_DrawStringInWindow(gWorkspace_EnergyTitle, window, 0, 0);
      else
        UiText_DrawStringInWindow(gWorkspace_StatusTitle, window, 0, 0);
      UiText_DrawStringInWindow(gWorkspace_PageLabel, window, 0, 8);
      UiText_DrawNumberInWindow(page, 0, window, 40, 8);
      base = page << 5;
      UiText_DrawNumberInWindow(base, 3, window, 64, 8);
      UiText_DrawStringInWindow(gWorkspace_RangeDash, window, 88, 8);
      UiText_DrawNumberInWindow(base + 31, 3, window, 96, 8);
      for (i = 0; i <= 31; i++) {
        slot = -1;
        row = i / 8;
        x = (i - row * 8) * 16;
        y = row * 16 + 16;
        if (mode == 0) {
          UiIcon_BuildItemIconTiles(base + i, 1, &slot, &tile, mode);
          RenderOutput_Create(slot, OUTPUT_SIZE_16X16, window, x, y);
        } else if (mode == 1) {
          UiIcon_BuildAbilityIconTiles(base + i, 1, &slot, &tile, 0);
          RenderOutput_Create(slot, OUTPUT_SIZE_16X16, window, x, y);
        } else {
          slot = Resource_FindFreeEntry();
          Ui_BuildPatternToSlot(i, 0, slot);
          RenderOutput_Create(slot, OUTPUT_SIZE_16X16, window, x, y);
        }
      }
    }
    WaitFrames(1);
  }
  UiWork_Finalize(window, 2);
  gMenuCtrlWork->suspended = 0;
  return 0;
}

#if LANGUAGE == LANGUAGE_JAPANESE || LANGUAGE == LANGUAGE_ENGLISH
#define DEBUG_GLYPH_NAME MsgDebugGlyphName
#elif (LANGUAGE == LANGUAGE_GERMAN) || (LANGUAGE == LANGUAGE_FRENCH)
#define DEBUG_GLYPH_NAME MsgEnd
#else
#define DEBUG_GLYPH_NAME MsgDebugSecondForm
#endif

/// Browses the character and sprite portrait maps as one circular list. Shows
/// each entry's glyph and label, then releases both windows on A or B.
/// Left and Right step one entry, L and R ten; a sprite map entry's portrait
/// is its value + 128, shown in the second portrait slot's VRAM. Returns 0.
/// The debug level menu opens it (debug/level.c).
s32 DebugMenu_BrowsePortraits(void) {
  struct UiRenderWork *work;
  struct UiWindow *portrait;
  s32 redraw;
  struct UiWindow *window;
  s32 characterCount;
  s32 entryCount;
  s32 index;
  s32 i;
  s32 glyph;
  s32 tile;
  s32 slot;
  s32 cancelKey;
  u32 slotOffset;

  work = UI_WORK;
  redraw = 1;
  portrait = UiWindow_CreateWithSideObject(0, 0, 10, 5);
  window = UiWindow_Create(10, 10, 14, 3, 2);
  index = 0;
  for (i = 0; gPortrait_CharacterMap[i * 2] != -1; i++) {
  }
  characterCount = i;
  for (i = 0; gPortrait_SpriteMap[i * 2] != -1; i++) {
  }
  entryCount = characterCount + i;

  /* FAKEMATCH(dead): a zero from the count keeps the mask in r9 and work in
   * fp; as cancelKey = KEY_B, 7 of 179 instructions differ in tbs-en. */
  cancelKey = ((u32)(u16)characterCount >> 16) | KEY_B;
  /* FAKEMATCH(temporary): the offset defined before the loop keeps its load in
   * r1; as work->outputIds[1], 3 of 179 instructions differ in tbs-en. */
  slotOffset = RENDER_SLOT_VALUE_OFFSET + 2;
  while (1) {
    if (gKeysRepeat & KEY_LEFT) {
      redraw = 1;
      index--;
    }
    if (gKeysRepeat & KEY_RIGHT) {
      redraw = 1;
      index++;
    }
    if (gKeysRepeat & KEY_L) {
      redraw = 1;
      index -= 10;
    }
    if (gKeysRepeat & KEY_R) {
      redraw = 1;
      index += 10;
    }
    if (gKeysRepeat & KEY_A)
      break;
    if (gKeysRepeat & cancelKey)
      break;
    if (redraw) {
      redraw = 0;
      index = (index + entryCount) % entryCount;
      RenderOutput_PrepareForRedraw(window);
      if (index < characterCount)
        glyph = gPortrait_CharacterMap[index * 2 + 1];
      else
        glyph = gPortrait_SpriteMap[(index - characterCount) * 2 + 1] + 128;
      {
        /* FAKEMATCH(temporary): a word temporary for the slot keeps the
         * palette in r2 through the slot store; as the slot assigned directly
         * and 15 passed, 4 of 179 instructions differ in tbs-en. */
        s32 palette;
        s32 loadedSlot = *(u16 *)((u8 *)work + slotOffset);
        palette = 15;
        slot = loadedSlot;
        UiGlyph_LoadEntryWithPalette(glyph, 0, &slot, &tile, palette, 1);
      }
      UiText_DrawNumberInWindow(index, 2, window, 0, 0);
      UiText_DrawCharacterAtOffset(index + DEBUG_GLYPH_NAME, window, 24, 0);
    }
    WaitFrames(1);
  }
  UiWork_Finalize(window, 2);
  UiWork_Finalize(portrait, 2);
  WaitFrames(1);
  return 0;
}
